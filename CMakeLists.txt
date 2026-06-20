cmake_minimum_required(VERSION 3.18)

project(myopengl)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
# xcrun --sdk macosx --show-sdk-path
execute_process(COMMAND xcrun --sdk macosx --show-sdk-path OUTPUT_VARIABLE MY_SYSROOT ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Using ${MY_SYSROOT}")
set(CMAKE_OSX_SYSROOT ${MY_SYSROOT})

include_directories()
find_package(PkgConfig REQUIRED)
pkg_check_modules(glfw3 REQUIRED IMPORTED_TARGET glfw3)
pkg_check_modules(glew REQUIRED IMPORTED_TARGET glew)

set(ENV{PKG_CONFIG_PATH} "vcpkg_installed/arm64-osx/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
pkg_check_modules(assimp REQUIRED IMPORTED_TARGET assimp)


set(CMAKE_FIND_FRAMEWORK ONLY)

add_executable(myopengl
        src/Application.cpp
        src/Renderer.h
        src/Renderer.cpp
        src/VertexBuffer.h
        src/VertexBuffer.cpp
        src/IndexBuffer.h
        src/IndexBuffer.cpp
        src/VertexArray.h
        src/VertexArray.cpp
        src/VertexBufferLayout.h
        src/Shader.h
        src/Shader.cpp
        src/Texture.h
        src/Texture.cpp
        src/GLStructures.cpp
        src/GLStructures.h
        src/Model.cpp
        src/Model.h

        vendor/stb_image/stb_image.h
        vendor/stb_image/stb_image.cpp

        vendor/imgui/imgui_impl_glfw_gl3.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_demo.cpp

        src/tests/Test.h
        src/tests/Test.cpp
        src/tests/TestClearColor.h
        src/tests/TestClearColor.cpp
        src/tests/TestTexture2D.h
        src/tests/TestTexture2D.cpp
        src/tests/TestTriangle.h
        src/tests/TestTriangle.cpp
        src/tests/TestBatchVertex.h
        src/tests/TestBatchVertex.cpp
        src/tests/TestBatchTexture.h
        src/tests/TestBatchTexture.cpp
        src/tests/TestBatchDynamic.h
        src/tests/TestBatchDynamic.cpp
        src/tests/TestMeanBlur.h
        src/tests/TestMeanBlur.cpp
        src/tests/TestAntiAliasing.h
        src/tests/TestAntiAliasing.cpp
        src/tests/TestAntiAliasingEffect.h
        src/tests/TestAntiAliasingEffect.cpp
        src/tests/TestShadowMapping.cpp
        src/tests/TestShadowMapping.h
        src/tests/TestShadowMappingPerspective.cpp
        src/tests/TestShadowMappingPerspective.h
        src/tests/TestLisaShadow.cpp
        src/tests/TestLisaShadow.h
        src/tests/TestPointShadow.cpp
        src/tests/TestPointShadow.h
        src/tests/TestAssimp.cpp
        src/tests/TestAssimp.h
        src/tests/TestDepth.cpp
        src/tests/TestDepth.h
        src/tests/TestStencil.h
        src/tests/TestStencil.cpp
        src/tests/TestBlending.h
        src/tests/TestBlending.cpp
        src/tests/TestFaceCulling.cpp
        src/tests/TestFaceCulling.h
        src/tests/TestCubeMap.h
        src/tests/TestCubeMap.cpp
        src/tests/TestAdvanced.cpp
        src/tests/TestAdvanced.h
        src/tests/TestAdvancedLighting.cpp
        src/tests/TestAdvancedLighting.h
        src/tests/TestHDR.cpp
        src/tests/TestHDR.h
        src/tests/TestSpecularIBL.cpp
        src/tests/TestSpecularIBL.h
        src/tests/TestDiffuseIBL.cpp
        src/tests/TestDiffuseIBL.h
        src/tests/TestIBL.cpp
        src/tests/TestIBL.h
        src/tests/TestPBR.cpp
        src/tests/TestPBR.h
        src/tests/TestPBRTexture.cpp
        src/tests/TestPBRTexture.h
        src/tests/TestBloom.cpp
        src/tests/TestBloom.h
        src/tests/TestDeferredShading.cpp
        src/tests/TestDeferredShading.h
        src/tests/TestSSAOLighting.cpp
        src/tests/TestSSAOLighting.h
        src/tests/TestDeferredShadingVolums.cpp
        src/tests/TestDeferredShadingVolums.h
        src/tests/TestSSAO.cpp
        src/tests/TestSSAO.h
        src/tests/TestNormalMapping.cpp
        src/tests/TestNormalMapping.h
        src/tests/TestParallaxMapping.cpp
        src/tests/TestParallaxMapping.h
        src/tests/TestNormalModel.cpp
        src/tests/TestNormalModel.h
        src/tests/TestGeometry.cpp
        src/tests/TestGeometry.h
        src/tests/TestExploding.cpp
        src/tests/TestExploding.h
        src/tests/TestAsteroid.cpp
        src/tests/TestAsteroid.h
        src/tests/TestInstance.cpp
        src/tests/TestInstance.h

        src/FileUtil.cpp
        src/FileUtil.h
        src/Camera.cpp
        src/Camera.h
        src/WindowManager.cpp
        src/WindowManager.h
        src/FrameBuffer.cpp
        src/FrameBuffer.h
        src/RenderBuffer.cpp
        src/RenderBuffer.h
        src/UniformBuffer.cpp
        src/UniformBuffer.h

)

#find_package(assimp CONFIG REQUIRED)
#target_link_libraries(myopengl PRIVATE assimp::assimp)

# mac
target_link_libraries(myopengl PRIVATE "-framework Cocoa" "-framework IOKit" "-framework OpenGL")
target_link_libraries(myopengl PRIVATE PkgConfig::glfw3 PkgConfig::glew PkgConfig::assimp)

target_include_directories(myopengl PRIVATE "vendor")
target_include_directories(myopengl PRIVATE "src")
target_include_directories(myopengl PRIVATE "src/tests")
