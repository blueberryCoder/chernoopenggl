# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.24.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.24.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/bytedance/code/my/cherno/OpenGL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/bytedance/code/my/cherno/OpenGL/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/myopengl.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/myopengl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myopengl.dir/flags.make

CMakeFiles/myopengl.dir/Application.cpp.o: CMakeFiles/myopengl.dir/flags.make
CMakeFiles/myopengl.dir/Application.cpp.o: /Users/bytedance/code/my/cherno/OpenGL/Application.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/bytedance/code/my/cherno/OpenGL/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/myopengl.dir/Application.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myopengl.dir/Application.cpp.o -c /Users/bytedance/code/my/cherno/OpenGL/Application.cpp

CMakeFiles/myopengl.dir/Application.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myopengl.dir/Application.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/bytedance/code/my/cherno/OpenGL/Application.cpp > CMakeFiles/myopengl.dir/Application.cpp.i

CMakeFiles/myopengl.dir/Application.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myopengl.dir/Application.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/bytedance/code/my/cherno/OpenGL/Application.cpp -o CMakeFiles/myopengl.dir/Application.cpp.s

CMakeFiles/myopengl.dir/Renderer.cpp.o: CMakeFiles/myopengl.dir/flags.make
CMakeFiles/myopengl.dir/Renderer.cpp.o: /Users/bytedance/code/my/cherno/OpenGL/Renderer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/bytedance/code/my/cherno/OpenGL/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/myopengl.dir/Renderer.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myopengl.dir/Renderer.cpp.o -c /Users/bytedance/code/my/cherno/OpenGL/Renderer.cpp

CMakeFiles/myopengl.dir/Renderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myopengl.dir/Renderer.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/bytedance/code/my/cherno/OpenGL/Renderer.cpp > CMakeFiles/myopengl.dir/Renderer.cpp.i

CMakeFiles/myopengl.dir/Renderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myopengl.dir/Renderer.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/bytedance/code/my/cherno/OpenGL/Renderer.cpp -o CMakeFiles/myopengl.dir/Renderer.cpp.s

CMakeFiles/myopengl.dir/VertexBuffer.cpp.o: CMakeFiles/myopengl.dir/flags.make
CMakeFiles/myopengl.dir/VertexBuffer.cpp.o: /Users/bytedance/code/my/cherno/OpenGL/VertexBuffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/bytedance/code/my/cherno/OpenGL/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/myopengl.dir/VertexBuffer.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myopengl.dir/VertexBuffer.cpp.o -c /Users/bytedance/code/my/cherno/OpenGL/VertexBuffer.cpp

CMakeFiles/myopengl.dir/VertexBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myopengl.dir/VertexBuffer.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/bytedance/code/my/cherno/OpenGL/VertexBuffer.cpp > CMakeFiles/myopengl.dir/VertexBuffer.cpp.i

CMakeFiles/myopengl.dir/VertexBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myopengl.dir/VertexBuffer.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/bytedance/code/my/cherno/OpenGL/VertexBuffer.cpp -o CMakeFiles/myopengl.dir/VertexBuffer.cpp.s

CMakeFiles/myopengl.dir/IndexBuffer.cpp.o: CMakeFiles/myopengl.dir/flags.make
CMakeFiles/myopengl.dir/IndexBuffer.cpp.o: /Users/bytedance/code/my/cherno/OpenGL/IndexBuffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/bytedance/code/my/cherno/OpenGL/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/myopengl.dir/IndexBuffer.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myopengl.dir/IndexBuffer.cpp.o -c /Users/bytedance/code/my/cherno/OpenGL/IndexBuffer.cpp

CMakeFiles/myopengl.dir/IndexBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myopengl.dir/IndexBuffer.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/bytedance/code/my/cherno/OpenGL/IndexBuffer.cpp > CMakeFiles/myopengl.dir/IndexBuffer.cpp.i

CMakeFiles/myopengl.dir/IndexBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myopengl.dir/IndexBuffer.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/bytedance/code/my/cherno/OpenGL/IndexBuffer.cpp -o CMakeFiles/myopengl.dir/IndexBuffer.cpp.s

