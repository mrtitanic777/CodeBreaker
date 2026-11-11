"""prove.py -- a candidate held to the program: compiled (mended where the compiler stops), compared, and the
nearest refined by a beam search over the rewrites (REFINERS), the kind of difference choosing which first."""
import difflib
import os
import re

from codebreaker.base import settings
from codebreaker.exact import compare
from codebreaker.formats import omf
from codebreaker.exact.draft.text import _changed, _declare_pointer, BYTEPTR, split_body
from codebreaker.exact.draft.shapes import (adjacent_swaps, assign_loops, byte_heads, call_pairs, case_guards,
                                            chain_tails, char_widens, common_tails, const_tests, flat_returns,
                                            for_loops, heads_variant, label_sinks, loop_guards, loop_tests,
                                            return_cases_first, shared_exit, tail_continues, unsigned_fields,
                                            word_offsets)
from codebreaker.exact.draft.forms import (address_variants, and_returns, assign_in_args, assign_in_tests,
                                           chain_assigns, compare_results, compound_assigns, default_after,
                                           default_last, empty_cases_first, frame_more, frame_names, guard_flat,
                                           guard_returns, guard_shared, lead_filler, no_default, or_loops,
                                           pointer_widths, pre_increments, pre_steps, push_tails, register_copies,
                                           register_reads, register_temps, return_sources, scaled_places,
                                           shared_tails, split_ternaries, store_increments, switch_breaks, unmasked,
                                           unsigned_registers)
from codebreaker.exact.draft.candidates import (byte_fields, candidates, FPINFO, register_slots, registers_last,
                                                string_variants, unsigned_locals, void_variant)


ERRLINE = re.compile(r"\((\d+)\) : (?:error \w+: )?(.*)$", re.M)


def compile_fixes(src, con):
    """mendings of the line the compiler stopped at, as the error says ([] when none):
        left of `->fN' must have a struct/union type   E->fN  -> *(int *)((char *)(E) + N)
        illegal indirection                             *X     -> X an int * (or a char *); else *(char *)X
        subscript on non-array                          X[I]   -> X an int *; else ((int *)X)[I]
    (the model's expressions carry no types: what the code shows is given them - each way, the bytes decide)"""
    errs = [(int(m.group(1)), m.group(2)) for m in ERRLINE.finditer(con) if "warning" not in m.group(2)]
    if not errs:
        return []
    n, msg = errs[0]
    lines = src.split("\n")
    if not 0 < n <= len(lines):
        return []
    l = lines[n - 1]
    out = []
    m = re.search(r"(?<![\w)\]])\*(?!\()\s*(?:\+\+|--)?([A-Za-z_]\w*)", l) if "illegal indirection" in msg else \
        re.search(r"\b([A-Za-z_]\w*)\[", l) if "subscript on non-array" in msg else None
    if m:
        for ty in ("char", "int") if (m.group(1), 0) in BYTEPTR else ("int", "char"):   # (read a byte at a time: char)
            d = _declare_pointer(src, m.group(1), ty)
            if d:
                out.append(d)
    if m and "illegal indirection" in msg and (m.group(1), 0) not in BYTEPTR:   # (or a word through a cast in place:
        new = re.sub(r"(?<![\w)\]])\*(?!\()(\s*%s)\b" % re.escape(m.group(1)), r"*(int *)\1", l, count=1)   # the
        if new != l:                                                                                   # variable
            lines2 = list(lines)                                                                       # stays an int)
            lines2[n - 1] = new
            out.append("\n".join(lines2))
    if "bad left operand" in msg or "bad right operand" in msg:   # (a pointer in arithmetic only ints take: an int)
        for x in dict.fromkeys(re.findall(r"\b([A-Za-z_]\w*)\b", l)):
            d = re.sub(r"(?m)^([ \t]*)(register )?(?:unsigned )?char \*%s;$" % x, r"\1\2int %s;" % x, src)
            if d != src:
                out.append(d)
    if "2 pointers" in msg:                                 # (two pointers added: one of them an int - each way)
        for x in dict.fromkeys(re.findall(r"\b([A-Za-z_]\w*)\b", l)):
            d = re.sub(r"(?m)^([ \t]*)(register )?char \*%s;$" % x, r"\1\2int %s;" % x, src)
            if d != src:
                out.append(d)
    one = compile_fix(src, con)
    if one:
        out.append(one)
    return out


