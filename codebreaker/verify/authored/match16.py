"""match16.py -- does Microsoft C 6 turn a 16-bit module's reconstruction back into its bytes?

    python -m codebreaker match <16-bit module> [--flags "/AM /Gcsw /Os"] [--first N]

The 16-bit counterpart of match.py (see there for what the comparison is and is not). Each
reconstructed function is compiled by Microsoft C 6.0a (build/msc6.py: the real compiler, in an
emulator, a whole module's functions in one session) and its bytes are compared with the
original function's, outside the places a loader or linker fills in: in the original, every
word a relocation record of its segment names and the distance of every near call and jump to
another function; in the compiled function, every place the object file has a fix-up for. (What
is 16-bit about that is match.py's ARCH["x86-16"]; this is the driver.)

Without --flags the switches are found first: the memory model and options under which most of
a sample of the functions come out the original's length say how the module was built.
"""
import os

from codebreaker.verify.authored import match
from codebreaker import platforms
from codebreaker.verify import verdicts as V

def source_files(names, where, originals, address):
    """[[function, ...], ...] -- the original's source files, as far as its calls tell: a far call
    to a function as `push cs; call near` (the segment known: defined in the same file) or a near
    call, puts caller and callee in one file, and a file is contiguous, so everything between them
    too. A function no such call reaches is a file of its own."""
    md = platforms.decoder(16)
    at = {where[address[fn]]: fn for fn in names}
    by_seg = {}
    for fn in names:
        by_seg.setdefault(where[address[fn]][0], []).append(fn)
    files = []
    for seg, fns in by_seg.items():
        fns.sort(key=lambda f: where[address[f]][1])
        index = {f: i for i, f in enumerate(fns)}
        joined = [False] * len(fns)                  # joined[i]: fns[i] and fns[i + 1] in one file
        for fn in fns:
            code = bytes(originals[address[fn]][0])
            start = where[address[fn]][1]
            ins = list(md.disasm(code, 0))
            for k, i in enumerate(ins):
                if i.mnemonic != "call" or not i.op_str.startswith("0x") or "," in i.op_str:
                    continue                         # (a direct near call: `push cs; call` or a near callee)
                target = (seg, (start + int(i.op_str, 16)) & 0xFFFF)
                other = at.get(target)
                if other is None or other == fn:
                    continue
                a, b = sorted((index[fn], index[other]))
                for j in range(a, b):
                    joined[j] = True
        cur = [fns[0]]
        for j in range(1, len(fns)):
            if joined[j - 1]:
                cur.append(fns[j])
            else:
                files.append(cur)
                cur = [fns[j]]
        files.append(cur)
    return files


def _score(results):
    """how near a set of functions came: each counts the share of its bytes that are the original's
    (a function that does not compile, nothing), an identical one counting double"""
    return sum((2.0 if v[0] == V.IDENTICAL else 1.0 - v[1] / max(v[2], 1)) if v[0] != V.NO_COMPILE else 0.0
               for v in results.values())


