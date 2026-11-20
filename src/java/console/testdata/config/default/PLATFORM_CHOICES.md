# PLATFORM_CHOICES.md -- user decisions the converter emitted

These are the questions only you can answer. The current pick is checked; change it in
`platform.config.json` (or via `codebreaker java platform-config`) and re-emit. Everything below is wired
into the port -- ControllerConfig.h, KernelConfig.h, Kernel.cpp.

## Controller mapping

- Layout: **default** (options: default, southpaw, legacy, legacy_southpaw)
- Invert Y: **no**   Sensitivity: **1.00**   Stick deadzone: **0.24**   Trigger: **0.30**

| Action | Bound to |
|---|---|
| Mouse look (camera) | `RSTICK` |
| Move (WASD) | `LSTICK` |
| Attack / break (left mouse) | `RTRIGGER` |
| Use / place (right mouse) | `LTRIGGER` |
| Jump (space) | `A` |
| Sneak (shift) | `LSTICK_CLICK` |
| Inventory (E) | `Y` |
| Drop item (Q) | `B` |
| Hotbar left (wheel up) | `LBUMPER` |
| Hotbar right (wheel down) | `RBUMPER` |
| Pause / menu (esc) | `START` |
| Chat / command (T) | `DPAD_UP` |
| Toggle view (F5) | `DPAD_DOWN` |
| Sprint | `RSTICK_CLICK` |
| Virtual cursor in menus | `LSTICK` |

Available controls to rebind to: `A`, `B`, `X`, `Y`, `LBUMPER`, `RBUMPER`, `LTRIGGER`, `RTRIGGER`, `LSTICK`, `RSTICK`, `LSTICK_CLICK`, `RSTICK_CLICK`, `DPAD_UP`, `DPAD_DOWN`, `DPAD_LEFT`, `DPAD_RIGHT`, `START`, `BACK`.

## Subsystem decisions

Each of these is a real question you must answer for the port. The recommended answer and why are given; the current pick is checked.

### Text input — How does the user enter text (names, chat, addresses)?

*Why it matters:* A console has no attached keyboard by default. Any text field needs an input method, or the user is stuck at that prompt — required whenever the program asks for typed input.

*Recommended:* `xshowkeyboard` — the built-in system keyboard is familiar and needs no custom UI

- [x] **System virtual keyboard (XShowKeyboardUI) -- RECOMMENDED** (`xshowkeyboard`)  ⭐ — The platform's built-in on-screen keyboard. Blocks while open; returns the typed string. The standard way any console app takes text input.
- [ ] **In-game on-screen keyboard** (`osk`) — A custom controller-driven keyboard you render yourself. More work, fully styled, no system overlay.
- [ ] **Stub (fixed strings, no text entry)** (`stub`) — Text fields return a fixed placeholder. Boots, but the user cannot type. Bring-up only.

### Save / load storage — Where does save data live, and must it survive a reboot?

*Why it matters:* This decides whether saves persist across sessions and whether the build can pass the platform's certification. The wrong choice loses data on exit or fails cert.

*Recommended:* `stfs` — a proper content container persists across reboots and passes certification

- [x] **STFS content container (XShowDeviceSelectorUI + XContentCreate) -- RECOMMENDED** (`stfs`)  ⭐ — Persistent per-title saves. The device selector picks HDD/USB once; XContentCreate mounts a writable "Save:" root the save files live under. Survives reboots, passes cert.
- [ ] **Raw HDD dev path (game:\Save\)** (`rawhdd`) — Writes straight to a dev-kit path. Simple, persists on devkit, but NOT a content container (no device select, will not pass retail cert).
- [ ] **Memory-only (no persistence)** (`memory`) — Save data lives in RAM and is lost on exit. Boots and plays, but nothing persists.

### Audio — Which audio backend plays music and sound effects?

*Why it matters:* The platform offers a high-level banked audio system and a low-level voice API. The banked system provides an authoring pipeline; the low-level one makes you handle mixing and decode.

*Recommended:* `xact3` — the banked system saves you writing mixing, streaming, and format decode by hand

