// XAM functions that only the older dashboards import (the Blades devkit dashboards 1888 ... 6719 of Dashboard
// Versions, booted with CB_DASH=<version>): ordinals the 17559 XAM no longer exports, or exports under another name.
// What each does is worked out from the dash's own calls and, where the 17559 XAM still has it, from its code.

#include "kernel/kernel.h"
#include "kernel/spa.h"
#include "settings.h"

#include <format>
#include <string>

namespace
{
    std::string g_olderDash;
}

void kernel::SetOlderDashVersion(const std::string& version)
{
    g_olderDash = version;
}

const std::string& kernel::OlderDashVersion()
{
    return g_olderDash;
}

// The system update whose modules surround an older dashboard (its boot animation, its Guide: Dashboard Versions/
// tools/make_era_modules.py rebuilds them as SystemUpdate/converted/<module>_<update>.xex): its own version's when
// the user's archive has that update, else (the devkit-only builds) the update nearest before it, the one whose XAM
// it was built against (make_xam_sections.py's NEAREST: 3519, the Burnout branch, takes 4532's).
std::string kernel::OlderDashSystemUpdate()
{
    if (g_olderDash.empty())
        return {};
    static constexpr std::pair<const char*, const char*> kNearest[] = {
        { "2832", "2258" }, { "2858", "2258" }, { "2863", "2258" }, { "3519", "4532" }, { "4022", "4532" },
        { "4308", "4532" }, { "5632", "5759" }, { "6686", "6683" }, { "6719", "6717" },
    };
    for (const auto& [devkit, update] : kNearest)
        if (g_olderDash == devkit)
            return update;
    return g_olderDash;
}

// An older dashboard's XAM section (SystemUpdate/converted/<section>_<version>.xzp): shrdres (from the version's own
// folder of the user's archive) and, before 6683 (which carries its gamer card in its own "gamercar" resource),
// gamercrd, XAM's gamer card scene of that time. The 2241 ... 5787 dashes draw their card (mottoText, countryText ...)
// from gamercrd, which 17559's XAM and 6683's card lack, so without it they fault at 0x8 after sign-in. The real
// section comes from each version's own XAM, rebuilt from the launch base (Dashboard Versions/tools/make_xam_sections.py).
std::filesystem::path kernel::OlderDashSection(const std::string& section)
{
    if (g_olderDash.empty() || (section != "shrdres" && section != "gamercrd"))
        return {};
    std::filesystem::path path = DataDirectory() / "SystemUpdate" / "converted" / (section + "_" + g_olderDash + ".xzp");
    std::error_code ec;
    return std::filesystem::is_regular_file(path, ec) ? path : std::filesystem::path();
}

// XLiveBase (app 0xFC) API messages are 0x58000 + an index into XAM's table, and the table grew between the
// versions. Each entry here is a dash function found in both dashboards (Dashboard Versions: dashvcs blame), with
// the message each version's copy sends:
//   XPresenceInitializeEx   6690 0x921A28E0, 6717 / 6719 0x921A2A20, 6770 0x921A2938 (the same 96 bytes),
//                           NXE 7258 0x921B2518, 7356 / 7357 0x921D29E0, 7363 / 7371 0x921D2630 (the same start)
//                           -> 0x58046        8495 0x921E9A70, 9199 0x921EB4A0, 17559 0x9224C0F0 -> 0x58056
uint32_t kernel::OlderDashMessage(uint32_t app, uint32_t message)
{
    struct Renumbered
    {
        const char* version;
        uint32_t app, older, current;
    };
    static constexpr Renumbered kMessages[] = {
        { "6690", 0xFC, 0x58046, 0x58056 },
        { "6717", 0xFC, 0x58046, 0x58056 },
        { "6719", 0xFC, 0x58046, 0x58056 },
        { "6770", 0xFC, 0x58046, 0x58056 },
        { "7258", 0xFC, 0x58046, 0x58056 },
        { "7356", 0xFC, 0x58046, 0x58056 },
        { "7357", 0xFC, 0x58046, 0x58056 },
        { "7363", 0xFC, 0x58046, 0x58056 },
        { "7371", 0xFC, 0x58046, 0x58056 },
    };
    if (g_olderDash.empty())
        return message;
    for (const Renumbered& m : kMessages)
        if (m.app == app && m.older == message && g_olderDash == m.version)
            return m.current;
    return message;
}

