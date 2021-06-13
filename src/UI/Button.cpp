/*
 * PongX UI button
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

#include "Button.hpp"

Button::Button(sf::RenderWindow* window, sf::Vector2f relative_position, UIControl::Relativity relative_to,
			   sf::Vector2f size, sf::String title, unsigned int font_size, sf::Color color,
			   sf::Color bg_color, sf::Font* font) {
	init(window, relative_position, relative_to, size, title, font_size, color, bg_color, font);
}

Button::Button() {
	//Do nothing
}

void Button::init(sf::RenderWindow* window, sf::Vector2f relative_position, UIControl::Relativity relative_to,
				  sf::Vector2f size, sf::String title, unsigned int font_size, sf::Color color,
				  sf::Color bg_color, sf::Font* font) {
	this->window = window; //Assign fields
	this->color = color;
	this->bg_color = bg_color;
	this->relative_position = relative_position;
	this->relative_to = relative_to;
	this->size = size;

	//Setup the main rect
	main_rect.setSize(size);
	main_rect.setOutlineColor(color);
	main_rect.setOutlineThickness(3);
	main_rect.setFillColor(bg_color);
	main_rect.setPosition(position());

	//Setup the label
	label.parent = this; //And set it in the center of the button
	label.init(window, title, { 0, 0 }, UIControl::CenterCenter, font_size, color, font);
}

void Button::render() {
	//Set that not clicked
	clicked = false;

	//Check if mouse pressed
	bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	//If in area
	if (is_mouse_in_area()) {
		main_rect.setFillColor(color); //Swap colors
		label.set_color(bg_color);
	}
	else {
		main_rect.setFillColor(bg_color); //Normal state of colors
		label.set_color(color);
	}

	//Render label and rectangle
	window->draw(main_rect);
	label.render();

	//Accept click
	if (pressed && is_mouse_in_area() && !pressed_before)
		clicked = true;

	//Handle pressed_before
	if (pressed)
		pressed_before = true;
	else
		pressed_before = false;
}

inline bool Button::is_mouse_in_area() const {
	//Get mouse position in ints and convert it in floats
	auto mouse_pos_i = sf::Mouse::getPosition(*window);
	sf::Vector2f mouse_pos = { static_cast<float>(mouse_pos_i.x), static_cast<float>(mouse_pos_i.y) };
	return main_rect.getGlobalBounds().contains(mouse_pos);
}
