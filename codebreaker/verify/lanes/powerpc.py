"""powerpc.py -- what is particular to reconstructing a PowerPC image.

One of the kinds reconstruct.py works with (see there for what a kind provides). The image is
decompiled and made compilable by decompile/c_text.py, every function is compiled with a
PowerPC compiler and one image linked by build/functions.py, and each function is run beside
the original in the PowerPC interpreter (emulate/powerpc.py).

Both sides get the same eight argument registers. The rebuilt image is placed beside the
original in the same memory, so a global is the same storage for both and a call through a
pointer the original holds still lands in the original. Compared: how the run ends, the calls
to functions outside the image in order, the result in r3 (not for a function that returns
nothing), the memory left changed outside the stack, and whether r14-r31 and the stack pointer
were handed back.

How many arguments a function outside the image takes is not stated anywhere in a PowerPC
image. Where every call to it in the module passes the same number, that many are compared;
otherwise the call is compared by name only and a match involving it is marked weak. An
argument that points into either image's data is compared by what it points at, since the
rebuilt image keeps its own copy of a constant.
"""
import hashlib

from codebreaker.base import corpus
from codebreaker.verify import compare
from codebreaker.verify import lane_common

REV = 4                                 # 4: branch-guided inputs (verify/guided.py); calls into absent code
ARGS = 8
M32 = 0xFFFFFFFF
_OFFSET = 0x01000000                    # the rebuilt image sits this far above the original


def vectors():
    """The inputs every function is run on: eight argument registers and what calls outside the
    image return. Memory an argument points at reads as zeros unless the image put something
    there, on both sides alike."""
    from codebreaker.emulate import powerpc
    a = powerpc.ARGS_BASE
    return [{"args": [0] * ARGS}, {"args": lane_common.counting(ARGS)},
            {"args": lane_common.counting(ARGS, a, 0x1000)},
            {"args": lane_common.counting(ARGS, a, 0x1000), "stub_ret": 1},
            {"args": lane_common.counting(ARGS, 0x100, 0x100), "stub_ret": 0xFFFFFFFFFFFFFFFF},
            {"args": lane_common.alternating(ARGS, lambda i: a + 0x800 * i, lambda i: 0x20 * (i + 1))},
            {"args": lane_common.alternating(ARGS, lambda i: 0x20 * (i + 1), lambda i: a + 0x800 * i), "stub_ret": 1},
            {"args": lane_common.descending(ARGS, 0xFFFFFFFFFFFFFFFF)},
            {"args": [a, 3, a + 0x4000, 0x40, a + 0x8000, 1, 0, 2], "stub_ret": 0x103},
            {"args": lane_common.counting(ARGS, a, 0x1000), "fill": "pointers"},
            {"args": lane_common.counting(ARGS, a, 0x1000), "fill": "counts", "stub_ret": 1},
            {"args": lane_common.counting(ARGS, a, 0x1000), "fill": "text"},
            {"args": [a + 0x40, 5, a + 0x2000, 2, 1, a + 0x3000, 0, 9], "fill": "counts"},
            {"args": [7, a + 0x100, 0x30, a + 0x5000, a + 0x200, 4, 3, 1], "fill": "pointers", "stub_ret": 2}]


_DIRECTED = {}


def directed(image, span, most=6):
    """Inputs aimed at what the function's own code tests: the constants it compares values
    with, given in each argument register, as what calls outside return, and as what the memory
    behind a pointer holds."""
    if span not in _DIRECTED:
        from codebreaker.emulate import powerpc
        seen = []
        for va in range(span[0], span[1], 4):
            w = image.word(va) or 0
            if (w >> 26) in (10, 11):                        # cmpli, cmpi
                c = w & 0xFFFF
                if (w >> 26) == 11 and c & 0x8000:
                    c |= 0xFFFFFFFFFFFF0000
                if c not in seen and c not in (0, 1):
                    seen.append(c)
        a, out = powerpc.ARGS_BASE, []
        for c in seen[:most]:
            out.append({"args": [c] * ARGS, "stub_ret": c})
            out.append({"args": lane_common.alternating(ARGS, lambda i: a + 0x1000 * i, c), "stub_ret": c, "fill": "counts"})
        _DIRECTED[span] = out
    return _DIRECTED[span]


