// cb/port/c_source.hpp -- reading old C: the scanners the port's passes share.
//
// Everything here works on C text whose strings, character constants and comments have been blanked
// (cb::base::ctext::blank) unless it says otherwise, at byte offsets, and never looks inside a literal. A "line" is
// a piece of the text split at '\n' (empty pieces kept, so line numbers are the file's). Old-style C is the
// subject: `f(a, b) int a; char *b; { ... }` definitions, declarations without prototypes.
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/port/pre_ansi_types.hpp"

namespace cb::port::pre_ansi {

using Files = OrderedMap<std::string, std::string>;   // file name -> text, in the order the program lists them

// ---- vocabulary ------------------------------------------------------------------------------------------------
bool is_keyword(std::string_view word);     // a C keyword (the old compilers' set)
bool is_type_word(std::string_view word);   // a word a declaration's type is made of (int, char, struct, FILE, int16_t ...)

// Whether the port's rule `name` runs. The rules the MS C 3.00 port brought each have a name; the environment
// variable CB_PORT_RULES turns them on or off over the defaults, comma separated (`+rule` or `rule` on, `-rule`
// off, `+all` / `-all` every one) -- a regression's bisection.
bool rule_enabled(std::string_view name);

// ---- lines and braces --------------------------------------------------------------------------------------------
std::vector<std::string> split_lines(std::string_view text);   // at '\n'; "" gives one empty line
std::string join_lines(const std::vector<std::string>& lines);
// The brace depth at the start of each line.
std::vector<int> brace_depths(const std::vector<std::string>& code_lines);
// The index of the '}' that closes the '{' at `open` in blanked code, or code.size() if it never closes.
std::size_t match_brace(std::string_view code, std::size_t open);

// ---- text helpers the scanners need (the idioms of the regexes they replace) ------------------------------------
bool is_word_char(char c);                                     // [A-Za-z0-9_]
std::string collapse_space(std::string_view s);                // every run of whitespace one space
// The first place at or after `from` where `word` stands as a whole word ([A-Za-z0-9_] on neither side), or npos.
std::size_t find_word(std::string_view text, std::string_view word, std::size_t from = 0);
// `s` as a regex matching exactly it (the idiom of re.escape); a space is `\s+` when `spaces_flex` is set.
std::string regex_quote(std::string_view s, bool spaces_flex = false);
// The leading whitespace of a line.
std::string_view indent_of(std::string_view line);

// ---- declarations -------------------------------------------------------------------------------------------------
// s split at `sep`, but not inside () or [].
std::vector<std::string_view> split_top(std::string_view s, char sep = ',');

struct Declarator {
    std::string name;   // the identifier
    std::string text;   // its whole declarator: `*a`, `b[3]`, `(*f)()`
};
struct Declaration {
    std::string base;                    // `register struct s`
    std::vector<Declarator> names;
};
// 'register struct s *a, b[3]' -> base type and [(a, "*a"), (b, "b[3]")]; empty if there is no name in it.
std::optional<Declaration> declarators(std::string_view decl);
// The same, with size_t among the words a type is made of (the structures' members are read so).
std::optional<Declaration> declarators(std::string_view decl, bool size_t_is_type);

// A parameter declared an array (`T a[]`, `T a[][3]`) as the pointer it is (`T *a`, `T (*a)[3]`), which a prototype
// can name before T is complete.
std::string array_param(std::string_view type, std::string_view name);

// ---- functions ----------------------------------------------------------------------------------------------------
struct Definition {
    Fn fn;
    std::size_t first = 0;   // first line (the line before the name when the type stands alone there)
    std::size_t open = 0;    // the line of its '{'
    std::size_t last = 0;    // its last line
};
// The old-style definitions of a file (`f(a, b) int a; { ... }`).
std::vector<Definition> definitions(const std::vector<std::string>& lines, const std::vector<std::string>& code);

struct NamedDefinition {
    std::string name;
    std::size_t first = 0, open = 0, last = 0;
};
// The function definitions of a file, old style or new.
std::vector<NamedDefinition> definitions_any(const std::vector<std::string>& lines, const std::vector<std::string>& code);

// ---- calls ---------------------------------------------------------------------------------------------------------
struct Span {
    std::size_t begin = 0, end = 0;   // [begin, end) in the text it came from
    std::size_t size() const { return end - begin; }
};
// The arguments of the first call of `fn` on the line. With `partial`, a call that goes on to the next line gives
// the arguments whole on this one.
std::optional<std::vector<Span>> call_args(std::string_view line, std::string_view fn, bool partial = false);
// The arguments of every call of `fn` in a body, a call that runs out of text giving those it had.
std::vector<std::vector<Span>> calls_of(std::string_view body, std::string_view fn);
// Where every call of `fn` starts in the text (the name a whole word, then optional whitespace and '('), in order.
std::vector<std::size_t> call_starts(std::string_view text, std::string_view fn);
// The start of the postfix expression that ends just before `at` (the `->` of a member); empty if the brackets
// before it do not balance.
std::optional<std::size_t> object_before(std::string_view line, std::size_t at);
// 'register char *name' -> 'char *' (the type alone, for a cast)
std::string abstract_type(std::string_view decl, std::string_view name);
// Whether a parameter is only ever tested (if (p), !p, p && q): a pointer given for it may be p != 0.
bool only_tested(std::string_view body, std::string_view name);

// ---- structures -----------------------------------------------------------------------------------------------------
// {member: [the structures that have it]} of every structure the texts define, members in order of first sight.
OrderedMap<std::string, std::vector<std::string>> struct_members(const std::vector<std::string>& texts);

// ---- literals ---------------------------------------------------------------------------------------------------------
// The bytes of a C string literal ("a\0b" "c") as the compiler made them: the escapes read, the terminating 0 left
// out. A character past Latin-1 is '?'. Throws cb::ParseError on a \x escape without two hex digits; an escape C does not have is kept as written.
Bytes c_string_bytes(std::string_view literal);

}  // namespace cb::port::pre_ansi
