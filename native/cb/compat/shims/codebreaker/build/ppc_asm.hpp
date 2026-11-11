// codebreaker/build/ppc_asm.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/ppc_asm.hpp for the ports that still use the
// old API (none in the typed tree today: the Python formats/xex_edit reached it). No logic: conversion and forwarding.
// Staged under native/cb/compat/shims/ until the exe's build is wired for C++20.
//
// Old API: MASK32, AsmError, assemble_one, assemble. The encoding helpers (_reg, _imm, _mem, _dform, _xform, _spr_enc) and
// the tables only the assembler used are not carried over.
#pragma once

#include "cb/build/ppc_asm.hpp"
#include "cb/compat/py_bridge.hpp"

namespace codebreaker::build::ppc_asm {

inline const int64_t MASK32 = 0xFFFFFFFF;

struct AsmError : py::Exception {
    AsmError() : py::Exception() {}
    explicit AsmError(const std::string& m) : py::Exception(m) {}
    explicit AsmError(const char* m) : py::Exception(m) {}
    explicit AsmError(const py::str& m) : py::Exception(m.utf8()) {}
    const char* type() const override { return "AsmError"; }
};

// Assemble one instruction at virtual address `addr` -> u32 (or None for a blank/comment line). `labels` maps name->address.
inline std::optional<int64_t> assemble_one(const py::str& line, int64_t addr = 0,
                                           const py::dict<py::str, int64_t>& labels = py::dict<py::str, int64_t>()) {
    cb::build::Labels names;
    for (const auto& [name, value] : labels.items()) names[name.utf8()] = value;
    try {
        const auto word = cb::build::ppc_asm::assemble_one(line.utf8(), cb::compat::int_from_py<std::uint32_t>(addr & 0xFFFFFFFF), names);
        if (!word) return std::nullopt;
        return static_cast<int64_t>(*word);
    } catch (const cb::build::AsmError& e) {
        throw AsmError(std::string(e.what()));
    }
}

// Assemble multi-line asm -> big-endian bytes. Two passes for labels.
inline py::bytes assemble(const py::str& text, int64_t base_addr = 0) {
    try {
        const cb::Bytes out = cb::build::ppc_asm::assemble(text.utf8(), cb::compat::int_from_py<std::uint32_t>(base_addr & 0xFFFFFFFF));
        return cb::compat::to_py(cb::ByteView(out));
    } catch (const cb::build::AsmError& e) {
        throw AsmError(std::string(e.what()));
    }
}

}  // namespace codebreaker::build::ppc_asm
