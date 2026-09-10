#!/usr/bin/env python3
"""
cfg.py — recover a function's CONTROL FLOW and emit it as a C skeleton.

WHY THIS TOOL AND NOT ANOTHER IDENTIFIER
    Everything needed to say WHAT a function is already exists: its address and
    size, its argument count from `retf N`, every call resolved to a name, the
    globals it reads and writes, struct offsets, dispatch tables, and now
    Microsoft's own symbols. `batch.py --digest` delivers all of it for forty
    functions in one call.

    What has never existed is control flow. Turning a stream of instructions
    into if / while / switch has been done by hand, by eye, every single time —
    and that, not identification, is why transcription is slow. It is also why
    the bodies that do exist are thin: `DefWindowProc` is counted as
    transcribed and omits twenty-four calls the binary makes.

WHAT IT PRODUCES, AND WHAT IT DOES NOT
    It produces a C skeleton whose SHAPE is mechanically derived: basic blocks
    split at every branch and branch target, loops from back edges, two-way
    branches from forward conditionals. Each block carries its instructions as
    comments, with calls named.

    It does NOT produce expressions. `cmp ax,0x10 / je` becomes
    `if (/* ax == 0x10 */)`, not `if (msg == WM_CLOSE)`. Naming the operands is
    reading, and reading is the human's job — the whole point of this project.

    So a skeleton from here is a **documented partial** until someone fills the
    expressions in. It is not a transcription and must never be counted as one.

THE COMPLETION TEST
    `verify.py` already answers "is this body complete": a transcription must
    make every call the binary makes at that address. This tool prints that
    call list in the header so the test is visible while writing.

Usage:
    python cfg.py USER 1 0x679b
    python cfg.py CARDFILE 4 0x0000 --raw     # blocks and edges, no C
"""
import re, sys
import idord, trace

# Conditional jumps, and the condition each one tests, written as the SOURCE
# comparison rather than the flag state — `jle` after `cmp a,b` means a <= b.
CC = {
    "je": "==", "jz": "==", "jne": "!=", "jnz": "!=",
    "jl": "<", "jnge": "<", "jle": "<=", "jng": "<=",
    "jg": ">", "jnle": ">", "jge": ">=", "jnl": ">=",
    "jb": "< (unsigned)", "jnae": "< (unsigned)", "jbe": "<= (unsigned)",
    "jna": "<= (unsigned)", "ja": "> (unsigned)", "jnbe": "> (unsigned)",
    "jae": ">= (unsigned)", "jnb": ">= (unsigned)",
    "js": "< 0", "jns": ">= 0", "jo": "overflow", "jno": "no overflow",
    "jcxz": "cx == 0",
    # loop dec cx then branch while cx != 0 (and, for loope/loopne, on ZF).
    # These carry a "loop" marker so the lifter emits the `cx--` side effect.
    "loop": "loop", "loope": "loop", "loopz": "loop",
    "loopne": "loop", "loopnz": "loop",
}
UNCOND = ("jmp",)
RET = ("ret", "retf", "iret")

# *** SHARED PROLOGUES WITH AN INLINE DESCRIPTOR ***
#
# Most of USER's exports do not have a prologue of their own. They begin with a
# call to one of these, which pops the return address, reads a TWO-BYTE
# DESCRIPTOR sitting immediately after the call site, steps over it, and builds
# the frame from it. GDI has one of its own that records into a metafile.
#
# Two consequences, both of which will mislead a reader who does not know:
#   1. the two bytes after the call are DATA. Disassembling them yields a
#      plausible-looking instruction (`sbb BYTE PTR [di],al` in DefWindowProc)
#      and can shift everything after it.
#   2. the function does not return through its own `ret`, so the calling
#      convention CANNOT be read from the return instruction the way it can
#      everywhere else.
#
# {module: {prologue_address: description}}
SHARED_PROLOG = {
    "USER": {0x889f: "plain shared far-proc prolog (142 call sites)",
             0x885c: "stack-checked shared far-proc prolog (94 call sites)"},
    "GDI":  {0x3718: "ALTRECORDMETA — records into a metafile, reads an inline word"},
}


