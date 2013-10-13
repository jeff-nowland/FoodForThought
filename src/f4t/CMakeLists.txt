


ADD_LIBRARY (f4t SHARED
	Problem.cpp
	Element.cpp)

INSTALL (TARGETS f4t LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

INSTALL (FILES
	f4t.h
	Util.h
	View.h
	Virtual_Buffer.h
	Problem.h
	Element.h
	DESTINATION "${CMAKE_INSTALL_PREFIX}/include/f4t")
