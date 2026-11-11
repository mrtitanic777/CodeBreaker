// codebreaker/build/flat_thunk.hpp -- typed C++ of codebreaker.build.flat_thunk
//
// flat_thunk.py -- the authentic-rebuild + binary-verify engine, aimable at any Win95 module.
//
// Automates the workflow proven by hand on LZ32.DLL: CLASSIFY a binary by how Microsoft actually built
// it, RECOVER authentic source for that class, BUILD it with the period toolchain, DIFF the result
// against the shipped binary, and DOCUMENT the verification level. This file is the FOUNDATION --
// the binary model (PE/NE parse), the classifier, and the differ; the per-class recoverers and the
// build drivers plug into it.
//
// The guiding standard (design/RECONSTRUCTION_STANDARD.md): authentic-method + binary-verified, not
// source-text-exact. The differ reports what is actually verifiable -- entry point, section layout,
// exports, imports, size -- never "we guessed the names."
//
//     python -m codebreaker flat-thunk <binary> [--classify] [--diff <built.dll>]
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * PE and NE are structs holding the format reader (formats::pe::PEFile / formats::ne::NEFile); `load` answers
//     std::variant<std::monostate (None), PE, NE>.
//   * The dicts the Python answers with are records: Classification (its keys, in the Python's order, through
//     `items()`), BinDiff (the keys a PE or an NE diff has are optional fields), Toolchain, Discovery, CompileOw,
//     BuildResult, Recovered. A key the Python leaves out (an error-only answer has no `log`) is an empty optional.
//   * A stub's signature (argbytes, nptr) is std::tuple<std::optional<int64_t>, int64_t> (argbytes None when unresolved).
//   * SDK prototypes (`_load_sdk_protos`) are `SdkProto` records keyed by export name.
//   * The Python calls `platforms.of("pe")` for the OpenWatcom compiler and linker; those platform slots are
//     build::watcom::ow_tool("wcc386") / ow_tool("wlink") (platforms/win32.py names exactly those).
//   * Python's `subprocess.run(..., env=, timeout=)`: the runtime's has no env or timeout; the environment is set for
//     the run and put back (see EnvScope in the .cpp), timeouts are not applied.
//   * The set of public symbols omf.publics returns is walked in sorted order (as the Python does).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::flat_thunk {

namespace pe_format = codebreaker::formats::pe;
namespace ne_format = codebreaker::formats::ne;

// ------------------------------------------------------------------ binary model (PE + NE)
// sections as (name, va, vsize, rawsize, rawptr)
using Section = std::tuple<py::str, int64_t, int64_t, int64_t, int64_t>;

// A PE image (formats/pe reads it), in the shapes this engine works with: sections as
// (name, va, vsize, rawsize, rawptr), exports as sorted (ordinal, name), imports as
// {dll: [name or '@ordinal']}.
struct PE {
    py::str path;
    pe_format::PEFile f;
    py::bytes d;
    int64_t entry = 0;
    int64_t nsec = 0;
    py::list<Section> secs;
    py::set<int64_t> smapls_rvas;      // `_smapls_rvas`: set by recover_flat_thunk (the Python hangs it on the object)

    explicit PE(const py::str& path);

    std::optional<int64_t> _file2rva(int64_t o) const;
    py::list<std::tuple<int64_t, py::str>> exports() const;
    // -> dict dll -> list of 'name' or '@ordinal'.
    py::dict<py::str, py::list<py::str>> imports() const;
    // -> dict name -> function RVA (AddressOfFunctions[ordinal]); the stub location, so we can
    // read each export's code (arity, marshalling) straight from the binary.
    py::dict<py::str, int64_t> export_addrs() const;
    // Raw bytes at an RVA (for reading a stub's code).
    py::bytes read_at(int64_t rva, int64_t n) const;
};

// A 16-bit NE module (formats/ne reads it): its segments' kinds and resident names.
struct NE {
    py::str path;
    ne_format::NEFile f;
    py::bytes d;
    int64_t nseg = 0;
    py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> segs;       // (kind, file_off, length, flags)

    explicit NE(const py::str& path);

    py::list<std::tuple<int64_t, py::str>> resident_names() const;
};

using Binary = std::variant<std::monostate, PE, NE>;

Binary load(const py::str& path);

// ------------------------------------------------------------------ classifier
extern const py::list<py::str> _THUNK_MARKERS;

// {format, subtype, recipe, why} (and n_exports, n_imports) describing how the module was built.
struct Classification {
    py::str format, subtype;
    std::optional<py::str> recipe;
    std::optional<int64_t> n_exports, n_imports;
    py::str why;

