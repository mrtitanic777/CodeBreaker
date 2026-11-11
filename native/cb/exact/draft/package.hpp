// cb/exact/draft/package.hpp -- functions written by an inverse model of the compiler, kept only when the compiler
// proves them: the stage itself.
//
//     codebreaker exact PROJECT draft [--all] [--limit N] [-v]
//
// For every function carve finds that has no src/fADDR.c (--all: every function, the written ones left as they are),
// the toolchain profile's inverse model (a compiler's code generation run backwards: recover the statements from the
// bytes, print them as C) reads the function's bytes back into C. The text is made a source file of the project's
// kind:
//
//   - the places it names get the engine's names: data g_XXXX (DS:XXXX), functions fXXXXX (their image offset), the
//     library's routines by their names (the library stage's), each declared
//   - what the model leaves of the machine (a register it could not fold into an expression) makes the draft unfit: it
//     is not compiled
//
// and compiled by the project's toolchain and held to the program (exact/compare). Identical: written as
// src/fADDR.c. Otherwise it is kept in src/drafts/fADDR.c with what was wrong, for a person or a later model - the
// ledger never counts a draft.
//
// The package, each module using only those above it:
//
//     text         reading a draft's text: an if's parts, a block's ends, a test negated, a register read later
//     shapes       control flow read back into statements: returns, loops and their tests, switches, gotos
//     forms        values and places read back: registers folded, steps and stores joined, pointers, macros
//     candidates   the model's text made the project's source, and every rewrite applied to it
//     prove        a candidate compiled and held to the program; the beam search over the rewrites
//     package      the model's texts of a function, and the stage itself (this file)
//
// What the stage asks of the rest of CodeBreaker is declared here as interfaces (`InverseModel`, `Project`): the
// inverse models (invert/*), the project, the library stage and the prover (draft/prove) are refactored in later
// waves and implement them.
#pragma once

#include <any>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/exact/draft/text.hpp"
#include "cb/exact/symbols.hpp"
#include "cb/exact/x86.hpp"

