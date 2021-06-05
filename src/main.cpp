/*
 * PongX main function
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

#include <iostream>

#include <SFML/Graphics.hpp>

#include "UI/UIControl.hpp"
#include "UI/FPSMeter.hpp"
#include "Pages/Page.hpp"
#include "Pages/MainMenuPage.hpp"

///Global UI controls list
std::vector<UIControl*> ui_list;
///Current page
Page* page;

int main() {
	//Create a window
	sf::RenderWindow main_window = sf::RenderWindow(sf::VideoMode(1280, 720), "PongX");
	main_window.setFramerateLimit(60);

	//Add FPS ui control
	FPSMeter* fps_meter = new FPSMeter(&main_window, { 0, 0 }, UIControl::LeftTop, 72, sf::Color::White);
	ui_list.push_back(fps_meter);

	//Create page
	page = new MainMenuPage(&main_window);
	page->set_switch_page_callback( //Set callback on switch page
		[](Page* new_page) { //Delete current page and set new
			delete page;
			page = new_page;
		}
	);

	//Main loop
	while (main_window.isOpen()) {
		//Handle events
		sf::Event event;
		while (main_window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::EventType::Closed: { //Close event
					main_window.close(); //Close
					break;
				}
				default: { //Other
					break; //Ignore
				}
			}
		}

		//Render stuff
		main_window.clear();

		//BEGIN render stuff
		//Render UI
		for (unsigned int i = 0; i < ui_list.size(); i++)
			ui_list[i]->render();

		//Render page
		page->render();
		//END render stuff

		main_window.display();
	}

    return 0;
}
