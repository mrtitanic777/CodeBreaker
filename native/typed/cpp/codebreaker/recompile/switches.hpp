// codebreaker/recompile/switches.hpp -- typed C++ of codebreaker.recompile.switches
//
// switches.py -- the switch tables of an Xbox 360 image, for XenonRecomp.
//
//     auto [tables, report] = switches::find(xex_or_image_path);
//     switches::write(path, tables);            // XenonRecomp's switch-table file
//
// XenonAnalyse matches fixed instruction sequences, which miss compilers whose instruction order
// differs or whose linker interleaves nops. This scanner walks back from every `bctr`, evaluates
// CTR symbolically and classifies the table by the expression's shape:
//
//     absolute : ctr = load32(T + (idx << 2))
//     byte     : ctr = B + (load8(T + idx) << s)            (s may be 0)
//     short    : ctr = B + (load16(T + (idx << 1)) << s)
//
// `idx` is the register the guarding `cmplwi crN, rI, max` + `bgt/ble crN` checks (or, with no
// check, the table's length comes from its entries). Every label is checked against the function
// bounds .pdata gives. Every executable section is scanned.
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * A decoded instruction is a tuple in the Python whose length depends on its kind: here the struct `Insn`
//     (kind + the fields in order, `at(k)` is the Python's d[k]).  `repr` of one gives the tuple's text, which
//     the Unresolved messages contain.
//   * The symbolic expressions (nested tuples ('idx',) ('const', n) ('add', a, b) ('shl', a, n)
//     ('load', size, a)) are the struct `Expr`; `repr` gives the tuple's text (the 'ctr shape ...' message).
//   * writes() answers an int (one register), a set, or the string 'unknown': the struct `Writes`.
//   * A table is a dict with some keys present only for one way of finding it: the struct `Table` (`table` and
//     `inline_` are absent -- nullopt -- for a guarded table; `func` is set by find).
//   * find()'s second value is the dict of counts: the struct `Report`.
//   * Image.data is bytes; Image::load raises Unresolved outside the image.
#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::recompile::switches {

// Unresolved: the scanner could not follow a jump (a plain Exception subclass in the Python).
struct Unresolved : py::Exception {
    Unresolved() : py::Exception() {}
    explicit Unresolved(const std::string& m) : py::Exception(m) {}
    explicit Unresolved(const char* m) : py::Exception(m) {}
    explicit Unresolved(const py::str& m) : py::Exception(m.utf8()) {}
    const char* type() const override { return "Unresolved"; }
};

class Image {
public:
    py::bytes data;
    int64_t base = 0;

    Image() = default;
    Image(const py::bytes& data, int64_t base);

    int64_t u32(int64_t addr) const;
    int64_t u16(int64_t addr) const;
    int64_t u8(int64_t addr) const;
    // A computed table address outside the image (a value the scanner could not follow): that jump is
    // unresolved, not the end of the scan.
    int64_t load(int64_t size, int64_t addr) const;
};

int64_t sext16(int64_t v);

// A decoded instruction: nop, addis, addi, ori, rlwinm, cmplwi, cmpwi, lwz, bc, bclr, b, loadx, add, or,
// mtctr, other -- followed by the fields in the order decode() lists them.
enum class Kind { Nop, Addis, Addi, Ori, Rlwinm, Cmplwi, Cmpwi, Lwz, Bc, Bclr, B, Loadx, Add, Or, Mtctr, Other };

struct Insn {
    Kind kind = Kind::Nop;
    std::array<int64_t, 5> f{};   // the fields after the kind: d[1] is f[0] ...
    int64_t at(int64_t k) const { return f[static_cast<size_t>(k - 1)]; }
    bool operator==(const Insn& o) const { return kind == o.kind && f == o.f; }
};
py::str kind_name(Kind k);
py::str repr(const Insn& d);

// Decode only the instructions jump-table sequences use.
Insn decode(int64_t w);

extern const py::set<int64_t> X_STORES;      // stwx/stbx/sthx/stdx/stf*x (+u)
extern const py::set<int64_t> X_WRITES_RA;
extern const py::dict<int64_t, bool> D_LOADS;     // value: update form
extern const py::dict<int64_t, bool> D_STORES;
extern const py::dict<int64_t, bool> FP_LOADS;

// Set of GPRs an instruction writes, or None if unknown (treated as clobbering anything).
std::optional<py::set<int64_t>> gpr_writes(int64_t w);

// What writes() answers: unknown, one register, or a set of registers.
struct Writes {
    enum class Is { Unknown, Reg, Set } is = Is::Unknown;
    int64_t reg = 0;
    py::set<int64_t> regs;
    bool unknown() const { return is == Is::Unknown; }
    // `w == reg`
    bool is_reg(int64_t r) const { return is == Is::Reg && reg == r; }
    // `isinstance(w, set) and r in w`
    bool in_set(int64_t r) const { return is == Is::Set && regs.contains(r); }
};

// Register written by insn (decoded) or 'unknown'.
Writes writes(const Insn& insn, int64_t w);

