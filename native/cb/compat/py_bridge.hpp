// cb/compat/py_bridge.hpp -- the one place the new tree and the old py:: layer meet (docs/CPP_DESIGN.md 6.3).
//
// While ports under native/typed/cpp/codebreaker/ still run on the imitation layer (native/ported/include/py.hpp),
// each module that is refactored into native/cb/ leaves a SHIM at its old header path. A shim has no logic: it
// converts the old types to the new ones, calls the new function, and converts back. These are the converters:
//
//     to_py(std::string_view) -> py::str            from_py(const py::str&) -> std::string (UTF-8)
//     to_py(ByteView)         -> py::bytes          from_py(const py::bytes&) -> Bytes
//     std::vector<T>          <-> py::list<T'>      OrderedMap<K,V> / std::map / std::unordered_map -> py::dict
//     std::set<T>             <-> py::set<T'>       py::dict -> OrderedMap<K',V'> (insertion order kept)
//     std::optional<T>        <-> std::optional<T'>  std::pair / std::tuple <-> std::tuple
//     integers: any integral type converts to and from int64_t (an out-of-range value is a py::OverflowError)
//
// ...and the error mapping (3.6 reversed): a cb::Error that crosses into old code becomes the matching py::
// exception, by meaning (`rethrow_as_py`, or wrap the call in `guard`).
//
// Only a shim (and tests) include this header: new code never does. It is the only file in native/cb allowed to
// name `py::` (reviewer checklist), and it is deleted with the last shim (6.5).
#pragma once

#define _SILENCE_CXX20_OLD_SHARED_PTR_ATOMIC_SUPPORT_DEPRECATION_WARNING  // the old layer uses atomic_load on shared_ptr
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/core/ordered_map.hpp"

// The imitation layer is not warning-clean at /W4; it is not ours to fix (it is deleted, not repaired).
#pragma warning(push, 0)
#include "../../ported/include/py.hpp"
#pragma warning(pop)

