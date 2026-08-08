#!/usr/bin/env python3
r"""raiser.py — the CONDITIONAL-class raiser: lifted [AUTO] C -> compilable structured C
that MSVC 2.0 recompiles to the shipped bytes (relocations masked).

Proven recipe (see byte-exact-c-raiser memory): absolute addresses are NAMED GLOBALS (so
the compiler emits direct-absolute + a masked relocation, not a register-load); the guard-
skip `if(GUARD) goto L; BODY; L:` structures to `if(!(GUARD)){BODY}`; compile on the 2.0
oracle and compare with relocs masked. This automates that over the conditional class and
reports the honest match rate — the real measure of how much of Windows we can recompile.

    wt --target win95 raiser --measure --limit 500     # match-rate over sampled conditionals
    wt --target win95 raiser --show MODULE:seg:off      # show one function's candidate + result

Scope (this increment): functions whose body is globals + args + one guard-skip or if-then,
no calls (sub_/fnptr), no raw segment/registers. Misses are categorized -> next raiser work.
"""
import glob
from engine.foundation import cores
import os
import re
import sys

from engine.foundation import layout
matchbatch = cores.get('matchbatch')
try:
    from engine.ir_engine import structure2 as _s2
except Exception:
    _s2=None   # shipped_bytes (db-aware), _asm_for

CTYPE = {"8": "unsigned char", "16": "unsigned short", "32": "unsigned", "80": "long double"}


def rewrite_globals(body):
    """*(uN*)(0xADDR) -> g_ADDR (a named extern of the right width). Returns
    (rewritten_body, {name: ctype}). Absolute addresses become global variables so the
    compiler emits direct-absolute [disp32] + a relocation the oracle masks."""
    externs = {}

    def repl(m):
        w, addr = m.group(1), m.group(2)
        name = "g_" + addr[2:]
        externs[name] = CTYPE.get(w, "unsigned")
        return name

    rw = re.sub(r"\*\(u(\d+)\*\)\((0x[0-9a-f]+)\)", repl, body)
    return rw, externs


REG = r"(?:e[abcd]x|e[sd]i|ebp)"          # 32-bit GP registers (NOT lanes al/ah/ax/cx)
_HARD = re.compile(r"\b([abcd][hl]|[abcd]x|e?sp|__xch|goto \*)\b|\b(fs|gs|es|cs|ds|ss)\b")


def strip_frame(body):
    """Remove the prologue/epilogue frame scaffolding the faithful lift now surfaces and
    that the compiler RE-SYNTHESISES for itself: the stack-pointer adjustments (add/sub
    esp,N for pushed-arg cleanup and constant local-alloc), the frame set/teardown
    (mov ebp,esp / mov esp,ebp), and an alloca save/restore PAIR (`t = esp; ...; esp = t`).
    None of these carry a data value the body's computation reads -- esp is the stack
    pointer, and the compiler rebuilds every one of them from the declared C locals and
    the calling convention. So dropping them yields the same computation with a frame the
    compiler is free to lay out (which is exactly what byte-exact recompilation needs).

    Returns the cleaned body, or None if esp/ebp SURVIVES as a real value (a variable
    alloca `esp - eax`, or esp read into a computation) -- genuinely not recoverable yet.
    A no-op on frame-free bodies (the common already-in-scope case)."""
    if not re.search(r"\be?[sb]p\b", body):
        return body                                   # nothing to strip
    saved = set()                                     # temps holding a saved esp
    out = []
    for ln in body.split("\n"):
        s = ln.strip()
        m = re.match(r"(v\d+|e[abcd]x|e[sd]i|loc_[0-9a-f]+) = esp;$", s)
        if m:                                         # frame save into a temp -> remember, drop
            saved.add(m.group(1)); continue
        m = re.match(r"esp = (v\d+|e[abcd]x|e[sd]i|loc_[0-9a-f]+);$", s)
        if m and m.group(1) in saved:                 # matching restore -> drop
            continue
        if re.match(r"esp = \(esp [+-] 0x[0-9a-f]+\);$", s):   # arg cleanup / const local-alloc
            continue
        if re.match(r"esp = \(?ebp\)?;$", s) or re.match(r"ebp = \(?esp\)?;$", s):  # frame set/teardown
            continue
        out.append(ln)
    body2 = "\n".join(out)
    if re.search(r"\be?sp\b", body2):                 # esp still used as a value -> honest oos
        return None
    return body2