def detect_prolog(sg, tgt):
    """Is `tgt` a shared prologue — one that reads a 2-byte INLINE DESCRIPTOR
    out of its own return address?  Returns a description, or None.

    The table above was hand-built for USER and GDI, which is why every APP
    decoded wrong: the apps use the same convention (they were built with the
    same 1985 runtime), each with its own prologue at its own address, and
    nobody had tabulated them. CALENDAR's is seg3:0x5772 and its window
    procedures' is seg3:0x5799; there is no reason to keep discovering these by
    hand when the shape is unmistakable:

        pop  bx                 ; bx = the return address = the descriptor
        push bp ; mov bp,sp     ; (a far one does `inc bp` first)
        mov  dx,WORD PTR cs:[bx]; READ THE DESCRIPTOR
        inc  bx ; inc bx        ; step over it — the body starts at bx

    Requiring both the `pop bx` and the `mov dx,cs:[bx]` is what makes this
    safe: plenty of routines pop bx, and none of the others then read code
    memory through it."""
    rows = trace.disasm_whole(sg.code[tgt:tgt + 24], base=tgt)[:10]
    txt = [a for _, a in rows]
    if not any(a.startswith("pop") and a.endswith("bx") for a in txt):
        return None
    if not any(re.search(r"mov\s+\w+,WORD PTR cs:\[bx\]", a) for a in txt):
        return None
    return ("shared prolog at 0x%04x — reads a 2-byte inline descriptor "
            "(frame size, argument bytes) from the call site" % tgt)


SYSMODS = {"KERNEL", "GDI", "USER", "SYSTEM", "KEYBOARD", "MOUSE",
           "DISPLAY", "SOUND", "COMM"}


def open_any(mod):
    """The system modules live inside the WIN100.BIN/OVL bundle; the apps are
    ordinary .EXE files. Everything else here works the same on both, so the
    only difference worth having is which loader opens the file."""
    if mod in SYSMODS:
        return idord.open_module(mod)
    from codebreaker.disasm_engine.disasm import open_ne
    return open_ne("files/WF/%s.EXE" % mod)


def rows_for(mod, seg, start, limit=0x600):
    ne = open_any(mod)
    ords = trace.load_ordinals("ORDINALS.md")
    sg = trace.Seg(ne, seg, ords)
    f = sg.funcs.get(start)
    if f:
        end = f["end"]
    else:
        nxt = [s for s in sorted(sg.funcs) if s > start]
        end = nxt[0] if nxt else start + limit
    end = min(end, start + limit, len(sg.code))
    rows = trace.disasm_whole(sg.code[start:end], base=start)

    # If the function opens with a call to a shared prologue, the two bytes
    # after that call are the inline descriptor — data. Disassemble the rest
    # from AFTER them, so the descriptor cannot masquerade as an instruction
    # and cannot shift what follows.
    prolog = None
    if rows:
        m = re.match(r"call\s+0x([0-9a-f]+)$", rows[0][1])
        if m:
            tgt = int(m.group(1), 16)
            note = SHARED_PROLOG.get(mod, {}).get(tgt) or detect_prolog(sg, tgt)
            if note:
                prolog = (tgt, note)
                dstart = rows[0][0] + 3          # past the 3-byte near call
                desc = sg.code[dstart:dstart + 2]
                rest = trace.disasm_whole(sg.code[dstart + 2:end], base=dstart + 2)
                rows = [rows[0],
                        (dstart, ".word 0x%02x%02x   ; INLINE DESCRIPTOR (data)"
                         % (desc[1], desc[0]))] + rest
    return sg, rows, start, end, prolog


def target_of(asm):
    m = re.match(r"(\w+)\s+0x([0-9a-f]+)$", asm)
    if m and (m.group(1) in CC or m.group(1) in UNCOND):
        return m.group(1), int(m.group(2), 16)
    return None, None


def build(rows, start, end):
    """-> (blocks, leaders). A block is (start_off, [rows], kind, target, fall)."""
    offs = [o for o, _ in rows]
    leaders = {start}
    for i, (o, a) in enumerate(rows):
        op, tgt = target_of(a)
        if tgt is not None and start <= tgt < end:
            leaders.add(tgt)
        if op or a.startswith(RET):
            if i + 1 < len(rows):
                leaders.add(rows[i + 1][0])
    leaders = sorted(l for l in leaders if l in set(offs))

    blocks = []
    for i, b in enumerate(leaders):
        nxt = leaders[i + 1] if i + 1 < len(leaders) else end
        body = [(o, a) for o, a in rows if b <= o < nxt]
        if not body:
            continue
        last = body[-1][1]
        op, tgt = target_of(last)
        if op in CC:
            blocks.append((b, body, "cond", tgt, nxt, CC[op]))
        elif op in UNCOND:
            blocks.append((b, body, "goto", tgt, None, None))
        elif last.startswith(RET):
            blocks.append((b, body, "ret", None, None, None))
        else:
            blocks.append((b, body, "fall", None, nxt, None))
    return blocks


_SYMCACHE = {}


