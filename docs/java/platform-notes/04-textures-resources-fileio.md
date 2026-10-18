# 04 — Textures / Resources & File I/O (Xbox 360 platform layer)

Reverse-engineered from the OLD buggy reference port
`MinecraftAlpha/MinecraftAlpha/` (XDK / D3D9, ~379 .cpp) so we can re-implement the
same behaviour behind the NEW transpiler's symbol-only stubs in
`J2CTool/j2ctool/runtime.py`.

Java surface the NEW code emits → what the OLD port actually does on the 360.

---

## Overview

The OLD port does **not** faithfully reproduce Java's `ImageIO`/`BufferedImage`/`java.io`
pipeline. Instead it replaces those subsystems with hand-written Xbox/D3D9 glue and
keeps the converted *callers* working by reimplementing the converter-generated
interfaces as thin shims:

- **Textures**: Java `RenderEngine` (AWT `BufferedImage` + `ImageIO` + int-handle
  registry) is shimmed onto a flat `Textures` class that loads `.png` files straight
  off the game disc with `D3DXCreateTextureFromFileExA`. No `BufferedImage` ever
  exists; the int texture id is just an index into a fixed array of
  `IDirect3DTexture9*`.
- **Assets** (`terrain.png`, `gui/`, `mob/char.png`, fonts, …) ship **loose as PNG**
  under `game:\Content\…` (the read-only title/DVD image), **not** embedded in the XEX
  and **not** converted to DDS.
- **World save/load**: Java NBT + per-chunk `.dat` files are kept, but
  **uncompressed** (the gzip layer is dropped) and **base-36 chunk dir layout** is
  preserved. Files live on a **mounted STFS saved-game container** (`Save:\…`), driven
  by `Save360.cpp` (XContent APIs + signin/device-selector UI).
- **Java `java.io` surface** (`File`, `FileInputStream/OutputStream`,
  `DataInput/Output`, `BufferedReader`) is backed by a single bridge header
  **`JavaIO.h`** that wraps Win32 `CreateFile`/`ReadFile`/`WriteFile` and a
  big-endian byte-buffer codec.
- **`ZipFile`** (texture packs): **not used** — single built-in pack only.

