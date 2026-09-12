#!/usr/bin/env python3
"""Auto-axis brute force — explore source forms WITHOUT hand-marking spots.

Each "axis" detects a transformable pattern in the C and yields alternative full-source
variants.  auto_crack() runs greedy coordinate-descent: each round it tries every axis's
variants (batch-compiled for free), keeps any that lowers the target function's diff, and
repeats until 0 diffs or no improvement.  This is the automated form of the manual
brute-force -- the LLM supplies one base reconstruction and the tool tries the rest.

Axes here are the ones proven to crack real residuals (ternary cracked seg6 Flush;
positive-test flip took Refill 44->8; store-swap targets seg9-style init ordering).
"""
import re
import bruteforce


# ---- individual axes: each takes source text, returns list of variant sources ----

def axis_ternary(src):
    """if (C)\\n return A;\\n return B;   ->   return (C) ? A : B;   (and inverted)."""
    pat = re.compile(
        r"( *)if \((?P<c>[^\n]+?)\)\s*\n *return (?P<a>[^;\n]+);\s*\n *return (?P<b>[^;\n]+);")
    out = []
    for m in pat.finditer(src):
        ind = m.group(1)
        tern = "%sreturn (%s) ? %s : %s;" % (ind, m.group("c"), m.group("a"), m.group("b"))
        out.append(src[:m.start()] + tern + src[m.end():])
        inv = "%sif (!(%s))\n%s    return %s;\n%sreturn %s;" % (
            ind, m.group("c"), ind, m.group("b"), ind, m.group("a"))
        out.append(src[:m.start()] + inv + src[m.end():])
    return out


def axis_postest(src):
    """if (C)\\n goto L;\\n STMT;  <->  if (!C) STMT;  (positive/negative test flip)."""
    pat = re.compile(r"( *)if \((?P<c>[^\n]+?)\)\s*\n *goto (?P<l>\w+);\n(?P<s>(?: *[^\n]+;\n)+?)")
    out = []
    for m in pat.finditer(src):
        ind = m.group(1)
        body = m.group("s").rstrip("\n")
        flip = "%sif (!(%s)) {\n%s\n%s} else\n%s    goto %s;" % (
            ind, m.group("c"), body, ind, ind, m.group("l"))
        out.append(src[:m.start()] + flip + src[m.end():])
    return out


def axis_incdec(src):
    """x += 1;  <->  x++;   (per site)."""
    out = []
    for m in re.finditer(r"(\w+) \+= 1;", src):
        out.append(src[:m.start()] + m.group(1) + "++;" + src[m.end():])
    for m in re.finditer(r"(\w+)\+\+;", src):
        out.append(src[:m.start()] + m.group(1) + " += 1;" + src[m.end():])
    return out


def axis_store_swap(src):
    """Swap two adjacent same-indent global assignments (drives store ordering, e.g. seg9 init)."""
    pat = re.compile(r"( *)([A-Za-z_]\w* = [^\n;]+;)\n\1([A-Za-z_]\w* = [^\n;]+;)")
    out = []
    for m in pat.finditer(src):
        ind = m.group(1)
        swapped = "%s%s\n%s%s" % (ind, m.group(3), ind, m.group(2))
        out.append(src[:m.start()] + swapped + src[m.end():])
    return out


def axis_signed(src):
    """Toggle a local's signedness: `int NAME;` <-> `unsigned int NAME;` (drives JL vs JB)."""
    out = []
    for m in re.finditer(r"\n( +)int (\w+);", src):
        out.append(src[:m.start()] + "\n%sunsigned int %s;" % (m.group(1), m.group(2))
                   + src[m.end():])
    for m in re.finditer(r"\n( +)unsigned int (\w+);", src):
        out.append(src[:m.start()] + "\n%sint %s;" % (m.group(1), m.group(2)) + src[m.end():])
    return out


def axis_leading_local(src, funcname_hint=None):
    """Toggle an unused leading `int _i;` at the start of each far-function body."""
    out = []
    for m in re.finditer(r"(\n(?:int|long|void|WORD|DWORD|BYTE)[^\n]*\bfar\b[^\n]*\)\s*\n\{\n)", src):
        ins = m.end()
        if src[ins:ins + 40].lstrip().startswith("int _i;"):
            continue
        out.append(src[:ins] + "    int _i;\n" + src[ins:])
    # also the reverse: remove an existing leading int _i;
    for m in re.finditer(r"\n\{\n    int _i;\n", src):
        out.append(src[:m.start()] + "\n{\n" + src[m.end():])
    return out


