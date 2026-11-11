// The title host: runs one statically recompiled Xbox 360 title (its default.xex, recompiled by
// XenonRecomp into the library linked into this program) on CodeBreaker's Xbox 360 runtime.
//
// Usage: <title>.exe [title folder]
// The title folder holds the title's files as the disc or package has them, with default.xex
// rewritten in basic form (codebreaker.formats.xex.basic); it is the title's game: device. Without
// an argument it is the working directory if that has default.xex, else the nearest parent of the
// executable that has it. console_settings.ini and content/ (the hard drive: saves, profiles) are
// kept there too. A nand/ folder, the console's flash files from the user's own NAND dump, is
// mounted as \Device\Flash when present (the system UI's fonts and skin come from it).
#include "gpu/gpu.h"
#include "gpu/presenter.h"
#include "gpu/renderer.h"
#include "input.h"
#include "kernel/kernel.h"
#include "kernel/storage.h"
#include "modules.h"
#include "settings.h"
#include "vfs.h"

#include <windows.h>

#include <filesystem>
#include <format>
#include <thread>

// The recompiled title linked into this program (its library's module_info.cpp).
extern const modules::RecompiledModule g_titleModule;

namespace
{
    // The host call stack at a fault, as offsets into the program: its frames are the recompiled functions
    // (sub_<address>), so symbolized it is the guest call chain, also where the guest's LR does not say it (code
    // the runtime called, an APC or a callback). llvm-symbolizer --obj=<program> --relative-address reads them.
    std::string HostStack(const CONTEXT& faulting)
    {
        CONTEXT context = faulting;
        const auto module = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
        std::string out;
        for (int i = 0; i < 24 && context.Rip; i++)
        {
            out += std::format(" +{:X}", context.Rip - module);
            DWORD64 imageBase = 0;
            RUNTIME_FUNCTION* entry = RtlLookupFunctionEntry(context.Rip, &imageBase, nullptr);
            if (!entry)
                break;
            void* handlerData = nullptr;
            DWORD64 frame = 0;
            RtlVirtualUnwind(UNW_FLAG_NHANDLER, imageBase, context.Rip, entry, &context, &handlerData, &frame, nullptr);
        }
        return out;
    }

    // Reports a fault inside the guest address space with the guest address and the guest call site.
    LONG CALLBACK GuestFaultHandler(EXCEPTION_POINTERS* info)
    {
        const auto* record = info->ExceptionRecord;
        const auto* thread = kernel::CurrentThread();
        const bool fatalCode = record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION ||
            record->ExceptionCode == EXCEPTION_STACK_OVERFLOW || record->ExceptionCode == EXCEPTION_ILLEGAL_INSTRUCTION;
        const void* target = record->NumberParameters >= 2 ? reinterpret_cast<const void*>(record->ExceptionInformation[1]) : nullptr;
        uint32_t tableSlot = 0;
        if (record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && guest::g_memory.FunctionTableAddress(target, tableSlot))
            Fatal("Indirect call to {:08X}, outside every recompiled module, on thread {} (lr={:08X}, r1={:08X})",
                tableSlot, thread ? thread->id : 0,
                thread && thread->context ? uint32_t(thread->context->lr) : 0,
                thread && thread->context ? thread->context->r1.u32 : 0);
        if (fatalCode && !(record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && guest::g_memory.IsGuestPointer(target)))
            Fatal("Host exception {:08X} at rip {:p} (program +{:X}; target {:p}) on guest thread {} (lr={:08X}, "
                "r1={:08X}; host stack{})", uint32_t(record->ExceptionCode), record->ExceptionAddress,
                reinterpret_cast<uintptr_t>(record->ExceptionAddress) - reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr)),
                target, thread ? thread->id : 0,
                thread && thread->context ? uint32_t(thread->context->lr) : 0,
                thread && thread->context ? thread->context->r1.u32 : 0, HostStack(*info->ContextRecord));
        if (record->ExceptionCode != EXCEPTION_ACCESS_VIOLATION || target == nullptr)
            return EXCEPTION_CONTINUE_SEARCH;
        static const char* kinds[] = { "read", "write", "?", "?", "?", "?", "?", "?", "execute" };
        // The recompiled function the fault is in: the one whose host code starts nearest below rip.
        uint32_t function = 0;
        uintptr_t best = 0;
        const auto rip = reinterpret_cast<uintptr_t>(record->ExceptionAddress);
        for (const modules::LoadedModule* m : modules::All())
            if (m->recompiled)
                for (const PPCFuncMapping* f = m->recompiled->mappings; f->host; f++)
                    if (const auto host = reinterpret_cast<uintptr_t>(f->host); host <= rip && host > best)
                    {
                        best = host;
                        function = uint32_t(f->guest);
                    }
        Fatal("Guest memory {} fault at {:08X} in guest function {:08X} (host +{:X}) on thread {} (last guest call "
            "site lr={:08X}, r1={:08X}; stack {}; host stack{})",
            kinds[record->ExceptionInformation[0] & 0xF], guest::g_memory.ToGuest(target), function, rip - best,
            thread ? thread->id : 0,
            thread && thread->context ? uint32_t(thread->context->lr) : 0,
            thread && thread->context ? thread->context->r1.u32 : 0,
            thread && thread->context ? kernel::GuestBacktrace(*thread->context) : std::string(),
            HostStack(*info->ContextRecord));
    }

    std::filesystem::path FindTitleFolder(int argc, char** argv)
    {
        namespace fs = std::filesystem;
        if (argc > 1)
            return fs::absolute(argv[1]);
        if (fs::exists("default.xex"))
            return fs::current_path();
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(nullptr, exePath, MAX_PATH);
        fs::path dir = fs::path(exePath).parent_path();
        for (int i = 0; i < 5 && !dir.empty(); i++, dir = dir.parent_path())
        {
            if (fs::exists(dir / "default.xex"))
                return dir;
            if (dir == dir.parent_path())
                break;
        }
        Fatal("No default.xex in the working directory or beside the executable (or its parent folders). "
            "Pass the title folder as the first argument.");
    }
}

