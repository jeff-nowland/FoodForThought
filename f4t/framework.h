#pragma once

#include <f4t/config/compiler.h>
#include <f4t/config/platform.h>

#define F4T_BUILD(X)        F4T_BUILD_##X ()
#define F4T_BUILD_DEBUG()   0
#define F4T_BUILD_RELEASE() 0

#if defined(_DEBUG) || defined(DEBUG)
# undef F4T_BUILD_DEBUG
# define F4T_BUILD_DEBUG() 1
#else
# undef F4T_BUILD_RELEASE
# define F4T_BUILD_RELEASE() 1
#endif
