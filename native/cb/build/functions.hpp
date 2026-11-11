// cb/build/functions.hpp -- compile a module's reconstructed functions and link a DLL from them.
//
// One builder for every kind of module:
//
//     FunctionBuilder builder({.cache = &cache});
//     Built16 b = builder.build16(tools16, {.header = h, .texts = texts, .order = order, .imports = imports, .outdir = dir});
//     Built32 b = builder.build32(tools32, {...});
//     BuiltPowerPc b = builder.build_powerpc(ppc_tools, header, texts, order, externs, outdir, base);
//         where    {function name: where it is in the DLL} -- (segment, offset) for 16-bit, an address for 32-bit
//         dropped  {function name: why the compiler rejected it}, in the order of `order`
//
// Each function is compiled on its own, against a header that declares every function's convention and result. A
// function the compiler rejects cannot take others with it, it is known exactly which one failed and why, and the
// compiles run side by side. A rejected function is replaced by a stand-in with the same signature, so its callers
// still link (a match for such a caller is then marked as weaker: it ran against a stand-in).
//
// A 16-bit DLL is linked without the C runtime's start-up and with string constants in the code segment, so it has no
// data of its own: when it is run for judging, its data segment is given the original module's data and every global
// is then where the original had it.
//
// What a build needs from its surroundings is given, not looked up: the tools (where OpenWatcom is, its environment)
// in `Tools16`/`Tools32`/`PowerPcTools`, a cache for what compiles made in `BuildCache`, and the format knowledge the
// 32-bit link needs (a PE's exports, an OMF object's publics, which imports an object uses) as functions in `Tools32`.
// A FunctionBuilder holds what used to be module-level tables: the header each build folder was compiled against, the
// objects an engine has already made (`serve`), the digests of the last build of a folder.
#pragma once
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/core/parallel.hpp"

