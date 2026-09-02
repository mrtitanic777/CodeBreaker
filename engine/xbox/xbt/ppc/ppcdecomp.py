"""Ghidra-independent PowerPC decompiler — lift a function's machine code to
readable pseudo-C, using nothing but our own disassembler.

The model is deliberately simple and *always correct*: every GPR is a C
variable (``r0``..``r31``), every instruction becomes a C statement, memory
accesses become ``*(type*)(base + disp)``, and control flow is emitted in
goto-form (which is valid, compilable C for any CFG). On top of that base we do
two conservative structuring recoveries — forward `if`-then and backward
`do/while` — but ONLY when the shape is unambiguous; anything else falls back to
goto. That guarantees the output is never *wrong* structure, only sometimes
less pretty than a commercial decompiler.

Unhandled opcodes are passed through as ``/* mnemonic operands */`` comments so
the reader always sees exactly what could not be lifted — the decompiler never
silently drops or fabricates an effect.

Verified end-to-end in selftest: assemble a known function with our ppcasm,
decompile it, and check the C reproduces the intended semantics.
"""

import importlib
import sys

DESCRIPTION = "decompile Xbox 360 PowerPC functions to readable pseudo-C (Ghidra-independent)"


def _imp(name):
    """Import a sibling module whether we're loaded as a package or flat
    (selftest puts xbt/ppc on sys.path and imports flat)."""
    try:
        return importlib.import_module("." + name, __package__)
    except Exception:
        return importlib.import_module(name)

WORD = 4
# instructions that terminate a run (no fall-through)
_ENDERS = {"blr", "bctr", "b", "ba", "rfid", "rfi"}


# ── operand parsing ────────────────────────────────────────────────────────
def _split_ops(op_str):
    """Split a capstone op_str on top-level commas (memory ``d(rN)`` has none)."""
    return [t.strip() for t in op_str.split(",") if t.strip()]


def _is_reg(tok):
    return tok[:1] == "r" and tok[1:].isdigit()


def _imm(tok):
    try:
        return int(tok, 0)
    except ValueError:
        return None


def _mem(tok):
    """Parse ``disp(rA)`` -> (disp:int, 'rA'). Returns None if not a mem operand."""
    if "(" not in tok or not tok.endswith(")"):
        return None
    disp_s, base = tok[:-1].split("(", 1)
    disp_s = disp_s.strip() or "0"
    d = _imm(disp_s)
    if d is None or not _is_reg(base.strip()):
        return None
    return d, base.strip()


def _hex(n):
    if n is None:
        return "0"
    return ("-0x%X" % -n) if n < 0 else ("0x%X" % n)


def _rhs_add(ra, imm):
    if imm == 0:
        return ra
    return "%s - 0x%X" % (ra, -imm) if imm < 0 else "%s + 0x%X" % (ra, imm)


# ── one-instruction lifter ─────────────────────────────────────────────────
_LOADS = {"lwz": ("unsigned int", 4), "lwzu": ("unsigned int", 4),
          "lbz": ("unsigned char", 1), "lbzu": ("unsigned char", 1),
          "lhz": ("unsigned short", 2), "lhzu": ("unsigned short", 2),
          "lha": ("short", 2), "lhau": ("short", 2),
          "lwa": ("int", 4), "ld": ("unsigned long long", 8),
          "ldu": ("unsigned long long", 8)}
_STORES = {"stw": ("unsigned int", 4), "stwu": ("unsigned int", 4),
           "stb": ("unsigned char", 1), "stbu": ("unsigned char", 1),
           "sth": ("unsigned short", 2), "sthu": ("unsigned short", 2),
           "std": ("unsigned long long", 8), "stdu": ("unsigned long long", 8)}
_BINOP = {"add": "+", "or": "|", "and": "&", "xor": "^",
          "slw": "<<", "srw": ">>", "mullw": "*", "sld": "<<", "srd": ">>"}
_CONDSYM = {"beq": "==", "bne": "!=", "blt": "<", "bgt": ">", "ble": "<=", "bge": ">="}


