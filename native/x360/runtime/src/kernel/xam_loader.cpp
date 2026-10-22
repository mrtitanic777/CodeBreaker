// XAM loader exports: launch data passed between titles (by the dashboard, invites, or the previous title).
// A fresh boot has no launch data.

#include "kernel/stfs_mount.h"
#include "kernel/kernel.h"
#include "modules.h"

#include <windows.h>
#include "kernel/overlapped.h"
#include "kernel/users.h"
#include "settings.h"
#include "xex.h"

#include "transition/transition.h"

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <mutex>
#include <string>
#include <chrono>
#include <vector>

namespace
{
    constexpr uint32_t kErrorSuccess = 0;
    constexpr uint32_t kErrorInvalidParameter = 0x57;
    constexpr uint32_t kErrorNotFound = 0x490;

    std::mutex g_mutex;
    std::vector<uint8_t> g_launchData;

    // Launch data handed to a relaunched dashboard (a new process): written before it starts, read once.
    std::filesystem::path LaunchDataFile()
    {
        return DataDirectory() / "launch_data.bin";
    }
}

void kernel::LoadRelaunchData()
{
    if (!std::getenv("XBOX360RECOMP_DASH_RELAUNCH"))
        return;
    std::error_code ec;
    const auto path = LaunchDataFile();
    if (!std::filesystem::is_regular_file(path, ec))
        return; // a game going back to the dashboard: no launch data
    std::vector<uint8_t> data(size_t(std::filesystem::file_size(path, ec)));
    if (FILE* f = _wfopen(path.c_str(), L"rb"))
    {
        data.resize(std::fread(data.data(), 1, data.size(), f));
        std::fclose(f);
    }
    std::filesystem::remove(path, ec);
    {
        std::lock_guard lock(g_mutex);
        g_launchData = std::move(data);
        Log("Loader: relaunched dashboard with {} bytes of launch data", g_launchData.size());
    }
}

void kernel::RestoreRelaunchSignIns()
{
    // After dash.xex is the title (signing in reads its SPA for presence), before its main thread runs: the
    // profiles signed in when the previous dashboard relaunched itself or launched the game now coming back.
    if (!std::getenv("XBOX360RECOMP_DASH_RELAUNCH"))
        return;
    if (const size_t n = kernel::users::RestoreSignIns())
        Log("Loader: {} profile(s) still signed in from before (relaunch / back from a game)", n);
}

// DWORD XamLoaderGetLaunchDataSize(PDWORD Size)
GUEST_IMPORT(XamLoaderGetLaunchDataSize)
{
    const uint32_t sizePtr = ctx.r3.u32;
    if (sizePtr == 0)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    std::lock_guard lock(g_mutex);
    guest::g_memory.Write32(sizePtr, uint32_t(g_launchData.size()));
    ctx.r3.u64 = g_launchData.empty() ? kErrorNotFound : kErrorSuccess;
}

// DWORD XamLoaderGetLaunchData(PVOID Buffer, DWORD BufferSize)
GUEST_IMPORT(XamLoaderGetLaunchData)
{
    std::lock_guard lock(g_mutex);
    if (g_launchData.empty())
    {
        ctx.r3.u64 = kErrorNotFound;
        return;
    }
    const uint32_t copy = std::min<uint32_t>(uint32_t(g_launchData.size()), ctx.r4.u32);
    memcpy(guest::g_memory.Translate(ctx.r3.u32), g_launchData.data(), copy);
    ctx.r3.u64 = kErrorSuccess;
}

// DWORD XamLoaderSetLaunchData(PVOID Data, DWORD Size)
GUEST_IMPORT(XamLoaderSetLaunchData)
{
    std::lock_guard lock(g_mutex);
    const uint8_t* data = guest::g_memory.Translate(ctx.r3.u32);
    g_launchData.assign(data, data + ctx.r4.u32);
    ctx.r3.u64 = kErrorSuccess;
}

