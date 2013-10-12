#ifndef f4t_h
#define f4t_h	1

#include <cassert>
#include <cstddef>

namespace f4t
{
	template <typename T>
	inline T clamp(T value, T lower, T upper)
	{ return value < lower ? lower : upper < value ? upper : value; }
}

#endif

