#pragma once
#include <unordered_set>

#include "pch.h"
#include "recompiler_config.h"

struct RecompilerLocalVariables
{
    bool ctr{};
    bool xer{};
    bool reserved{};
    bool cr[8]{};
    bool r[32]{};
    bool f[32]{};
    bool v[128]{};
    bool env{};
    bool temp{};
    bool vTemp{};
    bool ea{};
};

enum class CSRState
{
    Unknown,
    FPU,
    VMX
};

struct Recompiler
{
    // Enforce In-order Execution of I/O constant for quick comparison
    static constexpr uint32_t c_eieio = 0xAC06007C;
    Image image;
    std::vector<Function> functions;
    std::string out;
    size_t cppFileIndex = 0;
    RecompilerConfig config;

    bool LoadConfig(const std::string_view& configFilePath);

    template<class... Args>
    void print(fmt::format_string<Args...> fmt, Args&&... args)
    {
        fmt::vformat_to(std::back_inserter(out), fmt.get(), fmt::make_format_args(args...));
    }

    template<class... Args>
    void println(fmt::format_string<Args...> fmt, Args&&... args)
    {
        fmt::vformat_to(std::back_inserter(out), fmt.get(), fmt::make_format_args(args...));
        out += '\n';
    }

    void Analyse();
    // Functions that keep r14-r31 / f14-f31 / v14-v31 / v64-v127 in the context although
    // non_volatile_as_local is set: SEH parents, their filter and __finally funclets (which read the parent's
    // non-volatile registers directly), every caller of a funclet, and the save/restore helpers they call.
    std::unordered_set<uint32_t> nonVolatileInContext;
    void FindNonVolatileInContext();
    bool NonVolatileAsLocal(const Function& fn) const
    {
        return config.nonVolatileRegistersAsLocalVariables && nonVolatileInContext.count(uint32_t(fn.base)) == 0;
    }

    // TODO: make a RecompileArgs struct instead this is getting messy
    bool Recompile(
        const Function& fn,
        uint32_t base,
        const ppc_insn& insn,
        const uint32_t* data,
        std::unordered_map<uint32_t, RecompilerSwitchTable>::iterator& switchTable,
        RecompilerLocalVariables& localVariables,
        CSRState& csrState);

    bool Recompile(const Function& fn);

    // relocatable_code: the current function's relocation values, in the order the code reads them.
    std::vector<uint32_t> relocValues;
    // A value read from the relocation array: PPC_RI(k) (int32_t) or PPC_RL(k) (int64_t), the type the decimal
    // literal it replaces would have had.
    std::string Reloc(int64_t value);
    // A label's name: loc_<address>, or loc_<offset in the function> with relocatable_code.
    std::string Label(const Function& fn, size_t address) const;
    // relocatable_code: the immediate of `instruction` that depends on the layout, replaced in the code it
    // generated (out from `start`, after its comment line) when it appears there exactly once.
    void RelocateImmediate(uint32_t instruction, size_t start);

    void Recompile(const std::filesystem::path& headerFilePath);

    void SaveCurrentOutData(const std::string_view& name = std::string_view());
};
