// codebreaker/platforms/decoder.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/platforms/decoder.hpp: the decoder part of the old
// platforms/package.hpp (`platforms::decoder(bits, arch, detail)`, `Decoder`, `Insn`) for the ports that still use it.
// No logic: conversion and forwarding. Staged under native/cb/compat/shims/ until the exe's build is wired for C++20;
// the registry part of package.hpp (Ref, Loader, Platform ... and the module names they carry) has no shim: it was
// Python-module-name data, and its few consumers are refactored onto cb::platforms::Registry instead.
//
// One decoder per thread and option set, as the old one kept them: each thread has a DecoderSet.
#pragma once

#include <functional>
#include <memory>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/capstone_api.hpp"
#include "cb/platforms/decoder.hpp"

namespace codebreaker::platforms {

// One decoded instruction: what capstone's CsInsn says that every caller reads.
struct Insn {
    int64_t address = 0;
    int64_t size = 0;
    py::str mnemonic;
    py::str op_str;
    py::bytes bytes;
};

class Decoder {
  public:
    Decoder(const cb::platforms::Decoder& inner, const py::str& arch_, int64_t bits_, bool detail_)
        : arch(arch_), bits(bits_), detail(detail_), handle(inner.native_handle()), inner_(&inner) {}

    py::list<Insn> disasm(const py::bytes& code, int64_t address) const {
        py::list<Insn> out;
        const cb::Bytes raw = cb::compat::from_py(code);
        for (const cb::platforms::Insn& i : cb::compat::guard([&] { return inner_->disasm(raw, static_cast<std::uint64_t>(address)); })) {
            Insn x;
            x.address = static_cast<int64_t>(i.address);
            x.size = i.size;
            x.mnemonic = cb::compat::to_py(i.mnemonic);
            x.op_str = cb::compat::to_py(i.op_str);
            x.bytes = cb::compat::to_py(i.raw());
            out.append(x);
        }
        return out;
    }
    void each(const std::string& code, int64_t address, size_t count, const std::function<bool(const cs_insn&)>& visit) const {
        cb::compat::guard([&] {
            inner_->each(cb::as_bytes(code), static_cast<std::uint64_t>(address), count, visit);
            return 0;
        });
    }
    // The same records as capstone fills them, for a caller that walks them more than once.
    cs_insn* raw(const uint8_t* code, size_t size, int64_t address, size_t count, size_t& n) const {
        cs_insn* insn = nullptr;
        n = cb::disasm::capstone::api().cs_disasm(static_cast<csh>(handle), code, size, static_cast<uint64_t>(address), count, &insn);
        return insn;
    }
    void release(cs_insn* insn, size_t n) const {
        if (insn) cb::disasm::capstone::api().cs_free(insn, n);
    }
    const char* reg_name(uint32_t reg) const { return inner_->register_name(reg); }
    std::optional<std::tuple<std::vector<uint16_t>, std::vector<uint16_t>>> regs_access(const cs_insn& insn) const {
        auto a = inner_->register_access(insn);
        if (!a) return std::nullopt;
        return std::make_tuple(a->read, a->written);
    }
    py::str arch;
    int64_t bits = 0;
    bool detail = false;
    uintptr_t handle = 0;                          // capstone's csh

  private:
    const cb::platforms::Decoder* inner_;
};

// A capstone decoder for `arch` ("x86": 16, 32 or 64 bits; "ppc": 32 or 64 bits, big-endian), kept per thread and option
// set. ValueError for any other arch; KeyError for a width capstone has no mode for (as the old table raised it).
inline std::shared_ptr<Decoder> decoder(int64_t bits = 16, const py::str& arch = py::str("x86"), bool detail = false) {
    thread_local cb::platforms::DecoderSet set;
    thread_local std::map<std::tuple<std::string, int64_t, bool>, std::shared_ptr<Decoder>> held;
    const auto key = std::make_tuple(arch.utf8(), bits, detail);
    if (auto it = held.find(key); it != held.end()) return it->second;
    cb::platforms::Arch a;
    if (arch == py::str("x86")) a = cb::platforms::Arch::X86;
    else if (arch == py::str("ppc")) a = cb::platforms::Arch::Ppc;
    else throw py::ValueError("no capstone decoder for " + py::repr(arch).utf8());
    if (bits < 0 || bits > 64) throw py::KeyError(std::to_string(bits));
    try {
        auto d = std::make_shared<Decoder>(set.get(a, static_cast<unsigned>(bits), detail), arch, bits, detail);
        held[key] = d;
        return d;
    } catch (const cb::UsageError&) {
        throw py::KeyError(std::to_string(bits));
    }
}

}  // namespace codebreaker::platforms
