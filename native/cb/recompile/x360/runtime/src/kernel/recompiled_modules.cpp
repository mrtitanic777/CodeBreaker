// The console's other system modules, recompiled (SystemRecomp/<module>, tools/scripts/prepare_module.py), that
// XexLoadImage can load on request, as the console's loader loads them from flash or the hard drive (CB_RECOMP_XAM:
// xam.xex loads hud.xex, the Guide, itself). Each is linked in and listed here; its image
// comes from the user's own converted file (SystemUpdate/converted/<name>).

#include "kernel/kernel.h"
#include "modules.h"
#include "settings.h"
#include "vfs.h"

#include <cctype>
#include <deque>
#include <filesystem>
#include <mutex>
#include <span>
#include <string>

namespace kernel
{
    // The build generates the list (linked_modules.cpp; x360.cmake: none for a title).
    extern const modules::RecompiledModule* const kLinkedSystemModules[];
    extern const size_t kLinkedSystemModuleCount;
}

namespace
{
    std::span<const modules::RecompiledModule* const> LinkedModules()
    {
        return { kernel::kLinkedSystemModules, kernel::kLinkedSystemModuleCount };
    }

    std::string Lower(std::string s)
    {
        for (char& c : s)
            c = char(std::tolower(static_cast<unsigned char>(c)));
        return s;
    }
}

namespace kernel
{
    const modules::RecompiledModule* LinkedRecompiledModule(const std::string& name)
    {
        for (const modules::RecompiledModule* m : LinkedModules())
            if (Lower(m->name) == Lower(name))
                return m;
        return nullptr;
    }

    bool HasRecompiledSystemModule(const std::string& name)
    {
        for (const modules::RecompiledModule* m : LinkedModules())
            if (Lower(m->name) == Lower(name))
                return std::filesystem::exists(DataDirectory() / "SystemUpdate" / "converted" / m->name);
        return false;
    }

    // The loaded module for `name` (a file name or a path to one), loading a linked recompiled system module and
    // running its entry point (DllMain(handle, DLL_PROCESS_ATTACH, 0), as XexLoadImage does) the first time. Null if
    // no linked module has that name.
    const modules::LoadedModule* LoadRecompiledSystemModule(const std::string& name, PPCContext& ctx)
    {
        std::string file = name;
        if (const size_t slash = file.find_last_of("\\/:"); slash != std::string::npos)
            file = file.substr(slash + 1);
        if (const modules::LoadedModule* loaded = modules::FindByName(file))
            return loaded;
        // A module with no program, only resources (huduiskin.xex: the system UI's skin; the IME dictionaries): its
        // image and loader entry, no code.
        static const std::string kDataModules[] = { "huduiskin.xex", "ximedic.xex", "ximedicex.xex", "XimeDicCh.xex",
                                                    "XimeDic_CHS.xex" };
        for (const std::string& data : kDataModules)
        {
            if (Lower(data) != Lower(file))
                continue;
            const std::filesystem::path path = DataDirectory() / "SystemUpdate" / "converted" / data;
            if (!std::filesystem::exists(path))
                Fatal("{} is missing: convert it with tools/scripts/prepare_module.py", path.string());
            static std::mutex mutex;
            static std::deque<std::string> names;
            static std::deque<PPCFuncMapping> terminator;
            static std::deque<modules::RecompiledModule> described;
            std::lock_guard lock(mutex);
            const xex::Module x = xex::Load(path.string());
            names.push_back(data);
            terminator.push_back({ 0, nullptr });
            described.push_back({ names.back().c_str(), x.imageBase, x.imageSize, 0, 0, &terminator.back(), 0 });
            const modules::LoadedModule& loaded = modules::Load(path.string(), described.back());
            Log("XexLoadImage(\"{}\"): {} loaded (data only, no code)", name, data);
            return &loaded;
        }
        for (const modules::RecompiledModule* m : LinkedModules())
        {
            if (Lower(m->name) != Lower(file))
                continue;
            const std::filesystem::path path = DataDirectory() / "SystemUpdate" / "converted" / m->name;
            if (!std::filesystem::exists(path))
                Fatal("{} is missing: convert it with tools/scripts/prepare_module.py", path.string());
            const modules::LoadedModule& loaded = modules::Load(path.string(), *m);
            if (const uint32_t entry = loaded.xex.entryPoint)
            {
                Log("XexLoadImage(\"{}\"): recompiled {} loaded, DllMain at {:08X}", name, m->name, entry);
                PPCContext call = ctx;
                call.r3.u64 = loaded.handle;
                call.r4.u64 = 1; // DLL_PROCESS_ATTACH
                call.r5.u64 = 0;
                CallGuest(call, entry);
                Log("XexLoadImage(\"{}\"): DllMain returned {:08X}", name, call.r3.u32);
            }
            return &loaded;
        }
        return nullptr;
    }

