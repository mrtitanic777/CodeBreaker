// analyze/convention_facts.hpp -- what the calling-convention pass (analyze/conventions) says of each function, as far
// as the passes that follow it need.
//
// The pass works out, from a function's own return and its callers, how it is called: how many bytes of arguments it
// removes, which registers it reads before writing them (ecx: `this`), whether it builds a frame, and -- when the
// system calls it -- which kind of procedure it is (window, dialog ...). dispatch and structures read those facts; they
// do not work them out, so they take this record and not the pass.
#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "analyze/native.hpp"

namespace cb::analyze {

struct ConventionFact {
    std::string convention;                 // "stdcall", "cdecl", "thiscall", "fastcall", "register"; 16-bit: "pascal far", "cdecl near" ...
    std::optional<std::string> procedure;   // "window procedure", "dialog procedure", "hook" ...
    std::optional<int> removes;             // bytes of arguments its return removes (0: the caller does); none: no return seen
    bool frame = false;                     // builds a frame (`push ebp / mov ebp,esp`): its arguments are found from ebp
    std::vector<std::string> reg_args;      // registers it reads before writing them

    bool is_far() const { return convention.ends_with("far"); }
    bool takes_in(std::string_view reg) const {
        for (const std::string& r : reg_args) {
            if (r == reg) return true;
        }
        return false;
    }
};

// By function start; every function of the image has one.
using ConventionFacts = std::map<Va, ConventionFact>;

}  // namespace cb::analyze
