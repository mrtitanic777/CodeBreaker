"""repair.py -- recover the authored source by shrinking the byte distance to the original.

    python -m codebreaker repair <module> [--flags "/O2 /Gz"] [--json FILE]

The behavioural pipeline (verify/) proves a reconstruction MEANS the same as the original, but
many different sources mean the same thing, and the decompiler's C is only one of them -- and
not usually the one that was written. The byte distance to the original, when it is compiled
with a compiler of its time (verify/authored/match.py), is the gradient that tells which of the many
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

from codebreaker.base import ctext
from codebreaker.verify.authored import match
from codebreaker import platforms

MAX_LOCALS = 6           # the leading locals whose order and storage are tried
MAX_SITES = 6            # places in the body each kind of rewrite is tried at
MAX_WORK = 300           # compiles per function, a ceiling on the search
BEAM = 3                 # the sources the search goes on from, each round
PATIENCE = 2             # rounds without a better source before the search stops

_ID = r"(?:[A-Za-z_]\w*|0[xX][0-9a-fA-F]+|\d+)"
_NAME = r"[A-Za-z_]\w*"
# an expression that is `a op b` and nothing more: it begins after ( = , ; { or `return`, and
# ends at ) ; or , -- so no neighbour's precedence can take an operand away
_PAIR = re.compile(r"(?P<pre>(?:[(,;{]|(?<![=!<>])=)\s*|\breturn\s+)(?P<a>" + _ID + r")\s*(?P<op>==|!=|<=|>=|[-+*&|^<>])\s*(?P<b>"
                   + _ID + r")(?P<post>\s*[);,])")
_COMMUTATIVE = {"+": "+", "*": "*", "&": "&", "|": "|", "^": "^", "==": "==", "!=": "!=",
                "<": ">", ">": "<", "<=": ">=", ">=": "<="}
# a statement that only steps a variable: x++;  ++x;  x += 1;  x = x + 1;
_STEP = re.compile(r"(?P<pre>[;{}]\s*|\belse\s+)(?:(?P<x1>" + _NAME + r")\s*(?P<o1>\+\+|--)|(?P<o2>\+\+|--)\s*(?P<x2>"
                   + _NAME + r")|(?P<x3>" + _NAME + r")\s*(?P<o3>[-+])=\s*1)(?P<post>\s*[;)])")
# x = x op y;   and   x op= y;
_LONG = re.compile(r"(?P<pre>[;{}]\s*|\belse\s+)(?P<x>" + _NAME + r")\s*=\s*(?P=x)\s*(?P<op>[-+*&|^])\s*(?P<y>" + _ID
                   + r")(?P<post>\s*[;)])")
_SHORT = re.compile(r"(?P<pre>[;{}]\s*|\belse\s+)(?P<x>" + _NAME + r")\s*(?P<op>[-+*&|^])=\s*(?P<y>" + _ID
                    + r")(?P<post>\s*[;)])")
_DECL = re.compile(r"^[A-Za-z_][\w ]*\**\s*[A-Za-z_]\w*(\s*\[[^\]]*\])?\s*;$")


def _split(source):
    """(everything up to and including the opening brace, the declaration lines, the rest)."""
    head, brace, body = source.partition("{")
    if not brace:
        return source, [], ""
    lines = body.split("\n")
    decls, i = [], 0
    while i < len(lines):
        s = lines[i].strip()
        if s == "" or _DECL.match(s):
            if s:
                decls.append(lines[i])
            i += 1
        else:
            break
    return head + "{", decls, "\n".join(lines[i:])


def _frame(code, bits=32):
    """The bytes a function reserves for locals: the N of `sub esp, N` (`sub sp, N`) in its
    prologue (PowerPC: of `stwu r1,-N(r1)`), 0 when there is none. `code` is the function's bytes."""
    if bits == "ppc":
        for k in range(0, min(len(code), 48) - 3, 4):
            w = int.from_bytes(code[k:k + 4], "big")
            if w >> 16 == 0x9421:                                  # stwu r1,-N(r1)
                return (-(w & 0xFFFF)) & 0xFFFF
        return 0
    md = platforms.decoder(32 if bits == 32 else 16)
    sp = "esp," if bits == 32 else "sp,"
    for k, (_a, _n, mnem, ops) in enumerate(md.disasm_lite(bytes(code[:32]), 0)):
        if mnem == "sub" and ops.startswith(sp):
            try:
                return int(ops.split(",")[1].strip(), 0)
            except ValueError:
                return 0
        if k >= 6 or mnem not in ("push", "mov"):
            break
    return 0


