// invert/model_tools.hpp -- the command lines of the models that are put to a compiler: a model's text compiled by the compiler it was written for, the
// bytes compared with the original's outside the places a linker fills in.
//
//     msc6            the Microsoft C 6.0a model against its compiler: a function that calls, far-pointer shapes, and real functions of LZEXPAND
//     msc6-idioms     what each switch set changes in the compiler's code for a battery of small functions
//     msvc2           the Microsoft C/C++ 2.0 model's inverse: bytes -> C -> compiler -> bytes
//     msvc2-forward   the forward model against the 1994 compiler, byte for byte (--show lists each case; --frame checks the stack-frame layout)
//     early-c-check   a function of a program built with the compiler of 1983-84, read and put to Microsoft C 2.03 (IMAGE OFFSET LENGTH, or --batch LIST.json)
//     early-c-round-trip, early-c-frame-probe, early-c-order-probe
//                     the early model's local layout round trip, and the two probes of how that compiler places locals and orders loads and stores
//
// Each says so and returns 2 where its compiler is not set up (the environment's hooks), after printing what the model read.
#pragma once

#include <span>
#include <string>

#include "invert/command.hpp"

namespace cb::invert::tools {

int msc6(const command::Environment& environment);
int msc6_idioms(const command::Environment& environment);
int msvc2(const command::Environment& environment, std::span<const std::string> args);
int msvc2_forward(const command::Environment& environment, std::span<const std::string> args);
int early_c_check(const command::Environment& environment, std::span<const std::string> args);
int early_c_round_trip(const command::Environment& environment);
int early_c_frame_probe(const command::Environment& environment);
int early_c_order_probe(const command::Environment& environment);

// `codebreaker invert <module>`: the module pass (cb/invert/module_pass.hpp) over a 16-bit module, its summary printed. `args` as the command line.
int module(const command::Environment& environment, std::span<const std::string> args);

}  // namespace cb::invert::tools
