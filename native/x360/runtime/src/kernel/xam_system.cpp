// XAM system services the dashboard uses (package manager, Kinect/NUI state, ...): what a console without Kinect,
// without Xbox LIVE and with the 17559 system update answers. References: XAM 17559's own code (its
// decompilation) and Nexia360 (BSD-3), which runs this dash.xex over an HLE XAM.

#include "kernel/kernel.h"
#include "kernel/overlapped.h"
#include "settings.h"
#include "xex.h"

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <mutex>
#include <string>

// HRESULT XamPackageManagerGetExperienceMode(DWORD* Mode). XAM asks its own app (0xFE, message 0x25005: its system
// content manager, FUN_816EA288, from the installed system packages). Bit 0x1 is the controller experience, bit 0x2
// the Kinect experience, installed on every console updated since 2010 whether or not a sensor is connected: with it
// the dash loads its Kinect layer (dashnui.xex, XdfLoadXexFromCache) and creates its hand-cursor objects, which the
// bing hub's search screen uses (Dash.Search.xex's scroller, dash 0x9278EA40); its Kinect hub still needs a working
// sensor (XamNuiIsDeviceReady). 3, as the user's console with its SysAux packages.
GUEST_IMPORT(XamPackageManagerGetExperienceMode)
{
    if (ctx.r3.u32)
        guest::g_memory.Write32(ctx.r3.u32, 3);
    ctx.r3.u64 = 0;
}

// DWORD XamUserFlushLogonQueue(PXOVERLAPPED Overlapped): waits until XAM's queued sign-ins/sign-outs are done. XAM
// queues a task behind them that completes the request with 0. Sign-ins here happen at once (kernel/users.h), so the
// queue is always empty: complete now (synchronous without an overlapped).
GUEST_IMPORT(XamUserFlushLogonQueue)
{
    if (ctx.r3.u32)
        kernel::CompleteOverlapped(ctx.r3.u32, 0, 0, 0);
    ctx.r3.u64 = 0;
}

// VOID XamBackgroundDownloadNetworkStorageRegisterChangeCallback(PFN Callback): XAM calls it when its network
// storage (Xbox LIVE cloud) changes. There is none here, so it never changes.
GUEST_IMPORT(XamBackgroundDownloadNetworkStorageRegisterChangeCallback)
{
    ctx.r3.u64 = 0;
}

// ---- Dashboard state ---------------------------------------------------------------------------------------------

namespace
{
    std::atomic<uint32_t> g_stagingMode = 0;
    std::atomic<uint32_t> g_dashContext = 0;
    std::atomic<uint32_t> g_launchRequestCallback = 0;
    std::mutex g_xdfMutex;
    uint32_t g_xdfReferences = 0;
}

// DWORD XamGetStagingMode(): Xbox LIVE "preview" (staging) mode, XAM 0x816E27E8 returns a global that starts at 0;
// only XamSetStagingMode changes it (the dash does so only on development hardware, from a debug key).
GUEST_IMPORT(XamGetStagingMode)
{
    ctx.r3.u64 = g_stagingMode.load();
}

// VOID XamSetStagingMode(DWORD Mode)
GUEST_IMPORT(XamSetStagingMode)
{
    g_stagingMode = ctx.r3.u32;
}

// VOID XamSetDashContext(DWORD Context) (XAM 0x816E1680): stores the dashboard's context (values 1, 3, 6, 7 ... name
// its screens; XamIsGuideDisabled reads it) and broadcasts XN_SYS_DASHCONTEXTCHANGED when it changes. XAM does this
// only for callers with executable privilege 8 (SetDashContext), which dash.xex has.
GUEST_IMPORT(XamSetDashContext)
{
    constexpr uint32_t kXnSysDashContextChanged = 0x8000000C;
    if (g_dashContext.exchange(ctx.r3.u32) != ctx.r3.u32)
        kernel::BroadcastNotification(kXnSysDashContextChanged, 0);
}

// DWORD XamGetDashContext()
GUEST_IMPORT(XamGetDashContext)
{
    ctx.r3.u64 = g_dashContext.load();
}

uint32_t kernel::DashContext()
{
    return g_dashContext.load();
}

