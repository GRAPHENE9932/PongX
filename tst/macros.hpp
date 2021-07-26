#pragma once

#include <SFML/Graphics.hpp>

namespace utils {
	//Macros for sf::Vector
	#define EXPECT_EQ_V2(exp, orig) \
		EXPECT_EQ(exp.x, orig.x); \
		EXPECT_EQ(exp.y, orig.y);
	#define EXPECT_NEAR_V2(exp, orig, abs_error) \
		EXPECT_NEAR(exp.x, orig.x, abs_error); \
		EXPECT_NEAR(exp.y, orig.y, abs_error);

	#define EXPECT_EQ_UNORDERED_V2_ARR_2(exp_1, exp_2, orig) \
		EXPECT_EQ_V2(exp_1, orig[0]); \
		EXPECT_EQ_V2(exp_2, orig[1]);

	#define EXPECT_NEAR_UNORDERED_V2_ARR_2(exp_1, exp_2, orig, abs_error) \
		EXPECT_NEAR_V2(exp_1, orig[0], abs_error); \
		EXPECT_NEAR_V2(exp_2, orig[1], abs_error);
}
