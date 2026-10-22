// Dynamic exports: system-module exports a title looks up with XexGetProcedureAddress instead of importing.
// Each bound export gets a 4-byte guest address in the unused gap after .text; its indirect-call table entry points
// at the host implementation registered with GUEST_EXPORT.

#include "kernel/kernel.h"

#include <map>
#include <mutex>

namespace
{
    using Key = std::pair<std::string, uint16_t>;

    std::map<Key, PPCFunc*>& Registry()
    {
        static std::map<Key, PPCFunc*> registry; // function-local: GUEST_EXPORT registers during static initialization
        return registry;
    }

    std::mutex g_bindMutex;
    std::map<Key, uint32_t> g_bound;
    uint32_t g_nextAddress = kernel::kDynamicExportStart;
}

namespace kernel
{
    void RegisterDynamicExport(const char* library, uint16_t ordinal, PPCFunc* func)
    {
        Registry()[{ library, ordinal }] = func;
    }

    uint32_t BindDynamicExport(const std::string& library, uint16_t ordinal)
    {
        const Key key{ library, ordinal };
        auto it = Registry().find(key);
        if (it == Registry().end())
            return 0;

        std::lock_guard lock(g_bindMutex);
        if (auto bound = g_bound.find(key); bound != g_bound.end())
            return bound->second;
        if (g_nextAddress + 4 > kDynamicExportEnd)
            Fatal("Out of guest addresses for dynamic exports");
        const uint32_t address = g_nextAddress;
        g_nextAddress += 4;
        PPC_LOOKUP_FUNC(guest::g_memory.base, address) = it->second;
        g_bound[key] = address;
        Log("Bound dynamic export {} ordinal {} at {:08X}", library, ordinal, address);
        return address;
    }
}
