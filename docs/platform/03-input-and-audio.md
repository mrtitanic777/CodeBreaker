# 03 — Input and Audio

> **CORRECTION (authoritative — supersedes the XInputGetState text below).** The 360 controller is
> driven by **XInput2** (`xinput2.h` + `xinput2.lib`/`xinput2d.lib`), NOT the PC `XInputGetState`/
> `XINPUT_STATE` API. There is no `xinput.h` in this XDK. The live `Input.cpp` is implemented against
> XInput2 and verified against `C:\Program Files (x86)\Microsoft Xbox 360 SDK\include\xbox\xinput2.h`.
>
> **How to read the pad (one sample per frame):**
> ```c
> XINPUT2CONTEXT ctx; BOOL changed;
> if (XInput2Sample(XInput2InputDevice(0), &ctx, &changed)) {
>     BOOL a; XInput2GetBool(ctx, XINPUTID_A, &a);                  // buttons: XINPUTID_A/B/X/Y/DPAD_*/START/BACK/*_SHOULDER/*_THUMB
>     float rt; XInput2GetFloat(ctx, XINPUTID_RIGHT_TRIGGER, &rt);  // triggers: normalized float
>     XINPUT2_POINT ls; XInput2GetPoint(ctx, XINPUTID_LEFT_THUMBSTICK, &ls);  // sticks: XINPUT2_POINT {x,y} normalized
> }
> ```
>
> **ONE unified controller IO — there is no keyboard or mouse.** `Input.cpp` reads the pad and feeds a
> virtual `Keyboard::`/`Mouse::` model purely so the faithful LWJGL-derived game code keeps compiling;
> nothing reads a real K/M. Two modes, switched automatically by `g_grabbed` (set by `MouseHelper`
> grab/ungrab — grabbed = in-world, ungrabbed = a GUI is open):
>
> | Control            | In-world (grabbed)                 | In a GUI (ungrabbed)                         |
> |--------------------|------------------------------------|----------------------------------------------|
> | Left stick         | move (analog + W/A/S/D fallback)   | move the virtual cursor                      |
> | Right stick        | camera look (getDX/getDY)          | (unused)                                     |
> | D-pad              | (unused)                           | nudge the virtual cursor                     |
> | A                  | jump (SPACE)                       | **click** the widget under the cursor        |
> | B                  | drop item (Q)                      | back / close (ESCAPE)                        |
> | Y                  | inventory (E)                      | —                                            |
> | RT / LT            | attack (mouse0) / use (mouse1)     | —                                            |
> | LB / RB            | hotbar prev / next (wheel)         | —                                            |
> | L3                 | sneak (LSHIFT)                     | —                                            |
> | START              | pause (ESCAPE)                     | back / close (ESCAPE)                        |
>
> GUI widgets highlight under the cursor (GuiScreen hover via `Mouse::getX/getY`); A synthesizes a
> mouse-0 click at the cursor. **NOT yet wired:** on-screen keyboard (XShowKeyboardUI) for text fields
> (world name, chat) — flagged, not silently stubbed. Tunables live in `InputConfig` (`Input.h`).

Platform research doc. Reverse-engineers how the **OLD** Xbox 360 port
(`MinecraftAlpha/MinecraftAlpha/`, builds on the XDK but is buggy) handled
controller input and audio, and maps that onto the **NEW** transpiler's
LWJGL/paulscode stub surface (`ConversionTool/j2x/runtime.py`) so the new build
can be backed by a real Xbox 360 implementation.

All `file:line` references are into the OLD port unless noted.

---

## Overview

- **Controller input — real and complete in the old port.** The old port reads
  the gamepad directly via **XInput** (`XInputGetState(0, &state)`), polled
  *inline* at three call sites, not through any input abstraction layer. There
  is a full, working button/stick→action mapping to reuse.
- **The Java input model the new code targets is keyboard+mouse**
  (`Keyboard::isKeyDown`, `Mouse::getDX/getDY`, `Mouse::isButtonDown`,
  `Controllers::*`). The old port did **not** route through that model — it
  bypassed it and poked engine fields/methods directly. So the mapping has to be
  *re-derived* into the Keyboard/Mouse API, not copied.
