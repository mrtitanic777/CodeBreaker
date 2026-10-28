"""win16.py -- what is particular to reconstructing a 16-bit Windows module.

One of the kinds reconstruct.py works with (see there for what a kind provides). Here: the
module is decompiled and made compilable by decompile/c_text.py, every function is compiled and a
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

from codebreaker.base import corpus
from codebreaker.verify import compare
from codebreaker.verify import lane_common

NOT_RUN = ("inconclusive", "judge: the emulator crashed on this function", [], "")
ARG_WORDS = 8
LEFTOVER = 0x7EF707E5                  # stands for "AX held no result" in a fingerprint
REV = 20                                # bump when what is built or compared changes


def vectors():
    """The inputs every function is run on: eight words of arguments each.

    Plain numbers alone go nowhere in 16-bit code -- most functions take far pointers, and the
    first use of a pointer whose selector word is 3 or 0x400 is a protection fault, on both
    sides, before anything has been compared. So half the inputs are (offset, selector) pairs
    naming a real, writable segment, in both orders (which word is the selector depends on where
    in the argument list a pointer sits)."""
    from codebreaker.emulate import windows16 as ne_probe
    n, sel = ARG_WORDS, ne_probe.SAFE_ARG
    return [{"args": [0] * n}, {"args": [0x0400] * n}, {"args": [0x0080] * n},
            {"args": lane_common.counting(n)}, {"args": lane_common.counting(n, 0x100, 0x100)},
            {"args": lane_common.descending(n, 0xFFFF)},
            {"args": lane_common.alternating(n, lambda i: 0x20 * (i + 1), sel)},     # offset, selector, ...
            {"args": lane_common.alternating(n, sel, lambda i: 0x20 * (i + 1))},     # selector, offset, ...
            {"args": [sel] * n},
            {"args": [1, 0x40, sel, 2, 0x80, sel, 3, 0x100]}] + _scattered(n, sel)


def _scattered(n, sel):
    """Four more, every word a different one spread over the segment (the same on every run):
    with the same word in each argument, or small ones pointing at the quiet start of the data
    segment, a function reading the wrong argument or the wrong field read what it should have
    -- planted that way, 17 of 40 such bugs went unseen (tools/gate_teeth16.py)."""
    import random
    rnd = random.Random(0x16)
    out = []
    for k in range(4):
        words = [rnd.randrange(0x0100, 0x7F00) & ~1 for _ in range(n)]
        if k % 2:                                            # (and as far pointers: offset, selector)
            words = [sel if i % 2 else w for i, w in enumerate(words)]
        out.append({"args": words})
    return out


def _call(hit, inputs, entry):
    """One Windows call as text (lane_common.call): its name, and its arguments when their size
    is known. An argument is written as what it IS rather than its bit pattern wherever the two
    builds would legitimately differ: the data or stack selector, an address in the stack, one of
    the function's own inputs (lane_common.argument; above 0xFF, since a small one is as likely a
    constant)."""
    name = hit.get("api") or "?"
    size = hit.get("arg_bytes")
    if size is None:
        return lane_common.call(name)
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
        else:
            out.append(lane_common.argument(w, inputs, above=0xFF))
    return lane_common.call(name, out)


_DIRECTED = {}


def directed(module_path, seg, span, most=8):
    """Inputs aimed at what the function's own code tests: the constants it compares its values
    with. A branch on `cmp ax, 0x102` is never taken by inputs chosen without looking, and
    everything behind it stays unexercised; giving the constant in each argument position (alone,
    and as the offset of a valid far pointer) reaches it."""
    key = (module_path, seg, span)
    if key not in _DIRECTED:
        import capstone
        from codebreaker.emulate import windows16 as emulator
        from codebreaker.formats.ne import NEFile
        nef = NEFile(module_path)
        s = nef.segments()[seg - 1]
        code = nef.data[s["file_off"]:s["file_off"] + s["length"]]
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
        md.detail = True
        seen = []
        # (a switch written as a chain of subtractions -- `dec ax / dec ax / sub ax, 4 / sub ax, 9
        # / sub ax, 72h`, each case's `je` after its step: a window procedure's messages 1, 2, 6,
        # 0Fh, 81h -- tests the running total, not each step's number: the totals are the cases)
        X = capstone.x86
        total = {}
        for ins in md.disasm(bytes(code[span[0]:span[1]]), span[0]):
            ops = ins.operands
            reg = ops[0].reg if ops and ops[0].type == X.X86_OP_REG else None
            if reg is not None and ins.mnemonic in ("dec", "inc", "sub", "add") and \
                    (ins.mnemonic in ("dec", "inc") or (len(ops) > 1 and ops[1].type == X.X86_OP_IMM)):
                step = 1 if ins.mnemonic in ("dec", "inc") else ops[1].imm
                step = step if ins.mnemonic in ("dec", "sub") else -step
                total[reg] = (total.get(reg, 0) + step) & 0xFFFF
                c = total[reg]
                if c not in seen and c not in (0, 0xFFFF):
                    seen.append(c)
                continue
            if reg is not None and ins.mnemonic not in ("cmp", "test", "or"):
                total.pop(reg, None)                         # (the register set anew: a chain ends)
            if ins.mnemonic not in ("cmp", "test", "and"):
                continue
            for op in ops:
                if op.type == X.X86_OP_IMM:
                    c = op.imm & 0xFFFF
                    if c not in seen and c not in (0, 0xFFFF):
                        seen.append(c)
        n, sel, out = ARG_WORDS, emulator.SAFE_ARG, []
        for c in seen[:max(most // 2, 24)]:
            out.append({"args": [c] * n})
            out.append({"args": lane_common.alternating(n, c, sel)})
        _DIRECTED[key] = out
    return _DIRECTED[key]


def fingerprint(path, seg, off, near, wide, v, data_image=None, span=None):
    """What one run of one function did, in a form two builds can be compared on."""
    from codebreaker.emulate import windows16 as ne_probe
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
    writes = lane_common.writes({k: x for k, x in state.items() if x != before.get(k)})
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
    module_path, dll, data_image = context
    key, seg, off, near, wide, span, insns, bseg, boff = item
    last = {}

    def original(v):
        # the original's run depends only on the original: remembered across rebuilds (a run cut
        # off at the step limit is not)
        k = compare.cache_key("ne", REV, corpus.sha(module_path), seg, off, near, wide, tuple(v["args"]), span)
        fp = lane_common.remembered(k, lambda: fingerprint(module_path, seg, off, near, wide, v, span=span),
                                    keep=lambda fp: not fp.get("truncated"))
        last["fp"] = fp
        return fp

    def rebuilt(v):
        fp = fingerprint(dll, bseg, boff, near, wide, v, data_image=data_image)
        if (last.get("fp") or {}).get("ret") == LEFTOVER and "err" not in fp and fp.get("ret") is not None:
            fp["ret"] = LEFTOVER                             # the original returned no value here
        return fp
    return lane_common.judged(lambda info: compare.compare(
        original, rebuilt, vectors(), (), insns=insns, info=info,
        directed=lambda covered, reached: directed(module_path, seg, span)))


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
PER_FUNCTION = True             # a verdict holds while the function and what it reaches are unchanged


def profile(module_path):
    """(digest, settings) of everything other than the source that a verdict depends on."""
    from codebreaker.build import functions
    settings = repr(("ne", REV, compare.REV, functions.CFLAGS16, vectors()))
    return hashlib.sha1(settings.encode()).hexdigest()[:12], settings


def key(seg, off):
    return "%d:%04x" % (seg, off)


def prepare(module_path, work):
    from codebreaker.decompile import c_text
    m = c_text.compilable16(module_path, work)
    m.key = {m.by_va[va]: key(*m.where[va]) for va in m.by_va}
    m.all_keys = sorted(m.key.values())
    m.sig = hashlib.sha1((m.header + "".join(m.texts[n] for n in m.order)).encode("utf-8", "replace")).hexdigest()
    return m


def build(m, module_path, work):
    """-> (context for judge, {name: item}, {name: why it is not in the build})"""
    from codebreaker.build import functions
    from codebreaker.formats.ne import NEFile
    dll, placed, dropped = functions.build16(m.header, m.texts, m.order, m.imports, work, segments=m.segments,
                                             code_segment={fn: m.where[va][0] for va, fn in m.by_va.items()})
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