def compile_fix(src, con):
    """the source with the line the compiler stopped at mended, as the error says, or None:
        left of `->fN' must have a struct/union type   E->fN  -> *(int *)((char *)(E) + N)
        illegal indirection                             *X     -> *(char *)X
        subscript on non-array                          X[I]   -> ((int *)X)[I]
    (the model's expressions carry no types; a cast gives the compiler the one the code shows)"""
    errs = [(int(m.group(1)), m.group(2)) for m in ERRLINE.finditer(con) if "warning" not in m.group(2)]
    if not errs:
        return None
    n, msg = errs[0]
    lines = src.split("\n")
    if not 0 < n <= len(lines):
        return None
    l = lines[n - 1]
    m = re.search(r"left of .->f(\d+). must have a struct", msg)
    if m:
        f = m.group(1)
        new = re.sub(r"((?:\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)|[A-Za-z_]\w*(?:\[[^\]]*\])?)(?:(?:\.|->)f\d+)*)->f%s\b" % f,
                     lambda x: "*(int *)((char *)%s + %s)" % (x.group(1), f), l, count=1)
    elif "illegal indirection" in msg:
        new = re.sub(r"(?<![\w)\]])\*(?!\()(\s*[A-Za-z_]\w*)", r"*(char *)\1", l, count=1)
    elif "subscript on non-array" in msg:
        new = re.sub(r"\b([A-Za-z_]\w*)\[", r"((int *)\1)[", l, count=1)
    else:
        return None
    if new == l:
        return None
    lines[n - 1] = new
    return "\n".join(lines)


def _prove(prj, tc, src, a, ln, tries=4, budget=10):
    """the verdict on src - mended where the compiler says it cannot be compiled, each way the error offers, and
    again (a few rounds, a few compiles): the best of what compiles. _prove.src is the source the verdict is of"""
    comp = lambda s: tc.compile(s.encode("latin1", errors="replace"), headers=prj.headers(), include=prj.include, switches=prj.switches)
    frontier = [src]
    best = None
    first_con = None
    for depth in range(tries + 1):
        nxt = []
        for s in frontier:
            if budget <= 0:
                break
            budget -= 1
            _prove.spent = getattr(_prove, "spent", 0) + 1
            raw, con = comp(s)
            if raw is None:
                first_con = first_con or (s, con)
                nxt += compile_fixes(s, con)[:3] if depth < tries else []
                continue
            v = _verdict(prj, raw, a, ln)
            if v == "identical":
                _prove.src = s
                return v
            if best is None or _rank(v) < _rank(best[1]):
                best = (s, v)
        if not nxt or budget <= 0:
            break
        frontier = nxt
    if best:
        _prove.src = best[0]
        return best[1]
    s, con = first_con if first_con else (src, "")
    _prove.src = s
    last = [l for l in con.strip().splitlines() if "(the pass" not in l]
    errs_ = [l for l in last if "warning" not in l]       # (the error, not a warning before it)
    last = errs_[:1] or last
    return "does not compile: %s" % (last[-1][:100] if last else "")


BATCH = int(os.environ.get("CB_BATCH", "16"))               # (candidates compiled in one run: batch.py; 0 none)


def _batched(prj, tc, srcs, a, ln):
    """{source: verdict} of those of srcs a batch compiled (one run of the compiler, counted as two compiles);
    the rest are left to _prove"""
    if len(srcs) < 2:
        return {}
    from codebreaker.exact import batch
    try:
        got = batch.compile_many(prj, tc, srcs)
    except Exception:
        return {}
    _prove.spent = getattr(_prove, "spent", 0) + 2
    out = {}
    for i, (obj, k) in got.items():
        try:
            out[srcs[i]] = _verdict(prj, obj, a, ln, k)
        except Exception:
            pass
    return out