- **Audio — real and complete in the old port.** There is a full **XACT3**
  backend (`XactAudio.cpp/.h`) with a dedicated audio thread, streaming wave
  banks, a music playlist, and shipped banks (`sounds.xwb` 4.6 MB, `music.xwb`
  16.6 MB). It is *not* stubbed/absent. The new code instead targets paulscode
  `SoundSystem` + `CodecJOrbis` (OGG), which the old `SoundManager.cpp` override
  already bridges onto XACT.
- **The new stubs are all no-ops today.** Every `Keyboard`/`Mouse`/`Controllers`
  method returns `false`/`0`, and `SoundSystem`/`CodecJOrbis` are empty stubs
  (`runtime.py` lines ~800-1015). So the new build currently has no input and no
  sound until these are backed.

---

## Controller input (old port)

### How the gamepad is read

The old port uses **XInput** (`XInputGetState(0, ...)` — controller port 0
only, player 1). No `XamUserGet*`, no DirectInput. It reads the raw
`XINPUT_STATE` / `XINPUT_GAMEPAD` struct each frame and interprets fields
inline. There is **no input wrapper class** — the same `XInputGetState` call is
copy-pasted at three sites, each owning its own edge-detection state:

| Site | File:line | Responsibility |
|------|-----------|----------------|
| In-game **action buttons** | `Minecraft.cpp:847-884` | RT/LT triggers, START, Y, LB/RB, B, BACK |
| In-game **move + look** | `EntityPlayerSP.cpp:132-152` | left stick = move, right stick = look, A = jump, L3 = sneak |
| **Menu navigation** | `GuiScreen.cpp:114-172` | d-pad / left stick = move selection, A = activate, B = back |

`MouseHelper.cpp` is a **dead delta holder** — see "Adapting" below.

### XInput specifics

- **Single call per consumer per frame**: `XINPUT_STATE st; if (XInputGetState(0, &st) == ERROR_SUCCESS) { ... }`.
  `XInputGetState(0, ...)` is hardcoded to user index 0; no multi-pad, no
  hot-plug handling beyond "call failed → skip this frame."
- **Stick deadzone**: a flat radial-per-axis deadzone. In-game uses
  `const short _DZ = 8000;` then normalizes `(float)sThumbLX / 32767.0F`
  (`EntityPlayerSP.cpp:135-139`). Menu nav uses a larger fixed threshold of
  `±16384` for a one-notch step (`GuiScreen.cpp:128-131`).
- **Trigger threshold**: `const BYTE TRIG = 40;` (out of 0-255). A trigger is
  "pulled" when `value > 40` (`Minecraft.cpp:855-856`).
- **Edge detection is manual** via `static` previous-state vars
  (`_prevBtns/_prevLT/_prevRT` in `Minecraft.cpp:848`; `s_prev/s_next/s_a/s_b`
  in `GuiScreen.cpp:126`). `pressed = (WORD)(b & ~_prevBtns)` is the rising-edge
  mask (`Minecraft.cpp:854`).
- **Cross-screen resync (bug fix):** while a screen is open, `Minecraft.cpp:878-883`
  re-reads XInput purely to overwrite `_prevBtns/_prevLT/_prevRT`, so a button
  *held* across a menu close is not re-fired as a fresh in-game press
  (documented as the "B-double-fire / hotbar-drop" bug).

---

## Control mapping

Derived from `Minecraft.cpp:847-884`, `EntityPlayerSP.cpp:132-152`, and
`GuiScreen.cpp:114-172`.

### In-game (no screen open)

