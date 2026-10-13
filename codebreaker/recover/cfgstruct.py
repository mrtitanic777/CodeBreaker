#!/usr/bin/env python3
r"""cfgstruct.py -- STRUCTURED reconstruction: the control-flow skeleton with the fragments filled in.

The leaf/interproc engines recover WHAT a function does (calls, writes, expressions) but emit it
flat -- a call inside an `if` comes out unconditional, which is wrong. This engine recovers the
control-flow structure and places the fragments where they belong:

  1. disassemble the function, split into basic blocks (leaders at branch targets / after branches),
  2. classify each block tail: conditional (cmp/test + jcc), goto, return, or fall-through,
  3. recover each conditional's PREDICATE from its compare + jcc (e.g. `a == 0x10`, `x < y`),
  4. trace the function once and map every observed call (name + args, via the SDK proto DB) to its
     block, so each block's real calls sit in the block,
  5. structure it: forward conditional -> if/else, back edge -> while, else labelled goto (always
     correct, if not always goto-free), and emit structured C.

This is the skeleton that assembles the other engines' output into a real function body.
"""
from codebreaker.emulate import trace32 as tracer
from codebreaker.recover import interproc as ip

_CC = {   # jcc mnemonic -> (C op, signed?)  applied to the preceding cmp's (lhs, rhs)
    "je": "==", "jz": "==", "jne": "!=", "jnz": "!=",
    "jl": "<", "jnge": "<", "jle": "<=", "jng": "<=", "jg": ">", "jnle": ">", "jge": ">=", "jnl": ">=",
    "jb": "<", "jnae": "<", "jbe": "<=", "jna": "<=", "ja": ">", "jnbe": ">", "jae": ">=", "jnb": ">=",
    "js": "< 0", "jns": ">= 0",
}
_UNCOND = {"jmp"}
_RET = ("ret", "retn", "retf")

import re as _re
# jmp dword ptr [BASE + reg*4]  /  jmp [reg*4 + BASE]  -- a switch jump table
_JTAB = _re.compile(r"(?:dword ptr )?\[(?:(0x[0-9a-f]+)\s*\+\s*)?([a-z]+)\*4(?:\s*\+\s*(0x[0-9a-f]+))?\]")


def _jumptable(ops):
    """(index_reg, table_base) if ops is a switch-table indirect jump operand, else None."""
    m = _JTAB.search(ops)
    if not m:
        return None
    base = m.group(1) or m.group(3)
    if not base:
        return None
    return m.group(2), int(base, 16)


def _disasm(code, va0, off, end):
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    rows = []
    for ins in md.disasm(code[off:end], va0 + off):
        rows.append((ins.address, ins.mnemonic, ins.op_str, ins.size))
    return rows


def _blocks(rows, lo, hi, extra_leaders=()):
    """Split rows into basic blocks. Returns [(start, rows, kind, target, fall, cc_op)]."""
    addrs = {r[0] for r in rows}
    leaders = {rows[0][0]} if rows else set()
    for t in extra_leaders:                               # switch-table case targets
        if lo <= t < hi:
            leaders.add(t)
    for i, (a, mn, ops, sz) in enumerate(rows):
        tgt = _branch_target(mn, ops)
        if mn in _CC or mn in _UNCOND or mn in _RET:
            if tgt is not None and lo <= tgt < hi:
                leaders.add(tgt)
            if i + 1 < len(rows):
                leaders.add(rows[i + 1][0])
    leaders = sorted(l for l in leaders if l in addrs)
    out = []
    for i, b in enumerate(leaders):
        nxt = leaders[i + 1] if i + 1 < len(leaders) else hi
        body = [r for r in rows if b <= r[0] < nxt]
        if not body:
            continue
        _, mn, ops, _ = body[-1]
        tgt = _branch_target(mn, ops)
        jt = None
        if mn == "jmp":                                   # switch: table access may be `mov r,[tab+i*4]; jmp r`
            for _a, _m, _o, _s in body[-4:]:
                jt = _jumptable(_o)
                if jt:
                    break
        if mn in _CC:
            out.append([b, body, "cond", tgt, nxt, _CC[mn]])
        elif jt is not None:                              # jmp [base + idx*4] -> switch
            out.append([b, body, "switch", jt, None, None])
        elif mn in _UNCOND:
            out.append([b, body, "goto", tgt, None, None])
        elif mn in _RET:
            out.append([b, body, "ret", None, None, None])
        else:
            out.append([b, body, "fall", None, nxt, None])
    return out


_SUBLOW = _re.compile(r"^(e[a-z]{2}),\s*(0x[0-9a-f]+|\d+)$")
_ABOVE = ("ja", "jae", "jnbe", "jnb", "jg", "jnle")           # unsigned/signed out-of-range jumps


