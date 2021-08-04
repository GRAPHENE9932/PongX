/*
 * PongX basic math unit test
 * Copyright (C) 2021  Artem Kliminskyi artemklim50@gmail.com
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

#include <gtest/gtest.h>

#include "macros.hpp"
#include "../src/game_math.hpp"

TEST(basic_math, distance) {
	EXPECT_EQ(4.0F, gm::distance({-2.0F, 0.0F}, {2.0F, 0.0F}));
	EXPECT_EQ(5.0F, gm::distance({7.0F, -1.0F}, {4.0F, 3.0F}));
	EXPECT_EQ(5.0F, gm::distance({1.0F, -1.0F}, {4.0F, 3.0F}));
}

TEST(basic_math, rect_distance) {
	EXPECT_EQ(1.0F, gm::rect_distance(sf::FloatRect(-2, -2, 4, 4), {-3, 0}));
	EXPECT_EQ(2.0F, gm::rect_distance(sf::FloatRect(-1, 1, 5, 3), {1, 6}));
	EXPECT_EQ(0.0F, gm::rect_distance(sf::FloatRect(-2, -2, 4, 4), {-1, 0}));
	EXPECT_NEAR(1.414213F, gm::rect_distance(sf::FloatRect(-1, 1, 5, 3), {5, 5}), 0.00001F);
}

TEST(basic_math, is_between) {
	EXPECT_EQ(true, gm::is_between(1, 1, 6));
	EXPECT_EQ(true, gm::is_between(6, 1, 6));
	EXPECT_EQ(true, gm::is_between(5, 1, 6));
	EXPECT_EQ(true, gm::is_between(5, 6, 1));
	EXPECT_EQ(true, gm::is_between(-5, -1, -6));
	EXPECT_EQ(true, gm::is_between(-5, -6, -1));
	EXPECT_EQ(true, gm::is_between(5, 6, -6));
	EXPECT_EQ(true, gm::is_between(5, -6, 6));
}

TEST(basic_math, quadrant) {
	EXPECT_EQ(1, gm::quadrant({4, 2}, {8, 3}));
	EXPECT_EQ(2, gm::quadrant({4, 2}, {1, 3}));
	EXPECT_EQ(3, gm::quadrant({4, 2}, {2, 1}));
	EXPECT_EQ(4, gm::quadrant({4, 2}, {7, -1}));
}

TEST(basic_math, is_higher_semiplane) {
	EXPECT_EQ(true, gm::is_higher_semiplane(3, -4, {-2, 0}));
	EXPECT_EQ(false, gm::is_higher_semiplane(-2, 4, {-2, 4}));
}
