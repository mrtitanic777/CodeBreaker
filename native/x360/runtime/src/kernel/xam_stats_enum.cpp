// The stats enumerators: XamUserCreateStatsEnumerator, behind XUserCreateStatsEnumeratorByRank / ByXuid / ByRating (the
// XDK's xapilib calls it with kind 1 / 0 / 3). See kernel/xam_stats.h.

#include "kernel/enumerator.h"
#include "kernel/kernel.h"
#include "kernel/overlapped.h"
#include "kernel/xam_stats.h"

#include <memory>

namespace
{
    constexpr uint32_t kErrorInvalidParameter = 0x57;
}

// DWORD XamUserCreateStatsEnumerator(DWORD TitleId, DWORD Kind, ULONGLONG Pivot, DWORD NumRows, DWORD NumSpecs,
//                                    XUSER_STATS_SPEC* Specs, PDWORD pcbBuffer, PHANDLE phEnum) (XAM 0x816702D8)
// Kind 0: rows around the XUID Pivot, 1: from the rank Pivot, 3: from the first row at or below the rating Pivot. XAM
// checks the specs and rows (1-100) and that the output pointers are not NULL, makes the enumerator (app 0xFB, messages
// 0xB0023 next / 0xB0024 close, NumRows items) and answers the buffer size XEnumerate needs for a page.
GUEST_IMPORT(XamUserCreateStatsEnumerator)
{
    auto& mem = guest::g_memory;
    const uint32_t titleId = ctx.r3.u32, kind = ctx.r4.u32, rows = ctx.r6.u32, specCount = ctx.r7.u32, specsAt = ctx.r8.u32,
                   sizePtr = ctx.r9.u32, handlePtr = ctx.r10.u32;
    const uint64_t pivot = ctx.r5.u64;
    if (specCount == 0 || specCount > kernel::stats::kViewsMax || specsAt == 0 || rows == 0 || rows > kernel::stats::kRowsMax ||
        sizePtr == 0 || handlePtr == 0 || (kind != 0 && kind != 1 && kind != 3))
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    std::vector<kernel::stats::Spec> specs = kernel::stats::ReadSpecs(specsAt, specCount);
    mem.Write32(sizePtr, kernel::stats::ResultsSize(rows, specs));
    auto state = std::make_shared<kernel::stats::Enumerator>(titleId, kind, pivot, rows, std::move(specs));
    kernel::EnumeratorInfo info;
    info.appId = 0xFB;
    info.message = 0xB0023;
    info.closeMessage = 0xB0024;
    info.userIndex = 0xFE;
    info.itemsPerEnumerate = rows;
    info.produce = [state](uint32_t buffer, uint32_t bufferSize, uint32_t& count) { return state->Next(buffer, bufferSize, count); };
    mem.Write32(handlePtr, kernel::CreateHostEnumerator(info, 0, {}));
    ctx.r3.u64 = kernel::kErrorSuccess;
}
