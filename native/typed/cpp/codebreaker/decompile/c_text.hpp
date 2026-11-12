// codebreaker/decompile/c_text.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/decompile/c_text*.{hpp,cpp}; this
// header keeps the old py:: surface for the ports and the dynamic exe that still use it: the records the decompiler and the lanes read (a
// module made compilable, its facts), and the functions that make one. Conversion and forwarding only.
//
// What the module asks of the decompiler (running the native program, writing a function's C from its L2 document) is the new decompiler,
// native/cb/decompile/decompiler*, and of the symbol tables the old ports (COMPAT(py): `OldEnvironment` in decompiler.hpp): the functions
// that need them are in c_text_impl.hpp, which this header ends by including through decompiler.hpp (the decompiler's own header ends by
// including it, so the functions that call it are defined after both).
//
// Not carried over (see cb/decompile/c_text_test.cpp): compilable16's result is not kept in the build cache (Python kept it pickled);
// `generation()` is the decompiler's own with the version of this port.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/c_text.hpp"
#include "cb/translate/backends/c.hpp"
#include "codebreaker/core16_types.hpp"
#include "codebreaker/decompile/struct_text.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/translate/native_l2.hpp"
#include "py.hpp"

namespace codebreaker::decompile::describe {
class Description;   // codebreaker/decompile/describe.hpp
}

