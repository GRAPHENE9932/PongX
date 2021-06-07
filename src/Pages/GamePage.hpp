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

#pragma once

#include "../GameManager.hpp"
#include "../Enemies/Enemy.hpp"
#include "Page.hpp"

///Page where the main game playing. Contains field, player and enemy
class GamePage : public Page {
public:
	GamePage(sf::RenderWindow* window, GameType game_type);

	void render() override;

private:
	Enemy* enemy;
	sf::FloatRect player_rect, enemy_rect;
	///Shape only for render. Syncronized with the player_rect ot enemy_rect
	sf::RectangleShape player_shape, enemy_shape;
};