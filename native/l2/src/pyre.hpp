// pyre.hpp -- regular expressions with Python's `re` semantics: a backtracking matcher over the same
// syntax (groups, (?:), (?P<name>), lookahead and fixed-width lookbehind, lazy quantifiers, \b, the
// MULTILINE and DOTALL flags, inline (?ms)) and the same match, fullmatch, search, finditer, findall
// and sub, so the C back end's patterns are carried over as they are written in its Python.
// Strings are UTF-8 and matched by code point; \w, \d and \s are Unicode's, as Python 3's str patterns.
#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace re {

enum Flags { I = 2, M = 8, S = 16 };

struct Node;

struct Match {
    std::shared_ptr<const std::string> s;     // (its own copy of the text: a match outlives a temporary)
    std::vector<long> caps;          // start, end of each group (group 0 first); -1 unmatched
    bool ok = false;
    explicit operator bool() const { return ok; }
    bool has(int g) const { return ok && caps[2 * g] >= 0; }
    std::string group(int g = 0) const;     // '' where the group did not take part (check has())
    long start(int g = 0) const { return caps[2 * g]; }
    long end(int g = 0) const { return caps[2 * g + 1]; }
    int ngroups() const { return (int)caps.size() / 2 - 1; }
};

class Regex {
public:
    Regex() {}
    explicit Regex(const std::string& pattern, int flags = 0);
    Match match(const std::string& s, size_t pos = 0, size_t endpos = std::string::npos) const;
    Match fullmatch(const std::string& s, size_t pos = 0, size_t endpos = std::string::npos) const;
    Match search(const std::string& s, size_t pos = 0, size_t endpos = std::string::npos) const;
    std::vector<Match> finditer(const std::string& s) const;
    // findall: each match's text (no groups), its one group, or (several groups) the groups joined
    // into a vector -- see findall_groups
    std::vector<std::string> findall(const std::string& s) const;
    std::vector<std::vector<std::string>> findall_groups(const std::string& s) const;
    std::string sub(const std::string& repl, const std::string& s, int count = 0) const;   // a template
    std::string sub_fn(const std::function<std::string(const Match&)>& fn, const std::string& s, int count = 0) const;
    int groups() const { return ngroups_; }
    const std::string& pattern() const { return pattern_; }
    int flags() const { return flags_; }
    const std::vector<std::pair<std::string, int>>& names() const { return names_; }   // (?P<name>...): name, group

private:
    std::shared_ptr<Node> root_;
    int ngroups_ = 0;
    int flags_ = 0;
    std::string pattern_;
    std::vector<std::pair<std::string, int>> names_;
    Match run(const std::shared_ptr<const std::string>& s, size_t pos, size_t endpos, int mode, bool must_advance = false) const;
    friend struct Parser;
};

std::string escape(const std::string& s);                 // re.escape

// a pattern compiled once, wherever it is first used (the Python's module-level re.compile)
const Regex& rx(const std::string& pattern, int flags = 0);

}  // namespace re
