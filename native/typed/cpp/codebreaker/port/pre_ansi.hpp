// codebreaker/port/pre_ansi.hpp -- typed C++ port of codebreaker.port.pre_ansi.
//
// pre_ansi.py -- C of before the standard made C a compiler of today takes, the meaning kept.
// (The module docstring is kept in pre_ansi.cpp.)
//
// TYPING NOTES (shared by the parts of this port)
//   * The port is split over several translation units (one per stretch of the Python file):
//       pre_ansi.cpp     lines 1-885 of the Python (rules, text helpers, the rewriting functions, Port.__init__)
//                        and the mend loop (4761-end)
//       pre_ansi_p2.cpp  Port methods 886-2145 (agree_globals ... _globals)
//       pre_ansi_p3.cpp  Port methods 2144-3077 (anon_tags ... offset_views)
//       pre_ansi_p4.cpp  Port methods 3077-3735 and the free functions 3739-4600
//     Each part declares its functions in its own section of this header.
//   * `definitions` returns (Fn, first line, '{' line, last line) with the Fn shared (std::shared_ptr<Fn>), as
//     Python's Fn objects are shared references.
//   * The Python Fn gets attributes at run time (file, ret_set, implicit, varargs, va_slots, int_ret, keep_int,
//     body ...): they are fields of Fn, with the defaults the Python's getattr(..., default) gave.
//   * The profile (a runtimes/*.json) is a record: Profile.
//   * notes / mended / union_notes are Python defaultdict(list): py::dict<str, list<str>>, an entry made on use.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include "py.hpp"
#include "py_os.hpp"

#include "codebreaker/port/asm16.hpp"

namespace codebreaker::port::pre_ansi {

// ---------------------------------------------------------------- shared constants and records

extern const py::set<py::str> KEYWORDS;
extern const py::set<py::str> STDINT;      // (the Python's frozenset)
extern const py::set<py::str> TYPEWORDS;
extern const py::set<py::str> PRINTF;
extern const py::set<py::str> RULES_OFF;   // (empty)

// the profile of the old runtime (runtimes/NAME.json): the keys the port reads
struct Profile {
    py::str name;
    py::list<py::str> libc;
    py::list<py::str> runtime_defines;
    py::list<py::str> runtime_headers;
    py::list<py::str> runtime_internals;
    py::str platform_header;                       // platform["header"]
    py::list<py::str> platform_calls;              // platform["calls"]
    py::dict<py::str, py::str> platform_renames;   // platform.get("renames", {})
    std::optional<int64_t> widths_pointer;         // widths.get("pointer")
    std::optional<int64_t> int_storage;
    bool printf_hex_upper = false;
    std::optional<int64_t> struct_align;
    std::optional<py::list<py::str>> keywords_removed;
    py::str arg_order = py::str("right");
    // reads a runtimes/*.json
    static Profile load(const py::str& path);
};

// a function of the program (Python's Fn, with the attributes later passes give it)
struct Fn {
    py::str name;
    py::str ret;
    py::list<std::tuple<py::str, py::str>> params;
    bool static_ = false;
    int64_t line = 0;
    bool void_ = false;
    bool loose = false;          // (calls it does not match: old-style kept)
    int64_t bare = 0;            // (bare returns given a value)
    // attributes the passes add (getattr defaults)
    bool varargs = false;
    bool va_slots = false;
    bool ret_set = false;
    bool int_ret = false;
    bool keep_int = false;
    py::set<py::str> implicit;   // (parameters it never declared: ints by default)
    std::optional<py::str> file;
    py::str body;

