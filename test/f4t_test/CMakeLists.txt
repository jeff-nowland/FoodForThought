

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src")
ADD_EXECUTABLE(f4t_test
	main.cpp
	view_test.cpp)

TARGET_LINK_LIBRARIES (f4t_test rt)
TARGET_LINK_LIBRARIES (f4t_test f4tcheck f4t)

