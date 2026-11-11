// codebreaker/exact/draft/forms.hpp -- typed C++ from codebreaker.exact.draft.forms.
//
// forms.py -- the forms of values and places, read back: registers folded into expressions, steps and stores
// joined, pointers and their widths, the library's macros, the frame's locals, and the variants a beam tries.
//
// Every pass is a text rewrite over a C-like draft (lines of text).  C++ spelling notes:
//  * a pass that Python gives back "the text, or None when it changes nothing" returns std::optional<py::str>;
//    one that gives a list of variants returns py::list<py::str>;
//  * the module-level tables (ARRAYS, STRUCTARR, BYTEPTR, FILEPTR, LONGINFO, ...) are the ones of exact/draft/text
//    (its typed port): shared handles, filled in place as Python's are;
//  * `prj` (an exact.project.Project) and the toolchain profile it carries are objects of modules with no C++ port
//    yet: what forms.py asks of them is the plain struct Prj below (filled by the caller).  Prj is shared by
//    candidates and prove, which read more of the project / profile than forms does.
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/exact/compare.hpp"
#include "codebreaker/exact/draft/text.hpp"
#include "py.hpp"

namespace codebreaker::exact::draft::forms {

// ---- what the draft modules ask of a project and its toolchain's profile (tc.p.get(..))

// "table_macros": [{"header": "ctype.h"}]
struct TableMacro {
    py::str header;
};
// "data_macros": [{"base": "__iob", "size": 8, "names": [..], "fields": {"7": "fileno(%s)"}, "include": "stdio.h"}]
struct DataMacro {
    py::str base;
    int64_t size = 0;
    py::list<py::str> names;
    py::dict<py::str, py::str> fields;
    std::optional<py::str> include;
};
// "macros": [{"call": routine, "as": "getc(%s)", "type": "FILE", "include": "stdio.h", "byte_args": [1]}]
struct LibMacro {
    py::str call;
    py::str as;                                    // "as"
    std::optional<py::str> type;                   // absent: "FILE" (mac.get("type", "FILE"))
    std::optional<py::str> include;                // mac.get("include")
    py::list<int64_t> byte_args;
};
// "frame_order": {"mod": M} -- the rule (a dict; its "mod" may be absent)
struct FrameRule {
    std::optional<int64_t> mod;
};

struct Prj {
    py::str dir;                                   // prj.dir
    py::str toolchain_name;                        // prj.toolchain().name
    std::optional<py::str> kit;                    // prj.toolchain().p["kit"]  (nullopt: the profile has none -> KeyError)
    std::function<py::str(const py::str&)> kit_dir;   // toolchain.kit_dir(name): the kit's folder (it throws when not set up)
    // library.locate(prj) read as forms.data_macros reads it: {name: address} of the library's data (it may throw)
    std::function<py::dict<py::str, int64_t>()> library_places;
    // profile entries
    std::optional<FrameRule> frame_order;          // p.get("frame_order") (None / empty: nullopt)
    py::list<TableMacro> table_macros;             // p.get("table_macros")  (None: empty)
    py::list<DataMacro> data_macros;               // p.get("data_macros")
    py::list<LibMacro> macros;                     // p.get("macros")
    py::list<py::str> fp_conversions;              // p.get("fp_conversions")
    py::dict<py::str, py::str> long_helpers;       // p.get("long_helpers")   routine -> C operator
    py::list<py::str> carve_register_variables;    // p.get("carve", {}).get("register_variables", ["si", "di"])
    std::optional<py::str> fp_switch, file_switches, probe_switch;   // p.get("fp_switch") ...
    std::optional<py::str> tc_switches;            // p.get("switches", "")  (nullopt: absent -> "")
    // project
    std::optional<py::str> switches;               // prj.switches
    py::list<py::str> include;                     // prj.include
    std::function<py::dict<py::str, py::bytes>()> headers;   // prj.headers()
    py::bytes image;                               // prj.image
    int64_t dgroup = 0;                            // prj.dgroup
    compare::Symbols symbols;                      // dict(prj.symbols())
    compare::Project cmp;                          // the members compare.function reads
    compare::Decoder decode;                       // the 16-bit decoder
    // (prj._dnames, made by candidates._data_names)
    mutable std::optional<py::dict<int64_t, py::str>> dnames;
    // tc.compile(source, headers=, include=, switches=, listing=True) -> (object, console, listing text)
    std::function<std::tuple<std::optional<py::bytes>, py::str, py::str>(const py::bytes&)> compile_listing;
};

// the locals renamed so that the compiler gives each the frame slot the program shows (rule: the profile's
// "frame_order": {"mod": M})
std::optional<py::str> frame_names(const py::str& text, const FrameRule& rule, bool regs_last = false);

// the pointers a function's names are, from those it declares
py::str pointer_flow(const py::str& text);

// the text with one of its int parameters a char (each, at most `most`)
py::list<py::str> char_param_variants(const py::str& text, int64_t most = 2);

// the text with its register pointers made int * - each, then all
py::list<py::str> pointer_widths(const py::str& text, int64_t most = 2);

// X--; then a test of X: the step in the test
std::optional<py::str> pre_steps(const py::str& text);

// a number a pointer is compared with, or given: the place of data there -> [(name, text)]
py::list<std::tuple<py::str, py::str>> address_variants(const py::str& text, int64_t dsize, int64_t most = 2,
                                                        int64_t lo = 0x100);

// (E << 1)->fN, N a place in the data: an array of words there indexed by E
py::str word_arrays(const py::str& text, int64_t lo = 0x40);

// X = X + n; then *X read once: *++X in its place
std::optional<py::str> pre_increments(const py::str& text);

// a header of the kit, as text ('' when the kit has none)
py::str header_text(const Prj& prj, const py::str& header);

// the macros of a header that read a table of the library, read back where the model has the table's place
py::str table_macros(const Prj& prj, py::str text, const py::list<TableMacro>& specs);

// the functions and macros a header of the kit declares
py::set<py::str> header_names(const Prj& prj, const py::str& header);

// a library's data a macro names, read back where the model has their places
py::str data_macros(const Prj& prj, py::str text, const py::list<DataMacro>& specs);

// the locals declared in the function's body that nothing in it names any more, left out
py::str unused_locals(const py::str& text);

// a library macro the compiler expanded in line, read back as the macro
py::str library_macros(py::str text, const py::list<LibMacro>& macros);

std::optional<py::str> and_returns(const py::str& text);
py::str word_constants(const py::str& text);
py::list<py::str> unsigned_globals(const py::str& text, int64_t most = 2);
std::optional<py::str> assign_in_tests(const py::str& text);
py::str char_params(const py::str& text);
py::str loop_gotos(py::str text);
std::optional<py::str> default_last(const py::str& text);
std::optional<py::str> empty_cases_first(const py::str& text);
std::optional<py::str> or_loops(const py::str& text);
std::optional<py::str> compare_results(const py::str& text);
std::optional<py::str> register_reads(const py::str& text);
std::optional<py::str> switch_breaks(const py::str& text);
std::optional<py::str> default_after(const py::str& text);
std::optional<py::str> push_tails(const py::str& text);
std::optional<py::str> split_ternaries(const py::str& text);
py::str store_reloads(const py::str& text);
py::str fn_pointers(py::str text);
py::str scaled_index(const py::str& text);
py::str shared_arms(const py::str& text);
std::optional<py::str> guard_returns(const py::str& text);
std::optional<py::str> guard_flat(const py::str& text);
std::optional<py::str> guard_shared(const py::str& text);
py::str bool_trees(const py::str& text);
std::optional<py::str> shared_tails(const py::str& text);
std::optional<py::str> scaled_places(const py::str& text);
std::optional<py::str> frame_more(const py::str& text);
std::optional<py::str> lead_filler(const py::str& text);
std::optional<py::str> register_copies(const py::str& text);
std::optional<py::str> unsigned_registers(const py::str& text);
std::optional<py::str> no_default(const py::str& text);
std::optional<py::str> unmasked(const py::str& text);
std::optional<py::str> high_words(const py::str& text);
std::optional<py::str> assign_in_args(const py::str& text);
std::optional<py::str> return_sources(const py::str& text);
std::optional<py::str> compound_assigns(const py::str& text);
std::optional<py::str> chain_assigns(const py::str& text);
py::str guard_gotos(const py::str& text);
py::str merge_conditions(const py::str& text);
std::optional<py::str> register_temps(const py::str& text);
py::str case_falls(const py::str& text);
std::optional<py::str> inner_registers(const py::str& text);
py::str char_stores(const py::str& text);
std::optional<py::str> store_increments(const py::str& text);
py::str register_values(const py::str& text);
py::str shapes(const py::str& text);

// t without x in its `int a, x, b;` declaration (the line gone if x was all of it)
py::str _drop_decl(const py::str& t, const py::str& x);

// (the toolchain gives a register local a frame slot): Python's one-element list [False]
extern const py::list<bool> REGSLOTS;

// t with x's declaration (`int a, x, b;` - a parameter's, a local's) as `new;`, or None
std::optional<py::str> _redeclare(const py::str& t, const py::str& x, const py::str& new_);

}  // namespace codebreaker::exact::draft::forms
