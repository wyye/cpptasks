#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <iterator>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <new>
#include <cstring>
#include <initializer_list>
#include <utility>

#include "debug_utils.h"

template <typename T>
class Vector;

namespace std {
template <typename T>
inline void swap(Vector<T>& v1, Vector<T>& v2) noexcept;
}

template <typename T>
class Vector {
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef pointer iterator;
	typedef const_pointer const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	
	Vector() :
		m_capacity(0),
		m_size(0),
		m_data(nullptr)
	{}
	
	explicit Vector(size_type count) :
		m_capacity(count),
		m_size(count),
		m_data(static_cast<pointer>(operator new[](sizeof(T) * count)))
	{
		DEBUG_ONLY(std::cerr << "Vector::Vector(size_type)" << std::endl);
		for (size_type s = 0; s != count; ++s) {
			try {
				new (&m_data[s]) T(); 
			} catch (...) {
				for (size_type s2 = 0; s2 != s; ++s2) {
					m_data[s2].~T();
				}
				operator delete[](m_data);
				throw;
			}
		}
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
	}
		
	Vector(const Vector& other) :
		m_capacity(other.m_capacity),
		m_size(other.m_size),
		m_data(static_cast<pointer>(operator new[](sizeof(T) * m_capacity)))
	{
		DEBUG_ONLY(std::cerr << "Vector::Vector(const vector&)" << std::endl);
		for (size_type s = 0; s != m_size; ++s) {
			try {
				new (&m_data[s]) T(other.m_data[s]);
			} catch (...) {
				for (size_type s2 = 0; s2 != s; ++s2) {
					m_data[s2].~T();
				}
				operator delete[](m_data);
				throw;
			}
		}
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
	}
	
	Vector(Vector&& other) noexcept :
		Vector()
	{
		DEBUG_ONLY(std::cerr << "Vector::Vector(const vector&)" << std::endl);
		DEBUG_ONLY(dump());
		std::swap(*this, other);
	}
	
	Vector(std::initializer_list<T> ilist) :
		m_capacity(ilist.size()),
		m_size(ilist.size()),
		m_data(static_cast<pointer>(operator new[](sizeof(T) * m_capacity)))
	{
		DEBUG_ONLY(std::cerr << "Vector::Vector(std::initializer_list<T>)" << std::endl);
		auto it = ilist.begin();
		for (size_type s = 0; s != m_size; ++s) {
			try {
				new (&m_data[s]) T(*it);
			} catch (...) {
				for (size_type s2 = 0; s2 != s; ++s2) {
					m_data[s2].~T();
				}
				operator delete[](m_data);
				throw;
			}
			++it;
		}
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
	}
	
