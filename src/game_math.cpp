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

float gm::rect_distance(sf::FloatRect rect, sf::Vector2f point) {
	float distance_x = std::max({rect.left - point.x, 0.0F, point.x - rect.left - rect.width});
	float distance_y = std::max({rect.top - point.y, 0.0F, point.y - rect.top - rect.height});
	return std::hypot(distance_x, distance_y);
}

void gm::move_rect(sf::FloatRect* rect, sf::Vector2f rel_pos) {
	rect->left += rel_pos.x;
	rect->top += rel_pos.y;
}

bool gm::is_between(float number, float number_1, float number_2) {
	if (number_1 > number_2)
		std::swap(number_1, number_2);

	return number >= number_1 && number <= number_2;
}

bool gm::is_between_v2(sf::Vector2f point, sf::Vector2f point_1, sf::Vector2f point_2) {
	return is_between(point.x, point_1.x, point_2.x) && is_between(point.y, point_1.y, point_2.y);
}

unsigned char gm::quadrant(sf::Vector2f center, sf::Vector2f point) {
	sf::Vector2f local_point = point - center;
	if (local_point.x >= 0 && local_point.y >= 0)
		return 1;
	if (local_point.x <= 0 && local_point.y >= 0)
		return 2;
	if (local_point.x <= 0 && local_point.y <= 0)
		return 3;
	if (local_point.x >= 0 && local_point.y <= 0)
		return 4;
	else
		return 0;
}

bool gm::is_higher_semiplane(float line_k, float line_b, sf::Vector2f point) {
	//If specified point is higher than line point on the same Y, then it is on the higher semiplane
	const float line_y = line_k + point.x + line_b; //y=kx+b
	return line_y > point.y;
}

float gm::line_b_from_point(float line_k, sf::Vector2f line_point) {
	return line_point.y - line_k * line_point.x;
}

float gm::line_k_from_points(sf::Vector2f point_1, sf::Vector2f point_2) {
	float delta_x = point_2.x - point_1.x;
	float delta_y = point_2.y - point_1.y;
	return delta_y / delta_x;
}

