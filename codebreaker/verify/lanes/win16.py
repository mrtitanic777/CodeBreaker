"""win16.py -- what is particular to reconstructing a 16-bit Windows module.

One of the kinds reconstruct.py works with (see there for what a kind provides). Here: the
module is decompiled and made compilable by decompile/c_text.py, every function is compiled and a
16-bit DLL linked by build/functions.py, and each function is run beside the original in the
16-bit emulator.

Both sides are entered the way the function is really called (far or near) with the same words
on the stack, and the rebuilt module's data segment is given the original's data, so a global is
at the same offset and starts with the same value on both sides. Compared: how the run ends,
the Windows functions called and the UART and interrupt controller ports read and written
(emulate/ports16.py, a device both sides meet alike), in order and with their arguments, the result in AX (DX:AX when
the function returns four bytes), what is left in the data segment and on the far heap, how many
bytes of arguments were removed, and whether SI, DI and DS were preserved.

How many bytes of arguments a Windows function takes is read from the module that exports it
(emulate/api16.py). A call whose size could not be established is compared by name only, and a
match involving one is marked weak.

Besides the fixed inputs and the directed ones (the constants the code compares with), each
function is run on inputs found path by path (verify/concolic.py): the original is run with its
symbolic shadow (emulate/shadow.py: the decompiler's own p-code over the arguments, the words of
the data segment and of the scratch segment, and what an unmodelled Windows call returns), and
z3 is asked for inputs that turn each condition of its path -- one input kept per new path, and
the verdict's limits say how many paths there were (`paths=N`) and whether those are all
(`paths-all`). Where the lifter is not there, or with CB_NO_CONCOLIC=1, the inputs are guessed
instead (verify/guided.py, branch-guided).

What runs a function, how it is called and what links the rebuild are the platform's
(codebreaker/platforms/win16.py): its emulator, its convention, its packager and loader.
"""
import hashlib
import os

from codebreaker import platforms
from codebreaker.base import corpus
from codebreaker.verify import compare
from codebreaker.verify import lane_common

P = platforms.of("ne")                  # 16-bit Windows: the six slots this lane asks
NOT_RUN = ("inconclusive", "judge: the emulator crashed on this function", [], "")
ARG_WORDS = P.convention.arguments      # the words of arguments every function is given
LEFTOVER = 0x7EF707E5                  # stands for "AX held no result" in a fingerprint
REV = 23                                # bump when what is built or compared changes
                                        # (21: branch-guided inputs, an input's memory and what Windows returns;
                                        # the interrupt routines linked, their registers the build's own;
                                        # 22: in/out reach a UART and the interrupt controllers, and what was
                                        # done to them is compared with the calls; C still reading its code
                                        # segment at the original's offsets has the original's segment there;
                                        # 23: the branch-guided inputs found path by path (verify/concolic.py:
                                        # the original's symbolic shadow and z3), the seeds' paths known from
                                        # their runs; a fingerprint carries its path ("trail"); paths= limits;
                                        # what the deeper inputs showed of the judge itself: `int 21h` answered as
                                        # DOS3Call, a code segment's selector named by the segment it holds, the
                                        # rebuild's own data places left out on both sides, a rebuild still going
                                        # where the original returned given RETRY times the steps)


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


