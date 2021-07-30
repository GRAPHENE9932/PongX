/*
 * PongX basic abstract server
 * Copyright (C) 2021  Artem Kliminskyi <artemklim50@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LocalMultiplayerServer.hpp"
#include "../game_math.hpp"
#include "Server.hpp"

///PI constant
constexpr float PI = 3.14159265359F;
///If multiply this constant by an angle in degrees, result is in radians
constexpr float DEG2RAD = PI / 180.0F;

Server::Server(sf::Vector2u window_size) {
	//Initialize some parameters
	this->window_size = window_size;
	ball_pos = { window_size.x * 0.5F, window_size.y * 0.5F }; //Place the ball to the center of the window
	ball_direction = gm::random_number_double_range(-80.0F * DEG2RAD, 80.0F * DEG2RAD, //Random direction
													100.0F * DEG2RAD, 260.0F * DEG2RAD);
}

Server* Server::create(ServerSettings settings) {
	switch (settings.server_type) {
		case LocalMultiplayer: {
			LocalMultiplayerServer* cur_server =
				new LocalMultiplayerServer(settings.window_size); //Construct a new server

			///Set settings
			cur_server->server_type = LocalMultiplayer;
			cur_server->ball_radius = settings.ball_radius;
			cur_server->ball_speed = settings.ball_speed;
			cur_server->player_rect = settings.player_rect;
			cur_server->enemy_rect = settings.enemy_rect;
			cur_server->enemy_up_key = settings.enemy_up_key;
			cur_server->enemy_down_key = settings.enemy_down_key;

			return cur_server;
		}
		default: {
			return nullptr; //TODO other types
		}
	}
}

sf::Vector2f Server::get_ball_pos() {
	return ball_pos;
}

float Server::get_ball_dir(){
	return ball_direction;
}

sf::FloatRect Server::get_player_rect() {
	return player_rect;
}

sf::FloatRect Server::get_enemy_rect() {
	return enemy_rect;
}

unsigned int Server::get_player_score() {
    return player_score;
}

unsigned int Server::get_enemy_score() {
    return enemy_score;
}

//BEGIN player and enemy movement
void Server::move_player_up() {
	gm::move_rect(&player_rect, { 0, -10 });

	if (player_rect.top < 0.0F) //If the player already behind the limit, correct it's position
		player_rect.top = 0.0F;

	waiting_for_input = false; //Input received just now
}

void Server::move_player_down() {
	gm::move_rect(&player_rect, { 0, 10 });

	//If the player already behind the limit, correct it's position
	if (player_rect.top > window_size.y - player_rect.height)
		player_rect.top = window_size.y - player_rect.height;

	waiting_for_input = false; //Input received just now
}

void Server::move_enemy_up() {
	gm::move_rect(&enemy_rect, { 0, -10 });

	if (enemy_rect.top < 0.0F) //If the enemy already behind the limit, correct it's position
		enemy_rect.top = 0.0F;

	waiting_for_input = false; //Input received just now
}

void Server::move_enemy_down() {
	gm::move_rect(&enemy_rect, { 0, 10 });

	//If the enemy already behind the limit, correct it's position
	if (enemy_rect.top > window_size.y - enemy_rect.height)
		enemy_rect.top = window_size.y - enemy_rect.height;

	waiting_for_input = false; //Input received just now
}
//END player and enemy movement
void Server::update_ball_movement() {
    //Do not move if the ball suspended
	if (waiting_for_input)
		return;

	///Direction before the modifications on this frame
	auto old_direction = ball_direction;

	//Predict the next ball position (ignoring everything, but speed and direction)
	next_ball_pos = ball_pos +
		sf::Vector2f(std::sin(old_direction) * ball_speed, std::cos(old_direction) * ball_speed);

	//BEGIN check collision of next ball with bounds of window
	bool top_win_bound = next_ball_pos.y - ball_radius <= 0; //Top global bound
	bool bottom_win_bound = next_ball_pos.y + ball_radius >= window_size.y; //Bottom global bound

	//Clamp the position of the ball
	std::clamp(next_ball_pos.y, ball_radius, window_size.y - ball_radius);

	//Change the direction of the ball considering the collisions with horizontal window bounds
	if (top_win_bound || bottom_win_bound) {
		// +================o==================+
		// |               /|\                 |
		// |    Before -> / | \ <- After       |
		// |             /  |  \               |
		ball_direction = 180.0F * DEG2RAD - ball_direction;
	}
	//END check collision with bounds of window

	//  |          |                         |          |
	//  |     2    |   4                  9  |     7    |
	//  |          |                         |          |
	//  +==========+------             ------+==========+
	//            ||                         ||
	//            ||                         ||
	//            ||                         ||
	//            ||                         ||
	//            ||                         ||
	//   Player   ||   1      if no       6  ||  Enemy
	//            ||        collision        ||
	//            ||            0            ||
	//            ||                         ||
	//            ||                         ||
	//            ||                         ||
	//  +==========+------             ------+==========+
	//  |          |                         |          |
	//  |     3    |   5                 10  |     8    |
	//  |          |                         |          |
	//BEGIN check collision with the player and the enemy
	//Check horizontal surfaces, vertical surfaces and corners separately
	unsigned char collision = 0; //See scheme above

	//CORNERS
	//4
	if (gm::distance({ player_rect.left + player_rect.width, player_rect.top },
		next_ball_pos) <= ball_radius &&
		next_ball_pos.x >= player_rect.left + player_rect.width &&
		next_ball_pos.y <= player_rect.top) {
		collision = 4;
	}
	//5
	else if (gm::distance({ player_rect.left + player_rect.width, player_rect.top + player_rect.height },
		next_ball_pos) <= ball_radius &&
		next_ball_pos.x >= player_rect.left + player_rect.width &&
		next_ball_pos.y >= player_rect.top + player_rect.height) {
		collision = 5;
	}
	//9
	else if (gm::distance({ enemy_rect.left, enemy_rect.top }, next_ball_pos) <= ball_radius &&
		next_ball_pos.x <= enemy_rect.left &&
		next_ball_pos.y <= enemy_rect.top) {
		collision = 9;
	}
	//10
	else if (gm::distance({ enemy_rect.left, enemy_rect.top + enemy_rect.height },
		next_ball_pos) <= ball_radius &&
		next_ball_pos.x <= enemy_rect.left &&
		next_ball_pos.y >= enemy_rect.top + enemy_rect.height) {
		collision = 10;
	}
	//VERTICAL SURFACES
	//1
	if (next_ball_pos.x - ball_radius <= player_rect.left + player_rect.width &&
		next_ball_pos.y >= player_rect.top &&
		next_ball_pos.y <= player_rect.top + player_rect.height) {
		collision = 1;
	}
	//6
	else if (next_ball_pos.x + ball_radius >= enemy_rect.left &&
		next_ball_pos.y >= enemy_rect.top &&
		next_ball_pos.y <= enemy_rect.top + enemy_rect.height) {
		collision = 6;
	}
	//HORIZONTAL SURFACES
	//2
	else if (next_ball_pos.y + ball_radius >= player_rect.top &&
		next_ball_pos.y <= player_rect.top + player_rect.height * 0.5F &&
		next_ball_pos.x >= player_rect.left &&
		next_ball_pos.x <= player_rect.left + player_rect.width) {
		collision = 2;
	}
	//3
	else if (next_ball_pos.y - ball_radius <= player_rect.top + player_rect.height &&
		next_ball_pos.y >= player_rect.top + player_rect.height * 0.5F &&
		next_ball_pos.x >= player_rect.left &&
		next_ball_pos.x <= player_rect.left + player_rect.width) {
		collision = 3;
	}
	//7
	else if (next_ball_pos.y + ball_radius >= enemy_rect.top &&
		next_ball_pos.y <= enemy_rect.top + enemy_rect.height * 0.5F &&
		next_ball_pos.x >= enemy_rect.left &&
		next_ball_pos.x <= enemy_rect.left + enemy_rect.width) {
		collision = 7;
	}
	//8
	else if (next_ball_pos.y - ball_radius <= enemy_rect.top + enemy_rect.height &&
		next_ball_pos.y >= enemy_rect.top + enemy_rect.height * 0.5F &&
		next_ball_pos.x >= enemy_rect.left &&
		next_ball_pos.x <= enemy_rect.left + enemy_rect.width) {
		collision = 8;
	}

	if (!collided_before) {
		switch (collision) {
			case 1: //Vertical surfaces
			case 6: {
				//                           \  ||
				//                  Before -> \ ||
				//                             \||
				//                              0| <- Ball
				//                             /||
				//                   After -> / ||
				//                           /  ||
				ball_direction = 360.0F * DEG2RAD - ball_direction;
				break;
			}
			case 2: //Horizontal surfaces
			case 3:
			case 7:
			case 8: {
				// +================o==================+
				// |               /|\                 |
				// |    Before -> / | \ <- After       |
				// |             /  |  \               |
				ball_direction = 180.0F * DEG2RAD - ball_direction;
				break;
			}
			case 4: //Corners
			case 5:
			case 9:
			case 10: {
				//                             ||
				//                             ||
				//                             o=====
				//                            /
				//                           /
				//                Before -> / <- After
				//                         /
				//                        /
				ball_direction += 180.0F * DEG2RAD;
				break;
			}
		}
	}

	//Set value to variable "collided_before"
	collided_before = collision != 0;
	//END check collision with the player and the enemy

	//BEGIN prevent collision again (get out the ball)
	//We dont need to prevent collision if there was no collision
	if (collision != 0) {
		//           +=========+
		//        ---|         |---
		//       - 5 |         | 6 -
		//     ||----+    1    +----||
		//     ||     \       /     ||
		//     ||      \     /      ||
		//     ||       \   /       ||
		//     ||        \ /        ||
		//     ||    4    *    2    ||
		//     ||        / \        ||
		//     ||       /   \       ||
		//     ||      /     \      ||
		//     ||     /       \     ||
		//     ||----+    3    +----||
		//       - 8 |         | 7 -
		//        ---|         |---
		//           +=========+
		//Zones 1, 2, 3, 4 will place the ball near straight lines
		//Zones 5, 6, 7, 8 will place the ball near rounded corners
		//First, check the rounded corners

	}
	//END prevent collision again (get out the ball)

	ball_pos = next_ball_pos;
}

void Server::scored(bool is_player) {
	//Add 1 point to one of the scores
	if (is_player)
		player_score++;
	else
		enemy_score++;

	ball_pos = { window_size.x * 0.5F, window_size.y * 0.5F }; //Place the ball to the center of the window
	ball_direction = gm::random_number_double_range(10.0F * DEG2RAD, 170.0F * DEG2RAD, //Random direction
												190.0F * DEG2RAD, 350.0F * DEG2RAD);

	waiting_for_input = true; //Suspend
}
