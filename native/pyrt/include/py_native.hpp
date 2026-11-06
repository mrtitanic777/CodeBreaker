// py_native.hpp -- a C++ module of a translated program (pyengine --dynamic --dyn-native DIR).
//
// A program's own C++ (its native folder, compiled into the executable beside the translated modules)
// registers modules the translated code imports by name: at static initialisation,
//
//     static const bool registered = (py::register_native_module("codebreaker_native", init), true);
//
// and init fills the module with mod_def / mod_set when it is first imported.
#pragma once
#include "py_object.hpp"
#include "py_emit.hpp"

namespace py {
using native_module_init = void (*)(const var& module);
void register_native_module(const char* name, native_module_init init);
void mod_set(const var& module, const char* name, const var& v);
void mod_def(const var& module, const char* name, native_fn fn);
}  // namespace py
