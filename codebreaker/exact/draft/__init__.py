"""draft -- functions written by an inverse model of the compiler, kept only when the compiler proves them.

    python -m codebreaker exact PROJECT draft [--all] [--limit N] [-v]

For every function carve finds that has no src/fADDR.c (--all: every function, the written ones
left as they are), the toolchain profile's inverse model ("inverse": a module of
codebreaker.invert with recover(code) and to_c(...)) reads the function's bytes back into C. The
text is made a source file of the project's kind:

  - the places it names get the engine's names: data g_XXXX (DS:XXXX), functions fXXXXX (their
    image offset), the library's routines by their names (the library stage's), each declared
  - what the model leaves of the machine (a register it could not fold into an expression) makes
    the draft unfit: it is not compiled

and compiled by the project's toolchain and held to the program (exact/compare.py). Identical:
written as src/fADDR.c. Otherwise it is kept in src/drafts/fADDR.c with what was wrong, for a
person or a later model - the ledger never counts a draft.

    draft.run(project) -> {address: verdict}

The package, each module using only those above it:

    text.py        reading a draft's text: an if's parts, a block's ends, a test negated, a register read later
    shapes.py      control flow read back into statements: returns, loops and their tests, switches, gotos
    forms.py       values and places read back: registers folded, steps and stores joined, pointers, macros
    candidates.py  the model's text made the project's source, and every rewrite applied to it
    prove.py       a candidate compiled and held to the program; the beam search over the rewrites
    __init__.py    the model's texts of a function, and the stage itself (run, main)
"""
import importlib
import os
import re
import struct

from codebreaker.exact import carve, library
from codebreaker.exact.draft.text import LONGINFO
from codebreaker.exact.draft.candidates import FPINFO, FRAME
from codebreaker.exact.draft.prove import prove_texts


def _names(prj):
    """{image offset: name} of the code the program has names for (SYMBOLS, the library's modules)"""
    out = {}
    mp = library.locate(prj)
    for p in mp.coded + mp.lone:
        for m in [p.m] + p.also:
            for n, (s, off) in m.pubs.items():
                if s == m.code[0][0]:
                    out.setdefault(p.code + off, n.lstrip("_"))
    for n, (t, a) in prj.symbols().items():
        if t == "C" and ":" not in n:
            out.setdefault(a, n.lstrip("_"))
    return out


def table_hook(code):
    """SWITCH(address) for the model: the places a near jump through a table goes (`jmp cs:[bx + T]`, after
    `cmp R, N`: N + 1 words at T) - addresses in the function's own code, from 0. T is the program's offset of
    the table, so the function's own offset is found where its code begins being the program's"""
    from codebreaker.exact import x86
    ins = list(x86.disasm(code, 0, len(code), False))

    def hook(addr):
        k = next((j for j, i in enumerate(ins) if i.address == addr), None)
        if k is None:
            return None
        m = re.search(r"cs:\[bx ([+-]) (0x[0-9a-f]+)\]", ins[k].op_str)
        n = next((int(x.group(1), 0) for i in reversed(ins[max(0, k - 8):k])
                  for x in [re.fullmatch(r"\w+, (0x[0-9a-f]+|\d+)", i.op_str)] if i.mnemonic == "cmp" and x), None)
        if not m or n is None:
            return None
        t = ((int(m.group(2), 16) if m.group(1) == "+" else -int(m.group(2), 16)) & 0xFFFF) - hook.base   # (above 32K:
                                                                                                         #  printed less)
        if not (0 <= t and t + 2 * (n + 1) <= len(code)):
            return None
        return [struct.unpack_from("<H", code, t + 2 * j)[0] - hook.base for j in range(n + 1)]
    hook.base = 0
    return hook


