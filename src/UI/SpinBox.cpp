/*
 * PongX UI spin box
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

#include <sstream>
#include <iomanip>

#include "SpinBox.hpp"

SpinBox::SpinBox(sf::RenderWindow* window, sf::Vector2f relative_position, UIControl::Relativity relative_to,
				 UIControl::Relativity alignment, sf::Vector2f size, unsigned int font_size, sf::Color color,
				 sf::Color bg_color, sf::Font* font) {
	//Assign the base variables
	this->window = window;
	this->relative_position = relative_position;
	this->relative_to = relative_to;
	this->alignment = alignment;
	this->size = size;

	//Compute buttons and label width
	float buttons_width = size.x * 0.25F;
	float label_width = size.x - buttons_width;

	//Initialize the label
	//Compute the center of the label's X axis
	float label_center = label_width * 0.5F - size.x * 0.5F;
	//Init label
	label.parent = this;
	label.init(window, "", { label_center, 0 }, UIControl::CenterCenter, UIControl::CenterCenter, font_size,
			   color, font);

	//Initialize the buttons
	button_up.parent = this;
	button_up.init(window, { -buttons_width, 0 }, UIControl::RightTop, UIControl::LeftTop,
				   { buttons_width, size.y * 0.5F }, "+", 24);

	button_down.parent = this;
	button_down.init(window, { -buttons_width, size.y * 0.5F }, UIControl::RightTop, UIControl::LeftTop,
					 { buttons_width, size.y * 0.5F }, "-", 24);

	//Initialize the main rect
	main_rect.setSize(size);
	main_rect.setPosition(position());
	main_rect.setFillColor(bg_color);
	main_rect.setOutlineThickness(3);
	main_rect.setOutlineColor(color);
}

void SpinBox::set_maximum(float max) {
	this->max = max;
	if (value > max)
		value = max;
}

void SpinBox::set_minimal(float min) {
	this->min = min;
	if (value < min)
		value = min;
}

void SpinBox::set_step(float step) {
	this->step = step;
}

void SpinBox::set_value(float value) {
	if (value > max)
		this->value = max;
	else if (value < min)
		this->value = min;
	else
		this->value = value;
}

void SpinBox::set_precision(unsigned char precision) {
	this->precision = precision;
}

float SpinBox::get_value() {
	return value;
}

void SpinBox::update_text() {
	//Set value to the label
	std::stringstream ss;
	ss << std::fixed << std::setprecision(precision) << value;
	label.set_text(ss.str());
}

void SpinBox::render() {
	window->draw(main_rect);
	button_up.render();
	button_down.render();
	label.render();

	if (button_up.clicked) { //Check if the up button is clicked
		value += step; //Add step to the value
		if (value > max) //Clamp the value
			value = max;
	}
	if (button_down.clicked) { //Check if the down button is clicked
		value -= step; //Substract step from the value
		if (value < min) //Clamp the value
			value = min;
	}
	if (button_down.clicked || button_up.clicked) {
		update_text();
	}
}
