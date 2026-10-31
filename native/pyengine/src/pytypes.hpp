// pytypes.hpp -- what the inference knows about a value (the old front end's pytypes.py).
//
// A type is immutable and compared by value (its `why` aside). Kinds:
//   bottom          nothing known yet (a fixpoint starts here)
//   top             no one C++ type fits; `why` says what clashed
//   none bool float str
//   int             with an interval [lo, hi] of Python (unbounded) integers; unset = unbounded
//   bytes bytearray `hi`: the most bytes it can hold
//   tuple           `args`: the element types, in order
//   opt             args[0] or None
//   list dict set   a container object: `site` names it (union-find in Program)
//   inst            an instance of a class of the program (`name`: its qualified name)
//   cls fn module   a class, function or module as a value
//   extern          something the front end does not map (`name` says which)
//   iter            an iterator (args[0] its items, `lo` how many)
//   variant         (new) a tagged union of named alternatives: `args` the alternatives
//   closure         (new) a nested function as a value (`name`: its qualified name)
#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "big.hpp"

namespace pyengine {

enum class TK : uint8_t {
    Bottom, Top, None, Bool, Float, Str, Int, Bytes, Bytearray, Tuple, Opt, List, Dict, Set,
    Inst, Cls, Fn, Module, Extern, Iter, Variant, Closure
};
const char* kind_name(TK k);

using OBig = std::optional<Big>;

struct T;
struct TX {
    std::vector<T> args;
    OBig lo, hi;
    std::string name;
    std::string why;
};

struct T {
    TK kind = TK::Bottom;
    int site = 0;
    std::shared_ptr<const TX> x;

    T() = default;
    explicit T(TK k) : kind(k) {}
    T(TK k, std::vector<T> args, OBig lo = {}, OBig hi = {}, std::string name = {}, int site = 0,
      std::string why = {});

    const std::vector<T>& args() const;
    const OBig& lo() const;
    const OBig& hi() const;
    const std::string& name() const;
    const std::string& why() const;

    bool is(TK k) const { return kind == k; }
    bool fits_i64() const;
    bool nonneg() const;
    bool positive() const;
    std::string repr() const;
};

bool operator==(const T& a, const T& b);
inline bool operator!=(const T& a, const T& b) { return !(a == b); }
std::string key_of(const T& t);         // a string that is equal exactly when the types are

// constants and constructors
extern const Big I64_MIN, I64_MAX, MEM, HUGE_BOUND;
T BOTTOM();
T NONE();
T BOOL();
T FLOAT();
T STR();
T BYTES();
T BYTEARRAY();
T top(const std::string& why);
T INT(OBig lo = {}, OBig hi = {});
T const_int(const Big& v);
T SIZE();
T INDEX();
T FIND();
T u(int bits);
T s(int bits);
T bytes_of(OBig maxlen, TK kind = TK::Bytes);
T opt(const T& t);
T tuple_of(std::vector<T> ts);
T strip_opt(const T& t);
T named(TK k, const std::string& name);           // inst / cls / fn / module / extern
T container(TK k, int site);                       // list / dict / set
T iter_of(const T& elem, OBig count);

// intervals
T int_join(const T& a, const T& b);
T iadd(const T& a, const T& b);
T isub(const T& a, const T& b);
T ineg(const T& a);
T imul(const T& a, const T& b);
T ifloordiv(const T& a, const T& b);
T imod(const T& a, const T& b);
T ishl(const T& a, const T& b);
T ishr(const T& a, const T& b);
T iand(const T& a, const T& b);
T ior(const T& a, const T& b);
T iinvert(const T& a);
T ipow_t(const T& a, const T& b);
T meet(const T& a, OBig lo, OBig hi);
T widen_int(const T& old, const T& nw);

std::string bstr(const OBig& b);       // str() of a bound, "None" when unbounded

}  // namespace pyengine