    // XexUnloadImage of a module LoadRecompiledSystemModule loaded, its last reference gone: its entry point with
    // DLL_PROCESS_DETACH, then the module leaves (the console's loader frees its pages; the next app loads into the
    // same slot). False for any other module.
    bool UnloadRecompiledSystemModule(uint32_t handle, PPCContext& ctx)
    {
        const modules::LoadedModule* loaded = modules::FindByHandle(handle);
        if (!loaded)
            return false;
        bool ours = false;
        for (const modules::RecompiledModule* m : LinkedModules())
            ours |= m == loaded->recompiled;
        if (!ours)
            return false;
        if (const uint32_t entry = loaded->xex.entryPoint)
        {
            PPCContext call = ctx;
            call.r3.u64 = handle;
            call.r4.u64 = 0; // DLL_PROCESS_DETACH
            call.r5.u64 = 0;
            CallGuest(call, entry);
        }
        modules::Unload(*loaded);
        return true;
    }
}

namespace
{
    // The title's other recompiled modules (modules.h: the title's module_info lists them).
    const modules::RecompiledModule* const* g_titleModules = nullptr;
    std::recursive_mutex g_titleModulesMutex;   // a module's loading loads its dependencies, on the same thread
    thread_local PPCContext* g_loadContext = nullptr;

    std::string FileName(const std::string& name)
    {
        const size_t slash = name.find_last_of("\\/:");
        return slash == std::string::npos ? name : name.substr(slash + 1);
    }

    std::string Stem(const std::string& file)
    {
        const size_t dot = file.find_last_of('.');
        return dot == std::string::npos ? file : file.substr(0, dot);
    }
}

namespace kernel
{
    void SetTitleModules(const modules::RecompiledModule* const* list)
    {
        g_titleModules = list;
    }

    const modules::RecompiledModule* FindTitleModule(const std::string& name)
    {
        if (!g_titleModules)
            return nullptr;
        const std::string file = Lower(FileName(name));
        for (const modules::RecompiledModule* const* m = g_titleModules; *m; m++)
            if (Lower((*m)->name) == file || Lower(Stem((*m)->name)) == file)
                return *m;
        return nullptr;
    }

    const modules::LoadedModule* LoadRecompiledTitleModule(const std::string& name, PPCContext& ctx)
    {
        const modules::RecompiledModule* described = FindTitleModule(name);
        if (!described)
            return nullptr;
        std::lock_guard lock(g_titleModulesMutex);
        if (const modules::LoadedModule* loaded = modules::FindByName(described->name))
            return loaded;
        std::string guestPath = "game:\\";
        for (const char* c = described->path ? described->path : described->name; *c; c++)
            guestPath += *c == '/' ? '\\' : *c;
        const auto resolved = vfs::Resolve(guestPath);
        std::error_code ec;
        if (!resolved || !std::filesystem::exists(resolved->host, ec))
            Fatal("{}: the title's module {} is missing", guestPath, described->name);
        // Its imports of other title modules load those first (modules.cpp asks through LoadTitleDependency).
        PPCContext* outer = g_loadContext;
        g_loadContext = &ctx;
        const modules::LoadedModule& loaded = modules::Load(resolved->host.string(), *described);
        g_loadContext = outer;
        if (const uint32_t entry = loaded.xex.entryPoint)
        {
            Log("XexLoadImage(\"{}\"): title module {} loaded, DllMain at {:08X}", name, described->name, entry);
            PPCContext call = ctx;
            call.r3.u64 = loaded.handle;
            call.r4.u64 = 1; // DLL_PROCESS_ATTACH
            call.r5.u64 = 0;
            CallGuest(call, entry);
            Log("XexLoadImage(\"{}\"): DllMain of {} returned {:08X}", name, described->name, call.r3.u32);
        }
        return &loaded;
    }

    const modules::LoadedModule* LoadTitleDependency(const std::string& library)
    {
        PPCContext* ctx = g_loadContext;
        if (!ctx)
            if (const GuestThread* thread = CurrentThread())
                ctx = thread->context;
        if (!ctx)
            Fatal("{}: a title module is imported while no guest thread loads", library);
        return LoadRecompiledTitleModule(library, *ctx);
    }

    void CallModuleExport(const char* library, uint16_t ordinal, PPCContext& ctx, uint8_t* base)
    {
        const modules::LoadedModule* module = LoadRecompiledTitleModule(library, ctx);
        const uint32_t address = module ? modules::ExportAddress(*module, ordinal) : 0;
        if (address == 0)
            Fatal("{} has no export of ordinal {} (called from {:08X})", library, ordinal, uint32_t(ctx.lr));
        PPCFunc* func = PPC_LOOKUP_FUNC(base, address);
        if (func == nullptr)
            Fatal("{} ordinal {} at {:08X}: no recompiled function starts there", library, ordinal, address);
        func(ctx, base);
    }
}