def _switch_meta(blocks, sw_addr, idxreg):
    """Recover a switch's (low_base, default_target): the `sub idx, LOW` and the bounds-check `ja`."""
    succ = {}
    for b0, body, kind, tgt, fall, cc in blocks:
        s = []
        if kind == "cond":
            s = [fall, tgt]
        elif kind in ("goto",):
            s = [tgt]
        elif kind in ("fall", "switch"):
            s = [fall]
        succ[b0] = [x for x in s if x is not None]
    preds = {}
    for b0, s in succ.items():
        for t in s:
            preds.setdefault(t, []).append(b0)
    by = {b[0]: b for b in blocks}
    scan = [by[sw_addr]] + [by[p] for p in preds.get(sw_addr, []) if p in by]
    low, default = 0, None
    for b0, body, kind, tgt, fall, cc in scan:
        for _a, mn, ops, _s in body:
            if mn in ("sub", "add"):
                m = _SUBLOW.match(ops)
                if m and m.group(1) == idxreg:
                    v = int(m.group(2), 0)
                    low = v if mn == "sub" else (-v & 0xFFFFFFFF)   # add -LOW is also a base shift
        if kind == "cond" and body and body[-1][1] in _ABOVE:       # the bounds check -> default
            default = tgt
    return low, default


def _branch_target(mn, ops):
    if (mn in _CC or mn in _UNCOND) and ops.startswith("0x"):
        try:
            return int(ops, 16)
        except ValueError:
            return None
    return None


_REG_RE = _re.compile(r"^(e?[abcd]x|e?[sd]i|e?bp|[abcd][lh])$")


def _operand(body, idx, tok, ctx=None, block_addr=None):
    """Name a cmp operand: resolve a register to its source (cross-block when a ctx is given), a
    memory ref to a field/local/param."""
    from codebreaker.recover import dataflow
    tok = tok.strip()
    if tok.startswith("0x") or tok.isdigit():
        return tok
    if ctx is not None and block_addr is not None:
        from codebreaker.recover import ssa
        return ssa.operand(ctx, block_addr, body, idx, tok)      # cross-block resolution
    if _REG_RE.match(tok):
        return dataflow.source_of(body, idx, tok)
    if "[" in tok:
        return dataflow._mem_name(tok) or tok
    return tok


def _predicate(body, ctx=None, block_addr=None):
    """Recover a conditional block's predicate from its trailing cmp/test + jcc, with operands
    resolved to variable names (arg/local/field/const). With an ssa ctx, resolution is cross-block."""
    if len(body) < 2:
        return "/* cond */"
    ci = len(body) - 2
    cmp_mn, cmp_ops = body[ci][1], body[ci][2]
    jcc = body[-1][1]
    op = _CC.get(jcc, "?")
    if cmp_mn == "test":
        a = _operand(body, ci, cmp_ops.split(",")[0], ctx, block_addr)
        if op in ("==", "!="):
            return "%s %s 0" % (a, op)
        return "(%s & ...) %s 0" % (a, op if op[0] in "<>" else "!=")
    if cmp_mn == "cmp":
        parts = [p.strip() for p in cmp_ops.split(",")]
        if len(parts) == 2:
            lhs = _operand(body, ci, parts[0], ctx, block_addr)
            if op.endswith("0"):
                return "%s %s" % (lhs, op)
            return "%s %s %s" % (lhs, op, _operand(body, ci, parts[1], ctx, block_addr))
    if op.endswith("0"):
        return "result %s" % op
    return "/* %s */" % jcc


def _calls_by_block(module, va, blocks, arity=4):
    """Trace once; bucket each observed call (named + args) into the block whose range holds its pc."""
    try:
        tr = tracer.trace_function(module, func=hex(va), args=ip._SENT[:arity],
                                   stub_externals=True, insn_cap=8000)
    except Exception:
        return {}, None
    from codebreaker.symbols import symdb
    _, va0, _c, _s, _f, _sha, _imp = tracer._load(module)
    by = {}
    mnem = tr.get("mnemonics", {})
    for c in tr.get("calls", []):
        pc = c.get("pc")
        nm = ip._name_target(c, va0, mnem.get("0x%x" % (pc or 0)))
        if pc is None or not nm:
            continue
        argc = c.get("argc")
        if argc is None and c.get("api"):
            argc = symdb.arity(nm)
        real = ip._call_args(c.get("args_stack") or [], argc)
        from codebreaker.recover import datarec
        from codebreaker.recover import constants
        callstr = "%s(%s);" % (nm, ", ".join(constants.render(nm, p, v) or datarec.render(module, v) or ip._arg(v)
                                              for p, v in enumerate(real)))
        # find the block containing pc
        for blk in blocks:
            b0 = blk[0]; b1 = blk[1][-1][0] + blk[1][-1][3]
            if b0 <= pc < b1:
                by.setdefault(b0, []).append(callstr)
                break
    return by, tr.get("ret")


