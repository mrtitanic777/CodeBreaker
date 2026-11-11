// codebreaker/analyze/image.hpp -- interface of codebreaker.analyze.image (typed C++; implementation pending).
//
// image.py -- a module as the analyses see it.
//
//     auto img = image::load("comctl32.dll");   // a 32-bit module, a 16-bit one, or a driver
//     img->kind, img->bits                      // "pe" | "ne" | "le",  32 | 16
//     img->functions                            // start addresses, sorted
//     auto fn = img->function(va);              // ->insns (by address), ->tables (jump tables), ->end
//     img->read(va, n), img->u32(va)            // the module's bytes at their addresses
//     img->imports[addr]                        // "USER32!DefWindowProcA", "USER!DEFWINDOWPROC"
//     image::arguments(*fn, i, 4)               // what the four pushes before the call at insns[i] hold
//
// One address space per module, whatever its format. A 32-bit module's addresses are its own. A 16-bit module is laid
// out as decompile/describe.py lays it out for the decompiler (each segment at selector * 0x10000, relocations
// applied, each imported function at an address standing for it), and so is a driver; an address an analysis reports
// is therefore the address the decompiler and the function store use.
//
// A function is decoded by following its code from its start (in C++: native/analyze, cbanalyze.dll, through
// analyze/native.hpp). An instruction is a small record (`Ins`): mnemonic, operands, the registers it reads and
// writes, and where it goes when it is a branch.
//
// Typing notes:
//   * An operand is the tuple ("r", name, size) / ("i", value, size) / ("m", base, index, scale, disp, seg, size);
//     `Op` below holds all three (+ the two shapes image.definition() adds: ("lea", base, index, scale, disp) and
//     ("call", target)); `kind` says which, the fields of the other kinds stay at their defaults. Op has ==, which the
//     Python uses on whole tuples (`ops[0] == ops[1]`, `ops[:2] == ("r", reg)`), and a std::hash.
//   * Ins.rd / Ins.wr are frozenset(str) -> py::set<py::str>; Ins.flow is None | "call" | "jmp" | "jcc" | "ret".
//   * Function.tables are the jump tables the DLL answers with (JumpTable: keys site, selector, bytes, targets,
//     default, low, table, bounded); `default` is spelled default_ (a C++ keyword).
//   * Image is a class (shared state: the memo of analyses, the DLL handle). load() answers a shared_ptr. The
//     per-image "memo" of the Python is Image::memo<T>: a typed slot per key.
//   * spans / pointers reuse analyze::native::Span / Pointer. `signed` is spelled signed_ (keyword).
//   * The already-ported analyses (call_graph, strings, globals, resources) each declare their own stand-in
//     Image/Function/Ins/Span (their headers say so). Once this header is implemented they should take these types;
//     until then adapters (or deriving from those stand-ins) bridge them. Their functions are called from the other
//     analyses of this group (call_graph::build, strings::at/literal/table, globals::pointer_tables).
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/analyze/native.hpp"
#include "codebreaker/formats/le.hpp"
#include "py.hpp"

namespace codebreaker::analyze::image {

// One operand (see the typing notes).
struct Op {
    py::str kind;                         // "r" | "i" | "m" | "lea" | "call"
    py::str name;                         // "r": the register
    int64_t value = 0;                    // "i": the immediate, masked to its size
    std::optional<py::str> base;          // "m"/"lea": base register (None: disp is the address)
    std::optional<py::str> index;         // "m"/"lea": index register
    int64_t scale = 1;
    int64_t disp = 0;
    std::optional<py::str> seg;           // "m": segment override ("fs", "ds", ...)
    int64_t size = 0;                     // "r"/"i"/"m": the operand's size in bytes
    std::optional<int64_t> target;        // "call": the callee (None: an indirect call not through a slot)

