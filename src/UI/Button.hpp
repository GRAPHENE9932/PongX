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

#pragma once

#include "UIControl.hpp"

class Button : public UIControl {
public:
	Button(sf::RenderWindow* window, sf::Vector2f relative_position, UIControl::Relativity relative_to,
		   sf::Vector2f size, sf::String title, unsigned int font_size, sf::Color color,
		   sf::Color bg_color, sf::Font* font);

	void set_callback(std::function<void()> on_click);

	void render() override;

private:
	sf::RectangleShape main_rect;
	sf::Text text;

	sf::Color color, bg_color;

	std::function<void()> on_click;

	///Is mouse was pressed on the last frame?
	bool pressed_before = false;

	inline bool is_mouse_in_area() const;
};
