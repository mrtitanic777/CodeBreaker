// codebreaker/platforms/win16.hpp -- typed C++ of codebreaker.platforms.win16
//
// 16-bit Windows: NE modules (Windows 3.x and the 16-bit half of Windows 95). Their functions
// come back as C or, where they were written in assembly, as assembly (analyze/origin.py).
//
// The six slots: the NE reader (formats/ne.py); the x86 in 16-bit protected mode; the Pascal and C
// conventions of 16-bit Windows (describe16 writes them: __stdcall16far ... ); the 16-bit emulator
// (emulate/windows16.py, the judge's runner); OpenWatcom's wcc; the module linked by
// build/module._build16 (wlink, with what is kept as assembly assembled by wasm), the judge's
// build of the functions by build/functions.build16. Its steps on the engine: engine/win16.py.
//
// Typing notes:
//   * PLATFORM() is the platforms::Platform of this descriptor (platforms/package.hpp), built on first use.
//   * segments() / entries() return the typed NE reader's Segment / Entry records (the Python's dicts).
//   * call(): the Python asks PLATFORM.emulator.run (emulate/windows16.recorded) to run the export; that
//     module has no typed port, so the runner is a parameter: run(path, ordinal, args) -> RunRecord (what the
//     Python reads of the record: `ended` and `results`).  `export` is passed as its ordinal.
//     An ImportError from the runner goes on; any other exception is a result, "error: <type>".
//   * results.get("AX") may be any value in the Python; the emulator gives ints or none, so it is
//     std::optional<int64_t> here.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/formats/ne.hpp"
#include "codebreaker/platforms/package.hpp"
#include "py.hpp"

namespace codebreaker::platforms::win16 {

// What call() reads of the emulator's record.
struct RunRecord {
    py::str ended;
    py::dict<py::str, int64_t> results;
};
using Runner = std::function<RunRecord(const py::str& path, int64_t ordinal, const py::list<int64_t>& args)>;

// The segment table: [{idx, kind, file_off, length, flags, minalloc, ...}].
py::list<formats::ne::Segment> segments(const py::str& path);

py::list<formats::ne::Entry> entries(const py::str& path);

// One export of a module called with zero arguments: (how it ended, its result).
std::tuple<py::str, std::optional<int64_t>> call(const py::str& path, int64_t ordinal, const Runner& run);

// The medium model's C library (clibm.lib), linked with the rebuilt C.
py::str library();

py::dict<py::str, py::str> tools();

// lane_of(path) == "ne"
bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const py::str& path);

const Platform& PLATFORM();

}  // namespace codebreaker::platforms::win16