// XAM's system cache (0x81A9B240): files stored by ID (XamCacheStoreFile) in the hard drive's cache partition, which
// XAM empties when it is reset. This console's cache is empty. IDs XAM knows (0x81675BD0): 1-3, 5, 7-9, 0xB-0x10,
// 0x12-0x21; another is ERROR_INVALID_PARAMETER.
namespace
{
    bool KnownCacheFile(uint32_t id)
    {
        return (id >= 1 && id <= 3) || id == 5 || (id >= 7 && id <= 9) || (id >= 0xB && id <= 0x10) ||
            (id >= 0x12 && id <= 0x21);
    }
}

// DWORD XamCacheFetchFile(DWORD FileId, ...) (XAM 0x816793F0 -> 0x81678C20): the cached file's data; a file the cache
// does not hold is ERROR_FILE_NOT_FOUND (0x816786C8). The dash asks for 0x1C (title IDs whose content to re-check).
GUEST_IMPORT(XamCacheFetchFile)
{
    ctx.r3.u64 = KnownCacheFile(ctx.r3.u32) ? 2 : 0x57;
}

// DWORD XamCacheDeleteFile(DWORD FileId, ...) (XAM 0x81679418 -> 0x81678D60): ERROR_FILE_NOT_FOUND when absent.
GUEST_IMPORT(XamCacheDeleteFile)
{
    ctx.r3.u64 = KnownCacheFile(ctx.r3.u32) ? 2 : 0x57;
}

// HRESULT XamFitnessInitialize(DWORD* Result) (XAM 0x8168F4E0): XMsgProcessRequest(0xFB, 0xB0080) -> 0x8191EB10. Once
// per boot, when fitness tracking applies (the running title is the dash, 0x8193A248): *Result = 1, each signed-in
// user's fitness records are loaded (Kinect fitness; nothing in these profiles), then *Result = 0 when the clock is
// past 2011-08-01, which it always is. Later calls leave *Result as it is. Returns S_OK; NULL is E_INVALIDARG.
GUEST_IMPORT(XamFitnessInitialize)
{
    static std::atomic<bool> initialized{ false };
    const uint32_t result = ctx.r3.u32;
    if (result == 0)
    {
        ctx.r3.u64 = 0x80070057;
        return;
    }
    if (kernel::TitleId() != 0xFFFE07D1)
        Fatal("XamFitnessInitialize from title {:08X}: XAM's fitness check for other titles is not implemented",
              kernel::TitleId());
    if (!initialized.exchange(true))
        guest::g_memory.Write32(result, 0);
    ctx.r3.u64 = 0;
}

// DWORD XamBackgroundDownloadNetworkStorageEnable(BOOL Enable) (XAM 0x8170FF90): sets the background downloader's
// network-storage flag (0x81715DF8); disabling also cancels its network-storage downloads, of which an offline
// console has none. Returns 0.
GUEST_IMPORT(XamBackgroundDownloadNetworkStorageEnable)
{
    ctx.r3.u64 = 0;
}

// HRESULT XamFitnessClearBodyProfileRecords(DWORD* Info): named so in Xenia's ordinal table (1217), but XAM 17559's
// export (0x81698BF8) reports kiosk information: Info[0] must be the size 0x10; it fills Info[1] = 1, Info[2] = kiosk
// hardware (XboxHardwareInfo flag 0x1000; a retail console: 0) and Info[3] = the kiosk's SMC location byte, or
// 0xFFFFFFFF without kiosk hardware. The dash builds its kiosk home page from it only on kiosks.
GUEST_IMPORT(XamFitnessClearBodyProfileRecords)
{
    auto& mem = guest::g_memory;
    const uint32_t info = ctx.r3.u32;
    if (info == 0)
    {
        ctx.r3.u64 = 0x80070057; // E_INVALIDARG
        return;
    }
    if (mem.Read32(info) != 0x10)
    {
        ctx.r3.u64 = 0x8000FFFF; // E_UNEXPECTED
        return;
    }
    mem.Write32(info + 4, 1);
    mem.Write32(info + 8, 0);
    mem.Write32(info + 12, 0xFFFFFFFF);
    ctx.r3.u64 = 0;
}

