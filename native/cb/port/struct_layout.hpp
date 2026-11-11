// cb/port/struct_layout.hpp -- structures laid out as the 8086 compilers did.
//
// A word (a member of 2 or more bytes) sits on an even offset, a structure's size is rounded up to a word, a union's
// members all start at 0, and a structure or union nested in another is one member of its own size. The size of a
// pointer is the caller's: 2 bytes for the original's, 8 for the port's.
#pragma once

#include <span>
#include <string>

#include "cb/port/pre_ansi_types.hpp"

namespace cb::port::pre_ansi {

// The layouts of every structure and union the texts define (the first definition of a tag wins), for a near
// pointer of `ptr` bytes and a far pointer of `far_ptr`. A tag may use a structure defined later in the texts: the
// layouts are worked out again until the sizes settle (six rounds at most). The nested structures of a structure
// are entries too: `struct s.inner`. This is the LayoutFn the port uses unless a caller hands in another.
StructLayouts lay_out_structs(std::span<const std::string> texts, unsigned ptr, unsigned far_ptr);

}  // namespace cb::port::pre_ansi
