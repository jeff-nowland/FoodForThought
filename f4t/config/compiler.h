#pragma once

#define F4T_COMPILER(X)       F4T_COMPILER_##X ()
#define F4T_COMPILER_CLANG()  0
#define F4T_COMPILER_GNUC()   0
#define F4T_COMPILER_MSVC()   0

#if defined(_MSC_VER)
# undef F4T_COMPILER_MSVC
# define F4T_COMPILER_MSVC() _MSC_VER
#elif defined(__clang__)
# undef F4T_COMPILER_CLANG
# define F4T_COMPILER_CLANG() ((__clang_major__ * 100) + __clang_minor__)
#elif defined(__GNUC__)
# undef F4T_COMPILER_GNUC
# define F4T_COMPILER_GNUC() ((__GNUC__ * 100) + __GNUC_MINOR__)
#else
# error "unsupported compiler"
#endif

