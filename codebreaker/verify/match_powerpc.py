"""match_powerpc.py -- `match` for a PowerPC image: does the Xbox 360 compiler turn the
reconstruction back into the original's bytes?

    python -m codebreaker match <image or .xex> [--flags "/O2"] [--try] [--json FILE]

The same optional signal match.py gives for 32-bit Windows modules, never the measure of a
reconstruction (that is whether it behaves as the original: `measure`). Each reconstructed
function is compiled on its own with the PowerPC compiler settings.json names (powerpc_tools)
and compared with the original function's bytes.

What may differ is masked at the level of bits, not bytes. Every PowerPC instruction is one
word, and the compiler's object says, as a relocation, which instructions hold something the
linker fills in -- the 24-bit distance of a `bl`, the 14 bits of a conditional branch to
another function, the 16-bit halves of an address built by `lis`/`addi` -- so only that field
of the word is ignored and its opcode and registers are still compared. The image has no such
records; the object's mask is laid over the original's function at the same places (where the
two agree that far, they agree on which words are relocated), and the original's own calls out
of the function are masked as well.

    identical     every bit outside the masked fields is the same
    same size     as many instructions as the original, some differ (registers, order, choice)
    different     another length
"""
import json
import os
import pickle
import struct
import subprocess
from concurrent.futures import ThreadPoolExecutor

REV = 3                                 # how an object is read (object_functions): part of the cache key
FLAG_SETS = ("/O2", "/Ox", "/O1", "/Os", "/O2 /Oy-", "/Od")
_BASE_FLAGS = ["/nologo", "/c", "/TC", "/GS-", "/Zl", "/Gy", "/W0", "/FIm.h"]

# IMAGE_REL_PPC_* (winnt.h) -> which bits of the 4-byte word the linker fills in
_LI, _BD, _HALF, _WORD = 0x03FFFFFC, 0x0000FFFC, 0x0000FFFF, 0xFFFFFFFF
_FIELD = {0x03: _LI, 0x06: _LI,                            # ADDR24, REL24 (b/bl)
          0x05: _BD, 0x07: _BD,                            # ADDR14, REL14 (bc)
          0x04: _HALF, 0x08: _HALF, 0x09: _HALF, 0x0F: _HALF, 0x10: _HALF, 0x11: _HALF,
          0x13: _HALF, 0x14: _HALF,                        # ADDR16, TOCREL, SECREL16, REFHI/LO ...
          0x02: _WORD, 0x0B: _WORD}                        # ADDR32, SECREL


