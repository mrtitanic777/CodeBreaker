// Kernel variables the game imports from xboxkrnl.exe, and the kernel process object.
// Fixed addresses match where Xenia Canary (and a retail kernel) place them, since titles may compare against them.

#include "kernel/kernel.h"
#include "guest_clock.h"
#include "kernel/objects.h"
#include "pool.h"
#include "settings.h"

#include <windows.h>
#include <timeapi.h>

#include <map>
#include <thread>

namespace kernel
{
    void InitializeThreading(uint32_t tlsSlotCount, uint32_t tlsDataSize, uint32_t tlsRawAddress, uint32_t tlsRawSize,
        uint32_t titleProcess, uint32_t systemProcess);
    void SetTitleTls(uint32_t tlsSlotCount, uint32_t tlsDataSize, uint32_t tlsRawAddress, uint32_t tlsRawSize);

    namespace
    {
        // xboxkrnl.exe ordinals of the variables TU46, bootanim.xex and dash.xex import.
        constexpr uint16_t kExConsoleGameRegion = 12;
        constexpr uint16_t kExEventObjectType = 14;
        constexpr uint16_t kExThreadObjectType = 27;
        constexpr uint16_t kIoDeviceObjectType = 58;
        // Imported by xam.xex (CB_RECOMP_XAM, kernel/xam_recompiled.cpp).
        constexpr uint16_t kExMutantObjectType = 18;
        constexpr uint16_t kExSemaphoreObjectType = 23;
        constexpr uint16_t kExTimerObjectType = 28;
        constexpr uint16_t kIoFileObjectType = 62;
        constexpr uint16_t kUsbdBootEnumerationDoneEvent = 731;
        constexpr uint16_t kStfsDeviceErrorEvent = 732;
        constexpr uint16_t kUsbdDriverLoadRequiredEvent = 753;
        constexpr uint16_t kKeDebugMonitorData = 89;
        constexpr uint16_t kKeTimeStampBundle = 173;
        constexpr uint16_t kXboxHardwareInfo = 342;
        constexpr uint16_t kXboxKrnlBaseVersion = 343;
        constexpr uint16_t kXboxKrnlVersion = 344;
        constexpr uint16_t kXexExecutableModuleHandle = 403;
        constexpr uint16_t kExLoadedCommandLine = 430;
        constexpr uint16_t kVdGlobalDevice = 446;
        constexpr uint16_t kVdGlobalXamDevice = 447;
        constexpr uint16_t kVdGpuClockInMHz = 448;
        constexpr uint16_t kVdHSIOCalibrationLock = 449;
        constexpr uint16_t kKeCertMonitorData = 614;

        // Fixed kernel addresses (all inside the committed 0x80000000-0x8020FFFF window).
        constexpr uint32_t kFixedWindowStart = 0x80000000;
        constexpr uint32_t kFixedWindowSize = 0x00210000;
        constexpr uint32_t kAddrXboxKrnlVersion = 0x8004045C;
        constexpr uint32_t kAddrExConsoleGameRegion = 0x80205150;
        constexpr uint32_t kAddrXboxHardwareInfo = 0x801D0030;
        constexpr uint16_t kBootLoaderFlags = 0x1000;
        constexpr uint32_t kHypervisorPage = 0x8E030000;
        constexpr uint32_t kHypervisorPageSize = 0x10000;
        constexpr uint32_t kAddrKeCertMonitorData = 0x80207A60;
        constexpr uint32_t kAddrKeDebugMonitorData = 0x80207A64;
        constexpr uint32_t kAddrExLoadedCommandLine = 0x80207C60;
        constexpr uint32_t kAddrXexExecutableModuleHandle = 0x802080D4;
        constexpr uint32_t kAddrVdGpuClockInMHz = 0x801D0E94;
        constexpr uint32_t kAddrVdHSIOCalibrationLock = 0x801D1210;
        constexpr uint32_t kAddrVdGlobalDevice = 0x801E6FC4;
        constexpr uint32_t kAddrVdGlobalXamDevice = 0x801E6FC8;

        constexpr uint32_t kObjectTypeSize = 0x20;