// HRESULT XdfInitialize() / VOID XdfShutdown(): XAM's download-feed client (XAM 0x817F5EA8 / 0x817F6E68), reference
// counted per title process. Nothing is downloaded on this console; the count is kept so a missing XdfInitialize
// before an Xdf request shows up.
GUEST_IMPORT(XdfInitialize)
{
    std::lock_guard lock(g_xdfMutex);
    g_xdfReferences++;
    ctx.r3.u64 = 0;
}

GUEST_IMPORT(XdfShutdown)
{
    std::lock_guard lock(g_xdfMutex);
    if (g_xdfReferences == 0)
        Fatal("XdfShutdown without XdfInitialize");
    g_xdfReferences--;
}

// BOOL XamFeatureEnabled(DWORD Feature): XAM 17559 (0x8168EEB8) answers from a constant mask: features 1-32 and 39
// are on, everything else off. The dash only asks for 3 (its music player).
GUEST_IMPORT(XamFeatureEnabled)
{
    const uint32_t bit = (ctx.r3.u32 - 1) & 0x7F;
    ctx.r3.u64 = bit < 64 && ((1ull << bit) & 0x40FFFFFFFFull) ? 1 : 0;
}

// VOID XamAvatarShutdown(): XAM unloads its avatar system app (XamUnloadSysApp(0xF2, 1)). The runtime keeps no avatar
// app state.
GUEST_IMPORT(XamAvatarShutdown)
{
}

// BYTE XamAvatarManifestGetBodyType(const X_AVATAR_METADATA* manifest) (XAM 0x816753C8 -> 0x8196CA80): the body asset
// ID at +0x120 compared with XAM's two body assets (0x81632E68 / 0x81632E78): 1 male, 2 female, 0 neither (the
// default silhouettes' empty manifest while avatars are off).
GUEST_IMPORT(XamAvatarManifestGetBodyType)
{
    static constexpr uint8_t kMale[16] = { 0, 0, 0, 2, 0, 0, 0, 1, 0xC1, 0xC8, 0xF1, 0x09, 0xA1, 0x9C, 0xB2, 0xE0 };
    static constexpr uint8_t kFemale[16] = { 0, 0, 0, 2, 0, 1, 0, 2, 0xC1, 0xC8, 0xF1, 0x09, 0xA1, 0x9C, 0xB2, 0xE0 };
    const uint8_t* body = guest::g_memory.Translate(ctx.r3.u32 + 0x120);
    ctx.r3.u64 = std::memcmp(body, kMale, 16) == 0 ? 1 : std::memcmp(body, kFemale, 16) == 0 ? 2 : 0;
}

// VOID XamLoaderRegisterLaunchRequestCallback(PFN Callback) (XAM 0x8169D0C8): XAM calls it whenever a title launch
// request is queued, with &{request flags, request data, XamExecutingOnBehalfOfTitle(), out}; a non-zero `out` makes
// XAM add 0x200 to the request. Kept for the launch path (no launches yet); 0 unregisters.
GUEST_IMPORT(XamLoaderRegisterLaunchRequestCallback)
{
    g_launchRequestCallback = ctx.r3.u32;
}

// ---- Xbox LIVE schema --------------------------------------------------------------------------------------------

namespace
{
    // XamGetOnlineSchema's descriptor in xam.xex 17559 (0x81A903E0, returned by XAM 0x8182FFC0): {schema, size}.
    constexpr uint32_t kXamOnlineSchemaDescriptor = 0x81A903E0;
    std::once_flag g_schemaOnce;
    uint32_t g_schemaDescriptor = 0;

    // Copies XAM's online schema into guest memory (descriptor + data, never freed: the title binds pointers into it).
    // The data is Microsoft's, read from the user's own xam.xex (converted like the other system modules).
    void LoadOnlineSchema()
    {
        const std::filesystem::path path = DataDirectory() / "SystemUpdate" / "converted" / "xam.xex";
        const xex::HostImage& xam = kernel::XamImage();
        const uint8_t* descriptor = xam.At(kXamOnlineSchemaDescriptor, 8);
        if (descriptor == nullptr)
            Fatal("{} is not xam.xex 2.0.17559 (no online schema descriptor at {:08X})", path.string(),
                kXamOnlineSchemaDescriptor);
        const uint32_t schema = uint32_t(descriptor[0]) << 24 | descriptor[1] << 16 | descriptor[2] << 8 | descriptor[3];
        const uint32_t size = uint32_t(descriptor[4]) << 24 | descriptor[5] << 16 | descriptor[6] << 8 | descriptor[7];
        const uint8_t* data = xam.At(schema, size);
        if (data == nullptr || size < 0x2C)
            Fatal("{}: the online schema descriptor ({:08X}, {} bytes) does not point into the image", path.string(),
                schema, size);
        auto& mem = guest::g_memory;
        g_schemaDescriptor = mem.SystemAlloc(8 + size, 16);
        mem.Write32(g_schemaDescriptor, g_schemaDescriptor + 8);
        mem.Write32(g_schemaDescriptor + 4, size);
        memcpy(mem.Translate(g_schemaDescriptor + 8), data, size);
        Log("XAM online schema: {} bytes from {}", size, path.string());
    }
}