CMakeFiles/myopengl.dir/VertexArray.cpp.o: CMakeFiles/myopengl.dir/flags.make
CMakeFiles/myopengl.dir/VertexArray.cpp.o: /Users/bytedance/code/my/cherno/OpenGL/VertexArray.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/bytedance/code/my/cherno/OpenGL/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/myopengl.dir/VertexArray.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myopengl.dir/VertexArray.cpp.o -c /Users/bytedance/code/my/cherno/OpenGL/VertexArray.cpp

CMakeFiles/myopengl.dir/VertexArray.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myopengl.dir/VertexArray.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/bytedance/code/my/cherno/OpenGL/VertexArray.cpp > CMakeFiles/myopengl.dir/VertexArray.cpp.i

CMakeFiles/myopengl.dir/VertexArray.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myopengl.dir/VertexArray.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/bytedance/code/my/cherno/OpenGL/VertexArray.cpp -o CMakeFiles/myopengl.dir/VertexArray.cpp.s

CMakeFiles/myopengl.dir/Shader.cpp.o: CMakeFiles/myopengl.dir/flags.make
CMakeFiles/myopengl.dir/Shader.cpp.o: /Users/bytedance/code/my/cherno/OpenGL/Shader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/bytedance/code/my/cherno/OpenGL/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/myopengl.dir/Shader.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myopengl.dir/Shader.cpp.o -c /Users/bytedance/code/my/cherno/OpenGL/Shader.cpp

CMakeFiles/myopengl.dir/Shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myopengl.dir/Shader.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/bytedance/code/my/cherno/OpenGL/Shader.cpp > CMakeFiles/myopengl.dir/Shader.cpp.i

CMakeFiles/myopengl.dir/Shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myopengl.dir/Shader.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/bytedance/code/my/cherno/OpenGL/Shader.cpp -o CMakeFiles/myopengl.dir/Shader.cpp.s

# Object files for target myopengl
myopengl_OBJECTS = \
"CMakeFiles/myopengl.dir/Application.cpp.o" \
"CMakeFiles/myopengl.dir/Renderer.cpp.o" \
"CMakeFiles/myopengl.dir/VertexBuffer.cpp.o" \
"CMakeFiles/myopengl.dir/IndexBuffer.cpp.o" \
"CMakeFiles/myopengl.dir/VertexArray.cpp.o" \
"CMakeFiles/myopengl.dir/Shader.cpp.o"

# External object files for target myopengl
myopengl_EXTERNAL_OBJECTS =

myopengl: CMakeFiles/myopengl.dir/Application.cpp.o
myopengl: CMakeFiles/myopengl.dir/Renderer.cpp.o
myopengl: CMakeFiles/myopengl.dir/VertexBuffer.cpp.o
myopengl: CMakeFiles/myopengl.dir/IndexBuffer.cpp.o
myopengl: CMakeFiles/myopengl.dir/VertexArray.cpp.o
myopengl: CMakeFiles/myopengl.dir/Shader.cpp.o
myopengl: CMakeFiles/myopengl.dir/build.make
myopengl: /usr/local/lib/libglfw3.a
myopengl: /usr/local/lib/libGLEW.dylib
myopengl: CMakeFiles/myopengl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/bytedance/code/my/cherno/OpenGL/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable myopengl"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/myopengl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myopengl.dir/build: myopengl
.PHONY : CMakeFiles/myopengl.dir/build

CMakeFiles/myopengl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/myopengl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/myopengl.dir/clean

CMakeFiles/myopengl.dir/depend:
	cd /Users/bytedance/code/my/cherno/OpenGL/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/bytedance/code/my/cherno/OpenGL /Users/bytedance/code/my/cherno/OpenGL /Users/bytedance/code/my/cherno/OpenGL/cmake-build-debug /Users/bytedance/code/my/cherno/OpenGL/cmake-build-debug /Users/bytedance/code/my/cherno/OpenGL/cmake-build-debug/CMakeFiles/myopengl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/myopengl.dir/depend

