"""win16.py -- what is particular to reconstructing a 16-bit Windows module.

One of the kinds reconstruct.py works with (see there for what a kind provides). Here: the
module is decompiled and made compilable by decompile/c16.py, every function is compiled and a
16-bit DLL linked by build/functions.py, and each function is run beside the original in the
16-bit emulator.

Both sides are entered the way the function is really called (far or near) with the same words
on the stack, and the rebuilt module's data segment is given the original's data, so a global is
at the same offset and starts with the same value on both sides. Compared: how the run ends,
the Windows functions called, in order and with their arguments, the result in AX (DX:AX when
the function returns four bytes), what is left in the data segment and on the far heap, how many
bytes of arguments were removed, and whether SI, DI and DS were preserved.

How many bytes of arguments a Windows function takes is read from the module that exports it
(emulate/api16.py). A call whose size could not be established is compared by name only, and a
match involving one is marked weak.
"""
import hashlib

NOT_RUN = ("inconclusive", "judge: the emulator crashed on this function", [], "")
ARG_WORDS = 8
LEFTOVER = 0x7EF707E5                  # stands for "AX held no result" in a fingerprint
REV = 12                                # bump when what is built or compared changes


def vectors():
    """The inputs every function is run on: eight words of arguments each.

    Plain numbers alone go nowhere in 16-bit code -- most functions take far pointers, and the
    first use of a pointer whose selector word is 3 or 0x400 is a protection fault, on both
    sides, before anything has been compared. So half the inputs are (offset, selector) pairs
    naming a real, writable segment, in both orders (which word is the selector depends on where
    in the argument list a pointer sits)."""
    from codebreaker.emulate import trace16 as ne_probe
    n, sel = ARG_WORDS, ne_probe.SAFE_ARG
    return [{"args": [0] * n}, {"args": [0x0400] * n}, {"args": [0x0080] * n},
            {"args": [i + 1 for i in range(n)]}, {"args": [0x100 * (i + 1) for i in range(n)]},
            {"args": [0xFFFF - i for i in range(n)]},
            {"args": [sel if i % 2 else 0x20 * (i + 1) for i in range(n)]},     # offset, selector, ...
            {"args": [0x20 * (i + 1) if i % 2 else sel for i in range(n)]},     # selector, offset, ...
            {"args": [sel] * n},
            {"args": [1, 0x40, sel, 2, 0x80, sel, 3, 0x100]}]


