cmake_minimum_required(VERSION 3.0)
project(array)

enable_testing()
find_package(GTest REQUIRED)

add_executable(array_test
	array_test.cpp
	debug_utils.cpp
)

target_compile_options(array_test PUBLIC 
	-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused)

target_compile_options(array_test PUBLIC -g -O0)
target_include_directories(array_test PUBLIC ${GTEST_INCLUDE_DIR})
target_link_libraries(array_test ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

add_test(
	NAME array_test_exec
	COMMAND array_test
)
