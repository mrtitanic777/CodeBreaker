// codebreaker/exact/aim.hpp -- compat shim for cb/exact/aim.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion and
// forwarding. Deleted with its last old-layer consumer (exact/draft/prove, ui/mcp_server).
//
// The instructions are read by codebreaker::exact::x86's installed decoder (x86::set_decoder), as before.
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/aim.hpp"
#include "codebreaker/exact/x86.hpp"
#include "py.hpp"

namespace codebreaker::exact::aim {

inline const std::tuple<py::str, py::str, py::str, py::str, py::str> KINDS{
    py::str("frame"), py::str("width"), py::str("sign"), py::str("order"), py::str("shape")};

inline const py::dict<py::str, py::str>& signed_pairs() {
    static const py::dict<py::str, py::str> d = [] {
        py::dict<py::str, py::str> out;
        for (const auto& [from, to] : std::initializer_list<std::pair<const char*, const char*>>{
                 {"jl", "jb"}, {"jle", "jbe"}, {"jg", "ja"}, {"jge", "jae"}, {"sar", "shr"}, {"imul", "mul"},
                 {"idiv", "div"}, {"cwd", "xor"}, {"cbw", "sub"}})
            out.set(py::str(from), py::str(to));
        return out;
    }();
    return d;
}

// One instruction of a listing: (offset, bytes, the source line before it or None)
using ListingEntry = std::tuple<int64_t, py::bytes, std::optional<int64_t>>;

// diagnose()'s result: {"kinds": {kind: n}, "lines": [source lines of the differing stretches, first first],
// "first": line}
struct Diagnosis {
    py::dict<py::str, int64_t> kinds;
    py::list<int64_t> lines;
    std::optional<int64_t> first;
};
template <class F> void for_each_field(const Diagnosis& r, F&& f) {
    f("kinds", r.kinds);
    f("lines", r.lines);
    f("first", r.first);
}
template <class F> void for_each_field_mut(Diagnosis& r, F&& f) {
    f("kinds", r.kinds, true);
    f("lines", r.lines, true);
    f("first", r.first, true);
}

// difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes(): (tag, i1, i2, j1, j2) with tag "replace" /
// "delete" / "insert" / "equal"
using Opcode = std::tuple<py::str, int64_t, int64_t, int64_t, int64_t>;

namespace detail {
inline std::vector<cb::exact::aim::ListingEntry> entries(const py::list<ListingEntry>& listing) {
    std::vector<cb::exact::aim::ListingEntry> out;
    for (const ListingEntry& e : listing) {
        cb::exact::aim::ListingEntry x;
        x.offset = cb::compat::int_from_py<std::uint32_t>(std::get<0>(e));
        x.bytes = cb::compat::from_py(std::get<1>(e));
        if (std::get<2>(e)) x.line = cb::compat::int_from_py<std::uint32_t>(*std::get<2>(e));
        out.push_back(std::move(x));
    }
    return out;
}
inline std::vector<std::string> strings(const py::list<py::str>& v) {
    std::vector<std::string> out;
    for (const py::str& s : v) out.push_back(cb::compat::from_py(s));
    return out;
}
}  // namespace detail

inline py::list<ListingEntry> listing_code(const py::str& text, const std::optional<py::str>& name = std::nullopt) {
    return cb::compat::guard([&] {
        const std::string t = cb::compat::from_py(text);
        std::optional<std::string> n;
        if (name) n = cb::compat::from_py(*name);
        py::list<ListingEntry> out;
        for (const auto& e : cb::exact::aim::listing_code(t, n ? std::optional<std::string_view>(*n) : std::nullopt)) {
            std::optional<int64_t> line;
            if (e.line) line = *e.line;
            out.append(ListingEntry(e.offset, cb::compat::to_py(cb::ByteView(e.bytes)), line));
        }
        return out;
    });
}

inline py::str _norm(const x86::Insn& ins, int64_t /*base*/) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::exact::aim::normalize(x86::detail::from_old(ins))); });
}

inline py::list<x86::Insn> _insns(const py::bytes& code) { return x86::disasm(code, 0, py::len(code), true); }

inline py::str _kind(const py::list<py::str>& a, const py::list<py::str>& b) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cb::exact::aim::kind_name(cb::exact::aim::classify(detail::strings(a), detail::strings(b))));
    });
}

inline Diagnosis diagnose(const py::bytes& orig, const py::bytes& code, const py::list<ListingEntry>& listing) {
    return cb::compat::guard([&] {
        const cb::Bytes o = cb::compat::from_py(orig), c = cb::compat::from_py(code);
        const auto d = cb::exact::aim::diagnose(o, c, detail::entries(listing), x86::detail::active());
        Diagnosis out;
        for (const auto& [kind, n] : d.kinds) out.kinds.set(cb::compat::to_py(cb::exact::aim::kind_name(kind)), static_cast<int64_t>(n));
        for (const auto line : d.lines) out.lines.append(static_cast<int64_t>(line));
        if (d.first()) out.first = static_cast<int64_t>(*d.first());
        return out;
    });
}

inline py::bytes code_of(const py::list<ListingEntry>& listing) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cb::exact::aim::code_of(detail::entries(listing)))); });
}

inline py::list<Opcode> _get_opcodes(const py::list<py::str>& a, const py::list<py::str>& b) {
    return cb::compat::guard([&] {
        py::list<Opcode> out;
        for (const auto& op : cb::exact::aim::opcodes(detail::strings(a), detail::strings(b))) {
            using Tag = cb::exact::aim::Opcode::Tag;
            const char* tag = op.tag == Tag::Replace ? "replace" : op.tag == Tag::Delete ? "delete" : op.tag == Tag::Insert ? "insert" : "equal";
            out.append(Opcode{py::str(tag), static_cast<int64_t>(op.a1), static_cast<int64_t>(op.a2), static_cast<int64_t>(op.b1),
                              static_cast<int64_t>(op.b2)});
        }
        return out;
    });
}

}  // namespace codebreaker::exact::aim
