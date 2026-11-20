// XamGetServiceEndpoint: the URL of an Xbox LIVE service, from the Live Hive our LIVE server sends (kernel/xam_live.cpp).

#include "kernel/kernel.h"
#include "kernel/overlapped.h"

#include <string>

// DWORD XamGetServiceEndpoint(LPCSTR Service, LPSTR Buffer, DWORD Size, PXOVERLAPPED Overlapped) (ordinal 1399; XAM
// 0x816BAEF8): XAM reads the Live Hive for the key "LiveServiceUrl." + the service name (formatted into a 35-byte buffer,
// so the key is cut at 34 characters; the values are the server's own LiveServiceUrl.<service>, kernel::LiveHiveValue)
// with XamQueryLiveHiveA (xam_live.cpp: found copies the value, an unknown key is 1, a buffer too small 0x8007007A, and
// without a profile on Xbox LIVE the hive was never downloaded: XONLINE_E_LOGON_NOT_LOGGED_ON 0x80151802). The hive's
// answer becomes: found 0; no such key, a buffer too small and not logged on all ERROR_NOT_FOUND (0x490, XAM's one
// failure; winerror.h in the XDK references); with an overlapped (completed at once as XamQueryLiveHive does:
// InternalLow 0, the hive's result as the extended error) ERROR_IO_PENDING, but only when the hive was reachable. A NULL
// service or buffer is ERROR_INVALID_PARAMETER (XAM would fault).
GUEST_IMPORT(XamGetServiceEndpoint)
{
    auto& mem = guest::g_memory;
    const uint32_t namePtr = ctx.r3.u32, buffer = ctx.r4.u32, size = ctx.r5.u32, overlapped = ctx.r6.u32;
    if (!namePtr || !buffer)
    {
        ctx.r3.u64 = 87; // ERROR_INVALID_PARAMETER
        return;
    }
    std::string key = "LiveServiceUrl.";
    for (uint32_t i = 0; i < 20; i++)
    {
        const char c = char(mem.Read8(namePtr + i));
        if (!c)
            break;
        key += c;
    }
    if (key.size() > 34)
        key.resize(34);

    uint32_t hive = 0x80151802; // XONLINE_E_LOGON_NOT_LOGGED_ON
    if (kernel::ConsoleOnLive())
    {
        hive = 1; // not set
        if (const auto value = kernel::LiveHiveValue(key))
        {
            if (size < value->size() + 1)
                hive = 0x8007007A; // buffer too small
            else
            {
                for (size_t i = 0; i < value->size(); i++)
                    mem.Write8(buffer + uint32_t(i), uint8_t((*value)[i]));
                mem.Write8(buffer + uint32_t(value->size()), 0);
                hive = 0;
            }
        }
    }
    Log("XamGetServiceEndpoint(\"{}\"): {}", key,
        hive == 0 ? "found" : hive == 1 ? "not set" : hive == 0x8007007A ? "buffer too small" : "not logged on to Xbox LIVE");
    if (overlapped && int32_t(hive) >= 0)
    {
        kernel::CompleteOverlapped(overlapped, 0, hive, 0);
        ctx.r3.u64 = kernel::kErrorIoPending;
        return;
    }
    ctx.r3.u64 = hive == 0 ? 0 : 0x490; // ERROR_NOT_FOUND
}

// The import is also what a title finds by ordinal (XexGetProcedureAddress).
PPC_FUNC(__imp__XamGetServiceEndpoint);
namespace
{
    kernel::DynamicExportRegistrar g_getServiceEndpoint("xam.xex", 1399, __imp__XamGetServiceEndpoint);
}