def _lift(mn, ops, cr, used, symbols=None):
    """Return (list-of-C-statements, control) for one instruction.

    `cr` is a dict {field: (lhs, rhs, signed)} tracking the last compare so a
    later conditional branch can render its condition. `used` is a set of GPR
    names we touch (for declarations). `symbols` optionally maps a call target
    address to a readable name (imports, discovered functions). `control` is None
    or a tuple describing a branch: ('goto', target) | ('call', target) |
    ('cond', ctext, target) | ('ret',) | ('icall'|'ijmp', reg)."""
    base = mn.rstrip(".")
    dot = mn.endswith(".")

    def reg(t):
        if _is_reg(t) or (t[:1] == "f" and t[1:].isdigit()):
            used.add(t)
        return t

    # ---- no-op / moves -----------------------------------------------------
    if base in ("nop",) or (base == "ori" and ops[:3] == ["r0", "r0", "0x0"]):
        return [], None
    if base == "mr" and len(ops) == 2:
        return ["%s = %s;" % (reg(ops[0]), reg(ops[1]))], None
    if base == "li" and len(ops) == 2:
        return ["%s = %s;" % (reg(ops[0]), _hex(_imm(ops[1])))], None
    if base == "lis" and len(ops) == 2:
        return ["%s = 0x%X;" % (reg(ops[0]), (_imm(ops[1]) & 0xFFFF) << 16)], None

    # ---- arithmetic immediates --------------------------------------------
    if base in ("addi", "addic") and len(ops) == 3:
        i = _imm(ops[2])
        if ops[1] == "r0":                                 # addi rD,0,imm == li
            return ["%s = %s;" % (reg(ops[0]), _hex(i))], None
        return ["%s = %s;" % (reg(ops[0]), _rhs_add(reg(ops[1]), i))], None
    if base == "addis" and len(ops) == 3:
        return ["%s = %s + 0x%X;" % (reg(ops[0]), reg(ops[1]),
                                     (_imm(ops[2]) & 0xFFFF) << 16)], None
    if base in ("subi",) and len(ops) == 3:
        return ["%s = %s - 0x%X;" % (reg(ops[0]), reg(ops[1]), _imm(ops[2]))], None
    if base == "mulli" and len(ops) == 3:
        return ["%s = %s * %s;" % (reg(ops[0]), reg(ops[1]), _hex(_imm(ops[2])))], None
    if base in ("ori", "oris", "xori", "xoris", "andi", "andis") and len(ops) == 3:
        sym = {"o": "|", "x": "^", "a": "&"}[base[0]]
        i = _imm(ops[2])
        if base.endswith("s"):
            i = (i & 0xFFFF) << 16
        return ["%s = %s %s 0x%X;" % (reg(ops[0]), reg(ops[1]), sym, i & 0xFFFFFFFF)], None

    # ---- register-register -------------------------------------------------
    if base in _BINOP and len(ops) == 3:
        if base == "or" and ops[1] == ops[2]:              # or rD,rA,rA == mr
            return ["%s = %s;" % (reg(ops[0]), reg(ops[1]))], None
        return ["%s = %s %s %s;" % (reg(ops[0]), reg(ops[1]), _BINOP[base], reg(ops[2]))], None
    if base in ("subf", "sub") and len(ops) == 3:
        # subf rD,rA,rB = rB - rA
        return ["%s = %s - %s;" % (reg(ops[0]), reg(ops[2]), reg(ops[1]))], None
    if base in ("nand", "nor") and len(ops) == 3:
        sym = "&" if base == "nand" else "|"
        return ["%s = ~(%s %s %s);" % (reg(ops[0]), reg(ops[1]), sym, reg(ops[2]))], None
    if base == "andc" and len(ops) == 3:
        return ["%s = %s & ~%s;" % (reg(ops[0]), reg(ops[1]), reg(ops[2]))], None
    if base in ("divw", "divwu") and len(ops) == 3:
        cast = "(int)" if base == "divw" else "(unsigned)"
        return ["%s = %s%s / %s%s;" % (reg(ops[0]), cast, reg(ops[1]), cast, reg(ops[2]))], None
    if base == "sraw" and len(ops) == 3:
        return ["%s = (int)%s >> %s;" % (reg(ops[0]), reg(ops[1]), reg(ops[2]))], None
    if base == "neg" and len(ops) == 2:
        return ["%s = -%s;" % (reg(ops[0]), reg(ops[1]))], None
    if base in ("extsb", "extsh", "extsw") and len(ops) == 2:
        c = {"extsb": "signed char", "extsh": "short", "extsw": "int"}[base]
        return ["%s = (%s)%s;" % (reg(ops[0]), c, reg(ops[1]))], None
    if base in ("slwi", "srwi", "srawi") and len(ops) == 3:      # capstone rlwinm aliases
        sym = "<<" if base == "slwi" else ">>"
        pre = "(int)" if base == "srawi" else ""
        return ["%s = %s%s %s %s;" % (reg(ops[0]), pre, reg(ops[1]), sym, _hex(_imm(ops[2])))], None
    if base == "clrlwi" and len(ops) == 3:      # rlwinm rD,rA,0,n,31 — clear top n
        n = _imm(ops[2]) or 0
        return ["%s = %s & 0x%X;" % (reg(ops[0]), reg(ops[1]),
                                     (0xFFFFFFFF >> n) & 0xFFFFFFFF)], None
    if base == "clrrwi" and len(ops) == 3:      # clear bottom n bits
        n = _imm(ops[2]) or 0
        return ["%s = %s & 0x%X;" % (reg(ops[0]), reg(ops[1]),
                                     (~((1 << n) - 1)) & 0xFFFFFFFF)], None
    if base == "rotlwi" and len(ops) == 3:
        n = _imm(ops[2]) or 0
        return ["%s = (%s << %d) | (%s >> %d);" % (reg(ops[0]), reg(ops[1]), n,
                                                   reg(ops[1]), (32 - n) & 31)], None
    if base == "rlwinm" and len(ops) == 5:
        return ["%s = /* rlwinm */ %s;  // rotate&mask sh=%s mb=%s me=%s"
                % (reg(ops[0]), reg(ops[1]), ops[2], ops[3], ops[4])], None
    # ---- floating point (f* are double pseudo-vars) -----------------------
    if base in ("fadd", "fadds", "fsub", "fsubs", "fmul", "fmuls",
               "fdiv", "fdivs") and len(ops) == 3:
        sym = {"fadd": "+", "fsub": "-", "fmul": "*", "fdiv": "/"}[base.rstrip("s")]
        return ["%s = %s %s %s;" % (reg(ops[0]), reg(ops[1]), sym, reg(ops[2]))], None
    if base in ("fmadd", "fmadds", "fmsub", "fmsubs") and len(ops) == 4:
        sym = "+" if "madd" in base else "-"
        # fmadd fD,fA,fC,fB = fA*fC (+/-) fB
        return ["%s = %s * %s %s %s;" % (reg(ops[0]), reg(ops[1]), reg(ops[2]),
                                         sym, reg(ops[3]))], None
    if base in ("fnmadd", "fnmadds", "fnmsub", "fnmsubs") and len(ops) == 4:
        sym = "+" if "nmadd" in base else "-"
        return ["%s = -(%s * %s %s %s);" % (reg(ops[0]), reg(ops[1]), reg(ops[2]),
                                            sym, reg(ops[3]))], None
    if base in ("fmr", "fmr.") and len(ops) == 2:
        return ["%s = %s;" % (reg(ops[0]), reg(ops[1]))], None
    if base in ("fneg", "fabs", "fnabs") and len(ops) == 2:
        expr = {"fneg": "-%s", "fabs": "fabs(%s)", "fnabs": "-fabs(%s)"}[base]
        return ["%s = %s;" % (reg(ops[0]), expr % reg(ops[1]))], None
    if base in ("frsp",) and len(ops) == 2:
        return ["%s = (float)%s;" % (reg(ops[0]), reg(ops[1]))], None
    if base in ("fctiwz", "fctidz") and len(ops) == 2:
        return ["%s = (long long)%s;  // round toward zero" % (reg(ops[0]), reg(ops[1]))], None
    if base in ("lfs", "lfd") and len(ops) == 2:
        m = _mem(ops[1])
        if m:
            ty = "float" if base == "lfs" else "double"
            reg(m[1])
            return ["%s = *(%s*)(%s);" % (reg(ops[0]), ty, _rhs_add(m[1], m[0]))], None
    if base in ("stfs", "stfd") and len(ops) == 2:
        m = _mem(ops[1])
        if m:
            ty = "float" if base == "stfs" else "double"
            reg(m[1])
            return ["*(%s*)(%s) = %s;" % (ty, _rhs_add(m[1], m[0]), reg(ops[0]))], None
    if base in ("fcmpu", "fcmpo") and len(ops) == 3:
        field = ops[0] if ops[0].startswith("cr") else "cr0"
        cr[field] = (reg(ops[1]), reg(ops[2]), True)
        return [], None

    # ---- special-purpose register moves (lr/ctr as pseudo-vars) ------------
    if base == "mflr" and len(ops) == 1:
        used.add("lr"); return ["%s = lr;" % reg(ops[0])], None
    if base == "mtlr" and len(ops) == 1:
        used.add("lr"); return ["lr = %s;" % reg(ops[0])], None
    if base == "mfctr" and len(ops) == 1:
        used.add("ctr"); return ["%s = ctr;" % reg(ops[0])], None
    if base == "mtctr" and len(ops) == 1:
        used.add("ctr"); return ["ctr = %s;" % reg(ops[0])], None

    # ---- memory ------------------------------------------------------------
    if base in _LOADS and len(ops) == 2:
        m = _mem(ops[1])
        if m:
            ty, _sz = _LOADS[base]
            reg(m[1]); stmts = ["%s = *(%s*)(%s);" % (reg(ops[0]), ty, _rhs_add(m[1], m[0]))]
            if base.endswith("u"):
                stmts.append("%s = %s;" % (m[1], _rhs_add(m[1], m[0])))
            return stmts, None
    if base in _STORES and len(ops) == 2:
        m = _mem(ops[1])
        if m:
            ty, _sz = _STORES[base]
            reg(m[1]); stmts = ["*(%s*)(%s) = %s;" % (ty, _rhs_add(m[1], m[0]), reg(ops[0]))]
            if base.endswith("u"):
                stmts.append("%s = %s;" % (m[1], _rhs_add(m[1], m[0])))
            return stmts, None

    # ---- compares (record for the following branch) ------------------------
    if base in ("cmpw", "cmplw", "cmpwi", "cmplwi", "cmpd", "cmpld", "cmpdi", "cmpldi"):
        signed = "l" not in base[3:5]      # cmpw/cmpd signed; cmplw/cmpld unsigned
        o = ops[:]
        if o and o[0].startswith("cr"):
            field = o[0]; o = o[1:]
        else:
            field = "cr0"
        lhs = reg(o[0])
        rhs = reg(o[1]) if _is_reg(o[1]) else _hex(_imm(o[1]))
        cr[field] = (lhs, rhs, signed)
        return [], None

    # ---- branches ----------------------------------------------------------
    if base in ("b", "ba") and len(ops) == 1 and _imm(ops[0]) is not None:
        return [], ("goto", _imm(ops[0]))
    if base in ("bl", "bla") and len(ops) == 1 and _imm(ops[0]) is not None:
        used.add("r3")
        tgt = _imm(ops[0]) & 0xFFFFFFFF
        callee = (symbols or {}).get(tgt) or ("sub_%08X" % tgt)
        return ["r3 = %s(r3, r4, r5, r6, r7, r8);" % callee], None
    if base == "blr":
        used.add("r3")
        return [], ("ret",)
    if base in ("bctr", "bctrl"):
        used.add("r3")
        s = ["r3 = (*(func_t)ctr)();"] if base == "bctrl" else []
        return s, (None if base == "bctrl" else ("ijmp", "ctr"))
    if base in _CONDSYM and _imm(ops[-1]) is not None:
        o = ops[:]
        field = o[0] if o[0].startswith("cr") else "cr0"
        tgt = _imm(o[-1])
        cmp = cr.get(field)
        if cmp:
            lhs, rhs, signed = cmp
            if not signed:
                cond = "(unsigned)%s %s (unsigned)%s" % (lhs, _CONDSYM[base], rhs)
            else:
                cond = "%s %s %s" % (lhs, _CONDSYM[base], rhs)
        else:
            cond = "%s_%s" % (field, base)      # branch with no visible compare
        return [], ("cond", cond, tgt)
    if base in ("bdnz", "bdz"):
        tgt = _imm(ops[-1])
        used.add("ctr")
        pre = "--ctr, ctr != 0" if base == "bdnz" else "--ctr, ctr == 0"
        return [], ("cond", pre, tgt)

    # ---- system / unhandled: honest passthrough ---------------------------
    if base in ("mflr", "mtlr", "mfctr", "mtctr", "mfspr", "mtspr", "isync",
                "sync", "eieio", "sc", "trap", "tw", "twi", "dcbf", "dcbz", "icbi"):
        return ["/* %s %s */" % (mn, ", ".join(ops))], None
    return ["/* %s %s */" % (mn, ", ".join(ops))], None