def guided_vectors(shared, va, span, externs, seeds, absent=None):
    """Inputs that take branches of the original no seed took (verify/guided.py)."""
    from codebreaker.emulate import powerpc

    def run(v):
        try:
            r = powerpc.run(v.get("mem_init") or (), va, v["args"], externs=externs, span=span,
                            stub_ret=v.get("stub_ret", 0), shared=shared, fill=v.get("fill"), branches=True,
                            absent=absent)
        except Exception:
            return None
        return {"branches": r["branches"], "reach": r["path"], "args_at": None,
                "stack": (powerpc.STACK_LOW, powerpc.STACK_TOP)}
    return lane_common.guided(run, seeds, va & 0xFFFF, byteorder="big", word=8)


def directed_and_guided(module_path, shared, va, span, externs, absent=None):
    from codebreaker.formats import powerpc_image
    d = directed(powerpc_image.read(module_path), span)
    return list(d) + guided_vectors(shared, va, span, externs, vectors() + list(d), absent)


def _argument(machine, v, images):
    """One argument of a recorded call, written as what it is rather than its bit pattern where
    the two builds would legitimately differ: an address in the stack, or what an address in
    either image points at; otherwise as lane_common.argument writes it."""
    from codebreaker.emulate import powerpc
    x = v & M32
    if powerpc.STACK_LOW <= x < powerpc.STACK_TOP:
        return "stack"
    for lo, hi in images:
        if lo <= x < hi:
            return "data:" + bytes(machine.read(x + i, 1) for i in range(12)).hex()
    return lane_common.argument(x)


def fingerprint(shared, entry, v, externs, argc, images, span=None, novalue=False):
    """What one run of one function did, in a form two builds can be compared on."""
    from codebreaker.emulate import powerpc
    try:
        r = powerpc.run(v.get("mem_init") or (), entry, v["args"], externs=externs, span=span,
                        stub_ret=v.get("stub_ret", 0), shared=shared, fill=v.get("fill"), absent=images[0])
    except Exception as e:
        return {"err": type(e).__name__}
    ex = compare._exit_norm(r["exit"])
    m = r["machine"]
    deps = list(lane_common.calls([(name, None if argc.get(name) is None else args[:argc[name]])
                                   for name, args in r["calls"]], lambda x: _argument(m, x, images)))
    if r["truncated"] and len(r["calls"]) >= powerpc.MAX_CALLS:
        deps.append("...")
    lo_r, hi_r = images[-1]                                  # the rebuilt image, when there is one
    writes = lane_common.writes({a: new for a, (_old, new) in r["writes"].items()
                                 if not (len(images) > 1 and lo_r <= a < hi_r)})
    lo = span[0] if span else 0
    return {"ret": None if (novalue or ex != "ret") else r["ret"] & M32, "exit": ex, "truncated": bool(r["truncated"]),
            "deps": tuple(deps), "writes": writes, "pop": None, "saved": r["saved"] if ex == "ret" else None,
            "cov": tuple(sorted(p - lo for p in r["path"])), "reach": ()}


_SHARED = {}


def _memory(module_path, dll):
    """(pages of the original alone, pages of original + rebuilt, [(lo, hi)] of each image)"""
    key = (module_path, dll)
    if key not in _SHARED:
        from codebreaker.emulate import powerpc
        from codebreaker.formats import powerpc_image
        a = powerpc_image.regions(module_path)
        b = powerpc_image.regions(dll)

        def extent(regs):
            return min(va for va, _d in regs), max(va + len(d) for va, d in regs)
        _SHARED.clear()
        _SHARED[key] = (powerpc.pages(a), powerpc.pages(a + b), [extent(a), extent(b)])
    return _SHARED[key]


