cmake_minimum_required(VERSION 3.14)

project(myopengl)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

include_directories()
find_package(PkgConfig REQUIRED)
pkg_check_modules(glfw3 REQUIRED IMPORTED_TARGET glfw3)
pkg_check_modules(glew REQUIRED IMPORTED_TARGET glew)

set(CMAKE_FIND_FRAMEWORK ONLY)

add_executable(myopengl Application.cpp Renderer.h Renderer.cpp VertexBuffer.h VertexBuffer.cpp IndexBuffer.h
        IndexBuffer.cpp VertexArray.h VertexArray.cpp VertexBufferLayout.h Shader.cpp Shader.h
        ../vendor/stb_image/stb_image.h
        ../vendor/stb_image/stb_image.cpp

        ../vendor/imgui/imgui_impl_glfw_gl3.cpp
        ../vendor/imgui/imgui_draw.cpp
        ../vendor/imgui/imgui.cpp
        ../vendor/imgui/imgui_demo.cpp

        Texture.h
        Texture.cpp tests/Test.h tests/TestClearColor.h tests/TestClearColor.cpp)
# mac
target_link_libraries(myopengl PRIVATE "-framework Cocoa" "-framework IOKit" "-framework OpenGL")
target_link_libraries(myopengl PRIVATE PkgConfig::glfw3 PkgConfig::glew)
target_include_directories(myopengl PRIVATE "../vendor")
target_include_directories(myopengl PRIVATE "tests")
