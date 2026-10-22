#pragma once

#include <cstdint>
#include <functional>

#include <windows.h>

// Host input behind XAM's controller functions (kernel/xam_input.cpp).
//
// Each guest user index 0-3 has one virtual controller:
// - CB_INPUT_SCRIPT set: the controllers are driven only by the script (reproducible runs, also headless). User 0
//   always has a scripted controller; users 1-3 have one (connected from the start) if any entry names them.
//   Format: entries separated by commas or new lines, "@<file>" reads them from a file ('#' comments):
//     [<user>@]<frame>:<buttons>[:<frames>]  hold <buttons> on user <user>'s controller (default 0) from presented
//                                            frame <frame> for <frames> frames (default 4)
//     [<user>@]+<n>:<buttons>[:<frames>]     the same, <n> frames after the previous entry's start (any user's)
//     <frame>:EXIT                           end the process (log flushed)
//     <frame>:TYPE=<text>                    type into the open system UI text box (the Guide's keyboard; '_' is a
//                                            space), as the PC keyboard does (SetTextInput)
//     [<user>@]<frame>:SHOW=<screen>[/<hex>] open a system screen as a title's XamShow*UI would, for user <user>
//                                            (kernel::ShowSystemScreenForTest: games, gamercard, ...; <hex> a XUID)
//   <buttons> is NONE or names joined by '+': A B X Y START BACK LB RB LT RT UP DOWN LEFT RIGHT (d-pad) LS RS
//   (stick clicks) LSUP LSDOWN LSLEFT LSRIGHT RSUP RSDOWN RSLEFT RSRIGHT (full stick deflection) GUIDE.
// - Otherwise user N is host XInput controller N, and while the game window exists the keyboard and mouse are one
//   more controller: the first user index without a host controller (user 0 with no pads, user 1 with one pad, so a
//   pad and the keyboard can play split-screen; see kKeyboardMapping in input.cpp). It reads as idle while the window
//   does not have focus.
//
// Keystrokes (XamInputGetKeystrokeEx) are generated from each virtual controller's state like XInput does: button
// releases before presses, thumbsticks and triggers as VK_PAD_* directions past a threshold, repeats while held.
namespace input
{
    struct PadState
    {
        uint16_t buttons = 0;
        uint8_t leftTrigger = 0, rightTrigger = 0;
        int16_t thumbLX = 0, thumbLY = 0, thumbRX = 0, thumbRY = 0;
        bool guide = false; // the Guide button: the system's own, never seen by the title
        bool operator==(const PadState&) const = default;
    };

    struct Keystroke
    {
        uint16_t virtualKey;
        uint16_t unicode;
        uint16_t flags;
        uint8_t userIndex;
        uint8_t hidCode;
    };

    void Initialize();

    // False if no controller is connected for this user. packetNumber changes whenever the state changes.
    bool GetState(uint32_t user, PadState& state, uint32_t& packetNumber);
    // ERROR_SUCCESS with an event, ERROR_EMPTY with none, ERROR_DEVICE_NOT_CONNECTED without a controller.
    uint32_t GetKeystroke(uint32_t user, Keystroke& keystroke);

    // Command-processor thread, after each presented frame: advances the input script and feeds an open system UI.
    void OnFrame(uint64_t frame);

    // System UI (the Guide's dialogs, kernel/xam_ui.cpp). While one is open the title's controllers read as idle and
    // produce no keystrokes; each frame, buttons newly pressed on `user`'s controller (any user for XUSER_INDEX_ANY;
    // stick directions count as the d-pad, the triggers as kUiLeftTrigger / kUiRightTrigger) go to `onPress`, which
    // returns true when the dialog has closed. Buttons still held when a dialog closes stay hidden from the title until
    // released (the dialog consumed them).
    constexpr uint16_t kUiLeftTrigger = 0x0400, kUiRightTrigger = 0x0800; // bits XINPUT_GAMEPAD leaves unused
    void BeginSystemUi(uint32_t user, std::function<bool(uint32_t user, uint16_t pressed)> onPress);
    // The user index whose controller last pressed a button (0 until one does). The sign-in UI's native dialog signs a
    // profile in to it, since a mouse click has no controller.
    uint32_t LastActiveUser();
    // Closes the system UI from outside onPress (e.g. the windowed host dialog was answered).
    void EndSystemUi();
    // While held, the title's controllers read as idle even with no system UI taking input: XAM keeps its system
    // app input until its UI has gone (the closing animation). Releasing hides the buttons still held, like
    // EndSystemUi.
    void HoldTitleInput(bool hold);
    // The Guide button (XInputGetStateEx's 0x0400; the script's GUIDE; the Home key on the keyboard controller):
    // each press calls `handler` with the user index of its controller, on the command-processor thread after the
    // frame's input was handled (outside the input lock, so it may open or close a system UI).
    void SetGuideHandler(std::function<void(uint32_t user)> handler);
    // A system UI text box that takes PC typing: while `active` says so (called under the input lock), the
    // keyboard's characters and Backspace go to `type` instead of acting as controller buttons; arrows, Esc (B),
    // Enter (START: Done), Tab, Page Up / Down and Home keep working as buttons.
    void SetTextInput(std::function<bool()> active, std::function<void(char16_t)> type);

    // Game window thread: keyboard/mouse messages and focus changes. Returns true if the message was consumed.
    bool HandleWindowMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    void SetWindow(HWND hwnd);
}
