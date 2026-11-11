// codebreaker/decompile/m68k.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/m68k.{hpp,cpp}; this header keeps the old py:: surface for what still calls it: the 68000
// interpreter (emulate/m68k), build/m68k_asm, formats/genesis, source/genesis_writer. Conversion and forwarding only;
// deleted with its last py:: consumer.
#pragma once

#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/m68k.hpp"
#include "codebreaker/decompile/z80.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::decompile::m68k {

namespace nm = ::cb::decompile::m68k;

// An index register: ("D" or "A", number).
using XReg = std::tuple<py::str, int64_t>;

// One operand. kind: D A ind post pre disp index absw absl pcdisp pcindex imm quick label label8 regs ccr sr usp expr.
struct Op {
    py::str kind;
    int64_t reg = 0;
    int64_t value = 0;
    std::optional<XReg> xreg;
    py::str xsize = py::str("W");
    // None; the size of an immediate ("B" "W" "L"); the target address of a label8
    std::variant<std::monostate, py::str, int64_t> size;

    Op() = default;
    explicit Op(const py::str& kind, int64_t reg = 0, int64_t value = 0, const std::optional<XReg>& xreg = std::nullopt,
                const py::str& xsize = py::str("W"))
        : kind(kind), reg(reg), value(value), xreg(xreg), xsize(xsize) {}
    int64_t num() const { return value; }   // `value` as a number
    bool size_is(const char* s) const {
        const py::str* p = std::get_if<py::str>(&size);
        return p && *p == py::str(s);
    }
    py::str key() const;
    friend bool operator==(const Op& a, const Op& b) { return a.key() == b.key(); }
    friend bool operator!=(const Op& a, const Op& b) { return !(a == b); }
};

struct Insn {
    py::str mnemonic;
    py::str size;
    py::list<Op> operands;
    int64_t length = 0;
    std::optional<int64_t> target;
    py::str flow;
    int64_t pc = 0;
    py::bytes bytes;
    int64_t word = 0;

    Insn() = default;
    Insn(const py::str& mnemonic, const py::str& size, const py::list<Op>& operands, int64_t length, int64_t pc,
         const py::bytes& raw, int64_t word)
        : mnemonic(mnemonic), size(size), operands(operands), length(length), flow(py::str("")), pc(pc), bytes(raw), word(word) {}
    py::str shape() const;
    py::str repr() const;   // Insn(MOVE.L D0,D1)
};