def fastcall_prep(body, args):
    """If the body reads ecx/edx before defining them, those are register-convention inputs.
    When they're the ONLY register inputs, turn them into __fastcall params (ecx=1st, edx=2nd)
    and return (body, new_args, '__fastcall'); a dummy edx slot is inserted when only ecx is
    used with stack args (thiscall-shaped). No register inputs -> (body, args, '__stdcall').
    A non-ecx/edx register input (esi/edi/ebx/ebp) -> None (out of scope, callee-saved)."""
    inp = []
    for r in ("ecx", "edx", "esi", "edi", "ebx", "ebp"):
        fu = re.search(r"\b%s\b" % r, body)
        fd = re.search(r"\b%s\s*=" % r, body)
        if fu and (not fd or fu.start() < fd.start()):
            inp.append(r)
    if not inp:
        return body, args, "__stdcall"
    if set(inp) - {"ecx", "edx"}:
        return None
    stack = [] if not args.strip() or args.strip() == "void" else [args]
    params = []
    if "ecx" in inp:
        body = re.sub(r"\becx\b", "pcx", body); params.append("int pcx")
    if "edx" in inp:
        body = re.sub(r"\bedx\b", "pdx", body); params.append("int pdx")
    elif "ecx" in inp and stack:
        params.append("int pdx")                      # dummy edx slot so stack args line up
    return body, ", ".join(params + stack), "__fastcall"


