/*
 * <one line to give the program's name and a brief idea of what it does.>
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

#include <limits>

#include <gtest/gtest.h>

#include "../src/game_math.hpp"

#define INF_F std::numeric_limits<float>().infinity()
//Macros for sf::Vector
#define EXPECT_EQ_V2(exp, orig) \
	EXPECT_EQ(exp.x, orig.x); \
	EXPECT_EQ(exp.y, orig.y);
#define EXPECT_NEAR_V2(exp, orig, abs_error) \
	EXPECT_NEAR(exp.x, orig.x, abs_error); \
	EXPECT_NEAR(exp.y, orig.y, abs_error);

TEST(lines_intersection, horizontal) {
	sf::Vector2f tmp_result;
	//y=x and y=0 lines
	EXPECT_EQ(true, gm::hor_segment_line_intersection(1.0F, {0, 0}, -1.0F, 1.0F, 0.0F, tmp_result));
	EXPECT_EQ_V2(sf::Vector2f(0.0F, 0.0F), tmp_result);

	//y=0,5x and y=0 lines
	EXPECT_EQ(true, gm::hor_segment_line_intersection(0.5F, {0, 0}, -1.0F, 1.0F, 0.0F, tmp_result));
	EXPECT_EQ_V2(sf::Vector2f(0.0F, 0.0F), tmp_result);

	//y=x and line segment from -15 to 5
	EXPECT_EQ(false, gm::hor_segment_line_intersection(1.0F, {0, 0}, -15.0, -5.0, 0.0F, tmp_result));

	//Similar, but more random test
	EXPECT_EQ(true, gm::hor_segment_line_intersection(3.5F, {0, 1}, 0.0F, 2.0F, 5.0F, tmp_result));
	EXPECT_NEAR_V2(sf::Vector2f(1.14286F, 5.0F), tmp_result, 0.00001F);

	//Same, but with other line segment points
	EXPECT_EQ(false, gm::hor_segment_line_intersection(3.5F, {0, 1}, -2.0F, 0.0F, 5.0F, tmp_result));

	//Some crazy tests with infinity
	EXPECT_EQ(true, gm::hor_segment_line_intersection(INF_F, {0, 0}, -1.0F, 1.0F, 0.0F, tmp_result));
	EXPECT_EQ_V2(sf::Vector2f(0.0F, 0.0F), tmp_result);

	EXPECT_EQ(true, gm::hor_segment_line_intersection(INF_F, {0, 0}, -1.0F, 1.0F, 5.0F, tmp_result));
	EXPECT_EQ_V2(sf::Vector2f(0.0F, 5.0F), tmp_result);
}

TEST(lines_intersection, vertical) {
	EXPECT_EQ(0, 0);
}
