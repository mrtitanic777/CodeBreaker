// pyisl_typed.hpp -- the typed side of a typed island (pyengine --dyn-typed): py.hpp's values to and from
// the boundary's plain values (pyisl_val.hpp).
//
// Compiled only in the typed translation units, with py renamed (/Dpy=pyt: the dynamic runtime has its
// own namespace py), so `py::str` here is the typed path's str. The generated islands.cpp calls each typed
// function through these: arguments in by get<T>, the result out by put.
#pragma once
#include <process.h>

#include <cstdio>
#include <cstdlib>
#include <optional>
#include <tuple>
#include <utility>
#include <variant>

#include "py.hpp"
#include "pyisl_val.hpp"

namespace pyisl {

// ------------------------------------------------------------------------------------------ results
inline void put(val& v, bool x) { v.k = val::K::Bool; v.i = x ? 1 : 0; }
inline void put(val& v, int64_t x) { v.k = val::K::Int; v.i = x; }
inline void put(val& v, double x) { v.k = val::K::Float; v.f = x; }
inline void put(val& v, py::none_t) { v.k = val::K::None; }
inline void put(val& v, const py::str& x) { v.k = val::K::Str; v.s = x.utf8(); }
inline void put(val& v, const py::bytes& x) { v.k = val::K::Bytes; v.s = x.raw(); }
inline void put(val& v, const py::bytearray& x) { v.k = val::K::ByteArray; v.s = x.raw(); }
template <class T> void put(val& v, const std::optional<T>& x) {
    if (x) put(v, *x);
    else v.k = val::K::None;
}
template <class... A> void put(val& v, const std::tuple<A...>& x) {
    v.k = val::K::Tuple;
    v.items.resize(sizeof...(A));
    size_t i = 0;
    std::apply([&](const auto&... e) { (put(v.items[i++], e), ...); }, x);
}
template <class... A> void put(val& v, const std::variant<A...>& x) {
    std::visit([&](const auto& e) { put(v, e); }, x);
}
template <class T> void put(val& v, const py::list<T>& x) {
    v.k = val::K::List;
    v.items.resize(static_cast<size_t>(x.size()));
    size_t i = 0;
    for (const auto& e : x.vec()) put(v.items[i++], static_cast<const T&>(e));
}
template <class K, class V> void put(val& v, const py::dict<K, V>& x) {
    v.k = val::K::Dict;
    for (const auto& kv : x.items()) {
        v.items.emplace_back();
        put(v.items.back(), std::get<0>(kv));
        v.items.emplace_back();
        put(v.items.back(), std::get<1>(kv));
    }
}

// ------------------------------------------------------------------------------------------ arguments
// (the dynamic side checked each value's kind against the parameter's type before the call)
template <class T> struct get_;
template <> struct get_<bool> { static bool of(const val& v) { return v.i != 0; } };
template <> struct get_<int64_t> { static int64_t of(const val& v) { return v.i; } };
template <> struct get_<double> { static double of(const val& v) { return v.f; } };
template <> struct get_<py::str> { static py::str of(const val& v) { return py::str(v.s); } };
template <> struct get_<py::bytes> { static py::bytes of(const val& v) { return py::bytes(v.s.data(), v.s.size()); } };
template <class T> struct get_<std::optional<T>> {
    static std::optional<T> of(const val& v) {
        if (v.k == val::K::None) return std::nullopt;
        return get_<T>::of(v);
    }
};
template <class... A> struct get_<std::tuple<A...>> {
    template <size_t... I> static std::tuple<A...> at(const val& v, std::index_sequence<I...>) { return std::tuple<A...>(get_<A>::of(v.items[I])...); }
    static std::tuple<A...> of(const val& v) { return at(v, std::index_sequence_for<A...>()); }
};
template <class T> T get(const val& v) { return get_<T>::of(v); }

// ------------------------------------------------------------------------------------------ counts
// PYISL_STATS=<file> (%p: the process id): at exit, per island, the calls its typed body answered and the
// calls it raised in (those ran the dynamic body). Calls whose arguments did not fit never reach this side.
struct counter {
    const char* name;
    uint64_t typed = 0, raised = 0;
    counter* next;
    explicit counter(const char* n);
};
inline counter*& counters_() {
    static counter* head = nullptr;
    return head;
}
inline void report_() {
    const char* p = std::getenv("PYISL_STATS");
    if (!p || !*p) return;
    std::string path = p;
    if (size_t k = path.find("%p"); k != std::string::npos) path.replace(k, 2, std::to_string(_getpid()));
    FILE* f = std::fopen(path.c_str(), "wb");
    if (!f) return;
    for (counter* c = counters_(); c; c = c->next)
        if (c->typed || c->raised) std::fprintf(f, "%s %llu %llu\n", c->name, (unsigned long long)c->typed, (unsigned long long)c->raised);
    std::fclose(f);
}
inline counter::counter(const char* n) : name(n), next(counters_()) {
    counters_() = this;
    static const bool once = (std::atexit(report_), true);
    (void)once;
}

}  // namespace pyisl
