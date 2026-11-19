// cb/decompile/decompiler.hpp -- CodeBreaker's decompiler: a module's functions as C.
//
//     Decompiler decompiler(context);                              // what it runs, what it keeps, what it asks of the rest
//     AsC c = decompiler.as_c("lz32.dll");                         // any executable the readers know, as the text of one .c file
//     c_text::Compilable32 m = decompiler.compilable32(path);      // a 32-bit module made compilable (c_text, with this as its Services)
//
// One decompiler for everything. The native program (native/decompiler) knows processors, not file formats: this module
// takes the file apart with the readers in cb/formats, describes it (decompile/describe), runs the native program on the
// description, takes what it said of each function besides its C (`/*@G ... */` notes: split_notes) and, for the lanes that
// ask for it, has the L2 library's C back end write each function's C from its L2 document (c_from_l2).
//
// The native program is run in processes of 64 functions each, as many at once as the machine's slots and memory carry, and
// the C it wrote is kept (base/cache) under a hash of everything it depends on: the program, its language files, the
// description (by what the files it names hold, not where they are) and, for a module described again with some functions
// changed, each function's own lines -- so only those are made again. A process that dies part way is started again from the
// next function it had not written; a function it dies on at once is left out and said on stderr.
//
// `Decompiler` is also the `c_text::Services` that c_text asks for the decompiler proper (the native program's output, a 16-bit
// module decompiled in rounds, a function's C from its L2 document) and, through `Environment`, for the symbol tables and
// readers that are not part of this module (what an API pops from the stack, the structures recovered for a module, the
// PowerPC reader, the L2 library itself). It owns the arena the L2 documents it makes live in; the documents of the results
// point into it and the results keep it alive.
//
// The switches that were environment variables are the fields of `Policy` (`from_environment` reads them); the settings
// (the native program, its languages, struct_recovery, threads) are `Settings`.
#pragma once

// <windows.h> defines `near` and `far` as empty macros; this header and what it includes use them as names.
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

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
#include <vector>

#include "cb/base/cache.hpp"
#include "cb/base/config.hpp"
#include "cb/base/converted.hpp"
#include "cb/base/corpus.hpp"
#include "cb/base/parallel.hpp"
#include "cb/core/arena.hpp"
#include "cb/core/bytes.hpp"
#include "cb/core/json_fwd.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/core16_types.hpp"
#include "cb/decompile/c_text.hpp"
#include "cb/decompile/describe.hpp"
#include "cb/decompile/struct_text.hpp"
#include "cb/formats/pe.hpp"
#include "cb/translate/ir.hpp"
#include "cb/translate/native_l2.hpp"