// xam ordinal 2790 (0xAE6, between CurlWipeBackingFiles 0xAE5 and CurlOpenTitleBackingFile 0xAE7): the 6719 dash
// calls it once at start-up (0x9210F1F0), its only argument the address of a table of seven of its own functions and
// a count, 0x18 (0x927FC914: 0x92587390 ...; each takes a critical section and works on files, CRC32 and
// NtQueryFullAttributesFile): it gives XAM's URL cache ("Curl") a backing store for the dash's content. Nothing reads
// its result. This XAM keeps no such cache, so nothing ever calls the table back: it is accepted and noted.
GUEST_IMPORT(xam_Ordinal_2790)
{
    Log("xam ordinal 2790 (the URL cache's backing store): table {:08X}, accepted; no cache calls it",
        ctx.r3.u32);
    ctx.r3.u64 = 0;
}

// DWORD XamGetCachedTitleName(DWORD TitleId, LPWSTR Buffer, PDWORD cch) (XAM 0x816A3D58 -> XamGetCachedTitleNameEx
// 0x816A3A80): XAM's title name cache. Title 0: an empty name, *cch = 1, ERROR_INVALID_PARAMETER. No cache:
// ERROR_OUTOFMEMORY (0xE). A title the cache does not hold yet: ERROR_IO_PENDING (0x3E5) while XAM fetches its
// title info from Xbox LIVE (XamCacheFetchFile 9; this console never gets it). A hit copies the name (up to 0x2E
// characters; *cch is left as given). The cache here holds the running title's own name (its SPA, string 0x8000), the 6719 dash asking
// for its own (FFFE07D1).
GUEST_IMPORT(XamGetCachedTitleName)
{
    auto& mem = guest::g_memory;
    const uint32_t title = ctx.r3.u32, buffer = ctx.r4.u32, cchPtr = ctx.r5.u32;
    if (title == 0)
    {
        mem.Write16(buffer, 0);
        mem.Write32(cchPtr, 1);
        ctx.r3.u64 = 0x57;
        return;
    }
    const std::u16string name = title == kernel::TitleId() ? kernel::spa::TitleName() : std::u16string();
    if (name.empty())
    {
        ctx.r3.u64 = 0x3E5;
        return;
    }
    const uint32_t cch = mem.Read32(cchPtr);
    if (cch <= name.size())
        Fatal("XamGetCachedTitleName({:08X}): the name has {} characters for a buffer of {}", title, name.size(), cch);
    for (size_t i = 0; i < name.size(); i++)
        mem.Write16(buffer + uint32_t(i) * 2, uint16_t(name[i]));
    mem.Write16(buffer + uint32_t(name.size()) * 2, 0);
    ctx.r3.u64 = 0;
}