// xam.xex's image, decoded once from the user's converted copy: XAM's data the runtime hands out as is (online schema,
// resource sections).
const xex::HostImage& kernel::XamImage()
{
    static xex::HostImage image;
    static std::once_flag once;
    std::call_once(once, [] {
        const std::filesystem::path path = DataDirectory() / "SystemUpdate" / "converted" / "xam.xex";
        if (!std::filesystem::exists(path))
            Fatal("{} is missing (the dashboard needs XAM's data: online schema, resources). Convert it from your "
                "system update: python -I codebreaker.formats.xex.basic SystemUpdate/flash/$flash_xam.xex {}",
                path.string(), path.string());
        image = xex::ReadImage(path.string());
    });
    return image;
}

// DWORD XamGetOnlineCountryFeatures(BYTE Country) (XAM 0x816A1BB0): the Xbox LIVE features of an online country, from
// XAM's table at 0x81A82000 (46 rows of 8 bytes: country byte, 3 bytes, be32 feature bits); 0x80000000 for a
// country not listed. Read from the user's xam.xex. The dash tests bits 2 and 3 (hub items per country).
GUEST_IMPORT(XamGetOnlineCountryFeatures)
{
    constexpr uint32_t kTable = 0x81A82000, kRows = 0x2E;
    const uint8_t* table = kernel::XamImage().At(kTable, kRows * 8);
    if (table == nullptr)
        Fatal("XamGetOnlineCountryFeatures: xam.xex has no country table at {:08X}", kTable);
    const uint8_t country = uint8_t(ctx.r3.u32);
    ctx.r3.u64 = 0x80000000;
    for (uint32_t i = 0; i < kRows; i++)
        if (table[i * 8] == country)
        {
            const uint8_t* f = table + i * 8 + 4;
            ctx.r3.u64 = uint32_t(f[0]) << 24 | uint32_t(f[1]) << 16 | uint32_t(f[2]) << 8 | f[3];
            break;
        }
}

namespace kernel
{
    // The service schema's bytes (the same blob the dash and XAM's modules link in): sent to our server, which decodes
    // the .ashx service tasks with it (xam_msg.cpp, live::XonTask).
    std::vector<uint8_t> OnlineSchemaBlob()
    {
        std::call_once(g_schemaOnce, LoadOnlineSchema);
        auto& mem = guest::g_memory;
        const uint32_t data = mem.Read32(g_schemaDescriptor), size = mem.Read32(g_schemaDescriptor + 4);
        const uint8_t* p = mem.Translate(data);
        return std::vector<uint8_t>(p, p + size);
    }
}

// const XONLINE_SCHEMA* XamGetOnlineSchema(): XAM's built-in Xbox LIVE service schema. The dash passes it straight to
// XOnlineStartupEx, which fails without it (and the dash then shuts down).
GUEST_IMPORT(XamGetOnlineSchema)
{
    std::call_once(g_schemaOnce, LoadOnlineSchema);
    ctx.r3.u64 = g_schemaDescriptor;
}

// ---- Extra audio/video codecs ------------------------------------------------------------------------------------
// HRESULT Load(PVOID* Entry) / VOID Unload(): xam 2012/2013 (named XamGetDashBackstackData / ...NodesCount in Xenia's
// ordinal table, but XAM 17559's exports at 0x8168EE38 / 0x8168EE48 are these) load dash.ExtraAVCodecs.xex through
// XdfLoadXexFromCache and return its ordinal 1, reference counted; 2019/2020 do the same for dash.ExtraAVCodecs2.xex.
// The dash's XAVInitialize asks for them at start-up and, when the load fails, skips MFStartup and DRM set-up (media
// playback). Those modules are in the system update but not recompiled yet, so the load fails as XAM's does for a
// missing file: HRESULT_FROM_WIN32(ERROR_NOT_FOUND), *Entry untouched.
namespace
{
    constexpr uint32_t kHresultNotFound = 0x80070490;