namespace cb::decompile::decompiler {

using core16::FunctionNotes;
using Va = std::int64_t;

// Bump when what is fed to / taken from the native program changes: results kept under another revision are not used.
inline constexpr int kRevision = 16;
// Instructions a function may have, tried again when the first limit stops it.
inline constexpr std::int64_t kLargeInstructionLimit = 200000;
// Functions to a process of the native program, and what the governor counts for each (the program's own budget).
inline constexpr std::size_t kChunk = 64;
inline constexpr int kProcessMegabytes = 1536;

// ---- what it is run with ------------------------------------------------------------------------------------------------

// The native program and the settings that decide what it is asked.
struct Settings {
    std::filesystem::path checkout;   // the checkout the program belongs to (the project's own data files)
    std::filesystem::path program;    // decompiler.exe
    std::filesystem::path languages;  // its language files (CB_LANGUAGES_DIR names another folder: an older decompiler's)
    std::string struct_recovery = "all";   // "off", "32" (32-bit x86 only) or "all"
    std::string threads;              // functions analysed at once in one process, by hand; empty: processes share the functions
    // `<settings decompiler_bin>/decompiler.exe`, CB_LANGUAGES_DIR else `<checkout>/native/decompiler/languages`, and the settings
    // "struct_recovery" and "threads".
    static Settings from_config(const config::Config& config);
};

// The switches of the module (each was an environment variable).
struct Policy {
    bool frame16 = false;          // CB_FRAME16=1: a 16-bit function's locals laid out as the original's frame (frame16, shape16)
    bool asm_blocks = false;       // CB_ASM_BLOCKS set: a 16-bit function's _asm blocks placed (asm_blocks)
    bool c_from_l2 = false;        // CB_C_FROM_L2 set: the lanes that do not always ask have each function's C written from its L2
    bool l2_requested = false;     // CB_L2 set: the native program is asked for the L2 documents whatever the lane
    static Policy from_environment();
    // Whether a 16-bit function's L2 tree is given the passes that read its code.
    bool tree_images() const { return asm_blocks || frame16; }
};

// A function of a program that is the compiler's own library, not the program's authors': what it is called and which library has it.
struct LibraryFunction {
    std::string name;      // as C: "strcpy"
    std::string library;   // "Microsoft C 4.0: large-model C runtime (LLIBC.LIB)"
};

// What the decompiler asks of the rest of CodeBreaker that is not part of it: the symbol tables, readers and the L2 library.
// (The shim over the old ports answers some of them; the rest of CodeBreaker's modules are being moved to cb/ one at a time.)
class Environment {
public:
    virtual ~Environment() = default;
    // What describing a module asks of the symbol tables (the API argument sizes, ordinals' names ...).
    virtual describe::Knowledge& knowledge() = 0;
    // The L2 core library (the C back end).
    virtual const translate::native_l2::Library& library() = 0;
    // {C name: DLL} of what a PE module imports by name.
    virtual std::map<std::string, std::string> import_dlls(const std::filesystem::path& module) = 0;
    // Bytes of arguments `dll`'s export removes from the stack; nullopt when not known.
    virtual std::optional<std::uint32_t> argument_bytes(std::string_view dll, std::string_view name) = 0;
    // The structures recovered for a 32-bit module (nullopt: the analysis cannot).
    virtual std::optional<std::vector<struct_text::Structure>> recover_structures(const std::filesystem::path& module) = 0;
    // Where a PowerPC image's bytes sit, which stubs are its imports, and what its run-time type information names.
    virtual describe::PowerPcFacts powerpc_facts(const std::filesystem::path& module) = 0;
    // The addresses a PowerPC image states for its functions (its .pdata table), ascending.
    virtual std::vector<Va> powerpc_functions(const std::filesystem::path& module) = 0;
    // The functions of a real-mode program that are a period compiler's library (the C runtime), by signature: {address: what}.
    // `starts` are the function starts the analysis found, as linear addresses. Nothing identified by default.
    virtual std::map<Va, LibraryFunction> dos_library(const formats::dos::Program& program, std::span<const Va> starts) {
        (void)program;
        (void)starts;
        return {};
    }
    // A PE module has been described (what its description says of it: the names it gave its functions, those that take their first
    // argument in ECX, the imports by number). For whatever else wants to know what the description said.
    virtual void described(const std::filesystem::path& module, const describe::PeDescription& description) {
        (void)module;
        (void)description;
    }
};

// Everything a decompiler works with. `cache` may be null (nothing is kept); `environment` is required and outlives it.
struct Context {
    Settings settings;
    Policy policy;
    base::cache::Cache* cache = nullptr;
    Environment* environment = nullptr;
    base::budget::Limits limits;      // how much of the machine the native programs may take
    std::filesystem::path scratch;    // where a module's description is written when no folder is given ("" : build scratch)
};

// ---- the native program's output ----------------------------------------------------------------------------------------

// A block of the native program's output: the line `/*@@ <name> <hex address> */` and the text after it, up to the next such line.
struct Block {
    std::string name;
    Va va = 0;
    std::size_t start = 0;   // where the marker line starts
    std::size_t body = 0;    // where its text starts (after the line's newline)
    std::size_t end = 0;     // where the next marker line starts, or the end
};
std::vector<Block> scan_blocks(std::string_view output);

// What the native program says of a function besides its C is on lines of their own before it (decompiler.cc decompileOne):
//     /*@G <va> <bytes> <bytes pointed at> */          a global it touches        -> globals
//     /*@P <name> <bytes> */                           a pointer and its size     -> pointers
//     /*@S <name> <bytes> <off>:<bytes>:<type>... */   a structure it recovered   -> structs
//     /*@L2 {...} */                                   its L2 document (CB_L2)    -> l2 (a node of `arena`)
// The C is what follows them. A note that is not well formed is a FormatError.
struct Split {
    std::string text;
    FunctionNotes notes;
};
Split split_notes(Arena& arena, std::string_view text);

// What a run of the native program gave, function by function.
struct Run {
    OrderedMap<std::string, std::string> texts;   // C alone (the notes taken off), by name
    std::vector<std::string> order;
    std::map<Va, std::string> by_va;
    std::string stderr_text;
    std::map<std::string, FunctionNotes> notes;   // what it said of each besides the C
    std::shared_ptr<Arena> arena;                 // the notes' L2 documents are nodes of it
};

struct RunOptions {
    std::optional<std::int64_t> max_instructions;   // a function longer than this is given up (the program's own default: none)
    // Each function's C is the C back end's, from its L2 document. nullopt: the policy says (c_from_l2).
    std::optional<bool> l2;
    // false: the L2 documents are kept and the printed C is handed on (a module writes its C from them once it is whole).
    bool write = true;
};

// ---- the answers of the lanes -------------------------------------------------------------------------------------------

struct Functions {
    OrderedMap<std::string, std::string> texts;
    std::vector<std::string> order;
    std::map<Va, std::string> by_va;
};

// One virtual device driver of a file: its name, its functions, how it was described.
struct LeMember {
    std::string member;
    Functions functions;
    std::shared_ptr<describe::LeDescription> description;
};

struct DosProgramC {
    Functions functions;
    std::vector<LibraryFunction> library;   // the functions that are the compiler's library: named, called by name, not decompiled
    std::string header;                     // the `#include`s they come with
    std::shared_ptr<describe::DosDescription> description;
};

// An executable as the text of one .c file: the functions decompiled, and the functions found.
struct AsC {
    std::string text;
    std::size_t decompiled = 0;
    std::size_t found = 0;
    std::size_t library = 0;   // of `found`: identified as the compiler's library and left out of the text
};

// ---- the decompiler -----------------------------------------------------------------------------------------------------

class Decompiler final : public c_text::Services {
public:
    explicit Decompiler(Context context);
    Decompiler(const Decompiler&) = delete;
    Decompiler& operator=(const Decompiler&) = delete;

