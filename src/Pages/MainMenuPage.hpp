/*
 * PongX main menu page
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

#include <vector>

#include "../UI/UIControl.hpp"
#include "Page.hpp"

class MainMenuPage : public Page {
public:
	MainMenuPage(sf::RenderWindow* window);
	~MainMenuPage() override;

	void render() override;

private:
	///UI controls list for this page
	std::vector<UIControl*> ui_list;

	///Pointer to the "clicked" filed of the "local multiplayer" button
	bool* local_multiplayer_clicked;

	///When "Local multiplayer" button clicked
	void local_multiplayer_click();
};
