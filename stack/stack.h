#ifndef STACK_H
#define STACK_H

#include "../vector/vector.h"

#include <iostream>
#include <cstring>
#include <string>
#include <limits>

template <
	typename T,
	typename Container = Vector<T> >
class Stack {
public:
	
	typedef T value_type;
	typedef size_t size_type;
	typedef T& reference;
	typedef const T& const_reference;
	
	Stack()
	{}
	
	Stack(Stack const& stack) :
		m_container(stack.m_container)
	{}
	
	Stack(Stack&& stack) : 
		m_container(std::move(stack))
	{}
	
	Stack operator=(const Stack& stack)
	{
		m_container = stack.m_container;
		return *this;
	}
	
	Stack operator=(Stack&& stack) {
		m_container = std::move(stack.m_container);
		return *this;
	}
	
	reference top() {
		return m_container.back();
	}
		
	const_reference top() const {
		return m_container.back();
	}
	
	bool empty() const {
		return m_container.empty();
	}
	
	size_type size() const {
		return m_container.size();
	}
	
	void push(const value_type& value) {
		m_container.push_back(value);
	}
	
	void push(value_type&& value) {
		m_container.push_back(std::move(value));
	}
	
	void pop() {
		m_container.pop_back();
	}
	
	size_t capacity() const {
		return m_container.capacity();
	}
	
	void dump() const {
		return m_container.dump();
	}
	
	bool ok() const {
		return m_container.ok();
	}
	
private:
	
	Container m_container;
};

#endif