    const Context& context() const { return context_; }

    // The native program run on a description (any processor it has a language for); `only` limits it to those addresses.
    // Each function's C is kept on its own with the lines of the description it was made from, so a description changed in
    // part makes again only the functions it touches.
    Run run(const std::filesystem::path& description, std::span<const Va> only = {}, const RunOptions& options = {});

    // The version of what makes decompiled C, as other caches key on it: the revision, the program, its languages.
    struct Generation {
        int revision = kRevision;
        std::string program;     // the program's identity (name, size, date)
        std::string languages;   // the language files' (CB_LANGUAGES_DIR does not change it: a comparison run shares it)
    };
    Generation generation() const;
    // The language files' folder digest.
    const std::string& language_stamp() const;
    // The native program and its languages are there.
    bool available() const;

    // Whether the native program recovers structures for this description ("1") or not ("0"): the setting `struct_recovery`, and
    // for "32" the description's architecture.
    std::string structs_for(const std::filesystem::path& description) const;

    // A 16-bit module decompiled in rounds (function starts come first from what the file states; the calls to places not yet
    // known as functions found in each round are added to the description and the module is done again).
    c_text::Decompiled16 decompile16(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir, int rounds);

    // A PE module (x86 or PowerPC) described and run: `starts` its functions, `outdir` where the description is written.
    struct ModuleRun {
        Run run;
        std::map<std::string, describe::NumberedImport> numbered;   // imports by number, as the description named them
    };
    ModuleRun run_module(const std::filesystem::path& module, std::span<const Va> starts, const std::filesystem::path& outdir,
                         const RunOptions& options = {});

    // A document of the L2 library's (JSON) made a node of this decompiler's arena.
    translate::ir::Node* node_of(const Json& l2_document);

    // ---- c_text::Services: the decompiler proper
    c_text::Decompiled raw(const std::filesystem::path& module, std::span<const Va> functions,
                           const std::filesystem::path& outdir) override;
    c_text::Decompiled16 decompile16(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir) override;
    std::optional<std::string> c_from_l2(const std::optional<std::string>& text, FunctionNotes& notes,
                                         const std::optional<c_text::SegmentImages>& images, c_text::ModuleFacts* module) override;
    bool recovers_structures(const describe::NeDescription& description) override;
    bool tree_images() override { return context_.policy.tree_images(); }
    c_text::SegmentImages segment_images(const describe::NeDescription& description) override;
    // ---- c_text::Services: the symbol tables and readers
    const translate::native_l2::Library& library() override { return context_.environment->library(); }
    std::map<std::string, std::string> import_dlls(const std::filesystem::path& module) override {
        return context_.environment->import_dlls(module);
    }
    std::optional<std::uint32_t> argument_bytes(std::string_view dll, std::string_view name) override {
        return context_.environment->argument_bytes(dll, name);
    }
    std::optional<std::vector<struct_text::Structure>> recover_structures(const std::filesystem::path& module) override {
        return context_.environment->recover_structures(module);
    }
    describe::PowerPcFacts powerpc_facts(const std::filesystem::path& module) override { return context_.environment->powerpc_facts(module); }

    // ---- the lanes
    // A 32-bit module made compilable. No functions given: those the carve finds. No folder: the build's scratch.
    c_text::Compilable32 compilable32(const std::filesystem::path& module, const std::optional<std::vector<Va>>& functions = std::nullopt,
                                      const std::optional<std::filesystem::path>& outdir = std::nullopt);
    // A virtual device driver file: one LE image, or every member of a container.
    std::vector<LeMember> decompile_le(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir = std::nullopt);
    // A DOS program.
    DosProgramC decompile_dos(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir = std::nullopt);
    // Any executable as C. UsageError for a file that is not an executable this can read.
    AsC as_c(const std::filesystem::path& module);

