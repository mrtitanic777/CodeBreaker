"""early_c.py - INVERSE of mc2model: MC2/LC2 (Microsoft C 2.03 / 3.00) output bytes -> C, by
recognizing the exact codegen templates (not heuristics), with if/while control flow from a recursive
block walker (recover) or the structured control-flow graph (recover_cfg, flow16).  Validated by round
trip (recovered C recompiles identical).

The model is C++ (native/invert: early_c.cc, early_c_text.cc, through native_invert.py); this module keeps
its Python API: the settings and hooks below (read at each call), LAST, recover, recover_cfg, to_c,
local_decls, detect_copies and _fold_ladders.

Usage: python early_c.py <image.bin> <hexoff> <len>
"""
import os
import sys

from codebreaker import platforms
from codebreaker.invert import early_c_frame as framemodel
from codebreaker.invert import native_invert

_MD = platforms.decoder(16)

# Optional driver hook: CALLINFO(call_addr_0based, "near"|"far", target) -> number of bytes the
# CALLEE pops on return (`ret N`: N>0 = Pascal convention, args pushed left-to-right and N/2
# words consumed), 0 for a plain `ret` (C convention), or None when unknown.
CALLINFO = None
# Optional driver hook: CALLREGS(call_addr_0based, "near"|"far", target) -> the scratch registers
# (ax/bx/cx/dx) the callee reads before writing: arguments passed in registers. Those registers
# are settled into their variables before the call and the call is preceded by `USE(regs);`.
CALLREGS = None
# Verification aid: when True, byte-wide memory operands are emitted as BYTE(x) so a checker can
# tell 8-bit from 16-bit accesses. Off for normal (readable) output.
MARK = False
FARPTR = os.environ.get("MC2INV_FAR", "0") == "1"           # checker mode: es is a real register, far derefs are FARM(es, x)
DEADREGS = os.environ.get("MC2INV_DEADREGS", "0") == "1"     # drop register assignments nothing reads
UNSIGNED_CAST = os.environ.get("MC2INV_UNSIGNED", "0") == "1"   # readable listings: mark unsigned compares
FPCASTS = False                                              # (8087 field operands through casts: the variant model text)
# Optional driver fact: does the function being lifted return a value? (True/False from a
# whole-program "is the result ever read by a caller" analysis; None -> local heuristic.)
RETVAL = None
# What recover() last found of MS-C 3.00's register variables: {"saved": [si, di] the function keeps its
# register variables in, "regparam": {register: parameter index} the parameters declared register}; and
# of the 8087's values: "fptypes", "fpret", "unsigned_longs".
LAST = {}
REGVARS_EAGER = False    # (a driver of 3.00 code: a register variable updated from itself - di = di >> 3 - is a statement at once)
FRAME_AFTER_BP = False   # (a driver of 3.00 or later code: `sub sp,N` after `mov bp,sp` - no stack probe, -Gs - is its frame too)
REGPARAM_REREAD = True    # (MS C 3.00: a register a parameter is loaded into is the parameter only if its slot is not read again)
# Optional driver hook: SWITCH(addr_0based) -> case-target addresses (0-based, table order) of the
# jump-table dispatch `jmp cs:[bx+table]` at that address, or None.
SWITCH = None


def _flags():
    from codebreaker.invert import flow16
    return (MARK, FARPTR, DEADREGS, UNSIGNED_CAST, FPCASTS, REGVARS_EAGER, FRAME_AFTER_BP, REGPARAM_REREAD,
            os.environ.get("MASM_CRET"), flow16.CMPSWITCH, os.environ.get("CFG_NOCAP"))


def recover(code, base=0, cxmap=None):
    """(framesize, params, statements, return type, local offsets) by the linear block walker."""
    return native_invert.early_c(0, code, base, cxmap, _flags(), RETVAL, LAST, SWITCH, CALLINFO, CALLREGS)


def recover_cfg(code, base=0, cxmap=None):
    """The same by the structured control-flow graph (flow16), with the text passes after it."""
    from codebreaker.invert import flow16
    flow16.SWITCH = SWITCH
    return native_invert.early_c(1, code, base, cxmap, _flags(), RETVAL, LAST, SWITCH, CALLINFO, CALLREGS)


def detect_copies(stmts):
    """Live local-to-local copies `vX = vY;` in straight-line stmts, as (dest_off, src_off) pairs, where
    src is initialized before and NOT overwritten after (the condition under which MC2 swaps offsets)."""
    return native_invert.early_c_text(4, [list(stmts)])


def _fold_ladders(lines):
    """A 32-bit compare's ladder (high words, then the low words without sign) folded into one comparison."""
    return native_invert.early_c_text(3, [list(lines), bool(MARK)])


def local_decls(fs, locals, longset=None, copies=None):
    """framemodel-driven local declarations (gap-filled, layout-exact) as a string block."""
    ordered, _ = framemodel.emit_local_decls(locals or [], fs, longset, copies)
    return "".join("    %s\n" % framemodel.fmt_decl(*d) for d in ordered)


def to_c(name, fs, params, stmts, ptype="int", locals=None, longset=None):
    return native_invert.early_c_text(2, [name, fs, list(params), list(stmts), ptype, list(locals or []),
                                          sorted(longset or ())])


def main():
    img = open(sys.argv[1], "rb").read(); off = int(sys.argv[2], 16); ln = int(sys.argv[3])
    fs, params, stmts, pt, loc = recover(img[off:off + ln]); print(to_c("f%05x" % off, fs, params, stmts, pt, loc))


if __name__ == "__main__":
    main()
