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

The interpreter, the convention, the compiler's options and the build of the rebuilt image are
the platform's (codebreaker/platforms/xbox360.py): its emulator, convention, compiler, packager.
"""
import hashlib

from codebreaker import platforms
from codebreaker.base import corpus
from codebreaker.verify import compare
from codebreaker.verify import lane_common

P = platforms.of("powerpc")             # the Xbox 360: the six slots this lane asks
REV = 5                                 # 4: branch-guided inputs (verify/guided.py); calls into absent code;
                                        # 5: those found path by path (explore_vectors: ShadowPPC, verify/concolic.py),
                                        # a verdict's limits saying paths=N / paths-all
ARGS = P.convention.arguments           # r3-r10
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
            r = P.emulator.run(v.get("mem_init") or (), va, v["args"], externs=externs, span=span,
                               stub_ret=v.get("stub_ret", 0), shared=shared, fill=v.get("fill"), branches=True,
                               absent=absent)
        except Exception:
            return None
        return {"branches": r.raw["branches"], "reach": r.path, "args_at": None,
                "stack": (powerpc.STACK_LOW, powerpc.STACK_TOP)}
    return lane_common.guided(run, seeds, va & 0xFFFF, byteorder="big", word=8)


def directed_and_guided(module_path, shared, va, span, externs, absent=None, found=None):
    """The directed inputs, then those found path by path (or guessed). `found`: a dict the
    exploration is left in (["got"]: its concolic.Exploration)."""
    from codebreaker.formats import powerpc_image
    d = directed(powerpc_image.read(module_path), span)
    seeds = vectors() + list(d)
    if concolic_ready():
        kept, got = explore_vectors(shared, va, span, externs, seeds, absent, _writable(module_path))
        if found is not None:
            found["got"] = got
        return list(d) + kept
    return list(d) + guided_vectors(shared, va, span, externs, seeds, absent)


def _mode():
    """The lift mode of the platform's processor (emulate/shadow.py mode: its cpu slot)."""
    from codebreaker.emulate import shadow
    return shadow.mode(P.cpu)


# Path by path (verify/concolic.py), as the x86 judges find them: the original run with its
# symbolic shadow (emulate/shadow.py ShadowPPC, on the decompiler's ppc_64_xenon_be p-code) -- the
# eight argument registers, the bytes of the memory pointer arguments point into (ARGS_BASE) and
# of the image's writable sections (its globals), and what a call out returns are the inputs --
# and inputs solved for to take each path its conditions allow. Where the lifter is not there or
# CB_NO_CONCOLIC is set, the inputs are guessed as before.
def concolic_ready():
    from codebreaker.emulate import pcode
    return lane_common.guided_on() and lane_common.concolic_on() and pcode.lifter(_mode()) is not None


# A question over 64-bit registers is a bigger one for the solver than the 16-bit lane's: those it
# cannot answer cost it four times as long before it gives up (a C library's number parsing: 52 of
# 256 questions). A quarter of concolic.SOLVER_STEPS -- still z3's own steps, the same on any machine.
SOLVER_STEPS = 500000


def _derive(v, values, names):
    """Input `v` with the named inputs (shadow.vars) set to `values`: an argument register, what a
    call out returns, bytes of memory -- behind the pointers given as whole pages, the input's
    `fill` under them (a page an input puts memory in is made from the image and that alone)."""
    from codebreaker.emulate import powerpc
    out = dict(v)
    args = list(v["args"])
    mem = {}
    for at, data in v.get("mem_init") or ():
        for k, b in enumerate(bytes(data)):
            mem[at + k] = b
    changed = False
    for name, value in values.items():
        kind, where, bits, was = names.get(name, (None, None, 0, None))
        if kind == "arg" and where < len(args):
            changed |= args[where] != value & powerpc.M64
            args[where] = value & powerpc.M64
        elif kind == "ret":
            changed |= int(v.get("stub_ret", 0)) & powerpc.M64 != value & powerpc.M64
            out["stub_ret"] = value & powerpc.M64
        elif kind == "mem":
            changed |= mem.get(where, was) != value & 0xFF
            mem[where] = value & 0xFF
    if not changed:
        return None
    out["args"] = args
    if mem:
        under = powerpc.filled(v["fill"]) if v.get("fill") else b""
        pages, runs = {}, []
        for at in sorted(mem):
            lo = (at & ~(powerpc.PAGE - 1)) - powerpc.ARGS_BASE
            if 0 <= lo < len(under):                         # (in the fill: the whole page)
                base = at & ~(powerpc.PAGE - 1)
                if base not in pages:
                    pages[base] = bytearray(under[lo:lo + powerpc.PAGE])
                pages[base][at - base] = mem[at]
            elif runs and runs[-1][0] + len(runs[-1][1]) == at:
                runs[-1][1].append(mem[at])
            else:
                runs.append((at, [mem[at]]))
        out["mem_init"] = tuple(sorted([(base, bytes(page)) for base, page in pages.items()] +
                                       [(at, bytes(data)) for at, data in runs]))
    return out


def _writable(module_path):
    """[(low, high)] of the image's writable sections: its globals, which an input may set."""
    from codebreaker.formats import powerpc_image
    try:
        return [(at, at + size) for _name, at, size, chars in powerpc_image.read(module_path).sections
                if chars & 0x80000000 and not chars & 0x20000000]
    except Exception:
        return []


