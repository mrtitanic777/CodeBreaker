"""isolated16.py -- each function of a rebuilt 16-bit module run beside the original's, on its own.

    from codebreaker.verify import isolated16
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
"""
import hashlib
import os
import re

from codebreaker.verify import win16

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


def _word(w, entry, nxt):
    """An argument word as what it is, where two builds legitimately differ in its bits."""
    if w == entry.get("DS"):
        return "DS"
    if w == entry.get("SS"):
        return "SS"
    if nxt == entry.get("SS") or 0xE000 <= w < 0xFFF0:
        return "stack"                                       # a place in the stack (frames differ)
    return "%x" % w


def _fingerprint(r, wide, data_len):
    entry = r.get("entry_regs") or {}
    ex = str(r.get("exit"))
    ex = "fault" if ("exception" in ex or "fault" in ex) else ex
    calls = []
    for c in r.get("calls_out") or ():
        ws = c["args"]
        calls.append("%s(%s)" % (c["name"], ",".join(_word(w, entry, ws[i + 1] if i + 1 < len(ws) else None)
                                                     for i, w in enumerate(ws))))
    for h in r.get("imports_hit") or ():
        calls.append("api " + win16._call(h, [], entry))
    ds = (r.get("layout") or {}).get("ds_base")
    state, before = {}, {}
    for w in r.get("mem_writes") or ():
        a, n, val = w["addr"], w["size"], w["value"] or 0
        if ds is None or not ds <= a < ds + data_len:
            continue
        for i in range(min(n, 4)):
            k = a - ds + i
            before.setdefault(k, ((w.get("old") or 0) >> (8 * i)) & 0xFF)
            state[k] = (val >> (8 * i)) & 0xFF
    state = {k: x for k, x in state.items() if x != before.get(k)}
    regs = r.get("exit_regs") or {}
    out = {"exit": ex, "calls": tuple(calls), "data": tuple(sorted(state.items()))}
    if ex == "ret":
        out["result"] = (r.get("ret32") if wide else r.get("ret"))
        if r.get("unsure"):                                  # a callee removing it is not known how many
            return dict(out, result=out["result"])           # bytes: the stack after it says nothing
        if out["result"] is not None and (out["result"] & 0xFFFF) == entry.get("DS") and not wide:
            out["result"] = "DS"
        out["pop"] = r.get("sp_delta")
        out["kept"] = (regs.get("SI") == entry.get("SI"), regs.get("DI") == entry.get("DI"),
                       regs.get("DS") == entry.get("DS"))
    return out


def _differ(a, b):
    """The first way two fingerprints differ, in words, or None."""
    for k, what in (("exit", "ends"), ("calls", "calls"), ("data", "leaves in the data segment"),
                    ("result", "returns"), ("pop", "removes argument bytes"), ("kept", "keeps SI, DI, DS")):
        if a.get(k) != b.get(k):
            x, y = a.get(k), b.get(k)
            if k == "calls":
                i = next((i for i, (p, q) in enumerate(zip(x, y)) if p != q), min(len(x), len(y)))
                x, y = list(x[i:i + 2]), list(y[i:i + 2])
            elif k == "data":
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
    """What check() needs of the decompiled module: {funcs, place, wide} (made once)."""
    from codebreaker.decompile import c_text
    m = c_text.compilable16(original)
    return {"funcs": _functions(m), "wide": sorted(m.wide),
            "place": {n: list(m.where[va]) for va, n in m.by_va.items() if m.where.get(va)}}


def check(original, folder, names, image=None, inputs=None, workers=None, tables=None):
    """Each named function (one the build has as C) beside the original's. Yields
    (name, "same" | "differs" | "not run", why). Many at once: shared out over processes."""
    names = list(names)
    workers = workers or min(os.cpu_count() or 4, 12)
    tables = tables or _tables(original)
    if workers > 1 and len(names) > 2 * workers and inputs is None:
        # (each share a Python of its own, started fresh: nothing of the caller's script runs again)
        import json
        import subprocess
        import sys
        root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
        code = ("import sys, json; sys.path.insert(0, %r); import capstone; "
                "from codebreaker.verify import isolated16 as v; a = json.load(sys.stdin); "
                "print(json.dumps(v._part(a)))" % root)
        procs = [subprocess.Popen([sys.executable, "-c", code], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                  stderr=subprocess.DEVNULL, text=True)
                 for _ in range(workers)]
        for k, p in enumerate(procs):
            p.stdin.write(json.dumps([original, folder, names[k::workers], image, tables]))
            p.stdin.close()
        for p in procs:
            out = p.stdout.read()
            p.wait()
            try:
                for item in json.loads(out.strip().splitlines()[-1]):
                    yield tuple(item)
            except (ValueError, IndexError):
                pass
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
    seg = next(s for s in nef.segments() if s["idx"] == auto)
    data = bytes(nef.data[seg["file_off"]:seg["file_off"] + seg["length"]]) if seg["file_off"] else b""
    callees_orig = {place[n]: (n, pop, far) for n, (pop, far) in funcs.items() if n in place}
    callees_ours = {ours[n]: (n, pop, far) for n, (pop, far) in funcs.items() if n in ours}
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
        aimed = win16.directed(original, oseg, (ooff, min(later) if later else 0x10000), most=12)
        for v in (inputs or win16.vectors() + aimed):
            try:
                a = emu.probe(original, seg=oseg, offset=ooff, args=v["args"], near=not far,
                              callees=callees_orig, insn_cap=20000)
                b = emu.probe(image, seg=bseg, offset=boff, args=v["args"], near=not far,
                              callees=callees_ours, insn_cap=20000, data_image=data)
            except Exception as e:                           # (the emulator itself fell over)
                why = why or "the emulator: %s" % type(e).__name__
                continue
            fa, fb = _fingerprint(a, wide, len(data)), _fingerprint(b, wide, len(data))
            d = _differ(fa, fb)
            if d:
                yield name, "differs", "args %s: %s" % (" ".join("%x" % w for w in v["args"][:4]), d)
                break
        else:
            yield name, ("not run" if why else "same"), why or ""
