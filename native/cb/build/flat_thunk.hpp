// cb/build/flat_thunk.hpp -- the authentic-rebuild and binary-verify engine, aimable at any Win95 module.
//
// The workflow proven by hand on LZ32.DLL, automated: CLASSIFY a binary by how Microsoft actually built it, RECOVER
// authentic source for that class, BUILD it with a toolchain, DIFF the result against the shipped binary, and say what is
// actually verifiable -- entry point, section layout, exports, imports, size -- never "we guessed the names". The standard
// is authentic-method + binary-verified, not source-text-exact (design/RECONSTRUCTION_STANDARD.md).
//
// A *flat thunk* DLL is a 32-bit veneer over a 16-bit DLL (LZ32 over LZEXPAND, MSACM32 over MSACM ...): every export is a
// tiny stdcall stub that maps 32-bit pointers to 16:16 (SMapLS), calls the 16-bit side through QT_Thunk and returns with
// `ret N`. From the stub bytes alone this module recovers the real signature the thunk compiler enforced:
//
//     const auto kind = flat_thunk::classify(dll);                  // Subtype::FlatThunk, recipe "thunk-ml-cl-link"
//     const auto rec = flat_thunk::recover(dll, outdir, sdk);       // lz32.def, lz32k32.def, lz32.thk, lz32.c
//     const auto built = flat_thunk::build_period(locations, srcdir);   // thunk.exe, ML, CL, LINK -> lz32.dll (built.ok, built.dll)
//     const auto diff = flat_thunk::bindiff(*built.dll, dll);       // structural parity, or what differs
//
// Two builds exist: `build_period` (the period tools of the Chicago SDK and Visual C++ 2.0, which gives the closest
// imports) and `build_openwatcom` (from-source OpenWatcom wcc386 + wlink, no Microsoft tools: the project's own compiler).
//
// The stub reader is conservative on purpose: a table-driven length decoder walks the stub and stops, rather than
// mis-measure, at an opcode it does not model; a signature is recovered where the walk is certain and the export falls
// back to an honest `(void)` scaffold where it is not. Never a WRONG signature.
//
// The `flat-thunk` command (`command()`) is declared with the app's commands.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/build/watcom.hpp"
#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/formats/ne.hpp"
#include "cb/formats/pe.hpp"
#include "cb/symbols/prototypes.hpp"

namespace cb::build::flat_thunk {

// ---- the binary model (PE and NE) ---------------------------------------------------------------------------------

// A PE image in the shapes this engine works with.
class PeModule {
public:
    // FormatError / TruncatedError when the file is not a readable PE; IoError when it cannot be read.
    static PeModule open(const std::filesystem::path& file);

    const formats::pe::Image& image() const { return image_; }
    std::uint32_t entry_rva() const { return image_.header().entry_rva; }

    struct NamedExport {
        std::uint64_t ordinal = 0;
        std::string name;
        friend auto operator<=>(const NamedExport&, const NamedExport&) = default;
    };
    // The exports that have a name, by ordinal and then name.
    std::vector<NamedExport> exports() const;

    // The imported symbols of each library, in the import table's order: the name, or "@<ordinal>" for an import by
    // ordinal. A library listed twice keeps the symbols of its later listing at its first place.
    OrderedMap<std::string, std::vector<std::string>> imports() const;

    // name -> function RVA: where each named export's stub is, so its code can be read (arity, marshalling).
    std::map<std::string, std::uint32_t> export_addresses() const;

    // Up to `n` bytes at an RVA (fewer at the end of the file; none when the RVA is not in the file).
    ByteView read_at(std::uint32_t rva, std::size_t n) const;

private:
    explicit PeModule(formats::pe::Image image) : image_(std::move(image)) {}
    formats::pe::Image image_;
};

enum class SegmentKind { Code, Data };

// A 16-bit NE module: its segments' kinds and resident names.
class NeModule {
public:
    static NeModule open(const std::filesystem::path& file);

