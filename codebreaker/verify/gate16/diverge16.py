"""diverge16.py -- where a function rebuilt from C first stops doing what the original does.

    from codebreaker.verify.gate16 import diverge16
    for name, report in diverge16.explain(original_path, build_folder, names):
        ...
    python -m codebreaker.verify.gate16.diverge16 ORIGINAL BUILD_FOLDER NAME [NAME ...]

The gate (isolated16) says THAT a function differs: how its run ends, which call or result or
byte of data is not the original's. This says WHERE, and of what kind. Each side is run the way
the gate runs it, on the input the gate found differing, and each run is read as one stream of
what it can be seen to do, in order:

    rd   a read of the data segment (offset, size, value) or of an argument (its slot)
    wr   a write to the data segment (offset, size, value)
    call another of the module's functions, with its argument words
    api  a Windows function, with its argument words

The two streams are walked together; the first event where they part is the divergence, with
the original's instruction there and what each side did. Its class says what to look at:

    reads another place     -- an offset or a field the C computes wrongly (a struct, a global)
    reads another value     -- the same place, but what was there differs (an earlier write)
    writes another value    -- the same place written, a different value (arithmetic, width, sign)
    writes another place    -- a store gone to the wrong offset
    calls with other args   -- a call's arguments built wrongly (an argument computed, or missing)
    calls another function  -- control flow went another way (a condition)
    ends early / goes on    -- one side stopped where the other went on (a fault, a missing return)
"""
import os
import sys

from codebreaker.verify.gate16 import isolated16

from codebreaker.verify.lanes import win16


def _events(rec, data_len, entry_sp, stack_base, wide_words=None):
    """The run as an ordered stream of what can be compared between builds."""
    lay = rec.get("layout") or {}
    ds = lay.get("ds_base")
    entry = rec.get("entry_regs") or {}
    ev = []
    for kind, bucket in (("rd", rec.get("mem_reads") or ()), ("wr", rec.get("mem_writes") or ())):
        for m in bucket:
            a = m["addr"]
            if ds is not None and ds <= a < ds + data_len:
                ev.append((m.get("n", 0), kind, ("ds", a - ds, m["size"]), m.get("value"), m.get("pc")))
            elif stack_base is not None and stack_base + entry_sp <= a < stack_base + 0x10000:
                # (above the return address: the arguments -- the same slots on both sides)
                ev.append((m.get("n", 0), kind, ("arg", a - stack_base - entry_sp, m["size"]), m.get("value"), m.get("pc")))
    for c in rec.get("calls_out") or ():
        ws = c["args"]
        args = tuple(isolated16._word(w, entry, ws[i + 1] if i + 1 < len(ws) else None) for i, w in enumerate(ws))
        ev.append((c.get("n", 0), "call", c["name"], args, c.get("ret_ip")))
    for h in rec.get("imports_hit") or ():
        ev.append((h.get("n", 0), "api", win16._call(h, [], entry), None, h.get("ret_ip")))
    ev.sort(key=lambda e: (e[0], {"rd": 0, "wr": 1, "call": 2, "api": 2}[e[1]]))
    return ev


def _effects(ev):
    """What a reader would call the function's effects: writes, calls -- reads only as context.
    A write to its own arguments is not one: they are its own to use (OpenWatcom keeps locals
    there where the original's compiler did not), and the gate does not count them either."""
    return [e for e in ev if e[1] != "rd" and not (e[1] == "wr" and e[2][0] == "arg")]


def _show(e):
    if e is None:
        return "nothing more"
    n, kind, what, val, pc = e
    if kind in ("rd", "wr"):
        where = "%s+%04x/%d" % (what[0], what[1], what[2])
        return "%s %s = %s (at %s)" % (kind, where, "?" if val is None else "%x" % val, "%04x" % (pc & 0xFFFF) if pc else "?")
    if kind == "call":
        return "call %s(%s) (from %s)" % (what, ",".join(val), "%04x" % pc if pc is not None else "?")
    return "api %s" % what


def _classify(a, b, reads_a, reads_b):
    if a is None or b is None:
        return "ends early / goes on"
    if a[1] != b[1]:
        return "calls another function" if "call" in (a[1], b[1]) or "api" in (a[1], b[1]) else "does another thing"
    if a[1] == "wr":
        if a[2] != b[2]:
            return "writes another place"
        # (the value differs: were its inputs the same?)
        ra = {(r[2], r[3]) for r in reads_a}
        rb = {(r[2], r[3]) for r in reads_b}
        pa, pb = {r[2] for r in reads_a}, {r[2] for r in reads_b}
        if pa != pb:
            return "reads another place"
        if ra != rb:
            return "reads another value"
        return "writes another value"
    if a[1] in ("call", "api"):
        if a[2] != b[2]:
            return "calls another function"
        pa, pb = {r[2] for r in reads_a}, {r[2] for r in reads_b}
        if pa != pb:
            return "reads another place"
        return "calls with other args"
    return "differs"


def explain_one(original, image, oplace, bplace, pop_far, args, data, callees_orig, callees_ours, wide=False,
                data_end=None, mem=None, stub_ret=None):
    """-> the report, or None where the gate would find this input the same on both sides. `mem`,
    `stub_ret`: a branch-guided input's words of memory and what an unmodelled Windows call returns."""
    from codebreaker.emulate import windows16 as emu
    pop, far = pop_far
    a = emu.probe(original, seg=oplace[0], offset=oplace[1], args=args, near=not far, callees=callees_orig,
                  insn_cap=20000, data_end=data_end, mem_init=mem, stub_ret=stub_ret)
    b = emu.probe(image, seg=bplace[0], offset=bplace[1], args=args, near=not far, callees=callees_ours,
                  insn_cap=20000, data_image=data, data_end=data_end, mem_init=mem, stub_ret=stub_ret)
    # (the gate's own judgement picks the input: what it does not count -- DX of a function
    # returning a word -- is not a divergence here either)
    gate = isolated16._differ(isolated16._fingerprint(a, wide, len(data), {k: v[0] for k, v in (callees_orig or {}).items()}),
                              isolated16._fingerprint(b, wide, len(data), {k: v[0] for k, v in (callees_ours or {}).items()}))
    if not gate:
        return None
    r = _explain_runs(a, b, data, wide)
    r["gate"] = gate
    return r


