// cb/exact/draft/text.hpp -- what every rewrite of a draft reads its text with: the parts of an if, a block's ends, a
// test's parentheses and negation, whether a register is read later, and the names of registers and the library's
// data.
//
// A draft is C text written by an inverse model of the compiler, in the layout the model prints: one statement to a
// line, blocks indented, tests parenthesised, labels `L_xxxx:` alone on a line. The functions here read exactly that
// layout (not C in general): they are small parsers over lines, which replace the pattern matching the original did.
// Lines are `std::string`; a block's depth is its indentation.
//
// The state the draft passes build up while they read one function (which bytes are unsigned, which globals are
// arrays, which values are longs ...) lives in `FunctionFacts`, and what is kept between functions of a project in
// `ProjectCaches`; the passes (shapes, forms, candidates, prove) hold and hand them down - there are no tables at
// namespace scope.
#pragma once

#include <array>
#include <compare>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/ordered_map.hpp"

namespace cb::exact::draft::text {

// The registers a draft names (a draft is of 16-bit code): the sixteen-bit ones, their halves, and es.
inline constexpr std::array<std::string_view, 15> kRegisters = {"ax", "bx", "cx", "dx", "si", "di", "al", "ah",
                                                                "bl", "bh", "cl", "ch", "dl", "dh", "es"};
inline constexpr std::array<std::string_view, 6> kWordRegisters = {"ax", "bx", "cx", "dx", "si", "di"};

bool is_register(std::string_view word);
bool is_word_register(std::string_view word);

// The size of a C type's last word (char 1, int/unsigned/short 2, long 4), or `otherwise` for any other word.
std::uint32_t size_of_type(std::string_view last_word, std::uint32_t otherwise = 2);

// ---- tests: parentheses, truth, negation ----------------------------------------------------------------------

// ((X)) -> (X): the parentheses a test is printed in, once.
std::string strip_parens(std::string_view c);

// A test as the value it tests: (X == 0) -> !X, (X != 0) -> X.
std::string truth(std::string_view c);

// Do the parentheses of s pair up (none closes before it opens, none left open)?
bool balanced(std::string_view s);

// c split at op (&& or ||) outside parentheses, or just c.
std::vector<std::string> top_split(std::string_view c, std::string_view op);

// c split at its commas outside parentheses and brackets.
std::vector<std::string> top_commas(std::string_view s);

// The test that is true when c is false: !(X) -> (X), De Morgan a level down, a comparison flipped, X == 0 -> X.
std::string negate(std::string_view c);

// `a op b` for op && or ||, an operand that is the other kind of join parenthesized (|| inside &&).
std::string join_tests(std::string_view op, std::string_view a, std::string_view b);

// ---- blocks, by indentation ---------------------------------------------------------------------------------------

// The line opening the block that line k (`}` or `} else {`) closes: the nearest before it at its indentation.
std::optional<std::size_t> opener(std::span<const std::string> lines, std::size_t k);

// The line opening each block around line k, the innermost first.
std::vector<std::size_t> openers(std::span<const std::string> lines, std::size_t k);

// The index of the line closing the block line k opens (by indentation: the first line back at its indent).
std::optional<std::size_t> block_end(std::span<const std::string> lines, std::size_t k);

// Is line j inside a loop or a switch opened after line `lo`, deeper than the indentation `ind`? (`lo` may be -1:
// from the top.)
bool in_loop(std::span<const std::string> lines, std::ptrdiff_t lo, std::size_t j, std::size_t ind);

// `} else if (C) {` chains written out as nested ifs, each inside the else before it (the lines are rewritten in
// place).
void else_ifs(std::vector<std::string>& lines);

// The if at line k: `if (C) {`, its arms' lines, and the line after it.
struct IfParts {
    std::string cond;
    std::vector<std::string> then_arm;
    std::optional<std::vector<std::string>> else_arm;
    std::size_t end = 0;   // the line after the whole if
};
std::optional<IfParts> if_parts(std::span<const std::string> lines, std::size_t k);

// An arm without the label the other arm's goto went to (the only label it loses: others are wanted). Without
// `other`, every label is dropped.
std::vector<std::string> unlabelled(std::span<const std::string> arm,
                                    const std::optional<std::vector<std::string>>& other = std::nullopt);

// Do the arms x and y run the same: equal, or x the statements y begins with and then a goto to y's label there (the
// compiler's one copy of a tail both reach) - and the label then wanted by none else.
bool same_arm(std::span<const std::string> x, std::span<const std::string> y);

// `goto L_xxx;` at the start of the line (after any indent): the label. With `whole`, nothing may follow the
// semicolon.
std::optional<std::string_view> goto_target(std::string_view line, bool whole);

// `L_xxx:` alone on the line (after any indent).
bool label_line(std::string_view line);

// ---- registers read later -------------------------------------------------------------------------------------

struct LiveOptions {
    bool follow_gotos = true;   // a jump elsewhere: is the register read there before it is given a value?
};

// Is `reg` (or any of the words in `reads`: its byte too) read from line `first` on, before it is given a new value
// - on any path: a value given it inside a nested block (an arm, a loop) is not on every path, so it ends nothing;
// one given at the same depth, in a bare block, or out of the block does. A dropped line (a pass that has deleted
// one without renumbering) is nullopt and is skipped. `reads` empty means just `reg`.
bool live_after(std::span<const std::string> lines, std::size_t first, std::string_view reg,
                std::span<const std::string_view> reads = {}, LiveOptions options = {});
bool live_after(std::span<const std::optional<std::string>> lines, std::size_t first, std::string_view reg,
                std::span<const std::string_view> reads = {}, LiveOptions options = {});

// `X = E;` with reg read in E alone and E giving nothing a value: reg's value read before the store.
bool plain_store(std::string_view line, std::string_view reg);

// ---- the function's text --------------------------------------------------------------------------------------

// The source with x (an int parameter or local) declared a pointer to `type`, or nullopt when no declaration of x
// as an int is found.
std::optional<std::string> declare_pointer(std::string_view source, std::string_view x, std::string_view type);

struct BodySplit {
    std::string head;   // up to the function body's own brace: its signature and what a pass declared before it
    std::string brace;  // "{", or empty when there is no brace at all
    std::string body;   // what follows the brace
};
// Split at the function body's own brace - a line of "{" alone, after its signature - not a struct's that a pass
// declared before it; with no such line, at the first "{".
BodySplit split_body(std::string_view source);

// f(text), or nullopt when it changes nothing (a pass as a variant).
inline std::optional<std::string> changed(const std::function<std::string(const std::string&)>& f, const std::string& text) {
    std::string t = f(text);
    if (t == text) return std::nullopt;
    return t;
}

// ---- what the passes learn of the function being drafted ----------------------------------------------------

// A place the model prints as (pointer, offset): a register or "*"/"data" and a constant offset.
struct Site {
    std::string pointer;
    std::int64_t offset = 0;
    friend auto operator<=>(const Site&, const Site&) = default;
};

struct StructField {
    std::uint32_t offset = 0;
    std::string type;
};

// An array of structs in the data: the element's size and what is known of its fields.
struct StructArray {
    std::uint32_t size = 0;
    std::string first_type;              // the first field's type
    std::vector<StructField> fields;     // once the struct pass has read the rest: every field (empty before)
};

// The range a loaded register is tested against.
struct Range {
    std::int64_t lo = 0, hi = 0;
    bool above = false;                  // the test was jg/ja
    friend auto operator<=>(const Range&, const Range&) = default;
};

// What the code shows of the function's longs.
struct LongInfo {
    std::set<std::string> unsigned_names;      // the longs shifted without sign
    std::optional<bool> ret;                   // the function's value is a long (unknown: nullopt)
    std::set<std::int64_t> fields;             // struct fields that are the low words of longs
    std::set<std::string> globals;             // globals that are
    std::set<std::string> funcs;               // routines whose value is one
};

struct FunctionFacts {
    std::set<Site> unsigned_sites;                         // bytes the code widens with zeros: unsigned char
    std::set<std::string> unsigned_globals;                //  (the same, of globals)
    std::set<std::string> byte_globals;                    // globals the code reads or writes a byte of: char
    std::set<Site> byte_sites;                             // the byte-wide places the code reads
    std::set<std::string> word_pointers;                   // data a draft steps through by 2: a pointer to words
    std::map<std::string, StructArray> struct_arrays;      // data that is an array of structs
    std::map<std::string, std::string> arrays;             // data a draft indexes: name -> element type
    OrderedMap<std::string, std::string> streams;          // a stream a library macro reads: name -> its type
    std::optional<std::string> include, include2;          // the headers those macros need
    std::set<Range> ranges;
    LongInfo longs;
    void clear() { *this = FunctionFacts(); }
};

// A library macro that reads a table (the draft reads its uses back as the macro).
struct TableMacro {
    std::string macro, table;
    std::int64_t offset = 0, mask = 0;
};

// What is kept between the functions of a project.
struct ProjectCaches {
    std::map<std::string, std::map<std::string, std::uint32_t>> library_data;   // project dir -> {name: address}
    std::map<std::pair<std::string, std::string>, std::set<std::string>> headers;   // (toolchain, header) -> names
    std::map<std::pair<std::string, std::string>, std::vector<TableMacro>> tables;  // (project dir, header) -> macros
};

}  // namespace cb::exact::draft::text