# ── CFG construction ───────────────────────────────────────────────────────
def _instrs(code, base):
    disasm_ppc = _imp("disasm_ppc")
    out = []
    for va, raw, mn, op_str in disasm_ppc.listing(code, base):
        out.append((va, mn, _split_ops(op_str)))
    return out


def _leaders(instrs, base, code_len):
    lo, hi = base, base + code_len
    leaders = {base}
    for i, (va, mn, ops) in enumerate(instrs):
        b = mn.rstrip(".")
        ctl = None
        if b in ("b", "ba", "bl", "bla") and ops and _imm(ops[-1]) is not None:
            ctl = _imm(ops[-1])
        elif b in _CONDSYM and ops and _imm(ops[-1]) is not None:
            ctl = _imm(ops[-1])
        elif b in ("bdnz", "bdz") and ops and _imm(ops[-1]) is not None:
            ctl = _imm(ops[-1])
        if ctl is not None and lo <= ctl < hi:
            leaders.add(ctl)
        # instruction after any branch/ender starts a new block
        if b in _ENDERS or b in _CONDSYM or b in ("bl", "bla", "bctrl", "bdnz", "bdz"):
            nxt = va + WORD
            if lo <= nxt < hi:
                leaders.add(nxt)
    return sorted(leaders)


def _blocks(instrs, leaders):
    by_va = {va: (mn, ops) for va, mn, ops in instrs}
    order = [va for va, _, _ in instrs]
    idx = {va: k for k, va in enumerate(order)}
    blocks = []
    for k, start in enumerate(leaders):
        end = leaders[k + 1] if k + 1 < len(leaders) else None
        body = []
        for va in order[idx[start]:]:
            if end is not None and va >= end:
                break
            body.append((va, by_va[va][0], by_va[va][1]))
        blocks.append((start, body))
    return blocks