    // the dict's items in the Python's order, the value as str() gives it
    py::list<std::tuple<py::str, py::str>> items() const;
};

Classification classify(const py::str& path);

// ------------------------------------------------------------------ differ
// Structural diff of two binaries -> report dict + a verification verdict.
struct BinDiff {
    std::optional<py::str> error;
    bool named = false;                                    // built / shipped / sizes are in the answer
    py::str built, shipped;
    int64_t size_built = 0, size_shipped = 0;
    std::optional<py::str> kind;                           // "PE" / "NE"
    // PE
    bool entry_match = false;
    std::tuple<py::str, py::str> entry;                    // (hex(built), hex(shipped))
    py::list<py::str> sections_built, sections_shipped;
    bool sections_match = false, exports_match = false;
    std::tuple<int64_t, int64_t> n_exports, n_imports;
    py::list<py::str> import_dlls_built, import_dlls_shipped, imports_only_in_shipped, imports_only_in_built;
    // NE
    std::tuple<int64_t, int64_t> nseg;
    py::list<py::str> seg_kinds_built, seg_kinds_shipped;
    bool seg_kinds_match = false;
    std::optional<py::str> verdict;
};

BinDiff bindiff(const py::str& built, const py::str& shipped);

py::str _fmt_diff(const BinDiff& r);

// ------------------------------------------------------------------ toolchain resolver
std::optional<py::str> _first(const py::list<std::optional<py::str>>& cands);

// The 'reference' folder (Win95 DDK + WIN32SDK + the thunk compiler 1.01): corpus.reference().
py::list<py::str> _reference_roots();

// The period build tools. Flat-thunk path is all native (runs on modern Windows):
// thunk.exe (prefer the v1.01 from the Chicago beta SDK -> closest imports), MASM 6.11c, MSVC 2.0.
struct Toolchain {
    std::optional<py::str> thunk, ml;
    py::str cl, link, lib, rc, include;
    py::list<py::str> lib_dirs;
};
Toolchain toolchain();

py::str _winpath(const py::str& p);

// The pieces of a flat-thunk module found in srcdir.
//   thk = the *.thk thunk script; cfiles = every *.c (veneer); moddef = the module .def (LIBRARY <MODULE>, not the
//   *k32 import-lib def); k32def = the *k32.def (KERNEL32 by-ordinal import lib); rc = optional *.rc;
//   module = LIBRARY name from moddef (lowercased)
struct Discovery {
    std::optional<py::str> error;
    py::str thk;
    py::list<py::str> cfiles;
    py::str moddef, k32def;
    std::optional<py::str> rc;
    py::str module;
};
Discovery _discover_flat_thunk(const py::str& srcdir);

extern const py::dict<py::str, int64_t> _BYVAL;

// Fallback size of one stdcall parameter (used only if the PUBDEF lookup misses): pointers 4,
// known by-value structs/64-bit types 8/16, else the machine word 4.
int64_t _param_bytes(const py::str& part);

// (wcc386, wlink, include-dirs) for the from-source OpenWatcom toolchain -- the compiler the
// project builds with (no Microsoft tools). None if the OW tree is not present.
struct OwPaths {
    py::str root, wcc386, wlink;
    py::list<py::str> include;
};
std::optional<OwPaths> _ow_paths();

// Compile every recovered veneer .c in `srcdir` with OpenWatcom wcc386 against OpenWatcom's own
// Win32 headers -- the from-source toolchain, no Microsoft tools. This is the project's real build
// compiler (owbuild/vmm32 lanes); it verifies the recovered source is compilable as authored.
struct CompileOw {
    bool ok = false;
    std::optional<py::str> error;
    py::list<std::tuple<py::str, int64_t, py::str>> results;     // (file, rc, first_error)
};
CompileOw compile_ow(const py::str& srcdir);

// What a build answers: ok, the DLL, its module, and the log (a failed one has an error or a log instead).
struct BuildResult {
    bool ok = false;
    std::optional<py::str> error, log;
    std::optional<py::str> dll, module;
    int64_t n_exports = 0, bytes = 0;
};

// Build a flat-thunk veneer to a real DLL with the FROM-SOURCE OpenWatcom toolchain -- no
// Microsoft tools. Compiles the recovered veneer .c with wcc386 (auto-exporting each function via
// __declspec(dllexport), so no fragile export-decoration guessing), stubs the thunk-compiler's
// connection routine (<module>_ThunkConnect32 -- the real one is generated by the thunk compiler at
// body curation), resolves KERNEL32/USER32/GDI32 against OpenWatcom's own import libs, and links
// `system nt_dll`. Produces a loadable DLL whose export table matches the retail module's function
// exports. Returns {ok, dll, module, n_exports, log}.
BuildResult build_flat_thunk_ow(const py::str& srcdir, const std::optional<py::str>& outdir = std::nullopt);

