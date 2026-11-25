// engine/pcode_cpu_a64_unicorn.hpp -- Unicorn's AArch64 (QEMU's translator, CPU model "max") as the independent check of the AArch64 tests: one instruction run from given registers.
// Header only, included by the tests (engine/pcode_ops_a64_test.cpp, engine/pcode_cpu_a64_test.cpp).
#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "core/unicorn_api.hpp"
#include "emulate/a64.hpp"

namespace cb::engine::a64_uc {

using u64 = std::uint64_t;
using u32 = std::uint32_t;
namespace ea = cb::emulate::a64;

constexpr u64 kCode = 0x10000;
constexpr u64 kData = 0x100000, kDataBytes = 0x10000;   // (the data window of a Unicorn made with data = true)

struct Unicorn {
    uc_engine* uc = nullptr;
    explicit Unicorn(bool data = false) {
        const auto& u = emu::unicorn::api();
        emu::unicorn::check(u.uc_open(UC_ARCH_ARM64, UC_MODE_ARM, &uc), "uc_open");
        emu::unicorn::check(u.uc_ctl(uc, UC_CTL_WRITE(UC_CTL_CPU_MODEL, 1), static_cast<int>(UC_CPU_ARM64_MAX)), "uc_ctl cpu model");
        emu::unicorn::check(u.uc_mem_map(uc, kCode, 0x1000, UC_PROT_ALL), "uc_mem_map");
        if (data) emu::unicorn::check(u.uc_mem_map(uc, kData, kDataBytes, UC_PROT_ALL), "uc_mem_map data");
        u64 cpacr = 3u << 20;   // FP and Advanced SIMD enabled
        u.uc_reg_write(uc, UC_ARM64_REG_CPACR_EL1, &cpacr);
    }
    ~Unicorn() { emu::unicorn::api().uc_close(uc); }
    Unicorn(const Unicorn&) = delete;
    Unicorn& operator=(const Unicorn&) = delete;
};

inline int x_reg(unsigned i) { return i <= 28 ? UC_ARM64_REG_X0 + static_cast<int>(i) : i == 29 ? UC_ARM64_REG_X29 : UC_ARM64_REG_X30; }

// Run `word` on Unicorn from `in`; false (and `why`) if Unicorn faults (an instruction its CPU lacks). Registers and flags only: no data memory is mapped.
inline bool run_unicorn(Unicorn& e, u32 word, const ea::Registers& in, ea::Registers& out, std::string& why) {
    const auto& u = emu::unicorn::api();
    std::uint8_t code[4];
    for (unsigned i = 0; i < 4; ++i) code[i] = static_cast<std::uint8_t>(word >> (8 * i));
    u.uc_mem_write(e.uc, kCode, code, 4);
    for (unsigned i = 0; i < 31; ++i) u.uc_reg_write(e.uc, x_reg(i), &in.x[i]);
    for (unsigned i = 0; i < 32; ++i) {
        std::uint8_t b[16];
        std::memcpy(b, &in.v[i].lo, 8);
        std::memcpy(b + 8, &in.v[i].hi, 8);
        u.uc_reg_write(e.uc, UC_ARM64_REG_Q0 + static_cast<int>(i), b);
    }
    u64 nzcv = in.nzcv, zero = 0, pc = kCode;
    u.uc_reg_write(e.uc, UC_ARM64_REG_NZCV, &nzcv);
    u.uc_reg_write(e.uc, UC_ARM64_REG_FPCR, &zero);
    u.uc_reg_write(e.uc, UC_ARM64_REG_FPSR, &zero);
    u.uc_reg_write(e.uc, UC_ARM64_REG_PC, &pc);
    const uc_err err = u.uc_emu_start(e.uc, kCode, kCode + 4, 0, 1);
    if (err != UC_ERR_OK) {
        why = u.uc_strerror(err);
        return false;
    }
    out = in;
    for (unsigned i = 0; i < 31; ++i) u.uc_reg_read(e.uc, x_reg(i), &out.x[i]);
    for (unsigned i = 0; i < 32; ++i) {
        std::uint8_t b[16];
        u.uc_reg_read(e.uc, UC_ARM64_REG_Q0 + static_cast<int>(i), b);
        std::memcpy(&out.v[i].lo, b, 8);
        std::memcpy(&out.v[i].hi, b + 8, 8);
    }
    u64 n = 0;
    u.uc_reg_read(e.uc, UC_ARM64_REG_NZCV, &n);
    out.nzcv = static_cast<u32>(n) & 0xF0000000u;
    return true;
}

// run_unicorn with the data window: `data` (kDataBytes bytes from kData) is written first and read back after the instruction.
inline bool run_unicorn_data(Unicorn& e, u32 word, const ea::Registers& in, std::vector<std::uint8_t>& data, ea::Registers& out, std::string& why) {
    const auto& u = emu::unicorn::api();
    u.uc_mem_write(e.uc, kData, data.data(), data.size());
    if (!run_unicorn(e, word, in, out, why)) return false;
    u.uc_mem_read(e.uc, kData, data.data(), data.size());
    return true;
}

// The registers the tests compare: the general ones, the vector ones and the flags (not the stack pointer, the pc or the floating-point status).
inline bool same_registers(const ea::Registers& a, const ea::Registers& b) {
    for (unsigned i = 0; i < 31; ++i)
        if (a.x[i] != b.x[i]) return false;
    for (unsigned i = 0; i < 32; ++i)
        if (!(a.v[i] == b.v[i])) return false;
    return a.nzcv == b.nzcv;
}

}  // namespace cb::engine::a64_uc
