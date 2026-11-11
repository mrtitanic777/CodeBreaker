// codebreaker/translate/frontends/python/pytypes.hpp -- COMPAT SHIM over cb/translate/frontends/python/pytypes.hpp
// (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/frontends/python/pytypes.{hpp,cpp} (the types) and wide.{hpp,cpp} (the integers of
// their intervals). This header keeps the old API for the inference, lowering and method tables that still use it: the
// record `T` with its public fields (kind as a py::str, bounds as pyengine::Big), and every function of the module over it.
// No logic of its own: a T is converted to the new Type, the new function runs, and its result is converted back.
//
// DIVERGES (see the new module's tests): `ixor` has its own bounds (it was `ior`'s: wrong for two non-negative operands);
// a bound beyond 2**256 in magnitude is saturated by the new integers (the old Big was unbounded).
//
// Not kept, nothing used them: HUGE_ and THRESHOLDS (internal constants of the module).
//
// T's equality, as before, is its fields' (kind, args, bounds, name, site; not `why`): the shim's own record compares itself
// rather than converting both sides for every comparison the inference makes.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "../../../../../../pyengine/src/big.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/translate/frontends/python/pytypes.hpp"
#include "py.hpp"

namespace codebreaker::translate::frontends::python::pytypes {

using Big = pyengine::Big;
using Bound = std::optional<Big>;  // an interval bound: an int, or None (unbounded)

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
               const std::optional<int64_t>& site = std::nullopt, const std::optional<py::str>& why = std::nullopt)
        : kind(kind), args(args), lo(lo), hi(hi), name(name), site(site), why(why) {}

    // (kind, args, lo, hi, name, site) -- what two types are compared by (`why` is not part of it)
    std::tuple<py::str, std::vector<T>, Bound, Bound, std::optional<py::str>, std::optional<int64_t>> key() const {
        return {kind, args, lo, hi, name, site};
    }
    bool operator==(const T& o) const { return key() == o.key(); }
    bool operator!=(const T& o) const { return !(*this == o); }
    int64_t hash() const;
    py::str repr() const;

    // -- questions
    bool is_int() const;
    bool fits_i64() const;
    bool nonneg() const;
    bool positive() const;
    bool has_zero() const;
};

// (the new module's names, apart from the old ones)
namespace nw = cb::translate::frontends::python;
namespace np = cb::translate::frontends::python::pytypes;

namespace detail_shim {

inline nw::Wide wide_of(const Big& b) {
    if (b.small()) return nw::Wide(b.i64());
    return *nw::Wide::from_decimal(b.str());   // (beyond int64: through the digits; a value past 2**256 saturates)
}
inline Big big_of(const nw::Wide& w) {
    if (const auto v = w.to_i64()) return Big(*v);
    return Big::from_decimal(w.to_string());
}
inline np::Bound bound_of(const Bound& b) { return b ? np::Bound(wide_of(*b)) : np::Bound(); }
inline Bound old_bound(const np::Bound& b) { return b ? Bound(big_of(*b)) : Bound(); }

inline np::Kind kind_of(const py::str& word) {
    static const np::Kind all[] = {np::Kind::Bottom, np::Kind::Top,   np::Kind::None,   np::Kind::Bool,   np::Kind::Float,
                                   np::Kind::Str,    np::Kind::Int,   np::Kind::Bytes,  np::Kind::Bytearray, np::Kind::Tuple,
                                   np::Kind::Opt,    np::Kind::List,  np::Kind::Dict,   np::Kind::Set,    np::Kind::Inst,
                                   np::Kind::Cls,    np::Kind::Fn,    np::Kind::Module, np::Kind::Extern};
    const std::string w = word.utf8();
    for (const np::Kind k : all) {
        if (np::kind_word(k) == w) return k;
    }
    throw py::ValueError("not a kind of type: " + w);
}

inline np::Type to_new(const T& t) {
    np::Type out{.kind = kind_of(t.kind), .lo = bound_of(t.lo), .hi = bound_of(t.hi)};
    for (const T& a : t.args) out.args.push_back(to_new(a));
    if (t.name) out.name = t.name->utf8();
    if (t.site) out.site = static_cast<std::uint32_t>(*t.site);
    if (t.why) out.why = t.why->utf8();
    return out;
}

inline T to_old(const np::Type& t) {
    std::vector<T> args;
    for (const np::Type& a : t.args) args.push_back(to_old(a));
    return T(py::str(std::string(np::kind_word(t.kind))), args, old_bound(t.lo), old_bound(t.hi),
             t.name ? std::optional<py::str>(py::str(*t.name)) : std::nullopt,
             t.site ? std::optional<int64_t>(static_cast<int64_t>(*t.site)) : std::nullopt,
             t.why ? std::optional<py::str>(py::str(*t.why)) : std::nullopt);
}

}  // namespace detail_shim

inline int64_t T::hash() const { return static_cast<int64_t>(detail_shim::to_new(*this).hash()); }
inline py::str T::repr() const { return cb::compat::to_py(np::describe(detail_shim::to_new(*this))); }
inline bool T::is_int() const { return detail_shim::to_new(*this).is_int(); }
inline bool T::fits_i64() const { return detail_shim::to_new(*this).fits_i64(); }
inline bool T::nonneg() const { return detail_shim::to_new(*this).nonneg(); }
inline bool T::positive() const { return detail_shim::to_new(*this).positive(); }
inline bool T::has_zero() const { return detail_shim::to_new(*this).has_zero(); }

