# Recompiled titles: playability status

2026-11-15. Every Xbox 360 title in `C:/Nexia360/Games` that `recompile-project` accepted, checked with
`recompile-project <path> --check 120` (recompile, build, run headless for 120 s). Tetris Splash, BlockWorld and Up
are left out as asked. "Frames" is what the title presented in 120 s (60 a second when it runs, so 6600 is the
ceiling of this check). The first stop is the first thing that ended the run. Method: `state/work/recompile-track`
(scratch, deleted). Each project was deleted after its check (`build/`, and the whole project afterwards).

## Results (before the runtime changes of 2026-11-15)

| Folder | Title | Functions | Built | Frames | First stop | After the runtime changes |
|---|---|---|---|---|---|---|
| 415487DB | Catherine Demo | 58643 | ok | 15 | GPU: texture fetch constant 1 is not implemented: format k_24_8_FLOAT (type 2 24_8_FLOAT 2D 1280x720 tiled pit | stop moved to a 2_10_10_10_FLOAT render target (InterlockedPopEntrySList, 24_8_FLOAT textures done) |
| 415608CB | (not an Xbox 360 title: no default.xex) |  |  | - |  |  |
| 423607D1 | (title 423607D1, GoD) | 26038 | ok | 0 | XexGetProcedureAddress on module handle 00000000 (ordinal 18): only system modules are supported |  |
| 423607D3 | (GoD image unreadable: xdvdfs read before the start) |  |  | - |  |  |
| 45410806 | (EA, disc, 33,319 functions) | 33319 | ok | 0 | XexGetProcedureAddress(xam.xex, ordinal 442): no implementation of this export (add a GUEST_EXPORT) | 6600 frames (XInput lookups answered) |
| 454188D6 | Left 4 Dead 2 Demo | 347 | ok | 0 | XexLoadImage("d:\bin\tier0_360.dll", flags 00000009, version 00000000): not a loaded recompiled module or a sy |  |
| 4D5389F0 | Forza Motorsport 4 | 69307 | ok | 0 | XexGetProcedureAddress(xam.xex, ordinal 442): no implementation of this export (add a GUEST_EXPORT) | 0 frames; stop moved to XexLoadImage(XMediaFacade_default.xex) |
| 545487F5 | Top Spin 2 Wimbledon Demo | 41857 | ok | 0 | Guest memory write fault at 7EF53014 in guest function 82200998 (host +8B) on thread 6 (last guest call site l | 0 frames; stop moved to a null read in worker threads (stack start fixed) |
| 584107D1 | Hexic HD | 11094 | ok | 1 | GPU: 1D texture fetches are not implemented (tf0) | 4800 frames, no stop (1D textures; XMP play) |
| 584107D5 | Small Arms | 453144 | FAILED | - |   C:\Users\mrtit\Documents\GitHub\CodeBreaker\Converted\Small_Arms\src\ppc\ppc_recomp.1.cpp(8,54): error: rede |  |
| 584107D7 | Marble Blast Ultra | 26484 | ok | 0 | XMP: creating a title playlist with songs is not implemented | 6600 frames (XMP title playlist with songs) |
| 584107E7 | Cloning Clyde | 18837 | ok | 1 | GPU: pixel shader EA0C9EF9873CFCF6 slot 2: tfetch3D is not implemented by the HLSL translator |  |
| 584107ED | Geometry Wars Evolved | 4789 | ok | 6600 |  |  |
| 584107F1 | Feeding Frenzy | 13844 | ok | 6600 |  |  |
| 584107F3 | UNO | 12393 | ok | 1 | Guest memory write fault at 7EFA2014 in guest function 920E2660 (host +75) on thread 7 (last guest call site l | 1 frame; stop moved to tfetch3D (stack start fixed) |
| 584107F4 | Street Fighter II' HF | 19440 | ok | 1 | Guest memory write fault at 00000008 in guest function 82073580 (host +EF) on thread 6 (last guest call site l |  |
| 584107F8 | LUMINES LIVE! - Base Pack | 14096 | ok | 0 | GPU: render target 0 format 14 (32_FLOAT) is not implemented by the EDRAM output merger |  |
| 584107FE | Assault Heroes | 32761 | ok | 6600 |  |  |
| 58410810 | Heavy Weapon | 26254 | ok | 1 | Guest memory read fault at 00000070 in guest function 820CA570 (host +45E) on thread 1 (last guest call site l |  |
| 58410811 | PAC-MAN | 12618 | ok | 6600 |  |  |
| 58410841 | 3D Ultra Minigolf | 35081 | ok | 6600 |  |  |
| 58410870 | TMNT 1989 Classic Arcade | 16037 | ok | 6600 |  |  |
| 584108A4 | SuperStreetFighter2THD | 23227 | ok | 6600 |  |  |
| 584108A8 | Vigilante 8 Arcade | 12200 | ok | 0 | XexGetProcedureAddress(xam.xex, ordinal 442): no implementation of this export (add a GUEST_EXPORT) | 96 frames; stop moved to tfetch3D (XInput lookups answered) |
| 584108B7 | Castle Crashers | 5732 | ok | 6600 |  |  |
| 58410AE9 | Happy Wars | 535116 | ok | 0 | Guest memory write fault at 00000002 in guest function 83853DF4 (host +8A) on thread 1 (last guest call site l |  |
| 584111D6 | Bejeweled 3 | 39416 | ok | 6600 |  |  |
| 584111F7 | Minecraft Xbox 360 Edition | 59753 | ok | 6600 |  | 6600 frames in 120 s, 35400 in 600 s, no stop (idles at its menu without input) |
| 5841128F | Terraria | 2516 | ok | 0 | XexLoadImage("GAME:\HostLoader.dll", flags 00000009, version 00000000): not a loaded recompiled module or a sy |  |
| Arcadecraft | (not an Xbox 360 title: no default.xex) |  |  | - |  |  |
| Bomberman LIVE | Bomberman LIVE | 16114 | ok | 6600 |  |  |
| DIG-DUG | DIG DUG | 13053 | ok | 6600 |  |  |
| Doritos Crash Course | Doritos Crash Course | 38350 | ok | 6000 |  |  |
| Echo The Dolphin | Ecco the Dolphin | 13766 | ok | 6600 |  |  |
| GALAGA | GALAGA | 12712 | ok | 6600 |  |  |
| Joy-Ride-Turbo | Joy Ride Turbo | 118899 | ok | 1 | NtQueryInformationFile class 27 is not implemented (C:\Users\mrtit\Documents\GitHub\CodeBreaker\Converted\Joy_ |  |
| MS. PAC-MAN | MS.PAC-MAN | 13100 | ok | 6600 |  |  |
| MW3.iso | Modern Warfare 3 | 35133 | ok | 1 | XamInputGetCapabilities(user 0): flags 00000002 (only the gamepad is implemented) | 6600 frames (MmSetAddressProtect, keyboard capabilities) |
| Millipede & Centipede | Centipede & Millipede | 18389 | ok | 6600 |  |  |
| Monopoly PLUS | MONOPOLY PLUS | 89428 | ok | 0 | XexGetProcedureAddress(xam.xex, ordinal 442): no implementation of this export (add a GUEST_EXPORT) | 140 frames; stop is a null write at 0x1C (guest) |
| PAC-MAN CE DX | PAC-MAN CE DX | 16885 | ok | 6600 |  |  |
| PAC-MAN CE | Pac-Man C.E. | 9610 | ok | 6600 |  |  |
| PAC-MAN MUSEUM | PAC-MAN MUSEUM | 68618 | ok | 0 | Indirect call to 821BED60, where no recompiled function starts (call at 821BFDBC) |  |
| RALLY-X | NEW RALLY-X | 13080 | ok | 6600 |  |  |
| REAL-STEEL | REAL STEEL | 26284 | ok | 6600 |  |  |
| Rock-Band-Blitz | Rock Band Blitz | 55060 | ok | 0 | Guest memory write fault at 00000001 in guest function 827F5EC4 (host +8A) on thread 4 (last guest call site l |  |
| Spongebob Underpants Slam | SpongeBob UnderPants! | 33832 | ok | 6600 |  |  |
| TEMPEST | Tempest | 22590 | ok | 600 | XMP: creating a title playlist with songs is not implemented |  |
| TRON | TRON | 15701 | ok | 6600 |  |  |
| TrialsHD | Trials HD | 26935 | ok | 6600 |  |  |
| UNO Rush | UNO RUSH | 27004 | ok | 6600 |  |  |
| WALL-E | WALL-E Demo | 38625 | ok | 0 | Guest memory write fault at 7EF3D014 in guest function 820E6278 (host +75) on thread 4 (last guest call site l | 5400 frames (thread stack start) |
| Zuma | Zuma | 15742 | ok | 6600 |  |  |

Counts (Tetris Splash excluded): 53 entries tried; 3 are not recompilable (no `default.xex`, or an unreadable GoD image),
50 recompiled and 49 built (Small Arms: 453,144 functions, `sub_0` defined twice, so no link; Happy Wars is 535,116 functions
for the same reason but builds). Before the changes 26 ran the whole check (6000-6600 frames) and 23 stopped or stalled;
after them 31 do (Hexic HD, Marble Blast Ultra, MW3, the EA title 45410806 and WALL-E Demo joined). Not rerun after the
changes: Tempest (its stop, the XMP playlist, is fixed).

## Most common gaps, ranked by titles stopped

| # | Gap | Titles |
|---|---|---|
| 1 | Guest memory faults: a thread entry storing above its stack (fixed: UNO, WALL-E, Top Spin 2); null objects (open: SF2 HD, Heavy Weapon, Happy Wars, Rock Band Blitz, Monopoly Plus, Top Spin 2's workers) | 8 |
| 2 | XAM/kernel exports looked up at run time by XInput start-up (xam 442, 520, 1800; xboxkrnl 642-649) | 4 (fixed): Vigilante 8, Monopoly Plus, Forza 4, the EA title |
| 3 | `tfetch3D`: 3D (volume) textures | 3 (open): Cloning Clyde, UNO, Vigilante 8 |
| 4 | `XexLoadImage` of a second module (`HostLoader.dll`, `tier0_360.dll`, `XMediaFacade_default.xex`) | 3 (open): Terraria, Left 4 Dead 2 Demo, Forza 4 |
| 5 | XMP title playlists with songs and the play controls | 3 (fixed): Marble Blast Ultra, Tempest, Hexic HD |
| 6 | Render targets: 32_FLOAT (Lumines), 2_10_10_10_FLOAT (Catherine) | 2 (open) |
| 7 | GPU: 1D textures; 24_8_FLOAT textures | 2 (fixed): Hexic HD, Catherine |
| 8 | XMA decode errors (audio only, the title runs): `channel transform bit` (406,031 in Heavy Weapon), `channel longer than the frame` (Hexic HD), `reserved subframe bit set` (WALL-E) | 3 (open) |
| 9 | One title each: `XamInputGetCapabilities` keyboard (MW3, fixed); `NtQueryInformationFile` class 27 (Joy Ride Turbo); indirect call to 821BED60 (PAC-MAN Museum); `XexGetProcedureAddress` on handle 0 ordinal 18 (title 423607D1); GoD image read (423607D3); `.pdata` zero entries (Small Arms, Happy Wars) | |

### Unimplemented imports (by titles that link them, 39 built projects)

`__C_specific_handler` 39 (never called), `KeLockL2`/`KeUnlockL2` 38 (now implemented), `NtReadFileScatter` 22,
`XUsbcam*` 12-13 each, `XamParseGamerTileKey` 10, `XamShowDirtyDiscErrorUI` 8, `XamShowGameInviteUI` 7,
`NetDll___WSAFDIsSet` 7, `XamShowPlayersUI` 6, `XamContentGetDeviceName` 6, `XAudioGetDuckerReleaseTime` 6,
`NtWriteFileGather` 6, `MmSetAddressProtect` 5 (now implemented), `Io*Device*`/`IoShareAccess` 5, `LDIDecompress*` 4.
Called during the checks: `InterlockedPopEntrySList` (105,707 calls, Catherine; implemented), `MmSetAddressProtect` (594, MW3;
implemented), `XamInputGetCapabilitiesEx`, `NtQueueApcThread`, `MmCreateKernelStack`, `XamLrcLogError` (open).

## What was implemented (native/x360)

- A thread's stack pointer starts 0x100 below the top of its stack (`kernel::kInitialStackOffset`): thread functions store
  up to 0xC4 bytes above their own frame. Audio, NIC, XInputd and title-end threads too. UNO, WALL-E, Top Spin 2 faulted at once.
- `InterlockedPushEntrySList`, `Pop`, `Flush`, `QueryDepthSList` (64-bit compare-and-swap on the big-endian header),
  `KeLockL2`/`KeUnlockL2`, `MmSetAddressProtect`.
- XMP: a title playlist with songs (a handle per song), `XMPPlayTitlePlaylist`, continue/stop/pause/next/previous,
  `XMPSetPlaybackBehavior`, `XMPGet/SetVolume`; the same messages through `XMsgSystemProcessCall` (queued: result dropped).
- Run-time export lookups that XInput start-up code makes and tolerates failing (xam 442, 520, 1800; xboxkrnl 642-649):
  answered not found, like an empty export slot.
- `XamInputGetCapabilities` for a keyboard or other device: not connected, not a stop.
- GPU: 1D textures (a one-row 2D texture; linear only), `k_24_8_FLOAT` depth textures (20e4 to float, as Xenia).
- `CB_KERNEL_TEST=2 <title>.exe`: kernel checks a title's program can run (13 checks: lists, XMP, stack start); all pass.

## Open, by what each would unblock

1. 3D textures (`tfetch3D`, volume tiling): Cloning Clyde, UNO, Vigilante 8.
2. Loading a second recompiled module by name (`XexLoadImage`): Terraria, L4D2, Forza 4.
3. Render targets 32_FLOAT / 2_10_10_10_FLOAT in the EDRAM output merger: Lumines, Catherine.
4. XMA frame decode errors (3 titles: sound missing, no stop).
5. Null-object faults: SF2 HD (write at 8), Heavy Weapon (read at 0x70), Top Spin 2 (workers at 0x14), Rock Band Blitz, Monopoly Plus, Happy Wars.
6. `.pdata` with zero entries (Small Arms, Happy Wars): `sub_0` emitted over and over; fix in the function discovery.
7. Minecraft: no stop in 600 s headless (35,400 frames); further play needs input (`CB_INPUT_SCRIPT`).

## Round 2 (2026-11-15, later)

Re-checked after each fix (`--check 120`, one project at a time, deleted after).

| Title | Round 1 | Now |
|---|---|---|
| Small Arms | no link (`sub_0` twice, 453,144 functions) | builds (42,521 functions), 1 frame, then a 64-bit render target |
| Happy Wars | 535,116 functions, null write | recompiles smaller; not rerun (40-minute build) |
| Rock Band Blitz | 0 frames, null write | 6600 frames (`XamInputGetCapabilitiesEx` now implemented) |
| Street Fighter II' HF | 1 frame, null write at 8 | 6600 frames (`MmCreateKernelStack`) |
| Top Spin 2 | 0 frames, null read | 267 frames, then its loader thread stalls |
| Catherine Demo | 7 frames, `2_10_10_10_FLOAT` target | 57 frames, then a 16_16_16_16_FLOAT (64-bit) render target |
| Lumines Live | 0 frames, `32_FLOAT` target | 0 frames, 16_16_16_16_FLOAT (64-bit) render target |
| Forza 4 | 0 frames, `XexLoadImage` | 0 frames: XMediaFacade is a real module; `XamShowDirtyDiscErrorUI` loops |
| Monopoly Plus | 140 frames, null write | 139 frames, then a resolve to `k_8` |
| Regression set | | Minecraft, Hexic HD, Marble Blast Ultra, WALL-E, PAC-MAN CE, Defender, UNO Rush, Zuma, MW3: 6600/5400 frames as before |

What changed:
- **Recompiler** (`native/xenonrecomp`): a `.pdata` entry that is all zeros (begin address 0) is no longer a function, and a zero word
  between functions is padding, not a one-word function. Small Arms and Happy Wars had 450,000 `.long 0x0` functions and a `sub_0` per
  padding entry. Every title has fewer functions now (Minecraft 59,753 to 43,128, Defender 14,752 to 10,025) and all of the above
  still play. The golden of `recompile.command.the_xbox_360_project_matches_the_golden` (LoganMinecraftTest: 8744 to 5861
  functions, 38 to 27 generated files, `x360_manifest.txt`) is re-recorded for this.
- **Null objects** were three missing kernel behaviours: `ObOpenObjectByPointer` takes `(Object, PHANDLE)` in a title's XDK (it wrote
  through r5, garbage); `MmCreateKernelStack` (+ `MmDeleteKernelStack`); and kernel objects have no `OBJECT_HEADER` (a title's
  inlined wait reads `Type->DefaultObject` at body-8): `CreateOwnedObject` now puts a 0x10-byte header before the body.
  Also `ObOpenObjectByName` (not found).
- **GPU**: render targets `k_32_FLOAT` and `k_2_10_10_10_FLOAT` (and `_AS_16_16_16_16`) in the EDRAM output merger (7e3 floats as
  Xenia); resolve of those to `k_16_16_16_16_FLOAT` (64 bits per pixel, 8 KiB macro tiles); `k_16_16_16_16_FLOAT` / `_EXPAND` textures.
- `XexLoadImage` of a module `recompile-project` has not translated: `STATUS_DLL_NOT_FOUND` instead of a stop.

Still open, by titles:
1. **64-bit render targets** (`16_16_16_16_FLOAT`, `16_16_16_16`): Catherine, Lumines, Small Arms (3). A 64-bit pixel is two EDRAM
   dwords (x doubled) with its own pitch in tiles (twice the 32-bit one), and the renderer's tracking of the tiles a surface covers
   (`rovSpan`, resolves) must know it.
2. **3D textures** (`tfetch3D`, volume tiling `GetTiledOffset3D` of rexglue's `texture/util.cpp`, a Texture3D and its untile pass): Cloning Clyde,
   UNO (the Xbox Live Arcade one), Vigilante 8 (3). Not started: the layout (z strides, 3D mips) and the HLSL translator need the same pass.
3. **A second recompiled module**: Forza 4 (`XMediaFacade_default.xex`), Terraria (`HostLoader.dll`), Left 4 Dead 2 (the engine DLLs).
   `recompile-project` recompiles `default.xex` only; the runtime links several modules already (the dashboard's), so this is a
   project-generation step (find the title's other XEX/DLL files, one configuration each, linked and named).
4. Resolve to 8-bit textures (`k_8`, Monopoly Plus; a byte store per pixel).
5. Top Spin 2's loader thread (a critical section at 0x3721C, after `ObOpenObjectByName`); Heavy Weapon (null read at 0x70 after
   `resources.xpr`, not yet traced); `KeSetCurrentStackPointers` (Street Fighter II' HF calls it 6,348 times: fiber switches; the
   arguments are `(sp?, thread, base, base, limit)`, and treating r3 as the new r1 broke the title, so it stays unimplemented);
   `XeKeysSetKey`/`XeKeysAesCbc` (Rock Band Blitz); XMA decode errors.
