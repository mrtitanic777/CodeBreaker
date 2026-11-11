// codebreaker/platforms/genesis.hpp -- C++17 port of codebreaker.platforms.genesis.
//
// The Sega Genesis / Mega Drive: 68000 code on a cartridge image (the Z80 beside it runs sound
// code the 68000 copies in). Its programs were mostly written in assembly, which is what comes back
// (source/genesis_writer.py, Motorola syntax); the cartridge built from that source is compared
// with the original, and its functions are run in CodeBreaker's own 68000 (emulate/m68k.py).
//
// PLATFORM() is the platforms::Platform that platforms::machine(...) makes (platforms/package.hpp), built on first use.
#pragma once

#include "py.hpp"
#include "codebreaker/platforms/package.hpp"

namespace codebreaker::platforms::genesis {

bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const Platform& platform, const py::str& path);

// function-local static: built on first use, so no other file's start-up order matters
const Platform& PLATFORM();

}  // namespace codebreaker::platforms::genesis