| Gamepad input | XInput field/flag | Minecraft action | Source |
|---|---|---|---|
| **Left stick X** | `sThumbLX` (DZ 8000) | strafe (`MovementInput.m_field_1174_a`) | `EntityPlayerSP.cpp:143` |
| **Left stick Y** | `sThumbLY` (DZ 8000) | forward/back (`m_field_1173_b = -ly`) | `EntityPlayerSP.cpp:144` |
| **Right stick X** | `sThumbRX` (DZ 8000) | look yaw (`rotationYaw += rx*6`) | `EntityPlayerSP.cpp:147` |
| **Right stick Y** | `sThumbRY` (DZ 8000) | look pitch (`rotationPitch += ry*6`, clamp ±90) | `EntityPlayerSP.cpp:148-150` |
| **A** | `XINPUT_GAMEPAD_A` | jump (`m_field_1176_d`) | `EntityPlayerSP.cpp:145` |
| **L3 (left thumb click)** | `XINPUT_GAMEPAD_LEFT_THUMB` | sneak (`m_field_1175_e`) | `EntityPlayerSP.cpp:146` |
| **RT** (held + aiming block) | `bRightTrigger > 40` | progressive mining (`sendBlockRemoving` each tick) | `Minecraft.cpp:859-863` |
| **RT** (rising edge) | `bRightTrigger > 40` | attack / insta-break → `clickMouse(0)` | `Minecraft.cpp:864` |
| **LT** (rising edge) | `bLeftTrigger > 40` | place / use → `clickMouse(1)` | `Minecraft.cpp:865` |
| **START** | `XINPUT_GAMEPAD_START` | pause menu (`func_6252_g`) | `Minecraft.cpp:866` |
| **Y** | `XINPUT_GAMEPAD_Y` | open inventory (`GuiInventory`) | `Minecraft.cpp:867` |
| **LB** | `XINPUT_GAMEPAD_LEFT_SHOULDER` | hotbar prev (`currentItem = (i+8)%9`) | `Minecraft.cpp:868` |
| **RB** | `XINPUT_GAMEPAD_RIGHT_SHOULDER` | hotbar next (`currentItem = (i+1)%9`) | `Minecraft.cpp:869` |
| **B** | `XINPUT_GAMEPAD_B` | drop held item (`dropPlayerItem`) | `Minecraft.cpp:870-873` |
| **BACK** | `XINPUT_GAMEPAD_BACK` | cycle render distance (`(rd+1)&3`) | `Minecraft.cpp:874` |

Notes:
- Left-stick→move and A/L3 are written **directly into the `MovementInput`
  fields** *after* the (all-zero) keyboard pass `func_797_a`, so they win
  (`EntityPlayerSP.cpp:126-127`). The d-pad is intentionally **not** consumed
  in-game so it remains free for menus.
- Look is applied by **directly mutating** `rotationYaw`/`rotationPitch` (not
  via mouse deltas). `*6.0F` is the per-frame sensitivity; the `+=` and strafe
  sign were tuned against an X-mirror render fix (see Known Bugs).
- RT/LT map to the Java mouse buttons through `clickMouse(0)` = left/attack,
  `clickMouse(1)` = right/place (`Minecraft.cpp:649`). `m_field_6282_S` is the
  left-click attack cooldown.

### Menus (screen open)

| Gamepad input | Condition | Menu action | Source |
|---|---|---|---|
| **D-pad Up/Left** or **left stick** up/left | `wButtons` flag or `sThumbLY>16384` / `sThumbLX<-16384` | select previous enabled control | `GuiScreen.cpp:128-129, 144-150` |
| **D-pad Down/Right** or **left stick** down/right | flag or `sThumbLY<-16384` / `sThumbLX>16384` | select next enabled control | `GuiScreen.cpp:130-131, 151-157` |
| **A** | rising edge | activate selected button (`actionPerformed`, plays `random.click`) | `GuiScreen.cpp:158-165` |
| **B** | rising edge | back out (`keyTyped(0, 1)`, i.e. Esc code 1) | `GuiScreen.cpp:168-171` |

Selection lives in `m_padSelected` (a "virtual cursor" highlighted in
`drawScreen`). The skip-disabled/invisible loop is `GuiScreen.cpp:146-156`.

### Text entry

Chat/sign/texture-pack screens were ported with `keyTyped(char, code)` intact
and an ASCII allowed-char set, but **no on-screen keyboard is wired up**
(`GuiChat.cpp:9-12, 39-44, 56`). Text input is effectively reference-only in the
old port — there is no XInput→character path. The new build will need a real
Xbox OSK (`XShowKeyboardUI` / `XOverlapped`) feeding `keyTyped`.

