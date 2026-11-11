// codebreaker/exact/draft/package.hpp -- typed C++ from codebreaker/exact/draft/__init__.py.
//
// draft -- functions written by an inverse model of the compiler, kept only when the compiler proves them.
//
//     python -m codebreaker exact PROJECT draft [--all] [--limit N] [-v]
//
// For every function carve finds that has no src/fADDR.c (--all: every function, the written ones left as they are),
// the toolchain profile's inverse model ("inverse": a module of codebreaker.invert with recover(code) and to_c(...))
// reads the function's bytes back into C. The text is made a source file of the project's kind:
//
//   - the places it names get the engine's names: data g_XXXX (DS:XXXX), functions fXXXXX (their image offset), the
//     library's routines by their names (the library stage's), each declared
//   - what the model leaves of the machine (a register it could not fold into an expression) makes the draft unfit: it
//     is not compiled
//
// and compiled by the project's toolchain and held to the program (exact/compare.py). Identical: written as
// src/fADDR.c. Otherwise it is kept in src/drafts/fADDR.c with what was wrong, for a person or a later model - the
// ledger never counts a draft.
//
//     draft::run(project) -> {address: verdict}
//
// The package, each module using only those above it:
//
//     text.py        reading a draft's text: an if's parts, a block's ends, a test negated, a register read later
//     shapes.py      control flow read back into statements: returns, loops and their tests, switches, gotos
//     forms.py       values and places read back: registers folded, steps and stores joined, pointers, macros
//     candidates.py  the model's text made the project's source, and every rewrite applied to it
//     prove.py       a candidate compiled and held to the program; the beam search over the rewrites
//     __init__.py    the model's texts of a function, and the stage itself (run, main)
//
// Typing and runtime notes (the modules this one uses that have no typed port are given here as interfaces):
//   * The inverse model is a Python module (`inv`) whose attributes the stage reads and sets (hasattr / getattr /
//     assignment): the abstract class InverseModel. An attribute the module may lack is a std::optional (absent: the
//     hasattr is false); `LAST` is the struct Last; `SWITCH` (a function or None, and present or not) is `has_switch` and
//     `SWITCH`. recover / recover_cfg / to_c / _fold_ladders are its virtual functions; what recover returns besides the
//     frame size, the parameter names and the statements (`pt`, `loc`: the model's own tables, only handed back to to_c) is
//     an opaque std::shared_ptr<void>.
//   * The project (`prj`) and the stages draft asks of it (exact.library.locate, exact.carve.functions,
//     draft.prove.prove_texts, prj.toolchain(), prj.symbols(), prj.sources(), prj.path()) are the abstract class Project;
//     importlib.import_module("codebreaker.invert." + model) is the std::function `inverse_model_loader`.
//   * candidates.FPINFO / candidates.FRAME and invert.flow16.CMPSWITCH, which this module sets, are FPINFO, FRAME and
//     flow16_cmpswitch here (those modules have no typed port yet; they belong there).
//   * `hook = table_hook(code); hook.base = ...` is a TableHook object; SWITCH takes it as a std::function.
//   * main(prj, argv) prints with py::print; `raise SystemExit(msg)` is py::SystemExit(msg).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::exact::draft {

// what the model read of the function besides its text (inv.LAST)
struct Last {
    py::dict<py::str, int64_t> regparam;        // register -> the index of the parameter it carries
    py::dict<py::str, py::str> fptypes;         // the 8087's places -> their types
    std::optional<py::str> fpret;
    py::set<py::str> unsigned_longs;
};

// (fs, params, stmts, pt, loc) of inv.recover / inv.recover_cfg
struct Recovery {
    int64_t fs = 0;
    py::list<py::str> params, stmts;
    std::shared_ptr<void> pt, loc;
};

// where a switch's jump table sends each value, or None (inv.SWITCH)
using SwitchHook = std::function<std::optional<py::list<int64_t>>(int64_t)>;

// a module of codebreaker.invert: the inverse model of a compiler
class InverseModel {
  public:
    virtual ~InverseModel() = default;
    virtual py::str module_name() const = 0;        // (for the AttributeError of an absent attribute)

    virtual Recovery recover(const py::bytes& code) = 0;
    bool has_recover_cfg = false;
    virtual Recovery recover_cfg(const py::bytes& code) = 0;
    virtual py::str to_c(const py::str& name, int64_t fs, const py::list<py::str>& params, const py::list<py::str>& stmts,
                         const std::shared_ptr<void>& pt, const std::shared_ptr<void>& loc) = 0;
    bool has_fold_ladders = false;
    virtual py::list<py::str> _fold_ladders(const py::list<py::str>& stmts) = 0;

