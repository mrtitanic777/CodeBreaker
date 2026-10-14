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


def run(module_path, flags=None, limit=0, log=print):
    """-> (flags used, {name: (verdict, differing bytes, total)})"""
    from codebreaker.build import msc6
    from codebreaker.base import settings
    from codebreaker.formats.ne import NEFile
    from codebreaker.verify import win16
    if not msc6.available():
        raise SystemExit("Microsoft C 6 is not set up (settings.json: dosbox_x, msc6)")
    name = os.path.basename(module_path).lower()
    m = win16.prepare(module_path, settings.state("match16", name, "source"))
    spans = win16._spans(NEFile(module_path), {va: m.where[va] for va in m.by_va})
    originals = original_functions(module_path, {va: m.where[va] for va in m.by_va}, spans)
    address = {fn: va for va, fn in m.by_va.items()}
    todo = [fn for fn in m.order if address.get(fn) in originals][:limit or None]
    header, _ = msc6.dialect(m.header, "")
    work = settings.state("match16", name, "work")

    def judged(fl, names):
        built = msc6.compile_many({fn: m.texts[fn] for fn in names}, header, fl, work)
        return {fn: (match.distance(built[fn], originals[address[fn]])[:3] if built.get(fn) else ("does not compile", 0, 0))
                for fn in names}
    if not flags:
        sample = todo[::max(1, len(todo) // 40)][:40]
        best = (-1, msc6.FLAG_SETS[0])
        for fl in msc6.FLAG_SETS:
            r = judged(fl, sample)
            score = sum(3 if v[0] == "identical" else 1 if v[0] == "same size" else 0 for v in r.values())
            if log:
                log("  %-18s %d identical, %d the same size, of %d" % (
                    fl, sum(v[0] == "identical" for v in r.values()), sum(v[0] == "same size" for v in r.values()), len(sample)))
            if score > best[0]:
                best = (score, fl)
        flags = best[1]
    return flags, judged(flags, todo)


def main(argv):
    """python -m codebreaker match <16-bit module> [--flags "..."] [--first N]"""
    from codebreaker.base import corpus
    flags = argv[argv.index("--flags") + 1] if "--flags" in argv else None
    limit = int(argv[argv.index("--first") + 1]) if "--first" in argv else 0
    args = [a for a in argv if not a.startswith("--") and a != flags and a != str(limit)]
    path = args[0] if os.path.isfile(args[0]) else corpus.retail(args[0])
    flags, results = run(path, flags, limit)
    counts = {}
    for v in results.values():
        counts[v[0]] = counts.get(v[0], 0) + 1
    print("%s: %d functions compiled with Microsoft C 6 (%s): %s" % (
        os.path.basename(path), len(results), flags, ", ".join("%s %d" % (k, counts[k]) for k in (
            "identical", "same size", "different", "does not compile") if counts.get(k))))
    near = sorted((v[1] / max(v[2], 1), n, v) for n, v in results.items() if v[0] in ("same size", "different"))[:8]
    for _share, n, v in near:
        print("  %-28s %-10s %d of %d bytes differ" % (n, v[0], v[1], v[2]))
    return 0
