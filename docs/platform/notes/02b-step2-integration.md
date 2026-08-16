# Step 2 — GL→D3D9 emulator: deliverables & integration checklist

Reference implementation written (Xbox-side; not built by the j2x repo — they need the XDK's d3d9 +
the platform layer, so they're verified by building the Xbox project, not the converter):
- `GL11.reference.h` / `GL11.reference.cpp` — the OpenGL→D3D9 emulator (matrices, state, color,
  textures, the vertex-array hot path → `DrawPrimitiveUP` via `Shaders`, fog, alpha-ref, display
  lists). Built directly on the old port's proven `Shaders.cpp` + `Tesselator.cpp` mechanism.
- `GLU.reference.cpp` — `gluPerspective` (→ `D3DXMatrixPerspectiveFovLH`) + `gluErrorString`.
- GL12: no functions used — only the `GL_RESCALE_NORMAL` enum token (a `glEnable` no-op).

## Integration steps (in the Xbox VS project)
1. Drop `GL11.h`/`GL11.cpp` (+ a small `GLU.h`/`GLU.cpp`, `GL12.h` with the one token) in, REPLACING
   the converter's no-op stub headers for those names. Call `GL11::glInit()` once after the D3D
   device is up (in Main, after `InitD3D`).
2. **Prerequisite — real NIO buffers (DELIVERED in Step 5).** The converter's
   `ByteBuffer`/`FloatBuffer`/`IntBuffer` are no-op stubs, but the Tessellator packs vertex data
   *through* them and the emulator reads it back. `NIOBuffers.reference.h/.cpp` provides the real
   byte-backed buffers (contiguous backing, `position()/limit()/clear()/flip()`, `put`/`get`,
   `asIntBuffer()/asFloatBuffer()` views over the SAME bytes, `char* base()`). NOTE: `base()` now
   folds in the current byte position, so the emulator's gather uses `((ByteBuffer*)buf)->base()`
   alone (this file was updated to match). See `05b-step5-integration.md`.
3. **Textures registry methods** (overlaps Step 3): the emulator calls `Textures::genId()`,
   `deleteId(id)`, `getById(id)`, `uploadRGBA(id,w,h,px)`, `uploadSubRGBA(id,x,y,w,h,px)`. The old
   port's `Textures.cpp` has the loader; add these id/upload entry points.
4. Build the Xbox project (XDK). Fix any LWJGL-overload mismatches the compiler flags — the emulator
   covers the overloads the new Tessellator + game use (raw-offset + NIO-buffer forms); add any
   stragglers as thin wrappers.

## HARDWARE-VERIFY (can't be checked off-device — iterate on the 360)
- **Projection handedness / winding.** GL is RH, D3D LH. The emulator uses reversed triangle winding
  (CW cull, matching `Tesselator::flush`). If the world renders **mirrored**, negate `m._11` in
  `GLU::gluPerspective` and re-check `D3DRS_CULLMODE`. If faces are **inside-out** (backface showing),
  flip the winding in `glDrawArrays` or the cull mode.
- **Matrix order.** Emulator uses `top = delta * top` (derivation in GL11.cpp). If models are
  translated/rotated wrong, the multiply order or a transpose is off.
- **Color.** RGBA→ARGB swizzle in `swizzleRGBAtoARGB`. If red/blue are swapped on screen, adjust it.
- **2D GUI (`glOrtho`).** GL is y-up, D3D y-down. If the HUD/menus are upside-down, swap `t`/`b` (or
  negate) in `glOrtho`.
- **Display lists.** Currently re-tessellate every frame (correct, slower). If chunk draw is too slow,
  upgrade `glNewList`/`glCallList` to record/replay into D3D vertex buffers.

## Status
Core emulator + GLU delivered as reference. Remaining for a first rendered frame: real NIO buffers
(step above) + Textures id/upload methods (Step 3) + building/iterating the Xbox project on hardware.
