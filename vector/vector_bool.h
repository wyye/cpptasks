#ifndef VECTOR_BOOL_H
#define VECTOR_BOOL_H

#include "vector_generic.h"
#include "vector_bool_reference.h"
#include "vector_bool_iterator.h"

template <>
class Vector<bool> {
public:
	typedef VectorBoolReference::datatype datatype;
	static constexpr int BITS = 8 * sizeof(datatype);
	
	typedef bool value_type;
	typedef size_t size_type;
	typedef std::ptrdiff_t difference_type;	
	
	typedef VectorBoolReference reference;
	typedef bool const_reference;
	
	typedef VectorBoolReference* pointer;
	typedef const bool* const_pointer;
	
	typedef VectorBoolIterator iterator;
	typedef ConstVectorBoolIterator const_iterator;
	
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	
	Vector() :
		m_capacity(0),
		m_size(0),
		m_data(nullptr)
	{}
	
	explicit Vector(size_type count) :
		m_capacity(num_bits(count)),
		m_size(count),
		m_data(static_cast<datatype*>(operator new[](sizeof(datatype) * num_bytes(count), 0)))
	{
		DEBUG_ONLY(std::cerr << "Vector::Vector(size_type)" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
	}
		
	Vector(const Vector& other) :
		m_capacity(num_bits(other.m_capacity)),
		m_size(other.m_size),
		m_data(static_cast<datatype*>(operator new[](sizeof(datatype) * num_bytes(m_capacity), 0)))
	{
		DEBUG_ONLY(std::cerr << "Vector::Vector(const vector&)" << std::endl);
		memcpy(m_data, other.m_data, sizeof(datatype) * num_bytes(m_capacity));
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
	
	Vector(std::initializer_list<bool> ilist) :
		m_capacity(num_bits(ilist.size())),
		m_size(ilist.size()),
		m_data(static_cast<datatype*>(operator new[](sizeof(datatype) * num_bytes(m_capacity), 0)))
	{
		DEBUG_ONLY(std::cerr << "Vector::Vector(std::initializer_list<T>)" << std::endl);
		auto it = ilist.begin();
		for (size_type s = 0; s != m_size; ++s) {
			(*this)[s] = *it;
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
		std::swap(*this, other);
		return *this;
	}
	
	Vector& operator=(std::initializer_list<bool> ilist) {
		Vector other(ilist);
		std::swap(*this, other);
		return *this;
	}
	
	~Vector() {
		DEBUG_ONLY(std::cerr << "Vector::~Vector()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		if (m_data != nullptr)
			operator delete[](m_data);
	}
	
	reference at(size_type pos) {
		DEBUG_ONLY(std::cerr << "Vector::at(size_type)" << std::endl);
		DASSERT(ok(), "State is not ok");
		if (pos >= m_size)
			throw std::out_of_range("Out of array size");
		return (*this)[pos];
	}
		
	const_reference at(size_type pos) const {
		DEBUG_ONLY(std::cerr << "Vector::at(size_type) const" << std::endl);
		DASSERT(ok(), "State is not ok");
		if (pos >= m_size)
			throw std::out_of_range("Out of array size");
		return (*this)[pos];
	}
	
	reference operator[](size_type pos) {
		DEBUG_ONLY(std::cerr << "Vector::operator[](size_type)" << std::endl);
		DASSERT(pos < m_size, "Out of array size");
		DASSERT(ok(), "State is not ok");
		return reference(&m_data[pos/BITS], pos % BITS);
	}
		
	const_reference operator[](size_type pos) const {
		DEBUG_ONLY(std::cerr << "Vector::operator[](size_type) const" << std::endl);
		DASSERT(pos < m_size, "Out of array size");
		DASSERT(ok(), "State is not ok");
		return (*const_cast<Vector<bool>*>(this))[pos];
	}
		
	reference front() {
		DEBUG_ONLY(std::cerr << "Vector::front()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(m_size > 0, "Taking front element from empty vector");
		DASSERT(ok(), "State is not ok");
		return (*this)[0];
	}
		
	const_reference front() const {
		DEBUG_ONLY(std::cerr << "Vector::front() const" << std::endl);
		return const_cast<Vector<bool>*>(this)->front();
	}
	
	reference back() {
		DEBUG_ONLY(std::cerr << "Vector::back()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(m_size > 0, "Taking back element from empty vector");
		DASSERT(ok(), "State is not ok");
		return (*this)[m_size - 1];
	}
		
	const_reference back() const {
		DEBUG_ONLY(std::cerr << "Vector::back() const" << std::endl);
		return const_cast<Vector<bool>*>(this)->back();
	}
	
	datatype* data() {
		DEBUG_ONLY(std::cerr << "Vector::data()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return m_data;
	}

	const datatype* data() const {
		DEBUG_ONLY(std::cerr << "Vector::data() const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return m_data;
	}
	
	iterator begin() {
		DEBUG_ONLY(std::cerr << "Vector::begin()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return iterator(m_data, 0);
	}
		
	const_iterator begin() const {
		DEBUG_ONLY(std::cerr << "Vector::begin() const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return const_iterator(m_data, 0);
	}
	
	const_iterator cbegin() const {
		DEBUG_ONLY(std::cerr << "Vector::cbegin() const" << std::endl);
		return begin();
	}
	
	iterator end() {
		DEBUG_ONLY(std::cerr << "Vector::end()" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return iterator(m_data+(m_size/BITS), m_size % BITS);
	}
	
	const_iterator end() const {
		DEBUG_ONLY(std::cerr << "Vector::end() const" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return const_cast<Vector*>(this)->end();
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
	iterator insert(const_iterator pos, bool value) {
		DEBUG_ONLY(std::cerr << "Vector::insert(const_iterator, const T&)" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return emplace(pos, value);
	}

	iterator insert(const_iterator pos, size_type count, bool value);
	
	template<typename InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last);
	
	iterator insert(const_iterator pos, std::initializer_list<bool> ilist);
	
	iterator emplace(const_iterator pos, bool args) {
		DEBUG_ONLY(std::cerr << "Vector::emplace(const_iterator, Args&&...)" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		DASSERT(pos >= begin(), "");
		DASSERT(pos <= end(), "");
		size_type ind = static_cast<size_type>(pos - cbegin());
		if (ind > m_size)
			throw std::out_of_range("Out of array size");
		if (m_capacity == m_size) {
			reserve(new_capacity(m_size+1));
		}
		
		size_type old_size = m_size;
		++m_size;
		if (ind != old_size) {
			for (size_type s = old_size-ind; s != 0; --s) {
				(*this)[ind+s] = (*this)[ind+s-1]; 
			}
		}
		(*this)[ind] = bool(args);
		
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		return getIterator(ind);
	}

	iterator erase(const_iterator pos) {
		DEBUG_ONLY(std::cerr << "Vector::erase(const_iterator)" << std::endl);
		DEBUG_ONLY(dump());
		DASSERT(ok(), "State is not ok");
		DASSERT(pos >= begin(), "");
		erase(pos, pos+1);
		return pos;
	}
	
	iterator erase(const_iterator first, const_iterator last);
	
	void push_back(bool value) {
		DEBUG_ONLY(std::cerr << "Vector::emplace_back(const T&)" << std::endl);
		emplace_back(value);
	}
	
	reference emplace_back(bool args) {
		DEBUG_ONLY(std::cerr << "Vector::emplace_back(Args&&...)" << std::endl);
		return *emplace(end(), args);
	}
	
	void pop_back() {
		DEBUG_ONLY(std::cerr << "Vector::pop_back()" << std::endl);
		erase(getIterator(m_size-1));
	}
	
	void resize(size_type count) {
		DEBUG_ONLY(std::cerr << "Vector::resize(size_type)" << std::endl);
		resize(count, false);
	}
	
	void resize(size_type count, bool value);
	
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
		bool check3 = m_capacity % BITS == 0;
		return check1 && check2 && check3;
	}
	
	void dump() const;
	
private:
	
	iterator getIterator(size_type ind) {
		return iterator(m_data + ind / BITS, ind % BITS);
	}
	
	static size_t num_bytes(size_t size) {
		return size==0 ? 0 : (size-1)/BITS+1;
	}
	
	static size_t num_bits(size_t size) {
		return num_bytes(size) * BITS;
	}
	
	size_type new_capacity(size_type desired_size) {
		size_type result = m_capacity;
		while (result < desired_size) {
			result = result ? result * 2 : 1;
		}
		return result;
	}
	
	size_type m_capacity = 0;
	size_type m_size = 0;
	datatype* m_data = nullptr;
};

void Vector<bool>::reserve(size_type new_cap) {
	DEBUG_ONLY(std::cerr << "Vector::reserve(size_type)" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");

	new_cap = num_bits(new_cap);
	
	if (new_cap <= capacity()) return;
	
	if (capacity() == 0) {
		
		DASSERT(m_data == nullptr, "Incorrect state");
		m_data = static_cast<datatype*>(operator new[](new_cap * sizeof(datatype), 0));
		m_capacity = new_cap;
		
	} else {
		
		DASSERT(m_data != nullptr, "Incorrect state");
		datatype* new_m_data = static_cast<datatype*>(operator new[](sizeof(datatype) * new_cap, 0));
		std::memcpy(new_m_data, m_data, num_bytes(m_size) * sizeof(datatype));
		operator delete[](m_data);
		m_data = new_m_data;
		m_capacity = new_cap;
		
	}
	
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
}

void Vector<bool>::shrink_to_fit() {
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
		datatype* new_m_data = static_cast<datatype*>(operator new[](sizeof(datatype) * num_bytes(m_size), 0));
		std::memcpy(new_m_data, m_data, sizeof(datatype) * num_bytes(m_size));
		operator delete[](m_data);
		m_data = new_m_data;
		m_capacity = num_bits(m_size);
		
	}
	
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
}

void Vector<bool>::clear() {
	DEBUG_ONLY(std::cerr << "Vector::clear()" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	m_size = 0;
}

typename Vector<bool>::iterator Vector<bool>::insert(const_iterator pos, size_type count, bool value) {
	DEBUG_ONLY(std::cerr << "Vector::insert(const_iterator, size_type, const T&)" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	if (!(cbegin() <= pos || pos <= cend()))
		throw std::out_of_range("Out of array size");
	size_type ind = static_cast<size_type>(pos - cbegin());
	reserve(new_capacity(m_size+count));
	DASSERT(m_capacity >= m_size+count, "Capacity not enough");
	size_type old_size = m_size;
	m_size += count;
	if (ind != old_size) {
		for (size_type s = old_size+count; s != ind+count; --s) {
			(*this)[s-1] = (*this)[s-1-count]; 
		}
	}
	DEBUG_ONLY(dump());
	for (size_type i = ind; i != ind + count; ++i) {
		(*this)[i] = value;
	}
	DEBUG_ONLY(dump());
	return getIterator(ind);
}

template <typename InputIt>
typename Vector<bool>::iterator Vector<bool>::insert(const_iterator pos, InputIt first, InputIt last) {
	DEBUG_ONLY(std::cerr << "Vector::insert(const_iterator, InputIt, InputIt)" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	DASSERT(last >= first, "");
	if (!(cbegin() <= pos || pos <= cend()))
		throw std::out_of_range("Out of array size");
	size_type ind = static_cast<size_type>(pos - cbegin());
	size_type count = static_cast<size_type>(last - first);
	reserve(new_capacity(m_size+count));
	DASSERT(m_capacity >= count, "Capacity not enough");
	size_type old_size = m_size;
	m_size += count;
	if (ind != old_size) {
		for (size_type s = old_size+count; s != ind+count; --s) {
			(*this)[s-1] = (*this)[s-1-count]; 
		}
	}
	DEBUG_ONLY(dump());
	auto it = first;
	for (size_type i = ind; i != ind+count; ++i) {
		(*this)[i] = *it;
		++it;
	}
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	return getIterator(ind);
}

typename Vector<bool>::iterator Vector<bool>::insert(const_iterator pos, std::initializer_list<bool> ilist) {
	DEBUG_ONLY(std::cerr << "Vector::insert(const_iterator, std::initializer_list<T>)" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	return insert(pos, ilist.begin(), ilist.end());
}

typename Vector<bool>::iterator Vector<bool>::erase(const_iterator first, const_iterator last) {
	DEBUG_ONLY(std::cerr << "Vector::erase(const_iterator, const_iterator)" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	DASSERT(first <= last, "Wrong input");
	DASSERT(begin() <= first && last <= end(), "Wrong input");
	iterator it2 = first;
	for (auto it = last; it != end(); ++it) {
		*it2 = *it;
	}
	m_size -= static_cast<size_type>(last-first);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	return first;
}

void Vector<bool>::dump() const {
	std::cerr << "class Vector<T> {" << std::endl;
	std::cerr << "\tsize = " << m_size << std::endl;
	std::cerr << "\tcapacity = " << m_capacity << std::endl;
	std::cerr << "\tdata [" << static_cast<void*>(m_data) << "] = {" << std::endl;
	for (size_t i = 0; i < m_capacity; ++i) {
		std::cerr << "\t\t" << i << " : ";
		if (i >= m_size) std::cerr << "GARBAGE ";
		std::cerr << bool(m_data[i / BITS] & (1 << (i % BITS))) << std::endl;
	}
	std::cerr << "\t}" << std::endl;
	std::cerr << "}" << std::endl;
}

void Vector<bool>::resize(size_type count, bool value) {
	DEBUG_ONLY(std::cerr << "Vector::resize(size_type, const value_type&)" << std::endl);
	DEBUG_ONLY(dump());
	DASSERT(ok(), "State is not ok");
	if (m_size < count) {
		size_type old_size = m_size;
		m_size = count;
		reserve(new_capacity(count));
		for (size_type i = old_size; i != m_size; ++i) {
			(*this)[i] = value;
		}
	} else {
		m_size = count;
	}
}

#endif
