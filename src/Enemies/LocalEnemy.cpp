/*
 * PongX local enemy
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

#include <SFML/Window/Keyboard.hpp>

#include "LocalEnemy.hpp"

//BEGIN global functions
template<typename T>
inline void move_rect(sf::Rect<T>* rect, sf::Vector2<T> rel_pos) {
	rect->left += rel_pos.x;
	rect->top += rel_pos.y;
}
//END global functions

LocalEnemy::LocalEnemy(sf::FloatRect cur_rect, float top_bound, float bottom_bound) {
	this->player_rect = cur_rect;
	this->top_bound = top_bound;
	this->bottom_bound = bottom_bound;
}

float LocalEnemy::update() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player_rect.top > top_bound)
		move_rect(&player_rect, { 0, -10 });
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
		player_rect.top + player_rect.height < bottom_bound)
		move_rect(&player_rect, { 0, 10 });

	return player_rect.top;
}
