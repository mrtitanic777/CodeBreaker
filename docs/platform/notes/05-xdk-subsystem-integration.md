# 05 — XDK Subsystem Integration Plan

Master map of every game subsystem we stubbed → the **real Xbox 360 XDK API** that backs it, verified
against `C:\Program Files (x86)\Microsoft Xbox 360 SDK\include\xbox\` and `…\lib\xbox\`. Built from a
five-way parallel SDK/codebase audit. **Rule: never assume a Win32/PC API — grep the SDK header first.**

## Status at a glance

| Subsystem | State | Real XDK API | Lib (already linked?) | Priority |
|---|---|---|---|---|
| **Audio** | ✅ REAL (XACT3, fully wired) | XACT3 (`xact3.h`); XAudio2 only if 3D/pitch needed | `xact3.lib`/`xactd3.lib` ✅ | P0 verify, P2 polish |
| **File I/O** | ✅ REAL (Win32-backed `FileIO.cpp`) | `CreateFile`/`ReadFile` (`winbase.h`), C stdio | `xapilib.lib` ✅ | — |
| **Save storage** | ⚠️ REAL but UNWIRED | XContent/STFS (`xbox.h`), mount `Save:` | `xapilib.lib` ✅ | **HIGH** |
| **Compression** | ❌ STUB (inert) | LZX `XMemCompress`/`Decompress` (`xcompress.h`) | `xapilib.lib` ✅ | **HIGH** |
| **Text input** | ❌ STUB (no letters) | `XShowKeyboardUI` async (`xbox.h`) | `xapilib.lib` ✅ | P1 |
| **Networking** | ❌ STUB (no-op `Socket.h`) | Winsock2 (`winsockx.h`) + `XNetStartup` | `xnet.lib` ✅ | LOW |

Every needed lib is ALREADY linked — no linker changes for any of these (XInput2 was the exception, now added).

---

## 1. Audio — ✅ ALREADY REAL (XACT3). Do not rebuild.
`SoundSystem.h` → `Audio.h`/`Audio.cpp` → `XactAudio.cpp` is a complete XACT3 backend: `XACT3CreateEngine`
(`xact3.h:1407`) + `Initialize` + `CreateStreamingWaveBank`/`CreateSoundBank` from `game:\Content\resources\
sounds.xwb/.xsb` + `music.xwb/.xsb`, plays via `SoundBank->Play()` on an audio thread (HW thread 5). Music
playlist auto-advances.
- **Gaps (deliberate):** no per-SFX volume/pitch (binary music mute only), no 3D positional, ambient loops
  fire once. Alpha-new cues (mobs/portal/door) are mapped but silently no-op if not authored into the bank.
- **OGG is NOT decodable by the XDK** (verified: no Vorbis anywhere). Wave banks accept PCM/XMA/ADPCM only
  (`xact3wb.h:76-78`); new sounds must be transcoded offline to XMA/PCM and authored with `xactbld3`.
- **Do:** P0 verify on device (cue coverage, music advance, first-sound race). P2 (optional) re-enable the
  `DopplerPitchScalar` RPC path (`XactAudio.cpp:556-608`) for pitch + category volume. P3 (only if 3D
  required) switch to XAudio2 (`XAudio2Create xaudio2.h:1255`) + X3DAudio.

## 2. File I/O — ✅ ALREADY REAL (Win32). 
`File.h`/`FileInputStream.h`/`FileOutputStream.h` → `FileIO.h`/`.cpp`, backed by `CreateFile`
(`winbase.h:2262`)/`ReadFile`/`WriteFile`/`MoveFile`/`CreateDirectory`. Big-endian `DataInput/DataOutput`
NBT codec is correct. C stdio (`fopen` etc., `stdio.h`) also works on `game:\`/`Save:` paths.
- **`RandomAccessFile` not needed** — Alpha uses loose gzipped base-36 chunk files, no region/Anvil files.
- **Bug:** `File::listFiles()` returns NULL (`FileIO.h:117`) → empty world/texturepack lists (use
  ContentStorage enumeration instead). LOW.

## 3. Save storage — ⚠️ REAL but NEVER CALLED. **HIGH — this is why saves vanish.**
`Save360.cpp` (signin/device-select/`XContentCreate(OPENALWAYS)`/mount as `Save:`/`XContentFlush`) and
`ContentStorage.cpp` (per-world STFS containers) are fully implemented and compiled — but `Save360_Init`,
`Save360_Flush`, and `g_contentStorage` have **zero call sites**. Worse, the world is built against the
**read-only** `game:\minecraft\saves` (`Minecraft.cpp:1061`), not the writable `Save:` mount → every write
silently no-ops.
- **Do:** (a) `Save360_SetPresentCallback(...)` then `Save360_Init(titleId,0)` at startup; (b) route the
  world saveDir (`Minecraft.cpp:1061`, `World.cpp:239-243/497-501`) to the `Save:` mount; (c) call
  `Save360_Flush()` after each save batch / on pause / on world close (STFS isn't persisted until flush).
- XContent API: `XContentCreate xbox.h:1078`, `XContentFlush :1133`, `XContentCreateEnumerator :1329`,
  `XCONTENTTYPE_SAVEDGAME=0x1 :995`. Keep loose `CreateFile("Save:\...")` files *inside* the mounted
  container — do NOT hand-pack STFS.

## 4. Compression — ❌ INERT STUB. **HIGH — blocks world save/load.**
`CompressedStreamTools.cpp` GZIP/Deflater/Inflater are empty shims; `ByteArrayOutputStream::toByteArray()`
**returns an empty array** (the core bug). So `level.dat`/chunk saves write **zero bytes** and load gets
nothing (`World.cpp:825`, `ChunkLoader.cpp:178/140`).
- **SDK has ONLY LZX** (`xcompress.h`): `XMEMCODEC_LZX` (`:35`), `XMemCreateCompressionContext :54`,
  `XMemCompress :97`, `XMemDecompress :168`, `XMemDestroy* :75/146`. **No zlib/gzip/DEFLATE exists** — LZX
  is incompatible with PC Minecraft saves and real-MC-server packets (both acceptable: saves are internal).
- **Do (recommended — LZX for internal saves):**
  1. Make `ByteArrayOutputStream`/`ByteArrayInputStream` real (vector-backed) — fixes the empty-array bug.
  2. Add `CompressBufferLZX`/`DecompressBufferLZX` helpers with a 5-byte header `[magic 'X'/'0'][len BE32]`
     (store-uncompressed fallback + records raw size for the decompress dest buffer).
  3. Rewire `compress`/`decompress`/`writeGzippedCompoundToOutputStream`/`readCompressed` to buffer→LZX
     (NBT logic untouched; gzip "streaming" becomes buffer-based — Java only wraps a whole tag).
  4. Round-trip test: save→load one chunk + level.dat.
  - Endianness: write the length header byte-by-byte (360 is big-endian). Worst-case compress dest =
    `srcSize + ceil(srcSize/32768)*6155` (`XCOMPRESS_LZX_BLOCK_*` `:198/199`).
- Network packet paths (`Packet51MapChunk inflate`, `Packet59` compress) stay stubbed — same helpers later,
  but they won't interop with real MC servers (LZX≠DEFLATE).

## 5. Text input (OSK) — ❌ no way to type. P1 (multiplayer/chat/signs; NOT needed for world create).
Alpha has no `GuiTextField`; each screen's `keyTyped` appends `Keyboard::getEventCharacter()` — which a
gamepad can't produce, so all text fields are dead. World slots auto-name (`GuiSelectWorld.cpp:165`), so
**world creation does NOT need this.** Needed for `GuiMultiplayer` server address, `GuiChat`, `GuiEditSign`.
- **API:** `XShowKeyboardUI(user, VKBD_* flags, defaultW, titleW, descW, resultW, cch, PXOVERLAPPED)`
  (`xbox.h:2365`), async; poll `XHasOverlappedIoCompleted` (`xbox.h:966`) / `XGetOverlappedResult(...,FALSE)`.
  Flags `VKBD_LATIN_FULL=0x01`, `VKBD_LATIN_IP_ADDRESS=0x10` (`xam.h:467-494`). Strings are UTF-16 (convert).
  Use ASYNC (sync blocks the render thread — forbidden).
- **Design:** new `Osk` module (`open/poll`, static `XOVERLAPPED`+`WCHAR` buffer); poll in
  `GuiScreen::handleInput`; add `virtual oskResult(std::string)`; each text screen launches in `initGui`
  and drops the result into its public string member. No char-event synthesis needed.

## 6. Networking — ❌ no-op `Socket.h`. LOW (after single-player).
Packet pipeline + reader/writer threads are real; only the socket is a stub returning NULL streams → a
connect spins two threads that instantly die. Client-only (outbound TCP to a vanilla server, port 25565).
- **API:** full Berkeley/Winsock2 in `winsockx.h` (`socket :1273`, `connect :874`, `recv :1104`,
  `send :1179`, `closesocket :857`, `WSAStartup :1300`). **Requires `XNetStartup` FIRST** (`:1943`) with
  **`XNET_STARTUP_BYPASS_SECURITY=0x01`** (`:1714`) to reach a PC host — **devkit/RGH only** (the #1
  hardware-verify item; ignored on retail). DNS via `XNetDnsLookup` (or require numeric IP + `inet_addr`).
- **Landmines:** the converter **dropped all `synchronized` monitors** on the send queues
  (`NetworkManager.cpp:73-86`) → data race; add a `CRITICAL_SECTION`. Pin reader/writer threads with
  `XSetThreadProcessor`. The `minecraft.net` session-auth GET (`NetClientHandler.cpp:506`) is dead —
  stub to "ok"/offline-mode.

---

## Recommended integration order
1. **Compression (LZX) + Save storage wiring** — done together = **world save/load actually works** (the
   next milestone). Both HIGH, both fully specced above, both use already-linked `xapilib`.
2. **Audio on-device verification** (P0, no code) + optional pitch/volume (P2).
3. **OSK text input** (P1) — unblocks server address / chat / signs.
4. **Networking** (LOW) — multiplayer milestone; needs on-device `BYPASS_SECURITY` validation.
5. **Polish:** options.txt real I/O, `File::listFiles`, ambient sound loops.

Related: [[xbox360-input-xinput2]], [[xbox360-d3d9-shader-only]], [[xbox-swap-buildout]].
