
set(f4t_sources
    win32/win32.cpp
)

set(f4t_headers
    framework.h
    config/compiler.h
    config/platform.h
    f4t.h
)

add_library(f4t SHARED
    ${f4t_sources}
    ${f4t_headers}
)
target_include_directories(f4t PUBLIC ${CMAKE_SOURCE_DIR})


add_subdirectory(f4t_catch)

