#pragma once

#include <ppc_config.h>
#include <ppc_context.h>

#include "log.h"
#include "memory.h"

#include <atomic>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace xex
{
    struct HostImage;
}

// Native replacement for the Xbox 360 kernel (xboxkrnl.exe) and XAM (xam.xex).
//
// Each import is written with GUEST_IMPORT(Name) { ... }: arguments arrive in ctx.r3..r10 (f1.. for floats) and the
// result goes in ctx.r3, exactly as the PowerPC ABI the game was compiled for. Imports not yet implemented are
// generated as reporting stubs (tools/scripts/gen_import_stubs.py).
//
// Guest structure layouts below were cross-checked against Xenia Canary (BSD-3, Ben Vanik and contributors).

namespace kernel
{
    void TraceImport(const char* name, const PPCContext& ctx);
    // Return addresses up the guest stack (LR, then each caller's saved LR along the back chain), for diagnostics.
    std::string GuestBacktrace(const PPCContext& ctx);
    // CB_WATCH diagnostics (watchpoint.cpp): arms the data watchpoint on a guest thread's host thread (created suspended).
    void ArmWatchpoint(void* hostThread);
    // CB_SCAN and CB_PROFILE diagnostics (watchpoint.cpp, profiler.cpp), called after each presented frame.
    void DiagnosticsFrame(uint64_t frame);
    void ProfilerFrame(uint64_t frame);
    // Logs every guest thread's last import, its caller and backtrace (e.g. when no frame was presented for a while).
    void DumpThreads(const std::string& reason);
    // The import this host thread entered last and its caller (for error reports).
    const char* CurrentImport(uint32_t* caller = nullptr);
    // Reports a call to an import the runtime does not implement. Exits unless CB_CONTINUE_UNIMPLEMENTED=1, in which
    // case it returns with r3 = 0 so the caller can continue (for exploration only: results may diverge from hardware).
    void UnimplementedImport(const char* name, PPCContext& ctx);
    // _snwprintf(buffer, count, format, args...) with host values for the arguments (xboxkrnl_printf.cpp): what XAM
    // code calling the kernel's _snwprintf does.
    int32_t FormatWide(uint32_t buffer, uint32_t count, uint32_t format, std::initializer_list<uint64_t> args);
    // XCONFIG_USER_RETAIL_FLAGS bit 8: the console clock shows 24-hour time (xboxkrnl_xconfig.cpp).
    bool TwentyFourHourClock();
    // xam.xex's image read from the user's converted copy (kernel/xam_system.cpp).
    const xex::HostImage& XamImage();
    // HMODULE of a native system module ("xam.xex", "xboxkrnl.exe"), created on first use (xboxkrnl_modules.cpp).
    uint32_t SystemModuleHandle(const std::string& name);
    // XboxHardwareInfo +0xE, the boot loader's flags (0x1000: flash patch slot 1 active).
    uint16_t BootLoaderFlags();
    // A 4-byte XConfig setting as the console stores it (big endian), from the store or its default.
    uint32_t XConfigRead32(uint32_t category, uint32_t id);
    // Any XConfig setting's bytes as the console stores them, from the store or its default.
    std::vector<uint8_t> XConfigRead(uint32_t category, uint32_t id);
    // Starts the live client (kernel/xam_live.cpp; the ports' own Xbox LIVE server from console_settings.ini).
    void InitializeLive();
    // A dashboard started by XamLoaderLaunchTitleEx: takes over the launch data the previous process left.
    void LoadRelaunchData();
    // The relaunched dashboard (itself or a game going back) signs the previous profiles in again; once dash.xex is
    // the title (main.cpp).
    void RestoreRelaunchSignIns();
    // Whether `user`'s profile is signed in to Xbox LIVE (the ports' own server accepted it; kernel/xam_live.cpp).
    bool UserOnLive(uint32_t user);
    // XAM's Xbox LIVE service schema blob (xam_system.cpp).
    std::vector<uint8_t> OnlineSchemaBlob();
    // The title has seen this user's Xbox LIVE logon (XamShowSigninUIp): from now on our server's sign-ins of the user
    // (reconnects) raise XN_LIVE_CONNECTIONCHANGED; before it they are silent (kernel/xam_live.cpp).
    void LiveLogonCommitted(uint32_t user);
    // XAM's sign-in toast (kernel/xam_ui.cpp): 7 "<gamertag> signed in" for a local profile, 8 "... signed in to Xbox
    // Live" / 9 "... signed in offline" for an Xbox LIVE profile by `online`.
    void SignInToast(uint32_t user, const std::string& gamertag, bool liveProfile, bool online);
    // Whether the console is logged on to Xbox LIVE (a profile is on LIVE; XLiveBase 0x58003 "connected").
    bool ConsoleOnLive();
    // Another player's gamer picture from our Xbox LIVE (XAM's tile cache: the 64x64 PNG, or 32x32 when `size32`);
    // nullopt when nobody is on LIVE or the player has none. Blocks on the first read of a player
    // (PrefetchGamerPictures fetches them in the background first).
    std::optional<std::vector<uint8_t>> LiveGamerPicture(uint64_t onlineXuid, bool size32);
    void PrefetchGamerPictures(std::vector<uint64_t> onlineXuids);
    // A LiveHive value (Xbox LIVE's configuration: the live server's values plus EPIX's defaults; xam_live.cpp).
    std::optional<std::string> LiveHiveValue(const std::string& key);
    // Sends `user`'s rich presence (this title's XLAST presence mode with the contexts and properties it set through
    // XUserSetContext / XUserSetPropertyEx, live/presence.h) to our Xbox LIVE (kernel/xam_msg.cpp).
    void RefreshPresence(uint32_t user);
    // A resource of the running executable (XEX header 0x2FF: dash.xex's "epix", the SPA ...), empty if absent.
    std::span<const uint8_t> ExecutableResource(const std::string& name);
    // The hub's online home: our EPIX manifest, made from dash.xex's own offline one (kernel/epix_home.cpp).
    std::optional<std::string> EpixOnlineManifest(std::string& error);
    // The hub's spotlight (ad) pictures the user supplies: images/ads/ beside the executable (epix_home.cpp), and
    // one of them by file name (served as http://epix.localhost/ads/<file>).
    const std::filesystem::path& SpotlightDirectory();
    std::optional<std::vector<uint8_t>> SpotlightPicture(const std::string& file);
    // Opens the Guide's Community on a tab for `user`, as XamShowPartyUI / XamShowFriendsUI do: 0 party, 1 friends,
    // 2 messages, 3 players (kernel/xam_ui.cpp). Returns the XamShow*UI result.
    uint32_t ShowCommunityTab(uint32_t user, int tab);
    // XLiveBase 0x58020 (XFriendsCreateEnumerator) on its argument slots (kernel/xam_live.cpp).
    uint32_t FriendsCreateEnumerator(uint32_t arguments);
    // XLiveBase 0x58024 (XMessageEnumerate: the user's message summaries) on its argument slots.
    uint32_t MessageEnumerate(uint32_t arguments);
    // XLiveBase 0x58007 (XOnlineGetServiceInfo).
    uint32_t OnlineGetServiceInfo(uint32_t serviceId, uint32_t info);
    // XLiveBase 0x58072 (XOnlineGetWebServiceTaskBufferSize) on its argument block (kernel/xam_web_service.cpp).
    uint32_t WebServiceTaskBufferSize(uint32_t args);
    // Stores an XConfig setting (what ExSetXConfigSetting does); `value` must have the setting's size.
    void XConfigWrite(uint32_t category, uint32_t id, const std::vector<uint8_t>& value);
    // XConfig country (XCONFIG_USER_COUNTRY) of an XC_LOCALE, from XAM's locale table; 0 if unknown.
    uint8_t XConfigCountryFromLocale(uint32_t locale);
    // The DVD drive's tray (kernel/xboxkrnl_hal.cpp), in the values XamLoaderGetDvdTrayState returns: 0 closed,
    // 1 closing, 2 open, 3 opening.
    uint32_t DvdTrayState();
    // Opens or closes the DVD tray (what HalOpenCloseODDTray does).
    void OpenCloseDvdTray(bool open);
    // The dashboard's context (XamSetDashContext; kernel/xam_system.cpp).
    uint32_t DashContext();
    // The device selector's UI (XamShowDeviceSelectorUI; kernel/xam_ui.cpp): the offered devices for `user`; `done`
    // gets the chosen row, or -1 when the player backs out. False when the system UI cannot show it.
    struct DeviceChoice
    {
        std::u16string name;
        uint32_t kind = 1;
        uint64_t freeBytes = 0;
        uint64_t totalBytes = 0;
    };
    bool ShowDeviceSelectorUi(uint32_t user, const std::vector<DeviceChoice>& devices,
                              std::function<void(int row)> done);
    // XamLoaderRebootToDash with a 0x3FC-byte request (+4 command, +8 user index, kernel/xam_loader.cpp).
    void RebootToDash(const std::vector<uint8_t>& request);
    // The dashboard as a new process with this launch data (0x3FC bytes; nullptr: what the title set), the sign-ins
    // kept; this process exits (kernel/xam_loader.cpp).
    [[noreturn]] void RelaunchDashboard(const std::vector<uint8_t>* launchData);
}