---

## Adapting controller → the new code's Keyboard/Mouse API

This is the core of the new input stub. The new game code is **Java-faithful**:
it calls `Keyboard::isKeyDown(KEY_*)`, `Mouse::getDX()/getDY()`,
`Mouse::isButtonDown(0/1)`, and uses `Controllers::*`. The old port bypassed all
of that, so we cannot copy its plumbing — but its *mapping* (the table above) is
exactly what we re-implement *behind* these stubs.

### Strategy: one XInput poll per frame → fill a virtual K/M state

Add a single platform module (e.g. `Xbox360Input`) that calls
`XInputGetState(0, ...)` **once per frame**, then exposes that state through the
LWJGL stub API. Replace the no-op bodies in `runtime.py`-generated
`Keyboard.h` / `Mouse.h` / `Controllers.h` with calls into it. The Java code
reads these and runs the *original* Minecraft input logic
(`MovementInputFromOptions`, `EntityRenderer` mouse-look, `GuiScreen.handleInput`),
which is what we want for faithfulness.

#### `Mouse::getDX()` / `Mouse::getDY()` — back with the **right stick**

The Java look path (`EntityRenderer.func_4136_b`, present in the old port at
`EntityRenderer.cpp:393-404`) does:

```
mouseHelper.updateDelta();           // func_772_c()  -> fills m_field_1114_a/b
dx = mouseHelper.m_field_1114_a;      // getDX
dy = mouseHelper.m_field_1113_b;      // getDY
yaw   += dx * sensitivity_curve;
pitch += dy * sensitivity_curve * invert;
```

So back `Mouse::getDX()/getDY()` with the right-stick deltas:
- `getDX()` → scaled `sThumbRX` (deadzone 8000, normalize /32767, multiply to a
  comfortable per-frame magnitude — old direct path used `* 6.0F` on the
  normalized value at `EntityPlayerSP.cpp:147`).
- `getDY()` → scaled `sThumbRY`. **Watch the sign**: LWJGL `getDY` is
  screen-space (down-positive in Minecraft's pitch math via the `invertMouse`
  flag), whereas XInput `sThumbRY` is up-positive. Expect to negate one axis;
  the old direct path used `+=` on both and relied on `m_invertMouse`
  (`EntityRenderer.cpp:399-402`). Validate empirically — the old port has
  comments admitting sign tuning (`EntityPlayerSP.cpp:140-142`).
- Crucially, feed deltas *through* `MouseHelper` so the Java sensitivity curve
  (`sens*0.6+0.2`, cubed, `*8`) is applied once — do **not** double-apply.

#### `Mouse::isButtonDown(0/1)` — back with the **triggers**

- `isButtonDown(0)` (left / attack) → `bRightTrigger > TRIG`.
- `isButtonDown(1)` (right / place) → `bLeftTrigger > TRIG`.

The Java `clickMouse`/`isButtonDown` callers will then handle held-vs-edge
themselves. (The old port used RT=attack/LT=place via `clickMouse`; preserve
that orientation.) `getEventButton`/`getEventButtonState` need a small event
queue if any Java caller polls events instead of state — minimally, synthesize a
press/release event on trigger-edge.

#### `Keyboard::isKeyDown(KEY_*)` — back with **sticks + buttons mapped to key codes**

The Java movement reads keybinds (`MovementInputFromOptions::func_796_a` maps a
keyCode to the move array; see new-side equivalent). Back the relevant key codes:

| LWJGL key (new `Keyboard` const) | Default bind | Controller source |
|---|---|---|
| `KEY_W` (17) | forward | left stick up (`sThumbLY > +DZ`) |
| `KEY_S` (31) | back | left stick down (`sThumbLY < -DZ`) |
| `KEY_A` (30) | left | left stick left (`sThumbLX < -DZ`) |
| `KEY_D` (32) | right | left stick right (`sThumbLX > +DZ`) |
| `KEY_SPACE` (57) | jump | `XINPUT_GAMEPAD_A` |
| `KEY_LSHIFT` (42) | sneak | `XINPUT_GAMEPAD_LEFT_THUMB` (L3) |
| `KEY_ESCAPE` (1) | pause/back | `XINPUT_GAMEPAD_START` / `B` |
| `KEY_E` (18) | inventory | `XINPUT_GAMEPAD_Y` |
| `KEY_Q` (16) | drop | `XINPUT_GAMEPAD_B` |
| `KEY_1..KEY_9` (2..10) | hotbar slot | LB/RB step `currentItem` (or map directly) |

