// codebreaker/exact/x86.hpp -- compat shim for cb/exact/x86.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion and
// forwarding; the one piece of old-world state is the installed decoder (`set_decoder`). Deleted with its last
// old-layer consumer.
#pragma once

#include <functional>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/x86.hpp"
#include "py.hpp"

namespace codebreaker::exact::x86 {

struct Insn {
    int64_t address = 0;
    py::bytes bytes;
    py::str mnemonic;
    py::str op_str;
    int64_t size = 0;

    Insn() = default;
    Insn(int64_t a, const py::bytes& b, const py::str& m, const py::str& o)
        : address(a), bytes(b), mnemonic(m), op_str(o), size(py::len(b)) {}
    friend bool operator==(const Insn& a, const Insn& b) {
        return a.address == b.address && a.bytes == b.bytes && a.mnemonic == b.mnemonic && a.op_str == b.op_str;
    }
};

// An installed decoder: given the bytes at `address`, the first instruction they decode to, or nullopt.
using Decoder = std::function<std::optional<Insn>(const py::bytes& code, int64_t address)>;

namespace detail {
inline Insn to_old(const cb::exact::x86::Insn& i) {
    return Insn(i.address, cb::compat::to_py(cb::ByteView(i.bytes)), cb::compat::to_py(i.mnemonic), cb::compat::to_py(i.op_str));
}
inline cb::exact::x86::Insn from_old(const Insn& i) {
    return cb::exact::x86::Insn{cb::compat::int_from_py<std::uint32_t>(i.address), cb::compat::from_py(i.bytes),
                                cb::compat::from_py(i.mnemonic), cb::compat::from_py(i.op_str)};
}
inline Decoder& installed() {   // (the old layer's one decoder slot)
    static Decoder d;
    return d;
}
// The new Decoder16 over the installed old decoder.
class Installed final : public cb::exact::x86::Decoder16 {
public:
    std::optional<cb::exact::x86::Insn> first(cb::ByteView code, std::uint32_t address) const override {
        const auto i = installed()(cb::compat::to_py(code), address);
        if (!i) return std::nullopt;
        return from_old(*i);
    }
};
inline const cb::exact::x86::Decoder16& active() {
    static const Installed installed_decoder;
    if (installed()) return installed_decoder;
    return cb::exact::x86::capstone_decoder();
}
}  // namespace detail

inline void set_decoder(Decoder d) { detail::installed() = std::move(d); }

inline py::list<Insn> disasm(const py::bytes& img, int64_t at, int64_t end, bool emulator = true) {
    return cb::compat::guard([&] {
        py::list<Insn> out;
        const cb::Bytes bytes = cb::compat::from_py(img);
        for (const auto& i : cb::exact::x86::disasm(bytes, cb::compat::int_from_py<std::uint32_t>(at), cb::compat::int_from_py<std::uint32_t>(end),
                                                    emulator, detail::active()))
            out.append(detail::to_old(i));
        return out;
    });
}

inline py::dict<int64_t, int64_t> tables(const py::list<Insn>& insns, const py::bytes& img, int64_t lo, int64_t hi, int64_t base = 0) {
    return cb::compat::guard([&] {
        std::vector<cb::exact::x86::Insn> converted;
        for (const Insn& i : insns) converted.push_back(detail::from_old(i));
        const cb::Bytes bytes = cb::compat::from_py(img);
        return cb::compat::to_py(cb::exact::x86::tables(converted, bytes, cb::compat::int_from_py<std::uint32_t>(lo),
                                                        cb::compat::int_from_py<std::uint32_t>(hi), cb::compat::int_from_py<std::uint32_t>(base)));
    });
}

inline py::list<Insn> body(const py::bytes& img, int64_t lo, int64_t hi, bool emulator = true, int64_t base = 0) {
    return cb::compat::guard([&] {
        py::list<Insn> out;
        const cb::Bytes bytes = cb::compat::from_py(img);
        for (const auto& i : cb::exact::x86::body(bytes, cb::compat::int_from_py<std::uint32_t>(lo), cb::compat::int_from_py<std::uint32_t>(hi),
                                                  emulator, cb::compat::int_from_py<std::uint32_t>(base), detail::active()))
            out.append(detail::to_old(i));
        return out;
    });
}

}  // namespace codebreaker::exact::x86