    void LoadCodecModule(PPCContext& ctx, const char* module)
    {
        if (ctx.r3.u32 == 0)
        {
            ctx.r3.u64 = 0x80004003; // E_POINTER
            return;
        }
        Log("XAM: {} is not available (not recompiled yet); media codecs it carries are off", module);
        ctx.r3.u64 = kHresultNotFound;
    }
}

GUEST_IMPORT(XamGetDashBackstackData)
{
    LoadCodecModule(ctx, "dash.ExtraAVCodecs.xex");
}

GUEST_IMPORT(XamLoadExtraAVCodecs2)
{
    LoadCodecModule(ctx, "dash.ExtraAVCodecs2.xex");
}

// The unloads only follow a successful load, which cannot happen yet.
GUEST_IMPORT(XamGetDashBackstackNodesCount)
{
    Fatal("Unload of dash.ExtraAVCodecs.xex (xam 2013) without a loaded module");
}

GUEST_IMPORT(XamUnloadExtraAVCodecs2)
{
    Fatal("Unload of dash.ExtraAVCodecs2.xex (xam 2020) without a loaded module");
}

// ---- Avatars -----------------------------------------------------------------------------------------------------
// HRESULT XamAvatarInitialize(DWORD Version, DWORD CoordinateSystem, DWORD, PVOID TextureFunctions[5], PVOID Buffer,
// DWORD BufferSize) (XAM 0x816755E8): loads XAM's avatar system app (0xF2) and opens the avatar asset package
// (AvatarAssetPack.toc, which the system update's manifest places in FFFE07DF00000002). Not implemented yet: the
// runtime answers what XAM 17559 answers when the package cannot be found, 0x80070002. The dash ignores the result;
// every XAvatar* wrapper then reports "not initialised" without calling XAM, so avatars are simply absent. A real
// 17559 console has the package (the user's update carries it): loading it is a later job.
namespace
{
    uint32_t AvatarInitialize()
    {
        static std::once_flag logged;
        std::call_once(logged, [] { Log("XAM: avatar system not available yet (asset package not loaded): avatars are off"); });
        return 0x80070002;
    }
}

GUEST_IMPORT(XamAvatarInitialize)
{
    ctx.r3.u64 = AvatarInitialize();
}

// The early NXE dashboards (7258 ... 7371) look it up at run time instead (XexGetProcedureAddress, ordinal 1500).
GUEST_EXPORT("xam.xex", 1500, XamAvatarInitializeByAddress)
{
    ctx.r3.u64 = AvatarInitialize();
}

// PVOID XamGetXTweakManager() (xam ordinal 2170; Xenia's table calls it XamIsNuiAutomationEnabled, the stub
// generator's name): XAM 17559 (0x81964638) returns NULL.
GUEST_IMPORT(XamIsNuiAutomationEnabled)
{
    ctx.r3.u64 = 0;
}

namespace
{
    std::mutex g_backUriMutex;
    std::string g_backUri;
    uint32_t g_backUriKind = 0;
}

// HRESULT XamPushBackURI(LPCSTR Uri) (XAM ordinal 1202): stores the URI to come back to (fewer than 0x3FC
// characters; a longer one clears it and returns 0x8007007A) and whether a system title (FFFExxxx: 1) or a game (2) pushed it; NULL clears
// it. Ignored while XAM's flag 0x81B5896C is set (not modelled: never set here).
GUEST_IMPORT(XamPushBackURI)
{
    std::lock_guard lock(g_backUriMutex);
    const uint32_t uri = ctx.r3.u32;
    if (uri == 0)
    {
        g_backUri.clear();
        g_backUriKind = 0;
        ctx.r3.u64 = 0;
        return;
    }
    const char* text = reinterpret_cast<const char*>(guest::g_memory.Translate(uri));
    const size_t length = strnlen(text, 0x3FC);
    if (length >= 0x3FC)
    {
        g_backUri.clear();
        g_backUriKind = 0;
        ctx.r3.u64 = 0x8007007A;
        return;
    }
    g_backUri.assign(text, length);
    g_backUriKind = (kernel::TitleId() & 0xFFFE0000) == 0xFFFE0000 ? 1 : 2;
    ctx.r3.u64 = 0;
}

