
CMAKE_MINIMUM_REQUIRED	(VERSION 2.8 FATAL_ERROR)
CMAKE_POLICY (VERSION 2.8)

PROJECT (FoodForThought)
SET (VERSION "0")
SET (SUBVERSION "1")

IF	(NOT CMAKE_BUILD_TYPE)
  SET (CMAKE_BUILD_TYPE RELEASE CACHE STRING "Choose the type of build, options are: DEBUG RELEASE" FORCE)
ENDIF (NOT CMAKE_BUILD_TYPE)

IF ((NOT CMAKE_BUILD_TYPE STREQUAL DEBUG) AND (NOT CMAKE_BUILD_TYPE STREQUAL RELEASE))
	MESSAGE (STATUS "Unknown CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
	MESSAGE (STATUS "Supported types: DEBUG RELEASE XWIN32")
	MESSAGE (FATAL_ERROR "Aborting")
ENDIF ((NOT CMAKE_BUILD_TYPE STREQUAL DEBUG) AND (NOT CMAKE_BUILD_TYPE STREQUAL RELEASE))

ADD_DEFINITIONS (-DF4T_ROOT_DIR="${PROJECT_SOURCE_DIR}")
ADD_DEFINITIONS(-DF4T_VERSION="${VERSION}")

IF (CMAKE_BUILD_TYPE STREQUAL RELEASE)
	MESSAGE (STATUS "Release build: stripping executables")
	SET (CPACK_STRIP_FILES true)
ELSE (CMAKE_BUILD_TYPE STREQUAL RELEASE)
	SET (CPACK_STRIP_FILES false)
ENDIF (CMAKE_BUILD_TYPE STREQUAL RELEASE)

INCLUDE (TestCXXAcceptsFlag)

IF (CMAKE_COMPILER_IS_GNUCXX)
	MESSAGE (STATUS "Compiler is GNU C++")
	CHECK_CXX_ACCEPTS_FLAG (-std=c++11 CXX_C11)
	IF (CXX_C11)
		SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	ELSE (CXX_C11)
		MESSAGE(FATAL_ERROR, "G++ needs to be at least 4.7")
	ENDIF (CXX_C11)
ELSE (CMAKE_COMPILER_IS_GNUCXX)
	MESSAGE (FATAL_ERROR, "Compiler is not GNU C++")
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

# For the moment all i need is lexical_cast, but i do need it
INCLUDE (FindBoost)

IF (Boost_FOUND)
	IF (NOT Boost_INCLUDE_DIRS STREQUAL "/usr/include")
		SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${Boost_INCLUDE_DIRS}")
	ENDIF (NOT Boost_INCLUDE_DIRS STREQUAL "/usr/include")
ELSE (Boost_FOUND)
	MESSAGE (FATAL_ERROR, "Cannot find BOOST")
ENDIF (Boost_FOUND)


MESSAGE (STATUS "Current build type: " ${CMAKE_BUILD_TYPE})
MESSAGE (STATUS "Install prefix: " ${CMAKE_INSTALL_PREFIX})

ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (test)
