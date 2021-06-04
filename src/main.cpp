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
#include <thread>

#include <SFML/Graphics.hpp>

#include "UI/UIControl.hpp"
#include "UI/FPSMeter.hpp"

///Global UI controls list
std::vector<UIControl*> ui_list;

bool stop_rendering = false;

void rendering(sf::RenderWindow* window) {
	window->setActive(true); //Activate window

	//Render loop
	while (window->isOpen()) {
		//Check if stop requested
		if (stop_rendering)
			break;

		//Render stuff
		window->clear();

		//BEGIN render stuff
		//Render UI
		for (unsigned int i = 0; i < ui_list.size(); i++)
			ui_list[i]->render();
		//END render stuff

		window->display();
	}
}

int main() {
	//Create a window
	sf::RenderWindow main_window = sf::RenderWindow(sf::VideoMode(1280, 720), "PongX");
	main_window.setActive(false); //And deactivate it

	//Add FPS ui control
	FPSMeter* fps_meter = new FPSMeter(&main_window, 72, sf::Color::White);
	ui_list.push_back(fps_meter);

	//Start render thread
	std::thread render_thread(rendering, &main_window);

	//Main loop
	while (main_window.isOpen()) {
		//Handle events
		sf::Event event;
		while (main_window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::EventType::Closed: { //Close event
					stop_rendering = true; //Request stop to rendering thread
					render_thread.join(); //Wait
					main_window.close(); //Close
					break;
				}
				default: { //Other
					break; //Ignore
				}
			}
		}
	}

    return 0;
}
