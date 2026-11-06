// py_emit.hpp -- the helpers translated statements call (pyengine --dynamic), apart from py_object.hpp.
//
// py_object.hpp is every translated module's precompiled header: a change to it rebuilds the whole
// program. The helpers here are the part that grows (a new statement form is a new helper), so they
// are not in it: the runtime's own sources include this file, and pyengine writes into each module
// only the declarations of the helpers that module calls (by their py::name), each under its own
// guard (PYE_<name>) so modules batched into one unity file declare each once. A new helper goes
// here, so adding one rebuilds the runtime and the modules that call it, not the program.
//
// The format pyengine reads: inside namespace py, one declaration (or inline definition) per entry,
// its comment lines above it; an entry ends at the line where its braces close with ';' or '}'.
// An entry's name is the struct it declares or the name before its first '(' (templates skipped);
// an entry that names another entry (at's slot_) brings that one along.
#pragma once
#include "py_object.hpp"

namespace py {

// a comparison as Python's value (op: 0 < 1 <= 2 == 3 != 4 > 5 >=)
var compare_value_(const var& a, const var& b, int op);

// async for: type(x).__aiter__(x)
var aiter_(const var& x);
// async with: the bound __aexit__ (__aenter__ checked too)
var awith_exit_of(const var& mgr);
// async with: __aenter__() (awaited by the caller)
var awith_enter(const var& mgr);
// exit_fn(type, value, tb)'s value
var with_exit_call(const var& exit_fn, const error& e);
// match: the subject's items as a list ([before..., [starred], after...]), or None when it is not a sequence of that length
var match_sequence_(const var& subj, size_t before, size_t after, bool star);
// match: the values of the keys in order (then, with rest, a dict of the other items), or None
var match_mapping_(const var& subj, const var& keys, bool rest);
// match: the attributes a class pattern names (npos by __match_args__, then the keywords'), or None
var match_class_(const var& subj, const var& cls, size_t npos, const var& kwd_names);

// a def's annotations, as their source text: {"param", "text", ..., "return", "text"} (__annotations__)
var annotate_(const var& f, std::initializer_list<const char*> name_text);
// a def's __doc__
var doc_(const var& f, const char* doc);
// a lambda's first line
var at_line_(const var& f, int line);
// async def: a coroutine function (kind 1) or an async generator function (kind 2); fn as def_generator's
var mark_async_(const var& f, int kind);
template <class F> var def_coroutine(const char* name, const char* sig, std::vector<var> defaults, F fn,
                                     const char* qualname = nullptr, const char* module = nullptr) {
    return mark_async_(def_generator(name, sig, std::move(defaults), fn, qualname, module), 1);
}
template <class F> var def_async_generator(const char* name, const char* sig, std::vector<var> defaults, F fn,
                                           const char* qualname = nullptr, const char* module = nullptr) {
    return mark_async_(def_generator(name, sig, std::move(defaults), fn, qualname, module), 2);
}

// a module name a builtin stands in for until bound
inline var or_builtin(const var& v, const char* name) { return v.is_unbound() ? builtin(name) : v; }
// globals()
var globals_of(module_def& m);
// exec / eval of a runtime string: refused at run time
var exec_unsupported(const char* what);

// list displays with *spread, dict displays with **spread
var list_display(std::initializer_list<var> items, std::initializer_list<int> starred);
var tuple_display(std::initializer_list<var> items, std::initializer_list<int> starred);
var set_display(std::initializer_list<var> items, std::initializer_list<int> starred);
// {**m}
void dict_update_(const var& d, const var& mapping);

// f-strings: ''.join of the pieces (format(value, spec) after !r / !s / !a)
var fstr(std::initializer_list<var> parts);
// one {v!conv:spec}
var fmt(const var& v, char conv, const var& spec);

inline bool truthy_(const var& v) { return static_cast<bool>(v); }
// a name of a class body being run: py::at(ns, "x"_s) = v stores it (an unbound value deletes it)
struct slot_ {
    var d, k;
    slot_& operator=(const var& v) {
        if (v.is_unbound()) d.delitem(k);
        else d.setitem(k, v);
        return *this;
    }
};
inline slot_ at(const var& d, const var& k) { return {d, k}; }
// a class body's name: its namespace, then outside
var class_name(const var& ns, const var& key, const var& fallback);
// with: the manager's bound __exit__ (looked up before __enter__)
var with_exit_of(const var& mgr);
// with: __enter__()
var with_enter(const var& mgr);
// with: __exit__(type, value, tb) -- true: suppressed
bool with_exit_exc(const var& exit_fn, const error& e);

}  // namespace py
