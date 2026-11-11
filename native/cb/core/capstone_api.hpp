// cb/core/capstone_api.hpp -- the Capstone disassembler's C API, called through the prebuilt capstone.dll.
//
// `cb::disasm::capstone::api()` loads capstone.dll (beside the executable, see dynlib.hpp) on first use and
// returns a table of typed function pointers; the types come from the real <capstone/capstone.h>
// (native/third_party/include, Capstone 5.0.7). Includes the Capstone headers: for the disassembly wrapper in
// cb/analyze or cb/decompile only, never for a module's public header.
//
//     const auto& cs = cb::disasm::capstone::api();
//     csh handle = 0;
//     cb::disasm::capstone::check(cs.cs_open(CS_ARCH_X86, CS_MODE_32, &handle), "cs_open");
#pragma once

#pragma warning(push, 0)
#include <capstone/capstone.h>
#pragma warning(pop)

#include <string>
#include <string_view>

#include "cb/core/dynlib.hpp"

namespace cb::disasm::capstone {

#define CB_CAPSTONE_FUNCTIONS(X)                                                                    \
    X(cs_version) X(cs_support) X(cs_open) X(cs_close) X(cs_option) X(cs_errno) X(cs_strerror)       \
    X(cs_disasm) X(cs_free) X(cs_malloc) X(cs_disasm_iter) X(cs_reg_name) X(cs_insn_name)            \
    X(cs_group_name) X(cs_insn_group) X(cs_reg_read) X(cs_reg_write) X(cs_op_count) X(cs_op_index)   \
    X(cs_regs_access)

struct Api {
#define CB_X(name) decltype(&::name) name = nullptr;
    CB_CAPSTONE_FUNCTIONS(CB_X)
#undef CB_X
};

// The loaded library's function table; loads capstone.dll on the first call. Throws NotFoundError if the DLL or
// one of the functions is missing. Thread-safe.
const Api& api();
const DynLib& library();

// Turn a cs_err into an Error carrying Capstone's own text; does nothing for CS_ERR_OK.
void check(cs_err code, std::string_view what);

// Capstone's version as "major.minor".
std::string version_string();

}  // namespace cb::disasm::capstone
