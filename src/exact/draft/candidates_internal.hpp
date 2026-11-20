// exact/draft/candidates_internal.hpp -- the small readers the files of the candidates module share (internal to
// exact/draft/candidates*.cpp): sites in a set, a name made safe for a pattern, the parts of an `int a, x, b;` line,
// and the unsigned-ness of a byte, as the passes ask for them.
#pragma once

#include <cstdint>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "core/regex.hpp"
#include "exact/draft/candidates.hpp"
#include "exact/draft/forms_lines.hpp"

namespace cb::exact::draft::candidates::inner {

using namespace forms::ln;
using text::Site;

// Does the set of byte-wide places hold (pointer, offset)?
inline bool has_site(const std::set<Site>& sites, sv pointer, std::int64_t offset) {
    return sites.contains(Site{str(pointer), offset});
}

// Is any place of this offset (through any pointer) one the code widened with zeros?
inline bool unsigned_at(const std::set<Site>& unsigned_sites, std::int64_t offset) {
    for (const Site& s : unsigned_sites)
        if (s.offset == offset) return true;
    return false;
}

// `text` as a pattern's literal: every byte the pattern language reads as syntax escaped.
inline std::string re_escape(sv text) {
    std::string out;
    for (const char c : text) {
        if (sv("\\.^$|()[]{}*+?").find(c) != npos) out += '\\';
        out += c;
    }
    return out;
}

// The text's name stripped of the characters of `chars` at its start (a name's `lstrip("r_")`).
inline sv strip_left(sv s, sv chars) {
    while (!s.empty() && chars.find(s.front()) != npos) s.remove_prefix(1);
    return s;
}

// The parts of a line `[ \t]*int a, X, b;` (words, each followed by ", ") around X: the names before it and after it.
struct IntDecl {
    std::string indent;
    std::vector<std::string> before, after;
    std::string rest() const;   // the other names, joined by ", " (empty when X was alone)
};
std::optional<IntDecl> int_decl_parts(sv line, sv x);

// Every line of the text through `fn` (a line it does not change stays); the lines are joined with "\n" again.
template <class F>
std::string map_lines(sv text, F fn) {
    Lines lines = split_nl(text);
    for (std::string& l : lines) l = fn(l);
    return join_nl(lines);
}

// The groups of `m` as text, numbered from 1 (a group that did not take part is empty).
inline std::string group_text(const RegexMatch& m, std::size_t g) { return str(m.group(g)); }

}  // namespace cb::exact::draft::candidates::inner
