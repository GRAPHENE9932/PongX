/*
 * PongX page class
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

#include <functional>

class Page {
public:
	bool enabled;

	virtual ~Page() { }; //Empty destructor

	virtual void render() = 0;

	void set_switch_page_callback(std::function<void(Page*)> switch_page_callback) {
		this->switch_page_callback = switch_page_callback;
	}

protected:
	std::function<void(Page*)> switch_page_callback;
	sf::RenderWindow* window;
};