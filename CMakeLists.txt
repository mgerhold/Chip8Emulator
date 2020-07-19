cmake_minimum_required(VERSION 3.10.2)

project(Chip8Emulator VERSION 0.1 LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

set(Chip8Core_SRC
	"include/Chip8Core/Chip8.hpp"
	"include/Chip8Core/Chip8Memory.hpp"
	"include/Chip8Core/Chip8Opcode.hpp"
	"src/Chip8Core/Chip8.cpp"
	"src/Chip8Core/Chip8Memory.cpp"
	"src/Chip8Core/Chip8Opcode.cpp"
)

set(Chip8Renderer_SRC
	"include/Chip8Renderer/Chip8Renderer.hpp"
	"src/Chip8Renderer/Chip8Renderer.cpp"
)

set(ImGui_SRC
	"vendor/imgui/imconfig.h"
    "vendor/imgui/imgui_demo.cpp"
    "vendor/imgui/imgui_draw.cpp"
    "vendor/imgui/examples/imgui_impl_glfw.cpp"
    "vendor/imgui/examples/imgui_impl_glfw.h"
    "vendor/imgui/examples/imgui_impl_opengl3.cpp"
    "vendor/imgui/examples/imgui_impl_opengl3.h"
    "vendor/imgui/imgui_internal.h"
    "vendor/imgui/imgui_widgets.cpp"
    "vendor/imgui/imgui.cpp"
    "vendor/imgui/imgui.h"
    "vendor/imgui/imstb_rectpack.h"
    "vendor/imgui/imstb_textedit.h"
    "vendor/imgui/imstb_truetype.h"
)

set(Chip8Emulator_SRC	
	"src/Chip8Emulator/main.cpp"
)

# set targets
add_library(Chip8Core STATIC ${Chip8Core_SRC})
add_library(ImGui STATIC ${ImGui_SRC})
add_library(Chip8Renderer STATIC ${Chip8Renderer_SRC})
add_executable(Chip8Emulator ${Chip8Emulator_SRC})

# set warning levels
if (MSVC)
	target_compile_options(Chip8Core PUBLIC /W4 /WX)
	target_compile_options(Chip8Renderer PUBLIC /W4 /WX)
	target_compile_options(Chip8Emulator PUBLIC /W4 /WX)
else()
	target_compile_options(Chip8Core PUBLIC -Wall -Wextra -pedantic -Werror)
	target_compile_options(Chip8Renderer PUBLIC -Wall -Wextra -pedantic -Werror)
	target_compile_options(Chip8Emulator PUBLIC -Wall -Wextra -pedantic -Werror)
endif()

# C++17
target_compile_features(Chip8Core PUBLIC cxx_std_17)
target_compile_features(Chip8Renderer PUBLIC cxx_std_17)
target_compile_features(Chip8Emulator PUBLIC cxx_std_17)
# Enable Code Analysis
set_target_properties(Chip8Core PROPERTIES VS_GLOBAL_EnableCppCoreCheck "true")
set_target_properties(Chip8Core PROPERTIES VS_GLOBAL_CodeAnalysisRuleSet "CppCoreCheckRules.ruleset")
set_target_properties(Chip8Core PROPERTIES VS_GLOBAL_RunCodeAnalysis "true")
set_target_properties(Chip8Renderer PROPERTIES VS_GLOBAL_EnableCppCoreCheck "true")
set_target_properties(Chip8Renderer PROPERTIES VS_GLOBAL_CodeAnalysisRuleSet "CppCoreCheckRules.ruleset")
set_target_properties(Chip8Renderer PROPERTIES VS_GLOBAL_RunCodeAnalysis "true")
set_target_properties(Chip8Emulator PROPERTIES VS_GLOBAL_EnableCppCoreCheck "true")
set_target_properties(Chip8Emulator PROPERTIES VS_GLOBAL_CodeAnalysisRuleSet "CppCoreCheckRules.ruleset")
set_target_properties(Chip8Emulator PROPERTIES VS_GLOBAL_RunCodeAnalysis "true")

# include directories
target_include_directories(Chip8Core PUBLIC
	${PROJECT_SOURCE_DIR}/include
)
target_include_directories(Chip8Renderer PUBLIC
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/vendor/imgui
	${PROJECT_SOURCE_DIR}/vendor/imgui/examples
)
target_include_directories(ImGui PUBLIC
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/vendor/imgui
	${PROJECT_SOURCE_DIR}/vendor/imgui/examples
)
target_include_directories(Chip8Emulator PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

# Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Chip8Emulator)

# external libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# link libraries
target_link_libraries(Chip8Emulator PRIVATE
	Chip8Core
	Chip8Renderer
)
target_link_libraries(Chip8Renderer PRIVATE
	glfw
	glad::glad
	glm ImGui
)
target_link_libraries(ImGui PRIVATE
	glfw
	glad::glad
)

enable_testing()
add_subdirectory(test)