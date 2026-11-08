// codebreaker/platforms/genesis.hpp -- C++17 port of codebreaker.platforms.genesis.
//
// The Sega Genesis / Mega Drive: 68000 code on a cartridge image (the Z80 beside it runs sound
// code the 68000 copies in). Its programs were mostly written in assembly, which is what comes back
// (source/genesis_writer.py, Motorola syntax); the cartridge built from that source is compared
// with the original, and its functions are run in CodeBreaker's own 68000 (emulate/m68k.py).
//
// The Python builds PLATFORM with platforms.machine(...). The platforms package has no typed port
// of its own, so the description uses the plain records of the msx port (Ref, Cpu, Platform), which
// carry the same values the Python's slot objects do (as the gb and sms ports do).
#pragma once

#include "py.hpp"
#include "codebreaker/platforms/msx.hpp"

namespace codebreaker::platforms::genesis {

using msx::Cpu;
using msx::Platform;
using msx::Ref;

bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const Platform& platform, const py::str& path);

// function-local static: built on first use, so no other file's start-up order matters
const Platform& PLATFORM();

}  // namespace codebreaker::platforms::genesis
