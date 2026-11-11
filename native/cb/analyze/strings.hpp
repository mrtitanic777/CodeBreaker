// cb/analyze/strings.hpp -- a module's strings: where they are, who uses them, and what they say.
//
//     auto graph = call_graph::build(image);
//     strings::Table table = strings::build_table(image, graph);       // address -> string
//     auto uses = strings::find_uses(image, graph, table);              // function -> the strings it refers to
//     auto stack = strings::stack_strings(image);                       // strings assembled by immediate stores
//     auto formats = strings::find_formats(image, graph, table);        // printf-like call sites and their formats
//     strings::Report report = strings::analyse(image, graph);          // all of it, with counts and a summary
//
// A string is a run of printable characters ending in a zero byte (or of 16-bit characters ending in a zero word).
// One the code refers to is believed from two characters up; one nothing refers to is listed only from five (four
// in 16-bit code), and only outside the code (early linkers put constants in the code section, where an
// unreferenced "string" is usually instructions).
//
// A format string says what follows it in the argument list: `"%s = %ld"` is a string pointer and a long. That types
// the unnamed arguments of every call to wsprintf and its relatives -- and finds the module's own printf-like
// functions: a function that is handed a literal with conversions in it, at the same position, from several places.
//
// The text of a string is ASCII (printable characters, tab, line feed, carriage return), so it is held as it is.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/native.hpp"

namespace cb::analyze::strings {

enum class Encoding { Ascii, Wide };   // one byte per character, or 16-bit characters

// ---- the table ----------------------------------------------------------------------------------------------

struct Found {
    std::string text;
    Encoding encoding = Encoding::Ascii;
};

// A string of at least `least` characters starting at `start` in `data`: printable bytes (or letter-and-zero pairs)
// followed by a zero; nothing otherwise.
std::optional<std::string> read_ascii(ByteView data, std::int64_t start, std::int64_t least);
std::optional<std::string> read_wide(ByteView data, std::int64_t start, std::int64_t least);

// The string at `offset` in a span's bytes, the first `initialised` of which the file holds; 16-bit characters are
// tried first in 32-bit code. A short one that follows a printable byte is the tail of a longer string, not a string.
std::optional<Found> string_in(ByteView data, std::int64_t offset, std::int64_t initialised, int bits, int least = 2);

// The string at exactly `va` (at least `least` characters), or nothing.
std::optional<Found> string_at(const Image& image, Va va, int least = 2);

struct Entry {
    std::string text;
    Encoding encoding = Encoding::Ascii;
    bool referenced = false;              // does some code (or the loader's relocations) refer to it?
};
using Table = std::map<Va, Entry>;        // by address

// Every string in the module: the ones the code or the file's relocations point at, then the ones nothing points at.
Table build_table(const Image& image, const call_graph::Graph& graph);

// ---- uses ---------------------------------------------------------------------------------------------------

struct Use {
    Va site = 0;                          // the instruction
    Va address = 0;                       // the string
    std::string text;
};
struct FunctionUses {
    Va function = 0;
    std::vector<Use> uses;
};

// The strings each function refers to, in the order the functions first do. In 16-bit code a use also marks the
// string referenced in `table` (the code names an offset in the data segment, not an address).
std::vector<FunctionUses> find_uses(Image& image, const call_graph::Graph& graph, Table& table);

// ---- strings built on the stack -----------------------------------------------------------------------------

struct StackString {
    Va function = 0;
    std::string where;                    // "ebp-0x20": the first byte's place in the frame
    std::string text;
    bool wide = false;
};

// Strings a function assembles by storing immediates into its frame.
std::vector<StackString> stack_strings(Image& image);

// Immediate stores into a frame, by (base register, offset): the byte stored and the number of the store that wrote it.
struct Store {
    std::uint8_t byte = 0;
    int number = 0;
};
using Stores = std::map<std::pair<std::string, std::int64_t>, Store>;
struct Piece {
    std::string base;
    std::int64_t offset = 0;
    std::string text;
    bool wide = false;
};
// The printable runs in a set of stores: at least `least` characters written by at least two stores, with letters in
// them; or 16-bit characters (a letter and a zero, three or more times).
std::vector<Piece> assemble(const Stores& stores, std::size_t least = 4);

// ---- format strings -----------------------------------------------------------------------------------------

// The C types of the arguments a format string consumes, in order. `wide` is the format's own character width.
std::vector<std::string> conversions(std::string_view format, bool wide = false);

// The string a call's k-th argument points at.
struct Literal {
    Va address = 0;
    const Entry* entry = nullptr;
};
std::optional<Literal> literal(const Image& image, const Table& table, const std::vector<Argument>& args, std::size_t k);

struct FormatCall {
    Va function = 0;                      // the function making the call
    Va site = 0;
    std::string api;                      // "USER32!wsprintfA", or the label of the module's own printf-like function
    bool imported = true;                 // false: a printf-like function of the module's own
    std::string format;
    bool wide = false;
    bool lists_arguments = true;          // false: the arguments are in a va_list elsewhere
};
struct Formats {
    std::vector<FormatCall> calls;
    std::int64_t sites = 0;               // call sites of printf-like imports
    std::map<Va, int> own;                // the module's own printf-like functions -> the position of their format
};
Formats find_formats(Image& image, const call_graph::Graph& graph, const Table& table);

// ---- everything, as facts ---------------------------------------------------------------------------------------

struct Counts {
    std::int64_t strings = 0;
    std::int64_t referenced = 0;
    std::int64_t wide = 0;
    std::int64_t functions_using_strings = 0;
    std::int64_t string_uses = 0;
    std::int64_t stack_strings = 0;
    std::int64_t formatter_call_sites = 0;
    std::int64_t format_strings_read = 0;
    std::int64_t own_printf_like_functions = 0;
    std::int64_t format_strings_at_own_functions = 0;
    std::int64_t arguments_typed_by_formats = 0;
};

struct StringFact {
    Va at = 0;
    std::string text;
    bool wide = false;
    bool referenced = false;
};
struct StringUsesFact {
    Va function = 0;
    std::vector<std::string> strings;     // sorted, distinct
};
struct FormatFact {
    Va function = 0;
    Va site = 0;
    std::string api;
    bool imported = true;
    std::string format;
    std::optional<std::vector<std::string>> arguments;   // none: the argument list is elsewhere
};
struct PrintfLikeFact {
    Va function = 0;
    int format_argument = 0;
};

struct Report {
    Counts counts;
    std::vector<StringFact> strings;                  // by address
    std::vector<StringUsesFact> string_uses;          // by function
    std::vector<StackString> stack_strings;
    std::vector<FormatFact> formats;
    std::vector<PrintfLikeFact> printf_like;          // by function
    std::vector<std::string> text;                    // a short summary for people
};

// The report from the parts (a caller that holds them in another shape builds it so); `label` names an address.
std::vector<FormatFact> format_facts(const Formats& formats);   // the formats with their argument types
Report make_report(const Table& table, const std::vector<FunctionUses>& uses, const std::vector<StackString>& stack,
                   const std::vector<FormatFact>& formats, const std::map<Va, int>& own, std::int64_t formatter_sites,
                   const std::function<std::string(Va)>& label);

Report analyse(Image& image, const call_graph::Graph& graph);

}  // namespace cb::analyze::strings