namespace
{
    std::mutex g_dashAppInfoMutex;
    uint64_t g_dashAppInfoId = 0;
    uint32_t g_dashAppInfoValue = 0;
}

// VOID XamSetActiveDashAppInfo(const APPINFO* Info) (XAM 0x816E30F8): keeps 12 bytes (a u64 and a u32) for
// XamGetActiveDashAppInfo (0x816E3168, which XAM's own UI reads; no title imports it); NULL clears them.
GUEST_IMPORT(XamSetActiveDashAppInfo)
{
    auto& mem = guest::g_memory;
    std::lock_guard lock(g_dashAppInfoMutex);
    g_dashAppInfoId = ctx.r3.u32 ? mem.Read64(ctx.r3.u32) : 0;
    g_dashAppInfoValue = ctx.r3.u32 ? mem.Read32(ctx.r3.u32 + 8) : 0;
}

// DWORD XamLoaderGetPriorTitleId() (XAM 0x8169CFC8): the title that ran before the current one. The dashboard is the
// first title after power-on: 0.
GUEST_IMPORT(XamLoaderGetPriorTitleId)
{
    ctx.r3.u64 = 0;
}

// BOOL XamGetCurrentDemand(PDWORD Id, PDWORD Value) (XAM 0x81994C40): XAM's pending "demand" (a failed requirement,
// e.g. a LIVE sign-up landing). None was ever raised: the initial state { 0xFE, 0 }, FALSE.
GUEST_IMPORT(XamGetCurrentDemand)
{
    auto& mem = guest::g_memory;
    if (ctx.r3.u32)
        mem.Write32(ctx.r3.u32, 0xFE);
    if (ctx.r4.u32)
        mem.Write32(ctx.r4.u32, 0);
    ctx.r3.u64 = 0;
}

// BOOL XamNuiIsDeviceReady() (XAM 0x817F9988): Kinect sensor status bit 1. No sensor: FALSE.
GUEST_IMPORT(XamNuiIsDeviceReady)
{
    ctx.r3.u64 = 0;
}

// BOOL XamIsUIActive() (XAM 0x816C7D78): whether a system UI (an app XAM loaded: Guide, sign-in, message box...) is up:
// XAM's flag 0x81AA2188, set while XamAppLoad's app runs (XN_SYS_UI 1 to XN_SYS_UI 0). The dash polls it after
// XamShowSigninUIp until the sign-in UI has come and gone.
GUEST_IMPORT(XamIsUIActive)
{
    ctx.r3.u64 = kernel::SystemUiActive() ? 1 : 0;
}

// VOID XamOfflineTimerGetData(PWORD Data) (XAM 0x819962F8): XAM's offline (family) timer state. Its buffer exists
// only on consoles whose XboxHardwareInfo flags have 0x200 (with BootLoaderFlags bit 0x400 clear); this console's
// flags are 0x20, so XAM has no buffer and writes 0 (the dash then keeps checking it on its timer).
GUEST_IMPORT(XamOfflineTimerGetData)
{
    if (ctx.r3.u32 == 0)
        Fatal("XamOfflineTimerGetData(NULL): XAM writes through the pointer unchecked");
    guest::g_memory.Write16(ctx.r3.u32, 0);
}

// HRESULT XamPackageManagerFindPackageContainingIndexedXEX(LPCSTR Name, LPSTR Path, DWORD cchPath, PDWORD Value):
// XAM 17559 (app 0xFE message 0x25002) looks the file up in the system update's system.manifest and writes the
// package's path ("<$SystemUpdate dir>\FFFE07DF0000000x") and a value from its record; 0x80070002 when the manifest
// is not loaded or no package holds the name, 0x80070057 for a NULL Value. The packages it finds (FFFE07DF*) are
// avatar data (the dash asks for "SigninSleepiesIndex": sleeping-avatar animations), and avatars are off in this
// port (no avatar asset package, PIRS packages not mounted), so every name answers "not found" as on a console
// whose manifest is not loaded; the dash then skips those animations.
GUEST_IMPORT(XamPackageManagerFindPackageContainingIndexedXEX)
{
    auto& mem = guest::g_memory;
    const std::string name = ctx.r3.u32 ? std::string(reinterpret_cast<const char*>(mem.Translate(ctx.r3.u32))) : "";
    if (ctx.r6.u32 == 0)
    {
        ctx.r3.u64 = 0x80070057;
        return;
    }
    Log("XamPackageManagerFindPackageContainingIndexedXEX(\"{}\"): not found (avatar packages are not loaded)", name);
    ctx.r3.u64 = 0x80070002;
}