def internal_symbols(mod, seg, sg):
    """{offset: MICROSOFT'S NAME} for intra-segment targets, from the SDK .SYM.

    An intra-segment call resolves to a bare address, so the whole call list of
    a USER function reads `Sub_0x07e5, Sub_0x09be, ...` — every internal callee
    anonymous. The SDK's symbol files name them (DESTROYWINDOW, INITPWSB, and
    ~1100 more), and symfile.py established that the debug and retail layouts
    agree, so those names attach to these addresses.

    The right .SYM segment is chosen the same way symscore does it — by which
    one's offsets land on THIS segment's independently carved function starts —
    rather than by trusting file order, which is not the retail order.
    """
    key = (mod, seg)
    if key in _SYMCACHE:
        return _SYMCACHE[key]
    out = {}
    try:
        import symscore
        groups, order = symscore.sym_groups(mod)
        starts = set(sg.funcs)
        # *** RANK BY ABSOLUTE HITS, NOT BY RATIO ***
        #
        # Ranking by ratio alone picks a segment with ONE symbol that happens
        # to land on a function start: a perfect 1.00 that beats _TEXT's 0.86
        # across 271 symbols. That is exactly the bug symscore.correlate
        # already guards against with `hit >= 2`, reappearing because this
        # simplified copy dropped the guard. A ratio computed over a tiny
        # denominator is not evidence.
        best, besthits, bestratio = None, 0, 0.0
        for nm in order:
            d = groups[nm]
            if not d:
                continue
            hits = len(set(d) & starts)
            ratio = hits / float(len(d))
            if hits >= 5 and ratio >= 0.5 and hits > besthits:
                best, besthits, bestratio = nm, hits, ratio
        if best:
            out = dict(groups[best])
    except Exception:
        out = {}
    _SYMCACHE[key] = out
    return out


def name_call(sg, off, asm, syms=None):
    if not asm.startswith(("call", "jmp")):
        return None
    try:
        kind, d = sg.resolve_call(off, asm)
    except Exception:
        return None
    if kind == "sys":
        return d[1] or d[0]
    if kind == "int":
        if syms and d in syms:
            return "%s /*0x%04x*/" % (syms[d], d)
        return "Sub_0x%04x" % d
    if kind == "far":
        return str(d)
    return None


