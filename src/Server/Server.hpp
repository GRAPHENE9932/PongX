/*
 * PongX abstract server
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
	virtual sf::FloatRect get_player_rect() = 0;
	///Get the current rect of the enemy
	virtual sf::FloatRect get_enemy_rect() = 0;
	///Get the current position of the ball
	virtual sf::Vector2f get_ball_pos() = 0;

	///Request player move up
	virtual void move_player_up() = 0;

private:

};