// A symbolic value: ('idx',) ('const', n) ('add', a, b) ('shl', a, n) ('load', size, a)
struct Expr;
using ExprP = std::shared_ptr<const Expr>;
struct Expr {
    enum class Is { Idx, Const, Add, Shl, Load } is = Is::Idx;
    int64_t n = 0;   // the constant, the shift, or the load's size
    ExprP a, b;      // add: a + b; shl and load: a
};
py::str repr(const Expr& e);
ExprP simplify(const ExprP& e);

// (kind, size, table, base, shift) of a CTR expression that reads a jump table.
struct Shape {
    py::str kind;
    int64_t size = 0, table = 0, base = 0, shift = 0;
};
Shape table_shape(const ExprP& ctr);
int64_t table_entry(const Image& img, const Shape& shape, int64_t i);
std::tuple<py::str, py::list<int64_t>> classify(const Image& img, const ExprP& ctr, int64_t count);

// One switch table found.
struct Table {
    int64_t bctr = 0;
    int64_t r = 0;                           // the index register
    py::str kind;                            // absolute | byte | short
    py::list<int64_t> labels;
    int64_t default_ = 0;                    // `default` in the Python
    std::optional<int64_t> guard;            // address of the cmplwi, None for an unguarded table
    std::optional<int64_t> table;            // unguarded only
    std::optional<bool> inline_;             // unguarded only
    std::optional<std::tuple<int64_t, int64_t>> func;   // set by find: the .pdata function holding the bctr
};

class Scanner {
public:
    static constexpr int64_t WINDOW = 40;

    Scanner(const Image& image, int64_t text_start, int64_t text_end);

    Insn insn(int64_t addr) const;
    // `ble crN, a+4` at a-4 skipping `b default` at a: returns (crN, default) or None.
    std::optional<std::tuple<int64_t, int64_t>> branch_over(int64_t a) const;
    // Locate `cmplwi crN, rI, max` guarded by `bgt/ble crN`. Returns (addr, rI, count, default).
    //
    // Two guard shapes occur: `bgt crN, default` falling through to the table code, and
    // `ble crN, L; b default; L:` (the conditional branch skips an unconditional one).
    std::optional<std::tuple<int64_t, int64_t, int64_t, int64_t>> find_guard(int64_t bctr) const;
    // Symbolic value of `reg` just before the instruction at addr.
    ExprP eval_reg(int64_t reg, int64_t addr, const std::tuple<int64_t, int64_t>& guard) const;
    ExprP eval_insn(const Insn& d, int64_t a, const std::tuple<int64_t, int64_t>& guard) const;
    // (address, offset) of `lwz rG, offset(r1)` loading the guard's register just before the cmplwi.
    std::optional<std::tuple<int64_t, int64_t>> guard_slot(const std::tuple<int64_t, int64_t>& guard) const;
    // No store to offset(r1) and no write to r1 in [start, end).
    bool slot_unchanged(int64_t offset, int64_t start, int64_t end) const;
    bool unchanged(int64_t reg, int64_t start, int64_t end) const;
    // The index register must hold the same value at the bctr as at the cmplwi.
    bool index_reg_live(const std::tuple<int64_t, int64_t, int64_t, int64_t>& guard, int64_t bctr) const;
    // Jump table with no bounds check before it (a `switch` whose default the compiler knows is
    // unreachable, e.g. a leaf function switching on an argument, or `default: __assume(0)`).
    //
    // The index is the register the table load scales (or uses directly for byte tables), taken
    // where it is used; it must still hold that value at the bctr. Without a bound, the table runs
    // while its entries are word-aligned addresses inside the function `extent` (lo, hi): an entry
    // equal to hi is MSVC's slot for the unreachable default. An inline table (data right after the
    // bctr) ends exactly where the first case's code begins. Extra trailing entries only add case
    // labels that are never taken; a missing one would fall back to an indirect jump and be reported.
    std::optional<Table> analyze_unguarded(int64_t bctr, const std::tuple<int64_t, int64_t>& extent) const;
    std::optional<Table> analyze(int64_t bctr) const;

private:
    Image img_;
    int64_t text_start_, text_end_;
};

// True if an indexed load (lwzx/lhzx/lbzx) precedes the bctr in straight-line code.
bool indexed_load_before(const Image& img, int64_t bctr, int64_t text_start, int64_t window = 12);

// Bounds of the .pdata function containing addr, else the gap between .pdata functions.
std::tuple<int64_t, int64_t> extent(const py::list<std::tuple<int64_t, int64_t>>& funcs, int64_t addr, int64_t text_start,
                                    int64_t text_end);
std::optional<std::tuple<int64_t, int64_t>> containing(const py::list<std::tuple<int64_t, int64_t>>& funcs, int64_t addr);

// The counts find() answers with (a dict in the Python).
struct Report {
    int64_t tables = 0;
    py::dict<py::str, int64_t> kinds;
    py::list<std::tuple<int64_t, py::str>> unresolved;
    py::list<std::tuple<int64_t, std::tuple<int64_t, int64_t>>> outside;
    py::list<int64_t> pointer_calls;
};

// -> ([table], {counts}) for an image or .xex.
std::tuple<py::list<Table>, Report> find(const py::str& path);

// XenonRecomp's switch-table file.
void write(const py::str& path, const py::list<Table>& found);

}  // namespace codebreaker::recompile::switches
