// symbols/evidence.hpp -- what is known of one function that could name it: its calls, the strings it uses, who calls it, what shape it has.
//
// The facts a proposal rests on, as plain data. analyze/function_evidence.hpp reads them from a module; symbols/proposals.hpp turns them into
// proposed names (rules) or into the packet an assistant reads (JSON). A call's target is an import (named as the loader names it,
// "KERNEL32!CloseHandle"), another function of the module (by address; its name is the namer's to give) or not known (a call through a register).
#pragma once

#include <cstdint>
#include <map>
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

// Where a function's address is handed to an import, which keeps it to call back: `EnumWindows(EnumProc, 0)`, `CreateThread(..., ThreadProc,
// ...)`, a WNDCLASS's `lpfnWndProc` given to `RegisterClassA`. What the API calls that argument is what the function is.
struct HandedEvidence {
    std::string import_name;        // "USER32!EnumWindows", as CallEvidence names imports
    int argument = 0;               // which argument it is, from 1 (as the API's prototype counts them); 0: a field of a structure
    std::string field;              // a structure's field: "lpfnWndProc"
    std::uint64_t by = 0;           // the function that hands it over
    std::map<int, std::int64_t> numbers;   // the call's other arguments that are numbers, by position (`SetWindowsHookExA`'s hook, a dialog's id)
    std::map<int, std::string> texts;      // ... and those that are strings (a dialog template's name); for a structure, its class name at 0
    std::string caption;                   // a dialog procedure's: the title of the dialog template the module carries for it
    friend bool operator==(const HandedEvidence&, const HandedEvidence&) = default;
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
    // The imports its address is handed to, to be called back (32-bit modules).
    std::vector<HandedEvidence> handed_to;
    friend bool operator==(const FunctionEvidence&, const FunctionEvidence&) = default;
};

inline constexpr int kListingMost = 48;

}  // namespace cb::symbols