# ── emit pseudo-C ──────────────────────────────────────────────────────────
def decompile_code(code, base=0, name=None, symbols=None, jumptables=None):
    """Decompile a raw big-endian PPC code blob to a pseudo-C string. `symbols`
    optionally maps call-target addresses to readable names; `jumptables` maps a
    bctr address to (index_reg, [case targets]) so it renders as a switch."""
    name = name or ("sub_%08X" % base)
    jumptables = jumptables or {}
    instrs = _instrs(code, base)
    if not instrs:
        return "/* empty */\n"
    leaders = set(_leaders(instrs, base, len(code)))
    for _va, (_idx, targets) in jumptables.items():      # case targets are leaders
        for t in targets:
            if base <= t < base + len(code):
                leaders.add(t)
    leaders = sorted(leaders)
    blocks = _blocks(instrs, leaders)
    label = {b[0]: "L_%08X" % b[0] for b in blocks}
    order = [b[0] for b in blocks]
    nextof = {order[i]: (order[i + 1] if i + 1 < len(order) else None)
              for i in range(len(order))}

    used = set()
    targeted = set()                      # labels that actually get a goto
    lifted = []                           # (start, [lines], terminator)
    for start, body in blocks:
        cr = {}
        lines = []
        term = ("fall",)
        for j, (va, mn, ops) in enumerate(body):
            stmts, ctl = _lift(mn, ops, cr, used, symbols)
            lines.extend(stmts)
            if ctl is None:
                continue
            kind = ctl[0]
            if kind == "ret":
                term = ("ret",)
            elif kind == "goto":
                term = ("goto", ctl[1]); targeted.add(ctl[1])
            elif kind == "cond":
                term = ("cond", ctl[1], ctl[2]); targeted.add(ctl[2])
            elif kind == "ijmp":
                if va in jumptables:                     # resolved switch dispatch
                    idx, targets = jumptables[va]
                    used.add("r%d" % idx)
                    term = ("switch", "r%d" % idx, list(targets))
                    for t in targets:
                        targeted.add(t)
                else:
                    term = ("ijmp", ctl[1])
        lifted.append((start, lines, term))

    bystart = {s: (l, t) for s, l, t in lifted}
    pos = {s: i for i, s in enumerate(order)}

    # ---- conservative do-while loop structuring --------------------------
    # A block L ending `if (cond) goto H` with H at-or-before L (a back-edge),
    # a contiguous region H..L whose body blocks (H..before L) are all straight
    # fall-through, and which nothing outside the region jumps into, is a
    # do { H..L } while (cond) loop. Anything else stays goto-form.
    loop_hdr = {}                          # H_start -> (cond, [region starts])
    loop_member = set()
    for start, lines, term in lifted:
        if term[0] != "cond":
            continue
        cond, h = term[1], term[2]
        if h > start or h not in bystart:  # need a backward (or self) edge
            continue
        region = order[pos[h]:pos[start] + 1]
        rset = set(region)
        if rset & loop_member:             # no nested/overlapping loops (conservative)
            continue
        if any(bystart[b][1][0] != "fall" for b in region[:-1]):
            continue                       # body must be straight-line
        outside = set()
        for s2, _l2, t2 in lifted:
            if s2 in rset:
                continue
            if t2[0] == "goto":
                outside.add(t2[1])
            elif t2[0] == "cond":
                outside.add(t2[2])
        if outside & rset:                 # single-entry only
            continue
        loop_hdr[h] = (cond, region)
        loop_member |= rset

    # exact predecessor counts (how many goto/cond edges target each block)
    from collections import Counter
    target_count = Counter()
    for _s, _l, t in lifted:
        if t[0] == "goto":
            target_count[t[1]] += 1
        elif t[0] == "cond":
            target_count[t[2]] += 1
    consumed = set()

    # ---- conservative if-else structuring --------------------------------
    # A ends `if (cond) goto E`; its fall-through THEN is a single block (no other
    # predecessor) ending `goto J`; E is the block right after THEN, targeted only
    # by A, and falling straight into J. That is a clean two-armed if/else:
    #   if (!(cond)) { THEN } else { E }
    ifelse = {}                           # A_start -> (cond, THEN_start, E_start)
    for start, lines, term in lifted:
        if term[0] != "cond" or start in loop_member:
            continue
        cond, e = term[1], term[2]
        then = nextof.get(start)
        if e <= start or then is None or then not in bystart:
            continue
        if then in loop_member or then in consumed or target_count[then] != 0:
            continue                      # THEN reached only by A's fall-through
        tterm = bystart[then][1]
        if tterm[0] != "goto":
            continue
        j = tterm[1]
        if nextof.get(then) != e or e not in bystart or e in loop_member:
            continue                      # E is the block immediately after THEN
        if target_count[e] != 1 or bystart[e][1][0] != "fall":
            continue                      # E reached only by A's branch, no exit of its own
        if nextof.get(e) != j or j <= start:
            continue                      # both arms converge forward at J
        ifelse[start] = (cond, then, e)
        consumed.add(then)
        consumed.add(e)

    # ---- conservative if-then structuring --------------------------------
    # Fold the single, unambiguous shape:  A ends `if (cond) goto C`, its
    # fall-through B is a single block reached ONLY from A that then continues
    # to C (by falling into it, or by returning). Rewrite as `if (!(cond)) { B }`.
    # `!(cond)` keeps negation provably correct; anything else stays goto-form.
    ifthen = {}                           # A_start -> (cond, B_start, is_ret)
    for start, lines, term in lifted:
        if term[0] != "cond" or start in loop_member or start in ifelse:
            continue
        cond, c = term[1], term[2]
        b = nextof.get(start)
        if b is None or b not in bystart or b in targeted or b in consumed or b in loop_member:
            continue                      # B must be a real, single-predecessor block
        _bl, bterm = bystart[b]
        if nextof.get(b) != c or c <= start:
            continue                      # C must be the block right after B (forward)
        if bterm[0] == "fall":
            ifthen[start] = (cond, b, False); consumed.add(b)
        elif bterm[0] == "ret":
            ifthen[start] = (cond, b, True); consumed.add(b)

    # recompute which labels are still reached after folding
    targeted2 = set()
    for start, lines, term in lifted:
        if (start in consumed or start in ifthen or start in ifelse
                or start in loop_member):
            continue                      # folded blocks / loop bodies emit no goto/cond
        if term[0] == "goto":
            targeted2.add(term[1])
        elif term[0] == "cond":
            targeted2.add(term[2])
        elif term[0] == "switch":
            targeted2.update(term[2])

    # body emission
    out = []
    for start, lines, term in lifted:
        if start in consumed:
            continue                      # folded into the preceding if-block
        if start in loop_member and start not in loop_hdr:
            continue                      # emitted inline by its do-while header
        if start in loop_hdr:
            cond, region = loop_hdr[start]
            out.append("    do {")
            for rb in region:
                for ln in bystart[rb][0]:
                    out.append("        " + ln)
            out.append("    } while (%s);" % cond)
            continue                      # loop's own back-edge handled here
        if start in targeted2:
            out.append("%s:" % label[start])
        for ln in lines:
            out.append("    " + ln)
        if start in ifelse:
            cond, then, e = ifelse[start]
            out.append("    if (!(%s)) {" % cond)
            for ln in bystart[then][0]:
                out.append("        " + ln)
            out.append("    } else {")
            for ln in bystart[e][0]:
                out.append("        " + ln)
            out.append("    }")
            continue                      # both arms converge at J
        if start in ifthen:
            cond, b, is_ret = ifthen[start]
            blines, _bt = bystart[b]
            out.append("    if (!(%s)) {" % cond)
            for ln in blines:
                out.append("        " + ln)
            if is_ret:
                out.append("        return r3;")
            out.append("    }")
            continue                      # falls through to C
        if term[0] == "ret":
            out.append("    return r3;")
        elif term[0] == "goto":
            if term[1] != nextof.get(start):
                out.append("    goto %s;" % label.get(term[1], "loc_%08X" % term[1]))
        elif term[0] == "cond":
            tgt = label.get(term[2], "loc_%08X" % term[2])
            out.append("    if (%s) goto %s;" % (term[1], tgt))
        elif term[0] == "switch":
            idxreg, targets = term[1], term[2]
            out.append("    switch (%s) {" % idxreg)
            for i, t in enumerate(targets):
                out.append("        case %d: goto %s;" % (i, label.get(t, "loc_%08X" % t)))
            out.append("    }")
        elif term[0] == "ijmp":
            out.append("    goto *%s;  /* computed jump (switch/tail-call) */" % term[1])
        # 'fall' -> nothing

    def _key(r):
        return int(r[1:]) if r[1:].isdigit() else 99
    params = {"r3", "r4", "r5", "r6"}
    gpr = sorted((r for r in used if r[0] == "r" and r not in params), key=_key)
    fpr = sorted((r for r in used if r[0] == "f"), key=_key)
    spr = sorted(r for r in used if r in ("lr", "ctr"))
    args = "unsigned int r3, unsigned int r4, unsigned int r5, unsigned int r6"
    hdr = "unsigned int %s(%s)\n{\n" % (name, args)
    decls = []
    if gpr or spr:
        decls.append("    unsigned int %s;" % ", ".join(gpr + spr))
    if fpr:
        decls.append("    double %s;" % ", ".join(fpr))
    decl = ("\n".join(decls) + "\n") if decls else ""
    return hdr + decl + ("\n" if decl else "") + "\n".join(out) + "\n}\n"


