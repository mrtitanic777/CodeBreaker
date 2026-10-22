// XAM controller input. The virtual controllers behind it (host XInput pads, keyboard/mouse, CB_INPUT_SCRIPT) and
// the keystroke generation are in input.h. Structures are written big-endian.

#include "kernel/kernel.h"

#include "input.h"

#include <cstring>

namespace
{
    constexpr uint32_t kErrorSuccess = 0;
    constexpr uint32_t kErrorDeviceNotConnected = 0x48F;
    constexpr uint32_t kErrorEmpty = 0x10D2;
    constexpr uint32_t kUserIndexAny = 0xFF;
    constexpr uint32_t kFlagBigButton = 8, kFlagGamepad = 0x00000001;
    constexpr uint32_t kFlagAnyUser = 0x40000000;
}

// DWORD XamInputGetState(DWORD UserIndex, DWORD Flags, PXINPUT_STATE State)
// XINPUT_STATE: dwPacketNumber (u32), XINPUT_GAMEPAD { wButtons (u16), bLeftTrigger, bRightTrigger (u8),
// sThumbLX, sThumbLY, sThumbRX, sThumbRY (s16) }. The title's XInputGetState passes Flags = XINPUT_FLAG_GAMEPAD.
GUEST_IMPORT(XamInputGetState)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32;
    const uint32_t flags = ctx.r4.u32;
    const uint32_t state = ctx.r5.u32;
    // XINPUT_FLAG_BIGBUTTON (8): XAM 17559 reads the user's device slot 3 instead of the gamepad's (state zeroed
    // first); no Big Button pad is ever connected here.
    if (flags == kFlagBigButton)
    {
        if (state)
            std::memset(mem.Translate(state), 0, 0x10);
        ctx.r3.u64 = kErrorDeviceNotConnected;
        return;
    }
    if (flags != kFlagGamepad && flags != 0)
        Fatal("XamInputGetState: flags {:08X} not implemented (gamepad and Big Button only)", flags);
    input::PadState pad;
    uint32_t packet = 0;
    if (!input::GetState(user, pad, packet))
    {
        ctx.r3.u64 = kErrorDeviceNotConnected;
        return;
    }
    if (state)
    {
        mem.Write32(state + 0x00, packet);
        mem.Write16(state + 0x04, pad.buttons);
        mem.Write8(state + 0x06, pad.leftTrigger);
        mem.Write8(state + 0x07, pad.rightTrigger);
        mem.Write16(state + 0x08, uint16_t(pad.thumbLX));
        mem.Write16(state + 0x0A, uint16_t(pad.thumbLY));
        mem.Write16(state + 0x0C, uint16_t(pad.thumbRX));
        mem.Write16(state + 0x0E, uint16_t(pad.thumbRY));
    }
    ctx.r3.u64 = kErrorSuccess;
}

// DWORD XamInputGetCapabilities(DWORD UserIndex, DWORD Flags, PXINPUT_CAPABILITIES Capabilities)
// XAM 17559 (0x816F2E70): zeroes the 0x14-byte result, calls XamInputGetCapabilitiesEx(1, ...) and copies its result
// on success. Ex (0x816F12B8): Flags 0 = 3; gamepad (1) and Big Button (8) ask the controller driver
// (XInputdGetCapabilities: the controller's own capability report), no device = ERROR_DEVICE_NOT_CONNECTED.
// XINPUT_CAPABILITIES: Type (u8), SubType (u8), Flags (u16), XINPUT_GAMEPAD (the bits and ranges the pad has, 0xC),
// XINPUT_VIBRATION (u16 left, u16 right). Our pads report a wired Xbox 360 controller as Windows' XInput reads it
// from the same report: gamepad 1/1, buttons 0xF3FF, triggers 0xFF, thumbsticks 0xFFC0, motors 0xFF.
// The dash calls it for users 0-3 to find the first connected controller.
GUEST_IMPORT(XamInputGetCapabilities)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, flags = ctx.r4.u32 ? ctx.r4.u32 : 3, caps = ctx.r5.u32;
    if (caps)
        std::memset(mem.Translate(caps), 0, 0x14);
    if (!(flags & kFlagGamepad))
        Fatal("XamInputGetCapabilities(user {}): flags {:08X} (only the gamepad is implemented)", user, ctx.r4.u32);
    input::PadState pad;
    uint32_t packet = 0;
    if (user >= 4 || !input::GetState(user, pad, packet))
    {
        ctx.r3.u64 = kErrorDeviceNotConnected;
        return;
    }
    if (caps)
    {
        mem.Write8(caps + 0x00, 1);       // XINPUT_DEVTYPE_GAMEPAD
        mem.Write8(caps + 0x01, 1);       // XINPUT_DEVSUBTYPE_GAMEPAD
        mem.Write16(caps + 0x04, 0xF3FF); // buttons
        mem.Write8(caps + 0x06, 0xFF);
        mem.Write8(caps + 0x07, 0xFF);
        for (uint32_t i = 0; i < 4; i++)
            mem.Write16(caps + 0x08 + 2 * i, 0xFFC0);
        mem.Write16(caps + 0x10, 0xFF);
        mem.Write16(caps + 0x12, 0xFF);
    }
    ctx.r3.u64 = kErrorSuccess;
}

// DWORD XamInputGetKeystrokeEx(PDWORD UserIndex, DWORD Flags, PXINPUT_KEYSTROKE Keystroke)
// XINPUT_KEYSTROKE: VirtualKey (u16), Unicode (u16), Flags (u16), UserIndex (u8), HidCode (u8).
// UserIndex is in/out: XUSER_INDEX_ANY (or XINPUT_FLAG_ANYUSER) reads the next event of any user and reports which
// user produced it. Only gamepads exist (no chatpad or USB keyboard is attached), so a request without
// XINPUT_FLAG_GAMEPAD finds no device. The title calls it with XUSER_INDEX_ANY and ANYUSER | ANYDEVICE.
GUEST_IMPORT(XamInputGetKeystrokeEx)
{
    auto& mem = guest::g_memory;
    const uint32_t userPtr = ctx.r3.u32;
    const uint32_t flags = ctx.r4.u32;
    const uint32_t keystroke = ctx.r5.u32;
    const uint32_t requested = userPtr ? mem.Read32(userPtr) : kUserIndexAny;
    const bool anyUser = (requested & 0xFF) == kUserIndexAny || (flags & kFlagAnyUser);
    if (!(flags & kFlagGamepad))
    {
        ctx.r3.u64 = kErrorDeviceNotConnected;
        return;
    }
    uint32_t result = kErrorDeviceNotConnected;
    input::Keystroke event{};
    for (uint32_t user = anyUser ? 0 : requested; user < (anyUser ? 4u : requested + 1); user++)
    {
        const uint32_t r = input::GetKeystroke(user, event);
        if (r == kErrorSuccess)
        {
            result = r;
            break;
        }
        if (r == kErrorEmpty)
            result = r;
    }
    if (result == kErrorSuccess)
    {
        if (keystroke)
        {
            mem.Write16(keystroke + 0x0, event.virtualKey);
            mem.Write16(keystroke + 0x2, event.unicode);
            mem.Write16(keystroke + 0x4, event.flags);
            mem.Write8(keystroke + 0x6, event.userIndex);
            mem.Write8(keystroke + 0x7, event.hidCode);
        }
        if (userPtr)
            mem.Write32(userPtr, event.userIndex);
    }
    ctx.r3.u64 = result;
}
