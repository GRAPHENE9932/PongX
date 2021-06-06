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

#include "../Enemies/LocalEnemy.hpp"
#include "GamePage.hpp"

//BEGIN global functions
template<typename T>
inline void move_rect(sf::Rect<T>* rect, sf::Vector2<T> rel_pos) {
	rect->left += rel_pos.x;
	rect->top += rel_pos.y;
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
}

void GamePage::render() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player_rect.top > 0)
		move_rect(&player_rect, { 0, -10 });
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
		player_rect.top + player_rect.height < window->getSize().y)
		move_rect(&player_rect, { 0, 10 });

	player_shape.setPosition( { player_rect.left, player_rect.top } ); //Set position

	//Update enemy
	enemy_rect.top = enemy->update();

	enemy_shape.setPosition({ enemy_rect.left, enemy_rect.top }); //Set position

	//Render
	window->draw(player_shape);
	window->draw(enemy_shape);
}
