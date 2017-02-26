#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <cstring>
#include <string>
#include <limits>

struct StackEmpty : public std::runtime_error
{
	StackEmpty(std::string const& message) : 
		std::runtime_error("StackEmpty: " + message)
	{}
};

struct StackLarge : public std::runtime_error
{
	StackLarge(std::string const& message) : 
		std::runtime_error("StackLarge: " + message)
	{}
};

template <typename T>
class Stack {
public:
	
	typedef T value_type;
	typedef size_t size_type;
	typedef T& reference;
	typedef const T& const_reference;
	
	Stack() :
		m_size(0),
		m_capacity(0),
		m_data(nullptr)
	{
	}
	
	Stack(Stack const& stack) :
		m_size(stack.size()),
		m_capacity(stack.capacity()),
		m_data(new T[stack.capacity()])
	{
		memcpy(m_data, stack.m_data, stack.capacity());
	}
	
	Stack(Stack&& stack) : 
		m_size(std::move(stack.m_size)),
		m_capacity(std::move(stack.m_size))
	{
		m_data = stack.m_data;
		stack.m_data = nullptr;
	}
	
	Stack operator=(const Stack& stack)
	{
		m_size = stack.size();
		m_capacity = stack.capacity();
		delete[] m_data;
		m_data = new T[stack.capacity()];
		memcpy(m_data, stack.m_data, stack.capacity());
		return *this;
	}
	
	Stack operator=(Stack&& stack) {
		m_size = std::move(stack.m_size);
		m_capacity = std::move(stack.m_capacity);
		m_data = stack.m_data;
		stack.m_data = nullptr;
	}
	
	~Stack() {
		if (m_data != nullptr)
			delete[] m_data;
	}
	
	reference top() {
		if (m_size == 0) {
			throw StackEmpty("r top");
		}
		return m_data[m_size-1];
	}
		
	const_reference top() const {
		if (m_size == 0) {
			throw StackEmpty("cr top");
		}
		return m_data[m_size-1];
	}
	
	bool empty() const {
		return !m_size;
	}
	
	size_type size() const {
		return m_size;
	}
	
	void push(const value_type& value) {
		if (m_size == m_capacity) stretch();
		m_size += 1;
		m_data[m_size-1] = value;
	}
	
	void push(value_type&& value) {
		if (m_size == m_capacity) stretch();
		m_size += 1;
		m_data[m_size-1] = std::move(value);
	}
	
	void pop() {
		if (m_size == 0) {
			throw StackEmpty("pop");
		}
		m_size -= 1;
	}
	
	size_t capacity() const {
		return m_capacity;
	}
	
	void dump() const;
	bool ok() const;
	
private:
	void stretch();
	
	size_t m_size;
	size_t m_capacity;
	T* m_data;
};

template <typename T>
void Stack<T>::stretch() {
	if (m_data == nullptr) {
		m_capacity = 1;
		m_size = 0;
		m_data = new T[1];
	} else {
		if (std::numeric_limits<size_t>::max() / 2 <= m_capacity)
			throw StackLarge("stretch");
		size_t new_m_capacity = m_capacity * 2;
		T* new_m_data = new T[new_m_capacity];
		memcpy(new_m_data, m_data, m_capacity * sizeof(T));
		memset(new_m_data+m_capacity, 0xFE, (new_m_capacity - m_capacity) * sizeof(T));
		delete[] m_data;
		m_data = new_m_data;
		m_capacity = new_m_capacity;
	}
}

template <typename T>
void Stack<T>::dump() const {
	std::cout << "class Stack<T> {" << std::endl;
	std::cout << "\tsize = " << m_size << std::endl;
	std::cout << "\tcapacity = " << m_capacity << std::endl;
	std::cout << "\tdata [" << static_cast<void*>(m_data) << "] = {" << std::endl;
	for (size_t i = 0; i < m_capacity; ++i) {
		std::cout << "\t\t" << i << " [" << static_cast<void*>(&m_data[i]) << 
			"] : ";
		if (i >= m_size) 
			std::cout << "GARBAGE ";
		std::cout << m_data[i] << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std::cout << "}" << std::endl;
}

template <typename T>
bool Stack<T>::ok() const {
	bool check1 = m_size <= m_capacity;
	bool check2 = (m_data == nullptr && m_capacity == 0) || (m_data != nullptr && m_capacity > 0);
	return check1 && check2;
}

#endif
