// cb/translate/frontends/python/pytypes.hpp -- the Python front end's types: what the inference knows about a value.
//
// A type is immutable and compared by value. Kinds:
//
//     Bottom             nothing known yet (a fixpoint starts here)
//     Top                no one C++ type fits; `why` says what clashed -- the function is refused
//     None Bool Float Str
//     Int                with an interval [lo, hi] of Python (unbounded) integers; nullopt = unbounded.
//                        The C++ is int64_t, so an int whose interval leaves int64 is refused.
//     Bytes Bytearray    `hi`: the most bytes it can hold (a slice d[a:a+4] holds at most 4)
//     Tuple              `args`: the element types, in order
//     Opt                `args[0]` or None
//     List Dict Set      a container object: `site` names it. Containers are mutable and aliased, so their element
//                        types are facts about the object, not the value: every place a container literal is written
//                        is a site, and sites that flow together are one object class (union-find in the inference's
//                        Program). A dict site whose every use names a constant string key is a record (a struct);
//                        any other is a dict.
//     Inst               an instance of a class of the program: `name` is the class's qualified name
//     Cls Fn Module      a class, function or module as a value (calls resolve through them)
//     Extern             something of a library the front end does not map: `name` says which
//
// The interval bounds are `Wide` integers (wide.hpp): exact to 2**256, saturating beyond, which is far past the 2**128
// at which a bound is dropped as unbounded. The functions on intervals (add, multiply, shifts, ...) are what the
// inference uses to follow an integer through arithmetic; each gives the interval every result of the operation lies in.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/translate/frontends/python/wide.hpp"

namespace cb::translate::frontends::python::pytypes {

using Bound = std::optional<Wide>;   // an interval bound; nullopt is unbounded

enum class Kind {
    Bottom, Top, None, Bool, Float, Str, Int, Bytes, Bytearray, Tuple, Opt, List, Dict, Set, Inst, Cls, Fn, Module, Extern
};

// "bottom", "top", "int", "bytes" ...: the kind as the ledgers and refusal reasons name it.
std::string_view kind_word(Kind kind);

// ---- constants -----------------------------------------------------------------------------------------
const Wide& i64_min();
const Wide& i64_max();
// No object in memory has more bytes or elements than the address space: user mode on Windows x64 is 2**47 bytes
// (128 TiB). len(), an index into a sequence, a count of loop passes over one are bounded so.
const Wide& mem();
// A bound past this is as good as none (and keeps the arithmetic small).
const Wide& huge();

struct Type {
    Kind kind = Kind::Bottom;
    std::vector<Type> args;
    Bound lo, hi;
    std::optional<std::string> name;
    std::optional<std::uint32_t> site;
    std::optional<std::string> why;   // what clashed, for a Top; not part of what two types are compared by

    // Equal by kind, args, bounds, name and site (not `why`).
    bool operator==(const Type& other) const;
    std::size_t hash() const;

    // -- questions
    bool is_int() const { return kind == Kind::Int; }
    bool fits_i64() const;
    bool nonneg() const;
    bool positive() const;
    bool has_zero() const;
};

// The type as text: "int[0..255]" (None for an unbounded end), "bytes(<=4)", "list#3", "tuple[int[0..1], str]",
// "inst Point", "top(why)".
std::string describe(const Type& t);

const Type& bottom();
const Type& none();
const Type& boolean();
const Type& floating();
const Type& str();
const Type& bytes();
const Type& bytearray();
Type top(std::string why);

// An int in [lo, hi] (a bound beyond 2**128 in magnitude is dropped).
Type int_range(const Bound& lo = std::nullopt, const Bound& hi = std::nullopt);
// An int of exactly this value.
Type const_int(const Wide& v);
const Type& size_type();    // len(), a count
const Type& index_type();   // anything that indexed a sequence successfully
const Type& find_type();    // str.find
Type unsigned_int(unsigned bits);   // what `bits` unsigned bits hold
Type signed_int(unsigned bits);
Type bytes_of(const Bound& max_len = std::nullopt, Kind kind = Kind::Bytes);
// `t` or None (a type that is already optional, None, Top or Bottom stays as it is).
Type optional_of(const Type& t);
Type tuple_of(std::vector<Type> elements);
// The type inside an Opt, else the type itself.
Type strip_optional(const Type& t);

// ---- intervals (each: the interval the result of the operation lies in) ------------------------------
Type join_int(const Type& a, const Type& b);
Type add(const Type& a, const Type& b);
Type subtract(const Type& a, const Type& b);
Type negate(const Type& a);
Type multiply(const Type& a, const Type& b);
// a // b (floor), the interval of b not holding 0 (or the division raises).
Type floor_divide(const Type& a, const Type& b);
// a % b: the sign of b.
Type modulo(const Type& a, const Type& b);
Type shift_left(const Type& a, const Type& b);
Type shift_right(const Type& a, const Type& b);
Type bit_and(const Type& a, const Type& b);
Type bit_or(const Type& a, const Type& b);
Type bit_xor(const Type& a, const Type& b);
Type bit_invert(const Type& a);
Type power(const Type& a, const Type& b);
// `a` limited to [lo, hi] (a guard proved it).
Type meet(const Type& a, const Bound& lo = std::nullopt, const Bound& hi = std::nullopt);
// Widening with thresholds: a bound that grew moves out to the next threshold (then to unbounded).
Type widen_int(const Type& old_type, const Type& new_type);

}  // namespace cb::translate::frontends::python::pytypes

template <>
struct std::hash<cb::translate::frontends::python::pytypes::Type> {
    std::size_t operator()(const cb::translate::frontends::python::pytypes::Type& t) const { return t.hash(); }
};