def _symbol_map(x, funcs):
    """{addr: name} for call-target resolution: import thunks resolved to their
    kernel/XAM names, every other discovered function to sub_XXXXXXXX."""
    syms = {a: "sub_%08X" % a for a in funcs}
    ordinals = None
    try:
        ordinals = _imp("ordinals")
    except Exception:
        pass
    try:
        for lib, entries in x.imports().items():
            mod = lib.rsplit(".", 1)[0] if "." in lib else lib
            for a, ordinal, kind in entries:
                if kind != "thunk":
                    continue
                nm = None
                if ordinals is not None:
                    try:
                        r = ordinals.lookup(mod, ordinal)
                        if r:
                            nm = r[0]
                    except Exception:
                        pass
                syms[a & 0xFFFFFFFF] = nm or ("%s_ord%d" % (mod, ordinal))
    except Exception:
        pass
    return syms


def _image_readers(img, base, secs):
    """Big-endian guest-memory readers over a loaded image + an is_exec test."""
    import struct

    def _off(a):
        for s in secs:
            va = base + s["vaddr"]
            if va <= a < va + s["vsize"]:
                o = s["raw_off"] + (a - va)
                return o if 0 <= o < len(img) else None
        return None

    def rd(a, n, fmt):
        o = _off(a)
        return struct.unpack_from(fmt, img, o)[0] if o is not None and o + n <= len(img) else None

    def is_exec(a):
        for s in secs:
            va = base + s["vaddr"]
            if va <= a < va + s["vsize"]:
                ch = s.get("chars", 0) or 0
                return bool(ch & (0x20 | 0x20000000)) or s.get("code", False)
        return False

    return (lambda a: rd(a, 1, ">B"), lambda a: rd(a, 2, ">H"),
            lambda a: rd(a, 4, ">I"), is_exec)