namespace cb::build::functions {

using Env = std::map<std::string, std::string>;               // environment variables added to the inherited ones
using Texts = std::unordered_map<std::string, std::string>;   // {function name: its C text}; looked up, never iterated
using ExtraFlags = std::map<std::string, std::vector<std::string>>;   // {function name: switches of its own}
using Reasons = OrderedMap<std::string, std::string>;         // {function name: why}, in the order the functions were given

// ---- constants ------------------------------------------------------------------------------------------------------

// Medium model (far code, near data), as the module build has it: the C runtime's helpers (long multiply, divide,
// shifts) are then called far, so a function reaches them from whichever physical segment the linker packs it into.
// -ecd: a function declared without a convention is __stdcall, the decompiler's default. -j: char is signed, as the
// decompiler means it (OpenWatcom's own is unsigned, and a byte compared with -1 never matched).
const std::vector<std::string>& cflags16();
// The PowerPC compiler's options.
const std::vector<std::string>& cflags_powerpc();

// ---- texts ----------------------------------------------------------------------------------------------------------

// The declaration of the function a text defines (everything before its body).
std::string prototype(std::string_view text);
// A definition with the same signature that does nothing.
std::string stand_in(std::string_view text);
// The C name behind a linker symbol (`_NAME@8`, `NAME_`, `_NAME`).
std::string plain_symbol(std::string_view symbol);
// The options one function's text asks for on top of the module's: the Windows prologue for an export, the frame kept
// for a function that reads where SP stood on entry.
std::vector<std::string> adjust_flags(std::span<const std::string> cflags, std::string_view text);

// ---- objects --------------------------------------------------------------------------------------------------------

// An OpenWatcom object kept from a run in another folder, as the tool writes it run in `folder`; nullopt when that
// cannot be said (then the tool is run). wasm and wcc name the source by its full path (THEADR: <the folder they ran
// in>\<stem>.asm or .c) and, in their dependency records (COMENT class E9h), each file they read with its date: the
// folder named is the one of now, the date that of the file there now (written before this is asked), and every
// record's checksum again as the tool makes it.
std::optional<Bytes> omf_here(ByteView object, const std::filesystem::path& folder);
// A tool's message as the build cache keeps it: the folder it ran in (which a message may name) as a mark, put back
// by message_here.
Bytes message_kept(std::string_view message, const std::filesystem::path& folder);
// message_kept's message, naming `folder` as the tool run there names it.
std::string message_here(ByteView kept, const std::filesystem::path& folder);
// A digest (lowercase hex SHA-1) of what a compiled object file IS -- its code, data, the places to patch and the names
// it defines and refers to -- and of nothing else: not the source file's name or date, nor the compiler's notes. Two
// compilations that give the same machine code give the same digest, whatever was changed in the source to no effect.
// nullopt when there is no file.
std::optional<std::string> object_digest(const std::filesystem::path& object);

// ---- the compilers and what they leave behind -----------------------------------------------------------------------

struct Compiler {
    std::filesystem::path exe;
    Env env;   // added to the environment the compiler runs in (INCLUDE, WATCOM, PATH ...)
};

// What the build remembers between runs, by content: compiled objects and flow checks. Given by whoever owns the
// shared cache; a FunctionBuilder without one compiles everything it is asked.
class BuildCache {
public:
    virtual ~BuildCache() = default;
    virtual std::optional<Bytes> get(std::string_view kind, std::string_view key) = 0;
    virtual void put(std::string_view kind, std::string_view key, ByteView value) = 0;
};

// What compiling one function came to.
struct CompileOutcome {
    std::string why;      // empty on success, else the compiler's first error (with the line it is about)
    bool keepable = true; // whether the outcome may be remembered: a rejection with a line is; a compiler that fell over is not
};

// Compile one function, running the compiler (no cache). `cflags` are already adjusted (adjust_flags). On success the
// object is at <outdir>/<stem>.obj. A compiler that runs longer than `timeout` is killed and the function counts as
// rejected.
CompileOutcome run_compile(const Compiler& cc, std::span<const std::string> cflags, const std::filesystem::path& outdir,
                           std::string_view stem, std::string_view text,
                           std::chrono::milliseconds timeout = std::chrono::minutes(10));
// The variables a function reads that nothing in it ever sets, as the compiler sees them (W200, its flow check, which
// the build's -w0 keeps quiet), sorted, without the cache. One the decompiler made up -- a register on entry
// (`in_AX`, `unaff_ES`), or a stack slot it split off from a buffer whose address it passes on (`iStack_1b` beside
// `axStack_34`) -- is a value the original got from somewhere this C does not: what it reads is whatever is there.
std::vector<std::string> run_never_set(const Compiler& cc, std::span<const std::string> cflags,
                                       const std::filesystem::path& outdir, std::string_view stem, std::string_view text);

// ---- every function -------------------------------------------------------------------------------------------------

// One folder of compile_groups.
struct Group {
    std::vector<std::string> cflags;
    std::vector<std::string> order;   // the functions to compile, in this order
    std::filesystem::path outdir;
};
// One folder's outcome.
struct Compiled {
    std::map<std::string, std::string> stems;   // {name: object file stem}; a function that could not even be stood in for is absent
    Reasons dropped;                            // {name: why it was rejected}
};

// ---- what a build is given ------------------------------------------------------------------------------------------

// OpenWatcom's 16-bit Windows tools.
struct Tools16 {
    Compiler wcc;
    std::filesystem::path wlink;
    std::filesystem::path wasm;       // assembles the interrupt helpers
    std::filesystem::path lib_dir;    // OpenWatcom's 16-bit Windows libraries
    // The assembler and C text of the routines the interrupt helpers call (decompile/c_text interrupt_routines).
    struct InterruptCode {
        std::string assembly;
        std::string c;
    };
    std::function<InterruptCode(const std::set<int>& vectors)> interrupt_routines;
};

// OpenWatcom's 32-bit tools, and the knowledge of formats the link needs.
struct Tools32 {
    Compiler wcc;
    std::filesystem::path wlink;
    std::filesystem::path wasm;                      // assembles the jumps into the original
    std::vector<std::string> link_head;              // the link lines that follow `option undefsok` (the base address ...)
    std::vector<std::string> runtime_lines;          // the link lines for the C runtime
    // The link lines (import statements) an object file needs for the imports its code calls.
    std::function<std::vector<std::string>(ByteView object)> import_lines;
    // The names an OMF object file defines (its PUBDEFs), in the order it lists them.
    std::function<std::vector<std::string>(ByteView object)> publics;
    // The image base of the linked DLL and its exports {name: RVA}.
    struct PeExports {
        std::uint64_t image_base = 0;
        std::map<std::string, std::uint32_t> rvas;
    };
    std::function<PeExports(const std::filesystem::path& dll)> exports;
};

// The Xbox 360 SDK's PowerPC compiler and linker.
struct PowerPcTools {
    std::filesystem::path cl, link, lib;
};

struct Import16 {
    std::string module;
    std::uint32_t ordinal = 0;
};

struct SegmentOffset {
    std::uint16_t segment = 0;
    std::uint32_t offset = 0;
    friend bool operator==(const SegmentOffset&, const SegmentOffset&) = default;
};

struct Build16Input {
    std::string header;
    const Texts& texts;
    std::vector<std::string> order;
    std::map<std::string, Import16> imports;          // {symbol: (module, ordinal)}, imported by name sorted
    std::filesystem::path outdir;
    std::map<int, Bytes> segments;                    // {segment: bytes}: other segments the code reads from
    std::optional<int> jobs;
    // {name: the number of the original's segment the function is in}: the functions of one segment are put in one
    // segment again. A segment holds 64K at most, and functions of one segment call each other near, of different ones
    // far -- as in the original.
    std::map<std::string, int> code_segment;
    std::set<int> interrupts;                         // the software interrupts the texts take
    // {segment: C}: the original's data its functions read through CS, compiled into that code segment.
    std::map<int, std::string> code_data;
    // {segment: bytes}: the original's whole segment, put at the start of the rebuilt one -- for C that still reads its
    // code segment at the original's offsets, so that what it reads there is what the original has; the functions come
    // after it. Left out again when the segment would not fit.
    std::map<int, Bytes> code_image;
};

struct Build32Input {
    std::string header;
    const Texts& texts;
    std::vector<std::string> order;
    std::filesystem::path outdir;
    std::vector<std::string> cflags;                  // the module's
    std::optional<int> jobs;
    // {name: address} in the original: where a function the build lacks is jumped to (the original's code is in memory
    // beside the rebuilt image whenever that is run).
    std::map<std::string, std::uint64_t> originals;
};

// What a 16-bit build answers: the DLL (nullopt: nothing was linked), where each function is, and why each rejected
// one was (a link that failed gives every function that was not rejected the linker's reason).
struct Built16 {
    std::optional<std::filesystem::path> dll;
    std::map<std::string, SegmentOffset> where;
    Reasons dropped;
};
struct Built32 {
    std::optional<std::filesystem::path> dll;
    std::map<std::string, std::uint64_t> where;
    Reasons dropped;
};
using BuiltPowerPc = Built32;

struct BuilderOptions {
    BuildCache* cache = nullptr;           // not owned; may be null
    parallel::WorkerPolicy workers;        // how many compiles run at once, unless a build says
};

// What the engine already made (function.object, function.never_set): while the link runs, compile() and never_set()
// of a function with the header, options and text such a fact was made from answer it, and the compiler is not started.
struct ServedObject {
    std::string error;   // the compiler's first error (empty when it made the object)
    Bytes object;        // the object, when there is no error
};
struct ServedCounts {
    std::int64_t served = 0;
    std::int64_t compiled = 0;
};

class FunctionBuilder {
public:
    explicit FunctionBuilder(BuilderOptions options = {});
    FunctionBuilder(const FunctionBuilder&) = delete;
    FunctionBuilder& operator=(const FunctionBuilder&) = delete;
    ~FunctionBuilder();

