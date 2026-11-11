// cb/port/complaints.hpp -- what the compilers say, by the kind of complaint.
//
// The port's compiler loop builds the rewritten program with a compiler of today and mends what it complains of.
// The complaints come in the words of three compilers (gcc, clang, MSVC); `said` names the kind of one and gives
// the parts of it the mending needs (the function, the argument's number, the type it was and the type it should be).
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/regex.hpp"

namespace cb::port::pre_ansi {

// The kinds of complaint, in the order `said` tries them.
enum class Complaint {
    LibraryName,           // conflicting types for a name of the C library
    ArgumentType,          // an argument of the wrong kind of pointer or integer
    FunctionPointer,
    IntegerFromPointer,    // a pointer stored in an integer
    PointerAssign,
    ReturnsPointerAsInt,
    ReturnsIntAsPointer,
    ReturnsOtherPointer,
    PointerFromInteger,
    NegativeCompare,       // a comparison that limited range makes always true or false
    NoMember,              // a structure without the member read from it
    VoidValue,
    PointerCastSize,
    IncompleteType,
    UnionAssigned,
    UnionArgument,
    Unsequenced,
    IntegerCastSize,
    Arity,
};

struct Said {
    Complaint kind;
    RegexMatch match;   // views into the text given to `said`: it must outlive this
    // A named part of the complaint: `fn`, `n`, `how`, `frm`, `to`, `m`, `t`, `v`.
    std::optional<std::string_view> part(std::string_view name) const { return match.named(name); }
};

// Each kind with the patterns that say it (the compilers' words, in the order they are tried).
struct ComplaintPatterns {
    Complaint kind;
    std::vector<std::string_view> patterns;
};
const std::vector<ComplaintPatterns>& complaint_table();

// The kind of complaint `text` is, with its parts; empty when it is none of them.
std::optional<Said> said(std::string_view text);

// A cast as a compiler wrote it, as a regex: `(struct s *)`, `(char)`, `(int (*)(char *))`.
std::string_view cast_pattern();

}  // namespace cb::port::pre_ansi
