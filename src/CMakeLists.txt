cmake_minimum_required(VERSION 3.14)

project(myopengl)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

include_directories()
find_package(PkgConfig REQUIRED)
pkg_check_modules(glfw3 REQUIRED IMPORTED_TARGET glfw3)
pkg_check_modules(glew REQUIRED IMPORTED_TARGET glew)

set(CMAKE_FIND_FRAMEWORK ONLY)

add_executable(myopengl
        Application.cpp
        Renderer.h
        Renderer.cpp
        VertexBuffer.h
        VertexBuffer.cpp
        IndexBuffer.h
        IndexBuffer.cpp
        VertexArray.h
        VertexArray.cpp
        VertexBufferLayout.h
        Shader.h
        Shader.cpp
        Texture.h
        Texture.cpp

        ../vendor/stb_image/stb_image.h
        ../vendor/stb_image/stb_image.cpp

        ../vendor/imgui/imgui_impl_glfw_gl3.cpp
        ../vendor/imgui/imgui_draw.cpp
        ../vendor/imgui/imgui.cpp
        ../vendor/imgui/imgui_demo.cpp

        tests/Test.h
        tests/Test.cpp
        tests/TestClearColor.h
        tests/TestClearColor.cpp
        tests/TestTexture2D.h
        tests/TestTexture2D.cpp
        tests/TestTriangle.h
        tests/TestTriangle.cpp
        tests/TestBatchVertex.h
        tests/TestBatchVertex.cpp
        tests/TestBatchTexture.h
        tests/TestBatchTexture.cpp
        tests/TestBatchDynamic.h
        tests/TestBatchDynamic.cpp
        )
# mac
target_link_libraries(myopengl PRIVATE "-framework Cocoa" "-framework IOKit" "-framework OpenGL")
target_link_libraries(myopengl PRIVATE PkgConfig::glfw3 PkgConfig::glew)
target_include_directories(myopengl PRIVATE "../vendor")
target_include_directories(myopengl PRIVATE "tests")

