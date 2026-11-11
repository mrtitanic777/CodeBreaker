// cb/port/csource.hpp -- reading old-style C source for the port's rewriting passes.
//
// The port turns the C of before the standard into C a compiler of today takes (docs: port/pre_ansi). Its passes
// work on the text of a program, line by line, and every one of them asks the same questions of it: how deep in
// braces is this line, where does this function begin and end, which function is this line in, what are the
// arguments of the call on this line, is this variable computed with as a number. They are answered here, once, over
// text whose strings and comments have been blanked (cb::base::ctext::blank keeps every offset), so a search never
// looks inside a literal.
//
// Lines are 0-based, as in the Python's lists; a "line number" a compiler gives (1-based) is converted by the
// caller. Offsets are byte offsets into the line.
#pragma once

#include <cstddef>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/core/regex.hpp"

namespace cb::port::csource {

// ---- vocabulary ---------------------------------------------------------------------------------------

bool is_keyword(std::string_view word);   // the words of C that are not names
bool is_type_word(std::string_view word); // the words that make up a type (int, struct, FILE, int16_t ...)

// Every run of whitespace made one space (the ends are kept: trim for that).
std::string collapse_space(std::string_view s);

// The text's lines, split at '\n' only (a '\r' stays on its line), and the text again.
std::vector<std::string> split_lines(std::string_view text);
std::string join_lines(const std::vector<std::string>& lines);

// ---- patterns that carry a name -----------------------------------------------------------------------

// A pattern in which the placeholder "@@" stands for one identifier of the program (a variable, a function, a
// parameter): the identifier is escaped and put in, and the pattern compiled. This is the ONE sanctioned dynamic
// regular expression of the port -- the name is part of the rule, there is no pattern to compile once -- and every
// caller uses it once per complaint of the compiler, never per character or per line of a file.
Regex name_regex(std::string_view pattern, std::string_view name);
// The name with every punctuation byte escaped, to be one alternative of a pattern built once.
std::string regex_escape(std::string_view name);
// The same with two names: "@1@" and "@2@".
Regex name_regex(std::string_view pattern, std::string_view first, std::string_view second);

// ---- structure ----------------------------------------------------------------------------------------

// The brace depth at the start of each line.
std::vector<int> depths(const std::vector<std::string>& code_lines);

// The line (0-based) a byte offset of a text is on.
class LineIndex {
public:
    explicit LineIndex(std::string_view text);
    std::size_t line_of(std::size_t offset) const;

private:
    std::vector<std::size_t> starts_;
};

// A replacement of text[start, end) by `text`, found in blanked code and made in the original (same offsets).
struct Edit {
    std::size_t start = 0, end = 0;
    std::string text;
    friend auto operator<=>(const Edit&, const Edit&) = default;
};
// Makes the edits from the last to the first, so that each one's offsets are still the original's. Edits that
// overlap are made as given (the later one's offsets then point into text the earlier one changed): the passes never
// produce overlapping edits except by a bug, which this does not hide.
std::string apply_edits(std::string_view text, std::vector<Edit> edits);

// A function defined in a text. Lines are 0-based; `last` may be the line count when the braces never close.
struct FunctionSpan {
    std::string name;
    std::size_t first = 0; // the line it starts on (an old-style definition's type may be on the line before its name)
    std::size_t open = 0;  // the line of its '{'
    std::size_t last = 0;  // the line of its closing '}'
};

// The definitions of a file, new style (`int f(int a) {` with the '{' on the next line, as the port writes them).
std::vector<FunctionSpan> definitions_any(const std::vector<std::string>& code_lines);

// A text with its code blanked and its functions found, kept so that the questions asked of one text during a
// round of mending are answered from one reading of it.
class SourceIndex {
public:
    explicit SourceIndex(std::string text);

    const std::string& text() const { return text_; }
    const std::string& code() const { return code_; } // the text with its literals and comments blanked
    const std::vector<std::string>& code_lines() const { return code_lines_; }
    // Every function: new style first, then old style (a definition with its parameters declared after the head).
    const std::vector<FunctionSpan>& functions() const { return functions_; }
    // The function line `line` (1-based, as a compiler counts) is in.
    const FunctionSpan* enclosing(std::size_t line) const;
    // Whether the function declares `var` itself, as a parameter or a local.
    bool declares_local(const FunctionSpan& fn, std::string_view var) const;

private:
    std::string text_;
    std::string code_;
    std::vector<std::string> code_lines_;
    std::vector<FunctionSpan> functions_;
};

// ---- calls --------------------------------------------------------------------------------------------

// The extent of one argument within its line.
struct Span {
    std::size_t start = 0, end = 0;
    friend bool operator==(const Span&, const Span&) = default;
};

// The arguments of the first call of `fn` on the line. `partial`: a call that goes on to the next line, its
// arguments whole on this one, is read too. nullopt: no such call, or its parentheses do not close.
std::optional<std::vector<Span>> call_args(std::string_view line, std::string_view fn, bool partial = false);

// The innermost call on the line with an argument at `col`.
struct CallAt {
    std::string function;
    std::size_t index = 0; // which argument
    Span span;
};
std::optional<CallAt> call_at(std::string_view line, std::size_t col);

// The start of the postfix expression (a name, its ->, ., [], () chains) that ends just before `at`.
std::size_t object_before(std::string_view line, std::size_t at);

// ---- declarations -------------------------------------------------------------------------------------

// A declaration cut at its top-level commas: 'register struct s *a, b[3]' is the base type 'register struct s' and
// the declarators (a, "*a") (b, "b[3]").
struct NamedDeclarator {
    std::string name;
    std::string text; // the declarator as written: "*a", "b[3]"
};
struct Declaration {
    std::string base;
    std::vector<NamedDeclarator> declarators;
};
// nullopt: no name in it (a declaration of nothing but type words).
std::optional<Declaration> declarators(std::string_view decl);

// The pieces of `s` between the separators that are not inside parentheses or brackets.
std::vector<std::string> split_top(std::string_view s, char sep = ',');

// 'register char *name' -> 'char *': the type alone, for a cast.
std::string abstract_type(std::string_view decl, std::string_view name);

// ---- how a name is used -------------------------------------------------------------------------------

// Whether the variable is computed with as a number somewhere in the code (n /= 2, n * v, n << 1, n & 7, n % k,
// compared with a constant, an index).
bool used_as_number(std::string_view code, std::string_view var);

// Whether a parameter is only ever tested in the body (if (p), !p, p && q): a pointer given for it may be p != 0.
bool only_tested(std::string_view body, std::string_view name);

// {member: the structures that have it}, as "struct tag" / "union tag", of every structure the texts define. A member
// keeps the place it was first seen at, and its structures the order of the texts (insertion order is the meaning:
// the first structure that has the member and is visible to a file is the one a cast names).
using StructMembers = OrderedMap<std::string, std::vector<std::string>>;
StructMembers members(const std::vector<std::string_view>& texts);

// The offset just after the '{' of the first `struct tag {` (or `union tag {`; `keyword` says which) in blanked
// code, or npos.
std::size_t find_tag_body(std::string_view code, std::string_view keyword, std::string_view tag);

// ---- types ----------------------------------------------------------------------------------------------

// The struct and union tags a type may name bare.
using TagSet = std::set<std::string, std::less<>>;

// A type as a compiler wrote it, for a cast or a declaration of the port's (MSVC leaves out the word struct): the
// tags in `tags` get their `struct` back.
std::string ctype(std::string_view type, const TagSet& tags = {});

} // namespace cb::port::csource