    // ---- the header a folder is compiled against ----
    // The key of a header's text (what compiles in a folder are remembered under).
    static std::string header_digest(std::string_view header_text);
    // Write <outdir>/m.h and note its key.
    void write_header(const std::filesystem::path& outdir, std::string_view header_text);
    // Note the key of a header written by the caller.
    void set_header_key(const std::filesystem::path& outdir, std::string key);
    std::optional<std::string> header_key(const std::filesystem::path& outdir) const;

    // ---- compiling one function ----
    // What a compiler reads besides its command line and the source, by content: the program's bytes, the headers it
    // may include (every file of the INCLUDE folders) and the environment variable of options it reads too (OpenWatcom's
    // WCC, WASM). A cache key; worked out once per compiler.
    std::string tool_inputs(const Compiler& cc, std::string_view options_variable);
    // Compile one function: <outdir>/<stem>.obj, or the compiler's first error ("" on success). What the engine served
    // answers first; then what the cache has; then the compiler runs (and what it makes is kept).
    std::string compile(const Compiler& cc, std::span<const std::string> cflags, const std::filesystem::path& outdir,
                        std::string_view stem, std::string_view text);
    // run_compile, kept in the cache (kind "objects"). `cflags` are already adjusted. The key is every input of the
    // object: the compiler, its INCLUDE headers and its WCC variable (tool_inputs), the options, the header m.h the
    // function includes (`header_key`), the function's text, and the stem (the object names its source file, stem.c,
    // and an error names it too). The object also carries m.h's date (the compiler's dependency record), which is not an
    // input of what is linked: a kept object has the date of the first compile. Without a header key nothing is kept.
    CompileOutcome compile_kept(const Compiler& cc, std::span<const std::string> cflags, const std::filesystem::path& outdir,
                                std::string_view stem, std::string_view text, const std::optional<std::string>& header_key);
    // run_never_set, answered from what the engine served, then kept in the cache (kind "never_set").
    std::vector<std::string> never_set(const Compiler& cc, std::span<const std::string> cflags,
                                       const std::filesystem::path& outdir, std::string_view stem, std::string_view text);
    std::vector<std::string> never_set_kept(const Compiler& cc, std::span<const std::string> cflags,
                                            const std::filesystem::path& outdir, std::string_view stem, std::string_view text,
                                            const std::optional<std::string>& header_key);