// VOID XamLoaderRebootToDash(PVOID Request) (XAM 0x816A1330): Request is 0x3FC bytes (+4 the dashboard command, +8 the
// user index, then command data; hud.xex 0x913E9378 fills it for the Guide's dashboard items). XAM stores it as the
// launch data, reads the user's controller and, when the dash is the title, its context is not 5 and the right
// trigger is not held, broadcasts XN 0x80040016 (user << 16 | command): the dash navigates in place. Otherwise it
// launches the dash (XamLoaderLaunchTitleEx(NULL, NULL, NULL, 0x1000 with the trigger held, 0x100000 for command
// 0x48, else 0)): not implemented (stops). The right-trigger reboot is not read here.
void kernel::RebootToDash(const std::vector<uint8_t>& request)
{
    auto be32 = [&request](size_t at) {
        return uint32_t(request[at]) << 24 | uint32_t(request[at + 1]) << 16 | uint32_t(request[at + 2]) << 8 |
            request[at + 3];
    };
    const uint32_t command = be32(4), user = be32(8);
    {
        std::lock_guard lock(g_mutex);
        g_launchData = request;
        g_launchData.resize(0x3FC);
    }
    if (kernel::TitleId() != 0xFFFE07D1 || kernel::DashContext() == 5)
        Fatal("XamLoaderRebootToDash(command {:X}, user {}): launching the dashboard as a new title (title {:08X}, dash "
            "context {}) is not implemented", command, user, kernel::TitleId(), kernel::DashContext());
    Log("XamLoaderRebootToDash(command {:X}, user {}): the dash navigates (notification 80040016)", command, user);
    kernel::BroadcastNotification(0x80040016, (user & 0xFFFF) << 16 | (command & 0xFFFF));
}

// The dashboard starts again as a new process with the launch data (`launchData`, else what XamLoaderSetLaunchData
// set), the sign-ins kept (signed_in.txt); this one exits. XamLoaderLaunchTitleEx(NULL) and signin.xex's network test.
[[noreturn]] void kernel::RelaunchDashboard(const std::vector<uint8_t>* launchData)
{
    {
        std::lock_guard lock(g_mutex);
        if (launchData)
        {
            g_launchData = *launchData;
            g_launchData.resize(0x3FC);
        }
        if (FILE* f = _wfopen(LaunchDataFile().c_str(), L"wb"))
        {
            std::fwrite(g_launchData.data(), 1, g_launchData.size(), f);
            std::fclose(f);
        }
        Log("Loader: relaunching the dashboard ({} bytes of launch data)", g_launchData.size());
    }
    kernel::users::SaveSignIns();
    wchar_t self[MAX_PATH];
    GetModuleFileNameW(nullptr, self, MAX_PATH);
    SetEnvironmentVariableW(L"XBOX360RECOMP_DASH_RELAUNCH", L"1");
    if (const char* script = std::getenv("CB_INPUT_SCRIPT_RELAUNCH"))
    {
        SetEnvironmentVariableA("CB_INPUT_SCRIPT", script);
        SetEnvironmentVariableA("CB_INPUT_SCRIPT_RELAUNCH", nullptr);
    }
    wchar_t directory[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, directory);
    // Seamless (native/x360/system/transition): black over the screen until the new dashboard shows its first frame.
    transition::CoverScreen();
    uint32_t pid = 0, error = 0;
    const auto result = transition::Launch(self, directory, 0, std::chrono::seconds(30), &pid, &error,
                                           GetCommandLineW());
    if (result == transition::Result::kFailed)
        Fatal("XamLoaderLaunchTitleEx: cannot start the dashboard again (Windows error {})", error);
    Log("XamLoaderLaunchTitleEx: the dashboard continues as process {}", pid);
    kernel::stfs_mount::WaitForAllWrites(); // content packages still being written back
    logging::Flush();
    ExitProcess(0);
}

