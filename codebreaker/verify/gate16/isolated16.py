"""isolated16.py -- each function of a rebuilt 16-bit module run beside the original's, on its own.

    from codebreaker.verify.gate16 import isolated16
    for name, verdict, why in isolated16.check(original_path, build_folder, names):
        ...

Both are entered the way the function is called (far or near), on the same arguments, with the
original's data segment on both sides. What one function calls -- another of the module's
functions, a Windows function, an interrupt -- is not run: it is recorded (who, with which
argument words) and answered at once with the same made-up result on both sides
(emulate/windows16.py, `callees`). So a function is judged on what it does itself: a callee
rebuilt wrongly, or one that would need the whole of Windows behind it, cannot make it look
wrong, or right.

Compared, for each set of inputs: how the run ends; the calls it makes, in order, with their
arguments; the result (AX, and DX for a function that returns four bytes); what is left in the
data segment; how many bytes of arguments were removed; and whether SI, DI and DS were kept.
Where the two differ is the first thing a reader needs: it says which.

The inputs are the 16-bit judge's (verify/lanes/win16.py): the fixed ones, the directed ones (the
constants the code compares with) and the branch-guided ones (verify/guided.py through
lane_common.guided: arguments, words of the data segment and what an unmodelled Windows call
returns, chosen to take branches of the original no other input took), and the scattered ones
once more over a patterned data segment. CB_NO_GUIDED=1 leaves the branch-guided ones out.
"""
import os
import re

from codebreaker.verify import lane_common
from codebreaker.verify.lanes import win16

_MAP = re.compile(r"^([0-9a-f]{4}):([0-9a-f]{4,8})[+*]?\s+_?([A-Za-z_]\w*?)(?:@\d+)?\s*$")


def _built_places(folder):
    """{name: (segment, offset)} of the functions in a build, from its map."""
    out = {}
    with open(os.path.join(folder, "m.map"), errors="replace") as f:
        for line in f:
            m = _MAP.match(line.strip())
            if m:
                out.setdefault(m.group(3), (int(m.group(1), 16), int(m.group(2), 16)))
    return out


def _functions(m):
    """{name: (bytes of arguments it removes, far)} from the description's F lines."""
    out = {}
    with open(m.description.path, encoding="utf-8") as f:
        for line in f:
            p = line.rstrip("\n").split("\t")
            if p[0] == "F" and len(p) > 4:
                out[p[2]] = (int(p[3]) if p[3].lstrip("-").isdigit() else -1, "far" in p[4])
    return out


def _gdt_span():
    from codebreaker.emulate import windows16 as emu
    return emu.GDT_ADDR, emu.GDT_ADDR + emu._N_GDT * 8


_GDT = _gdt_span()


def _word(w, entry, nxt, places=None):
    """An argument word as what it is, where two builds legitimately differ in its bits."""
    if places and nxt is not None:
        # (the offset half of a far pointer to one of the module's own functions -- a timer's or
        # an enumeration's callback -- is that function: it sits elsewhere in a rebuild. USER's
        # SetCaretBlinkTime passes 13:0D65 in the original, 13:04B6 in the build: the same one)
        from codebreaker.emulate import windows16 as emu
        seg = (nxt >> 3) - emu._MODULE_SLOT + 1
        if (seg, w) in places:
            return "&" + places[(seg, w)]
    if w == entry.get("DS"):
        return "DS"
    if w == entry.get("SS"):
        return "SS"
    if nxt == entry.get("SS") or 0xE000 <= w < 0xFFF0:
        return "stack"                                       # a place in the stack (frames differ)
    return "%x" % w


def _input(v):
    """An input as a verdict names it: its first argument words, and for a branch-guided one
    what else it sets (words of the data segment, what an unmodelled Windows call returns)."""
    out = "args %s" % " ".join("%x" % w for w in v["args"][:4])
    if v.get("guided"):
        extra = ["DS:%04x=%s" % (w[1], bytes(data).hex()) if isinstance(w, tuple) else "%x=%s" % (w, bytes(data).hex())
                 for w, data in (v.get("mem") or ())[:2]]
        if v.get("stub_ret") is not None:
            extra.append("windows returns %x" % v["stub_ret"])
        out = "guided " + out + "".join(", " + e for e in extra)
    return out


