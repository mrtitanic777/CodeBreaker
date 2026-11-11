// codebreaker/platforms/gb.hpp -- C++17 port of codebreaker.platforms.gb.
//
// The Game Boy and Game Boy Color: SM83 code on a cartridge image. Their programs were written in
// assembly, which is what comes back (source/gb_writer.py, RGBDS syntax); the cartridge built from
// that source is compared with the original, and its functions are run in CodeBreaker's own SM83
// (emulate/sm83.py).
//
// PLATFORM() is the platforms::Platform that platforms::machine(...) makes (platforms/package.hpp), built on first use.
#pragma once

#include "py.hpp"
#include "codebreaker/platforms/package.hpp"

namespace codebreaker::platforms::gb {

bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const Platform& platform, const py::str& path);

// function-local static: built on first use, so no other file's start-up order matters
const Platform& PLATFORM();

}  // namespace codebreaker::platforms::gb