def reconstruct(module, func=None, arity=4):
    va = int(func, 0) if isinstance(func, str) else int(func)
    _, va0, code, _sec, funcs, _sha, _imp = tracer._load(module)
    off = va - va0
    fend = None
    for o, f in sorted(funcs.items()):
        if o == off:
            fend = f["end"]; break
    if fend is None:
        nexts = sorted(o for o in funcs if o > off)
        fend = nexts[0] if nexts else min(off + 0x400, len(code))
    rows = _disasm(code, va0, off, fend)
    if not rows:
        return {"reconstructed": False, "reason": "no instructions"}
    from codebreaker.recover import datarec
    lo, hi = va0 + off, va0 + fend
    blocks = _blocks(rows, lo, hi)
    # resolve switch jump-tables: read consecutive in-function code pointers from each table base
    sw = {}                                               # block start -> (idx_reg, [case targets])
    extra = []
    for b in blocks:
        if b[2] == "switch":
            idxreg, base = b[3]
            targets = []
            for k in range(64):
                raw = datarec.read_va(module, base + k * 4, 4)
                if len(raw) != 4:
                    break
                t = int.from_bytes(raw, "little")
                if not (lo <= t < hi):
                    break
                targets.append(t)
            if targets:
                low, dflt = _switch_meta(blocks, b[0], idxreg)
                sw[b[0]] = (idxreg, targets, low, dflt); extra.extend(targets)
    if extra:                                             # rebuild with case targets as leaders
        blocks = _blocks(rows, lo, hi, extra_leaders=extra)
    ncond = sum(1 for b in blocks if b[2] in ("cond", "switch"))
    if ncond == 0:
        return {"reconstructed": False, "reason": "no branches (not a CFG case)"}
    calls, ret = _calls_by_block(module, va, blocks, arity)
    # lift the goto/label CFG into structured if / if-else / while where provable (else goto)
    from codebreaker.recover import structure
    blk_by_addr = {b[0]: b for b in blocks}
    seq = []
    for b0, body, kind, tgt, fall, cc in blocks:
        bl = structure.Blk(b0, kind, taken=tgt, fall=fall)
        bl._body = body
        if kind == "switch" and b0 in sw:
            bl.switch_reg, bl.cases, bl.switch_low, bl.switch_default = sw[b0]
        seq.append(bl)
    retstr = ip._arg(ret) if ret is not None else "0"
    from codebreaker.recover import rettype              # infer void / value / pointer return
    try:
        rtype = rettype.infer(module, va) or "unsigned"
    except Exception:
        rtype = "unsigned"
    retline = "return;" if rtype == "void" else "return %s;" % retstr
    from codebreaker.recover import fpblock
    from codebreaker.recover import simdblock
    from codebreaker.recover import ssa   # x87/SIMD + cross-block SSA
    try:
        ctx = ssa.from_blocks(blocks)                            # cross-block operand resolution
    except Exception:
        ctx = None

    def _body_lines(bl):
        return list(calls.get(bl.addr, [])) + fpblock.emit(bl._body) + simdblock.emit(bl._body)

    # short-circuit merge: consecutive cond blocks that jump to the SAME target are a && / || chain
    # (both jump to the short-circuit exit when their test holds) -> one OR-of-jumps predicate.
    epreds = (ctx or {}).get("preds", {})
    merged, k = [], 0
    while k < len(seq):
        bl = seq[k]
        if bl.kind == "cond":
            texts = [_predicate(bl._body, ctx, bl.addr)]
            while (k + 1 < len(seq) and seq[k + 1].kind == "cond"
                   and seq[k + 1].taken == bl.taken and bl.fall == seq[k + 1].addr
                   and not calls.get(seq[k + 1].addr)                      # pure test block only
                   and len(epreds.get(seq[k + 1].addr, ())) == 1):         # reached only by fall-through
                nb = seq[k + 1]
                texts.append(_predicate(nb._body, ctx, nb.addr))
                bl.fall = nb.fall
                k += 1
            if len(texts) > 1:
                bl._predtext = " || ".join("(%s)" % t for t in texts)
        merged.append(bl); k += 1
    seq = merged

    lines, _used = structure.structure(
        seq,
        predicate=lambda bl: getattr(bl, "_predtext", None) or _predicate(bl._body, ctx, bl.addr),
        body_lines=_body_lines,
        ret_line=lambda bl: retline)
    body_c = "\n".join("    " + ln for ln in lines)
    params = ", ".join("unsigned a%d" % i for i in range(arity))
    from codebreaker.recover import stackframe        # declare recovered local variables
    decls = stackframe.declarations(module, va)
    frame = ("\n".join(decls) + "\n\n") if decls else ""
    src = "%s sub_%x(%s)\n{\n%s%s\n}\n" % (rtype, off, params, frame, body_c)
    from codebreaker.recover import localname          # name locals from use (i/j/p/...)
    try:
        src = localname.apply(src, localname.suggest(module, va))
        src = structure.forfold(src)                            # i=init; while(cond){..i++} -> for(...)
    except Exception:
        pass
    from codebreaker.recover import seh                # wrap __try/__except if present
    h = seh.detect(module, va)
    if h is not None:
        src = seh.wrap(src, h, va0)
    return {"reconstructed": True, "verified": None, "nblocks": len(blocks), "nconds": ncond,
            "ncalls": sum(len(v) for v in calls.values()), "emitted_c": src, "seh": h is not None}