def object_functions(obj):
    """{symbol: (bytes, [ignored bits per word])} for the functions of a PowerPC COFF object.
    (Headers little-endian, code big-endian.)"""
    machine, nsect, _ts, symptr, nsyms = struct.unpack_from("<HHIII", obj, 0)
    if machine != 0x01F2:
        raise ValueError("not a PowerPC object (machine %#x)" % machine)
    opt = struct.unpack_from("<H", obj, 16)[0]
    sects = []
    for i in range(nsect):
        p = 20 + opt + 40 * i
        rawsize, rawptr, relptr, _ln, nrel, _nln, chars = struct.unpack_from("<IIIIHHI", obj, p + 16)
        sects.append((rawptr, rawsize, relptr, nrel, chars))
    strings = symptr + 18 * nsyms
    funcs, i = {}, 0
    while i < nsyms:
        p = symptr + 18 * i
        raw, value, sect, typ, cls, naux = struct.unpack_from("<8sIhHBB", obj, p)
        if raw[:4] == b"\0\0\0\0":
            off = strings + struct.unpack_from("<I", raw, 4)[0]
            name = obj[off:obj.index(b"\0", off)].decode("latin1")
        else:
            name = raw.rstrip(b"\0").decode("latin1")
        # a function: its symbol says so (labels inside one -- $M..., class 6 -- do not)
        if sect > 0 and sects[sect - 1][4] & 0x20 and typ == 0x20 and cls != 6:
            funcs.setdefault(sect, []).append((value, name))
        i += 1 + naux
    out = {}
    for sect, lst in funcs.items():
        rawptr, rawsize, relptr, nrel, _chars = sects[sect - 1]
        mask = [0] * ((rawsize + 3) // 4)
        for k in range(nrel):
            at, _sym, kind = struct.unpack_from("<IIH", obj, relptr + 10 * k)
            if at // 4 < len(mask):
                mask[at // 4] |= _FIELD.get(kind, 0)
        lst.sort()
        for k, (value, name) in enumerate(lst):
            end = lst[k + 1][0] if k + 1 < len(lst) else rawsize
            out[name] = (obj[rawptr + value:rawptr + end], mask[value // 4:(end + 3) // 4])
    return out


def _trim(code, mask):
    """Without the zero words a compiler pads a function with."""
    n = len(code) // 4
    while n > 1 and code[4 * n - 4:4 * n] == b"\0\0\0\0":
        n -= 1
    return code[:4 * n], mask[:n]


def original_functions(img, addresses):
    """{address: (bytes, [ignored bits per word])} for functions of a PowerPC image: each runs to
    the length its .pdata record gives, else to the next function, and ends before the first zero
    word (the padding between functions -- no instruction is zero; a record's length can take in
    padding and a small function that has no record of its own); a `b`/`bl` that leaves the
    function has its distance masked (where the callee sits is the linker's choice)."""
    starts = sorted(set(img.functions) | set(img.imports) | set(img.inline))
    nxt = {a: b for a, b in zip(starts, starts[1:] + [img.code[1]])}
    out = {}
    for va in addresses:
        size = img.extents.get(va, (0,))[0] or (nxt.get(va, va) - va)
        code = img.bytes_at(va, size)
        for i in range(0, len(code) - 3, 4):
            if code[i:i + 4] == b"\0\0\0\0":
                code = code[:i]
                break
        if len(code) < 4:
            continue
        mask = []
        for i in range(len(code) // 4):
            w = struct.unpack_from(">I", code, 4 * i)[0]
            ignore = 0
            if w >> 26 == 18:                                       # b / bl
                d = w & 0x03FFFFFC
                d -= 0x04000000 if d & 0x02000000 else 0
                if not (w & 2) and not (va <= va + 4 * i + d < va + len(code)):
                    ignore = _LI
            mask.append(ignore)
        out[va] = _trim(code, mask)
    return out


def distance(built, original):
    """-> (verdict, words that differ outside masked bits, words compared, first differing word or -1)"""
    from codebreaker.compare import image
    (a, ma), (b, mb) = _trim(*built), original
    return image.distance(a, b, ma, mb, unit=4, byteorder="big")


def compile_function(cl, header, text, name, work, flags):
    """Compile one function on its own. -> (bytes, mask), or None when it does not compile.
    What the compiler made of a text is kept (base/cache.py)."""
    from codebreaker.base import cache
    k = cache.key(cache.tool(cl), "powerpc", REV, flags, header, text, name)
    had = cache.get("period", k)
    if had is not None:
        return pickle.loads(had)
    built = _compile(cl, header, text, name, work, flags)
    cache.put("period", k, pickle.dumps(built))
    return built


def _compile(cl, header, text, name, work, flags):
    os.makedirs(work, exist_ok=True)
    with open(os.path.join(work, "m.h"), "w", newline="\n", encoding="utf-8") as f:
        f.write(header)
    for ext in (".obj", ".c"):
        try:
            os.remove(os.path.join(work, "one" + ext))
        except OSError:
            pass
    with open(os.path.join(work, "one.c"), "w", newline="\n", encoding="utf-8") as f:
        f.write(text + "\n")
    r = subprocess.run([cl] + _BASE_FLAGS + flags.split() + ["/Foone.obj", "one.c"], cwd=work,
                       capture_output=True, text=True, errors="replace", timeout=600)
    if r.returncode or not os.path.isfile(os.path.join(work, "one.obj")):
        return None
    funcs = object_functions(open(os.path.join(work, "one.obj"), "rb").read())
    return funcs.get(name) or (next(iter(funcs.values())) if len(funcs) == 1 else None)


def run(module_path, flags, work, jobs=4):
    """Compile every reconstructed function of the image and compare.
    -> {name: (verdict, words differing, words, original words)}"""
    from codebreaker.build import functions
    from codebreaker.verify import powerpc
    tools = functions.powerpc_tools()
    if not tools:
        raise SystemExit("no PowerPC compiler (settings.json: powerpc_tools, powerpc_lib)")
    cl = tools[0]
    m = powerpc.prepare(module_path, os.path.join(work, "source"))
    address = {name: va for va, name in m.by_va.items()}
    originals = original_functions(m.image, sorted(address.values()))
    names = list(m.order)

    def one(i):
        return compile_function(cl, m.header, m.texts[names[i]], names[i], os.path.join(work, "w%d" % (i % jobs)), flags)
    by_slot = {}
    for i in range(len(names)):
        by_slot.setdefault(i % jobs, []).append(i)
    built = {}

    def slot(ids):                                           # one working folder per worker
        for i in ids:
            built[names[i]] = one(i)
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        list(ex.map(slot, by_slot.values()))
    results = {}
    for name in names:
        orig = originals.get(address.get(name))
        if built[name] is None or orig is None:
            results[name] = ("does not compile" if built[name] is None else "no original extent", 0, 0,
                             len(orig[0]) // 4 if orig else 0)
            continue
        verdict, diff, words, _first = distance(built[name], orig)
        results[name] = (verdict, diff, words, len(orig[0]) // 4)
    return results


_ORDER = ("identical", "same size", "different", "no original extent", "does not compile")


def main(argv):
    from codebreaker.base import settings
    from codebreaker.formats import xex
    args = [a for a in argv if not a.startswith("--")]
    flags = argv[argv.index("--flags") + 1] if "--flags" in argv else FLAG_SETS[0]
    out_json = argv[argv.index("--json") + 1] if "--json" in argv else None
    args = [a for a in args if a not in (flags, out_json)]
    path = xex.basefile(args[0]) if args[0].lower().endswith(".xex") else args[0]
    work = settings.state("match", os.path.basename(path).lower())
    sets = FLAG_SETS if "--try" in argv else (flags,)
    best = {}
    for fl in sets:
        r = run(path, fl, work)
        if len(sets) > 1:
            print("  %-12s %d identical" % (fl, sum(1 for v in r.values() if v[0] == "identical")), flush=True)
        for name, v in r.items():
            rank = (_ORDER.index(v[0]), v[1])
            if name not in best or rank < best[name][0]:
                best[name] = (rank, v, fl)
    tally = {}
    for _rank, v, _fl in best.values():
        tally[v[0]] = tally.get(v[0], 0) + 1
    print("%s: %d functions" % (os.path.basename(args[0]), len(best)))
    for verdict in _ORDER:
        if tally.get(verdict):
            print("  %-20s %d" % (verdict, tally[verdict]))
    if out_json:
        with open(out_json, "w", encoding="utf-8") as f:
            json.dump({n: {"verdict": v[0], "words differing": v[1], "words": v[2], "original words": v[3],
                           "flags": fl} for n, (_r, v, fl) in sorted(best.items())}, f, indent=1)
    return 0