def _verdict(prj, raw, a, ln, k=0):
    """the verdict on the k-th function of the object (a batch's: batch.compile_many)"""
    o = omf.read(raw)
    segs = omf.code_segments(o)
    fns = sorted(omf.functions(o, segs[0]), key=lambda f: f[1]) if segs else []
    if len(fns) <= k:
        return "no function compiled"
    name, off, l2 = fns[k]
    bad, notes = compare.function(o, prj.image, name, off, l2, a, prj, dict(prj.symbols()), {}, segs[0])
    if compare.identical(bad, notes) and l2 == ln:
        return "identical"
    return "differs: %d bytes%s" % (len(bad), ", length %d not %d" % (l2, ln) if l2 != ln else "")


def _rank(v):
    """how near a verdict is: one that differs (the right length first, then by fewer bytes), before one that
    does not compile, before one with registers left"""
    import re as _re
    m = _re.match(r"differs: (\d+) bytes(?:, length (\d+) not (\d+))?", v)
    if m:
        gap = abs(int(m.group(2)) - int(m.group(3))) if m.group(2) else 0
        return (0, 1 if gap else 0, int(m.group(1)) + gap)  # (what is missing counts as differing)
    return (1, 0, 0) if v.startswith("does not") else (2, 0, 0)


def prove_texts(prj, tc, inv, texts, a, ln, names):
    """-> (source, verdict): of each of the model's texts [(text, register parameters)] and their variants, the
    first the compiler proves identical; else the best of them (one that differs before one that does not
    compile, before one with registers left)"""
    best = None
    texts = list(texts)
    for text, regparam in list(texts):                      # (a register loaded from a parameter and then given
        given = {r for r in (regparam or {}) if re.search(r"(?m)^[ \t]*%s = " % r, text)}   # a new value: maybe a
        for g_ in sorted(given):                                                             # local the parameter
            texts.append((text, {r: i for r, i in regparam.items() if r != g_}))             # started - the compiler says)
        if len(given) > 1:
            texts.append((text, {r: i for r, i in regparam.items() if r not in given}))
    for text, regparam in texts:
        src, v = prove_best(prj, tc, inv, text, a, ln, names, regparam)
        if v == "identical":
            return src, v
        rank = _rank(v)
        if best is None or rank < best[0]:
            best = (rank, src, v)
    return best[1], best[2]


MAXCANDS = 64
FUNC_BUDGET = 150                                           # (compiles a function's candidates may take in all)


def _probes(code):
    """True: the prologue calls the stack probe (`mov ax,N` or `xor ax,ax`, then a call, after `mov bp,sp`);
    False: a frame without one (`sub sp,N`, or none and no call there); None: not a frame the rule knows"""
    from codebreaker.exact import x86
    ins = list(x86.disasm(bytes(code), 0, min(len(code), 16), False))
    if len(ins) < 2 or ins[0].mnemonic != "push" or ins[0].op_str != "bp" or ins[1].op_str != "bp, sp":
        return None
    if len(ins) > 3 and (ins[2].op_str.startswith("ax, ") and ins[2].mnemonic in ("mov", "xor")) and ins[3].mnemonic == "call":
        return True
    return False


HOPELESS = (0.25, 0.6)                                      # (a length off by more than this part of the function's,


def _hopeless(v, ln):
    """a verdict too far from the program's code for the beam's rewrites to close (a statement missing, a loop
    misread): the model's reading is wrong, not its spelling"""
    m = re.match(r"differs: (\d+) bytes(?:, length (\d+) not (\d+))?", v)
    if not m or not ln:
        return False
    gap = abs(int(m.group(2)) - int(m.group(3))) if m.group(2) else 0
    # (by the length alone: a byte too many early shifts every byte after it, so the bytes that differ say
    #  little - a function 36 bytes off by one instruction was closed by the beam)
    return gap > max(16, HOPELESS[0] * ln)


def _canon(src):
    """a candidate as the compiler sees it, for telling two apart: a doubled pair of parentheses one, the
    spacing one (neither changes a byte of the code) - not what is compiled, only what is compared"""
    t = re.sub(r"[ \t]+", " ", src)
    for _ in range(6):
        t2 = re.sub(r"\(\((?P<x>[^()]*(?:\([^()]*\)[^()]*)*)\)\)", r"(\g<x>)", t)
        if t2 == t:
            break
        t = t2
    return t


