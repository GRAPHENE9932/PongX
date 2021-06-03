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
#include "FPSMeter.hpp"

FPSMeter::FPSMeter(sf::RenderWindow* window, unsigned int font_size, sf::Color color) {
	this->window = window;

	//Set basic parameters of text
	sfml_text = sf::Text();
	sfml_text.setCharacterSize(font_size);
	sfml_text.setFillColor(color);
	sfml_text.setPosition(position);
	//Load and set font
	font.loadFromFile("VCR OSD Mono.ttf");
	sfml_text.setFont(font);
}

void FPSMeter::render() {
	//Get elapsed time
	sf::Time elapsed = clock.getElapsedTime();
	clock.restart();
	//Calculate FPS
	unsigned short fps = 1.0F / elapsed.asSeconds();

	//Display
	sfml_text.setString(std::to_string(fps) + " FPS");
	window->draw(sfml_text);
}