// Build a flat-thunk module from its recovered source (native chain, no DOSBox):
//   thunk.exe -> .asm ; ml -DIS_32 -> thunk obj ; cl each veneer .c -> obj ; rc (opt) ;
//   lib (KERNEL32 by-ordinal import lib) ; link /NODEFAULTLIB /ENTRY:DllMain@12 -> <module>.dll.
// Discovers the pieces in srcdir. Returns {ok, dll, module, log}.
BuildResult build_flat_thunk(const py::str& srcdir, const std::optional<py::str>& outdir = std::nullopt);

// ------------------------------------------------------------------ RECOVER (flat-thunk)
// KERNEL32 by-ordinal flat-thunk / handle helpers (names from the Windows Me symbol file; Geoff
// Chappell, "Ordinal-Only Exports In KERNEL32 4.0"). Value = (name, stdcall-arg-bytes or None).
extern const py::dict<int64_t, std::tuple<py::str, std::optional<int64_t>>> _KERNEL32_ORDINALS;

// ------------------------------------------------------------------ stub analysis (x86-32)
// A flat-thunk export is a tiny stdcall stub: set up args, marshal 32:16 pointers via SMapLS,
// call the 16-bit side through QT_Thunk/FT_Prolog, unmap, and `ret N`. From the stub bytes alone
// we can recover the REAL signature the compiler enforced -- the `ret N` epilogue pops exactly the
// argument bytes (stdcall), so N/4 = the DWORD-sized argument count; and each SMapLS call marshals
// one 32-bit pointer argument down to a 16:16 far pointer, so counting them bounds the pointer args.
//
// A conservative, table-driven length decoder walks the stub; on an opcode it does not model it
// stops (returns 0) rather than mis-measure -- so arity is recovered where the walk is certain and
// the module cleanly falls back to the (void) scaffold where it is not. Never a WRONG signature.
extern const py::set<int64_t> _ARITH_RM, _ARITH_AX, _MODRM_ONLY, _MODRM_IMM8, _MODRM_IMMW;

// Bytes in the ModRM (+SIB +disp) cluster starting at d[i].
int64_t _modrm_len(const py::bytes& d, int64_t i);

// Length of the x86-32 instruction at d[i]; 0 if the opcode is not modelled (caller stops).
int64_t _ilen(const py::bytes& d, int64_t i);

int64_t _imagebase(const PE& pe);

// Absolute VAs that, when the target of a stub's call, mark a 32:16 pointer marshalling:
// the IAT slots of KERNEL32!SMapLS/SUnMapLS, plus any `jmp [slot]` incremental-link thunks that
// forward to them. Used to count pointer arguments in a stub.
py::set<int64_t> _smapls_iat_rvas(const PE& pe);

using StubSignature = std::tuple<std::optional<int64_t>, int64_t>;      // (argbytes or None, nptr)

// Recover (argbytes, nptr) for an export at RVA `frva`, or (None, 0) if it does not cleanly
// resolve. argbytes = stdcall bytes popped by `ret N` (every stdcall return in a function pops the
// same N, so the first one reached on the linearly-decoded path is authoritative); nptr = SMapLS
// marshalling calls seen (32-bit pointer args). `maxlen` bounds the walk to the function's extent.
// A `ret N` is only accepted when N is a plausible stdcall count (N%4==0, N<=256) so a decode
// desync at inline data cannot fabricate a signature -- unresolved beats wrong.
StubSignature _stub_signature(const PE& pe, int64_t frva, int64_t maxlen = 8192);

// Plausible `ret N` immediate from a function's bytes (N%4==0, N<=256) without decoding -- robust
// to a decode desync at an inline jump table. Accept a value if it either repeats (>=2 return sites,
// the usual multi-return case) or occurs once immediately after an epilogue terminator (pop reg /
// leave -- a single-return function); an ambiguous single-occurrence set is left unresolved.
std::optional<int64_t> _argbytes_by_epilogue(const py::bytes& d);

// The 16-bit target DLL name for a flat-thunk module, read from the ThunkConnect32 string
// args in the image ("<x>.DLL","<module>.DLL"). Returns (target16, candidates).
std::tuple<std::optional<py::str>, py::list<py::str>> _thunk_dll_targets(const PE& pe, const py::str& module);

// An SDK prototype: {"ret", "params", "conv", "header", "doc"}.
struct SdkProto {
    py::str ret, params, conv, header, doc;
};

