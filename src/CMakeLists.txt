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

set(ENV{PKG_CONFIG_PATH} "../vcpkg_installed/arm64-osx/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
pkg_check_modules(assimp REQUIRED IMPORTED_TARGET assimp)


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
        GLStructures.cpp
        GLStructures.h
        Model.cpp
        Model.h

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
        tests/TestMeanBlur.h
        tests/TestMeanBlur.cpp
        tests/TestAntiAliasing.h
        tests/TestAntiAliasing.cpp
        tests/TestAntiAliasingEffect.h
        tests/TestAntiAliasingEffect.cpp
        tests/TestShadowMapping.cpp
        tests/TestShadowMapping.h
        tests/TestShadowMappingPerspective.cpp
        tests/TestShadowMappingPerspective.h
        tests/TestLisaShadow.cpp
        tests/TestLisaShadow.h
        tests/TestPointShadow.cpp
        tests/TestPointShadow.h
        tests/TestAssimp.cpp
        tests/TestAssimp.h
        tests/TestCanonicalFace.cpp
        tests/TestCanonicalFace.h
        tests/TestDepth.cpp
        tests/TestDepth.h
        tests/TestStencil.h
        tests/TestStencil.cpp
        tests/TestBlending.h
        tests/TestBlending.cpp
        tests/TestFaceCulling.cpp
        tests/TestFaceCulling.h
        tests/TestCubeMap.h
        tests/TestCubeMap.cpp
        tests/TestCubeMap.cpp
        tests/TestCubeMap.h
        tests/TestAdvanced.cpp
        tests/TestAdvanced.h
        tests/TestAdvancedLighting.cpp
        tests/TestAdvancedLighting.h
        tests/TestHDR.cpp
        tests/TestHDR.h
        tests/TestSpecularIBL.cpp
        tests/TestSpecularIBL.h
        tests/TestDiffuseIBL.cpp
        tests/TestDiffuseIBL.h
        tests/TestIBL.cpp
        tests/TestIBL.h
        tests/TestPBR.cpp
        tests/TestPBR.h
        tests/TestPBRTexture.cpp
        tests/TestPBRTexture.h
        tests/TestBloom.cpp
        tests/TestBloom.h
        tests/TestDeferredShading.cpp
        tests/TestDeferredShading.h
        tests/TestSSAOLighting.cpp
        tests/TestSSAOLighting.h
        tests/TestDeferredShadingVolums.cpp
        tests/TestDeferredShadingVolums.h
        tests/TestSSAO.cpp
        tests/TestSSAO.h
        tests/TestNormalMapping.cpp
        tests/TestNormalMapping.h
        tests/TestParallaxMapping.cpp
        tests/TestParallaxMapping.h
        tests/TestNormalModel.cpp
        tests/TestNormalModel.h
        tests/TestGeometry.cpp
        tests/TestGeometry.h
        tests/TestExploding.cpp
        tests/TestExploding.h
        tests/TestAsteroid.cpp
        tests/TestAsteroid.h
        tests/TestInstance.cpp
        tests/TestInstance.h

        FileUtil.cpp
        FileUtil.h
        Camera.cpp
        Camera.h
        WindowManager.cpp
        WindowManager.h
        FrameBuffer.cpp
        FrameBuffer.h
        RenderBuffer.cpp
        RenderBuffer.h
        UniformBuffer.cpp
        UniformBuffer.h

)

#find_package(assimp CONFIG REQUIRED)
#target_link_libraries(myopengl PRIVATE assimp::assimp)

# mac
target_link_libraries(myopengl PRIVATE "-framework Cocoa" "-framework IOKit" "-framework OpenGL")
target_link_libraries(myopengl PRIVATE PkgConfig::glfw3 PkgConfig::glew PkgConfig::assimp)

target_include_directories(myopengl PRIVATE "../vendor")
target_include_directories(myopengl PRIVATE "tests")