def _call(hit, inputs, entry, segs=None):
    """One Windows call as text (lane_common.call): its name, and its arguments when their size
    is known. An argument is written as what it IS rather than its bit pattern wherever the two
    builds would legitimately differ: the data or stack selector, an address in the stack, one of
    the function's own inputs (lane_common.argument; above 0xFF, since a small one is as likely a
    constant), the selector of one of the module's code segments (`segs`: selector -> the
    original's segment it holds -- a build orders its segments its own way: KEYBOARD's
    GETBIOSKEYPROC sits in segment 9 of the original and 1 of its rebuild, and the selector of
    "its segment" is handed to AllocCStoDSAlias), and a far pointer into one of them as what it
    points at (hit["pointed"]: KEYBOARD's sub_2_031F hands GetModuleHandle a name kept in its code
    segment, at 2E1h in the original and BA5h in the rebuild)."""
    name = hit.get("api") or "?"
    size = hit.get("arg_bytes")
    if size is None:
        return lane_common.call(name)
    words = (hit.get("args_stack") or [])[:size // 2]
    ss, ds = entry.get("SS"), entry.get("DS")
    out = []
    for i, w in enumerate(words):
        nxt = words[i + 1] if i + 1 < len(words) else None
        pointed = (hit.get("pointed") or {}).get(i)
        if w == ds:
            out.append("DS")
        elif segs and (w & ~3) in segs:
            out.append(segs[w & ~3])
        elif segs and nxt is not None and (nxt & ~3) in segs and pointed is not None:
            out.append("&" + pointed.hex())                  # (a far pointer into a code segment: what is there)
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
        nef = P.loader.open(module_path)
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


# A helper that works in its caller's frame (c_text._caller_frame_slots: KEYBOARD's sub_9_0A62
# clears [bp-4] and [bp-6] of OemToAnsi's frame) takes no arguments in the original and, in its C,
# a far pointer to each of those places. Run alone, the original is entered with BP at CALLER_BP --
# a caller's frame, well below the stack the run itself uses -- and the C with far pointers to
# those very places in the stack segment, not the input's words (pointers to nowhere: "exit ret
# vs fault"). The places start out holding FRAME_FILL on both sides, and what is left in them is
# compared, as data is.
CALLER_BP = 0x8000
FRAME_FILL = 0xA5


def frame_slots(text, name):
    """[(BP less, bytes)] of a caller-frame helper's parameters, in their order, from its C; or None"""
    import re
    from codebreaker.decompile import c_text
    head = text.split("{", 1)[0]
    m = re.search(r"\b%s\(((?:frame_[0-9a-f]+,?)+)\)" % re.escape(name), head)
    if not m:
        return None
    types = dict((less, ty) for ty, less in re.findall(r"(\w+) __far \*frame_([0-9a-f]+);", head))
    out = []
    for p in m.group(1).split(","):
        less = p[len("frame_"):]
        if less not in types:
            return None
        out.append((int(less, 16), c_text._SIZEOF.get(types[less], 2)))
    return tuple(out)


def _frame_arguments(frame):
    """The words a frame helper's C is entered with: a far pointer into the stack segment to each
    place, the last parameter deepest (its callers push them so)"""
    from codebreaker.emulate import windows16 as ne_probe
    words = []
    for less, _size in reversed(frame):
        words += [ne_probe.STACK_SEL, (CALLER_BP - less) & 0xFFFF]
    return words


def fingerprint(path, seg, off, near, wide, v, data_image=None, span=None, own=(), frame=None, ran=None, segs=None,
                steps=None):
    """What one run of one function did, in a form two builds can be compared on. `own`: ranges
    of the data segment that are the build's own (_own_data), whose writes are left out. `frame`:
    a caller-frame helper's places (frame_slots), whose writes are compared. `ran`: the record of
    this very run, made already (by the path explorer). With `span`, also the path the function's
    own jumps took ("trail"). `segs`: the module's code segment selectors as _call names them.
    `steps`: the instructions a run may take, when not the emulator's own limit."""
    from codebreaker.emulate import windows16 as ne_probe
    mem, bp = v.get("mem"), None
    if frame:                                                # (a caller-frame helper: see CALLER_BP)
        bp = CALLER_BP
        mem = tuple(mem or ()) + tuple((ne_probe.STACK_BASE + ((CALLER_BP - less) & 0xFFFF), bytes([FRAME_FILL]) * size)
                                       for less, size in frame)
    try:
        r = ran or P.emulator.run(path, seg=seg, offset=off, args=v["args"], near=near, data_image=data_image,
                           stub_ret=v.get("stub_ret"), mem_init=mem, bp=bp, ports=True, insn_cap=steps)
    except ImportError:
        raise
    except Exception as e:
        return {"err": type(e).__name__}
    ex = compare._exit_norm(r.get("exit"))
    lay = r.get("layout") or {}
    ds = lay.get("ds_base")
    state, before = {}, {}
    places = {}                                              # a frame helper's places in the stack: byte -> its own
    for less, size in frame or ():
        for i in range(size):
            places[ne_probe.STACK_BASE + ((CALLER_BP - less + i) & 0xFFFF)] = ("F", -less + i)
    for w in r.get("mem_writes") or []:
        a, n, val = w["addr"], w["size"], w["value"] or 0
        if places and any(a + i in places for i in range(min(n, 4))):
            for i in range(min(n, 4)):
                if a + i in places:
                    before.setdefault(places[a + i], ((w.get("old") or 0) >> (8 * i)) & 0xFF)
                    state[places[a + i]] = (val >> (8 * i)) & 0xFF
            continue
        if ds is not None and ds <= a < ds + 0x10000:
            if any(lo <= a - ds < hi for lo, hi in own):
                continue
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
            "deps": _effects(r.get("imports_hit") or (), r.get("io") or (), lambda h: _call(h, v["args"], entry, segs)),
            "writes": writes, "pop": r.get("sp_delta") if ex == "ret" else None,
            "saved": all(regs.get(r) == entry.get(r) for r in P.convention.preserved) if ex == "ret" else None,
            "cov": cov, "reach": (), "trail": tuple(trail(path, seg, span, r.get("path") or ())) if span else ()}


