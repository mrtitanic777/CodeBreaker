// codebreaker/invert/early_c_order_probe.hpp -- C++17 port of codebreaker.invert.early_c_order_probe.
//
// early_c_order_probe.py - characterize MC2's emission-order scheduling (load hoist / store defer).
//
// Typing notes: stream answers the list of probe strings ("LD ax=0xb", "ax=[4]", "ST[-2]=ax", "inc word ptr [bp - 2]");
// the Python's platforms.decoder(16) is the Capstone of native/invert (cs.h).
#pragma once

#include "py.hpp"

namespace codebreaker::invert::early_c_order_probe {

py::list<py::str> stream(const py::str& src);

extern const py::list<py::str> TESTS;

// the module's `if __name__ == "__main__"` block
void main();

}  // namespace codebreaker::invert::early_c_order_probe