def _decl_sig(src):
    """a candidate's declarations - its parameters' and locals' types, names aside - and whether it returns a
    value: the kind of reading it is"""
    head, sep, body = split_body(src)
    decls = re.findall(r"(?m)^[ \t]*((?:register )?(?:unsigned )?(?:char|int|long|short|struct \w+|double|float)\b[^;(]*);",
                       head.split("\n", 1)[-1] + body.split("\n\n", 1)[0])
    norm = tuple(sorted(re.sub(r"\b(?:[a-z]|v\d+\w*|r_\w+|_p\w*)\b(?=[\[;, ]|$)", "X", d) for d in decls))
    return norm, bool(re.search(r"\breturn [^;]", body))


def prove_best(prj, tc, inv, text, a, ln, names, regparam=None):
    """-> (source, verdict): of the model's text and its variants, the first the compiler proves identical; else
    the first that compiles (or the first)"""
    regs = tc.p.get("carve", {}).get("register_variables", ["si", "di"])
    if regparam is None:
        regparam = dict(getattr(inv, "LAST", {}).get("regparam") or {})
    widths = byte_fields(inv, bytes(prj.image[a:a + ln]))
    BYTEPTR.clear()
    BYTEPTR.update(widths)
    cands = candidates(prj, text, a, names, regs, regparam, widths)
    seen = set()
    uniq = []
    for src, left in cands:                                 # (each text once - spacing and doubled parentheses
        c_ = _canon(src)                                    #  aside; the first readings first, and at most
        if c_ not in seen:                                  #  MAXCANDS of them: a bench must end)
            seen.add(c_)
            uniq.append((src, left))
    cands = uniq[:MAXCANDS]
    fpsw, fsw, gs = tc.p.get("fp_switch"), tc.p.get("file_switches"), tc.p.get("probe_switch")
    sw = prj.switches if prj.switches is not None else tc.p.get("switches", "")
    want = sw.split()
    if FPINFO.get("fp") and fpsw and fpsw not in want:      # (code of the 8087's: compiled for it - the profile's
        want.append(fpsw)                                   #  "fp_switch")
    if gs:                                                  # (a stack probe in the prologue, or none: the switch
        probe = _probes(bytes(prj.image[a:a + ln]))         #  that leaves it out - "probe_switch" - as it shows)
        if probe and gs in want:
            want.remove(gs)
        elif probe is False and gs not in want:
            want.append(gs)
    if fsw and want != sw.split():
        cands = [("/* %s: %s */\n" % (fsw, " ".join(want)) + src, left) for src, left in cands]
    best = None
    tops = []                                               # (every reading that compiled, near or far: the beam's seeds)
    _prove.spent = 0
    known = {}                                              # (verdicts a batch gave: batch.compile_many)
    for ci, (src, left) in enumerate(cands):
        if left:
            v = "registers left: %s" % " ".join(left)
        elif _prove.spent >= FUNC_BUDGET and best is not None:
            break                                           # (compiled enough for one function: the best so far)
        else:
            if BATCH and src not in known:                  # (the next ones compiled together, one run of the compiler)
                ahead = [s for s, l in cands[ci:] if not l and s not in known][:BATCH]
                known.update(_batched(prj, tc, ahead, a, ln))
            if src in known:
                v = known[src]
            else:
                v = _prove(prj, tc, src, a, ln)
                src = _prove.src                            # (as the compiler's errors mended it)
            if v != "identical":
                for alt in string_variants(prj, src):       # (a number that is a string's place: the string)
                    if _prove(prj, tc, alt, a, ln) == "identical":
                        src, v = _prove.src, "identical"
                        break
        if v == "identical":
            return src, v
        rank = _rank(v)
        if v.startswith("differs"):
            tops.append((rank, src, v))
        if best is None or rank < best[0]:
            best = (rank, src, v)
    if best and best[2].startswith("differs") and _hopeless(best[2], ln):
        return best[1], best[2]                             # (too far for rewrites to close: the reading's own fault)
    if best and best[2].startswith("differs"):              # (the nearest that compile, rewritten - a beam of them:
        ranked = sorted(tops, key=lambda x: x[0])
        seeds = ranked[:BEAM]
        kinds = {}                                          # (and the nearest of each other way of declaring the
        for r_ in ranked:                                   #  locals - a buffer, a pointer's type - the beam's
            sig = _decl_sig(r_[1])                          #  start from each reading, not only the nearest)
            if sig not in kinds:
                kinds[sig] = r_
        seeds += [r_ for r_ in kinds.values() if r_ not in seeds][:BEAM]
        return refine(prj, tc, a, ln, best, seeds=seeds)    # combinations the
    return best[1], best[2]                                                                  #  variants alone miss)


