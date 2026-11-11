// pyre.hpp -- regular expressions with Python's `re` semantics: a backtracking matcher over the same
// syntax (groups, (?:), (?P<name>), lookahead and fixed-width lookbehind, lazy quantifiers, \b, the
// MULTILINE and DOTALL flags, inline (?ms)) and the same match, fullmatch, search, finditer, findall
// and sub, so the C back end's patterns are carried over as they are written in its Python.
// Strings are UTF-8 and matched by code point; \w, \d and \s are Unicode's, as Python 3's str patterns.
#pragma once
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace re {

// what a template or pattern raises, by Python's exception name (re.error is PatternError, an unknown group
// name IndexError)
struct Error : std::runtime_error {
    std::string kind;
    Error(std::string k, const std::string& m) : std::runtime_error(m), kind(std::move(k)) {}
};

enum Flags { I = 2, M = 8, S = 16, A = 256 };   // (A: re.ASCII -- \w \d \s ASCII only)

struct Node;
struct Prog;          // the compiled program a Regex runs
struct CpIndex;       // byte offset -> code-point offset table, shared by the matches of one subject

struct Match {
    std::shared_ptr<const std::string> s;     // (its own copy of the text: a match outlives a temporary)
    std::vector<long> caps;          // start, end of each group (group 0 first), as UTF-8 byte offsets; -1 unmatched
    mutable std::shared_ptr<CpIndex> cpx;   // (built on the first cp_start / cp_end; none for an ASCII text)
    bool ascii = false;              // the text is ASCII: a byte offset is a code-point offset
    bool ok = false;
    explicit operator bool() const { return ok; }
    bool has(int g) const { return ok && caps[2 * g] >= 0; }
    std::string group(int g = 0) const;     // '' where the group did not take part (check has())
    long start(int g = 0) const { return caps[2 * g]; }
    long end(int g = 0) const { return caps[2 * g + 1]; }
    int ngroups() const { return (int)caps.size() / 2 - 1; }
    long cp_offset(long byte) const;         // a byte offset as a code-point index (Python's), in constant time; -1 stays -1
    long cp_start(int g = 0) const { return cp_offset(caps[2 * g]); }
    long cp_end(int g = 0) const { return cp_offset(caps[2 * g + 1]); }
};

// a subject: the UTF-8 text and the code-point index shared by the matches made on it
struct Subject {
    std::shared_ptr<const std::string> s;
    std::shared_ptr<CpIndex> cpx;       // (may be none for an ASCII text)
    bool ascii = false;
};
// (ascii: the text is all ASCII, so a byte offset is its code-point offset and no table is needed)
std::shared_ptr<CpIndex> new_index(bool ascii = false);

class Regex {
public:
    Regex() {}
    explicit Regex(const std::string& pattern, int flags = 0);
    Match match(const std::string& s, size_t pos = 0, size_t endpos = std::string::npos) const;
    Match fullmatch(const std::string& s, size_t pos = 0, size_t endpos = std::string::npos) const;
    Match search(const std::string& s, size_t pos = 0, size_t endpos = std::string::npos) const;
    std::vector<Match> finditer(const std::string& s) const;
    // the same over a subject the caller keeps (no copy of the text; its cp index shared by every match)
    Match match_in(const Subject& sub, size_t pos = 0, size_t endpos = std::string::npos) const;
    Match fullmatch_in(const Subject& sub, size_t pos = 0, size_t endpos = std::string::npos) const;
    Match search_in(const Subject& sub, size_t pos = 0, size_t endpos = std::string::npos) const;
    std::vector<Match> finditer_in(const Subject& sub) const;
    std::vector<std::string> findall_in(const Subject& sub) const;
    std::vector<std::vector<std::string>> findall_groups_in(const Subject& sub) const;
    // The next match of the scanning finditer / findall / sub / split do: into m (reused from one match to the next, so the
    // scan allocates nothing per match), searching from pos; pos and adv move on past it. False when there are no more.
    bool next_in(Match& m, const Subject& sub, size_t& pos, bool& adv) const;
    // sub: *nsubs (when given) is the number of replacements made; 0 means the text is unchanged and the result is left empty
    std::string sub_in(const std::string& repl, const Subject& sub, int count = 0, int* nsubs = nullptr) const;
    std::string sub_fn_in(const std::function<std::string(const Match&)>& fn, const Subject& sub, int count = 0, int* nsubs = nullptr) const;
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
    std::shared_ptr<Prog> prog_;
    int ngroups_ = 0;
    int flags_ = 0;
    std::string pattern_;
    std::vector<std::pair<std::string, int>> names_;
    Match run(const Subject& sub, size_t pos, size_t endpos, int mode, bool must_advance = false) const;
    bool run_into(Match& out, const Subject& sub, size_t pos, size_t endpos, int mode, bool must_advance) const;
    friend struct Parser;
};

std::string escape(const std::string& s);                 // re.escape

// a pattern compiled once, wherever it is first used (the Python's module-level re.compile)
const Regex& rx(const std::string& pattern, int flags = 0);
const Regex& rx(const char* pattern, size_t n, int flags);     // (no string made to look it up)

}  // namespace re
