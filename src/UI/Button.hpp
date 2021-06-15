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

#include "../GameManager.hpp"
#include "Label.hpp"
#include "UIControl.hpp"

class Button : public UIControl {
public:
	Button(sf::RenderWindow* window, sf::Vector2f relative_position, UIControl::Relativity relative_to,
		   UIControl::Relativity alignment, sf::Vector2f size, sf::String title, unsigned int font_size,
		   sf::Color color = sf::Color::White, sf::Color bg_color = sf::Color::Black,
		   sf::Font* font = GameManager::get_default_font());
	Button();

	void init(sf::RenderWindow* window, sf::Vector2f relative_position, UIControl::Relativity relative_to,
			  UIControl::Relativity alignment, sf::Vector2f size, sf::String title, unsigned int font_size,
			  sf::Color color = sf::Color::White, sf::Color bg_color = sf::Color::Black,
			  sf::Font* font = GameManager::get_default_font());

	void set_callback(std::function<void()> on_click);

	void render() override;

	bool clicked = false;

private:
	sf::RectangleShape main_rect;
	Label label;

	///Means the color of the rect and text
	sf::Color color;
	///Means the color of background of the button
	sf::Color bg_color;

	///Is mouse was pressed on the last frame?
	bool pressed_before = false;

	///Check is mouse pointer in the area of the button
	inline bool is_mouse_in_area() const;
};
