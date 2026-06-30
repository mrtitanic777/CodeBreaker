# Step 3 — Textures & resources: deliverables & integration

The new RenderEngine (override) IS the texture manager. Its path:
`getResourceAsStream(name)` → `ImageIO::read(stream)` → `BufferedImage` → `getRGB()` →
`glBindTexture`/`glTexImage2D`/`glTexSubImage2D` → (GL11 emulator) → `Textures::uploadRGBA`.

Reference implementations (Xbox-side; not built by j2x):
- `Textures.ext.reference.cpp` — `genId`/`deleteId`/`uploadRGBA`/`uploadSubRGBA` to ADD to the old
  port's `Textures` class (it already has `load`/`getById`/`bind`/`s_textures[128]`). `uploadRGBA`
  creates an A8R8G8B8 MANAGED texture and copies ARGB pixels (1:1, no swizzle); `uploadSubRGBA` does
  the animated terrain/water/lava cells + the dynamic light map.
- `ImageIO.reference.cpp` (+ the real `BufferedImage` sketched in its header comment) — REPLACES the
  no-op ImageIO/BufferedImage stubs. `ImageIO::read` pulls bytes from the InputStream and decodes the
  PNG via D3DX (`D3DXLoadSurfaceFromFileInMemory` into a scratch A8R8G8B8 surface → lock → copy).
  `BufferedImage` holds a real `JArray<int>` of ARGB pixels with `getRGB`/`setRGB`.

## Integration steps
1. Make `BufferedImage` real (per the header comment in ImageIO.reference.cpp), replacing the stub.
2. Add `ImageIO::read(InputStream*)` (D3DX decode). Replace the stub ImageIO.
3. Add the four `Textures` methods to the Xbox `Textures.{h,cpp}`.
4. Assets already ship as loose PNGs under `game:\Content\…` (old port) — keep that layout.

## Dependency (RESOLVED in Step 5)
`Class::getResourceAsStream(name)` must resolve `name` (e.g. `/terrain.png`, `/gui/gui.png`) to
`game:\Content\<name>` and return a real **InputStream**. Step 5 delivers this:
`FileIO.reference.h/.cpp` provides `J2X_getResourceAsStream(name)` (leading `/` dropped, `/`→`\`,
prefix `game:\Content`, real `FileInputStream`) and a real `InputStream::read(buf,off,len)` — wire
`Class::getResourceAsStream` to `J2X_getResourceAsStream`. `ImageIO::readFile(path)` (provided) is no
longer needed once that's live; the old port's `Textures::load(path)` also still works for the four
boot textures loaded in Main. See `05b-step5-integration.md`.

## HARDWARE-VERIFY
- Color order: D3D A8R8G8B8 == Java ARGB (0xAARRGGBB) — no swizzle expected; if textures look
  channel-swapped, the decode surface format differs — force A8R8G8B8.
- Animated cells / light map go through `glTexSubImage2D` → `uploadSubRGBA`; verify the sub-rect
  offsets line up (terrain.png is a 16×16 tile atlas).
