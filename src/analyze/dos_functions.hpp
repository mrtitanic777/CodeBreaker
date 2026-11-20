// analyze/dos_functions.hpp -- where the functions of a real-mode program are, and what is code at all.
//
//     const formats::dos::Program program = formats::dos::read_program(bytes, "demo.exe");
//     const dos_functions::Analysis a = dos_functions::discover(program);
//     a.functions        by linear address: name, distance (far/near), the bytes of arguments it removes, the evidence
//     a.interrupts       every `int` reached: address, number, AH and AL where a constant was loaded, its function
//     a.tables           jump tables read: address -> number of entries
//     a.census           the program's bytes by what is known about them
//     dos_functions::report("demo.exe", program, a, true, services)   the lines `codebreaker dos-functions <program> --all` prints
//
// A real-mode program has no table of its functions, and code and data share its segments, so function starts are
// established from what the program itself does, each with its evidence:
//
//   Entry      the entry point; a device driver's strategy and interrupt routines
//   Called     the target of a near or far call that is itself reached from a known function
//   Vector     a handler the program installs: int 21h AH=25h with a constant DS:DX, or a pair of stores into the
//              interrupt table at 0000:4n
//   Table      a target of `call [table + index]`; the targets of `jmp [table + index]` are followed as part of the
//              function that jumps
//   Variable   the value the file gives a pointer variable that the program calls or jumps through (`call
//              [variable]`), where that decodes cleanly
//   Pointer    the target of a far pointer in data (a relocated segment preceded by an offset) into a segment known
//              to hold code, where that decodes cleanly to a return
//   Prologue   `push bp / mov bp, sp` or `enter n, 0` in bytes nothing reaches, decoding cleanly to a return
//   Follows    what stands directly behind a return or jump of code already found (routines written in assembly lie
//              back to back and have no prologue), where that decodes cleanly to a return and does not read as text
//
// The first five follow the program's own control flow; the last three are guesses checked only for looking like code
// (`sure` is false for them and for what only they call), and they are kept apart in the census. Everything not
// reached stays "not classified": it may be data, or code reached only through a computed pointer.
//
// Whether a function is far or near, and how many bytes of arguments it removes, is read from its own return
// instructions, not from how it was found.
//
// Needs capstone.dll (cb/core/capstone_api.hpp): NotFoundError without it.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "formats/dos.hpp"
#include "symbols/dos_interrupts.hpp"

namespace cb::analyze::dos_functions {

enum class Evidence { Entry, Called, Vector, Table, Variable, Pointer, Prologue, Follows };
std::string_view name_of(Evidence evidence);   // "entry", "called", "vector", "table", "variable", "pointer", "prologue", "follows"

enum class Distance { Near, Far };
std::string_view name_of(Distance distance);   // "near", "far"

struct Function {
    std::uint32_t address = 0;         // linear
    std::string name;                  // the program's, or "sub_%05X" from its offset in the image
    Distance distance = Distance::Near;
    Evidence evidence = Evidence::Entry;
    std::uint32_t cs = 0;              // the segment base the function runs with
    bool sure = true;                  // found by following the program from its entry points
    int removes = -1;                  // bytes of arguments its return removes; -1: not established
    bool interrupt = false;            // returns with iret
    std::uint32_t size = 0;            // bytes of instructions reached from its start
};

// One `int` instruction.
struct Interrupt {
    std::uint32_t address = 0;
    std::uint8_t number = 0;
    std::optional<std::uint8_t> ah, al;                  // where a constant was loaded in the same straight run
    std::optional<std::uint32_t> function;               // the address of the function it is in, if any
};

// The bytes of the program's own spans by what is known about them.
struct Census {
    std::uint64_t code = 0;
    std::uint64_t guessed_code = 0;    // reached only from Pointer, Prologue and Follows starts
    std::uint64_t jump_tables = 0;
    std::uint64_t not_classified = 0;
    std::uint64_t total = 0;
};

// What is known of one byte of the image.
enum class ByteClass : std::uint8_t {
    Unknown = 0,       // not classified: data, or code reached only through a computed pointer
    Code = 1,          // an instruction starts here
    Inside = 2,        // inside an instruction
    Table = 3,         // part of a jump table
};

struct Analysis {
    std::map<std::uint32_t, Function> functions;         // by linear address
    std::vector<Interrupt> interrupts;                   // in address order
    std::map<std::uint32_t, std::size_t> tables;         // jump table address -> entries
    std::map<std::uint32_t, std::uint8_t> vectors;       // handler address -> the interrupt it is installed for
    Census census;
    std::vector<ByteClass> bytes;                        // by offset from the program's base: every byte of its image
    std::vector<bool> guessed;                           // by the same offset: reached only from a guessed function start
};

// Find the functions of a program. The same program always gives the same answer.
Analysis discover(const formats::dos::Program& program);

// What `codebreaker dos-functions <program> [--all]` prints for the program called `name`: its kind, how many
// functions by which evidence, the census, the interrupt calls (how many of them `services` can name) and the
// program's notes; with `all`, one line per function.
std::vector<std::string> report(std::string_view name, const formats::dos::Program& program, const Analysis& analysis, bool all,
                                const symbols::dos_interrupts::ServiceTable& services);

}  // namespace cb::analyze::dos_functions
