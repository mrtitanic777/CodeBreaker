# Recompiled titles: playability status

2026-11-15. Every Xbox 360 title in `C:/Nexia360/Games` that `recompile-project` accepted, checked with
`recompile-project <path> --check 120` (recompile, build, run headless for 120 s). Tetris Splash, BlockWorld and Up
are left out as asked. "Frames" is what the title presented in 120 s (60 a second when it runs, so 6600 is the
ceiling of this check). The first stop is the first thing that ended the run. Method: `work/recompile-track`
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

## What was implemented (src/recompile/x360)

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
- **Recompiler** (`src/recompile/xenonrecomp`): a `.pdata` entry that is all zeros (begin address 0) is no longer a function, and a zero word
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

## Round 3 (2026-11-15, evening)

The 54 entries again (`--check 120`, one project at a time, deleted after), with Monopoly Plus and Happy Wars rerun after their last fixes.

| Folder | Title | Functions | Frames | Stop |
|---|---|---|---|---|
| 584107D1 | Hexic HD | 7563 | 6000 |  |
| 584107D5 | Small Arms | 42521 | 12 | no frame presented for 20 s after frame 239 |
| 584107D7 | Marble Blast Ultra | 17631 | 6600 |  |
| 584107E7 | Cloning Clyde | 12753 | 2400 |  |
| 584107ED | Geometry Wars Evolved | 3378 | 6600 |  |
| 584107F1 | Feeding Frenzy | 9795 | 6600 |  |
| 584107F3 | UNO | 8391 | 6600 |  |
| 584107F4 | Street Fighter II' HF | 13051 | 6600 |  |
| 584107F8 | LUMINES LIVE! - Base Pack | 9629 | 6600 |  |
| 584107FE | Assault Heroes | 21947 | 6600 |  |
| 58410810 | Heavy Weapon | 19041 | 1 | Guest memory read fault at 00000070 in guest function 820CA570 (host +45E) on thread 1 (last guest c |
| 58410811 | PAC-MAN | 8504 | 6600 |  |
| 58410841 | 3D Ultra Minigolf | 23428 | 6600 |  |
| 58410870 | TMNT 1989 Classic Arcade | 11073 | 6600 |  |
| 584108A4 | SuperStreetFighter2THD | 15413 | 6600 |  |
| 584108A8 | Vigilante 8 Arcade | 8268 | 6600 |  |
| 584108B7 | Castle Crashers | 3938 | 6600 |  |
| 58410AE9 | Happy Wars | 331546 | 1 | texture k_16_16_FLOAT (after the k_16_16_FLOAT resolve and 16_16_FLOAT target) |
| 584111D6 | Bejeweled 3 | 30246 | 6600 |  |
| 584111F7 | Minecraft Xbox 360 Edition | 43128 | 6600 |  |
| 5841128F | Terraria | 1740 | 0 | no frame presented for 20 s after frame 0 |
| 423607D1 | (title 423607D1, GoD) | 17598 | 0 | XexGetProcedureAddress on module handle 00000000 (ordinal 18): only system modules are supported |
| 423607D3 | (GoD image unreadable: xdvdfs read before the start) |  | - |  |
| 415487DB | Catherine Demo | 41103 | 2400 |  |
| 415608CB | (not an Xbox 360 title: no default.xex) |  | - |  |
| 545487F5 | Top Spin 2 Wimbledon Demo | 27674 | 1 | no frame presented for 20 s after frame 266 |
| 454188D6 | Left 4 Dead 2 Demo | 270 | 0 |  |
| 4D5389F0 | Forza Motorsport 4 | 47808 | 0 | no frame presented for 20 s after frame 0 |
| 45410806 | (EA, disc, 33,319 functions) | 23123 | 6600 |  |
| Arcadecraft | (not an Xbox 360 title: no default.xex) |  | - |  |
| Bomberman LIVE | Bomberman LIVE | 10944 | 6600 |  |
| DIG-DUG | DIG DUG | 8789 | 6600 |  |
| Doritos Crash Course | Doritos Crash Course | 25945 | 6000 |  |
| Echo The Dolphin | Ecco the Dolphin | 9264 | 6600 |  |
| GALAGA | GALAGA | 8567 | 6600 |  |
| Joy-Ride-Turbo | Joy Ride Turbo | 77975 | 1 | NtQueryInformationFile class 27 is not implemented (C:\Users\mrtit\Documents\GitHub\CodeBreaker\Conv |
| MS. PAC-MAN | MS.PAC-MAN | 8802 | 6600 |  |
| Millipede & Centipede | Centipede & Millipede | 12574 | 6600 |  |
| Monopoly PLUS | MONOPOLY PLUS | 57814 | 1800 | no frame presented for 20 s after frame 233 (a stall) |
| PAC-MAN CE DX | PAC-MAN CE DX | 11372 | 6600 |  |
| PAC-MAN CE | Pac-Man C.E. | 6548 | 6600 |  |
| PAC-MAN MUSEUM | PAC-MAN MUSEUM | 46345 | 0 | Indirect call to 821BED60, where no recompiled function starts (call at 821BFDBC) |
| RALLY-X | NEW RALLY-X | 8791 | 6600 |  |
| REAL-STEEL | REAL STEEL | 17966 | 6600 |  |
| Rock-Band-Blitz | Rock Band Blitz | 42819 | 6600 |  |
| Spongebob Underpants Slam | SpongeBob UnderPants! | 22377 | 6600 |  |
| TEMPEST | Tempest | 15406 | 6600 |  |
| TRON | TRON | 10700 | 6600 |  |
| TrialsHD | Trials HD | 18199 | 6600 |  |
| UNO Rush | UNO RUSH | 18169 | 6600 |  |
| WALL-E | WALL-E Demo | 25815 | 5400 |  |
| Zuma | Zuma | 11289 | 6600 |  |
| MW3.iso | Modern Warfare 3 | 24603 | 6600 |  |
| DEFENDER |  | 10025 | 6600 |  |

40 of the 54 entries run the whole check with no stop (frames 1800-6600; Cloning Clyde and Catherine are CPU-bound at 20 frames a second); round 2 had 31, round 1 24. 3 are not recompilable titles.

### Implemented in round 3 (src/recompile/x360)

- **64-bit render targets**: `16_16_16_16_FLOAT` in the EDRAM output merger (two dwords a pixel with x doubled, a pitch of twice the tiles
  in `xe_om_poly_back.y`), as resolve source (converted to `k_16_16_16_16_FLOAT`) and for the colour clear (two dwords). Also render targets
  `2_10_10_10` and `16_16_FLOAT`, resolve to `k_8` (a byte store by interlocked and/or), `k_16_16_FLOAT` and `k_16_16_16_16_FLOAT`.
  Lumines: 0 to 6600 frames; Catherine and Small Arms run until a later stall.
- **Textures**: gamma (sRGB view of RGB), `DXN`, `2_10_10_10` (and `_AS_16_16_16_16`), and **3D textures**: a Texture3D untiled with
  rexglue's volume tiling (base level only; not exercised yet: the three titles use `tfetch3D` on 2D constants).
- **`tfetch3D` of a 2D constant** (shader variant by constant: `volumeAs2D`) and `getCompTexLOD`: Cloning Clyde, UNO and Vigilante 8
  run (Cloning Clyde 1 to 2400 frames, UNO 1 to 6600, Vigilante 8 103 to 6600).

### Traced, not fixed

- **Heavy Weapon**: the read at 0x70 is `*(*(owner+72)+2684)+112` in `sub_820CA570` (a title object's field +2684 never set); the stores to that
  offset are in a dozen game functions, so it is game state, not a kernel call we answered wrongly.
- **Top Spin 2**: the loader thread's `LeaveCriticalSection` is called with 0x3721C (a `this` of 0x37214 from an uninitialised table) after
  `ObOpenObjectByName` answers not found: the title expects a named object that exists on the console.
- **Stalls** in Catherine (a thread spinning in `KeGetCurrentProcessType` callers), Small Arms and Monopoly Plus after frames 233-745: not traced.
- **Extra modules** (Terraria, L4D2, Forza 4): `recompile-project` would need one XenonRecomp configuration per XEX/DLL (with the function
  suffix the config has), a `module_info` and registration for each in `x360.cmake`, and `XexLoadImage` loading a linked module by name
  and calling its entry; not started.

## Round 4 (2026-11-15, night)

Items 1-6 of the round. Measured with `--check 120` (one project at a time, deleted after) for the titles the round touched and a regression
set; the rest of the 54 entries were not rerun (Happy Wars alone is a 40-minute build; see "Left").

| Title | Round 3 | Round 4 |
|---|---|---|
| Terraria | 0 frames, `XexLoadImage(HostLoader.dll)` not translated | 0 frames: HostLoader.dll, RuntimeHost.xex and NetCFUserMode.dll load by name and run their entries; stops at `KeCreateUserMode` / `KeEnterUserMode` (the .NET Compact Framework's JIT, below) |
| Forza Motorsport 4 | 0 frames (no frame for 20 s) | 1 frame: XMediaFacade_default.xex and SpeechFacade_default.xex translated and linked, XMediaFacade loads and answers; stops at `NtQueryInformationFile` class 26 (`media\ui\textures.zip`) |
| Left 4 Dead 2 Demo | 0 frames (`tier0_360.dll`) | 21 modules translated (112,000 functions); the first build failed to link (`__imp__vstdlib_360_Ordinal_<n>`; fixed in `gen_import_stubs.py`, not rebuilt: a 40-minute build) |
| Top Spin 2 Wimbledon Demo | 1 frame, loader thread stalls after frame 266 | 6000 frames, no stop |
| Monopoly Plus | no frame for 20 s after frame 233 | 1800 frames (the "stall" is a burst of shader compiles) |
| Small Arms | 12 frames, stall after 239 | past frame 236, then FXC compiles of 2.3 s each (94 s in 110 s): a cold shader cache, not a stall |
| Catherine Demo | 2400 frames | 1800 frames, no stop (the same compile cost) |
| Heavy Weapon | 1 frame, null read at 0x70 | unchanged (see 4) |
| Regression set | | Hexic HD 6600, UNO 6600, Cloning Clyde 2400, Vigilante 8 6600, Lumines 6000, SF2 HD 6600, Rock Band Blitz 6600, WALL-E 5400, Minecraft 6600, Zuma 6600, PAC-MAN 6600, the EA title 6600, Geometry Wars 6600 |

1. **Extra modules.** `recompile-project` translates every XEX in the title's folder that is a PowerPC program (code and `.pdata`, not a CLR
   image, more than the 18 KB resource stub every .NET assembly and `.nlp` table of Terraria carries): one configuration, switch tables and
   `module_info.cpp` each in `src/modules/<key>/`, symbols suffixed `_<key>` (`symbol_suffix`, `suffix_function_names`), the module's image in
   `assets/` replaced by its basic form; a module that fails to translate is left out and reported (`reports/modules.txt`). `x360.cmake` builds each
   as a library and links it; the title's `module_info.cpp` lists them (`RecompiledModule::extras`, `path`). The runtime's `XexLoadImage` loads a
   listed module by name (any path, any case), binds its imports (an import of another module's export loads that one first; calls go through the
   `<library>_Ordinal_<n>` stub to the export), runs `DllMain(DLL_PROCESS_ATTACH)`, and `XexGetProcedureAddress` finds its exports. Also: an
   export is a function root for the discovery (Forza's XMediaFacade has leaf exports no `.pdata` lists and nothing calls); an executable section
   counts as code in XenonRecomp even without the code flag (NetCFUserMode.dll's `.text`); exports the runtime implements as imports are found by
   their ordinal name (`gen_ordinal_names.py`; `XamIsUIActive` 748 for Terraria); `NtSignalAndWaitForSingleObjectEx`; a 10-bit front buffer
   (`k_2_10_10_10_AS_16_16_16_16`, identity ramp; Forza); and a relaunch loop closed: a title that asks to go back to the dashboard before any
   frame, in a process that is itself a relaunch, no longer starts itself again without end (Terraria spawned a hundred processes).
   Terraria cannot go further statically: the Compact Framework JIT-compiles managed code to PowerPC at run time (`KeCreateUserMode`,
   `KeEnterUserMode`, a user-mode window at `BF8A....`), code no recompiler has seen.
2. **The stalls** are not stalls. Catherine, Small Arms and Monopoly Plus recover on their own: each new scene brings 50-60 shaders, FXC takes 1-3 s
   for each on the command processor's thread (`dxcompiler.dll` is not on this machine), so a 20 s watchdog fires and the check's clock runs out
   in a cold shader cache. A second run reads `shader_cache/` and the manifest queues its shaders at start-up. FXC optimisation level 1 and
   queuing a draw's two shaders side by side were tried and did not shorten it (the translated HLSL is large); neither is kept.
3. **Happy Wars `k_16_16_FLOAT`**: texture formats `k_16_16_FLOAT` and `k_16_FLOAT` (halves to floats, `kUntileHalf2`), in the CPU reference and the
   random-texture test (400 random textures, 1451 levels, 0 differ). Happy Wars itself was not rerun.
4. **Top Spin 2**: `ObOpenObjectByName` answered "not found" for every name, so a title that creates an event by name and opens it by name never
   found its own; it now looks the name up among the named objects (`NtCreateEvent` and the others register them). The console's own names
   (`kConsoleNamedObjects`) are created on first open: `XNAGSVEN<40 hex>`, the XNA game-services event Terraria's HostLoader opens before it starts
   the managed runtime (signalled). Top Spin 2: 1 to 6000 frames. **Heavy Weapon** is in its game loop (polls `NetDll_recvfrom` and
   `XNotifyGetNext`, reads profile settings of user FFFE07D1 five times) when it reads the unset field; the call that should have set it is
   not found yet.
5. **Texture3D**: `CB_SHADER_TEST=1|2` (it was no longer wired into `main.cpp`) runs the GPU harness; it now has 7 volumes (tiled and linear, 1, 2
   and 4 bytes a texel, sizes that are not tile multiples) laid out by `ReferenceVolumeOffset` and untiled by the GPU pass, read back slice by
   slice: 0 differ; and a check that the 32 x 32 x 4 tiling is a bijection of the padded volume over 18 shapes. The formula is the untiler's own,
   verified as a permutation and through the GPU path; no title with a real volume texture has run yet (the three that fetch a volume use 2D
   constants).
6. `CB_KERNEL_TEST=2`: all passed (on Top Spin 2's program). `CB_SHADER_TEST=2`: 0 failures (present pass with the 10-bit buffer, random
   textures, volumes, sampling).

Left: Happy Wars (rerun), L4D2 (rebuild with the stub fix and run), Forza past class 26, Heavy Weapon's field, Terraria's JIT, the full
54-entry table.

## Round 5 (2026-11-15 to 11-18)

All 54 entries measured again with `--check 120` (one project at a time, deleted after; scratch `work/x360-r5`), after the changes below.
Frames are what the title presented in 120 s (the check's ceiling is about 7200; the log prints every 600). 46 of the 51 recompilable
titles run the whole check with no stop (round 3: 40); the sweep ran before the Joy Ride Turbo and Game-on-Demand 423607D1 fixes, which
are re-run by hand (below).

| Title | Round 4 | Round 5 |
|---|---|---|
| Catherine Demo | 1800 frames, a 21 s hitch at frame 744 | 3000, no stop; 118 compiles, 12.9 s in all (FXC: 33.0 s) |
| Small Arms | past frame 236, 2.3 s a compile (94 s of 110) | 2400, no stop; 97 compiles, 314 ms on average |
| Monopoly Plus | 1800 | 4800 |
| Left 4 Dead 2 Demo | 0 frames (link failed) | 2400: the four engine DLLs load and run, the intro movie plays |
| Forza Motorsport 4 | 1 frame, `NtQueryInformationFile` class 26 | 4800: the intro video plays |
| Happy Wars | 1 frame (`k_16_16_FLOAT`, not rerun in round 4) | 3000 (15-minute build) |
| Joy Ride Turbo | 1 frame, class 27 | 1 in the sweep; 2700+ after the fixes below |
| Game-on-Demand 423607D1 | 0, the thread-name exception | 0 in the sweep; past it after the fix: stops at `XamContentOpenFile("stexemplar", "update:\Database.xmplr")` |
| Hexic HD, Lumines | 6600, 6000 | 6600, 6600 (the 6000 was machine load: other sessions were building; the run is not the one that moved) |
| Heavy Weapon | 1 frame, null read at 0x70 | 5 frames, same read: traced, below |
| Terraria, PAC-MAN Museum | 0 | 0 (the .NET JIT; indirect call to 821BED60) |
| Everything else | | unchanged: 6600 frames (WALL-E 5400, Cloning Clyde 3000), no stop |

1. **Shader compile time.** The real cost was FXC. `dxcompiler.dll` is not on the PATH but the Windows SDK ships it (with `dxil.dll`);
   round 4 had looked for it beside the program and not found it. `x360.cmake` now copies both next to every title's program (from the SDK, or
   `CB_DXC_DIR`), and the runtime's existing DXC path compiles with them: on Catherine's four largest shaders 0.43-0.55 s instead of 0.85-2.3 s
   (FXC) and half the bytecode (23.8 KB against 51 KB for the 1.4 s vertex shader), and over a whole run 118 compiles took 12.9 s instead of
   33.0 s, the slowest 375 ms instead of 1505 ms. The harness passes with DXC (`CB_SHADER_TEST=1`: 57,696 instruction states, 25,600
   output-merger cases, the resolves, the present pass and texture sampling: 0 mismatches), so the bit-exact translation survives it. And the
   compile workers are on by default now (`CB_ASYNC_SHADERS=0` for the old behaviour and the pixel-exact tests): the command processor never
   waits for a compile, a draw whose shaders are not ready is skipped (first run only: the manifest and `shader_cache/` start the next run with
   everything compiled). The HLSL itself was not made smaller: with DXC it no longer is the limit.
2. **L4D2.** The round-4 import-stub fix links. Three things stood in the way: an XEX whose import-name table has stray bytes in its padding
   (the engine DLLs: the name after `xboxkrnl.exe` read as `\`, so the module's variable imports of `vstdlib` found no library; `src/formats/xex`
   reads the table at its 4-byte-aligned offsets now, as XenonRecomp's reader always did), `FscGetCacheElementCount` (still unimplemented and
   skipped by the check), and render target format 1: **`8_8_8_8_GAMMA` in the output merger** (the 8_8_8_8 dword holds gamma-encoded RGB; the
   shader's linear colour is converted on store and back for blending, with the piecewise-linear curve of slopes 1/4, 1/2, 1, 2; the CPU
   reference and the random-key test cover it).
3. **Forza 4.** Past class 26 and onward: `NtQueryInformationFile` class 26 (not an NT file class; a 4-byte buffer the title's file wrapper
   returns to its caller right after opening a zip: the file length), a read longer than the rest of a file now transfers the bytes that exist
   (the host call checked the whole 128 KiB buffer the guest had not committed), the **10-bit front buffer through the piecewise-linear gamma
   ramp** (128 segments a channel, 10-bit input c: base + delta * (c & 7) / 8 of entry c >> 3; the GPU pass and the CPU reference agree) and
   texture format `k_8_8_8_8_AS_16_16_16_16` (the 8_8_8_8 layout; sampled as the same unorm or gamma value).
4. **The 54-entry table**: above. Lumines and Hexic did not change.
5. **Heavy Weapon, traced further: not game state.** `X+2684` (X is the title object at `0x40001850`) is a 120-byte object created in
   `sub_82173B90` (a loader thread, created by the main thread, which carries on). That thread's last calls are XMA (`XMADisableContext`,
   `XMAIsInputBuffer0Valid` ...) on a context whose first frame never decodes: `APU: frame at buffer 0 bit 988 does not decode: channel
   transform bit`, 406,000 times. The WMA Pro decoder stops on a set channel-transform bit (FFmpeg's decoder, which ours follows, does too), the
   context parks with a bitstream-corruption error as the hardware's would, the title waits for audio that never arrives, and the main thread
   reads the null object. The way on is the channel transform of the WMA Pro frame (and whether the frame is XMA1 rather than XMA2).

Also: `RtlRaiseException` of 0x406D1388 (the debugger-only thread name) returns; `RtlDowncaseUnicodeChar`; `NtPulseEvent`;
`NtQueryInformationFile` class 27 (a dword, bit 0 set: the wrapper of Joy Ride Turbo's data.bpk takes another path with 0 and reads through a
null buffer).

Left: the channel transform in the WMA Pro decoder (Heavy Weapon), `XamContentOpenFile` of a non-HDD package (423607D1), Terraria's JIT,
`FscGetCacheElementCount`, PAC-MAN Museum's indirect call, a host render target path for `8_8_8_8_GAMMA` (it takes the ROV output merger).
