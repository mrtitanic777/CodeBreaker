// The console's other system modules, recompiled (SystemRecomp/<module>, tools/scripts/prepare_module.py), that
// XexLoadImage can load on request, as the console's loader loads them from flash or the hard drive (CB_RECOMP_XAM:
// xam.xex loads hud.xex, the Guide, itself). Each is linked in and listed here; its image
// comes from the user's own converted file (SystemUpdate/converted/<name>).

#include "kernel/kernel.h"
#include "modules.h"
#include "settings.h"

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
