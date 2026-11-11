// XAM exports TU46 looks up at run time (XAPI GetProcAddress and lazy-binding helpers) instead of importing.
// A console with the final dashboard provides all of them, so their lookups must succeed. Exports whose behaviour
// has not been established yet report the call when the title actually makes it.

#include "kernel/kernel.h"
#include "pool.h"

// PVOID XuiAlloc(SIZE_T Size): XAM's allocator for the XUI runtime.
GUEST_EXPORT("xam.xex", 873, XuiAlloc)
{
    ctx.r3.u64 = KernelPool().Allocate(ctx.r3.u32);
    if (ctx.r3.u32 == 0)
        Fatal("XuiAlloc({:X}): kernel pool exhausted", ctx.r3.u32);
}

GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 625, XamContentCreateInternal)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 633, XamContentAggregateCreateEnumerator)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 773, XamShowPartyUI)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 779, XamShowCommunitySessionsUI)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 792, XamVoiceGetMicArrayStatus)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 794, XamVoiceGetDirectionalData)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1399, XamGetServiceEndpoint)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 2827, XamPartySetCustomData)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 2832, XamPartyGetBandwidth)

// HRESULT XamVoiceSetMicArrayIdleUsers(DWORD IdleUserMask, BOOL Unknown): tells the Kinect microphone array which
// local talkers are idle. No Kinect is attached, so there is nothing to steer; XHV2's callers ignore the result.
GUEST_EXPORT("xam.xex", 1164, XamVoiceSetMicArrayIdleUsers)
{
    ctx.r3.u64 = 0; // S_OK
}
