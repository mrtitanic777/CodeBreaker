// cb/port/old_c_scan.hpp -- the small scanners the port's data passes share (old_c_passes.hpp).
//
// The passes read C text whose strings and comments are blanked (cb::base::ctext::blank keeps every offset) and
// rewrite the original text at the offsets they find. What they have in common is here: reading a C integer,
// finding a call by name, the pointer variables of a file, applying a batch of edits, and a regex whose pattern
// holds a name only known at run time (compiled once per name).
#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/core/regex.hpp"

namespace cb::port::pre_ansi {

// A C integer constant as the passes read sizes and counts: `0x1f`, `31` or `010` (octal, as C reads it; one with a digit
// past 7 is not C and reads as decimal), the whole text (no sign, no suffix).
std::optional<std::uint64_t> c_integer(std::string_view text);

// The names of the files, in the order the program lists them.
std::vector<std::string> file_names(const OrderedMap<std::string, std::string>& files);

// The names of the files, sorted (the order the passes that sort their files visit them in).
std::vector<std::string> sorted_names(const OrderedMap<std::string, std::string>& files);

// lines[first..last] (both included; `last` is clamped to the last line) joined with '\n'.
std::string join_range(const std::vector<std::string>& lines, std::size_t first, std::size_t last);

// The number of '\n' before `offset`: the 0-based line the offset is on.
std::size_t line_of(std::string_view text, std::size_t offset);

// A type as one space-separated line with its stars close to the type and the first one apart: `char  * *` is
// `char **`. (The form the program's `extern char **g;` declarations are compared and rewritten in.)
std::string pointer_type_text(std::string_view type);

// {variable: the structure it points to} of every `struct s *v` / `union u *v` declared (or passed) in blanked code:
// `struct  s` is `struct s`; a variable declared twice keeps the last.
std::map<std::string, std::string, std::less<>> pointer_targets(std::string_view code);

// The calls of a name in blanked code: where the name stands, for each word `wanted` accepts that is followed
// by `(` (white space between allowed). A word that is part of a longer one, or (with `not_member`) one that
// follows `.` or `->`, is not a call.
struct CallSite {
    std::size_t at = 0;      // the name's first byte
    std::string_view name;
};
std::vector<CallSite> call_sites(std::string_view code, const std::function<bool(std::string_view)>& wanted,
                                 bool not_member = false);

// A replacement of [begin, end) by `text`.
struct Edit {
    std::size_t begin = 0, end = 0;
    std::string text;
    friend bool operator==(const Edit&, const Edit&) = default;
    friend auto operator<=>(const Edit&, const Edit&) = default;
};
// The edits applied from the last offset back, so each one's offsets are those of the text it was found in.
// Duplicates (the same edit found twice) are applied once when `unique` is set.
std::string apply_edits(std::string text, std::vector<Edit> edits, bool unique = false);

// A regex whose pattern has a name (or two) in it that only the program tells: `@1@` and `@2@` stand where they go
// and are replaced by the names, quoted. Each name is compiled once, when first asked for; make one per call of the
// pass that needs it (it is not thread-safe, and its cache is the pass's).
class NamedRegex {
public:
    explicit NamedRegex(std::string pattern, RegexFlags flags = RegexFlags::None);
    const Regex& with(std::string_view first, std::string_view second = {});

private:
    std::string pattern_;
    RegexFlags flags_;
    std::map<std::string, Regex, std::less<>> cache_;
};

}  // namespace cb::port::pre_ansi
