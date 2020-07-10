#ifdef _MSC_VER
#pragma warning(disable: 26812)
#endif

#include <gtest/gtest.h>

#ifdef _MSC_VER
#pragma warning(default: 26812)
#endif

TEST(TestSuiteName, ExampleTest) {
	ASSERT_TRUE(true);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}