    Fn() = default;
    Fn(const py::str& name_, const py::str& ret_, const py::list<std::tuple<py::str, py::str>>& params_, bool static__,
       int64_t line_)
        : name(name_), ret(ret_), params(params_), static_(static__), line(line_) {}
    py::str proto(bool names = true) const;
};
using FnP = std::shared_ptr<Fn>;

// (Fn, first line, the line of its '{', its last line)
using Definition = std::tuple<FnP, int64_t, int64_t, int64_t>;
// (name, first line, the line of its '{', its last line)
using DefinitionAny = std::tuple<py::str, int64_t, int64_t, int64_t>;
// the byte spans (start, end) of a call's arguments
using Span = std::tuple<int64_t, int64_t>;
// notes: {why: [places]}
using Notes = py::dict<py::str, py::list<py::str>>;

// ---------------------------------------------------------------- PART 1: pre_ansi.cpp (lines 1-885)

bool rule(const py::str& name);
py::list<int64_t> depths(const py::list<py::str>& code_lines);
py::list<py::str> split_top(const py::str& s, const py::str& sep = py::str(","));
// 'register struct s *a, b[3]' -> (base type, [(name, its declarator)]) - or None
std::optional<std::tuple<py::str, py::list<std::tuple<py::str, py::str>>>> declarators(const py::str& decl);
std::optional<std::tuple<py::str, py::list<std::tuple<py::str, py::str>>>> declarators(const py::str& decl,
                                                                                         const py::set<py::str>& types);
py::str array_param(const py::str& t, const py::str& n);
py::list<Definition> definitions(const py::list<py::str>& lines, const py::list<py::str>& code);
std::tuple<py::str, py::str> widths(py::str text, py::str code, Notes& notes, const py::str& fname, bool int16 = false);
py::str formats(py::str text, const py::str& code, Notes& notes, const py::str& fname, bool int16 = false,
                bool hex_upper = false);

// the constants of lines 305-315 (patterns as text; GLOBAL_DECL / LOCAL_DECL take the variable by % as the Python)
extern py::dict<py::str, py::set<std::tuple<py::str, int64_t>>> POOLS;   // ({allocator: {(struct, old size)}})
extern const py::str ARGCAST;
extern const py::str ALLOC_PATTERN;      // (ALLOC, compiled in pre_ansi.cpp)
extern const py::str CASTCALL_PATTERN;
extern const py::list<py::str> MEMFNS;
extern const py::str GLOBAL_DECL;
extern const py::str LOCAL_DECL;
extern const py::re::Pattern ALLOC;
extern const py::re::Pattern CASTCALL;

bool used_as_number(const py::str& code, const py::str& var);
// the functions of a text, new style and then old: (name, first line, '{' line, last line)
py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> function_map(const py::str& text);
std::optional<std::tuple<py::str, int64_t, int64_t, int64_t>> enclosing(const py::str& text, int64_t ln);
bool declares_local(const py::str& text, const std::tuple<py::str, int64_t, int64_t, int64_t>& fn, const py::str& var);
py::str apply_local_retypes(const py::str& text, const py::str& fname,
                            const py::dict<std::tuple<py::str, py::str, py::str>, py::str>& retypes);
// int16_t a, tag, b;  ->  int16_t a, b; struct tag *tag;  (a re.sub replacement: one name of a list retyped)
struct split_decl {
    py::str name, typ;
    split_decl(const py::str& n, const py::str& t) : name(n), typ(t) {}
    py::str operator()(const py::re::Match& m) const;
    friend bool operator==(const split_decl& a, const split_decl& b) { return a.name == b.name && a.typ == b.typ; }
};
// olds: {global array: bytes it had}; elems: {global array: elements it had}
py::str mem_sizes(py::str text, const py::dict<py::str, int64_t>& olds, const py::dict<py::str, int64_t>& elems,
                  Notes& notes, const py::str& fname);
py::str header_offsets(py::str text, const asm16::Layouts& old, Notes& notes, const py::str& fname);
std::optional<py::str> prefix_member(const asm16::Layout* lay, int64_t n);
// allocs: {allocator: the argument places its size was given at}
py::str struct_sizes(py::str text, const asm16::Layouts& old, Notes& notes, const py::str& fname,
                     const py::dict<py::str, py::set<int64_t>>& allocs = py::dict<py::str, py::set<int64_t>>());
py::str old_declarations(const py::str& text, const py::str& code, const py::set<py::str>& known, Notes& notes,
                         const py::str& fname, const py::list<py::str>& platform = py::list<py::str>());
std::tuple<py::str, py::list<py::str>> header_data(const py::str& text, const py::str& code, const py::str& fname,
                                                   Notes& notes);

// ---------------------------------------------------------------- the Port (state: PART 1 owns the ones set in
// __init__; a part that needs another member declares it in its own block below, after looking that no other part
// has it; the type of a member is the type of what the Python keeps in it)

// ---- PART 3 types: a file's own view of another structure (offset_views). mapping: a member of the view ->
// either the other's members it is ("names"), or a "pun" (a part of a member of the other's, or a word over two
// of its bytes: base member, byte offset, C type)
struct ViewMap {
    bool pun = false;
    py::list<py::str> names;       // (not a pun: the other's members)
    py::str base;                  // (a pun: ("pun", base, off, ct))
    int64_t off = 0;
    py::str ct;
    friend bool operator==(const ViewMap& a, const ViewMap& b) {
        return a.pun == b.pun && a.names == b.names && a.base == b.base && a.off == b.off && a.ct == b.ct;
    }
};
// (y, mapping, n): the other structure, the mapping, the file the view is in
struct ViewEntry {
    py::str y;
    py::dict<py::str, ViewMap> mapping;
    py::str n;
    friend bool operator==(const ViewEntry& a, const ViewEntry& b) {
        return a.y == b.y && a.mapping == b.mapping && a.n == b.n;
    }
};

// ---- PART 4 type: a rule of sanitize.py's null-segment reads: (file, variable, type) or with a kind ("var",
// "assign", "mem"); kind absent = the plain 3-tuple
struct NpRule {
    py::str file, var, typ;
    std::optional<py::str> kind;
};

// (pattern, replacement) of Port.decl_rules: the replacement is a template text, or (mend) a split_decl callable.
// [F5; re.sub(rx, new, t) is apply_decl_rule(rule, t)]
struct DeclRule {
    py::str pattern;
    py::str repl;                          // (the template, when fn is not set)
    std::optional<split_decl> fn;
    DeclRule() = default;
    DeclRule(const py::str& p, const py::str& r) : pattern(p), repl(r) {}
    DeclRule(const py::str& p, const split_decl& f) : pattern(p), fn(f) {}
};
py::str apply_decl_rule(const DeclRule& rule, const py::str& text);

struct P4Native;   // (the new code's state for PART 4: pre_ansi_p4.cpp)

class Port {
  public:
    // ---- PART 1 members (set by __init__) ----
    Profile profile;
    Notes notes;                                              // (defaultdict(list))
    py::list<py::str> member_fixes;
    py::dict<py::str, int64_t> reach;                         // (tables a run read past their ends: sanitize.py)
    py::dict<std::tuple<py::str, py::str, py::str>, py::str> local_retypes;   // ((file, function, variable): type)
    py::dict<py::str, py::list<py::str>> need_headers;        // ({file: headers a mend made it need})
    py::dict<py::str, py::set<py::str>> extra_decls;          // ({file: declarations a mend made it need})
    py::dict<py::str, py::str> left_out;                      // (file: why)
    py::dict<py::str, py::str> originals;                     // (the source as it was)
    py::dict<py::str, py::str> files;
    py::dict<py::str, py::list<py::str>> union_notes;
    py::set<py::str> view_headers;
    py::dict<py::str, FnP> fns;
    py::list<std::tuple<py::str, py::str>> data;
    py::dict<py::str, py::list<py::str>> mended;
    py::dict<py::str, py::str> renames;
    py::list<DeclRule> decl_rules;                            // ((pattern, replacement) on every file; a replacement is text or a split_decl)
    bool int16 = false;
    py::dict<py::str, py::str> out;                           // (modernize's result)

