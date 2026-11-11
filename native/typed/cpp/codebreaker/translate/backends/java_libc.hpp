// codebreaker/translate/backends/java_libc.hpp -- typed C++ port of codebreaker.translate.backends.java_libc.
//
// java_libc.py -- a C library call as Java: which method (codebreaker/java/api_maps/libc.json, the API
// map's C rows) and how its arguments travel.
//
// crt.LibC's methods take C's parameters by their Java types (a pointer is an int address, size_t a
// long), except that a `char *` parameter is an Object: a string literal travels as a Java String, so
// `printf("%d\n", x)` stays `printf("%d\n", x)`; any other char pointer is its address. Arguments past a
// variadic prototype's last parameter go as they are (boxed), and LibC's printf reads them as the format
// says, as C's does. A row of kind 'direct' names the JDK's own method (Math.sqrt); 'unsupported' and
// missing rows are refused with the reason.
//
// Typing notes: `w` is the FnWriter (java.hpp; an ObjWriter is one); the registry is
// codebreaker/java/apimap's, loaded once (the Python's module list `_REG`).
#pragma once

#include <memory>

#include "codebreaker/java/apimap.hpp"
#include "codebreaker/translate/backends/java.hpp"

namespace codebreaker::translate::backends::java_libc {

namespace jb = codebreaker::translate::backends::java;

std::shared_ptr<codebreaker::java::apimap::Registry> registry();
// w: the FnWriter; e: the ir.Call; -> jb::J.
jb::J call(jb::FnWriter& w, const py::ref<jb::ir::Call>& e, const py::str& name, const py::list<jb::NodeP>& args);

}  // namespace codebreaker::translate::backends::java_libc
