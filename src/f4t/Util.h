#ifndef f4t_Util_h
#define f4t_Util_h	1

#include "f4t.h"
#include <ostream>

namespace f4t
{
	template <typename I>
	inline std::ostream& print_container_range(std::ostream& strm, I first, I last, const char* delim = nullptr)
	{
		char front, back, sep;

		if (!delim) {
			front = '\0';
			back = '\0';
			sep = ',';
		}
		else {
			front = delim[0];
			back = delim[1];
			sep = delim[2];
		}

		if (front) {
			strm << front;
		}
		if (first != last) {
			strm << *first++;
			while (first != last) {
				if (sep) {
					strm << sep;
				}
				strm << *first++;
			}
		}
		if (back) {
			strm << back;
		}
		return strm;
	}

	template <typename C>
	inline std::ostream& print_container(std::ostream& strm, const C& con, const char* delim = nullptr)
	{
		return print_container_range(strm, std::begin(con), std::end(con), delim);
	}
}

#endif