def _place_variants(prj, src):
    """the numbers a pointer is given or compared with in a localized source as the places in the data they are"""
    out = []
    for name, t in address_variants(src, 0x10000, most=3):
        if not re.search(r"(?m)^extern .*\b%s\b" % name, t):
            t = "extern char %s[];\n" % name + t
        out.append(t)
    return out


REFINERS = [lambda prj, s: [assign_in_args(s)], lambda prj, s: [return_sources(s)], lambda prj, s: [compound_assigns(s)],
            lambda prj, s: [chain_assigns(s)], lambda prj, s: [for_loops(s)], lambda prj, s: [store_increments(s)],
            lambda prj, s: [pre_steps(s)], lambda prj, s: [pre_increments(s)], lambda prj, s: [assign_in_tests(s)],
            lambda prj, s: [and_returns(s)], lambda prj, s: [register_temps(s)], lambda prj, s: [void_variant(s)],
            lambda prj, s: pointer_widths(s), _place_variants, lambda prj, s: string_variants(prj, s),
            lambda prj, s: [split_ternaries(s)], lambda prj, s: [default_last(s)], lambda prj, s: [push_tails(s)],
            lambda prj, s: [unsigned_registers(s)], lambda prj, s: [unmasked(s)], lambda prj, s: [no_default(s)],
            lambda prj, s: [empty_cases_first(s)], lambda prj, s: [register_slots(s)], lambda prj, s: [register_copies(s)],
            lambda prj, s: [lead_filler(s)], lambda prj, s: [scaled_places(s)], lambda prj, s: [shared_tails(s)],
            lambda prj, s: [guard_returns(s)], lambda prj, s: [default_after(s)], lambda prj, s: [frame_more(s)],
            lambda prj, s: [loop_tests(s)], lambda prj, s: [switch_breaks(s)], lambda prj, s: [register_reads(s)],
            lambda prj, s: [compare_results(s)], lambda prj, s: [or_loops(s)], lambda prj, s: [loop_guards(s)],
            lambda prj, s: [const_tests(s)], lambda prj, s: [flat_returns(s)], lambda prj, s: [return_cases_first(s)],
            lambda prj, s: [registers_last(s)], lambda prj, s: unsigned_locals(s), lambda prj, s: [guard_shared(s)], lambda prj, s: [guard_flat(s)], lambda prj, s: [_changed(common_tails, s)], lambda prj, s: [_changed(byte_heads, s)], lambda prj, s: [_changed(word_offsets, s)], lambda prj, s: [chain_tails(s)], lambda prj, s: [heads_variant(s)], lambda prj, s: case_guards(s), lambda prj, s: [shared_exit(s)], lambda prj, s: [label_sinks(s)], lambda prj, s: [call_pairs(s)], lambda prj, s: [assign_loops(s)], lambda prj, s: [char_widens(s)], lambda prj, s: [_changed(tail_continues, s)], lambda prj, s: adjacent_swaps(s), lambda prj, s: unsigned_fields(s),
            lambda prj, s: [frame_names(s, prj.toolchain().p["frame_order"]), frame_names(s, prj.toolchain().p["frame_order"], True)]
            if prj.toolchain().p.get("frame_order") else [],
            lambda prj, s: [re.sub(r"\b([A-Za-z_]\w*), 0\)", r"(long)\1)", s, count=1)],   # (a value and a zero word
                                                                                               #  pushed: it widened)
            lambda prj, s: [re.sub(r"\b0, 0\)", "0L)", s)],     # (two zero words pushed: a long 0)
            lambda prj, s: [re.sub(r"(?<=\()0, 0(?=, )", "0L", s, count=1)]]   # (the same first of more arguments)