    // The arena the L2 documents of everything this made are nodes of.
    const std::shared_ptr<Arena>& arena() const { return arena_; }

    // Told how many functions the native program has finished of how many it was given (a window's progress bar): once at the start
    // (0 of n), after each chunk of kChunk functions, and at the end. Called from the thread that runs the decompiler; none by default.
    void set_progress(std::function<void(std::size_t done, std::size_t total)> progress) { progress_ = std::move(progress); }

private:
    struct NativeOutput {
        std::string out, err;
        bool whole = true;   // every function was attempted
    };

    // ---- the native program (decompiler.cpp)
    NativeOutput resuming(const std::filesystem::path& description, bool first_has_at, std::map<std::string, std::string> env,
                          std::vector<Va> left) const;
    NativeOutput run_native(const std::filesystem::path& description, std::span<const Va> only, std::optional<std::int64_t> max_instructions,
                            bool l2) const;
    struct Texts {
        OrderedMap<std::string, std::string> texts;
        std::vector<std::string> order;
        std::map<Va, std::string> by_va;
        std::string err;
    };
    Texts run_whole(const std::filesystem::path& description, std::span<const Va> only, std::optional<std::int64_t> max_instructions,
                    bool l2) const;
    Texts run_functions(const std::filesystem::path& description, std::span<const Va> only, std::optional<std::int64_t> max_instructions,
                        bool l2) const;
    std::vector<base::cache::KeyPart> l2_key(bool l2) const;

    // The folder a module's description is written in when none is given: under the context's scratch, else the build's.
    std::filesystem::path scratch_for(std::initializer_list<std::string_view> parts) const;

    // A PE module described to the native program (a PowerPC image with what its reader says of it).
    describe::PeDescription describe_module(const std::filesystem::path& module, const formats::pe::Image& image, std::span<const Va> starts,
                                            const std::filesystem::path& outdir);

    Context context_;
    std::shared_ptr<Arena> arena_ = std::make_shared<Arena>();
    std::mutex arena_lock_;
    std::function<void(std::size_t, std::size_t)> progress_;
    std::once_flag kinds_once_;
    mutable std::mutex stamp_lock_;
    mutable std::optional<std::string> language_stamp_;
};

// {segment: bytes} of a 16-bit module's code, from its description's `bytes` lines.
c_text::SegmentImages images_in_description(const std::filesystem::path& description);

// ---- what a 16-bit function's L2 document says (the rounds of decompile16 read it) ----------------------------------------

// The addresses of places a function calls that are not known as functions yet (`func_0x<8 hex>`): from its L2 document's
// calls, or without one (null) from its text.
std::set<Va> placeholders(std::string_view text, translate::ir::Node* doc);
// The number of arguments each call to `name` passes: in its L2 document's calls, or without one in `text`.
std::vector<std::int64_t> call_arguments(std::string_view text, std::string_view name, translate::ir::Node* doc);
// Does the function call `name`?
bool calls(std::string_view text, translate::ir::Node* doc, std::string_view name);
// Does the function name `name` anywhere (L2: among its body's names)?
bool mentions(std::string_view text, translate::ir::Node* doc, std::string_view name);

// The bytes of each of the arguments a function declares, `"2"`, `"4"`, or `"4p"` for a far pointer -- from its L2 document
// (null: from its own text) -- and how many; nullopt when the declaration is not found. `pointers` are the notes' pointers
// (which of them are four bytes).
struct ArgumentSizes {
    std::vector<std::string> sizes;
    friend bool operator==(const ArgumentSizes&, const ArgumentSizes&) = default;
};
std::optional<ArgumentSizes> own_arguments(const translate::native_l2::Library& library, std::string_view text, std::string_view name,
                                           translate::ir::Node* doc, const OrderedMap<std::string, std::int64_t>* pointers);

// A text with every `func_0x<8 hex>` that `known` ({8 hex: name}) names replaced by the name.
std::string by_their_names(std::string_view text, const std::map<std::string, std::string>& known);

// ---- the command --------------------------------------------------------------------------------------------------------

// `codebreaker decompile <module> [--out FILE]`: decompile any module the corpus or a path names -- a 32-bit or 16-bit Windows
// module, a virtual device driver, a DOS program, a PowerPC image, a NES cartridge -- and write its functions as C. Nothing is
// built or judged. Without --out the C goes to the binary's project folder (Converted/<name>/src/). Returns the exit code
// (0, or 2 for no arguments; a module that is not found is NotFoundError).
int command(Decompiler& decompiler, const base::Corpus& corpus, const base::converted::Store& converted, std::span<const std::string> args);

}  // namespace cb::decompile::decompiler
