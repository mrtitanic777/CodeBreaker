// cb/port/old_c_text.hpp -- the text passes of the port that rewrite one file at a time (the Python's pre_ansi lines
// 230-735): the widths of the old types, the formats of printf, the sizes and offsets a program gave as the numbers
// they were, the old `extern f();` declarations, and the shared variables of a header.
//
// Each takes a file's text and returns the new text; the places it changed go to the report's notes as "file:line".
// None looks inside a string or a comment: they search the code (cb::base::ctext::blank keeps every offset) and edit
// the original at the offsets they find.
#pragma once

#include <cstdint>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/port/old_c_data.hpp"
#include "cb/port/original_program.hpp"
#include "cb/port/pre_ansi_types.hpp"

namespace cb::port::pre_ansi {

// `long` is int32_t everywhere (what it was); in a structure, and in a pointer cast that reads the bytes of a buffer, int
// and unsigned are int16_t and uint16_t (the layout the bytes have); with `int16` every int and unsigned is (the arithmetic
// that rotates a word, or counts on its wrapping round, as it was; main's own line is the system's).
std::string widths(std::string text, Notes& notes, std::string_view fname, bool int16);

// The argument of a %ld %lx %lu of printf's family (an int32_t now, or a long of the C library's, as ftell's) is given as
// (long): what the format says, on every system. Microsoft's %Fs and %Np are %s and %p (one flat memory), and with
// `hex_upper` the old printf's %x is %X.
std::string formats(std::string text, Notes& notes, std::string_view fname, bool int16, bool hex_upper);

// memset(g_seg_assume, 0, 24): the bytes of a global array given as the number it had are sizeof it now. `olds` are the
// arrays' old sizes, `elems` the bytes of one element of each.
std::string mem_sizes(std::string text, const OrderedMap<std::string, std::uint64_t>& olds,
                      const OrderedMap<std::string, std::uint64_t>& elems, Notes& notes, std::string_view fname);

// p = alloc(n + 8); p[i + 8] = c; b = (struct blk *)p -- a structure with its bytes after a head (a trailing array): the
// head's old length as the number it was is offsetof(struct blk, its array) now.
std::string header_offsets(std::string text, const StructLayouts& old, Notes& notes, std::string_view fname);

// The member a structure's old layout has at the offset `n` (not its start, not its end): where a block of n bytes, a
// prefix of the structure, ended then.
std::optional<std::string> prefix_member(const StructLayout& layout, std::uint64_t n);

// (AllocatorSizes, the sizes the program's own allocators kept their blocks by, is old_c_data.hpp's.)

// (struct S *)malloc(n + 29): the recovered source gave a structure's size as the number it was (the bytes the same); now it
// is sizeof(struct S) and the rest, since a structure of today's layout is longer. `allocs` are the program's allocators
// and the argument places their size was given at. An allocator of the program's own that was given a structure's whole old
// size is added to `pools`.
std::string struct_sizes(std::string text, const StructLayouts& old, Notes& notes, std::string_view fname,
                         const Allocators& allocs, AllocatorSizes& pools);

// `extern T f(), g();` lines of functions the program defines or the C library has are gone (proto.h and the library's
// headers declare them); a variable on the same line is kept -- unless the platform layer declares it (what the old
// runtime's assembler defined). `known` are the names the program and its library define.
std::string old_declarations(std::string_view text, const std::set<std::string, std::less<>>& known, Notes& notes,
                             std::string_view fname, const std::vector<std::string>& platform);

// A header's variables without extern (each file that included it had them; the linker made them one) are declared
// extern here; the definitions are for data.c.
struct HeaderData {
    std::string text;
    std::vector<std::string> definitions;
};
HeaderData header_data(std::string_view text, std::string_view fname, Notes& notes);

}  // namespace cb::port::pre_ansi
