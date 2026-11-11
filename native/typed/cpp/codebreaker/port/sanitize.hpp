// codebreaker/port/sanitize.hpp -- typed C++ port of codebreaker.port.sanitize.
//
// sanitize.py -- what the port does wrong only when it runs: found by AddressSanitizer on the verify cases, mended.
//
// An old program may write past the end of an array where its own linker put the next variable - harmless there
// (what it overwrote it did not read again, or read as it meant to), memory of something else here. The compiler
// cannot see it; a run can. The port is built with -fsanitize=address, every verify case run, and each overflow the
// sanitizer reports becomes a rule of the port:
//
//   a global array written (or read) past its end   the array given room for what the original reached
//   a table read before its start (t[k - 0x101],    what the old linker had put there, by the program's data
//   k at the end of the input: -1)                  segment's places (SYMBOLS): that variable read instead
//
// Typing notes:
//   * `port` is pre_ansi::Port (the Python's duck-typed attributes are its members: ds, bss, reach, grown, np_rules,
//     files, extra_decls, need_headers, line_origin, line_fixes, mended, decl_rules, old_layouts_for, joins_next,
//     join_bss).  `grown` is a member this port adds to pre_ansi::Port (the Python's __init__ sets it: `self.grown = {}`).
//   * `out` is the port's {file: its text} (port.out); a report is (case name, the sanitizer's text).
//   * A rule of np_rules is NpRule {file, var, typ, kind} (kind None for the three-element form).
//   * Python's `port.np_rules = port.np_rules + [rule]` makes a new list; so does this port (the old one is untouched).
//   * glob() stands in for Python's glob.glob (not in the runtime): same patterns, same matching (fnmatch, hidden
//     names), same joined paths; the order is the directory's, as in Python (every caller sorts).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

#include "codebreaker/port/pre_ansi.hpp"

namespace codebreaker::port::sanitize {

// glob.glob(pattern)
py::list<py::str> glob(const py::str& pattern);

extern const py::re::Pattern GLOBAL;
extern const py::re::Pattern BEFORE;
extern const py::re::Pattern BEFORE_VAR;
// {C type: (size, alignment)}
extern const py::dict<py::str, std::tuple<int64_t, int64_t>> C_SIZES;
extern const py::re::Pattern INDEX;
extern const py::re::Pattern LOW;

// {member: (offset, size)} and the size of a structure of today's C (x64: pointers 8, natural alignment) - of
// its members' text (the port's: int16_t and the like); (None, None) when a member is not known
std::tuple<std::optional<py::dict<py::str, std::tuple<int64_t, int64_t>>>, std::optional<int64_t>> c_layout(const py::str& body);

// a read before a table's start: what the original read there (the variable its linker put there) - a rule
// for the line; -> why, or None
std::optional<py::str> before_start(pre_ansi::Port& port, const py::dict<py::str, py::str>& out, const py::str& case_,
                                    const py::str& text);

// where clang's sanitizer runtime is, beside the compiler (Windows: a DLL the program needs on its path)
std::optional<py::str> runtime_dir(const py::str& cc);

// a read at a small address (below 100h) on a line that reads p[i]->m: p's words small numbers at times - on
// DOS a read of the null segment there; each p[...]-> of the file read through dos_np; -> why, or None
std::optional<py::str> low_reads(pre_ansi::Port& port, const py::dict<py::str, py::str>& out, const py::str& case_,
                                 const py::str& text);

// the rules from the sanitizer's reports; -> what was mended ([(variable, why)])
py::list<std::tuple<py::str, py::str>> mend(pre_ansi::Port& port, const py::dict<py::str, py::str>& out,
                                            const py::list<std::tuple<py::str, py::str>>& reports);

}  // namespace codebreaker::port::sanitize