def _effects(hits, io, call):
    """The run's effects outside itself, in the order it had them: each Windows call (`call(hit)`)
    and each access to a device (emulate/ports16.py) -- `out(3fb,83)`, `in(3fd)`. What a function
    did to the UART is what it is for: a rebuild writing another divisor, or reading the line
    status of another port, did something else."""
    out = [(h.get("n") or 0, 0, k, call(h)) for k, h in enumerate(hits)]
    for k, (n, what, port, value) in enumerate(io):
        out.append((n, 1, k, "..." if what == "..." else "in(%x)" % port if what == "in" else
                    "out(%x,%x)" % (port, value)))
    return tuple(text for _n, _o, _k, text in sorted(out))


RETRY = 8                       # times the emulator's step limit a rebuild still going is given


def _steps():
    from codebreaker.emulate import emulator
    return emulator._MAX_INSNS


def _judge(context, item):
    """Worker. item = (key, seg, off, near, wide, span, insns, rebuilt seg, rebuilt off[, frame]):
    `frame` a caller-frame helper's places (frame_slots), else None."""
    module_path, dll, data_image, own = context[:4]
    order = context[4] if len(context) > 4 else {}          # rebuilt segment -> the original's
    key, seg, off, near, wide, span, insns, bseg, boff = item[:9]
    frame = item[9] if len(item) > 9 else None
    last, runs, paths = {}, {}, {}

    def original(v, ran=None):
        # the original's run depends only on the original: remembered across rebuilds (a run cut
        # off at the step limit is not), and within this judgement (the path explorer's runs too)
        # (and on what of the rebuild it is compared with: the places in the data segment the
        # rebuild keeps for itself, left out on both sides -- NEWTABLEEX, its table put at DS:0 by
        # an input, wrote there in both builds, and only the original's writes were counted -- and
        # which code segment of the rebuild holds which of the original's)
        k = compare.cache_key("ne", REV, corpus.sha(module_path), seg, off, near, wide, tuple(v["args"]), span,
                              v.get("mem"), v.get("stub_ret"), own, sorted(order.items()),
                              *((CALLER_BP, FRAME_FILL, frame) if frame else ()))
        fp = runs.get(k)
        if fp is None:
            fp = lane_common.remembered(k, lambda: fingerprint(module_path, seg, off, near, wide, v, span=span, own=own,
                                                               frame=frame, ran=ran, segs=_named(order.values())),
                                        keep=lambda fp: not fp.get("truncated"))
            runs[k] = fp
        last["fp"] = fp
        return fp

    def rebuilt(v):
        if frame:                                            # (pointers to the places, not the input's words)
            v = dict(v, args=_frame_arguments(frame))
        fp = fingerprint(dll, bseg, boff, near, wide, v, data_image=data_image, own=own, frame=frame,
                         segs=_named(order.values(), order))
        if fp.get("exit") == "truncated" and (last.get("fp") or {}).get("exit") == "ret":
            # (the original returned and its C was still going: C takes more instructions than the
            # code it was made from -- KEYBOARD's sub_2_06B6 over a table of 448 entries an input
            # gave it. Given RETRY times the steps; one still going then does not end)
            fp = fingerprint(dll, bseg, boff, near, wide, v, data_image=data_image, own=own, frame=frame,
                             segs=_named(order.values(), order), steps=RETRY * _steps())
        if (last.get("fp") or {}).get("ret") == LEFTOVER and "err" not in fp and fp.get("ret") is not None:
            fp["ret"] = LEFTOVER                             # the original returned no value here
        return fp
    def aimed(covered, reached):
        # the directed inputs, then those found path by path (or guessed) seeded with every input
        # -- of the fixed ones the paths they took are known: one per path is run again
        d = directed(module_path, seg, span)
        seeds = vectors() + list(d)
        known = {i: original(v).get("trail") for i, v in enumerate(vectors()) if "err" not in original(v)}
        kept, got, ran = explore_vectors(module_path, seg, off, near, span, seeds, known=known)
        if not frame:                                        # (the explorer ran the original as the judge does)
            for v, r in ran:
                original(v, ran=r)
        paths["got"] = got
        return list(d) + kept

    def judge(info):
        verdict, reason, limits = compare.compare(original, rebuilt, vectors(), (), insns=insns, info=info,
                                                  directed=aimed)
        if verdict != "divergent":
            limits = list(limits) + lane_common.path_limits(paths.get("got"))
        return verdict, reason, limits
    return lane_common.judged(judge)