- [x] **XACT3 (wave/sound banks)** (`xact3`)  ⭐ — Authored wave/sound banks, cue-driven. A bank-authoring pipeline instead of hand-written mixing.
- [ ] **XAudio2 (source voices)** (`xaudio2`) — Low-level source voices; you own mixing, streaming, and format decode. More code, no authoring pipeline.
- [ ] **Silent (no audio)** (`silent`) — No sound output. Boots quietly — bring-up only.

### Video — How is the program's OpenGL rendering realized on this platform?

*Why it matters:* The console has no OpenGL — every GL call must reach the native graphics API (Direct3D 9). A translation layer keeps the existing render code; a native rewrite is far more work.

*Recommended:* `gl_emulator` — translating GL→D3D9 reuses the existing render code instead of a full rewrite

- [x] **GL→D3D9 emulation layer** (`gl_emulator`)  ⭐ — Map each OpenGL call onto Direct3D 9 through a translation layer — keeps the existing render code.
- [ ] **Native Direct3D 9 backend** (`d3d9_fresh`) — Rewrite the renderer directly against D3D9. Most control, most work.
- [ ] **Stub (renders nothing)** (`stub`) — No-op graphics — the game runs but draws nothing. Bring-up only.

### Networking — Does the program need networked multiplayer — and how?

*Why it matters:* This gates whether the networking code is live. Sockets enable networked play; offline builds skip it entirely. Play across different subnets may need a relay you provide.

*Recommended:* `sockets` — sockets keep multiplayer working; offline disables it completely

- [x] **Sockets (networked multiplayer)** (`sockets`)  ⭐ — BSD-style sockets over the console network stack. Off-subnet play may need a relay.
- [ ] **Offline (single-player only)** (`offline`) — No networking — single-player only.

### Memory — How should the program report how much memory is free?

*Why it matters:* Some program logic branches on available memory. Reporting the platform's true budget keeps that behavior honest; a fixed number is only safe if nothing reads it.

*Recommended:* `real_budget` — report the true budget unless you know no code depends on the value

- [x] **Real console budget** (`real_budget`)  ⭐ — Report the platform's true free/total memory (via the console memory API).
- [ ] **Fixed constants** (`fixed`) — Free/total memory return a nominal number — fine unless game logic branches on it.

### User / profile — How is the user's identity / profile obtained?

*Why it matters:* Used for the on-screen name and any per-user data. The real name comes from the console profile API; the stub hardcodes a placeholder.

*Recommended:* `profile` — reading the signed-in profile makes the on-screen name correct

- [x] **Signed-in profile name** (`profile`)  ⭐ — Read the console user/profile name for the on-screen identity.
- [ ] **Fixed name** (`stub`) — Hardcode a placeholder identity.

### Debug support — Include the debug instrumentation in this build?

*Why it matters:* Emits one toggleable debug file (a single master switch, greppable markers, rate-limited output). Keep it on while bringing the port up; turn it off for a clean release build.

*Recommended:* `on` — keep it on during bring-up so crashes and leaks are traceable

- [x] **On (emit debug file)** (`on`)  ⭐ — Emit the single toggleable debug file (master switch, greppable markers, rate-limited output).
- [ ] **Off (no debug support)** (`off`) — Do not emit any debug facility.

### Achievements — How are the program's achievements delivered?

*Why it matters:* A console can't invent achievements at runtime — each must be declared in the title config and unlocked by a networked write to the signed-in profile (needs a Title ID, a profile, and network; offline it queues). Local-only skips the online write.

*Recommended:* `online` — online writes give real profile achievements; needs a Title ID + signed-in user

- [x] **Online profile achievements** (`online`)  ⭐ — Real achievements written to the signed-in profile over the online service. Needs a Title ID + network (queues offline).
- [ ] **Local-only (no online write)** (`local`) — Track unlocks locally; nothing is written to the online profile. Bring-up / no-LIVE builds.
- [ ] **No achievements** (`off`) — Skip achievement handling entirely.

## SDK

Chosen mediums need no SDK-gated kernel symbols (or a portable target).

