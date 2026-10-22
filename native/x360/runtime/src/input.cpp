// Host input: virtual controllers for the four guest users (input.h).

#include "kernel/kernel.h"
#include "kernel/stfs_mount.h"
#include "input.h"

#include "live/client.h"
#include "log.h"
#include "pads/pads.h"

#include <Xinput.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace input
{
    namespace
    {
        constexpr uint32_t kUsers = 4;
        constexpr uint32_t kErrorSuccess = 0;
        constexpr uint32_t kErrorDeviceNotConnected = 0x48F;
        constexpr uint32_t kErrorEmpty = 0x10D2;

        // XINPUT_GAMEPAD button bits (xinputdefs.h).
        constexpr uint16_t kDpadUp = 0x0001, kDpadDown = 0x0002, kDpadLeft = 0x0004, kDpadRight = 0x0008;
        constexpr uint16_t kStart = 0x0010, kBack = 0x0020, kLeftThumb = 0x0040, kRightThumb = 0x0080;
        constexpr uint16_t kLeftShoulder = 0x0100, kRightShoulder = 0x0200;
        constexpr uint16_t kA = 0x1000, kB = 0x2000, kX = 0x4000, kY = 0x8000;
        constexpr uint16_t kGuide = 0x0400; // XInputGetStateEx only
        constexpr int16_t kFull = 32767;

        // XInputGetStateEx (xinput1_4.dll ordinal 100): XInputGetState with the Guide button (0x0400) reported.
        using GetStateEx = DWORD(WINAPI*)(DWORD, XINPUT_STATE*);
        GetStateEx LoadGetStateEx()
        {
            HMODULE dll = LoadLibraryW(L"xinput1_4.dll");
            auto f = dll ? reinterpret_cast<GetStateEx>(GetProcAddress(dll, MAKEINTRESOURCEA(100))) : nullptr;
            if (!f)
                Log("Input: XInputGetStateEx not found; the controller's Guide button is not read (Home key: Guide)");
            return f;
        }

        // ---- Keystrokes ---------------------------------------------------------------------------------------------
        // Key bits: 0-15 the XINPUT_GAMEPAD buttons, 16/17 the triggers, 18-25 left-stick directions, 26-33 right-stick
        // directions (up, down, right, left, up-left, up-right, down-right, down-left), with their VK_PAD_* codes
        // (xam.h). The guide button and bit 11 have none.
        constexpr std::array<uint16_t, 34> kVirtualKeys = {
            0x5810, 0x5811, 0x5812, 0x5813, 0x5814, 0x5815, 0x5816, 0x5817, // d-pad, START, BACK, stick clicks
            0x5805, 0x5804, 0, 0, 0x5800, 0x5801, 0x5802, 0x5803,           // LSHOULDER, RSHOULDER, -, -, A, B, X, Y
            0x5806, 0x5807,                                                 // LTRIGGER, RTRIGGER
            0x5820, 0x5821, 0x5822, 0x5823, 0x5824, 0x5825, 0x5826, 0x5827, // LTHUMB_*
            0x5830, 0x5831, 0x5832, 0x5833, 0x5834, 0x5835, 0x5836, 0x5837, // RTHUMB_*
        };
        constexpr uint16_t kKeystrokeKeyDown = 0x0001, kKeystrokeKeyUp = 0x0002, kKeystrokeRepeat = 0x0004;
        // XInput's keystroke model (not documented for the console; the values Windows XInput and Xenia use): a
        // trigger is pressed beyond XINPUT_GAMEPAD_TRIGGER_THRESHOLD, a stick direction beyond 0x4E00 on its axis,
        // two neighbouring directions together are the diagonal instead; the last key pressed repeats after 400 ms,
        // then every 100 ms.
        constexpr uint8_t kTriggerThreshold = 30;
        constexpr int32_t kThumbThreshold = 0x4E00;
        constexpr int64_t kRepeatDelayMs = 400, kRepeatRateMs = 100;

        uint64_t KeyBits(const PadState& s)
        {
            uint64_t bits = s.buttons;
            bits |= uint64_t(s.leftTrigger > kTriggerThreshold) << 16;
            bits |= uint64_t(s.rightTrigger > kTriggerThreshold) << 17;
            auto stick = [&](int32_t x, int32_t y, uint32_t first) {
                bool up = y > kThumbThreshold, down = y < -kThumbThreshold;
                bool right = x > kThumbThreshold, left = x < -kThumbThreshold;
                uint32_t diagonal = 0;
                if (up && left)
                    diagonal = 4;
                else if (up && right)
                    diagonal = 5;
                else if (down && right)
                    diagonal = 6;
                else if (down && left)
                    diagonal = 7;
                if (diagonal)
                    bits |= uint64_t(1) << (first + diagonal);
                else
                    bits |= uint64_t(up) << first | uint64_t(down) << (first + 1) | uint64_t(right) << (first + 2) |
                        uint64_t(left) << (first + 3);
            };
            stick(s.thumbLX, s.thumbLY, 18);
            stick(s.thumbRX, s.thumbRY, 26);
            return bits;
        }

        enum class Repeat { kIdle, kWaiting, kRepeating };
        struct KeystrokeState
        {
            uint64_t down = 0;
            Repeat repeat = Repeat::kIdle;
            uint32_t repeatKey = 0;
            int64_t repeatTime = 0;
        };

        int64_t NowMs()
        {
            using namespace std::chrono;
            return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
        }

        // ---- Script -------------------------------------------------------------------------------------------------
        struct ScriptEntry
        {
            uint32_t user = 0;
            uint64_t start = 0;
            uint64_t frames = 4;
            PadState pad;
            bool exit = false;
            std::string text;
            std::u16string typed; // TYPE=<text>: typed into a system UI's text box at `start` ('_' = space)
            std::string show;     // SHOW=<screen>[/<hex>]: kernel::ShowSystemScreenForTest at `start`
        };

        bool ParseButtons(std::string_view names, PadState& pad)
        {
            struct Name { const char* name; uint16_t button; int16_t lx, ly, rx, ry; uint8_t lt, rt; };
            static const Name kNames[] = {
                { "A", kA }, { "B", kB }, { "X", kX }, { "Y", kY }, { "START", kStart }, { "BACK", kBack },
                { "LB", kLeftShoulder }, { "RB", kRightShoulder }, { "UP", kDpadUp }, { "DOWN", kDpadDown },
                { "LEFT", kDpadLeft }, { "RIGHT", kDpadRight }, { "LS", kLeftThumb }, { "RS", kRightThumb },
                { "LT", 0, 0, 0, 0, 0, 255, 0 }, { "RT", 0, 0, 0, 0, 0, 0, 255 },
                { "LSUP", 0, 0, kFull }, { "LSDOWN", 0, 0, -kFull }, { "LSLEFT", 0, -kFull, 0 }, { "LSRIGHT", 0, kFull, 0 },
                { "RSUP", 0, 0, 0, 0, kFull }, { "RSDOWN", 0, 0, 0, 0, -kFull }, { "RSLEFT", 0, 0, 0, -kFull, 0 },
                { "RSRIGHT", 0, 0, 0, kFull, 0 }, { "NONE", 0 }, { "GUIDE", kGuide },
            };
            while (!names.empty())
            {
                const size_t plus = names.find('+');
                std::string name(names.substr(0, plus));
                std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return char(std::toupper(c)); });
                const Name* found = nullptr;
                for (const Name& n : kNames)
                    if (name == n.name)
                        found = &n;
                if (!found)
                    return false;
                pad.buttons |= found->button & ~kGuide;
                pad.guide |= (found->button & kGuide) != 0;
                pad.thumbLX = found->lx ? found->lx : pad.thumbLX;
                pad.thumbLY = found->ly ? found->ly : pad.thumbLY;
                pad.thumbRX = found->rx ? found->rx : pad.thumbRX;
                pad.thumbRY = found->ry ? found->ry : pad.thumbRY;
                pad.leftTrigger = std::max(pad.leftTrigger, found->lt);
                pad.rightTrigger = std::max(pad.rightTrigger, found->rt);
                names = plus == std::string_view::npos ? std::string_view() : names.substr(plus + 1);
            }
            return true;
        }

        std::vector<ScriptEntry> ParseScript(std::string text)
        {
            if (!text.empty() && text[0] == '@')
            {
                std::ifstream file(text.substr(1));
                if (!file)
                    Fatal("CB_INPUT_SCRIPT: cannot read {}", text.substr(1));
                std::stringstream contents;
                contents << file.rdbuf();
                text = contents.str();
            }
            std::vector<ScriptEntry> entries;
            uint64_t previous = 0;
            std::string item;
            auto parseItem = [&](std::string s) {
                s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char c) { return std::isspace(c); }), s.end());
                if (s.empty())
                    return;
                ScriptEntry e;
                e.text = s;
                if (s.size() > 2 && s[1] == '@')
                {
                    if (s[0] < '0' || s[0] > '3')
                        Fatal("CB_INPUT_SCRIPT: user index in \"{}\" must be 0-3", e.text);
                    e.user = uint32_t(s[0] - '0');
                    s.erase(0, 2);
                }
                const size_t c1 = s.find(':');
                if (c1 == std::string::npos)
                    Fatal("CB_INPUT_SCRIPT: entry \"{}\" has no ':'", s);
                const bool relative = s[0] == '+';
                char* end = nullptr;
                const uint64_t frame = std::strtoull(s.c_str() + (relative ? 1 : 0), &end, 10);
                if (end != s.c_str() + c1)
                    Fatal("CB_INPUT_SCRIPT: bad frame number in \"{}\"", s);
                e.start = relative ? previous + frame : frame;
                previous = e.start;
                const size_t c2 = s.find(':', c1 + 1);
                const std::string buttons = s.substr(c1 + 1, c2 == std::string::npos ? std::string::npos : c2 - c1 - 1);
                if (c2 != std::string::npos)
                {
                    e.frames = std::strtoull(s.c_str() + c2 + 1, &end, 10);
                    if (*end != 0 || e.frames == 0)
                        Fatal("CB_INPUT_SCRIPT: bad frame count in \"{}\"", s);
                }
                std::string upper = buttons;
                std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char ch) { return char(std::toupper(ch)); });
                if (upper == "EXIT")
                    e.exit = true;
                else if (upper.starts_with("SHOW="))
                {
                    e.show = buttons.substr(5);
                    e.frames = 1;
                }
                else if (upper.starts_with("TYPE="))
                {
                    for (char ch : buttons.substr(5))
                        e.typed += ch == '_' ? u' ' : char16_t(uint8_t(ch));
                    e.frames = 1;
                }
                else if (!ParseButtons(buttons, e.pad))
                    Fatal("CB_INPUT_SCRIPT: unknown button name in \"{}\" (see input.h)", s);
                entries.push_back(e);
            };
            bool comment = false; // '#' up to the end of the line
            for (char ch : text)
            {
                if (ch == '\n')
                    comment = false;
                else if (ch == '#')
                    comment = true;
                if (comment)
                    continue;
                if (ch == ',' || ch == '\n' || ch == ';')
                {
                    parseItem(item);
                    item.clear();
                }
                else
                    item += ch;
            }
            parseItem(item);
            return entries;
        }

        // ---- Keyboard and mouse -------------------------------------------------------------------------------------
        // The keyboard/mouse controller (the first user without a host controller), documented in HANDOFF.md:
        //   W A S D            left stick           mouse              right stick (click the window to capture,
        //   I J K L            right stick                             F1 or leaving the window releases it)
        //   arrow keys         d-pad                left / right click RT / LT
        //   Space, Enter       A                    wheel up / down    LB / RB (one press per notch)
        //   Esc, Backspace, Q  B                    Page Up / Down     LB / RB
        //   C                  X                    Left Ctrl          left stick click
        //   E                  Y                    Left Shift         right stick click
        //   Tab                BACK                 P, Pause           START
        //   Home               Guide button
        struct KeyBinding { int vk; uint16_t button; };
        constexpr KeyBinding kKeyboardMapping[] = {
            { VK_UP, kDpadUp }, { VK_DOWN, kDpadDown }, { VK_LEFT, kDpadLeft }, { VK_RIGHT, kDpadRight },
            { VK_SPACE, kA }, { VK_RETURN, kA }, { VK_ESCAPE, kB }, { VK_BACK, kB }, { 'Q', kB }, { 'C', kX }, { 'E', kY },
            { VK_TAB, kBack }, { 'P', kStart }, { VK_PAUSE, kStart }, { VK_PRIOR, kLeftShoulder },
            { VK_NEXT, kRightShoulder }, { VK_LCONTROL, kLeftThumb }, { VK_LSHIFT, kRightThumb },
        };
        // Mouse motion per presented frame -> right-stick deflection: a step over the title's dead zone plus a
        // proportional part (CB_MOUSE_SENSITIVITY scales it).
        constexpr float kMouseBase = 7000.0f, kMouseScale = 1500.0f;
        constexpr uint64_t kWheelPressFrames = 3, kWheelGapFrames = 2;

        struct Keyboard
        {
            HWND hwnd = nullptr;
            bool focused = false;
            bool captured = false;
            std::array<bool, 256> keys{};
            bool leftButton = false, rightButton = false;
            int64_t mouseDx = 0, mouseDy = 0;      // accumulated since the last frame
            int16_t stickRX = 0, stickRY = 0;      // latched each frame
            int wheelUp = 0, wheelDown = 0;        // pending notches
            uint16_t wheelButton = 0;              // LB/RB while a wheel press is active
            uint64_t wheelPhaseEnd = 0;            // frame where the current press (or gap) ends
            bool wheelGap = false;
        };

        struct State
        {
            // Recursive: OnFrame runs a system UI's press handler under it, and a dialog that closes at once (a
            // native one, no closing animation) calls HoldTitleInput from that handler.
            std::recursive_mutex mutex;
            bool scripted = false;
            std::vector<ScriptEntry> script;
            std::array<bool, kUsers> scriptConnected{}; // users with a scripted controller (user 0 and those named)
            std::array<PadState, kUsers> scriptPad{};
            Keyboard keyboard;
            float mouseSensitivity = 1.0f;
            uint64_t frame = 0;
            std::array<bool, kUsers> hostConnected{}; // a host controller at the latest read of each user
            std::array<bool, kUsers> xinputConnected{}; // of those, an XInput one
            // When an empty XInput slot may be asked again: XInputGetState on a slot without a controller opens the
            // device stack each call (~0.1-1 ms; called every frame for each of 4 users by the title and the GPU
            // thread), so empty slots are probed twice a second (Microsoft's guidance) instead.
            std::array<std::chrono::steady_clock::time_point, kUsers> xinputProbe{};
            std::array<PadState, kUsers> lastState{};
            std::array<uint32_t, kUsers> packet{};
            std::array<KeystrokeState, kUsers> keystrokes{};
            // System UI
            bool uiActive = false;
            bool titleHeld = false;              // HoldTitleInput
            uint32_t uiUser = 0;
            std::function<bool(uint32_t, uint16_t)> uiOnPress;
            // The controller that last pressed a button, as seen by the title's polls or an open system UI.
            uint32_t lastActiveUser = 0;
            std::array<uint16_t, kUsers> uiPrevious{};
            std::array<uint16_t, kUsers> suppressed{}; // buttons hidden from the title until released
            std::array<bool, kUsers> guidePrevious{};
            std::function<void(uint32_t)> guideHandler;
            std::function<bool()> textActive;          // SetTextInput
            std::function<void(char16_t)> textType;
            GetStateEx getStateEx = nullptr;
        };
        State& G()
        {
            static State state;
            return state;
        }

        // While a system UI's text box takes typing (SetTextInput), only these keys still act as buttons; Enter is
        // its Done (START) instead of A.
        constexpr KeyBinding kTextEntryMapping[] = {
            { VK_UP, kDpadUp }, { VK_DOWN, kDpadDown }, { VK_LEFT, kDpadLeft }, { VK_RIGHT, kDpadRight },
            { VK_ESCAPE, kB }, { VK_RETURN, kStart }, { VK_TAB, kBack }, { VK_PRIOR, kLeftShoulder },
            { VK_NEXT, kRightShoulder },
        };

        PadState KeyboardPad(Keyboard& k, bool textEntry)
        {
            PadState pad;
            if (!k.focused)
                return pad;
            if (textEntry)
            {
                for (const KeyBinding& b : kTextEntryMapping)
                    if (k.keys[b.vk])
                        pad.buttons |= b.button;
                pad.guide = k.keys[VK_HOME];
                return pad;
            }
            for (const KeyBinding& b : kKeyboardMapping)
                if (k.keys[b.vk])
                    pad.buttons |= b.button;
            pad.buttons |= k.wheelButton;
            pad.guide = k.keys[VK_HOME];
            pad.thumbLX = int16_t((k.keys['D'] ? kFull : 0) - (k.keys['A'] ? kFull : 0));
            pad.thumbLY = int16_t((k.keys['W'] ? kFull : 0) - (k.keys['S'] ? kFull : 0));
            pad.thumbRX = int16_t((k.keys['L'] ? kFull : 0) - (k.keys['J'] ? kFull : 0));
            pad.thumbRY = int16_t((k.keys['I'] ? kFull : 0) - (k.keys['K'] ? kFull : 0));
            if (k.stickRX || k.stickRY)
            {
                pad.thumbRX = k.stickRX;
                pad.thumbRY = k.stickRY;
            }
            pad.rightTrigger = k.captured && k.leftButton ? 255 : 0;
            pad.leftTrigger = k.captured && k.rightButton ? 255 : 0;
            return pad;
        }

        // Caller holds the mutex.
        bool ReadPad(uint32_t user, PadState& pad)
        {
            State& g = G();
            pad = {};
            if (g.scripted)
            {
                if (user >= kUsers || !g.scriptConnected[user])
                    return false;
                pad = g.scriptPad[user];
                return true;
            }
            if (user >= kUsers)
                return false;
            XINPUT_STATE host{};
            const auto now = std::chrono::steady_clock::now();
            if (g.xinputConnected[user] || now >= g.xinputProbe[user])
            {
                g.hostConnected[user] = (g.getStateEx ? g.getStateEx(user, &host) : XInputGetState(user, &host)) ==
                    ERROR_SUCCESS;
                if (!g.hostConnected[user])
                    g.xinputProbe[user] = now + std::chrono::milliseconds(500);
            }
            else
                g.hostConnected[user] = false;
            g.xinputConnected[user] = g.hostConnected[user];
            if (!g.hostConnected[user])
            {
                // A PlayStation controller (native/x360/system/pads) for a player without an XInput
                // controller: the n-th such player gets the n-th one.
                uint32_t n = 0;
                for (uint32_t u = 0; u < user; u++)
                    n += !g.xinputConnected[u];
                pads::Pad ps;
                if (pads::Read(n, ps))
                {
                    g.hostConnected[user] = true;
                    pad.buttons = ps.buttons;
                    pad.guide = ps.guide;
                    pad.leftTrigger = ps.leftTrigger;
                    pad.rightTrigger = ps.rightTrigger;
                    pad.thumbLX = ps.thumbLX;
                    pad.thumbLY = ps.thumbLY;
                    pad.thumbRX = ps.thumbRX;
                    pad.thumbRY = ps.thumbRY;
                    return true;
                }
            }
            if (g.hostConnected[user])
            {
                pad.buttons = host.Gamepad.wButtons & ~kGuide;
                pad.guide = (host.Gamepad.wButtons & kGuide) != 0;
                pad.leftTrigger = host.Gamepad.bLeftTrigger;
                pad.rightTrigger = host.Gamepad.bRightTrigger;
                pad.thumbLX = host.Gamepad.sThumbLX;
                pad.thumbLY = host.Gamepad.sThumbLY;
                pad.thumbRX = host.Gamepad.sThumbRX;
                pad.thumbRY = host.Gamepad.sThumbRY;
                return true;
            }
            // The keyboard and mouse are the first user without a host controller (as of the latest polls).
            uint32_t keyboardUser = 0;
            while (keyboardUser < kUsers && g.hostConnected[keyboardUser])
                keyboardUser++;
            if (user == keyboardUser && g.keyboard.hwnd)
            {
                pad = KeyboardPad(g.keyboard, g.textActive && g.textActive());
                return true;
            }
            return false;
        }

        // The title's view of a controller: idle while a system UI is open, without buttons a dialog consumed.
        // Caller holds the mutex.
        bool ReadTitlePad(uint32_t user, PadState& pad)
        {
            State& g = G();
            if (!ReadPad(user, pad))
                return false;
            g.suppressed[user] &= pad.buttons;
            if (g.uiActive || g.titleHeld)
                pad = {};
            pad.buttons &= ~g.suppressed[user];
            pad.guide = false;
            return true;
        }

        constexpr uint8_t kUiTriggerThreshold = 30; // XINPUT_GAMEPAD_TRIGGER_THRESHOLD

        uint16_t UiButtons(const PadState& pad)
        {
            uint16_t buttons = pad.buttons;
            buttons |= pad.thumbLY > kThumbThreshold ? kDpadUp : 0;
            buttons |= pad.thumbLY < -kThumbThreshold ? kDpadDown : 0;
            buttons |= pad.thumbLX < -kThumbThreshold ? kDpadLeft : 0;
            buttons |= pad.thumbLX > kThumbThreshold ? kDpadRight : 0;
            buttons |= pad.leftTrigger > kUiTriggerThreshold ? kUiLeftTrigger : 0;
            buttons |= pad.rightTrigger > kUiTriggerThreshold ? kUiRightTrigger : 0;
            return buttons;
        }

        // Caller holds the mutex.
        void EndSystemUiLocked()
        {
            State& g = G();
            if (!g.uiActive)
                return;
            g.uiActive = false;
            g.uiOnPress = nullptr;
            for (uint32_t user = 0; user < kUsers; user++)
            {
                PadState pad;
                g.suppressed[user] = ReadPad(user, pad) ? pad.buttons : 0;
            }
        }

        std::string DescribePad(const PadState& p)
        {
            static const char* kNames[16] = { "UP", "DOWN", "LEFT", "RIGHT", "START", "BACK", "LS", "RS", "LB", "RB",
                "GUIDE", "?", "A", "B", "X", "Y" };
            std::string text;
            for (int i = 0; i < 16; i++)
                if (p.buttons & (1 << i))
                    text += std::string(text.empty() ? "" : "+") + kNames[i];
            if (p.guide)
                text += std::string(text.empty() ? "" : "+") + "GUIDE";
            if (p.leftTrigger || p.rightTrigger)
                text += std::format(" LT={} RT={}", p.leftTrigger, p.rightTrigger);
            if (p.thumbLX || p.thumbLY || p.thumbRX || p.thumbRY)
                text += std::format(" L=({},{}) R=({},{})", p.thumbLX, p.thumbLY, p.thumbRX, p.thumbRY);
            return text.empty() ? "NONE" : text;
        }

        void SetCapture(Keyboard& k, bool capture)
        {
            if (k.captured == capture)
                return;
            k.captured = capture;
            if (capture)
            {
                RECT rect;
                GetClientRect(k.hwnd, &rect);
                MapWindowPoints(k.hwnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
                ClipCursor(&rect);
                SetCursor(nullptr);
            }
            else
            {
                ClipCursor(nullptr);
                k.leftButton = k.rightButton = false;
                k.mouseDx = k.mouseDy = 0;
            }
        }

        // Caller holds the mutex: latches the mouse motion of the last frame and runs the wheel's button presses.
        void UpdateKeyboard(State& g, uint64_t frame)
        {
            Keyboard& k = g.keyboard;
            auto toStick = [&](int64_t d) {
                if (d == 0)
                    return int16_t(0);
                const float magnitude =
                    std::min(float(kFull), kMouseBase + std::abs(float(d)) * kMouseScale * g.mouseSensitivity);
                return int16_t(d > 0 ? magnitude : -magnitude);
            };
            k.stickRX = toStick(k.mouseDx);
            k.stickRY = toStick(-k.mouseDy);
            k.mouseDx = k.mouseDy = 0;
            if (frame >= k.wheelPhaseEnd)
            {
                if (k.wheelButton)
                {
                    k.wheelButton = 0;
                    k.wheelPhaseEnd = frame + kWheelGapFrames;
                }
                else if (k.wheelUp || k.wheelDown)
                {
                    k.wheelButton = k.wheelUp ? kLeftShoulder : kRightShoulder;
                    (k.wheelUp ? k.wheelUp : k.wheelDown)--;
                    k.wheelPhaseEnd = frame + kWheelPressFrames;
                }
            }
        }
    }

    void Initialize()
    {
        State& g = G();
        if (const char* script = std::getenv("CB_INPUT_SCRIPT"); script && *script)
        {
            g.scripted = true;
            g.script = ParseScript(script);
            g.scriptConnected[0] = true;
            for (const ScriptEntry& e : g.script)
                g.scriptConnected[e.user] = true;
            std::string users;
            for (uint32_t user = 0; user < kUsers; user++)
                if (g.scriptConnected[user])
                    users += std::format("{}{}", users.empty() ? "" : ", ", user);
            Log("Input: scripted controllers on users {} ({} entries); host controllers and keyboard ignored", users,
                g.script.size());
        }
        if (!g.scripted)
            pads::Start(); // PlayStation controllers (native/x360/system/pads)
        if (const char* s = std::getenv("CB_MOUSE_SENSITIVITY"); s && *s)
            g.mouseSensitivity = std::strtof(s, nullptr);
        if (!g.scripted)
            g.getStateEx = LoadGetStateEx();
    }

    bool GetState(uint32_t user, PadState& state, uint32_t& packetNumber)
    {
        State& g = G();
        std::lock_guard lock(g.mutex);
        if (user >= kUsers || !ReadTitlePad(user, state))
            return false;
        if (state != g.lastState[user])
        {
            if (UiButtons(state) & ~UiButtons(g.lastState[user]))
                g.lastActiveUser = user;
            g.lastState[user] = state;
            g.packet[user]++;
        }
        packetNumber = g.packet[user];
        return true;
    }

    uint32_t GetKeystroke(uint32_t user, Keystroke& keystroke)
    {
        State& g = G();
        std::lock_guard lock(g.mutex);
        PadState pad;
        if (user >= kUsers || !ReadTitlePad(user, pad))
            return kErrorDeviceNotConnected;
        KeystrokeState& k = g.keystrokes[user];
        const int64_t now = NowMs();
        keystroke = { 0, 0, 0, uint8_t(user), 0 };

        if (k.repeat == Repeat::kWaiting && now - k.repeatTime > kRepeatDelayMs)
            k.repeat = Repeat::kRepeating;
        if (k.repeat == Repeat::kRepeating && now - k.repeatTime > kRepeatRateMs)
        {
            k.repeatTime = now;
            keystroke.virtualKey = kVirtualKeys[k.repeatKey];
            keystroke.flags = kKeystrokeKeyDown | kKeystrokeRepeat;
            return kErrorSuccess;
        }
        const uint64_t current = KeyBits(pad);
        const uint64_t changed = current ^ k.down;
        // Releases first (so a stick moving from up-left to left reports UPLEFT up before LEFT down), then presses.
        for (int pass = 0; pass < 2; pass++)
        {
            for (uint32_t bit = 0; bit < kVirtualKeys.size(); bit++)
            {
                const uint64_t mask = uint64_t(1) << bit;
                if (!(changed & mask) || kVirtualKeys[bit] == 0)
                    continue;
                const bool pressed = (current & mask) != 0;
                if (pass == 0 && !pressed)
                {
                    k.down &= ~mask;
                    if (k.repeatKey == bit)
                        k.repeat = Repeat::kIdle;
                    keystroke.virtualKey = kVirtualKeys[bit];
                    keystroke.flags = kKeystrokeKeyUp;
                    return kErrorSuccess;
                }
                if (pass == 1 && pressed)
                {
                    k.down |= mask;
                    k.repeat = Repeat::kWaiting;
                    k.repeatKey = bit;
                    k.repeatTime = now;
                    keystroke.virtualKey = kVirtualKeys[bit];
                    keystroke.flags = kKeystrokeKeyDown;
                    return kErrorSuccess;
                }
            }
        }
        return kErrorEmpty;
    }

    void OnFrame(uint64_t frame)
    {
        State& g = G();
        std::vector<uint32_t> guidePresses;
        std::vector<std::pair<uint32_t, std::string>> shows; // SHOW= entries, opened after the lock
        std::function<void(uint32_t)> guideHandler;
        {
            std::lock_guard lock(g.mutex);
            g.frame = frame;
            if (g.scripted)
            {
                std::array<PadState, kUsers> pads{};
                for (const ScriptEntry& e : g.script)
                {
                    if (frame < e.start || frame >= e.start + e.frames)
                        continue;
                    if (e.exit)
                    {
                        Log("Input script: frame {}: EXIT", frame);
                        live::SignOutAllForExit(); // as closing the window
                        kernel::stfs_mount::WaitForAllWrites(); // content packages still being written back
                        logging::Flush();
                        ExitProcess(0);
                    }
                    if (!e.show.empty())
                    {
                        Log("Input script: frame {}: {}", frame, e.text);
                        shows.emplace_back(e.user, e.show);
                        continue;
                    }
                    if (!e.typed.empty())
                    {
                        const bool active = g.textActive && g.textType && g.textActive();
                        Log("Input script: frame {}: {}", frame, active ? e.text : e.text + " (no text box open)");
                        if (active)
                            for (char16_t c : e.typed)
                                g.textType(c);
                        continue;
                    }
                    PadState& pad = pads[e.user];
                    pad.buttons |= e.pad.buttons;
                    pad.guide |= e.pad.guide;
                    pad.leftTrigger = std::max(pad.leftTrigger, e.pad.leftTrigger);
                    pad.rightTrigger = std::max(pad.rightTrigger, e.pad.rightTrigger);
                    pad.thumbLX = e.pad.thumbLX ? e.pad.thumbLX : pad.thumbLX;
                    pad.thumbLY = e.pad.thumbLY ? e.pad.thumbLY : pad.thumbLY;
                    pad.thumbRX = e.pad.thumbRX ? e.pad.thumbRX : pad.thumbRX;
                    pad.thumbRY = e.pad.thumbRY ? e.pad.thumbRY : pad.thumbRY;
                }
                for (uint32_t user = 0; user < kUsers; user++)
                {
                    if (pads[user] != g.scriptPad[user])
                        Log("Input script: frame {}: user {}: {}", frame, user, DescribePad(pads[user]));
                    g.scriptPad[user] = pads[user];
                }
            }
            else
                UpdateKeyboard(g, frame);

            if (g.uiActive)
            {
                for (uint32_t user = 0; user < kUsers; user++)
                {
                    if (g.uiUser != 0xFF && user != g.uiUser)
                        continue;
                    PadState pad;
                    const uint16_t buttons = ReadPad(user, pad) ? UiButtons(pad) : 0;
                    const uint16_t pressed = buttons & ~g.uiPrevious[user];
                    g.uiPrevious[user] = buttons;
                    if (pressed)
                        g.lastActiveUser = user;
                    if (pressed && g.uiOnPress(user, pressed))
                    {
                        EndSystemUiLocked();
                        break;
                    }
                }
            }
            // Guide button presses, any controller.
            for (uint32_t user = 0; user < kUsers; user++)
            {
                PadState pad;
                const bool guide = ReadPad(user, pad) && pad.guide;
                if (guide && !g.guidePrevious[user])
                {
                    guidePresses.push_back(user);
                    g.lastActiveUser = user;
                }
                g.guidePrevious[user] = guide;
            }
            guideHandler = g.guideHandler;
        }
        if (guideHandler)
            for (uint32_t user : guidePresses)
                guideHandler(user);
        for (const auto& [user, screen] : shows)
            kernel::ShowSystemScreenForTest(user, screen);
    }

    void SetGuideHandler(std::function<void(uint32_t user)> handler)
    {
        State& g = G();
        std::lock_guard lock(g.mutex);
        g.guideHandler = std::move(handler);
    }

    void SetTextInput(std::function<bool()> active, std::function<void(char16_t)> type)
    {
        State& g = G();
        std::lock_guard lock(g.mutex);
        g.textActive = std::move(active);
        g.textType = std::move(type);
    }

    uint32_t LastActiveUser()
    {
        State& g = G();
        std::lock_guard lock(g.mutex);
        return g.lastActiveUser;
    }

    void BeginSystemUi(uint32_t user, std::function<bool(uint32_t user, uint16_t pressed)> onPress)
    {
        State& g = G();
        std::lock_guard lock(g.mutex);
        if (g.uiActive)
            Fatal("Input: a system UI opened while another one is open");
        g.uiActive = true;
        g.uiUser = user;
        g.uiOnPress = std::move(onPress);
        // Buttons already held do not count as presses.
        for (uint32_t u = 0; u < kUsers; u++)
        {
            PadState pad;
            g.uiPrevious[u] = ReadPad(u, pad) ? UiButtons(pad) : 0;
        }
    }

    void EndSystemUi()
    {
        State& g = G();
        std::lock_guard lock(g.mutex);
        EndSystemUiLocked();
    }

    void HoldTitleInput(bool hold)
    {
        State& g = G();
        std::lock_guard lock(g.mutex);
        if (g.titleHeld == hold)
            return;
        g.titleHeld = hold;
        if (!hold && !g.uiActive)
            for (uint32_t user = 0; user < kUsers; user++)
            {
                PadState pad;
                g.suppressed[user] = ReadPad(user, pad) ? pad.buttons : 0;
            }
    }

    void SetWindow(HWND hwnd)
    {
        State& g = G();
        std::lock_guard lock(g.mutex);
        g.keyboard.hwnd = hwnd;
        g.keyboard.focused = GetForegroundWindow() == hwnd;
        RAWINPUTDEVICE mouse{ 0x01, 0x02, 0, hwnd }; // generic desktop, mouse
        if (!RegisterRawInputDevices(&mouse, 1, sizeof(mouse)))
            Log("Input: RegisterRawInputDevices failed ({}); mouse look unavailable", GetLastError());
    }

    bool HandleWindowMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        State& g = G();
        std::lock_guard lock(g.mutex);
        Keyboard& k = g.keyboard;
        switch (message)
        {
        case WM_SETFOCUS:
            k.focused = true;
            return false;
        case WM_KILLFOCUS:
            k.focused = false;
            k.keys = {};
            k.wheelUp = k.wheelDown = 0;
            SetCapture(k, false);
            return false;
        case WM_CHAR:
            // Typing into a system UI's text box (the Guide's keyboard): characters and Backspace; Enter and Esc
            // stay buttons (KeyboardPad).
            if (g.textActive && g.textType && wParam != '\r' && wParam != 0x1B && g.textActive())
            {
                g.textType(char16_t(wParam));
                return true;
            }
            return false;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            const bool down = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;
            int vk = int(wParam);
            if (vk == VK_SHIFT)
                vk = MapVirtualKeyW(UINT(lParam >> 16) & 0xFF, MAPVK_VSC_TO_VK_EX);
            else if (vk == VK_CONTROL)
                vk = (lParam & (1 << 24)) ? VK_RCONTROL : VK_LCONTROL;
            if (vk == VK_F1 && down)
                SetCapture(k, false);
            if (vk >= 0 && vk < 256)
                k.keys[vk] = down;
            if (vk == VK_F4 && (lParam & (1 << 29)))
                return false; // Alt+F4 closes the window
            return message == WM_SYSKEYDOWN || message == WM_SYSKEYUP ? vk != VK_MENU : true;
        }
        case WM_LBUTTONDOWN:
            if (!k.captured)
                SetCapture(k, true); // the capturing click is not a trigger press
            else
                k.leftButton = true;
            return true;
        case WM_LBUTTONUP:
            k.leftButton = false;
            return true;
        case WM_RBUTTONDOWN:
            k.rightButton = true;
            return true;
        case WM_RBUTTONUP:
            k.rightButton = false;
            return true;
        case WM_MOUSEWHEEL:
        {
            const int notches = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
            if (k.captured || k.focused)
                (notches > 0 ? k.wheelUp : k.wheelDown) += std::abs(notches);
            return true;
        }
        case WM_SETCURSOR:
            if (k.captured && LOWORD(lParam) == HTCLIENT)
            {
                SetCursor(nullptr);
                return true;
            }
            return false;
        case WM_SIZE:
        case WM_MOVE:
            if (k.captured)
            {
                SetCapture(k, false);
                SetCapture(k, true);
            }
            return false;
        case WM_INPUT:
        {
            RAWINPUT raw{};
            UINT size = sizeof(raw);
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, &raw, &size, sizeof(RAWINPUTHEADER)) !=
                    UINT(-1) && raw.header.dwType == RIM_TYPEMOUSE && k.captured &&
                !(raw.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE))
            {
                k.mouseDx += raw.data.mouse.lLastX;
                k.mouseDy += raw.data.mouse.lLastY;
            }
            return false; // DefWindowProc must still run for WM_INPUT
        }
        }
        (void)hwnd;
        return false;
    }
}
