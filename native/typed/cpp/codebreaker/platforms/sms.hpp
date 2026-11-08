// codebreaker/platforms/sms.hpp -- C++17 port of codebreaker.platforms.sms.
//
// The Sega Master System and Game Gear (and the SG-1000 before them): Z80 code on a cartridge
// image. Their programs were written in assembly, which is what comes back (source/sms_writer.py,
// WLA-DX syntax); the cartridge built from that source is compared with the original, and its
// functions are run in CodeBreaker's own Z80 (emulate/z80.py).
//
// The Python builds PLATFORM with platforms.machine(...). The platforms package has no typed port
// yet, so the description uses the plain records of the msx port (Ref, Cpu, Platform), which carry
// the same values the Python's slot objects do.
#pragma once

#include "py.hpp"
#include "codebreaker/platforms/msx.hpp"

namespace codebreaker::platforms::sms {

using msx::Cpu;
using msx::Platform;
using msx::Ref;

bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const Platform& platform, const py::str& path);

extern const Platform PLATFORM;

}  // namespace codebreaker::platforms::sms
