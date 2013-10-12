#pragma once
#include <f4t/config/compiler.h>

#define F4T_PLATFORM(X)       F4T_PLATFORM_##X ()
#define F4T_PLATFORM_LINUX()  0
#define F4T_PLATFORM_WIN32()  0

#define F4T_ARCHITECTURE(X)     F4T_ARCHITECTURE_##X ()
#define F4T_ARCHITECTURE_X64()  0

#define F4T_EXPORT_API
#define F4T_IMPORT_API
#define F4T_INTERNAL_API

#if F4T_COMPILER(MSVC)
# if defined(_WIN32)
#   undef F4T_PLATFORM_WIN32
#   define F4T_PLATFORM_WIN32() 1
#   if defined(_M_X64)
#     undef F4T_ARCHITECTURE_X64
#     define F4T_ARCHITECTURE_X64() 1
#   else
#     error "unsuported WIN32 architecture"
#   endif
#   undef F4T_EXPORT_API
#   undef F4T_IMPORT_API
#   define F4T_EXPORT_API __declspec(dllexport)
#   define F4T_IMPORT_API __declspec(dllimport)
# else
#   error "unsupported MSVC platform"
# endif
#elif F4T_COMPILER(CLANG)
# if defined(__linux__)
#   undef F4T_PLATFORM_LINUX
#   define F4T_PLATFORM_LINUX() 1
#   if defined(__x86_64__)
#     undef F4T_ARCHITECTURE_X64
#     define F4T_ARCHITECTURE_X64() 1
#   else
#     error "unsupported linux architecture"
#   endif
#   undef F4T_EXPORT_API
#   define F4T_EXPORT_API   __attribute__ ((visibility ("default")))
#   define F4T_INTERNAL_API __attribute__ ((visibility ("hidden")))
# else
#   error "unsupport CLANG platform"
# endif
#elif F4T_COMPILER(GNUC)
# if defined(__linux__)
#   undef F4T_PLATFORM_LINUX
#   define F4T_PLATFORM_LINUX() 1
#   if defined(__x86_64__)
#     undef F4T_ARCHITECTURE_X64
#     define F4T_ARCHITECTURE_X64() 1
#   else
#     error "unsupported GNUC architecture"
#   endif
#   undef F4T_EXPORT_API
#   undef F4T_INTERNAL_API
#   define F4T_EXPORT_API   __attribute__ ((visibility ("default")))
#   define F4T_INTERNAL_API __attribute__ ((visibility ("hidden")))
# else
#   error "unsupported GNUC platform"
# endif
#else
# error "unsupported compiler"
#endif

