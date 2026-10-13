"""repair.py -- recover the authored source by shrinking the byte distance to the original.

    python -m codebreaker repair <module> [--flags "/O2 /Gz"] [--json FILE]

The behavioural pipeline (verify/) proves a reconstruction MEANS the same as the original, but
many different sources mean the same thing, and the decompiler's C is only one of them -- and
not usually the one that was written. The byte distance to the original, when it is compiled
with a compiler of its time (verify/match.py), is the gradient that tells which of the many
behaviourally-equal sources is closest to what was written: a register variable rather than a
stack one, declarations in the order that lays the frame out the same way, an unused local the
author left in, the operands of a sum in the order they were typed.

This is a repair, not a fresh decompile. It starts from the behaviourally-verified candidate and
changes one thing at a time -- a KNOB -- keeping a change when it brings the bytes closer:

    pad       add an unused local the size of the frame the original reserves but the draft does not
    order     the order the local variables are declared in (the compiler lays the frame out in
              declaration order)
    swap      the operands of a commutative operator (a + b, a * b, a & b, ... and == / !=)

Every knob is a rewrite that cannot change what the function does -- an unused local, a
reordering of declarations, a commutative swap -- so a variant always still behaves like the
original: the search moves toward the authored bytes without ever risking a plausible-but-wrong
result, which was the old inverse engine's worst failure. The variants, their distances and the
knob each changed are kept in the store (functions.variant); the closest is the reconstruction
offered as the authored source.

Byte-exactness is not the goal and not the measure -- behaviour is. This is the signal that
chooses, among sources that all behave right, the one nearest to what a person wrote.
"""
import itertools
import os
import re

from codebreaker.verify import match

MAX_ORDER = 4            # permutations of this many leading locals are tried (n! grows fast)
MAX_SWAPS = 6            # commutative operators whose operands are tried both ways
MAX_WORK = 200           # compiles per function, a ceiling on the search


def _split(source):
    """(everything up to and including the opening brace, the declaration lines, the rest)."""
    head, brace, body = source.partition("{")
    if not brace:
        return source, [], ""
    lines = body.split("\n")
    decls, i = [], 0
    while i < len(lines):
        s = lines[i].strip()
        if s == "" or re.match(r"^[A-Za-z_][\w ]*\**\s*[A-Za-z_]\w*(\s*\[[^\]]*\])?\s*;$", s):
            if s:
                decls.append(lines[i])
            i += 1
        else:
            break
    return head + "{", decls, "\n".join(lines[i:])