// VOID XamLoaderLaunchTitleEx(LPCSTR ImagePath, LPCSTR DDrivePath, LPCSTR CommandLine, DWORD Flags) (XAM 0x816A0D70):
// ImagePath NULL (or Flags 2) launches the default title, the dashboard (loader request 10), keeping the launch data
// the caller set (XamLoaderSetLaunchData); the current title ends. The dash does this to reopen itself on a page
// (e.g. its Friends tile: launch data {.., +8 user}, export 0x92189BE8). Here the dashboard is a process: a new one
// starts (no boot animation: a title launch, not a power-on) with the launch data in launch_data.bin, and this one
// exits. Launching another image by path is not implemented (games start through XamContentLaunchImageInternalEx).
// Tests: CB_INPUT_SCRIPT_RELAUNCH becomes the new process's CB_INPUT_SCRIPT.
GUEST_IMPORT(XamLoaderLaunchTitleEx)
{
    auto& mem = guest::g_memory;
    const uint32_t imagePath = ctx.r3.u32, flags = ctx.r6.u32;
    if (imagePath != 0 && !(flags & 2))
        Fatal("XamLoaderLaunchTitleEx(\"{}\", flags {:X}): launching an image is not implemented",
              reinterpret_cast<const char*>(mem.Translate(imagePath)), flags);
    if (flags & ~0x2u)
        Fatal("XamLoaderLaunchTitleEx(NULL, flags {:X}): flags other than 2 are not implemented", flags);
    kernel::RelaunchDashboard(nullptr);
}

GUEST_IMPORT(XamLoaderRebootToDash)
{
    const uint8_t* data = guest::g_memory.Translate(ctx.r3.u32);
    kernel::RebootToDash(std::vector<uint8_t>(data, data + 0x3FC));
}

// VOID XamLoaderGetMediaInfo(PDWORD MediaType, PDWORD TitleId) (XAM 0x8169CF20): the disc in the drive, from XAM's
// DVD detection (media type 0 none, 1 game disc, 2 original Xbox disc, 4 audio, 5 video...; the title ID only for
// 1 and 2). This console's drive is empty: 0 and 0.
GUEST_IMPORT(XamLoaderGetMediaInfo)
{
    auto& mem = guest::g_memory;
    if (ctx.r3.u32)
        mem.Write32(ctx.r3.u32, 0);
    if (ctx.r4.u32)
        mem.Write32(ctx.r4.u32, 0);
}

// DWORD XamLoaderGetDvdTrayState() (XAM 0x8169CEB0, through its loader object): the DVD tray. Values as the dash
// names them (0x9225CE20, dashStrings): 1 "Closing", 2 "Close Tray" (open), 3 "Opening", 4 "Reading"; anything else,
// 0, is closed (the dash then reads the drive's sense data: kernel/xboxkrnl_device.cpp). Xenia's enum (0 open, 2
// closed) does not match the dash. The tray follows HalOpenCloseODDTray (kernel::DvdTrayState); it starts closed.
GUEST_IMPORT(XamLoaderGetDvdTrayState)
{
    ctx.r3.u64 = kernel::DvdTrayState();
}

// BOOL XamIsCurrentTitleDash(void): XAM 17559 (0x816E0C68) compares the running title's ID (0 while no title is in
// the running state) with the dashboard's, FFFE07D1.
GUEST_IMPORT(XamIsCurrentTitleDash)
{
    ctx.r3.u64 = kernel::TitleId() == 0xFFFE07D1 ? 1 : 0;
}

