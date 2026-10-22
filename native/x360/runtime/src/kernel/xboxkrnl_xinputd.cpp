// The kernel's controller driver (XInputd*) for the console's own XAM (CB_RECOMP_XAM, kernel/xam_recompiled.cpp).
// The runtime's virtual controllers (input.h) are the devices. As the console's driver does, a controller that
// connects is bound to a user through the binding callback XAM registered (DrvSetUserBindingCallback, xam.xex
// 0x816D9060), XAM then reads it with XInputdReadState by its handle, and the Guide button goes to XAM's system
// request callback (DrvSetSysReqCallback, 0x816F09E8), whose worker loads hud.xex. The binding and Guide calls run on
// a guest thread of the driver's own (as the audio driver's callbacks do).
//
// Semantics from xam.xex 17559 (analysis/xam_all_ann.c): binding callback (r3 binding key, r4 device handle, r5 slot
// type 0 = gamepad, r6 0 = bind / 1 = unbind, r7 -> the preferred user, which XAM overwrites with the user it chose);
// XInputdReadState(handle, packet*, XINPUT_GAMEPAD*, restricted*); XInputdGetCapabilities(handle, caps*, restricted*);
// STATUS_DEVICE_NOT_CONNECTED for a device that is not there.

#include "kernel/kernel.h"
#include "input.h"

#include <windows.h>

#include <array>
#include <atomic>
#include <deque>
#include <mutex>

namespace
{
    constexpr uint32_t kStatusDeviceNotConnected = 0xC000009D;
    constexpr uint32_t kUsers = 4;
    // A gamepad's handle and binding key: stable per user, top nibble 1 (XAM gives 0x2-0x7 nibbles other meanings).
    constexpr uint32_t PadHandle(uint32_t user) { return 0x10000000u | user; }
    constexpr uint32_t PadKey(uint32_t user) { return 0x01000000u | user; }

    std::atomic<uint32_t> g_bindingCallback = 0, g_sysReqCallback = 0;
    std::atomic<bool> g_started = false;
    std::mutex g_guideMutex;
    std::deque<uint32_t> g_guidePresses; // users whose Guide button was pressed, for the driver thread

    bool UserOfHandle(uint32_t handle, uint32_t& user)
    {
        if ((handle & 0xFFFFFFF0u) != 0x10000000u || (handle & 0xF) >= kUsers)
            return false;
        user = handle & 0xF;
        return true;
    }

    // Calls a guest function on the driver thread's context.
    void Call(PPCContext& ctx, uint32_t function, std::initializer_list<uint32_t> args)
    {
        const uint64_t stack = ctx.r1.u64;
        // By name: PPCContext does not keep the registers in number order.
        PPCRegister* const regs[] = { &ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6, &ctx.r7, &ctx.r8, &ctx.r9, &ctx.r10 };
        uint32_t i = 0;
        for (uint32_t a : args)
            regs[i++]->u64 = a;
        kernel::CallGuest(ctx, function);
        ctx.r1.u64 = stack;
    }

    void DriverThread()
    {
        kernel::ThreadParams params;
        params.stackSize = 0x10000;
        params.processType = kernel::kProcessTypeSystem;
        kernel::GuestThread* thread = kernel::CreateThread(params);
        PPCContext* ctx = kernel::AttachHostThread(thread);
        ctx->r1.u64 = thread->stackBase; // the calls into XAM run on this thread's own guest stack
        auto& mem = guest::g_memory;
        const uint32_t userByte = mem.SystemAlloc(4);
        std::array<bool, kUsers> bound{};
        for (;;)
        {
            Sleep(8);
            const uint32_t binding = g_bindingCallback.load();
            if (binding == 0)
                continue;
            for (uint32_t u = 0; u < kUsers; u++)
            {
                input::PadState state;
                uint32_t packet = 0;
                const bool connected = input::GetState(u, state, packet);
                if (connected == bound[u])
                    continue;
                mem.Write8(userByte, uint8_t(u));
                Call(*ctx, binding, { PadKey(u), PadHandle(u), 0, connected ? 0u : 1u, userByte });
                bound[u] = connected;
                Log("XInputd: controller {} {} (XAM's user {})", u, connected ? "bound" : "unbound", mem.Read8(userByte));
            }
            std::deque<uint32_t> presses;
            {
                std::lock_guard lock(g_guideMutex);
                presses.swap(g_guidePresses);
            }
            for (uint32_t user : presses)
            {
                // XAM passes 0 while it handles a request: a press meanwhile is dropped, as the console's driver does.
                const uint32_t sysReq = g_sysReqCallback.load();
                if (sysReq == 0 || user >= kUsers || !bound[user])
                    continue;
                Log("XInputd: Guide button on controller {} -> XAM's system request callback {:08X}", user, sysReq);
                Call(*ctx, sysReq, { PadHandle(user), 0, 0 }); // kind 0: a Guide button press
            }
        }
    }

    void Start()
    {
        if (g_started.exchange(true))
            return;
        input::SetGuideHandler([](uint32_t user) {
            std::lock_guard lock(g_guideMutex);
            g_guidePresses.push_back(user);
        });
        kernel::StartHostThread(DriverThread);
    }
}

namespace kernel
{
    // The binding callback: registered by kernel/xboxkrnl_drivers.cpp's DrvSetUserBindingCallback.
    void SetXInputdBindingCallback(uint32_t callback)
    {
        g_bindingCallback = callback;
        if (callback && RecompiledXam())
            Start();
    }
}

// VOID DrvSetSysReqCallback(PVOID Callback): XAM's handler for the Guide button and the other system requests (0 while
// it handles one; it registers again after).
GUEST_IMPORT(DrvSetSysReqCallback) { g_sysReqCallback = ctx.r3.u32; }

