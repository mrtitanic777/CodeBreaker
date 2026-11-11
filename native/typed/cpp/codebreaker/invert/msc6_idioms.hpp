// codebreaker/invert/msc6_idioms.hpp -- C++17 port of codebreaker.invert.msc6_idioms.
//
// msc6_idioms.py -- the MSC 6.0a codegen idiom / flag test suite.
//
// A curated set of tiny C snippets compiled under the MSC 6.0a oracle across the flag
// matrix, recording the exact _TEXT bytes. This does three jobs:
//
//   1. DOCUMENTS undocumented codegen behavior by example (the `sub sp,2` frame under
//      /Ox, the loadds prologue, the sbb/neg boolean, pascal vs cdecl arg cleanup).
//   2. Provides the REFERENCE FORMS the per-function flag prober (detect_recipe) anchors
//      on -- which flag profile produces which prologue/frame/epilogue.
//   3. Seeds the msc6dec rule set and the divergence-pattern KB with ground-truth
//      (C construct) -> (byte sequence) mappings, per flag.
//
//     wt inverse idioms                 # print the flag-sensitivity table
//
// Typing notes:
//   * probe answers {name: {flag: hexbytes}} ("ERR" for a compile that failed); flag_sensitivity answers
//     [(name, number of distinct forms, {hex: [the last word of each flag set that gave it]})], in order.
//   * main is the module's `if __name__ == "__main__"` block; it returns the exit status.
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::invert::msc6_idioms {

// the flag axes worth sweeping (medium-model DLL code). /Gsw = loadds; /G2 = 286.
extern const py::list<py::str> FLAGS;

// (name, C source).  Each F is one function so its _TEXT is the whole snippet.
extern const py::list<std::tuple<py::str, py::str>> IDIOMS;

extern const py::bytes _PRO6;   // push ds;pop ax loadds (6.0a)
extern const py::bytes _EPI6;

// A short fingerprint of a snippet's frame/prologue/epilogue shape.
py::str _tag(const py::bytes& b);

// Compile each idiom under each flag; return {name: {flag: hexbytes}}.
py::dict<py::str, py::dict<py::str, py::str>> probe(const std::optional<py::list<py::str>>& flags = std::nullopt,
                                                    const std::optional<py::list<std::tuple<py::str, py::str>>>& idioms = std::nullopt);

// For each idiom, how many DISTINCT byte forms across the flag matrix (>1 = the
// construct is flag-sensitive -- the prober must pin the flag before reconstructing).
py::list<std::tuple<py::str, int64_t, py::dict<py::str, py::list<py::str>>>> flag_sensitivity(const py::dict<py::str, py::dict<py::str, py::str>>& res);

int64_t main();

}  // namespace codebreaker::invert::msc6_idioms