// DWORD XamLaunchURI(DWORD UserIndex, LPCSTR Uri, DWORD Flags, PXOVERLAPPED Overlapped) (XAM 0x816FC4B0): copies the
// URI (under 0x3FC characters) and posts {Flags, UserIndex, Uri} to XAM's app (0xFE message 0x22003); with an
// overlapped it returns 0x8000000A and completes it when handled (the dash's export 10666 waits on it). The handler
// (0x819A06D0) resolves the URI (0x819A0000, up to 10 rewrites) until its scheme is title / app / game / sap:
// XAM's built-in schemes (table 0x81637700: dash FFFE07D1, avatareditor, zune, newlivesignup -> "title:%08X:%s" /
// "app:%08X:%s"; titles need the caller's trust, Flags bit 31, or a rewrite that granted it) and its scheme templates
// (providers 0x8163773C: on Xbox LIVE (XLiveBase 0x58003 connected) the LiveHive value named by the scheme, for a
// trusted caller only (0x8199F1F0); then table 0x81637748 {scheme, open to anyone, offline template, online template},
// the online template on LIVE when there is one, else the offline one (0x8199FEE8); none: E_INVALIDARG). E.g.
// "gameslibrary:_RootTitle=My Games;" -> contentlibrary:PackageType=1;... -> hub:root:BuiltIn.ContentApp.xzp:... ->
// dash:pam:BuiltIn.Hub.xzp:... -> title:FFFE07D1:pam:BuiltIn.Hub.xzp:root:BuiltIn.ContentApp.xzp:contentlibrary:....
// A "title:<ID>:<rest>" target: XAM first looks for a handler of that title (table 0x81637968: FFFE07D1 dash ->
// 0x819A0440, 584D07D1 avatar editor, 5848085B Zune, FFFE07DE LIVE signup); without one it launches the title from
// the disc or its installed content, or (Flags bit 0) asks the dash to acquire it (0x819A05B8, 0x80040022 ->
// "pam:Dash.MP.Purchase.lex:acquire:<URI>", the store). The dash's handler (0x819A0440): launch data {0, 0xC,
// UserIndex, <rest>} (0x3FC bytes, the XamLoaderRebootToDash layout); with Flags bit 2 and the dash running,
// XNotifyBroadcast(0x80040021, UserIndex << 16 | 0xC), which the dash handles by navigating to <rest> (0x92186xxx case
// 0xB: XamLoaderGetLaunchData, 0x921905B8), else XamLoaderRebootToDash (not implemented). Other titles' handlers,
// launches and acquisitions, app: and sap: targets stop (not implemented).
namespace
{
    // Whether a URI template names a dashboard module (a ":<name>.lex:" / ".xex" part) that this console does not
    // have: the Dash.MP.* modules come from Xbox LIVE (XdfLoadXex), so an online template that needs one (My Games'
    // "contentlibrary:" -> Dash.MP.ContentExplorer.lex) cannot open here.
    bool NeedsMissingModule(const std::string& format)
    {
        size_t at = 0;
        while (at <= format.size())
        {
            size_t end = format.find(':', at);
            if (end == std::string::npos)
                end = format.size();
            const std::string part = format.substr(at, end - at);
            at = end + 1;
            std::string lower = part;
            for (char& c : lower)
                c = char(std::tolower(uint8_t(c)));
            if ((lower.ends_with(".lex") || lower.ends_with(".xex")) && part.find_first_of("\\/") == std::string::npos &&
                !std::filesystem::exists(DataDirectory() / "SystemUpdate" / "$SystemUpdate" / part) &&
                !kernel::HasRecompiledSystemModule(part + ".xex"))
                return true;
        }
        return false;
    }
}

