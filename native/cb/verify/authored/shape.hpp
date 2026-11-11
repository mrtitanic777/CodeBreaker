// cb/verify/authored/shape.hpp -- the same function written in the other shapes a person might have written it in.
//
//     for (const Rewrite& r : shape::switches(body, shadow))   an if-chain on one value as a switch
//     auto new_body = shape::returns(head, decls, body, shadow)  `r = x; ... return r;` as `return x;`
//     for (const Rewrite& r : shape::folds(decls, body, shadow)) `t = f(...); if (t < N) {` as `if (f(...) < N) {`
//
// A decompiler gives every function one shape: tests in chains of if and else, one variable that collects the result,
// one return at the end. A compiler that optimises sees through small differences of spelling (the order of a
// comparison's sides, the order of declarations) and gives the same bytes for all of them -- but not through these: a
// `switch` is compiled as a switch, and a function that returns from the middle is laid out differently from one that
// does not. Each rewrite here changes no meaning; the repair step of the authored-source match tries them and keeps what
// brings the bytes nearer the original's.
//
// `shadow` is the body with its strings and comments blanked, the same length: where things are is found in it, what is
// written comes from the body. All offsets are byte offsets into the body (C source is ASCII).
#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace cb::verify::authored::shape {

// [begin, end) of a statement.
struct Span {
    std::size_t begin = 0, end = 0;
    bool operator==(const Span&) const = default;
};

// Replace body[begin, end) by `text`.
struct Rewrite {
    std::size_t begin = 0, end = 0;
    std::string text;
    bool operator==(const Rewrite&) const = default;
};

// The statements between `lo` and `hi`, an if with all its elses (and a do with its while) as one. Stops, saying
// nothing more, at a block that is not closed within `hi`.
std::vector<Span> statements(std::string_view shadow, std::size_t lo, std::size_t hi);

// Each chain `if (v == A) {..} else if (v == B) {..} [else {..}]` as a switch. Not when a branch holds a `break` (it
// would leave the switch instead of the loop it was in), nor when a value is tested twice, nor when a constant
// is an octal one (written with a leading 0) or does not fit 64 bits.
std::vector<Rewrite> switches(std::string_view body, std::string_view shadow);

// The body with every `r = x;` that only the final `return r;` follows written `return x;`, or nullopt when there is
// none. Only when `r` has the type the function returns: handing a value back through a narrower variable cuts it
// down, and returning it directly would not. `head` is the function's heading, `decls` its local declarations.
std::optional<std::string> returns(std::string_view head, std::span<const std::string> decls, std::string_view body,
                                   std::string_view shadow);

// The type the function returns, from its heading (the word before its convention and name); nullopt if the heading
// has none.
std::optional<std::string> returned_type(std::string_view head);

// Each `t = f(...);` followed at once by `if (t <compare> N) {`, where that is all `t` is for: written
// `if (f(...) <compare> N) {`. A decompiler names every value it sees; a person tests the call. Only for a variable of
// the type a function returns (int4), so that nothing is cut down or widened on the way, and only when the two lines
// are its only mentions.
std::vector<Rewrite> folds(std::span<const std::string> decls, std::string_view body, std::string_view shadow);

}  // namespace cb::verify::authored::shape