AIM_KIND = {                                                # (the refiners each kind of difference asks for first -
    "frame": {"frame_names", "registers_last", "frame_more", "lead_filler", "register_slots"},   # aim.py's kinds;
    "width": {"pointer_widths", "char_widens", "unsigned_registers", "unmasked", "byte_heads", "unsigned_fields", "high_words",   # the
              "word_offsets", "scaled_places"},                                                  # rest after them)
    "sign": {"unsigned_locals", "unsigned_fields", "unsigned_registers"},
    "order": {"adjacent_swaps", "return_cases_first", "empty_cases_first", "default_after", "default_last", "switch_breaks",
              "flat_returns", "guard_returns", "guard_shared", "guard_flat", "shared_tails", "push_tails", "loop_tests",
              "or_loops", "loop_guards", "for_loops", "label_sinks", "shared_exit", "case_guards", "chain_tails",
              "common_tails", "heads_variant", "no_default", "assign_loops", "tail_continues"},
    "shape": {"assign_in_args", "return_sources", "compound_assigns", "chain_assigns", "store_increments", "pre_steps",
              "pre_increments", "assign_in_tests", "and_returns", "register_temps", "split_ternaries", "compare_results",
              "const_tests", "register_reads", "register_copies", "word_offsets", "call_pairs", "string_variants",
              "address_variants"},
}
LINE_KINDS = AIM_KIND["shape"]                              # (rewrites of a statement: kept only where the code differs)


def _refiner_name(f):
    skip = {"re", "sub", "toolchain", "p", "get", "prj", "_changed", "search"}
    return next((n for n in f.__code__.co_names if n not in skip), "other")


_AIMS = {}
PRIORS = {}                                                 # ({kind: {refiner: wins}} - the beam's history, learned)


def _priors():
    if not PRIORS:
        try:
            import json
            PRIORS.update(json.load(open(settings.state("exact", "priors.json"))))
        except Exception:
            PRIORS["_"] = {}
    return PRIORS


def _learn(kinds, name):
    """a refiner that brought a reading nearer, for a difference of these kinds: counted (state/exact/priors.json)"""
    import json
    p = _priors()
    for k in kinds or ("shape",):
        p.setdefault(k, {})
        p[k][name] = p[k].get(name, 0) + 1
    f = settings.state("exact", "priors.json")
    try:                                                    # (merged with what other processes wrote meanwhile)
        disk = json.load(open(f))
        for k, d in disk.items():
            for n_, c in d.items():
                if isinstance(c, int):
                    p.setdefault(k, {})
                    p[k][n_] = max(p[k].get(n_, 0), c)
    except Exception:
        pass
    tmp = "%s.%d.tmp" % (f, os.getpid())
    try:
        json.dump(p, open(tmp, "w"), sort_keys=True)
        os.replace(tmp, f)
    except OSError:
        pass


def _aim(prj, tc, src, a, ln):
    """aim.diagnose of a candidate (one compile more, with the listing; kept per text)"""
    if src in _AIMS:
        return _AIMS[src]
    from codebreaker.exact import aim
    d = None
    try:
        obj, con, lst = tc.compile(src.encode("latin1", errors="replace"), headers=prj.headers(), include=prj.include,
                                   switches=prj.switches, listing=True)
        code = aim.listing_code(lst)
        if obj is not None and code:
            d = aim.diagnose(bytes(prj.image[a:a + ln]), aim.code_of(code), code)
    except Exception:
        d = None
    _AIMS[src] = d
    return d


def _touches(s, alt, lines):
    """does alt change s at one of these source lines (or within one of them)"""
    if not lines:
        return True
    sl, al = s.split("\n"), alt.split("\n")
    sm = difflib.SequenceMatcher(None, sl, al, autojunk=False)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag != "equal" and any(i1 - 1 <= l - 1 <= max(i2, i1 + 1) for l in lines):
            return True
    return False


