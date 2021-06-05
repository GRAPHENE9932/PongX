/*
 * PongX main game manager
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

#include <vector>

#include "Pages/Page.hpp"
#include "UI/UIControl.hpp"

enum GameType : unsigned char {
	LocalMultiplayer, Singleplayer, LocalNetworkMultiplayer
};

///Static class. Analog of main, but members of this class are accessible from any place
class GameManager {
public:
	///Start game. WARNING: DO NOT CALL MORE THAN 1 TIME
	static int start();

	///Switch current page to specified one and delete previous
	static void switch_page(Page* new_page);

	///Get default font ("default.ttf"). This function loads the font only once, then just return cached
	static sf::Font* get_default_font();
};
