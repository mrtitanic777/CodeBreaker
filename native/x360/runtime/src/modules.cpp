#include "modules.h"

#include "kernel/kernel.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <filesystem>
#include <format>
#include <memory>
#include <mutex>

namespace modules
{
    namespace
    {
        // LDR_DATA_TABLE_ENTRY (layout cross-checked with Xenia Canary's X_LDR_DATA_TABLE_ENTRY).
        constexpr uint32_t kLdrEntrySize = 0x80;
        constexpr uint32_t kLdrDllBase = 0x18;
        constexpr uint32_t kLdrImageBase = 0x1C;
        constexpr uint32_t kLdrImageSize = 0x20;
        constexpr uint32_t kLdrFullImageSize = 0x38;
        constexpr uint32_t kLdrEntryPoint = 0x3C;
        constexpr uint32_t kLdrLoadCount = 0x40;
        constexpr uint32_t kLdrXexHeaderBase = 0x58;

        std::mutex g_mutex;
        std::vector<std::unique_ptr<LoadedModule>> g_modules;

        std::string Lower(std::string s)
        {
            std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return char(std::tolower(c)); });
            return s;
        }

        // Every table slot without a recompiled function reports the indirect call. CTR is a local of the recompiled
        // code, so the target comes from the `mtctr rN` before the bctrl when rN is a context register
        // (r0-r13; compiled code mostly uses r11 or r12).
        void MissingFunction(PPCContext& ctx, uint8_t*)
        {
            const uint32_t callSite = uint32_t(ctx.lr) - 4;
            std::string target = "an unknown address";
            PPCRegister* registers[] = { &ctx.r0, &ctx.r1, &ctx.r2, &ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6, &ctx.r7,
                &ctx.r8, &ctx.r9, &ctx.r10, &ctx.r11, &ctx.r12, &ctx.r13 };
            for (uint32_t i = 1; i <= 8 && FindByCode(callSite - i * 4) != nullptr; i++)
            {
                const uint32_t insn = guest::g_memory.Read32(callSite - i * 4);
                if ((insn & 0xFC1FFFFF) == 0x7C0903A6) // mtctr rS
                {
                    const uint32_t rs = (insn >> 21) & 31;
                    if (rs < 14)
                        target = std::format("{:08X}", registers[rs]->u32);
                    break;
                }
            }
            Fatal("Indirect call to {}, where no recompiled function starts (call at {:08X})", target, callSite);
        }

        void FillTable(uint32_t start, uint32_t size)
        {
            uint8_t* base = guest::g_memory.base;
            guest::g_memory.CommitFunctionTable(start, size);
            for (uint32_t address = start; address < start + size; address += 4)
                PPC_LOOKUP_FUNC(base, address) = MissingFunction;
        }

        // .pdata from the image's PE headers (little-endian; the section table is right for .pdata, only .edata is
        // stale in system modules).
        void FindPdata(LoadedModule& m)
        {
            auto& mem = guest::g_memory;
            auto le32 = [&](uint32_t a) { uint32_t v; memcpy(&v, mem.Translate(a), 4); return v; };
            auto le16 = [&](uint32_t a) { uint16_t v; memcpy(&v, mem.Translate(a), 2); return v; };
            const uint32_t base = m.xex.imageBase;
            const uint32_t pe = base + le32(base + 0x3C);
            if (le32(pe) != 0x00004550) // "PE\0\0"
                Fatal("{}: no PE header in the image", m.path);
            const uint16_t sections = le16(pe + 6);
            const uint32_t table = pe + 0x18 + le16(pe + 0x14);
            for (uint16_t i = 0; i < sections; i++)
            {
                const uint32_t s = table + i * 40;
                if (memcmp(mem.Translate(s), ".pdata\0\0", 8) == 0)
                {
                    m.pdata = base + le32(s + 12);
                    m.pdataSize = le32(s + 8);
                }
            }
            if (m.pdata == 0)
                Fatal("{}: no .pdata section", m.path);
        }

        void ResolveImports(const LoadedModule& m)
        {
            auto& mem = guest::g_memory;
            size_t functions = 0, variables = 0;
            std::string missing; // every variable import the runtime has no value for, reported together
            for (const auto& imp : m.xex.imports)
            {
                if (imp.thunk)
                {
                    // The slot holds the function's address; point it at the thunk, which the recompiler mapped to the
                    // runtime's __imp__ implementation, so taking the import's address and calling it indirectly works.
                    mem.Write32(imp.slot, imp.thunk);
                    kernel::RegisterImportThunk(imp.library, imp.ordinal, imp.thunk);
                    functions++;
                }
                else
                {
                    const uint32_t address = kernel::ResolveVariable(imp.library, imp.ordinal);
                    if (address == 0)
                    {
                        missing += std::format("{}{} {}", missing.empty() ? "" : ", ", imp.library, imp.ordinal);
                        continue;
                    }
                    mem.Write32(imp.slot, address);
                    variables++;
                }
            }
            if (!missing.empty())
                Fatal("{}: no implementation for variable imports: {}", m.path, missing);
            Log("{}: resolved {} function imports and {} variable imports", m.recompiled->name, functions, variables);
        }

        void CreateLoaderEntry(LoadedModule& m)
        {
            auto& mem = guest::g_memory;
            m.xexHeader = mem.SystemAlloc(uint32_t(m.xex.header.size()));
            memcpy(mem.Translate(m.xexHeader), m.xex.header.data(), m.xex.header.size());
            // The loader's copy holds the security info's address, not the file's offset to it (xam.xex reads the
            // executable's allowed media types and image flags through it, 0x8169ED78).
            mem.Write32(m.xexHeader + 0x10, m.xexHeader + mem.Read32(m.xexHeader + 0x10));
            const uint32_t e = mem.SystemAlloc(kLdrEntrySize);
            for (uint32_t links = 0; links < 0x18; links += 8) // three empty list heads
            {
                mem.Write32(e + links, e + links);
                mem.Write32(e + links + 4, e + links);
            }
            mem.Write32(e + kLdrDllBase, m.xex.imageBase);
            mem.Write32(e + kLdrImageBase, m.xex.imageBase);
            mem.Write32(e + kLdrImageSize, m.xex.imageSize);
            mem.Write32(e + kLdrFullImageSize, m.xex.imageSize);
            mem.Write32(e + kLdrEntryPoint, m.xex.entryPoint);
            mem.Write16(e + kLdrLoadCount, 1);
            mem.Write32(e + kLdrXexHeaderBase, m.xexHeader);
            m.handle = e;
        }
    }

    void Initialize()
    {
        FillTable(kernel::kDynamicExportStart, kernel::kDynamicExportEnd - kernel::kDynamicExportStart);
    }

    const LoadedModule& Load(const std::string& path, const RecompiledModule& recompiled)
    {
        auto m = std::make_unique<LoadedModule>();
        m->recompiled = &recompiled;
        m->path = path;
        m->xex = xex::Load(path);
        if (m->xex.imageBase != recompiled.imageBase || m->xex.imageSize != recompiled.imageSize)
            Fatal("{} does not match the recompiled {} (base {:08X} size {:X}, expected {:08X} size {:X})", path,
                recompiled.name, m->xex.imageBase, m->xex.imageSize, recompiled.imageBase, recompiled.imageSize);
        if (recompiled.codeSize != 0 && (recompiled.codeBase < recompiled.imageBase ||
            recompiled.codeBase + recompiled.codeSize > recompiled.imageBase + recompiled.imageSize))
            Fatal("{}: code range outside the image", recompiled.name);
        {
            std::lock_guard lock(g_mutex);
            for (const auto& other : g_modules)
                if (m->xex.imageBase < other->xex.imageBase + other->xex.imageSize &&
                    other->xex.imageBase < m->xex.imageBase + m->xex.imageSize)
                    Fatal("{} overlaps {} in guest memory", recompiled.name, other->recompiled->name);
        }

        if (recompiled.codeSize != 0)
            FillTable(recompiled.codeBase, (recompiled.codeSize + 3) & ~3u);
        size_t count = 0;
        for (const PPCFuncMapping* f = recompiled.mappings; f->host != nullptr; f++, count++)
        {
            if (f->guest < recompiled.codeBase || f->guest >= recompiled.codeBase + recompiled.codeSize)
                Fatal("{}: recompiled function {:08X} lies outside its code section", recompiled.name, uint32_t(f->guest));
            PPC_LOOKUP_FUNC(guest::g_memory.base, f->guest) = f->host;
        }
        if (recompiled.codeSize != 0) // a data-only module (resources: huduiskin.xex) has no program
            FindPdata(*m);
        ResolveImports(*m);
        CreateLoaderEntry(*m);
        Log("Module {} ({}): {} recompiled functions, handle {:08X}", recompiled.name, path, count, m->handle);

        std::lock_guard lock(g_mutex);
        g_modules.push_back(std::move(m));
        return *g_modules.back();
    }

    void Unload(const LoadedModule& module)
    {
        const RecompiledModule& recompiled = *module.recompiled;
        if (recompiled.codeSize != 0)
            FillTable(recompiled.codeBase, (recompiled.codeSize + 3) & ~3u);
        Log("Module {} unloaded", recompiled.name);
        std::lock_guard lock(g_mutex);
        std::erase_if(g_modules, [&](const std::unique_ptr<LoadedModule>& m) { return m.get() == &module; });
    }

    const LoadedModule* FindByHandle(uint32_t handle)
    {
        std::lock_guard lock(g_mutex);
        for (const auto& m : g_modules)
            if (m->handle == handle)
                return m.get();
        return nullptr;
    }

    const LoadedModule* FindByName(const std::string& name)
    {
        std::string file = name;
        if (const size_t slash = file.find_last_of("\\/"); slash != std::string::npos)
            file = file.substr(slash + 1);
        file = Lower(file);
        std::lock_guard lock(g_mutex);
        for (const auto& m : g_modules)
            if (Lower(m->recompiled->name) == file)
                return m.get();
        return nullptr;
    }

    const LoadedModule* FindByCode(uint32_t address)
    {
        std::lock_guard lock(g_mutex);
        for (const auto& m : g_modules)
            if (address >= m->recompiled->codeBase && address < m->recompiled->codeBase + m->recompiled->codeSize)
                return m.get();
        return nullptr;
    }

    std::vector<const LoadedModule*> All()
    {
        std::lock_guard lock(g_mutex);
        std::vector<const LoadedModule*> all;
        for (const auto& m : g_modules)
            all.push_back(m.get());
        return all;
    }

    uint32_t ExportAddress(const LoadedModule& module, uint16_t ordinal)
    {
        auto it = module.xex.exports.find(ordinal);
        return it == module.xex.exports.end() ? 0 : it->second;
    }
}