namespace cb::compat {

// ---- text and bytes -----------------------------------------------------------------------------------

inline py::str to_py(std::string_view s) { return py::str(std::string(s)); }
inline py::str to_py(const std::string& s) { return py::str(s); }
inline py::str to_py(const char* s) { return py::str(s); }
inline std::string from_py(const py::str& s) { return s.utf8(); }

inline py::bytes to_py(ByteView b) { return py::bytes(std::string(as_text(b))); }
inline Bytes from_py(const py::bytes& b) { return to_bytes(b.raw()); }

// ---- scalars ------------------------------------------------------------------------------------------

template <class T>
    requires std::is_integral_v<T> && (!std::is_same_v<T, bool>)
std::int64_t to_py(T v) {
    if constexpr (std::is_unsigned_v<T> && sizeof(T) >= sizeof(std::int64_t)) {
        if (v > static_cast<T>(INT64_MAX)) throw py::OverflowError("value does not fit the old layer's int64");
    }
    return static_cast<std::int64_t>(v);
}
inline bool to_py(bool v) { return v; }
inline double to_py(double v) { return v; }

// An integer read back from the old layer into a new-tree type; a value that does not fit is an error, not a wrap.
template <class T>
    requires std::is_integral_v<T> && (!std::is_same_v<T, bool>)
T int_from_py(std::int64_t v) {
    if (!std::in_range<T>(v)) throw py::OverflowError("value does not fit the new type");
    return static_cast<T>(v);
}

// ---- containers (declared first: they are mutually recursive through the element types) ----------------

template <class T> auto to_py(const std::optional<T>& v) -> std::optional<decltype(to_py(*v))>;
template <class T> auto to_py(const std::vector<T>& v);
template <class K, class V, class H, class E> auto to_py(const OrderedMap<K, V, H, E>& m);
template <class K, class V, class C, class A> auto to_py(const std::map<K, V, C, A>& m);
template <class T, class C, class A> auto to_py(const std::set<T, C, A>& s);

template <class T>
auto to_py(const std::optional<T>& v) -> std::optional<decltype(to_py(*v))> {
    if (!v) return std::nullopt;
    return to_py(*v);
}

template <class T>
auto to_py(const std::vector<T>& v) {
    using Elem = decltype(to_py(std::declval<const T&>()));
    py::list<Elem> out;
    for (const T& x : v) out.vec().push_back(to_py(x));
    return out;
}

template <class K, class V, class H, class E>
auto to_py(const OrderedMap<K, V, H, E>& m) {
    using PK = decltype(to_py(std::declval<const K&>()));
    using PV = decltype(to_py(std::declval<const V&>()));
    py::dict<PK, PV> out;
    for (const auto& [k, v] : m) out.set(to_py(k), to_py(v));   // insertion order is the dict's order
    return out;
}

template <class K, class V, class C, class A>
auto to_py(const std::map<K, V, C, A>& m) {
    using PK = decltype(to_py(std::declval<const K&>()));
    using PV = decltype(to_py(std::declval<const V&>()));
    py::dict<PK, PV> out;
    for (const auto& [k, v] : m) out.set(to_py(k), to_py(v));   // sorted order, as the map iterates
    return out;
}

template <class K, class V, class H, class E, class A>
auto to_py(const std::unordered_map<K, V, H, E, A>& m) {
    // An unordered_map has no order to preserve; the dict gets the key order of the *sorted* keys so that what the
    // old code iterates is deterministic.
    return to_py(std::map<K, V>(m.begin(), m.end()));
}

template <class T, class C, class A>
auto to_py(const std::set<T, C, A>& s) {
    using Elem = decltype(to_py(std::declval<const T&>()));
    py::set<Elem> out;
    for (const T& x : s) out.add(to_py(x));
    return out;
}

template <class A, class B>
auto to_py(const std::pair<A, B>& p) {
    return std::make_tuple(to_py(p.first), to_py(p.second));
}

template <class... Ts>
auto to_py(const std::tuple<Ts...>& t) {
    return std::apply([](const auto&... x) { return std::make_tuple(to_py(x)...); }, t);
}

// ---- back from the old layer --------------------------------------------------------------------------

inline std::int64_t from_py(std::int64_t v) { return v; }
inline bool from_py(bool v) { return v; }
inline double from_py(double v) { return v; }

template <class T>
auto from_py(const std::optional<T>& v) -> std::optional<decltype(from_py(*v))> {
    if (!v) return std::nullopt;
    return from_py(*v);
}

template <class T>
auto from_py(const py::list<T>& l) {
    using Elem = decltype(from_py(std::declval<const T&>()));
    std::vector<Elem> out;
    out.reserve(l.vec().size());
    for (const T& x : l.vec()) out.push_back(from_py(x));
    return out;
}

template <class K, class V>
auto from_py(const py::dict<K, V>& d) {
    using NK = decltype(from_py(std::declval<const K&>()));
    using NV = decltype(from_py(std::declval<const V&>()));
    OrderedMap<NK, NV> out;
    for (const auto& kv : d.items()) out.insert_or_assign(from_py(std::get<0>(kv)), from_py(std::get<1>(kv)));
    return out;
}

template <class T>
auto from_py(const py::set<T>& s) {
    using Elem = decltype(from_py(std::declval<const T&>()));
    std::set<Elem> out;
    for (const auto& x : s) out.insert(from_py(x));
    return out;
}

template <class... Ts>
auto from_py(const std::tuple<Ts...>& t) {
    return std::apply([](const auto&... x) { return std::make_tuple(from_py(x)...); }, t);
}

// ---- errors, the 3.6 table reversed -------------------------------------------------------------------

// Throw the py:: exception the old code expects for a cb::Error: by meaning, never by message.
[[noreturn]] inline void rethrow_as_py(const Error& e) {
    const std::string m = e.what();
    if (dynamic_cast<const TruncatedError*>(&e) != nullptr) throw py::StructError(m);       // struct.error: a short read
    if (dynamic_cast<const NotFoundError*>(&e) != nullptr) throw py::FileNotFoundError(m);
    if (dynamic_cast<const AccessError*>(&e) != nullptr) throw py::PermissionError(m);
    if (dynamic_cast<const TimeoutError*>(&e) != nullptr) throw py::TimeoutError(m);
    if (dynamic_cast<const IoError*>(&e) != nullptr) throw py::OSError(m);
    if (dynamic_cast<const UnsupportedError*>(&e) != nullptr) throw py::NotImplementedError(m);
    if (dynamic_cast<const FormatError*>(&e) != nullptr) throw py::ValueError(m);
    if (dynamic_cast<const ParseError*>(&e) != nullptr) throw py::ValueError(m);
    if (dynamic_cast<const UsageError*>(&e) != nullptr) throw py::ValueError(m);
    if (dynamic_cast<const ToolError*>(&e) != nullptr) throw py::RuntimeError(m);
    if (dynamic_cast<const EmulationError*>(&e) != nullptr) throw py::RuntimeError(m);
    if (dynamic_cast<const InternalError*>(&e) != nullptr) throw py::AssertionError(m);
    throw py::RuntimeError(m);
}

// Run f; a cb::Error it throws reaches the caller as the matching py:: exception.
template <class F>
decltype(auto) guard(F&& f) {
    try {
        return std::forward<F>(f)();
    } catch (const Error& e) {
        rethrow_as_py(e);
    }
}

}  // namespace cb::compat