    // ---- what the engine already made ----
    // The key a fact is served under: the header key, the options and the text it was made from.
    static std::string served_key(std::string_view header_key, std::span<const std::string> cflags, std::string_view text);
    // Offer objects (by served_key) and flow checks to the next compiles; calls nest, and the last unserve() takes it all
    // back.
    void serve(const std::map<std::string, ServedObject>& objects,
               const std::map<std::string, std::vector<std::string>>& never_sets);
    void unserve();
    ServedCounts served_counts() const;

    // ---- every function ----
    // Compile every function of `order` into `outdir`. `more` gives switches of a function's own.
    Compiled compile_all(const Compiler& cc, std::span<const std::string> cflags, std::string_view header, const Texts& texts,
                         std::span<const std::string> order, const std::filesystem::path& outdir,
                         std::optional<int> jobs = std::nullopt, const ExtraFlags& more = {});
    // compile_all for several folders at once, every function of every folder in the one pool, so that a segment of a
    // few functions does not leave the compilers idle while the largest ends. One Compiled for each group.
    std::vector<Compiled> compile_groups(const Compiler& cc, std::string_view header, const Texts& texts,
                                         std::span<const Group> groups, std::optional<int> jobs = std::nullopt,
                                         const ExtraFlags& more = {});

    // ---- building a DLL ----
    // Throws UsageError when the compiler or linker is not set up (exe paths empty or missing).
    Built16 build16(const Tools16& tools, const Build16Input& in);
    Built32 build32(const Tools32& tools, const Build32Input& in);
    // `externs` are the names of functions outside the module (imports): each gets a place of its own in the image, so a
    // call to it can be seen when the image is run. The result's `where` has the functions and the externs.
    BuiltPowerPc build_powerpc(const PowerPcTools& tools, std::string_view header, const Texts& texts,
                               std::span<const std::string> order, std::span<const std::string> externs,
                               const std::filesystem::path& outdir, std::uint64_t base, std::optional<int> jobs = std::nullopt);

    // The digest of each function's object ("stand-in:" in front of a stand-in's, "the original's" for a function left to
    // the original) in the last build of a folder; nullopt for a folder not built.
    std::optional<std::map<std::string, std::string>> object_digests(const std::filesystem::path& outdir) const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::build::functions
