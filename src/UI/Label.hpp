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

#pragma once

#include "../GameManager.hpp"
#include "UIControl.hpp"

class Label : public UIControl {
public:
	Label(sf::RenderWindow* window, sf::String string, sf::Vector2f relative_position,
		  UIControl::Relativity relative_to, UIControl::Relativity alignment, unsigned int font_size,
		  sf::Color color = sf::Color::White, sf::Font* font = GameManager::get_default_font());
	Label();

	void init(sf::RenderWindow* window, sf::String string, sf::Vector2f relative_position,
			  UIControl::Relativity relative_to, UIControl::Relativity alignment, unsigned int font_size,
			  sf::Color color = sf::Color::White, sf::Font* font = GameManager::get_default_font());

	///Set fill color of the text
	void set_color(sf::Color color);

	///Set new text of the label
	void set_text(sf::String string);

	void render() override;

private:
	sf::Text text;
	///Original relative position that assigned in the init()
	sf::Vector2f original_rel_pos;

	///Refresh the position to correct
	void refresh_pos();
};
