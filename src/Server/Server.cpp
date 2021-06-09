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
#include "Server.hpp"

///PI constant
constexpr float PI = 3.14159265359F;
///If multiply this constant by an angle in degrees, result is in radians
constexpr float DEG2RAD = PI / 180.0F;

Server::Server(sf::Vector2u window_size) {
	//Initialize some parameters
	this->window_size = window_size;
	ball_pos = { window_size.x * 0.5F, window_size.y * 0.5F }; //Place the ball to the center of the window
	ball_direction = random_number_double_range(10.0F * DEG2RAD, 170.0F * DEG2RAD, //Random direction
												190.0F * DEG2RAD, 350.0F * DEG2RAD);
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

sf::FloatRect Server::get_player_rect() {
	return player_rect;
}

sf::FloatRect Server::get_enemy_rect() {
	return enemy_rect;
}

//BEGIN maths
inline float Server::distance(sf::Vector2f point_1, sf::Vector2f point_2) {
	return std::abs(point_1.x - point_2.x) * std::abs(point_1.x - point_2.x) +
		std::abs(point_1.y - point_2.y) * std::abs(point_1.y - point_2.y);
}

inline void Server::move_rect(sf::FloatRect* rect, sf::Vector2f rel_pos) {
	rect->left += rel_pos.x;
	rect->top += rel_pos.y;
}

inline float Server::random_number_double_range(const float min_1, const float max_1,
										const float min_2, const float max_2) {
	float max = (max_1 - min_1) + (max_2 - min_2); //Prepare for number generation
	float raw_random = GameManager::random_number(0.0F, max); //Generate raw number

	//If in first half of the range
	if (raw_random <= max_1 - min_1)
		return min_1 + raw_random;
	else //If in second half of the range
		return min_2 + raw_random - (max_1 - min_1);
}

inline bool Server::intersects_with_vertical_line(float line_1_y, float line_2_y, float line_x,
												  sf::Vector2f circle_pos, float radius) {
	//Force line points to be in order line_1_y < line_2_y
	if (line_1_y > line_2_y)
		std::swap(line_1_y, line_2_y);

	//Check if the circle center contained inside Y borders of the line
	// +===========================================+
	// |                                           |
	// |----+--------------------------------------| <- Y border
	// |    |                                      |
	// |    | <- Line                              |
	// |    |                                      |
	// |----+--------------------------------------| <- Y border
	// |                                           |
	// |                                           |
	// |                                           |
	// +===========================================+
	if (circle_pos.y >= line_1_y && circle_pos.y <= line_2_y) {
		//Check intersection in X axis
		return std::abs(line_x - circle_pos.x) <= radius;
	}
	else { //Circle still have a chance
		//If distance from the line points to the center less than radius, then yes, intersects
		float distance_1 = distance({ line_x, line_1_y }, circle_pos);
		float distance_2 = distance({ line_x, line_2_y }, circle_pos);

		return std::min(distance_1, distance_2) <= radius;
	}
}

inline bool Server::intersects_with_horizontal_line(float line_1_x, float line_2_x, float line_y,
													sf::Vector2f circle_pos, float radius) {
	//Force line points to be in order line_1.x < line_2.x
	if (line_1_x > line_2_x)
		std::swap(line_1_x, line_2_x);

	//Check if the circle center contained inside X borders of the line
	// +=======================================+
	// |          |           |                |
	// |          +-----------+ <- Line        |
	// |          |           |                |
	// |          |           |                |
	// |X border->|           | <- X border    |
	// |          |           |                |
	// |          |           |                |
	// +=======================================+
	if (circle_pos.x >= line_1_x && circle_pos.x <= line_2_x) {
		//Check intersection in Y axis
		return std::abs(line_y - circle_pos.y) <= radius;
	}
	else { //Circle still have a chance
		//If distance from the line points to the center less than radius, then yes, intersects
		float distance_1 = distance({ line_1_x, line_y }, circle_pos);
		float distance_2 = distance({ line_2_x, line_y }, circle_pos);

		return std::min(distance_1, distance_2) <= radius;
	}
}
//END maths

//BEGIN player and enemy movement
void Server::move_player_up() {
	move_rect(&player_rect, { 0, -10 });

	if (player_rect.top < 0.0F) //If the player already behind the limit, correct it's position
		player_rect.top = 0.0F;
}

void Server::move_player_down() {
	move_rect(&player_rect, { 0, 10 });

	//If the player already behind the limit, correct it's position
	if (player_rect.top > window_size.y - player_rect.height)
		player_rect.top = window_size.y - player_rect.height;
}

void Server::move_enemy_up() {
	move_rect(&enemy_rect, { 0, -10 });

	if (enemy_rect.top < 0.0F) //If the enemy already behind the limit, correct it's position
		enemy_rect.top = 0.0F;
}

void Server::move_enemy_down() {
	move_rect(&enemy_rect, { 0, 10 });

	//If the enemy already behind the limit, correct it's position
	if (enemy_rect.top > window_size.y - enemy_rect.height)
		enemy_rect.top = window_size.y - enemy_rect.height;
}
//END player and enemy movement
void Server::update_ball_movement() {
	//Move in the specified direction
	ball_pos += { std::sin(ball_direction) * ball_speed, std::cos(ball_direction) * ball_speed };

	//BEGIN check collision with the bounds (window)
	bool top_win_bound = ball_pos.y - ball_radius <= 0; //Top global bound

	bool bottom_win_bound = ball_pos.y + ball_radius >= window_size.y; //Bottom global bound

	if (top_win_bound || bottom_win_bound) {
		// +================o==================+
		// |               /|\                 |
		// |    Before -> / | \ <- After       |
		// |             /  |  \               |
		ball_direction = 180.0F * DEG2RAD - ball_direction;
	}
	//END check collision with the bounds (window)

	//BEGIN check collision with the player and the enemy
	bool player_side = intersects_with_vertical_line(player_rect.top,
													 player_rect.top + player_rect.height,
													 player_rect.left + player_rect.width,
													 ball_pos, ball_radius); //Player's right side

	bool enemy_side = intersects_with_vertical_line(enemy_rect.top,
													enemy_rect.top + enemy_rect.height,
													enemy_rect.left,
													ball_pos, ball_radius); //Enemy's left side

	bool player_top = intersects_with_horizontal_line(player_rect.left,
													  player_rect.left + player_rect.width,
													  player_rect.top,
													  ball_pos, ball_radius); //Player's top side

	bool player_bottom = intersects_with_horizontal_line(player_rect.left,
														 player_rect.left + player_rect.width,
														 player_rect.top + player_rect.height,
														 ball_pos, ball_radius); //Player's bottom side

	bool enemy_top = intersects_with_horizontal_line(enemy_rect.left,
													 enemy_rect.left + enemy_rect.width,
													 enemy_rect.top,
													 ball_pos, ball_radius); //Enemy's top side

	bool enemy_bottom = intersects_with_horizontal_line(enemy_rect.left,
														enemy_rect.left + enemy_rect.width,
														enemy_rect.top + enemy_rect.height,
														ball_pos, ball_radius); //Enemy's bottom side

	if (player_side || enemy_side) {
		//                           \  ||
		//                  Before -> \ ||
		//                             \||
		//                              0| <- Ball
		//                             /||
		//                   After -> / ||
		//                           /  ||
		ball_direction = 360.0F * DEG2RAD - ball_direction;
	}
	else if (player_top || player_bottom || enemy_top || enemy_bottom) {
		// +================o==================+
		// |               /|\                 |
		// |    Before -> / | \ <- After       |
		// |             /  |  \               |
		ball_direction = 180.0F * DEG2RAD - ball_direction;
	}

	//Prevent collision again
	if (player_side)
		ball_pos.x = player_rect.left + player_rect.width + ball_radius;
	else if (enemy_side)
		ball_pos.x = enemy_rect.left - ball_radius;
	else if (player_top)
		ball_pos.y = player_rect.top - ball_radius;
	else if (enemy_top)
		ball_pos.y = enemy_rect.top - ball_radius;
	else if (player_bottom)
		ball_pos.y = player_rect.top + player_rect.height + ball_radius;
	else if (enemy_bottom)
		ball_pos.y = enemy_rect.top + enemy_rect.height + ball_radius;
	//END check collision with the player and the enemy
}
