/*
 * PongX local multiplayer server
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

#include "Server.hpp"

class LocalMultiplayerServer : public Server {
public:
	LocalMultiplayerServer(sf::Vector2u window_size);

	void update() override;

	///Key which has to be pressed to move the enemy up
	sf::Keyboard::Key enemy_up_key;
	///Key which has to be pressed to move the enemy up
	sf::Keyboard::Key enemy_down_key;
};
