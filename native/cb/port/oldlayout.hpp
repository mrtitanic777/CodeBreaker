// cb/port/oldlayout.hpp -- the records a program keeps as bytes, kept in their old layout.
//
// A compiler of the 8086 writes its records to its own files as they lie in memory: `p = (char *)&e->e_v`, then the
// bytes, so many of them; it reads them back the same way and goes on. On the 8086 a pointer in such a record was two
// bytes, and the bytes of one member were the bytes of another (a union: a long here, two pointers there). Laid out
// anew - pointers of eight bytes - the counts no longer fit and the overlaps are gone.
//
// So those records keep the layout they had: the program's own ints 16 bits, its longs 32, its pointers two bytes - a
// handle (dos_ph) that stands for the pointer, turned back into it (dos_hp) wherever the program reads the member. A
// record is one of them when the program takes it as bytes - `(char *)&x`, `(char *)&x.member`, `(char *)p` of a pointer
// to it, or a pointer to it given to read or write. Every read and write of a pointer member is found in clang's tree of
// the port (-ast-dump=json, read by arith16 into typed nodes) and given the conversion:
//
//   r = e->e_sym          r = ((struct sym *)dos_hp(e->e_sym))
//   e->e_sym = s          e->e_sym = dos_ph(s)
//
// and the members are declared uint16_t in the record. A value of the program's that is no address - a symbol's
// number put where its pointer was - stays the number (dos_ph keeps what is below 8000h as it is).
//
//     RecordSet olds = byte_records(tree);                              // the records taken as bytes
//     Analysis a = analyse(tree, text, tree.find_file("a.c"), olds);    // what to insert, and what was found
//     std::string converted = apply(text, a.splices);
//     converted = declare(converted, olds, a.found);                    // the definitions: uint16_t members
//     converted = null_reads(converted);
//
// run() does that for the files of a port and keeps each file's results in a cache by everything they depend on.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/base/cache.hpp"
#include "cb/port/arith16.hpp"

namespace cb::port::oldlayout {

// The records the program keeps old, as the tag names them: "struct expr", "union val".
using RecordSet = std::set<std::string>;

// Whether each parameter of an old-style function (one the port keeps declared the old way) is a pointer, by the
// function's name. A handle given to such a parameter is turned back into the pointer it stands for.
using PointerParams = std::map<std::string, std::vector<bool>>;

// A member of a record.
struct Member {
    std::string record;   // "struct expr"
    std::string name;
    friend auto operator<=>(const Member&, const Member&) = default;
};

// Every pointer member found in use, with its type ("struct sym *").
using Members = std::map<Member, std::string>;

// Text to put at a byte offset of the file. A conversion wraps an expression: an opening at its start and a closing at
// its end, and wrappers of one expression nest by depth (how deep in the tree the wrapped expression is).
struct Splice {
    std::uint64_t at = 0;
    bool closing = false;
    std::int32_t depth = 0;
    std::string text;
    friend bool operator==(const Splice&, const Splice&) = default;
};

// A use of a pointer member that is not converted: its address taken, or changed in place (p->next++, &p->next).
struct Refusal {
    std::uint64_t at = 0;   // where the member is used, in the file
    std::string op;   // the operator that takes it, as C spells it: "&", "++", "--", "+="
    friend bool operator==(const Refusal&, const Refusal&) = default;
};

// ---- the records ------------------------------------------------------------------------------------------------

// The tag of a type that is a record or a pointer to one: "struct expr *" and "const struct expr" are both
// "struct expr"; an unnamed record, a pointer to a pointer, and any other type give "".
std::string record_tag(std::string_view type);

// The named records the program takes as bytes: (char *)&x, (char *)&x.m, (char *)p, and a record pointer given to
// read / write / fread / fwrite (or their dos_ forms).
RecordSet byte_records(const arith16::Tree& tree);

// ---- the conversions --------------------------------------------------------------------------------------------

struct Analysis {
    std::vector<Splice> splices;   // every read and write of a pointer member of the records kept old
    Members found;
    std::vector<Refusal> refused;
};

// The conversions of the file `main` of the tree (its text is `text`: the tree's offsets are bytes of it).
Analysis analyse(const arith16::Tree& tree, std::string_view text, arith16::FileId main, const RecordSet& olds,
                 const PointerParams& pointer_params = {});

// The text with the insertions made: at one place the outer ones' openings come before the inner ones', and the
// inner ones' closings before the outer ones'. An offset past the end of the text is an error (UsageError).
std::string apply(std::string_view text, std::span<const Splice> splices);

// ((struct n *)dos_hp(h))->op, and l = ((struct n *)dos_hp(h)); ... l->op with l never tested: a read through what
// may be NULL - on DOS the start of the data segment (no fault) - is dos_hpr, the null segment for handle 0; a
// pointer the function tests (if (!l), l == 0, l && ...) keeps dos_hp, its NULL a NULL.
std::string null_reads(std::string_view text);

// The pointer members of the records kept old declared uint16_t (a handle), in their definitions; an int the port
// made as wide as a pointer is one word again.
std::string declare(std::string_view text, const RecordSet& olds, const Members& found);

// Whether a file of `folder`, or a header of the program's it includes, names one of the tags (as a word).
bool mentions(const std::filesystem::path& folder, std::string_view file, std::span<const std::string> tags);

// ---- the port ---------------------------------------------------------------------------------------------------

inline constexpr std::string_view kCacheKind = "port-oldlayout";
// Change it whenever the results for a file would change.
inline constexpr std::string_view kCodeVersion = "oldlayout-1";

struct Conversions {
    std::map<std::string, std::string> texts;   // for each file that needed conversions: its text with them
    RecordSet olds;
    Members found;
    struct FileRefusal {
        std::string file;
        std::uint64_t at = 0;
        std::string op;
    };
    std::vector<FileRefusal> refused;
};

// For the C files of the port (clang reads each one): the records taken as bytes, and the conversions of the files
// that name them. Nothing found: an empty Conversions. A file clang can read none of is left out. Throws ToolError when
// clang cannot be started.
Conversions run(base::cache::Cache& cache, const std::filesystem::path& clang, const std::filesystem::path& folder,
           std::span<const std::string> files, const PointerParams& pointer_params = {});

}  // namespace cb::port::oldlayout
