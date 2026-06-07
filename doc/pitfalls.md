# Pitfalls Log

## Depth Buffer Not Cleared Causes Flicker
- **Context:** `TestAdvanced` enabled `GL_DEPTH_TEST` while rendering point sprites and face-based quads via `res/shaders/test_fg_facing.shader`.
- **Symptom:** The textured cube driven by `test_fg_facing.shader` flickered between front/back textures despite a stable camera.
- **Root Cause:** The renderer was only clearing the color buffer (`glClear(GL_COLOR_BUFFER_BIT)`), so stale depth values from previous frames caused fragments to intermittently fail depth tests.
- **Fix:** Add a depth-clear helper (`Renderer::ClearDepth`) and clear the depth buffer when depth testing is enabled, or update `Clear` to include `GL_DEPTH_BUFFER_BIT`.

## BRDF LUT Generation Broken By Global Blending
- **Context:** `TestSpecularIBL` generates a BRDF integration LUT into an offscreen `GL_RG16F` texture in `CaptureBrdfLutTexture`, while `Application.cpp` globally enables blending for the whole app.
- **Symptom:** The `Specular IBL` scene rendered abnormally only when the real `brdfLUT` was used. The same scene looked correct when using diffuse IBL only, or when replacing the LUT with a constant `vec2(1.0, 0.0)` texture.
- **Root Cause:** The BRDF LUT render pass inherited the app-level `GL_BLEND` state. That offscreen pass should write deterministic integration results directly into the LUT texture, but blending corrupted the generated texels.
- **Fix:** In `TestSpecularIBL::CaptureBrdfLutTexture`, save the current blend-enable state, temporarily disable `GL_BLEND` for the LUT pass, then restore the previous state after the FBO render completes.
