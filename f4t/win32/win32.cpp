
#include <f4t/framework.h>

#if F4T_COMPILER(MSVC)
# define WIN32_LEAN_AND_MEAN
# include <Windows.h>

BOOL APIENTRY DllMain (HMODULE, DWORD, LPVOID) {
  return TRUE;
}
#endif
