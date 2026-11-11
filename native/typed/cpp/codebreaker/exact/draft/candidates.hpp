// codebreaker/exact/draft/candidates.hpp -- typed C++ from codebreaker.exact.draft.candidates.
//
// candidates.py -- the model's text made the project's source: the engine's names, structures, longs, the frame's
// chars and buffers, the library's helpers, and every rewrite applied to make the candidates a compiler is given.
//
// C++ spelling notes:
//  * `prj` (an exact.project.Project and its toolchain profile) is the plain struct forms::Prj (see forms.hpp);
//  * `inv` (the model's checker: its MARK mode, recover and to_c) is the small interface Inv below: what byte_fields
//    asks of it, the C text of the code read in the checker's byte-marking mode (nullopt: it raised);
//  * `widths` - {(pointer, offset)} and, for the globals read a byte at a time, {("g", "g_xxxx")} - are the two sets
//    of ByteFields (only the first kind is ever looked up by an offset); BYTEPTR (text.hpp) takes the first;
//  * the module-level tables (FPINFO, STRUCTPTR, FRAMEW, FRAME, FILLNAME) are shared handles / a struct, as
//    text.hpp's: the other modules of the package (and prove) fill and read them.
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/exact/draft/forms.hpp"
#include "py.hpp"

namespace codebreaker::exact::draft::candidates {

using Widths = py::set<std::tuple<py::str, int64_t>>;

// byte_fields' result: the places read a byte of ("*" / "data" / a register's "bx*", "bxw" ..: (name, offset)) and the
// globals read a byte at a time (Python's ("g", "g_xxxx") members)
struct ByteFields {
    Widths fields;
    py::set<py::str> globals;
};

// the model's checker as byte_fields takes it
struct Inv {
    bool has_mark = false;   // hasattr(inv, "MARK")
    // inv.MARK = True; fs, params, stmts, pt, loc = inv.recover(code); inv.to_c("f", ..) -> the C text (nullopt: raised)
    std::function<std::optional<py::str>(const py::bytes& code)> checked_c;
};

// (the function's floating point: types, its value, any)
struct FpInfo {
    py::dict<py::str, py::str> types;      // "types": name -> type
    std::optional<py::str> ret;            // "ret"
    bool fp = false;                       // "fp"
    void clear() {
        types.clear();
        ret = std::nullopt;
        fp = false;
    }
};
extern FpInfo FPINFO;

extern const py::dict<py::str, py::str> STRUCTPTR;           // (data the draft declares a struct pointer: localize)
extern const py::set<std::tuple<int64_t, py::str>> FRAMEW;   // (the frame slots the code reads or writes, by width: (+-N, width))
extern const py::list<int64_t> FRAME;                        // Python's [0]
extern const py::list<py::str> FILLNAME;                     // Python's ["_pf"]

// the pointers the text goes through to fields declared pointers to a struct of those fields
py::str structs(py::str text, const Widths& widths);

// [(source, registers left)] - the model's text as the project's source, then the variants
py::list<std::tuple<py::str, py::list<py::str>>> candidates(const forms::Prj& prj, const py::str& text, int64_t at,
                                                            const py::dict<int64_t, py::str>& names,
                                                            const py::list<py::str>& regs,
                                                            const py::dict<py::str, int64_t>& regparam,
                                                            const ByteFields& widths = ByteFields());

// the fields the code reads or writes a byte of (fills FRAMEW and text::RANGES)
ByteFields byte_fields(const Inv& inv, const py::bytes& code);

py::str frame_chars(py::str text);
std::optional<py::str> buffers(py::str text);
py::str long_returns(const py::str& text);
py::str long_pairs(const py::str& text);
py::str long_ladders(const py::str& text);
py::str local_pointers(py::str text);
std::optional<py::str> void_variant(const py::str& text);
py::str register_variables(const py::str& text, const py::list<py::str>& regs, const py::dict<py::str, int64_t>& regparam);
std::optional<py::str> register_slots(const py::str& text);
py::list<py::str> unsigned_locals(const py::str& text, int64_t most = 3);
std::optional<py::str> registers_last(const py::str& text);
py::list<py::str> string_variants(const forms::Prj& prj, const py::str& text);

// the model's text with the engine's names and the declarations it needs -> (source, registers left)
std::tuple<py::str, py::list<py::str>> localize(const forms::Prj& prj, py::str text, int64_t at,
                                                const py::dict<int64_t, py::str>& names);

py::str long_helpers(const py::str& text, const py::dict<py::str, py::str>& helpers);
py::str fp_conversions(py::str text, const py::list<py::str>& routines);
py::str fp_types(const py::str& text);
py::str fill_frame(const py::str& text, int64_t fs, bool regs_count = true);

}  // namespace codebreaker::exact::draft::candidates
