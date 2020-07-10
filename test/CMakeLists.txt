add_executable(
	Tests
	tests.cpp
)

target_link_libraries(Tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(
	NAME
		unit
	COMMAND
		Tests
)