// HRESULT XamBuildLegacySystemResourceLocator(LPCWSTR Resource, LPWSTR Buffer, DWORD cch) (ordinal 796): the 6719
// dash's "sharedres://<name>" URLs (0x9210DAA0). The 17559 XAM (0x816C8CB0) answers eight button pictures it keeps in
// memory (a_button.png ..., "memory://%X,%X") and every other name with XamBuildResourceLocator(XAM, "shrdres",
// Resource); here every name is the "shrdres" section locator (the same pictures, read from the section).
GUEST_IMPORT(XamBuildLegacySystemResourceLocator)
{
    auto& mem = guest::g_memory;
    const uint32_t buffer = ctx.r4.u32, cch = ctx.r5.u32;
    std::string resource;
    if (ctx.r3.u32)
        for (char16_t c : kernel::ReadGuestWideString(ctx.r3.u32, 0x400))
        {
            if (c > 0x7F)
                Fatal("XamBuildLegacySystemResourceLocator: non-ASCII character {:04X} in a resource name", uint32_t(c));
            resource += char(c);
        }
    const std::string text = std::format("section://{:X},shrdres#{}", kernel::SystemModuleHandle("xam.xex"), resource);
    if (buffer == 0 || cch == 0 || text.size() >= cch)
        Fatal("XamBuildLegacySystemResourceLocator: \"{}\" does not fit the buffer ({:08X}, {} characters)", text,
            buffer, cch);
    for (size_t i = 0; i < text.size(); i++)
        mem.Write16(buffer + uint32_t(i) * 2, uint8_t(text[i]));
    mem.Write16(buffer + uint32_t(text.size()) * 2, 0);
    ctx.r3.u64 = 0;
}

// xam ordinal 2055 (0x807): XuiClassDerivesFrom in 17559's XAM (the Guide modules import it so), but in 9199's
// (rebuilt from the user's system update, Dashboard Versions/tools/make_xam_sections.py) a getter without
// arguments: `lwz r3, [0x81BFEC04]; blr`, a XAM boolean whose setter (0x81940120) stores value > 0 and broadcasts the
// XAM-internal notification 0x80050019 (area 64: not one of the XDK's). 7357's XAM has no such export; of the
// dashboards only 9199 imports it (at start-up, 0x9214145C, keeping the answer in its object at +332). The state
// starts at 0 in that XAM's image and only XAM's own events set it, which this runtime does not have: 0.
GUEST_IMPORT(XuiClassDerivesFrom)
{
    if (kernel::OlderDashVersion() == "9199")
    {
        ctx.r3.u64 = 0;
        return;
    }
    // 17559's XuiClassDerivesFrom (the Guide modules') comes from the recompiled XAM (GUEST_IMPORT forwards first).
    kernel::UnimplementedImport("XuiClassDerivesFrom", ctx);
}

// BOOL XamFirstRunExperienceShouldRun(void) (ordinal 2001; the NXE dashboards). As 9199's XAM (0x81932230): it reads
// XConfig 10/2, the dashboard version whose first run the console has had (0 if never stamped: no first run). A
// stamp up to 2.0.7199.255 (0x201C1FFF), with a flag of XAM's (0x8193D788's bit 1) set, answers 1; up to
// 2.0.9049.255 (0x202359FF) XAM clears two USER_RETAIL_FLAGS bits (0x04000000, 0x08000000), stamps its own version and
// answers 0. Unstamped (this runtime's console) or newer: 0. The older stamps need XAM's flag, not known here: stop.
GUEST_IMPORT(XamFirstRunExperienceShouldRun)
{
    const uint32_t stamp = kernel::XConfigRead32(10, 2);
    if (stamp != 0 && stamp <= 0x202359FF)
        Fatal("XamFirstRunExperienceShouldRun: the console's first-run stamp {:08X} needs XAM's own first-run flag, "
              "which is not known here", stamp);
    ctx.r3.u64 = 0;
}

// void XamWebInstrumentationCreateSampledReport(LPCSTR Name, DWORD SampleRate, HANDLE* Report) (ordinal 2866; the NXE
// dashboards' usage reports to Microsoft's web instrumentation). 9199's XAM (0x81932BD0) makes a report and keeps it
// only when the request is sampled in (0x81932808 with the rate); otherwise it destroys it and *Report = NULL, which
// the dash handles. There is no instrumentation service here: every request is sampled out.
GUEST_IMPORT(XamWebInstrumentationCreateSampledReport)
{
    if (ctx.r5.u32)
        guest::g_memory.Write32(ctx.r5.u32, 0);
    ctx.r3.u64 = 0;
}
