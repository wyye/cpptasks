#include "array.h"
#include "gtest/gtest.h"

TEST(Array, construct) {
	Array<int, 3> a = {1, 2, 3};
	Array<int, 3> b = {4, 5, 6};
	Array<int, 3> c = a;
	Array<int, 3> d = b;
	std::swap(a, b);
	ASSERT_EQ(a, d);
	ASSERT_EQ(b, c);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
