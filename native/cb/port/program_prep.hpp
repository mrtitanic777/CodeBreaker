// cb/port/program_prep.hpp -- the first reading of a program: which of its files the port keeps, and the mending that is
// the same for every program before the passes of the port begin (the Python's Port.__init__).
//
// The program's own C library (the modules that define fopen, malloc, _main...) and its headers are left out: the C
// library of today stands in. What is kept is mended where the old bytes mean something a compiler of today would not
// see: a structure laid over a union's view, another's first bytes, a far pointer made of a segment and an offset, the
// words of the 8086's segments (far, near, pascal), and the structure passes of views.hpp.
#pragma once

#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/port/bss.hpp"
#include "cb/port/c_source.hpp"

namespace cb::port::pre_ansi {

// Why a file of the program is not the port's: it is the old C library's (a header the library has, a module that defines
// what the library defines).
std::optional<std::string> runtime_left_out(std::string_view name, std::string_view text, const Profile& profile);

struct PreparedProgram {
    Files files;                                   // the files the port works on (the old library's left out)
    Files originals;                               // the sources as they were (after the members' corrections)
    OrderedMap<std::string, std::string> left_out; // file: why it is left out
    Notes notes;                                   // for the report
    Notes union_notes;                             // for the report: the unions' and structures' mending
    std::vector<std::string> member_fixes;         // a member declared otherwise than it is used, corrected
    OrderedMap<std::string, std::size_t> far_near_dropped;   // file: how many far / near / pascal words went
    std::map<std::string, std::string> renames;    // a function whose name the C library has: renamed (the platform's own)
    bool int16 = false;                            // every int 16 bits (else only where bytes are laid out)
    Aliases aliases;                               // a name that is an element of another: where, owner's
    std::set<std::string> view_vars;               // the variables that became members of a structure variable
    std::set<std::string> view_headers;            // the headers of those structures
};

// `files`: the source tree; `profile`: the old runtime's; `members`: {"struct.member": type}, the port's corrections of
// members declared otherwise than they are used.
PreparedProgram prepare_program(const Files& files, const Profile& profile, const OrderedMap<std::string, std::string>& members = {});

}  // namespace cb::port::pre_ansi
