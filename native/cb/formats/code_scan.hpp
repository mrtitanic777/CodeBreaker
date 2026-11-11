// cb/formats/code_scan.hpp -- finding function starts in a cartridge image by following the code.
//
// The console cartridge formats (NES, Master System, Game Boy, Genesis) can name where a program's functions
// start without running it: the reset and interrupt vectors are known, and from them the code is followed (falling
// through, branches, jumps, calls); every call target is a function start. The formats do not know any CPU's
// instruction set -- the caller hands in a Decoder that does (the decompiler's disassemblers for the 6502, Z80,
// SM83 and 68000) and says how the address space is laid out.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <set>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats {

// What an instruction does to the flow of control.
enum class Flow {
    Next,      // falls through to the next instruction
    Branch,    // may jump to `target` or fall through (a conditional branch; a software trap whose handler is `target`)
    Jump,      // continues at `target` only (or nowhere that is known)
    Call,      // calls `target`, then falls through; `target` is a function start
    Return,    // leaves the function
    Indirect,  // jumps through a register or memory cell
    Halt,      // stops (HALT, STOP, BRK)
};

struct DecodedInsn {
    std::uint32_t size = 0;               // length in bytes; the next instruction is at pc + size
    Flow flow = Flow::Next;
    std::optional<std::uint32_t> target;  // the address a branch, jump or call goes to, when it is a constant
};

// Decode the instruction at `pc` in the memory image `mem` (the image is indexed by address); nullopt when the
// bytes are not an instruction of the CPU.
using Decoder = std::function<std::optional<DecodedInsn>(ByteView mem, std::uint32_t pc)>;

// The part of the address space that holds code, and how it is laid out.
struct CodeSpace {
    std::uint32_t begin = 0;              // code lives in [begin, end)
    std::uint32_t end = 0;
    bool even_targets_only = false;       // 68000: an odd target is not code
    // Addresses in [data_begin, data_end) are data (the Game Boy cartridge header) and end a run of code.
    std::uint32_t data_begin = 0;
    std::uint32_t data_end = 0;
};

// Follow the code from `starts`; returns every function start: `starts` themselves plus each call target
// reached, in ascending order. A start outside the code space is kept in the answer but not followed.
std::vector<std::uint32_t> find_functions(ByteView mem, std::set<std::uint32_t> starts, const Decoder& decode,
                                          const CodeSpace& space);

}  // namespace cb::formats