def register_recover(body):
    """Data-flow recovery over register temporaries: fold the lift's register-machine trace
    into expressions/variables so the C reads like source and is far likelier to recompile
    to the shipped bytes. Three transforms, applied within straight-line runs (never across
    a branch/label — that would be unsound):
      * copy/expression propagation  `R = EXPR;` then R used once before any redef  -> inline
      * dead-def elimination         `R = pure;` R never used                        -> drop
      * survivors                    registers still live across branches            -> locals
    Returns (new_body, {reg: 'unsigned'}) or None if the body has HARD constructs (byte/word
    lanes, segment regs, esp, xchg, computed jumps) that aren't clean-C recoverable yet."""
    if _HARD.search(body):
        return None
    # INPUT registers: a GP register whose first mention is a USE (not `R =`) is read as
    # incoming — a __fastcall param (ecx/edx) or callee-saved restore. Not clean-C
    # recoverable yet; bail so we never emit an uninitialized-local (wrong) candidate.
    for r in set(re.findall(REG, body)):
        first_def = re.search(r"\b%s\s*=" % r, body)
        first_use = re.search(r"\b%s\b" % r, body)
        if first_use and (not first_def or first_use.start() < first_def.start()):
            # allow `R = ...R...` self-ref where the def IS the first mention
            if not (first_def and first_def.start() == first_use.start()):
                return None

    lines = body.split("\n")

    def _is_barrier(s):
        return bool(re.search(r"[{}]|goto|:\s*$", s))     # block/branch/label ends the run

    changed, guard = True, 0
    while changed and guard < 60:
        changed = False
        guard += 1
        for i, ln in enumerate(lines):
            m = re.match(r"\s*(%s|v\d+) = (.+);\s*$" % REG, ln)
            if not m:
                continue
            var, expr = m.group(1), m.group(2)
            if re.search(r"\bsub_|\(fnptr\)|\(\*fp_", expr):
                continue                                  # call def: side effects, don't duplicate
            if re.search(r"&\s*\(*\s*%s\b" % re.escape(var), body):
                continue                                  # address-taken -> must stay a
                #  materialised local: `tmp = v; f(&tmp)` differs from `f(&v)` in the bytes
            # Scan var's WHOLE live range (def -> next redef), counting straight-line-run
            # uses and flagging any CROSS-BARRIER use (past a branch/label/call). A value
            # read past a barrier is a genuine local: inlining or dropping it there would
            # leave the variable undefined in the emitted C (counting only to the first
            # barrier was a liveness bug that corrupted candidates and, tellingly, matches
            # the shipped code's own explicit stack locals when we keep it).
            run_use_j, run_uses, cross_use, seen_barrier = None, 0, False, False
            for j in range(i + 1, len(lines)):
                s = lines[j]
                rhs = s.split("=", 1)[1] if re.match(r"\s*(%s|v\d+)\s*=" % REG, s) else s
                n = len(re.findall(r"\b%s\b" % re.escape(var), rhs))   # uses = RHS/cond mentions
                if n:
                    if seen_barrier:
                        cross_use = True
                    else:
                        run_uses += n
                        if run_use_j is None:
                            run_use_j = j
                if re.match(r"\s*%s\b\s*=(?!=)" % re.escape(var), s):
                    if seen_barrier:
                        # This "redef" is PAST a barrier -- it may be a SIBLING-branch assignment
                        # (`if{ R=1 } else{ R=2 }`), not a kill on this path. A control-flow-blind
                        # scan would drop the if-branch R=1 as dead (the else R=2 looked like a
                        # redef before the post-merge use), leaving an empty branch that MSVC then
                        # compiles with INVERTED polarity (je vs jne) -- a byte-exact miss AND a
                        # correctness bug. Keep the def; a genuinely dead one MSVC elides anyway.
                        cross_use = True
                    break                                 # var redefined here — live range ends
                if _is_barrier(s):
                    seen_barrier = True
            if not cross_use and run_uses == 1:           # single same-run use -> inline the def
                lines[run_use_j] = re.sub(r"\b%s\b(?!\s*=)" % re.escape(var),
                                          "(%s)" % expr, lines[run_use_j], count=1)
                lines[i] = None
                changed = True
                break
            if not cross_use and run_uses == 0 and \
                    not re.search(r"\bsub_|\(fnptr\)|\(\*fp_|\*\(|=\s*\*|__", expr):
                lines[i] = None                           # truly dead pure def -> drop
                changed = True
                break
            # else: multiple uses OR a cross-barrier use -> KEEP the def (a real local)
        lines = [l for l in lines if l is not None]

    body2 = "\n".join(lines)
    survivors = {r for r in re.findall(REG, body2)}
    return body2, {r: "unsigned" for r in survivors}


def rewrite_calls(body):
    """Turn lifted calls into real C calls whose rel32/reloc the oracle masks:
      * `(*(fnptr)*(uN*)(0xADDR))(a)` -> `(*fp_ADDR)(a)`  (global function pointer)
      * `sub_HEX(a,b)`                -> `C_HEX(a,b)`      (extern callee)
    Run AFTER rewrite_globals so data-global derefs are already names (no nested parens
    left in the direct-call args). Returns (body, {fp_name:None}, {C_name:argcount})."""
    fps, callees = {}, {}

    def _ind(m):
        fps["fp_" + m.group(1)[2:]] = None
        return "(*fp_%s)" % m.group(1)[2:]

    body = re.sub(r"\(\*\(fnptr\)\*\(u\d+\*\)\((0x[0-9a-f]+)\)\)", _ind, body)

    def _dir(m):
        h, argstr = m.group(1), m.group(2)
        callees["C_" + h] = 0 if not argstr.strip() else argstr.count(",") + 1
        return "C_%s(%s)" % (h, argstr)

    prev = None
    while prev != body:                                  # innermost-out for any nesting
        prev = body
        body = re.sub(r"\bsub_([0-9a-f]+)\(([^()]*)\)", _dir, body)
    return body, fps, callees