GUEST_IMPORT(XamLaunchURI)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, uriPtr = ctx.r4.u32, flags = ctx.r5.u32, overlapped = ctx.r6.u32;
    constexpr uint32_t kHrInvalidArg = 0x80070057, kHrAccessDenied = 0x80070005, kHrPending = 0x8000000A;
    if (uriPtr == 0 || mem.Read8(uriPtr) == 0)
    {
        ctx.r3.u64 = kHrInvalidArg;
        return;
    }
    std::string uri(reinterpret_cast<const char*>(mem.Translate(uriPtr)));
    if (uri.size() + 1 >= 0x3FC)
    {
        ctx.r3.u64 = 0x8007007A;
        return;
    }

    const xex::HostImage& xam = kernel::XamImage();
    auto cString = [&xam](uint32_t address) -> std::string {
        if (address == 0)
            return {};
        const uint8_t* p = xam.At(address, 1);
        if (p == nullptr)
            Fatal("XamLaunchURI: XAM string {:08X} lies outside xam.xex", address);
        return reinterpret_cast<const char*>(p);
    };
    auto be32 = [](const uint8_t* p) { return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3]; };
    auto equalNoCase = [](const std::string& a, const std::string& b) { return _stricmp(a.c_str(), b.c_str()) == 0; };

    bool trusted = (flags >> 31) != 0;
    uint32_t hr = 0;
    std::string scheme, rest;
    bool resolved = false;
    for (int step = 0; step < 10 && hr == 0 && !resolved; step++)
    {
        const size_t colon = uri.find(':');
        scheme = uri.substr(0, colon);
        rest = colon == std::string::npos ? std::string() : uri.substr(colon + 1);
        if (scheme.empty())
        {
            hr = kHrInvalidArg;
            break;
        }
        if (equalNoCase(scheme, "title") || equalNoCase(scheme, "app") || equalNoCase(scheme, "game") ||
            equalNoCase(scheme, "sap"))
        {
            resolved = true;
            break;
        }
        bool found = false;
        for (uint32_t row = 0x81637700;; row += 12)
        {
            const uint8_t* r = xam.At(row, 12);
            if (r == nullptr || be32(r) == 0)
                break;
            if (!equalNoCase(scheme, cString(be32(r))))
                continue;
            const uint32_t title = be32(r + 4), kind = be32(r + 8);
            if (kind < 3 && !trusted)
                hr = kHrAccessDenied;
            else if (kind < 3)
                uri = std::format("title:{:08X}:{}", title, rest);
            else if (kind == 3)
                uri = std::format("app:{:08X}:{}", title, rest);
            else
                hr = 0x80004005;
            found = true;
            break;
        }
        if (found)
            continue;
        const bool online = kernel::ConsoleOnLive();
        if (online)
            if (const auto hive = kernel::LiveHiveValue(scheme))
            {
                if (!trusted)
                    hr = kHrAccessDenied;
                else
                {
                    const size_t at = hive->find("%s");
                    uri = at == std::string::npos ? *hive : hive->substr(0, at) + rest + hive->substr(at + 2);
                }
                continue;
            }
        for (uint32_t row = 0x81637748;; row += 16)
        {
            const uint8_t* r = xam.At(row, 16);
            if (r == nullptr || be32(r) == 0)
                break;
            if (!equalNoCase(scheme, cString(be32(r))))
                continue;
            if (!trusted && be32(r + 4) == 0)
            {
                hr = kHrAccessDenied;
                found = true;
                break;
            }
            trusted = true;
            std::string onlineFormat = online ? cString(be32(r + 12)) : std::string();
            const std::string offlineFormat = cString(be32(r + 8));
            // PC port: an online page whose module never came from Xbox LIVE opens the offline one (owner: My
            // Games online gave "temporarily unavailable"); without an offline template it stays as on a console.
            if (!onlineFormat.empty() && !offlineFormat.empty() && NeedsMissingModule(onlineFormat))
            {
                Log("XamLaunchURI: {}: the online page ({}) needs an Xbox LIVE module this console does not have; "
                    "the offline one", scheme, onlineFormat);
                onlineFormat.clear();
            }
            const std::string format = onlineFormat.empty() ? offlineFormat : onlineFormat;
            if (!format.empty())
            {
                const size_t at = format.find("%s");
                uri = at == std::string::npos ? format : format.substr(0, at) + rest + format.substr(at + 2);
                found = true;
            }
            break;
        }
        if (!found && hr == 0)
            hr = kHrInvalidArg;
    }
    if (hr == 0 && !resolved)
        hr = kHrInvalidArg;

    if (hr == 0)
    {
        const size_t colon = rest.find(':');
        const uint32_t title = equalNoCase(scheme, "title") && colon == 8 ?
            uint32_t(std::strtoul(rest.substr(0, 8).c_str(), nullptr, 16)) : 0;
        const std::string original(reinterpret_cast<const char*>(mem.Translate(uriPtr)));
        if (title != 0xFFFE07D1 || !(flags & 4) || kernel::TitleId() != 0xFFFE07D1)
            Fatal("XamLaunchURI(\"{}\", flags {:08X}): resolved to \"{}:{}\"; only dash URIs while the dash runs are "
                "implemented", original, flags, scheme, rest);
        const std::string target = rest.substr(9);
        Log("XamLaunchURI(user {}, \"{}\"): the dash navigates to \"{}\" (launch data {{0, 0xC, {}, URI}}, "
            "notification 80040021)", user, original, target, user);
        {
            std::lock_guard lock(g_mutex);
            g_launchData.assign(0x3FC, 0);
            const uint32_t header[3] = { 0, 0xC, user };
            for (int k = 0; k < 3; k++)
                for (int b = 0; b < 4; b++)
                    g_launchData[k * 4 + b] = uint8_t(header[k] >> (24 - 8 * b));
            std::memcpy(g_launchData.data() + 12, target.data(), std::min<size_t>(target.size(), 0x3EF));
        }
        kernel::BroadcastNotification(0x80040021, (user & 0xFFFF) << 16 | 0xC);
    }
    else
        Log("XamLaunchURI(user {}, \"{}\"): {:08X}", user, reinterpret_cast<const char*>(mem.Translate(uriPtr)), hr);

    if (overlapped)
    {
        kernel::CompleteOverlapped(overlapped, int32_t(hr) < 0 ? 0x65B : 0, hr, 0);
        ctx.r3.u64 = kHrPending;
        return;
    }
    ctx.r3.u64 = hr;
}