int main(int argc, char** argv)
{
    const std::filesystem::path folder = FindTitleFolder(argc, argv);
    std::filesystem::current_path(folder);
    AddVectoredExceptionHandler(1, GuestFaultHandler);
    LoadSettings(folder.string());

    // The title's own files are its game: device (and the disc drive it was read from).
    vfs::MountDevice("game", folder, true);
    vfs::MountDevice("d", folder, true);
    vfs::MountDevice("\\Device\\Cdrom0", folder, true);
    // The console's flash file system, when the user has extracted it from their NAND dump.
    if (const std::filesystem::path flash = folder / "nand"; std::filesystem::exists(flash))
    {
        vfs::MountDevice("\\Device\\Flash", flash, true);
        vfs::CreateLink("\\SystemRoot", "\\Device\\Flash");
    }

    guest::g_memory.Reserve();
    modules::Initialize();
    input::Initialize();
    gpu::Initialize();
    gpu::StartPresenter(Settings().videoWidth, Settings().videoHeight);
    kernel::storage::Initialize();
    kernel::Initialize(0, 0, 0, 0, 0);
    kernel::InitializeAudio();
    kernel::InitializeGuide();

    const modules::LoadedModule& loaded = modules::Load((folder / "default.xex").string(), g_titleModule);
    const xex::Module& title = loaded.xex;
    kernel::SetExecutable(loaded.handle, loaded.xexHeader, title.titleId, title.tls.slotCount, title.tls.dataSize,
        title.tls.rawDataAddress, title.tls.rawDataSize);
    Log("Title {:08X}: entry {:08X}, stack {:X}, TLS {} slots, {:X} bytes of data ({:X} initialised at {:08X})",
        title.titleId, title.entryPoint, title.defaultStackSize, title.tls.slotCount, title.tls.dataSize,
        title.tls.rawDataSize, title.tls.rawDataAddress);
    // Our Xbox LIVE (console_settings.ini live_server): profiles that sign in go online on its server
    // (kernel/xam_live.cpp), as the title's own client (its title ID, known now).
    kernel::InitializeLive();

    // Stall watchdog: when no frame is presented for CB_WATCHDOG seconds (default 30, 0 = off), log what every
    // guest thread is doing (once per stall).
    const int watchdogSeconds = [] { const char* v = std::getenv("CB_WATCHDOG"); return v ? std::atoi(v) : 30; }();
    if (watchdogSeconds > 0)
        std::thread([watchdogSeconds] {
            uint64_t lastFrame = gpu::SwapCount();
            int idle = 0;
            for (;;)
            {
                Sleep(1000);
                const uint64_t frame = gpu::SwapCount();
                idle = frame == lastFrame ? idle + 1 : 0;
                lastFrame = frame;
                if (idle == watchdogSeconds)
                {
                    kernel::DumpThreads(std::format("no frame presented for {} s after frame {}", idle, frame));
                    Log("  GPU ring: read pointer {:X}, write pointer {:X}; {}", gpu::ReadRegister(gpu::reg::kCpRbRptr),
                        gpu::ReadRegister(gpu::reg::kCpRbWptr), gpu::DescribeCommandProcessor());
                }
            }
        }).detach();

    gpu::PrepareRenderer();
    // CB_REPLAY_TRACE=<file.xtr> [CB_REPLAY_FRAMES=<n>]: instead of running the title, its GPU commands as a
    // Xenia-based renderer recorded them are drawn by this renderer (frames saved with CB_PRESENT_DUMP), to compare the
    // two on the same input.
    if (const char* trace = std::getenv("CB_REPLAY_TRACE"); trace && *trace)
    {
        const char* frames = std::getenv("CB_REPLAY_FRAMES");
        gpu::ReplayTrace(trace, frames ? std::strtoull(frames, nullptr, 10) : 0);
        TerminateProcess(GetCurrentProcess(), 0);
    }
    kernel::ThreadParams params;
    params.stackSize = title.defaultStackSize;
    params.startAddress = title.entryPoint;
    params.mainThread = true;
    params.processType = kernel::kProcessTypeTitle;
    kernel::GuestThread* mainThread = kernel::CreateThread(params);
    kernel::StartThread(mainThread);
    WaitForSingleObject(mainThread->hostHandle, INFINITE);
    Log("{} main thread exited with {:08X}", g_titleModule.name, mainThread->exitCode);
    logging::PauseIfOwnConsole();
    // The runtime's host threads (command processor, vblank, audio, presenter) never stop; returning from main would
    // run static destructors underneath them. End the process at once instead.
    TerminateProcess(GetCurrentProcess(), 0);
    return 0;
}
