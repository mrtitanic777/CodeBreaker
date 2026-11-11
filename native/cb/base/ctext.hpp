// cb/base/ctext.hpp -- C text with its strings, character constants and comments blanked: what is code, at the
// offsets it has, for the searches and counts that must not look inside a literal.
//
// Every function here keeps the text's length and every newline, so an offset into the blanked text is an offset
// into the original (byte offsets: the text is UTF-8 and a multi-byte character inside a literal becomes that many
// spaces). A comment is `/* ... */` only; `//` comments are not handled (the code this serves never had them).
#pragma once
#include <functional>
#include <string>
#include <string_view>

#include "cb/core/regex.hpp"

namespace cb::base::ctext {

enum class Mode {
    // What is inside each comment, string and character constant becomes spaces: a string or constant keeps its
    // quotes, a comment loses its delimiters too, and a comment's newlines are kept. One left open runs to the end
    // of its line (a string, a constant) or of the text (a comment).
    InsideLiterals,
    // Each closed one becomes spaces whole, quotes and newlines included. A quote with no partner on its line is
    // left as it is (an apostrophe in prose).
    WholeClosed,
    // The same, but a string or constant runs on across lines to its partner (how a brace count has always read them).
    WholeClosedAcrossLines,
};

// The text with its comments, strings and character constants blanked, as `mode` says.
std::string blank(std::string_view text, Mode mode = Mode::InsideLiterals);

// rx.replace_all over what is code alone: matches are found in the blanked text, so a pattern never matches inside
// a literal or comment, and `fn` is given each match (its offsets are the original text's) and returns the
// replacement; text between matches is copied from the original, literals included.
std::string sub_code(std::string_view text, const Regex& rx, const std::function<std::string(const RegexMatch&)>& fn);

}  // namespace cb::base::ctext
