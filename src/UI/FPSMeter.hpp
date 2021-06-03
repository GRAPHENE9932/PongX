/*
 * <one line to give the program's name and a brief idea of what it does.>
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

#include "UIControl.hpp"

class FPSMeter : public UIControl {
public:
    FPSMeter(sf::RenderWindow* window, unsigned int font_size, sf::Color color);

	void render() override;
private:
	sf::Text sfml_text;

	sf::Clock clock;
	sf::Font font;
};