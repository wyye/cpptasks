#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <iostream>
#include <cstdlib>

void vassert(
	const char *file,
	const int line,
	const char *exp,
	const char *msg);

#define VASSERT(exp, msg) \
	if (!(exp)) vassert(__FILE__, __LINE__, #exp, msg);
	
#ifdef NDEBUG
#define DASSERT(exp, msg)
#else
#define DASSERT(exp, msg) VASSERT(exp, msg)
#endif
	
#ifdef NDEBUG
#define DEBUG_ONLY(exp)
#else
#define DEBUG_ONLY(exp) exp
#endif
	
void* operator new[](size_t size, int val);

#endif // DEBUG_UTILS_H
	