# Branch-guided inputs (verify/guided.py, through lane_common.guided). The explorer reads an
# input's arguments as words rising up the stack from `args_at`; the tracer pushes them in Pascal
# order, the first deepest. So to the explorer an input's words are the other way round, and its
# memory is linear addresses of the original's run -- in the data segment ("D", offset there in
# whichever build), or the scratch segment pointers go to (at one place in every build); a place
# elsewhere is not something an input sets, and is left out.
def _to_explorer(v, ds):
    g = {"args": list(reversed(v["args"]))}
    if v.get("mem"):
        g["mem_init"] = [((ds + w[1]) if isinstance(w, tuple) else w, data) for w, data in v["mem"]]
    if v.get("stub_ret") is not None:
        g["stub_ret"] = v["stub_ret"]
    return g


def _from_explorer(g, ds):
    from codebreaker.emulate import windows16 as ne_probe
    v = {"args": [int(a) & 0xFFFF for a in reversed(g["args"])]}
    mem = []
    for at, data in g.get("mem_init") or ():
        if ds is not None and ds <= at < ds + 0x10000:
            mem.append((("D", at - ds), bytes(data)))
        elif ne_probe.SCRATCH_BASE <= at < ne_probe.SCRATCH_BASE + 0x10000:
            mem.append((at, bytes(data)))
    if mem:
        v["mem"] = tuple(mem)
    if g.get("stub_ret") is not None:
        v["stub_ret"] = int(g["stub_ret"]) & 0xFFFFFFFF
    return v


def _branch_run(module_path, seg, off, near, span, v, **probe):
    """The original run on `v` with its branches logged, as the explorer reads it, or None when
    the run is not an observation. `probe`: more of ne_probe.probe's options, for every run."""
    from codebreaker.emulate import windows16 as ne_probe
    try:
        r = P.emulator.run(module_path, seg=seg, offset=off, args=v["args"], near=near,
                           stub_ret=v.get("stub_ret"), mem_init=v.get("mem"), branches=span,
                           **dict({"ports": True}, **probe))
    except ImportError:
        raise
    except Exception:
        return None
    base = ne_probe.STACK_BASE
    return {"branches": r.get("branches") or [], "reach": set(r.get("path") or ()),
            "args_at": base + ne_probe.FAR_RET - 2 * len(v["args"]), "stack": (base, base + 0x10000)}