    // ---- PART 2 members ----
    bool _agreed = false, _bounds_done = false, _stacks_done = false, _numbered_done = false;
    bool _size_tables_done = false, _pools_done = false, _local_copies_done = false, _byte_places_done = false;
    bool _shared_blocks_done = false, _word_copies_done = false, _fixed = false;
    int64_t _merged = 0;                                      // (False at first, then len(reach) + 1)
    py::list<py::str> agree_notes, long_notes, alloc_notes, table_notes, notes_word_copies, notes_shared;
    py::list<py::str> notes_byte_places, notes_local_copies, notes_pools, notes_size_tables;
    py::dict<py::str, int64_t> notes_init;                    // (a Counter: file -> how many far/near dropped; the
                                                              //  constructor's keyword removal fills it)
    py::dict<py::str, int64_t> ds;                            // (set from outside: variable named by its place -> address)
    py::set<py::str> old_records;                             // (set from outside)
    // ---- PART 3 members ----
    py::dict<py::str, std::tuple<py::str, py::str>> aliases;  // (a name that is an element of another: where, owner's)
    py::set<py::str> view_vars;
    py::list<py::str> view_puns_refused;
    py::list<py::str> notes_view_wide;
    std::optional<py::dict<py::str, ViewEntry>> _views;       // (hasattr(self, "_views"))
    py::set<py::str> _views_weak;
    py::set<py::str> _views_new;
    std::optional<py::set<py::str>> _copied;
    std::optional<py::set<py::str>> _ptr2;
    bool _pass_done = false;
    // ---- PART 4 members ----
    py::dict<py::str, py::str> asm_protos;                    // (the program's assembly routines: their C declarations) [F4]
    py::list<std::tuple<py::str, py::str>> asm_data;          // ((module, definition) of a module of data alone) [F4]
    // {file: {line number: (its text before the rules, n)}} [F4]
    py::dict<py::str, py::dict<int64_t, std::tuple<py::str, int64_t>>> line_origin;
    // (file, a line's text, n): the line mended; the Python's two-tuple key (file, text) is n == -1 [F4]
    py::dict<std::tuple<py::str, py::str, int64_t>, py::str> line_fixes;
    py::list<NpRule> np_rules;                                // (set from outside: sanitize.py) [F4]
    py::dict<py::str, int64_t> array_sizes;                   // (each array the program defines: its elements) [F4]
    py::dict<py::str, int64_t> asm_sizes;                     // (what the assembler's data placed; set from outside) [F4]
    // (the BSS plan, the original program's facts and decl_rules converted: the new code's state, native/cb/port; the shim
    //  in pre_ansi_p4.cpp makes it when first needed)
    std::shared_ptr<P4Native> p4_native_;
    // (caches of what old callers hold references to: the shim fills them from the new code's answers)
    std::optional<py::dict<py::str, py::set<int64_t>>> _allocs;
    std::optional<py::dict<py::str, int64_t>> _old_elems;
    std::optional<asm16::Layouts> _old;                       // (laid out by asm16: its layouts carry what old callers read)
    py::dict<py::str, asm16::Layouts> _lay_cache;             // (keyed by the include order, joined with "\n")
    // ---- PART 5 members ----
    py::dict<py::str, py::set<py::str>> fptr;                 // (a function pointer: the types of what it is given) [F5]
    py::set<py::str> fptr_done;                               // [F5]
    // (an int variable that holds a pointer: its type. The Python's key is the variable's name, or the tuple (file,
    //  function, variable) for a local: here a local's key is retyped_key(file, function, variable)) [F5]
    py::dict<py::str, py::str> retyped;
    py::set<std::tuple<py::str, int64_t>> given_params;       // ((function, index) given pointers by callers; getattr default set()) [F5]
    py::set<py::str> fptr_mixed;                              // (pointers given functions of several prototypes) [F5]
    // ---- SHARED ADDITIONS (members more than one part uses; add below, check first) ----
    py::dict<std::tuple<py::str, int64_t>, py::str> param_types;   // ((function, parameter index): its type, the loop's) [F3]
    py::dict<py::str, int64_t> bss;                                // (set from outside: variable -> its size; empty by default) [F3]
    py::dict<py::str, int64_t> grown;                              // (an array the sanitizer saw reached past its end: size; sanitize.py)

