/*
 * PongX server settings structure
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
#include <SFML/Window/Keyboard.hpp>

#include "../GameManager.hpp"

struct ServerSettings {
	///Necessary setting
	GameType server_type;
	///Necessary setting
	float ball_radius = 10.0F, ball_speed = 5.0F;
	///Necessary setting
	sf::FloatRect player_rect = sf::FloatRect( { 10, 0 }, { 45, 225 } ),
        enemy_rect = sf::FloatRect( { 1225, 0 }, { 45, 225 } );
	///Necessary setting
	sf::Vector2u window_size;

	///Only for local multiplayer
	sf::Keyboard::Key enemy_up_key = sf::Keyboard::Up, enemy_down_key = sf::Keyboard::Down;
};
