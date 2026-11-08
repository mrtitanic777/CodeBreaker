// codebreaker/java/twophase.hpp -- C++ port of codebreaker.java.twophase.
//
// twophase.py -- two-phase construction for classes whose constructor makes a virtual call.
//
// Java dispatches a virtual call made inside a constructor to the most-derived override, even though the
// subclass's own constructor has not run yet. C++ does not: while a base constructor runs, the object IS a
// base, so the call reaches the base's version. Minecraft relies on the Java rule in a few hierarchies --
// Block's constructor reads isOpaqueCube() into opaqueCubeLookup (every transparent block came out opaque:
// wrong light and culling), Entity's calls entityInit(), World's createChunkProvider() -- so a plain C++
// constructor gets them wrong.
//
// For those classes (the set `compute` finds) every Java constructor becomes a method
// `C* C::j2xInit(params)`; the C++ constructor `C::C()` only zeroes the fields. j2xInit calls the parent's
// j2xInit first (Java's super(...)), then runs the field initialisers and the body, and returns `this`. It
// runs on a fully built C++ object, so its virtual calls reach the override as in Java. Creating an object
// is then `(new C())->j2xInit(args)` -- `rewrite_news` turns every `new C(args)` in the emitted tree (and in
// the probe's harness) into that.
//
// Notes on the port: `compute` reads the classes codebreaker.java.parse produces (ir::JClass, held as py::ref as
// in the typed port of codebreaker.java.ir). The Python reads `cls.constructors()` and `cls.instance_fields()`;
// they are the JClass methods of the same names.
#pragma once

#include "py.hpp"
#include "py_os.hpp"
#include "codebreaker/java/ir.hpp"

namespace codebreaker::java::twophase {

// One statement of a body: (line number, text).
using Statement = std::tuple<int64_t, py::str>;
using Body = py::list<Statement>;

// A parsed class (codebreaker.java.parse's result).
using ClassInfo = py::ref<ir::JClass>;

// The record run() returns ({'sites': int, 'files': [str]}).
struct RunResult {
    int64_t sites = 0;
    py::list<py::str> files;
};

extern const py::re::Pattern _CALL;
extern const py::set<py::str> _KEYWORDS;
extern const py::re::Pattern _SUPER_ARGS;

py::str _ctor_text(const ClassInfo& cls);

// The classes to build in two phases: each class whose constructor (or instance initialiser) calls one
// of its own instance methods that a subclass overrides (or that is abstract), and every descendant of
// such a class. Left out (with the subtree below it): a value type, and a root whose parent is another
// user class that it constructs with arguments (the parent's constructor would have to run in `C()`).
// `values` is the set of value-type names.
py::set<py::str> compute(const py::list<ClassInfo>& classes, const py::set<py::str>& values = py::set<py::str>());

// Index just past the parenthesis that closes the one opened before `i`, or -1.
int64_t _close(const py::str& text, int64_t i);

// `new C(args)` -> `(new C())->j2xInit(args)` for C in names. -> (text, how many). Leaves alone a
// `new C()` already in that form and `new C[n]` (an array).
std::tuple<py::str, int64_t> rewrite_news(const py::str& text, const py::set<py::str>& names);

// A stack object `C tmp(args);` / `C tmp;` (the leak-temporary rewrite makes these) ->
// `C tmp; tmp.j2xInit(args);`. -> (text, how many)
std::tuple<py::str, int64_t> rewrite_stack(const py::str& text, const py::set<py::str>& names);

// The post-pass over the emitted tree: every `new C(...)` of a two-phase class.
RunResult run(const py::str& out_dir, const py::set<py::str>& names);

}  // namespace codebreaker::java::twophase