def fp_native(code):
    """the code with the floating-point emulator's calls (int 34h..3Bh: an 8087 instruction, its first byte
    D8h + n; int 3Ch: one with a segment; int 3Dh: fwait) written as the 8087's own - each as long as it was
    (a wait before it), so the model reads them as the instructions they stand for"""
    from codebreaker.exact import x86
    b = bytearray(code)
    if not re.search(rb"\xcd[\x34-\x3d]", bytes(code)):
        return bytes(code)
    for i in x86.disasm(bytes(code), 0, len(code), True):
        raw = bytes(i.bytes)
        if raw[:1] != b"\xcd" or len(raw) < 2 or not 0x34 <= raw[1] <= 0x3D:
            continue
        a = i.address
        if raw[1] <= 0x3B:
            b[a:a + 2] = bytes([0x9B, 0xD8 + raw[1] - 0x34])
        elif raw[1] == 0x3D:
            b[a:a + 2] = b"\x9b\x90"
        elif len(raw) >= 3:
            seg = {0x00: 0x26, 0x40: 0x2E, 0x80: 0x36, 0xC0: 0x3E}[raw[2] & 0xC0]
            b[a:a + 3] = bytes([0x9B, seg, 0xD8 | (raw[2] & 7)])
    return bytes(b)


def inline_goto_arms(lines):
    """an arm that is only `goto L;`, L the start of statements to the end of their block: those statements in
    its place (the compiler's one copy of a tail two arms share); L dropped when nothing goes to it then"""
    lines = list(lines)
    for _ in range(8):
        hit = None
        for k, l in enumerate(lines):
            g = re.fullmatch(r"(\s*)goto (L_\w+);", l)
            if not g or k == 0 or not re.search(r"\{\s*$", lines[k - 1]) or k + 1 >= len(lines) or not lines[k + 1].strip().startswith("}"):
                continue
            at = next((j for j, x in enumerate(lines) if x.strip() == g.group(2) + ":"), None)
            if at is None:
                continue
            ind = len(lines[at]) - len(lines[at].lstrip())
            j, depth = at + 1, 0
            while j < len(lines):                           # (to the end of the label's block)
                if depth == 0 and lines[j].strip().startswith("}"):
                    break
                depth += lines[j].count("{") - lines[j].count("}")
                j += 1
            tail = lines[at + 1:j]
            if not tail or len(tail) > 40 or any(re.fullmatch(r"\s*L_\w+:", x) for x in tail) or at < k < j:
                continue
            hit = (k, g.group(1), [g.group(1) + x[ind:] if x.strip() else x for x in tail], g.group(2), at)
            break
        if not hit:
            break
        k, gi, body, lab, at = hit
        lines[k:k + 1] = body
        if not any(re.search(r"\bgoto %s;" % lab, x) for x in lines):
            lines = [x for x in lines if x.strip() != lab + ":"]
    return lines