def _judge(context, item):
    """Worker. item = (key, address, span, instruction count, rebuilt address, returns nothing)."""
    module_path, dll, ext_original, ext_rebuilt, argc = context
    _key, va, span, insns, rebuilt_va, novalue = item
    sha = corpus.sha(module_path)

    def compare_one(info):
        alone, both, images = _memory(module_path, dll)
        merged = {**ext_original, **ext_rebuilt}

        def original(v):
            # the original's run depends on the original, and on what the build says of the calls
            # out of it (their names, how many arguments each takes): remembered across rebuilds
            key = compare.cache_key(KIND, REV, sha, va, span, novalue, sorted(v.items()),
                                    sorted(ext_original.items()), sorted(argc.items())) if sha else None
            return lane_common.remembered(key, lambda: fingerprint(alone, va, v, ext_original, argc, images[:1],
                                                                   span=span, novalue=novalue))
        return compare.compare(
            original,
            lambda v: fingerprint(both, rebuilt_va, v, merged, argc, images, novalue=novalue),
            vectors(), (), insns=insns, info=info,
            directed=lambda covered, reached: directed_and_guided(module_path, alone, va, span, ext_original,
                                                                  images[0]))
    return lane_common.judged(compare_one)


KIND = "powerpc"
PER_FUNCTION = True             # a verdict holds while the function and what it reaches are unchanged


def profile(module_path):
    """(digest, settings) of everything other than the source that a verdict depends on."""
    from codebreaker.build import functions
    settings = repr(("powerpc", REV, compare.REV, functions.CFLAGS_POWERPC, vectors()))
    return hashlib.sha1(settings.encode()).hexdigest()[:12], settings


def prepare(module_path, work):
    from codebreaker.decompile import c_text
    m = c_text.compilable_powerpc(module_path, work)
    m.key = {name: "%08x" % va for va, name in m.by_va.items()}
    m.all_keys = sorted("%08x" % va for va in m.image.functions)
    m.sig = hashlib.sha1((m.header + "".join(m.texts[n] for n in m.order)).encode("utf-8", "replace")).hexdigest()
    return m


def build(m, module_path, work):
    """-> (context for judge, {name: item}, {name: why it is not in the build})"""
    from codebreaker.build import functions
    img = m.image
    top = max(va + size for va, _off, size in img.spans)
    base = (img.base + max(_OFFSET, (top - img.base + 0xFFFF) & ~0xFFFF)) & 0xFFFF0000
    dll, placed, dropped = functions.build_powerpc(m.header, m.texts, m.order, sorted(m.externs), work, base)
    starts = sorted(set(img.functions) | set(img.imports) | set(img.run_names))
    nxt = {a: b for a, b in zip(starts, starts[1:] + [img.code[1]])}
    items, dropped = {}, dict(dropped)
    for va, fn in m.by_va.items():
        if fn in dropped:
            continue
        if fn not in placed:
            dropped[fn] = "not in the build"
            continue
        span = (va, nxt.get(va, va + 4))
        items[fn] = (m.key[fn], va, span, max((span[1] - span[0]) // 4, 1), placed[fn], fn in m.novalue)
    ext_original = {va: name for name, va in m.externs.items()}
    ext_original.update({va: name for va, name in img.imports.items() if va not in ext_original})
    ext_rebuilt = {placed[name]: name for name in m.externs if name in placed}
    # Calls to code the image does not hold (func_0x<address>: a partial image calling into the
    # rest of its title) are recorded by the interpreter as calls to "@<address>"; their argument
    # counts, where every call in the C agrees, from the C as for any function outside.
    import re
    from codebreaker.decompile import c_text
    absent = {"func_0x" + h for h in re.findall(r"\bfunc_0x([0-9a-f]{8})\s*\(", "\n".join(m.texts.values()))}
    argc = dict(m.argc)
    argc.update({"@" + n[len("func_0x"):]: k for n, k in c_text.call_counts(m.texts, absent).items()})
    return (module_path, dll, ext_original, ext_rebuilt, argc), items, dropped


judge = _judge
