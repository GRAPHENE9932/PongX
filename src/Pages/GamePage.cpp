/*
 * PongX main game page
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

#include <cmath>

#include "../Enemies/LocalEnemy.hpp"
#include "GamePage.hpp"

constexpr float PI = 3.14159265359F;
///If multiply this constant by an angle in degrees, result is in radians
constexpr float DEG2RAD = PI / 180.0F;

//BEGIN global functions
inline float distance(sf::Vector2f point_1, sf::Vector2f point_2) {
	return std::abs(point_1.x - point_2.x) * std::abs(point_1.x - point_2.x) +
		std::abs(point_1.y - point_2.y) * std::abs(point_1.y - point_2.y);
}

///Add current rect position to specified position
template<typename T>
inline void move_rect(sf::Rect<T>* rect, sf::Vector2<T> rel_pos) {
	rect->left += rel_pos.x;
	rect->top += rel_pos.y;
}

///Generate random number in range [min_1;max_1]&[min_2;max_2)
inline float random_number_double_range(const float min_1, const float max_1,
										const float min_2, const float max_2) {
	float max = (max_1 - min_1) + (max_2 - min_2); //Prepare for number generation
	float raw_random = GameManager::random_number(0.0F, max); //Generate raw number

	//If in first half of the range
	if (raw_random <= max_1 - min_1)
		return min_1 + raw_random;
	else //If in second half of the range
		return min_2 + raw_random - (max_1 - min_1);
}

///Is specified vertical line intersects with specified circle?
inline bool intersects_with_vertical_line(float line_1_y, float line_2_y, float line_x, sf::Vector2f circle_pos,
										  float radius) {
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

inline bool intersects_with_horizontal_line(float line_1_x, float line_2_x, float line_y, sf::Vector2f circle_pos,
											float radius) {
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
//END global functions

GamePage::GamePage(sf::RenderWindow* window, GameType game_type) {
	this->window = window;

	//Initialize enemy
	switch (game_type) {
		case LocalMultiplayer: {
			enemy = new LocalEnemy(sf::FloatRect({ 10, 0 }, { 45, 225 }),
								   0, window->getSize().y);
			break;
		}
		default: {
			break; //TODO 05.06.2021: Singleplayer, LocalNetworkMultiplayer
		}
	}

	//Create player and enemy rect
	player_rect = sf::FloatRect( { 10, 0 }, { 45, 225 } );
	enemy_rect = sf::FloatRect( { 1225, 0 }, { 45, 225 } );

	//Initialize player shape
	player_shape.setSize({ player_rect.width, player_rect.height });
	player_shape.setPosition({ player_rect.left, player_rect.top });
	player_shape.setFillColor(sf::Color::White);
	//Initialize enemy shape
	enemy_shape.setSize({ enemy_rect.width, enemy_rect.height });
	enemy_shape.setPosition({ enemy_rect.left, enemy_rect.top });
	enemy_shape.setFillColor(sf::Color::White);

	//Set start position, radius, direction and speed of the ball
	radius = 25.0F;
	direction = random_number_double_range(10.0F * DEG2RAD, 170.0F * DEG2RAD,
										   190.0F * DEG2RAD, 350.0F * DEG2RAD);
	speed = 5.0F;
	ball_pos = { window->getSize().x / 2.0F, window->getSize().y / 2.0F }; //Place ball in the center of window
	//Initialize ball shape
	ball_shape.setOrigin(radius, radius);
	ball_shape.setPosition(ball_pos);
	ball_shape.setFillColor(sf::Color::White);
	ball_shape.setRadius(radius);
}

void GamePage::render() {
	//BEGIN Keyboard control
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player_rect.top > 0)
		move_rect(&player_rect, { 0, -10 });
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
		player_rect.top + player_rect.height < window->getSize().y)
		move_rect(&player_rect, { 0, 10 });
	player_shape.setPosition( { player_rect.left, player_rect.top } ); //Set position
	//END Keyboard control

	//BEGIN Enemy
	//Update enemy
	enemy_rect.top = enemy->update();
	enemy_shape.setPosition({ enemy_rect.left, enemy_rect.top }); //Set position of the enemy
	//END Enemy

	//BEGIN Ball
	//Move in the specified direction
	ball_pos += { std::sin(direction) * speed, std::cos(direction) * speed };
	//Syncronize ball_shape and ball_pos
	ball_shape.setPosition(ball_pos);

	check_player_enemy_collision();
	check_bounds_collision();
	//END Ball

	//Render
	window->draw(player_shape);
	window->draw(enemy_shape);
	window->draw(ball_shape);
}

void GamePage::check_player_enemy_collision() {
	bool player_side = intersects_with_vertical_line(player_rect.top,
													 player_rect.top + player_rect.height,
													 player_rect.left + player_rect.width,
													 ball_pos, radius); //Player's right side

	bool enemy_side = intersects_with_vertical_line(enemy_rect.top,
													enemy_rect.top + enemy_rect.height,
													enemy_rect.left,
													ball_pos, radius); //Enemy's left side

	bool player_top = intersects_with_horizontal_line(player_rect.left,
													  player_rect.left + player_rect.width,
													  player_rect.top,
													  ball_pos, radius); //Player's top side

	bool player_bottom = intersects_with_horizontal_line(player_rect.left,
														 player_rect.left + player_rect.width,
														 player_rect.top + player_rect.height,
														 ball_pos, radius); //Player's bottom side

	bool enemy_top = intersects_with_horizontal_line(enemy_rect.left,
													 enemy_rect.left + enemy_rect.width,
													 enemy_rect.top,
													 ball_pos, radius); //Enemy's top side

	bool enemy_bottom = intersects_with_horizontal_line(enemy_rect.left,
														enemy_rect.left + enemy_rect.width,
														enemy_rect.top + enemy_rect.height,
														ball_pos, radius); //Enemy's bottom side

	if (player_side || enemy_side) {
		//                           \  ||
		//                  Before -> \ ||
		//                             \||
		//                              0| <- Ball
		//                             /||
		//                   After -> / ||
		//                           /  ||
		direction = 360.0F * DEG2RAD - direction;
	}
	else if (player_top || player_bottom || enemy_top || enemy_bottom) {
		// +================o==================+
		// |               /|\                 |
		// |    Before -> / | \ <- After       |
		// |             /  |  \               |
		direction = 180.0F * DEG2RAD - direction;
	}

	//Prevent collision again
	if (player_side)
		ball_pos.x = player_rect.left + player_rect.width + radius;
	else if (enemy_side)
		ball_pos.x = enemy_rect.left - radius;
	else if (player_top)
		ball_pos.y = player_rect.top - radius;
	else if (enemy_top)
		ball_pos.y = enemy_rect.top - radius;
	else if (player_bottom)
		ball_pos.y = player_rect.top + player_rect.height + radius;
	else if (enemy_bottom)
		ball_pos.y = enemy_rect.top + enemy_rect.height + radius;
}

void GamePage::check_bounds_collision() {
	bool top_win_bound = ball_pos.y - radius <= 0; //Top global bound

	bool bottom_win_bound = ball_pos.y + radius >= window->getSize().y; //Bottom global bound

	if (top_win_bound || bottom_win_bound) {
		// +================o==================+
		// |               /|\                 |
		// |    Before -> / | \ <- After       |
		// |             /  |  \               |
		direction = 180.0F * DEG2RAD - direction;
	}
}