def _ordered(prj, tc, s_, a, ln):
    """the refiners in the order a reading's difference asks for: [(refiner, name, lines-filter or None)] - the
    kinds diagnosed first (by their wins so far), the rest after"""
    d = _aim(prj, tc, s_, a, ln)
    kinds = sorted((d or {}).get("kinds", {}), key=lambda k: -(d["kinds"][k]))
    pri = _priors()
    named = [(f, _refiner_name(f)) for f in REFINERS]
    first, rest = [], []
    for f, n in named:
        hit = [k for k in kinds if n in AIM_KIND.get(k, ())]
        (first if hit else rest).append((f, n))
    score = lambda fn: -sum(pri.get(k, {}).get(fn[1], 0) for k in (kinds or ["shape"]))
    first.sort(key=score)
    rest.sort(key=score)
    lines = (d or {}).get("lines", [])[:6] if kinds and set(kinds) <= {"shape"} else None
    return [(f, n, lines if n in LINE_KINDS else None) for f, n in first] + [(f, n, None) for f, n in rest], kinds


BEAM = 4                                                    # (the readings the search carries on with each round)
AIM = os.environ.get("CB_AIM", "1") != "0"                 # (the beam aimed by the differences: aim.py)
REFINE_BUDGET = 240                                         # (the compiles it may spend on one function)


def refine(prj, tc, a, ln, best, rounds=6, budget=None, seeds=None):
    """-> (source, verdict): a beam search over the REFINERS' rewritings - the BEAM nearest readings (the best and
    the other seeds) each rewritten every way, the nearest BEAM of all kept, again, until one is identical, no round
    brings any nearer, or the budget of compiles is spent. Two rewritings neither of which helps alone are found
    together: a reading as near as the worst kept goes on (sideways moves) when the beam has room"""
    budget = REFINE_BUDGET if budget is None else budget
    beam = []
    seen = set()
    for r_, s_, v_ in [best] + list(seeds or []):
        if _canon(s_) not in seen:
            seen.add(_canon(s_))
            beam.append((r_, s_, v_))
    def keep(xs):                                           # (the nearest BEAM, and the nearest of each other kind
        xs = sorted(xs, key=lambda x: x[0])                 #  of reading - its declarations - up to BEAM more)
        out, kinds = xs[:BEAM], {_decl_sig(x[1]) for x in xs[:BEAM]}
        for x in xs[BEAM:]:
            k = _decl_sig(x[1])
            if k not in kinds and len(out) < 2 * BEAM:
                kinds.add(k)
                out.append(x)
        return out
    beam = keep(beam)
    spent = 0
    top = beam[0]
    for _ in range(rounds):
        pool = []
        for r_, s_, v_ in beam:
            order, kinds = _ordered(prj, tc, s_, a, ln) if AIM else ([(f, _refiner_name(f), None) for f in REFINERS], [])
            work, deferred = [], []                         # (the rewrites of this reading, in the aim's order: those
            for f, name, lines in order:                    #  at the lines that differ, then the rest)
                try:
                    alts = [x for x in f(prj, s_) if x and x != s_]
                except Exception:
                    continue
                for alt in alts:
                    c_ = _canon(alt)
                    if c_ in seen:
                        continue
                    seen.add(c_)
                    (deferred if lines is not None and not _touches(s_, alt, lines) else work).append((name, alt))
            work += deferred
            for w0 in range(0, len(work), max(1, BATCH)):
                if spent >= budget:
                    break
                chunk = work[w0:w0 + max(1, BATCH)]
                known = _batched(prj, tc, [x for _, x in chunk], a, ln) if BATCH else {}
                spent += 2 if known else 0
                for name, alt in chunk:
                    if alt in known:
                        v2, got = known[alt], alt
                    else:
                        if spent >= budget:
                            break
                        spent += 1
                        v2 = _prove(prj, tc, alt, a, ln, tries=2, budget=3)
                        got = _prove.src
                    if v2 == "identical":
                        if AIM:
                            _learn(kinds, name)
                        return got, v2
                    if v2.startswith("differs"):
                        if AIM and _rank(v2) < r_:
                            _learn(kinds, name)
                        pool.append((_rank(v2), got, v2))
        if not pool:
            break
        merged = keep(beam + pool)
        if merged[0][0] >= top[0] and all(x in beam for x in merged):
            break                                           # (nothing nearer, nothing new: the search is done)
        beam = merged
        top = min(top, beam[0], key=lambda x: x[0])
        if spent >= budget:
            break
    return top[1], top[2]