// ---- XamFormatMessage --------------------------------------------------------------------------------------------
// DWORD XamFormatMessage(PWSTR Buffer, DWORD Chars, PCWSTR Format, ...) (XAM 0x816E09F8 -> 0x816E0860): a
// FormatMessage-style template made printf-style, then _snwprintf: "%N" (1-9) takes argument N as "%s", "%N!fmt!" takes
// argument N with "%fmt", "%%" is a percent sign, a '%' followed by anything else takes the next argument in order and
// keeps the printf conversion that follows. At most 9 arguments (each the low word of an 8-byte slot) and 200
// template characters; a template that breaks a rule gives an empty string. Conversions here: s (wide string), S (byte
// string), c, d, i, u, x, X, with flags, width and precision.
namespace
{
    std::u16string GuestWide(uint32_t address, size_t max = 4096)
    {
        std::u16string s;
        if (address == 0)
            return u"(null)";
        auto& mem = guest::g_memory;
        for (size_t i = 0; i < max; i++)
        {
            const char16_t c = char16_t(mem.Read16(address + uint32_t(2 * i)));
            if (c == 0)
                break;
            s.push_back(c);
        }
        return s;
    }

    // One printf conversion (spec = flags/width/precision, conv = the letter) of a 32-bit argument.
    std::u16string Convert(const std::string& spec, char conv, uint32_t value)
    {
        std::u16string text;
        if (conv == 's' || conv == 'S')
        {
            if (conv == 's')
                text = GuestWide(value);
            else if (value)
                for (uint32_t a = value; guest::g_memory.Read8(a) != 0 && a - value < 4096; a++)
                    text.push_back(char16_t(guest::g_memory.Read8(a)));
            else
                text = u"(null)";
            if (const size_t dot = spec.find('.'); dot != std::string::npos)
                text = text.substr(0, size_t(std::atoi(spec.c_str() + dot + 1)));
        }
        else
        {
            char buffer[64];
            const std::string fmt = "%" + spec + conv;
            if (conv == 'c')
                std::snprintf(buffer, sizeof(buffer), "%c", int(value & 0xFF));
            else if (conv == 'd' || conv == 'i')
                std::snprintf(buffer, sizeof(buffer), fmt.c_str(), int32_t(value));
            else
                std::snprintf(buffer, sizeof(buffer), fmt.c_str(), value);
            for (const char* p = buffer; *p; p++)
                text.push_back(char16_t(uint8_t(*p)));
            return text;
        }
        // width for strings
        size_t width = 0;
        bool left = false;
        for (char c : spec)
        {
            if (c == '-')
                left = true;
            else if (c == '.')
                break;
            else if (c >= '0' && c <= '9')
                width = width * 10 + size_t(c - '0');
        }
        if (text.size() < width)
            text = left ? text + std::u16string(width - text.size(), u' ') : std::u16string(width - text.size(), u' ') + text;
        return text;
    }
}

