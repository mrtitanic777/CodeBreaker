"""gen_golden.py -- makes golden/*.asm and golden/facts.json for asm16_test.cpp from the Python original.

    python native/cb/port/testdata/asm16/gen_golden.py        (from the repository root)

The cases are the old harness's synthetic modules (synth.txt: five hand-written modules and the C header and functions they
go with, each ported plain, with a data map and a listing, and the pascal way) and three small real modules of the old C
library 2.20 (lib220_*.asm). asm16_test.cpp builds the same cases and compares the text emitted and the facts. A golden file
is read and judged before it is checked in; the one thing this script adds is the form.
"""
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.abspath(os.path.join(HERE, "..", "..", "..", "..", "..")))

from codebreaker.port import asm16  # noqa: E402

OUT = os.path.join(HERE, "golden")


def collapse(s):
    return re.sub(r"\s+", " ", s).strip()


def make_listing(m):
    """a synthetic old listing: a place for each label, and for each table line of the code"""
    off = {}
    for n, idx in m.labels.items():
        off[n] = (m.code[idx].seg, 16 * idx)
    off["@lines"] = []
    for i, l in enumerate(m.code):
        if l.directive and l.directive[0] in ("DB", "DW", "DD"):
            off["@lines"].append((l.seg, 16 * i + 1, 2, collapse(asm16.strip_comment(l.text))[:24],
                                  bytes([0x8B, 0x1E, 0x34, 0x12][: (i % 4) + 1])))
    return off


def facts(m):
    return {
        "labels": list(m.labels.items()),
        "cname": sorted(m.cname.items()),
        "pascal": sorted(m.pascal),
        "called": sorted(m.called),
        "far": sorted(x for x in m.far if x is not None),
        "own_ptr": sorted(m.own_ptr),
        "params": [[("" if r is None else r), [list(x) for x in v]] for r, v in
                   sorted(m.params.items(), key=lambda kv: (kv[0] is not None, kv[0] or ""))],
        "unreached": list(m.unreached),
        "asm_calls": sorted(m.asm_calls),
        "c_calls": sorted(m.c_calls),
        "data_used": sorted(m.data_used),
        "notes": list(m.notes),
        "used_int21": m.used_int21,
        "used_null": getattr(m, "used_null", False),
    }


def main():
    os.makedirs(OUT, exist_ok=True)
    texts = open(os.path.join(HERE, "synth.txt"), encoding="latin1").read().split("\n@@@@\n")
    mods = {"m1": texts[0], "m2": texts[1], "m3": texts[2], "m4": texts[3], "m5": texts[4]}
    hdr, cfun = texts[5], texts[6]
    ct = asm16.CTypes([hdr], [hdr])
    cf = asm16.c_functions([cfun.replace("PASC", "pascal")])
    calls = ["walk", "sum", "lenp", "strx", "far1", "dosit", "p2", "p3", "bad", "q", "q5", "cfun", "other"]
    all_facts = {}

    def save(case, m, longret=()):
        try:
            m.run()
            text = m.emit(longret)
        except asm16.Asm16Error as e:        # (a module the port cannot carry over: the complaint is the golden)
            all_facts[case] = {"error": str(e)}
            return
        open(os.path.join(OUT, case + ".asm"), "w", encoding="latin1", newline="\n").write(text)
        all_facts[case] = facts(m)

    for name, tx in mods.items():
        for variant in range(3):
            m = asm16.Port(asm16.module(name, tx), ct, cnames=calls, cfuncs=cf, asm_routines=["_other"] if variant == 1 else [],
                           pascal={"sum"} if variant == 2 else (), underscore=variant != 1)
            m.cname = dict(sorted(m.cname.items()))
            if variant:
                m.param_types = {p: [(2, "struct node"), (4, None), (2, None)] for p in m.cname}
                m.far = {"strx"} if variant == 1 else set()
                m.data_map = [(0x100, "_val"), (0x200, "_arr"), (0x230, "_ptrs")]
                m.aliases = {"val": ("arr", 4)}
                m.used_outside = {"_walk"} if variant == 2 else set()
                m.offsets = make_listing(m)
            save("synth_%s_%d" % (name, variant), m, {"far1"} if variant == 2 else ())
    # the old C library's own assembly: no C program (the headers are empty)
    empty = asm16.CTypes([""], [""])
    for case, fname, cnames, others in (("lib220_strlen", "lib220_strlen.asm", ["strlen"], []),
                                        ("lib220_string", "lib220_string.asm", ["strcpy", "strcat"], ["_strlen"]),
                                        ("lib220_movbytes", "lib220_movbytes.asm", ["movbytes"], [])):
        tx = open(os.path.join(HERE, fname), encoding="latin1").read()
        m = asm16.Port(asm16.module(fname, tx), empty, cnames=cnames, asm_routines=others)
        m.cname = dict(sorted(m.cname.items()))
        save(case, m)
    json.dump(all_facts, open(os.path.join(OUT, "facts.json"), "w", encoding="utf-8", newline="\n"), indent=1, sort_keys=True)
    sys.stderr.write("%d cases\n" % len(all_facts))


if __name__ == "__main__":
    main()