def instructions(built, bits):
    """A function's code as its instructions, each the bytes of one with the linker's bytes
    blanked -- what two versions of a function are lined up by. (PowerPC: each word, with the
    bits the linker fills in cleared.)"""
    code, mask = bytes(built[0]), built[1]
    if bits == "ppc":
        return [(int.from_bytes(code[i:i + 4], "big") & ~mask[i // 4]) & 0xFFFFFFFF
                for i in range(0, len(code) - 3, 4)]
    md = platforms.decoder(32 if bits == 32 else 16)
    out, at = [], 0
    while at < len(code):
        n = 0
        for a, size, _m, _o in md.disasm_lite(code[at:], at):
            out.append(bytes(0 if mask[i] else code[i] for i in range(a, a + size)))
            n = a + size
        if n <= at:                              # not an instruction: one byte, and on
            out.append(code[at:at + 1])
            n = at + 1
        at = n
    return out


def score(built, original, bits=0, target=None):
    """How far a compiled function is from the original, smaller the nearer: (the instructions
    that have no counterpart, the bytes that differ). Bytes alone mislead: one instruction more
    near the top moves every later byte, and the function looks wholly wrong when it is nearly
    right. Lined up by instruction, it is one instruction away."""
    if bits == "ppc":
        v, d, t, _f = match.distance(built, original, "powerpc")
    else:
        v, d, t, _f = match.distance(built, original)
    if not d or not bits:
        return (0 if not d else d, d), (v, d, t)
    import difflib
    mine = instructions(built, bits)
    theirs = target if target is not None else instructions(original, bits)
    same = sum(b.size for b in difflib.SequenceMatcher(None, mine, theirs, autojunk=False).get_matching_blocks())
    return (max(len(mine), len(theirs)) - same + 1, d), (v, d, t)


def _blank(text):
    """The text with its strings, characters and comments blanked, the same length: what the
    rewrites look for their places in, so that none is found inside one."""
    return ctext.blank(text, whole=True)


def _depth(shadow, at):
    return shadow.count("(", 0, at) - shadow.count(")", 0, at)


_FLIP = {"<": ">", ">": "<", "<=": ">=", ">=": "<=", "==": "==", "!=": "!="}


def _tests(shadow, body):
    """(start, end, rewritten) for each `if (A op B)` / `while (A op B)` whose whole condition is
    one comparison of two things that do nothing but have a value: (B flipped-op A)."""
    for m in re.finditer(r"\b(?:if|while)\s*\(", shadow):
        i, depth = m.end(), 1
        j = i
        while j < len(shadow) and depth:
            depth += (shadow[j] == "(") - (shadow[j] == ")")
            j += 1
        if depth:
            continue
        inner, ops, d, k = shadow[i:j - 1], [], 0, 0
        plain = True
        while k < len(inner):
            c, two = inner[k], inner[k:k + 2]
            if c in "([":
                d += 1
            elif c in ")]":
                d -= 1
            elif two in ("++", "--", "+=", "-=", "*=", "/=", "|=", "&=", "^=", "%="):
                plain = False
            elif d == 0 and (two in ("&&", "||") or c in "?,"):
                plain = False
            elif two in ("<<", ">>", "->"):
                k += 1
            elif two in ("==", "!=", "<=", ">="):
                if d == 0:
                    ops.append((k, two))
                k += 1
            elif c in "<>":
                if d == 0:
                    ops.append((k, c))
            elif c == "=":
                plain = False
            elif d == 0 and c in "&|^":
                plain = False                    # binds looser than a comparison
            k += 1
        if not plain or len(ops) != 1 or re.search(r"[A-Za-z_]\w*\s*\(", inner):
            continue
        at, op = ops[0]
        left, right = body[i:i + at].strip(), body[i + at + len(op):j - 1].strip()
        if left and right and left != right:
            yield i, j - 1, "%s %s %s" % (right, _FLIP[op], left)


def _toggle_register(line):
    s = line.strip()
    if "[" in s or s.startswith(("static ", "extern ")) or not _DECL.match(s):
        return None
    indent = line[:len(line) - len(line.lstrip())]
    other = s[len("register "):] if s.startswith("register ") else "register " + s
    if other.split()[-1] in ("register", "int;") and len(other.split()) == 1:
        return None
    return indent + other


def _knobs(source, target_frame, draft_frame):
    """Yield (name of the knob, rewritten source). Each rewrite preserves what the function does."""
    head, decls, body = _split(source)
    top = head + ("\n" + "\n".join(decls) if decls else "") + "\n"
    # frame: the draft keeps the original's locals in one block with spare room at its end; the
    # room is made what the original's frame says it was
    spare = re.search(r"uint1 _rest\[(\d+)\]; \} _f;", source)
    if spare and target_frame and draft_frame is not None and draft_frame != target_frame:
        room = int(spare.group(1)) - (draft_frame - target_frame)
        if room >= 0:
            new = ("uint1 _rest[%d]; } _f;" % room) if room else "} _f;"
            yield "frame%d" % room, source[:spare.start()] + new + source[spare.end():]
    # pad: the original reserves more stack than the draft -- an unused local of the difference
    if target_frame and draft_frame is not None and target_frame > draft_frame:
        gap = target_frame - draft_frame
        pad = "  char codebreaker_unused[%d];" % gap
        yield "pad%d" % gap, head + "\n" + pad + ("\n" + "\n".join(decls) if decls else "") + "\n" + body
    # order: two of the leading locals change places (the frame is laid out in declaration order;
    # any order is some changes of two, and the search goes on from each)
    lead = min(len(decls), MAX_LOCALS)
    for n, (i, j) in enumerate(itertools.combinations(range(lead), 2)):
        moved = list(decls)
        moved[i], moved[j] = moved[j], moved[i]
        if moved != decls:
            yield "order%d" % n, head + "\n" + "\n".join(moved) + "\n" + body
    # register: a local kept in a register, or not (the compilers of the time did as they were told)
    for i, d in enumerate(decls[:MAX_LOCALS]):
        other = _toggle_register(d)
        if other:
            yield "register%d" % i, head + "\n" + "\n".join(decls[:i] + [other] + decls[i + 1:]) + "\n" + body
    # regparam: the same for an argument, where each is declared on a line of its own (f(a) int a; {)
    close = head.rfind(")")
    if close >= 0:
        lines = head[close + 1:].split("\n")
        for i, d in enumerate(lines):
            other = _toggle_register(d) if d.strip() else None
            if other:
                yield "regparam%d" % i, head[:close + 1] + "\n".join(lines[:i] + [other] + lines[i + 1:]) + (
                    "\n" + "\n".join(decls) if decls else "") + "\n" + body
    shadow = _blank(body)
    # swap: the operands of a commutative operator the other way round; a < b as b > a
    sites = [m for m in _PAIR.finditer(shadow) if m.group("op") in _COMMUTATIVE and m.group("a") != m.group("b")]
    for j, m in enumerate(sites[:MAX_SITES]):
        new = "%s%s %s %s%s" % (m.group("pre"), m.group("b"), _COMMUTATIVE[m.group("op")], m.group("a"), m.group("post"))
        yield "swap%d" % j, top + body[:m.start()] + new + body[m.end():]
    # switch: a chain of `if (v == A) .. else if (v == B) ..` as a switch on v
    from codebreaker.verify.authored import shape
    for j, (i0, i1, new) in enumerate(shape.switches(body, shadow)):
        if j >= MAX_SITES:
            break
        yield "switch%d" % j, top + body[:i0] + new + body[i1:]
    # fold: `t = f(..);` and at once `if (t == N)`, as `if (f(..) == N)`
    for j, (i0, i1, new) in enumerate(shape.folds(decls, body, shadow)):
        if j >= MAX_SITES:
            break
        yield "fold%d" % j, top + body[:i0] + new + body[i1:]
    # return: `r = x;` that only the final `return r;` follows, as `return x;`
    early = shape.returns(head, decls, body, shadow)
    if early:
        yield "return", top + early
    # test: the two sides of the one comparison an `if` or `while` makes, the other way round
    for j, (i0, i1, new) in enumerate(_tests(shadow, body)):
        if j >= MAX_SITES:
            break
        yield "test%d" % j, top + body[:i0] + new + body[i1:]
    # step: the ways of writing a statement that adds or takes one
    n = 0
    for m in _STEP.finditer(shadow):
        if n >= MAX_SITES:
            break
        d = _depth(shadow, m.start())
        if (m.group("post").strip() == ";" and d != 0) or (m.group("post").strip() == ")" and d != 1):
            continue                             # the condition of a for: its value is used
        x = m.group("x1") or m.group("x2") or m.group("x3")
        o = m.group("o1") or m.group("o2") or m.group("o3")
        sign = o[0]
        forms = ["%s%s%s" % (x, sign, sign), "%s%s%s" % (sign, sign, x), "%s %s= 1" % (x, sign)]
        mine = 0 if m.group("x1") else 1 if m.group("x2") else 2
        for k, f in enumerate(forms):
            if k != mine:
                yield "step%d.%d" % (n, k), top + body[:m.start()] + m.group("pre") + f + m.group("post") + body[m.end():]
        n += 1
    # assign: x = x + y  and  x += y
    n = 0
    for rx, form in ((_LONG, "%(x)s %(op)s= %(y)s"), (_SHORT, "%(x)s = %(x)s %(op)s %(y)s")):
        for m in rx.finditer(shadow):
            if n >= MAX_SITES:
                break
            d = _depth(shadow, m.start())
            if (m.group("post").strip() == ";" and d != 0) or (m.group("post").strip() == ")" and d != 1):
                continue
            yield "assign%d" % n, top + body[:m.start()] + m.group("pre") + form % m.groupdict() + m.group("post") + body[m.end():]
            n += 1


def repair_function(cl, env, header, source, name, original, work, flags):
    """repair() with Microsoft's 32-bit compiler of 1994 (verify/authored/match.py).

    Before the search, the setting the function is compiled in is chosen, since the files of
    one module were not all compiled alike: with and without a frame pointer kept (/Oy-), and
    with the whole module's declarations before it or only those it uses (match.needed_header).
    The search then runs in the setting whose plain compile is nearest; what was chosen is the
    first thing in `knobs` when it is not the default."""
    settings = [(flags, header, "")]
    if "/Oy-" not in flags:
        settings.append((flags + " /Oy-", header, "/Oy-"))
    near = match.needed_header(header, source)
    settings += [(f, near, (label + " own-declarations").strip()) for f, _h, label in list(settings)]
    best = None
    for f, h, label in settings:
        built = match.compile_function(cl, env, h, source, name, work, f)
        if built is None:
            continue
        s = score(built, original, 32)[0]
        if best is None or s < best[0]:
            best = (s, f, h, label)
        if not s[1]:
            break
    if best is None:
        return None
    _s, f, h, label = best
    r = repair(lambda text: match.compile_function(cl, env, h, text, name, work, f), source, original, bits=32)
    if r and label:
        r["knobs"] = ("[%s]," % label + r["knobs"]).strip(",")
    return r


def repair(compile, source, original, bits=0):
    """Search behaviour-preserving rewrites of one function's source for the bytes closest to the
    original's. `compile` is any period compiler: source text -> (bytes, mask) or None
    (build/period.py). `bits` (16 or 32, or "ppc" for PowerPC, whose masks are per word and per
    bit -- verify/authored/match.py, ARCH["powerpc"]) is the processor mode of the code: with it the search
    lines the two up by instruction and reads the size of the locals; without it, it has the
    bytes alone to go by. -> {source, verdict, distance, total, knobs, tried} for the best
    found, or None when the starting source does not compile.

    The search keeps the BEAM nearest sources and goes on from all of them: the first step
    toward the original is not always one that brings the bytes nearer by itself."""
    built = compile(source)
    if built is None:
        return None
    target = instructions(original, bits) if bits else None
    target_frame = _frame(original[0], bits) if bits else 0
    s, (v, d, t) = score(built, original, bits, target)
    best = (s, {"source": source, "verdict": v, "distance": d, "total": t, "knobs": ""})
    frontier = [(s, source, built, "")]
    seen, tried, stale = {source}, 1, 0
    while best[0][1] and tried < MAX_WORK and stale < PATIENCE and frontier:
        found = []
        for _s, text, was, path in frontier:
            draft_frame = _frame(was[0], bits) if bits else None
            for knob, variant in _knobs(text, target_frame, draft_frame):
                if variant in seen or tried >= MAX_WORK:
                    continue
                seen.add(variant)
                tried += 1
                now = compile(variant)
                if now is None:
                    continue
                s, (v, d, t) = score(now, original, bits, target)
                path2 = (path + "," + knob).strip(",")
                found.append((s, variant, now, path2))
                if s < best[0]:
                    best, stale = (s, {"source": variant, "verdict": v, "distance": d, "total": t, "knobs": path2}), -1
                if not d:
                    break
            if not best[0][1]:
                break
        stale += 1
        found.sort(key=lambda x: (x[0], len(x[3])))
        frontier = found[:BEAM]
    out = best[1]
    out["tried"] = tried
    return out


def truth(profile, limit=0, log=print):
    """Test the search on sources KNOWN to be the authored ones: a period compiler's own units,
    each of which compiles to the original's bytes. Each is knocked away from the original by
    one knob and by two (a rewrite that keeps its meaning but not its bytes) and the search must
    bring it back. -> (units, identical as they stand, knocked away, brought back)."""
    units = profile.units()
    work = os.path.join(_scratch(), "truth")
    compile = lambda text: profile.compile(text, work)
    n = same = knocked = back = 0
    for u in units[:limit or None]:
        n += 1
        if match.distance(compile(u.source), u.original)[0] != "identical":
            continue
        same += 1
        one = list(_knobs(u.source, 0, None))
        two = [(k + "," + k2, s2) for k, s in one[:2] for k2, s2 in list(_knobs(s, 0, None))[-2:]]
        kinds = {}
        for k, s in one:
            kinds.setdefault(k.rstrip("0123456789"), []).append((k, s))
        for knob, moved in [x for group in kinds.values() for x in group[:2]] + two[:2]:
            b = compile(moved)
            if b is None or match.distance(b, u.original)[1] == 0:
                continue                       # the compiler gives the same bytes either way
            knocked += 1
            r = repair(compile, moved, u.original, bits=getattr(profile, 'bits', 0))
            ok = bool(r) and r["distance"] == 0
            back += ok
            if log and not ok:
                log("  %-12s %-16s not brought back: distance %s after %s" % (
                    u.name, knob, r and r["distance"], r and r["knobs"]))
    return n, same, knocked, back


def _scratch():
    from codebreaker.base import settings
    return settings.state("repair")


def best_flags(cl, env, header, texts, order, originals, address, work, sample=60):
    """The compiler switches the module was most likely built with: the set under which most of
    a sample of its functions come out as the original's bytes, or at least the same length."""
    names = [n for n in order if originals.get(address[n])]
    names = names[::max(1, len(names) // sample)][:sample]
    best = (-1, match.FLAG_SETS[0])
    for flags in match.FLAG_SETS:
        score = 0
        for n in names:
            built = match.compile_function(cl, env, header, texts[n], n, work, flags)
            if built:
                v = match.distance(built, originals[address[n]])[0]
                score += 3 if v == "identical" else 1 if v == "same size" else 0
        if score > best[0]:
            best = (score, flags)
    return best[1]


def run_powerpc(module_path, flags, work, fs=None, log=print, limit=0):
    """run() for a PowerPC image, with the Xbox 360 compiler (verify/authored/match.py)."""
    from codebreaker.base import corpus
    from codebreaker.build import functions
    from codebreaker.store import functions as F
    from codebreaker.verify.lanes import powerpc
    tools = functions.powerpc_tools()
    if not tools:
        raise SystemExit("no PowerPC compiler (settings.json: powerpc_tools, powerpc_lib)")
    cl = tools[0]
    sha = corpus.sha(module_path) or ""
    fs = fs or F.open()
    m = powerpc.prepare(module_path, os.path.join(work, "source"))
    address = {name: va for va, name in m.by_va.items()}
    originals = match.ARCH["powerpc"].originals(m.image, sorted(address.values()))
    fs.add_module(sha, os.path.basename(module_path).lower(), module_path, "powerpc")
    fs.add_functions(sha, [m.key[n] for n in address])
    if not flags:
        sample = [n for n in m.order if originals.get(address[n])][::10][:60]
        best = (-1, match.POWERPC_FLAG_SETS[0])
        for f in match.POWERPC_FLAG_SETS:
            got = 0
            for n in sample:
                b = match.compile_powerpc(cl, m.header, m.texts[n], n, os.path.join(work, "flags"), f)
                if b:
                    v = match.distance(b, originals[address[n]], "powerpc")[0]
                    got += 3 if v == "identical" else 1 if v == "same size" else 0
            best = max(best, (got, f))
        flags = best[1]
        if log:
            log("  compiler switches that fit best: %s" % flags)
    todo = [n for n in m.order if originals.get(address[n]) is not None][:limit or None]
    import threading
    from concurrent.futures import ThreadPoolExecutor
    from codebreaker.base import parallel

    def one(name):
        own = os.path.join(work, "t%d" % threading.get_ident())
        return repair(lambda text: match.compile_powerpc(cl, m.header, text, name, own, flags),
                      m.texts[name], originals[address[name]], bits="ppc")
    with ThreadPoolExecutor(max_workers=parallel.workers()) as ex:
        results = list(ex.map(one, todo))
    counts = [0, 0, 0, 0]
    for name, r in zip(todo, results):
        if r is None:
            continue
        fs.record_variant(sha, m.key[name], "xbox 360 cl " + flags, r["source"], r["distance"], r["total"],
                          r["verdict"], knobs=r["knobs"], behaves="")
        counts[0] += 1
        counts[1 if r["verdict"] == "identical" else 2 if r["verdict"] == "same size" else 3] += 1
        if log and r["knobs"]:
            log("  %-28s %-9s distance %d/%d  via %s" % (name, r["verdict"], r["distance"], r["total"], r["knobs"]))
    return tuple(counts)


def run(module_path, flags, work, fs=None, log=print, limit=0):
    """Repair every function of a module and record the variants. -> (functions, identical, same
    size, different). `flags` None: the switches are found first (best_flags)."""
    from codebreaker.store import functions as F
    from codebreaker.verify.lanes import win32
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
    # the declarations a period compiler is given: imports as imports, globals as variables
    header, texts = match.period_source(m.header, m.texts, set(m.imports), match.relocation_targets(module_path))
    if not flags:
        flags = best_flags(cl, env, header, texts, m.order, originals, address, os.path.join(work, "flags"))
        if log:
            log("  compiler switches that fit best: %s" % flags)
    counts = [0, 0, 0, 0]
    todo = [n for n in m.order if originals.get(address[n]) is not None][:limit or None]
    import threading
    from concurrent.futures import ThreadPoolExecutor
    from codebreaker.base import parallel

    def one(name):
        own = os.path.join(work, "t%d" % threading.get_ident())      # a folder per worker
        return repair_function(cl, env, header, texts[name], name, originals[address[name]], own, flags)
    with ThreadPoolExecutor(max_workers=parallel.workers()) as ex:
        results = list(ex.map(one, todo))
    for name, r in zip(todo, results):
        va = address[name]
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
    if "--truth" in argv:
        from codebreaker.build import period
        rest = argv[argv.index("--truth") + 1:]
        if not rest:
            raise SystemExit("repair --truth <compiler> [target]   (have: %s)" % ", ".join(period.names()))
        n, same, knocked, back = truth(period.get(*rest[:2]))
        print("%s: %d sources, %d identical as they stand; %d knocked away from the original, %d brought back" % (
            " ".join(rest[:2]), n, same, knocked, back))
        return 0
    args = [a for a in argv if not a.startswith("--")]
    flags = argv[argv.index("--flags") + 1] if "--flags" in argv else None
    limit = int(argv[argv.index("--first") + 1]) if "--first" in argv else 0
    args = [a for a in args if a != flags and not ("--first" in argv and a == str(limit))]
    if not args:
        print(main.__doc__)
        return 2
    path = args[0] if os.path.isfile(args[0]) else corpus.retail(args[0])
    powerpc = args[0].lower().endswith(".xex")
    if powerpc:
        from codebreaker.formats import xex
        path = xex.basefile(args[0])
    elif path and corpus.lane(path) == "pe":
        from codebreaker.formats.pe import open_pe
        powerpc = open_pe(path).machine == 0x01F2
    if not path or corpus.lane(path) != "pe":
        raise SystemExit("%s: a 32-bit Windows module or a PowerPC image is needed" % args[0])
    work = settings.state("repair", os.path.basename(path).lower())
    n, ident, same, diff = (run_powerpc if powerpc else run)(path, flags, work, log=print, limit=limit)
    print("%s: %d functions repaired toward the original's bytes: %d identical, %d same size, %d different" % (
        os.path.basename(path), n, ident, same, diff))
    return 0
