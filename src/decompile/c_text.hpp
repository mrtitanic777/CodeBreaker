// decompile/c_text.hpp -- make the decompiler's output into C that a compiler accepts and that means what the original meant.
//
//     Compilable32 m = compilable32(services, module, functions, outdir);       a 32-bit module: its preamble, each function's C
//     Module16     m = compilable16(services, module, outdir);                  a 16-bit module: one L2 module made, its C per
//                                                                               function, and what 16-bit code needs said
//     ModulePPC    m = compilable_powerpc(services, module, outdir);            a PowerPC image: the same, with its imports
//
// The decompiler writes each function; what is left for a module to decide is what no single function can: the types of the
// globals the functions share, the structures recovered, which functions are called through a pointer, the selectors a 16-bit
// module's code uses as values, the data a function reads through CS from beside its code, the software interrupts it takes,
// and every declaration the C of the whole needs ahead of it. Those decisions are made here, from what each function was found
// to use (the C back end's report of it) and from the binary.
//
// What this module asks of the rest of CodeBreaker -- the decompiler proper (running the native program, writing a function's C
// from its L2 document), the symbol tables, structure recovery, the PowerPC reader -- it asks through `Services`; the C
// back end's module planners are asked through the L2 library Services hands out. A test gives it recorded answers.
//
// The facts a function is written with and what the back end reports of it are `ModuleFacts`; the plans the back end hands the
// module and the module hands back (a frame plan, the segments' values, the places in a code segment) are the library's own
// documents and cross as JSON text in `plans`.
#pragma once

// <windows.h> defines `near` and `far` as empty macros; this header uses them as names, whatever was included before it.
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "core/arena.hpp"
#include "core/bytes.hpp"
#include "core/ordered_map.hpp"
#include "core16_types.hpp"
#include "decompile/describe.hpp"
#include "decompile/struct_text.hpp"
#include "translate/ir.hpp"
#include "translate/l2.hpp"
#include "translate/native_l2.hpp"

namespace cb::decompile::c_text {

using core16::FunctionNotes;
using core16::SegOff;
using core16::StructNote;
using Va = std::int64_t;

// ---- constants --------------------------------------------------------------------------------------------------------

// The options OpenWatcom's 32-bit compiler is given (decompiled C is stdcall by default: -ecd).
std::span<const std::string_view> cflags();
// The C the 16-bit modules' headers start with (base types, helpers); the 32-bit one is the types and helpers below.
std::string_view header16();
// `#define SEG_PTR(T, s) ...`
std::string_view seg_ptr();
// The registers of a software interrupt's record, in the record's order.
std::span<const std::string_view> interrupt_registers();
// The decompiler's names for instructions C has no word for, in a 16-bit module, and the C declaring each.
const OrderedMap<std::string, std::string>& intrinsics16();

// ---- what a module says of a function and what writing it reported -----------------------------------------------------

// An interrupt a module takes: the vector and the registers its call sets. Ordered: the header sorts them.
struct Interrupt {
    std::uint32_t vector = 0;
    std::vector<std::string> registers;
    friend auto operator<=>(const Interrupt&, const Interrupt&) = default;
};
using Interrupts = std::set<Interrupt>;

// The imports a 16-bit module's code uses for their value (describe's selector imports: made-up value -> import, how).
using SelectorImports = std::map<Va, describe::SelectorImport>;

// A function's heading as the C back end reports one: its parameters (the heading text, [(name, type)]) when it parsed, the
// types it names, its calls (callee, arguments as written), its declaration with the argument list left open, and whether it is
// __fastcall16.
struct Heading {
    struct Params {
        std::string text;
        std::vector<std::pair<std::string, std::string>> list;
    };
    std::optional<Params> params;
    OrderedMap<std::string, std::string> types;
    std::vector<std::pair<std::string, std::vector<std::string>>> calls;
    std::string declaration;   // `head();`
    bool fastcall = false;
};

// What a function's tree names, as the C back end reports it.
struct NamesUsed {
    std::set<std::string> called;           // functions and imports it calls
    std::set<std::string> called_through;   // globals it calls through (they hold function pointers)
    std::set<std::pair<std::string, std::string>> globals;   // (prefix, 8 hex digits of the address) of `<prefix>Ram<addr>`
    std::set<Va> segments;                  // the module's own segments used as values
    bool mk_fp = false;                     // it builds far pointers with MK_FP
};

// The facts of a 32-bit module that are the same for every function of it (its function names, the globals it calls through, the
// globals that hold pointers, the arguments its definitions take). They are held once and shared by every function's ModuleFacts, with
// their text as the C back end takes it written once at first use (the decompiler makes it): a copy of each in each function's facts,
// and writing them out again for each, was most of what a 32-bit module's C cost.
struct FlatShared {
    std::set<std::string> functions;
    std::set<std::string> called_through;
    std::set<std::string> pointer_globals;
    std::map<std::string, std::int64_t> takes;
    mutable std::once_flag text_once;
    mutable std::string functions_text, called_through_text, pointer_globals_text, takes_text;   // JSON, the library's form
};

// The module's facts handed to the C back end for ONE function, and what the back end leaves in them. The 16-bit ones are
// module_facts16()'s plus the module's later decisions; the 32-bit and PowerPC ones are what compilable32 and
// compilable_powerpc decide.
struct ModuleFacts {
    enum class Path { Flat32, Win16, PowerPc };
    Path path = Path::Flat32;
    bool have_code = false;                              // 16-bit: the segment's bytes were known (widths, loads ... are then said)

