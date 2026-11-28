// emulate/cpu/pcode_cpu_arm_unicorn.hpp -- Unicorn's 32-bit ARM (QEMU's translator, CPU model Cortex-A15: ARMv7-A with VFPv4 and Advanced SIMD) as the independent check of the
// Advanced SIMD and VFP tests: one instruction run from given registers. Header only, included by the tests.
#pragma once

#include <cstdint>
#include <cstring>
#include <string>

#include "core/unicorn_api.hpp"

namespace cb::emulate::arm_uc {

using u64 = std::uint64_t;
using u32 = std::uint32_t;

constexpr u32 kCode = 0x10000;

// What the tests compare: the core registers r0-r12, the flags, the 32 D registers and the status register of the floating point unit.
struct State {
    u32 r[13] = {};
    u32 nzcv = 0;     // bits 31:28
    u64 d[32] = {};
    u32 fpscr = 0;
};

struct Unicorn {
    uc_engine* uc = nullptr;
    Unicorn() {
        const auto& u = emu::unicorn::api();
        emu::unicorn::check(u.uc_open(UC_ARCH_ARM, UC_MODE_ARM, &uc), "uc_open");
        emu::unicorn::check(u.uc_ctl(uc, UC_CTL_WRITE(UC_CTL_CPU_MODEL, 1), static_cast<int>(UC_CPU_ARM_CORTEX_A15)), "uc_ctl cpu model");
        emu::unicorn::check(u.uc_mem_map(uc, kCode, 0x1000, UC_PROT_ALL), "uc_mem_map");
        u32 cpacr = 0xF << 20;   // cp10 and cp11 fully accessible
        u.uc_reg_write(uc, UC_ARM_REG_C1_C0_2, &cpacr);
        u32 fpexc = 1u << 30;    // the floating point unit on
        u.uc_reg_write(uc, UC_ARM_REG_FPEXC, &fpexc);
    }
    ~Unicorn() { emu::unicorn::api().uc_close(uc); }
    Unicorn(const Unicorn&) = delete;
    Unicorn& operator=(const Unicorn&) = delete;
};

// Run the instruction `code` (4 bytes: an ARM word, or the two halfwords of a 32-bit Thumb instruction, or 2 bytes of a 16-bit one) from `in`; false (and `why`) if Unicorn
// faults (an instruction its CPU lacks or leaves undefined).
inline bool run_unicorn(Unicorn& e, const std::uint8_t* code, unsigned length, bool thumb, const State& in, State& out, std::string& why) {
    const auto& u = emu::unicorn::api();
    u.uc_mem_write(e.uc, kCode, code, length);
    for (unsigned i = 0; i < 13; ++i) u.uc_reg_write(e.uc, UC_ARM_REG_R0 + static_cast<int>(i), &in.r[i]);
    for (unsigned i = 0; i < 32; ++i) u.uc_reg_write(e.uc, UC_ARM_REG_D0 + static_cast<int>(i), &in.d[i]);
    u32 cpsr = (in.nzcv & 0xF0000000u) | 0x13u | (thumb ? 0x20u : 0u), fpscr = in.fpscr, sp = 0x20000, lr = 0, fpexc = 1u << 30;
    u.uc_reg_write(e.uc, UC_ARM_REG_FPEXC, &fpexc);
    u.uc_reg_write(e.uc, UC_ARM_REG_FPSCR, &fpscr);
    u.uc_reg_write(e.uc, UC_ARM_REG_SP, &sp);
    u.uc_reg_write(e.uc, UC_ARM_REG_LR, &lr);
    u.uc_reg_write(e.uc, UC_ARM_REG_CPSR, &cpsr);
    const uc_err err = u.uc_emu_start(e.uc, kCode | (thumb ? 1u : 0u), kCode + length, 0, 1);
    if (err != UC_ERR_OK) {
        why = u.uc_strerror(err);
        return false;
    }
    out = in;
    for (unsigned i = 0; i < 13; ++i) u.uc_reg_read(e.uc, UC_ARM_REG_R0 + static_cast<int>(i), &out.r[i]);
    for (unsigned i = 0; i < 32; ++i) u.uc_reg_read(e.uc, UC_ARM_REG_D0 + static_cast<int>(i), &out.d[i]);
    u32 c = 0, f = 0;
    u.uc_reg_read(e.uc, UC_ARM_REG_CPSR, &c);
    u.uc_reg_read(e.uc, UC_ARM_REG_FPSCR, &f);
    out.nzcv = c & 0xF0000000u;
    out.fpscr = f;
    return true;
}

}  // namespace cb::emulate::arm_uc
