#!/usr/bin/env python3
r"""recon16.py -- 16-bit (Win16 NE) reconstruction, the counterpart to recon.py on the
tracer16 oracle.

v1 reconstructs the PURE-LEAF tier: an exported function whose far-Pascal return (AX, or DX:AX)
is a function of its 16-bit args with no external calls and no side effects beyond its own
frame. It names the function and its module from the Win16 ordinal oracle (symdb), reads the
arity straight off the `retf N` (Pascal callees clean their args), samples the tracer16 oracle,
searches a constrained 16-bit expression grammar, validates every survivor against the binary,
and emits named C. Non-pure exports (the many Set* attribute wrappers that call an internal GDI
dispatcher and return the previous value) are declined honestly -- struct/effect-16 territory,
not faked.

    wt --target win95 recon16 <NE-module> [--limit N] [--ordinal K]
"""
import os
import sys

from codebreaker.emulate import trace16 as tracer16

M16 = 0xFFFF


def _md16():
    import capstone
    return capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)


def _arity_words(nef, seg, offset):
    """Read the Pascal arg size off the function's `retf N` (callee-clean). Returns word count,
    or None if the terminal isn't a clean retf N (caller-clean / unknown)."""
    segs = {s["idx"]: s for s in nef.segments()}
    s = segs.get(seg)
    if not s:
        return None
    body = nef.data[s["file_off"] + offset: s["file_off"] + offset + 256]
    for ins in _md16().disasm(body, offset):
        if ins.mnemonic == "retf":
            try:
                return (int(ins.op_str, 0) // 2) if ins.op_str else 0
            except ValueError:
                return 0
        if ins.mnemonic == "ret":
            try:
                return (int(ins.op_str, 0) // 2) if ins.op_str else 0
            except ValueError:
                return 0
    return None


def _template_delegation(nef, seg, offset, aw, fname):
    """Recognize the ubiquitous Win16 'validated delegation wrapper' idiom and reconstruct it
    faithfully from the disassembly (structure + constants are byte-derived, not guessed):

        push bp; mov bp,sp; [push <retf-thunk>]
        mov ax,[bp+k]              ; an arg
        cmp ax,LO; jl E; cmp ax,HI; jle K; E: mov bx,ERR; call VALIDATOR
        K: mov <reg>,[bp+j] ; mov dx,INDEX ; call HELPER ; ...epilogue; retf N

    i.e. `return HELPER(args.., INDEX)` with an optional range check. Emits C delegating to the
    named internal helpers (sub_<seg>_<off>), which are reconstructed separately. Returns a dict
    with emitted_c or None if the function doesn't fit the idiom."""
    segs = {s["idx"]: s for s in nef.segments()}
    s = segs.get(seg)
    if not s:
        return None
    body = nef.data[s["file_off"] + offset: s["file_off"] + offset + 128]
    ins = list(_md16().disasm(body, offset))
    if len(ins) < 5 or ins[0].mnemonic != "push" or ins[0].op_str != "bp":
        return None
    calls, idx_const, cmps, arg_slots = [], None, [], []
    last_imm = {}                                            # reg -> last imm moved into it
    for i in ins:
        if i.mnemonic == "mov":
            ops = [o.strip() for o in i.op_str.split(",")]
            if len(ops) == 2:
                if ops[1].startswith("0x") or ops[1].lstrip("-").isdigit():
                    try:
                        last_imm[ops[0]] = int(ops[1], 0)
                    except ValueError:
                        pass
                if "bp +" in ops[1]:                         # arg read [bp + k]
                    try:
                        arg_slots.append(int(ops[1].split("bp +")[1].strip(" ]"), 0))
                    except ValueError:
                        pass
        elif i.mnemonic == "cmp":
            ops = i.op_str.split(",")
            if len(ops) == 2 and ("0x" in ops[1] or ops[1].strip().lstrip("-").isdigit()):
                try:
                    cmps.append(int(ops[1].strip(), 0))
                except ValueError:
                    pass
        elif i.mnemonic in ("call",):
            try:
                calls.append(int(i.op_str, 0))
            except ValueError:
                calls.append(None)
            # the index constant is whatever was last loaded into dx/bx/cx at the delegating call
            idx_const = last_imm.get("dx", idx_const)
        elif i.mnemonic == "retf":
            break
    if not calls:
        return None
    helper = calls[-1]
    if helper is None:
        return None
    # Pascal frame: far-call pushes CS:IP (4) then `push bp` -> [bp+6] is the LAST-pushed arg =
    # the RIGHTMOST parameter; [bp+6+2*i] walks leftward. The true parameter COUNT is aw (from the
    # retf N), which is authoritative even when some args aren't individually read. Name params
    # left-to-right a0..a{aw-1}; a0 is at the highest slot.
    def slot_to_param(k):                                    # [bp+k] -> param name (a0 leftmost)
        j = (k - 6) // 2                                     # 0 = rightmost
        return "a%d" % (aw - 1 - j) if 0 <= aw - 1 - j < aw else "a_bp%d" % k
    params = ", ".join("WORD a%d" % j for j in range(aw)) or "void"
    lines = []
    if len(cmps) >= 2 and arg_slots:
        lo, hi = cmps[0], cmps[1]
        v = slot_to_param(min(k for k in arg_slots if k >= 6)) if any(k >= 6 for k in arg_slots) else "a0"
        lines.append("    if (%s < 0x%x || %s > 0x%x)" % (v, lo, v, hi))
        lines.append("        sub_%x(); /* param validator */" % calls[0] if len(calls) >= 2
                     else "        /* validate */;")
    used = sorted({k for k in arg_slots if k >= 6})
    callargs = ", ".join(slot_to_param(k) for k in used)
    if idx_const is not None:
        callargs = (callargs + ", " if callargs else "") + "0x%x /*index*/" % idx_const
    lines.append("    return sub_%x(%s);" % (helper, callargs))
    c = "WORD %s(%s)\n{\n%s\n}\n" % (fname, params, "\n".join(lines))
    return {"reconstructed": True, "method": "template-delegation",
            "expr": "delegate->sub_%x(index=0x%x)" % (helper, idx_const or 0),
            "emitted_c": c, "helper": helper, "index": idx_const,
            "validator": calls[0] if len(calls) >= 2 else None}


def export_names(nef):
    """{ordinal: NAME} of a module's OWN exports, from its resident + non-resident name tables."""
    out = {}
    try:
        for nm, o in list(nef.resident_names()) + list(nef.nonresident_names()):
            if o and nm:                                      # ordinal 0 = the module / description
                out.setdefault(o, nm)
    except Exception:
        pass
    return out


def _name(nef, ordinal):
    """(MODULE, name) for one of the module's own exports -- its authored name if the NE name table
    has it (the real function name, e.g. MAINWINDOWPROC), else the SDK ord16 oracle."""
    mod = (nef.module_name().upper() if nef else "?")
    try:
        nm = export_names(nef).get(ordinal)
        if nm:
            return mod, nm
        from codebreaker.symbols import symdb
        return mod, symdb._load().get("ord16", {}).get(mod, {}).get(ordinal)
    except Exception:
        return mod, None


def _oracle(path, ordinal, args):
    r = tracer16.trace_function16(path, ordinal=ordinal, args=args)
    if r.get("exit") != "ret":
        return None, r
    return r["ret"] & M16, r


# --- constrained 16-bit grammars --------------------------------------------
def _grammar1(outs):
    consts = sorted({0, 1, 2, 3, 4, 7, 8, 15, 16, 0xFF, 0x100, 0x7FFF, 0x8000, 0xFFFF} | set(outs))
    yield ("x", "return x;", lambda x: x)
    yield ("~x", "return (WORD)~x;", lambda x: (~x) & M16)
    yield ("-x", "return (WORD)-x;", lambda x: (-x) & M16)
    yield ("hi(x)", "return x >> 8;", lambda x: x >> 8)
    yield ("lo(x)", "return x & 0xFF;", lambda x: x & 0xFF)
    yield ("swab(x)", "return (x >> 8) | (x << 8);", lambda x: ((x >> 8) | (x << 8)) & M16)
    for c in consts:
        yield ("const 0x%x" % c, "return 0x%x;" % c, (lambda c: lambda x: c)(c))
        if c:
            yield ("x+0x%x" % c, "return x + 0x%x;" % c, (lambda c: lambda x: (x + c) & M16)(c))
            yield ("x-0x%x" % c, "return x - 0x%x;" % c, (lambda c: lambda x: (x - c) & M16)(c))
            yield ("x&0x%x" % c, "return x & 0x%x;" % c, (lambda c: lambda x: x & c)(c))
            yield ("x|0x%x" % c, "return x | 0x%x;" % c, (lambda c: lambda x: (x | c) & M16)(c))
            yield ("x^0x%x" % c, "return x ^ 0x%x;" % c, (lambda c: lambda x: (x ^ c) & M16)(c))
            yield ("x*0x%x" % c, "return x * 0x%x;" % c, (lambda c: lambda x: (x * c) & M16)(c))
            if c < 16:
                yield ("x<<%d" % c, "return x << %d;" % c, (lambda c: lambda x: (x << c) & M16)(c))
                yield ("x>>%d" % c, "return x >> %d;" % c, (lambda c: lambda x: x >> c)(c))


def _grammar2(outs):
    yield ("x+y", "return x + y;", lambda x, y: (x + y) & M16)
    yield ("x-y", "return x - y;", lambda x, y: (x - y) & M16)
    yield ("y-x", "return y - x;", lambda x, y: (y - x) & M16)
    yield ("x*y", "return x * y;", lambda x, y: (x * y) & M16)
    yield ("x&y", "return x & y;", lambda x, y: x & y)
    yield ("x|y", "return x | y;", lambda x, y: x | y)
    yield ("x^y", "return x ^ y;", lambda x, y: x ^ y)
    yield ("x|(y<<8)", "return x | (y << 8);", lambda x, y: (x | (y << 8)) & M16)
    yield ("y|(x<<8)", "return y | (x << 8);", lambda x, y: (y | (x << 8)) & M16)


def synthesize16(path, ordinal):
    import random
    from codebreaker.formats.ne import NEFile
    nef = NEFile(path)
    ents = nef.entries()
    e = ents[ordinal - 1] if 0 < ordinal <= len(ents) else None
    mod, nm = _name(nef, ordinal)
    res = {"module": os.path.basename(path), "ordinal": ordinal, "name": nm,
           "label": "%s.%s" % (mod, nm or ("@%d" % ordinal)), "reconstructed": False}
    if not e or e.get("kind") == "UNUSED":
        res["reason"] = "no live entry"; return res
    seg, off = e["seg"], e["offset"]
    aw = _arity_words(nef, seg, off)
    if aw is None:
        res["reason"] = "no clean retf N (caller-clean/unknown arity)"; return res
    res["arity"] = aw
    fname = nm or ("sub_%d" % ordinal)
    # candidacy: must return cleanly under the tracer16 oracle
    base, r0 = _oracle(path, ordinal, [0] * aw)
    if base is None:
        res["reason"] = "does not return cleanly (%s)" % r0.get("exit"); return res
    nonstack = [w for w in r0.get("mem_writes", []) if not (0x900000 <= w["addr"] < 0x910000)]
    # IMPURE (delegates to a helper / has side effects): try the static delegation-wrapper
    # template, which reconstructs the common Win16 'validate + delegate(index)' idiom faithfully
    # from the disassembly. Falls through to an honest decline if the idiom doesn't fit.
    if r0.get("imports_hit") or nonstack:
        t = _template_delegation(nef, seg, off, aw, fname)
        if t:
            res.update(t); return res
        # caller/orchestrator: reconstruct the (static) Win16 call sequence -- the 16-bit interproc.
        try:
            from codebreaker.recover import interproc16
            ip = interproc16.reconstruct(path, ordinal)
        except Exception:
            ip = {"reconstructed": False}
        if ip.get("reconstructed"):
            res.update(reconstructed=True, method="interproc16", expr="%d-call caller" % ip["ncalls"],
                       emitted_c=ip["emitted_c"], ncalls=ip["ncalls"], apis=ip.get("calls"))
            return res
        if r0.get("imports_hit"):
            res["reason"] = "calls imports %s; not a delegation-wrapper template" % \
                            sorted({h.get("api") for h in r0["imports_hit"]})[:3]
        else:
            res["reason"] = "has memory side effects; not a delegation-wrapper template"
        return res
    if aw == 0:
        # a nullary function returning a constant in our bare sandbox is almost always an
        # ENVIRONMENT artifact (it queried unmodeled state / a helper that failed), not a genuine
        # pure constant. Decline rather than fake -- a const claim here would be unfaithful.
        res["reason"] = "nullary const 0x%x in sandbox -- likely environment-dependent, declined" % base
        return res

    # sample the oracle over arg0 (and arg1 if arity>=2)
    seeds = [0, 1, 2, 3, 7, 8, 0xFF, 0x100, 0x1234, 0x7FFF, 0x8000, 0xABCD, 0xFFFF]
    if aw == 1:
        obs = {}
        for x in seeds:
            v, _ = _oracle(path, ordinal, [x])
            if v is not None:
                obs[x] = v
        outs = set(obs.values())
        surv = [(l, c, fn) for (l, c, fn) in _grammar1(outs) if all(fn(x) == obs[x] for x in obs)]
        rng = random.Random(ordinal)
        extra = {rng.randint(0, M16): None for _ in range(30)}
        for x in list(extra):
            extra[x], _ = _oracle(path, ordinal, [x])
        surv = [(l, c, fn) for (l, c, fn) in surv if all(extra[x] is None or fn(x) == extra[x] for x in extra)]
        surv.sort(key=lambda t: len(t[1]))
        params = "WORD x"
    else:
        ix, iy = 0, 1
        def two(x, y):
            a = [0] * aw; a[ix] = x; a[iy] = y; return a
        pairs = [(0, 0), (1, 0), (0, 1), (1, 1), (2, 3), (255, 1), (1, 255), (0x12, 0x34),
                 (0x1234, 0x5678), (0xFF00, 0x00FF), (0xABCD, 0x1234)]
        obs = {}
        for (x, y) in pairs:
            v, _ = _oracle(path, ordinal, two(x, y))
            if v is not None:
                obs[(x, y)] = v
        outs = set(obs.values())
        surv = [(l, c, fn) for (l, c, fn) in _grammar2(outs)
                if all(fn(x, y) == o for (x, y), o in obs.items())]
        rng = random.Random(ordinal)
        extra = {}
        for _ in range(30):
            x, y = rng.randint(0, M16), rng.randint(0, M16)
            v, _ = _oracle(path, ordinal, two(x, y))
            extra[(x, y)] = v
        surv = [(l, c, fn) for (l, c, fn) in surv
                if all(v is None or fn(x, y) == v for (x, y), v in extra.items())]
        surv.sort(key=lambda t: len(t[1]))
        params = "WORD x, WORD y"

    if len(set(obs.values())) <= 1:
        # output never changes across inputs -> not a genuine function of its args. In a bare
        # sandbox this is the signature of an environment-dependent function that degenerated to a
        # constant (e.g. a create/alloc that failed to 0), NOT a pure constant. Decline honestly.
        res["reason"] = "return constant (0x%x) across all sampled inputs -- environment-dependent, not a pure arg function; declined" % next(iter(obs.values()))
        return res
    if not surv:
        res["reason"] = "pure leaf but no grammar-16 expression reproduces it (tested %d inputs)" % len(obs)
        return res
    best = surv[0]
    fname = nm or ("sub_%d" % ordinal)
    res.update(reconstructed=True, expr=best[0],
               emitted_c="WORD %s(%s)\n{\n    %s\n}\n" % (fname, params, best[1]),
               candidates=[s[0] for s in surv[:4]], tested_inputs=len(obs))
    return res


def reconstruct_module(path, limit=None, ordinal=None):
    from codebreaker.formats.ne import NEFile
    nef = NEFile(path)
    segs = {s["idx"]: s for s in nef.segments()}
    ents = [e for e in nef.entries()
            if e.get("kind") in ("FIXED", "MOVEABLE") and segs.get(e["seg"], {}).get("kind") == "CODE"]
    if ordinal is not None:
        ents = [e for e in ents if e["ordinal"] == ordinal]
    elif limit:
        ents = ents[:limit]
    if ordinal is not None:                                   # single-export mode: leaf synth path
        out = []
        for e in ents:
            try:
                out.append(synthesize16(path, e["ordinal"]))
            except Exception as ex:
                out.append({"ordinal": e["ordinal"], "reconstructed": False, "reason": "ERR %r" % ex})
        return out
    # whole module: cover EVERY function (exports + internal). Try the static caller reconstruction
    # (interproc16) first -- most reliable -- then fall back to the leaf synth for exported ordinals.
    from codebreaker.recover import interproc16
    ord_of = {(e["seg"], e["offset"]): e["ordinal"] for e in ents}
    funcs = interproc16.all_functions(nef)
    if limit:
        funcs = funcs[:limit]
    out = []
    for seg, off in funcs:
        try:
            r = interproc16.reconstruct_at(path, seg, off, ordinal=ord_of.get((seg, off)), _nef=nef)
            if not r.get("reconstructed") and (seg, off) in ord_of:
                r = synthesize16(path, ord_of[(seg, off)])   # leaf/pure fallback for an export
        except Exception as ex:
            r = {"seg": seg, "offset": off, "reconstructed": False, "reason": "ERR %r" % ex}
        r.setdefault("seg", seg); r.setdefault("offset", off)
        r["exported"] = (seg, off) in ord_of
        out.append(r)
    return out


from codebreaker.base.args import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    from codebreaker.base import registry as _registry
    target = _registry.get("target")
    module = argv[0]
    path = target.resolve(module)
    limit = _opt(argv, "--limit"); limit = int(limit) if limit is not None else 40
    ordinal = _opt(argv, "--ordinal"); ordinal = int(ordinal) if ordinal is not None else None
    rs = reconstruct_module(path, limit=limit, ordinal=ordinal)
    rec = [r for r in rs if r.get("reconstructed")]
    print("=== recon16 %s : %d exports, %d reconstructed (pure-leaf tier) ===" % (module, len(rs), len(rec)))
    for r in rec:
        print("  %-22s [%s]  %s" % (r["label"], r.get("expr"), r.get("emitted_c", "").replace("\n", " ").strip()))
    # a compact reason tally for the rest
    import collections
    reasons = collections.Counter()
    for r in rs:
        if not r.get("reconstructed"):
            why = r.get("reason", "?")
            key = ("imports" if "imports" in why else "side effects" if "side effect" in why
                   else "no-clean-return" if "return cleanly" in why else "no-expr" if "grammar-16" in why
                   else "arity" if "arity" in why else "other")
            reasons[key] += 1
    print("  unreconstructed:", dict(reasons))


if __name__ == "__main__":
    main()
