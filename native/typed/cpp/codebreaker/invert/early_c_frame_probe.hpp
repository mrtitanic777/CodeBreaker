// codebreaker/invert/early_c_frame_probe.hpp -- C++17 port of codebreaker.invert.early_c_frame_probe.
//
// early_c_frame_probe.py - recover MC2's frame-offset allocation rule empirically.
//
// Compiles a battery of local-variable C functions through the MC1/MC2 oracle in ONE
// dosbox run, then for each function disassembles the prologue framesize (`sub sp,N`)
// and every `[bp+disp]` reference, printing the layout so we can correlate
// source-local-usage -> assigned offset and recover the allocator rule.
//
// Typing notes: analyze answers (framesize or None, sorted distinct bp displacements); the Python's
// platforms.decoder(16, detail=True) is the Capstone of native/invert (cs.h); main prints as the Python does.
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::invert::early_c_frame_probe {

// Battery: each function probes a local-allocation pattern. K&R only, int locals.
extern const py::str BATTERY;

std::tuple<std::optional<int64_t>, py::list<int64_t>> analyze(const py::str& name, const py::bytes& code);

void main();

}  // namespace codebreaker::invert::early_c_frame_probe