// NTSTATUS XInputdReadState(HANDLE Device, PDWORD PacketNumber, PXINPUT_GAMEPAD Gamepad, PDWORD Restricted)
GUEST_IMPORT(XInputdReadState)
{
    auto& mem = guest::g_memory;
    uint32_t user;
    input::PadState state;
    uint32_t packet = 0;
    if (!UserOfHandle(ctx.r3.u32, user) || !input::GetState(user, state, packet))
    {
        ctx.r3.u64 = kStatusDeviceNotConnected;
        return;
    }
    if (ctx.r4.u32)
        mem.Write32(ctx.r4.u32, packet);
    if (const uint32_t pad = ctx.r5.u32)
    {
        mem.Write16(pad, uint16_t(state.buttons | (state.guide ? 0x0400 : 0))); // XAM hides 0x0400 from titles
        mem.Write8(pad + 2, state.leftTrigger);
        mem.Write8(pad + 3, state.rightTrigger);
        mem.Write16(pad + 4, uint16_t(state.thumbLX));
        mem.Write16(pad + 6, uint16_t(state.thumbLY));
        mem.Write16(pad + 8, uint16_t(state.thumbRX));
        mem.Write16(pad + 10, uint16_t(state.thumbRY));
    }
    if (ctx.r6.u32)
        mem.Write32(ctx.r6.u32, 0);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS XInputdGetCapabilities(HANDLE Device, PXINPUT_CAPABILITIES Capabilities, PDWORD Restricted): a standard
// gamepad (type 1, subtype 1, no force feedback flag: XAM rumbles it through XInputdWriteState), every control.
GUEST_IMPORT(XInputdGetCapabilities)
{
    auto& mem = guest::g_memory;
    uint32_t user;
    input::PadState state;
    uint32_t packet;
    if (!UserOfHandle(ctx.r3.u32, user) || !input::GetState(user, state, packet))
    {
        ctx.r3.u64 = kStatusDeviceNotConnected;
        return;
    }
    if (const uint32_t caps = ctx.r4.u32)
    {
        for (uint32_t i = 0; i < 0x20; i++)
            mem.Write8(caps + i, 0);
        mem.Write8(caps + 0, 1);         // XINPUT_DEVTYPE_GAMEPAD
        mem.Write8(caps + 1, 1);         // XINPUT_DEVSUBTYPE_GAMEPAD
        mem.Write16(caps + 4, 0xF3FF);   // Gamepad: every button
        mem.Write8(caps + 6, 0xFF);
        mem.Write8(caps + 7, 0xFF);
        for (uint32_t i = 0; i < 4; i++)
            mem.Write16(caps + 8 + i * 2, 0xFFC0);
        mem.Write16(caps + 0x10, 0xFFFF); // Vibration: both motors
        mem.Write16(caps + 0x12, 0xFFFF);
    }
    if (ctx.r5.u32)
        mem.Write32(ctx.r5.u32, 0);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS XInputdWriteState(HANDLE Device, DWORD Level, PXINPUT_VIBRATION Vibration, ...): rumble. The runtime's
// controllers do not rumble yet.
GUEST_IMPORT(XInputdWriteState)
{
    uint32_t user;
    ctx.r3.u64 = UserOfHandle(ctx.r3.u32, user) ? kernel::kStatusSuccess : kStatusDeviceNotConnected;
}

// VOID XInputdSetRingOfLight(DWORD UsersMask) / XInputdNotify(HANDLE Device, DWORD Code): the ring of light's lit
// quadrants (no ring on a PC controller).
GUEST_IMPORT(XInputdSetRingOfLight) {}
GUEST_IMPORT(XInputdNotify) { ctx.r3.u64 = kernel::kStatusSuccess; }

// NTSTATUS XInputdGetDeviceStats(HANDLE Device, DWORD, PVOID Stats, PDWORD Restricted): a wired controller's (no
// battery, no radio).
GUEST_IMPORT(XInputdGetDeviceStats)
{
    auto& mem = guest::g_memory;
    for (uint32_t i = 0; ctx.r5.u32 && i < 0x14; i++)
        mem.Write8(ctx.r5.u32 + i, 0);
    if (ctx.r6.u32)
        mem.Write32(ctx.r6.u32, 0);
    uint32_t user;
    ctx.r3.u64 = UserOfHandle(ctx.r3.u32, user) ? kernel::kStatusSuccess : kStatusDeviceNotConnected;
}

GUEST_IMPORT(XInputdPowerDownDevice) { ctx.r3.u64 = kernel::kStatusSuccess; }
GUEST_IMPORT(XInputdSendStayAliveRequest) { ctx.r3.u64 = kernel::kStatusSuccess; }
GUEST_IMPORT(XInputdGetFailedConnectionOrBind) { ctx.r3.u64 = kStatusDeviceNotConnected; }

// NTSTATUS XInputdReadTextKeystroke(DWORD User, PXINPUT_KEYSTROKE Keystroke): a chatpad's or keyboard's keystroke;
// none (0x10D2 = empty).
GUEST_IMPORT(XInputdReadTextKeystroke) { ctx.r3.u64 = 0x10D2; }
GUEST_IMPORT(XInputdGetTextDeviceKeyLocks) { ctx.r3.u64 = kStatusDeviceNotConnected; }
GUEST_IMPORT(XInputdControl) { ctx.r3.u64 = kStatusDeviceNotConnected; }

// NTSTATUS HidGetCapabilities(DWORD User, PVOID Capabilities): a USB keyboard bound to the user (XamInputGetCapabilities
// with the keyboard flag, 0x816F149C; the virtual keyboard asks). None.
GUEST_IMPORT(HidGetCapabilities) { ctx.r3.u64 = kStatusDeviceNotConnected; }
