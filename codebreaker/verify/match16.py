"""match16.py -- does Microsoft C 6 turn a 16-bit module's reconstruction back into its bytes?

    python -m codebreaker match <16-bit module> [--flags "/AM /Gcsw /Os"] [--first N]

The 16-bit counterpart of match.py (see there for what the comparison is and is not). Each
reconstructed function is compiled by Microsoft C 6.0a (build/msc6.py: the real compiler, in an
emulator, a whole module's functions in one session) and its bytes are compared with the
original function's, outside the places a loader or linker fills in: in the original, every
word a relocation record of its segment names and the distance of every near call and jump to
another function; in the compiled function, every place the object file has a fix-up for.

Without --flags the switches are found first: the memory model and options under which most of
a sample of the functions come out the original's length say how the module was built.
"""
import os
import struct

from codebreaker.verify import match

_SIZE = {0: 1, 2: 2, 3: 4, 5: 2, 11: 6, 13: 4}              # bytes a relocation of each source type covers


def original_functions(module_path, where, spans):
    """{address: (bytes, mask)} for a 16-bit module's functions. `where` {address: (segment,
    offset)}, `spans` {address: ((start, end), instructions)} as verify/win16.py makes them."""
    import capstone
    from codebreaker.formats.ne import NEFile
    nef = NEFile(module_path)
    segs = {s["idx"]: s for s in nef.segments()}
    masks = {}
    for idx, s in segs.items():
        code = nef.data[s["file_off"]:s["file_off"] + s["length"]]
        mask = bytearray(len(code))
        for r in nef.relocations(s):
            n = _SIZE.get(r["src"], 2)
            at, seen = r["offset"], 0
            while 0 <= at < len(code) and seen < 0x4000:     # the places are chained through the code
                nxt = struct.unpack_from("<H", code, at)[0] if at + 2 <= len(code) else 0xFFFF
                mask[at:at + n] = b"\1" * min(n, len(code) - at)
                if r["additive"] or nxt == 0xFFFF:
                    break
                at, seen = nxt, seen + 1
        masks[idx] = (bytes(code), mask)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    out = {}
    for va, (seg, _off) in where.items():
        if va not in spans or seg not in masks:
            continue
        (start, end), _n = spans[va]
        code, mask = masks[seg]
        piece, m = code[start:end], bytearray(mask[start:end])
        for addr, n, _mn, _ops in md.disasm_lite(piece, start):
            first = piece[addr - start]
            if first in (0xE8, 0xE9) and n == 3:             # a near call or jump: its distance
                target = (addr + 3 + struct.unpack_from("<h", piece, addr - start + 1)[0]) & 0xFFFF
                if not start <= target < end:
                    m[addr - start + 1:addr - start + 3] = b"\1\1"
        out[va] = match._trim(piece, bytes(m))
    return out


# Microsoft C 8.00c (Visual C++ 1.52c) built Setup's SUWIN, without /Gw, with switches that differ
# from one source file to the next: its segment 1 at /Od, its segment 3 at /G2 /Ox.
MSC8_FLAG_SETS = ("/AM /Gs /Od", "/AM /G2s /Ox", "/AM /G2s /Oxs", "/AM /G2s /Os", "/AM /Gs /Ox", "/AM /Gs /Oxs",
                  "/AM /Gsw /Od", "/AM /G2sw /Ox", "/AM /G3s /Ox", "/AM /G3s /Oxs")   # (/G3: push dword ptr)   # (/Oxs: SUWIN's sub_2_5CFC, a 2-byte cleanup as pop bx)


def _score(results):
    """how near a set of functions came: each counts the share of its bytes that are the original's
    (a function that does not compile, nothing), an identical one counting double"""
    return sum((2.0 if v[0] == "identical" else 1.0 - v[1] / max(v[2], 1)) if v[0] != "does not compile" else 0.0
               for v in results.values())