def model_texts(inv, code, name, base=None):
    """[(text, register parameters)] - the model's text of a function, from each of its structurers (a linear walk;
    a structured control-flow graph, where the model has one): the compiler decides between them"""
    raw, code = code, fp_native(code)
    if hasattr(inv, "FRAME_AFTER_BP"):                      # (a draft is of 3.00 or later code: its -Gs frames,
        inv.FRAME_AFTER_BP = True                           #  its register variables)
    if hasattr(inv, "REGVARS_EAGER"):
        inv.REGVARS_EAGER = True
    out = []
    try:
        fs, params, stmts, pt, loc = inv.recover(code)
    except Exception:
        if raw == code:
            raise
        fs, params, stmts, pt, loc = inv.recover(raw)       # (the linear walk lost in the 8087's code: as it was)
    FRAME[0] = fs                                           # (the frame's size: fill_frame)
    regparam = dict(getattr(inv, "LAST", {}).get("regparam") or {})
    out.append((inv.to_c(name, fs, params, stmts, pt, loc), regparam))
    if hasattr(inv, "recover_cfg"):
        from codebreaker.invert import flow16
        flow16.CMPSWITCH = True                             # (switches by compares read as switches)
        old = getattr(inv, "SWITCH", None)
        if base is not None and hasattr(inv, "SWITCH"):     # (and by a table: where it sends each value)
            hook = table_hook(code)
            hook.base = base
            inv.SWITCH = hook
        try:
            fs, params, stmts, pt, loc = inv.recover_cfg(code)
        except Exception:
            _fpinfo(inv, raw, code)
            return out
        finally:
            flow16.CMPSWITCH = False
            if hasattr(inv, "SWITCH"):
                inv.SWITCH = old
        stmts = [s for s in stmts if not s.startswith("/* register variables")]
        for reg, ix in regparam.items():                    # (a register parameter's load is not a statement)
            p = chr(ord("a") + ix)
            stmts = [s for k, s in enumerate(stmts) if not (k < 3 and re.fullmatch(r"%s = %s;" % (reg, p), s.strip()))]
            if p not in params:
                params = sorted(set(params) | {p})
        t = inv.to_c(name, fs, params, stmts, pt, loc)
        if t != out[0][0]:
            out.append((t, regparam))
        if hasattr(inv, "FPCASTS") and any(re.search(r"->|\[", k) for k in (getattr(inv, "LAST", {}).get("fptypes") or {})):
            inv.FPCASTS = True                              # (the 8087's fields at their own widths: read again with
            flow16.CMPSWITCH = True                         #  casts - a union's place used both ways)
            try:
                fs2, params2, stmts2, pt2, loc2 = inv.recover_cfg(code)
                stmts2 = [s for s in stmts2 if not s.startswith("/* register variables")]
                for reg, ix in regparam.items():
                    p = chr(ord("a") + ix)
                    stmts2 = [s for k, s in enumerate(stmts2) if not (k < 3 and re.fullmatch(r"%s = %s;" % (reg, p), s.strip()))]
                    if p not in params2:
                        params2 = sorted(set(params2) | {p})
                t2 = inv.to_c(name, fs2, params2, stmts2, pt2, loc2)
                if all(t2 != x for x, _ in out):
                    out.append((t2, regparam))
            except Exception:
                pass
            finally:
                inv.FPCASTS = False
                flow16.CMPSWITCH = False
        # (a long compared the compiler's way - the high words, then the low words without sign - three nested
        #  ifs: read again with the unsigned compares marked, and the ladders folded into one long comparison)
        from codebreaker.exact import x86 as _x86          # (or a compare without sign: the text with it marked)
        _ins = list(_x86.disasm(bytes(code), 0, len(code), False))
        unsigned = any(a_.mnemonic == "cmp" and b_.mnemonic in ("jb", "jbe", "ja", "jae") and "bp" not in a_.op_str.split(",")[1]
                       for a_, b_ in zip(_ins, _ins[1:]))
        if hasattr(inv, "_fold_ladders") and (unsigned or re.search(r"if \(\((\w+) [<>]= [^)]*\)\) \{\n\s*if \(\(\1 [<>]= ", t)):
            olds = (inv.UNSIGNED_CAST, getattr(inv, "SWITCH", None))
            try:
                inv.UNSIGNED_CAST = True
                flow16.CMPSWITCH = True
                fs2, params2, stmts2, pt2, loc2 = inv.recover_cfg(code)
                stmts2 = [s for s in inv._fold_ladders(inline_goto_arms(stmts2)) if not s.startswith("/* register variables")]
                for reg, ix in regparam.items():
                    p = chr(ord("a") + ix)
                    stmts2 = [s for k, s in enumerate(stmts2) if not (k < 3 and re.fullmatch(r"%s = %s;" % (reg, p), s.strip()))]
                t2 = inv.to_c(name, fs2, params, stmts2, pt2, loc2)
                if t2 != t:
                    out.append((t2, regparam))
            except Exception:
                pass
            finally:
                inv.UNSIGNED_CAST = olds[0]
                flow16.CMPSWITCH = False
    _fpinfo(inv, raw, code)
    for reg, ix in regparam.items():                        # (a register parameter's register given the parameter:
        p = chr(ord("a") + ix)                              #  itself - the model's reload at a join, no statement)
        out = [(re.sub(r"(?m)^[ \t]*%s = %s;\n" % (reg, p), "", t), rp) for t, rp in out]
    return out


