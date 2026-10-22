# Step 4 — Input (XInput → LWJGL Keyboard/Mouse): deliverables & integration

Reference implementation written (Xbox-side; not built by the CodeBreaker repo — it needs the XDK's XInput +
the platform layer, so it's verified by building the Xbox project, not the converter):

- `Input.reference.h` / `Input.reference.cpp` — the LWJGL→XInput adapter. ONE `XInputGetState(0,…)`
  poll per render frame (`Input::poll()`) rebuilds a virtual keyboard/mouse model that the faithful
  Java input code reads through `Keyboard::` / `Mouse::`. REPLACES the converter's no-op `Keyboard.h`
  and `Mouse.h` stubs (and `Controllers.h`). Grounded in the OLD port's proven inline XInput mapping
  (`Minecraft.cpp:847-884`, `EntityPlayerSP.cpp:132-152`, `GuiScreen.cpp:114-172`) and doc 03.

The new game code is Java-faithful and already calls exactly these (verified in the generated output
`…/Temp/j2xv7/`):
- `Mouse::getDX()/getDY()` via `MouseHelper::updateMouseDelta()` → `EntityRenderer` look (LIVE path).
- `Mouse::isButtonDown(0|1)`, `Mouse::next()/getEventButton()/getEventButtonState()/getEventX/Y`,
  `Mouse::getEventDWheel()` (hotbar scroll), `Mouse::getX()/getY()` (GUI cursor),
  `Mouse::setGrabbed()/setCursorPosition()` (MouseHelper — these two are MISSING from the runtime stub;
  see "Converter gap" below).
- `Keyboard::isKeyDown(KEY_*)`, `Keyboard::next()/getEventKey()/getEventKeyState()/getEventCharacter()`,
  `Keyboard::getKeyName()`, `Keyboard::enableRepeatEvents()`, `Keyboard::create()/destroy()`.
- `Controllers::create()` only (kept inert — `isCreated()` returns false so the game stays on K/M).

## Integration steps (in the Xbox VS project)

1. Drop `Input.h` / `Input.cpp` in, REPLACING the converter's no-op `Keyboard.h`, `Mouse.h`,
   `Controllers.h` stub headers for those names. `Input.h` re-declares the `Keyboard`/`Mouse`/
   `Controllers` namespaces with the SAME `KEY_*` constant values (LWJGL DirectInput scancodes,
   identical to `runtime.py:801-824`) so every `== KEY_*` / `keyBind->keyCode` comparison still matches.
2. **Call `Input::poll()` once per frame, at the TOP of the game-loop body.** In the generated
   `Minecraft::run()` the loop is the `while(this->running …)` at `Minecraft.cpp:507`. Add the poll as
   the first statement inside that loop, BEFORE the `for(... elapsedTicks) runTick()` block:

   ```cpp
   while (this->running && (this->mcApplet == NULL || this->mcApplet->isActive())) {
       Input::poll();                       // <-- ADD: single XInput read; fills virtual K/M state
       AxisAlignedBB::clearBoundingBoxPool();
       …
       for (int var6 = 0; var6 < this->timer->elapsedTicks; ++var6) { this->runTick(); }
       …  // render (EntityRenderer reads Mouse::getDX/getDY here, at render rate)
   }
   ```

   Why once per FRAME (not per tick): the look path (`MouseHelper::updateMouseDelta` → `getDX/getDY`)
   runs at render rate in `EntityRenderer`, and the event queues (`Mouse::next`, `Keyboard::next`) are
   drained in `runTick()` (0..N times per frame). Polling once per frame and letting the queues
   accumulate is correct for both. `getDX/getDY` are read-once (drain on read), matching LWJGL.
3. Tell the adapter the back-buffer size once (so `Mouse::getX()/getY()` cursor scaling is right):
   `Input::setDisplaySize(displayWidth, displayHeight);` after the D3D device/resolution is set.
4. **Grab state.** `MouseHelper::grabMouseCursor()/ungrabMouseCursor()` already call
   `Mouse::setGrabbed(true/false)`; the adapter uses that to route the right stick into look deltas
   (grabbed = in-game) vs. nudging the GUI cursor (ungrabbed = menu). No extra wiring needed.
5. **Analog movement (recommended).** The cleanest feel is analog left-stick movement, as the old port
   did. Add a tiny override of the converted `MovementInput` (the class the new code calls each tick)
   so that, when `Input::connected()`, it writes `moveStrafe = Input::moveStrafe()` and
   `moveForward = Input::moveForward()` (sign per the table) AFTER the keyboard pass, exactly like
   `EntityPlayerSP.cpp:143-144`. If you skip this, the adapter ALSO digitizes the left stick into
   `KEY_W/A/S/D`, so movement still works through the pure-keyboard Java path — just on/off, not analog.
6. Build the Xbox project (XDK). XInput comes in via `xtl.h`; no extra lib beyond the XDK default.

## Converter gap to close centrally (CodeBreaker owner)

`MouseHelper.cpp` (generated) calls `Mouse::setGrabbed(bool)` and `Mouse::setCursorPosition(int,int)`,
which are **absent from the `runtime.py` Mouse stub** (`Mouse.h` only has create/destroy/setNativeCursor
+ the getters). The generator papered over this by reopening `namespace Mouse { inline void setGrabbed…}`
inside `MouseHelper.cpp`. Two options:
- **Add them to the runtime stub** (`runtime.py` Mouse.h `void_fns`: add `setGrabbed`, `setCursorPosition`)
  so the local shim in `MouseHelper.cpp` is no longer emitted. `Input.h` already declares the real ones.
- Or leave the shim — but then the real `Input.cpp` `setGrabbed/setCursorPosition` will COLLIDE with the
  inline shim in `MouseHelper.cpp` at link/compile. **Recommend: add the two methods to the stub and
  drop the shim.** (This is a converter fix, not an Xbox-side one.)

## Control map (final — matches doc 03, derived from the old-port XInput sites)

### In-game (no screen open)

| Gamepad input | XInput field/flag | LWJGL surface the adapter drives | Minecraft action |
|---|---|---|---|
| Left stick (analog) | `sThumbLX/LY` (DZ 8000) | `Input::moveStrafe()/moveForward()` (+ digital `KEY_W/A/S/D`) | move |
| Right stick | `sThumbRX/RY` (DZ 8000) | `Mouse::getDX()/getDY()` | look (live, via MouseHelper→EntityRenderer) |
| RT | `bRightTrigger > 40` | `Mouse::isButtonDown(0)` + button-0 events | attack / break |
| LT | `bLeftTrigger > 40` | `Mouse::isButtonDown(1)` + button-1 events | place / use |
| A | `GAMEPAD_A` | `Keyboard::KEY_SPACE` | jump |
| L3 (left stick click) | `GAMEPAD_LEFT_THUMB` | `Keyboard::KEY_LSHIFT` | sneak |
| Y | `GAMEPAD_Y` | `Keyboard::KEY_E` | inventory |
| B | `GAMEPAD_B` | `Keyboard::KEY_Q` | drop held item |
| START | `GAMEPAD_START` | `Keyboard::KEY_ESCAPE` | pause / open menu |
| LB | `GAMEPAD_LEFT_SHOULDER` | `Mouse::getEventDWheel()` = −120 | hotbar prev |
| RB | `GAMEPAD_RIGHT_SHOULDER` | `Mouse::getEventDWheel()` = +120 | hotbar next |
| BACK | `GAMEPAD_BACK` | (no Alpha keybind) | render-distance cycle — see note |

> **BACK / render distance:** Alpha has no keybind for render distance (the old port poked the setting
> directly at `Minecraft.cpp:874`). There is no clean `KEY_*` to synthesize, so the adapter leaves BACK
> unmapped. Cleanest fix on the Xbox side: in the loop, after `Input::poll()`, read a small
> `Input::backPressedEdge()` helper (add if wanted) and do
> `gameSettings->renderDistance = (renderDistance+1)&3;` — i.e. keep this one action as a direct poke,
> exactly as the old port did, rather than forcing it through the key model.

### Menus (screen open)

| Gamepad input | Threshold | LWJGL surface | Menu action |
|---|---|---|---|
| D-pad/left-stick Up/Left | flag or stick > ±16384 | `KEY_UP` / `KEY_LEFT` key events | move selection prev |
| D-pad/left-stick Down/Right | flag or stick > ±16384 | `KEY_DOWN` / `KEY_RIGHT` key events | move selection next |
| A | rising edge | `KEY_RETURN` key event | activate |
| B | rising edge | `KEY_ESCAPE` key event | back out |
| X | rising edge, **container GUI only** | mouse **button 1** (right-click) event | split stack / place one item (`gui-right-click`) |
| Right stick | — | nudges `Mouse::getX()/getY()` cursor | pointer (for click-driven screens) |

Edge detection + the menu-step threshold (16384) reuse the old port's `GuiScreen.cpp:126-139` logic.
The **cross-screen resync bug fix** (a button held across a menu close re-firing as a fresh press —
`Minecraft.cpp:878-883`) is partly structural (edges vs the previous frame in the single shared `poll()`),
but that alone does NOT cover the case where a held button's IN-GAME meaning goes through `setKey()` (e.g.
B = ESCAPE in a menu but B = Q/drop in-game): those mapped keys are never driven during the GUI period, so
`g_prevKeyDown` is false for them and the first in-game frame sees a false rising edge. The DELIVERED fix
(`Input.reference.cpp` `Mouse::setGrabbed`, `cross-screen-resync`) therefore also samples the pad on the
grab and pre-seeds `g_prevKeyDown` with the in-game binding of held face buttons (A→SPACE, B→Q, Y→E,
R3→LSHIFT). This is what the "A/B double-purpose" HARDWARE-VERIFY note below anticipated.

