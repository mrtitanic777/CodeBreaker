// cb/port/pre_ansi_types.hpp -- the records the port of pre-ANSI C shares (docs/CPP_DESIGN.md 3.5: a struct with
// named, typed members, not a tuple or a dict).
//
// The port rewrites C of before the standard (the old MS C compilers' own sources) into C a compiler of today takes,
// the meaning kept. These are its vocabulary: the runtime profile, a function of the program, the notes a pass leaves
// for the report, the rules that rewrite declarations, and the structures' layouts as the old compiler gave them.
#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/core/regex.hpp"

namespace cb::port::pre_ansi {

// ---- the profile of the old runtime (runtimes/NAME.json): the keys the port reads -------------------------------

struct Profile {
    std::string name;
    std::vector<std::string> libc;                  // the C library's names the program's own macros may not redefine
    std::vector<std::string> runtime_defines;
    std::vector<std::string> runtime_headers;       // the old library's headers (lower case): #include -> port.h
    std::vector<std::string> runtime_internals;     // BSS names that belong to the old runtime, not the program
    std::string platform_header;                    // the header of the platform's calls (included by port.h)
    std::vector<std::string> platform_calls;
    std::map<std::string, std::string> platform_renames;
    std::optional<unsigned> widths_pointer;         // the old pointer's width in bits (16 when the profile says nothing)
    std::optional<unsigned> int_storage;
    bool printf_hex_upper = false;
    std::optional<unsigned> struct_align;           // bytes: the old compiler's structure packing
    std::optional<std::vector<std::string>> keywords_removed;
    std::string arg_order = "right";

    // The old pointer's size in bytes (the profile's width, 16 bits when it gives none).
    unsigned old_pointer_bytes() const { return widths_pointer.value_or(16) / 8; }
    // Reads a runtimes/*.json; throws cb::IoError / cb::ParseError.
    static Profile load(const std::string& path);
};

// ---- a function of the program (the attributes the later passes give it have the defaults the first pass leaves) -

struct Param {
    std::string name;
    std::string type;
    friend bool operator==(const Param&, const Param&) = default;
};

struct Fn {
    std::string name;
    std::string ret;               // empty: no type said (an int, in old C)
    std::vector<Param> params;
    bool is_static = false;
    std::size_t line = 0;          // 1-based line of the definition
    bool returns_void = false;
    bool loose = false;            // calls it does not match: the old-style definition is kept
    std::size_t bare = 0;          // bare returns given a value
    bool varargs = false;
    bool va_slots = false;
    bool ret_set = false;
    bool int_ret = false;
    bool keep_int = false;
    std::set<std::string> implicit;   // parameters it never declared: ints by default
    std::optional<std::string> file;
    std::string body;

    // The prototype as it is written in proto.h (and at the head of a file for a static function), without the ';'.
    std::string proto() const;
};

// ---- notes: {why: [places]} in the order each reason first appeared -------------------------------------------

using Notes = OrderedMap<std::string, std::vector<std::string>>;
inline void note(Notes& notes, const std::string& why, std::string place) { notes[why].push_back(std::move(place)); }

// ---- the structures as the old compiler laid them out ------------------------------------------------------------

struct Member {
    std::string name;
    std::uint32_t offset = 0;
    std::uint32_t size = 0;
    friend bool operator==(const Member&, const Member&) = default;
};

struct StructLayout {
    std::vector<Member> members;   // in declaration order
    std::uint32_t size = 0;
    const Member* find(std::string_view name) const {
        for (const Member& m : members)
            if (m.name == name) return &m;
        return nullptr;
    }
    friend bool operator==(const StructLayout&, const StructLayout&) = default;
};

// "struct S" / "union U" (and the "struct S.member" of what is nested in them) -> its layout; in the order the
// program defines them.
using StructLayouts = OrderedMap<std::string, StructLayout>;

// Lays out the structures defined in `texts` for a pointer of `ptr` bytes and a far pointer of `far_ptr` (the
// assembler-data module's layout engine, handed in: this module does not own it).
using LayoutFn = std::function<StructLayouts(std::span<const std::string> texts, unsigned ptr, unsigned far_ptr)>;

// ---- declaration rules the passes add and the final loop applies to every file ------------------------------------

// int16_t a, tag, b;  ->  int16_t a, b; struct tag *tag;  (one name of a list of an `int a, b;` retyped)
struct SplitDecl {
    std::string name;
    std::string type;
    // `m` is a match of the rule's pattern: groups 1 (the lead), 2 (the base), 3 and 4 (the names before and after)
    std::string operator()(const RegexMatch& m) const;
    friend bool operator==(const SplitDecl&, const SplitDecl&) = default;
};

// (pattern, replacement): a regex and the template its matches become ($1.. and ${1}; a literal '$' is '$$'), or a
// SplitDecl. The pattern is compiled once, when the rule is made.
class DeclRule {
public:
    DeclRule(std::string pattern, std::string replacement);
    DeclRule(std::string pattern, SplitDecl split);
    const std::string& pattern() const { return regex_->pattern(); }
    const std::string& replacement() const { return replacement_; }
    const std::optional<SplitDecl>& split() const { return split_; }
    std::string apply(std::string_view text) const;

private:
    std::shared_ptr<const Regex> regex_;
    std::string replacement_;
    std::optional<SplitDecl> split_;
};

// A rule of sanitize.py's null-segment reads: variable `var` of `type` in `file` is read through the null segment,
// as the original did on DOS.
struct NullRead {
    enum class Kind { Plain, Var, Assign, Mem };
    std::string file, var, type;
    Kind kind = Kind::Plain;
};

// A line of a file as the rules left it, and which of its identical lines it is (two alike lines in one file are
// two places).
struct LineOrigin {
    std::string text;
    std::size_t nth = 0;
    friend bool operator==(const LineOrigin&, const LineOrigin&) = default;
};

// The lines the compiler loop mended: (file, a line's text before the rules, which of the alike lines) -> the line
// mended. `nth` empty: the rule is for every such line of the file.
struct LineFixKey {
    std::string file, text;
    std::optional<std::size_t> nth;
    friend auto operator<=>(const LineFixKey&, const LineFixKey&) = default;
};
using LineFixes = std::map<LineFixKey, std::string>;

}  // namespace cb::port::pre_ansi