(The `KEY_*` codes above are the LWJGL DirectInput scancodes already defined in
`runtime.py:801-824`.) Digitizing the analog stick into W/A/S/D is the faithful
route for movement *if* you let the Java `MovementInput` consume it; the
alternative is to keep movement analog by also writing the `MovementInput`
fields (what the old port did at `EntityPlayerSP.cpp:143-146`) and only use
digital keys for non-movement binds. **Analog movement is preferable** for feel,
so the cleanest design is: analog stick → `MovementInput` floats directly (as
old port), digital buttons → `Keyboard::isKeyDown`, right stick →
`Mouse::getDX/getDY`, triggers → `Mouse::isButtonDown`.

#### `Controllers::*`

The new code includes `Controllers` (LWJGL JInput). The old port never used it.
Simplest: keep `Controllers::isCreated()` returning `false` so the Java code
falls back to its Keyboard/Mouse path (which we are now backing with XInput).
Only implement `Controllers` if the new Java code *requires* a controller to be
present; otherwise routing everything through Keyboard/Mouse is less surface
area.

#### Menu navigation

The new Java `GuiScreen` will poll `Keyboard`/`Mouse` events for menu nav (LWJGL
`Keyboard.next()` / arrow keys / `Mouse` clicks). Back this by synthesizing key
events from d-pad/stick edges (Up/Down → `KEY_UP`/`KEY_DOWN`, A → `KEY_RETURN`,
B → `KEY_ESCAPE`) **or** by porting the old port's `m_padSelected` virtual-cursor
approach. The old port's edge-detection + skip-disabled logic
(`GuiScreen.cpp:135-171`) is directly reusable as the reference algorithm.

---

## Audio (old port state)

**A full, real XACT3 audio backend exists** — not stubbed, not absent.

- **Engine**: `XactAudio.cpp` / `XactAudio.h`, built on `<xact3.h>`
  (`XactAudio.h:4`). Global instance `g_audio` (`XactAudio.cpp:31`).
