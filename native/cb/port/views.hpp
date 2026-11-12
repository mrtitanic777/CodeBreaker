// cb/port/views.hpp -- the passes that mend how the program's structures meet each other (the Python's Port methods
// anon_tags .. offset_views).
//
// The old compilers laid structures out by their bytes, and the programs used that: an anonymous structure given by value
// and also a header's member; a word that was a pointer in one place and a number in another; a file's own structure that
// is another's first bytes; globals one after another that were a structure's members in order; a link a macro read as a
// pointer. Each pass here finds one of those by the evidence in the program (the old layouts of its structures say where
// the bytes were) and says the one thing the old bytes meant in a form a compiler of today accepts.
//
// The passes read and write the program through a ViewContext: the files as the port has them now, the originals, the
// notes of the report and the few facts the passes leave for the later ones. The views a file has of another structure
// (offset_views) are worked out once for the whole program and kept in ViewCaches.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/port/bss.hpp"
#include "cb/port/c_source.hpp"
#include "cb/port/rewrite.hpp"

namespace cb::port::pre_ansi {

// A member of a file's own view of another structure: either the other's members it is (`names`, the ones that begin
// at its place and cover it), or a part of one of them (a pun: a byte of a word, a word over two bytes -- the type read
// at `offset` bytes into the other's member `base`).
struct ViewMember {
    bool pun = false;
    std::vector<std::string> names;
    std::string base;
    std::uint32_t offset = 0;
    std::string type;
    friend bool operator==(const ViewMember&, const ViewMember&) = default;
};

// struct f071f5_p { long v; int x; unsigned char c; } *p, given (struct f071f5_p *)s->type: a view of `other`, in
// `file`, each member of it the other's at its old place.
struct ViewOf {
    std::string other;
    std::map<std::string, ViewMember> mapping;
    std::string file;
    friend bool operator==(const ViewOf&, const ViewOf&) = default;
};
using ViewTable = OrderedMap<std::string, ViewOf>;   // by the view's structure ("struct f071f5_p"), in the order found

// What the passes work out once for the whole program (the program does not change in a way that matters to them
// between one file and the next).
struct ViewCaches {
    std::optional<ViewTable> views;
    std::set<std::string> weak;           // views whose functions have objects of the structure (a local `struct t x;`)
    std::set<std::string> fresh;          // views the casts alone gave
    std::optional<std::set<std::string>> copied;   // members copied one to another as they are
    std::optional<std::set<std::string>> ptr2;     // members and variables declared char **
    bool pass_done = false;               // pass_through_params has run
};

struct ViewContext {
    Files& files;                                   // the port's text of each file
    const Files& originals;                         // the sources as they were
    Notes& notes;                                   // the report's notes: why -> places
    Notes& mended;                                  // the notes of the mending (pass_through_params leaves its own)
    Aliases& aliases;                               // a name that became a place in another variable
    std::set<std::string>& view_vars;               // the variables that became members of a structure variable
    std::set<std::string>& view_headers;            // the headers of the structures those are
    std::vector<std::string>& view_wide;            // a view's pointer where the other has a number (the report)
    std::vector<std::string>& view_puns_refused;    // views a cast would give that read a byte of the other's word
    std::map<std::string, std::set<std::string>>& need_headers;   // file -> headers it must include
    OrderedMap<std::string, Fn>& fns;               // the functions of the program
    ParamTypes& param_types;                        // (function, parameter): the type the mending gave it
    const BssSizes& bss;                            // variable -> bytes, in the old linker's order (set from outside)
    ViewCaches& caches;
};

// struct { char kind, flags; } t; -- an anonymous structure given by value (a parameter) that a header's member also is:
// one type, by a tag the header's gets (two anonymous ones are two types).
void anon_tags(ViewContext& cx);
// struct pnode { ...; struct pnode *kid[2]; int pa; ... } -- kid[k] for k up to 2 (three children), pa never named: the
// word after an array, of its elements' old size, at the place its next element would be, is that element: the array
// one longer, the word gone.
void array_tails(ViewContext& cx);
// #define NEXT(s) (*(struct isym **)(s)) -- NEXT(s) = *pp: a structure's first word its chain's link (an int then, the
// width of a pointer): its first member that pointer now.
void link_words(ViewContext& cx);
// f(t, s, fn) int (*fn)(); ... (*fn)() -- and every call gives it a function of a pointer: int was a pointer's width on
// the old machine, not here; the parameter is a function of that pointer.
void fnptr_returns(ViewContext& cx);
// struct opt { char *pat; int o2; } here, struct flagdef { char *pat; intptr_t *var; } there, the old bytes of both
// alike and the one cast to the other: one structure under two names; the copy takes the other's types.
void cast_twins(ViewContext& cx);
// struct macro { int f0, f2, f4; char *body; } in a file of its own: another structure's bytes with ints where that one
// has pointers: its members of the other's types, its names kept.
void twin_structs(ViewContext& cx);
// int top = 20; char *v[20]; -- globals one after another that are a structure's members in order, the first's place
// taken: one variable of the structure, their names its members'.
void struct_views(ViewContext& cx);
// A table of structures with an int * member given (int *)&p: a word's place that held a pointer in some rows, a number
// in others: the member intptr_t *, the ints placed in it intptr_t.
void word_columns(ViewContext& cx);

// (char *)&g -- the place of the variable the old linker put after a char array is the array's end, as the program
// means it (its bytes as they were).
std::string bss_ends(ViewContext& cx, std::string text, std::string_view fname);
// The members the program copies from one to another as they are: x.val.i = y->val.i.
const std::set<std::string>& copied_members(ViewContext& cx);
// The members and variables the program declares char ** (a block's link, its first word).
const std::set<std::string>& ptr2_names(ViewContext& cx);
// The structure a variable of the text points to (struct S *v, S v[]).
std::optional<std::string> pointee_of(std::string_view text, std::string_view var);
// A pointer parameter a function only gives on (never through it, never a member of it) that every caller gives a pointer
// of another structure is that one's.
void pass_through_params(ViewContext& cx);
// A file's own view of another structure by its old places: each member the other's at its place (see ViewOf).
std::string offset_views(ViewContext& cx, std::string text, std::string_view fname);

}  // namespace cb::port::pre_ansi