GUEST_IMPORT(XamFormatMessage)
{
    auto& mem = guest::g_memory;
    const uint32_t buffer = ctx.r3.u32, chars = ctx.r4.u32, format = ctx.r5.u32;
    uint32_t args[9];
    const uint32_t registers[5] = { ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32, ctx.r10.u32 };
    for (int i = 0; i < 9; i++)
        args[i] = i < 5 ? registers[i] : kernel::StackArg32(ctx, 8 + (i - 5));
    const std::u16string tmpl = GuestWide(format, 200);
    std::u16string out;
    size_t next = 0;
    bool bad = false;
    for (size_t i = 0; i < tmpl.size() && !bad; i++)
    {
        if (tmpl[i] != u'%')
        {
            out.push_back(tmpl[i]);
            continue;
        }
        if (++i >= tmpl.size())
            break;
        if (tmpl[i] == u'%')
        {
            out.push_back(u'%');
            continue;
        }
        if (next >= 9)
        {
            bad = true;
            break;
        }
        uint32_t value;
        std::string spec;
        char conv = 's';
        if (tmpl[i] >= u'1' && tmpl[i] <= u'9')
        {
            value = args[tmpl[i] - u'1'];
            next++;
            if (i + 1 < tmpl.size() && tmpl[i + 1] == u'!')
            {
                const size_t end = tmpl.find(u'!', i + 2);
                if (end == std::u16string::npos)
                {
                    bad = true;
                    break;
                }
                std::string f;
                for (size_t k = i + 2; k < end; k++)
                    f.push_back(char(tmpl[k]));
                conv = f.empty() ? 's' : f.back();
                spec = f.empty() ? "" : f.substr(0, f.size() - 1);
                i = end;
            }
        }
        else
        {
            value = args[next++];
            while (i < tmpl.size() && std::strchr("-+ #0123456789.", char(tmpl[i])))
                spec.push_back(char(tmpl[i++]));
            if (i >= tmpl.size())
                break;
            conv = char(tmpl[i]);
        }
        if (!std::strchr("sScdiuxX", conv))
            Fatal("XamFormatMessage: conversion '{}' in the template is not handled", conv);
        out += Convert(spec, conv, value);
    }
    if (bad)
        out.clear();
    if (buffer && chars)
    {
        const size_t n = std::min<size_t>(out.size(), chars - 1);
        for (size_t k = 0; k < n; k++)
            mem.Write16(buffer + uint32_t(2 * k), uint16_t(out[k]));
        mem.Write16(buffer + uint32_t(2 * n), 0);
    }
    ctx.r3.u64 = uint32_t(std::min<size_t>(out.size(), chars ? chars - 1 : 0));
}

// ---- SmartGlass keyboards (XamLrcKeyboard*) --------------------------------------------------------------------------
// "LRC" is the SmartGlass remote control: a keyboard on screen registers (two slots) so a connected phone or tablet can
// type into it. XAM keeps the slots only while its SmartGlass service runs (0x81AD2954); this console has no
// SmartGlass connection, so every call takes XAM's path for a stopped service: Register answers slot 2 (none,
// 0x81814678), GetInput ERROR_FUNCTION_FAILED (its worker's ERROR_NO_MORE_ITEMS), Unregister / UpdateText do nothing.
// The text-change calls are reached only with a SmartGlass edit, so they stop with specifics if ever called.
GUEST_IMPORT(XamLrcKeyboardGetRequiredBufferSize)
{
    ctx.r3.u64 = 0x2D58; // XAM 0x818106E8
}

GUEST_IMPORT(XamLrcKeyboardRegister)
{
    ctx.r3.u64 = 2;
}

GUEST_IMPORT(XamLrcKeyboardUnregister)
{
    ctx.r3.u64 = 0;
}

GUEST_IMPORT(XamLrcKeyboardUpdateText)
{
    ctx.r3.u64 = 0;
}

GUEST_IMPORT(XamLrcKeyboardGetInput)
{
    ctx.r3.u64 = ctx.r4.u32 == 0 ? 0x57 : 0x65B;
}

GUEST_IMPORT(XamLrcKeyboardApplyTextChange)
{
    Fatal("XamLrcKeyboardApplyTextChange: a SmartGlass text edit, but SmartGlass is not connected here");
}

GUEST_IMPORT(XamLrcKeyboardValidateTextChange)
{
    Fatal("XamLrcKeyboardValidateTextChange: a SmartGlass text edit, but SmartGlass is not connected here");
}

// The console dims its screen after a while without input; a title resets the timer when something on screen keeps
// the player busy (a video, a long load) and can turn the processing off. The PC's own screen saver is the host's.
// DWORD XamResetInactivity(void), void XamEnableInactivityProcessing(DWORD dwInactivityTime, BOOL fEnable)
GUEST_IMPORT(XamResetInactivity)
{
    ctx.r3.u64 = 0;
}

GUEST_IMPORT(XamEnableInactivityProcessing)
{
    ctx.r3.u64 = 0;
}
