#include <iostream>
#include "stack.h"

using namespace std;

void test_assert(
	const char *file,
	const int line,
	const char *exp) 
{
	cerr << "CHECK FAILED " << file << ":" << line << ": " << exp << endl;
}

void test_unreach(
	const char *file,
	const int line) 
{
	cerr << "UNREACH FAILED " << file << ":" << line << ": " << endl;
}

#define TEST_CHECK(expr) \
	if (!(expr)) \
		test_assert(__FILE__, __LINE__, #expr);
	
#define TEST_UNREACH \
	test_unreach(__FILE__, __LINE__);

void test1() {
	Stack<int> s;
	s.push(10);
	TEST_CHECK(s.top() == 10);
	s.push(20);
	TEST_CHECK(s.size() == 2);
	TEST_CHECK(s.capacity() >= 2);
	TEST_CHECK(s.top() == 20);
	s.push(1);
	TEST_CHECK(s.top() == 1);
	s.push(2);
	s.push(10);
	s.pop();
	TEST_CHECK(s.top() == 2);
	TEST_CHECK(s.size() == 4);
	s.pop();
	TEST_CHECK(s.top() == 1);
	TEST_CHECK(s.ok());
	s.pop();
	TEST_CHECK(s.size() == 2);
	TEST_CHECK(s.top() == 20);
	s.pop();
	TEST_CHECK(s.top() == 10);
	TEST_CHECK(s.size() == 1);
	s.pop();
	TEST_CHECK(s.size() == 0);
	TEST_CHECK(s.ok());
}

void test2() {
	Stack<double> s;
	try {
		s.pop();
		TEST_UNREACH;
	} catch (const StackEmpty& e) {
	}
	
	s.push(20.0);
		s.pop();
		s.push(30.0);
		TEST_CHECK(s.size() == 1);
		TEST_CHECK(s.ok());
		TEST_CHECK(s.top() == 30.0);
		s.pop();
	try {
		s.top();
		TEST_UNREACH;
	} catch (const StackEmpty& e) {
	}
}

void test3() {
	Stack<int> s;
	for (int i = 0; i != 1000000; ++i) {
		TEST_CHECK(s.ok());
		s.push(i);
	}
	for (int i = 1000000; i != 0; --i) {
		TEST_CHECK(s.top() == i - 1);
		TEST_CHECK(s.ok());
		s.pop();
	}
	try {
		s.pop();
		TEST_UNREACH;
	} catch (const StackEmpty& e) {
	} 
}

int main() {
	test1();
	test2();
	test3();
}
