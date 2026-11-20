// exact/names.hpp -- a program's functions and data named after another version of it that has names.
//
//     codebreaker exact PROJECT names --from OTHER [--write] [-v]
//
//     names::Carried c = names::carry(*project, *other);      // {address: (name, how)}, {DS offset: (name, how)}
//
// Most of a program's functions are the same source in its neighbouring versions; their code is the same but for where things are.
// Each function's instructions are read with the places masked -- call and jump targets, the addresses of data, the words the linker
// fills -- so that code that moved is still the same code:
//
//   1. a function whose masked code is the same as one function of OTHER's (and of no other function of either) is that function: it
//      takes its name
//   2. from a pair, the calls each makes at the same instruction name each other's callees (a function whose own code changed is still
//      found, by who calls it); the same for the data words at the same places
//   3. a name OTHER has that this program already gives another function is not given twice
//
// OTHER's names: its src/SYMBOLS (C code, D data) and its "place_by" and "names" files (lines `NAME fXXXXX|g_XXXX`, or `PXXXXX|GXXXX
// NAME`). --write puts the names into this project's src/SYMBOLS (a name it has already is kept); without it they are listed.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "core/ordered_map.hpp"
#include "exact/project.hpp"

namespace cb::exact::names {

// The names a project has: its SYMBOLS (a place of no kind is code where a function begins), its "place_by" file and its "names" file.
// In the order they were met (the first name for a place is the one kept).
struct Known {
    OrderedMap<std::int64_t, std::string> code;   // code address -> name
    OrderedMap<std::int64_t, std::string> data;   // DS offset -> name
};

// `starts`: where the functions begin (none: the project's functions are asked for).
Known known(project::Project& project, const std::optional<std::set<std::int64_t>>& starts = std::nullopt);

// Where each function of the project begins, and how long it is, in address order: carve's, or -- for a toolchain with no carver --
// the places SYMBOLS names (each to the next, at most 0x400 bytes).
std::vector<std::pair<std::int64_t, std::int64_t>> functions(project::Project& project);

// One instruction with its places masked, the near call's target and the data word it reads.
struct Insn {
    std::string masked;                    // "mov ax, word ptr [N]"
    std::optional<std::int64_t> call;      // the near call's target
    std::optional<std::int64_t> data;      // the address in `[0xNNNN]`
};
// The function [a, a+length) of the image as the masked instructions.
std::vector<Insn> read(project::Project& project, std::int64_t a, std::int64_t length);

struct Named {
    std::string name, how;
    friend bool operator==(const Named&, const Named&) = default;
};

struct Carried {
    std::map<std::int64_t, Named> code;                     // this program's function -> the other's name for it, and how it was found
    std::map<std::int64_t, Named> data;                     // this program's DS offset -> the name
    OrderedMap<std::int64_t, std::int64_t> pairs;           // this program's place -> the other's (exact/seed), in the order they were found
    OrderedMap<std::int64_t, std::int64_t> data_pairs;
};

Carried carry(project::Project& project, project::Project& other);

// `exact PROJECT names --from OTHER [--write] [-v]`: 1 without --from; a relative OTHER is from the working directory.
int run(project::Project& project, const std::vector<std::string>& args);

}  // namespace cb::exact::names