// ---- constants
inline const Big& I64_MIN() { static const Big v = detail_shim::big_of(np::i64_min()); return v; }
inline const Big& I64_MAX() { static const Big v = detail_shim::big_of(np::i64_max()); return v; }
inline const Big& MEM() { static const Big v = detail_shim::big_of(np::mem()); return v; }

inline const T& BOTTOM() { static const T t = detail_shim::to_old(np::bottom()); return t; }
inline const T& NONE() { static const T t = detail_shim::to_old(np::none()); return t; }
inline const T& BOOL() { static const T t = detail_shim::to_old(np::boolean()); return t; }
inline const T& FLOAT() { static const T t = detail_shim::to_old(np::floating()); return t; }
inline const T& STR() { static const T t = detail_shim::to_old(np::str()); return t; }
inline const T& BYTES() { static const T t = detail_shim::to_old(np::bytes()); return t; }
inline const T& BYTEARRAY() { static const T t = detail_shim::to_old(np::bytearray()); return t; }
inline const T& SIZE() { static const T t = detail_shim::to_old(np::size_type()); return t; }     // len(), a count
inline const T& INDEX() { static const T t = detail_shim::to_old(np::index_type()); return t; }   // anything that indexed a sequence successfully
inline const T& FIND() { static const T t = detail_shim::to_old(np::find_type()); return t; }     // str.find

// ---- constructors
inline T top(const py::str& why) { return detail_shim::to_old(np::top(why.utf8())); }
inline T INT(const Bound& lo = std::nullopt, const Bound& hi = std::nullopt) {
    return detail_shim::to_old(np::int_range(detail_shim::bound_of(lo), detail_shim::bound_of(hi)));
}
inline T const_int(const Big& v) { return detail_shim::to_old(np::const_int(detail_shim::wide_of(v))); }
inline T u(int64_t bits) { return detail_shim::to_old(np::unsigned_int(static_cast<unsigned>(bits))); }
inline T s(int64_t bits) { return detail_shim::to_old(np::signed_int(static_cast<unsigned>(bits))); }
inline T bytes_of(const Bound& maxlen = std::nullopt, const py::str& kind = py::str("bytes")) {
    return detail_shim::to_old(np::bytes_of(detail_shim::bound_of(maxlen), detail_shim::kind_of(kind)));
}
inline T opt(const T& t) { return detail_shim::to_old(np::optional_of(detail_shim::to_new(t))); }
inline T tuple_of(const std::vector<T>& ts) {
    std::vector<np::Type> elements;
    for (const T& t : ts) elements.push_back(detail_shim::to_new(t));
    return detail_shim::to_old(np::tuple_of(std::move(elements)));
}
inline T strip_opt(const T& t) { return detail_shim::to_old(np::strip_optional(detail_shim::to_new(t))); }

// ---- intervals
#define CODEBREAKER_PYTYPES_SHIM_BINARY(old_name, new_name) \
    inline T old_name(const T& a, const T& b) { return detail_shim::to_old(np::new_name(detail_shim::to_new(a), detail_shim::to_new(b))); }
CODEBREAKER_PYTYPES_SHIM_BINARY(int_join, join_int)
CODEBREAKER_PYTYPES_SHIM_BINARY(iadd, add)
CODEBREAKER_PYTYPES_SHIM_BINARY(isub, subtract)
CODEBREAKER_PYTYPES_SHIM_BINARY(imul, multiply)
CODEBREAKER_PYTYPES_SHIM_BINARY(ifloordiv, floor_divide)
CODEBREAKER_PYTYPES_SHIM_BINARY(imod, modulo)
CODEBREAKER_PYTYPES_SHIM_BINARY(ishl, shift_left)
CODEBREAKER_PYTYPES_SHIM_BINARY(ishr, shift_right)
CODEBREAKER_PYTYPES_SHIM_BINARY(iand, bit_and)
CODEBREAKER_PYTYPES_SHIM_BINARY(ior, bit_or)
CODEBREAKER_PYTYPES_SHIM_BINARY(ixor, bit_xor)
CODEBREAKER_PYTYPES_SHIM_BINARY(ipow, power)
#undef CODEBREAKER_PYTYPES_SHIM_BINARY

inline T ineg(const T& a) { return detail_shim::to_old(np::negate(detail_shim::to_new(a))); }
inline T iinvert(const T& a) { return detail_shim::to_old(np::bit_invert(detail_shim::to_new(a))); }
// `a` limited to [lo, hi] (a guard proved it).
inline T meet(const T& a, const Bound& lo = std::nullopt, const Bound& hi = std::nullopt) {
    return detail_shim::to_old(np::meet(detail_shim::to_new(a), detail_shim::bound_of(lo), detail_shim::bound_of(hi)));
}
// Widening with thresholds: a bound that grew moves out to the next threshold (then to unbounded).
inline T widen_int(const T& old, const T& new_) {
    return detail_shim::to_old(np::widen_int(detail_shim::to_new(old), detail_shim::to_new(new_)));
}

}  // namespace codebreaker::translate::frontends::python::pytypes