def guided_vectors(module_path, seg, off, near, span, seeds, **probe):
    """Inputs that take branches of the original no seed took (verify/guided.py). `probe`: how the
    original is run, when not as the judge runs it -- the build's gate (verify/gate16/isolated16.py)
    answers the module's own callees and fills the data segment's tail, and explores so."""
    from codebreaker.emulate import windows16 as ne_probe
    auto = P.loader.open(module_path).h.get("autodata_seg")
    ds = ne_probe.segment_base(auto) if auto else None
    kept = lane_common.guided(
        lambda g: _branch_run(module_path, seg, off, near, span, _from_explorer(g, ds), **probe),
        [_to_explorer(v, ds) for v in seeds], off, word=2)
    return [_from_explorer(g, ds) for g in kept]


# Path by path (verify/concolic.py): the original run with its symbolic shadow (emulate/shadow.py)
# -- its arguments, the words of its data segment and of the scratch segment pointers go to, and
# what an unmodelled Windows call returns are the inputs -- and inputs solved for to take each
# path its conditions allow. Where the lifter is not there (an older pcodecheck) or CB_NO_CONCOLIC
# is set, the inputs are guessed as before (guided_vectors).
def concolic_ready():
    from codebreaker.emulate import pcode
    return lane_common.guided_on() and lane_common.concolic_on() and pcode.lifter() is not None


_JUMPS = {}


def _jumps(module_path, seg):
    """{offset: target offset} of the conditional jumps at each offset of a code segment, as found
    (decoded the first time an offset is asked about; None: not a conditional jump)."""
    key = (module_path, seg)
    if key not in _JUMPS:
        from codebreaker.formats.ne import NEFile
        nef = NEFile(module_path)
        s = nef.segments()[seg - 1]
        _JUMPS[key] = (bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]]), {})
    return _JUMPS[key]


def trail(module_path, seg, span, path):
    """[(site, taken)] -- the function's own conditional jumps in the order a run took them, from
    the run's instructions (rec["path"], linear addresses): the path it went, as the explorer
    names one (concolic.py), for a run made without a shadow."""
    from codebreaker.decompile.describe import decoder16
    from codebreaker.emulate import windows16 as ne_probe
    code, known = _jumps(module_path, seg)
    base = ne_probe.segment_base(seg)
    lo, hi = base + span[0], base + span[1]
    out = []
    md = None
    for i, a in enumerate(path[:-1]):
        if not lo <= a < hi:
            continue
        off = a - base
        if off not in known:
            md = md or decoder16()
            ins = next(md.disasm(code[off:off + 8], off, 1), None)
            j = None
            if ins is not None and (ins.mnemonic.startswith("j") and ins.mnemonic != "jmp" or
                                    ins.mnemonic.startswith("loop")) and ins.op_str.startswith("0x"):
                j = (int(ins.op_str, 16), off + ins.size)
            known[off] = j
        j = known[off]
        if j is not None and j[0] != j[1] and path[i + 1] - base in j:
            out.append((a, path[i + 1] - base == j[0]))
    return out


def _derive(v, values, names, ds):
    """Input `v` with the named inputs (shadow.vars) set to `values`: an argument word, what an
    unmodelled call returns, bytes of the data segment or of the scratch segment."""
    from codebreaker.emulate import windows16 as ne_probe
    out = dict(v)
    args = list(v["args"])
    mem = {}
    for where, data in v.get("mem") or ():
        at = (ds + where[1]) if isinstance(where, tuple) else where
        if isinstance(where, tuple) and ds is None:
            continue
        for k, b in enumerate(bytes(data)):
            mem[at + k] = b
    changed = False
    for name, value in values.items():
        kind, where, bits, was = names.get(name, (None, None, 0, None))
        if kind == "arg" and where < len(args):
            changed |= args[where] != value & 0xFFFF
            args[where] = value & 0xFFFF
        elif kind == "ret":
            changed |= (v.get("stub_ret") or 0) != value
            out["stub_ret"] = value & 0xFFFFFFFF
        elif kind == "mem":
            changed |= mem.get(where, was) != value & 0xFF
            mem[where] = value & 0xFF
    if not changed:
        return None
    out["args"] = args
    runs = []
    for at in sorted(mem):
        if runs and runs[-1][0] + len(runs[-1][1]) == at:
            runs[-1][1].append(mem[at])
        else:
            runs.append((at, [mem[at]]))
    entries = []
    for at, data in runs:
        if ds is not None and ds <= at < ds + 0x10000:
            entries.append((("D", at - ds), bytes(data)))
        elif ne_probe.SCRATCH_BASE <= at < ne_probe.SCRATCH_BASE + 0x10000:
            entries.append((at, bytes(data)))
    if entries:
        out["mem"] = tuple(entries)
    else:
        out.pop("mem", None)
    return out