def run(module_path, flags=None, limit=0, log=print, compiler=None, choose=False, sdk=False, fold=True, search=0):
    """-> ({segment: (compiler, flags)}, {name: (verdict, differing bytes, total)})

    Without `flags` the compiler and switches are chosen segment by segment (a segment is a source
    file's code): those under which most of a sample of its functions come out the original's
    length, or the original's bytes."""
    from codebreaker.build import msc6
    from codebreaker.base import settings
    from codebreaker.formats.ne import NEFile
    from codebreaker.verify import win16
    compilers = [compiler] if compiler else [c for c in ("msc6", "msvc152") if msc6.available(c)]
    if not compilers or not all(msc6.available(c) for c in compilers):
        raise SystemExit("Microsoft C is not set up (settings.json: dosbox_x, msc6, msvc152)")
    name = os.path.basename(module_path).lower()
    m = win16.prepare(module_path, settings.state("match16", name, "source"))
    spans = win16._spans(NEFile(module_path), {va: m.where[va] for va in m.by_va})
    originals = original_functions(module_path, {va: m.where[va] for va in m.by_va}, spans)
    address = {fn: va for va, fn in m.by_va.items()}
    todo = [fn for fn in m.order if address.get(fn) in originals][:limit or None]
    source_header = msc6.cdecl_imports(m.header)       # (a C function imported by number: not pascal)
    header, _ = msc6.dialect(source_header, "")
    pascal = msc6.pascal_names(source_header, *m.texts.values())
    header = msc6.pascal_order(header, pascal)
    decls, header = msc6.ds_globals(header)
    header += decls                                  # (after the typedefs; the header's macros use them)
    work = settings.state("match16", name, "work")
    segment = {fn: m.where[address[fn]][0] for fn in todo}
    # Each function is said several ways -- as the decompiler wrote it, with its temporaries folded
    # back into the expressions they came from, through the SDK's prototypes -- and judged by the
    # one nearest the original: which a function's source was is the compiler's to say (a
    # temporary may be the programmer's variable, at /Od a stack slot of its own).
    stages = ["fold", "far"] if fold else []         # (each subset of these is a way of saying it)
    if sdk:
        stages.append("sdk")
    variants = {}
    for k in range(1 << len(stages)):
        on = {st for b, st in enumerate(stages) if k >> b & 1}
        variants["+".join(st for st in stages if st in on) or "plain"] = on
    protos = msc6.sdk_prototypes() if sdk else {}
    sdk_head = msc6.sdk_header(header) if sdk else header

    def said(fn, on):
        t = msc6.fold_temporaries(m.texts[fn]) if "fold" in on else m.texts[fn]
        t = msc6.join_far_parameters(t) if "far" in on else t
        t = msc6.pascal_order(t, pascal)
        return "".join(msc6.ds_globals(msc6.sdk_calls(t, protos) if "sdk" in on else t))
    texts = {v: {fn: said(fn, on) for fn in todo} for v, on in variants.items()}
    winner, code = {}, {}

    def judged(cc, fl, names, only=("plain",)):
        best = {}
        for v in only:
            how = variants[v]
            built = msc6.compile_many({fn: texts[v][fn] for fn in names}, sdk_head if "sdk" in how else header, fl, work,
                                      compiler=cc, names={fn: fn for fn in names})
            for fn in names:
                got = match.distance(built[fn], originals[address[fn]])[:3] if built.get(fn) else ("does not compile", 0, 0)
                if fn not in best or _score({0: got}) > _score({0: best[fn]}):
                    best[fn] = got
                    winner[fn] = v
                    code[fn] = built.get(fn)
        return best
    segs = sorted(set(segment.values()))
    import json
    remembered = settings.state("match16", name, "choices.json")
    known = {}
    if os.path.isfile(remembered) and not choose and not compiler:
        known = {int(s): tuple(v) for s, v in json.load(open(remembered)).items()}
    if flags:
        chosen = {s: (compilers[0], flags) for s in segs}
    elif all(s in known for s in segs):
        chosen = known                               # (found by an earlier run: --choose finds them again)
        if log:
            log("  each segment's compiler and switches as found before (%s)" % remembered)
    else:
        sample = {s: [fn for fn in todo if segment[fn] == s] for s in segs}
        sample = {s: v[::max(1, len(v) // 6)][:6] for s, v in sample.items()}
        every = [fn for v in sample.values() for fn in v]
        scores = {}
        for cc in compilers:
            for fl in (msc6.FLAG_SETS if cc == "msc6" else MSC8_FLAG_SETS):
                r = judged(cc, fl, every, tuple(v for v in ("plain", "fold") if v in variants))
                for s in segs:
                    scores[(s, cc, fl)] = _score({fn: r[fn] for fn in sample[s]})
        chosen = {}
        for s in segs:
            best = max(((scores[(s, cc, fl)], cc, fl) for (s2, cc, fl) in scores if s2 == s), key=lambda x: x[0])
            chosen[s] = best[1:]
            if log:
                log("  segment %-3s %-8s %-16s (%.1f of a sample of %d)" % (s, best[1], best[2], best[0], len(sample[s])))
        if not compiler:
            with open(remembered, "w") as f:
                json.dump({str(s): list(v) for s, v in chosen.items()}, f, indent=1)
    out = {}
    for s in segs:
        out.update(judged(chosen[s][0], chosen[s][1], [fn for fn in todo if segment[fn] == s], tuple(variants)))
    if search:                                       # the last bytes of the near misses (verify/search16.py)
        from codebreaker.verify import search16
        near = {fn: (texts[winner[fn]][fn], v, chosen[segment[fn]] + ("sdk" in variants[winner[fn]],))
                for fn, v in out.items() if v[0] in ("same size", "different") and v[1] <= 0.4 * v[2]}

        def compile_group(g, sources):
            built = msc6.compile_many(sources, sdk_head if g[2] else header, g[1], work, compiler=g[0],
                                      names={fn: fn for fn in sources})
            return {fn: match.distance(built[fn], originals[address[fn]])[:3] if built.get(fn) else
                    ("does not compile", 0, 0) for fn in sources}
        found = search16.search(near, compile_group, lambda v: _score({0: v}), search, log=log)
        for fn, (t, v) in found.items():
            if _score({0: v}) > _score({0: out[fn]}):
                out[fn] = v
                winner[fn] = "search"
                texts.setdefault("search", {})[fn] = t
    run.texts = {fn: texts[winner[fn]][fn] for fn in todo if fn in winner}
    run.winner = dict(winner)                        # (which way of saying each function came nearest)
    run.code = {fn: (code.get(fn), originals[address[fn]]) for fn in todo}   # (its bytes, and the original's)
    return chosen, out


def main(argv):
    """python -m codebreaker match <16-bit module> [--flags "..."] [--compiler msc6|msvc152] [--first N] [--choose] [--sdk] [--no-fold] [--search ROUNDS]"""
    from codebreaker.base import corpus
    flags = argv[argv.index("--flags") + 1] if "--flags" in argv else None
    compiler = argv[argv.index("--compiler") + 1] if "--compiler" in argv else None
    limit = int(argv[argv.index("--first") + 1]) if "--first" in argv else 0
    rounds = argv[argv.index("--search") + 1] if "--search" in argv else None
    args = [a for a in argv if not a.startswith("--") and a not in (flags, compiler, str(limit), rounds)]
    path = args[0] if os.path.isfile(args[0]) else corpus.retail(args[0])
    chosen, results = run(path, flags, limit, compiler=compiler, choose="--choose" in argv, sdk="--sdk" in argv, fold="--no-fold" not in argv,
                          search=int(argv[argv.index("--search") + 1]) if "--search" in argv else 0)
    counts = {}
    for v in results.values():
        counts[v[0]] = counts.get(v[0], 0) + 1
    print("%s: %d functions compiled with Microsoft C (%s): %s" % (
        os.path.basename(path), len(results), "; ".join(sorted({"%s %s" % c for c in chosen.values()})),
        ", ".join("%s %d" % (k, counts[k]) for k in ("identical", "same size", "different", "does not compile")
                  if counts.get(k))))
    won = {}
    for fn, v in getattr(run, "winner", {}).items():
        if results.get(fn, ("",))[0] == "identical":
            won[v] = won.get(v, 0) + 1
    if won:
        print("  identical by way of saying: %s" % ", ".join("%s %d" % kv for kv in sorted(won.items())))
    near = sorted((v[1] / max(v[2], 1), n, v) for n, v in results.items() if v[0] in ("same size", "different"))[:8]
    for _share, n, v in near:
        print("  %-28s %-10s %d of %d bytes differ" % (n, v[0], v[1], v[2]))
    return 0
