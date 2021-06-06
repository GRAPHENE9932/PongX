/*
 * PongX start game page
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
#include "StartGamePage.hpp"

StartGamePage::StartGamePage(sf::RenderWindow* window, GameType game_type) {
	this->window = window;
	this->game_type = game_type;

	//BEGIN UI
	//Initialize start game button
	Button* start_button = new Button(window, { -425, -125 }, UIControl::RightBottom, { 400, 100 }, "Start", 48);
	start_clicked = &start_button->clicked;

	//Initialize back button
	Button* back_button = new Button(window, { 25, -125 }, UIControl::LeftBottom, { 400, 100 }, "Back", 48);
	back_clicked = &back_button->clicked;

	ui_list.push_back(start_button);
	ui_list.push_back(back_button);
	//END UI
}

StartGamePage::~StartGamePage() {
	for (unsigned int i = 0; i < ui_list.size(); i++)
		delete ui_list[i];
}

void StartGamePage::render() {
	//Render UI
	for (unsigned int i = 0; i < ui_list.size(); i++)
		ui_list[i]->render();

	//Handle events
	if (*start_clicked)
		start_click();
	if (*back_clicked)
		back_click();
}

void StartGamePage::start_click() {
	//Create a new game page
	Page* new_page = new GamePage(window, game_type);
	//Switch to this page
	GameManager::switch_page(new_page);
}

void StartGamePage::back_click() {
	//Create a new "Main menu page"
	Page* new_page = new MainMenuPage(window);
	//Switch to this page
	GameManager::switch_page(new_page);
}
