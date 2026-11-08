// codebreaker/build/driver.hpp -- C++17 port of codebreaker.build.driver.
//
// driver.py -- build a virtual device driver from assembly source, and check it against the original.
//
//     python -m codebreaker driver-build <source folder or .asm> [--original FILE] [--out DIR]
//
//     BuildResult r = driver::build(py::str("..\\Windows95\\vmm32\\vkd"), py::str("vkd.vxd"));
//     r.assembled, r.linked, r.path, r.checks, r.notes
//
// The source is one .asm written with the driver kit's macros (Declare_Virtual_Device,
// Begin_Service_Table, VMMcall ...). It is assembled with OpenWatcom's wasm against the kit's
// include files and linked with wlink into an LE image whose one export is the device descriptor
// block. Segments get the attributes the kit's own module-definition file gives each class of
// segment (locked code is preloaded, initialisation code is discardable, ...); whether the driver
// is loaded at start-up or on demand is taken from the original when there is one.
//
// The check is structural -- does the rebuilt file have what the original has:
//
//     objects      the same number of objects, each of the same size and attributes
//     descriptor   the same device descriptor block (name, numbers, versions, flags, order, what
//                  it points at)
//     services     the same service table (as many services, the same ones left empty, each
//                  starting with the same code)
//     exports      the same exported names and ordinals, pointing at the descriptor block
//     module       the same kind of module (start-up, on demand, old style)
//
// and, where objects pair up, how many of their bytes are equal outside the places the loader
// patches. Whether the two BEHAVE alike is verify/lanes/driver.py's question (`driver-judge`).
// Everything is written under the state folder; the source folder is only read.
//
// Typing notes:
//   * Python's result dicts are structs: structure() -> Structure, build() -> BuildResult (`path`, `original`
//     and `same` are None-able: std::optional).  compare()'s (checks, notes, same) is a tuple.
//   * `_run(cmd, cwd)` has no timeout here: the runtime's py::subprocess::run cannot give one (Python's
//     gives up after 300 s and answers (-1, "timed out after 300 s")).  The tool environment
//     (watcom.ow_env("pe")) is set in the process around the child, since the runtime's run() takes none.
//   * `link`'s `classes` is optional (None: the kit's SEGMENT_CLASSES), `definitions` optional (None: no .def).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/formats/le.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::driver {

// What the kit's module-definition file says of each class of segment, as wlink spells it.
extern const py::list<std::tuple<py::str, py::str>> SEGMENT_CLASSES;
extern const py::str _DECLARE_PATTERN;
extern const int64_t _TIMEOUT;

// The assembler or linker to use. Drivers need the builds of wasm and wlink that understand the kit's
// macros and write the driver flavour of LE; in the OpenWatcom tree those are the ones in binnt.
// Absolute path, or '' when there is none.
py::str tool(const py::str& name);

// The kit's include files in a folder whose path has no spaces (the assembler cannot be given one that
// has): the kit's own folder if it has none, else a copy under state/.
py::str kit_folder();

// The .asm to build: `path` itself, or in a folder the file named after the folder. (ValueError: none in it)
py::str source_of(const py::str& path);

// The corpus driver a source file was made from: the one with the same name, or None.
std::optional<py::str> original_of(const py::str& source);

// -> (return code, the messages, stdout and stderr, stripped)
std::tuple<int64_t, py::str> _run(const py::list<py::str>& cmd, const py::str& cwd);

// -> (object file or None, the assembler's messages)
std::tuple<std::optional<py::str>, py::str> assemble(const py::str& source, const py::str& work);

// Link one object file into a driver exporting <device>_DDB. -> (file or None, messages)
// `classes` [(segment class, attributes)] replaces the kit's classes, for source that names
// its own (one per object of the original). `named` False exports the DDB by ordinal only,
// with no name -- which is how a member of a container (VMM in VMM32.VXD) exports it, the
// name living in the container's own directory. `definitions` is the text of the driver's
// .def: its DESCRIPTION and EXPORTS (name @ordinal) are linked as it says -- the
// description and the exports go in the non-resident name table, as Microsoft's LINK put them.
std::tuple<std::optional<py::str>, py::str> link(const py::str& obj, const py::str& device, const py::str& out,
                                                 bool dynamic = false,
                                                 const std::optional<py::list<std::tuple<py::str, py::str>>>& classes = std::nullopt,
                                                 bool named = true,
                                                 const std::optional<py::str>& definitions = std::nullopt);

// ---- The structural check ----

// {object: set of offsets the loader writes into}
py::dict<int64_t, py::set<int64_t>> _patched(const formats::le::LEFile& lf);

// The first bytes of code at a place, with the bytes the loader patches left out.
py::bytes _code_at(const formats::le::LEFile& lf, const formats::le::Place& where, int64_t n = 12);

// What compare() looks at, for one image.
struct Structure {
    py::list<std::tuple<int64_t, int64_t>> objects;               // (vsize, flags)
    std::optional<formats::le::Descriptor> descriptor;
    py::list<std::optional<formats::le::Place>> services;
    py::list<std::tuple<py::str, int64_t>> exports;               // (name, ordinal)
    bool export_is_descriptor = false;
    int64_t module = 0;
    int64_t references = 0;
};
Structure structure(const formats::le::LEFile& lf);

// Check a rebuilt image against the original (two LEFile). -> (checks, notes, same):
// checks {name: True/False} for objects, descriptor, services, exports, module; notes, one
// line for each that failed; same = share of bytes equal outside patched places, over the
// objects that pair up by number and size (None when none do).
std::tuple<py::dict<py::str, bool>, py::list<py::str>, std::optional<double>> compare(const formats::le::LEFile& built,
                                                                                      const formats::le::LEFile& original);

// {"name", "source", "assembled", "linked", "path", "log", "original", "checks", "notes", "same"}
struct BuildResult {
    py::str name, source;
    bool assembled = false, linked = false;
    std::optional<py::str> path;
    py::str log;
    std::optional<py::str> original;
    py::dict<py::str, bool> checks;
    py::list<py::str> notes;
    std::optional<double> same;
};

// Assemble and link the driver whose source is `source` (a folder or an .asm). With
// `original` (a file or a corpus name; found by name when not given) the result is checked against it.
BuildResult build(const py::str& source, const std::optional<py::str>& original = std::nullopt,
                  const std::optional<py::str>& out = std::nullopt);

// One line for a build result.
py::str report(const BuildResult& r);

// python -m codebreaker driver-build <source folder or .asm> ... [--original FILE] [--out DIR]
extern const py::str MAIN_DOC;
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::build::driver