    bool operator==(const Op& o) const;
    bool operator!=(const Op& o) const { return !(*this == o); }
};

// How a branch's flow is told ("call", "jmp", "jcc", "ret"; None for the rest).
using Flow = std::optional<py::str>;

// One instruction. `flow` is "call", "jmp", "jcc", "ret" or None; `target` the address a direct call or branch
// goes to.
struct Ins {
    int64_t addr = 0;
    int64_t size = 0;
    py::str mn;
    py::str text;
    py::list<Op> ops;
    py::set<py::str> rd;
    py::set<py::str> wr;
    Flow flow;
    std::optional<int64_t> target;
};
using InsP = std::shared_ptr<Ins>;

// A jump table the DLL found in a function (the dict ask("image.function") answers in row[4]).
struct JumpTable {
    int64_t site = 0;                              // the indirect jump
    py::str selector;                              // what is switched on, as text
    std::optional<py::list<int64_t>> bytes;        // the byte table in front of the address table (None: none)
    py::list<int64_t> targets;
    std::optional<int64_t> default_;               // where an out-of-range value goes (None: the DLL found none)
    int64_t low = 0;                               // the first case's value
    int64_t table = 0;                             // the table's address
    bool bounded = false;                          // a compare bounds the index
};

// One function: `insns` in address order, `tables` its jump tables, `limit` where the next function starts, `end` the
// end of the last instruction reached.
struct Function {
    int64_t start = 0;
    int64_t limit = 0;
    int64_t end = 0;
    py::str name;
    py::list<InsP> insns;
    py::list<JumpTable> tables;
    py::dict<int64_t, int64_t> index;              // an instruction's address -> its position in insns

    // The position in `insns` of the instruction at `addr`, or None.
    std::optional<int64_t> at(int64_t addr) const;
};
using FunctionP = std::shared_ptr<Function>;

class Image {
  public:
    Image(const py::str& path, const py::str& kind, int64_t bits);
    ~Image();

    py::str path, kind;
    int64_t bits = 32;
    py::str name;                                         // lower-cased file name ("vmm32.vxd#vmm" for a bundle's member)
    py::list<native::Span> spans;                         // (va, bytes, executable, writable, name, initialised length)
    py::dict<int64_t, py::str> names;                     // function start -> name
    py::dict<int64_t, int64_t> limits;                    // function start -> where its bytes may run to
    py::dict<int64_t, py::str> imports;                   // address standing for an import -> "MODULE!name"
    py::dict<int64_t, py::str> exports;                   // address -> exported name
    py::list<int64_t> roots;                              // where execution can enter from outside
    // {address: (kind, target)} -- where the file says the loader writes an address, and which; kind "abs32" | "far" |
    // "selector" | "offset" | "byte" (None: the file does not say)
    std::optional<py::dict<int64_t, native::Pointer>> pointers;
    std::optional<formats::le::Ddb> device;               // a driver's device descriptor block
    py::set<int64_t> discovered;                          // function starts found here that the description did not have
    int64_t base = 0;                                     // a 32-bit module's preferred load address
    int64_t data_segment = 0;                             // 16-bit: base address of the automatic data segment
    py::list<int64_t> functions;                          // start addresses, sorted

    // A flat 32-bit image (PE): one address space from its base, run in user mode.
    bool flat() const;

    // ---- the bytes ----
    void _finish();
    // The span holding `va`, or null.
    const native::Span* span(int64_t va) const;
    py::bytes read(int64_t va, int64_t n) const;
    std::optional<int64_t> u16(int64_t va) const;
    std::optional<int64_t> u32(int64_t va) const;
    bool is_code(int64_t va) const;
    // In the module, and not at an address standing for an import.
    bool is_data(int64_t va) const;
    // Does the file hold a value for this address (as opposed to memory it only reserves)?
    bool initialised(int64_t va) const;
    // The start of the function whose bytes hold `va`, or None.
    std::optional<int64_t> owner(int64_t va) const;
    // A name for an address: the function, the import, or the address itself.
    py::str label(int64_t va) const;

