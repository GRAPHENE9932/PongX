//Macros for sf::Vector
#define EXPECT_EQ_V2(exp, orig) \
	EXPECT_EQ(exp.x, orig.x); \
	EXPECT_EQ(exp.y, orig.y);
#define EXPECT_NEAR_V2(exp, orig, abs_error) \
	EXPECT_NEAR(exp.x, orig.x, abs_error); \
	EXPECT_NEAR(exp.y, orig.y, abs_error);
