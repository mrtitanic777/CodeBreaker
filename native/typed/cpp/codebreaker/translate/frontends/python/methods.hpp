// codebreaker/translate/frontends/python/methods.hpp -- typed C++ from
// codebreaker.translate.frontends.python.methods.
//
// methods.py -- the types of Python's builtins, the methods of its builtin types and the standard
// library calls the front end maps. A call not typed here is refused with its name (stdlib:<name>), so the
// coverage report lists exactly what is missing. How each one is written in C++ is a row of
// codebreaker/java/api_maps/python.json (the API map), keyed by the same name.
//
// Notes on the port:
//   * the Python's `ev` (an infer.Eval) is `Eval&`; a result `T` or `(T, res)` is a `TR`;
//   * an argument that is a lambda or generator is an empty std::optional<T> in `args` / `kw`.
#pragma once

#include "codebreaker/translate/frontends/python/infer.hpp"

namespace codebreaker::translate::frontends::python::methods {

using infer::Args;
using infer::Eval;
using infer::Kw;
using infer::State;
using infer::TR;
using program::Str;
using program::T;

// recv.attr(args) for a builtin type's value
TR call(Eval& ev, const pyfront::Call* n, const T& recv, const Str& attr, const Args& args, const Kw& kw,
        const State& state);

TR from_bytes(Eval& ev, const pyfront::Call* n, const Args& args, const Kw& kw);

TR builtin(Eval& ev, const pyfront::Call* n, const Str& name, const Args& args, const Kw& kw, const State& state);

TR std_call(Eval& ev, const pyfront::Call* n, const Str& name, const Args& args, const Kw& kw, const State& state);

}  // namespace codebreaker::translate::frontends::python::methods
