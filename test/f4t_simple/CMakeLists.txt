

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src")
ADD_EXECUTABLE(f4t_simple_test main.cpp)

TARGET_LINK_LIBRARIES (f4t_simple_test rt)
TARGET_LINK_LIBRARIES (f4t_simple_test f4tcheck)