namespace cb::exact::draft {

// ---- the inverse model ----------------------------------------------------------------------------------

// Where a switch's jump table sends each value (addresses in the function's own code, from 0), asked for the
// address of the indirect jump; nothing when the table cannot be read.
using SwitchHook = std::function<std::optional<std::vector<std::int64_t>>(std::uint32_t address)>;

// What the model reads of the function besides its text (the model's own record of the last recovery).
struct ModelLast {
    OrderedMap<std::string, int> regparam;       // register -> the index of the parameter it carries
    std::map<std::string, std::string> fptypes;  // the 8087's places -> their types
    std::optional<std::string> fpret;
    std::set<std::string> unsigned_longs;
};

// The model's recovery of a function: the frame's size, the parameter names, the statements, and the model's own
// tables (opaque here: only handed back to `to_c`).
struct Recovery {
    std::uint32_t frame_size = 0;
    std::vector<std::string> params, stmts;
    std::any model_state;
};

// What the model is told before a recovery: how it reads the code it is given.
struct Reading {
    bool frame_after_bp = false;   // a draft is of 3.00-or-later code: its -Gs frames
    bool regvars_eager = false;    // ... its register variables
};

// What a structuring recovery is asked for (the model's mutable switches of the original, made arguments).
struct Structuring {
    bool compare_switches = false;   // switches by compares read as switches
    bool fp_casts = false;           // the 8087's fields at their own widths: read again with casts
    bool unsigned_compares = false;  // compares without sign marked
    SwitchHook table_switch;         // where a switch's table sends each value (empty: not asked)
};

// Which of the optional parts of the interface a model has.
struct ModelCapabilities {
    bool frame_after_bp = false, regvars_eager = false;   // understands `Reading`
    bool structured = false;                              // has recover_cfg
    bool table_switch = false, fp_casts = false, unsigned_compares = false;   // understands those of `Structuring`
    bool fold_ladders = false;
};

// A compiler's inverse model (a module of invert/*).
class InverseModel {
public:
    virtual ~InverseModel() = default;
    virtual ModelCapabilities capabilities() const = 0;
    // A linear walk of the code. Throws when the model declines.
    virtual Recovery recover(ByteView code, const Reading& reading) = 0;
    // The structured control-flow graph's recovery (only if capabilities().structured).
    virtual Recovery recover_structured(ByteView code, const Reading& reading, const Structuring& how) = 0;
    // The recovery printed as a C function. `r` may have had its parameters and statements adjusted by the caller.
    virtual std::string to_c(std::string_view name, const Recovery& r) = 0;
    // Nested ifs of a long compared the compiler's way folded into one comparison (only if capabilities().fold_ladders).
    virtual std::vector<std::string> fold_ladders(const std::vector<std::string>& stmts) = 0;
    // What the last recovery read.
    virtual ModelLast last() const = 0;
};

// ---- reading the model's text ---------------------------------------------------------------------------

// A jump-table switch for the model (`jmp cs:[bx + T]`, after `cmp R, N`: N + 1 words at T): addresses in the
// function's own code, from 0. T is the program's offset of the table, so the function's own offset is found where
// its code begins being the program's: `base` is that offset.
class TableHook {
public:
    TableHook(ByteView code, std::uint32_t base);
    std::optional<std::vector<std::int64_t>> operator()(std::uint32_t address) const;

private:
    Bytes code_;
    std::uint32_t base_;
    std::vector<x86::Insn> insns_;
};

// The code with the floating-point emulator's calls (int 34h..3Bh: an 8087 instruction, its first byte D8h + n; int
// 3Ch: one with a segment; int 3Dh: fwait) written as the 8087's own - each as long as it was (a wait before it), so
// the model reads them as the instructions they stand for.
Bytes fp_native(ByteView code);

// An arm that is only `goto L;`, L the start of statements to the end of their block: those statements in its place
// (the compiler's one copy of a tail two arms share); L dropped when nothing goes to it then.
std::vector<std::string> inline_goto_arms(std::vector<std::string> lines);

// One text of a function, and the register parameters the model found for it.
struct DraftText {
    std::string text;
    OrderedMap<std::string, int> regparam;   // register -> the index of the parameter it carries
};

// What the model read of the 8087's values: types by place, the return, whether the code has any.
struct FpInfo {
    std::map<std::string, std::string> types;
    std::optional<std::string> ret;
    bool fp = false;
};

// The texts of a function and what the later passes need to know of how they were made (they were module-level
// tables of the original: the frame, the 8087's values, the longs).
struct ModelTexts {
    std::vector<DraftText> texts;
    std::uint32_t frame_size = 0;
    FpInfo fp;
    text::LongInfo longs;
};

// The model's text of a function, from each of its structurers (a linear walk; a structured control-flow graph,
// where the model has one): the compiler decides between them. `base` (the function's image offset) lets the
// structured walk read jump tables. Throws what the model throws when even the linear walk declines.
ModelTexts model_texts(InverseModel& model, ByteView code, std::string_view name, std::optional<std::uint32_t> base = std::nullopt);

// ---- the stage ----------------------------------------------------------------------------------------------

// A library module's public names in its first code segment (what the library stage found).
struct LibraryModule {
    OrderedMap<std::string, std::pair<std::uint32_t, std::uint32_t>> publics;   // name -> (segment, offset)
    std::uint32_t first_code_segment = 0;
};
struct LibraryPlace {
    std::uint32_t code = 0;               // where the module's code is in the image
    LibraryModule module;
    std::vector<LibraryModule> also;      // more modules the same code serves
};

struct ProveOutcome {
    std::string source;
    std::string verdict;    // "identical", or what was wrong
};

// What the stage asks of the project.
class Project {
public:
    virtual ~Project() = default;
    virtual std::string toolchain_name() const = 0;
    virtual std::optional<std::string> inverse_model_name() const = 0;               // the profile's "inverse"
    virtual std::shared_ptr<InverseModel> load_inverse_model(std::string_view name) = 0;
    virtual std::vector<LibraryPlace> library_places() const = 0;                    // the library stage's: coded, then lone
    virtual const SymbolTable& symbols() const = 0;
    virtual ByteView image() const = 0;
    virtual std::set<std::uint32_t> source_addresses() const = 0;                    // functions that already have a source
    virtual std::filesystem::path path(std::string_view name) const = 0;             // a file in the project's src
    virtual std::vector<std::pair<std::uint32_t, std::uint32_t>> functions() const = 0;   // (address, length) of each function carve finds
    // The texts made the project's source, compiled and held to the program.
    virtual ProveOutcome prove_texts(InverseModel& model, const ModelTexts& texts, std::uint32_t address, std::uint32_t length,
                                     const std::map<std::uint32_t, std::string>& names) = 0;
};

// {image offset: name} of the code the program has names for (the library's modules, SYMBOLS).
std::map<std::uint32_t, std::string> code_names(const Project& project);

struct Options {
    bool every = false;                       // --all: every function, the written ones left as they are
    std::optional<std::size_t> limit;         // --limit N
    bool verbose = false;                     // -v: a line for each function
};

// {address: verdict} in the order the functions were drafted.
using Verdicts = OrderedMap<std::uint32_t, std::string>;

// The stage: each function drafted, proved, and written to src/ (identical) or src/drafts/ (with what was wrong). A
// function the model declines, or a pass fails on, gets that as its verdict; it does not stop the run. Throws
// UsageError when the toolchain names no inverse model.
Verdicts run(Project& project, const Options& options);

// The command: run, then the summary line "N functions drafted: K verdict, ...". Returns 0.
int run_command(Project& project, const Options& options);

}  // namespace cb::exact::draft
