// codebreaker/platforms/vxd.hpp -- typed C++ of codebreaker.platforms.vxd
//
// Windows 95 device drivers: LE images that run in ring 0 (VxDs), written in assembly.
//
// The six slots: the LE reader (formats/le.py; a W3 container's members, vmm32.vxd's); the 386 in
// flat ring 0; a driver's register convention (arguments and results in registers, the carry and
// zero flags an answer too); the driver emulator (emulate/driver.py, on Unicorn); the kit's wasm
// (build/driver.tool: the one that knows the DDK's macros); the driver written back as assembly and
// linked as an LE by wlink (source/driver_writer.py, build/driver.py).
//
// Typing notes:
//   * PLATFORM() is the platforms::Platform of this descriptor (platforms/package.hpp), built on first use.
//   * entries() returns the typed LE reader's Place records ((object, offset)).
#pragma once

#include <cstdint>
#include <tuple>

#include "codebreaker/formats/le.hpp"
#include "codebreaker/platforms/package.hpp"
#include "py.hpp"

namespace codebreaker::platforms::vxd {

// An LE image, or a W3 container of them (vmm32.vxd: VMM and the drivers loaded with it).
bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const py::str& path);

// {label: (object, offset)} of the first driver in the file: its control procedure, its
// services, its API procedures (formats/le.entry_points).
py::dict<py::str, formats::le::Place> entries(const py::str& path);

// {"wasm": exe, "wlink": exe}
py::dict<py::str, py::str> tools();

const Platform& PLATFORM();

}  // namespace codebreaker::platforms::vxd
