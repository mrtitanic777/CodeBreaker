// codebreaker/source/java_writer.hpp -- interface of codebreaker.source.java_writer (typed C++; implementation pending).
//
// java_writer.py -- a Java program's C++ source in a folder, every class labelled with what is known.
//
// Reached through module_writer::write_module for a module of the java kind.
//
// Typing notes:
//   * `fs` is the function store (store::functions::open()), `sha`/`digest` the module's and verifier's ids.
//   * `log` is a callback (Python's default is print: nullptr here).
#pragma once

#include <functional>
#include <memory>

#include "codebreaker/engine/native_engine.hpp"
#include "py.hpp"

namespace codebreaker::source::java_writer {

// "// %s -- transpiled from Java by CodeBreaker (codebreaker/java).\n// [%s] %s\n"
extern const py::str BANNER;

// -> the folder written (out_dir, or the state folder when it holds files written by hand)
py::str write_java(const py::str& path, const std::shared_ptr<engine::native_engine::FunctionStore>& fs,
                   const py::str& sha, const py::str& digest, const py::str& out_dir,
                   const std::function<void(const py::str&)>& log = nullptr);

}  // namespace codebreaker::source::java_writer
