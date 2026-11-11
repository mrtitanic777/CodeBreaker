// codebreaker/build/period16.hpp -- typed C++ port of codebreaker.build.period16.
//
// period16.py -- a 16-bit Windows DLL built from its source with the tools of its time.
//
//     python -m codebreaker build16 <source folder> [--original MODULE] [--work DIR] [--stub DONOR]
//
//     auto dll = period16::build(folder);                  the built DLL's path, or None
//     auto [same, total] = period16::compare(dll, original);  segments identical to the original's
//     period16::normalize(dll, donor, out);                the original's DOS stub put in front
//
// The 16-bit modules of Windows 95 were compiled with Microsoft C 6.0a and linked with the
// Windows SDK's LINK 5.60 against the Windows 3.1 DLL runtime. Microsoft C 6 here compiles but
// has no linker or libraries, so the build uses both kits, in DOSBox-X:
//
//     each module   Microsoft C 6.0a   cl /c /AM /G2 /Gsw /Ox            (the code)
//     the DLL       Visual C++ 1.52c   link @<folder's .lnk> ; rc          (the NE file, its runtime)
//
// A source folder holds the modules' .c files, the headers, the module-definition (.def) file
// and the linker's response file (.lnk) that lists the objects and names the DLL. Objects with
// no .c (the DLL entry, LIBENTRY.OBJ) are taken from the Windows 3.1 DLL kit
// (toolchains/period/libs/win31-kit) or the DDK. That kit's lean runtime is the one the
// originals were linked with; Visual C++ 1.52c's own is a third larger.
//
// settings: dosbox_x, msc6 (Microsoft C 6.0a), msvc152 (Visual C++ 1.52c: LINK, RC, LIB).
//
// The comparison is per segment, byte for byte: LINK lays out the code exactly as the original
// was laid out when the source is the source. tests/fixtures/lzexpand is a reconstruction of
// LZEXPAND.DLL made this way. Its notes said eight of twelve segments came out identical; against
// the corpus's copies today it builds eleven segments, several the original's length, none
// identical (the old tool gives the same) -- a measure of what is left, not of the build.
//
// Typing notes:
//   * `None` results are std::optional; a Python str path is py::str.
//   * compare()'s `log` is a std::function of the line (the Python's default is print; an empty function
//     is the Python's None, nothing is logged). main() prints its lines the same way.
//   * main(argv) returns the exit code the Python returns; `print(__doc__)` is DOC (the module's text above).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::build::period16 {

using Log = std::function<void(const py::str& line)>;

// The default log of compare(): print() to standard output.
void print_line(const py::str& line);

extern const py::str DOC;   // the module's docstring, which main() prints with no arguments

py::str _win(const py::str& p);

// The Windows 3.1 DLL kit: MDLLCEW.LIB, LIBW.LIB, LIBENTRY.OBJ -- or None.
std::optional<py::str> _kit();
std::optional<py::str> _libentry();
py::str _response_file(const py::str& folder);

// Compile and link the DLL a source folder describes. -> path of the built DLL, or None
// (the work folder keeps CL.TXT, LINK.TXT and RC.TXT).
std::optional<py::str> build(const py::str& folder, const std::optional<py::str>& work = std::nullopt,
                             const py::str& flags = py::str("/AM /G2 /Gsw /Ox"));

// [(length, bytes)] of an NE file's segments, in order.
py::list<std::tuple<int64_t, py::bytes>> segments(const py::str& path);

// Segment by segment. -> (identical, segments of the original)
std::tuple<int64_t, int64_t> compare(const py::str& built, const py::str& original, const Log& log = print_line);

// Put the DOS stub of `donor` (an NE or PE file, or a raw stub) in front of `built`'s NE
// header and fix every file-absolute field that moves with it (e_lfanew, the segment
// sectors, the non-resident names, the fast-load area). LINK 5.20 pads the stub to 1024
// bytes; the retail modules have a shorter one. No segment byte moves relative to the header.
py::bytes normalize(const py::str& built, const py::str& donor, const py::str& out);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::build::period16
