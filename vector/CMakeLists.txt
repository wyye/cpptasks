cmake_minimum_required(VERSION 3.0)
project(vector)

enable_testing()
find_package(GTest REQUIRED)

add_executable(vector_test
	vector_test.cpp
	debug_utils.cpp
)

add_executable(vector_bool_test
	vector_bool_test.cpp
	debug_utils.cpp
)



target_compile_options(vector_test PUBLIC 
	-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused)

target_compile_options(vector_test PUBLIC -g)
target_include_directories(vector_test PUBLIC ${GTEST_INCLUDE_DIR})
target_link_libraries(vector_test ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})



target_compile_options(vector_bool_test PUBLIC 
	-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused)

target_compile_options(vector_bool_test PUBLIC -g -DNDEBUG)
target_include_directories(vector_bool_test PUBLIC ${GTEST_INCLUDE_DIR})
target_link_libraries(vector_bool_test ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})





add_test(
	vector_test_exec
	vector_test
)

add_test(
	vector_bool_test_exec
	vector_bool_test
)