def structure(body):
    """Structure the control flow. guard-skip `if (G) goto L; BODY; ... L:` -> `if (!(G))
    { BODY }`. if-then `if (C) { ... }` passes through. Returns structured body or None if
    the shape isn't handled yet."""
    gm = re.search(r"if \((.+?)\) goto (\w+);(.*)", body, re.S)
    if gm:
        guard, label, tail = gm.group(1), gm.group(2), gm.group(3)
        lm = re.search(r"\n\s*%s:\s*\n?" % re.escape(label), tail)
        if not lm:
            return None
        between = tail[:lm.start()]
        after = tail[lm.end():]
        # De Morgan the guard into its positive form (a==0||b>=c -> a!=0&&b<c) so the branch
        # structure matches; falls back to !(guard) which the compiler simplifies.
        return "if (!(%s)) {%s\n}\n%s" % (guard.strip(), between, after)
    if "goto" not in body:
        return body                                  # straight-line or already-structured if-then/else
    return None                                      # goto that isn't a recognized guard-skip


def _sig(args, ret_is_void, conv):
    a = args.strip()
    if not a or a == "void":
        params = "void"
    else:
        # keep the lift's own arg NAMES (arg2, arg3, ...) so body references stay in sync
        params = ", ".join("int %s" % p.strip().split()[-1] for p in a.split(","))
    rt = "void" if ret_is_void else "int"
    return "%s %s f(%s)" % (rt, conv, params)


def _size_stack_buffers(st):
    """Declare address-taken `loc_XX` locals as correctly-SIZED char buffers instead of scalars.
    A lifter local named by its frame offset (loc_64 = ebp-0x64) whose ADDRESS is taken (`&loc_64`,
    passed to a callee) is a stack BUFFER, but the lift declared it `u32` (4 bytes). MSVC then
    allocates a 4-byte frame (`sub esp,4`) where the original reserved the buffer (`sub esp,0x64`),
    so the prologue and every `[ebp-X]` offset diverge -> a byte-exact miss. Recover each buffer's
    size from the gap to the next-higher local (or the frame base) and emit `char loc_X[size];` so
    the frame matches. Only sizes >4 become arrays (scalars stay scalars)."""
    taken = set(re.findall(r"&\s*(loc_[0-9a-f]+)", st))
    if not taken:
        return st
    offs = {n: int(n[4:], 16) for n in set(re.findall(r"\b(loc_[0-9a-f]+)\b", st))}
    order = sorted(offs.values())
    sized = {}
    for n in taken:
        x = offs[n]
        prev = max([o for o in order if o < x], default=0)
        if x - prev > 4:
            sized[n] = x - prev
    if not sized:
        return st

    def _fix(m):
        names = [x.strip() for x in m.group(1).split(",")]
        keep = [x for x in names if x not in sized]
        out = ("    unsigned %s;\n" % ", ".join(keep)) if keep else ""
        return out + "".join("    char %s[0x%x];\n" % (n, sized[n]) for n in sorted(sized))
    return re.sub(r"    unsigned ([^;]+);\n", _fix, st, count=1)