def _frame(code):
    """The bytes a function reserves for locals: the N of `sub esp, N` just after the prologue,
    0 when there is none. `code` is the function's bytes."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    seen_push = False
    for _a, _n, mnem, ops in md.disasm_lite(bytes(code[:24]), 0):
        if mnem == "push" and ops == "ebp":
            seen_push = True
        elif mnem == "mov" and ops.replace(" ", "") == "ebp,esp":
            continue
        elif mnem == "sub" and ops.startswith("esp,"):
            try:
                return int(ops.split(",")[1].strip(), 0)
            except ValueError:
                return 0
        elif seen_push:
            return 0
    return 0


_SWAP = re.compile(r"([A-Za-z_]\w*|\b\d+\b)\s*([-+*&|^]|==|!=)\s*([A-Za-z_]\w*|\b\d+\b)")
_COMMUTATIVE = {"+", "*", "&", "|", "^", "==", "!="}


def _knobs(source, target_frame, draft_frame):
    """Yield (name of the knob, rewritten source). Each rewrite preserves what the function does."""
    head, decls, body = _split(source)
    # pad: the original reserves more stack than the draft -- an unused local of the difference
    if target_frame and draft_frame is not None and target_frame > draft_frame:
        gap = target_frame - draft_frame
        pad = "  char codebreaker_unused[%d];" % gap
        yield "pad%d" % gap, head + "\n" + pad + ("\n" + "\n".join(decls) if decls else "") + "\n" + body
    # order: permutations of the leading locals (the frame is laid out in declaration order)
    if 2 <= len(decls) <= MAX_ORDER + 4:
        lead = decls[:MAX_ORDER]
        for i, perm in enumerate(itertools.permutations(range(len(lead)))):
            if perm == tuple(range(len(lead))):
                continue
            reordered = [lead[k] for k in perm] + decls[len(lead):]
            yield "order%d" % i, head + "\n" + "\n".join(reordered) + "\n" + body
    # swap: a commutative operator's operands the other way round
    sites = [m for m in _SWAP.finditer(body) if m.group(2) in _COMMUTATIVE][:MAX_SWAPS]
    for j, m in enumerate(sites):
        swapped = body[:m.start()] + "%s %s %s" % (m.group(3), m.group(2), m.group(1)) + body[m.end():]
        yield "swap%d" % j, head + ("\n" + "\n".join(decls) if decls else "") + "\n" + swapped


def repair_function(cl, env, header, source, name, original, work, flags):
    """Search behaviour-preserving rewrites of one function's source for the bytes closest to the
    original's. -> {source, verdict, distance, total, knobs, tried} for the best found, or None
    when the starting source does not compile."""
    base = match.compile_function(cl, env, header, source, name, work, flags)
    if base is None:
        return None
    verdict, dist, total, _first = match.distance(base, original)
    best = {"source": source, "verdict": verdict, "distance": dist, "total": total, "knobs": "", "tried": 1}
    target_frame = _frame(original[0])
    improved, tried = True, 1
    while improved and dist and tried < MAX_WORK:
        improved = False
        cur = match.compile_function(cl, env, header, best["source"], name, work, flags)
        draft_frame = _frame(cur[0]) if cur else None
        for knob, variant in _knobs(best["source"], target_frame, draft_frame):
            if tried >= MAX_WORK:
                break
            tried += 1
            built = match.compile_function(cl, env, header, variant, name, work, flags)
            if built is None:
                continue
            v, d, t, _f = match.distance(built, original)
            if d < dist:
                dist, best = d, {"source": variant, "verdict": v, "distance": d, "total": t,
                                 "knobs": (best["knobs"] + "," + knob).strip(","), "tried": tried}
                improved = True
                break                # greedy: take the first improvement and search again from it
    best["tried"] = tried
    return best


def run(module_path, flags, work, fs=None, log=print):
    """Repair every function of a module and record the variants. -> (functions, identical, same
    size, different)."""
    from codebreaker.store import functions as F
    from codebreaker.verify import win32
    cl = match.compiler()
    if not cl:
        raise SystemExit("no period compiler (settings.json: period_compiler)")
    from codebreaker.base import corpus
    sha = corpus.sha(module_path) or ""
    fs = fs or F.open()
    env = match.compiler_env(cl)
    m = win32.prepare(module_path, os.path.join(work, "source"))
    address = {name: va for va, name in m.by_va.items()}
    originals = match.original_functions(module_path, sorted(address.values()))
    behaves = {r["addr"]: r.get("verdict") for r in fs.candidates(sha, win32.settings_profile("pe")[0])}
    fs.add_module(sha, os.path.basename(module_path).lower(), module_path, "pe")
    fs.add_functions(sha, [win32.LANES["pe"].key(a) for a in address.values()])
    counts = [0, 0, 0, 0]
    for name in m.order:
        va = address[name]
        orig = originals.get(va)
        if orig is None:
            continue
        r = repair_function(cl, env, m.header, m.texts[name], name, orig, os.path.join(work, "one"), flags)
        if r is None:
            continue
        key = win32.LANES["pe"].key(va)
        fs.record_variant(sha, key, os.path.basename(cl).lower() + " " + flags, r["source"], r["distance"],
                          r["total"], r["verdict"], knobs=r["knobs"], behaves=behaves.get(key) or "")
        counts[0] += 1
        counts[1 if r["verdict"] == "identical" else 2 if r["verdict"] == "same size" else 3] += 1
        if log and r["knobs"]:
            log("  %-28s %-9s distance %d/%d  via %s" % (name, r["verdict"], r["distance"], r["total"], r["knobs"]))
    return tuple(counts)


def main(argv):
    """python -m codebreaker repair <module> [--flags "/O2 /Gz"] [--json FILE]

    Move each function's reconstruction toward the authored bytes with behaviour-preserving
    rewrites, compiled with a period compiler. Records the variants in the store."""
    from codebreaker.base import corpus, settings
    args = [a for a in argv if not a.startswith("--")]
    flags = argv[argv.index("--flags") + 1] if "--flags" in argv else match.FLAG_SETS[0]
    args = [a for a in args if a != flags]
    if not args:
        print(main.__doc__)
        return 2
    path = args[0] if os.path.isfile(args[0]) else corpus.retail(args[0])
    if not path or corpus.lane(path) != "pe":
        raise SystemExit("%s: a 32-bit Windows module is needed" % args[0])
    work = settings.state("repair", os.path.basename(path).lower())
    n, ident, same, diff = run(path, flags, work, log=print)
    print("%s: %d functions repaired toward the original's bytes: %d identical, %d same size, %d different" % (
        os.path.basename(path), n, ident, same, diff))
    return 0
