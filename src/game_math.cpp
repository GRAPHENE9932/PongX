/*
 * PongX game math
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

#include <cmath>

#include "GameManager.hpp"
#include "game_math.hpp"

float gm::distance(sf::Vector2f point_1, sf::Vector2f point_2) {
	return std::sqrt((point_1.x - point_2.x) * (point_1.x - point_2.x) +
		(point_1.y - point_2.y) * (point_1.y - point_2.y));
}

void gm::move_rect(sf::FloatRect* rect, sf::Vector2f rel_pos) {
	rect->left += rel_pos.x;
	rect->top += rel_pos.y;
}

float gm::random_number_double_range(const float min_1, const float max_1,
									 const float min_2, const float max_2) {
	float max = (max_1 - min_1) + (max_2 - min_2); //Prepare for number generation
	float raw_random = GameManager::random_number(0.0F, max); //Generate raw number

	//If in first half of the range
	if (raw_random <= max_1 - min_1)
		return min_1 + raw_random;
	else //If in second half of the range
		return min_2 + raw_random - (max_1 - min_1);
}

bool gm::ver_segment_line_intersection(float line_tangent, sf::Vector2f line_point,
									   float line_seg_y_1, float line_seg_y_2,
									   float line_seg_x, sf::Vector2f& intersection_point) {
	//          ---
	//           |/
	//           *
	//          /|
	//         / |
	//        /  |
	//       /   |
	//      /   ---

	//line_seg_y_2 has to be greater than line_seg_y_1
	if (line_seg_y_1 > line_seg_y_2)
		std::swap(line_seg_y_1, line_seg_y_2);

	//Compute the b variable (y=kx+b)
	const float b = line_point.y - line_tangent * line_point.x;

	//Compute the intersection point of the lines, ignoring that there is line segment, but not the line
	const float raw_intersection_y = line_tangent * line_seg_x + b;

	//Consider that there is line segment
	if (raw_intersection_y < line_seg_y_1 || raw_intersection_y > line_seg_y_2)
		return false; //There is no intersection

	//If we reached this point, raw intersection point is correct!
	intersection_point = { line_seg_x, raw_intersection_y };
	return true;
}

bool gm::hor_segment_line_intersection(float line_tangent, sf::Vector2f line_point,
									   float line_seg_x_1, float line_seg_x_2,
									   float line_seg_y, sf::Vector2f& intersection_point) {
	//                /
	//     |---------*---------|
	//              /
	//             /
	//            /

	//line_seg_y_2 has to be greater than line_seg_y_1
	if (line_seg_x_1 > line_seg_x_2)
		std::swap(line_seg_x_1, line_seg_x_2);

	//Compute the b variable (y=kx+b)
	const float b = line_point.y - line_tangent * line_point.x;

	//Compute the intersection point of the lines, ignoring that there is line segment, but not the line
	const float raw_intersection_x = (line_seg_y - b) / line_tangent;

	//Consider that there is line segment
	if (raw_intersection_x < line_seg_x_1 || raw_intersection_x > line_seg_x_2)
		return false; //There is no intersection

	//If we reached this point, raw intersection point is correct!
	intersection_point = { raw_intersection_x, line_seg_y };
	return true;
}