def explore_vectors(shared, va, span, externs, seeds, absent=None, data=()):
    """(inputs, exploration): inputs taking paths of the original no seed took, found path by path
    (lane_common.explored), and how far the exploration got. The seeds are run first as the
    guessing explorer runs them (their branches logged): of those that took the same path only
    one is run again with its shadow. Where the paths are not all and a jump of the function's
    own was seen one way only, the guessing explorer gets lane_common.GUIDED_REST runs, seeded
    with every run made."""
    from codebreaker.emulate import pcode, powerpc, shadow
    from codebreaker.verify import guided
    lifter = pcode.lifter(_mode())
    lo, hi = span
    region = (powerpc.ARGS_BASE, powerpc.ARGS_BASE + 0x10000)

    def inputs(b):
        return region[0] <= b < region[1] or any(a <= b < z for a, z in data)

    def observe(v, **more):
        try:
            r = P.emulator.run(v.get("mem_init") or (), va, v["args"], externs=externs, span=span,
                               stub_ret=v.get("stub_ret", 0), shared=shared, fill=v.get("fill"), branches=True,
                               absent=absent, **more)
        except Exception:
            return None, None
        return r, {"branches": r.raw["branches"], "reach": r.path, "args_at": None,
                   "stack": (powerpc.STACK_LOW, powerpc.STACK_TOP)}

    seen, known = [], {}
    for i, v in enumerate(seeds):
        _r, o = observe(v)
        if o is not None:
            seen.append((v, o))
            known[i] = [(a, taken) for a, taken, _cmp in o["branches"]]

    def run(v):
        sh = shadow.ShadowPPC(lifter, inputs, span=span)
        sh.enter([("r%d" % (3 + k), int(a) & powerpc.M64, k) for k, a in enumerate(v["args"][:8])], bits=64)
        r, o = observe(v, shadow=sh)
        if r is None:
            return None
        seen.append((v, o))
        lost = set(sh.lost)
        if r.truncated:
            lost.add("truncated")
        return {"trail": sh.trail, "path": sh.path, "vars": sh.vars, "lost": lost, "cut": r.truncated}
    got = lane_common.explored(run, seeds, _derive, own=lambda site: lo <= site < hi, known=known, steps=SOLVER_STEPS)
    kept = list(got.kept)
    one_way = {}
    for site, taken in got.edges:
        one_way[site] = one_way.get(site, 0) | (1 << int(taken))
    if not got.complete and any(w != 3 for w in one_way.values()) and lane_common.GUIDED_REST and seen:
        extra = guided.guided_inputs(lambda v: observe(v)[1], [], budget=lane_common.GUIDED_REST,
                                     keep=lane_common.MAX_GUIDED, seed=va & 0xFFFF, byteorder="big", word=8,
                                     observed=seen)
        kept += extra
        got.guessed = len(extra)
    return kept, got


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
        r = P.emulator.run(v.get("mem_init") or (), entry, v["args"], externs=externs, span=span,
                           stub_ret=v.get("stub_ret", 0), shared=shared, fill=v.get("fill"), absent=images[0])
    except Exception as e:
        return {"err": type(e).__name__}
    ex = r.ended
    m = r.raw["machine"]
    deps = list(lane_common.calls([(c.name, None if argc.get(c.name) is None else c.args[:argc[c.name]])
                                   for c in r.calls], lambda x: _argument(m, x, images)))
    if r.truncated and len(r.calls) >= powerpc.MAX_CALLS:
        deps.append("...")
    lo_r, hi_r = images[-1]                                  # the rebuilt image, when there is one
    lo = span[0] if span else 0
    return lane_common.fingerprint(
        r, deps, ret=None if (novalue or ex != "ret") else r.results["r3"] & M32,
        writes=lane_common.left(r, lambda a: None if len(images) > 1 and lo_r <= a < hi_r else a), saved=r.kept(),
        cov=tuple(sorted(p - lo for p in r.path)), pop=None)


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
        found = {}
        verdict, reason, limits = compare.compare(
            original,
            lambda v: fingerprint(both, rebuilt_va, v, merged, argc, images, novalue=novalue),
            vectors(), (), insns=insns, info=info,
            directed=lambda covered, reached: directed_and_guided(module_path, alone, va, span, ext_original,
                                                                  images[0], found))
        if verdict != "divergent":
            limits = list(limits) + lane_common.path_limits(found.get("got"))
        return verdict, reason, limits
    return lane_common.judged(compare_one)


KIND = "powerpc"
PER_FUNCTION = True             # a verdict holds while the function and what it reaches are unchanged


def profile(module_path):
    """(digest, settings) of everything other than the source that a verdict depends on."""
    from codebreaker.emulate import pcode
    ready = concolic_ready()
    settings = repr(("powerpc", REV, compare.REV, P.compiler.options(), vectors(),
                     ("paths", ready, lane_common.PATH_BUDGET, lane_common.MAX_PATHS, lane_common.GUIDED_REST, SOLVER_STEPS,
                      pcode.stamp(_mode()) if ready else "")))
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
    img = m.image
    top = max(va + size for va, _off, size in img.spans)
    base = (img.base + max(_OFFSET, (top - img.base + 0xFFFF) & ~0xFFFF)) & 0xFFFF0000
    dll, placed, dropped = P.packager.functions(m.header, m.texts, m.order, sorted(m.externs), work, base)
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
    # counts, where every call agrees, from the module's calls (its trees), as for any function outside.
    import re
    from codebreaker.decompile import c_text
    absent = {n for n in m.calls if re.fullmatch(r"func_0x[0-9a-f]{8}", n)}
    argc = dict(m.argc)
    argc.update({"@" + n[len("func_0x"):]: k for n, k in c_text.call_counts(m.calls, absent).items()})
    return (module_path, dll, ext_original, ext_rebuilt, argc), items, dropped


judge = _judge
