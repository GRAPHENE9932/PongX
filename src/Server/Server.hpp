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

#include "../GameManager.hpp"
#include "ServerSettings.hpp"

///Server takes input like player's moves and
///returns data about player's, enemy's and ball's position.
///Management of player movement (changing speed) is on GamePage.
///Management of enemy movement (changing speed) is on inherited Server.
class Server {
public:
	///Create a new server using the specified settings
	static Server* create(ServerSettings setting);

	Server(sf::Vector2u window_size);

	///Update server's state: ball, collisions, movement etc
	virtual void update() = 0;

	///Speed of player that relative to max (1 - max down, 0 - static, -1 - max up)
	float player_relative_speed;

	///Get the current rect of the player
	sf::FloatRect get_player_rect();
	///Get the current rect of the enemy
	sf::FloatRect get_enemy_rect();
	///Get the current position of the ball
	sf::Vector2f get_ball_pos();
	///Get the current direction of the ball in radians
	float get_ball_dir();
    ///Get current player's score
    unsigned int get_player_score();
    ///Get current enemy's score
    unsigned int get_enemy_score();

protected:
	GameType server_type;

	sf::Vector2f ball_pos;
	///Ball's radius in pixels
	float ball_radius;
	///Ball's direction in radians
	float ball_direction;
	///Ball's speed (pixels per frame)
	float ball_speed;

	///Speed of enemy that relative to max (1 - max down, 0 - static, -1 - max up)
	float enemy_relative_speed;

    unsigned int player_score = 0, enemy_score = 0;
    ///If true, ball suspended until input from player or enemy incomes
    bool waiting_for_input = true;
	///Is collision occured on the last frame
	bool collided_before = false;

	sf::Vector2u window_size;

	sf::FloatRect player_rect, enemy_rect;

	///Update ball movement, check player (enemy) movement and other stuff
	void internal_update();

	///When someone scores
	///@param is_player who scored?
	void scored(bool is_player);

private:
	///Update player and enemy movement, check collision with window borders
	void update_player_movement();
	///Update ball movement, check for collisions and change direction
	void update_ball_movement();
};
