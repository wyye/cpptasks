#include "vector.h"
#include "gtest/gtest.h"

TEST(VectorBool, Create) {
	Vector<bool> a = {false, true, false};
	ASSERT_EQ(a[1], true);
	ASSERT_EQ(a.size(), 3);
}

TEST(VectorBool, Constructors) {
	Vector<bool> v1;
	Vector<bool> v2(10);
	Vector<bool> v3(15);
	Vector<bool> v4 = {true, false, true, true};
	v1 = {false, false, 1, true, false};
	Vector<bool> v5(v1);
	Vector<bool> v6(std::move(v2));
	Vector<bool> v7 = v3;
	Vector<bool> v8 = std::move(v4);
	ASSERT_EQ(v5.size(), 5);
	ASSERT_EQ(v6.size(), 10);
	ASSERT_EQ(v7.size(), 15);
	ASSERT_EQ(v8.size(), 4);
	ASSERT_EQ(v2.size(), 0);
	ASSERT_EQ(v4.size(), 0);
}

TEST(VectorBool, resize) {
	Vector<bool> v1{true, true, false};
	v1.reserve(70);
	ASSERT_GE(v1.capacity(), 70);
	v1.resize(50);
	v1.shrink_to_fit();
	ASSERT_EQ(v1.size(), 50);
}

TEST(Vector, insert) {
	Vector<bool> v2(2);
	v2[1] = true;
	v2.at(0) = false;
	ASSERT_EQ(v2[0], false);
	ASSERT_EQ(v2[1], true);
	v2.insert(v2.begin()+1, {true, false, false, true});
	ASSERT_EQ(v2.size(), 6);
}

TEST(Vector, insert_count_val) {
	Vector<bool> v2(2);
	v2[1] = true;
	v2.at(0) = false;
	ASSERT_EQ(v2[0], false);
	ASSERT_EQ(v2[1], true);
	v2.insert(v2.begin()+2, 3, true);
	v2.insert(v2.begin()+4, 7, false);
	ASSERT_EQ(v2, Vector<bool>({0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1}));
	ASSERT_EQ(v2.size(), 12);
}

TEST(Vector, insert_two_iterators) {
	Vector<bool> v2{1, 0, 1, 0, 1};
	Vector<int> v1 = {1, 0, 1, 1, 1, 1, 0, 1};
	v2.insert(v2.begin()+2, v1.begin(), v1.end());
	ASSERT_EQ(v2, Vector<bool>({1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1}));
	ASSERT_EQ(v2.size(), 13);
}

TEST(Vector, emplace) {
	Vector<bool> v;
	v.reserve(17);
	v.resize(14);
	v.emplace(v.begin()+2, true);
	v.shrink_to_fit();
	v.emplace_back(true);
	ASSERT_EQ(v[2], true);
	ASSERT_EQ(v[15], true);
}

TEST(Vector, PushPop) {
	Vector<bool> v;
	for (int i = 0; i != 13; ++i) {
		v.push_back(i % 2);
	}
	for (int i = 0; i != 13; ++i) {
		v.pop_back();
	}
	ASSERT_EQ(v.size(), 0);
}

TEST(Vector, EmptyInsert) {
	Vector<bool> v(10);
	v.clear();
	v.shrink_to_fit();
	v.insert(v.begin(), {1, false, 0, 1, 0});
	ASSERT_EQ(v.size(), 5);
	ASSERT_EQ(v.at(3), 1);
	v.clear();
	v.emplace(v.begin(), 1);
}

TEST(Vector, Erase) {
	Vector<bool> v = {1, 0, 1, 0, 1, 0};
	v.erase(v.begin()+1, v.begin()+3);
	ASSERT_EQ(v.size(), 4);
	ASSERT_EQ(v[0], 1);
	ASSERT_EQ(v[1], 0);
}

TEST(Vector, print) {
	Vector<bool> v = {1, 0, 1, 0, 1, 0};
	for (auto it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << " ";
		it->flip();
	}
	std::cout << std::endl;
	v[3] = v[4];
	for (auto it = v.cbegin(); it != v.cend(); ++it) {
// 		Vector<bool>::iterator it2 = it;
// 		it2->flip();
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
