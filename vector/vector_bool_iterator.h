#ifndef VECTOR_BOOL_ITERATOR_H
#define VECTOR_BOOL_ITERATOR_H

#include <iterator>

#include "vector_bool_reference.h"

template <bool CONST>
struct VecBoolItRefType;

template <>
struct VecBoolItRefType<false> {
	typedef VectorBoolReference val;
};

template <>
struct VecBoolItRefType<true> {
	typedef bool val;
};

template<bool CONST>
class VecBoolIt
: public std::iterator<std::random_access_iterator_tag, bool>
{
public:
	typedef unsigned char datatype;
	typedef std::size_t size_type;
	
	typedef typename VecBoolItRefType<CONST>::val reference;
		
	static constexpr size_t BITS = 8 * sizeof(datatype);
	
	VecBoolIt() : m_byte(nullptr), m_bit(0) {}
	
	VecBoolIt(datatype* byte, size_type bit) : m_byte(byte), m_bit(bit) {}
	
	template<bool CONST2>
	VecBoolIt(const VecBoolIt<CONST2>& rhs)
		: m_byte(rhs.getByte()), m_bit(rhs.getBit())
		{}
	
	bool operator==(const VecBoolIt& rhs) const {
		return m_byte == rhs.m_byte && m_bit == rhs.m_bit;
	}
	
	bool operator!=(const VecBoolIt& rhs) const {
		return !operator==(rhs);
	}
	
	bool operator<(const VecBoolIt& rhs) const {
		return 
			(m_byte < rhs.m_byte) || 
			(m_byte == rhs.m_byte && m_bit < rhs.m_bit);
	}
	
	bool operator>(const VecBoolIt& rhs) const { return rhs < *this; }
	
	bool operator<=(const VecBoolIt& rhs) const { return !(*this > rhs); }
	
	bool operator>=(const VecBoolIt& rhs) const { return !(*this < rhs); }
	
	VecBoolIt& operator+=(difference_type offset) {
		DASSERT(m_bit < BITS, "");
		difference_type byte_start_offset = offset + static_cast<difference_type>(m_bit);
		m_byte += byte_start_offset / static_cast<difference_type>(BITS);
		byte_start_offset = byte_start_offset % static_cast<difference_type>(BITS);
		// no correction in case byte_start_offset % sizeof(datatype) == 0, byte_start_offset < 0
		if (byte_start_offset < 0) {
			m_byte -= 1;
			byte_start_offset += static_cast<difference_type>(BITS);
		}
		DASSERT(byte_start_offset < static_cast<difference_type>(BITS), "");
		m_bit = static_cast<size_type>(byte_start_offset);
		DASSERT(m_bit < BITS, "");
		return *this;
	}
		
	VecBoolIt& operator-=(difference_type offset) {
		*this += -offset;
		return *this;
	}
	
	VecBoolIt& operator++() {
		*this += 1;
		return *this;
	}
	
	VecBoolIt operator++(int) {
		VecBoolIt res(*this);
		*this += 1;
		return res;
	}
	
	VecBoolIt& operator--() {
		*this -= 1;
		return *this;
	}
	
	VecBoolIt operator--(int) {
		VecBoolIt res(*this);
		*this -= 1;
		return res;
	}
	
	VecBoolIt operator+(difference_type n) const {
		VecBoolIt res(*this);
		return res += n;
	}

	VecBoolIt operator-(difference_type n) const {
		VecBoolIt res(*this);
		return res -= n;
	}
	
	reference operator*() const {
		return VectorBoolReference(m_byte, m_bit);
	}

	reference operator->() const {
		return (*this)[0];
	}
	
	reference operator[](difference_type pos) const {
		return *(*this + pos);
	}
	
	datatype* getByte() const { return m_byte; }
	
	size_type getBit() const { return m_bit; }
	
private:
	datatype* m_byte;
	size_type m_bit;
};

template <bool CONST2>
ptrdiff_t operator-(const VecBoolIt<CONST2>& lhs, const VecBoolIt<CONST2>& rhs)
{
	ptrdiff_t byte_diff = lhs.getByte() - rhs.getByte();
	ptrdiff_t byte_in_bit_diff = byte_diff * static_cast<ptrdiff_t>(VecBoolIt<CONST2>::BITS);
	ptrdiff_t bit_diff = static_cast<ptrdiff_t>(lhs.getBit() - rhs.getBit());
	
	return byte_in_bit_diff + bit_diff;
}

template <bool CONST>
VecBoolIt<CONST> operator+(ptrdiff_t lhs, VecBoolIt<CONST>& rhs) {
	return rhs + lhs;
}

typedef VecBoolIt<false> VectorBoolIterator;
typedef VecBoolIt<true> ConstVectorBoolIterator;

#endif
