/*
 * PongX base enemy
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

class Enemy {
public:
	///Update the enemy to let him move
	///@return new position of the enemy
	virtual float update() = 0;

protected:
	///Enemy (this) rect
	sf::FloatRect player_rect;
	///Acceptable bound where player_rect can exist
	float bottom_bound, top_bound;
};
