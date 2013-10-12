
include_guard(GLOBAL)

include(ExternalProject)

ExternalProject_Add(catch2-git-repo
    GIT_REPOSITORY https://github.com/catchorg/catch2.git
    GIT_TAG v2.13.8
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "")

ExternalProject_Get_property(catch2-git-repo SOURCE_DIR)

add_library(Catch2 INTERFACE)
add_dependencies(Catch2 catch2-git-repo)
target_include_directories(Catch2 SYSTEM INTERFACE ${SOURCE_DIR}/single_include)
