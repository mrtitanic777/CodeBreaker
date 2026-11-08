// codebreaker/java/debug.hpp -- typed C++ port of codebreaker.java.debug.
//
// debug.py -- emit the ONE debug file for the converted game.
//
// The converter routes every piece of debug instrumentation through a single header (`Debug.h`) with a
// single master switch. When `DEBUG_ENABLED` is 0 (retail), every debug macro compiles to nothing, so
// ALL debug code anywhere in the codebase -- as long as it goes through these macros -- is disabled by
// that one toggle. No scattered `#ifdef`s, no debug path left hot in a release build.
//
// Design constraints the user set:
//   * concise, not spam -- thousands of log lines is the wrong answer. Every channel is rate-limited:
//     DBG_ONCE (once per call site), DBG_EVERY(n) (every Nth hit), DBG_THROTTLE(ms) (at most once per
//     interval). Leaks and packet loss are ACCUMULATED and reported as a periodic/at-exit SUMMARY, not
//     a line per event.
//   * categorized so you can find one class of problem fast: BUG (invariants/asserts), LEAK (alloc
//     tracking), CRASH soft (recovered) + hard (fatal, with context), NET (connect/latency/packet loss).
//   * one file, toggleable at all times; toggling it off disables everything downstream.
//
// Output sink is OutputDebugStringA on the 360 (visible in the XDK / PIX / a debugger), plain stderr on
// PC builds -- selected by the platform macro so the same code runs in both.
//
// Notes on the port:
//   * the three files are written in Python's text mode (encoding utf-8, newline '\n' becoming '\r\n' on
//     Windows), as the Python's open(..., 'w', encoding='utf-8') does;
//   * _DEBUG_MARKER is an accessor (the compiled pattern lives in a function-local static, not in a
//     namespace-scope object, so it does not depend on the order translation units start in).
#pragma once

#include <cstdint>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::debug {

extern const py::str DEBUG_H;
extern const py::str DEBUG_CPP;
extern const py::str DEBUG_README;

// Emit Debug.h + Debug.cpp + DEBUG.md into out_dir. `enabled_default` sets the initial
// DEBUG_ENABLED value baked into Debug.h (still overridable per build config). Returns paths.
py::list<py::str> write_debug(const py::str& out_dir, bool enabled_default = true);

// re.compile(r'J2X-DEBUG\[([a-z0-9-]+)\]')
const py::re::Pattern& _DEBUG_MARKER();

// Return {tag: [(file, line)]} for every J2X-DEBUG marker across the tree -- the full,
// greppable catalogue of debug facilities and (once instrumented) call sites.
py::dict<py::str, py::list<std::tuple<py::str, int64_t>>> scan_debug_markers(const py::str& out_dir);

}  // namespace codebreaker::java::debug
