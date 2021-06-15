/*
 * PongX main game manager
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

#include "UI/FPSMeter.hpp"
#include "Pages/MainMenuPage.hpp"
#include "GameManager.hpp"

//Define static variables
std::vector<UIControl*> GameManager::ui_list;
Page* GameManager::page = nullptr;
sf::Font GameManager::default_font;
std::mt19937 GameManager::randomizer;
bool GameManager::randomizer_initialized = false;

int GameManager::start() {
	//Create a window
	sf::RenderWindow main_window = sf::RenderWindow(sf::VideoMode(1280, 720), "PongX",
													sf::Style::Titlebar | sf::Style::Close);
	main_window.setFramerateLimit(60);

	//Add FPS ui control
	FPSMeter* fps_meter = new FPSMeter(&main_window, { 0, 0 }, UIControl::LeftTop, UIControl::LeftTop, 36,
									   sf::Color::White);
	ui_list.push_back(fps_meter);

	//Create page
	page = new MainMenuPage(&main_window);

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

void GameManager::switch_page(Page* new_page) {
	delete page;
	page = new_page;
}

sf::Font* GameManager::get_default_font() {
	if (default_font.getInfo().family == "")
		default_font.loadFromFile("default.ttf");

	return &default_font;
}

float GameManager::random_number(float min, float max) {
	if (!randomizer_initialized) { //Initialize randomizer if it is not
		std::random_device true_gen; //Get the true random number which used for seed
		randomizer = std::mt19937(true_gen()); //Seed and initialize our randomizer
	}
	//This thing handles raw number from randomizer and turns it into the float in range [min;max)
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(randomizer); //Use distribution and randomizer to generate the number
}