    // --- given, 16-bit
    std::optional<std::string> distance;                 // "far" | "near"
    std::optional<std::string> model;                    // "__stdcall16far" ...
    bool far_return = false;                             // it returns a far pointer
    std::optional<Va> selector;                          // the made-up selector of its code segment
    std::map<std::string, std::int64_t> widths_far;      // {a<n>: bytes the original reads through far pointer argument n}, as a far function
    std::map<std::string, std::int64_t> widths_near;     //   ... as a near function
    std::set<std::int64_t> loads;                        // BP offsets of the argument words loaded as far pointers
    bool exported = false;                               // starts with Windows' export prologue
    bool loadds = false;                                 // loads its own data segment whoever calls it
    std::vector<std::int64_t> ss_stores;                 // where it stores SS into its frame
    bool selector_structs = false;                       // structures of selectors are recovered
    bool callee_pops = false;
    bool without_notes = false;                          // no notes above a function (the module's C)
    SelectorImports selector_values;                     // imports used for their value

    // --- given, 32-bit and PowerPC
    std::set<std::string> functions;                     // the module's functions
    std::map<std::string, std::int64_t> struct_sizes;    // {structure name: bytes} recovered up to this function
    std::set<std::string> called_through;                // globals the module calls through
    std::map<std::string, std::string> aliases;          // {global: alias used at this function's own width}
    std::set<std::string> pointer_globals;               // globals holding pointers
    std::map<std::string, std::int64_t> takes;           // {function: arguments its definition takes}, not __cdecl ones
    std::optional<std::map<std::string, struct_text::PlannedParam>> struct_fields;   // the fields of the structures this function reads through its parameters
    std::shared_ptr<const FlatShared> shared;            // when set, the module's `functions`, `called_through`, `pointer_globals` and `takes` are these (and the four above are empty)

    // --- left by the back end
    std::optional<std::string> declared;                 // 32-bit and PowerPC: its heading as written
    std::optional<std::string> registers;                // "ecx" | "edx": where its first argument(s) travel
    std::set<std::string> structs_used;                  // structures its text names
    bool novalue = false;                                // PowerPC: it leaves nothing in r3 worth reading
    Interrupts interrupts;
    std::vector<std::string> selector_decls;
    std::map<std::string, std::map<std::string, std::map<std::int64_t, std::string>>> selector_shared;   // {structure: {function: {offset: type}}}
    std::map<std::string, std::string> selector_imports; // import -> "segment" | "offset"
    std::optional<NamesUsed> names;
    std::optional<Heading> heading;
    // The documents the back end's planners answer and the module carries from one pass to the next, by key ("frame",
    // "frame_slots", "segment_values", "code_places", "selector_renames", "code_reads"): JSON text in the library's form.
    OrderedMap<std::string, std::string> plans;
    // The facts as the back end left them (JSON text, the library's form): flags() reads them.
    std::optional<std::string> left;
};

// ---- what c_text asks of the rest ---------------------------------------------------------------------------------------

using SegmentImages = std::map<std::int64_t, Bytes>;

// What the native decompiler made of the functions asked for: each one's C (empty text when the decompiler made none), the
// order, the address of each, what it said besides the C. The notes point into `arena`.
struct Decompiled {
    OrderedMap<std::string, std::string> texts;
    std::vector<std::string> order;
    std::map<Va, std::string> by_va;
    std::map<std::string, FunctionNotes> notes;
    std::map<std::string, describe::NumberedImport> numbered;   // imports by number, as the description named them
    std::shared_ptr<Arena> arena;
    std::map<Va, std::string> described;   // x64: every function the description names, the runtime library's (not decompiled) too
};

// ... and of a 16-bit module: the description holds each function's notes.
struct Decompiled16 {
    OrderedMap<std::string, std::string> texts;
    std::vector<std::string> order;
    std::map<Va, std::string> by_va;
    std::shared_ptr<describe::NeDescription> description;
    std::shared_ptr<Arena> arena;
};

class Services {
public:
    virtual ~Services() = default;