def run(module_path, flags=None, limit=0, log=print, compiler=None, choose=False, sdk=False, fold=True, search=0, wine=False):
    """-> ({segment: (compiler, flags)}, {name: (verdict, differing bytes, total)})

    Without `flags` the compiler and switches are chosen segment by segment (a segment is a source
    file's code): those under which most of a sample of its functions come out the original's
    length, or the original's bytes."""
    from codebreaker.build import msc6
    from codebreaker.base import settings
    from codebreaker.formats.ne import NEFile
    from codebreaker.verify.lanes import win16
    # Microsoft C 8.00c built Windows 95's Setup modules and runs on Windows itself; Microsoft C 6.0a
    # (DOS, the emulator) is tried only when asked for (settings: match16_msc6 = on) -- choosing
    # SUWIN's switches among both gave nothing more than among C 8's alone (35 either way).
    wanted = ("msc6", "msvc152") if str(settings.get("match16_msc6") or "").lower() in ("on", "1", "true", "yes")         else ("msvc152",)
    compilers = [compiler] if compiler else [c for c in wanted if msc6.available(c)]
    if not compilers or not all(msc6.available(c) for c in compilers):
        raise SystemExit("Microsoft C is not set up (settings.json: dosbox_x, msc6, msvc152)")
    name = os.path.basename(module_path).lower()
    m = win16.prepare(module_path, settings.state("match16", name, "source"))
    spans = win16._spans(NEFile(module_path), {va: m.where[va] for va in m.by_va})
    originals = match.ARCH["x86-16"].originals(module_path, {va: m.where[va] for va in m.by_va}, spans)
    address = {fn: va for va, fn in m.by_va.items()}
    todo = [fn for fn in m.order if address.get(fn) in originals][:limit or None]
    source_header = msc6.cdecl_imports(m.header)       # (a C function imported by number: not pascal)
    header, _ = msc6.dialect(source_header, "")
    pascal = msc6.pascal_names(source_header, *m.texts.values())
    header = msc6.pascal_order(header, pascal)
    decls, header = msc6.ds_globals(header)
    header += decls                                  # (after the typedefs; the header's macros use them)
    run.header = header
    work = settings.state("match16", name, "work")
    segment = {fn: m.where[address[fn]][0] for fn in todo}
    files = source_files(todo, m.where, originals, address)   # (compiled together, one setting each)
    file_of = {fn: k for k, f in enumerate(files) for fn in f}
    # Each function is said several ways -- as the decompiler wrote it, with its temporaries folded
    # back into the expressions they came from, through the SDK's prototypes -- and judged by the
    # one nearest the original: which a function's source was is the compiler's to say (a
    # temporary may be the programmer's variable, at /Od a stack slot of its own).
    stages = ["fold", "far", "frame"] if fold else []   # (each subset of these is a way of saying it)
    if sdk:
        stages.append("sdk")
    if wine:
        stages.append("wine")                        # (imports typed by Wine's .spec: symbols/wine16.py)
    variants = {}
    for k in range(1 << len(stages)):
        on = {st for b, st in enumerate(stages) if k >> b & 1}
        variants["+".join(st for st in stages if st in on) or "plain"] = on
    protos = msc6.sdk_prototypes() if sdk else {}
    sdk_head = msc6.sdk_header(header) if sdk else header

    from codebreaker.symbols import wine16
    own = {e.name.upper(): e for e in wine16.exports(name.split(".")[0]).values()} if wine else {}

    def said(fn, on):
        t = m.texts[fn]
        if "wine" in on and fn.upper() in own:        # (the module's own export, typed by Wine's .spec)
            t = msc6.wine_definition(t, own[fn.upper()])
        t = msc6.fold_temporaries(t) if "fold" in on else t
        t = msc6.join_far_parameters(t) if "far" in on else t
        t = msc6.split_frame(t) if "frame" in on else t
        t = msc6.pascal_order(t, pascal)
        return "".join(msc6.ds_globals(msc6.sdk_calls(t, protos) if "sdk" in on else t))
    texts = {v: {fn: said(fn, on) for fn in todo} for v, on in variants.items()}
    heads = {}
    for v, on in variants.items():
        h = header
        if "wine" in on:
            h, texts[v] = msc6.wine_typing(header, texts[v])
        heads[v] = msc6.sdk_header(h) if "sdk" in on else h
    winner, code = {}, {}

    def judged(cc, fl, names, only=("plain",)):
        best = {}
        for v in only:
            how = variants[v]
            built = msc6.compile_many({fn: texts[v][fn] for fn in names}, heads[v], fl, work,
                                      compiler=cc, names={fn: fn for fn in names}, together=files)
            for fn in names:
                got = match.distance(built[fn], originals[address[fn]])[:3] if built.get(fn) else (V.NO_COMPILE, 0, 0)
                if fn not in best or _score({0: got}) > _score({0: best[fn]}):
                    best[fn] = got
                    winner[fn] = v
                    code[fn] = built.get(fn)
        return best
    segs = sorted(set(segment.values()))
    import json
    remembered = settings.state("match16", name, "choices.json")
    known, each = {}, {}
    if os.path.isfile(remembered) and not choose and not compiler:
        saved = json.load(open(remembered))
        if "segments" not in saved:
            saved = {"segments": saved}
        known = {int(s): tuple(v) for s, v in saved["segments"].items()}
        each = {fn: tuple(v) for fn, v in saved.get("functions", {}).items()}
    if flags:
        chosen = {s: (compilers[0], flags) for s in segs}
    elif all(s in known for s in segs):
        chosen = known                               # (found by an earlier run: --choose finds them again)
        if log:
            log("  each segment's and function's compiler and switches as found before (%s)" % remembered)
    else:
        sample = {s: [fn for fn in todo if segment[fn] == s] for s in segs}
        sample = {s: v[::max(1, len(v) // 6)][:6] for s, v in sample.items()}
        every = [fn for v in sample.values() for fn in v]
        scores = {}
        for cc in compilers:
            for fl in (msc6.FLAG_SETS if cc == "msc6" else match.ARCH["x86-16"].flag_sets):
                r = judged(cc, fl, every, tuple(v for v in ("plain", "fold", "frame", "fold+frame") if v in variants))
                for s in segs:
                    scores[(s, cc, fl)] = _score({fn: r[fn] for fn in sample[s]})
        chosen, top = {}, {}
        for s in segs:
            ranked = sorted(((scores[(s, cc, fl)], cc, fl) for (s2, cc, fl) in scores if s2 == s), key=lambda x: -x[0])
            chosen[s] = ranked[0][1:]
            top[s] = [r[1:] for r in ranked[:3]]
            if log:
                log("  segment %-3s %-8s %-16s (%.1f of a sample of %d)" % (s, ranked[0][1], ranked[0][2], ranked[0][0],
                                                                          len(sample[s])))
        # A segment of the file can be several source files' code (the linker packs them), each built
        # with its own switches: every function is judged under its segment's three best, and keeps
        # the one it comes nearest under (its segment's first where none is nearer).
        each = {}
        tried = {}
        for cc, fl in sorted({c for v in top.values() for c in v}):
            names = [fn for fn in todo if (cc, fl) in top[segment[fn]]]
            for fn, v in judged(cc, fl, names, ("plain",)).items():
                tried.setdefault(fn, []).append((_score({0: v}), (cc, fl) == chosen[segment[fn]], cc, fl))
        for f in files:                              # one file, one command line: the setting its functions
            total = {}                               # come nearest under together
            for fn in f:
                for sc, first, cc, fl in tried.get(fn, ()):
                    total[(cc, fl)] = total.get((cc, fl), 0) + sc + (1e-6 if first else 0)
            if not total:
                continue
            best = max(total, key=total.get)
            for fn in f:
                if best != chosen[segment[fn]]:
                    each[fn] = best
        if log:
            log("  %d functions nearer under another of their segment's switches than its first" % len(each))
        if not compiler:
            with open(remembered, "w") as f:
                json.dump({"segments": {str(s): list(v) for s, v in chosen.items()},
                           "functions": {fn: list(v) for fn, v in sorted(each.items())}}, f, indent=1)
    setting = {fn: each.get(fn, chosen[segment[fn]]) for fn in todo}
    for f in files:                                  # (one setting a file: its first function's, where they differ)
        votes = [setting[fn] for fn in f]
        one = max(votes, key=votes.count)
        for fn in f:
            setting[fn] = one
    out = {}
    for cc, fl in sorted(set(setting.values())):
        out.update(judged(cc, fl, [fn for fn in todo if setting[fn] == (cc, fl)], tuple(variants)))
    from codebreaker.verify.gate16 import runtime16         # (the compiler's run-time library: linked, not written)
    run.library = runtime16.identify({fn: originals[address[fn]] for fn in todo})
    for fn, lib in run.library.items():
        if out.get(fn, ("",))[0] != V.IDENTICAL:
            out[fn] = (V.IDENTIFIED, 0, len(originals[address[fn]][0]))
    if search:                                       # the last bytes of the near misses (verify/authored/search16.py)
        from codebreaker.verify.authored import search16
        near = {fn: (texts[winner[fn]][fn], v, setting[fn] + (winner[fn],))
                for fn, v in out.items() if v[0] in (V.SAME_SIZE, V.DIFFERENT) and v[1] <= 0.4 * v[2]}

        def compile_group(g, sources):
            mates = {fn: texts[winner[fn]][fn] for k in {file_of[s] for s in sources} for fn in files[k]
                     if fn not in sources and fn in winner}   # (a rewrite is judged in its file)
            built = msc6.compile_many(dict(mates, **sources), heads.get(g[2], header), g[1], work, compiler=g[0],
                                      names={fn: fn for fn in list(sources) + list(mates)}, together=files)
            return {fn: match.distance(built[fn], originals[address[fn]])[:3] if built.get(fn) else
                    (V.NO_COMPILE, 0, 0) for fn in sources}
        found = search16.search(near, compile_group, lambda v: _score({0: v}), search, log=log)
        for fn, (t, v) in found.items():
            if _score({0: v}) > _score({0: out[fn]}):
                out[fn] = v
                winner[fn] = "search"
                texts.setdefault("search", {})[fn] = t
    run.texts = {fn: texts[winner[fn]][fn] for fn in todo if fn in winner}
    run.settings = set(setting.values())
    run.setting, run.files, run.segment = dict(setting), files, dict(segment)   # (for scoreboard16)
    run.winner = dict(winner)                        # (which way of saying each function came nearest)
    run.code = {fn: (code.get(fn), originals[address[fn]]) for fn in todo}   # (its bytes, and the original's)
    return chosen, out


def main(argv):
    """python -m codebreaker match <16-bit module> [--flags "..."] [--compiler msc6|msvc152] [--first N] [--choose] [--sdk] [--no-fold] [--search ROUNDS] [--wine]"""
    from codebreaker.base import corpus
    flags = argv[argv.index("--flags") + 1] if "--flags" in argv else None
    compiler = argv[argv.index("--compiler") + 1] if "--compiler" in argv else None
    limit = int(argv[argv.index("--first") + 1]) if "--first" in argv else 0
    rounds = argv[argv.index("--search") + 1] if "--search" in argv else None
    args = [a for a in argv if not a.startswith("--") and a not in (flags, compiler, str(limit), rounds)]
    path = args[0] if os.path.isfile(args[0]) else corpus.retail(args[0])
    chosen, results = run(path, flags, limit, compiler=compiler, choose="--choose" in argv, sdk="--sdk" in argv, fold="--no-fold" not in argv,
                          search=int(argv[argv.index("--search") + 1]) if "--search" in argv else 0,
                          wine="--wine" in argv)
    counts = {}
    for v in results.values():
        counts[v[0]] = counts.get(v[0], 0) + 1
    print("%s: %d functions compiled with Microsoft C (%s): %s" % (
        os.path.basename(path), len(results), "; ".join(sorted({"%s %s" % c for c in getattr(run, "settings", chosen.values())})),
        ", ".join("%s %d" % (V.MATCH_LABEL[k], counts[k])
                  for k in (V.IDENTICAL, V.IDENTIFIED, V.SAME_SIZE, V.DIFFERENT, V.NO_COMPILE) if counts.get(k))))
    won = {}
    for fn, v in getattr(run, "winner", {}).items():
        if results.get(fn, ("",))[0] == V.IDENTICAL:
            won[v] = won.get(v, 0) + 1
    if won:
        print("  identical by way of saying: %s" % ", ".join("%s %d" % kv for kv in sorted(won.items())))
    near = sorted((v[1] / max(v[2], 1), n, v) for n, v in results.items() if v[0] in (V.SAME_SIZE, V.DIFFERENT))[:8]
    for _share, n, v in near:
        print("  %-28s %-10s %d of %d bytes differ" % (n, V.MATCH_LABEL[v[0]], v[1], v[2]))
    return 0