- **Banks (shipped, present on disk)**:
  - SFX: `sounds.xwb` (4.6 MB) + `sounds.xsb` (6 KB) — streaming wave bank
    (`XactAudio.cpp:295-374, 694-695`).
  - Music: `music.xwb` (16.6 MB) + `music.xsb` (321 B) — streaming, with
    in-memory fallback (`XactAudio.cpp:379-469, 705-706`).
  - Loaded from `game:\Content\resources\` (`XactAudio.cpp:694, 705`).
- **Dedicated audio thread** pinned to **HW thread 5** via
  `XSetThreadProcessor` (`XactAudio.cpp:99-159`). Main thread enqueues cue names
  into a 64-slot lock-free-ish ring buffer (`audioQueueRequest`,
  `XactAudio.cpp:128-143`); the audio thread pumps `engine->DoWork()` ~60 Hz and
  calls `SoundBank->Play()` with a small retry queue
  (`XactAudio.cpp:99-126, 73-97`).
- **Music playlist**: shuffles `calm1/calm2/calm3`, auto-advances when a cue
  finishes, with a 60-frame startup delay guard (`XactAudio.cpp:36-47, 787-861`).
- **Cue naming**: short bank cue names (e.g. `step_grass`, `step_grass 2`,
  `dig_stone3`, `random_click`, `glass1`, `pig1`), **not** the dotted Java names.
- **Bridge to Java sound model**: `SoundManager.cpp` is a hand-written override
  that keeps the converter-generated `SoundManager.h` interface (so every Java
  caller is unchanged) and maps dotted Java names → bank cues via `toCue()`
  (`SoundManager.cpp:62-166`), then calls the free functions `XaPlayCue` /
  `XaSetSoundEnabled` / `XaSetMusicEnabled` (`XactAudio.cpp:1012-1023`). Volume
  is treated as a binary gate (`vol > 0` = on); **XACT has no per-cue volume API
  used here** (`SoundManager.cpp:18-19, 168-170`). 3D positional audio is
  **dropped** — everything is fire-and-forget 2D (`SoundManager.cpp:259-267`,
  `XactAudio.cpp:764-771`).

### Caveats in the old audio

- `playSFXWithPitch` is **disabled** — it `return`s after a plain `playSFX`; the
  whole pitch/`DopplerPitchScalar` path is commented out
  (`XactAudio.cpp:556-608`). So pitch variation never worked.
- Music can be force-disabled at runtime if its banks fail to load
  (`g_musicDisabled`, `XactAudio.cpp:707-710`).
- Cues for many Alpha-new mobs (`chicken`, `cow`, `creeper`, `zombie`, etc.) are
  *mapped* in `toCue` but only sound if authored into the bank; otherwise
  `GetCueIndex` fails and the play silently no-ops
  (`SoundManager.cpp:138-162`).

---

## Audio plan for the new build

The new game code targets **paulscode `SoundSystem`** + **`CodecJOrbis`** (OGG),
which on Xbox 360 do not exist. Two viable routes:

**Route A (recommended — reuse the proven XACT backend).** Re-do exactly what
the old port did: write a hand override of the converted `SoundManager` that
keeps the new generated `SoundManager.h` interface but bridges onto `XactAudio`
(XACT3) + the shipped `sounds.xwb`/`music.xwb` banks. Leave the
`SoundSystem`/`CodecJOrbis` stubs in `runtime.py` (1007-1015) as no-ops — the
override means the Java `SoundSystem` calls are never reached. This is the lowest
risk because the XACT path is shipped and working, and `SoundManager.cpp` /
`XactAudio.cpp` are directly portable. Carry over: the `toCue` name mapping, the
audio-thread-on-HW-5 design, and the music playlist.

**Route B (faithful — implement paulscode `SoundSystem` for real).** Back the
`SoundSystem` stub with an Xbox audio engine (XAudio2 or XACT) and back
`CodecJOrbis::read/readAll` with a real OGG Vorbis decoder (e.g. stb_vorbis /
libvorbis compiled for 360), so the Java sound model runs unmodified including
3D positional sources, per-source pitch/volume, and streaming music from `.ogg`.
This is more work (codec + per-source mixer + 3D listener) and only worth it if
faithful per-cue pitch/3D attenuation matters — both of which the old XACT path
*dropped*.

**Recommendation: Route A.** It restores audio immediately using assets that
already ship, and matches what the old port proved works. Defer Route B unless
3D audio / per-cue pitch is a hard requirement. The new `SoundSystem` /
`CodecJOrbis` stubs can stay no-op under Route A.

### Stub surface to satisfy (new side, `runtime.py`)

- `SoundSystem` (`runtime.py:1012-1015`): `cleanup, newSource,
  newStreamingSource, play, setListenerOrientation, setListenerPosition,
  setPitch, setVolume, stop, pause, removeSource`, `playing()->bool`. Under
  Route A these stay no-op (overridden SoundManager never calls them).
- `CodecJOrbis` (`runtime.py:1007-1011`): `initialize, cleanup, setAudioFormat,
  reverseByteOrder`, `read/readAll/getAudioFormat`, `initialized/endOfStream`.
  Only needed for Route B.

---

## Known bugs / pitfalls

From the old port (some already fixed there, but instructive for the new build):

1. **Dead `MouseHelper` delta path.** `MouseHelper::func_772_c()` is a no-op
   (`MouseHelper.cpp:57-61`) and *nothing ever writes* `m_field_1114_a/b` from
   XInput, so `EntityRenderer.cpp:393-404`'s mouse-look computes
   `var4=var5=0` — the entire LWJGL-style look path is **dead** in the old port.
   Look only works because `EntityPlayerSP.cpp:147-148` separately mutates
   `rotationYaw/Pitch`. **For the new build, do the opposite: actually drive
   `Mouse::getDX/getDY` from the right stick so the faithful Java look path is
   the live one** (don't leave two competing look paths).
2. **Uninitialized key-state array → auto-jump.** `new bool[10]` is *not*
   zero-initialized in C++; garbage at index 4 (jump) made the player jump
   nonstop. Fixed with `new bool[10]()` (`MovementInputFromOptions.cpp:11-12,
   17`). Mirror this in the new build.
3. **OOB write from `unsigned char var3 = -1`.** `(unsigned char)-1 == 255`, and
   `var3 >= 0` is always true, so a non-matching key wrote `m_field_1179_f[255]`
   past the `bool[10]`. Fixed to `int var3 = -1` + `var3 < 10` bound
   (`MovementInputFromOptions.cpp:30-31, 56`).
4. **Cross-screen button bleed.** A button held while closing a menu re-fired as
   a fresh in-game press (B-double-fire / accidental item drop). Fixed by
   resyncing `_prevBtns/_prevLT/_prevRT` while a screen is open
   (`Minecraft.cpp:878-883`). Any edge-detected scheme needs this.
5. **Look sign / axis inversion is fiddly.** The old port has explicit comments
   that yaw/strafe signs were tuned against an X-mirror render fix and against
   *observed* alpha behavior, and may need flipping (`EntityPlayerSP.cpp:128-130,
   140-144`). Budget for empirical sign tuning of `getDX/getDY`.
6. **Pitch audio never worked** (`playSFXWithPitch` short-circuits,
   `XactAudio.cpp:556-561`). If pitch matters, Route B or fix the XACT RPC path.
7. **No on-screen keyboard.** Text screens accept `keyTyped` but nothing feeds
   characters (`GuiChat.cpp:39-44`). Chat/signs are effectively unusable until an
   Xbox OSK is wired.
8. **Single controller only** (`XInputGetState(0, ...)` everywhere). No hot-plug,
   no player 2.

---

## Open questions

1. **Is there a usable control mapping to reuse?** **Yes — fully.** The complete
   button/stick→action mapping is the table above (`Minecraft.cpp:847-884`,
   `EntityPlayerSP.cpp:132-152`, `GuiScreen.cpp:114-172`). It is directly
   portable as the *behavior spec* for the new XInput→Keyboard/Mouse adapter.
   The only open design choice is **analog movement (write `MovementInput`
   floats, as old port) vs. digitized W/A/S/D (`Keyboard::isKeyDown`)** — analog
   recommended for feel.
2. **Does any audio exist?** **Yes — a complete XACT3 backend plus shipped banks**
   (`sounds.xwb` 4.6 MB, `music.xwb` 16.6 MB). Route A (reuse XACT via a
   `SoundManager` override) restores sound immediately with no new codec work.
3. **Route A vs B for audio?** Need a decision: reuse XACT (fast, 2D, no pitch)
   or implement real paulscode `SoundSystem` + OGG (faithful 3D/pitch, much more
   work). Recommendation is A.
4. **Look path ownership.** New build should pick ONE look path — the faithful
   `Mouse::getDX/getDY` → `EntityRenderer`/`MouseHelper` path — and back it from
   the right stick, avoiding the old port's two-competing-paths trap. Confirm the
   new generated `EntityRenderer`/`MouseHelper` actually call the delta path.
5. **Sensitivity / deadzone tuning.** Old constants: stick DZ `8000` (in-game),
   menu step `16384`, trigger `40`, look gain `*6.0F`. These are starting points;
   the Java sensitivity curve (`sens*0.6+0.2` cubed `*8`) must not be
   double-applied on top of the stick gain.
6. **Sign/inversion** of right-stick → `getDY` vs `m_invertMouse`. Needs on-device
   testing per the old port's own warnings.
7. **Text input**: does the new build need a working Xbox OSK
   (`XShowKeyboardUI`) for chat/signs, or is text entry out of scope?
8. **Missing Alpha cues**: several mob/effect cues are mapped but may not be
   authored into the shipped banks (`SoundManager.cpp:138-162`). Confirm bank
   contents if those sounds are required.
