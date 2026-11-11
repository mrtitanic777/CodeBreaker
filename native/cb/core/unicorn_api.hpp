// cb/core/unicorn_api.hpp -- the Unicorn emulator's C API, called through the prebuilt unicorn.dll.
//
// `cb::emu::unicorn::api()` loads unicorn.dll (beside the executable, see dynlib.hpp) on first use and returns a
// table of typed function pointers, one per function CodeBreaker calls. The types come from the real
// <unicorn/unicorn.h> (native/third_party/include, Unicorn 2.1.4), so a call through the table is checked
// against the C declaration. This header pulls in the Unicorn headers: it is included only by the emulator
// wrapper in cb/emulate, never by a module's public header (docs/CPP_DESIGN.md 3.13).
//
//     const auto& uc = cb::emu::unicorn::api();
//     uc_engine* engine = nullptr;
//     cb::emu::unicorn::check(uc.uc_open(UC_ARCH_X86, UC_MODE_32, &engine), "uc_open");
#pragma once

// Third-party header: warnings are theirs, not ours (the one justified #pragma warning of the core).
#pragma warning(push, 0)
#include <unicorn/unicorn.h>
#pragma warning(pop)

#include <string_view>

#include "cb/core/dynlib.hpp"

namespace cb::emu::unicorn {

// Every function CodeBreaker calls. Adding one is one line here.
#define CB_UNICORN_FUNCTIONS(X)                                                                     \
    X(uc_version) X(uc_arch_supported) X(uc_open) X(uc_close) X(uc_query) X(uc_errno) X(uc_strerror) \
    X(uc_reg_write) X(uc_reg_read) X(uc_reg_write_batch) X(uc_reg_read_batch)                        \
    X(uc_mem_write) X(uc_mem_read) X(uc_mem_map) X(uc_mem_map_ptr) X(uc_mem_unmap) X(uc_mem_protect)  \
    X(uc_mem_regions) X(uc_free) X(uc_emu_start) X(uc_emu_stop) X(uc_hook_add) X(uc_hook_del)          \
    X(uc_context_alloc) X(uc_context_save) X(uc_context_restore) X(uc_context_free)                   \
    X(uc_context_size) X(uc_context_reg_read) X(uc_context_reg_write) X(uc_ctl)

struct Api {
#define CB_X(name) decltype(&::name) name = nullptr;
    CB_UNICORN_FUNCTIONS(CB_X)
#undef CB_X
};

// The loaded library's function table; loads unicorn.dll on the first call. Throws NotFoundError if the DLL or
// one of the functions is missing. Thread-safe.
const Api& api();
// The library, for its path (messages, the third-party report).
const DynLib& library();

// Turn a uc_err into an EmulationError carrying Unicorn's own text; does nothing for UC_ERR_OK.
void check(uc_err code, std::string_view what);

// Unicorn's version as "major.minor".
std::string version_string();

}  // namespace cb::emu::unicorn
