// engine/pcode_cpu_arm.hpp -- what the ARM adapter (engine/pcode_cpu_arm.cpp) hands to the own core (emulate/arm), counted: for a test or a report to say how much of what
// runs does not run on the one emulator. The run and call themselves are declared in engine/pcode_cpu.hpp.
#pragma once

#include <cstdint>
#include <map>
#include <string>

namespace cb::engine::pcode_cpu {

struct ArmHandover {
    std::map<std::string, std::uint64_t> by_class;   // the steps handed over, by the kind of instruction
    std::map<std::string, std::uint64_t> refused;    // the runs the adapter ended where the own core refuses an instruction it decodes (a 32-bit Thumb one), by mnemonic
    std::uint64_t steps = 0;                         // the steps all runs took (the handed ones included)
};

// This thread's counts since it last cleared them (the adapter adds to them in every run).
ArmHandover& arm_handover();

}  // namespace cb::engine::pcode_cpu
