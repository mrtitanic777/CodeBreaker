// Title servers (XLSP), the XAM side: the enumerators a title makes with XamCreateEnumeratorHandle for
// XTitleServerCreateEnumerator (Minecraft makes one when it signs in to Xbox LIVE).
// For now an enumeration lists no title servers (ERROR_NO_MORE_FILES), as for a title that has none: the Dashboard's
// runtime (Recompiled kernel/xam_titleserver*.cpp, with live/titleserver.h) answers it from our server and replaces this
// when it is ported here.

#include "kernel/kernel.h"
#include "kernel/objects.h"
#include "log.h"
#include "memory.h"

#include <cstring>
#include <mutex>
#include <set>

namespace
{
    // XTitleServerCreateEnumerator's enumerator: app 0xFC (XLIVEBASE), messages 0x58039 / 0x5803A, 0xD4 bytes of
    // private data (Minecraft's call: r4 FC, r5 58039, r6 5803A, r7 D4).
    constexpr uint32_t kAppXLive = 0xFC, kEnumerateMessage = 0x58039, kCloseMessage = 0x5803A, kPrivateSize = 0xD4;
    constexpr uint32_t kErrorNoMoreFiles = 0x12;
    constexpr uint32_t kBodySize = 0x18;
    constexpr uint32_t kErrorInvalidHandle = 6, kErrorInvalidParameter = 0x57;

    std::mutex g_mutex;
    std::set<uint32_t> g_enumerators; // objects made by XamCreateEnumeratorHandle for title servers
}

// DWORD XamCreateEnumeratorHandle(DWORD UserIndex, DWORD AppId, DWORD EnumerateMessage, DWORD CloseMessage, DWORD
// ExtraSize, DWORD ItemsPerEnumerate, DWORD Flags, PHANDLE Handle): an enumerator object (X_KENUMERATOR: app, enumerate
// message, close message, user index, items per enumerate, flags; 0x18 bytes) followed by ExtraSize bytes of private
// data that the creating library fills. Only the XLSP enumerator (app 0xFC, message 0x58039) is served here: the others
// are answered by XAM apps this runtime does not have.
GUEST_IMPORT(XamCreateEnumeratorHandle)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, app = ctx.r4.u32, message = ctx.r5.u32, close = ctx.r6.u32, extra = ctx.r7.u32,
                   items = ctx.r8.u32, flags = ctx.r9.u32, handlePtr = ctx.r10.u32;
    if (app != kAppXLive || message != kEnumerateMessage ||
        close != kCloseMessage)
        Fatal("XamCreateEnumeratorHandle(user {:X}, app {:02X}, message {:08X}, close {:08X}): enumerators served by XAM "
              "apps are not implemented", user, app, message, close);
    if (handlePtr == 0 || extra != kPrivateSize || items == 0)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    const uint32_t object = kernel::CreateOwnedObject(kBodySize + extra);
    std::memset(mem.Translate(object), 0, kBodySize + extra);
    mem.Write32(object + 0x00, app);
    mem.Write32(object + 0x04, message);
    mem.Write32(object + 0x08, close);
    mem.Write32(object + 0x0C, user);
    mem.Write32(object + 0x10, items);
    mem.Write32(object + 0x14, flags);
    {
        std::lock_guard lock(g_mutex);
        g_enumerators.insert(object);
    }
    kernel::SetObjectCleanup(object, [object] {
        std::lock_guard lock(g_mutex);
        g_enumerators.erase(object);
    });
    mem.Write32(handlePtr, kernel::CreateHandle(object));
    ctx.r3.u64 = 0;
}

// DWORD XamGetPrivateEnumStructureFromHandle(HANDLE Handle, PVOID* Object): the enumerator object, referenced (the
// caller releases it with ObDereferenceObject); its private data is at +0x18.
GUEST_IMPORT(XamGetPrivateEnumStructureFromHandle)
{
    const uint32_t object = kernel::LookupHandle(ctx.r3.u32);
    bool ours;
    {
        std::lock_guard lock(g_mutex);
        ours = object != 0 && g_enumerators.count(object) != 0;
    }
    if (!ours || ctx.r4.u32 == 0)
    {
        ctx.r3.u64 = kErrorInvalidHandle;
        return;
    }
    kernel::ReferenceObject(object);
    guest::g_memory.Write32(ctx.r4.u32, object);
    ctx.r3.u64 = 0;
}

// XamEnumerate on an XLSP enumerator (kernel/xam_enum.cpp): false when `object` is not one.
namespace kernel
{
    bool TitleServerEnumerate(uint32_t object, uint32_t buffer, uint32_t bufferSize, uint32_t& written, uint32_t& error)
    {
        {
            std::lock_guard lock(g_mutex);
            if (!g_enumerators.count(object))
                return false;
        }
        (void)buffer;
        (void)bufferSize;
        written = 0;
        error = kErrorNoMoreFiles;
        return true;
    }
}