    // ---- PART 1 methods ----
    Port(const py::dict<py::str, py::str>& files, const Profile& profile,
         const py::dict<py::str, py::str>& members = py::dict<py::str, py::str>());
    std::optional<py::str> _runtime(const py::str& name, const py::str& text);
    // ---- PART 2 methods ----
    void agree_globals();
    bool joins_next(const py::str& var);
    void fixed_allocators();
    void array_bounds();
    void word_stacks();
    void merge_tables();
    void numbered_tables();
    void size_tables();
    void pool_blocks();
    void local_copies();
    void byte_places();
    void shared_blocks();
    void word_copies();
    py::dict<py::str, py::str> modernize();
    void loose_calls(py::dict<py::str, py::str>& out);
    // [(line, name, base, shape, initializer or None)] of the one-name definitions at a file's top
    py::list<std::tuple<int64_t, py::str, py::str, py::str, std::optional<py::str>>> _globals(const py::str& text);
    // ---- PART 3 methods ----
    void anon_tags();
    void array_tails();
    void link_words();
    void fnptr_returns();
    void cast_twins();
    void twin_structs();
    void struct_views();
    void word_columns();
    static py::str _widen(const py::re::Match& m, const py::str& v);
    py::str bss_ends(const py::str& t, const py::str& fname);
    const py::set<py::str>& copied_members();
    const py::set<py::str>& ptr2_names();
    std::optional<py::str> pointee_of(const py::str& text, const py::str& var);
    void pass_through_params();
    py::str offset_views(py::str t, const py::str& fname);
    // ---- PART 4 methods ----
    void bss_views();
    // the program's allocators: name -> the argument places their size was at (with the alloc_places rule off the
    // Python returns the names alone: here the same names with no places)
    const py::dict<py::str, py::set<int64_t>>& allocators();
    py::dict<py::str, std::tuple<py::str, py::str>> bss_decls();
    bool join_bss(const py::str& var);
    py::list<py::str> bss_definitions();
    const py::dict<py::str, int64_t>& old_elem_sizes();
    py::dict<py::str, int64_t> old_var_sizes();
    const asm16::Layouts& old_layouts();
    const asm16::Layouts& old_layouts_for(const py::str& fname);
    py::str finish(const py::str& fname, py::str text);
    py::str _preprocessor(const py::str& text, const py::str& fname);
    py::str _definitions(const py::str& text, const py::str& fname);
    // port.h, proto.h, data.c...: {name: text, or None}
    py::dict<py::str, std::optional<py::str>> headers();
    bool _same_def(const py::str& tag, const py::str& h1, const py::str& h2);
    // ---- PART 5 methods ----
};

// ---------------------------------------------------------------- PART 2 free declarations
// (modernize calls part 5's word_unions, word_copies, stdio_inits, file_internals, whole_word_stores, block_heads,
// agree_wide, numbered_places, old_c_ways: declared in PART 5's section)
// ---------------------------------------------------------------- PART 3 free declarations
// (owned by other parts, declared here for part 3's calls; no default arguments, the owners' declarations give them)
std::optional<py::list<Span>> call_args(const py::str& line, const py::str& fn, bool partial);          // F4
std::optional<py::str> widen_member(const py::str& text, const py::str& tag, const py::str& mem);       // F5
// ---------------------------------------------------------------- PART 4 free declarations
// the size a declaration had in the original (its pointers ptr bytes) - or None (fixed here: asm16 calls it)
std::optional<int64_t> size_of(const py::str& base, const py::str& decl, const py::str& name, int64_t ptr,
                               const asm16::Layouts& structs);
// {"struct S": {member: (offset, size), "@size": n}} of every structure (layouts(texts, ptr))
struct SimpleLayout {
    py::dict<py::str, std::tuple<int64_t, int64_t>> members;
    int64_t size = 0;
};
py::dict<py::str, SimpleLayout> layouts(const py::list<py::str>& texts, int64_t ptr = 2);
extern const py::dict<py::str, int64_t> SIZES;
extern const py::dict<py::str, int64_t> OLD_SCALARS;
py::str split_view_lists(py::str text);
// sizes: {struct: (old size, new size)}
py::str union_views(py::str text, Notes& notes, const py::str& fname,
                    const py::dict<py::str, std::tuple<int64_t, int64_t>>& sizes = py::dict<py::str, std::tuple<int64_t, int64_t>>());
// the bytes of a C string literal ("a\0b" "c"), as latin-1 text of one char a byte
py::bytes c_string_bytes(const py::str& lit);
void frames(Port& port, py::dict<py::str, py::str>& out);
// the argument spans of every call of g in a body
py::list<py::list<Span>> _calls(const py::str& body, const py::str& g);
py::list<DefinitionAny> definitions_any(const py::list<py::str>& lines, const py::list<py::str>& code);
// a local of a body's head: (base, declarator, size or None, line, register)
struct LocalDecl {
    py::str base, decl;
    std::optional<int64_t> size;
    int64_t line = 0;
    bool reg = false;
};
py::dict<py::str, LocalDecl> _locals(const py::list<py::str>& lines, const py::list<py::str>& code, int64_t j, int64_t k,
                                     int64_t ptr = 2);
// {member: [the structures that have it]}
py::dict<py::str, py::list<py::str>> members(const py::list<py::str>& texts);
int64_t object_before(const py::str& line, int64_t at);
py::str abstract(const py::str& decl, const py::str& name);
// [(start, end) of each argument] of the first call of fn on the line, or None
std::optional<py::list<Span>> call_args(const py::str& line, const py::str& fn, bool partial = false);
bool truthy(const py::str& body, const py::str& name);
// what the compilers say, by the kind of complaint (in the Python's order)
extern const py::list<std::tuple<py::str, py::list<py::str>>> SAYS;
extern const py::str CAST;
std::tuple<std::optional<py::str>, std::optional<py::re::Match>> said(const py::str& text);
// ---------------------------------------------------------------- PART 5 free declarations  [F5: pre_ansi_p5.cpp]
// the local variable key of Port.retyped (the Python's tuple (file, function, variable))
py::str retyped_key(const py::str& file, const py::str& function, const py::str& variable);
// (function, argument index, (start, end)) of the innermost call with an argument at col (0-based) - or None
using CallAt = std::tuple<py::str, int64_t, Span>;
std::optional<CallAt> call_at(const py::str& line, int64_t col);
// a type as a compiler wrote it, for a cast or a declaration of the port's (MSVC leaves out the word struct)
py::str ctype(const py::str& t, const py::set<py::str>& tags = py::set<py::str>());
py::str block_heads(py::str t, Notes& notes, const py::str& fname, const py::set<py::str>& ptr2);
py::str whole_word_stores(const py::str& t, Notes& notes, const py::str& fname);
py::str file_internals(py::str t, Notes& notes, const py::str& fname);
py::str stdio_inits(py::str t, Notes& notes, const py::str& fname);
// the free function word_copies (the Port's method of the same name is PART 2's)
py::str word_copies(py::str t, Notes& notes, const py::str& fname, const py::set<py::str>& copied);
py::str word_unions(const py::str& t, Notes& notes, const py::str& fname,
                    const py::set<py::str>& olds = py::set<py::str>());
void old_c_ways(py::dict<py::str, py::str>& out, Notes& notes);
void numbered_places(py::dict<py::str, py::str>& out, const py::list<py::str>& extra, Notes& notes);
void agree_wide(py::dict<py::str, py::str>& out, const py::list<py::str>& extra, Notes& notes);
std::optional<py::str> widen_member(const py::str& text, const py::str& tag, const py::str& mem);
bool stored_wide(Port& port, py::dict<py::str, py::str>& out);
bool passed_on(Port& port, py::dict<py::str, py::str>& out);
std::optional<py::str> union_member_for(const py::dict<py::str, py::str>& out, const py::str& union_,
                                        const py::str& typ);
py::set<py::str> visible_tags(const py::dict<py::str, py::str>& out, const py::str& f);
// a complaint of the compiler (port/__main__.py's diagnostics()): file, line, col, kind, text, the notes after it
struct Diag {
    py::str file;
    int64_t line = 0;
    std::optional<int64_t> col;            // (d.get("col"))
    py::str kind;
    py::str text;
    py::list<py::str> notes;               // (d.get("notes", []))
};
// one round of mending from the compiler's complaints; -> whether anything was changed
bool mend(Port& port, py::dict<py::str, py::str>& out, const py::list<Diag>& diags);

}  // namespace codebreaker::port::pre_ansi

// the functions of PART 5, over the new code (native/cb/port): defined inline there
#include "codebreaker/port/pre_ansi_p5.hpp"
