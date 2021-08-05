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

GamePage::GamePage(sf::RenderWindow* window, ServerSettings settings) {
	this->window = window;

	settings.window_size = window->getSize();

	switch (settings.server_type) {
		case LocalMultiplayer: {
			settings.enemy_up_key = sf::Keyboard::Up;
			settings.enemy_down_key = sf::Keyboard::Down;
			break;
		}
		default: {
			//TODO: 09.06.2021: Singleplayer, LocalNetworkHost, LocalNetworkClient
		}
	}

	server = Server::create(settings);

	//Initialize player shape
	player_shape.setSize({ settings.player_rect.width, settings.player_rect.height });
	player_shape.setPosition({ settings.player_rect.left, settings.player_rect.top });
	player_shape.setFillColor(sf::Color::White);
	//Initialize enemy shape
	enemy_shape.setSize({ settings.enemy_rect.width, settings.enemy_rect.height });
	enemy_shape.setPosition({ settings.enemy_rect.left, settings.enemy_rect.top });
	enemy_shape.setFillColor(sf::Color::White);

	//Initialize ball shape
	ball_shape.setOrigin(settings.ball_radius, settings.ball_radius);
	ball_shape.setFillColor(sf::Color::White);
	ball_shape.setRadius(settings.ball_radius);

    //Initialize separator
    separator.setSize({ 6, static_cast<float>(window->getSize().y) });
    separator.setPosition({ (window->getSize().x - separator.getSize().x) * 0.5F, 0 });
    separator.setFillColor(sf::Color::White);

    //Initialize player's and enemy's score text
    player_score_text.init(window, "0", { -10, 10 }, UIControl::CenterTop, UIControl::RightTop, 150);
    enemy_score_text.init(window, "0", { 10, 10 }, UIControl::CenterTop, UIControl::LeftTop, 150);
}

void GamePage::render() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		server->move_player_up();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		server->move_player_down();

	//Update server
	server->update();

	//Set position of the player
	player_shape.setPosition({ server->get_player_rect().left, server->get_player_rect().top }); //Set position

	//Set position of the enemy
	enemy_shape.setPosition({ server->get_enemy_rect().left, server->get_enemy_rect().top });

	//Syncronize ball_shape and ball_pos
	ball_shape.setPosition(server->get_ball_pos());

	//Syncronize scores
	player_score_text.set_text(std::to_string(server->get_player_score()));
	enemy_score_text.set_text(std::to_string(server->get_enemy_score()));

	//Render
	window->draw(player_shape);
	window->draw(enemy_shape);
	window->draw(ball_shape);
    window->draw(separator);
    player_score_text.render();
    enemy_score_text.render();

	//Show direction
	sf::VertexArray line(sf::LineStrip, 2);
	line[0] = server->get_ball_pos();
	line[1] = server->get_ball_pos() +
		sf::Vector2f(std::cos(server->get_ball_dir()) * 5000, std::sin(server->get_ball_dir()) * 5000);
	window->draw(line);
}
