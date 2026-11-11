// codebreaker/invert/flow16.hpp -- C++17 port of codebreaker.invert.flow16.
//
// flow16.py -- Control-flow-graph analysis for the 16-bit inverse decompiler.
//
// Builds basic blocks from a capstone instruction list, the CFG edges, dominators and natural
// loops, then runs dominator-based structural analysis to recover a structure tree (seq / if /
// ifelse / while / dowhile / loop / switch / goto fallback). The analysis is C++ (native/invert:
// flow16.cc); this module keeps the Python API: Block, the SWITCH and CMPSWITCH settings,
// build_cfg and structure.
//
// Scope notes:
//  * `call`/`lcall` are NOT control-flow edges for structuring (the call returns); a block continues
//    past them. Only jmp/jcc/ret/retf and fall-through form edges.
//  * A jmp/jcc whose target is outside [lo,hi) is a tail call / return and becomes an exit edge.
//  * Conditions are kept as (mnemonic, operand-text) on the branch block; the value model that turns
//    them into C expressions stays in mc2inv -- this module only recovers STRUCTURE.
//
// Typing notes:
//   * A capstone instruction is `Insn` (address, size, mnemonic, op_str: the fields the CFG reads).
//   * Block's attributes that the Python leaves None until set are std::optional; `default` (a C++
//     keyword) is `default_`.
//   * A structure-tree node is the Python's tuple, whose length and element types depend on its kind;
//     here it is the record `Node`: `kind` is the tuple's first element and the fields a kind does not
//     have are empty optionals / empty lists (see the table at structure()).  A sequence stays a list.
//   * `case_values` of a switch arm hold ints and the string "default": std::variant<int64_t, py::str>.
//   * SWITCH is the std::function hook (an empty one is None).  An exception from the hook is taken
//     as "no table", as _switch_targets does.
//   * The model itself is C++ (native/invert flow16.cc); Python reached it through ctypes, this calls
//     cbinv::build_cfg and cbinv::structure directly.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::invert::flow16 {

// a capstone instruction, as far as the CFG reads it
struct Insn {
    int64_t address = 0;
    int64_t size = 0;
    py::str mnemonic;
    py::str op_str;
};

struct Block {
    int64_t id = 0;                                  // 0-based block index
    int64_t lo = 0;                                  // first instruction index (into the function's insn list)
    int64_t hi = 0;                                  // one past the last instruction index
    py::list<Insn> insns;                            // the capstone instructions in this block
    int64_t addr = 0;                                // start address
    std::optional<py::str> term;                     // 'ret' | 'jmp' | 'cond' | 'fall' | 'tailcall'
    py::list<int64_t> succ;                          // successor block ids; for 'cond' = [taken, fallthrough]
    std::optional<int64_t> cond_idx;                 // instruction index of the terminating jcc (for 'cond')
    std::optional<py::list<int64_t>> table;          // 'switch' by a jump table: the case blocks in table order
    std::optional<py::list<std::tuple<int64_t, int64_t>>> cases;   // 'switch' by compares: [(value, block)]
    std::optional<int64_t> default_;                 // the block the rest go to
    std::optional<py::str> selreg;
    std::optional<int64_t> selbase;                  // (by a table: the instruction in the block where the selector's code ends)
    Block(int64_t bid, int64_t lo_, int64_t hi_, const py::list<Insn>& insns_, int64_t addr_);
    py::str repr() const;                            // __repr__
};

// Optional driver hook: SWITCH(addr) -> list of case-target addresses (table order) for the
// indirect `jmp cs:[bx+table]` at `addr`, or None. Set by mc2inv from its own SWITCH hook.
using SwitchHook = std::function<std::optional<py::list<int64_t>>(int64_t)>;
extern SwitchHook SWITCH;                            // (empty: None)

// A switch a compiler lays out as compares (a few cases, or values far apart): the selector in a register,
// then blocks that only compare it with a constant and branch. Off unless the driver asks (CMPSWITCH): the
// 2.03 path keeps its own reading.
extern bool CMPSWITCH;

// insns: capstone instructions for [lo_addr, hi_addr). Returns (blocks, idx2blk, entry_id):
// blocks from the leaders (the entry, every in-range branch target, every instruction after a
// branch or ret), their edges, unreachable blocks marked dead; with CMPSWITCH, switches by
// compares and by a jump table behind its bounds test read as one 'switch' block.
std::tuple<py::list<Block>, py::dict<int64_t, int64_t>, int64_t> build_cfg(const py::list<Insn>& insns, int64_t lo_addr, int64_t hi_addr);

// A node of the structure tree (the Python's tuples):
//     ('block', bid)                         a straight-line basic block
//     ('if', bid, then_tree, join, neg)      if-then (cond block bid, join = merge; neg: the taken side)
//     ('ifelse', bid, then_tree, else_tree, join)
//     ('while', bid, body_tree, join, neg)   pre-tested loop (header bid tests, exits to join)
//     ('dowhile', header, body_tree, latch, taken_continues)  post-tested loop (test at latch)
//     ('loop', header, body_tree, follow)    endless or multi-exit loop
//     ('switch', bid, [(case_values, arm_tree)], join)
//     ('break',), ('continue',), ('goto', None, target)   (unstructured flow: never fabricated)
struct Node;
using CaseValue = std::variant<int64_t, py::str>;
struct Node {
    py::str kind;
    std::optional<int64_t> bid;                      // the block / header / cond block (not for break, continue, goto)
    py::list<Node> body;                             // then_tree / body_tree
    py::list<Node> else_body;                        // else_tree (ifelse)
    std::optional<int64_t> join;                     // join / latch / follow
    std::optional<bool> neg;                         // neg (if, while) / taken_continues (dowhile)
    py::list<std::tuple<py::list<CaseValue>, py::list<Node>>> arms;   // switch
    std::optional<int64_t> target;                   // goto
};

// Recover a structure tree from the CFG.  Dominator/post-dominator guided; CFG_NOCAP (the
// environment) lifts the cap on copying shared tails.
py::list<Node> structure(const py::list<Block>& blocks, int64_t entry = 0);

}  // namespace codebreaker::invert::flow16
