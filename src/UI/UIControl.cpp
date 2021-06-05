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

#include "UIControl.hpp"

float UIControl::pos_x() const {
	switch (relative_to) {
		case Relativity::LeftTop: //Left
		case Relativity::LeftCenter:
		case Relativity::LeftBottom: {
			return relative_position.x + parent_pos().x;
			break;
		}

		case Relativity::CenterTop: //Center
		case Relativity::CenterCenter:
		case Relativity::CenterBottom: {
			return relative_position.x + (parent_size().x - size.x) / 2 + parent_pos().x;
			break;
		}

		case Relativity::RightTop: //Right
		case Relativity::RightCenter:
		case Relativity::RightBottom: {
			return relative_position.x + parent_size().x + parent_pos().x;
			break;
		}

		default: { //What?
			return relative_position.x + parent_pos().y;
			break;
		}
	}
}

float UIControl::pos_y() const {
	switch (relative_to) {
		case Relativity::LeftTop: //Top
		case Relativity::CenterTop:
		case Relativity::RightTop: {
			return relative_position.y + parent_pos().y;
			break;
		}

		case Relativity::LeftCenter: //Center
		case Relativity::CenterCenter:
		case Relativity::RightCenter: {
			return relative_position.y + (parent_size().y - size.y) / 2 + parent_pos().y;
			break;
		}

		case Relativity::LeftBottom: //Bottom
		case Relativity::CenterBottom:
		case Relativity::RightBottom: {
			return relative_position.y + parent_size().y + parent_pos().y;
			break;
		}

		default: { //What?
			return relative_position.y + parent_pos().y;
			break;
		}
	}
}

sf::Vector2f UIControl::position() const {
	return { pos_x(), pos_y() };
}

sf::Vector2f UIControl::parent_size() const {
	if (parent == nullptr) {
		auto size_i = window->getSize(); //Window size and convert it to float
		return { static_cast<float>(size_i.x), static_cast<float>(size_i.y) };
	}
	else
		return parent->size;
}

sf::Vector2f UIControl::parent_pos() const {
	if (parent == nullptr)
		return { 0, 0 };
	else
		return parent->position();
}
