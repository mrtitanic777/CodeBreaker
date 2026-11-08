// codebreaker/translate/frontends/python/pytypes.hpp -- typed C++ from
// codebreaker.translate.frontends.python.pytypes.
//
// pytypes.py -- the Python front end's types: what the inference knows about a value.
//
// A type is immutable and compared by value. Kinds:
//
//     bottom             nothing known yet (a fixpoint starts here)
//     top                no one C++ type fits; `why` says what clashed -- the function is refused
//     none bool float str
//     int                with an interval [lo, hi] of Python (unbounded) integers; None = unbounded.
//                        The C++ is int64_t, so an int whose interval leaves int64 is refused.
//     bytes bytearray    `hi`: the most bytes it can hold (a slice d[a:a+4] holds at most 4)
//     tuple              `args`: the element types, in order
//     opt                `args[0]` or None
//     list dict set      a container object: `site` names it. Containers are mutable and aliased, so
//                        their element types are facts about the object, not the value: every place a
//                        container literal is written is a site, and sites that flow together are one
//                        object class (union-find in infer.Program). A dict site whose every use names
//                        a constant string key is a record (a struct); any other is a dict.
//     inst               an instance of a class of the program: `name` is the class's qualified name
//     cls fn module      a class, function or module as a value (calls resolve through them)
//     extern             something of a library the front end does not map: `name` says which
//
// Notes on the port:
//   * the interval bounds are Python (unbounded) ints and the module leans on that -- bounds reach 2**128,
//     their products 2**256, and ipow forms x ** y up to (2**64) ** 128 -- so a bound is a `Big`
//     (pyengine::Big, the engine's unbounded integer, native/pyengine/src/big.hpp; link big.cc), and an
//     absent bound (`None`, unbounded) is std::optional<Big>. This is the one place int64_t is not the type;
//   * `kind` is the Python's string (a py::str), compared against the kind names exactly as the Python does;
//   * `args` is the Python tuple of T: a std::vector<T> (T is immutable once built, as in Python);
//   * `name`, `why` are "str or None" (std::optional<py::str>), `site` is "int or None";
//   * Python's __eq__ / __hash__ / __repr__ are operator== / hash() / repr(); the hash value differs from
//     CPython's but equal types hash equal;
//   * module constants are accessor functions (a function-local static each, so no static-init order);
//     HUGE is HUGE_ here because <math.h> defines a HUGE macro;
//   * `ixor` is `ior` (the Python assigns the one function to both names).
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "../../../../../../pyengine/src/big.hpp"
#include "py.hpp"

namespace codebreaker::translate::frontends::python::pytypes {

using Big = pyengine::Big;
using Bound = std::optional<Big>;  // an interval bound: an int, or None (unbounded)

// -------------------------------------------------------------------------------------- constants
const Big& I64_MIN();
const Big& I64_MAX();
// No object in memory has more bytes or elements than the address space: user mode on Windows x64 is
// 2**47 bytes (128 TiB). len(), an index into a sequence, a count of loop passes over one are bounded so.
const Big& MEM();
// a bound past this is as good as none (and keeps the arithmetic small)
const Big& HUGE_();
// widening thresholds: 0, 1, 255, 0xFFFF, 0xFFFFFFFF, MEM, I64_MAX
const std::vector<Big>& THRESHOLDS();

// ------------------------------------------------------------------------------------------- T
class T {
  public:
    py::str kind;
    std::vector<T> args;
    Bound lo;
    Bound hi;
    std::optional<py::str> name;
    std::optional<int64_t> site;
    std::optional<py::str> why;

    explicit T(const py::str& kind, const std::vector<T>& args = {}, const Bound& lo = std::nullopt,
               const Bound& hi = std::nullopt, const std::optional<py::str>& name = std::nullopt,
               const std::optional<int64_t>& site = std::nullopt, const std::optional<py::str>& why = std::nullopt);

    // (kind, args, lo, hi, name, site) -- what two types are compared by (`why` is not part of it)
    std::tuple<py::str, std::vector<T>, Bound, Bound, std::optional<py::str>, std::optional<int64_t>> key() const;
    bool operator==(const T& o) const;
    bool operator!=(const T& o) const;
    int64_t hash() const;
    py::str repr() const;

    // -- questions
    bool is_int() const;
    bool fits_i64() const;
    bool nonneg() const;
    bool positive() const;
    bool has_zero() const;
};

const T& BOTTOM();
const T& NONE();
const T& BOOL();
const T& FLOAT();
const T& STR();
const T& BYTES();
const T& BYTEARRAY();

T top(const py::str& why);

T INT(const Bound& lo = std::nullopt, const Bound& hi = std::nullopt);

T const_int(const Big& v);

const T& SIZE();   // len(), a count
const T& INDEX();  // anything that indexed a sequence successfully
const T& FIND();   // str.find

T u(int64_t bits);
T s(int64_t bits);

T bytes_of(const Bound& maxlen = std::nullopt, const py::str& kind = py::str("bytes"));

T opt(const T& t);

T tuple_of(const std::vector<T>& ts);

T strip_opt(const T& t);

// ---------------------------------------------------------------------------------------- intervals
T int_join(const T& a, const T& b);
T iadd(const T& a, const T& b);
T isub(const T& a, const T& b);
T ineg(const T& a);
T imul(const T& a, const T& b);
// a // b (Python's floor), b's interval not holding 0 or the division raises.
T ifloordiv(const T& a, const T& b);
// a % b: the sign of b (Python).
T imod(const T& a, const T& b);
T ishl(const T& a, const T& b);
T ishr(const T& a, const T& b);
T iand(const T& a, const T& b);
T ior(const T& a, const T& b);
T ixor(const T& a, const T& b);
T iinvert(const T& a);
T ipow(const T& a, const T& b);
// `a` limited to [lo, hi] (a guard proved it).
T meet(const T& a, const Bound& lo = std::nullopt, const Bound& hi = std::nullopt);
// Widening with thresholds: a bound that grew moves out to the next threshold (then to unbounded).
T widen_int(const T& old, const T& new_);

}  // namespace codebreaker::translate::frontends::python::pytypes
