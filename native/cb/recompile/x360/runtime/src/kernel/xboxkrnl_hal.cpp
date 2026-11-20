// xboxkrnl HAL exports: the system management controller (SMC).

#include "kernel/kernel.h"

#include <atomic>
#include <chrono>

namespace
{
    // The tray's last command and when it was given: it moves for about a second (the drive's motor), then rests.
    std::atomic<bool> g_trayOpen = false;
    std::atomic<int64_t> g_trayMovedAt = INT64_MIN / 2;
    constexpr int64_t kTrayMoveMs = 1000;

    int64_t NowMs()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
    }
}

uint32_t kernel::DvdTrayState()
{
    const bool moving = NowMs() - g_trayMovedAt.load() < kTrayMoveMs;
    return g_trayOpen ? (moving ? 3 : 2) : (moving ? 1 : 0);
}

// VOID HalOpenCloseODDTray(BOOL Open): the SMC opens or closes the DVD drive's tray. The dash's eject action
// (0x92332408: no disc and the tray closed or closing -> open; open or opening -> close). The drive stays empty.
GUEST_IMPORT(HalOpenCloseODDTray)
{
    kernel::OpenCloseDvdTray(ctx.r3.u32 != 0);
}

void kernel::OpenCloseDvdTray(bool open)
{
    Log("HalOpenCloseODDTray: the DVD tray {}", open ? "opens" : "closes");
    if (open != g_trayOpen.exchange(open))
        g_trayMovedAt = NowMs();
}

// VOID HalSendSMCMessage(PVOID Request, PVOID Response)
// Requests and responses are 16 bytes, the command in the first byte (Free60 SMC documentation; Xenia's smc.h).
// Commands the PC has no hardware for are accepted and logged; unknown ones stop with the bytes.
GUEST_IMPORT(HalSendSMCMessage)
{
    auto& mem = guest::g_memory;
    const uint32_t request = ctx.r3.u32;
    const uint32_t response = ctx.r4.u32;
    if (request == 0)
        return;
    uint8_t bytes[16];
    for (uint32_t i = 0; i < 16; i++)
        bytes[i] = mem.Read8(request + i);
    switch (bytes[0])
    {
    case 0x8C:
        // SET_POWER_LED: bootanim sends 8C 01 01 when the animation starts and 8C 01 00 when it ends (the ring of
        // light's power-on sweep). No response.
        Log("SMC: set power LED {:02X} {:02X}", bytes[1], bytes[2]);
        break;
    case 0x07:
    {
        // QUERY_TEMPERATURES: 07 then four big-endian 8.8 fixed-point readings, CPU, GPU, memory, board (xam.xex
        // 0x816B65A8 rounds them). An idle console's values.
        static constexpr uint8_t kTemps[4] = { 52, 49, 46, 41 };
        if (response != 0)
        {
            for (uint32_t i = 0; i < 16; i++)
                mem.Write8(response + i, 0);
            mem.Write8(response, 0x07);
            for (uint32_t i = 0; i < 4; i++)
                mem.Write8(response + 1 + i * 2, kTemps[i]);
        }
        return;
    }
    case 0x1E:
        // A hardware query whose reply starts with a generation nibble and a 12-bit version (xam.xex 0x816B6AA0 asks
        // for more only from generation 2 on). Generation 0, as the older boards report: no follow-up queries.
        if (response != 0)
        {
            for (uint32_t i = 0; i < 16; i++)
                mem.Write8(response + i, 0);
            mem.Write8(response, 0x1E);
        }
        return;
    case 0x12:
        // QUERY_SMC_VERSION: 12, then the SMC's type and version (xam.xex reads byte 3: 0x816B29B8). A retail
        // console's.
        if (response != 0)
        {
            static constexpr uint8_t kVersion[4] = { 0x12, 0x41, 0x02, 0x03 };
            for (uint32_t i = 0; i < 16; i++)
                mem.Write8(response + i, i < 4 ? kVersion[i] : 0);
        }
        return;
    case 0x0A:
    {
        // QUERY_TRAY_STATE: 0A then the tray (xam.xex 0x816AE000: 60 open, 62 closed, 63 opening, 64 closing).
        static constexpr uint8_t kTray[4] = { 0x62, 0x64, 0x60, 0x63 }; // kernel::DvdTrayState(): closed closing open opening
        if (response != 0)
        {
            for (uint32_t i = 0; i < 16; i++)
                mem.Write8(response + i, 0);
            mem.Write8(response, 0x0A);
            mem.Write8(response + 1, kTray[kernel::DvdTrayState() & 3]);
        }
        return;
    }
    default:
        Fatal("HalSendSMCMessage: SMC command {:02X} ({:02X} {:02X} {:02X} {:02X} ...) is not implemented (response {:08X}, "
              "from {:08X})", bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], response, uint32_t(ctx.lr));
    }
    if (response != 0)
        Fatal("HalSendSMCMessage: command {:02X} with a response buffer", bytes[0]);
}
