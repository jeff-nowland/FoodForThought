#pragma once
#include <f4t/framework.h>

#include <cassert>
#include <cstddef>
#include <cstdint>

#ifdef f4t_EXPORTS
# define F4TAPI F4T_EXPORT_API
#else
# define F4TAPI F4T_IMPORT_API
#endif
#define F4TINTERNAPI F4T_INTERNAL_API

namespace f4t
{
}