        std::map<uint16_t, uint32_t> g_kernelVariables;
        uint32_t g_executableXexHeader = 0;
        uint32_t g_executableLdrEntry = 0;
        std::map<std::pair<std::string, uint16_t>, uint32_t> g_importThunks;
        uint32_t g_timeStampBundle = 0;

        // KeTimeStampBundle: InterruptTime (u64, 100 ns since boot), SystemTime (u64 FILETIME), TickCount (u32, ms).
        // The console kernel refreshes it from its clock interrupt; a host thread does the same here every 1 ms.
        void UpdateTimeStampBundle(uint64_t, uint64_t)
        {
            auto& mem = guest::g_memory;
            const uint64_t interrupt100ns = guest_clock::Nanoseconds() / 100; // src/guest_clock.h
            FILETIME ft;
            GetSystemTimeAsFileTime(&ft);
            mem.Write64(g_timeStampBundle + 0x00, interrupt100ns);
            mem.Write64(g_timeStampBundle + 0x08, uint64_t(ft.dwHighDateTime) << 32 | ft.dwLowDateTime);
            mem.Write32(g_timeStampBundle + 0x10, uint32_t(interrupt100ns / 10000));
        }
    }

    namespace
    {
        uint32_t g_titleId = 0;
    }

    uint64_t InterruptTime()
    {
        return guest::g_memory.Read64(g_timeStampBundle + 0x00);
    }

    uint32_t TitleId()
    {
        return g_titleId;
    }

