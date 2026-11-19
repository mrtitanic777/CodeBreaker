// cb/symbols/evidence.hpp -- what is known of one function that could name it: its calls, the strings it uses, who calls it, what shape it has.
//
// The facts a proposal rests on, as plain data. analyze/function_evidence.hpp reads them from a module; symbols/proposals.hpp turns them into
// proposed names (rules) or into the packet an assistant reads (JSON). A call's target is an import (named as the loader names it,
// "KERNEL32!CloseHandle"), another function of the module (by address; its name is the namer's to give) or not known (a call through a register).
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace cb::symbols {

struct CallEvidence {
    enum class Kind { Import, Function, Indirect };
    Kind kind = Kind::Function;
    std::string import_name;        // Import: "MODULE!name" ("MODULE!#17" for a number no table names)
    std::uint64_t target = 0;       // Function: its address, as the namer addresses functions
    int sites = 1;                  // how many places in the function make the call
    friend bool operator==(const CallEvidence&, const CallEvidence&) = default;
};

struct FunctionEvidence {
    std::uint64_t address = 0;      // as the namer addresses functions
    int instructions = 0;
    int bytes = 0;
    std::vector<CallEvidence> calls;            // each distinct target once, in the order the code first reaches it
    std::vector<std::uint64_t> callers;         // the functions that call or jump to it, ascending
    std::vector<std::string> strings;           // the text it refers to, each once, in order of use
    std::vector<std::string> listing;           // its instructions as text, as many as `kListingMost`
    // The shape of a function that only passes its arguments on to one import: its instructions are the frame's, moves of its arguments, the one
    // call or jump through the import, and the return. `pure`: it does not even touch its arguments (`jmp [import]`, or `call [import]; ret`).
    bool forwards_to_import = false;
    bool forwards_purely = false;
    // The shape of a function that only passes its arguments on to one other function of the module (its instructions are the frame's, moves of
    // its arguments, the one call or jump, and the return): where it goes.
    std::optional<std::uint64_t> forwards_to_function;
    friend bool operator==(const FunctionEvidence&, const FunctionEvidence&) = default;
};

inline constexpr int kListingMost = 48;

}  // namespace cb::symbols
