// codebreaker/decompile/z80.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/z80.{hpp,cpp}; this header keeps the old py:: surface for what still calls it: the Z80 interpreter
// (emulate/z80), the SM83 and 68000 assemblers (which use value / _split_operands / _balanced), build/z80_asm,
// source/sms_writer, formats/sms. Conversion and forwarding only; deleted with its last py:: consumer.
#pragma once

#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/z80.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::decompile::z80 {

namespace nz = ::cb::decompile::z80;

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
// KeyError for an unknown label (carrying the name), ValueError for a bad expression or instruction.
template <class F>
decltype(auto) guarded(F&& f) {
    try {
        return std::forward<F>(f)();
    } catch (const nz::UnknownLabel& e) {
        throw py::KeyError(py::str(e.name()));
    } catch (const ::cb::Error& e) {
        ::cb::compat::rethrow_as_py(e);
    }
}
}  // namespace detail

inline const py::list<py::str> R = detail::names(nz::kRegisters);
inline const py::list<py::str> RP = detail::names(nz::kRegisterPairs);
inline const py::list<py::str> RP2 = detail::names(nz::kRegisterPairsAf);
inline const py::list<py::str> CC = detail::names(nz::kConditions);
inline const py::list<py::str> ALU = detail::names(nz::kAlu);
inline const py::list<py::str> ROT = detail::names(nz::kRotates);
inline const py::list<py::str> IM = detail::names(nz::kInterruptModes);

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
inline nz::Insn to_new(const Insn& i) {
    nz::Insn n;
    n.pattern = i.pattern.utf8();
    n.size = static_cast<std::uint8_t>(i.size);
    for (int64_t v : i.values) {
        if (n.value_count < n.values.size()) n.values[n.value_count++] = static_cast<std::int32_t>(v);
    }
    if (i.target) n.target = static_cast<std::uint16_t>(*i.target);
    const std::string f = i.flow.utf8();
    for (int k = 0; k <= static_cast<int>(nz::Flow::Halt); ++k) {
        if (nz::flow_name(static_cast<nz::Flow>(k)) == f) n.flow = static_cast<nz::Flow>(k);
    }
    n.pc = static_cast<std::uint32_t>(i.pc);
    const std::string raw = i.bytes.raw();
    for (std::size_t k = 0; k < raw.size() && k < n.raw.size(); ++k) n.raw[k] = static_cast<std::uint8_t>(raw[k]);
    return n;
}
inline Insn from_new(const nz::Insn& n) {
    Insn i;
    i.pattern = ::cb::compat::to_py(n.pattern);
    i.size = n.size;
    for (std::int32_t v : n.operands()) i.values.append(v);
    if (n.target) i.target = *n.target;
    i.flow = ::cb::compat::to_py(nz::flow_name(n.flow));
    i.pc = n.pc;
    i.bytes = ::cb::compat::to_py(n.bytes());
    return i;
}
}  // namespace detail

// The instruction at code[at:] (it runs at address `pc`), or None if code ends inside it.
inline std::optional<Insn> decode(const py::bytes& code, int64_t at, int64_t pc) {
    if (at < 0) throw py::IndexError("bytes index out of range");
    const ::cb::Bytes bytes = ::cb::compat::from_py(code);
    auto n = nz::decode(bytes, static_cast<std::size_t>(at), static_cast<std::uint32_t>(pc));
    if (!n) return std::nullopt;
    return detail::from_new(*n);
}

inline py::str text(const Insn& insn, const py::dict<int64_t, py::str>& labels = py::dict<int64_t, py::str>()) {
    return ::cb::compat::to_py(nz::text(detail::to_new(insn), detail::address_labels(labels)));
}
inline py::str _plain_text(const Insn& insn, const py::dict<int64_t, py::str>& labels) {
    return ::cb::compat::to_py(nz::plain_text(detail::to_new(insn), detail::address_labels(labels)));
}
inline bool _canonical(const Insn& insn) { return nz::is_canonical(detail::to_new(insn)); }

inline py::str Insn::text() const { return z80::text(*this); }
inline py::str Insn::repr() const { return py::str("Insn(") + z80::text(*this) + py::str(")"); }

// A numeric expression ($1F 1Fh %0101 31 'c', labels, $, + - * / & | ^ ~ << >> and parentheses). KeyError for an unknown
// label, ValueError for a malformed expression.
inline int64_t value(const py::str& expr, const py::dict<py::str, int64_t>& labels, int64_t pc = 0) {
    return detail::guarded([&] { return nz::value(expr.utf8(), detail::name_labels(labels), static_cast<std::uint32_t>(pc)); });
}

inline py::list<py::str> _split_operands(const py::str& s) { return ::cb::compat::to_py(nz::split_operands(s.utf8())); }
inline bool _balanced(const py::str& s) { return nz::balanced(s.utf8()); }

// The bytes of one instruction (`labels` {name: address}); KeyError for an unknown label, ValueError for something that is
// not an instruction.
inline py::bytes assemble(const py::str& line, int64_t pc = 0,
                          const py::dict<py::str, int64_t>& labels = py::dict<py::str, int64_t>()) {
    return detail::guarded([&] {
        ::cb::Bytes b = nz::assemble(line.utf8(), static_cast<std::uint32_t>(pc), detail::name_labels(labels));
        return ::cb::compat::to_py(::cb::ByteView(b));
    });
}

}  // namespace codebreaker::decompile::z80
