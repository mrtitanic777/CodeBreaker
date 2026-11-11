// codebreaker/platforms/xbox360.hpp -- typed C++ from codebreaker.platforms.xbox360.
//
// The Xbox 360: PowerPC code (Xenon, with VMX128) in a big-endian PE image (machine type
// 0x1F2), shipped inside a XEX2 container whose image -- the basefile -- native/xex unpacks.
//
// The six slots: the XEX2 container opened to its basefile, read as a PowerPC image
// (formats/powerpc_image.py); Xenon, 64-bit big-endian; the PowerPC calling convention (arguments
// in r3-r10, the result in r3, r14-r31 kept); CodeBreaker's own PowerPC interpreter
// (emulate/powerpc.py); the Xbox 360 SDK's cl.exe; its link.exe (build/functions.build_powerpc).
//
// Typing notes:
//   * The record types of a platform (Ref, Cpu, Loader ...) are those of win32.hpp (a platform's descriptor is the same six slots);
//     see there for how PLATFORM() holds the Python's values.  The Python's `loader.readable=_open` (a function, not a Ref) is
//     the Ref ("codebreaker.platforms.xbox360", "_open") here, named as the function is.
//   * recognises() is Platform.recognises (OSError and ValueError mean "no"); readable() is Platform.readable.
//   * Replaces nothing of the hooks listed for this batch; the platform's `xex.basefile` / `xex.is_xex` are the typed xex port's.
#pragma once

#include <optional>
#include <tuple>

#include "codebreaker/platforms/win32.hpp"
#include "py.hpp"

namespace codebreaker::platforms::xbox360 {

using win32::Compiler;
using win32::Convention;
using win32::Cpu;
using win32::Emulator;
using win32::Loader;
using win32::Packager;
using win32::Platform;
using win32::Ref;

// xex.is_xex(path) or (lane_of(path) == "pe" and pe_machine(path) == 0x01F2)
bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const py::str& path);

// xex.basefile(path) if xex.is_xex(path) else path
py::str _open(const py::str& path);

// Platform.readable(path): the loader's `readable` (_open) is given
py::str readable(const py::str& path);

py::str compiler();

py::dict<py::str, py::str> tools();

const Platform& PLATFORM();

}  // namespace codebreaker::platforms::xbox360