## HARDWARE-VERIFY (can't be checked off-device — iterate on the 360)

- **Right-stick sensitivity.** `InputConfig::LOOK_PIXELS_PER_SECOND` (default 750 ≈ 112°/s at full stick;
  deg/s = value/2400*360) is the per-SECOND
  "mouse pixels" at full deflection (poll scales by real frame dt, so it is frame-rate independent —
  see [J2X-BUGFIX fps-look-scale]), BEFORE the Java curve `(sens*0.6+0.2)^3 * 8`
  (`EntityRenderer.cpp:411-414`). It is deliberately NOT pre-gained (the old port's `*6.0F` was on the
  rotation directly; baking it here would double-apply through the curve). Tune for comfortable turn
  speed; also expose the in-game mouse-sensitivity slider as usual.
- **Y-invert sign.** `InputConfig::LOOK_INVERT_Y` (default true) is the BASE pitch sign; the game then
  applies its own `invertMouse` option on top (`EntityRenderer.cpp:416-419`). If, with `invertMouse`
  OFF, pushing the stick up looks DOWN, flip `LOOK_INVERT_Y`. `LOOK_INVERT_X` similarly for yaw.
- **Deadzones.** `STICK_DEADZONE 8000`, `MENU_STEP 16384`, `TRIGGER 40` are the old port's values and a
  good start; widen the deadzone if there's stick drift, raise MENU_STEP if menu nav over-scrolls.
