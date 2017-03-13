#include "vector.h"
#include "gtest/gtest.h"

TEST(Vector, construct) {
	Vector<double> v1;
	Vector<double> v2(10);
	Vector<double> v3(15);
	Vector<double> v4 = {5.0, 6.0, 7.0, 8.0};
	v1 = {1.0, 2.0, 3.0, 4.5, 5.2};
	Vector<double> v5(v1);
	Vector<double> v6(std::move(v2));
	Vector<double> v7 = v3;
	Vector<double> v8 = std::move(v4);
	ASSERT_EQ(v5.size(), 5);
	ASSERT_EQ(v6.size(), 10);
	ASSERT_EQ(v7.size(), 15);
	ASSERT_EQ(v8.size(), 4);
	ASSERT_EQ(v2.size(), 0);
	ASSERT_EQ(v4.size(), 0);
}

TEST(Vector, resize) {
	Vector<double> v1 = {7, 6, 5};
	v1.reserve(7);
	v1.resize(5);
	v1.shrink_to_fit();
}

TEST(Vector, insert) {
	Vector<double> v2(2);
	v2[1] = 10;
	v2.at(0) = 15;
	ASSERT_EQ(v2[0], 15);
	ASSERT_EQ(v2[1], 10);
	v2.insert(v2.begin()+1, {4, 5, 6, 7});
	ASSERT_EQ(v2.size(), 6);
}

TEST(Vector, emplace) {
	Vector<int> v;
	v.reserve(7);
	v.resize(4);
	v.emplace(v.begin()+2, 17);
	v.shrink_to_fit();
	v.emplace_back(18);
	ASSERT_EQ(v[2], 17);
	ASSERT_EQ(v[5], 18);
}

TEST(Vector, PushPop) {
	Vector<int> v;
	for (int i = 0; i != 3; ++i) {
		v.push_back(i);
	}
	for (int i = 0; i != 3; ++i) {
		v.pop_back();
	}
	ASSERT_EQ(v.size(), 0);
}

TEST(Vector, EmptyInsert) {
	Vector<int> v(10);
	v.clear();
	v.shrink_to_fit();
	v.insert(v.begin(), {1, 2, 3, 4, 5});
	ASSERT_EQ(v.size(), 5);
	ASSERT_EQ(v.at(3), 4);
	v.clear();
	v.emplace(v.begin(), 10);
}

TEST(Vector, Erase) {
	Vector<int> v = {1, 2, 3, 4, 5, 6};
	v.erase(v.data()+1, v.data()+3);
	ASSERT_EQ(v.size(), 4);
	ASSERT_EQ(v[0], 1);
	ASSERT_EQ(v[1], 4);
	Vector<int> v2;
	std::swap(v, v2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