The single most important platform fact: **Xbox 360 is big-endian PowerPC**, and the
OLD port handles NBT/binary I/O by reading/writing **byte-at-a-time big-endian**
(Java-faithful), so it is *accidentally correct* regardless of host endianness. See
[Known bugs / pitfalls](#known-bugs--pitfalls).

---

## Texture loading

### The path: Java texture-name → Content PNG → `IDirect3DTexture9`

Image bytes come from **loose `.png` files on the game disc** (`game:\Content\…`),
decoded at load time by D3DX. There is no embedded image data, no DDS, no
`BufferedImage`.

The texture registry is `Textures` (flat static arrays), not Java's `RenderEngine`:

- `Textures.h:30` — `MAX_TEXTURES = 128`
- `Textures.h:33-35` — `s_textures[128]` (`IDirect3DTexture9*`), `s_paths[128][256]`
  (the cache key), `s_count`
- `Textures.cpp:25-95` — `Textures::load(path)`:
  - Line 27-31: linear scan of `s_paths` with `_stricmp` → **path-cached**; a repeat
    load returns the existing id (this is what makes the int id behave like Java's
    `textureMap`).
  - Line 42-56: `D3DXCreateTextureFromFileExA(g_pd3dDevice, path, D3DX_DEFAULT,
    D3DX_DEFAULT, 1 mip, 0 usage, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE,
    D3DX_FILTER_NONE, colorkey=0, …)` — forces **32-bit ARGB**, **1 mip level**, **no
    filtering**, and **no color key** (the colorkey arg is `0`; see pitfalls).
  - Line 66-83: **basename fallback** — if the full path fails, retry as
    `game:\Content\<basename>` (recovers assets that were flattened out of a subfolder
    during asset conversion).
- `Textures.cpp:97-100` — `getById(id)` → `s_textures[id]` (bounds-checked, returns
  `NULL` out of range).
- `Textures.cpp:102-108` — `bind(id)` / `bindDirect(tex)` → `g_pd3dDevice->SetTexture(0, …)`.

### The texture-id ↔ D3D9 map

It is just **array index ↔ `s_textures[index]`**. The "Java texture id" returned to
converted code *is* the index into `Textures::s_textures`. There is no separate hash map.

### RenderEngine shim (what converted callers actually hit)

`RenderEngine.cpp` is a hand-written override that forwards everything to `Textures`:

- `RenderEngine.cpp:63-67` — `getTexture(name)` → `Textures::load(toContentPath(name))`.
- `RenderEngine.cpp:113-117` — `bindTexture(id)` → `g_pd3dDevice->SetTexture(0, Textures::getById(id))`.
- `RenderEngine.cpp:22-45` — `toContentPath()` maps a Java resource name to a Content
  path and **strips Java texture-name prefixes**:
  - `"##…"` (animation-strip marker) → strip leading `##`
  - `"%blur%/…"`, `"%clamp%/…"` (filter markers) → strip the leading `%…%` token
  - then prefix `game:\Content`, convert `/` → `\`.
- `RenderEngine.cpp:69-90, 119-137` — the `BufferedImage` upload path
  (`allocateAndSetupTexture`, `setupTexture`, `readTextureImage`,
  `unwrapImageByColumns`) is **stubbed out** (returns `-1`/`NULL`); the 360 never
  builds textures from pixel arrays.
- `RenderEngine.cpp:96-107` — `registerTextureFX` / `func_1067_a` keep the animated
  texture (water/lava/fire) **tick** running, but the per-tile `LockRect` re-upload
  into the atlas is **deferred/cosmetic** (noted as a TODO; FX state advances but the
  atlas isn't repainted).

### Global texture handles (`Common.h`)

`Common.h:9-12` declares `g_terrainTexture`, `g_guiTexture`, `g_charTexture`,
`g_defaultTexture` as raw `IDirect3DTexture9*`. They are loaded once at boot:

- `Main.cpp:32-35` — definitions (init `NULL`).
- `Main.cpp:82-87` — `Textures::init(); Textures::loadAll();` then the four globals are
  assigned via `Textures::getById(Textures::load("game:\Content\terrain.png"))` etc.

### `GLAllocation` (LWJGL/NIO glue)

`GLAllocation.cpp` — display lists and `glGenTextures` have no D3D9 equivalent:

- `generateDisplayLists` (line 32-42) hands back monotonic dummy base handles.
- `generateTextureNames` (line 44-49) is a **no-op** (texture creation is owned by `Textures`).
- `createDirectByteBuffer/IntBuffer/FloatBuffer` (line 58-80) → plain `new
  unsigned char[]` / `new int[]` / `new float[]` (NIO direct buffers become raw C arrays).

---

## Asset packaging

| Question | Answer (OLD port) |
|---|---|
| Where do assets live? | Loose files under **`game:\Content\…`** — the read-only DVD/title image (XGD/`game:` drive), deployed alongside the XEX. |
| Embedded in XEX? | **No.** |
| On HDD? | **No** (HDD is used only for *saves*, via the `Save:` STFS mount). |
| Format | **PNG**, 32-bit RGBA (decoded to `D3DFMT_A8R8G8B8` at load). **No DDS.** |
| Fonts | `game:\Content\mob\char.png` (Minecraft's `default.png`/`char.png` glyph sheet); `FontRenderer.cpp` samples it as a normal texture. |

The full known asset manifest is `Textures::loadAll()` (`Textures.cpp:110-160`), which
pre-loads ~40 PNGs. Notable layout facts encoded there:

- root: `terrain.png`, `default.png`, `particles.png`, `rain.png`, `water.png`,
  `dirt.png`, `grass.png`, `rock.png`, `shadow.png`
- `gui\` : `gui.png`, `icons.png`, `items.png`, `logo.png`, `inventory.png`,
  `container.png`, `crafting.png`
- `mob\` : `char.png`, `creeper.png`, `pig.png`, `sheep.png`, `sheep_fur.png`,
  `skeleton.png`, `spider.png`, `spider_eyes.png`, `zombie.png`
- `item\`, `misc\`, `armor\`, `terrain\` (sun/moon), `environment\clouds.png`
- Comment at `Textures.cpp:117`: clouds live under `environment\`, **not** Content root —
  i.e. the on-disc folder tree mirrors Minecraft's jar layout, with a couple of
  hand-fixed exceptions.

Texture packs: `TexturePackList.cpp` only ever exposes the **built-in default pack**
(`TexturePackBase`); `.zip` scanning and the `texturepacks/` directory are explicitly
removed (`TexturePackList.cpp:8-11, 41, 77-88`). So `ZipFile` is dead on this platform.

---

## World save/load

### NBT model

The full Java NBT tag hierarchy is ported 1:1 (`NBTBase`, `NBTTagCompound`,
`NBTTagList`, `NBTTagByte/Short/Int/Long/Float/Double/ByteArray/String/End`). Tag
dispatch is unchanged from Java:

- `NBTTagCompound.cpp:32-52` — `writeTagContents`/`readTagContents` iterate the map and
  call `NBTBase::writeTag`/`readTag`, terminated by a `0` (End) byte — identical wire
  format to desktop Minecraft.
- `NBTTagCompound.cpp:54-152` — typed getters/setters over a
  `std::map<std::string, void*>* m_tagMap`.

### Per-chunk files — `ChunkLoader.cpp`

- **Layout** (`ChunkLoader.cpp:17, 90-123`): `<saveDir>/<x&63>/<z&63>/c.<x>.<z>.dat`,
  with `<x>`, `<z>`, `<x&63>`, `<z&63>` formatted as **base-36** via
  `intToBase36` (`ChunkLoader.cpp:36-67`) — Alpha's original two-level directory scheme.
- **Save** (`ChunkLoader.cpp:177-207`): builds `Level` compound
  (`storeChunkInCompound`, line 209-252: `Blocks/Data/SkyLight/BlockLight/HeightMap`
  byte arrays + `Entities`/`TileEntities` lists), serializes via
  `CompressedStreamTools::func_1139_a` into a `DataOutput` byte buffer, and writes it
  straight to the file with `FileOutputStream`.
- **Load** (`ChunkLoader.cpp:125-175`): `FileInputStream` reads the whole file into a
  `std::vector<unsigned char>`, then `CompressedStreamTools::func_1140_a(&buf[0])`
  parses raw NBT; validates `Level` + `Blocks` keys, calls
  `loadChunkIntoWorldFromCompound`.
- **Uncompressed**: `CompressedStreamTools.cpp:1-10, 33-43` — the gzip wrappers
  (`func_1138_a` stream reader, `writeGzippedCompoundToOutputStream`) are **NULL
  stubs**; only the raw byte-buffer variants `func_1140_a`/`func_1142_a`/`func_1139_a`
  are used. **Saves are raw NBT, not gzip.** This is self-consistent (the port only
  reads its own saves) but **incompatible with PC `.dat` files.**
- **Simplifications vs Java** (`ChunkLoader.cpp:7-18, 185-188`): the `JavaIO` `File`
  bridge has **no `length()` and no `renameTo()`**, so Java's write-temp-then-rename +
  `sizeOnDisk` bookkeeping is collapsed to a **direct overwrite** of the final file.

### `level.dat` and world container

- `ContentStorage.cpp:25` — world-level data path is **`Save:\level.dat`** (drive form,
  read/written through `CompressedStreamTools` → same raw-NBT path).
- `World.cpp:499, 557, 654` — world create/delete/save are routed through the STFS
  container API (`ContentStorage`/`Save360`); the converter's `java.io.File`-based
  level enumeration is bypassed.
- `WorldProvider.cpp:63-64`, `WorldIso.cpp:33`, `WorldProviderHell.cpp:69-75` —
  `getChunkLoader(saveDir*)` → `new ChunkLoader(saveDir, createIfNecessary)`.
- `m_saveDir` is a `std::string*` (the converter replaced Java's `File` with a path
  string).

### Xbox filesystem paths

| Logical | Xbox path | Backing | Set up by |
|---|---|---|---|
| Game assets (read-only) | `game:\Content\…` | XGD/title image | shipped with XEX |
| World saves (read/write) | `Save:\…` (mount name `"Save"`) | **STFS saved-game container** on the user-selected device (HDD / USB) | `Save360.cpp` |
| `level.dat` | `Save:\level.dat` | same container | `ContentStorage.cpp:25` |
| Chunks | `Save:\<x36>\<z36>\c.<x36>.<z36>.dat` (saveDir = `Save:` mount) | same container | `ChunkLoader.cpp` |
| `options.txt` | `<mcDataDir>\options.txt` (`File(*var2, "options.txt")`) | same mount | `GameSettings.cpp:108` |

### `Save360.cpp` — the STFS mount

This is the real Xbox storage layer (no Java analog):

- Mount name `"Save"`, container filename `"LOGANCRAFT"`, `XCONTENTTYPE_SAVEDGAME`
  (`Save360.cpp:33-34, 238-244`).
- `Save360_Init` (line 200-299): `XUserGetSigninState`/`XShowSigninUI` →
  `XShowDeviceSelectorUI` → `XContentCreate(OPENALWAYS)` → mount as `"Save:"`. Pumps the
  guide UI via a present-callback while waiting on `XOVERLAPPED`.
- After mount, **plain `CreateFile("Save:\…")` works** — that is the whole point of the
  STFS mount, and it is what `JavaIO.h`'s `FileInputStream`/`FileOutputStream` rely on.
- `XContentFlush`/`XContentClose` on flush/shutdown (line 320-337) — **required** to
  actually persist; an STFS container is not durable until flushed.

---

## Java file I/O → Xbox (mapping table)

Everything is in **`JavaIO.h`** (with `File.h` just `#include`-ing it).
`DataInput.h`/`DataOutput.h` provide the byte codec.

| Java type / call | OLD port impl (`JavaIO.h` unless noted) | Xbox/Win32/CRT primitive |
|---|---|---|
| `new File(p)` / `File(parent, child)` | `File` struct; `_join` normalizes `\` (line 43-60) | string only |
| `File.exists()` | `GetFileAttributes(path) != 0xFFFFFFFF` (line 51) | Win32 |
| `File.mkdirs()` | `CreateDirectory` (or `ERROR_ALREADY_EXISTS`) (line 52) | Win32 — **single level only**, not recursive! |
| `File.delete()` (`deleteFile`) | `DeleteFile` (line 53) | Win32 |
| `File.length()` / `renameTo()` / `listFiles()` | **absent** in the bridge (see ChunkLoader workaround) | — |
| `new FileInputStream(f)` | reads **entire file** into `std::vector` via `CreateFile`+`GetFileSize`+`ReadFile` (line 63-84) | Win32 |
| `FileInputStream.read()` / `read(b,len)` | pops from the in-memory buffer (line 79-83) | — |
| `new FileOutputStream(f)` | buffers writes in `std::vector` (line 86-93) | — |
| `FileOutputStream.write(...)` | append to buffer (line 92-93) | — |
| `FileOutputStream.close()` | `CreateFile(CREATE_ALWAYS)`+`WriteFile` then `CloseHandle` (line 94-100) — **write happens on close** | Win32 |
| `InputStream`/`OutputStream` | abstract base w/ default `read()/write()` loop (line 21-40) | — |
| `DataInputStream(in)` | drains the wrapped stream into an owned buffer, then *is* a `DataInput` (line 126-139) | — |
| `DataOutputStream(out)` | *is* a `DataOutput`; `close()` flushes buffer to wrapped stream (line 141-152) | — |
| `DataInput.readShort/Int/Long/Float/Double/UTF` | **byte-at-a-time big-endian** assembly (`DataInput.h:17-41`) | endian-agnostic |
| `DataOutput.writeShort/Int/Long/Float/Double/UTF` | **byte-at-a-time big-endian** emit (`DataOutput.h:12-25`) | endian-agnostic |
| `GZIPInputStream`/`GZIPOutputStream` | **pass-through** (no compression) (line 104-121) | — |
| `BufferedReader.readLine()` (options.txt) | **not used**: `GameSettings::loadOptions` reads the whole file via `FileInputStream` and splits on `\n`/`\r` itself (`GameSettings.cpp:275-313`) | Win32 (via FIS) |
| `ZipFile` | **unused** (single built-in texture pack) | — |
| `ByteBuffer.allocateDirect` etc. | raw `new[]` arrays (`GLAllocation.cpp:58-80`) | heap |

---

## Mapping to the new converter

The NEW stubs in `j2ctool/runtime.py` are **symbol-only no-ops** today
(`_ext_banner`, line 709-722 explicitly says behaviour is "the Xbox 360 platform
layer's job"). To make the new faithful-Java code actually run, the platform layer must
fill them in roughly as the OLD port does:

- **`ImageIO.read(...)` → `BufferedImage*`** (`runtime.py:867-868`, returns `0` now):
  the cleanest port is **not** to build a real `BufferedImage`. Either
  (a) decode the PNG to an `IDirect3DTexture9` directly (D3DX) and have the
  `BufferedImage` carry an opaque texture-id, or (b) decode to a 32-bit ARGB pixel
  array. The OLD port chose a third option — bypass `ImageIO` entirely and load by
  path. If the new code keeps the Java `RenderEngine` shape, `ImageIO.read` needs to
  return a `BufferedImage` whose `getWidth/getHeight/getRGB` are real.
- **`BufferedImage`** (`runtime.py:1141-1145`, all `0`/no-op): `getWidth`, `getHeight`,
  `getRGB`, `setRGB` must be real for the converted `RenderEngine.setupTexture` /
  texture-FX upload path. Back it with a heap ARGB array; on `setupTexture`, upload via
  `LockRect` into a `D3DFMT_A8R8G8B8` texture. (The OLD port skips this — that is why
  animated water/lava re-upload is a known gap.)
- **`File`** (`runtime.py:1106-1115`, returns `false`/`0`): must implement `exists`,
  `mkdir`/`mkdirs`, `getPath`/`getName`/`getParent`, and — for the faithful chunk saver
  — also `length`, `renameTo`, and `listFiles` (the OLD bridge omitted these and had to
  work around them). Back with `GetFileAttributes`/`CreateDirectory`/`FindFirstFile`.
- **`FileInputStream`/`FileOutputStream`** (`runtime.py:896-897`): back with
  `CreateFile`+`ReadFile`/`WriteFile`. Decide eager-vs-streaming: the OLD port slurps
  the whole file (fine for chunk-sized data). Remember **write-on-close** semantics or
  add explicit flush.
- **`DataInputStream`/`DataOutputStream` + `DataInput`/`DataOutput`**
  (`runtime.py:873-893`): these are the most important — implement **byte-at-a-time
  big-endian** exactly like `DataInput.h`/`DataOutput.h`. Do **not** `memcpy` a native
  multi-byte int. (`readFloat`/`readDouble` may `memcpy` *after* the int/long has been
  assembled big-endian, as the OLD port does — `DataInput.h:31-32`.)
- **`BufferedReader.readLine()`** (`runtime.py:1018-1020`): implement real line reading
  for `options.txt` (or follow the OLD shortcut and split in the caller).
- **`ZipFile`** (`runtime.py:898-900`): can stay a no-op unless texture-pack support is
  wanted; OLD port ships only the default pack.
- **`FloatBuffer`/`IntBuffer`/`ByteBuffer`** (`runtime.py:767-787`, return self/`0`):
  for the Tesselator/vertex path these must become real backing arrays with working
  `put`/`get`/`position`/`flip`. OLD port maps them to raw C arrays.
- **`GZIP*`**: the OLD port deliberately stores **uncompressed**. If the new save format
  should interoperate with PC worlds, implement real gzip; otherwise pass-through is
  fine but **document the incompatibility**.

---

## Known bugs / pitfalls

1. **Big-endian (Xbox 360 is PowerPC, big-endian).** NBT/binary I/O in both ports uses
   **explicit byte-at-a-time big-endian** shifts (`DataInput.h:17-41`,
   `DataOutput.h:12-25`), so it is correct on PPC *and* on a little-endian dev host —
   the byte order is independent of native word order. **The trap for the new
   converter:** any "optimized" reader that does `memcpy(&i32, ptr, 4)` or
   `*(int*)ptr` will be **byte-swapped on the 360** relative to the Java wire format and
   the .png/D3DX side. Keep the manual big-endian assembly. (Floats/doubles are safe
   because the int/long is assembled big-endian *first*, then bit-cast.)
   - **Cross-ref (where the endian fix actually lives):** the red-noise texture and red-vertex-colour bugs
     were NOT in this file-based `Textures` path (D3DX decodes the PNG, byte-order-agnostic) — they were in
     the **memory-upload** path inline in `GL11.reference.cpp` (`_uploadRGBA`/`glTexSubImage2D`/`glDrawArrays`
     colour read), fixed by reading the byte-array `[R,G,B,A]` source directly (`tex-endian-byte-read`) and
     the packed vertex colour as a native int (`color-endian-int-read`). Look there, not here, for that class.
2. **Saves are uncompressed NBT.** Gzip is stubbed out (`CompressedStreamTools.cpp`,
   `JavaIO.h:103-121`). 360 saves are **not** readable by desktop Minecraft and vice
   versa. The reader assumes no compression and relies on the NBT End tag to terminate
   (`CompressedStreamTools.cpp:48-59`, parses with `length = INT_MAX`).
3. **`mkdirs()` is not recursive** (`JavaIO.h:52` — single `CreateDirectory`).
   `ChunkLoader` only ever needs one level at a time (it creates `<x&63>` then `<z&63>`
   separately, `ChunkLoader.cpp:99-116`), so it works *there*, but any caller expecting
   true `mkdirs` behaviour will silently fail to create nested paths.
4. **Write-on-close.** `FileOutputStream` buffers in RAM and only touches disk in
   `close()` (`JavaIO.h:94-100`). A missing `close()` (or a crash before it) **loses the
   whole file**. Combined with STFS, you also need `Save360_Flush`/`XContentFlush`
   (`Save360.cpp:320-326`) or the container isn't durable.
5. **No `length()`/`renameTo()`** in the File bridge → the safe write-temp-then-rename
   save pattern was replaced by a **direct overwrite** (`ChunkLoader.cpp:185-207`). A
   crash mid-write **corrupts the chunk** (no atomic replace).
6. **Texture color-key history (the HUD-corruption bug).** An earlier version passed
   color key `0xFF000000` to `D3DXCreateTextureFromFileExA`, which forced every
   opaque-black texel (heart/hotbar outlines, lava detail) to alpha 0 and corrupted the
   HUD/world. Fixed to `0` (use the PNG's real alpha) — `Textures.cpp:51-54, 78`. Keep
   color key **off** in the new loader.
7. **NULL-texture leak (the "Steve skin / terrain stuck on screen" bug).** On Xenos the
   pixel shader always samples stage 0; `SetTexture(0, NULL)` leaves the **previous**
   texture bound, so a failed-to-load texture shows a leaked atlas. Two guards exist:
   prefix-stripping in `toContentPath` (`RenderEngine.cpp:22-45`) and the basename
   fallback in `Textures::load` (`Textures.cpp:66-83`). The new loader must **never
   bind NULL** — load a fallback (`default.png`) instead.
8. **`MAX_TEXTURES = 128`, no eviction.** `Textures.cpp` never frees individual textures
   (`deleteTexture` is a no-op, `RenderEngine.cpp:78-80`); hitting 128 distinct paths
   logs an error and returns `-1` (→ NULL bind → see #7). Downloadable/skin textures and
   any per-world dynamic textures eat into this fixed budget.
9. **Animated textures don't repaint the atlas.** `func_1067_a` ticks FX state but the
   per-tile `LockRect` re-upload is deferred (`RenderEngine.cpp:101-107`) — water/lava/
   fire are static in the OLD port. The new `BufferedImage`/`setupTexture` path is the
   place to fix this properly.
10. **Case-insensitive path cache** (`_stricmp`, `Textures.cpp:28`) — two names
    differing only by case collide into one texture id. Usually harmless but worth
    knowing.

---

## Open questions

1. **Exact on-disc `Content` tree.** `Textures::loadAll` (`Textures.cpp:110-160`) is the
   best manifest we have, but it's not exhaustive (fonts, lang files, sounds aren't
   here). Need to confirm the full `game:\Content\…` directory the title image actually
   ships, and which assets are loaded lazily vs. at boot.
2. **Font data.** Confirmed `mob\char.png` is the glyph sheet, but is there a glyph-width
   table (Alpha uses a hardcoded width array in `FontRenderer`)? Need to check
   `FontRenderer.cpp` for whether widths come from the PNG alpha (as in Java) or a table.
3. **`options.txt` location.** `GameSettings.cpp:108` uses `File(*mcDataDir,
   "options.txt")` but the value of `mcDataDir` (is it `Save:` or `game:`?) wasn't traced
   to its construction site — needs confirming. If it's on `game:` (read-only) saving
   options would silently fail.
4. **Save container scope.** `Save360.cpp` uses a same-title `XCONTENTTYPE_SAVEDGAME`
   container named `LOGANCRAFT`; the display name is hardcoded `"Minecraft Indev Saves"`
   (`Save360.cpp:242`) — likely copy-paste from the indev port. Cosmetic, but flag it.
5. **Single world only?** All chunks share one mounted `Save:` container; multi-world
   selection / per-world subdirectories under the container weren't found. Confirm
   whether the saveDir ever varies per world or is always the mount root.
6. **gzip interop decision.** Does the NEW port need to read/write PC-compatible
   (gzipped) NBT, or is the self-contained uncompressed format acceptable? Drives whether
   we port a real zlib/gzip on the 360.
