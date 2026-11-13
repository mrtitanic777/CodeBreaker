// cb/invert/msvc2_battery.hpp -- the constructs the Microsoft C/C++ 2.0 model is checked on: every construct of its class (battery), the shapes it must
// decline rather than guess (declined_battery), and the stack-frame cases (frame_battery), each with the C the compiler is given.
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "cb/invert/msvc2.hpp"
#include "cb/invert/term.hpp"

namespace cb::invert::msvc2 {

struct Case {
    std::string name;
    Term node;
};

// Every construct in the class: constants, arguments, the six operators over arguments and constants, chains, mixed precedence, derefs and indexes,
// comparisons, stores, calls (stdcall, cdecl, indirect), globals, empty bodies, unary operators, call results combined with an operand.
std::vector<Case> battery();
// Shapes `emit` must decline (division, shifts): guards against a wrong guess.
std::vector<Case> declined_battery();

struct FrameCase {
    std::string name;
    std::string source;          // the C the compiler is given
    std::vector<Local> locals;   // in declaration order
};
std::vector<FrameCase> frame_battery();

}  // namespace cb::invert::msvc2
