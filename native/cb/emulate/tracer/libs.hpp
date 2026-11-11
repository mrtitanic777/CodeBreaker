// libs.hpp -- Unicorn and Capstone, as the recorder calls them.
//
// By default the entry points are the prebuilt DLLs' of cb/core (unicorn_api, capstone_api). The C-interface DLL
// (cbtrace.dll, for the callers that still reach the recorder through ctypes or LoadLibrary) is handed engines made by
// *their* Unicorn, so it calls that very DLL: `use_libraries()` points the table at the DLLs named by path, once,
// before the first run.
#pragma once
#include <atomic>
#include <cstddef>
#include <cstdint>

#include "cb/core/capstone_api.hpp"
#include "cb/core/unicorn_api.hpp"

namespace cb::emulate::tracer {

struct UcLib {
    decltype(&uc_reg_read) reg_read = nullptr;
    decltype(&uc_reg_write) reg_write = nullptr;
    decltype(&uc_mem_read) mem_read = nullptr;
    decltype(&uc_mem_write) mem_write = nullptr;
    decltype(&uc_mem_map) mem_map = nullptr;
    decltype(&uc_mem_unmap) mem_unmap = nullptr;
    decltype(&uc_emu_stop) emu_stop = nullptr;
    decltype(&uc_hook_add) hook_add = nullptr;
    decltype(&uc_hook_del) hook_del = nullptr;
    decltype(&uc_emu_start) emu_start = nullptr;
    decltype(&uc_ctl) ctl = nullptr;
};

struct CsLib {
    decltype(&cs_open) open = nullptr;
    decltype(&cs_option) option = nullptr;
    decltype(&cs_disasm) disasm = nullptr;
    decltype(&cs_free) free = nullptr;
    decltype(&cs_reg_name) reg_name = nullptr;
    decltype(&cs_regs_access) regs_access = nullptr;
};

namespace detail {
struct Tables {
    UcLib uc;
    CsLib cs;
};
extern std::atomic<const Tables*> in_force;   // set once; read on every hook of every run
const Tables& init_tables();                  // the cb/core DLLs' tables, made on first use
}  // namespace detail

// The tables in force: the cb/core DLLs' unless use_libraries() named others. One acquire load on the fast path.
inline const UcLib& ucl() {
    const detail::Tables* t = detail::in_force.load(std::memory_order_acquire);
    return (t != nullptr ? *t : detail::init_tables()).uc;
}
inline const CsLib& csl() {
    const detail::Tables* t = detail::in_force.load(std::memory_order_acquire);
    return (t != nullptr ? *t : detail::init_tables()).cs;
}

// Call the Unicorn and Capstone DLLs at these full paths instead (loading an already loaded path gives the same
// module: the Unicorn whose engines the caller made). False, with `err` set, when one will not load or lacks an
// entry point. Only the first call counts; the later ones answer as it did.
bool use_libraries(const wchar_t* unicorn_dll, const wchar_t* capstone_dll, char* err, std::size_t errlen);

// A register as the recorder reads it (a uint64 the value is read into, zero first).
inline std::uint64_t reg(uc_engine* engine, int id) {
    std::uint64_t v[8] = {0};                    // (room for any register class: only v[0] is used)
    ucl().reg_read(engine, id, v);
    return v[0];
}

inline void set_reg(uc_engine* engine, int id, std::uint64_t value) {
    std::uint64_t v[8] = {value};
    ucl().reg_write(engine, id, v);
}

inline bool read32(uc_engine* engine, std::uint64_t address, std::uint32_t& out) {
    std::uint8_t b[4];
    if (ucl().mem_read(engine, address, b, 4) != UC_ERR_OK) return false;
    out = std::uint32_t(b[0]) | std::uint32_t(b[1]) << 8 | std::uint32_t(b[2]) << 16 | std::uint32_t(b[3]) << 24;
    return true;
}

inline std::uint64_t little(const std::uint8_t* b, std::size_t n) {
    std::uint64_t v = 0;
    for (std::size_t i = 0; i < n && i < 8; i++) v |= std::uint64_t(b[i]) << (8 * i);
    return v;
}

}  // namespace cb::emulate::tracer
