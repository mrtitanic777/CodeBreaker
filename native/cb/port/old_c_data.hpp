// cb/port/old_c_data.hpp -- the passes that carry what the old compilers' data layout allowed over to a program
// a compiler of today builds, and the steps that put a ported program together.
//
// A program written for the 8086 compilers counted on things no standard promised: a variable declared `int` in one
// file and `char *` in another was one variable (the linker did not mind); a table read past its end went on into
// the variable the linker had put after it; a pointer was two bytes, so a block of `32` bytes was a structure's
// size, a word of a structure was its first pointer, a number that was a place in the data segment named a table.
// Each pass here finds one of these habits in the program's files and rewrites it to what it meant, and leaves a
// line for the report (`ProgramState::lists`, `notes`). The structures' old sizes and layouts come from the
// OriginalProgram, the sources as the old compiler saw them.
//
// The passes work on a ProgramState (the program's files and what the passes know and learn about it) and are
// called once each, in the order Port::modernize gives. The steps after them (`rename_pascal_calls` ...
// `array_sizes_of`) are the parts of that pipeline that are rewriting in their own right.
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
#include "cb/port/original_program.hpp"

namespace cb::port::pre_ansi {

// {file: the declarations a pass made the file need} (written into the file, sorted, by `add_missing_includes`).
using ExtraDecls = std::map<std::string, std::set<std::string>>;

// One list per pass: the places it changed, as `file (what)`, in the order it changed them.
struct PassNotes {
    std::vector<std::string> agreed;          // agree_globals: a variable declared a pointer in another file
    std::vector<std::string> longs;           // agree_globals: a long defined as its two words
    std::vector<std::string> allocators;      // fixed_allocators
    std::vector<std::string> tables;          // merge_tables
    std::vector<std::string> word_copies;     // first_word_copies
    std::vector<std::string> shared_blocks;
    std::vector<std::string> byte_places;
    std::vector<std::string> local_copies;
    std::vector<std::string> pools;
    std::vector<std::string> size_tables;
};

struct ProgramState {
    explicit ProgramState(OriginalProgram& original_program) : original(original_program) {}

