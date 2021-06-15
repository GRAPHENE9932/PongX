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

#pragma once

#include "Button.hpp"
#include "Label.hpp"
#include "UIControl.hpp"

///UI control where user can set any number using the buttons
class SpinBox : public UIControl {
public:
	SpinBox(sf::RenderWindow* window, sf::Vector2f relative_position, UIControl::Relativity relative_to,
			UIControl::Relativity alignment, sf::Vector2f size, unsigned int font_size,
			sf::Color color = sf::Color::White, sf::Color bg_color = sf::Color::Black,
			sf::Font* font = GameManager::get_default_font());

	void render() override;

	///Set the minimal allowable value.
	///WARNING: visible effect only after the render() function
	void set_minimal(float min);
	///Set the maximal allowable value.
	///WARNING: visible effect only after the render() function
	void set_maximum(float max);
	///Set the variable that adds or substracts to the value when a one of the buttons clicked.
	///WARNING: visible effect only after the render() function
	void set_step(float step);
	///Set the сurrent value of the spinbox (clamp between the minimum and maximum).
	///WARNING: visible effect only after the render() function
	void set_value(float value);
	///Decimal precision of the value (applies on the display only).
	///WARNING: visible effect only after the render() function
	void set_precision(unsigned char precision);

	///Get the current value of the spinbox
	float get_value();

private:
	///The minimal allowable value
	float min;
	///The maximal allowable value
	float max;
	///This variable adds or substracts to the value when button clicked
	float step;
	///Current value of the spinbox
	float value;
	///Decimal precision of the value (applies on the display only)
	unsigned char precision;

	///Button that increases the value
	Button button_up;
	///Button that decreases the value
	Button button_down;
	///Labels that displays the number
	Label label;
	///Main shape of the spinbox (background)
	sf::RectangleShape main_rect;
};
