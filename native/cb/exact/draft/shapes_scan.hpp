// cb/exact/draft/shapes_scan.hpp -- the small readers the shape passes share (internal to exact/draft/shapes_*.cpp).
//
// A draft is text in the layout the inverse model prints; the passes work on its lines (split on "\n", joined back with
// "\n", so a pass that changes nothing gives back the very text it was given) and read names as whole words. What is
// here is that vocabulary: lines, indentation, a name found as a word, and the few shapes of line that several passes
// ask about.
#pragma once

#include <cstddef>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/regex.hpp"

namespace cb::exact::draft::shapes::scan {

using Lines = std::vector<std::string>;
using MaybeLines = std::vector<std::optional<std::string>>;   // a pass that deletes lines by index marks them nullopt

// The text's lines: split at every "\n" (a text ending in "\n" ends in an empty line; the empty text is one).
Lines split_lines(std::string_view text);
std::string join_lines(std::span<const std::string> lines);
// The lines still there, joined.
std::string join_live(std::span<const std::optional<std::string>> lines);
Lines live_lines(std::span<const std::optional<std::string>> lines);
MaybeLines maybe(std::span<const std::string> lines);

// How far a line is indented: its length less what is left when its leading blanks are taken off.
std::size_t indent_of(std::string_view line);
// The line's leading blanks.
std::string_view indent_text(std::string_view line);
// The line without a four-blank step of indentation, when it is indented that far past `ind`; else as it is.
std::string dedent(std::string_view line, std::string_view ind);
bool blank(std::string_view line);

bool is_word_char(char c);
// Is there a word boundary between the bytes before and at `pos`?
bool boundary_at(std::string_view s, std::size_t pos);
// How many times `name` stands in s as a whole word (a name that starts or ends with a non-word byte is bounded by
// word characters there, as `\b` reads it).
std::size_t count_word(std::string_view s, std::string_view name);
bool has_word(std::string_view s, std::string_view name);
// The first occurrence at or after `from`, as a whole word.
std::optional<std::size_t> find_word(std::string_view s, std::string_view name, std::size_t from = 0);
std::string replace_word_first(std::string_view s, std::string_view name, std::string_view with);
std::string replace_word_all(std::string_view s, std::string_view name, std::string_view with);

// `name = ` or `name++` / `name--` / `++name` / `--name`: the places a line gives a name a value.
bool is_assigned(std::string_view s, std::string_view name);                // \bname\s*=[^=]
bool is_assigned_after_non_name(std::string_view s, std::string_view name); // (?<![\w>.])name\s*(?:=[^=]|\+\+|--)
bool is_updated(std::string_view s, std::string_view name);                 // is_assigned, name++ name-- ++name --name

// The text up to the first `\n\s*REG = ` (all of it when there is none): where a register is given a new value. With
// `unless_read`, an assignment whose right side reads REG (up to the `;` or the line's end) does not count. The text
// is taken as it is: its first line is not preceded by a newline unless the caller put one there.
std::string until_given(std::string_view text, std::string_view reg, bool unless_read);

// The statements of a block header: `case K:` / `default:` alone on a line.
bool is_case_label(std::string_view line);
// `case K:` / `default:` exactly after the indentation `ind` (nothing between: the case line of a switch at `ind`).
bool is_case_label_at(std::string_view line, std::string_view ind);

// A decimal number (digits, an optional leading minus) read as a number; nullopt if it does not fit.
std::optional<std::int64_t> number(std::string_view s);

// The pieces joined, whatever kind of string each is (std::string has no operator+ for string_view).
template <class... Parts>
std::string cat(const Parts&... parts) {
    std::string out;
    (out.append(std::string_view(parts)), ...);
    return out;
}

// Is `reg` (or one of `reads`) read from line `first` on, before it is given a value: text::live_after on lines of this
// module (a line may be nullopt: dropped).
bool live_from(std::span<const std::string> lines, std::size_t first, std::string_view reg,
               std::span<const std::string_view> reads = {});
bool live_from(std::span<const std::optional<std::string>> lines, std::size_t first, std::string_view reg,
               std::span<const std::string_view> reads = {});

// The lines from `first` up to (not including) `last`, clamped to the vector's ends.
Lines slice(const Lines& v, std::size_t first, std::size_t last);

// Labels and the gotos to them.
bool goes_to(std::string_view text, std::string_view label);                  // \bgoto LABEL;
std::size_t count_gotos(std::string_view text, std::string_view label);
std::set<std::string> used_labels(std::string_view text);                      // every L_xxx something goes to
Lines without_label(const Lines& lines, std::string_view label);               // `[ \t]*LABEL:` lines gone (not the last line)
Lines drop_unused_labels(const Lines& lines, const std::set<std::string>& used);

// `\s*REG = (.*);\s*$` (with `.+` when `nonempty`): the value a line gives the register, up to the last `;`.
std::optional<std::string> value_given(std::string_view line, std::string_view reg, bool nonempty);
// `\s*REG = [^;]+;` as a whole line: the statement giving the register a value, nothing after its `;`.
std::optional<std::string> plain_value(std::string_view line, std::string_view reg);
// `<prefix>return\b[^;]*;` as a whole line.
bool is_return_at(std::string_view line, std::string_view prefix);

}  // namespace cb::exact::draft::shapes::scan
