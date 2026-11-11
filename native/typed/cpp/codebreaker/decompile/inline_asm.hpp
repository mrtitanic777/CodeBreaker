// codebreaker/decompile/inline_asm.hpp -- typed C++ port of codebreaker.decompile.inline_asm.
//
// inline_asm.py -- a 16-bit function as Microsoft C 8 source whose body is one _asm block.
//
//     from codebreaker.decompile import inline_asm
//     text, why = inline_asm.whole_body(name, code, start, end, fixups, name_at, result="int2")
//
// A function analyze/origin finds holding an _asm block (INT n, IN/OUT, CLI/STI, the carry -- inside
// the BP frame C 8 gives every function with one) was written as C with assembly in it. Its first
// form is the whole body as the block: the instructions between the frame C 8 makes and the return
// it makes, written back in the inline assembler's words. C 8 then builds the frame round them --
// `push bp / mov bp, sp` or `enter n, 0`, `leave`, the return its convention says -- so whether the
// text is the function is the compiler's to say, byte for byte (verify/authored/match16.py: the "asm"
// way of saying). Measured with C 8.00c (2026-10-28): SUWIN's 2:7E3C (`mov bx, h / mov ah, 68h /
// int 21h`) and 8:0538 (INT 2Fh, then C round a second block) come out as the original's bytes.
//
// What the block keeps as the original has it: places in the frame as [bp+k] / [bp-k] (the frame is
// declared as many bytes as the original's), the module's data as ds:[n]. What it names: jumps
// within the function as labels, calls (a far call's fix-up, a near call's target) by the name the
// module's header declares. A function this cannot say (a jump out of it, a return in its middle,
// a fix-up it cannot name) is left as the decompiler wrote it: -> (None, why).
//
// Typing notes:
//   * `import capstone.x86 as X` has no C++ counterpart: the decoded instruction is the struct Instruction below (the
//     fields of capstone's CsInsn that this module reads), the X86_* constants are capstone 5's numbers. The decoder is
//     platforms::decoder(16, detail=True) (decode16); whole_body also takes another as its first parameter (an override):
//     md.disasm(code, address) as a std::function.
//   * The fixups dict mixes two kinds of key: an offset (a number) -> a target name or None, and the
//     tuple ("segment", offset) -> a segment (a number) or None. Fixups holds the two as two dicts.
//   * `seg` is a segment number (it is what name_at is called with), so int64_t.
//   * whole_body's (C text, None) | (None, why) is a tuple of two optionals.
//   * code is bytes or bytearray: one overload each.
//   * Addresses are uint64_t, as capstone makes them: a negative `start` gives addresses that wrap at 2**64
//     (Python's ints do not wrap, but nothing in whole_body can tell the difference except an
//     instruction that would end exactly at 2**64 or beyond).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::decompile::inline_asm {

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
// One operand of a decoded instruction (capstone keeps reg and imm in one union: both are given as the
// decoder read them).
struct Operand {
    int64_t type = X86_OP_INVALID;
    int64_t reg = 0;
    int64_t imm = 0;
    MemOperand mem;
};
// One decoded instruction (capstone's CsInsn).
struct Instruction {
    uint64_t address = 0;            // (capstone's address is unsigned: a negative start wraps at 2**64)
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

extern const py::set<py::str> _STRING;
extern const py::set<int64_t> _REGS32;
extern const py::set<py::str> _JUMPS;

// Is it an instruction the inline assembler does not take -- the 386's, which /G3 makes: a
// 32-bit register, an operand-size prefix, a near conditional jump?
bool _386(const Instruction& i);

// -> (instructions it takes, frame bytes, loads DS) -- the frame C 8 makes for the function; None if there is none.
std::optional<std::tuple<int64_t, int64_t, bool>> _prologue(const py::list<Instruction>& ins);

// -> (instructions it takes, far, bytes it removes) or None.
std::optional<std::tuple<int64_t, bool, int64_t>> _epilogue(const py::list<Instruction>& ins, bool loadds);

// The instruction's operands in the inline assembler's words (fixups, name_at and seg are not used).
py::str _operands(const Instruction& i, const Fixups& fixups, const NameAt& name_at, int64_t seg);

// md.disasm(code, address) of platforms.decoder(16, detail=True), as Instructions
py::list<Instruction> decode16(const py::bytes& code, uint64_t address);

// The function at code[start:end] as a C function whose body is one _asm block.
// fixups: {offset in the segment: target name or None}; name_at(seg, off) -> a function's name.
// -> (C text, None) or (None, why)
std::tuple<std::optional<py::str>, std::optional<py::str>> whole_body(
    const py::str& name, const py::bytes& code, int64_t start, int64_t end, const Fixups& fixups,
    const NameAt& name_at, int64_t seg, const py::str& result = py::str("int2"));
std::tuple<std::optional<py::str>, std::optional<py::str>> whole_body(
    const py::str& name, const py::bytearray& code, int64_t start, int64_t end, const Fixups& fixups,
    const NameAt& name_at, int64_t seg, const py::str& result = py::str("int2"));
// The same with another decoder (an override of platforms.decoder(16, detail=True): md.disasm(code, address)).
std::tuple<std::optional<py::str>, std::optional<py::str>> whole_body(
    const Decoder& md, const py::str& name, const py::bytes& code, int64_t start, int64_t end, const Fixups& fixups,
    const NameAt& name_at, int64_t seg, const py::str& result = py::str("int2"));
std::tuple<std::optional<py::str>, std::optional<py::str>> whole_body(
    const Decoder& md, const py::str& name, const py::bytearray& code, int64_t start, int64_t end, const Fixups& fixups,
    const NameAt& name_at, int64_t seg, const py::str& result = py::str("int2"));

}  // namespace codebreaker::decompile::inline_asm
