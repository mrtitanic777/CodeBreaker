// codebreaker/decompile/m6502.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/m6502.{hpp,cpp}; this header keeps the old py:: surface for the ports that still call it
// (build/m6502_asm, emulate/m6502, formats/nes, source/nes_writer). Conversion and forwarding only; deleted with
// its last py:: consumer.
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/m6502.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::decompile::m6502 {

namespace detail {
namespace nm = ::cb::decompile::m6502;

inline py::str mode_py(nm::Mode m) { return ::cb::compat::to_py(nm::mode_name(m)); }
inline nm::Mode mode_of(const py::str& name) {
    for (int i = 0; i <= static_cast<int>(nm::Mode::Rel); ++i) {
        if (mode_py(static_cast<nm::Mode>(i)) == name) return static_cast<nm::Mode>(i);
    }
    throw py::KeyError(name);
}
}  // namespace detail

// addressing mode -> instruction size in bytes
inline const py::dict<py::str, int64_t> SIZE = [] {
    py::dict<py::str, int64_t> d;
    for (int i = 0; i <= static_cast<int>(detail::nm::Mode::Rel); ++i) {
        auto m = static_cast<detail::nm::Mode>(i);
        d.set(detail::mode_py(m), detail::nm::instruction_size(m));
    }
    return d;
}();
// mnemonic -> {mode: opcode}
using ModeTable = py::dict<py::str, int64_t>;
inline const py::dict<py::str, ModeTable> _GROUPS = [] {
    py::dict<py::str, ModeTable> d;
    const auto& table = detail::nm::opcode_table();
    for (const auto& entry : table) {
        if (!entry) continue;
        ModeTable& modes = d.setdefault(::cb::compat::to_py(entry->mnemonic), ModeTable());
        modes.set(detail::mode_py(entry->mode), *detail::nm::opcode_of(entry->mnemonic, entry->mode));
    }
    return d;
}();
// opcode -> (mnemonic, mode)
inline const py::dict<int64_t, std::tuple<py::str, py::str>> OPCODES = [] {
    py::dict<int64_t, std::tuple<py::str, py::str>> d;
    const auto& table = detail::nm::opcode_table();
    for (std::size_t op = 0; op < table.size(); ++op) {
        if (table[op]) d.set(static_cast<int64_t>(op), {::cb::compat::to_py(table[op]->mnemonic), detail::mode_py(table[op]->mode)});
    }
    return d;
}();
inline const py::set<py::str> BRANCHES = {"BCC", "BCS", "BEQ", "BMI", "BNE", "BPL", "BVC", "BVS"};
inline const py::set<py::str> ENDS = {"JMP", "RTS", "RTI", "BRK"};   // flow does not fall through

// One decoded instruction (the Python's __slots__ class).
struct Insn {
    int64_t at = 0;
    int64_t op = 0;
    py::str mnemonic;
    py::str mode;
    std::optional<int64_t> operand;  // None for imp / acc
    int64_t size = 0;
    std::optional<int64_t> target;   // None unless a branch, JMP or JSR

    Insn() = default;
    Insn(int64_t at, int64_t op, const py::str& mnemonic, const py::str& mode, std::optional<int64_t> operand,
         int64_t size, std::optional<int64_t> target)
        : at(at), op(op), mnemonic(mnemonic), mode(mode), operand(operand), size(size), target(target) {}
    py::str repr() const;  // <8000 LDA #$01>
};

namespace detail {
inline detail::nm::Insn to_new(const Insn& i) {
    detail::nm::Insn n;
    n.at = static_cast<std::uint32_t>(i.at);
    n.op = static_cast<std::uint8_t>(i.op);
    n.mode = mode_of(i.mode);
    const auto& entry = nm::opcode_table()[n.op];
    if (entry) n.mnemonic = entry->mnemonic;
    if (i.operand) n.operand = static_cast<std::uint16_t>(*i.operand);
    n.size = static_cast<std::uint8_t>(i.size);
    if (i.target) n.target = static_cast<std::uint16_t>(*i.target);
    return n;
}
}  // namespace detail

// The instruction at code[offset], which sits at address `pc`; None for an undocumented
// opcode or one cut off by the end of `code`.
inline std::optional<Insn> decode(const py::bytes& code, int64_t offset, int64_t pc) {
    if (offset < 0) throw py::IndexError("bytes index out of range");
    ::cb::Bytes bytes = ::cb::compat::from_py(code);
    auto n = detail::nm::decode(bytes, static_cast<std::size_t>(offset), static_cast<std::uint32_t>(pc));
    if (!n) return std::nullopt;
    Insn out(n->at, n->op, ::cb::compat::to_py(n->mnemonic), detail::mode_py(n->mode), std::nullopt, n->size, std::nullopt);
    if (n->operand) out.operand = *n->operand;
    if (n->target) out.target = *n->target;
    return out;
}

inline py::str Insn::repr() const {
    return py::percent("<%04X %s>", at, py::str(::cb::compat::to_py(detail::nm::text(detail::to_new(*this)))));
}

// The instruction in ca65 syntax; an address that has a label is written as the label.
inline py::str text(const Insn& insn, const py::dict<int64_t, py::str>& labels = py::dict<int64_t, py::str>()) {
    detail::nm::AddressLabels l;
    for (const auto& kv : labels.items()) {
        int64_t address = std::get<0>(kv);
        if (address >= 0 && address <= 0xFFFF) l[static_cast<std::uint16_t>(address)] = ::cb::compat::from_py(std::get<1>(kv));
    }
    return ::cb::compat::to_py(detail::nm::text(detail::to_new(insn), l));
}

// The bytes of one instruction written as text() writes it; ValueError when it is not one.
inline py::bytes assemble(const py::str& line, int64_t pc,
                          const py::dict<py::str, int64_t>& labels = py::dict<py::str, int64_t>()) {
    detail::nm::NameLabels l;
    for (const auto& kv : labels.items()) l.insert_or_assign(::cb::compat::from_py(std::get<0>(kv)), std::get<1>(kv));
    return ::cb::compat::guard([&] {
        ::cb::Bytes b = detail::nm::assemble(::cb::compat::from_py(line), static_cast<std::uint32_t>(pc), l);
        return ::cb::compat::to_py(::cb::ByteView(b));
    });
}

}  // namespace codebreaker::decompile::m6502
