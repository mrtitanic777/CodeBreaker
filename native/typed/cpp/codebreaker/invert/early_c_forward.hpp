// codebreaker/invert/early_c_forward.hpp -- C++17 port of codebreaker.invert.early_c_forward.
//
// early_c_forward.py - deterministic forward model of the MC2/LC2 code generator:
// quad records -> exact 8086 bytes, no compiler. First brick of the Compiler-Inverse
// Engine. Operand/action model: const/global/param/deref(+field) x load/store/alu.
// Covers integer arith + relationals + assignment + globals/pointers/struct-fields.
//
// Typing notes:
//   * An operand is the Python's dict with a fixed set of possible keys ("k" always, then "val", "id",
//     "ty", "off" depending on the kind): the record Operand, a key that the dict lacks being an empty
//     optional (reading it as dict["id"] raises KeyError, as the Python does).
//   * A resolve() item is the Python's tuple ('b',bytes) | ('L',name) | ('j',opcode,name): the record Item.
//   * A quad record is quad_align's QuadRecord (opcode, payload bytes).
//   * bs(*h) takes its hex strings as an initializer list: bs({"8b46", hx(3)}).
//   * main() takes sys.argv (the program name first) and returns nothing.
#pragma once

#include <cstdint>
#include <initializer_list>
#include <optional>
#include <tuple>

#include "codebreaker/invert/quad_align.hpp"
#include "py.hpp"

namespace codebreaker::invert::early_c_forward {

using QuadRecord = codebreaker::invert::quad_align::QuadRecord;

// {"k": kind, ...}: kind is "const", "node", "deref" or "temp"
struct Operand {
    py::str k;
    std::optional<int64_t> val;   // const
    std::optional<int64_t> id;    // node, deref
    std::optional<int64_t> ty;    // node
    std::optional<int64_t> off;   // deref
};

// ('b', data) | ('L', name) | ('j', opcode, name)
struct Item {
    py::str kind;         // "b", "L" or "j"
    py::bytes data;       // "b"
    int64_t opcode = 0;   // "j"
    py::str name;         // "L", "j"
};

extern const py::dict<int64_t, int64_t> ALU_MEM;   // ax, [mem]  (mod=01 rm=110 [bp+d8] -> +0x46)
extern const py::dict<int64_t, int64_t> ALU_IMM;   // ax, imm16
extern const py::dict<int64_t, int64_t> RELJCC;
extern const py::set<int64_t> BINCLASS;
extern const py::dict<int64_t, int64_t> INV_JCC;   // if(cond) -> skip if NOT cond

py::bytes bs(std::initializer_list<py::str> h);
py::bytes w16(int64_t v);

// --- operand parsing / classification ------------------------------------
py::list<Operand> parse_operands(const py::bytes& payload);

struct Ctx {
    int64_t fn;
    int64_t fs;
    int64_t psz;   // param stride (2 int, 4 long)
    Ctx(int64_t fn_, int64_t fs_, int64_t psz_ = 2) : fn(fn_), fs(fs_), psz(psz_) {}
    bool is_param(int64_t nid) const;
    int64_t frame(int64_t nid) const;
};

py::bytes emit_binop_long(int64_t opcode, const py::list<Operand>& srcs, const Ctx& c);

// --- action emitters ------------------------------------------------------
py::bytes load_ax(const Operand& op, const Ctx& c);
py::bytes store_ax(const Operand& op, const Ctx& c);
py::bytes alu_ax(int64_t opcode, const Operand& op, const Ctx& c);   // ax <op>= operand
py::bytes emit_binop(int64_t opcode, const py::list<Operand>& srcs, const Ctx& c);

int64_t parse_funcbegin(const py::bytes& pl);

// items: ('b',bytes) | ('L',name) | ('j',opcode,name) short rel8. Two-pass.
py::bytes resolve(const py::list<Item>& items);

py::bytes model(const py::list<QuadRecord>& func);

// The script: `python early_c_forward.py <quads> <obj>`; `argv` is sys.argv (the program name first).
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::invert::early_c_forward