def explore_vectors(module_path, seg, off, near, span, seeds, known=None, **probe):
    """(inputs, exploration, [(input, record)]): inputs taking paths of the original no seed took,
    found path by path (verify/concolic.py; lane_common.explored), how far the exploration got
    (its .paths, .complete), and every run it made, input and record (a judge running the
    original as `probe` does need not run those again). `known`: {seed index: trail} of seeds
    already run. Falls back to guided_vectors (exploration None) where it cannot.

    Where the paths found are not all there are and a jump of the function's own was still seen
    going one way only, what is left of the guessing explorer's budget (lane_common.GUIDED_REST)
    goes to it, seeded with the runs made: a condition over what the shadow does not follow --
    the made-up answer of a callee not run, a Windows call modelled from its arguments -- is no
    question for the solver, and guessing still reaches some of them."""
    if not concolic_ready():
        return guided_vectors(module_path, seg, off, near, span, seeds, **probe), None, []
    from codebreaker.emulate import pcode, shadow
    from codebreaker.emulate import windows16 as ne_probe
    from codebreaker.formats.ne import NEFile
    auto = NEFile(module_path).h.get("autodata_seg")
    ds = ne_probe.segment_base(auto) if auto else None
    base = ne_probe.segment_base(seg)
    lo, hi = base + span[0], base + span[1]
    scratch = ne_probe.SCRATCH_BASE
    lifter = pcode.lifter()
    made = []                                                # (input, record) of every run

    def inputs(b):
        return (ds is not None and ds <= b < ds + 0x10000) or scratch <= b < scratch + 0x10000

    def run(v):
        sh = shadow.Shadow16(lifter, ne_probe.selector_base, inputs, span=(lo, hi))
        try:
            r = ne_probe.probe(module_path, seg=seg, offset=off, args=v["args"], near=near,
                               stub_ret=v.get("stub_ret"), mem_init=v.get("mem"), shadow=sh, branches=span,
                               **dict({"ports": True}, **probe))
        except ImportError:
            raise
        except Exception:
            return None
        lost = set(sh.lost)
        cut = bool(r.get("truncated") or str(r.get("exit")) == "truncated")
        if cut:
            lost.add("truncated")
        made.append((v, r))
        return {"trail": sh.trail, "path": sh.path, "vars": sh.vars, "lost": lost, "cut": cut}
    got = lane_common.explored(run, seeds, lambda v, values, names: _derive(v, values, names, ds),
                               own=lambda site: lo <= site < hi, known=known)
    kept = list(got.kept)
    one_way = {}
    for site, taken in got.edges:
        one_way[site] = one_way.get(site, 0) | (1 << int(taken))
    if not got.complete and any(w != 3 for w in one_way.values()) and lane_common.GUIDED_REST and made:
        from codebreaker.verify import guided

        def plain(g):
            v = _from_explorer(g, ds)
            try:
                r = ne_probe.probe(module_path, seg=seg, offset=off, args=v["args"], near=near,
                                   stub_ret=v.get("stub_ret"), mem_init=v.get("mem"), branches=span,
                                   **dict({"ports": True}, **probe))
            except ImportError:
                raise
            except Exception:
                return None
            made.append((v, r))
            return _observed(r, v)

        def _observed(r, v):
            stack = ne_probe.STACK_BASE
            return {"branches": r.get("branches") or [], "reach": set(r.get("path") or ()),
                    "args_at": stack + ne_probe.FAR_RET - 2 * len(v["args"]), "stack": (stack, stack + 0x10000)}
        seen = [(_to_explorer(v, ds), _observed(r, v)) for v, r in made]
        extra = guided.guided_inputs(plain, [], budget=lane_common.GUIDED_REST, keep=lane_common.MAX_GUIDED,
                                     seed=off, word=2, observed=seen)
        kept += [_from_explorer(g, ds) for g in extra]
        got.guessed = len(extra)
    return kept, got, made


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
    from codebreaker.emulate import pcode
    ready = concolic_ready()
    settings = repr(("ne", REV, compare.REV, P.compiler.options(), vectors(),
                     ("guided", lane_common.GUIDED_BUDGET, lane_common.MAX_GUIDED),
                     ("paths", ready, lane_common.PATH_BUDGET, lane_common.MAX_PATHS, pcode.stamp() if ready else "")))
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
    nef = P.loader.open(module_path)
    dll, placed, dropped = P.packager.functions(m.header, m.texts, m.order, m.imports, work, segments=m.segments,
                                                code_segment={fn: m.where[va][0] for va, fn in m.by_va.items()},
                                                interrupts={v for v, _regs in getattr(m, "interrupts", ())},
                                                code_data=getattr(m, "code_data", None),
                                                code_image=_code_images(m, nef))
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
        frame = frame_slots(m.texts.get(fn, ""), fn)
        if frame:                                            # (a caller-frame helper: _judge)
            items[fn] += (frame,)
    order = {}                                               # rebuilt code segment -> the original's
    for it in items.values():
        order.setdefault(it[7], int(it[0].split(":")[0]))
    return (module_path, dll, data_image, _own_data(work), order), items, dropped


