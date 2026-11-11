// codebreaker/invert/early_c_check.hpp -- C++17 port of codebreaker.invert.early_c_check.
//
// early_c_check.py -- read a function of a program built with the C compiler of 1983-84, and
// put the text to that compiler.
//
//     python -m codebreaker invert --tool early-c-check IMAGE OFFSET LENGTH     (offset in hex)
//     python -m codebreaker invert --tool early-c-check IMAGE --batch LIST.json  [{name, off, size}]
//
//     auto [text, same, note] = early_c_check::reconstruct(image_bytes, offset, size);
//
// The model (early_c.py) reads the function into C; the text is compiled by the compiler itself --
// Microsoft C 2.03, MC1 and MC2 (settings: kits.msc203, the folder holding them) -- and the
// function's bytes are compared with the original's, outside the places the linker fills in: the
// fix-ups of the object file and the distance of every call or jump that leaves the function.
// `same` is None when the text was not compiled (verify=False).
//
// Typing notes:
//   * external_branches answers the Python's bytearray as py::bytes (0/1 flags).
//   * reconstruct answers (C text or None, True/False/None, what happened): std::tuple<std::optional<py::str>,
//     std::optional<bool>, py::str>.
//   * The disassembly (early_c._MD = platforms.decoder(16)) is the Capstone of native/invert (cs.h).
//   * main takes sys.argv (the program name first); it prints as the Python does, raises as it does (OSError for a
//     missing image, ValueError for a bad offset) and returns nothing.
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::invert::early_c_check {

// 1 for each byte of a call's or jump's distance whose target is outside `code`.
py::bytes external_branches(const py::bytes& code);

// The C text the model reads `code` as, or raises.
py::str source(const py::bytes& code, const py::str& name);

// -> (C text or None, True/False/None, what happened)
std::tuple<std::optional<py::str>, std::optional<bool>, py::str> reconstruct(const py::bytes& image, int64_t off, int64_t size, bool verify = true);

void main(const py::list<py::str>& argv);

}  // namespace codebreaker::invert::early_c_check