namespace modules
{
    struct LoadedModule;
    struct RecompiledModule;
}

namespace kernel
{
    // CB_RECOMP_XAM=1: the console's own xam.xex runs (kernel/xam_recompiled.cpp). XamForward gives its export of
    // that name (null without it, or for a kernel import); every XAM GUEST_IMPORT calls it instead of its body.
    bool RecompiledXam();
    // A linked recompiled system module loaded on request (kernel/recompiled_modules.cpp; XexLoadImage).
    const modules::LoadedModule* LoadRecompiledSystemModule(const std::string& name, PPCContext& ctx);
    // Whether a linked recompiled system module has that file name (and its converted file is there).
    bool HasRecompiledSystemModule(const std::string& name);
    // The linked recompiled module with that file name (an older dashboard: "dash_6719.xex"), null if none.
    const modules::RecompiledModule* LinkedRecompiledModule(const std::string& name);
    // The older dashboard version booted (CB_DASH, main.cpp: "6719"), empty for 17559 (kernel/xam_blades.cpp). XAM's
    // sections of its time ("shrdres", "gamercrd") then come from SystemUpdate/converted/<section>_<version>.xzp when
    // the user has it.
    void SetOlderDashVersion(const std::string& version);
    const std::string& OlderDashVersion();
    std::filesystem::path OlderDashSection(const std::string& section);
    // The system update whose modules (boot animation, Guide) surround the older dashboard: its own version, or the
    // nearest update for a devkit-only build. Empty for 17559.
    std::string OlderDashSystemUpdate();
    // An older dashboard's XAM message as the 17559 XAM numbers it (unchanged for 17559 and for messages not known
    // to differ).
    uint32_t OlderDashMessage(uint32_t app, uint32_t message);
    // XexUnloadImage's last reference to a module LoadRecompiledSystemModule loaded: DllMain detach, then unloaded.
    bool UnloadRecompiledSystemModule(uint32_t handle, PPCContext& ctx);
    // The host file a guest file handle (NtCreateFile / NtOpenFile) has open; nullopt for another handle or a directory.
    std::optional<std::filesystem::path> HostFilePath(uint32_t handle);
    // Objects of the types xam.xex defines itself (kernel/xboxkrnl_drivers.cpp, ObCreateObject).
    uint32_t CreatedObjectType(uint32_t object);
    bool IsCreatedObjectType(uint32_t type);
    // The dispatcher object a wait on `object` waits on (ObGetWaitableObject's rule; the object itself for the
    // runtime's own objects).
    uint32_t WaitableObject(uint32_t object);
    // The controller driver for XAM (kernel/xboxkrnl_xinputd.cpp): starts binding the pads once XAM registers.
    void SetXInputdBindingCallback(uint32_t callback);
    // XAM's graphics notification routine (VdRegisterXamGraphicsNotification), 0 if none.
    uint32_t XamGraphicsNotification();
    PPCFunc* XamForward(const char* name);
    void LoadRecompiledXam(const std::string& path);
}