def _named(original_segments, order=None):
    """{selector: "seg<n>"} for a build's code segments, n the original's number of the segment
    each holds: the original's own (`order` None), or a rebuild's through `order` (its segment ->
    the original's)."""
    from codebreaker.emulate import windows16 as ne_probe
    pairs = order.items() if order is not None else ((n, n) for n in original_segments)
    return {ne_probe._sel(ne_probe._MODULE_SLOT + mine - 1): "seg%d" % theirs for mine, theirs in pairs}


def _code_images(m, nef):
    """{segment: the original's bytes of it} for each code segment some of whose C still reads it
    through CS at the original's offsets -- what c_text could not name (m.code_reads "left": an
    offset from a register, a table of the code's own addresses), or, with the naming turned off
    (CB_CODE_DATA=0), every segment whose C reads through CS. The judge's build puts them at the
    start of the rebuilt segment (functions.build16 code_image): CS:offset there is then what it is
    in the original (the file's bytes: a place the loader fixes up is not)."""
    if os.environ.get("CB_CODE_DATA") == "0":
        want = {m.where[va][0] for va, fn in m.by_va.items() if "codebreaker_cs()" in m.texts.get(fn, "")}
    else:
        want = {int(sg) for sg, r in (getattr(m, "code_reads", None) or {}).items() if r.get("left")}
    segs = {s["idx"]: s for s in nef.segments()}
    return {sg: bytes(nef.data[segs[sg]["file_off"]:segs[sg]["file_off"] + segs[sg]["length"]])
            for sg in sorted(want) if sg in segs and segs[sg]["file_off"]}


def _own_data(work):
    """The offsets in the rebuild's data segment of what the build itself keeps there and the
    original does not: the registers an interrupt helper hands its routine (codebreaker_int,
    c_text.interrupt_routines). Writing them is the build's bookkeeping, not the function's
    effect -- as a rebuilt 32-bit image's own statics are left out (win32._image_span)."""
    import re
    try:
        text = open(os.path.join(work, "m.map"), errors="replace").read()
    except OSError:
        return ()
    m = re.search(r"(?m)^[0-9A-Fa-f]{4}:([0-9A-Fa-f]{4,8})[+*]?\s+_codebreaker_int\s*$", text)
    return ((int(m.group(1), 16), int(m.group(1), 16) + _INT_REGISTERS),) if m else ()


_INT_REGISTERS = 20                     # sizeof(struct codebreaker_registers): eight words, CF, ZF, a word


judge = _judge
