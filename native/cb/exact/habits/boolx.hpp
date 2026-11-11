// cb/exact/habits/boolx.hpp -- do two stretches of 8086 code decide a condition the same way?
//
// Used by newcomp for MASM 3.00. Its compiler leaves a condition at the first term that settles it; MS-Pascal 3.11 does
// so too, except where the terms share the record of a WITH and are grouped so that AND THEN / OR ELSE cannot say it:
// there it works every term out to 0 or 1 (mov r,0 / jcc / inc r), combines them with AND and OR and tests the result
// once. The two are the same condition if, whatever the comparisons in them come out as, both go on to the same place.
//
// Each stretch is run on every outcome of its comparisons: a comparison of the same two operands is one unknown (less,
// equal or greater), a Boolean byte one (0 or 1), a word masked with a constant one (zero or not). A stretch may load
// registers, compare, combine the 0/1 values and keep one in a temporary of the frame; it leaves by falling through
// its end or by a jump out of it. Anything else it refuses.
#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>

namespace cb::exact::habits::boolx {

// One instruction as the disassembler prints it ("cmp ax, word ptr [si + 4]", "jb 0x1c2"), and where it is. A jump's
// operand is the target's offset; a variable's address ("[0x1234]") is replaced by the caller with a name that is
// the same in both stretches ("[g0]") where the linker's address is not.
struct Instruction {
    std::uint32_t offset = 0;
    std::string text;
};

// What SI and DI hold when the stretch starts: a name for it ("si0": whatever it was), or the memory operand it was
// loaded from ("bp - 6"), so that the same variable reached through SI in one stretch and by its own address in the
// other is the same variable.
struct Bases {
    std::string si = "si0";
    std::string di = "di0";
};

// A stretch of code at offsets [lo, hi): a jump to `hi` leaves at the end, as falling off the last instruction does.
struct Stretch {
    std::span<const Instruction> instructions;
    std::uint32_t lo = 0;
    std::uint32_t hi = 0;
    Bases bases;
};

// Where the two go when the condition is false: the original's jump target, the compiled one's.
struct ElseTargets {
    std::uint32_t original = 0;
    std::uint32_t compiled = 0;
    friend bool operator==(const ElseTargets&, const ElseTargets&) = default;
};

struct Verdict {
    bool same = false;
    std::optional<ElseTargets> else_targets;   // when same, and the condition jumps out of the stretch when false
    std::string why;                           // when not the same: why (what the comparison could not decide, or the case that differs)
};

Verdict same(const Stretch& original, const Stretch& compiled);

}  // namespace cb::exact::habits::boolx
