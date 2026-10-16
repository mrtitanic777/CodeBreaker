"""draft.py -- functions written by an inverse model of the compiler, kept only when the compiler proves them.

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
"""
import importlib
import os
import re

from codebreaker.exact import carve, compare, library, omf

REGS = r"\b(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh|es)\b"


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


def fold_registers(text):
    """what a model leaves of the machine's registers, folded where the text alone shows how: a register given a
    value no later statement reads (a call's: the call; a load: nothing), a register returned right after it was
    given, a call the model wrote twice (once alone, once into a register)"""
    lines = text.split("\n")
    out = []
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)(ax|al|bx|cx|dx|si|di) = (.*);\s*$", l)
        if m:
            ind, reg, expr = m.groups()
            rest = "\n".join(lines[k + 1:])
            nxt = next((x for x in lines[k + 1:] if x.strip()), "")
            if re.fullmatch(r"\s*return %s;\s*" % reg, nxt):
                out.append("%sreturn %s;" % (ind, expr))
                lines[lines.index(nxt, k + 1)] = "\x00"
                continue
            used = re.search(r"\b%s\b" % reg, re.split(r"\n\s*%s = " % reg, rest)[0])
            if not used:
                if "(" in expr and out and out[-1].strip() == expr + ";":
                    continue                                # (the call written twice)
                if "(" in expr:
                    out.append("%s%s;" % (ind, expr))
                continue                                    # (a load nothing reads)
        if l != "\x00":
            out.append(l)
    return "\n".join(out)


def register_variables(text, regs=("si", "di")):
    """the registers a compiler keeps its register variables in (MS C 3.00: SI, then DI), made those variables:
    declared in that order, pointers where the text goes through them"""
    head, sep, body = text.partition("{")
    if not sep:
        return text
    used = [r for r in regs if re.search(r"\b%s\b" % r, body)]
    if not used:
        return text
    decls = []
    for r in used:
        ptr = re.search(r"\*\s*%s\b|\b%s\s*\[" % (r, r), body)
        decls.append("    register %s%s;" % ("char *" if ptr else "int ", "r_" + r))
        body = re.sub(r"\b%s\b" % r, "r_" + r, body)
    return head + "{\n" + "\n".join(decls) + body


def string_variants(prj, text):
    """the drafts where a number passed or given is the place of a string in the data: that string (the model
    reads a literal's address as the number it is); the compiler decides"""
    ds = prj.image[prj.dgroup:]
    nums = sorted(set(int(x) for x in re.findall(r"(?<![\w.])(\d{2,5})(?![\w.])", text.split("{", 1)[-1])))
    cands = []
    for v in nums:
        if 0 < v < len(ds) - 1 and 32 <= ds[v] < 127 and (v == 0 or ds[v - 1] == 0):
            e = ds.find(b"\0", v)
            s = ds[v:e]
            if 0 < len(s) < 200 and all(32 <= c < 127 or c in (9, 10, 13) for c in s):
                from codebreaker.exact.data import cstr
                cands.append((v, cstr(s)))
    out = []
    for v, lit in cands:                                    # (each alone, then all together)
        out.append(re.sub(r"(?<![\w.])%d(?![\w.])" % v, lambda m: lit, text))
    if len(cands) > 1:
        t = text
        for v, lit in cands:
            t = re.sub(r"(?<![\w.])%d(?![\w.])" % v, lambda m: lit, t)
        out.append(t)
    return out


def localize(prj, text, at, names):
    """the model's text with the engine's names and the declarations it needs; None if a machine register is left"""
    def call(m):
        rel = int(m.group(1), 16)
        if rel >= 0x80000000:
            rel -= 1 << 32
        t = (at + rel) & 0xFFFFF
        return (names.get(t) or "f%05x" % t) + "("
    text = re.sub(r"\bsub_(-?[0-9a-f]+)\(", call, text)
    text = re.sub(r"\bg_([0-9a-f]{1,4})\b", lambda m: "g_%04x" % int(m.group(1), 16), text)
    body = text.split("{", 1)[1] if "{" in text else text
    # registers the model left: a draft for a person, not for the compiler
    left = sorted(set(re.findall(REGS, re.sub(r'"(?:[^"\\]|\\.)*"', "", body))))
    data = sorted(set(re.findall(r"\bg_[0-9a-f]{4}\b", text)))
    head = text.split("(", 1)[0].strip()
    funcs = sorted(set(re.findall(r"\b([A-Za-z_]\w*)\(", body)) - {head, "if", "while", "for", "switch", "return", "sizeof"})
    decl = []
    if data:
        decl.append("extern int %s;" % ", ".join(data))
    if funcs:
        decl.append("extern %s;" % ", ".join("%s()" % f for f in funcs))
    return ("\n".join(decl) + "\n\n" if decl else "") + text, left


def _prove(prj, tc, src, a, ln):
    raw, con = tc.compile(src.encode("latin1"), headers=prj.headers(), include=prj.include, switches=prj.switches)
    if raw is None:
        last = [l for l in con.strip().splitlines() if "(the pass" not in l]
        return "does not compile: %s" % (last[-1][:100] if last else "")
    o = omf.read(raw)
    segs = omf.code_segments(o)
    fns = omf.functions(o, segs[0]) if segs else []
    if not fns:
        return "no function compiled"
    name, off, l2 = fns[0]
    bad, notes = compare.function(o, prj.image, name, off, l2, a, prj, dict(prj.symbols()), {}, segs[0])
    if compare.identical(bad, notes) and l2 == ln:
        return "identical"
    return "differs: %d bytes%s" % (len(bad), ", length %d not %d" % (l2, ln) if l2 != ln else "")


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
            fs, params, stmts, pt, loc = inv.recover(bytes(img[a:a + ln]))
            text = inv.to_c("f%05x" % a, fs, params, stmts, pt, loc)
        except Exception as e:                              # (the model declines: it says why)
            out[a] = "declined: %s" % str(e).splitlines()[0][:100] if str(e) else "declined"
            continue
        regs = tc.p.get("carve", {}).get("register_variables", ["si", "di"])
        src, left = localize(prj, register_variables(fold_registers(text), regs), a, names)
        verdict = None
        if left:
            verdict = "registers left: %s" % " ".join(left)
        else:
            verdict = _prove(prj, tc, src, a, ln)
            if verdict != "identical":
                for alt in string_variants(prj, src):       # (a number that is a string's place: the string)
                    v2 = _prove(prj, tc, alt, a, ln)
                    if v2 == "identical":
                        src, verdict = alt, v2
                        break
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