#define GUEST_IMPORT(name)                                                     \
    static void name##_impl(PPCContext& __restrict ctx, uint8_t* base);        \
    PPC_FUNC(__imp__##name)                                                    \
    {                                                                          \
        static PPCFunc* const forward = kernel::XamForward(#name);             \
        if (forward)                                                           \
            return forward(ctx, base);                                         \
        kernel::TraceImport(#name, ctx);                                       \
        name##_impl(ctx, base);                                                \
    }                                                                          \
    static void name##_impl([[maybe_unused]] PPCContext& __restrict ctx, [[maybe_unused]] uint8_t* base)

// An export reachable only through XexGetProcedureAddress (the title does not import it).
#define GUEST_EXPORT(library, ordinal, name)                                   \
    static void name##_impl(PPCContext& __restrict ctx, uint8_t* base);        \
    static PPC_FUNC(name##_export)                                             \
    {                                                                          \
        kernel::TraceImport(#name, ctx);                                       \
        name##_impl(ctx, base);                                                \
    }                                                                          \
    static kernel::DynamicExportRegistrar name##_registrar(library, ordinal, name##_export); \
    static void name##_impl([[maybe_unused]] PPCContext& __restrict ctx, [[maybe_unused]] uint8_t* base)

// A dynamic export whose lookup must succeed (a console has it) but whose behaviour is not implemented yet:
// calling it reports the call like an unimplemented import.
#define GUEST_EXPORT_UNIMPLEMENTED(library, ordinal, name)     GUEST_EXPORT(library, ordinal, name) { kernel::UnimplementedImport(#name, ctx); }

namespace kernel
{
    // Exports a module looks up at run time (XexGetProcedureAddress) without importing them get guest addresses in an
    // unused part of the kernel's fixed window (where the console's kernel code lives); the shared indirect-call table
    // maps each address to its host function. No guest code or data is ever placed there.
    constexpr uint32_t kDynamicExportStart = 0x80100000;
    constexpr uint32_t kDynamicExportEnd = 0x80110000;

    void RegisterDynamicExport(const char* library, uint16_t ordinal, PPCFunc* func);
    // Guest-callable address for an export the title does not import, or 0 if the runtime has no implementation.
    uint32_t BindDynamicExport(const std::string& library, uint16_t ordinal);

    struct DynamicExportRegistrar
    {
        DynamicExportRegistrar(const char* library, uint16_t ordinal, PPCFunc* func) { RegisterDynamicExport(library, ordinal, func); }
    };

    // NTSTATUS values
    constexpr uint32_t kStatusSuccess = 0x00000000;
    constexpr uint32_t kStatusTimeout = 0x00000102;
    constexpr uint32_t kStatusUserApc = 0x000000C0;
    constexpr uint32_t kStatusUnsuccessful = 0xC0000001;
    constexpr uint32_t kStatusNotImplemented = 0xC0000002;
    constexpr uint32_t kStatusInvalidParameter = 0xC000000D;
    constexpr uint32_t kStatusNoMemory = 0xC0000017;

    // Dispatcher object types (DISPATCHER_HEADER.Type)
    constexpr uint8_t kObjectEventNotification = 0;
    constexpr uint8_t kObjectEventSynchronization = 1;
    constexpr uint8_t kObjectMutant = 2;
    constexpr uint8_t kObjectProcess = 3;
    constexpr uint8_t kObjectQueue = 4;
    constexpr uint8_t kObjectSemaphore = 5;
    constexpr uint8_t kObjectThread = 6;

    // KPCR: what r13 points to on every guest thread.
    namespace pcr
    {
        constexpr uint32_t kSize = 0x2D8;
        constexpr uint32_t kTlsPointer = 0x000;      // static TLS block (declspec(thread) data + slots)
        constexpr uint32_t kDpcProcessType = 0x00C;  // process type reported while a DPC runs
        constexpr uint32_t kCurrentIrql = 0x018;
        constexpr uint32_t kSelfPointer = 0x030;     // 64-bit pointer to this PCR
        constexpr uint32_t kStackBase = 0x070;       // high address
        constexpr uint32_t kStackEnd = 0x074;        // low address
        constexpr uint32_t kPrcb = 0x100;            // embedded KPRCB
        constexpr uint32_t kCurrentThread = 0x100;   // KPRCB.CurrentThread
        constexpr uint32_t kCurrentCpu = 0x10C;      // KPRCB.CurrentCpu
        constexpr uint32_t kDpcActive = 0x150;       // KPRCB.DpcActive
        constexpr uint32_t kPrcbPointer = 0x2A8;
    }

    // KTHREAD
    namespace kthread
    {
        constexpr uint32_t kSize = 0xAB0;
        constexpr uint32_t kMutantList = 0x010;
        constexpr uint32_t kStackBase = 0x05C;
        constexpr uint32_t kStackLimit = 0x060;
        constexpr uint32_t kStackKernel = 0x064;
        constexpr uint32_t kTlsAddress = 0x068;
        constexpr uint32_t kState = 0x06C;
        constexpr uint32_t kPriority = 0x070;
        constexpr uint32_t kProcessTypeCopy = 0x072;
        constexpr uint32_t kProcessType = 0x073;
        constexpr uint32_t kProcess = 0x084;
        constexpr uint32_t kMsrMask = 0x09C;
        constexpr uint32_t kBasePriority = 0x0B9;
        constexpr uint32_t kSuspendCount = 0x0BC;
        constexpr uint32_t kCurrentCpu = 0x0BF;
        constexpr uint32_t kPrcbPointer = 0x0C0;
        constexpr uint32_t kPrcbPointer2 = 0x0C4;
        constexpr uint32_t kStackAllocBase = 0x0D0;
        constexpr uint32_t kCreateTime = 0x130;
        constexpr uint32_t kThreadId = 0x14C;
        constexpr uint32_t kStartAddress = 0x150;
        constexpr uint32_t kLastError = 0x160;
        constexpr uint32_t kCreationFlags = 0x16C;
        constexpr uint32_t kFpuExceptionsOn = 0x071;
        constexpr uint32_t kApcDisableCount = 0x0B0;
        constexpr uint32_t kExitStatus = 0x140;
    }

    constexpr uint32_t kKProcessSize = 0x60;

    // Process types (KTHREAD.ProcessType)
    constexpr uint8_t kProcessTypeIdle = 0;
    constexpr uint8_t kProcessTypeTitle = 1;
    constexpr uint8_t kProcessTypeSystem = 2;

    struct ThreadParams
    {
        uint32_t stackSize = 0;
        uint32_t startAddress = 0;
        uint32_t startContext = 0;
        uint32_t xapiThreadStartup = 0; // if set, called as startup(startAddress, startContext)
        uint32_t preStart = 0;          // if set, called first with no arguments (XexStartExecutable's routine)
        uint32_t creationFlags = 0;
        bool mainThread = false;
        // KPROCESS the thread belongs to: kProcessTypeTitle or kProcessTypeSystem. ExCreateThread puts a thread in the
        // system process when CreationFlags has 0x2, otherwise in its creator's process (XAM's own threads without
        // the flag, e.g. its D3D interrupt thread, must stay system threads); threads the runtime creates without a
        // guest creator use the title process unless they say otherwise.
        uint8_t processType = 0; // 0 = decide as above
    };

    // A queued user-mode APC: NormalRoutine(NormalContext, SystemArgument1, SystemArgument2). A KAPC's
    // (KeInsertQueueApc) also has its KernelRoutine(Apc, &NormalRoutine, &NormalContext, &SystemArgument1,
    // &SystemArgument2), run first; it may change or clear the normal routine.
    struct UserApc
    {
        uint32_t routine = 0;
        uint32_t context = 0;
        uint32_t argument1 = 0;
        uint32_t argument2 = 0;
        uint32_t kernelRoutine = 0;
        uint32_t apc = 0; // the KAPC (its Inserted flag is cleared when it is delivered)
    };

    // Queues a DPC from the runtime (KeInsertQueueDpc's rules): false if it is already queued.
    bool QueueDpc(uint32_t dpc, uint32_t argument1, uint32_t argument2);

    struct GuestThread
    {
        ThreadParams params;
        uint32_t id = 0;
        uint32_t threadObject = 0; // KTHREAD
        uint32_t pcr = 0;
        uint32_t tlsStatic = 0;   // PCR.TlsPointer
        uint32_t tlsSlots = 0;    // KTHREAD.TlsAddress (array of 4-byte slots)
        uint32_t stackAllocation = 0;
        uint32_t stackBase = 0;   // high
        uint32_t stackLimit = 0;  // low
        PPCContext* context = nullptr;
        void* hostHandle = nullptr;
        std::atomic<bool> started = false;
        std::atomic<bool> finished = false;
        uint32_t exitCode = 0;
        uint32_t affinity = 0x3F; // all six hardware threads
        std::vector<UserApc> userApcs; // pending user APCs (guarded by the dispatcher lock)
        // The import this thread entered last, its caller and first argument (for the stall watchdog).
        std::atomic<const char*> lastImport = nullptr;
        std::atomic<uint32_t> lastImportCaller = 0;
        std::atomic<uint32_t> lastImportArgument = 0;
        // ExTerminateTitleProcess: the thread ends at its next kernel call (a wait it is blocked in ends first).
        std::atomic<bool> terminating = false;
    };

    // Thrown by ExTerminateThread to unwind the host stack back to the thread's entry.
    struct ThreadExit
    {
        uint32_t status;
    };

    // All guest threads ever created (threads are never destroyed while the process runs).
    GuestThread* FindThread(uint32_t threadObject);
    // Ends a title thread the way ExTerminateTitleProcess does: marks it and wakes it from a wait (it unwinds with
    // ThreadExit there, or at its next kernel call). kernel/title_process.cpp.
    void WakeForTermination(GuestThread* thread);
    // The title process's memory (allocated by its threads), freed when it terminates (kernel/title_process.cpp).
    // Audio clients (XAudioRegisterRenderDriverClient) whose callback is code in [start, end) go: their title ended.
    void UnregisterAudioClientsIn(uint32_t start, uint32_t end);
    void TrackTitleVirtual(uint32_t base);
    void UntrackTitleVirtual(uint32_t base);
    void TrackTitlePhysical(uint32_t windowAddress);
    void UntrackTitlePhysical(uint32_t windowAddress);
    // The registered title terminate notifications (kernel/xboxkrnl_misc.cpp).
    std::vector<uint32_t> TitleTerminateRegistrations();
    // An executable ExTerminateTitleProcess unloaded, loaded again by XexLoadExecutable; null when none of that name.
    const modules::LoadedModule* ReloadTerminatedExecutable(const std::string& name);
    template<class F> void ForEachThread(F&& f);
    void ForEachThreadImpl(const std::function<void(GuestThread*)>& f);
    template<class F> void ForEachThread(F&& f) { ForEachThreadImpl(std::function<void(GuestThread*)>(std::forward<F>(f))); }
    uint32_t TlsSlotCount();

    // Set up the kernel state that does not depend on threads (variables, title and system process objects). Call
    // before loading modules. Without a title (the boot animation alone) the title ID is 0, TLS has the default slots
    // and XexExecutableModuleHandle is null until SetExecutable.
    void Initialize(uint32_t titleId, uint32_t tlsSlotCount, uint32_t tlsDataSize, uint32_t tlsRawAddress, uint32_t tlsRawSize);
    // Makes a loaded module the title (XAM's loader on the console): XexExecutableModuleHandle,
    // RtlImageXexHeaderField(NULL..), the title ID and the title process's TLS (threads created afterwards).
    void SetExecutable(uint32_t moduleHandle, uint32_t xexHeader, uint32_t titleId, uint32_t tlsSlotCount,
        uint32_t tlsDataSize, uint32_t tlsRawAddress, uint32_t tlsRawSize);
    // KeTimeStampBundle's interrupt time: 100 ns units since boot.
    uint64_t InterruptTime();
    // The running title's ID (from the XEX execution info).
    uint32_t TitleId();
    // Whether a profile is signed in to controller slot `user` (0-3); kernel/users.h has the profiles.
    bool UserSignedIn(uint32_t user);
    // A NUL-terminated big-endian UTF-16 string from guest memory (at most maxLength characters).
    std::u16string ReadGuestWideString(uint32_t address, size_t maxLength);
    // Queues a XAM notification (XN_*) for every listener whose areas include it (xam_notify.cpp).
    void BroadcastNotification(uint32_t id, uint32_t param);
    // Hooks the controllers' Guide button to the Xbox Guide (xam_ui.cpp). Call after input::Initialize.
    void InitializeGuide();
    // Create Profile (XamShowCreateProfileUI, the Guide's sign-in pane; xam_ui.cpp): checks the console's gamertag rules,
    // creates the offline profile on the hard drive and lists it; its index into users::Profiles(), else nullopt and
    // the reason to show.
    std::optional<size_t> CreateLocalProfile(const std::u16string& gamertag, std::u16string& reason);
    // CB_INPUT_SCRIPT's SHOW=<screen>[/<hex>] (xam_ui.cpp): opens a system screen for `user` as a title's XamShow*UI
    // would, `screen` its short name ("games", "gamercard", ...) and an optional hex argument (a XUID).
    void ShowSystemScreenForTest(uint32_t user, const std::string& screen);
    // XamIsUIActive: a system UI is up, from XN_SYS_UI 1 to XN_SYS_UI 0 (xam_ui.cpp).
    bool SystemUiActive();
    // Commits the APU register block and allocates the XMA context array.
    void InitializeAudio();

    // Guest address for an imported kernel variable, or 0 if the runtime does not provide it.
    uint32_t ResolveVariable(const std::string& library, uint16_t ordinal);
    // Records the code thunk of a function import (lets XexGetProcedureAddress return a callable address).
    void RegisterImportThunk(const std::string& library, uint16_t ordinal, uint32_t thunk);
    uint32_t LookupImportThunk(const std::string& library, uint16_t ordinal);
    // Loader data table entry (HMODULE) of the executable.
    uint32_t ExecutableModuleHandle();

    GuestThread* CreateThread(const ThreadParams& params);
    // The hardware thread (0-5) a guest thread runs on: KPRCB.CurrentCpu in its PCR (what code reads through r13,
    // e.g. per-CPU tables) and KTHREAD.CurrentCpu. A new thread starts on the lowest CPU of its creation affinity
    // mask, else on its creator's CPU (Xbox 360 threads without an explicit processor stay on their parent's hardware
    // thread), else on CPU 0.
    void SetCurrentCpu(GuestThread* thread, uint8_t cpu);
    uint8_t CurrentCpu(const GuestThread* thread);
    // Starts the host thread (once); a thread created suspended is started by NtResumeThread.
    void StartThread(GuestThread* thread);
    GuestThread* CurrentThread();
    // Makes the calling host thread (a runtime-owned thread such as the GPU or vblank thread) run as the given guest
    // thread, which must have been created with CreateThread and not started. Returns its register context.
    PPCContext* AttachHostThread(GuestThread* thread);

    // Starts a runtime-owned host thread that will run guest code (via AttachHostThread). Like guest threads it gets a
    // 16 MiB host stack, since recompiled code nests one host frame per guest call.
    void StartHostThread(std::function<void()> body);

    // Calls a guest routine at DISPATCH_LEVEL as interrupt/DPC code runs on `cpu`: raises the PCR's IRQL, marks a DPC
    // active, reports `processType` (KeGetCurrentProcessType while the DPC is active), then restores the previous
    // state. Arguments go in r3, r4, ...
    void CallGuestAtDispatchLevel(GuestThread* thread, PPCContext& ctx, uint32_t address,
        std::initializer_list<uint32_t> args, uint32_t cpu, uint8_t processType = kProcessTypeTitle);
    // The process type of the calling guest thread (title if the caller is not a guest thread).
    uint8_t CurrentProcessType();

    // XEX optional-header lookup with RtlImageXexHeaderField semantics (0 if absent), and the executable's header.
    uint32_t XexHeaderField(uint32_t xexHeader, uint32_t key);
    uint32_t ExecutableXexHeader();

    // Arguments beyond the eighth are passed on the stack: argument n (n >= 8, zero-based) is the low word of the
    // 8-byte slot at r1 + 0x50 + (n - 8) * 8.
    inline uint32_t StackArg32(const PPCContext& ctx, uint32_t n)
    {
        return guest::g_memory.Read32(ctx.r1.u32 + 0x54 + (n - 8) * 8);
    }

    // Calls a guest function by address through the recompiler's lookup table.
    void CallGuest(PPCContext& ctx, uint32_t address);
    // CB_KERNEL_TEST=1 (kernel/kernel_tests.cpp): returns the number of failed checks.
    int RunKernelTests();
}