namespace codebreaker::decompile::c_text {

namespace nc = ::cb::decompile::c_text;

using core16::FunctionNotes;
using core16::L2Doc;
using core16::SegOff;
using core16::StructNote;

// ---- constants ------------------------------------------------------------------------------------
// The options OpenWatcom's 32-bit compiler is given (decompiled C is stdcall by default: -ecd)
inline const py::list<py::str> CFLAGS = [] {
    py::list<py::str> out;
    for (const std::string_view f : nc::cflags()) out.append(py::str(std::string(f)));
    return out;
}();
// bytes of each of the decompiler's scalar types (backends/c.SIZEOF)
inline const py::dict<py::str, int64_t>& _SIZEOF() {
    static const py::dict<py::str, int64_t> d = [] {
        py::dict<py::str, int64_t> out;
        for (const auto& s : ::cb::translate::backends::c::scalar_sizes()) out.set(py::str(std::string(s.name)), static_cast<int64_t>(s.bytes));
        return out;
    }();
    return d;
}
// The C the 16-bit modules' headers start with (base types, helpers); the 32-bit one is _TYPES + _HELPERS
inline const py::str HEADER = py::str(std::string(nc::header16()));
// A software interrupt's register record: the registers' names in the record's order
inline const py::list<py::str> INTERRUPT_REGISTERS = [] {
    py::list<py::str> out;
    for (const std::string_view r : nc::interrupt_registers()) out.append(py::str(std::string(r)));
    return out;
}();
// `#define SEG_PTR(T, s) ...`
inline const py::str SEG_PTR = py::str(std::string(nc::seg_ptr()));
// The decompiler's names for instructions C has no word for, in a 16-bit module -> the C declaring each
inline const py::dict<py::str, py::str> INTRINSICS16 = [] {
    py::dict<py::str, py::str> out;
    for (const auto& [name, c] : nc::intrinsics16()) out.set(py::str(name), py::str(c));
    return out;
}();

// ---- the per-function facts a module says of a 16-bit function, and what writing it reported --------
// An interrupt a module takes: (vector, the registers its call sets). Ordered: the header sorts them.
struct Interrupt {
    int64_t vector = 0;
    py::list<py::str> registers;
    bool operator<(const Interrupt& o) const {
        if (vector != o.vector) return vector < o.vector;
        const int64_t n = std::min(registers.size(), o.registers.size());
        for (int64_t i = 0; i < n; ++i)
            if (registers[i] != o.registers[i]) return registers[i] < o.registers[i];
        return registers.size() < o.registers.size();
    }
    bool operator==(const Interrupt& o) const {
        if (vector != o.vector || registers.size() != o.registers.size()) return false;
        for (int64_t i = 0; i < registers.size(); ++i)
            if (registers[i] != o.registers[i]) return false;
        return true;
    }
};
using Interrupts = std::set<Interrupt>;

// How an imported value is shown: the made-up selector or offset of an import used for its value
// (describe.selector_imports: made-up value -> (import symbol, "segment" | "offset")).
using SelectorImports = py::dict<int64_t, std::tuple<py::str, py::str>>;

// A function's heading as the C back end reports one (found["heading"]): its parameters (the heading text, [(name, type)]),
// the types it names, its calls, its declaration with the argument list left open, and whether it is __fastcall16.
struct Heading {
    std::optional<std::tuple<py::str, py::list<std::tuple<py::str, py::str>>>> params;   // None when the heading text did not parse
    py::dict<py::str, py::str> types;
    py::list<std::tuple<py::str, py::list<py::str>>> calls;   // (callee, its arguments as written)
    py::str declaration;                   // `head();`
    bool fastcall = false;
};

// What a function's tree names, as the C back end reports it (found["names"], backends/c._names_used)
struct NamesUsed {
    py::set<py::str> called;               // functions and imports it calls
    py::set<py::str> called_through;       // globals it calls through (they hold function pointers)
    py::set<std::tuple<py::str, py::str>> globals;   // (prefix, 8 hex digits of the address) of `<prefix>Ram<addr>`
    py::set<int64_t> segments;             // the module's own segments used as values
    bool mk_fp = false;                    // it builds far pointers with MK_FP
};

// The module's facts handed to the C back end for ONE function (backends/c.function's `facts`), and what the back end
// leaves in it. The 16-bit ones are module_facts16() plus the module's later decisions; the 32-bit / PowerPC ones are
// compilable32()'s `written` dict.
struct ModuleFacts {
    bool win16 = false;                                 // the 16-bit keys (module_facts16) are given; else the 32-bit / PowerPC ones
    bool have_code = false;                             // 16-bit: the segment's bytes were known (widths, loads ... are then said)
    // --- given (16-bit: module_facts16 and _write16)
    std::optional<py::str> distance;                    // "far" | "near" | None
    std::optional<py::str> model;                       // "__stdcall16far" ... | None
    bool far_return = false;                            // it returns a far pointer
    std::optional<int64_t> selector;                    // the made-up selector of its code segment (describe.selector)
    py::dict<py::str, int64_t> widths_far;              // {a<n>: bytes the original reads through far pointer argument n}, as a far function
    py::dict<py::str, int64_t> widths_near;             //   ... as a near function
    py::set<int64_t> loads;                             // BP offsets of the argument words loaded as far pointers
    bool export_ = false;                               // starts with Windows' export prologue ("export")
    bool loadds = false;                                // loads its own data segment whoever calls it
    py::list<int64_t> ss_stores;                        // where it stores SS into its frame (_ss_store_places)
    bool selector_structs = false;                      // structures of selectors are recovered (CB_SELECTOR_STRUCTS)
    bool callee_pops = false;
    bool without_notes = false;                         // no notes above a function (the module's C)
    SelectorImports selector_values;                    // imports used for their value
    // --- given (32-bit / PowerPC: the module's decisions, c_text.compilable32 / compilable_powerpc)
    std::optional<py::str> lane;                        // "powerpc" for the PowerPC path, else none
    py::set<py::str> functions;                         // the module's functions
    py::dict<py::str, int64_t> struct_sizes;            // {structure name: bytes} recovered up to this function
    py::set<py::str> called_through;                    // globals the module calls through
    py::dict<py::str, py::str> aliases;                 // {global: alias used at this function's own width}
    py::set<py::str> pointer_globals;                   // globals holding pointers
    py::dict<py::str, int64_t> takes;                   // {function: arguments its definition takes}, not __cdecl ones
    std::optional<py::dict<py::str, struct_text::Planned>> struct_fields;   // (struct_text.plan) {param: (structure, {offset: (type, size)})}
    // --- left by the back end
    std::optional<py::str> declared;                    // 32-bit / PowerPC: its heading as written (`written["declared"]`)
    std::optional<py::str> registers;                   // "ecx" | "edx" | None: where its first argument(s) travel
    py::set<py::str> structs_used;                      // structures its text names
    bool novalue = false;                               // PowerPC: it leaves nothing in r3 worth reading
    Interrupts interrupts;                              // found["interrupts"]
    py::list<py::str> selector_decls;                   // found["selector_decls"]
    py::dict<py::str, py::dict<py::str, py::dict<int64_t, py::str>>> selector_shared;   // {structure: {function: {offset: type}}}
    py::dict<py::str, py::str> selector_imports;        // found["selector_imports"]: import -> "segment" | "offset"
    std::optional<NamesUsed> names;                     // found["names"]
    std::optional<Heading> heading;                     // found["heading"]
    // The keys only the C back end looks into and the module merely carries from one pass to the next (the plans: "frame",
    // "frame_slots", "segment_values", "code_places", "selector_renames", "code_reads", "struct_fields" ...). A Wire dict: the
    // plans' keys are not all str ("code_places" is keyed by node number).
    translate::native_l2::Wire more = translate::native_l2::Wire::dict();
    // the facts dict as the back end left it (set by decompiler::c_from_l2): backends/c.flags reads it
    std::optional<translate::native_l2::Wire> wire;
};

// ---- 32-bit ---------------------------------------------------------------------------------------
// compilable32()'s answer: (preamble, {name: text}, [names], {va: name}, [import names used], in_ecx, in_edx,
// {name: its L2 document}, {name: its declaration})
struct Compilable32 {
    py::str preamble;
    py::dict<py::str, py::str> texts;
    py::list<py::str> order;
    py::dict<int64_t, py::str> by_va;
    py::list<py::str> imports;
    py::set<py::str> in_ecx;
    py::set<py::str> in_edx;
    py::dict<py::str, L2Doc> docs;
    py::dict<py::str, py::str> declarations;   // each heading as the back end last wrote it, `;` ended
};

// A 32-bit module made (decompiler.decompile's Module): imports, in_ecx, in_edx, l2, declarations beside the Made
struct Module32 : translate::l2::Made {
    py::list<py::str> imports;
    py::set<py::str> in_ecx;
    py::set<py::str> in_edx;
    py::dict<py::str, L2Doc> l2;
    py::dict<py::str, py::str> declarations;
    using translate::l2::Made::Made;
};

// ---- 16-bit ---------------------------------------------------------------------------------------
// What _code_segment_data reports of a code segment: reads placed in its structure, named, why some were left
struct CodeSegmentReport {
    int64_t read = 0;
    int64_t named = 0;
    py::list<py::str> left;
    py::list<int64_t> placed;
};

// A 16-bit module made compilable (_compilable16's l2.Made with its attributes)
struct Module16 : translate::l2::Made {
    py::dict<py::str, FunctionNotes> notes;                    // what the decompiler said of each besides its C
    py::dict<int64_t, std::optional<SegOff>> where;            // function address -> (segment, offset)
    py::set<py::str> near;                                     // functions the original returns from near
    py::set<py::str> wide;                                     // functions with a wide (32-bit) result
    py::set<py::str> novalue;                                  // functions with no value
    Interrupts interrupts;                                     // (vector, registers) of the interrupts taken
    py::dict<py::str, py::str> selector_imports;               // import -> "segment" | "offset": imports used for their selector/offset
    py::set<py::str> exported_prologue;                        // far functions the original starts with Windows' prologue
    py::dict<py::str, py::str> failed;                         // {function: why the build must replace it}
    py::set<py::str> prototyped;                               // functions declared with parameters (backends/c.prototypes)
    py::dict<py::str, py::str> declarations;                   // each function's declaration, argument list left open
    std::shared_ptr<describe::Description> description;
    py::dict<py::str, L2Doc> l2;                               // each function's L2 document
    py::dict<int64_t, py::str> code_data;                      // {segment: the C defining its codebreaker_code_<n> structure}
    py::dict<int64_t, CodeSegmentReport> code_reads;
    py::dict<py::str, std::tuple<py::str, int64_t>> imports;   // {symbol: (module, ordinal)} of the imports called
    py::dict<int64_t, py::bytes> segments;                     // {segment: its bytes}, for data.c
    bool bios_data = false;                                    // reads the BIOS data through selector 0040h
    using translate::l2::Made::Made;
};

// ---- PowerPC --------------------------------------------------------------------------------------
struct ModulePPC : translate::l2::Made {
    py::set<py::str> novalue;                                  // names of functions that return nothing
    py::dict<py::str, int64_t> externs;                        // {name: address in the original} of the imports used
    py::dict<py::str, py::set<int64_t>> calls;                 // {callee: the numbers of arguments its calls pass}
    py::dict<py::str, int64_t> argc;                           // {extern: argument count}
    py::ref<formats::powerpc_image::Image> image;
    using translate::l2::Made::Made;
};

// ---- the plain forwards ----------------------------------------------------------------------------
namespace shim_detail {

inline nc::Interrupts new_interrupts(const Interrupts& in) {
    nc::Interrupts out;
    for (const Interrupt& i : in) out.insert({static_cast<std::uint32_t>(i.vector), ::cb::compat::from_py(i.registers)});
    return out;
}
inline Interrupt old_interrupt(const nc::Interrupt& i) {
    Interrupt out;
    out.vector = i.vector;
    out.registers = ::cb::compat::to_py(i.registers);
    return out;
}

}  // namespace shim_detail

// "int<vector hex 2>_<registers joined by _>" (the helper's name)
inline py::str interrupt_helper_name(int64_t vector, const py::list<py::str>& regs) {
    const std::vector<std::string> registers = ::cb::compat::from_py(regs);
    return ::cb::compat::to_py(nc::interrupt_helper_name(static_cast<std::uint32_t>(vector), registers));
}
// The header's part for the interrupts a module takes
inline py::list<py::str> interrupt_helpers(const Interrupts& used) {
    return ::cb::compat::guard([&] { return ::cb::compat::to_py(nc::interrupt_helpers(shim_detail::new_interrupts(used))); });
}
// Assembly for the routines the interrupt helpers call, and the C that defines codebreaker_int.
// `vectors` are the interrupt vectors taken. -> (assembly text, C text)
inline std::tuple<py::str, py::str> interrupt_routines(const py::set<int64_t>& vectors) {
    std::set<std::uint32_t> v;
    for (const int64_t x : vectors) v.insert(static_cast<std::uint32_t>(x));
    const nc::InterruptRoutines r = nc::interrupt_routines(v);
    return {py::str(r.assembly), py::str(r.c)};
}
// {name: number of arguments} for the functions in `names` that every call passes the same number of arguments to
inline py::dict<py::str, int64_t> call_counts(const py::dict<py::str, py::set<int64_t>>& calls, const py::set<py::str>& names) {
    std::map<std::string, std::set<std::int64_t>> counts;
    for (const auto& [callee, numbers] : calls.items()) {
        std::set<std::int64_t>& into = counts[callee.utf8()];
        for (const int64_t n : numbers) into.insert(n);
    }
    std::set<std::string> wanted;
    for (const py::str& n : names) wanted.insert(n.utf8());
    return ::cb::compat::to_py(nc::call_counts(counts, wanted));
}
// The bytes of a string as a C string literal, when they are text (else None) (used by tests)
inline std::optional<py::str> _string_literal(const py::bytes& chunk) {
    return ::cb::compat::to_py(nc::string_literal(::cb::compat::from_py(chunk)));
}
// Where the function stores SS into its frame, `mov [bp-0Ch], ss`: [the decompiler's offset of each, from the entry SP]
// (used by tests/translate/test_c_backend)
inline py::list<int64_t> _ss_store_places(const py::bytes& code, int64_t start, int64_t end) {
    return ::cb::compat::guard([&] { return ::cb::compat::to_py(nc::ss_store_places(::cb::compat::from_py(code), start, end)); });
}

// ---- what needs the decompiler (c_text_impl.hpp, defined once decompiler.hpp is) -------------------------------------
// The version of what makes a module compilable (compilable16): (the source stamp its keys carry, the decompiler's own
// generation: decompiler::generation())
inline std::tuple<py::str, std::tuple<int64_t, py::str, py::str>> generation();

// -> (preamble, {name: text}, [names], {va: name}, [import names used], in_ecx, in_edx, {name: its L2 document}, {name: its
// declaration}) for a whole-module build (see Compilable32)
inline Compilable32 compilable32(const py::str& module_path, const py::list<int64_t>& funcs, const py::str& outdir);
// The module made compilable (see _compilable16)
inline Module16 compilable16(const py::str& module_path, const std::optional<py::str>& outdir = std::nullopt);
// compilable16 without the cache  (used by tests/frozen_subjects)
inline Module16 _compilable16(const py::str& module_path, const std::optional<py::str>& outdir = std::nullopt);
// A PowerPC image decompiled and made compilable.
inline ModulePPC compilable_powerpc(const py::str& module_path, const py::str& outdir);
// What the module says of a 16-bit function that its C is written with (backends/c.function's `facts`). `code` is the
// segment's bytes (None: not known); `seg` the number of the function's segment.
inline ModuleFacts module_facts16(const std::optional<py::bytes>& code, int64_t off, int64_t end, const std::optional<py::str>& distance,
                                  const std::optional<py::str>& model, const std::optional<int64_t>& dgroup, bool far_return,
                                  const std::optional<int64_t>& seg = std::nullopt);
// The structures of global selectors (used by tests): -> (declarations, {function: {the module's name: its own}})
inline std::tuple<py::list<py::str>, py::dict<py::str, py::dict<py::str, py::str>>> _shared_selector_structs(
    const py::dict<py::str, py::dict<py::str, py::dict<int64_t, py::str>>>& shared);

}  // namespace codebreaker::decompile::c_text

// The decompiler's own header includes this one for the facts above and ends by including c_text_impl.hpp; whichever of the two a file
// includes first, the functions are defined once both are.
#include "codebreaker/decompile/decompiler.hpp"
