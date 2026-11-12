// cb/port/sanitize.hpp -- what the port does wrong only when it runs: found by AddressSanitizer on the verify cases, mended.
//
// An old program may write past the end of an array where its own linker put the next variable -- harmless there (what it
// overwrote it did not read again, or read as it meant to), memory of something else here. The compiler cannot see it; a
// run can. The port is built with -fsanitize=address, every verify case run, and each overflow the sanitizer reports
// becomes a rule of the port:
//
//   a global array written (or read) past its end   the array given room for what the original reached
//   a table read before its start (t[k - 0x101],    what the old linker had put there, by the program's data
//   k at the end of the input: -1)                  segment's places (SYMBOLS): that variable read instead
//   a read at a small address (a NULL, or a         the read made through the null segment, as DOS made it
//   small number that was a pointer)
//
//     Target target{.texts = ..., .ds = ..., ...};
//     for (const Mended& m : sanitize::mend(target, reports)) say(m.why);
//
// `Target` is the part of the port these rules read and write, by reference (the port owns the pieces; the sanitizer
// does not know the port's other parts). `mend` adds rules to it and says what it did.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/port/c_source.hpp"
#include "cb/port/pre_ansi_types.hpp"

namespace cb::port::sanitize {

using Files = pre_ansi::Files;

// One report of the sanitizer: the case that was running and what the sanitizer printed.
struct Report {
    std::string case_name;
    std::string text;
};

// What a mend did: the case (or the variable) it is about, and why.
struct Mended {
    std::string subject;
    std::string why;
};

// The port as the sanitizer's rules see it.
struct Target {
    // The port's text of each file (what a report's line numbers refer to), and the files before the passes wrote them
    // (the rules that look a definition up through #includes read these under the port's text).
    const Files& out;
    const Files& files;
    // Variables named by their place in the old data segment: their address.
    const OrderedMap<std::string, std::uint64_t>& ds;
    // The structures of a file's view as the old compiler laid them out.
    std::function<const pre_ansi::StructLayouts&(std::string_view file)> old_layouts_for;
    // The BSS list: variable -> its bytes.
    const std::map<std::string, std::uint64_t>& bss;
    // Whether an array's definition is followed in its file by one of the same type; and the BSS array made one with the
    // variable after it (true if it was). Either may be empty: that rule is then not tried.
    std::function<bool(const std::string& table)> joins_next;
    std::function<bool(const std::string& variable)> join_bss;

    // What the rules leave.
    std::map<std::string, std::set<std::string>>& extra_decls;     // file: declarations it now needs
    std::map<std::string, std::set<std::string>>& need_headers;    // file: headers it now needs
    std::map<std::string, std::map<std::size_t, pre_ansi::LineOrigin>>& line_origin;   // file: line (1-based): its origin
    pre_ansi::LineFixes& line_fixes;
    pre_ansi::Notes& mended;
    std::vector<pre_ansi::NullRead>& null_reads;
    std::map<std::string, std::uint64_t>& reach;                   // tables a run read past their ends: elements it reached
    std::map<std::string, std::uint64_t>& grown;                   // arrays given room this run: their elements now
    std::vector<pre_ansi::DeclRule>& decl_rules;
};

// {member: (offset, size)} and the size of a structure of today's C (x64: pointers 8, natural alignment) -- of its
// members' text (the port's: int16_t and the like); nullopt when a member's type is not known.
struct CLayout {
    std::map<std::string, std::pair<std::uint64_t, std::uint64_t>> members;
    std::uint64_t size = 0;
};
std::optional<CLayout> c_layout(std::string_view body);

// A read before a table's start: what the original read there (the variable its linker put there), as a rule for the
// line. `text` is the sanitizer's report (or one bounds-check line of it). Returns why, or nullopt.
std::optional<std::string> before_start(Target& target, std::string_view case_name, std::string_view text);

// A read at a small address (below 100h) on a line that reads p[i]->m: p's words small numbers at times -- on DOS a read
// of the null segment there; each p[...]-> of the file read through dos_np. Returns why, or nullopt.
std::optional<std::string> low_reads(Target& target, std::string_view case_name, std::string_view text);

// The rules from the sanitizer's reports; what was mended.
std::vector<Mended> mend(Target& target, const std::vector<Report>& reports);

// Where clang's sanitizer runtime is, beside the compiler (Windows: a DLL the program needs on its path); nullopt when the
// compiler's folder has none.
std::optional<std::filesystem::path> runtime_dir(const std::filesystem::path& compiler);

}  // namespace cb::port::sanitize
