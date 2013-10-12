


ADD_LIBRARY (f4tcheck SHARED Check.cpp Check_Console.cpp)

INSTALL (TARGETS f4tcheck LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

INSTALL (FILES Check.h Check_Console.h DESTINATION "${CMAKE_INSTALL_PREFIX}/include/f4t/check")
