add_executable(
	Tests
	tests.cpp
)

target_link_libraries(Tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main Chip8Core)
target_include_directories(Tests PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

add_test(
	NAME
		unit
	COMMAND
		Tests
)