#pragma once

struct RecompilerSwitchTable
{
    uint32_t r;
    std::vector<uint32_t> labels;
};

struct RecompilerMidAsmHook
{
    std::string name;
    std::vector<std::string> registers;

    bool ret = false;
    bool returnOnTrue = false;
    bool returnOnFalse = false;

    uint32_t jumpAddress = 0;
    uint32_t jumpAddressOnTrue = 0;
    uint32_t jumpAddressOnFalse = 0;

    bool afterInstruction = false;
};

struct RecompilerConfig
{
    std::string directoryPath;
    std::string filePath;
    std::string patchFilePath;
    std::string patchedFilePath;
    std::string outDirectoryPath;
    std::string switchTableFilePath;
    // Appended to the names the recompiler gives fixed symbols (_xstart, the save/restore helpers, PPCFuncMappings)
    // so several recompiled modules can be linked into one program.
    std::string symbolSuffix;
    // suffix_function_names: the suffix goes on the generated function names (sub_<address>) too, for modules that
    // share a load address (XAM's apps at 0x90100000). Off by default: a runtime may refer to sub_<address> names.
    bool suffixFunctionNames = false;
    std::string FunctionSuffix() const { return suffixFunctionNames ? symbolSuffix : std::string(); }
    std::unordered_map<uint32_t, RecompilerSwitchTable> switchTables;
    bool skipLr = false;
    bool ctrAsLocalVariable = false;
    bool xerAsLocalVariable = false;
    bool reservedRegisterAsLocalVariable = false;
    bool skipMsr = false;
    bool crRegistersAsLocalVariables = false;
    bool nonArgumentRegistersAsLocalVariables = false;
    bool nonVolatileRegistersAsLocalVariables = false;
    // relocatable_code: no guest address in the generated code. Call targets, return addresses, address halves
    // (lis / addis / ori / oris, addi off a register) and load / store offsets off a register other than r1 are
    // read from the function's relocation array (PPC_RELOC_PROLOGUE, PPC_RI / PPC_RL), labels are named by
    // their offset in the function and direct calls go through the indirect-call table. Each function is
    // followed by a "//@R" line listing its relocation values. Functions of several versions of a module whose
    // generated text is then the same can share one host function (unify_xam.py).
    bool relocatableCode = false;
    uint32_t restGpr14Address = 0;
    uint32_t saveGpr14Address = 0;
    uint32_t restFpr14Address = 0;
    uint32_t saveFpr14Address = 0;
    uint32_t restVmx14Address = 0;
    uint32_t saveVmx14Address = 0;
    uint32_t restVmx64Address = 0;
    uint32_t saveVmx64Address = 0;
    uint32_t longJmpAddress = 0;
    uint32_t setJmpAddress = 0;
    std::unordered_map<uint32_t, uint32_t> functions;
    std::unordered_map<uint32_t, uint32_t> invalidInstructions;
    std::unordered_map<uint32_t, RecompilerMidAsmHook> midAsmHooks;

    void Load(const std::string_view& configFilePath);
};