def _match_brace(src, open_pos):
    """open_pos indexes a '{'; return index just past the matching '}' (or -1)."""
    depth = 0
    i = open_pos
    while i < len(src):
        if src[i] == "{":
            depth += 1
        elif src[i] == "}":
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    return -1


def axis_ifelse_swap(src):
    """if (C) {A} else {B}  ->  if (!(C)) {B} else {A}  — brace-aware; changes which
    branch is the fall-through, i.e. basic-block layout (the seg9-tail residual class)."""
    out = []
    for m in re.finditer(r"if \((.+?)\) \{", src):
        ob = m.end() - 1
        eb = _match_brace(src, ob)
        if eb < 0:
            continue
        em = re.match(r" else \{", src[eb:])
        if not em:
            continue
        ob2 = eb + em.end() - 1
        eb2 = _match_brace(src, ob2)
        if eb2 < 0:
            continue
        cond, a, b = m.group(1), src[ob + 1:eb - 1], src[ob2 + 1:eb2 - 1]
        swapped = "if (!(%s)) {%s} else {%s}" % (cond, b, a)
        out.append(src[:m.start()] + swapped + src[eb2:])
    return out


def axis_reassoc(src):
    """A + B + 1  ->  A + (B + 1) / (B + 1) + A / B + A + 1  — reorders pointer/index
    arithmetic so MSC picks a different operand to load first (Refill's gp4c residual)."""
    out = []
    for m in re.finditer(r"(\w+) \+ (\w+) \+ 1\b", src):
        a, b = m.group(1), m.group(2)
        for alt in ("%s + (%s + 1)" % (a, b), "(%s + 1) + %s" % (b, a),
                    "%s + %s + 1" % (b, a), "%s + 1 + %s" % (a, b)):
            out.append(src[:m.start()] + alt + src[m.end():])
    return out


def axis_ptr_temp(src):
    """G = P + expr;  ->  { T t = P; t += expr; G = t; }  and the reverse — nudges MSC to
    load the pointer into a fresh accumulator instead of reusing a dead index register."""
    out = []
    for m in re.finditer(r"( *)(\w+) = (\w+) \+ ([^;\n]+);", src):
        ind, g, p, ex = m.groups()
        form = ("%s%s = %s;\n%s%s += %s;" % (ind, g, p, ind, g, ex))
        out.append(src[:m.start()] + form + src[m.end():])
    return out


def axis_array_index(src):
    """G = P + expr;  ->  G = &P[expr];  and  *(P + expr)  <->  P[expr]  — array-index
    form makes MSC compute the address differently (loads base fresh vs reusing index reg)."""
    out = []
    for m in re.finditer(r"( *)(\w+) = (\w+) \+ ([^;\n]+);", src):
        ind, g, p, ex = m.groups()
        out.append(src[:m.start()] + "%s%s = &%s[%s];" % (ind, g, p, ex) + src[m.end():])
    for m in re.finditer(r"\*\((\w+) \+ ([^)]+)\)", src):
        out.append(src[:m.start()] + "%s[%s]" % (m.group(1), m.group(2)) + src[m.end():])
    return out


_TYPES = r"(?:register )?(?:unsigned )?(?:int|long|WORD|DWORD|BYTE|char)"


def axis_register(src):
    """Toggle `register` on each simple local decl — forces/frees a register variable,
    shifting MSC's allocation (the register-allocation-tie-break residual class)."""
    out = []
    for m in re.finditer(r"\n( +)((?:unsigned )?(?:int|long|WORD|DWORD|BYTE|char)) (\w+);", src):
        out.append(src[:m.start()] + "\n%sregister %s %s;" %
                   (m.group(1), m.group(2), m.group(3)) + src[m.end():])
    for m in re.finditer(r"\n( +)register ((?:unsigned )?\w+) (\w+);", src):
        out.append(src[:m.start()] + "\n%s%s %s;" %
                   (m.group(1), m.group(2), m.group(3)) + src[m.end():])
    return out