def _fingerprint(r, wide, data_len, places=None):
    entry = r.get("entry_regs") or {}
    ex = str(r.get("exit"))
    ex = "fault" if ("exception" in ex or "fault" in ex) else ex
    calls = []
    for c in r.get("calls_out") or ():
        ws = c["args"]
        calls.append("%s(%s)" % (c["name"], ",".join(_word(w, entry, ws[i + 1] if i + 1 < len(ws) else None, places)
                                                     for i, w in enumerate(ws))))
    for h in r.get("imports_hit") or ():
        calls.append("api " + win16._call(h, [], entry))
    lay = r.get("layout") or {}
    ds = lay.get("ds_base")
    stack = lay.get("stack_base")
    state, before = {}, {}
    # (and what it leaves anywhere else but its stack: a caller's buffer reached through a far
    # pointer, a block it was given -- the same place on both sides, the module's own segments
    # sitting at the same bases. The data segment alone hid a function filling a RECT wrongly)
    far, far_before = {}, {}
    for w in r.get("mem_writes") or ():
        a, n, val = w["addr"], w["size"], w["value"] or 0
        if ds is not None and ds <= a < ds + data_len:
            into, at = state, before
            base = ds
        elif stack is not None and stack <= a < stack + 0x10000:
            continue                                         # (its frame, its arguments)
        elif _GDT[0] <= a < _GDT[1]:
            # (the processor marking a descriptor accessed as a selector is loaded -- `lds si, p`
            # sets a bit at GDT+400h for selector 400h -- is not the function's doing: GDI's
            # sub_9_1110 loads DS before a loop that does not run; its C loads nothing)
            continue
        else:
            into, at = far, far_before
            base = 0
        for i in range(min(n, 4)):
            k = a - base + i
            at.setdefault(k, ((w.get("old") or 0) >> (8 * i)) & 0xFF)
            into[k] = (val >> (8 * i)) & 0xFF
    state = {k: x for k, x in state.items() if x != before.get(k)}
    # (through a far pointer: what is written, not whether it changed what was there -- the place
    # may be in the module's own segments, whose bytes the two builds lay out differently: USER's
    # GetCaretPos wrote 0 over 56F8h in the original and over 007Bh in the build, and its high
    # byte "changed" on one side only)
    regs = r.get("exit_regs") or {}
    out = {"exit": ex, "calls": tuple(calls), "data": tuple(sorted(state.items())), "far": tuple(sorted(far.items()))}
    if ex == "ret":
        out["result"] = (r.get("ret32") if wide else r.get("ret"))
        if r.get("unsure"):                                  # a callee removing it is not known how many
            return dict(out, result=out["result"])           # bytes: the stack after it says nothing
        if out["result"] is not None and (out["result"] & 0xFFFF) == entry.get("DS") and not wide:
            out["result"] = "DS"
        from codebreaker.emulate import windows16 as emu
        if isinstance(out["result"], int) and (out["result"] & 0xFFFF) == emu.ENTRY_AX and not wide:
            out["result"] = "none"                           # (AX as it came in: no result set)
        elif isinstance(out["result"], int) and (out["result"] & 0xFF00) == emu.ENTRY_AX & 0xFF00 and not wide:
            out["result"] = ("byte", out["result"] & 0xFF)   # (AL set, AH the caller's: a byte result)
        out["pop"] = r.get("sp_delta")
        out["kept"] = (regs.get("SI") == entry.get("SI"), regs.get("DI") == entry.get("DI"),
                       regs.get("DS") == entry.get("DS"))
    return out


def _differ(a, b):
    """The first way two fingerprints differ, in words, or None."""
    if "truncated" in (a.get("exit"), b.get("exit")):
        # (a run stopped at the instruction cap says nothing past where it stopped: a long loop --
        # a count read from the patterned memory -- ran its 1,052nd pass in the original and not
        # in the build, whose body is a few instructions longer. What both did, the calls up to
        # the shorter run's last, is what compares)
        x, y = a.get("calls") or (), b.get("calls") or ()
        n = min(len(x), len(y))
        if tuple(x[:n]) != tuple(y[:n]):
            i = next(i for i, (p, q) in enumerate(zip(x, y)) if p != q)
            return "calls: original %s, ours %s" % (list(x[i:i + 2]), list(y[i:i + 2]))
        return None
    for k, what in (("exit", "ends"), ("calls", "calls"), ("data", "leaves in the data segment"),
                    ("far", "leaves through a far pointer"),
                    ("result", "returns"), ("pop", "removes argument bytes"), ("kept", "keeps SI, DI, DS")):
        if k == "result" and a.get(k) == "none":
            continue
        if k == "result" and isinstance(a.get(k), tuple) and a[k][0] == "byte":
            mine = b.get(k)                                  # (the original returns a byte: AL is all)
            low = mine[1] if isinstance(mine, tuple) else (mine & 0xFF) if isinstance(mine, int) else mine
            if low == a[k][1]:
                continue                                         # (the original sets none: nothing to match --
                                                             # CopyRect only copies, AX is the caller's)
        if a.get(k) != b.get(k):
            x, y = a.get(k), b.get(k)
            if k == "calls":
                i = next((i for i, (p, q) in enumerate(zip(x, y)) if p != q), min(len(x), len(y)))
                x, y = list(x[i:i + 2]), list(y[i:i + 2])
            elif k in ("data", "far"):
                dx, dy = dict(x), dict(y)
                at = sorted({o for o in set(dx) | set(dy) if dx.get(o) != dy.get(o)})[:4]
                x = ["%04x=%s" % (o, dx.get(o)) for o in at]
                y = ["%04x=%s" % (o, dy.get(o)) for o in at]
            return "%s: original %s, ours %s" % (what, x, y)
    return None


