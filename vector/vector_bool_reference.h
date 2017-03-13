#ifndef VECTOR_BOOL_REFERENCE
#define VECTOR_BOOL_REFERENCE

#include <cstddef>

class VectorBoolReference {
public:
	typedef unsigned char datatype;
	typedef std::size_t size_type;
	
	VectorBoolReference() : m_byte(0), m_bit(0) {}
	
	VectorBoolReference(datatype* byte, size_type bit) : m_byte(byte), m_bit(bit) {}
	
	operator bool() const {
		return (*m_byte) & (1 << m_bit);
	}
	
	VectorBoolReference& operator=(bool x) {
		if (x) {
			*m_byte |= (1 << m_bit);
		} else {
			*m_byte &= ~(1 << m_bit);
		}
		return *this;
	}
	
	VectorBoolReference& operator=(const VectorBoolReference& rhs) {
		return *this = bool(rhs);
	}
	
	void flip() {
		*m_byte ^= 1 << m_bit;
	}
	
	VectorBoolReference* operator->() {
		return this;
	}
	
private:
	datatype* m_byte;
	size_type m_bit;
};

#endif
