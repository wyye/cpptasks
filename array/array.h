#ifndef ARRAY_H
#define ARRAY_H

#include <cstddef>
#include <algorithm>
#include <iterator>

#include "debug_utils.h"

template <typename T, std::size_t N>
class Array;

namespace std {
template <typename T, std::size_t N>
inline void swap(Array<T, N>& v1, Array<T, N>& v2) noexcept {
	v1.swap(v2);
}
}

template <typename T, std::size_t N>
class Array {
public:
	
	typedef T                                     value_type;
	typedef size_t                                size_type;
	typedef std::ptrdiff_t                        difference_type;
	typedef value_type&                           reference;
	typedef const value_type&                     const_reference;
	typedef T*                                    pointer;
	typedef const T*                              const_pointer;
	typedef pointer                               iterator;
	typedef const_pointer                         const_iterator;
	typedef std::reverse_iterator<iterator>       reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	
	static_assert(N != 0, "Trying to create empty array");
	
	reference at(size_type pos) {
		if (pos >= N)
			throw std::out_of_range("Wrong index");
		return m_data[pos];
	}
	
	const_reference at(size_type pos) const {
		if (pos >= N)
			throw std::out_of_range("Wrong index");
		return m_data[pos];
	}
	
	reference operator[](size_type pos) {
		DASSERT(pos < N, "Out of range");
		return m_data[pos];
	}
	
	const_reference operator[](size_type pos) const {
		DASSERT(pos < N, "Out of range");
		return m_data[pos];
	}
	
	reference front() {
		return m_data[0];
	}
	
	const_reference front() const {
		return m_data[0];
	}
	
	reference back() {
		return m_data[N-1];
	}
	
	const_reference back() const {
		return m_data[N-1];
	}
	
	T* data() {
		return m_data;
	}
	
	const T* data() const {
		return m_data;
	}
	
	iterator begin() {
		return iterator(m_data);
	}
	
	const_iterator begin() const {
		return const_iterator(m_data);
	}
	
	const_iterator cbegin() const {
		return begin();
	}
	
	iterator end() {
		return iterator(m_data+N);
	}
	
	const_iterator end() const {
		return const_iterator(m_data+N);
	}
	
	const_iterator cend() const {
		return end();
	}
		
	iterator rbegin() {
		return reverse_iterator(end());
	}
	
	const_iterator rbegin() const {
		return const_reverse_iterator(cend());
	}
	
	const_iterator crbegin() const {
		return const_reverse_iterator(cend());
	}
	
	iterator rend() {
		return reverse_iterator(begin());
	}
	
	const_iterator rend() const {
		return const_reverse_iterator(cbegin());
	}
	
	const_iterator crend() const {
		return const_reverse_iterator(cbegin());
	}

	
	bool empty() const {
		return false;
	}
	
	size_type size() const {
		return N;
	}
	
	void fill(const T& value) {
		std::fill(m_data, m_data + N, value);
	}
	
	void swap(Array& other) {
		std::swap(m_data, other.m_data);
	}
	
	T m_data[N];
};

template<class T, std::size_t N>
bool operator==(const Array<T,N>& lhs, const Array<T,N>& rhs) {
	for (std::size_t i = 0; i != N; ++i) {
		if (lhs[i] != rhs[i]) return false;
	}
	return true;
}

template<class T, std::size_t N>
bool operator!=(const Array<T,N>& lhs, const Array<T,N>& rhs) {
	return !(lhs == rhs);
}

#endif // ARRAY_H