    // the module's switches, written by the stage (absent: None of the optional)
    std::optional<bool> FRAME_AFTER_BP, REGVARS_EAGER, FPCASTS, UNSIGNED_CAST;
    bool has_switch = false;
    SwitchHook SWITCH;                                // (empty: None)
    Last LAST;                                        // (absent: the empty one)
};

// table_hook(code): the function SWITCH takes; `base` is set by the caller
class TableHook {
  public:
    int64_t base = 0;
    explicit TableHook(const py::bytes& code);
    std::optional<py::list<int64_t>> operator()(int64_t addr) const;
  private:
    py::bytes code_;
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

// SWITCH(address) for the model: the places a near jump through a table goes (`jmp cs:[bx + T]`, after `cmp R, N`: N + 1
// words at T) - addresses in the function's own code, from 0. T is the program's offset of the table, so the function's
// own offset is found where its code begins being the program's
std::shared_ptr<TableHook> table_hook(const py::bytes& code);

// the code with the floating-point emulator's calls (int 34h..3Bh: an 8087 instruction, its first byte D8h + n; int 3Ch:
// one with a segment; int 3Dh: fwait) written as the 8087's own - each as long as it was (a wait before it), so the model
// reads them as the instructions they stand for
py::bytes fp_native(const py::bytes& code);

// an arm that is only `goto L;`, L the start of statements to the end of their block: those statements in its place (the
// compiler's one copy of a tail two arms share); L dropped when nothing goes to it then
py::list<py::str> inline_goto_arms(const py::list<py::str>& lines);

// [(text, register parameters)] - the model's text of a function, from each of its structurers (a linear walk; a
// structured control-flow graph, where the model has one): the compiler decides between them
py::list<std::tuple<py::str, py::dict<py::str, int64_t>>> model_texts(InverseModel& inv, const py::bytes& code, const py::str& name,
                                                                      std::optional<int64_t> base = std::nullopt);

void _fpinfo(InverseModel& inv, const py::bytes& raw, const py::bytes& code);

// candidates.FPINFO: what the model read of the 8087's values (types by place, the return, whether the code has any)
struct FpInfo {
    py::dict<py::str, py::str> types;
    std::optional<py::str> ret;
    bool fp = false;
};
extern FpInfo FPINFO;
// candidates.FRAME: the one-element list [the frame's size]
extern py::list<int64_t> FRAME;
// invert.flow16.CMPSWITCH: switches by compares read as switches
extern bool flow16_cmpswitch;

// ---- the project and the stages draft asks of it ------------------------------------------------------------------------------
struct LibModule {
    py::dict<py::str, std::tuple<int64_t, int64_t>> pubs;     // name -> (segment, offset)
    int64_t first_code_segment = 0;                           // m.code[0][0]
};
struct LibPlace {
    int64_t code = 0;
    LibModule m;
    py::list<LibModule> also;
};
struct Located {
    py::list<LibPlace> coded, lone;
};

class Project {
  public:
    virtual ~Project() = default;
    virtual py::str toolchain_name() = 0;                     // prj.toolchain().name
    virtual std::optional<py::str> inverse_model() = 0;       // prj.toolchain().p.get("inverse")
    virtual Located locate() = 0;                             // exact.library.locate(prj): .coded, .lone
    virtual py::dict<py::str, std::tuple<py::str, int64_t>> symbols() = 0;     // {name: (type, address)}
    virtual py::bytes image() = 0;                            // prj.image
    virtual py::set<int64_t> source_addresses() = 0;          // {a for a, p in prj.sources()}
    virtual py::str path(const py::str& name) = 0;            // prj.path(name)
    virtual py::list<std::tuple<int64_t, int64_t, py::str>> functions() = 0;   // exact.carve.functions(prj)
    // draft.prove.prove_texts(prj, tc, inv, texts, a, ln, names) -> (source, verdict)
    virtual std::tuple<py::str, py::str> prove_texts(InverseModel& inv, const py::list<std::tuple<py::str, py::dict<py::str, int64_t>>>& texts,
                                                     int64_t a, int64_t ln, const py::dict<int64_t, py::str>& names) = 0;
};

// importlib.import_module("codebreaker.invert." + model)
extern std::function<std::shared_ptr<InverseModel>(const py::str&)> inverse_model_loader;

// {image offset: name} of the code the program has names for (SYMBOLS, the library's modules)
py::dict<int64_t, py::str> _names(Project& prj);

// draft.run(project) -> {address: verdict}
py::dict<int64_t, py::str> run(Project& prj, bool every = false, std::optional<int64_t> limit = std::nullopt, bool verbose = false);

int64_t main(Project& prj, const py::list<py::str>& argv);

}  // namespace codebreaker::exact::draft
