// codebreaker/java/console/platform_equivalents.hpp -- C++ port of codebreaker.java.console.platform_equivalents.
//
// platform_equivalents.py -- the authoritative Java-platform-API -> Xbox 360 XDK map.
//
// The point of this module: NEVER hand-guess a platform port again. For every Java standard-library /
// LWJGL / platform type the converter has to stub, this table names the REAL Xbox 360 XDK equivalent --
// the exact function(s) and the .lib they live in -- plus whether the port already implements it (via the
// hand-written reference layer) or still needs work.
//
// Two consumers wire off this table:
//   * the STUB EMITTER (runtime.py / stub headers)  -> every emitted stub gets a comment
//         `// [360] equivalent: <api>  (<lib>)`   or   `// [360] no equivalent -- <why>`
//     so the answer is right there in the file, not in someone's head.
//   * the COVERAGE REPORT (platform_coverage.py)    -> PLATFORM_COVERAGE.md lists the XDK equivalent and
//     STATUS for every platform symbol the game uses, up front.
//
// STATUS values:
//   'real'   the port implements it for real (reference layer / runtime) -- ships working.
//   'stub'   still a no-op/default, but the XDK equivalent below is the drop-in to implement it.
//   'dead'   genuinely no console equivalent / dead code path (applet, minecraft.net) -- stub is correct.
//
// Keyed by the emitted class/holder BASENAME (no .h). `methods` narrows the equivalent to specific calls
// when the class spans more than one XDK API. Grounded in docs/platform/reference/*, the XDK import table
// (xboxkrnl/xam/xnet/d3d9/xact3/xinput2), and the coverage audit.
#pragma once

#include "py.hpp"

namespace codebreaker::java::console::platform_equivalents {

// One entry of the map (the Python's dict with these keys; every entry in the table has xdk, lib and status).
//   xdk    : the XDK function(s) that implement this (the thing to resolve to)
//   lib    : the .lib / import module the xdk symbol lives in
//   status : 'real' | 'stub' | 'dead'
//   note   : one line of why / how (formats/gotchas)
//   methods: optional {method_name: 'xdk detail'} for per-method equivalents
struct Equiv {
    py::str xdk;
    py::str lib;
    py::str status;
    std::optional<py::str> note;
    std::optional<py::dict<py::str, py::str>> methods;
};

// subsystem -> ordered {basename: entry}
extern const py::dict<py::str, py::dict<py::str, Equiv>> EQUIV;

// Return the equivalent for an emitted class basename, or None.
std::optional<Equiv> lookup(const py::str& basename);
// Classify an equivalent so the emitted comment (and the report) can distinguish the three cases:
//    'xdk'         -> there IS an XDK function to call; resolve the stub to it.
//    'handwritten' -> NO XDK equivalent exists -> we implement it ourselves (bundled puff.c, a pure C++
//                     byte codec, a CRT call). These are the bespoke pieces that must never be lost.
//    'dead'        -> no console equivalent and a dead code path; the stub is correct as-is.
py::str kind(const Equiv& e);
// The one-line C++ comment dropped next to a stub -- THREE distinct, greppable markers so nothing is
// ever missed:
//     // [360] equivalent: <XDK api>  (<lib>)               -> resolve the stub to the named XDK call
//     // [360] NO XDK EQUIVALENT -- hand-implemented: ...    -> bespoke platform code (grep to find them ALL)
//     // [360] no equivalent -- dead on console: ...         -> genuinely dead path; stub is correct
// Returns '' if we have no mapping (emitter leaves the stub untouched).
py::str annotation(const py::str& basename);
// Flatten to (subsystem, basename, entry) for the coverage report.
py::list<std::tuple<py::str, py::str, Equiv>> all_entries();

}  // namespace codebreaker::java::console::platform_equivalents