// codebreaker/emulate/capstone_x86.hpp -- SHIM over cb/emulate/capstone_x86.hpp (docs/CPP_DESIGN.md 6.3): the old
// loader's names, forwarding to the new per-thread engines and to cb/core/capstone_api. No logic; deleted with its
// last py:: consumer (the decoders of platforms, the recorder's host).
//
// Differences kept on purpose (docs/CPP_DESIGN.md section 9, decision 1): the capstone.dll is the one beside the
// executable. load_capstone(path) -- the old override, for a host that found the DLL in Python's site-packages -- is
// accepted and ignored, and dll() answers the path in use.
#pragma once

#include <cstdint>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/capstone_api.hpp"
#include "cb/emulate/capstone_x86.hpp"

namespace codebreaker::emulate::capstone_x86 {

// capstone.dll's entry points
struct Api {
    decltype(&cs_open) open = nullptr;
    decltype(&cs_option) option = nullptr;
    decltype(&cs_disasm) disasm = nullptr;
    decltype(&cs_free) free_ = nullptr;
    decltype(&cs_reg_name) reg_name = nullptr;
    decltype(&cs_regs_access) regs_access = nullptr;
};

inline void load_capstone(const py::str&) {}

inline py::str dll() { return cb::compat::to_py(cb::emulate::capstone_x86::dll_path().generic_string()); }

inline const Api& api() {
    static const Api a = [] {
        const auto& cs = cb::disasm::capstone::api();
        Api out;
        out.open = cs.cs_open;
        out.option = cs.cs_option;
        out.disasm = cs.cs_disasm;
        out.free_ = cs.cs_free;
        out.reg_name = cs.cs_reg_name;
        out.regs_access = cs.cs_regs_access;
        return out;
    }();
    return a;
}

// The engine for 16, 32 or 64-bit x86 code (any other width: 32), with (detail) each instruction's operands and
// registers: one of each kind per thread.
inline csh handle(int64_t bits, bool detail) {
    return cb::emulate::capstone_x86::engine(bits == 16 ? 16 : bits == 64 ? 64 : 32, detail);
}

}  // namespace codebreaker::emulate::capstone_x86
