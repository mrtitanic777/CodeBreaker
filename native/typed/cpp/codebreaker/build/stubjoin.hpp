// codebreaker/build/stubjoin.hpp -- C++ port of codebreaker.build.stubjoin.
//
// stubjoin.py -- a Windows module put behind a DOS program of its own, as one file.
//
//     data = stubjoin::join(stub, built, at);
//
// Every Windows module starts with a DOS program, its stub; usually the linker's own two-line
// "cannot be run in DOS mode". Some carry a real one -- REGEDIT's is a registry editor for DOS,
// XCOPY32's and UNINSTAL's do their work from DOS too -- and those are built as programs of their
// own (source/dos_writer.py). join() takes the module as OpenWatcom linked it, with its own stub,
// and puts it behind that program: its new header at `at` (the original's place, when the program
// is no longer than that), and every offset the module gives from the start of the FILE moved
// with it.
//
//   PE   the headers (signature, file header, optional header, section table) at `at`, the
//        section data from the next file-alignment boundary; moved: each section's raw data,
//        relocations and line numbers, the symbol table, the debug data, the certificates;
//        SizeOfHeaders
//   NE   the header and its tables at `at`, the segments and resources from the next boundary of
//        the module's alignment unit; moved: each segment's and resource's sector, the
//        non-resident name table
//
// Errors are Python's: py::StructError (struct.error) where a field lies outside the module or a
// moved offset no longer fits its field, py::ValueError for "not a PE or NE module" and
// "resources aligned differently from segments".
// Ints are int64_t: a shift count (NE alignment) of 63 or more, which Python would carry as a big
// integer, raises py::OverflowError here (no real module has one).
#pragma once

#include "py.hpp"

namespace codebreaker::build::stubjoin {

// One little-endian field of `d` at `at`: format "<I" (default) or "<H".
int64_t _u(const py::bytes& d, int64_t at, const char* f = "<I");

int64_t _align(int64_t n, int64_t a);

// `stub`: the DOS program (an MZ file, its new-header field at 3Ch to be set); `built`: the
// Windows module as linked. -> the file.
py::bytes join(const py::bytes& stub, const py::bytes& built, std::optional<int64_t> at = std::nullopt);

py::bytes _pe(const py::bytes& b, int64_t lfa, int64_t at);

py::bytes _ne(const py::bytes& b, int64_t lfa, int64_t at);

}  // namespace codebreaker::build::stubjoin
