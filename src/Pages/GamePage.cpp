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

#include "../Server/ServerSettings.hpp"
#include "GamePage.hpp"

constexpr float PI = 3.14159265359F;
///If multiply this constant by an angle in degrees, result is in radians
constexpr float DEG2RAD = PI / 180.0F;

GamePage::GamePage(sf::RenderWindow* window, GameType game_type) {
	this->window = window;

	ServerSettings server_settings; //Create server settings

	server_settings.ball_radius = 25.0F; //Set ball radius
	server_settings.enemy_rect = sf::FloatRect( { 1225, 0 }, { 45, 225 } );
	server_settings.player_rect = sf::FloatRect( { 10, 0 }, { 45, 225 } );
	server_settings.window_size = window->getSize();
	server_settings.ball_speed = 5.0F;
	server_settings.server_type = game_type;

	switch (game_type) {
		case LocalMultiplayer: {
			server_settings.enemy_up_key = sf::Keyboard::Up;
			server_settings.enemy_down_key = sf::Keyboard::Down;
			break;
		}
		default: {
			//TODO: 09.06.2021: Singleplayer, LocalNetworkHost, LocalNetworkClient
		}
	}

	server = Server::create(server_settings);

	//Initialize player shape
	player_shape.setSize({ server_settings.player_rect.width, server_settings.player_rect.height });
	player_shape.setPosition({ server_settings.player_rect.left, server_settings.player_rect.top });
	player_shape.setFillColor(sf::Color::White);
	//Initialize enemy shape
	enemy_shape.setSize({ server_settings.enemy_rect.width, server_settings.enemy_rect.height });
	enemy_shape.setPosition({ server_settings.enemy_rect.left, server_settings.enemy_rect.top });
	enemy_shape.setFillColor(sf::Color::White);

	//Initialize ball shape
	ball_shape.setOrigin(server_settings.ball_radius, server_settings.ball_radius);
	ball_shape.setFillColor(sf::Color::White);
	ball_shape.setRadius(server_settings.ball_radius);
}

void GamePage::render() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		server->move_player_up();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		server->move_player_down();

	//Update server
	server->update();

	player_shape.setPosition({ server->get_player_rect().left, server->get_player_rect().top }); //Set position

	//Set position of the enemy
	enemy_shape.setPosition({ server->get_enemy_rect().left, server->get_enemy_rect().top });

	//Syncronize ball_shape and ball_pos
	ball_shape.setPosition(server->get_ball_pos());

	//Render
	window->draw(player_shape);
	window->draw(enemy_shape);
	window->draw(ball_shape);
}