def _fpinfo(inv, raw, code):
    last = getattr(inv, "LAST", {})                         # (what the model read of the 8087's values: localize)
    FPINFO.clear()
    FPINFO.update(types=dict(last.get("fptypes") or {}), ret=last.get("fpret"),
                  fp=raw != code or bool(re.search(rb"\x9b[\xd8-\xdf]", bytes(raw))))
    LONGINFO.clear()                                        # (the longs: shifted without sign; a value widened last)
    LONGINFO["unsigned"] = set(last.get("unsigned_longs") or ())
    from codebreaker.exact import x86
    ins = list(x86.disasm(bytes(code), 0, len(code), False))
    j = len(ins) - 1
    if ins and ins[j].mnemonic in ("ret", "retf"):
        j -= 1
        while j >= 0 and (ins[j].mnemonic == "pop" or ins[j].op_str == "sp, bp"):
            j -= 1
        epi = ins[j + 1].address if j + 1 < len(ins) else None
        gives_dx = lambda x: x.mnemonic in ("cwd", "cdq") or (x.mnemonic not in ("cmp", "test", "push") and
                                                            (x.op_str.startswith("dx, ") or x.op_str == "dx"))   # (any write of dx)
        ends = [j] + [k - 1 for k, x in enumerate(ins) if x.mnemonic == "jmp" and x.op_str in ("0x%x" % epi, str(epi)) and k > 0] if epi is not None else [j]
        near = lambda e: any(gives_dx(ins[q]) for q in range(max(0, e - 2), e + 1)) if e >= 0 else False
        LONGINFO["ret"] = j >= 0 and all(near(e) for e in ends)   # (dx given before every way out - just before it,


def run(prj, every=False, limit=None, verbose=False):
    tc = prj.toolchain()
    model = tc.p.get("inverse")
    if not model:
        raise SystemExit("the toolchain %s names no inverse model (its profile's \"inverse\")" % tc.name)
    inv = importlib.import_module("codebreaker.invert." + model)
    names = _names(prj)
    img = prj.image
    have = {a for a, p in prj.sources()}
    out = {}
    os.makedirs(prj.path("drafts"), exist_ok=True)
    todo = [(a, ln) for a, ln, w in carve.functions(prj) if every or a not in have]
    for a, ln in todo[:limit]:
        if a in have:
            continue
        try:
            texts = model_texts(inv, bytes(img[a:a + ln]), "f%05x" % a, a)
        except Exception as e:                              # (the model declines: it says why)
            out[a] = "declined: %s" % str(e).splitlines()[0][:100] if str(e) else "declined"
            continue
        try:
            src, verdict = prove_texts(prj, tc, inv, texts, a, ln, names)
        except Exception as e:                              # (a pass that fails on one function: noted, not fatal)
            out[a] = "failed: %s: %s" % (type(e).__name__, str(e)[:100])
            continue
        out[a] = verdict
        if verdict == "identical":
            with open(prj.path("f%05x.c" % a), "w", newline="\n") as f:
                f.write("/* drafted by codebreaker exact draft (%s), identical */\n%s" % (model, src))
        else:
            with open(prj.path(os.path.join("drafts", "f%05x.c" % a)), "w", newline="\n") as f:
                f.write("/* draft (%s): %s */\n%s" % (model, verdict, src))
        if verbose:
            print("%05x %5d  %s" % (a, ln, verdict))
    return out


def main(prj, argv):
    lim = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else None
    res = run(prj, "--all" in argv, lim, "-v" in argv)
    kinds = {}
    for v in res.values():
        k = v.split(":")[0]
        kinds[k] = kinds.get(k, 0) + 1
    print("%d functions drafted: %s" % (len(res), ", ".join("%d %s" % (n, k) for k, n in sorted(kinds.items(), key=lambda kv: -kv[1]))))
    return 0
