// codebreaker/exact/habits/boolx.hpp -- C++ port of codebreaker.exact.habits.boolx.
//
// boolx.py - do two stretches of 8086 code decide a condition the same way?
//
// Used by newcomp.py for MASM 3.00. Its compiler leaves a condition at the first term that settles
// it; MS-Pascal 3.11 does so too, except where the terms share the record of a WITH and are grouped
// so that AND THEN / OR ELSE cannot say it: there it works every term out to 0 or 1 (mov r,0 / jcc /
// inc r), combines them with AND and OR and tests the result once. The two are the same condition if,
// whatever the comparisons in them come out as, both go on to the same place.
//
// Each stretch is run on every outcome of its comparisons: a comparison of the same two operands is
// one unknown (less, equal or greater), a Boolean byte one (0 or 1), a word masked with a constant
// one (zero or not). A stretch may load registers, compare, combine the 0/1 values and keep one in a
// temporary of the frame; it leaves by falling through its end or by a jump out of it. Anything else
// it refuses.
//
// Typing notes (the Python mixes types in a few places):
//   * an unknown's key is the Python tuple ("bool", mem) / ("bits", mem, mask) / ("cmp", x, y): a Key;
//   * an unknown's value is 0/1 for "bool" and "bits" and "lt"/"eq"/"gt" for "cmp": all int64_t here, the
//     comparison's outcomes being LT = 0, EQ = 1, GT = 2 (a Need's dom lists the values it allows);
//   * an operand's value is an int, ("mem", key), ("reg", name) or ("bits", 0/1): a Val;
//   * a result is ("end",) or ("out", target): a Result.
#pragma once

#include <map>
#include <optional>
#include <tuple>
#include <vector>

#include "py.hpp"

namespace codebreaker::exact::habits::boolx {

// class No(Exception): the stretch is refused (its message says why).
class No : public py::Exception {
public:
    explicit No(const py::str& why) : py::Exception(why.utf8()) {}
    const char* type() const override { return "No"; }
};

// The name of an unknown: ("bool", mem), ("bits", mem, mask) or ("cmp", x, y).
struct Key {
    py::str kind;
    py::str a;
    py::str b;      // ("cmp", x, y) only
    int64_t n = 0;  // ("bits", mem, mask) only
    py::str repr() const;       // repr() of the Python tuple
    py::str tail_repr() const;  // repr() of the Python tuple without its first item (k[1:])
};
bool operator<(const Key& x, const Key& y);

constexpr int64_t LT = 0, EQ = 1, GT = 2;   // the outcomes of a comparison

// class Need(Exception): the stretch asked for an unknown that the assignment does not give.
class Need : public py::Exception {
public:
    Key key;
    std::vector<int64_t> dom;
    const char* type() const override { return "Need"; }

    Need(const Key& key, const std::vector<int64_t>& dom) : py::Exception(""), key(key), dom(dom) {}
};

// A value of an operand: an int, ("mem", key), ("reg", name) or ("bits", 0/1).
struct Val {
    enum Kind { Int, Mem, Reg, Bits } kind = Int;
    int64_t i = 0;  // Int: the value; Bits: 0 or 1
    py::str s;      // Mem: the key; Reg: the register
    py::str repr() const;
};

// ('end',) or ('out', target offset)
struct Result {
    bool out = false;
    int64_t target = 0;
};

using Instruction = std::tuple<int64_t, py::str>;   // (offset, text)
using Assignment = std::map<Key, int64_t>;
using Outcomes = std::map<Assignment, Result>;

extern const py::dict<py::str, py::str> R8;
extern const py::list<py::str> R16;
extern const py::list<py::str> JCC;
extern const py::re::Pattern MEM;

// ins: [(offset, text)] of the stretch at offsets lo..hi -> ('end',) or ('out', target offset)
Result run(const py::list<Instruction>& ins, int64_t lo, int64_t hi, const Assignment& assign,
           const std::optional<py::dict<py::str, py::str>>& init = std::nullopt);

// -> {frozenset(assignment items): result}, every assignment the stretch asks for
Outcomes outcomes(const py::list<Instruction>& ins, int64_t lo, int64_t hi,
                  const std::optional<py::dict<py::str, py::str>>& init = std::nullopt);

// same()'s answer: (True, (else target of the original, else target of the compiled) or None)
// or (False, why).
struct SameResult {
    bool same = false;
    std::optional<std::tuple<int64_t, int64_t>> elses;   // (when same)
    py::str why;                                         // (when not)
};
SameResult same(const py::list<Instruction>& iw, int64_t lw, int64_t hw,
                const py::list<Instruction>& ig, int64_t lg, int64_t hg,
                const std::optional<py::dict<py::str, py::str>>& initw = std::nullopt,
                const std::optional<py::dict<py::str, py::str>>& initg = std::nullopt);

}  // namespace codebreaker::exact::habits::boolx