def _call(hit, inputs, entry):
    """One Windows call as text: its name, and its arguments when their size is known. An
    argument is written as what it IS rather than its bit pattern wherever the two builds would
    legitimately differ: one of the function's own inputs, the data or stack selector, an
    address in the stack."""
    name = hit.get("api") or "?"
    size = hit.get("arg_bytes")
    if size is None:
        return name
    words = (hit.get("args_stack") or [])[:size // 2]
    ss, ds = entry.get("SS"), entry.get("DS")
    out = []
    for i, w in enumerate(words):
        nxt = words[i + 1] if i + 1 < len(words) else None
        if w == ds:
            out.append("DS")
        elif w == ss:
            out.append("SS")
        elif nxt == ss:
            out.append("stack")                              # offset part of a far pointer into the stack
        elif w in inputs and w > 0xFF:
            out.append("a%d" % inputs.index(w))
        else:
            out.append("%x" % w)
    return "%s(%s)" % (name, ",".join(out))


def fingerprint(path, seg, off, near, wide, v, data_image=None, span=None):
    """What one run of one function did, in a form two builds can be compared on."""
    from codebreaker.emulate import trace16 as ne_probe
    from codebreaker.verify import compare
    try:
        r = ne_probe.probe(path, seg=seg, offset=off, args=v["args"], near=near, data_image=data_image)
    except ImportError:
        raise
    except Exception as e:
        return {"err": type(e).__name__}
    ex = compare._exit_norm(r.get("exit"))
    lay = r.get("layout") or {}
    ds = lay.get("ds_base")
    state, before = {}, {}
    for w in r.get("mem_writes") or []:
        a, n, val = w["addr"], w["size"], w["value"] or 0
        if ds is not None and ds <= a < ds + 0x10000:
            where = ("D", a - ds)
        elif ne_probe.HEAP_BASE <= a < ne_probe.HEAP_BASE + 64 * ne_probe.HEAP_STRIDE:
            where = ("H", a - ne_probe.HEAP_BASE)
        else:
            continue                                         # the stack, or somewhere wild
        for i in range(min(n, 4)):
            k = (where[0], where[1] + i)
            before.setdefault(k, ((w.get("old") or 0) >> (8 * i)) & 0xFF)
            state[k] = (val >> (8 * i)) & 0xFF
    # a byte left holding what it held on entry is not an effect (one compiler clears a flag
    # with a word write, another with a byte write)
    state = {k: x for k, x in state.items() if x != before.get(k)}
    writes = "%d:%s" % (len(state), hashlib.sha1(repr(sorted(state.items())).encode()).hexdigest()[:16]) \
        if state else ""
    regs = r.get("exit_regs") or {}
    entry = r.get("entry_regs") or {}
    base = r.get("code_base") or 0
    lo, hi = span or (off, off)
    cov = tuple(sorted({a - base - lo for a in r.get("path") or () if lo <= a - base < hi}))
    ret = None
    if ex == "ret" and wide is not None:                     # None: the function returns nothing
        ret = (r.get("ret32") if wide else r.get("ret")) or 0
        # An exported function starts `push ds / pop ax`; one that sets no result returns with
        # that still in AX. Its own data selector is a leftover, not a result.
        if ret & 0xFFFF == (entry.get("DS") or -1) and not wide:
            ret = LEFTOVER
    return {"ret": ret, "exit": ex, "truncated": bool(r.get("truncated")),
            "deps": tuple(_call(h, v["args"], entry) for h in r.get("imports_hit") or ()),
            "writes": writes, "pop": r.get("sp_delta") if ex == "ret" else None,
            "saved": (regs.get("SI") == entry.get("SI") and regs.get("DI") == entry.get("DI")
                      and regs.get("DS") == entry.get("DS")) if ex == "ret" else None,
            "cov": cov, "reach": ()}


def _judge(context, item):
    """Worker. item = (key, seg, off, near, wide, span, insns, rebuilt seg, rebuilt off)."""
    from codebreaker.verify import compare
    module_path, dll, data_image = context
    key, seg, off, near, wide, span, insns, bseg, boff = item
    info = {}
    try:
        last = {}

        def original(v):
            last["fp"] = fingerprint(module_path, seg, off, near, wide, v, span=span)
            return last["fp"]

        def rebuilt(v):
            fp = fingerprint(dll, bseg, boff, near, wide, v, data_image=data_image)
            if (last.get("fp") or {}).get("ret") == LEFTOVER and "err" not in fp and fp.get("ret") is not None:
                fp["ret"] = LEFTOVER                         # the original returned no value here
            return fp
        verdict, reason, limits = compare.compare(original, rebuilt, vectors(), (), insns=insns, info=info)
    except ImportError:
        raise
    except Exception as e:
        verdict, reason, limits = "inconclusive", "judge: %s" % type(e).__name__, []
    return verdict, reason, list(limits), info.get("detail", "")


def _spans(nef, where):
    """{address: ((start, end), instruction count)} -- each function runs to the next one in its
    segment (or the segment's end)."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    segs = {s["idx"]: s for s in nef.segments()}
    by_seg = {}
    for va, (seg, off) in where.items():
        by_seg.setdefault(seg, []).append((off, va))
    out = {}
    for seg, lst in by_seg.items():
        lst.sort()
        s = segs[seg]
        code = nef.data[s["file_off"]:s["file_off"] + s["length"]]
        for i, (off, va) in enumerate(lst):
            end = lst[i + 1][0] if i + 1 < len(lst) else len(code)
            n = sum(1 for _ in md.disasm(code[off:end], off))
            out[va] = ((off, end), max(n, 1))
    return out


KIND = "ne"


def profile(module_path):
    """(digest, settings) of everything other than the source that a verdict depends on."""
    from codebreaker.build import functions
    from codebreaker.decompile import decompiler
    from codebreaker.verify import compare
    settings = repr(("ne", REV, decompiler.REV, compare.REV, functions.CFLAGS16, vectors()))
    return hashlib.sha1(settings.encode()).hexdigest()[:12], settings


def key(seg, off):
    return "%d:%04x" % (seg, off)


def prepare(module_path, work):
    from codebreaker.decompile import c16
    m = c16.compilable(module_path, work)
    m.key = {m.by_va[va]: key(*m.where[va]) for va in m.by_va}
    m.all_keys = sorted(m.key.values())
    m.sig = hashlib.sha1((m.header + "".join(m.texts[n] for n in m.order)).encode("utf-8", "replace")).hexdigest()
    return m


def build(m, module_path, work):
    """-> (context for judge, {name: item}, {name: why it is not in the build})"""
    from codebreaker.build import functions
    from codebreaker.formats.ne import NEFile
    dll, placed, dropped = functions.build16(m.header, m.texts, m.order, m.imports, work, segments=m.segments)
    nef = NEFile(module_path)
    auto = nef.h.get("autodata_seg")
    data_image = b""
    if auto:
        s = nef.segments()[auto - 1]
        data_image = bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]]) if s["file_off"] else b""
    spans = _spans(nef, {va: m.where[va] for va in m.by_va})
    items, dropped = {}, dict(dropped)
    for va, fn in m.by_va.items():
        if fn in dropped:
            continue
        if fn not in placed:
            dropped[fn] = "not in the build"
            continue
        seg, off = m.where[va]
        span, insns = spans[va]
        wide = None if fn in m.novalue else fn in m.wide
        items[fn] = (m.key[fn], seg, off, fn in m.near, wide, span, insns) + tuple(placed[fn])
    return (module_path, dll, data_image), items, dropped


judge = _judge