def _explain_runs(a, b, data, wide):
    sb = (a.get("layout") or {}).get("stack_base")
    ea = _events(a, len(data), _entry_sp(a), sb)
    eb = _events(b, len(data), _entry_sp(b), sb)
    fa, fb = _effects(ea), _effects(eb)
    k = 0
    while k < len(fa) and k < len(fb) and fa[k][1:4] == fb[k][1:4]:
        k += 1
    if k == len(fa) == len(fb):
        ends = (str(a.get("exit")), str(b.get("exit")))
        if ends[0] != ends[1]:
            return {"class": "ends early / goes on", "original": ends[0], "ours": ends[1], "at": k,
                    "fault": b.get("fault") or a.get("fault")}
        ra, rb = (a.get("ret32"), b.get("ret32")) if wide else (a.get("ret"), b.get("ret"))
        if ra != rb:
            return {"class": "returns another value", "original": "%x" % (ra or 0), "ours": "%x" % (rb or 0), "at": k}
        return {"class": "same effects (differs in what is kept or removed)", "at": k}
    x = fa[k] if k < len(fa) else None
    y = fb[k] if k < len(fb) else None
    na = x[0] if x else (ea[-1][0] + 1 if ea else 0)
    nb = y[0] if y else (eb[-1][0] + 1 if eb else 0)
    prev_a = fa[k - 1][0] if k else -1
    prev_b = fb[k - 1][0] if k else -1
    reads_a = [r for r in ea if r[1] == "rd" and prev_a < r[0] <= na]
    reads_b = [r for r in eb if r[1] == "rd" and prev_b < r[0] <= nb]
    return {"class": _classify(x, y, reads_a, reads_b), "at": k,
            "original": _show(x), "ours": _show(y),
            "reads_original": [_show(r) for r in reads_a][-6:], "reads_ours": [_show(r) for r in reads_b][-6:],
            "same_before": [_show(e) for e in fa[max(0, k - 2):k]]}


def _entry_sp(rec):
    """The stack pointer the function was entered with (where its return address is)."""
    sp = rec.get("sp_delta")
    regs = rec.get("entry_regs") or {}
    return 0xFFF0 - 2 * len((rec.get("input") or {}).get("args") or ()) - 4      # (as _enter lays it out)


def explain(original, folder, names, image=None):
    """(name, report) for each named function the build has as C."""
    tables = isolated16._tables(original)
    funcs = {n: tuple(v) for n, v in tables["funcs"].items()}
    place = {n: tuple(v) for n, v in tables["place"].items()}
    ours = isolated16._built_places(folder)
    image = image or next(os.path.join(folder, f) for f in os.listdir(folder)
                          if f.lower() == os.path.basename(original).lower())
    from codebreaker.formats.ne import NEFile
    from codebreaker.emulate import windows16 as emu
    nef = NEFile(original)
    auto = nef.h.get("autodata_seg")
    seg = next((s for s in nef.segments() if s["idx"] == auto), None)
    data = emu.relocated_data(original) if seg and seg["file_off"] else b""   # (fixed up, as loaded)
    data_end = (seg["length"], max(seg["length"], seg.get("minalloc") or 0) or 0x10000) if seg else None
    callees_orig = {place[n]: (n, pop, far) for n, (pop, far) in funcs.items() if n in place}
    callees_ours = {ours[n]: (n, pop, far) for n, (pop, far) in funcs.items() if n in ours}
    wide_set = set(tables["wide"])
    key = [None]                                             # (the original's side, made once)
    for name in names:
        if name not in place or name not in ours or name not in funcs:
            yield name, {"class": "not found on both sides"}
            continue
        oseg, ooff = place[name]
        later = [o for (sg, o) in place.values() if sg == oseg and o > ooff]
        span = (ooff, min(later) if later else 0x10000)
        aimed = win16.directed(original, oseg, span, most=12)
        report = None

        # (and the gate's branch-guided inputs, the same ones (isolated16.guided_inputs): a function
        # the gate found to differ only on one of them was otherwise "same effects" here)
        def every():
            yield from win16.vectors() + aimed
            if key[0] is None:
                key[0] = isolated16._original_key(original, callees_orig, data_end)
            yield from isolated16.guided_inputs(original, key[0], name, oseg, ooff, funcs[name][1], span,
                                                win16.vectors() + aimed, callees_orig, data_end)
        for v in every():
            try:
                r = explain_one(original, image, place[name], ours[name], funcs[name], v["args"], data,
                                callees_orig, callees_ours, name in wide_set, data_end,
                                mem=v.get("mem"), stub_ret=v.get("stub_ret"))
            except Exception as e:
                r = {"class": "the emulator: %s: %s" % (type(e).__name__, e)}
            if r is not None:
                report = dict(r, input=isolated16._input(v))
                break
        yield name, report or {"class": "same effects"}


if __name__ == "__main__":
    for n, r in explain(sys.argv[1], sys.argv[2], sys.argv[3:]):
        print("%s: %s" % (n, r.get("class")))
        for k in ("input", "gate", "original", "ours", "reads_original", "reads_ours", "same_before", "fault"):
            if r.get(k):
                print("    %-15s %s" % (k, r[k]))