def _jumptables(code, addr, img, base, secs):
    """Detect resolved bctr jump tables in a function -> {bctr_va: (idx_reg, [t])}."""
    try:
        jumptable = _imp("jumptable")
        disasm_ppc = _imp("disasm_ppc")
    except Exception:
        return {}
    insns = list(disasm_ppc.listing(code, addr))
    rd_u8, rd_u16, rd_u32, is_exec = _image_readers(img, base, secs)
    try:
        return jumptable.detect_jump_tables(insns, addr, addr + len(code),
                                            rd_u8, rd_u16, rd_u32, is_exec)
    except Exception:
        return {}


def from_xex(xex_path, addr):
    """Decompile the function at guest `addr` in a XEX (call targets resolved to
    import/function names, bctr jump tables recovered as switch statements)."""
    ppcdiscover = _imp("ppcdiscover")
    funcs, _src, x, img, secs = ppcdiscover.discover(xex_path)
    base = x.image_base or 0
    size = funcs.get(addr) or 0
    symbols = _symbol_map(x, funcs)
    # locate the code section containing addr
    for s in secs:
        va = base + s["vaddr"]
        raw_off = s["raw_off"]
        vsz = s["vsize"]
        if va <= addr < va + vsz:
            if not size:                       # unknown extent: run to section end
                size = va + vsz - addr
            start_off = raw_off + (addr - va)
            code = img[start_off:start_off + size]
            jts = _jumptables(code, addr, img, base, secs)
            return decompile_code(code, addr, "sub_%08X" % addr, symbols, jts)
    raise ValueError("address 0x%08X not in any code section" % addr)


