/*
 * PongX game math header
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

///Game math namespace
namespace gm {
	///Calculate distance between 2 points
	float distance(sf::Vector2f point_1, sf::Vector2f point_2);

	///Add current rect position to specified position
	void move_rect(sf::FloatRect* rect, sf::Vector2f rel_pos);

	///Generate random number in the range [min_1;max_1]&[min_2;max_2)
	float random_number_double_range(const float min_1, const float max_1,
									 const float min_2, const float max_2);

	///Is specified vertical line intersects with a specified circle?
	bool intersects_with_vertical_line(float line_1_y, float line_2_y, float line_x,
									   sf::Vector2f circle_pos, float radius);

	///Is specified horizontal line intersects with a specified circle?
	bool intersects_with_horizontal_line(float line_1_x, float line_2_x, float line_y,
										 sf::Vector2f circle_pos, float radius);

	///Get the intersection point of the specified vertical line segment and line
	///@param line_tangent tangent (k coefficient) of the line. k = tan(angle)
	///@param line_point random point on a line
	///@param line_seg_y_1 Y coordinate of the first point of the line segment
	///@param line_seg_y_2 Y coordinate of the second point of the line segment
	///@param line_seg_x X coordinate of the vertical line segment
	///@param intersection_point the result: intersection point (reference)
	///@returns does the specified line segment intersects with the specified line?
	static bool ver_segment_and_line_intersection(float line_tangent, sf::Vector2f line_point,
												  float line_seg_y_1, float line_seg_y_2,
												  float line_seg_x, sf::Vector2f& intersection_point);

	///Get the intersection point of specified horizontal line segment and line
	///@param line_tangent tangent (k coefficient) of the line. k = tan(angle)
	///@param line_point random point on a line
	///@param line_seg_x_1 X coordinate of the first point of the line segment
	///@param line_seg_x_2 X coordinate of the second point of the line segment
	///@param line_seg_y Y coordinate of the horizontal line segment
	///@param intersection_point the result: intersection point (reference)
	///@returns does the specified line segment intersects with the specified line?
	static bool hor_segment_and_line_intersection(float line_tangent, sf::Vector2f line_point,
												  float line_seg_x_1, float line_seg_x_2,
												  float line_seg_y, sf::Vector2f& intersection_point);
}