def candidate(args, body, conv="__stdcall", callee_conv="__stdcall", want_conv=None):
    """Build a compilable candidate from a lifted conditional/call body, or None if out of
    scope (raw registers/segments, unrecovered condition, un-lifted opcodes).

    want_conv (from the shipped ret: __cdecl for `ret`/0xC3, __stdcall for `ret N`/0xC2):
    the stack-args convention CAN'T be read from the body -- __cdecl and __stdcall both
    take stack args and differ only in WHO pops them (the ret). fastcall_prep defaults
    non-register-input functions to __stdcall, but EXE code is largely __cdecl, so that
    emitted a wrong `ret N` (extra bytes) on every cdecl function. When the body isn't
    __fastcall, honour want_conv so the emitted ret matches the shipped one."""
    if any(x in body for x in ("__asm", "cond not recovered", "__fpop", "__inp", "__out")):
        return None
    # A LIVE condition flag / carry helper that survived into the body (CF/ZF/SF/OF/PF,
    # __carry/__borrow/__ovf/__cf) means carry-dependent multi-word arithmetic (adc/sbb) or
    # an unstructured flag use -- not clean-C recoverable, and it leaks undeclared
    # identifiers. Bail to honest out-of-scope instead of a candidate that can't compile.
    if re.search(r"\b(?:[CZSOP]F|__carry|__borrow|__cf\d+|__ovf_\w+)\b", body):
        return None
    # CALLEE CONVENTION: a direct call followed by a caller-side arg cleanup (`esp = (esp
    # + N)`) means that callee is __cdecl (the CALLER pops the args); no cleanup => the
    # callee pops them itself (__stdcall). Detect it NOW, keyed by callee address -- the
    # very next thing strip_frame does is delete that `esp = (esp + N)` line. Emitting the
    # callee extern with the right convention makes the compiler re-emit (or omit) the
    # `add esp,N` cleanup, matching the shipped bytes.
    # Same tell for INDIRECT calls: a fn-pointer call followed by the cleanup => that fn
    # pointer is __cdecl too. Track global fn-ptrs (fp_ADDR) by address, and a flag for the
    # register/local-indirect calls that share the `fnptr` typedef.
    cdecl_callees, cdecl_fps, cdecl_fnptr = set(), set(), False
    _bl = body.split("\n")
    for _i in range(len(_bl) - 1):
        if not re.match(r"\s*esp = \(esp \+ 0x[0-9a-f]+\);\s*$", _bl[_i + 1]):
            continue
        _mc = re.search(r"\bsub_([0-9a-f]+)\(", _bl[_i])
        _mf = re.search(r"\(\*\(fnptr\)\*\(u\d+\*\)\((0x[0-9a-f]+)\)\)", _bl[_i])
        if _mc:
            cdecl_callees.add("C_" + _mc.group(1))
        elif _mf:
            cdecl_fps.add("fp_" + _mf.group(1)[2:])
        elif "(fnptr)" in _bl[_i]:
            cdecl_fnptr = True
    body = strip_frame(body)                          # drop compiler-resynthesised frame ops
    if body is None:
        return None                                  # esp survives as a value -> genuinely hard
    fc = fastcall_prep(body, args)                   # ecx/edx inputs -> __fastcall params
    if fc is None:
        return None                                  # non-fastcall register input (callee-saved)
    body, args, conv = fc
    if conv == "__stdcall" and want_conv in ("__cdecl", "__stdcall"):
        conv = want_conv                             # recover cdecl vs stdcall from the shipped ret
    rr = register_recover(body)                      # data-flow: fold registers into exprs/locals
    if rr is None:
        return None                                  # HARD: lanes/segment/esp/xchg/computed-jump
    body, reglocals = rr
    # fn-ptr calls FIRST: `(*(fnptr)*(uN*)(0xADDR))` -> `(*fp_ADDR)`, before rewrite_globals
    # would eat the inner *(uN*)(0xADDR) and strand the `fnptr` cast.
    fps = {}

    def _ind(m):
        fps["fp_" + m.group(1)[2:]] = None
        return "(*fp_%s)" % m.group(1)[2:]

    body = re.sub(r"\(\*\(fnptr\)\*\(u\d+\*\)\((0x[0-9a-f]+)\)\)", _ind, body)
    rw, gexterns = rewrite_globals(body)             # remaining data globals (removes their parens)
    rw, _fps2, callees = rewrite_calls(rw)           # direct sub_ calls -> extern callees
    st = (_s2.structure2(rw) if _s2 else None)
    if st is None:
        st = structure(rw)
    if st is None:
        return None
    st = re.sub(r"\bu(\d+)\b", lambda m: CTYPE.get(m.group(1), "unsigned"), st)
    st = _size_stack_buffers(st)                     # address-taken locals -> sized char buffers
    ret_is_void = "return" not in re.sub(r"return\s*;", "", st)  # only bare `return;`
    st = "".join("    %s %s;\n" % (t, n) for n, t in sorted(reglocals.items())) + st  # survivor regs -> locals
    decls = "".join("extern %s %s;\n" % (t, n) for n, t in sorted(gexterns.items()))
    for n in sorted(fps):                            # global function pointers
        fcc = "__cdecl" if n in cdecl_fps else callee_conv   # cdecl fn-ptr => caller pops
        decls += "extern int (%s *%s)();\n" % (fcc, n)
    for n, k in sorted(callees.items()):             # direct extern callees, prototyped by arg count
        params = ",".join(["int"] * k) if k else "void"
        cc = "__cdecl" if n in cdecl_callees else callee_conv   # cdecl callee => caller pops
        decls += "extern int %s %s(%s);\n" % (cc, n, params)
    if "(fnptr)" in st:                              # register/global-indirect calls kept by the
        tcc = "__cdecl" if cdecl_fnptr else callee_conv        # faithful lift -> declare the cast type
        decls = ("typedef int (%s *fnptr)();\n" % tcc) + decls
    sig = _sig(args, ret_is_void, conv)
    return decls + sig + " {\n" + st + "\n}\n"