def axis_decl_reorder(src):
    """Swap two adjacent local declarations — changes the allocator's variable priority."""
    out = []
    pat = re.compile(r"( *)(%s [^\n;]*;)\n\1(%s [^\n;]*;)" % (_TYPES, _TYPES))
    for m in pat.finditer(src):
        out.append(src[:m.start()] + "%s%s\n%s%s" %
                   (m.group(1), m.group(3), m.group(1), m.group(2)) + src[m.end():])
    return out


def axis_dummy_var(src):
    """Insert an unused local at the top of each far-function body — occupies a register
    slot and reshuffles allocation of the real variables."""
    out = []
    for m in re.finditer(r"(\)\s*\n\{\n)", src):
        ins = m.end()
        head = src[ins:ins + 60].lstrip()
        if head.startswith("int _d") or head.startswith("WORD _d"):
            continue
        for decl in ("    int _d;\n", "    WORD _d;\n", "    int _d, _e;\n"):
            out.append(src[:ins] + decl + src[ins:])
    return out


def axis_extern_reorder(src):
    """Swap adjacent `extern` global declarations.  MSC's global register allocator picks
    which operand of an expression to load first partly from declaration order, so this
    shifts the register assignment (the register-allocation-tie-break class).  This is the
    seg9-agent-validated lever for the exact class Refill/LZREAD/seg7-Decode fall into."""
    out = []
    pat = re.compile(r"(extern [^\n;]+;)([^\n]*)\n(extern [^\n;]+;)([^\n]*)")
    for m in pat.finditer(src):
        a = m.group(1) + m.group(2)
        b = m.group(3) + m.group(4)
        out.append(src[:m.start()] + b + "\n" + a + src[m.end():])
    return out


def axis_pin_memory(src):
    """Insert `(void)&NAME;` right after a local decl to pin it to memory (deny it a
    register), which frees that register for another variable — the seg7-agent-proven
    lever for the register-split residual class (di/hf landing in the wrong register)."""
    out = []
    for m in re.finditer(r"\n( +)((?:unsigned )?(?:int|long|WORD|DWORD|BYTE|char)(?: far \*| \*)?) (\w+);", src):
        ins = m.end()
        out.append(src[:ins] + "\n%s(void)&%s;" % (m.group(1), m.group(3)) + src[ins:])
    return out


AXES = [axis_ternary, axis_postest, axis_incdec, axis_store_swap,
        axis_signed, axis_leading_local, axis_ifelse_swap, axis_reassoc,
        axis_ptr_temp, axis_array_index,
        axis_register, axis_decl_reorder, axis_dummy_var, axis_extern_reorder,
        axis_pin_memory]


def auto_crack(base, workdir, seg, func_index, func_ranges,
               axes=AXES, rounds=6, cap=400, verbose=True):
    """Greedy breadth-first search over source forms.  Each round generates the variants
    from EVERY axis applied to the current best and compiles them ALL in ONE DOSBox launch
    (so a whole round = a single window), then keeps the best.  Returns (best_src, diffs)."""
    cur = base
    best = 10 ** 9
    for rnd in range(rounds):
        # one batch: the current base + every axis's variants of it
        variants = [("base", cur)]
        seen = {cur}
        for axis in axes:
            for i, c in enumerate(axis(cur)):
                if c in seen:
                    continue
                seen.add(c)
                variants.append(("%s_%d" % (axis.__name__[5:], i), c))
        if len(variants) > cap:
            variants = variants[:cap]
        objs = bruteforce.batch_compile(variants, workdir)      # <-- ONE DOSBox window
        round_best, round_src, round_lbl = best, cur, "base"
        for label, src in variants:
            obj = objs.get(label)
            if not obj:
                continue
            try:
                d = bruteforce._func_diff(obj, seg, func_index, func_ranges)
            except Exception:
                continue
            if d < round_best:
                round_best, round_src, round_lbl = d, src, label
        if verbose:
            print("  round %d: %d variants -> best=%d diffs (%s)" %
                  (rnd, len(variants), round_best, round_lbl))
        if round_best < best:
            best, cur = round_best, round_src
            if best == 0:
                return cur, 0
        else:
            break   # no improvement this round
    return cur, best
