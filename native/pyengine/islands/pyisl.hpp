// pyisl.hpp -- the dynamic side of a typed island (pyengine --dyn-typed): py::vars checked and unboxed into
// the boundary's plain values (pyisl_val.hpp), and the typed function's result boxed again.
//
// A wrapper the engine writes for an island looks like
//
//     static py::var f(py::var n, py::var s) {
//         int64_t n_; const std::string* s_;
//         if (pyisl::on() && pyisl::int_in(n, 0, 255, n_) && (s_ = pyisl::str_arg(s))) {
//             pyisl::val r_;
//             if (pyisl::island::m::f(n_, *s_, r_)) return pyisl::box(r_);
//         }
//         return f_dyn(n, s);   // the dynamic body: any other value, or the typed body raised
//     }
//
// Every check is exact: an int parameter takes an int (not a bool, not an int subclass, not one past int64)
// within the range the typed proof assumed its callers give; a str parameter a str, not a subclass. Whatever
// fails a check, and every exception the typed body raises, goes to the dynamic body, which gives Python's
// behaviour (its own exception, message and all). PYISL_OFF=1 in the environment sends every call there
// (the same program with and without its islands: the judge's and the timings' A/B).
#pragma once
#include <cstdint>
#include <cstdlib>
#include <string>

#include "py_object.hpp"
#include "pyisl_val.hpp"

namespace pyisl {

inline bool on() {
    static const bool o = [] {
        const char* e = std::getenv("PYISL_OFF");
        return !(e && *e && *e != '0');
    }();
    return o;
}

inline py::TypeObj* t_str() { static py::TypeObj* t = py::T(py::t::str); return t; }
inline py::TypeObj* t_bytes() { static py::TypeObj* t = py::T(py::t::bytes); return t; }
inline bool exact(const py::var& x, py::TypeObj* t) { return x.is_ref() && x.ref()->type == t; }

// ------------------------------------------------------------------------------------------ arguments
inline bool int_in(const py::var& x, int64_t lo, int64_t hi, int64_t& out) {
    if (!x.is_small_int()) return false;
    out = x.small();
    return out >= lo && out <= hi;
}
inline bool boolean(const py::var& x, bool& out) {
    if (!x.is_bool()) return false;
    out = x.small() != 0;
    return true;
}
inline bool real(const py::var& x, double& out) {
    if (!x.is_float()) return false;
    out = x.flt();
    return true;
}
inline const std::string* str_arg(const py::var& x) { return exact(x, t_str()) ? &py::str_utf8(x) : nullptr; }
inline const std::string* bytes_arg(const py::var& x) { return exact(x, t_bytes()) ? &py::bytes_raw(x) : nullptr; }

// a value by its type's spec: s str, b bytes, B bool, f float, ?X X or None, (XY...) a tuple of exactly those
inline void skip_(const char*& spec) {   // past one spec
    char c = *spec++;
    if (c == '?') skip_(spec);
    else if (c == '(') {
        while (*spec && *spec != ')') skip_(spec);
        if (*spec) spec++;
    }
}
inline bool value_(const py::var& x, const char*& spec, val& out) {
    char c = *spec++;
    switch (c) {
    case 's': if (auto* p = str_arg(x)) { out.k = val::K::Str; out.s = *p; return true; } return false;
    case 'b': if (auto* p = bytes_arg(x)) { out.k = val::K::Bytes; out.s = *p; return true; } return false;
    case 'B': { bool b; if (!boolean(x, b)) return false; out.k = val::K::Bool; out.i = b; return true; }
    case 'f': { double d; if (!real(x, d)) return false; out.k = val::K::Float; out.f = d; return true; }
    case '?':
        if (x.is_none()) {
            out.k = val::K::None;
            skip_(spec);
            return true;
        }
        return value_(x, spec, out);
    case '(': {
        if (!x.is_ref() || x.ref()->type != py::T(py::t::tuple)) return false;
        const auto& items = py::tuple_items(x);
        out.k = val::K::Tuple;
        out.items.clear();
        size_t n = 0;
        while (*spec != ')') {
            if (n >= items.size()) return false;
            out.items.emplace_back();
            if (!value_(items[n++], spec, out.items.back())) return false;
        }
        spec++;
        return n == items.size();
    }
    default: return false;
    }
}
inline bool value(const py::var& x, const char* spec, val& out) { return value_(x, spec, out); }

// ------------------------------------------------------------------------------------------ native modules
// (a native module has no Python body: a call it cannot take is Python's TypeError, a typed body's exception
// is raised as Python's of the same name -- struct.error and re.error by their modules)
[[noreturn]] inline void wrong_args(const char* fn, size_t n) {
    py::raise(py::T(py::t::TypeError), std::string(fn) + " takes " + std::to_string(n) + " positional arguments");
}
[[noreturn]] inline void wrong_type(const char* fn, const char* param, const py::var& x) {
    py::raise(py::T(py::t::TypeError), std::string(fn) + ": argument " + param + " of a type its typed C++ does not take: " +
                                           py::str_utf8(py::repr(x)));
}
[[noreturn]] inline void raise_named(const std::string& type, const std::string& msg) {
    py::var cls;
    if (type == "StructError") cls = py::import_from(py::import("struct"), "error");
    else if (type == "PatternError") cls = py::import_from(py::import("re"), "error");
    else {
        py::var b = py::import("builtins");
        try {
            cls = py::import_from(b, type.c_str());
        } catch (...) {
            cls = py::import_from(b, "RuntimeError");
        }
    }
    py::raise(py::T(cls), msg);
}

// ------------------------------------------------------------------------------------------ results
inline py::var box(const val& v) {
    switch (v.k) {
    case val::K::None: return py::None;
    case val::K::Bool: return py::var(v.i != 0);
    case val::K::Int: return py::var(static_cast<long long>(v.i));
    case val::K::Float: return py::var(v.f);
    case val::K::Str: return py::str_from(v.s);
    case val::K::Bytes: return py::bytes_from(v.s);
    case val::K::ByteArray: return py::t::bytearray(py::bytes_from(v.s));
    case val::K::Tuple: case val::K::List: {
        std::vector<py::var> items;
        items.reserve(v.items.size());
        for (const auto& x : v.items) items.push_back(box(x));
        return v.k == val::K::Tuple ? py::tuple_from(std::move(items)) : py::list_from(std::move(items));
    }
    case val::K::Dict: {
        py::var d = py::dict();
        for (size_t i = 0; i + 1 < v.items.size(); i += 2) d.setitem(box(v.items[i]), box(v.items[i + 1]));
        return d;
    }
    }
    return py::None;
}

}  // namespace pyisl
