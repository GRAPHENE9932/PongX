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
	//ball_direction = gm::random_number_double_range(0.0F * DEG2RAD, 75.0F * DEG2RAD, //Random direction
	//												100.0F * DEG2RAD, 260.0F * DEG2RAD);
	ball_direction = gm::random_number_triple_range(0.0F * DEG2RAD, 75.0F * DEG2RAD,
													115.0F * DEG2RAD, 255.0F * DEG2RAD,
													295.0F * DEG2RAD, 360.0F * DEG2RAD);
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

void Server::update_player_movement() {
	if (player_relative_speed != 0 || enemy_relative_speed != 0)
		waiting_for_input = false;
	else
		return;

	player_rect.top += player_relative_speed * 10.0F;
	enemy_rect.top += enemy_relative_speed * 10.0F;

	player_rect.top = std::clamp(player_rect.top, 0.0F, window_size.y - player_rect.height);
	enemy_rect.top = std::clamp(enemy_rect.top, 0.0F, window_size.y - enemy_rect.height);
}

void Server::update_ball_movement() {
    //Do not move if the ball suspended
	if (waiting_for_input)
		return;

	//Move ball by direction
	ball_pos +=
		sf::Vector2f(std::cos(ball_direction) * ball_speed, std::sin(ball_direction) * ball_speed);

	//BEGIN check collision of next ball with bounds of window
	bool top_win_bound = ball_pos.y - ball_radius <= 0; //Top global bound
	bool bottom_win_bound = ball_pos.y + ball_radius >= window_size.y; //Bottom global bound

	//Clamp the position of the ball
	std::clamp(ball_pos.y, ball_radius, window_size.y - ball_radius);

	//Change the direction of the ball considering the collisions with horizontal window bounds
	if (top_win_bound || bottom_win_bound) {
		// +================o==================+
		// |               /|\                 |
		// |    Before -> / | \ <- After       |
		// |             /  |  \               |
		ball_direction = 360.0F * DEG2RAD - ball_direction;
	}
	//END check collision with bounds of window
	//We will use "rounded rect - point" model, because it is identical but simplier than "rect - circle"
	//           +=========+
	//        ---|         |---
	//       - 5 |         | 6 -
	//     ||----+    1    +----||
	//     ||     \       /     ||
	//     ||      \     /      || 0 if no collision
	//     ||       \   /       ||
	//     ||        \ /        ||
	//     ||    4    *    2    ||  <- Player (enemy)
	//     ||        / \        ||
	//     ||       /   \       ||
	//     ||      /     \      ||
	//     ||     /       \     ||
	//     ||----+    3    +----||
	//       - 8 |         | 7 -
	//        ---|         |---
	//           +=========+
	unsigned char collision = 0;
	sf::FloatRect* cur_rect = nullptr;

	//Check collision with player and enemy
	if (gm::rounded_rect_contains(player_rect, ball_radius, ball_pos))
		cur_rect = &player_rect;
	else if (gm::rounded_rect_contains(enemy_rect, ball_radius, ball_pos))
		cur_rect = &enemy_rect;

	//If here was a collision, check in which segment
	if (cur_rect != nullptr)
		collision = gm::rounded_rect_segment_contains(*cur_rect, ball_radius, ball_pos);

	if (!collided_before) {
		//Also, change the direction of the ball
		switch (collision) {
			case 1:
			case 3: { //Horizontal surfaces
				// +================o==================+
				// |               /|\                 |
				// |    Before -> / | \ <- After       |
				// |             /  |  \               |
				ball_direction = 360.0F * DEG2RAD - ball_direction;
				break;
			}
			case 2:
			case 4: { //Vertical surfaces
				//                           \  ||
				//                  Before -> \ ||
				//                             \||
				//                              0| <- Ball
				//                             /||
				//                   After -> / ||
				//                           /  ||
				ball_direction = 180.0F * DEG2RAD - ball_direction + 360.0F * DEG2RAD;
				break;
			}
			case 5:
			case 6:
			case 7:
			case 8: { //Corners
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

	//BEGIN prevent collision again (get out the ball)
	//Move the ball out the player (or enemy) depending on segment of player (or enemy)
	switch (collision) {
		case 1: { //Top side
			ball_pos.y = cur_rect->top - ball_radius;
			break;
		}
		case 2: { //Right side
			ball_pos.x = cur_rect->left + cur_rect->width + ball_radius;
			break;
		}
		case 3: { //Bottom side
			ball_pos.y = cur_rect->top + cur_rect->height + ball_radius;
			break;
		}
		case 4: { //Left side
			ball_pos.x = cur_rect->left - ball_radius;
			break;
		}
		case 5:
		case 6:
		case 7:
		case 8: { //Rounded corners
			//Find out current center of current rounded corner circle
			sf::Vector2f circle_center;
			switch (collision) {
				case 5: {
					circle_center = {cur_rect->left, cur_rect->top};
					break;
				}
				case 6: {
					circle_center = {cur_rect->left + cur_rect->width, cur_rect->top};
					break;
				}
				case 7: {
					circle_center = {cur_rect->left + cur_rect->width, cur_rect->top + cur_rect->height};
					break;
				}
				case 8: {
					circle_center = {cur_rect->left, cur_rect->top + cur_rect->height};
					break;
				}
			}
			//     -----/ <- Intersection point we have to find out
			//   --    * <- Ball
			//  -     /    -
			// |     * <- Center of the circle
			//  -          -
			//   --      --
			//     ------
			//Compute angle of line
			float angle = gm::line_angle_from_points(circle_center, ball_pos);
			//Place ball in the correct place
			ball_pos.x = circle_center.x + std::cos(angle) * ball_radius;
			ball_pos.y = circle_center.y + std::sin(angle) * ball_radius;
		}
	}
	//END prevent collision again (get out the ball)

	//Set value to variable "collided_before"
	collided_before = collision != 0;
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

void Server::internal_update() {
	update_player_movement();
	update_ball_movement();
}