    OriginalProgram& original;                // the sources as the old compiler saw them: layouts, allocators
    Files files;                              // the program's files as the passes have them now
    Files originals;                          // the files as they were given (a name only the originals say)
    Notes notes;                              // why -> places, for the report
    PassNotes lists;
    ExtraDecls extra_decls;
    std::vector<DeclRule> decl_rules;         // rules the passes add; the final loop applies them to every file
    Aliases aliases;                          // a name that became a place in another variable
    OrderedMap<std::string, std::uint64_t> ds;   // variables named by their place in the old data segment: their address
    std::map<std::string, std::uint64_t> bss;    // the BSS list: variable -> its bytes (pool_blocks makes room)
    std::map<std::string, std::uint64_t> reach;  // tables a run read past their ends: elements it reached
    unsigned pointer_bytes = 2;               // the old pointer's size (the profile's)
};

// ---- the passes ------------------------------------------------------------------------------------------------------

// `extern unsigned g;` in one file, `extern char *g;` in another: a pointer in every file; the number that was the top
// of the segment (0xffff) is the top of memory now (UINTPTR_MAX); a long one file declares and another defines as
// its two words (`int g_0cb0 = 0; int g_0cb2 = 0;`) is the long. Rules agree_ptr_lists, agree_ptr_defs, agree_longs.
void agree_globals(ProgramState& program);

// Whether a table's definition is followed in its file by one of the same type: what an index past its end reached.
bool joins_next(const Files& files, std::string_view table);

// `char *pool_alloc() { ... malloc(36) ... }` whose callers cast it `(struct node *)pool_alloc()`: the program's own
// allocator of blocks of one old size, for structures: blocks of the structures' size now.
void fixed_allocators(ProgramState& program);

// `if (x >= g_16ec)`: x points into the table g_123c[200] and g_16ec is the variable the old linker put right after
// it (another type's): the comparison was with the table's end, `g_123c + 200`.
void array_bounds(ProgramState& program);

// `int *sp; ... sp[1] = out; fp = (int *)sp[2];`: words that held pointers as well as numbers (a stack of frames, a
// list of results) are as wide as a pointer, `intptr_t`, with every int pointer and array they are one with (given to
// each other, compared, passed on to a parameter).
void word_stacks(ProgramState& program);

// A table the program reads past its end at a constant place (`t[7]` of a `t[3]`) went on into the variables defined
// after it in its file: one table again, those names its elements (aliases).
void merge_tables(ProgramState& program);

// `f06988(f021ff, 0x3a20)`: a table's place in the old data segment given as its number: the table there by name
// (`g_3a20`); and an int table given where a table of pointers is wanted holds them: `intptr_t`.
void numbered_tables(ProgramState& program);

// `struct recfmt g_2096[] = { { "tknNdplbcga", 22 }, ... }`: a table of the old byte counts of a structure's first
// members, by kind: each the same part of today's structure (`offsetof` the member that began there, `sizeof` the whole).
void size_tables(ProgramState& program);

// A pooled allocator (`if (size > p->left) { ... malloc(p->size + 4) ... p->left = p->size; }`) and its table of pools:
// each pool's blocks of a size for the old pieces, today's pieces up to four times that: the blocks so many times the
// size.
void pool_blocks(ProgramState& program);

// A file's own `struct ins { int i0, i2; ... };` of a structure a header defines with pointers where the file has words
// (the old layout the same): the header's structure, the file's names of its members the header's.
void local_copies(ProgramState& program);

// `f00002(op, p) char *p; { p[0xa] = 2; *(int *)(p + 6) = 0; }` given a `struct opnd *` by every caller: the bytes by
// their old places: the members that were there.
void byte_places(ProgramState& program);

// `cl = f03f02(0x14, 0, 0)` for a `struct cell *cl` (20 bytes, struct fix's old size) whose cells are cast to
// `struct fix *` elsewhere: blocks for either, the larger of the two structures now.
void shared_blocks(ProgramState& program);

// `*(int *)s = *(int *)p;` with `s`, `p` structure pointers: the first word copied, its chain's link (two bytes
// then): the first member, as wide as it is now. (Port::word_copies; named for what it does.)
void first_word_copies(ProgramState& program);

// ---- the steps between them ------------------------------------------------------------------------------------------

// What `report` gathers: the notes the passes and the other parts of the port left, in the order the report lists them.
struct ReportInputs {
    const Notes& mended;                        // notes of the earlier rounds (the mending loop's)
    const PassNotes& lists;
    const std::vector<std::string>& view_wide;      // struct views: a view's pointer where the other has a number
    const std::vector<std::string>& view_puns_refused;
    const Notes& union_notes;
    const std::vector<std::string>& member_fixes;
    const OrderedMap<std::string, std::size_t>& far_near_dropped;   // file -> how many far / near / pascal words went
};
// The report's notes: each reason (a sentence) and the places it applies to.
Notes gather_notes(const ReportInputs& inputs);

// A call in capitals of a function the program defines in small letters (the linker made both capitals): the
// definition's name. `known` are the names that stand as they are; `renames` gets each new pair.
void rename_pascal_calls(const Files& files, const std::vector<std::string>& function_names,
                         const std::set<std::string, std::less<>>& known, std::map<std::string, std::string>& renames,
                         Notes& notes);

// `extern struct node *f();` in a caller where the definition left the return an old C int: the callers' declaration
// says what the function gives. `functions` is the program's, changed in place.
void declared_return_types(const Files& files, OrderedMap<std::string, Fn>& functions, Notes& notes);

// The headers every .c file needs for the structures it uses and did not include (the old build gave them some other
// way) and the declarations the passes made it need, written after its last #include. Returns {struct tag: the one
// header that defines it} (a tag two headers define is left out).
std::map<std::string, std::string> add_missing_includes(Files& out, const std::map<std::string, std::vector<std::string>>& need_headers,
                                                        const ExtraDecls& extra_decls, Notes& notes);

// The program's own allocators keep their blocks by a structure's old size (`{allocator: {(struct, old size)}}`): that
// size in the allocator's body becomes `sizeof(struct)`, and the header of the structure is included.
using AllocatorSizes = OrderedMap<std::string, std::set<std::pair<std::string, std::uint64_t>>>;
void allocator_block_sizes(Files& out, const OrderedMap<std::string, Fn>& functions, const AllocatorSizes& allocators,
                           const std::map<std::string, std::string>& header_of, Notes& notes);

// An integer given to an old-style definition kept (its calls unchecked): the argument made an `intptr_t`, which the
// old unprototyped call left half of in a 64-bit place.
void loose_calls(Files& out, const OrderedMap<std::string, Fn>& functions, const std::map<std::string, std::string>& renames,
                 Notes& notes);

// {array the program defines: its elements} of the texts, after the declaration rules have been applied to each: a
// `[n]` array and a `[] = { ... }` one (its rows counted). The first definition of a name counts.
OrderedMap<std::string, std::uint64_t> array_sizes_of(const Files& texts, const std::vector<DeclRule>& decl_rules);

// A header that is included twice (port.h and data.c include some of the program's headers too): guarded once.
void guard_headers(Files& out);

// ---- the one-name definitions at a file's top ------------------------------------------------------------------------

struct GlobalDefinition {
    std::size_t line = 0;                 // 0-based
    std::string name;
    std::string base;                     // the type: `unsigned int`, `struct s` (static left out)
    std::string shape;                    // the declarator with the name as `@`: `*@`, `@[8]`
    std::optional<std::string> initializer;
};
// The variables a file defines at its top, one name to a definition (`int a, b;` and the like are not listed).
std::vector<GlobalDefinition> global_definitions(std::string_view text);

}  // namespace cb::port::pre_ansi