// Per-export SDK prototypes keyed by export name: the 32-bit or the 16-bit SDK headers' prototypes
// (symbols/prototypes), so a 16-bit export gets its real 16-bit signature (FAR pointers, UINT/WORD)
// rather than the 32-bit one. Absent, callers fall back to recovered arity -- never a fabricated prototype.
py::dict<py::str, SdkProto> _load_sdk_protos(bool win16 = false);

// Parameter list text for an export. Real SDK prototype wins; else the stdcall arity recovered
// from `ret N` (each arg a machine word, the first `nptr` flagged as marshalled pointers); None if
// the stub did not resolve (keeps the honest (void)+TODO scaffold). -> (params or None, known)
std::tuple<std::optional<py::str>, bool> _sig_params(const StubSignature& sig, const std::optional<SdkProto>& proto);

py::str _sig_ret(const std::optional<SdkProto>& proto);

// The function's calling-convention keyword. From the SDK prototype when known (WINAPI, or the
// varargs VFWAPIV for ICCompress-style APIs) -- so the definition's convention matches the header
// and does not clash (C2373); else WINAPI (flat thunks are stdcall).
py::str _sig_conv(const std::optional<SdkProto>& proto);

// SDK headers whose contents <windows.h> already pulls in -- an explicit #include would be redundant.
extern const py::set<py::str> _IN_WINDOWS_H;

// some SDK headers only compile if a prerequisite is included first (the original source did so):
// msacm.h/vfw.h use the multimedia types from mmsystem.h; mciavi.h builds on both.
extern const py::dict<py::str, py::list<py::str>> _HEADER_PREREQ;

extern const py::set<py::str> _CKW;

// Split a parameter list on top-level commas (respecting nested parens, e.g. a function-pointer
// parameter's own arg list).
py::list<py::str> _split_params(const py::str& s);

// Ensure every parameter in a list has a NAME, so the text is valid in a function DEFINITION
// (the period C compiler rejects unnamed params in a definition: `void f(HWND,BOOL){}` -> C2055).
// SDK prototypes often omit names (`DragAcceptFiles(HWND, BOOL)`); synthesize aN for the bare ones.
// A parameter is considered named when its last token is a lowercase/mixed-case identifier that is
// not a type keyword and the param does not end in `*` (SDK types are ALL-CAPS or keywords, names
// are camelCase).
py::str _def_params(const py::str& params);

// The #include lines a recovered module needs so its SDK-typed signatures resolve: the real
// header each used prototype came from (msacm.h/vfw.h/commctrl.h/...), minus the ones <windows.h>
// already provides, with each header's prerequisites emitted first (mmsystem.h before msacm.h) so
// it parses. Emitting the authentic headers is how the original source compiled -- the struct/handle
// types (LPACMFORMATDETAILS, OPENFILENAME, ...) are then real, not fabricated.
py::list<py::str> _needed_includes(const py::list<SdkProto>& protos_used);

// True for an export that is thunk DATA, not a callable API -- the ThunkData32 tables the thunk
// compiler emits (e.g. VerFThk_ThunkData32, C16ThkSL01). These are real exports (kept in the .def)
// but must not be scaffolded as functions.
bool _is_thunk_data(const py::str& nm);

// What recover_flat_thunk answers (an error alone when the file is not a PE).
struct Recovered {
    std::optional<py::str> error;
    py::str module;
    int64_t n_exports = 0;
    py::list<int64_t> k32_ordinals, unknown_ordinals;
    py::str outdir;
    std::optional<py::str> target16;
    py::list<py::str> dll_candidates;
    int64_t n_signatures_recovered = 0, n_pointer_args = 0, n_sdk_prototypes = 0;
    py::list<py::str> recovered, scaffolded;
};

// Emit recovered flat-thunk source from the binary. The .def (exports) and <mod>k32.def
// (KERNEL32 by-ordinal imports) are recovered VERBATIM from the binary's tables; per-export
// SIGNATURES (stdcall arity + pointer-arg count) and the 16-bit target DLL are recovered from the
// stub code and the ThunkConnect32 strings; bodies remain scaffolds (marshalling flagged for
// curation). Returns a summary dict.
Recovered recover_flat_thunk(const py::str& pe_path, const py::str& outdir);

// ------------------------------------------------------------------ RECOVER from a decompile (logic modules)
std::optional<py::str> _opt(const py::list<py::str>& argv, const py::str& name);

// python -m codebreaker flat-thunk <binary> [--classify] [--diff <built.dll>]
// (the Python returns 0 after the help text and None otherwise: None is the empty optional)
std::optional<int64_t> main(const py::list<py::str>& argv);

}  // namespace codebreaker::build::flat_thunk
