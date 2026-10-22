// XSession handles. A title's XSessionCreate asks XAM for a session handle and the object behind it, then drives the
// session (join, start, end, stats) through XMsg requests to the LIVE app. A console without Xbox LIVE still has
// sessions (local ones, for presence and leaderboards written later), so the handle and its object exist; the object
// is a signalled notification event: nothing waits on a session here.

#include "kernel/kernel.h"
#include "kernel/objects.h"
#include "kernel/overlapped.h"

namespace
{
    constexpr uint32_t kSessionObjectSize = 0x40;
    constexpr uint32_t kErrorInvalidHandle = 6, kErrorInvalidParameter = 0x57;
}

// DWORD XamSessionCreateHandle(PHANDLE SessionHandle)
GUEST_IMPORT(XamSessionCreateHandle)
{
    auto& mem = guest::g_memory;
    const uint32_t handlePtr = ctx.r3.u32;
    if (handlePtr == 0)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    const uint32_t object = kernel::CreateOwnedObject(kSessionObjectSize);
    mem.Write8(object, kernel::kObjectEventNotification);   // DISPATCHER_HEADER.Type
    mem.Write32(object + kernel::header::kSignalState, 1);
    const uint32_t handle = kernel::CreateHandle(object);
    mem.Write32(handlePtr, handle);
    ctx.r3.u64 = kernel::kErrorSuccess;
}

// DWORD XamSessionRefObjByHandle(HANDLE SessionHandle, PVOID* Object): the object behind the handle, referenced
// (the title releases it with ObDereferenceObject).
GUEST_IMPORT(XamSessionRefObjByHandle)
{
    const uint32_t object = kernel::LookupHandle(ctx.r3.u32);
    if (object == 0 || ctx.r4.u32 == 0)
    {
        ctx.r3.u64 = object == 0 ? kErrorInvalidHandle : kErrorInvalidParameter;
        return;
    }
    kernel::ReferenceObject(object);
    guest::g_memory.Write32(ctx.r4.u32, object);
    ctx.r3.u64 = kernel::kErrorSuccess;
}