def _part(job):
    """One worker's share of check() (a process of its own)."""
    original, folder, names, image, tables = job
    return list(check(original, folder, names, image=image, workers=1, tables=tables))


def _tables(original):
    """What check() needs of the decompiled module: {funcs, place, wide} (made once, and kept:
    decompiling the whole module again was 85% of a gate run -- four minutes before anything ran).
    Kept by the original and all that makes the decompiled text: the decompiler, the passes over
    its output, what is known of Windows' functions."""
    import glob
    import json
    from codebreaker.base import cache
    from codebreaker.decompile import c_text, decompiler
    from codebreaker.symbols import api_arguments
    root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))   # codebreaker/
    sources = sorted(glob.glob(os.path.join(root, "decompile", "*.py")) + glob.glob(os.path.join(root, "symbols", "*.py")))
    try:
        sizes = open(api_arguments._cache16_path(), "rb").read()
    except OSError:
        sizes = b""
    with open(original, "rb") as f:
        k = cache.key(f.read(), cache.tool(decompiler._exe()), decompiler._language_stamp(), sizes,
                      *[open(p, "rb").read() for p in sources])
    got = cache.get("gate16_tables", k)
    if got is not None:
        return json.loads(got)
    m = c_text.compilable16(original)
    out = {"funcs": _functions(m), "wide": sorted(m.wide),
           "place": {n: list(m.where[va]) for va, n in m.by_va.items() if m.where.get(va)}}
    cache.put("gate16_tables", k, json.dumps(out).encode("utf-8"))
    return out


def _original_key(original, callees, data_end, chain=False):
    """What the original's side of a run depends on besides the function and the input: the
    original, how its callees and Windows' functions are answered, the checker and emulator as
    written."""
    from codebreaker.base import cache
    from codebreaker.emulate import emulator as _emulator
    from codebreaker.emulate import windows16 as emu
    from codebreaker.formats import ne as _ne
    from codebreaker.symbols import api_arguments
    try:
        sizes = open(api_arguments._cache16_path(), "rb").read()
    except OSError:
        sizes = b""
    with open(original, "rb") as f:
        return cache.key(
            f.read(), sizes, repr(sorted((callees or {}).items())), repr(data_end), bool(chain),
            *[open(mod.__file__, "rb").read() for mod in (emu, _emulator, _ne, win16)] + [open(__file__, "rb").read()])


def guided_inputs(original, original_key, name, oseg, ooff, far, span, seeds, callees, data_end, insn_cap=20000):
    """Branch-guided inputs (verify/lanes/win16.py guided_vectors, as the judge makes them):
    arguments, words of the data segment and what an unmodelled Windows call returns, chosen to
    take branches of the original no other input took -- the original run as this checker runs
    it, its callees answered. Kept, keyed on the original's side (_original_key) and the
    explorer's limits; none when CB_NO_GUIDED is set. (diverge16 explains with the same.)"""
    if not lane_common.guided_on():
        return []
    import pickle
    from codebreaker.base import cache
    from codebreaker.verify import guided as _explorer
    k = cache.key(original_key, name, oseg, ooff, far, span, repr(seeds), lane_common.GUIDED_BUDGET,
                  lane_common.MAX_GUIDED, *[open(mod.__file__, "rb").read() for mod in (_explorer, lane_common)])
    got = cache.get("gate16_guided", k)
    if got is not None:
        return pickle.loads(got)
    out = [dict(g, guided=True) for g in win16.guided_vectors(
        original, oseg, ooff, not far, span, seeds, callees=callees, data_end=data_end, insn_cap=insn_cap)]
    cache.put("gate16_guided", k, pickle.dumps(out))
    return out


