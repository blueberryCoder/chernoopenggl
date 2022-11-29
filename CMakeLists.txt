cmake_minimum_required(VERSION 3.14)

project(myopengl)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)
pkg_check_modules(glfw3 REQUIRED IMPORTED_TARGET glfw3)
pkg_check_modules(glew REQUIRED IMPORTED_TARGET glew)

set(CMAKE_FIND_FRAMEWORK ONLY)

add_executable(myopengl Application.cpp)
# mac
target_link_libraries(myopengl PRIVATE "-framework Cocoa" "-framework IOKit" "-framework OpenGL")
target_link_libraries(myopengl PRIVATE PkgConfig::glfw3 PkgConfig::glew)

