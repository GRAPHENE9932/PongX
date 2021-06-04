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

#include "../UI/Button.hpp"
#include "MainMenuPage.hpp"

MainMenuPage::MainMenuPage(sf::RenderWindow* window) {
	this->window = window;
	//Initialize font
	font.loadFromFile("default.ttf");

	//Initialize the play button
	Button* play_button = new Button(window, { 500, 500 }, { 500, 200 }, "Play", 72, sf::Color::White,
									 sf::Color::Black, &font);
	play_button->set_callback([this] { play_clicked(); });
	ui_list.push_back(play_button);
}

void MainMenuPage::render() {
	//Render UI
	for (unsigned int i = 0; i < ui_list.size(); i++)
		ui_list[i]->render();
}

void MainMenuPage::play_clicked() {

}
