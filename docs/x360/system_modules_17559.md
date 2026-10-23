# Dashboard 2.0.17559 system modules (facts for both projects)

Source: the owner's system update `SystemUpdate_17559_USB` (`$SystemUpdate/`: HDD modules and `L.*.xzp` language
packs; `su20076000_00000000` is an STFS package holding the flash files `$flash_*`). Measured with
the XEX engine (`codebreaker xex`) and `xex-imports` (2026-10-14). All modules below are unencrypted XEX2
(LZX-compressed, except the HDD dash.xex: basic), except `Xna_TitleLauncher.xex` (encrypted).

## The two modules the dashboard project recompiles first

| Module | Base | Entry | Code | .pdata functions | Imports | Exports |
|---|---|---|---|---|---|---|
| `dash.xex` (title FFFE07D1) | 0x92000000 | 0x92196610 | .text 0x9217E000, 0x7C0C8C | 28,350 | xam 522, xboxkrnl 300 | 1,217 (ordinals 10000+, used by dash plugins) |
| `bootanim.xex` (flash) | 0x98000000 | none (DLL) | .text 0x9800E000, 0x4A33C | 661 | xboxkrnl 114 | 1 @ 0x98020E60 (start), 2 @ 0x98020EC8, 3 @ 0x98020ED8 |

- `$flash_dash.xex` (flash) and the HDD `dash.xex` flatten to the **same image** (identical hash).
- The HDD `dash.xex` is already **basic-compressed**; the flash copy and bootanim.xex are LZX. `tools/scripts/xex_basic.py`
  rewrites any unencrypted XEX as basic-compressed (what both runtimes' loaders read).
- **dash.xex imports no XUI functions from XAM**: it links its own XUI runtime, D3D and Lua statically (like TU46 links
  the XDK's XUI). Its UI therefore runs as recompiled code over the kernel's Vd*/GPU path, exactly as TU46's does.
- dash.xex's import header has **three blocks** (xam.xex, xam.xex, xboxkrnl.exe).
- dash.xex resources (XUI packages): dashmain, dashcomm, dashuisk (skin), controlp, SharedUI, hubui/hubapp, dashlua,
  luaxbox, soclua, socxzp, neon, consoles, contui/contapp, memory, network, music, videos, pictures, epix, oobe,
  signinpr, signupbo, gamer, gamercar, friendsc, slots, dvd, download, parental, thermal, iptv, mediasit, arcade, plus
  `FFFE07D1.bin` (the dash's own title data, probably its SPA/XDBF). Language packs: `L.dash.xex.*.xzp`.
- bootanim.xex: one resource, `audio` (RIFF, 137,560 bytes: the boot sound). On a console the kernel starts it
  (`AniStartBootAnimation` calls export 1; Nexia360 does the same). It imports only xboxkrnl: it drives the GPU ring
  buffer (Vd*), the scaler and XMA/XAudio directly.

## Other modules

| Module | Base | Imports | Role |
|---|---|---|---|
| Dash.Search.xex | 0x9AD00000 | krnl, xam, dash | dash plugin |
| Dash.NetworkStorage.lex | 0x9AC00000 | xam, krnl, dash | dash plugin (cloud storage) |
| Dash.Kiosk.lex | 0x9B610000 | xam, krnl, dash | dash plugin (store kiosk mode) |
| Dash.FieldCalibration.lex | 0x9BD10000 | krnl, dash, xam | dash plugin |
| Dash.NuiTroubleshooter.lex | 0x9CB00000 | krnl, xam, dash | Kinect |
| dash.natalpregame.xex | 0x9C780000 | krnl, dash, xam | Kinect |
| dashnui.xex | 0x9E510000 | dash, krnl, xam | Kinect dashboard |
| dash.ClosedCaptionDll.xex | 0x9A9E0000 | dash, krnl, xam | media |
| dash.ExtraAVCodecs.xex / 2 | 0x9F7E0000 / 0x9C500000 | krnl | media codecs |
| PlayReady.xex | 0x9B5F0000 | krnl, xam | DRM (media) |
| AvatarEditor.xex, BiometricSetup.xex, Title.Zune.xex | 0x92000000 | xam, krnl | separate titles (replace the dash) |
| Guide.*.xex, nuihud.xex, Xam.WordRegister.xex | 0x90100000 | xam, krnl | Guide apps loaded by XAM |
| Xam.Community.xex | 0x91240000 | xam, krnl | XAM social |
| natalsu.xex | 0x98000000 | xam, krnl | Kinect update |
| FFFE07DF0000000{1,2,6,8} | — | — | avatar asset packages |

Flash (`su20076000_00000000` -> `$flash_*`): xam.xex (XAM, at 0x815F0000), hud.xex (the Guide,
0x913E0000), huduiskin, signin, vk, gamerprofile, createprofile, deviceselector, bootanim, dash, updater,
systemupdate, mfgbootlauncher, minimediaplayer, fonts (SegoeXbox-Light.xtt, xenonclatin/xenonjklatin.xttp), ...

## bootanim.xex protocol (from its own code and a reading of the kernel)

- The kernel's `AniStartBootAnimation`: `XexLoadImage("\Device\Flash\bootanim.xex", 0x40000009, 0x20449700)`, then
  `ExCreateThread(stack 0x8000, flags 0x020000A0)` whose body calls **ordinal 1(handle, flags)**. `flags` (a kernel boot
  word) bit 0x8000 = display off (no animation), 0x4000 = alternate device setup; a power-on passes 0.
- **Ordinal 2(terminate)**: 0 = finish the sequence then end (`AniBlockOnAnimation`), 1 = stop now
  (`AniTerminateAnimation`). Ordinal 3 draws a logo (`AniSetLogo`). XAM imports the Ani* exports.
- It sends SMC 0x8C (set power LED) `8C 01 01` at the first frame and `8C 01 00` at the end (the ring of light).
- Its D3D runs in the **system process** and registers in `VdGlobalXamDevice`. D3D's graphics interrupt routine
  (0x9802FD90) and its callbacks pick the device with `KeGetCurrentProcessType` without setting it: the kernel must run
  the interrupt callback in its registrant's process.
- Its XAudio mixes inside the render-driver callback with per-CPU state for **hardware thread 4** (looked up through
  KPRCB.CurrentCpu, PCR +0x10C): the kernel's audio driver calls its clients on CPU 4.
- Natural length: ~600 frames (about 10 s): ribbons, orb, logo at ~5.5 s, logo held until the end; the boot sound
  (XMA, ~6 s).