def check(original, folder, names, image=None, inputs=None, workers=None, tables=None, chain=False):
    """Each named function (one the build has as C) beside the original's. Yields
    (name, "same" | "differs" | "not run", why). Many at once: shared out over processes."""
    names = list(names)
    # (four: the machine is Logan's to work on while it runs -- twelve made it crawl. More than
    # one per hardware thread gains nothing: the run is computation, the emulator calling back
    # into Python on every instruction. CB_GATE_WORKERS for another number)
    workers = workers or max(1, int(os.environ.get("CB_GATE_WORKERS", "4")))
    tables = tables or _tables(original)
    if workers > 1 and len(names) > 2 * workers and inputs is None and not chain:
        # (each share a Python of its own, started fresh: nothing of the caller's script runs again)
        import json
        import subprocess
        import sys
        root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
        code = ("import sys, json; sys.path.insert(0, %r); import capstone; "
                "from codebreaker.verify.gate16 import isolated16 as v; a = json.load(sys.stdin); "
                "print(json.dumps(v._part(a)))" % root)
        import tempfile
        errs = [tempfile.TemporaryFile(mode="w+") for _ in range(workers)]
        procs = [subprocess.Popen([sys.executable, "-c", code], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                  stderr=errs[k], text=True)
                 for k in range(workers)]
        for k, p in enumerate(procs):
            p.stdin.write(json.dumps([original, folder, names[k::workers], image, tables]))
            p.stdin.close()
        for k, p in enumerate(procs):
            out = p.stdout.read()
            p.wait()
            got = set()
            try:
                for item in json.loads(out.strip().splitlines()[-1]):
                    got.add(item[0])
                    yield tuple(item)
            except (ValueError, IndexError):
                pass
            # (a share the worker did not answer for is NOT the same as the original: it was
            # not run, and says why)
            errs[k].seek(0)
            why = (errs[k].read().strip().splitlines() or ["the worker gave no answer"])[-1][:200]
            errs[k].close()
            for n in names[k::workers]:
                if n not in got:
                    yield (n, "not run", "the checker failed: " + why)
        return
    from codebreaker.emulate import windows16 as emu
    from codebreaker.formats.ne import NEFile
    funcs = {n: tuple(v) for n, v in tables["funcs"].items()}
    place = {n: tuple(v) for n, v in tables["place"].items()}
    wide_set = set(tables["wide"])
    ours = _built_places(folder)
    image = image or next(os.path.join(folder, f) for f in os.listdir(folder)
                          if f.lower() == os.path.basename(original).lower())
    nef = NEFile(original)
    auto = nef.h.get("autodata_seg")
    seg = next((s for s in nef.segments() if s["idx"] == auto), None)   # (a driver may have none)
    data = emu.relocated_data(original) if seg and seg["file_off"] else b""   # (fixed up, as loaded)
    # (and the scattered inputs once more over a data segment whose zeros are a pattern, the
    # same on both sides: much of it is zeros at load, and a C function reading the global
    # beside the one it should read the zero beside a zero -- planted so, 9 of 40 went unseen)
    noisy = bytes(b if b else (((i * 0x9E3779B1) >> 13) & 0xFF) | 1 for i, b in enumerate(data))
    noisy_runs = [dict(v, noisy=True) for v in win16.vectors()[-4:]] if data else []
    # (where the original's data ends and its segment ends: past them both sides hold the same)
    data_end = (seg["length"], max(seg["length"], seg.get("minalloc") or 0) or 0x10000) if seg else None
    # (functions both sides know by name: a data symbol -- the build's codebreaker_data at DS:0 --
    # or a name only one side's map has would make "0, DS" a pointer on one side alone)
    named = set(funcs) & set(place) & set(ours)
    places_orig = {tuple(place[n]): n for n in named}
    places_ours = {tuple(ours[n]): n for n in named}
    callees_orig = {place[n]: (n, pop, far) for n, (pop, far) in funcs.items() if n in place}
    callees_ours = {ours[n]: (n, pop, far) for n, (pop, far) in funcs.items() if n in ours}
    insn_cap = 200000 if chain else 20000
    if chain:
        # (chained: the module's other functions run for real on both sides -- what one function
        # leaves for another, through the stack, registers or memory, is then seen)
        callees_orig = callees_ours = None
    # The original's side of a run is the same whatever build it is compared with: kept, keyed on
    # all it depends on (the original, the checker and emulator as written, how its callees and
    # Windows' functions are answered, the input), so a module gated again after a change to the
    # decompiler runs only its own side. (The self-test, the teeth and the build share it too.)
    import pickle
    from codebreaker.base import cache
    original_key = _original_key(original, callees_orig, data_end, chain)

    def side(context, where):
        """A recorded call's context for one side: the module's own code its far pointers reach
        (a callback in the recorded memory) sent to that side's function of the same name."""
        if not context or not context.get("own"):
            return context
        return dict(context, redirect={tuple(place[n]): (where[n][0], where[n][1], n) for n in named})

    def original_run(name, oseg, ooff, far, wide, v, dimg):
        k = cache.key(original_key, name, oseg, ooff, far, wide, tuple(v["args"]), bool(v.get("noisy")),
                      # (a recorded call by the name it was given -- its memory is 64 MB)
                      (v["context"].get("key") or pickle.dumps(v["context"], protocol=4)) if v.get("context") else b"",
                      repr(v.get("mem")), repr(v.get("stub_ret")))
        got = cache.get("gate16_original", k)
        if got is not None:
            return pickle.loads(got)
        a = emu.probe(original, seg=oseg, offset=ooff, args=v["args"], near=not far,
                      callees=callees_orig, insn_cap=insn_cap, data_end=data_end,
                      data_image=dimg, context=side(v.get("context"), place),
                      mem_init=v.get("mem"), stub_ret=v.get("stub_ret"))
        fa = _fingerprint(a, wide, len(data), places_orig)
        cache.put("gate16_original", k, pickle.dumps(fa))
        return fa

    def guided(name, oseg, ooff, far, span, seeds):
        return guided_inputs(original, original_key, name, oseg, ooff, far, span, seeds, callees_orig,
                             data_end, insn_cap)
    for name in names:
        if name not in place or name not in ours or name not in funcs:
            yield name, "not run", "not found on both sides"
            continue
        pop, far = funcs[name]
        wide = name in wide_set
        (oseg, ooff), (bseg, boff) = place[name], ours[name]
        why = None
        # (and inputs aimed at what its own code tests: the constants it compares with)
        later = [o for (sg, o) in place.values() if sg == oseg and o > ooff]
        span = (ooff, min(later) if later else 0x10000)
        aimed = win16.directed(original, oseg, span, most=12)
        # (and the branch-guided ones the judge runs: a branch no fixed or aimed input takes --
        # a global tested before anything is done, a Windows call's result -- hid the path behind it.
        # Looked for only once the others all agree: a function found to differ needs no more)
        def every(fixed):
            yield from fixed
            yield from guided(name, oseg, ooff, far, span, win16.vectors() + aimed)
        for v in (inputs or every(win16.vectors() + aimed + noisy_runs)):
            try:
                # (a call recorded in the running system: its data segment as it was then, both sides)
                dimg = v["context"]["data"] if v.get("context") and v["context"].get("data") else \
                    noisy if v.get("noisy") else None
                fa = original_run(name, oseg, ooff, far, wide, v, dimg)
                b = emu.probe(image, seg=bseg, offset=boff, args=v["args"], near=not far,
                              callees=callees_ours, insn_cap=insn_cap, data_image=dimg or data,
                              data_end=data_end, context=side(v.get("context"), ours),
                              mem_init=v.get("mem"), stub_ret=v.get("stub_ret"))
            except Exception as e:                           # (the emulator itself fell over)
                why = why or "the emulator: %s" % type(e).__name__
                continue
            fb = _fingerprint(b, wide, len(data), places_ours)
            if fa.get("exit") == "fault" and fb.get("exit") != "fault":
                # An input the original faults on (a selector it loads at once, `lds si, p`, that
                # no caller would pass) is no behavior to match: its C, loading a selector only to
                # use it, went on. What the original did before it faulted must still be done.
                x, y = fa.get("calls") or (), fb.get("calls") or ()
                if tuple(y[:len(x)]) != tuple(x):
                    yield name, "differs", "%s: calls: original %s, ours %s" % (_input(v), list(x[:2]), list(y[:2]))
                    break
                continue
            d = _differ(fa, fb)
            if d:
                yield name, "differs", "%s: %s" % (_input(v), d)
                break
        else:
            yield name, ("not run" if why else "same"), why or ""
