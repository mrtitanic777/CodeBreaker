#!/usr/bin/env python3
r"""ssa.py -- cross-block value resolution (the interprocedural step dataflow can't reach alone).

dataflow.source_of names a register by scanning backward within ONE basic block. But a value is
usually defined earlier: `mov esi, [ebp+8]` in the entry block, then `cmp esi, edi` three blocks
later. Intra-block resolution gives up and leaves `esi`. This climbs: when a register isn't defined
in the current block, it follows the block's predecessor chain (while that chain is unambiguous --
a single predecessor, or a dominating one common to all) and resolves the definition there. So a
predicate reads `if (a0 == a1)` instead of `if (esi == edi)`, and a field written in one block is
named where it is later read.

Built on cfgstruct's block model; a small context is computed once per function and handed to the
predicate/operand namer.
"""
from codebreaker.recover import dataflow


def build(module, va):
    """A resolution context for one function: {rows_by_block, preds, order}."""
    from codebreaker.recover import cfgstruct
    from codebreaker.emulate import trace32 as tracer
    _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
    off = va - va0
    end = funcs[off]["end"] if off in funcs else min((o for o in funcs if o > off), default=off + 0x400)
    rows = cfgstruct._disasm(code, va0, off, end)
    blocks = cfgstruct._blocks(rows, va0 + off, va0 + end)
    return from_blocks(blocks)


def from_blocks(blocks):
    """Build the context from an already-computed cfgstruct block list (avoids re-disassembly)."""
    rows_by_block = {b[0]: b[1] for b in blocks}
    order = [b[0] for b in blocks]
    idx = {a: i for i, a in enumerate(order)}
    # successor edges -> predecessor sets
    preds = {a: set() for a in order}
    for b0, body, kind, tgt, fall, cc in blocks:
        succ = []
        if kind == "cond":
            succ = [fall, tgt]
        elif kind == "goto":
            succ = [tgt]
        elif kind == "fall":
            succ = [fall]
        for s in succ:
            if s in preds:
                preds[s].add(b0)
    return {"rows": rows_by_block, "preds": preds, "order": order, "idx": idx}


def _bare_reg(e):
    return e in ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp",
                 "ax", "bx", "cx", "dx", "si", "di", "al", "bl", "cl", "dl")


def resolve(ctx, block_addr, upto_idx, reg, depth=0):
    """Resolve `reg` at rows[upto_idx] of block_addr, climbing single-predecessor chains."""
    if ctx is None or depth > 6 or block_addr not in ctx["rows"]:
        return reg
    body = ctx["rows"][block_addr]
    e = dataflow.source_of(body, upto_idx, reg, 0)
    if e != reg and not _bare_reg(e):
        return e                                            # defined in this block
    # not defined locally -> climb, but only while the path is unambiguous
    ps = ctx["preds"].get(block_addr, set())
    if len(ps) == 1:
        pb = next(iter(ps))
        if pb != block_addr:                                # not a self-loop
            return resolve(ctx, pb, len(ctx["rows"][pb]), reg, depth + 1)
    return e


def operand(ctx, block_addr, body, local_idx, tok):
    """Name a cmp/operand token with cross-block resolution: register climbs, memory via dataflow."""
    tok = tok.strip()
    if tok.startswith("0x") or tok.lstrip("-").isdigit():
        return tok
    if "[" in tok:
        return dataflow._mem_name(tok) or tok
    if _bare_reg(tok) or tok in ("edi", "esi"):
        r = resolve(ctx, block_addr, local_idx, tok)
        return r
    return tok
