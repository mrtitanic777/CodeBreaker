// codebreaker/decompile/inline_asm.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/inline_asm.{hpp,cpp}; this header keeps the old py:: surface (the Instruction record that
// decompile/describe and decompile/asm_blocks derive from, the tables, the helpers asm_blocks calls, whole_body).
// Conversion and forwarding only; deleted with its last py:: consumer.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/inline_asm.hpp"
#include "py.hpp"

namespace codebreaker::decompile::inline_asm {

namespace ni = ::cb::decompile::inline_asm;

// capstone 5's X86_OP_* and the X86_REG_* this module tests
inline constexpr int64_t X86_OP_INVALID = 0;
inline constexpr int64_t X86_OP_REG = 1;
inline constexpr int64_t X86_OP_IMM = 2;
inline constexpr int64_t X86_OP_MEM = 3;
inline constexpr int64_t X86_REG_EAX = 19;
inline constexpr int64_t X86_REG_EBP = 20;
inline constexpr int64_t X86_REG_EBX = 21;
inline constexpr int64_t X86_REG_ECX = 22;
inline constexpr int64_t X86_REG_EDI = 23;
inline constexpr int64_t X86_REG_EDX = 24;
inline constexpr int64_t X86_REG_ESI = 29;
inline constexpr int64_t X86_REG_ESP = 30;

// op.mem of a capstone operand
struct MemOperand {
    int64_t segment = 0;
    int64_t base = 0;
    int64_t index = 0;
    int64_t scale = 0;
    int64_t disp = 0;
};
// One operand of a decoded instruction.
struct Operand {
    int64_t type = X86_OP_INVALID;
    int64_t reg = 0;
    int64_t imm = 0;
    MemOperand mem;
};
// One decoded instruction (capstone's CsInsn).
struct Instruction {
    uint64_t address = 0;
    int64_t size = 0;
    py::str mnemonic;
    py::str op_str;
    py::bytes bytes;                 // the instruction's own bytes
    py::list<int64_t> prefix;        // its four prefix bytes
    py::list<Operand> operands;
};
// md.disasm(code, address)
using Decoder = std::function<py::list<Instruction>(const py::bytes&, uint64_t)>;

// {offset in the segment: target name or None} and the ("segment", offset) keys: {offset: segment or None}
struct Fixups {
    py::dict<int64_t, std::optional<py::str>> names;
    py::dict<int64_t, std::optional<int64_t>> segments;
};
// name_at(seg, off): a function's name, or None
using NameAt = std::function<std::optional<py::str>(int64_t seg, int64_t off)>;

namespace detail {

inline ni::Instruction to_new(const Instruction& i) {
    ni::Instruction n;
    n.address = static_cast<std::uint32_t>(i.address);
    n.size = static_cast<std::uint8_t>(i.size);
    n.mnemonic = i.mnemonic.utf8();
    n.op_str = i.op_str.utf8();
    n.bytes = ::cb::compat::from_py(i.bytes);
    for (std::size_t k = 0; k < n.prefix.size() && k < static_cast<std::size_t>(py::len(i.prefix)); ++k) {
        n.prefix[k] = static_cast<std::uint8_t>(i.prefix[static_cast<int64_t>(k)]);
    }
    for (const Operand& o : i.operands) {
        ni::Operand no;
        no.kind = o.type == X86_OP_REG   ? ni::OperandKind::Reg
                  : o.type == X86_OP_IMM ? ni::OperandKind::Imm
                  : o.type == X86_OP_MEM ? ni::OperandKind::Mem
                                         : ni::OperandKind::Invalid;
        no.reg = static_cast<std::uint16_t>(o.reg);
        no.imm = o.imm;
        no.mem = {static_cast<std::uint16_t>(o.mem.segment), static_cast<std::uint16_t>(o.mem.base),
                  static_cast<std::uint16_t>(o.mem.index), static_cast<std::uint8_t>(o.mem.scale), o.mem.disp};
        n.operands.push_back(no);
    }
    return n;
}

inline Instruction from_new(const ni::Instruction& n) {
    Instruction i;
    i.address = n.address;
    i.size = n.size;
    i.mnemonic = ::cb::compat::to_py(n.mnemonic);
    i.op_str = ::cb::compat::to_py(n.op_str);
    i.bytes = ::cb::compat::to_py(::cb::ByteView(n.bytes));
    for (std::uint8_t p : n.prefix) i.prefix.append(p);
    for (const ni::Operand& no : n.operands) {
        Operand o;
        o.type = no.kind == ni::OperandKind::Reg   ? X86_OP_REG
                 : no.kind == ni::OperandKind::Imm ? X86_OP_IMM
                 : no.kind == ni::OperandKind::Mem ? X86_OP_MEM
                                                   : X86_OP_INVALID;
        o.reg = no.reg;
        o.imm = no.imm;
        o.mem = {no.mem.segment, no.mem.base, no.mem.index, no.mem.scale, no.mem.disp};
        i.operands.append(o);
    }
    return i;
}

inline std::vector<ni::Instruction> to_new(const py::list<Instruction>& ins) {
    std::vector<ni::Instruction> out;
    for (const Instruction& i : ins) out.push_back(to_new(i));
    return out;
}

inline py::set<py::str> names_set(std::span<const std::string_view> names) {
    py::set<py::str> s;
    for (std::string_view n : names) s.add(::cb::compat::to_py(n));
    return s;
}

inline py::bytes bytes_of(const py::bytes& b) { return b; }
inline py::bytes bytes_of(const py::bytearray& b) { return py::bytes(b.view()); }

template <class Data>
std::tuple<std::optional<py::str>, std::optional<py::str>> whole_body_(
    const Decoder* md, const py::str& name, const Data& code, int64_t start, int64_t end, const Fixups& fixups,
    const NameAt& name_at, int64_t seg, const py::str& result) {
    ni::Fixups f;
    for (const auto& kv : fixups.names.items()) {
        f.names[static_cast<std::uint32_t>(std::get<0>(kv))] =
            std::get<1>(kv) ? std::optional<std::string>(std::get<1>(kv)->utf8()) : std::nullopt;
    }
    for (const auto& kv : fixups.segments.items()) {
        f.segments[static_cast<std::uint32_t>(std::get<0>(kv))] =
            std::get<1>(kv) ? std::optional<std::uint32_t>(static_cast<std::uint32_t>(*std::get<1>(kv))) : std::nullopt;
    }
    ni::NameAt n = [&](std::uint32_t s, std::uint16_t off) -> std::optional<std::string> {
        auto r = name_at(s, off);
        return r ? std::optional<std::string>(r->utf8()) : std::nullopt;
    };
    ni::Decoder d;
    if (md) {
        d = [md](::cb::ByteView c, std::uint32_t address) {
            std::vector<ni::Instruction> out;
            for (const Instruction& i : (*md)(::cb::compat::to_py(c), address)) out.push_back(to_new(i));
            return out;
        };
    }
    const ::cb::Bytes bytes = ::cb::compat::from_py(bytes_of(code));
    constexpr int64_t kMax = 0xFFFFFFFF;
    auto text = ::cb::compat::guard([&] {
        return ni::whole_body(name.utf8(), bytes, static_cast<std::uint32_t>(std::clamp<int64_t>(start, 0, kMax)),
                              static_cast<std::uint32_t>(std::clamp<int64_t>(end, 0, kMax)), f, n,
                              static_cast<std::uint32_t>(seg), result.utf8(), d);
    });
    if (text.ok()) return {::cb::compat::to_py(text.value()), std::nullopt};
    if (dynamic_cast<const ni::NotAsmBody*>(&text.error()) == nullptr) ::cb::compat::rethrow_as_py(text.error());
    return {std::nullopt, ::cb::compat::to_py(std::string(text.error().what()))};
}

}  // namespace detail

inline const py::set<py::str> _STRING = detail::names_set(ni::string_op_names());
inline const py::set<int64_t> _REGS32 = py::set<int64_t>{X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX,
                                                         X86_REG_ESI, X86_REG_EDI, X86_REG_EBP, X86_REG_ESP};
inline const py::set<py::str> _JUMPS = detail::names_set(ni::jump_names());

// Is it an instruction the inline assembler does not take -- the 386's?
inline bool _386(const Instruction& i) { return ni::is_386(detail::to_new(i)); }

// -> (instructions it takes, frame bytes, loads DS) -- the frame C 8 makes for the function; None if there is none.
inline std::optional<std::tuple<int64_t, int64_t, bool>> _prologue(const py::list<Instruction>& ins) {
    auto p = ni::find_prologue(detail::to_new(ins));
    if (!p) return std::nullopt;
    return std::make_tuple(static_cast<int64_t>(p->taken), p->frame, p->loads_ds);
}

// -> (instructions it takes, far, bytes it removes) or None.
inline std::optional<std::tuple<int64_t, bool, int64_t>> _epilogue(const py::list<Instruction>& ins, bool loadds) {
    auto e = ni::find_epilogue(detail::to_new(ins), loadds);
    if (!e) return std::nullopt;
    return std::make_tuple(static_cast<int64_t>(e->taken), e->far, e->pops);
}

// The instruction's operands in the inline assembler's words (fixups, name_at and seg are not used).
inline py::str _operands(const Instruction& i, const Fixups&, const NameAt&, int64_t) {
    return ::cb::compat::to_py(ni::operands_text(detail::to_new(i)));
}

// md.disasm(code, address) of platforms.decoder(16, detail=True), as Instructions
inline py::list<Instruction> decode16(const py::bytes& code, uint64_t address) {
    py::list<Instruction> out;
    for (const ni::Instruction& n : ::cb::compat::guard([&] {
             return ni::decode16(::cb::compat::from_py(code), static_cast<std::uint32_t>(address));
         })) {
        out.append(detail::from_new(n));
    }
    return out;
}

// The function at code[start:end] as a C function whose body is one _asm block.
// -> (C text, None) or (None, why)
inline std::tuple<std::optional<py::str>, std::optional<py::str>> whole_body(
    const py::str& name, const py::bytes& code, int64_t start, int64_t end, const Fixups& fixups, const NameAt& name_at,
    int64_t seg, const py::str& result = py::str("int2")) {
    return detail::whole_body_(nullptr, name, code, start, end, fixups, name_at, seg, result);
}
inline std::tuple<std::optional<py::str>, std::optional<py::str>> whole_body(
    const py::str& name, const py::bytearray& code, int64_t start, int64_t end, const Fixups& fixups, const NameAt& name_at,
    int64_t seg, const py::str& result = py::str("int2")) {
    return detail::whole_body_(nullptr, name, code, start, end, fixups, name_at, seg, result);
}
// The same with another decoder (an override of platforms.decoder(16, detail=True): md.disasm(code, address)).
inline std::tuple<std::optional<py::str>, std::optional<py::str>> whole_body(
    const Decoder& md, const py::str& name, const py::bytes& code, int64_t start, int64_t end, const Fixups& fixups,
    const NameAt& name_at, int64_t seg, const py::str& result = py::str("int2")) {
    return detail::whole_body_(&md, name, code, start, end, fixups, name_at, seg, result);
}
inline std::tuple<std::optional<py::str>, std::optional<py::str>> whole_body(
    const Decoder& md, const py::str& name, const py::bytearray& code, int64_t start, int64_t end, const Fixups& fixups,
    const NameAt& name_at, int64_t seg, const py::str& result = py::str("int2")) {
    return detail::whole_body_(&md, name, code, start, end, fixups, name_at, seg, result);
}

}  // namespace codebreaker::decompile::inline_asm