def decompile_all(xex_path, limit=None):
    """Decompile every discovered function into one pseudo-C string. Returns
    (text, n_functions, n_failed). A function that fails to decompile becomes a
    /* could not decompile */ stub so one bad function never aborts the dump."""
    ppcdiscover = _imp("ppcdiscover")
    funcs, _src, x, img, secs = ppcdiscover.discover(xex_path)
    base = x.image_base or 0
    symbols = _symbol_map(x, funcs)

    def slice_of(a, n):
        for s in secs:
            va = base + s["vaddr"]
            if va <= a < va + s["vsize"]:
                o = s["raw_off"] + (a - va)
                return img[o:o + n]
        return b""

    addrs = sorted(funcs)
    if limit:
        addrs = addrs[:limit]
    parts = ["/* XboxTranspiler whole-program decompile: %d functions from %s */\n"
             % (len(addrs), xex_path)]
    ok = fail = 0
    for a in addrs:
        size = funcs.get(a) or 64
        code = slice_of(a, min(size, 0x2000))
        if not code:
            continue
        try:
            parts.append(decompile_code(code, a, "sub_%08X" % a, symbols))
            ok += 1
        except Exception as e:                # noqa: BLE001
            parts.append("/* sub_%08X: could not decompile (%s) */\n"
                         % (a, type(e).__name__))
            fail += 1
    return "\n".join(parts), ok, fail