def _conv_from_shipped(sb):
    if len(sb) >= 3 and sb[-3] == 0xC2:
        return "__stdcall"
    if sb and sb[-1] == 0xC3:
        return "__cdecl"
    return None


def _try_match(cc32, csrc, sb):
    """Compile csrc on the 2.0 oracle at a few flags; return matching flag or None.
    Compares with cc32's relocation mask (named-global disp32s are masked).

    Flag order leads with the SIZE-optimizing family (/O1, /Oxs): the empirical flag
    bake-off showed Win95 retail was built optimize-for-SIZE (/O1 strictly dominates
    /O2 -- reproduces every /O2 match plus functions /O2 misses; plain /Ox favor-speed
    matches nothing), consistent with the historical record. /Oy- variants keep the
    caller frame (frame-pointer omission was OFF in the shipped build)."""
    for fl in ("/c /O1", "/c /O1 /Oy-", "/c /Oxs", "/c /O2", "/c /O2 /Oy-",
               "/c /Ox", "/c /Ox /Oy-", "/c /Os", "/c /Gy"):
        r = cc32.compile_c(csrc, flags=fl)
        if "obj" not in r:
            continue
        fns = cc32.Obj(r["obj"]).functions()
        cand = next((v for k, v in fns.items() if k.startswith("_f")), None)
        if not cand:
            continue
        code, mask = bytes(cand[0]), set(cand[1])
        if len(code) == len(sb) and all(a == b for i, (a, b) in enumerate(zip(code, sb)) if i not in mask):
            return fl
    return None


FN_RE = re.compile(
    r"/\* \[AUTO\] \w+\s+\w+ seg(\d+):0x([0-9a-f]+)\s+clean \*/\n"
    r"[A-Za-z_][\w ]*\**\w+\(([^)]*)\)\n\{(.*?)\n\}", re.S)


def _iter_inscope(limit):
    """Non-trivial functions the raiser can attempt: has a branch OR a call, no loop.
    (The earlier if-only iterator missed call-then-return functions — which match well —
    and understated the rate badly.)"""
    for f in glob.glob(os.path.join(layout.generated_dir(), "**", "*.c"), recursive=True):
        if not re.search(r"\.(dll|exe)\.c$", f):
            continue
        mod = os.path.basename(f)[:-2]
        txt = open(f, encoding="utf-8", errors="replace").read()
        for m in FN_RE.finditer(txt):
            seg, off, args, body = m.groups()
            if "while" in body or "for" in body:
                continue
            if not (re.search(r"\bif\b", body) or "sub_" in body or "fnptr" in body):
                continue
            yield mod, seg, off, args, body
            limit -= 1
            if limit <= 0:
                return