    // Something an analysis works out once per image and others reuse: the slot `key`, made by make(*this) on
    // first use. (The Python's img.memo(key, make); each caller names its own result type T.)
    template <class T> std::shared_ptr<T> memo(const py::str& key, const std::function<std::shared_ptr<T>(Image&)>& make) {
        auto it = memo_.find(key.utf8());
        if (it == memo_.end()) it = memo_.emplace(key.utf8(), std::static_pointer_cast<void>(make(*this))).first;
        return std::static_pointer_cast<T>(it->second);
    }

    // ---- instructions (decoded in C++: native/analyze, image.cc) ----
    // The DLL's handle on this image (made on first use).
    native::Handle& _handle();
    // The decoded function starting at `start` (decoded once).
    FunctionP function(int64_t start);
    // Up to `n` instructions read straight on from `va` (no branch followed).
    py::list<InsP> linear(int64_t va, int64_t n);
    // Every function, decoded (a generator in the Python).
    py::list<FunctionP> each();
    // The auto-generated name for a function start the description did not have (sub_<rva> ...).
    py::str _auto_name(int64_t va) const;
    // Add the function starts the module itself gives away beyond those it was loaded with (call targets from known
    // code; addresses the file marks that point at code no known function reaches); repeated until nothing new turns
    // up; every function is decoded on the way. -> the starts added
    const py::set<int64_t>& discover();
    // The decoded instruction whose bytes hold `va`, or null when no function reaches it.
    InsP covering(int64_t va);

    // The image as cbanalyze.dll reads it (what native.Handle takes).
    native::Image as_native() const;

    // Implementation state (decoded-function cache, rows handed back by discover(), the native handle) lives here.
    struct Impl;
    std::shared_ptr<Impl> impl_;

  private:
    std::map<std::string, std::shared_ptr<void>> memo_;
};
using ImageP = std::shared_ptr<Image>;

// The whole register a register name is part of: al, ax -> eax.
py::str family(const py::str& reg);

// An immediate or displacement of `size` bytes as a signed number.
int64_t signed_(int64_t value, int64_t size);

// ---- arguments at a call ----
// What register `reg` holds just before fn.insns[i], looking back through straight-line code: an operand, a
// ("lea", ...) address, a ("call", target) result, or None when it cannot be said.
std::optional<Op> definition(const Function& fn, int64_t i, const py::str& reg, int64_t depth = 14);

// The address an indirect call or jump reads its destination from (`call [slot]`), or None.
std::optional<int64_t> _slot(const Ins& ins);

// Where a call or jump goes: the function, or the address standing for the import; None when it is decided at run
// time.
std::optional<int64_t> destination(const Image& img, const Ins& ins);

// The operands of the `n` pushes before fn.insns[i], nearest first (so the first argument first for C and stdcall,
// the last first for Pascal). A pushed register is replaced by what it was loaded with when that is in sight. The
// list is shorter when the code stops being a run of pushes.
py::list<Op> arguments(const Function& fn, int64_t i, int64_t n);

// ---- loading ----
// Names the capstone.dll to hand to cbanalyze.dll, to describe.cpp's decoder and to the 32-bit function carve (the C++
// stand-in for `import capstone`): once, before the first load().
void set_capstone_dll(const py::str& path);

// 'pe', 'ne' or 'le' for what load() reads (a bundle of drivers, vmm32.vxd's W3, is read as its drivers), else the
// file's lane (base/corpus.lane).
std::optional<py::str> kind_of(const py::str& path);

// The image of a module the corpus names (or a path). A driver inside a bundle is named "vmm32.vxd#vmm". `work`:
// where the description is written (default: settings.state("analyze", "work", <name>)). Raises ValueError for a
// module that is not a 32-bit module, a 16-bit module or a driver.
ImageP load(const py::str& module, const std::optional<py::str>& work = std::nullopt);

}  // namespace codebreaker::analyze::image

namespace std {
template <> struct hash<codebreaker::analyze::image::Op> {
    size_t operator()(const codebreaker::analyze::image::Op& o) const;
};
}  // namespace std
