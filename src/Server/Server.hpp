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

#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "ServerSettings.hpp"

///Server takes input like player's moves and
///returns data about player's, enemy's and ball's position
class Server {
public:
	///Create a new server using the specified settings
	static Server* create(ServerSettings setting);

	///Update server's state: ball, collisions, movement etc
	virtual void update() = 0;

	///Get the current rect of the player
	sf::FloatRect get_player_rect();
	///Get the current rect of the enemy
	sf::FloatRect get_enemy_rect();
	///Get the current position of the ball
	sf::Vector2f get_ball_pos();

	///Request player move up
	virtual void move_player_up() = 0;

private:
	ServerSettings::ServerType server_type;

	sf::Vector2f ball_pos;
	///Ball's radius in pixels
	float ball_radius;
	///Ball's direction in radians
	///0 rad - bottom
	///PI/2 rad - right
	float ball_direction;
	///Ball's speed (pixels per frame)
	float ball_speed;

	sf::Vector2f window_size;

	sf::FloatRect player_rect, enemy_rect;

	///Update ball movement, check for collisions and change direction
	void update_ball_movement();

	///Calculate distance between the 2 points
	inline float distance(sf::Vector2f point_1, sf::Vector2f point_2);

	///Add current rect position to specified position
	inline void move_rect(sf::FloatRect* rect, sf::Vector2f rel_pos);

	///Generate random number in range [min_1;max_1]&[min_2;max_2)
	inline float random_number_double_range(const float min_1, const float max_1,
											const float min_2, const float max_2);

	///Is specified vertical line intersects with a specified circle?
	inline bool intersects_with_vertical_line(float line_1_y, float line_2_y, float line_x,
											  sf::Vector2f circle_pos, float radius);

	///Is specified horizontal line intersects with a specified circle?
	inline bool intersects_with_horizontal_line(float line_1_x, float line_2_x, float line_y,
												sf::Vector2f circle_pos, float radius);
};