def main():
    a = sys.argv[1:]
    import cc32
    if "--show" in a:
        tgt = a[a.index("--show") + 1]
        mod, seg, off = tgt.split(":")
        f = glob.glob(os.path.join(layout.generated_dir(), "**", mod + ".c"), recursive=True)[0]
        txt = open(f, encoding="utf-8", errors="replace").read()
        m = re.search(r"seg%s:0x%s.*?\n[A-Za-z_][\w ]*\**\w+\(([^)]*)\)\n\{(.*?)\n\}" % (seg, off), txt, re.S)
        args, body = m.group(1), m.group(2)
        sb = matchbatch.shipped_bytes(matchbatch._asm_for(mod, seg, off))
        cand = candidate(args, body, want_conv=_conv_from_shipped(sb))
        print("candidate:\n" + (cand or "(out of scope)"))
        if cand:
            print("shipped: %s\nmatch: %s" % (sb.hex(" "), _try_match(cc32, cand, sb)))
        return

    limit = int(a[a.index("--limit") + 1]) if "--limit" in a else 500
    from collections import Counter
    FLAGS = ("/c /O1 /Oy-", "/c /O1", "/c /Oxs", "/c /O2 /Oy-", "/c /O2", "/c /Ox /Oy-", "/c /Os")
    tried = skipped = 0
    dist = Counter()
    for mod, seg, off, args, body in _iter_inscope(200000):   # limit caps TRIED, below
        if tried >= limit:
            break
        ap = matchbatch._asm_for(mod, seg, off)
        sb = matchbatch.shipped_bytes(ap) if ap else None
        conv = _conv_from_shipped(sb) if sb else None
        if not sb or conv is None:
            skipped += 1
            continue
        cand = candidate(args, body, want_conv=conv)     # match the shipped cdecl/stdcall ret
        if cand is None:
            skipped += 1
            continue
        tried += 1
        best = 999
        samelen = False                                  # did ANY flag reproduce the shipped length?
        for fl in FLAGS:
            r = cc32.compile_c(cand, flags=fl)
            if "obj" not in r:
                continue
            c = next((v for k, v in cc32.Obj(r["obj"]).functions().items() if k.startswith("_f")), None)
            if not c or len(c[0]) != len(sb):
                continue
            samelen = True
            mask = set(c[1])
            d = sum(1 for i in range(len(sb)) if i not in mask and c[0][i] != sb[i])
            best = min(best, d)
        # Split the far bucket by REASON: same shipped length but bytes differ = a REGALLOC/encoding
        # miss (right shape, wrong register/opcode choice -> model MSVC 2.0's allocator); never the
        # right length = a STRUCTURAL miss (our C lowers to a different instruction count -> raiser
        # shape work). These need opposite fixes, so counting them together hid the real lever.
        key = ("exact" if best == 0 else "1-2 bytes" if best <= 2 else "3-6 bytes" if best <= 6
               else "7+ regalloc(same-len)" if samelen else "structural(difflen)")
        dist[key] += 1
        if tried % 50 == 0:
            print("  ...tried %d (exact %d)" % (tried, dist["exact"]), flush=True)
    print("\n" + "=" * 60)
    print("RAISER CLOSENESS  (%d in-scope call/conditional functions)" % tried)
    print("=" * 60)
    for k in ("exact", "1-2 bytes", "3-6 bytes", "7+ regalloc(same-len)", "structural(difflen)"):
        v = dist.get(k, 0)
        tag = ("  <- BYTE-EXACT now" if k == "exact" else "  <- permute closes" if k == "1-2 bytes"
               else "  <- match MSVC2.0 regalloc" if k.startswith("7+") else
               "  <- raiser shape work" if k.startswith("structural") else "")
        print("  %-22s %4d  (%.0f%%)%s" % (k, v, 100 * v / max(1, tried), tag))
    close = dist.get("exact", 0) + dist.get("1-2 bytes", 0)
    print("-" * 60)
    print("  within 2 bytes: %d (%.0f%%)   |   skipped %d (out of scope: regs/shape)"
          % (close, 100 * close / max(1, tried), skipped))
    print("=" * 60)


if __name__ == "__main__":
    main()
