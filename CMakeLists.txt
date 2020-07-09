cmake_minimum_required(VERSION 3.10.2)

project(Chip8Emulator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

file(GLOB Chip8Core_SRC
	"include/Chip8Core/*.hpp"
	"include/Chip8Core/*.h"
	"src/Chip8Core/*.cpp"
)

file(GLOB Chip8Renderer_SRC
	"include/Chip8Renderer/*.hpp"
	"include/Chip8Renderer/*.h"
	"src/Chip8Renderer/*.cpp"
)

file(GLOB Chip8Emulator_SRC
	"src/Chip8Emulator/*.hpp"
	"src/Chip8Emulator/*.h"
	"src/Chip8Emulator/*.cpp"
)

add_library(Chip8Core STATIC ${Chip8Core_SRC})
add_library(Chip8Renderer STATIC ${Chip8Renderer_SRC})
add_executable(Chip8Emulator ${Chip8Emulator_SRC})
target_compile_features(Chip8Core PUBLIC cxx_std_17)
target_compile_features(Chip8Renderer PUBLIC cxx_std_17)
target_compile_features(Chip8Emulator PUBLIC cxx_std_17)

target_include_directories(Chip8Core PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(Chip8Renderer PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(Chip8Emulator PUBLIC ${PROJECT_SOURCE_DIR}/include)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Chip8Emulator)
# set_property(TARGET Chip8Emulator PROPERTY  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(Chip8Emulator Chip8Core Chip8Renderer)
target_link_libraries(Chip8Renderer PRIVATE glfw PRIVATE glad::glad PRIVATE glm)
