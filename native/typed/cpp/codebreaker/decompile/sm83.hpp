// codebreaker/decompile/sm83.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/sm83.{hpp,cpp}; this header keeps the old py:: surface for what still calls it: the SM83
// interpreter (emulate/sm83), build/sm83_asm, formats/gb and source/gb_writer. Conversion and forwarding only; deleted
// with its last py:: consumer.
#pragma once

#include <algorithm>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/sm83.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "codebreaker/decompile/z80.hpp"

namespace codebreaker::decompile::sm83 {

namespace ns = ::cb::decompile::sm83;

namespace detail {
template <std::size_t N>
py::list<py::str> names(const std::array<std::string_view, N>& a) {
    py::list<py::str> out;
    for (std::string_view s : a) out.append(::cb::compat::to_py(s));
    return out;
}
inline ::cb::decompile::NameLabels name_labels(const py::dict<py::str, int64_t>& labels) {
    ::cb::decompile::NameLabels out;
    for (const auto& kv : labels.items()) out.insert_or_assign(std::get<0>(kv).utf8(), std::get<1>(kv));
    return out;
}
inline ::cb::decompile::AddressLabels address_labels(const py::dict<int64_t, py::str>& labels) {
    ::cb::decompile::AddressLabels out;
    for (const auto& kv : labels.items()) {
        if (std::get<0>(kv) >= 0 && std::get<0>(kv) <= 0xFFFFFFFFll) out[static_cast<std::uint32_t>(std::get<0>(kv))] = std::get<1>(kv).utf8();
    }
    return out;
}
}  // namespace detail

inline const py::list<py::str> R = detail::names(ns::kRegisters);
inline const py::list<py::str> RP = detail::names(ns::kRegisterPairs);
inline const py::list<py::str> RP2 = detail::names(ns::kRegisterPairsAf);
inline const py::list<py::str> CC = detail::names(ns::kConditions);
inline const py::list<py::str> ALU = detail::names(ns::kAlu);
inline const py::list<py::str> ROT = detail::names(ns::kRotates);
inline const py::set<int64_t> ILLEGAL = [] {
    py::set<int64_t> s;
    for (std::uint8_t op : ns::kIllegalOpcodes) s.add(op);
    return s;
}();

// The table: (prefix bytes, opcode) -> pattern (None for an opcode that does not exist).
using TableKey = std::tuple<py::bytes, int64_t>;
using Table = py::dict<TableKey, std::optional<py::str>>;
inline const Table TABLE = [] {
    Table table;
    for (int64_t op = 0; op < 256; ++op) {
        if (op != 0xCB) {
            const auto p = ns::pattern_of(ns::Page::Main, static_cast<std::uint8_t>(op));
            table.set(TableKey(py::bytes(), op), p.empty() ? std::nullopt : std::optional<py::str>(::cb::compat::to_py(p)));
        }
        table.set(TableKey(py::bytes(std::string(1, static_cast<char>(0xCB))), op),
                  std::optional<py::str>(::cb::compat::to_py(ns::pattern_of(ns::Page::CB, static_cast<std::uint8_t>(op)))));
    }
    return table;
}();
inline const py::dict<py::str, int64_t> _PLACE = {{py::str("n"), 1}, {py::str("nn"), 2}, {py::str("e"), 1}, {py::str("s"), 1}, {py::str("h"), 1}};

// the operand words the assembler reads as themselves
inline const py::set<py::str> _WORDS = [] {
    py::set<py::str> words;
    for (const py::list<py::str>* l : {&R, &RP, &RP2, &CC}) {
        for (const py::str& w : *l) words.add(w);
    }
    for (const char* w : {"(BC)", "(DE)", "(C)", "(HL+)", "(HL-)", "(HLI)", "(HLD)", "HL"}) words.add(py::str(w));
    return words;
}();

struct Insn {
    py::str pattern;
    int64_t size = 0;
    py::list<int64_t> values;
    std::optional<int64_t> target;
    py::str flow;
    int64_t pc = 0;
    py::bytes bytes;

