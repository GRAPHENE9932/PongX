/*
 * PongX button
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

Button::Button(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size, sf::String title,
			   unsigned int font_size, sf::Color color, sf::Color bg_color, sf::Font* font) {
	this->window = window;
	this->color = color;
	this->bg_color = bg_color;
	this->position = position;
	this->size = size;

	//Setup the main rect
	main_rect.setSize(size);
	main_rect.setOutlineColor(color);
	main_rect.setOutlineThickness(3);
	main_rect.setFillColor(bg_color);
	main_rect.setPosition(position);

	//Setup the text
	text.setString(title);
	text.setFont(*font);
	text.setCharacterSize(font_size);
	text.setFillColor(color);
	//Calculate position manually for central alignment
	auto text_bounds = text.getLocalBounds();
	sf::Vector2f text_position = {
		position.x + (size.x - text_bounds.width) / 2,
		position.y + (size.y - text_bounds.height) / 2
	};
	text.setPosition(text_position);
}

void Button::set_callback(std::function<void()> on_click) {
	this->on_click = on_click;
}

void Button::render() {
	//Check if mouse pressed
	bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	//If in area
	if (is_mouse_in_area()) {
		main_rect.setFillColor(color); //Swap colors
		text.setFillColor(bg_color);
	}
	else {
		main_rect.setFillColor(bg_color); //Normal state of colors
		text.setFillColor(color);
	}

	//Accept click
	if (pressed && is_mouse_in_area() && !pressed_before)
		on_click();

	//Handle pressed_before
	if (pressed)
		pressed_before = true;
	else
		pressed_before = false;

	//Render text and rect
	window->draw(main_rect);
	window->draw(text);
}

inline bool Button::is_mouse_in_area() const {
	return (sf::Mouse::getPosition(*window).x >= position.x) &&
		(sf::Mouse::getPosition(*window).x <= position.x + size.x) &&
		(sf::Mouse::getPosition(*window).y >= position.y) &&
		(sf::Mouse::getPosition(*window).y <= position.y + size.y);
}
