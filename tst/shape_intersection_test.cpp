/*
 * PongX shape-line intersection unit test
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

#include "macros.hpp"
#include "../src/game_math.hpp"

bool vector_sorter(const sf::Vector2f& left, const sf::Vector2f& right) {
	//Just compare these structs like byte arrays
	auto cmp_result = memcmp(&left, &right, sizeof(sf::Vector2f));
	return cmp_result > 0;
}

TEST(shape_intersection, circle) {
	sf::Vector2f tmp_result[2];
	sf::Vector2f tmp_true[2];

	//Line y=0 and circle on 0;0, radius 1
	EXPECT_EQ(2, gm::circle_line_intersection({0, 0}, 1.0F, 0.0F, {0, 0}, tmp_result[0], tmp_result[1]));
	EXPECT_EQ_UNORDERED_V2_ARR_2(sf::Vector2f(1.0F, 0.0F), sf::Vector2f(-1.0F, 0.0F),
								 tmp_result);

	//Line y=1 and circle on 0;0, radius 1
	EXPECT_EQ(1, gm::circle_line_intersection({0, 0}, 1.0F, 0.0F, {0, 1}, tmp_result[0], tmp_result[1]));
	EXPECT_EQ_V2(sf::Vector2f(0.0F, 1.0F), tmp_result[0]);

	//Line y=x and circle on 0;0, radius 1
	EXPECT_EQ(2, gm::circle_line_intersection({0, 0}, 1.0F, 1.0F, {0, 0}, tmp_result[0], tmp_result[1]));
	EXPECT_NEAR_UNORDERED_V2_ARR_2(sf::Vector2f(0.70711F, -0.70711F), sf::Vector2f(-0.70711F, 0.70711F),
								   tmp_result, 0.00001F);
}

TEST(shape_intersection, rounded_rectangle) {

}