    Insn() = default;
    Insn(const py::str& pattern, int64_t size, const py::list<int64_t>& values, std::optional<int64_t> target,
         const py::str& flow, int64_t pc, const py::bytes& raw)
        : pattern(pattern), size(size), values(values), target(target), flow(flow), pc(pc), bytes(raw) {}
    py::str text() const;   // (the property: text(self))
    py::str repr() const;   // Insn(LD A,B)
};

namespace detail {
inline ns::Insn to_new(const Insn& i) {
    ns::Insn n;
    n.pattern = i.pattern.utf8();
    if (!i.values.empty()) n.value = static_cast<std::int32_t>(i.values[0]);
    if (i.target) n.target = static_cast<std::uint16_t>(*i.target);
    const std::string f = i.flow.utf8();
    for (int k = 0; k <= static_cast<int>(ns::Flow::Halt); ++k) {
        if (z80::nz::flow_name(static_cast<ns::Flow>(k)) == f) n.flow = static_cast<ns::Flow>(k);
    }
    n.pc = static_cast<std::uint32_t>(i.pc);
    const std::string raw = i.bytes.raw();
    for (std::size_t k = 0; k < raw.size() && k < n.raw.size(); ++k) n.raw[k] = static_cast<std::uint8_t>(raw[k]);
    n.size = static_cast<std::uint8_t>(std::min(raw.size(), n.raw.size()));   // (the bytes the record holds)
    return n;
}
inline Insn from_new(const ns::Insn& n) {
    Insn i;
    i.pattern = ::cb::compat::to_py(n.pattern);
    i.size = n.size;
    if (n.value) i.values.append(*n.value);
    if (n.target) i.target = *n.target;
    i.flow = ::cb::compat::to_py(z80::nz::flow_name(n.flow));
    i.pc = n.pc;
    i.bytes = ::cb::compat::to_py(n.bytes());
    return i;
}
}  // namespace detail

inline std::optional<py::str> _main(int64_t op) {
    const auto p = ns::pattern_of(ns::Page::Main, static_cast<std::uint8_t>(op));
    if (p.empty()) return std::nullopt;
    return ::cb::compat::to_py(p);
}
inline py::str _cb(int64_t op) { return ::cb::compat::to_py(ns::pattern_of(ns::Page::CB, static_cast<std::uint8_t>(op))); }
inline py::str _flow(const py::str& pattern) { return ::cb::compat::to_py(z80::nz::flow_name(ns::flow_of(pattern.utf8()))); }
inline py::list<py::str> _fields(const py::str& pattern) {
    py::list<py::str> out;
    const std::string p = pattern.utf8();
    for (std::size_t i = 0; i < p.size();) {
        const auto word = [](char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; };
        if (!word(p[i])) {
            ++i;
            continue;
        }
        std::size_t end = i;
        while (end < p.size() && word(p[end])) ++end;
        const std::string w = p.substr(i, end - i);
        if (w == "nn" || w == "n" || w == "e" || w == "s" || w == "h") out.append(py::str(w));
        i = end;
    }
    return out;
}

// The instruction at code[at:] (it runs at address `pc`), None if code ends inside it or the opcode does not exist.
inline std::optional<Insn> decode(const py::bytes& code, int64_t at, int64_t pc) {
    if (at < 0) throw py::IndexError("bytes index out of range");
    const ::cb::Bytes bytes = ::cb::compat::from_py(code);
    auto n = ns::decode(bytes, static_cast<std::size_t>(at), static_cast<std::uint32_t>(pc));
    if (!n) return std::nullopt;
    return detail::from_new(*n);
}

inline py::str text(const Insn& insn, const py::dict<int64_t, py::str>& labels = py::dict<int64_t, py::str>()) {
    try {
        return ::cb::compat::to_py(ns::text(detail::to_new(insn), detail::address_labels(labels)));
    } catch (const ::cb::InternalError&) {
        throw py::IndexError("list index out of range");   // (a hand-made Insn with fewer values than its pattern has fields)
    }
}

inline py::str Insn::text() const { return sm83::text(*this); }
inline py::str Insn::repr() const { return py::str("Insn(") + sm83::text(*this) + py::str(")"); }

// The bytes of one instruction (RGBDS syntax; `labels` {name: address}); KeyError for an unknown label, ValueError for
// something that is not an instruction.
inline py::bytes assemble(const py::str& line, int64_t pc = 0,
                          const py::dict<py::str, int64_t>& labels = py::dict<py::str, int64_t>()) {
    return z80::detail::guarded([&] {
        ::cb::Bytes b = ns::assemble(line.utf8(), static_cast<std::uint32_t>(pc), detail::name_labels(labels));
        return ::cb::compat::to_py(::cb::ByteView(b));
    });
}

// the odometer of operand readings the assembler tries (n nn e s h for each value operand, the first slowest)
inline py::list<py::list<py::str>> _choices(int64_t count) {
    py::list<py::list<py::str>> out;
    if (count == 0) {
        out.append(py::list<py::str>());
        return out;
    }
    for (const char* first : {"n", "nn", "e", "s", "h"}) {
        for (const py::list<py::str>& rest : _choices(count - 1).vec()) {
            py::list<py::str> one;
            one.append(py::str(first));
            for (const py::str& r : rest) one.append(r);
            out.append(one);
        }
    }
    return out;
}

}  // namespace codebreaker::decompile::sm83
