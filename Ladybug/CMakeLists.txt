cmake_minimum_required (VERSION 3.0)

project (ladybug)

add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(ladybug
	${HEADER_FILES}
	${SOURCE_FILES}
)

target_include_directories(ladybug PRIVATE include)
