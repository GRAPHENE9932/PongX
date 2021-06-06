/*
 * PongX base UI control
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

#include <SFML/Graphics.hpp>

class UIControl {

public:
	enum Relativity : unsigned char {
		LeftTop, LeftCenter, LeftBottom,
		CenterTop, CenterCenter, CenterBottom,
		RightTop, RightCenter, RightBottom
	};

	virtual ~UIControl() { };

	float pos_x() const;
	float pos_y() const;
	sf::Vector2f position() const;
	sf::Vector2f parent_size() const;
	sf::Vector2f parent_pos() const;

	UIControl* parent = nullptr;

	virtual void render() = 0;

protected:
	Relativity relative_to;
	sf::Vector2f relative_position = { 0, 0 };
	sf::Vector2f size = { 0, 0 };
	sf::RenderWindow* window = nullptr;
};
