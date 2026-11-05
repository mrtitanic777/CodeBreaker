"""flow16.py -- Control-flow-graph analysis for the 16-bit inverse decompiler.

Builds basic blocks from a capstone instruction list, the CFG edges, dominators and natural
loops, then runs dominator-based structural analysis to recover a structure tree (seq / if /
ifelse / while / dowhile / loop / switch / goto fallback). The analysis is C++ (native/invert:
flow16.cc, through native_invert.py); this module keeps the Python API: Block, the SWITCH and
CMPSWITCH settings, build_cfg and structure.

Scope notes:
 * `call`/`lcall` are NOT control-flow edges for structuring (the call returns); a block continues
   past them. Only jmp/jcc/ret/retf and fall-through form edges.
 * A jmp/jcc whose target is outside [lo,hi) is a tail call / return and becomes an exit edge.
 * Conditions are kept as (mnemonic, operand-text) on the branch block; the value model that turns
   them into C expressions stays in mc2inv -- this module only recovers STRUCTURE.
"""
import os

from codebreaker.invert import native_invert


class Block:
    __slots__ = ("id","lo","hi","insns","term","succ","cond_idx","addr", "table", "cases", "default", "selreg", "selbase")
    def __init__(s, bid, lo, hi, insns, addr):
        s.id = bid           # 0-based block index
        s.lo = lo            # first instruction index (into the function's insn list)
        s.hi = hi            # one past the last instruction index
        s.insns = insns      # the capstone instructions in this block
        s.addr = addr        # start address
        s.term = None        # 'ret' | 'jmp' | 'cond' | 'fall' | 'tailcall'
        s.succ = []          # list of successor block ids; for 'cond' = [taken, fallthrough]
        s.cond_idx = None    # instruction index of the terminating jcc (for 'cond')
        s.table = None       # 'switch' by a jump table: the case blocks in table order
        s.cases = None       # 'switch' by compares: [(value, block)]; default: the block the rest go to
        s.default = None
        s.selreg = None
        s.selbase = None     # (by a table: the instruction in the block where the selector's code ends)
    def __repr__(s):
        return "B%d[%05x..%05x %s->%s]" % (s.id, s.addr, (s.insns[-1].address if s.insns else s.addr),
                                           s.term, s.succ)


# Optional driver hook: SWITCH(addr) -> list of case-target addresses (table order) for the
# indirect `jmp cs:[bx+table]` at `addr`, or None. Set by mc2inv from its own SWITCH hook.
SWITCH = None

def _switch_targets(ins):
    if SWITCH is None or ins.mnemonic != "jmp" or "[" not in ins.op_str: return None
    try: return SWITCH(ins.address)
    except Exception: return None


# A switch a compiler lays out as compares (a few cases, or values far apart): the selector in a register,
# then blocks that only compare it with a constant and branch. Off unless the driver asks (CMPSWITCH): the
# 2.03 path keeps its own reading.
CMPSWITCH = False


def build_cfg(insns, lo_addr, hi_addr):
    """insns: capstone instructions for [lo_addr, hi_addr). Returns (blocks, idx2blk, entry_id):
    blocks from the leaders (the entry, every in-range branch target, every instruction after a
    branch or ret), their edges, unreachable blocks marked dead; with CMPSWITCH, switches by
    compares and by a jump table behind its bounds test read as one 'switch' block."""
    return _native_cfg(insns)


def structure(blocks, entry=0):
    """Recover a structure tree from the CFG. Node forms:
        ('block', bid)                         a straight-line basic block
        ('if', bid, then_tree, join, neg)      if-then (cond block bid, join = merge; neg: the taken side)
        ('ifelse', bid, then_tree, else_tree, join)
        ('while', bid, body_tree, join, neg)   pre-tested loop (header bid tests, exits to join)
        ('dowhile', header, body_tree, latch, taken_continues)  post-tested loop (test at latch)
        ('loop', header, body_tree, follow)    endless or multi-exit loop
        ('switch', bid, [(case_values, arm_tree)], join)
        ('break',), ('continue',), ('goto', None, target)   (unstructured flow: never fabricated)
    Dominator/post-dominator guided; CFG_NOCAP lifts the cap on copying shared tails."""
    return _tree(native_invert.structure(blocks, entry, os.environ.get("CFG_NOCAP")))


def _native_cfg(insns):
    """build_cfg by the C++ (native_invert): the same Blocks, over the caller's instructions."""
    if not insns:
        raise IndexError("list index out of range")
    blocks, idx2blk = [], {}
    for bi, (lo, hi, term, succ, cond_idx, table, cases, default, selreg, selbase) in enumerate(
            native_invert.cfg(insns, _switch_targets, CMPSWITCH)):
        b = Block(bi, lo, hi, insns[lo:hi], insns[lo].address)
        b.term, b.succ, b.cond_idx, b.table, b.default, b.selreg, b.selbase = term, succ, cond_idx, table, default, selreg, selbase
        b.cases = None if cases is None else [tuple(c) for c in cases]
        blocks.append(b)
        for k in range(lo, hi):
            idx2blk[k] = bi
    return blocks, idx2blk, 0


def _tree(nodes):
    """structure's JSON lists -> its tuples (a sequence stays a list)."""
    out = []
    for n in nodes:
        k = n[0]
        if k in ("if", "while", "dowhile", "loop"):
            n = (k, n[1], _tree(n[2])) + tuple(n[3:])
        elif k == "ifelse":
            n = (k, n[1], _tree(n[2]), _tree(n[3]), n[4])
        elif k == "switch":
            n = (k, n[1], [(vals, _tree(arm)) for vals, arm in n[2]], n[3])
        else:
            n = tuple(n)
        out.append(n)
    return out
