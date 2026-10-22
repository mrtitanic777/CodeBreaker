#pragma once

#include "xex.h"

#include <ppc_config.h>
#include <ppc_context.h>

#include <cstdint>
#include <string>
#include <vector>

// The recompiled modules of the process. On the console the kernel and XAM load several modules into one address
// space (bootanim.xex, dash.xex and its plugins, a title); here each is a XenonRecomp library linked into the program,
// whose guest image is loaded from the user's own file at run time. All modules share one indirect-call table
// (ppc_lookup.h), so guest code can call into any of them through a function pointer.
namespace modules
{
    // What a recompiled module's library describes about itself (its module_info.cpp, built with the module's own
    // ppc_config.h). The image must match the file loaded at run time.
    struct RecompiledModule
    {
        const char* name;          // file name as the console knows it, e.g. "bootanim.xex"
        uint32_t imageBase;
        uint32_t imageSize;
        uint32_t codeBase;         // XenonRecomp's PPC_CODE_BASE / PPC_CODE_SIZE (the executable sections)
        uint32_t codeSize;
        const PPCFuncMapping* mappings; // terminated by a null host function
        uint32_t saveGprLr14;      // __savegprlr_14 (exception unwinding finds saved LRs through it); 0 if none
    };

    struct LoadedModule
    {
        const RecompiledModule* recompiled = nullptr;
        std::string path;          // host path it was loaded from
        xex::Module xex;
        uint32_t handle = 0;       // HMODULE: guest loader data table entry (LDR_DATA_TABLE_ENTRY)
        uint32_t xexHeader = 0;    // guest copy of the XEX header (the entry's XexHeaderBase)
        uint32_t pdata = 0;        // .pdata (from the image's PE section table), for exception dispatch
        uint32_t pdataSize = 0;
    };

    // Commits the table slots of the runtime's dynamic-export range (kernel::kDynamicExportStart..End). Call once,
    // after guest::g_memory.Reserve().
    void Initialize();

    // Loads a module's XEX into guest memory, checks it against the recompiled code, installs its functions in the
    // indirect-call table, binds its imports to the runtime and gives it a loader entry (HMODULE). Exits on mismatch.
    const LoadedModule& Load(const std::string& path, const RecompiledModule& recompiled);

    // Removes a module loaded with Load: its functions leave the indirect-call table (an image at the same address can
    // be loaded next, as XAM's apps share one), its loader entry is no longer found.
    void Unload(const LoadedModule& module);

    const LoadedModule* FindByHandle(uint32_t handle);
    // By file name ("bootanim.xex"), case-insensitive; a path's directories are ignored.
    const LoadedModule* FindByName(const std::string& name);
    // The module whose code section contains the guest address.
    const LoadedModule* FindByCode(uint32_t address);
    std::vector<const LoadedModule*> All();

    // Guest address of an export by ordinal, 0 if the module has none.
    uint32_t ExportAddress(const LoadedModule& module, uint16_t ordinal);
}