    // ---- the decompiler proper (decompile/decompiler)
    // The native program's output for a 32-bit module's functions, with the L2 documents kept (not yet written as C). No functions
    // given: the module's own starts (a PowerPC image states them).
    virtual Decompiled raw(const std::filesystem::path& module, std::span<const Va> functions, const std::filesystem::path& outdir) = 0;
    // A 16-bit module decompiled (its functions found, rounds repeated until no new one shows).
    virtual Decompiled16 decompile16(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir) = 0;
    // A function's C written by the C back end from its L2 document in place of the printed text (`text`); without a document
    // or with one the back end declines, the printed text stays. Sets notes.from_l2. `images`: the code, for the passes that
    // read it. `module`: what the module says of the function, and where the back end puts what it reports. `text` empty:
    // the answer is the C only when written, else nullopt.
    virtual std::optional<std::string> c_from_l2(const std::optional<std::string>& text, FunctionNotes& notes,
                                                 const std::optional<SegmentImages>& images, ModuleFacts* module) = 0;
    // Whether the native program recovers structures for this 16-bit description; whether the L2 trees are given the passes that
    // read the code; the segments' bytes as those passes read them.
    virtual bool recovers_structures(const describe::NeDescription& description) = 0;
    virtual bool tree_images() = 0;
    virtual SegmentImages segment_images(const describe::NeDescription& description) = 0;

