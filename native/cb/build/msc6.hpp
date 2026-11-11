// cb/build/msc6.hpp -- Microsoft C 6.0a, the compiler of the 16-bit Windows modules, run for comparison.
//
//     msc6::Context ctx{tools, runner, &cache};
//     if (msc6::available(ctx.tools, msc6::Compiler::Msc6)) {
//         std::vector<msc6::Source> sources = {{.text = c_text}, ...};
//         std::vector<std::optional<msc6::Code>> built = msc6::compile_many(ctx, sources, header, "/AM /Gsw /Os", options);
//     }
//
// The compiler is a DOS program. It is run in DOSBox-X (Tools::dosbox_x -- the emulator's program; Tools::msc6 -- the
// folder holding the compiler's BIN and INCLUDE), every source of a call in ONE session: starting the emulator costs
// about two seconds, compiling a function a fraction of one. Each function is compiled as a file of its own (or in a batch)
// and its code read out of the object file, with the bytes the linker fills in (an address, a segment, the distance of a
// call to another file) marked in `Code::mask` and left out of every comparison -- as build/period16's other compilers.
//
// What the reconstruction is written in for judging is OpenWatcom's dialect; `dialect()` is the same text said the way this
// compiler takes it (its keywords for far and the calling conventions, no 64-bit integers). The text passes below
// (pascal_order, wine_typing, sdk_calls, fold_temporaries, split_frame ...) turn the decompiler's C into what a period
// compiler is given: they are rewrites of C text and have no state.
//
// Visual C++ 1.52c's compiler (C/C++ 8.00c) is run the same way (`Compiler::Msvc152`); its CL, C1 and C2 are Win32 console
// programs, which run on Windows itself when `native_paths` can give them short paths (see there).
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/base/cache.hpp"
#include "cb/build/dos_run.hpp"
#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/symbols/wine16.hpp"

namespace cb::build::msc6 {

// ---- the tools ------------------------------------------------------------------------------------------------------

enum class Compiler {
    Msc6,      // Microsoft C 6.0a
    Msvc152,   // Microsoft C/C++ 8.00c of Visual C++ 1.52c, which built a few of the 16-bit modules
};

// Where they are (settings.json: dosbox_x, msc6, msvc152, native16, native16_tools, native16_work).
struct Tools {
    std::filesystem::path dosbox_x;   // the emulator's program
    std::filesystem::path msc6;       // BIN/CL.EXE, INCLUDE
    std::filesystem::path msvc152;    // BIN/CL.EXE, INCLUDE (WINDOWS.H, the Windows 3.1 SDK's)
    bool native16 = true;             // run Visual C++ 1.52c on Windows itself when it can be (settings native16 = off: never)
    std::optional<std::filesystem::path> native16_tools;   // the short path of its folder (default: <profile>/.cbt)
    std::optional<std::filesystem::path> native16_work;    // the short folder to build in (default: <profile>/.cbw)