# ── self-verification ──────────────────────────────────────────────────────
def _selfcheck():
    """Assemble a known function, decompile it, assert the C carries the
    intended semantics. Returns (ok, detail)."""
    ppcasm = _imp("ppcasm")
    src = """
        addi  r5, r3, 0x10
        add   r3, r5, r4
        cmpwi r3, 0x0
        bge   done
        neg   r3, r3
    done:
        blr
    """
    code = ppcasm.assemble(src, 0x82000000)
    c = decompile_code(code, 0x82000000, "sample")
    # the forward `bge` skip folds into a structured if-then (abs value)
    checks = [
        "r5 = r3 + 0x10;" in c,
        "r3 = r5 + r4;" in c,
        "if (!(r3 >= 0x0)) {" in c,        # structured, provably-correct negation
        "r3 = -r3;" in c,
        "return r3;" in c,
        "goto" not in c,                   # this shape needs no goto
    ]
    ok = all(checks)
    if not ok:
        return ok, c
    # a backward branch is a counting loop -> structured do/while
    loop_src = ("li   r3, 0x0\n"
                "loop:\n"
                "addi r3, r3, 0x1\n"
                "cmpwi r3, 0xA\n"
                "blt  loop\n"
                "blr\n")
    lc = decompile_code(ppcasm.assemble(loop_src, 0x82000000), 0x82000000, "loop")
    if "do {" not in lc or "} while (r3 < 0xA);" not in lc or "r3 = r3 + 0x1;" not in lc:
        return False, "counting loop was not structured as do/while:\n" + lc
    if "goto" in lc:
        return False, "do/while loop still emitted a goto:\n" + lc
    # a two-armed branch -> structured if/else
    ie_src = ("cmpwi r3, 0x0\n"
              "beq   elsearm\n"
              "li    r4, 0x1\n"           # THEN
              "b     join\n"
              "elsearm:\n"
              "li    r4, 0x2\n"           # ELSE
              "join:\n"
              "mr    r3, r4\n"
              "blr\n")
    iec = decompile_code(ppcasm.assemble(ie_src, 0x82000000), 0x82000000, "ifelse")
    if ("if (!(r3 == 0x0)) {" not in iec or "} else {" not in iec
            or "r4 = 0x1;" not in iec or "r4 = 0x2;" not in iec or "goto" in iec):
        return False, "if/else not structured:\n" + iec
    # jump-table dispatch -> switch (bctr with a resolved table)
    sw_src = ("mtctr r5\n"                 # 0x82000000
              "bctr\n"                     # 0x82000004  <- switch dispatch
              "li r3, 0xA\n"               # 0x82000008  case 0
              "blr\n"                      # 0x8200000C
              "li r3, 0xB\n"               # 0x82000010  case 1
              "blr\n")                     # 0x82000014
    swcode = ppcasm.assemble(sw_src, 0x82000000)
    jts = {0x82000004: (5, [0x82000008, 0x82000010])}
    swc = decompile_code(swcode, 0x82000000, "dispatch", jumptables=jts)
    if ("switch (r5) {" not in swc or "case 0: goto" not in swc
            or "case 1: goto" not in swc):
        return False, "switch dispatch not emitted:\n" + swc
    # sign-extends (regression: mnemonic type char)
    ec = decompile_code(ppcasm.assemble("extsb r3, r4\nextsh r5, r6\nblr\n", 0x82000000),
                        0x82000000, "e")
    if "r3 = (signed char)r4;" not in ec or "r5 = (short)r6;" not in ec:
        return False, "sign-extend lowering wrong:\n" + ec
    # floating point: hand-encode a blob (ppcasm doesn't assemble FP) and lift it
    import struct
    fadd = (63 << 26) | (1 << 21) | (2 << 16) | (1 << 11) | (21 << 1)          # fadd f1,f2,f1
    lfd = (50 << 26) | (3 << 21) | (1 << 16) | 8                               # lfd  f3,8(r1)
    fmadd = (63 << 26) | (5 << 21) | (2 << 16) | (4 << 11) | (3 << 6) | (29 << 1)  # fmadd f5,f2,f3,f4
    stfd = (54 << 26) | (5 << 21) | (1 << 16) | 16                             # stfd f5,16(r1)
    blr = 0x4E800020
    blob = struct.pack(">5I", fadd, lfd, fmadd, stfd, blr)
    fc = decompile_code(blob, 0x82000000, "fp")
    fp_checks = [
        "f1 = f2 + f1;" in fc,
        "f3 = *(double*)(r1 + 0x8);" in fc,
        "f5 = f2 * f3 + f4;" in fc,
        "*(double*)(r1 + 0x10) = f5;" in fc,
        "double " in fc,                          # f-regs declared as double
    ]
    if not all(fp_checks):
        return False, "FP lowering wrong:\n" + fc
    # symbol resolution: a bl to a known address renders as its name
    scode = ppcasm.assemble("bl 0x82000100\nblr\n", 0x82000000)
    sc = decompile_code(scode, 0x82000000, "s", symbols={0x82000100: "NtCreateMutant"})
    if "r3 = NtCreateMutant(" not in sc:
        return False, "symbol resolution wrong:\n" + sc
    return True, c


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  ppcdecomp <file.xex> <0xADDR>          decompile one function\n"
              "  ppcdecomp <file.xex> --all [out.c]     decompile every function\n"
              "  ppcdecomp --raw <file.bin> [0xBASE]    decompile a raw code blob\n"
              "  ppcdecomp --selftest")
        return
    if "--selftest" in a:
        ok, c = _selfcheck()
        print(c)
        print("SELFTEST:", "PASS" if ok else "FAIL")
        sys.exit(0 if ok else 1)
    # pull out an explicit --addr if given
    addr = None
    if "--addr" in a:
        i = a.index("--addr"); addr = int(a[i + 1], 0); del a[i:i + 2]
    if a and a[0] == "--raw":
        code = open(a[1], "rb").read()
        base = int(a[2], 0) if len(a) > 2 else 0
        print(decompile_code(code, base))
        return
    pos = [t for t in a if not t.startswith("--")]
    xex_path = pos[0]
    if "--all" in a:
        limit = int(a[a.index("--limit") + 1]) if "--limit" in a else None
        text, ok, fail = decompile_all(xex_path, limit)
        outs = [t for t in pos[1:] if t.endswith((".c", ".txt"))]
        if outs:
            open(outs[0], "w", encoding="utf-8").write(text)
            print("wrote %s: %d functions decompiled, %d stubbed" % (outs[0], ok, fail))
        else:
            print(text)
            print("/* %d functions decompiled, %d stubbed */" % (ok, fail), file=sys.stderr)
        return
    if addr is None:
        addr = int(pos[1], 0)
    print(from_xex(xex_path, addr))


if __name__ == "__main__":
    main()
