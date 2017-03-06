#include <gtest/gtest.h>
#include "stack.h"

using namespace std;

TEST(Stack, SimplePushPop) {
	Stack<int> s;
	s.push(10);
	ASSERT_EQ(s.top(), 10);
	s.push(20);
	ASSERT_EQ(s.size(), 2);
	ASSERT_GE(s.capacity(), 2);
	ASSERT_EQ(s.top(), 20);
	s.push(1);
	ASSERT_EQ(s.top(), 1);
	s.push(2);
	s.push(10);
	s.pop();
	ASSERT_EQ(s.top(), 2);
	ASSERT_EQ(s.size(), 4);
	s.pop();
	ASSERT_EQ(s.top(), 1);
	ASSERT_TRUE(s.ok());
	s.pop();
	ASSERT_EQ(s.size(), 2);
	ASSERT_EQ(s.top(), 20);
	s.pop();
	ASSERT_EQ(s.top(), 10);
	ASSERT_EQ(s.size(), 1);
	s.pop();
	ASSERT_EQ(s.size(), 0);
	ASSERT_TRUE(s.ok());
}

TEST(Stack, TwoStacks) {
	Stack<double> s;
	Stack<double> s2;
	
	s.push(20.0);
	s.pop();
	s.push(30.0);
	s2 = s;
	ASSERT_EQ(s.size(), 1);
	ASSERT_TRUE(s.ok());
	ASSERT_EQ(s.top(), 30.0);
	s.pop();
	
	ASSERT_EQ(s2.size(), 1);
}

TEST(Stack, LargeStack) {
	Stack<int> s;
	for (int i = 0; i != 1000000; ++i) {
		ASSERT_TRUE(s.ok());
		s.push(i);
	}
	for (int i = 1000000; i != 0; --i) {
		ASSERT_EQ(s.top(), i - 1);
		ASSERT_TRUE(s.ok());
		s.pop();
	}
	ASSERT_EQ(s.size(), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