    const std::filesystem::path& home(Compiler compiler) const { return compiler == Compiler::Msc6 ? msc6 : msvc152; }
};

// What a compile needs: the tools, a way to run DOS programs, and (optional) the cache that keeps what was compiled.
struct Context {
    Tools tools;
    const DosRunner& runner;
    base::cache::Cache* cache = nullptr;
};

// The emulator and the compiler's BIN/CL.EXE are there.
bool available(const Tools& tools, Compiler compiler = Compiler::Msc6);

// The flag sets the matcher tries in turn. /Gc: a function declared without a convention is pascal, as the reconstruction's
// own default.
std::span<const std::string_view> flag_sets();

// Functions to a source file when each is cut out by name.
inline constexpr std::size_t kBatch = 40;

// ---- object files ---------------------------------------------------------------------------------------------------

// The bytes of a piece of code, and non-zero for every byte a fix-up record (or a hole) names.
struct Code {
    Bytes code;
    Bytes mask;
};

// The first code segment (the first segment the object defines) of an OMF object file: the data records of that segment
// laid end to end, and 1 for every byte a fix-up record names. FormatError when a record does not fit the file.
Code object_code(ByteView object);

// The code of the public function `name` in an OMF object file -- cut from its code segment between its public name and the
// next -- or nullopt. A header's own functions (static helpers, the reconstruction's stubs) come first in the segment, so
// the first code segment is not the function. A C name `_f` and a fastcall one `@f` are `f`; case is not compared.
std::optional<Code> function_code(ByteView object, std::string_view name);

// ---- the text of the reconstruction ------------------------------------------------------------------------------------

// `header` as Microsoft C 6 takes it. (The function text itself is taken as it is.)
std::string dialect(std::string_view header);

// The functions the texts declare or define with a callee-pops (__stdcall) convention.
std::set<std::string> pascal_names(std::span<const std::string> texts);

// `text` with the arguments of every call to, and the parameters of every definition of, a function in `names` in Pascal
// order: first pushed first, an offset and its segment one far pointer. (The reconstruction says a callee-pops convention as
// OpenWatcom's __stdcall, whose arguments go on the stack right to left: the C lists them in that order. Microsoft's _pascal
// pushes them left to right, so the same list said as _pascal would be backwards.)
std::string pascal_order(std::string_view text, const std::set<std::string>& names);

// The Windows SDK's WINDOWS.H (Visual C++ 1.52c's, the Windows 3.1 one), or nullopt.
std::optional<std::string> windows_h(const Tools& tools);

// The name of an export by number (symbols/ordinals), or nullopt.
using OrdinalName = std::function<std::optional<std::string>(std::string_view module, std::uint32_t ordinal)>;

// `header` with every import by number that is really a C function declared __cdecl, not __stdcall: one its module exports
// under a C name (`_SUFORMATMESSAGEBOX` -- a pascal export is in capitals, a cdecl one keeps the compiler's underscore), or
// one `sdk` (WINDOWS.H's text) declares CDECL (wsprintf). The caller removes such a function's arguments (`add sp, 12h`
// after the call), and its argument list is in C order.
std::string cdecl_imports(std::string_view header, std::string_view sdk, const OrdinalName& ordinal_name);

// An export of a 16-bit module as Wine types it, found by (module, ordinal): nullptr for none.
using ExportLookup = std::function<const symbols::wine16::Export*(std::string_view module, std::uint32_t ordinal)>;
// The lookup over Wine's folder.
ExportLookup lookup_in(const symbols::wine16::Specs& specs);

// `header` with every import by number that Wine's .spec types declared with its parameters and its result's width, and
// each call of it in `texts` said with one argument to a parameter -- a 4-byte parameter's two words joined (high first, as
// a pascal call pushes them; low first in a C call's list). The texts are in Pascal order (pascal_order). An import with a
// call whose arguments fit neither way keeps its declaration without parameters. The texts come back under the same names,
// in the same order.
struct WineTyped {
    std::string header;
    OrderedMap<std::string, std::string> texts;
};
WineTyped wine_typing(std::string_view header, const OrderedMap<std::string, std::string>& texts, const ExportLookup& exports);

// `text` (as decompiled: parameters in stack order, a 4-byte one as its low word then its high) with the parameters of an
// exported function typed as Wine's .spec has them: each 4-byte parameter's two words one parameter -- a far pointer or a long
// -- used as such where the two are passed on together, and as FP_OFF/FP_SEG (or its halves) where one is used alone.
// Unchanged when the words do not add up to the spec's parameters, or `exported` is null.
std::string wine_definition(std::string_view text, const symbols::wine16::Export* exported);

// WINDOWS.H, then `header` (whose imports by number keep their own declarations: a call that cannot be said through the SDK's
// prototype -- sdk_calls -- stays as it was). Without an SDK, the header alone.
std::string sdk_header(std::string_view header, const std::optional<std::string>& sdk);

// {API name: parameter types} of every WINAPI function WINDOWS.H declares on a line; nullopt for one with a variable argument
// list. A function that returns nothing is in `no_result`.
struct Prototypes {
    std::map<std::string, std::optional<std::vector<std::string>>> params;
    std::set<std::string> no_result;
};
Prototypes sdk_prototypes(std::string_view sdk);

// `text` (in Pascal order) with each call of an import by number that the SDK declares said by its name, `USER_33(...)` ->
// `GetClientRect(...)` -- the module's maker called the API through its prototype, which decides how every argument is passed.
// A call whose arguments are one to a parameter is renamed; one whose arguments are one to a word of the parameters has each
// 4-byte parameter's two words (high first, as pushed) joined -- MAKELP for a pointer, MAKELONG for a number; any other call
// keeps its number and its declaration without parameters.
std::string sdk_calls(std::string_view text, const Prototypes& protos, const OrdinalName& ordinal_name);

// `text` with each temporary the decompiler made -- a local assigned once and used once, in the statement right after, in
// the same block -- put back into the expression it came from: `n = lstrlen(s); p = LocalAlloc(0x40, n + 1);` -> `p =
// LocalAlloc(0x40, lstrlen(s) + 1);`, `iVar1 = f(x); return iVar1;` -> `return f(x);`. The source wrote one expression; the
// compiler evaluates it in the source's order (a pascal call's arguments first to last), which the temporaries change. Not
// into a loop's condition (that is evaluated again each time round).
std::string fold_temporaries(std::string_view text);

// `text` with each pair of a function's own parameters that is used only as a pair -- the lower one (the offset) then the next
// (its segment), side by side in an argument list -- made the one far pointer the source had: `f(a1,a2) int2 a1; int2 a2; ...
// lstrlen(a1,a2)` -> `f(a1) void __far *a1; ... lstrlen(a1)`. The words go on the stack as they did; what changes is that the
// compiler knows them for one value (it pushes them straight from the frame, not through registers, and a prototype takes
// them as the pointer they are).
std::string join_far_parameters(std::string_view text);

// `text` with the decompiler's frame structure -- every local of a function in one packed structure of byte arrays, reached
// as `(*(int2 *)((uint1 *)codebreaker_frame.axStack_12 + 8))` -- taken apart into the locals a source declares: a variable for
// each place read or written (of the widest type it is read as), a buffer for each place whose address is taken (to the next
// variable). The structure keeps the frame's layout for a build; a compiler given locals lays them out itself, as it did the
// source's. Declared nearest the frame pointer first (search16 tries other orders). A function whose frame is reached in a
// way not read here is left as it was.
std::string split_frame(std::string_view text);

// Every read or write through a constant near address, `*(T *)0x63b2`, made one through a variable of the data segment,
// `(*(T *)codebreaker_d63b2)`: Microsoft C addresses a variable directly (`mov ax, [63b2]`, the offset a fix-up); a constant
// pointer it loads into a register first -- the source had the variable. One variable per address, read as each place reads
// it.
struct DsGlobals {
    std::string declarations;   // `extern char codebreaker_d63b2[];` lines, one per address
    std::string text;
};
DsGlobals ds_globals(std::string_view text);

// ---- compiling ------------------------------------------------------------------------------------------------------

// One C text to compile. With `function`, the code is that public function's (function_code), not the object's first code
// segment.
struct Source {
    std::string text;
    std::optional<std::string> function;
};

struct CompileOptions {
    std::filesystem::path work;                 // the folder the emulator runs in (state/<compiler>/work); required
    std::chrono::seconds timeout{900};
    Compiler compiler = Compiler::Msc6;
    std::size_t batch = kBatch;
    // With functions named: which sources share a file (the files of the original, as far as they are known); else `batch`
    // at a time in their order. Indices into the sources.
    std::vector<std::vector<std::size_t>> together;
};

// Compile each of `sources` with `header` before it. -> its code, or nullopt where the compiler refused it (or the session
// was cut off). NotFoundError when the compiler is not set up.
//
// With functions named, sources share source files: starting the compiler's passes (CL, C1, C2) is most of the cost of
// compiling a function in the emulator. What a function compiles to depends on its file: a far call to a function defined in
// the same file is `push cs; call` (near, the segment known), to one elsewhere `lcall` -- so a function's result is kept
// (cached) for its whole file, and a file with one function not yet known is compiled again whole. A file the compiler refuses
// has the functions its errors are in compiled on their own, the rest together, so one function cannot sink the rest.
std::vector<std::optional<Code>> compile_many(const Context& ctx, std::span<const Source> sources, std::string_view header,
                                              std::string_view flags, const CompileOptions& options);

// ---- the pieces of a compile (exposed for tests) --------------------------------------------------------------------------

// Visual C++ 1.52c's CL, C1 and C2 (C/C++ 8.00c) are Win32 console programs (KERNEL32 only; the Phar Lap stub is for DOS), and
// run on 64-bit Windows given two things: paths short enough for a buffer in CL (a long one overruns it: CL.EXE+0xC46B reads
// part of the path as a pointer) and an environment small enough for its pass launcher (with a modern environment it says
// "Out of memory" before starting C2). The short paths are directory junctions, made the first time; Tools::native16 = false
// keeps the emulator. Microsoft C 6.0a is a DOS program: always the emulator.
struct NativePaths {
    std::filesystem::path tools;   // the compiler's folder by a short path
    std::filesystem::path work;    // a folder to build in by a short path
};
// nullopt when the compiler runs in the emulator. `make_junction` false: do not make the short path (it must exist).
std::optional<NativePaths> native_paths(const Tools& tools, Compiler compiler, bool make_junction = true);

// A group of functions per source file: ([(stem, text)], [[(first line, source)]]) -- each group's functions in one file, and
// the line each begins on (the file starts with the #include and a blank line). `todo` holds each source's text.
struct BatchFiles {
    struct File {
        std::string stem;
        std::string text;
    };
    std::vector<File> files;
    std::vector<std::vector<std::pair<std::size_t, std::size_t>>> starts;   // per file: (first line, source index)
};
BatchFiles batch_files(std::span<const std::vector<std::size_t>> groups, const std::map<std::size_t, std::string>& todo, std::string_view prefix);

// The sources of the functions a refused file's errors are in, given the compiler's message text and each function's first
// line -- empty when an error is not in the file itself (the header's: then none can be told from another).
std::vector<std::size_t> refused(std::string_view compiler_text, std::span<const std::pair<std::size_t, std::size_t>> starts);

}  // namespace cb::build::msc6