// HRESULT XdfLoadXex(LPCSTR Name, PVOID Context, ?, DWORD Flags, POVERLAPPED, PHANDLE Module) (XAM 0x817F6980):
// loads a dashboard feature module (a plugin .xex / .lex) through XAM's XDF cache. The system update carries only some
// (Dash.Kiosk.lex, ...); the others (Dash.Search.lex: the "bing" hub; the Dash.MP.* marketplace modules) come from
// Xbox LIVE into SysAux. One that is recompiled (prepare_module.py --name <Name>: SystemRecomp, converted/<Name>.xex)
// loads here, synchronously (the dash also takes 0x8B050005 = pending, then waits). A module not there answers
// 0x80070002 (the error XAM returns for a missing module is not traced). Others that exist stop: not recompiled.
GUEST_IMPORT(XdfLoadXex)
{
    const uint32_t path = ctx.r3.u32;
    const std::string name = path ? reinterpret_cast<const char*>(guest::g_memory.Translate(path)) : "";
    // A .xex keeps its name (Dash.Search.xex); a .lex was converted as <name>.xex (Dash.Search.lex.xex).
    const std::string recompiled = kernel::HasRecompiledSystemModule(name) ? name : name + ".xex";
    if (!name.empty() && kernel::HasRecompiledSystemModule(recompiled))
    {
        const uint32_t out = ctx.r8.u32;
        PPCContext call = ctx;
        const modules::LoadedModule* module = modules::FindByName(recompiled);
        if (!module)
            module = kernel::LoadRecompiledSystemModule(recompiled, call);
        if (module)
        {
            if (out)
                guest::g_memory.Write32(out, module->handle);
            Log("XdfLoadXex(\"{}\"): recompiled module, handle {:08X}", name, module->handle);
            ctx.r3.u64 = 0;
            return;
        }
    }
    const std::filesystem::path file = DataDirectory() / "SystemUpdate" / "$SystemUpdate" / name;
    if (name.empty() || name.find_first_of("\\/:") != std::string::npos || std::filesystem::exists(file))
        Fatal("XdfLoadXex(\"{}\", {:08X}, {:08X}, {:X}): dashboard plugin modules are not recompiled", name, ctx.r4.u32,
            ctx.r5.u32, ctx.r6.u32);
    Log("XdfLoadXex(\"{}\"): not in the system update (an Xbox LIVE module): 80070002", name);
    if (ctx.r8.u32)
        guest::g_memory.Write32(ctx.r8.u32, 0);
    ctx.r3.u64 = 0x80070002;
}

