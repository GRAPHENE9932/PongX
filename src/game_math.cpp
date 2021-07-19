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

#include "game_math.hpp"

///Thing that takes seed and produces random numbers
std::mt19937 randomizer;
///Is randomizer already initialized?
bool randomizer_initialized;

float gm::distance(sf::Vector2f point_1, sf::Vector2f point_2) {
	return std::sqrt((point_1.x - point_2.x) * (point_1.x - point_2.x) +
		(point_1.y - point_2.y) * (point_1.y - point_2.y));
}

void gm::move_rect(sf::FloatRect* rect, sf::Vector2f rel_pos) {
	rect->left += rel_pos.x;
	rect->top += rel_pos.y;
}

float gm::random_number(float min, float max) {
	if (!randomizer_initialized) { //Initialize randomizer if it is not
		std::random_device true_gen; //Get the true random number which used for seed
		randomizer = std::mt19937(true_gen()); //Seed and initialize our randomizer
	}
	//This thing handles raw number from randomizer and turns it into the float in range [min;max)
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(randomizer); //Use distribution and randomizer to generate the number
}

float gm::random_number_double_range(const float min_1, const float max_1,
									 const float min_2, const float max_2) {
	float max = (max_1 - min_1) + (max_2 - min_2); //Prepare for number generation
	float raw_random = random_number(0.0F, max); //Generate raw number

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

unsigned char gm::circle_line_intersection(sf::Vector2f circle_pos, float radius, float line_tangent,
										   sf::Vector2f line_point,
										   sf::Vector2f& point_1, sf::Vector2f& point_2) {

	//Compute the b (d) variable (y=kx+b)
	const float d = line_point.y - line_tangent * line_point.x;

	//Compute the t (temp) variable
	const float t = radius * radius * (1 + line_tangent * line_tangent) -
		(circle_pos.y - line_tangent * circle_pos.x - d) * (circle_pos.y - line_tangent * circle_pos.x - d);

	if (t < 0) //We will have to take the square root of t
		return 0; //So, here is no intersection

	//By formula
	point_1.x = (circle_pos.x + circle_pos.y * line_tangent - d * line_tangent + std::sqrt(t)) /
		(1 + line_tangent * line_tangent);

	point_2.x = (circle_pos.x + circle_pos.y * line_tangent - d * line_tangent - std::sqrt(t)) /
		(1 + line_tangent * line_tangent);

	point_1.y = (d + circle_pos.x * line_tangent + circle_pos.y * line_tangent * line_tangent +
		line_tangent * std::sqrt(t)) /
		(1 + line_tangent * line_tangent);

	point_2.y = (d + circle_pos.x * line_tangent + circle_pos.y * line_tangent * line_tangent -
		line_tangent * std::sqrt(t)) /
		(1 + line_tangent * line_tangent);

	//If points coincide
	if (point_1 == point_2)
		return 1;

	return 2;
}

unsigned char gm::rounded_rect_line_intersection(float line_tangent, sf::Vector2f line_point,
												 sf::FloatRect base_rect, float radius,
												 sf::Vector2f& point_1, sf::Vector2f& point_2) {
	//           +===================+
	//        ---|                   |---
	//       - 5 |         1         | 6 -
	//     ||----|-------------------|----||
	//     ||    |                   |    ||
	//     ||    |                   |    ||
	//     ||    |                   |    ||
	//     ||    |                   |    ||
	//     || 4  |     Base rect     |  2 ||
	//     ||    |                   |    ||
	//     ||    |                   |    ||
	//     ||    |                   |    ||
	//     ||    |                   |    ||
	//     ||----|-------------------|----||
	//       - 8 |                   | 7 -
	//        ---|         3         |---
	//           +===================+
	std::vector<sf::Vector2f> intersection_points;
	sf::Vector2f tmp_points[2];
	//Handle the straigh sides (1, 2, 3, 4)
	//1
	if (hor_segment_line_intersection(line_tangent, line_point, base_rect.left,
									  base_rect.left + base_rect.width, base_rect.top - radius,
									  tmp_points[0])) {
		intersection_points.push_back(tmp_points[0]);
	}
	//2
	if (ver_segment_line_intersection(line_tangent, line_point, base_rect.top,
		base_rect.top + base_rect.height, base_rect.left + base_rect.width + radius,
									  tmp_points[0])) {
		intersection_points.push_back(tmp_points[0]);
	}
	//3
	if (hor_segment_line_intersection(line_tangent, line_point, base_rect.left,
		base_rect.left + base_rect.width, base_rect.top + base_rect.height + radius,
									  tmp_points[0])) {
		intersection_points.push_back(tmp_points[0]);
	}
	//4
	if (ver_segment_line_intersection(line_tangent, line_point, base_rect.top,
		base_rect.top + base_rect.height, base_rect.left - radius,
									  tmp_points[0])) {
		intersection_points.push_back(tmp_points[0]);
	}

	//Handle the rounded corners (5, 6, 7, 8)
	unsigned char tmp_amount;
	sf::Vector2f tmp_center = {base_rect.left, base_rect.top};
	//5
	tmp_amount = circle_line_intersection(tmp_center, radius, line_tangent, line_point,
										  tmp_points[0], tmp_points[1]);
	switch (tmp_amount) {
		case 2: {
			if (tmp_points[1].x < tmp_center.x && tmp_points[1].y < tmp_center.y)
				intersection_points.push_back(tmp_points[1]);
		}
		case 1: {
			if (tmp_points[0].x < tmp_center.x && tmp_points[0].y < tmp_center.y)
				intersection_points.push_back(tmp_points[0]);
			break;
		}
	}
	//6
	tmp_center = {base_rect.left + base_rect.left, base_rect.top};
	tmp_amount = circle_line_intersection(tmp_center, radius,
										  line_tangent, line_point,
										  tmp_points[0], tmp_points[1]);
	switch (tmp_amount) {
		case 2: {
			if (tmp_points[1].x > tmp_center.x && tmp_points[1].y < tmp_center.y)
				intersection_points.push_back(tmp_points[1]);
		}
		case 1: {
			if (tmp_points[1].x > tmp_center.x && tmp_points[1].y < tmp_center.y)
				intersection_points.push_back(tmp_points[0]);
			break;
		}
	}
	//7
	tmp_center = {base_rect.left + base_rect.width, base_rect.top + base_rect.height};
	tmp_amount = circle_line_intersection(tmp_center,
										  radius, line_tangent, line_point,
										  tmp_points[0], tmp_points[1]);
	switch (tmp_amount) {
		case 2: {
			if (tmp_points[1].x > tmp_center.x && tmp_points[1].y > tmp_center.y)
				intersection_points.push_back(tmp_points[1]);
		}
		case 1: {
			if (tmp_points[0].x > tmp_center.x && tmp_points[0].y > tmp_center.y)
				intersection_points.push_back(tmp_points[0]);
			break;
		}
	}
	//8
	tmp_center = {base_rect.left, base_rect.top + base_rect.height};
	tmp_amount = circle_line_intersection({base_rect.left, base_rect.top + base_rect.height},
										  radius, line_tangent, line_point,
										  tmp_points[0], tmp_points[1]);
	switch (tmp_amount) {
		case 2: {
			if (tmp_points[1].x < tmp_center.x && tmp_points[1].y > tmp_center.y)
				intersection_points.push_back(tmp_points[1]);
		}
		case 1: {
			if (tmp_points[0].x < tmp_center.x && tmp_points[0].y > tmp_center.y)
				intersection_points.push_back(tmp_points[0]);
			break;
		}
	}

	//Return result
	switch (intersection_points.size()) {
		case 2: {
			point_2 = intersection_points[1];
		}
		case 1: {
			point_1 = intersection_points[0];
			break;
		}
	}
	return intersection_points.size();
}
