#include "debug_utils.h"
#include <cstring>

void vassert(
	const char *file,
	const int line,
	const char *exp,
	const char *msg)
{
	std::cerr << "ASSERT: " << file << ":" << line << ": " << exp << "\n";
	std::cerr << "MESSAGE: " << msg << std::endl;
	abort();
}

void* operator new[](size_t size, int val)
{
	void* res = ::operator new[](size);
	std::memset(res, val, size);
	return res;
}
