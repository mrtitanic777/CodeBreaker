// codebreaker/recompile/jumptable.hpp -- typed C++ port of codebreaker.recompile.jumptable.
//
// jumptable.py -- the switch tables behind a `bctr`, so a recompiled switch is a C++ switch.
//
//     jumptable.detect_jump_tables(insns, lo, hi, rd_u8, rd_u16, rd_u32, is_exec)
//         {bctr address: (index register, [targets])}
//
// The Xbox 360 compiler builds a switch four ways (a table of addresses; of byte, halfword or
// word offsets from a base); each is recognised by scanning back from the `bctr` for the bounds
// check, the table's address (lis/addi, also when the table register is the base register --
// the base is found first), and the load, and the table is read from the image. A target
// outside the function becomes a tail call. Derived from the rexglue SDK's function scanner
// (src/codegen/function_scanner.cpp; BSD-3-Clause -- see NOTICE), with its register-reuse and
// RA/RB-swap handling.
//
// Notes on the port:
//   * rd_u8 / rd_u16 / rd_u32 are callables "address -> value, or None if unmapped" (std::optional) and
//     is_exec "address -> bool" (std::function);
//   * an instruction is (va, raw_word, mn, op_str) where raw_word is an int or big-endian bytes (a
//     std::variant -- the Python mixes the two kinds in one list);
//   * the Python `m` dict with fixed keys is the TableMatch record; its register fields hold None as -1
//     (a decoded register is never negative, so comparing against it is False exactly as against None);
//   * Python ints are unbounded; int64_t is the type. Raw words from bytes longer than 8 significant bytes
//     raise OverflowError (no real instruction word comes near it).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::recompile::jumptable {

extern const int64_t MAX_SCAN_BACK;

using Reader = std::function<std::optional<int64_t>(int64_t)>;  // rd_u8 / rd_u16 / rd_u32 (None if unmapped)
using IsExec = std::function<bool(int64_t)>;
// one instruction: (va, raw_word (int or big-endian bytes), mn, op_str)
using Insn = std::tuple<int64_t, std::variant<int64_t, py::bytes>, py::str, py::str>;
// (index register, [targets])
using JumpTable = std::tuple<int64_t, py::list<int64_t>>;

// How the table's entries are turned into targets.
enum class TableType { None, Abs, Computed, ByteOff, ShortOff };

// The state of one backwards scan (the Python's dict `m`). Register fields are -1 for None.
struct TableMatch {
    TableType type = TableType::None;
    int64_t ctr_reg = -1;
    int64_t table_reg = -1;
    int64_t index_reg = -1;
    int64_t base_reg = -1;
    int64_t offset_reg = -1;
    int64_t shift = 0;
    int64_t table_high = 0;
    int64_t table_low = 0;
    int64_t base_high = 0;
    int64_t base_low = 0;
    bool found_mtctr = false;
    bool found_add = false;
    bool found_load = false;
    bool found_shift = false;
    bool found_table_lis = false;
    bool found_table_addi = false;
    bool found_base_lis = false;
    bool found_base_addi = false;
};

// ---- PPC instruction field decode (big-endian word) -----------------------
int64_t _op(int64_t w);
int64_t _xo(int64_t w);  // X/XO extended opcode (bits 1-10)
// D-form
int64_t _d_rt(int64_t w);
int64_t _d_ra(int64_t w);
int64_t _d_uimm(int64_t w);
// X-form / XO-form
int64_t _x_rt(int64_t w);
int64_t _x_ra(int64_t w);
int64_t _x_rb(int64_t w);
// M-form (rlwinm): RS=src(21-25), RA=dest(16-20), SH(11-15), MB(6-10), ME(1-5)
int64_t _m_rs(int64_t w);
int64_t _m_ra(int64_t w);
int64_t _m_sh(int64_t w);
int64_t _m_mb(int64_t w);
int64_t _m_me(int64_t w);

// ---- opcode predicates (match decode_instruction's Opcode:: classification) --
std::optional<int64_t> _is_mtctr(int64_t w);  // the register, or None
bool _is_mflr(int64_t w);
bool _is_add(int64_t w);
bool _is_lwzx(int64_t w);
bool _is_lbzx(int64_t w);
bool _is_lhzx(int64_t w);
bool _is_rlwinm(int64_t w);
bool _is_lis(int64_t w);  // addis rX,r0,imm
bool _is_addi(int64_t w);
bool _is_ori(int64_t w);
bool _is_bctr(int64_t w);
bool _is_bcctr_any(int64_t w);  // bctr or bctrl
bool _is_b(int64_t w);          // b / ba
bool _is_blr(int64_t w);
bool _is_cmpli(int64_t w);  // cmplwi/cmpldi
bool _is_cmpi(int64_t w);   // cmpwi/cmpdi
bool _is_stwu(int64_t w);

// Port of is_function_boundary().
bool _is_function_boundary(int64_t w);

int64_t _s16(int64_t v);

// Port of scanForBounds -> (max_entries, default_target).
std::tuple<int64_t, int64_t> _scan_bounds(const Reader& rd_u32, int64_t bctr_addr, int64_t expected_index_reg);

// Port of read_table_entries.
py::list<int64_t> _read_targets(const TableMatch& m, int64_t count, const Reader& rd_u8, const Reader& rd_u16,
                                const Reader& rd_u32, const IsExec& is_exec);

// Port of FunctionScanner::detect_jump_table for a single bctr address; None when there is no table.
std::optional<JumpTable> _detect_one(const Reader& rd_u8, const Reader& rd_u16, const Reader& rd_u32,
                                     const IsExec& is_exec, int64_t bctr_addr);

// For each `bctr` in the function, detect a jump table (the four kinds above).
// insns: list of (va, raw_word, mn, op_str); raw_word may be int or big-endian
// bytes. rd_u8/rd_u16/rd_u32(addr) read the image big-endian (None if unmapped);
// is_exec(addr) validates a candidate target lands in executable code.
// Returns {bctr_va: (index_reg, [targets])}.
py::dict<int64_t, JumpTable> detect_jump_tables(const py::list<Insn>& insns, int64_t func_lo, int64_t func_hi,
                                                const Reader& rd_u8, const Reader& rd_u16, const Reader& rd_u32,
                                                const IsExec& is_exec);

}  // namespace codebreaker::recompile::jumptable
