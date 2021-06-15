/*
 * PongX FPS meter
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

FPSMeter::FPSMeter(sf::RenderWindow* window, sf::Vector2f relative_position, UIControl::Relativity relative_to,
				   UIControl::Relativity alignment, unsigned int font_size, sf::Color color, sf::Font* font) {
	this->window = window;
	this->relative_position = relative_position;
	this->relative_to = relative_to;
	this->alignment = alignment;

	//Set basic parameters of text
	text.setCharacterSize(font_size);
	text.setFillColor(color);
	text.setPosition(position());
	text.setFont(*font);
}

void FPSMeter::render() {
	//Get elapsed time
	sf::Time elapsed = clock.getElapsedTime();
	clock.restart();
	//Calculate FPS
	unsigned short fps = 1.0F / elapsed.asSeconds();

	//Display
	text.setString(std::to_string(fps) + " FPS");
	window->draw(text);
}
