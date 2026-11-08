// codebreaker/java/scaffold.hpp -- C++ port of codebreaker.java.scaffold.
//
// scaffold.py -- emit SKELETON output for the non-C++ target languages (C / C# / Rust).
//
// C++ is the fully-translated target. The other languages are scaffolds: for each Java class we emit a
// single file in the target language with the type/struct shape, the fields, and the method signatures as
// unimplemented stubs -- enough to SEE how the port would be structured in that language, not a working
// translation. This is what the language picker's "coming soon" options produce so the whole flow is real.
//
// Typing notes:
//   * a Java type is the real java::ir::JType (null for None), a class the real java::ir::JClass, a
//     parameter the real java::ir::Param, a method the real java::ir::Method;
//   * the _EMIT table maps a language to an emitter, a function (class) -> (file name, content);
//   * run() returns the record {'written': [names], 'classes': n, 'language': language} as the struct Result.
#pragma once

#include <cstdint>
#include <functional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"
#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/parse.hpp"

namespace codebreaker::java::scaffold {

namespace jir = codebreaker::java::ir;

// every *.java under java_dir, sorted
py::list<py::str> _discover(const py::str& java_dir);

py::str _snake(const py::str& name);

// --- type mapping per language (skeleton fidelity, not exact) --------------
extern const py::dict<py::str, py::str> _C;
extern const py::dict<py::str, py::str> _CS;
extern const py::dict<py::str, py::str> _RS;

py::str _jtype_name(const py::ref<jir::JType>& jt);
int64_t _arr(const py::ref<jir::JType>& jt);

py::str _ctype(const py::ref<jir::JType>& jt);
py::str _cstype(const py::ref<jir::JType>& jt);
py::str _rstype(const py::ref<jir::JType>& jt);

py::str _params(const py::ref<jir::Method>& method, const std::function<py::str(const py::ref<jir::JType>&)>& render);

// Rust parameter order: `name: type`.
py::str _rparams(const py::ref<jir::Method>& method);

// --- C ---------------------------------------------------------------------
std::tuple<py::str, py::str> _emit_c(const py::ref<jir::JClass>& cls);
// --- C# --------------------------------------------------------------------
std::tuple<py::str, py::str> _emit_csharp(const py::ref<jir::JClass>& cls);
// --- Rust ------------------------------------------------------------------
std::tuple<py::str, py::str> _emit_rust(const py::ref<jir::JClass>& cls);

using Emitter = std::tuple<py::str, py::str> (*)(const py::ref<jir::JClass>&);
extern const py::dict<py::str, Emitter> _EMIT;
extern const py::dict<py::str, py::str> LANG_LABEL;

struct Result {
    py::list<py::str> written;
    int64_t classes = 0;
    py::str language;
};

// Parse the Java tree and emit one skeleton file per class in `language` (c|csharp|rust). Returns
// {'written': [names], 'classes': n, 'language': language}. ValueError for a language with no emitter.
Result run(const py::str& java_dir, const py::str& out_dir, const py::str& language);

}  // namespace codebreaker::java::scaffold
