// codebreaker/invert/early_c.hpp -- C++17 port of codebreaker/invert/early_c.py.
//
// early_c.py - INVERSE of mc2model: MC2/LC2 (Microsoft C 2.03 / 3.00) output bytes -> C, by
// recognizing the exact codegen templates (not heuristics), with if/while control flow from a recursive
// block walker (recover) or the structured control-flow graph (recover_cfg, flow16).  Validated by round
// trip (recovered C recompiles identical).
//
// The model is C++ (native/invert: early_c.cc, early_c_text.cc); this module keeps the Python module's
// API: the settings and hooks below (read at each call), LAST, recover, recover_cfg, to_c, local_decls,
// detect_copies and _fold_ladders.  Where the Python reached the model through native_invert's ctypes
// shim, this calls the model's C++ entry points (cbinv::ec_*) directly.
//
// Usage: early_c <image.bin> <hexoff> <len>   (main)
#pragma once

#include <array>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::invert::early_c {

// Optional driver hook: CALLINFO(call_addr_0based, "near"|"far", target) -> number of bytes the
// CALLEE pops on return (`ret N`: N>0 = Pascal convention, args pushed left-to-right and N/2
// words consumed), 0 for a plain `ret` (C convention), or None when unknown.
// The target is None, or an int (near) or a tuple of ints (far: the Python tuple, here a list).
// (std::variant: the Python target is an int for a near call and a tuple for a far one.)
using CallTarget = std::optional<std::variant<int64_t, py::list<int64_t>>>;
using CallinfoHook = std::function<std::optional<int64_t>(int64_t, const py::str&, const CallTarget&)>;
// Optional driver hook: CALLREGS(call_addr_0based, "near"|"far", target) -> the scratch registers
// (ax/bx/cx/dx) the callee reads before writing: arguments passed in registers. Those registers
// are settled into their variables before the call and the call is preceded by `USE(regs);`.
using CallregsHook = std::function<std::optional<py::list<py::str>>(int64_t, const py::str&, const CallTarget&)>;
// Optional driver hook: SWITCH(addr_0based) -> case-target addresses (0-based, table order) of the
// jump-table dispatch `jmp cs:[bx+table]` at that address, or None.
using SwitchHook = std::function<std::optional<py::list<int64_t>>(int64_t)>;

// An empty std::function is the Python's None.
extern CallinfoHook CALLINFO;
extern CallregsHook CALLREGS;
extern SwitchHook SWITCH;

// Verification aid: when True, byte-wide memory operands are emitted as BYTE(x) so a checker can
// tell 8-bit from 16-bit accesses. Off for normal (readable) output.
extern bool MARK;
extern bool FARPTR;           // checker mode: es is a real register, far derefs are FARM(es, x)
extern bool DEADREGS;         // drop register assignments nothing reads
extern bool UNSIGNED_CAST;    // readable listings: mark unsigned compares
extern bool FPCASTS;          // (8087 field operands through casts: the variant model text)
// Optional driver fact: does the function being lifted return a value? (True/False from a
// whole-program "is the result ever read by a caller" analysis; None -> local heuristic.)
extern std::optional<bool> RETVAL;

// What recover() last found of MS-C 3.00's register variables ("saved": [si, di] the function keeps its
// register variables in, "regparam": {register: parameter index} the parameters declared register); and
// of the 8087's values: "fptypes", "fpret", "unsigned_longs".  The Python's LAST is a dict holding only
// the keys it has; here each key is an optional.
struct Last {
    std::optional<py::list<py::str>> saved;
    std::optional<py::dict<py::str, int64_t>> regparam;
    std::optional<py::dict<py::str, py::str>> fptypes;
    std::optional<py::str> fpret;
    std::optional<py::set<py::str>> unsigned_longs;
};
extern Last LAST;

extern bool REGVARS_EAGER;    // (a driver of 3.00 code: a register variable updated from itself - di = di >> 3 - is a statement at once)
extern bool FRAME_AFTER_BP;   // (a driver of 3.00 or later code: `sub sp,N` after `mov bp,sp` - no stack probe, -Gs - is its frame too)
extern bool REGPARAM_REREAD;  // (MS C 3.00: a register a parameter is loaded into is the parameter only if its slot is not read again)

// Stand-in for flow16.CMPSWITCH (read by _flags): codebreaker.invert.flow16 is not ported to this tree
// yet; a driver that sets flow16's CMPSWITCH sets this one.
extern bool flow16_CMPSWITCH;
// The Python passes the model hash() of "ax","bx","cx","dx","si","di","es" in ITS process (the one set
// whose order early_c's output follows); C++ has no such hash, so a driver sets them (default 0).
extern std::array<int64_t, 7> REGISTER_HASHES;

std::tuple<bool, bool, bool, bool, bool, bool, bool, bool, std::optional<py::str>, bool, std::optional<py::str>> _flags();

// (framesize, params, statements, return type, local offsets)
using Recovered = std::tuple<int64_t, py::list<py::str>, py::list<py::str>, py::str, py::list<int64_t>>;

// (framesize, params, statements, return type, local offsets) by the linear block walker.
Recovered recover(const py::bytes& code, int64_t base = 0, const std::optional<py::dict<int64_t, py::str>>& cxmap = std::nullopt);
// The same by the structured control-flow graph (flow16), with the text passes after it.
Recovered recover_cfg(const py::bytes& code, int64_t base = 0, const std::optional<py::dict<int64_t, py::str>>& cxmap = std::nullopt);
// Live local-to-local copies `vX = vY;` in straight-line stmts, as (dest_off, src_off) pairs, where
// src is initialized before and NOT overwritten after (the condition under which MC2 swaps offsets).
py::list<std::tuple<int64_t, int64_t>> detect_copies(const py::list<py::str>& stmts);
// A 32-bit compare's ladder (high words, then the low words without sign) folded into one comparison.
py::list<py::str> _fold_ladders(const py::list<py::str>& lines);
// framemodel-driven local declarations (gap-filled, layout-exact) as a string block.
py::str local_decls(int64_t fs, const std::optional<py::list<int64_t>>& locals, const std::optional<py::set<int64_t>>& longset = std::nullopt,
                    const std::optional<py::list<std::tuple<int64_t, int64_t>>>& copies = std::nullopt);
py::str to_c(const py::str& name, int64_t fs, const py::list<py::str>& params, const py::list<py::str>& stmts, const py::str& ptype = py::str("int"),
             const std::optional<py::list<int64_t>>& locals = std::nullopt, const std::optional<py::set<int64_t>>& longset = std::nullopt);
// argv as sys.argv: [script, image.bin, hexoff, len]; prints the recovered function.
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::invert::early_c
