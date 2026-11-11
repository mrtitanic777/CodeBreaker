// cb/exact/habits/symx.hpp -- do two stretches of 8086 code without a jump in them compute the same?
//
// Used by newcomp where no habit of the later compiler it knows explains a difference. Both stretches are run on
// symbols from the same start (every register holds the same unknown value in both); they are the same if they push
// the same values, store the same values at the same places in the same order, and leave the same values in the
// frame's temporaries - which may be other temporaries, each of the one for one of the other. Sums, products and ORs
// are compared without regard to the order of their terms, a shift to the left as a product. The one jump allowed is
// the compiler's way to a Boolean's value, mov R,0 / jcc over / inc R. What it does not know it refuses.
//
// The instructions are the text the disassembler prints ("mov ax, word ptr [bp - 4]") next to their bytes (the
// bytes matter only to recognise that conditional jump).
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/core/ordered_map.hpp"

namespace cb::exact::habits::symx {

// A word or a byte of memory: the size an operand names (`word ptr [..]`, `byte ptr [..]`).
enum class Size { Byte, Word };

// A symbolic value. Which fields a kind uses:
//
//   Const    n, a 16-bit constant                    Linker   a constant the linker fills in (any value)
//   Reg      name, the unknown a register held at the start ("bp" too)
//   Neg      -args[0]                                 Lo       the low byte of args[0]
//   LoHi     args[0], the low byte, over args[1], the register it was put in
//   Zx       args[0] zero-extended
//   Sum, Product, Or, And, Xor   args = the terms in canonical order (a constant, if any, last)
//   Mem      args[0] = the address, size, n = how many stores had been made when it was read
//   Ord      n = the conditional jump's opcode, args = the flags it tested (empty: none known)  -- a Boolean
//   Shr      args[0] shifted right by n               High     the high word of the product args[0]
//   Div, Mod args = dx, ax, the divisor: the quotient / the remainder
//   Res      the flags of the result args[0]          Cmp      the flags of comparing args[0] with args[1]
struct Expr {
    enum class Kind { Const, Linker, Reg, Neg, Lo, LoHi, Zx, Sum, Product, Or, And, Xor, Mem, Ord, Shr, High, Div, Mod, Res, Cmp };
    Kind kind = Kind::Const;
    std::int64_t n = 0;
    std::string name;
    Size size = Size::Word;
    std::vector<Expr> args;
    friend bool operator==(const Expr&, const Expr&) = default;
};

// The expression as text, for a reason a person reads: "(+ ax 3)".
std::string show(const Expr& e);

// One instruction of a stretch.
struct Instruction {
    std::string text;
    Bytes bytes;
};

// For each instruction (by index in the stretch), the values in it that a linker fills in: a number equal to one of
// them is "some constant" rather than that constant.
using LinkerWords = std::map<std::size_t, std::set<std::int64_t>>;

inline constexpr std::size_t kRegisterCount = 6;   // ax cx dx bx si di

struct Store {
    Expr address;
    Size size = Size::Word;
    Expr value;
};

struct Slot {
    Size size = Size::Word;
    Expr value;
};

// The state after a stretch.
struct State {
    std::array<Expr, kRegisterCount> regs;   // ax cx dx bx si di
    std::vector<Expr> pushes;
    std::vector<Store> stores;                 // memory the frame does not own, in the order stored
    OrderedMap<std::int64_t, Slot> slots;      // the frame's temporaries, by offset from bp, in the order first stored
    std::optional<Expr> flags;                 // what the last flag-setting instruction left, if known
    const Expr& reg(std::string_view name) const;   // by name; CB_ASSERT on a name that is not one of the six
};

// Run the stretch on symbols. UnsupportedError when it holds something this does not know (its message says what).
Result<State> try_run(std::span<const Instruction> ins, const LinkerWords& linker_words);

struct Options {
    bool registers = false;   // the registers must end holding the same values too
    bool flags = false;       // and the flags (what the stretch's last comparison or result was)
};

struct Verdict {
    bool same = false;
    std::string why;   // when not the same: what differs
};

// Are `want` and `got` the same computation? `linker_words` are those of `got` (the original has none).
Verdict same(std::span<const Instruction> want, std::span<const Instruction> got, const LinkerWords& linker_words,
             Options options = {});

}  // namespace cb::exact::habits::symx