namespace detail {

inline nm::OpKind kind_of(const py::str& k) {
    const std::string name = k.utf8();
    for (int i = 0; i <= static_cast<int>(nm::OpKind::Expr); ++i) {
        if (nm::kind_name(static_cast<nm::OpKind>(i)) == name) return static_cast<nm::OpKind>(i);
    }
    return nm::OpKind::Expr;
}
inline nm::Size size_of(const py::str& s) {
    const std::string name = s.utf8();
    for (nm::Size z : {nm::Size::Byte, nm::Size::Word, nm::Size::Long, nm::Size::Short}) {
        if (nm::size_name(z) == name) return z;
    }
    return nm::Size::None;
}

inline nm::Op to_new(const Op& o) {
    nm::Op n;
    n.kind = kind_of(o.kind);
    n.reg = static_cast<std::uint8_t>(o.reg);
    n.value = o.value;
    if (o.xreg) {
        n.index.address = std::get<0>(*o.xreg) == py::str("A");
        n.index.number = static_cast<std::uint8_t>(std::get<1>(*o.xreg));
    }
    n.index.size = size_of(o.xsize) == nm::Size::Long ? nm::Size::Long : nm::Size::Word;
    n.predecrement = o.xsize == py::str("pre");
    if (const py::str* s = std::get_if<py::str>(&o.size)) n.size = size_of(*s);
    if (const int64_t* t = std::get_if<int64_t>(&o.size)) n.target = static_cast<std::uint32_t>(*t);
    return n;
}
inline Op from_new(const nm::Op& n) {
    Op o;
    o.kind = ::cb::compat::to_py(nm::kind_name(n.kind));
    o.reg = n.reg;
    o.value = n.value;
    if (n.kind == nm::OpKind::Index || n.kind == nm::OpKind::PcIndex) {
        o.xreg = XReg(py::str(n.index.address ? "A" : "D"), n.index.number);
        o.xsize = ::cb::compat::to_py(nm::size_name(n.index.size));
    } else if (n.kind == nm::OpKind::Regs) {
        o.xsize = py::str(n.predecrement ? "pre" : "");
    }
    if (n.kind == nm::OpKind::Imm) o.size = ::cb::compat::to_py(nm::size_name(n.size));
    if (n.kind == nm::OpKind::Label8) o.size = static_cast<int64_t>(n.target);
    return o;
}

inline nm::Insn to_new(const Insn& i) {
    nm::Insn n;
    n.mnemonic = i.mnemonic.utf8();
    n.size = size_of(i.size);
    for (const Op& o : i.operands) {
        if (n.op_count < n.ops.size()) n.ops[n.op_count++] = to_new(o);
    }
    n.length = static_cast<std::uint8_t>(i.length);
    if (i.target) n.target = static_cast<std::uint32_t>(*i.target);
    const std::string f = i.flow.utf8();
    for (int k = 0; k <= static_cast<int>(::cb::decompile::z80::Flow::Halt); ++k) {
        if (::cb::decompile::z80::flow_name(static_cast<::cb::decompile::z80::Flow>(k)) == f) n.flow = static_cast<::cb::decompile::z80::Flow>(k);
    }
    n.pc = static_cast<std::uint32_t>(i.pc);
    n.word = static_cast<std::uint16_t>(i.word);
    const std::string raw = i.bytes.raw();
    for (std::size_t k = 0; k < raw.size() && k < n.raw.size(); ++k) n.raw[k] = static_cast<std::uint8_t>(raw[k]);
    return n;
}
inline Insn from_new(const nm::Insn& n) {
    Insn i;
    i.mnemonic = ::cb::compat::to_py(n.mnemonic);
    i.size = ::cb::compat::to_py(nm::size_name(n.size));
    for (const nm::Op& o : n.operands()) i.operands.append(from_new(o));
    i.length = n.length;
    if (n.target) i.target = *n.target;
    i.flow = ::cb::compat::to_py(::cb::decompile::z80::flow_name(n.flow));
    i.pc = n.pc;
    i.bytes = ::cb::compat::to_py(n.bytes());
    i.word = n.word;
    return i;
}

}  // namespace detail

inline py::str Op::key() const { return ::cb::compat::to_py(detail::to_new(*this).key()); }
inline py::str Insn::shape() const { return ::cb::compat::to_py(detail::to_new(*this).shape()); }
inline py::str text(const Insn& insn, const py::dict<int64_t, py::str>& labels = py::dict<int64_t, py::str>());
inline py::str Insn::repr() const { return py::str("Insn(") + text(*this) + py::str(")"); }

// The instruction at code[at:] (running at address `pc`), or None if it is not one (or code ends inside it).
inline std::optional<Insn> decode(const py::bytes& code, int64_t at, int64_t pc) {
    if (at < 0) return std::nullopt;
    const ::cb::Bytes bytes = ::cb::compat::from_py(code);
    auto n = nm::decode(bytes, static_cast<std::size_t>(at), static_cast<std::uint32_t>(pc));
    if (!n) return std::nullopt;
    return detail::from_new(*n);
}

inline py::str operand_text(const Op& o, const py::dict<int64_t, py::str>& labels) {
    return ::cb::compat::to_py(nm::operand_text(detail::to_new(o), z80::detail::address_labels(labels)));
}
inline py::str text(const Insn& insn, const py::dict<int64_t, py::str>& labels) {
    return ::cb::compat::to_py(nm::text(detail::to_new(insn), z80::detail::address_labels(labels)));
}

// A numeric expression (the Z80 module's, with `*` for here). KeyError for an unknown label, ValueError for a bad one.
inline int64_t _value(const py::str& expr, const py::dict<py::str, int64_t>& labels, int64_t pc) {
    return z80::detail::guarded([&] { return nm::value(expr.utf8(), z80::detail::name_labels(labels), static_cast<std::uint32_t>(pc)); });
}

// The bytes of one instruction (Motorola syntax); KeyError for an unknown label, ValueError for what is not an instruction.
inline py::bytes assemble(const py::str& line, int64_t pc = 0, const py::dict<py::str, int64_t>& labels = py::dict<py::str, int64_t>()) {
    return z80::detail::guarded([&] {
        ::cb::Bytes b = nm::assemble(line.utf8(), static_cast<std::uint32_t>(pc), z80::detail::name_labels(labels));
        return ::cb::compat::to_py(::cb::ByteView(b));
    });
}

}  // namespace codebreaker::decompile::m68k
