# Step 5 — File I/O + real NIO buffers: deliverables & integration

This step delivers the **platform file/stream layer** AND the **real NIO buffers** that Steps 2 (GL)
and 3 (textures) depend on. Reference implementations are Xbox-side (need the XDK + the platform
layer; verified by building the Xbox project, not the converter):

- `NIOBuffers.reference.h` / `.cpp` — real byte-backed `ByteBuffer`/`FloatBuffer`/`IntBuffer`,
  REPLACING the converter's no-op `_buffer_stub` headers. One refcounted backing block; the float/int
  buffers are **views over the same bytes**; `char* base()` + byte-valued `position()` for the GL
  emulator. Includes `ByteBuffer::allocateDirect(n)` to back `GLAllocation::createDirectByteBuffer`.
- `FileIO.reference.h` / `.cpp` — `File`, `InputStream`/`OutputStream`,
  `FileInputStream`/`FileOutputStream`, `DataInput`/`DataOutput` (big-endian NBT codec),
  `DataInputStream`/`DataOutputStream`, GZIP pass-through, `BufferedReader` (options.txt), and
  `J2X_getResourceAsStream(name)` resolving `/terrain.png` → `game:\Content\terrain.png`. This is the
  modern equivalent of the old port's `JavaIO.h` + `DataInput.h` + `DataOutput.h`, with the call
  shapes the NEW converter emits (notably the 3-arg `read(buf,off,len)` ImageIO needs).

## Integration steps (in the Xbox VS project)

1. **NIO buffers (do this first — unblocks Steps 2 & 3).** Drop `NIOBuffers.h`/`.cpp` in, REPLACING
   the converter's `ByteBuffer.h`/`FloatBuffer.h`/`IntBuffer.h` stubs. Then fix the converted
   `GLAllocation.cpp`: its local `_bb_allocateDirect(int){ return new ByteBuffer(); }` shim builds a
   **zero-byte** buffer — change `createDirectByteBuffer` to `return ByteBuffer::allocateDirect(var0);`
   (details + the alternative in `NIOBuffers.reference.cpp`). The Tessellator's
   `asIntBuffer()/asFloatBuffer()/put/position/limit/clear` then operate on real shared bytes, and the
   GL emulator's `((ByteBuffer*)buf)->base() + position()` reads valid vertex data.
2. **java.io bridge.** Drop `FileIO.h`/`.cpp` in, REPLACING the converter stubs for `File`,
   `InputStream`, `OutputStream`, `FileInputStream`, `FileOutputStream`, `DataInput`, `DataOutput`,
   `DataInputStream`, `DataOutputStream`, `BufferedReader` (and the GZIP wrappers). If the converter
   emits these as separate headers, split `FileIO.h` per class or have each stub header `#include
   "FileIO.h"` — same approach the old port used (`File.h` just `#include`d `JavaIO.h`).
