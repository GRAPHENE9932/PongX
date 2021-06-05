/*
 * PongX UI label
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

#include "Label.hpp"

Label::Label(sf::RenderWindow* window, sf::String string, sf::Vector2f relative_position,
			 UIControl::Relativity relative_to, unsigned int font_size, sf::Color color,
			 sf::Font* font) {
	this->window = window; //Assign fields
	this->relative_position = relative_position;
	this->relative_to = relative_to;

	//Initialize text
	text.setString(string);
	text.setCharacterSize(font_size);
	text.setFillColor(color);
	text.setFont(*font);

	//Set position after size
	auto text_bounds = text.getLocalBounds(); //TODO: memory leak here
	this->size = { text_bounds.width, text_bounds.height };
	text.setPosition(pos_x(), pos_y());
	//pos_x() and pos_y() works properly only when size correctly assigned. See UIControl class
}

void Label::render() {
	//Render text
	window->draw(text);
}