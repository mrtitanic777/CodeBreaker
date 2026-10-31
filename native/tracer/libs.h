// libs.h -- Unicorn and Capstone, called through the DLLs Python's packages loaded.
//
// Nothing is linked: cbt_init finds each DLL by the full path Python loaded it from, so the
// handle Python passes (uc._uch) belongs to the same Unicorn this code calls, and an
// instruction decodes to the same text it does in Python (capstone 5.0.x).
#pragma once
#include <cstdint>
#include <cstddef>
#include <unicorn/unicorn.h>
#include <capstone/capstone.h>

namespace cbt {

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
};

struct CsLib {
    decltype(&cs_open) open = nullptr;
    decltype(&cs_option) option = nullptr;
    decltype(&cs_disasm) disasm = nullptr;
    decltype(&cs_free) free = nullptr;
    decltype(&cs_reg_name) reg_name = nullptr;
    decltype(&cs_regs_access) regs_access = nullptr;
};

extern UcLib UC;
extern CsLib CS;

bool load_libs(const wchar_t *unicorn_dll, const wchar_t *capstone_dll, char *err, size_t errlen);

// A register as Python reads it (a uint64 the value is read into, zero first).
inline uint64_t reg(uc_engine *uc, int id) {
    uint64_t v[8] = {0};                    // (room for any register class: only v[0] is used)
    UC.reg_read(uc, id, v);
    return v[0];
}

inline void set_reg(uc_engine *uc, int id, uint64_t value) {
    uint64_t v[8] = {value};
    UC.reg_write(uc, id, v);
}

inline bool read32(uc_engine *uc, uint64_t address, uint32_t &out) {
    uint8_t b[4];
    if (UC.mem_read(uc, address, b, 4) != UC_ERR_OK)
        return false;
    out = uint32_t(b[0]) | uint32_t(b[1]) << 8 | uint32_t(b[2]) << 16 | uint32_t(b[3]) << 24;
    return true;
}

inline uint64_t little(const uint8_t *b, size_t n) {
    uint64_t v = 0;
    for (size_t i = 0; i < n && i < 8; i++)
        v |= uint64_t(b[i]) << (8 * i);
    return v;
}

}  // namespace cbt
