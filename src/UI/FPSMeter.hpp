/*
 * PongX FPS meter
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

class FPSMeter : public UIControl {
public:
	FPSMeter(sf::RenderWindow* window, sf::Vector2f relative_position, UIControl::Relativity relative_to,
			 UIControl::Relativity alignment, unsigned int font_size, sf::Color color = sf::Color::White,
			 sf::Font* font = GameManager::get_default_font());

	void render() override;

private:
	///Text for render
	sf::Text text;
	///Clock to calculate the frametime
	sf::Clock clock;
};
