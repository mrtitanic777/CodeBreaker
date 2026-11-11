// codebreaker/platforms/dos.hpp -- typed C++ of codebreaker.platforms.dos
//
// Real-mode DOS programs: MZ executables, and .COM files and device drivers, which have no
// header (formats/dos.py tells them apart). Their source is assembly, rebuilt and compared.
//
// The six slots: the DOS reader (formats/dos.py: the load module, its relocations, a packed one
// unpacked); the 8086 in real mode; the near and far C conventions the decompiler writes; no
// emulator -- nothing is run: a program that comes back byte for byte IS the original; OpenWatcom's
// wasm; the program written back as assembly and linked by wlink (source/dos_writer.py).
//
// Typing notes:
//   * PLATFORM() is the platforms::Platform of this descriptor (platforms/package.hpp), built on first use.
//   * The Python's empty Emulator() slot has no fields: the record gives none.
#pragma once

#include <cstdint>
#include <tuple>

#include "codebreaker/platforms/package.hpp"
#include "py.hpp"

namespace codebreaker::platforms::dos {

// detect.kind(path) == "dos"   (.COM files and plain driver images too, which have no header)
bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const py::str& path);

// {"wasm": exe, "wlink": exe}
py::dict<py::str, py::str> tools();

const Platform& PLATFORM();

}  // namespace codebreaker::platforms::dos
