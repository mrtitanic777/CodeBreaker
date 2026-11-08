// codebreaker/build/dos_tools.hpp -- typed C++17 port of codebreaker.build.dos_tools.
//
// dos_tools.py -- run DOS programs (assemblers, compilers, linkers of the 1980s) in DOSBox-X.
//
//     dos_tools::emulator()                                       DOSBox-X (settings: dosbox_x), or ""
//     auto [ok, made, work] = dos_tools::run({{"c", tools}}, {"masm t.asm;"}, {"T.OBJ"}, {{"T.ASM", text}});
//     auto r = dos_tools::masm(text)                              {obj, lst, log, work} from MASM
//
// `run` mounts each folder as a drive (the work folder is d: unless named; the last drive listed is the
// current one), writes the commands to BLD.BAT, runs it and reads the named files back. `ok` is False -- and
// nothing is pretended -- when the emulator is not set up or a file was not made. The emulator runs without a
// window.
//
// `masm` assembles one source with the Microsoft Macro Assembler of the folder settings "masm" names (it holds
// BIN/MASM.EXE, INC, LIB), or the one given -- build/masm runs it.
//
// Notes on the port:
//   - dict arguments that hold ordered settings are py::dict<str, str> (cpu / dos / variables: their values
//     are written with %s, so any scalar is its text); `files` values are text (written with a text-mode
//     open: "\n" becomes "\r\n") or bytes;
//   - the Python's mounts (a dict in insertion order) is py::list of (drive, folder) pairs so the order the
//     caller gave is kept and `mounts.setdefault("d", work)` is a search.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/build/masm.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::dos_tools {

using FileData = std::variant<py::str, py::bytes>;
using Mounts = py::list<std::tuple<py::str, py::str>>;   // [(drive, folder)] in order

// DOSBox-X (settings: dosbox_x), or "" (not set up, not a file, or CB_NO_DOSBOX forbids it)
py::str emulator();

// -> (every artifact was made, {name: bytes}, the work folder or None)
//
// cpu, dos: the emulator's [cpu] and [dos] settings (default cpu: cycles=20000); variables: set before the
// commands run (PATH, INCLUDE, LIB ...); work: the folder to run in (default: a new one), kept.
std::tuple<bool, py::dict<py::str, py::bytes>, std::optional<py::str>> run(
    const Mounts& mounts, const py::list<py::str>& bat_lines, const py::list<py::str>& artifacts, double timeout = 90,
    const py::dict<py::str, FileData>& files = py::dict<py::str, FileData>(),
    const std::optional<py::dict<py::str, py::str>>& cpu = std::nullopt, const std::optional<py::dict<py::str, py::str>>& dos = std::nullopt,
    const py::dict<py::str, py::str>& variables = py::dict<py::str, py::str>(), const std::optional<py::str>& work = std::nullopt);

// the folder settings "masm" names when it holds BIN/MASM.EXE, else ""
py::str masm_root();

// Assemble `source` (build/masm). -> {obj, lst, log, work}; a part the assembler did not make is missing.
masm::Assembly masm(const py::str& source, const py::str& stem = py::str("t"), double timeout = 180,
                    const std::optional<py::str>& root = std::nullopt);

}  // namespace codebreaker::build::dos_tools