    void Initialize(uint32_t titleId, uint32_t tlsSlotCount, uint32_t tlsDataSize, uint32_t tlsRawAddress, uint32_t tlsRawSize)
    {
        g_titleId = titleId;
        auto& mem = guest::g_memory;
        mem.HostCommit(kFixedWindowStart, kFixedWindowSize);
        // The hypervisor's read-only security page the kernel maps at 0x8E038000: XAM and the dash read console flags
        // there directly (0x8E038610, 0x614, 0x630, 0x678: development-kit features, the dash's profiler hooks
        // (0x630 bit 0x40), Live-hive debugging (0x630 bit 0x20), parental-control set-up). A retail console has
        // none of them set: zeros, which is also what Xenia and Nexia360 map there for this dashboard.
        mem.HostCommit(kHypervisorPage, kHypervisorPageSize);

        // Kernel process objects for the title and for the system (the kernel's and XAM's threads, the boot animation).
        const uint32_t process = mem.SystemAlloc(kKProcessSize);
        mem.Write8(process, kObjectProcess);
        const uint32_t systemProcess = mem.SystemAlloc(kKProcessSize);
        mem.Write8(systemProcess, kObjectProcess);

        // flags: storage present (0x20). With the console's own XAM (CB_RECOMP_XAM) also 0x200, which every retail
        // console reports and which xam.xex's start-up requires for its UI (0x816966E8: the \System??\media: link and
        // the system UI's task, 0x816CEBD8).
        mem.Write32(kAddrXboxHardwareInfo + 0, kernel::RecompiledXam() ? 0x220 : 0x20);
        mem.Write8(kAddrXboxHardwareInfo + 4, 6);     // hardware threads
        // +0xE: the boot loader's flags. 0x1000 = the flash's patch slot 1 is active: the kernel and XAM use the
        // flash files *.xexp1 / *.xttp1 (the user's 17559 NAND has slot 1; 0x2000 would be slot 2).
        mem.Write16(kAddrXboxHardwareInfo + 0xE, kBootLoaderFlags);
        g_kernelVariables[kXboxHardwareInfo] = kAddrXboxHardwareInfo;

        // Final retail kernel version 2.0.17559.0 (major, minor, build, qfe as big-endian u16s).
        mem.Write16(kAddrXboxKrnlVersion + 0, 2);
        mem.Write16(kAddrXboxKrnlVersion + 2, 0);
        // An older dashboard (CB_DASH) runs on the kernel of its own version, as on the console.
        const std::string& older = kernel::OlderDashVersion();
        mem.Write16(kAddrXboxKrnlVersion + 4, older.empty() ? 17559 : uint16_t(std::stoi(older)));
        mem.Write16(kAddrXboxKrnlVersion + 6, 0);
        g_kernelVariables[kXboxKrnlVersion] = kAddrXboxKrnlVersion;
        // XboxKrnlBaseVersion: the launch kernel 2.0.1888.0 that retail system updates build on (same layout as
        // XboxKrnlVersion). dash.xex imports it; no use of it was found in its decompilation.
        const uint32_t baseVersion = mem.SystemAlloc(8);
        mem.Write16(baseVersion + 0, 2);
        mem.Write16(baseVersion + 2, 0);
        mem.Write16(baseVersion + 4, 1888);
        mem.Write16(baseVersion + 6, 0);
        g_kernelVariables[kXboxKrnlBaseVersion] = baseVersion;

        // ExConsoleGameRegion (u16, XC_GAME_REGION_*): the kernel loads it from the secured XConfig (category 2,
        // setting 3); dash.xex reads it (0x0101 Japan picks its NTSC-J video tables) and writes it back after
        // ExSetXConfigSetting(2, 3). Address as in Xenia Canary.
        mem.Write16(kAddrExConsoleGameRegion, uint16_t(Settings().gameRegion));
        g_kernelVariables[kExConsoleGameRegion] = kAddrExConsoleGameRegion;

        // Debug and certificate monitor pointers are null on a retail console.
        mem.Write32(kAddrKeDebugMonitorData, 0);
        mem.Write32(kAddrKeCertMonitorData, 0);
        g_kernelVariables[kKeDebugMonitorData] = kAddrKeDebugMonitorData;
        g_kernelVariables[kKeCertMonitorData] = kAddrKeCertMonitorData;

        const char commandLine[] = "\"default.xex\"";
        memcpy(mem.Translate(kAddrExLoadedCommandLine), commandLine, sizeof(commandLine));
        g_kernelVariables[kExLoadedCommandLine] = kAddrExLoadedCommandLine;

        // XexExecutableModuleHandle -> the executable's loader data table entry (SetExecutable); null until then.
        mem.Write32(kAddrXexExecutableModuleHandle, 0);
        g_kernelVariables[kXexExecutableModuleHandle] = kAddrXexExecutableModuleHandle;

        // Object type descriptors: titles only use their addresses (to name the expected type of a handle).
        g_kernelVariables[kExEventObjectType] = mem.SystemAlloc(kObjectTypeSize);
        g_kernelVariables[kExThreadObjectType] = mem.SystemAlloc(kObjectTypeSize);
        // IoDeviceObjectType: dash.xex passes it to ObReferenceObjectByName to open a device by name (0x92267A90).
        g_kernelVariables[kIoDeviceObjectType] = mem.SystemAlloc(kObjectTypeSize);
        g_kernelVariables[kExMutantObjectType] = mem.SystemAlloc(kObjectTypeSize);
        g_kernelVariables[kExSemaphoreObjectType] = mem.SystemAlloc(kObjectTypeSize);
        g_kernelVariables[kExTimerObjectType] = mem.SystemAlloc(kObjectTypeSize);
        g_kernelVariables[kIoFileObjectType] = mem.SystemAlloc(kObjectTypeSize);
        // The kernel's driver events XAM waits on (KEVENTs, notification type): the USB stack's boot enumeration is
        // over at once here (no USB bus to walk: signalled); no STFS device error and no USB driver to load (clear).
        for (const auto [ordinal, signalled] : { std::pair{ kUsbdBootEnumerationDoneEvent, 1u },
                                                 std::pair{ kStfsDeviceErrorEvent, 0u },
                                                 std::pair{ kUsbdDriverLoadRequiredEvent, 0u } })
        {
            const uint32_t event = mem.SystemAlloc(0x10);
            InitializeDispatcherHeader(event, kObjectEventNotification, signalled);
            g_kernelVariables[ordinal] = event;
        }

        // Video globals at their kernel addresses. VdGlobalDevice points at a zeroed 40-byte physical block;
        // VdHSIOCalibrationLock is a critical section (spin count 10000).
        const uint32_t globalDevice = mem.physical.Allocate(0x1000, 0x1000, 0, 0x1FFFFFFF, true);
        if (globalDevice == UINT32_MAX)
            Fatal("Unable to allocate VdGlobalDevice");
        mem.CommitPhysical(globalDevice, 0x1000);
        mem.Write32(kAddrVdGlobalDevice, guest::PhysicalToWindow(guest::kPhysicalWindow4k, globalDevice));
        mem.Write32(kAddrVdGlobalXamDevice, 0);
        mem.Write32(kAddrVdGpuClockInMHz, 500);
        InitializeDispatcherHeader(kAddrVdHSIOCalibrationLock, kObjectEventSynchronization, 0);
        mem.Write8(kAddrVdHSIOCalibrationLock + header::kAbsolute, uint8_t((10000 + 255) >> 8));
        mem.Write32(kAddrVdHSIOCalibrationLock + 0x10, 0xFFFFFFFF); // LockCount
        mem.Write32(kAddrVdHSIOCalibrationLock + 0x14, 0);          // RecursionCount
        mem.Write32(kAddrVdHSIOCalibrationLock + 0x18, 0);          // OwningThread
        g_kernelVariables[kVdGlobalDevice] = kAddrVdGlobalDevice;
        g_kernelVariables[kVdGlobalXamDevice] = kAddrVdGlobalXamDevice;
        g_kernelVariables[kVdGpuClockInMHz] = kAddrVdGpuClockInMHz;
        g_kernelVariables[kVdHSIOCalibrationLock] = kAddrVdHSIOCalibrationLock;

        g_timeStampBundle = mem.SystemAlloc(0x18);
        g_kernelVariables[kKeTimeStampBundle] = g_timeStampBundle;
        LARGE_INTEGER boot, frequency;
        QueryPerformanceCounter(&boot);
        QueryPerformanceFrequency(&frequency);
        UpdateTimeStampBundle(boot.QuadPart, frequency.QuadPart);
        std::thread([b = uint64_t(boot.QuadPart), f = uint64_t(frequency.QuadPart)]
            {
                timeBeginPeriod(1);
                for (;;)
                {
                    Sleep(1);
                    UpdateTimeStampBundle(b, f);
                }
            }).detach();

        // Kernel pool (ExAllocatePool*, strings allocated by Rtl* conversions).
        constexpr uint32_t kPoolSize = 16 * 1024 * 1024;
        KernelPool().Initialize(mem.SystemAlloc(kPoolSize, 0x10000), kPoolSize);

        InitializeThreading(tlsSlotCount, tlsDataSize, tlsRawAddress, tlsRawSize, process, systemProcess);
        Log("Kernel initialized for title {:08X}", titleId);
    }

