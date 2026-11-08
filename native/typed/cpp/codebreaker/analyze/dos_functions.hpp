// codebreaker/analyze/dos_functions.hpp -- typed C++ of codebreaker.analyze.dos_functions
//
// dos_functions.py -- where the functions of a real-mode program are, and what is code at all.
//
//     namespace df = codebreaker::analyze::dos_functions;
//     df::Analysis a = df::discover(dos::read(path));
//     a.functions        {linear address: Function}  .name .distance ("far"|"near") .removes .evidence .sure
//     a.interrupts       [Interrupt]  every `int` reached: .address .number .ah .al .function
//     a.tables           {address: number of entries}  jump tables read
//     a.census()         bytes of the program by what is known about them
//
//     python -m codebreaker dos-functions <program> [--all]      (main)
//
// A real-mode program has no table of its functions, and code and data share its segments, so
// function starts are established from what the program itself does, each with its evidence:
//
//   entry      the entry point; a device driver's strategy and interrupt routines
//   called     the target of a near or far call that is itself reached from a known function
//   vector     a handler the program installs: int 21h AH=25h with a constant DS:DX, or a pair of
//              stores into the interrupt table at 0000:4n
//   table      a target of `call [table + index]`; the targets of `jmp [table + index]` are
//              followed as part of the function that jumps
//   variable   the value the file gives a pointer variable that the program calls or jumps
//              through (`call [variable]`), where that decodes cleanly
//   pointer    the target of a far pointer in data (a relocated segment preceded by an offset)
//              into a segment known to hold code, where that decodes cleanly to a return
//   prologue   `push bp / mov bp, sp` or `enter n, 0` in bytes nothing reaches, decoding cleanly
//              to a return
//   follows    what stands directly behind a return or jump of code already found (routines
//              written in assembly lie back to back and have no prologue), where that decodes
//              cleanly to a return and does not read as text
//
// The first five follow the program's own control flow; the last three are guesses checked only
// for looking like code (`sure` is False for them and for what only they call), and they are kept
// apart in the census. Everything not reached stays "not classified": it may be data, or code
// reached only through a computed pointer.
//
// Whether a function is far or near, and how many bytes of arguments it removes, is read from
// its own return instructions, not from how it was found.
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * `import capstone` and `platforms.decoder(16, detail=True)` have no C++ counterpart (there is no port of
//     codebreaker.platforms).  The decoder is the very capstone.dll Python's `capstone` package carries, loaded
//     by path: hand it to set_capstone_dll() once (as analyze/native does); a use without it is an ImportError.
//     The .cpp includes capstone's own headers (the ones that package ships).
//   * The `_Ins` objects are the struct `Ins` (flow is an enum; ret is the optional (kind, removed bytes);
//     number / table / variable are std::optional<int64_t>).
//   * `regs` (a dict of register constants, plus the keys "pushed" and "table") is the struct `Regs`: a register
//     with no constant is absent from `consts`.
//   * Function objects are shared (the interrupt list and the function dict name the same ones), so they are
//     py::ref<Function>; a None function is the null ref.
//   * The Python's `_mark` / `_guess` bytearrays are private; _mark_bytes() / _guess_bytes() give them.
//   * A Program whose `ds` is None raises TypeError where the Python multiplies it (a table or pointer variable
//     through DS), as the Python does.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "codebreaker/formats/dos.hpp"
#include "py.hpp"

namespace codebreaker::analyze::dos_functions {

// Names the capstone.dll to decode with (the one Python's capstone package loads).
void set_capstone_dll(const py::str& path);

extern const py::list<int64_t> _PREFIX;
extern const py::dict<py::str, std::tuple<py::str, py::str>> _HALVES;
extern const py::dict<py::str, py::str> _HALF;
extern const py::list<py::str> _WHOLE;
extern const py::list<py::str> _VOLATILE;
extern const py::set<std::tuple<int64_t, std::optional<int64_t>>> _ENDS_PROGRAM;
// first bytes of instructions that real-mode programs hardly use and that text and tables decode to
extern const py::list<int64_t> _ODD;

class Function {
public:
    int64_t address;
    py::str name, distance, evidence;
    int64_t cs;
    bool sure;             // found by following the program from its entry points
    int64_t removes = -1;  // bytes of arguments its return removes; -1 not established
    bool interrupt = false;  // returns with iret
    int64_t size = 0;      // bytes of instructions reached from its start

    Function(int64_t address, const py::str& name, const py::str& distance, const py::str& evidence, int64_t cs, bool sure = true);
    py::str repr() const;  // Function(sub_00010 at 0x10, near, entry)
};

class Interrupt {
public:
    int64_t address, number;
    std::optional<int64_t> ah, al;
    py::ref<Function> function;   // null: None
    Interrupt(int64_t address, int64_t number, const std::optional<int64_t>& ah, const std::optional<int64_t>& al,
              const py::ref<Function>& function);
};

enum class Flow { Next, Call, Jump, Branch, Stop };
py::str flow_name(Flow f);

// One instruction, as far as control flow needs it.
struct Ins {
    int64_t size = 0;
    Flow flow = Flow::Next;
    std::vector<int64_t> targets;
    bool far = false;
    std::optional<std::tuple<py::str, int64_t>> ret;   // ("iret" | "near" | "far", bytes removed)
    std::optional<int64_t> number;                      // the interrupt number of an int
    std::optional<int64_t> table;
    std::optional<int64_t> variable;
    bool odd = false;
    int64_t cs = 0;
};

// Constants in registers along a straight run of instructions.
struct Regs {
    std::map<std::string, int64_t> consts;             // register -> constant (absent: not known)
    std::optional<int64_t> pushed;                     // regs["pushed"]
    std::map<int64_t, int64_t> table;                  // regs["table"]: stores into the interrupt table, offset -> value
};

// What discover() found. See the top.
class Analysis {
public:
    formats::dos::Program program;
    py::dict<int64_t, py::ref<Function>> functions;
    py::list<Interrupt> interrupts;
    py::dict<int64_t, int64_t> tables;
    py::dict<int64_t, int64_t> vectors;
    py::set<int64_t> _labels;                           // addresses jumped to

    explicit Analysis(const formats::dos::Program& program);

    // reading instructions
    // One instruction, as far as control flow needs it; None when the bytes are not one.
    std::optional<Ins> _decode(int64_t va, int64_t cs);
    // Does decoding from `va` run cleanly into a return, a known instruction or an indirect
    // jump -- without meeting bytes that are no instruction, the middle of a known one, or a
    // jump out of the program? `strict` also refuses instructions real-mode programs hardly
    // use, a start that reads as text, and an end in an indirect jump.
    bool _looks_like_code(int64_t va, int64_t cs, int64_t limit = 1500, bool strict = false);

    // following a function
    py::ref<Function> _add(int64_t va, const py::str& distance, const py::str& evidence, int64_t cs,
                           const std::optional<py::str>& name = std::nullopt, bool sure = true);
    // The entries of a jump table at `at`: words that are offsets of code in segment `cs`,
    // up to the first that is not, or to where the table would run into its own targets.
    py::list<int64_t> _table(int64_t at, int64_t cs);
    // Mark everything reached from one function's start; note its returns and its calls.
    void _follow(const py::ref<Function>& f);
    // The segment base of a far call or jump's target: the segment word of the instruction.
    int64_t _far_cs(int64_t va, const Ins& ins) const;

    // constants in registers, along a straight run of instructions
    static std::optional<int64_t> _word(const Regs& regs, const py::str& reg);
    // Update `regs` across the instruction at `va`. Only the simple ways a program loads a service
    // number or a pointer are understood; anything else that writes a register forgets it.
    // -> (handler address, interrupt number) when the instruction completes a pair of stores into
    // the interrupt table at 0000:4n, else None.
    std::optional<std::tuple<int64_t, int64_t>> _constants(int64_t va, const Ins& ins, Regs& regs);

    // the whole program
    Analysis& run();
    void _drain();
    // The segment base code at `va` runs with: that of the nearest function before it.
    int64_t _segment_of(int64_t va);
    // Function starts nothing was seen to call. See `pointer` and `prologue` above.
    void _guesses();
    // `follows`: what stands directly behind a return or jump of code already found.
    void _after_returns(py::set<int64_t>& tried);
    // The interrupt calls inside the functions found, with the service number where a constant was
    // loaded in the same straight run of instructions. A run is cut at every address something
    // jumps to, at every call and at every function start, so a number is only reported where no
    // other path can bring a different one.
    void _name_interrupts();
    // {"code": n, "guessed code": n, "jump tables": n, "not classified": n, "total": n} --
    // bytes of the program's own spans. "guessed code" is reached only from `pointer` and
    // `prologue` and `follows` starts.
    py::dict<py::str, int64_t> census() const;

    // the Python's _mark (1 instruction start, 2 inside one, 3 jump table) and _guess bytearrays
    py::bytes _mark_bytes() const;
    py::bytes _guess_bytes() const;
    // len(_cache) and how many of the cached decodings are instructions
    std::tuple<int64_t, int64_t> _cache_counts() const;

private:
    std::unordered_map<int64_t, std::optional<Ins>> cache_;
    std::vector<uint8_t> mark_;                          // 1 instruction start, 2 inside one, 3 jump table
    std::vector<uint8_t> guess_;                         // reached only from a guessed function start
    std::map<int64_t, int64_t> sites_;                   // address of an `int` -> its number
    py::set<int64_t> relocs_;
    std::vector<py::ref<Function>> queue_;
    std::set<int64_t> fn_set_;                           // function addresses, ordered (for _segment_of)

    uint8_t& m_(int64_t i);                              // mark[i], with a bytearray's indexing
    uint8_t g_(int64_t i) const;
    int64_t ds_linear_() const;                          // p.ds * 16 (TypeError when ds is None)
};

// Find the functions of a Program (formats.dos.read). -> Analysis
Analysis discover(const formats::dos::Program& program);

// python -m codebreaker dos-functions <program> [--all]
//
// The functions found in a real-mode program (a DOS .EXE or .COM, a device driver, IO.SYS):
// how many by which evidence, how much of the program is code, and the interrupt services it
// calls. --all lists every function.
extern const py::str MAIN_DOC;   // main.__doc__
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::analyze::dos_functions