// HRESULT XdfLoadXexFromCache(LPCSTR Name, DWORD Flags, DWORD Version, PHANDLE Module) (XAM 0x817F5968): loads a
// system module from XAM's XDF cache synchronously (the dash: dashnui.xex, its Kinect layer, when the Kinect experience
// is installed). A recompiled module loads (XexLoadImage's path); one that is not there answers 0x80070002.
GUEST_IMPORT(XdfLoadXexFromCache)
{
    const std::string name =
        ctx.r3.u32 ? std::string(reinterpret_cast<const char*>(guest::g_memory.Translate(ctx.r3.u32))) : std::string();
    const uint32_t out = ctx.r6.u32;
    if (name.empty() || out == 0 || ctx.r4.u32 == 0)
    {
        ctx.r3.u64 = 0x80070057;
        return;
    }
    guest::g_memory.Write32(out, 0);
    const std::string recompiled = kernel::HasRecompiledSystemModule(name) ? name : name + ".xex";
    if (kernel::HasRecompiledSystemModule(recompiled))
    {
        PPCContext call = ctx;
        const modules::LoadedModule* module = modules::FindByName(recompiled);
        if (!module)
            module = kernel::LoadRecompiledSystemModule(recompiled, call);
        if (module)
        {
            guest::g_memory.Write32(out, module->handle);
            Log("XdfLoadXexFromCache(\"{}\"): recompiled module, handle {:08X}", name, module->handle);
            ctx.r3.u64 = 0;
            return;
        }
    }
    Log("XdfLoadXexFromCache(\"{}\"): not recompiled: 80070002", name);
    ctx.r3.u64 = 0x80070002;
}

// BOOL XamIsSystemTitleId(DWORD TitleId) (XAM 0x816E0D60): 0, Microsoft's system IDs (0x58xxxxxx other than the
// 0x5841xxxx Xbox LIVE Arcade range) and 0xFFFExxxx.
GUEST_IMPORT(XamIsSystemTitleId)
{
    const uint32_t id = ctx.r3.u32;
    ctx.r3.u64 = id == 0 || ((id & 0xFF000000) == 0x58000000 && (id & 0xFF0000) != 0x410000) ||
        (id & 0xFFFF0000) == 0xFFFE0000 ? 1 : 0;
}

// BOOL XamIsSystemExperienceTitleId(DWORD TitleId) (XAM 0x816E0DA8): 0x584Axxxx, 0x5848xxxx, 584E07D1, 584E07D2.
GUEST_IMPORT(XamIsSystemExperienceTitleId)
{
    const uint32_t id = ctx.r3.u32;
    ctx.r3.u64 = (id >> 16) == 0x584A || (id >> 16) == 0x5848 || id == 0x584E07D2 || id == 0x584E07D1 ? 1 : 0;
}

// BOOL XamIsXbox1TitleId(DWORD TitleId) (XAM 0x816E0DE8): original Xbox titles: FFFE0000, or an ID whose low 15
// bits are under 2000 (0 excluded).
GUEST_IMPORT(XamIsXbox1TitleId)
{
    const uint32_t id = ctx.r3.u32;
    ctx.r3.u64 = id == 0xFFFE0000 || (id != 0 && (id & 0x7FFF) < 2000) ? 1 : 0;
}