	// copy-and-swap
	Vector& operator=(Vector other) {
		DEBUG_ONLY(std::cerr << "Vector::operator=(const Vector)" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		std::swap(*this, other);
		return *this;
	}
	
	Vector& operator=(Vector&& other) noexcept {
		clear();
		shrink_to_fit();
		std::swap(*this, other);
	}
	
	Vector& operator=(std::initializer_list<T> ilist) {
		Vector other(ilist);
		std::swap(*this, other);
		return *this;
	}
	
	~Vector() {
		DEBUG_ONLY(std::cerr << "Vector::~Vector()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		for (size_type s = 0; s != m_size; ++s) {
			m_data[s].~T();
		}
		if (m_data != nullptr)
			operator delete[](m_data);
	}
	
	reference at(size_type pos) {
		DEBUG_ONLY(std::cerr << "Vector::at(size_type)" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		if (pos >= m_size)
			throw std::out_of_range("Out of array size");
		return m_data[pos];
	}
		
	const_reference at(size_type pos) const {
		DEBUG_ONLY(std::cerr << "Vector::at(size_type) const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		if (pos >= m_size)
			throw std::out_of_range("Out of array size");
		return m_data[pos];
	}
	
	reference operator[](size_type pos) {
		DEBUG_ONLY(std::cerr << "Vector::operator[](size_type)" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(pos < m_size, "Out of array size");
		DASSERT(ok(), "State is not ok");
		return m_data[pos];
	}
		
	const_reference operator[](size_type pos) const {
		DEBUG_ONLY(std::cerr << "Vector::operator[](size_type) const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(pos < m_size, "Out of array size");
		DASSERT(ok(), "State is not ok");
		return m_data[pos];
	}
		
	reference front() {
		DEBUG_ONLY(std::cerr << "Vector::front()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(m_size > 0, "Taking front element from empty vector");
		DASSERT(ok(), "State is not ok");
		return m_data[0];
	}
		
	const_reference front() const {
		DEBUG_ONLY(std::cerr << "Vector::front() const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(m_size > 0, "Taking front element from empty vector");
		DASSERT(ok(), "State is not ok");
		return m_data[0];
	}
	
	reference back() {
		DEBUG_ONLY(std::cerr << "Vector::back()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(m_size > 0, "Taking back element from empty vector");
		DASSERT(ok(), "State is not ok");
		return m_data[m_size - 1];
	}
		
	const_reference back() const {
		DEBUG_ONLY(std::cerr << "Vector::back() const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(m_size > 0, "Taking back element from empty vector");
		DASSERT(ok(), "State is not ok");
		return m_data[m_size - 1];
	}
	
	T* data() {
		DEBUG_ONLY(std::cerr << "Vector::data()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return m_data;
	}

	const T* data() const {
		DEBUG_ONLY(std::cerr << "Vector::data() const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return m_data;
	}
	
	iterator begin() {
		DEBUG_ONLY(std::cerr << "Vector::begin()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return m_data;
	}
		
	const_iterator begin() const {
		DEBUG_ONLY(std::cerr << "Vector::begin() const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return m_data;
	}
	
	const_iterator cbegin() const {
		DEBUG_ONLY(std::cerr << "Vector::cbegin() const" << std::endl);
		return begin();
	}
	
	iterator end() {
		DEBUG_ONLY(std::cerr << "Vector::end()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return m_data+m_size;
	}
		
	const_iterator end() const {
		DEBUG_ONLY(std::cerr << "Vector::end() const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return m_data+m_size;
	}
			
	const_iterator cend() const {
		DEBUG_ONLY(std::cerr << "Vector::cend() const" << std::endl);
		return end();
	}
	
	reverse_iterator rbegin() {
		DEBUG_ONLY(std::cerr << "Vector::rbegin()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return reverse_iterator(end());
	}
		
	const_reverse_iterator rbegin() const {
		DEBUG_ONLY(std::cerr << "Vector::rbegin() const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return const_reverse_iterator(end());
	}
			
	const_reverse_iterator crbegin() const {
		DEBUG_ONLY(std::cerr << "Vector::crbegin()" << std::endl);
		return rbegin();
	}
	
	reverse_iterator rend() {
		DEBUG_ONLY(std::cerr << "Vector::rend()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return reverse_iterator(begin());
	}
		
	const_reverse_iterator rend() const {
		DEBUG_ONLY(std::cerr << "Vector::rend() const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return const_reverse_iterator(begin());
	}
			
	const_reverse_iterator crend() const {
		DEBUG_ONLY(std::cerr << "Vector::crend() const" << std::endl);
		return rend();
	}
	
	bool empty() const {
		DEBUG_ONLY(std::cerr << "Vector::empty() const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return m_size == 0;
	}
	
	size_type size() const {
		DEBUG_ONLY(std::cerr << "Vector::size() const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return m_size;
	}
	
	void reserve(size_type);
	
	size_type capacity() const {
		DEBUG_ONLY(std::cerr << "Vector::capacity() const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return m_capacity;
	}
	
	void shrink_to_fit();
	
	void clear();
	
	// intsert value before pos
	iterator insert(const_iterator pos, const T& value) {
		DEBUG_ONLY(std::cerr << "Vector::insert(const_iterator, const T&)" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		emplace(pos, value);
	}
	iterator insert(const_iterator pos, T&& value) {
		DEBUG_ONLY(std::cerr << "Vector::insert(const_iterator, T&&)" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		emplace(pos, std::move(value));
	}
	iterator insert(const_iterator pos, size_type count, const T& value);
	
	template<typename InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last);
	
	iterator insert(const_iterator pos, std::initializer_list<T> ilist);
	
	template< class... Args >
	iterator emplace(const_iterator pos, Args&&... args) {
		DEBUG_ONLY(std::cerr << "Vector::emplace(const_iterator, Args&&...)" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		size_type ind = static_cast<size_type>(pos - m_data);
		if (ind > m_size)
			throw std::out_of_range("Out of array size");
		if (m_capacity == m_size) {
			reserve(m_capacity ? 2*m_capacity : 1);
		}
		if (ind != m_size) {
			std::memmove(&m_data[ind+1], &m_data[ind], (m_size-ind)*sizeof(T));
		}
		new (&m_data[ind]) T(std::forward<Args>(args)...);
		++m_size;
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return const_cast<iterator>(pos);
	}

	iterator erase(const_iterator pos) {
		DEBUG_ONLY(std::cerr << "Vector::erase(const_iterator)" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		erase(pos, pos+1);
		return const_cast<iterator>(pos);
	}
	iterator erase(const_iterator first, const_iterator last);
	
	void push_back(const T& value) {
		DEBUG_ONLY(std::cerr << "Vector::emplace_back(const T&)" << std::endl);
		emplace_back(value);
	}
	void push_back(T&& value) {
		DEBUG_ONLY(std::cerr << "Vector::push_back(T&&)" << std::endl);
		emplace_back(std::move(value));
	}
	
	template<class... Args>
	reference emplace_back(Args&&... args) {
		DEBUG_ONLY(std::cerr << "Vector::emplace_back(Args&&...)" << std::endl);
		return *emplace(end(), std::forward<Args>(args)...);
	}
	
	void pop_back() {
		DEBUG_ONLY(std::cerr << "Vector::pop_back()" << std::endl);
		erase(&m_data[m_size-1]);
	}
	
	void resize(size_type count) {
		DEBUG_ONLY(std::cerr << "Vector::resize(size_type)" << std::endl);
		resize(count, T());
	}
	
	void resize(size_type count, const value_type& value);
	
	void swap(Vector& other) noexcept {
		std::swap(m_data, other.m_data);
		std::swap(m_size, other.m_size);
		std::swap(m_capacity, other.m_capacity);
	}
	
	bool ok() const noexcept {
		bool check1 = m_size <= m_capacity;
		bool check2 = 
			(m_data == nullptr && m_capacity == 0) 
			|| (m_data != nullptr && m_capacity > 0);
		return check1 && check2;
	}
	
	void dump() const;
	
private:
	
	size_type new_capacity(size_type desired_size) {
		size_type result = m_capacity;
		while (result < desired_size) {
			result = result ? result * 2 : 1;
		}
		return result;
	}
	
	size_type m_capacity = 0;
	size_type m_size = 0;
	pointer m_data = nullptr;
};

template <typename T>
void Vector<T>::reserve(size_type new_cap) {
	DEBUG_ONLY(std::cerr << "Vector::reserve(size_type)" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	
	if (new_cap <= capacity()) return;
	
	if (capacity() == 0) {
		
		DASSERT(m_data == nullptr, "Incorrect state");
		m_data = static_cast<pointer>(operator new[](sizeof(T) * new_cap));
		m_capacity = new_cap;
		
	} else {
		
		DASSERT(m_data != nullptr, "Incorrect state");
		T* new_m_data = static_cast<pointer>(operator new[](sizeof(T) * new_cap));
		std::memmove(new_m_data, m_data, m_size * sizeof(T));
		operator delete[](m_data);
		m_data = new_m_data;
		m_capacity = new_cap;
		
	}
	
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
}

template <typename T>
void Vector<T>::shrink_to_fit() {
	DEBUG_ONLY(std::cerr << "Vector::shrink_to_fit()" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	
	if (m_size == m_capacity) return;
	
	if (m_size == 0) {
	
		DASSERT(m_data != nullptr, "Incorrect state");
		operator delete[](m_data);
		m_data = nullptr;
		m_capacity = 0;
		
	} else {
		
		DASSERT(m_data != nullptr, "incorrect state");
		T* new_m_data = static_cast<pointer>(operator new[](sizeof(T) * m_size));
		std::memmove(new_m_data, m_data, sizeof(T) * m_size);
		operator delete[](m_data);
		m_data = new_m_data;
		m_capacity = m_size;
		
	}
	
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
}

template <typename T>
void Vector<T>::clear() {
	DEBUG_ONLY(std::cerr << "Vector::clear()" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	for (size_type s = 0; s != m_size; ++s)
		m_data[s].~T();
	m_size = 0;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, size_type count, const T& value) {
	DEBUG_ONLY(std::cerr << "Vector::insert(const_iterator, size_type, const T&)" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	if (pos >= m_size)
		throw std::out_of_range("Out of array size");
	reserve(new_capacity(m_size+count));
	DASSERT(m_capacity >= m_size+count, "Capacity not enough");
	if (pos != m_size) {
		std::memmove(&m_data[pos+count], m_data[pos], (m_size-pos)*sizeof(T));
	}
	for (size_type i = pos; i != pos + count; ++i) {
		new (&m_data[i]) T(value);
	}
	m_size += count;
	DEBUG_ONLY(std::cerr << "Vector::insert(const_iterator, size_type, const T&)" << std::endl);
	DEBUG_ONLY(dump());
	return &m_data[pos];
}

template <typename T>
template <typename InputIt>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, InputIt first, InputIt last) {
	DEBUG_ONLY(std::cerr << "Vector::insert(const_iterator, InputIt, InputIt)" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		
	if (!(m_data <= pos && pos <= m_data + m_size))
		throw std::out_of_range("Out of array size");
	size_type ind = static_cast<size_type>(pos - m_data);
	size_type count = static_cast<size_type>(last - first);
	reserve(new_capacity(m_size+count));
	DASSERT(m_capacity >= m_size+count, "Capacity not enough");
	if (pos != m_data + m_size) {
		std::memmove(&m_data[ind+count], &m_data[ind], (m_size-ind)*sizeof(T));
	}
	DEBUG_ONLY(dump());
	auto it = first;
	for (size_type i = ind; i != ind+count; ++i) {
		new (&m_data[i]) T(*it);
		++it;
	}
	m_size += count;
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	return &m_data[ind];
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, std::initializer_list<T> ilist) {
	DEBUG_ONLY(std::cerr << "Vector::insert(const_iterator, std::initializer_list<T>)" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	return insert(pos, ilist.begin(), ilist.end());
}

template <typename T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator first, const_iterator last) {
	DEBUG_ONLY(std::cerr << "Vector::erase(const_iterator, const_iterator)" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	DASSERT(first <= last, "Wrong input");
	DASSERT(&m_data[0] <= first && last <= &m_data[m_size], "Wrong input");
	for (auto it = first; it != last; ++it) {
		it->~T();
	}
	std::memmove(const_cast<iterator>(first), last, static_cast<size_type>(&m_data[m_size]-last) * sizeof(T));
	m_size -= static_cast<size_type>(last-first);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	return const_cast<iterator>(first);
}

template <typename T>
void Vector<T>::dump() const {
	std::cerr << "class Vector<T> {" << std::endl;
	std::cerr << "\tsize = " << m_size << std::endl;
	std::cerr << "\tcapacity = " << m_capacity << std::endl;
	std::cerr << "\tdata [" << static_cast<void*>(m_data) << "] = {" << std::endl;
	for (size_t i = 0; i < m_capacity; ++i) {
		std::cerr << "\t\t" << i << " [" << static_cast<void*>(&m_data[i]) << 
			"] : ";
		if (i >= m_size) 
			std::cerr << "GARBAGE ";
		std::cerr << m_data[i] << std::endl;
	}
	std::cerr << "\t}" << std::endl;
	std::cerr << "}" << std::endl;
}

template <typename T>
inline void std::swap(Vector<T>& v1, Vector<T>& v2) noexcept {
	DEBUG_ONLY(std::cerr << "my swap" << std::endl);
	v1.swap(v2);
}

template <typename T>
void Vector<T>::resize(size_type count, const value_type& value) {
	DEBUG_ONLY(std::cerr << "Vector::resize(size_type, const value_type&)" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	if (m_size < count) {
		reserve(new_capacity(count));
		for (size_type i = m_size; i != count; ++i) {
			new (&m_data[i]) T(value);
		}
	} else if (m_size > count) {
		for (size_type i = count; i != m_size; ++i)
			m_data[i].~T();
	} else {
		return;
	}
	m_size = count;
}

#endif