    void SetExecutable(uint32_t moduleHandle, uint32_t xexHeader, uint32_t titleId, uint32_t tlsSlotCount,
        uint32_t tlsDataSize, uint32_t tlsRawAddress, uint32_t tlsRawSize)
    {
        g_titleId = titleId;
        SetTitleTls(tlsSlotCount, tlsDataSize, tlsRawAddress, tlsRawSize);
        // RtlImageXexHeaderField and XexGetModuleHandle read the header through the entry's XexHeaderBase.
        g_executableXexHeader = xexHeader;
        g_executableLdrEntry = moduleHandle;
        guest::g_memory.Write32(kAddrXexExecutableModuleHandle, moduleHandle);
    }

    void RegisterImportThunk(const std::string& library, uint16_t ordinal, uint32_t thunk)
    {
        g_importThunks[{ library, ordinal }] = thunk;
    }

    uint32_t LookupImportThunk(const std::string& library, uint16_t ordinal)
    {
        auto it = g_importThunks.find({ library, ordinal });
        return it == g_importThunks.end() ? 0 : it->second;
    }

    uint32_t ExecutableModuleHandle()
    {
        return g_executableLdrEntry;
    }

    uint32_t ExecutableXexHeader()
    {
        return g_executableXexHeader;
    }

    uint32_t ResolveVariable(const std::string& library, uint16_t ordinal)
    {
        if (library != "xboxkrnl.exe")
            return 0;
        auto it = g_kernelVariables.find(ordinal);
        return it == g_kernelVariables.end() ? 0 : it->second;
    }

    uint16_t BootLoaderFlags()
    {
        return kBootLoaderFlags;
    }
}
