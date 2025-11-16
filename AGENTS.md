# Repository Guidelines

## Project Structure & Module Organization
- `src/` holds the C++14 core, renderer wrappers, and the ImGui-powered driver; list new translation units in `src/CMakeLists.txt`.
- `src/tests/` contains runnable samples (`TestAdvanced`, `TestCubeMap`, etc.); mirror an existing test to hook into `test::TestMenu`.
- `res/` stores shaders, textures, and models referenced through `../../res`.
- `vendor/` and `vcpkg_installed/` provide GLFW, GLEW, ImGui, glm, and Assimp; keep these mirrors read-only.

## Build, Test, and Development Commands
- `brew install pkg-config glfw glew` — macOS prerequisites so pkg-config can discover frameworks.
- `vcpkg install` — resolves the libs declared in `vcpkg.json` into `vcpkg_installed/`.
- `cmake -S src -B build -DCMAKE_BUILD_TYPE=Debug` — configures and captures the pkg-config/vcpkg hints.
- `cmake --build build` — produces the `myopengl` binary.
- `./build/myopengl` — opens the ImGui menu; select a test (e.g., "advanced") to validate behavior.

## Coding Style & Naming Conventions
- Keep 4-space indentation, brace-on-same-line layout, and early returns (see `src/Application.cpp`).
- Use PascalCase class names with matching filenames; functions stay lowerCamelCase; constants/macros remain ALL_CAPS (`GLCall`).
- Prefer the bundled RAII helpers (`VertexArray`, `FrameBuffer`) rather than raw GL calls so cleanup remains centralized.
- Store shaders under `res/shaders` with descriptive snake_case filenames and group related assets nearby.

## Testing Guidelines
- Tests are self-contained scenes derived from `test::Test` (`src/tests/Test.h`); keep GL state scoped so the next scene starts clean.
- Whenever a test enables a GL capability (depth test, blending, polygon mode, etc.) in its constructor, disable or restore it in the destructor so other scenes are unaffected.
- Name new tests `Test<Feature>` and register them via `testMenu->RegisterTest` in `Application.cpp`.
- Run `./build/myopengl`, open the new scene, and capture logs or screenshots for your PR.

## Commit & Pull Request Guidelines
- Match the conventional commit style in `git log` (`feat: add reflect`, `feat: cube map, …`), adjusting the prefix (`fix:`, `refactor:`) to reflect scope.
- Rebase before opening a PR, summarize the change set, link any tutorials or issues, and include screenshots for rendering changes.
- Document manual validation steps (command + selected test) so reviewers can replay the work quickly.

## Configuration & Asset Tips
- Keep large binaries in `prebuilt/` and call them out in the PR summary.
- Update `vcpkg.json` plus README instructions when you add dependencies, and regenerate `vcpkg_installed/` with `vcpkg install`.
- Use only redistributable assets stored under `res/` so `FileUtil` can locate them relative to the executable.
