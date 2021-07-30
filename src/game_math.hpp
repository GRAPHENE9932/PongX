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

#include <random>

#include <SFML/Graphics.hpp>

///Game math namespace
namespace gm {
	///Calculate distance between 2 points
	float distance(sf::Vector2f point_1, sf::Vector2f point_2);

	///Calculate distance between rect and point
	float rect_distance(sf::FloatRect rect, sf::Vector2f point);

	///Add current rect position to specified position
	void move_rect(sf::FloatRect* rect, sf::Vector2f rel_pos);

	///Check if given number is between specified 2 numbers
	bool is_between(float number, float number_1, float number_2);

	///Check if given point is between specified 2 points
	///It is like create the rectangle from specified 2 points and check if given point
	///intersects with this rectangle
	bool is_between_v2(sf::Vector2f point, sf::Vector2f point_1, sf::Vector2f point_2);

	///Get quadrant number of point where center is (0, 0) of coordinate plane
	///@returns quadrant number (1 - bottom right, 2 - bottom left, 3 - top left, 4 - top right)
	unsigned char quadrant(sf::Vector2f center, sf::Vector2f point);

	///Which semiplane specified point line
	///@return true if higher semiplane, false if not
	bool is_higher_semiplane(float line_tangent, sf::Vector2f line_point, sf::Vector2f point);

	///Generates a random number in range [min;max)
	float random_number(float min, float max);

	///Generate random number in the range [min_1;max_1]&[min_2;max_2)
	float random_number_double_range(const float min_1, const float max_1,
									 const float min_2, const float max_2);

	///Get the intersection point of the specified vertical line segment and line
	///@param line_tangent tangent (k coefficient) of the line. k = tan(angle)
	///@param line_point random point on line
	///@param line_seg_y_1 Y coordinate of the first point of the line segment
	///@param line_seg_y_2 Y coordinate of the second point of the line segment
	///@param line_seg_x X coordinate of the vertical line segment
	///@param intersection_point the result: intersection point (reference)
	///@returns does vertical line segment intersects with line?
	bool ver_segment_line_intersection(float line_tangent, sf::Vector2f line_point,
									   float line_seg_y_1, float line_seg_y_2,
									   float line_seg_x, sf::Vector2f& intersection_point);

	///Compute the intersection point of specified horizontal line segment and line
	///@param line_tangent tangent (k coefficient) of the line. k = tan(angle)
	///@param line_point random point on line
	///@param line_seg_x_1 X coordinate of the first point of the line segment
	///@param line_seg_x_2 X coordinate of the second point of the line segment
	///@param line_seg_y Y coordinate of the horizontal line segment
	///@param intersection_point the result: intersection point (reference)
	///@returns does horizontal line segment intersects with line?
	bool hor_segment_line_intersection(float line_tangent, sf::Vector2f line_point,
									   float line_seg_x_1, float line_seg_x_2,
									   float line_seg_y, sf::Vector2f& intersection_point);

	///Compute the intersection points of specified circle and line
	///@param circle_pos position of circle
	///@param radius radius of circle
	///@param line_tangent tangent (k coefficient) of the line. k = tan(angle)
	///@param line_point random point on line
	///@param point_1 first intersection point (reference)
	///@param point_2 second intersection point (reference)
	///@returns amount of intersection points
	unsigned char circle_line_intersection(sf::Vector2f circle_pos, float radius, float line_tangent,
										   sf::Vector2f line_point,
										   sf::Vector2f& point_1, sf::Vector2f& point_2);

	///Compute the intersection points of specified rounded rectangle and line
	///@param line_tangent tangent (k coefficient) of the line. k = tan(angle)
	///@param line_point random point on line
	///@param base_rect base rect of rounded rect
	///@param radius radius of rounded corners
	///@param point_1 first intersection point (reference)
	///@param point_2 second intersection point (reference)
	///@returns amount of intersection points
	unsigned char rounded_rect_line_intersection(float line_tangent, sf::Vector2f line_point,
												 sf::FloatRect base_rect, float radius,
												 sf::Vector2f& point_1, sf::Vector2f& point_2);

	///Is rounded rect contains specified point?
	bool rounded_rect_contains(sf::FloatRect base_rect, float radius, sf::Vector2f point);

	///Discover rounded rect segment number which contains specified point
	///@returns segment number (0 if point is outside the rounded rectangle)
	///0 - no intersection, 1 - top, 2 - right, 3 - bottom, 4 - left,
	///5 - left top corner, 6 - right top, 7 - right bottom, 8 - left bottom
	unsigned char rounded_rect_segment_contains(sf::FloatRect base_rect, float radius, sf::Vector2f point);
}