    // ---- what the symbol tables and the readers say
    // The L2 core library (the C back end's planners).
    virtual const translate::native_l2::Library& library() = 0;
    // {C name: DLL} of what a PE module imports by name.
    virtual std::map<std::string, std::string> import_dlls(const std::filesystem::path& module) = 0;
    // Bytes of arguments `dll`'s export removes from the stack; nullopt when not known.
    virtual std::optional<std::uint32_t> argument_bytes(std::string_view dll, std::string_view name) = 0;
    // The structures recovered for a 32-bit module (nullopt: the analysis cannot).
    virtual std::optional<std::vector<struct_text::Structure>> recover_structures(const std::filesystem::path& module) = 0;
    // Where a PowerPC image's bytes sit and which stubs are its imports.
    virtual describe::PowerPcFacts powerpc_facts(const std::filesystem::path& module) = 0;
};

// The switches of the module (each was an environment variable; `from_environment` reads them).
struct Policy {
    bool selector_structs = true;   // structures of selectors recovered (CB_SELECTOR_STRUCTS=0 turns off)
    bool code_data = true;          // the data a function reads through CS beside its code, by name (CB_CODE_DATA=0)
    bool prototypes = true;         // functions declared with their parameters where every call agrees (CB_PROTOTYPES=0)
    static Policy from_environment();
};

// ---- 32-bit --------------------------------------------------------------------------------------------------------------

// A 32-bit module made compilable as a whole: the preamble every function's text is compiled after, each function's C, the
// order, the address of each, the imports used, which functions take their first argument in ECX or EDX, each one's L2 document
// and its declaration (`;` ended). The documents point into `arena`.
struct Compilable32 {
    std::string preamble;
    OrderedMap<std::string, std::string> texts;
    std::vector<std::string> order;
    std::map<Va, std::string> by_va;
    std::vector<std::string> imports;
    std::set<std::string> in_ecx, in_edx;
    std::map<std::string, translate::ir::Node*> docs;
    std::map<std::string, std::string> declarations;
    std::shared_ptr<Arena> arena;
};
Compilable32 compilable32(Services& services, const std::filesystem::path& module, std::span<const Va> functions,
                          const std::filesystem::path& outdir);

// ---- 16-bit --------------------------------------------------------------------------------------------------------------

// What the data read through CS in a code segment came to: reads placed in a structure, named, why some were left.
struct CodeSegmentReport {
    std::int64_t read = 0;
    std::int64_t named = 0;
    std::vector<std::string> left;
    std::vector<std::int64_t> placed;
};

// A 16-bit module made compilable: one L2 Module (`made`, each function a MethodDecl with its C) and what the 16-bit build needs
// said of the module beside it.
struct Module16 {
    translate::l2::Made made;
    std::shared_ptr<describe::NeDescription> description;   // its notes hold each function's L2 document (they point into `arena`)
    std::shared_ptr<Arena> arena;
    std::set<std::string> near, wide, novalue;               // functions the original returns from near, with a 32-bit result, with no value
    Interrupts interrupts;                                   // (vector, registers) of the interrupts taken
    std::map<std::string, std::string> selector_imports;     // import -> "segment" | "offset": imports used for their selector or offset
    std::set<std::string> exported_prologue;                 // far functions the original starts with Windows' prologue
    std::map<std::string, std::string> failed;               // {function: why the build must replace it}
    std::set<std::string> prototyped;                        // functions declared with parameters
    std::map<std::string, std::string> declarations;         // each function's declaration, argument list left open
    std::map<std::int64_t, std::string> code_data;           // {segment: the C defining its codebreaker_code_<n> structure}
    std::map<std::int64_t, CodeSegmentReport> code_reads;
    std::map<std::string, std::pair<std::string, std::int64_t>> imports;   // {symbol: (module, ordinal)} of the imports called
    std::map<std::int64_t, Bytes> segments;                  // {segment: its bytes}, for data.c
    bool bios_data = false;                                  // reads the BIOS data through selector 0040h
    std::map<Va, SegOff> where;                              // function address -> (segment, offset)
};
Module16 compilable16(Services& services, const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir,
                      const Policy& policy = {});

// What the module says of a 16-bit function that its C is written with. `code` is the segment's bytes (none: not known); `seg`
// the number of the function's segment.
ModuleFacts module_facts16(std::optional<ByteView> code, std::int64_t off, std::int64_t end, const std::optional<std::string>& distance,
                           const std::optional<std::string>& model, const std::optional<Va>& dgroup, bool far_return,
                           const std::optional<std::int64_t>& seg = std::nullopt);

// Where the function stores SS into its frame, `mov [bp-0Ch], ss`: the decompiler's offset of each, from the entry SP.
std::vector<std::int64_t> ss_store_places(ByteView code, std::int64_t start, std::int64_t end);

// The bytes of a string as a C string literal, when they are text.
std::optional<std::string> string_literal(ByteView chunk);

// The structures of global selectors: one per global for the whole module where every function's fields agree, else each
// function's own. `shared` is {structure: {function: {offset: type}}}. -> the declarations, and {function: {the module's name:
// its own}}.
struct SharedSelectorStructs {
    std::vector<std::string> declarations;
    std::map<std::string, std::map<std::string, std::string>> renames;
};
SharedSelectorStructs shared_selector_structs(const translate::native_l2::Library& library,
                                              const std::map<std::string, std::map<std::string, std::map<std::int64_t, std::string>>>& shared);

// "int<vector hex 2>_<registers joined by _>" (the helper's name).
std::string interrupt_helper_name(std::uint32_t vector, std::span<const std::string> registers);
// The header's part for the interrupts a module takes.
std::vector<std::string> interrupt_helpers(const Interrupts& used);
// Assembly for the routines the interrupt helpers call, and the C that defines codebreaker_int. `vectors` are the interrupt vectors
// taken.
struct InterruptRoutines {
    std::string assembly, c;
};
InterruptRoutines interrupt_routines(const std::set<std::uint32_t>& vectors);

// ---- PowerPC -------------------------------------------------------------------------------------------------------------

struct ModulePPC {
    translate::l2::Made made;
    std::set<std::string> novalue;                              // functions that return nothing
    std::map<std::string, std::int64_t> externs;                // {name: address in the original} of the imports used
    std::map<std::string, std::set<std::int64_t>> calls;        // {callee: the numbers of arguments its calls pass}
    std::map<std::string, std::int64_t> argc;                   // {extern: argument count}
    describe::PowerPcFacts image;
};
ModulePPC compilable_powerpc(Services& services, const std::filesystem::path& module, const std::filesystem::path& outdir);

// ---- x64 -----------------------------------------------------------------------------------------------------------------

// An x64 module made compilable by Visual C++'s x64 compiler (c_text_x64.cpp). What the rebuilt functions reach that they do not
// have is the original's, where the original has it: a global by its address, an import through its slot, a function not rebuilt
// (the runtime library's) by its address -- so the rebuilt image is run beside the original.
struct ModuleX64 {
    translate::l2::Made made;                         // the header and each function's C, by address
    std::map<std::string, std::uint64_t> outside;     // {name the C calls: where in the original} of what it reaches that is not rebuilt
    std::set<std::string> novalue;                    // the functions the decompiler found to return nothing (written to return uint8)
    std::uint64_t image_base = 0;
    // A C++ program's: the classes its run-time type information names, declared in the header (decompile/cpp_text_x64), and
    // {the function's C name: the member it is written as} for its methods, whose texts are C++ (by the member's name).
    std::size_t classes = 0;
    std::map<std::string, std::string> methods;
};
ModuleX64 compilable_x64(Services& services, const std::filesystem::path& module, const std::filesystem::path& outdir);

// {name: number of arguments} for the functions in `names` that every call passes the same number of arguments to.
std::map<std::string, std::int64_t> call_counts(const std::map<std::string, std::set<std::int64_t>>& calls,
                                                const std::set<std::string>& names);

}  // namespace cb::decompile::c_text