    const formats::ne::Module& module() const { return module_; }
    std::size_t segment_count() const { return module_.header().segment_count; }
    std::vector<SegmentKind> segment_kinds() const;
    // (ordinal, name) of each resident name, in the table's order.
    std::vector<std::pair<std::uint16_t, std::string>> resident_names() const;

private:
    explicit NeModule(formats::ne::Module module) : module_(std::move(module)) {}
    formats::ne::Module module_;
};

using Binary = std::variant<PeModule, NeModule>;

// The module a file is; nullopt when it is neither a PE nor an NE (or not an MZ file at all).
std::optional<Binary> load(const std::filesystem::path& file);

// ---- classify -----------------------------------------------------------------------------------------------------

enum class Format { Unknown, Ne, Pe };
enum class Subtype { Unknown, Win16C, FlatThunk, Win32C };

std::string_view format_name(Format f);     // "?", "NE", "PE"
std::string_view subtype_name(Subtype s);   // "unknown", "win16-c", "flat-thunk", "win32-c"

// How the module was built.
struct Classification {
    Format format = Format::Unknown;
    Subtype subtype = Subtype::Unknown;
    std::optional<std::string> recipe;           // "msc6-dosbox", "thunk-ml-cl-link", "msvc-native"; none for an unknown module
    std::optional<std::uint64_t> exports;        // a PE's named exports
    std::optional<std::uint64_t> imports;        // a flat-thunk's imported symbols
    std::string why;
};

// KERNEL32 helpers whose presence in the import table marks a flat thunk.
inline constexpr std::string_view kThunkMarkers[] = {"SMapLS", "SUnMapLS", "ThunkConnect32", "ThunkInitLSF", "FT_Prolog", "QT_Thunk"};

// An NE module is Win16 C (built with MSC under DOSBox); a PE that imports the thunk helpers is a flat thunk, any other
// PE is Win32 C. FormatError when the file is a damaged PE or NE.
Classification classify(const std::filesystem::path& file);

// ---- diff ---------------------------------------------------------------------------------------------------------

enum class Verdict { ByteSurfaceIdentical, StructuralParity, Partial };
std::string_view verdict_name(Verdict v);   // "byte-surface-identical", "structural-parity", "partial"

struct PeDiff {
    bool entry_match = false;
    std::pair<std::uint32_t, std::uint32_t> entry;                          // (built, shipped) entry RVAs
    std::vector<std::string> sections_built, sections_shipped;
    bool sections_match = false, exports_match = false;
    std::pair<std::size_t, std::size_t> exports;                            // distinct ordinals (built, shipped)
    std::vector<std::string> import_dlls_built, import_dlls_shipped;       // sorted
    std::vector<std::string> imports_only_in_shipped, imports_only_in_built;   // "DLL!name", sorted
    std::pair<std::size_t, std::size_t> import_count;                       // distinct "DLL!name" (built, shipped)
};

struct NeDiff {
    std::pair<std::size_t, std::size_t> segments;
    std::vector<SegmentKind> kinds_built, kinds_shipped;
    bool kinds_match = false, exports_match = false;
};

struct BinDiff {
    std::string built, shipped;                  // file names
    std::uint64_t size_built = 0, size_shipped = 0;
    std::variant<PeDiff, NeDiff> detail;
    Verdict verdict = Verdict::Partial;
};

// Structural diff of two binaries. A PE pair is byte-surface-identical when entry, sections, exports and imports all
// agree and structural-parity when entry, sections and exports do; an NE pair is structural-parity when the segment
// kinds and resident names agree. UsageError when an input is not a PE or NE, or the two are of different kinds.
Result<BinDiff> bindiff(const std::filesystem::path& built, const std::filesystem::path& shipped);

// The diff as the lines the command prints.
std::string format_diff(const BinDiff& diff);

// ---- toolchains ---------------------------------------------------------------------------------------------------

// Where the tools are (settings.json: reference, period_compiler, openwatcom).
struct Locations {
    std::vector<std::filesystem::path> reference_roots;   // the reference folder: Win95 DDK, WIN32SDK, the thunk compiler 1.01
    std::filesystem::path period_compiler;                // the BIN folder of Visual C++ 2.0 (or its root)
    watcom::OpenWatcom openwatcom{std::filesystem::path()};
};

// The period tools. thunk and ml are empty when the reference folder lacks them; the Visual C++ 2.0 paths are where
// they would be.
struct Toolchain {
    std::filesystem::path thunk, ml;
    std::filesystem::path cl, link, lib, rc, include;
    std::vector<std::filesystem::path> lib_dirs;
};

// thunk.exe (the v1.01 of the Chicago beta SDK first: closest imports), MASM 6.11c, Visual C++ 2.0. All are native
// programs; they run on a modern Windows without DOSBox.
Toolchain toolchain(const Locations& where);

// ---- build --------------------------------------------------------------------------------------------------------

// The pieces of a flat-thunk module's source folder.
struct Discovery {
    std::filesystem::path thk;                   // the thunk script
    std::vector<std::filesystem::path> cfiles;   // every veneer .c, by name
    std::filesystem::path moddef;                // the module's .def (LIBRARY <MODULE>)
    std::filesystem::path k32def;                // the *k32.def: KERNEL32 by-ordinal import library
    std::optional<std::filesystem::path> rc;
    std::string module;                          // the LIBRARY name, lower case
};

// UsageError naming the missing piece when the folder lacks the script, a .def, the KERNEL32 .def or a veneer .c.
Result<Discovery> discover(const std::filesystem::path& srcdir);

struct BuildResult {
    bool ok = false;
    std::optional<std::filesystem::path> dll;
    std::string module;
    std::size_t exports = 0;                     // OpenWatcom build: the exports of the DLL it made
    std::uint64_t bytes = 0;
    std::string error;                           // why it failed ("" when ok)
    std::string log;                             // what the tools said
};

// The thunk compiler, ML, CL and LINK on the recovered source: thunk.exe -> .asm; ml -DIS_32 -> thunk object; cl each veneer
// .c; rc if there is a .rc; lib the KERNEL32 by-ordinal import library; link /NODEFAULTLIB /ENTRY:DllMain@12. The pieces are
// found in `srcdir`; the work happens in `outdir` (default <srcdir>/_build), which gets a copy of the sources. A missing tool
// or source piece, or a tool that does not start, is a failed BuildResult, not an exception.
BuildResult build_period(const Locations& where, const std::filesystem::path& srcdir,
                         const std::optional<std::filesystem::path>& outdir = std::nullopt);

// From-source OpenWatcom, no Microsoft tools: compile the veneer with wcc386, stub the thunk compiler's connection routine
// (<module>_ThunkConnect32; the real one is generated by the thunk compiler at body curation), name every export from the
// object's PUBDEF records (so by-value structs get the right stdcall byte count, not a guessed one) and link
// `system nt_dll` against OpenWatcom's own KERNEL32/USER32/GDI32 import libraries. The DLL's export table matches the retail
// module's function exports. `outdir` defaults to <srcdir>/_ow.
BuildResult build_openwatcom(const watcom::OpenWatcom& ow, const std::filesystem::path& srcdir,
                             const std::optional<std::filesystem::path>& outdir = std::nullopt);

struct CompiledFile {
    std::string file;
    std::uint32_t exit_code = 0;
    std::string first_error;                     // the first line that says "Error", at most 100 characters
};
struct CompileResult {
    bool ok = false;
    std::string error;                           // OpenWatcom not built
    std::vector<CompiledFile> files;
};

// Compile every recovered veneer .c in `srcdir` with wcc386 against OpenWatcom's own Win32 headers: verifies the recovered
// source is compilable as authored.
CompileResult compile_openwatcom(const watcom::OpenWatcom& ow, const std::filesystem::path& srcdir);

// ---- the stub reader ----------------------------------------------------------------------------------------------

// What a stub's code says of its signature.
struct StubSignature {
    std::optional<std::uint32_t> argument_bytes;   // the `ret N` that pops the stdcall arguments; none when it did not resolve
    std::uint32_t pointer_args = 0;                // SMapLS marshalling calls seen (32-bit pointers made 16:16)
};

// The length of the x86-32 instruction at `at`; 0 when the opcode is not modelled (the caller stops).
std::size_t instruction_length(ByteView code, std::size_t at);

// The absolute addresses that, as the target of a stub's call, mark a pointer marshalling: the IAT slots of
// KERNEL32!SMapLS/SUnMapLS and the `jmp [slot]` thunks of an incremental link that forward to them.
std::set<std::uint32_t> smapls_targets(const PeModule& pe);

// (`ret N` bytes, pointer-argument count) of the stub whose code is `code`, loaded at `address` (the image base plus the
// stub's RVA; 32-bit, wrapping). Every stdcall return in a function pops the same N, so the first one reached on the
// linear walk is authoritative; a `ret N` is accepted only when N is a plausible stdcall count (a multiple of 4, at most
// 256) so a decode desync at inline data cannot fabricate a signature -- unresolved beats wrong. A function with an inline
// switch table defeats the linear walk: its epilogue then comes from `argument_bytes_by_epilogue`. A `ret` cut off inside
// its operand resolves nothing.
StubSignature stub_signature(ByteView code, std::uint32_t address, const std::set<std::uint32_t>& marshalling);

// A plausible `ret N` immediate from a function's bytes without decoding them: accepted if it repeats (two or more return
// sites) or occurs once right after an epilogue terminator (pop reg / leave); an ambiguous set is left unresolved.
std::optional<std::uint32_t> argument_bytes_by_epilogue(ByteView code);

// The 16-bit DLL a flat-thunk module connects to, read from the "<x>.DLL" strings of its image: the 32-bit name with a
// trailing "32" dropped (AVICAP32 -> AVICAP) or one of the well-known renames (COMCTL32 -> COMMCTRL), when the image has it;
// else the lone .DLL string that is not a system library; else none (ambiguous). `candidates` are every such string.
struct ThunkTarget {
    std::optional<std::string> dll16;
    std::vector<std::string> candidates;
};
ThunkTarget thunk_target(const PeModule& pe, std::string_view module);

// KERNEL32 by-ordinal flat-thunk and handle helpers (names from the Windows Me symbol file; Geoff Chappell, "Ordinal-Only
// Exports In KERNEL32 4.0").
struct Kernel32Ordinal {
    std::string_view name;
    std::optional<std::uint32_t> argument_bytes;
};
const Kernel32Ordinal* kernel32_ordinal(std::uint32_t ordinal);

// ---- recover ------------------------------------------------------------------------------------------------------

// A prototype from the SDK headers, as the source text needs it.
struct SdkPrototype {
    std::string returns;
    std::string params;       // "void" for none
    std::string convention;   // WINAPI, or the varargs VFWAPIV
    std::string header;       // the header's file name, upper case
};

// Per-export SDK prototypes keyed by export name: the real signature (FAR pointers, UINT/WORD for 16 bits) rather than the
// recovered arity. Absent, the arity stands -- never a fabricated prototype.
std::map<std::string, SdkPrototype> sdk_prototypes(const symbols::prototypes::PrototypeTable& table,
                                                   symbols::prototypes::Bits bits = symbols::prototypes::Bits::W32);

// An export that is thunk DATA, not a callable API: the ThunkData32 tables the thunk compiler emits (VerFThk_ThunkData32,
// C16ThkSL01). Real exports (kept in the .def), but not to be scaffolded as functions.
bool is_thunk_data(std::string_view name);

// The bytes a parameter takes on the stdcall stack, for the fallback when a PUBDEF lookup misses: pointers 4, known
// by-value structs and 64-bit types 8 or 16, else the machine word 4.
std::uint32_t parameter_bytes(std::string_view parameter);

// A parameter list split on its top-level commas (a function-pointer parameter's own list stays whole).
std::vector<std::string> split_parameters(std::string_view list);

// The list with every parameter named, so the text is valid in a function DEFINITION (the period C compiler rejects
// unnamed parameters in a definition: `void f(HWND,BOOL){}` is C2055); a bare one gets aN. A parameter counts as named when
// its trailing identifier is a mixed- or lower-case word that is not a type keyword (SDK types are ALL-CAPS or keywords).
std::string name_parameters(std::string_view list);

// The #include lines (lower-case header names) a module needs so its SDK-typed signatures resolve: the header each used
// prototype came from, minus the ones <windows.h> already provides, each header's prerequisites first (mmsystem.h before
// msacm.h) so it parses.
std::vector<std::string> needed_includes(const std::vector<const SdkPrototype*>& used);

// A function the veneer defines, as its text gives it: `<type> <convention> <name>(<parameters>) {` at the start of a line.
struct VeneerFunction {
    std::string convention, name, parameters;
};
std::vector<VeneerFunction> veneer_functions(std::string_view c_source);

// What recover wrote and found.
struct Recovery {
    std::string module;
    std::size_t exports = 0;                         // named exports
    std::vector<std::uint32_t> kernel32_ordinals;    // the by-ordinal KERNEL32 helpers it imports
    std::vector<std::uint32_t> unknown_ordinals;     // those the table has no name for
    std::filesystem::path outdir;
    std::optional<std::string> target16;
    std::vector<std::string> dll_candidates;
    std::size_t signatures = 0;                      // exports whose signature was recovered (a prototype or the stub's arity)
    std::size_t pointer_args = 0;
    std::size_t sdk_prototypes = 0;
    std::vector<std::string> recovered;              // what came verbatim from the binary
    std::vector<std::string> scaffolded;             // what needs curation
};

// Emit recovered flat-thunk source from the binary into `outdir`: <module>.def (the exports, verbatim), <module>k32.def (the
// KERNEL32 by-ordinal imports, named), <module>.thk (one thunked function per export, arity from the stub, pointer
// arguments from the SMapLS calls, direction defaulted to input -- curate where a function writes back) and <module>.c (the
// veneer: DllMain connecting the real 16-bit target and a thunk-through per export with its recovered signature). Bodies
// stay scaffolds. The files are written as Windows text (CRLF), as the period tools and the earlier recoveries have them.
// UsageError when the file is not a PE.
Recovery recover(const std::filesystem::path& pe_file, const std::filesystem::path& outdir,
                 const std::map<std::string, SdkPrototype>& sdk);

// ---- the command --------------------------------------------------------------------------------------------------

struct Services {
    Locations locations;
    std::function<std::optional<std::filesystem::path>(std::string_view)> retail;   // a module name -> the retail binary
    std::function<std::map<std::string, SdkPrototype>()> sdk;                        // the SDK prototypes (made when first asked)
};

// `flat-thunk <module> [--recover DIR] [--ow-compile] [--build DIR] [--ow-build DIR] [--diff BUILT]`: classify the module
// and say how; with --recover write its source, with --ow-compile check that source compiles, with --build or --ow-build make
// a DLL from a source folder, and diff what was built (or --diff's file) against the module. Exits 1 when a requested step
// fails.
cli::Command command(Services services);

}  // namespace cb::build::flat_thunk
