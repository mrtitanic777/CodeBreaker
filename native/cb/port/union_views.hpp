// cb/port/union_views.hpp -- unions whose views meet at the same old places.
//
// The old programs read one structure through another by their shared old places: a union of structures such as
// `union { struct { char kind; int len; } a; struct { char *p; } b; }` where the view `a.len` and the view `b.p`
// were the same bytes. Their members have new sizes now (a pointer is 8 bytes), so the views no longer meet.
// union_views lays the views out again so that each old place is one new place in all of them (padding where a view
// has nothing there), and what one view writes another reads where it did.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <utility>

#include "cb/port/pre_ansi_types.hpp"

namespace cb::port::pre_ansi {

// In a union's structures: `char kind, flags;` -> one member a line (each can be given its own new place).
std::string split_view_lists(std::string_view text);

// The old and the new size of each structure (and union) tag: `sizes["struct inode"] = {32, 48}`.
using SizeChanges = std::map<std::string, std::pair<std::uint32_t, std::uint32_t>, std::less<>>;

// The text with its unions' views laid out again; `fname` is named in the note when one changed.
std::string union_views(std::string_view text, Notes& notes, std::string_view fname, const SizeChanges& sizes = {});

}  // namespace cb::port::pre_ansi