- **Analog vs digital movement.** Default ships BOTH (analog axes + digital W/A/S/D). If you do NOT add
  the `MovementInput` analog override (step 5), movement is digital on/off; add the override for analog
  feel. Don't run both into `MovementInput` at once.
- **A/B double-purpose.** A drives both `KEY_SPACE` (jump) and, on edge, `KEY_RETURN` (menu activate);
  B drives `KEY_Q` (drop) and `KEY_ESCAPE` (menu back). The Java side gates on `currentScreen`, so the
  unused half is ignored — but if you observe a double-action (e.g. closing a menu also drops an item),
  gate the in-game `KEY_SPACE/KEY_Q` lines on a "no screen open" flag pushed in from the game.
- **OSK for chat / sign text — DELIVERED for signs.** ~~likely out of scope~~ The `OSK` reference module
  (`XShowKeyboardUI` async, poll `XHasOverlappedIoCompleted`) is wired: `GuiEditSign` (overlay
  `console-sign-osk`) does d-pad line select + pops the system keyboard, with the `osk-cancel-wipes-line`
  guard so cancelling doesn't erase the pre-filled line. `keyToChar()` remains the desktop-dev fallback.
  Chat/world-name can reuse the same `OSK` module.
- **Single controller only.** `XInputGetState(0, …)`, as the whole old port did. No hot-plug / player 2.

## Status

Adapter delivered as reference: one XInput poll per frame → faithful LWJGL Keyboard/Mouse model, with
the right stick driving the LIVE look path (the thing the old port left dead), triggers → mouse buttons,
buttons/d-pad → mapped keys + menu nav, and analog left stick available for a `MovementInput` override.
Remaining: drop in + add the `poll()` call (step 2), close the `setGrabbed/setCursorPosition` converter
gap, optionally add the `MovementInput` analog override, and tune the HARDWARE-VERIFY constants on the
360. Text entry (OSK) is the one open feature decision.
