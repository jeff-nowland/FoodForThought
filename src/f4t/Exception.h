#ifndef f4t_Exception_h
#define f4t_Exception_h	1

#include "f4t.h"

#include <stdexcept>

namespace f4t
{
#define F4T_DECLARE_SIMPLE_EXCEPTION(NAME, BASE)	\
	class NAME :									\
	public BASE										\
	{												\
	public:											\
		NAME(const char* mesg) : BASE(mesg)	{}		\
		~NAME() noexcept					{}		\
	}

	F4T_DECLARE_SIMPLE_EXCEPTION(argument_error,			std::invalid_argument);
	F4T_DECLARE_SIMPLE_EXCEPTION(argument_null_error,		argument_error);
	F4T_DECLARE_SIMPLE_EXCEPTION(duplicate_argument_error,	argument_error);
}



#endif