3. **getResourceAsStream.** Wire `Class::getResourceAsStream` (runtime `Class.h`, currently
   `return 0`) to `return J2X_getResourceAsStream(name);`. `TexturePackBase::getResourceAsStream`
   already forwards to `Class(typeid(TexturePackBase)).getResourceAsStream(name)`, and the new
   `RenderEngine` already strips the `##` / `%blur%` / `%clamp%` prefixes via `substr(...)` before
   the call — so the resolver only handles the leading `/` and `/`→`\`. This is the dependency Step 3
   flagged: with it live, `ImageIO::read(stream)` works and `ImageIO::readFile(path)` is no longer
   needed.
4. **Saves.** The converted `ChunkLoader`/`level.dat` path uses `File` + `FileOutputStream` +
   `DataOutputStream` over the **`Save:`** mount. Mount it at boot via `Save360_Init` (below), then
   plain `CreateFile("Save:\...")` — which `FileInputStream`/`FileOutputStream` use — just works.
   Call `Save360_Flush()` after a save batch.
5. **options.txt (FileReader/FileWriter gap).** The converted `GameSettings` builds
   `new BufferedReader(new FileReader(optionsFile))` / `new PrintWriter(new FileWriter(optionsFile))`,
   but the converter has **no runtime header** for `FileReader`/`FileWriter`/`PrintWriter`, so it
   inlined LOCAL no-op shims (see `GameSettings.cpp`) — options are **never read or written**. To make
   options persist, either (a) add real `FileReader`/`FileWriter`/`PrintWriter` (FileReader = a
   char wrapper over `FileInputStream`, usable by `BufferedReader`; FileWriter/PrintWriter over
   `FileOutputStream`), or (b) follow the OLD port's shortcut (`GameSettings::loadOptions` reads the
   whole file via `FileInputStream` and splits on `\n`/`\r` itself). `BufferedReader::readLine()` here
   already does real line reading once it's handed a real stream. Put `optionsFile` on `Save:` (a
   writable mount), not read-only `game:`.
6. Build the Xbox project (XDK) and iterate the HARDWARE-VERIFY points below on the 360.

## !!! BIG-ENDIAN HAZARD (the single most important thing) !!!

Xbox 360 is **big-endian PowerPC**. There are TWO buffer worlds with OPPOSITE rules — do not mix them:

- **NBT / file I/O (`DataInput`/`DataOutput`)** assembles every multi-byte value **byte-at-a-time
  big-endian**. This is what makes the on-disc format identical on PPC and on a LE dev host. **NEVER**
  replace it with `memcpy(&i32, ptr, 4)` or `*(int*)ptr` over a serialized buffer — that is
  byte-swapped on the 360 relative to the Java/PC NBT wire format. Floats/doubles are safe **only**
  because the int/long is assembled big-endian *first*, then bit-cast (`readFloat`/`writeFloat`).
- **NIO vertex scratch (`ByteBuffer`/`Int`/`Float`)** is **native-order on purpose**: the Tessellator
  writes `floatToRawIntBits` ints and the D3D9 vertex path reads them back **on the same machine**, so
  a straight `*(int*)`/`*(float*)` over the backing bytes is correct and fast. Keep these native;
  never run NBT through them.

If a chunk loads as garbage / impossible block IDs after an "optimization", a big-endian shortcut
crept into the NBT path — revert to the byte-at-a-time codec.

## Save:\ mount flow (`Save360.cpp`, no Java analog)

1. `Save360_Init(titleId, userIndex)`:
   - `XUserGetSigninState` → `XShowSigninUI` if not signed in (polls ~30 s, pumping the guide UI).
   - `XShowDeviceSelectorUI` (SAVEDGAME, 512 KB hint) → user picks HDD/USB (`XOVERLAPPED`, pumped).
   - `XContentCreate(user, "Save", &XCONTENT_DATA{SAVEDGAME, deviceId, "LOGANCRAFT"}, OPENALWAYS)` —
     `ERROR_ALREADY_EXISTS` is treated as success. Mounts as **`Save:`** (no trailing backslash; falls
     back to `Save:\` if a write test fails — `save-writable-probe`).
   - **DELIVERED refinements (`Save360.reference.cpp`):** the whole `Save360_Init` is behind an Interlocked
     mount gate (`save-mount-race`, 0→1→2) so a racing caller can't double-enter the device selector; the
     device-selector wait has a ~5-min anti-hang backstop (`XCancelOverlapped` + `game:\` fallback) over a
     STATIC `XOVERLAPPED`. NOTE: the reference deliberately does **not** call `XShowSigninUI` (it can pop a
     guide blade that never dismisses under the emulator — hangs boot); no profile → `game:\` fallback.
2. After mount, `CreateFile("Save:\level.dat")` / `Save:\<x36>\<z36>\c.<x36>.<z36>.dat` work directly —
   that's the whole point of the STFS mount, and what the `FileIO` streams rely on.
3. **Durability:** an STFS container is **not persisted until flushed.** Call `Save360_Flush()`
   (`XContentFlush`) after a save batch and `Save360_Shutdown()` (`XContentFlush` + `XContentClose`) on
   exit. Combined with `FileOutputStream`'s **write-on-close**, a save is only safe once: the stream is
   `close()`d AND the container is flushed.
4. `Save360_SetPresentCallback(fn)` must be set so the guide UI renders during the signin/device waits.

Chunk layout (base-36, two-level), built by the converted `ChunkLoader` (mirrored in
`J2X_chunkPath` for reference): `Save:\<x&63 b36>\<z&63 b36>\c.<x b36>.<z b36>.dat`. `level.dat` →
`Save:\level.dat`. Create the two dir levels with `File(dir).mkdirs()` first — `mkdirs()` here is
**recursive** (the old single-level `CreateDirectory` was a documented pitfall).

## getResourceAsStream → game:\Content resolution

`name` is a JAR-style path (`"/terrain.png"`, `"/gui/gui.png"`). `J2X_getResourceAsStream`:
drop one leading `/`, convert `/`→`\`, prefix `game:\Content` → open a `FileInputStream`. A
**basename fallback** retries `game:\Content\<basename>` if the subfolder path is empty (an asset
flattened out of a subfolder — the old port's pitfall #7 guard). Assets ship **loose as PNG** under
`game:\Content\…` (read-only DVD/title image); no DDS, not embedded in the XEX. `game:` is read-only —
assets only; all read/write goes to `Save:`.

## Open decision — raw NBT vs real gzip (saves)

The old port (and these references) store **uncompressed raw NBT** — GZIP wrappers are pass-through.
Self-consistent (the port only reads its own saves) but **incompatible with desktop Minecraft `.dat`
files** in both directions. To interoperate with PC worlds you'd port a real zlib/gzip onto the 360
and wrap the chunk/`level.dat` streams; otherwise keep pass-through and document the incompatibility.
Recommendation: keep raw for now (no zlib dependency, smaller surface) unless PC-world import is a
goal — revisit if it is.

## HARDWARE-VERIFY (can't be checked off-device)

- **Mount + sign-in flow** on real hardware: signin UI, device selector, container create, and the
  `Save:` write test all need a profile + storage device; verify the present-callback keeps the guide
  UI responsive during the waits.
- **Durability:** confirm a world survives a full power cycle — i.e. `FileOutputStream::close()` +
  `Save360_Flush()` actually committed the STFS container (a missing flush silently loses everything).
- **Asset paths:** confirm the real on-disc `game:\Content\…` tree matches the resource names the new
  code requests (incl. `gui\`, `mob\`, `environment\clouds.png`); watch the basename fallback firing
  in the log as a sign a path is wrong.
- **NIO buffer correctness:** if geometry is wrong only after the buffers go live, check that
  `createDirectByteBuffer` allocates real bytes (not the zero-size shim) and that
  `(ByteBuffer*)floatBuffer)->position()` reports the expected **byte** offset (Tessellator parks the
  float view at element 3 = byte 12, color at byte 20).
- **`options.txt` location:** the converted `GameSettings` path (`mcDataDir` + `options.txt`) must be
  on a writable mount (`Save:`), not read-only `game:` — else option saves silently fail (doc 04 open
  question #3).
