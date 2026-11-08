// codebreaker/exact/emu/native_dosexec.hpp -- C++ port of codebreaker.exact.emu.native_dosexec.
//
// native_dosexec.py -- dos.run() answered by the C++ executive (native/dosexec: cbdos.dll), through ctypes.
//
// The executive (PSP, MCB arena, program load, INT 20h/21h over the MemFS or the disk, EXEC, the
// console and clock) and its DPMI host run in C++ on the same unicorn.dll Python loaded; Python
// only hands the run's arguments and a MemFS's files over and turns the result back into the same
// dos.Result (and the MemFS changed the same way).  A run with Python hooks of its own (instrument,
// instrument_when, driver), a DPMI host object given by the caller, or a file system that is not a
// dos.MemFS stays on the Python executive.
//
// In this port there is no DLL and no ctypes: the executive's sources (native/dosexec/*.cc, namespace
// cbd) are linked into the program and called directly.  What the Python did around the DLL stays:
// the MemFS's files are handed over, a run happens, the files it changed are handed back, and the
// exception the executive reports is raised as its Python class (UcError, OSError ...).
//
// The one thing that is not in the sources is WHERE unicorn.dll is (Python found it through its
// `unicorn` package): load_unicorn(path) hands it over once; failing that the environment variable
// CB_UNICORN_DLL names it.
#pragma once

#include <optional>

#include "codebreaker/exact/emu/dos.hpp"
#include "py.hpp"

namespace codebreaker::exact::emu::native_dosexec {

// CB_NATIVE_DOSEXEC when it is not set (identical on the gate: docs/NATIVE.md)
inline const char* const DEFAULT = "1";

// Where cbdos.dll is: the `dosexec_bin` setting (default native/dosexec/build/bin).  (Informational: this
// port links the executive in and loads no DLL.)
py::str dll_path();

// Hand over the path of unicorn.dll (the one Python's `unicorn` package carries).  RuntimeError if it
// cannot be loaded.
void load_unicorn(const py::str& dll);

// the reasons the executive could not be used here (_WHY)
py::list<py::str> why();

// Is the C++ executive switched on (CB_NATIVE_DOSEXEC not 0/off/no/false) and usable?
bool enabled();

// dos.run's arguments -> dos.Result, or the exception dos.run would have raised.
// fs: nullptr is the disk itself; the MemFS is handed over, and what the run changed is put back in it.
dos::Result run(const py::str& exe, const py::str& workdir, const py::str& cmdline, const py::bytes& stdin_,
                int64_t max_blocks, bool trace_calls, const py::bytes& env, std::optional<int64_t> psp,
                std::optional<int64_t> top, bool fast, const dos::MemFS* fs, bool dpmi, double fast_limit);

}  // namespace codebreaker::exact::emu::native_dosexec
