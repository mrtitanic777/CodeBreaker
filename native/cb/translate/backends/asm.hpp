// cb/translate/backends/asm.hpp -- the L2 back end whose output is assembly language.
//
// A Module whose parts stay machine code is written as the file its assembler reads. This is the back end of every
// lane whose output is assembly: the whole-file NASM writers (NE, PE, LE), the MASM writers (drivers, DOS), the
// console and home-computer writers, the 16-bit port's assembly, and the head of a driver's source file. Each of
// those works out what to write -- which bytes are code, which instructions its assembler gives back as they were,
// what each label is -- and builds the module as one L2 tree as it goes: each part of the file a MachineFragment (a
// section, a segment, a function; its origin, its name) or a Comment. This writes the tree as the file:
//
//     MachineFragment   its text, as it is (a fragment `within` another has its text there)
//     Comment           a comment line (`; text`)
//
// The parts are joined by a line break, nothing added and nothing taken away, so the file is the text the writer
// laid out, whatever the assembler. A unit of any other kind is not assembly: it is declined, and the lane that
// holds it writes it with the back end of its language (the C back end for C). Declining is the normal answer of a
// back end that is only asked "can you write this?", so the functions return Result<>, not an exception.
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

#include "cb/core/error.hpp"

namespace cb::translate::backends::assembly {

// A node this back end does not write.
class Declined : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(Declined)
};

// L2 node names the back end understands.
inline constexpr std::string_view kModuleNode = "Module";

// A part of the file written by an assembler. `text` is absent for a fragment whose text another fragment holds
// (`within` is then true); a fragment with neither is an error in the lane that built it.
struct MachineFragment {
    std::string name;
    std::optional<std::string> text;
    bool within = false;
};

// One comment line. `syntax` names the assembler it is written for ("" for the default, a semicolon lead).
struct Comment {
    std::string text;
    std::string syntax;
};

// The lead of a comment where it is not a semicolon, by syntax. A writer for an assembler with another comment lead says so
// here and gives the table to the back end's functions; there is no global registry.
using CommentLeads = std::unordered_map<std::string, std::string>;

// A node of some other kind (its L2 node name): declined.
struct OtherUnit {
    std::string kind;
};

using Unit = std::variant<MachineFragment, Comment, OtherUnit>;

// The L2 document: its node name (a Module is written) and its units in file order.
struct Document {
    std::string kind = std::string(kModuleNode);
    std::vector<Unit> units;
};

// The file and where each unit begins in it.
struct Assembled {
    std::string text;
    // The 1-based line each unit begins on, by unit index; absent for a fragment whose text another holds.
    std::vector<std::optional<std::size_t>> first_line;
};

// One comment line: the lead alone for an empty text, else the lead, a space and the text. The lead is a semicolon unless
// `leads` has one for the syntax.
std::string comment(std::string_view text, std::string_view syntax = {}, const CommentLeads& leads = {});

// The text of one unit; nullopt for a fragment whose text another holds; Declined for a unit that is not assembly.
Result<std::optional<std::string>> unit(const Unit& u, const CommentLeads& leads = {});

// The whole file the Module stands for, with the line each unit begins on.
Result<Assembled> assemble(const Document& doc, const CommentLeads& leads = {});

// Just the file.
Result<std::string> module(const Document& doc, const CommentLeads& leads = {});

}  // namespace cb::translate::backends::assembly