float gm::random_number(float min, float max) {
	if (!randomizer_initialized) { //Initialize randomizer if it is not initialized yet
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

bool gm::ver_segment_line_intersection(float line_k, float line_b,
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

	//Compute the intersection point of the lines, ignoring that there is line segment, but not the line
	const float raw_intersection_y = line_k * line_seg_x + line_b;

	//Consider that there is line segment
	if (raw_intersection_y < line_seg_y_1 || raw_intersection_y > line_seg_y_2)
		return false; //There is no intersection

	//If we reached this point, raw intersection point is correct!
	intersection_point = { line_seg_x, raw_intersection_y };
	return true;
}

bool gm::hor_segment_line_intersection(float line_k, float line_b,
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

	//Compute the intersection point of the lines, ignoring that there is line segment, but not the line
	const float raw_intersection_x = (line_seg_y - line_b) / line_k;

	//Consider that there is line segment
	if (raw_intersection_x < line_seg_x_1 || raw_intersection_x > line_seg_x_2)
		return false; //There is no intersection

	//If we reached this point, raw intersection point is correct!
	intersection_point = { raw_intersection_x, line_seg_y };
	return true;
}

unsigned char gm::circle_line_intersection(sf::Vector2f circle_pos, float radius, float line_k,
										   float line_b,
										   sf::Vector2f& point_1, sf::Vector2f& point_2) {
	//Compute the t (temp) variable
	const float t = radius * radius * (1 + line_k * line_k) -
		(circle_pos.y - line_k * circle_pos.x - line_b) *
		(circle_pos.y - line_k * circle_pos.x - line_b);

	if (t < 0) //We will have to take the square root of t
		return 0; //So, here is no intersection

	//By formula
	point_1.x = (circle_pos.x + circle_pos.y * line_k - line_b * line_k + std::sqrt(t)) /
		(1 + line_k * line_k);

	point_2.x = (circle_pos.x + circle_pos.y * line_k - line_b * line_k - std::sqrt(t)) /
		(1 + line_k * line_k);

	point_1.y = (line_b + circle_pos.x * line_k + circle_pos.y * line_k * line_k +
				 line_k * std::sqrt(t)) /
		(1 + line_k * line_k);

	point_2.y = (line_b + circle_pos.x * line_k + circle_pos.y * line_k * line_k -
				 line_k * std::sqrt(t)) /
		(1 + line_k * line_k);

	//If points coincide
	if (point_1 == point_2)
		return 1;

	return 2;
}

unsigned char gm::rounded_rect_line_intersection(float line_k, float line_b,
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
	if (hor_segment_line_intersection(line_k, line_b, base_rect.left,
									  base_rect.left + base_rect.width, base_rect.top - radius,
									  tmp_points[0])) {
		intersection_points.push_back(tmp_points[0]);
	}
	//2
	if (ver_segment_line_intersection(line_k, line_b, base_rect.top,
		base_rect.top + base_rect.height, base_rect.left + base_rect.width + radius,
									  tmp_points[0])) {
		intersection_points.push_back(tmp_points[0]);
	}
	//3
	if (hor_segment_line_intersection(line_k, line_b, base_rect.left,
		base_rect.left + base_rect.width, base_rect.top + base_rect.height + radius,
									  tmp_points[0])) {
		intersection_points.push_back(tmp_points[0]);
	}
	//4
	if (ver_segment_line_intersection(line_k, line_b, base_rect.top,
		base_rect.top + base_rect.height, base_rect.left - radius,
									  tmp_points[0])) {
		intersection_points.push_back(tmp_points[0]);
	}

	//Handle the rounded corners (5, 6, 7, 8)
	unsigned char tmp_amount;
	sf::Vector2f tmp_center = {base_rect.left, base_rect.top};
	//5
	tmp_amount = circle_line_intersection(tmp_center, radius, line_k, line_b,
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
										  line_k, line_b,
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
										  radius, line_k, line_b,
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
										  radius, line_k, line_b,
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

bool gm::rounded_rect_contains(sf::FloatRect base_rect, float radius, sf::Vector2f point) {
	return rect_distance(base_rect, point) <= radius;
}

unsigned char gm::rounded_rect_segment_contains(sf::FloatRect base_rect, float radius, sf::Vector2f point) {
	//           +=========+           +=================+
	//        ---|         |---        ||\             /||
	//       - 5 |         | 6 -       || \           / ||
	//     ||----+    1    +----||     ||  \   11    /  ||
	//     ||     \       /     ||     ||   \       / <- Second diagonal
	//     ||      \     /      ||     ||    \     /    ||
	//     ||       \   /       ||     ||     \   /     ||
	//     ||        \ /        ||     ||      \ /      ||
	//     ||    4    *    2    ||     ||  01   *   10  ||
	//     ||        / \        ||     ||      / \      ||
	//     ||       /   \       ||     ||     /   \     ||
	//     ||      /     \      ||     ||    /     \    ||
	//     ||     /       \     ||     ||   /       \ <- First diagonal
	//     ||----+    3    +----||     ||  /   00    \  ||
	//       - 8 |         | 7 -       || /           \ ||
	//        ---|         |---        ||/             \||
	//           +=========+           +=================+
	//Check if our rounded rect overall contains point
	//DO NOT REMOVE THIS PART! It is necessary for 1, 2, 3, 4
	if (!rounded_rect_contains(base_rect, radius, point))
		return 0;

	//Check corners first (5, 6, 7, 8)
	//5
	if (distance({base_rect.left, base_rect.top}, point) <= radius &&
		quadrant({base_rect.left, base_rect.top}, point) == 3) {
		return 5;
	}
	//6
	if (distance({base_rect.left + base_rect.width, base_rect.top}, point) <= radius &&
		quadrant({base_rect.left + base_rect.width, base_rect.top}, point) == 4) {
		return 6;
	}
	//7
	if (distance({base_rect.left + base_rect.width, base_rect.top + base_rect.height}, point) <= radius &&
		quadrant({base_rect.left + base_rect.width, base_rect.top + base_rect.height}, point) == 1) {
		return 7;
	}
	//8
	if (distance({base_rect.left, base_rect.top + base_rect.height}, point) <= radius &&
		quadrant({base_rect.left, base_rect.top + base_rect.height}, point) == 2) {
		return 8;
	}

	//Check diagonal segments (1, 2, 3, 4)
	//Compute line equation parameters for diagonals
	float k_1 = line_k_from_points({base_rect.left, base_rect.top},
											   {base_rect.left + base_rect.width, base_rect.top + base_rect.height});
	float k_2 = line_k_from_points({base_rect.left + base_rect.width, base_rect.top},
											   {base_rect.left, base_rect.top + base_rect.height});
	float b_1 = line_b_from_point(k_1, {base_rect.left, base_rect.top});
	float b_2 = line_b_from_point(k_2, {base_rect.left, base_rect.top + base_rect.height});
	//Check if our point is higher than every diagonal
	bool higher_1 = is_higher_semiplane(k_1, b_1, point);
	bool higher_2 = is_higher_semiplane(k_2, b_2, point);
	//Do conclusions
	if (higher_1 && higher_2)
		return 1;
	else if (!higher_1 && higher_2)
		return 4;
	else if (higher_1 && !higher_2)
		return 2;
	else
		return 3;
}
