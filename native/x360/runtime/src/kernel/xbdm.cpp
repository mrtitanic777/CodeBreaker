// xbdm.xex, the development kit's debug monitor: what devkit builds import (the 3519 dashboard, a debug build).
// Ordinals as the XDK's xbdm.lib names them. No debugger connects to this console.

#include "kernel/kernel.h"

namespace
{
    constexpr uint32_t kXbdmNoError = 0x02DA0000; // XBDM_NOERR
}

// HRESULT DmRegisterCommandProcessor(LPCSTR Processor, PDM_CMDPROC Handler) (ordinal 30): a handler for the
// commands a debugger on the PC sends with that prefix. None are sent here: accepted, never called.
GUEST_IMPORT(xbdm_Ordinal_30)
{
    const char* name = ctx.r3.u32 ? reinterpret_cast<const char*>(guest::g_memory.Translate(ctx.r3.u32)) : "";
    Log("DmRegisterCommandProcessor(\"{}\", {:08X}): no debugger connects, accepted", name, ctx.r4.u32);
    ctx.r3.u64 = kXbdmNoError;
}

// HRESULT DmSendNotificationString(LPCSTR String) (ordinal 36): a notification for the debugger: logged.
GUEST_IMPORT(xbdm_Ordinal_36)
{
    const char* text = ctx.r3.u32 ? reinterpret_cast<const char*>(guest::g_memory.Translate(ctx.r3.u32)) : "";
    Log("DmSendNotificationString: {}", text);
    ctx.r3.u64 = kXbdmNoError;
}