def emit(sg, blocks, start, mod, seg, prolog=None):
    """A C skeleton whose SHAPE is derived and whose EXPRESSIONS are not."""
    syms = internal_symbols(mod, seg, sg)
    byoff = {b[0]: b for b in blocks}
    # back edges = loops
    back = set()
    for b, body, kind, tgt, fall, cc in blocks:
        if tgt is not None and tgt <= b:
            back.add((b, tgt))
    calls = []
    for b, body, kind, tgt, fall, cc in blocks:
        for o, a in body:
            n = name_call(sg, o, a, syms)
            if n and a.startswith("call"):
                calls.append(n)

    L = []
    L.append("/* %s seg%d:0x%04x — CONTROL-FLOW SKELETON (cfg.py)" % (mod, seg, start))
    L.append(" *")
    L.append(" * The SHAPE below is derived from the branch structure and is")
    L.append(" * mechanically correct. The CONDITIONS are not: `if (/* ax == 0x10 */)`")
    L.append(" * means the binary compares against 0x10 there, not that anyone has")
    L.append(" * worked out what ax holds. Naming the operands is the reading, and")
    L.append(" * this file is a DOCUMENTED PARTIAL until that is done.")
    L.append(" *")
    L.append(" * %d basic blocks, %d back edge(s)." % (len(blocks), len(back)))

    # *** THE CALLING CONVENTION, AND WHICH [bp+N] IS WHICH ARGUMENT ***
    #
    # This block exists because the absence of it caused a real error. A blind
    # reading of CARDFILE seg3:0x04d4 got the whole body right and the ARGUMENT
    # ORDER backwards, because far PASCAL's left-to-right push order was
    # assumed for a function that is far C.
    #
    # The return instruction settles it, always:
    #     retf N / ret N  -> callee cleans -> PASCAL -> pushed LEFT to RIGHT,
    #                        so parameter 1 sits at the HIGHEST [bp+N]
    #     retf   / ret    -> caller cleans -> C      -> pushed RIGHT to LEFT,
    #                        so parameter 1 sits at the LOWEST [bp+N]
    #
    # Only exported callbacks in this system are far PASCAL; internal helpers
    # are C convention even when they are far.
    rets = [a for _, body, k, _, _, _ in blocks if k == "ret" for _, a in body
            if a.startswith(("ret", "retf"))]
    conv = None
    if prolog:
        L.append(" *")
        L.append(" * *** SHARED PROLOGUE at seg%d:0x%04x — %s" % (seg, prolog[0], prolog[1]))
        L.append(" *")
        L.append(" * This function has no prologue of its own. The two bytes after the")
        L.append(" * opening call are an INLINE DESCRIPTOR the prologue reads and steps")
        L.append(" * over; they are data, and are shown below as `.word`, not")
        L.append(" * disassembled. (Left as code they read as a plausible instruction.)")
        L.append(" *")
        L.append(" * The calling convention CANNOT be taken from a `ret` here — the")
        L.append(" * function does not return through one. Use the entry table: an")
        L.append(" * exported USER function is far PASCAL, so parameter 1 sits at the")
        L.append(" * HIGHEST [bp+N].")
    elif rets:
        r = rets[-1]
        far = r.startswith("retf")
        base = 6 if far else 4
        m = re.match(r"retf?\s+0x([0-9a-f]+)", r)
        nbytes = int(m.group(1), 16) if m else None
        seen_off = sorted({int(x, 16) for x in re.findall(
            r"\[bp\+0x([0-9a-f]+)\]",
            "\n".join(a for _, bd, _, _, _, _ in blocks for _, a in bd))})
        # BOUND THE ARGUMENT REGION. Without a bound, one mis-disassembled byte
        # contributes a displacement like [bp+0x6702] and it is reported as an
        # argument. For PASCAL the bound is exact — `retf N` says the arguments
        # occupy exactly N bytes starting at `base`. For C convention nothing
        # states the count, so cap at a generous 32 bytes and say so.
        if nbytes is not None:
            hi = base + nbytes - 2
        else:
            hi = base + 30
        args = [o for o in seen_off if base <= o <= hi]
        L.append(" *")
        if nbytes is not None:
            conv = "%s PASCAL — `%s`, callee cleans %d bytes" % (
                "far" if far else "near", r, nbytes)
            order = "parameter 1 is at the HIGHEST offset (pushed left to right)"
            args = list(reversed(args))
        else:
            conv = "%s C — `%s`, caller cleans" % ("far" if far else "near", r)
            order = "parameter 1 is at the LOWEST offset (pushed right to left)"
        L.append(" * CONVENTION: %s" % conv)
        L.append(" *   %s" % order)
        if args:
            L.append(" *   so: " + ", ".join("arg%d = [bp+0x%02x]" % (i + 1, o)
                                             for i, o in enumerate(args)))
    if calls:
        L.append(" *")
        L.append(" * COMPLETION TEST — verify.py requires the finished body to make")
        L.append(" * every one of these calls:")
        seen, line = set(), " *   "
        for c in calls:
            if c in seen:
                continue
            seen.add(c)
            if len(line) + len(c) > 72:
                L.append(line); line = " *   "
            line += c + " "
        L.append(line)
    L.append(" */")
    L.append("")

    for b, body, kind, tgt, fall, cc in blocks:
        looped = any(s == b for _, s in back)
        L.append("L_%04x:%s" % (b, "   /* loop head */" if looped else ""))
        for o, a in body[:-1] if kind in ("cond", "goto") else body:
            n = name_call(sg, o, a, syms)
            L.append("    /* %04x  %-32s */%s" % (o, a, ("  " + n) if n else ""))
        if kind == "cond":
            # A bare `if (/* > (unsigned) */)` says nothing useful. The
            # comparison it tests is the last flag-setting instruction in the
            # block, so quote that alongside the sense of the branch: the pair
            # `or ax,ax` + `ja` reads as "ax != 0, unsigned" at a glance.
            flagsrc = ""
            for o2, a2 in reversed(body[:-1]):
                if re.match(r"(cmp|test|or|and|sub|add|dec|inc|xor|neg|shl|shr|sar|div|mul)\b", a2):
                    flagsrc = a2
                    break
            cond = ("%s  ->  %s" % (flagsrc, cc)) if flagsrc else cc
            L.append("    if (/* %s */)  goto L_%04x;   /* %04x %s */"
                     % (cond, tgt, body[-1][0], body[-1][1]))
            L.append("    goto L_%04x;" % fall)
        elif kind == "goto":
            L.append("    goto L_%04x;                  /* %04x */" % (tgt, body[-1][0]))
        elif kind == "ret":
            L.append("    return;")
        else:
            L.append("    goto L_%04x;" % fall)
        L.append("")
    return "\n".join(L)


def main():
    mod, seg, start = sys.argv[1].upper(), int(sys.argv[2]), int(sys.argv[3], 0)
    sg, rows, s, e, prolog = rows_for(mod, seg, start)
    blocks = build(rows, s, e)
    if "--raw" in sys.argv:
        print("# %s seg%d:0x%04x  %d bytes, %d blocks" % (mod, seg, s, e - s, len(blocks)))
        for b, body, kind, tgt, fall, cc in blocks:
            t = ("-> 0x%04x" % tgt) if tgt is not None else ""
            f = ("fall 0x%04x" % fall) if fall is not None else ""
            print("  L_%04x  %2d instr  %-5s %-11s %s" % (b, len(body), kind, t, f))
        return
    print(emit(sg, blocks, s, mod, seg, prolog))


if __name__ == "__main__":
    main()
