// cb/port/old_c_passes.hpp -- what the C of before the standard did that today's does not: the rewriting passes of
// the port's last stretch (the Python's pre_ansi PART 5, minus the mending loop in mend.hpp).
//
// The program the port reads was written for a 16-bit compiler on the 8086, where an int, a near pointer and the
// word a structure keeps were all 16 bits and so one could stand in for another. Today's pointers are wider than an
// int, so wherever the old code kept one in the other the port gives the place the width of either (intptr_t), and
// where the old library's insides were read (FILE's _file and _flag), or a string's text written to, or a block
// chained by its first word, it writes what today's C wants. Every pass is text in, text out, over C whose strings
// and comments are skipped; each records what it did in `Notes`, one line a place, so a person can see why a line
// is not what the original said.
#pragma once

#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/ordered_map.hpp"

namespace cb::port {

// Why a pass changed something, and where: {why: [places]}, in the order the reasons were first given.
using Notes = OrderedMap<std::string, std::vector<std::string>>;

// The program's files: name -> text, in the order the program lists them. An entry whose text is empty is a file
// the port has nothing to say about (the Python held None there).
using Sources = OrderedMap<std::string, std::string>;

// The port's rules, each by a name, on or off. A rule is on unless the spec turns it off: a comma-separated list
// of `+name` / `name` (on), `-name` (off), `+all` / `-all`, applied in order (CB_PORT_RULES: a regression's
// bisection).
class Rules {
public:
    explicit Rules(std::string_view spec = {});
    static Rules from_environment();
    bool on(std::string_view name) const;

private:
    std::vector<std::string> items_;
};

using Names = std::set<std::string, std::less<>>;

// (char *)p->base + 4: a block chained by its first word (char **), its bytes after a head of 4: the link is a
// pointer of today's, so the head is DOS_HEAD(4); the block's allocation (malloc(size + 4)) and the first one's place
// (g + 4 beside (char **)g) likewise. `ptr2` are the names of members that hold such a link.
std::string block_heads(std::string_view text, Notes& notes, std::string_view fname, const Names& ptr2);

// *(int *)p = v; with v an intptr_t (the callers give it pointers): the word it stored was either, so it is stored
// whole.
std::string whole_word_stores(std::string_view text, Notes& notes, std::string_view fname);

// fp->_file, fp->_flag read: the old library's FILE opened up (today's keeps its own to itself). fileno(fp), and the
// old flags of end of file (10h) and error (20h) from feof and ferror.
std::string file_internals(std::string_view text, Notes& notes, std::string_view fname);

// FILE *f = stdout; at the top of a file: a constant to the old C, a call of the library's now. The variable is
// set by a routine run before main (DOS_INIT).
std::string stdio_inits(std::string_view text, Notes& notes, std::string_view fname);

// union tokval { int16_t i; char *s; ... } and x.val.i = y.val.i somewhere: the int member the word was copied by
// carried a pointer as well (16 bits all then), so it is an intptr_t and carries the whole of either. `copied` are
// the members that are copied so.
std::string word_copies(std::string_view text, Notes& notes, std::string_view fname, const Names& copied);

// union { int16_t val; char *name; }: a word that was a number or a pointer (16 bits both) gets the number as wide
// as the pointer, so either is read whole through either. `olds` are tags of unions that keep their old layout.
std::string word_unions(std::string_view text, Notes& notes, std::string_view fname, const Names& olds = {});

// What old C allowed that today's does not, line by line: a handle kept in a word over two bytes and read as a
// pointer (dos_hp); char *g = "xLIBCE"; *g = 'S'; (a string's text written to: an array of its own, writable);
// time(&t) with t a long of 4 bytes (today's time_t is 8: the time given back, made the long). Edits the files of
// `out` in place.
void old_c_ways(Sources& out, Notes& notes);

// f06988(f021ff, (struct hsym **)0x3a20): a variable's place in the old data segment given as its number: the
// variable there, g_3a20, by name (its extern declared where it is not). `extra` are further definitions (the
// program's data) the variables may be found in.
void numbered_places(Sources& out, const std::vector<std::string>& extra, Notes& notes);

// extern intptr_t g_74be; in some files (a pointer held there) and extern int16_t g_74be in another: one variable,
// one width, intptr_t in every file (a file that wrote 16 bits where the others read 64). With the rule
// agree_wide_defs the definitions and the tables' places are made so too.
void agree_wide(Sources& out, const std::vector<std::string>& extra, Notes& notes, const Rules& rules);

// The text with struct `tag`'s integer member `mem` made an intptr_t (one of a list, int s4, s6, s8;, taken out of
// it to a declaration of its own); nullopt when it is not there, is a pointer's handle, or is not a plain integer.
std::optional<std::string> widen_member(std::string_view text, std::string_view tag, std::string_view mem);

} // namespace cb::port
