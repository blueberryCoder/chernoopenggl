# Pitfalls Log

## Depth Buffer Not Cleared Causes Flicker
- **Context:** `TestAdvanced` enabled `GL_DEPTH_TEST` while rendering point sprites and face-based quads via `res/shaders/test_fg_facing.shader`.
- **Symptom:** The textured cube driven by `test_fg_facing.shader` flickered between front/back textures despite a stable camera.
- **Root Cause:** The renderer was only clearing the color buffer (`glClear(GL_COLOR_BUFFER_BIT)`), so stale depth values from previous frames caused fragments to intermittently fail depth tests.
- **Fix:** Add a depth-clear helper (`Renderer::ClearDepth`) and clear the depth buffer when depth testing is enabled, or update `Clear` to include `GL_DEPTH_BUFFER_BIT`.
