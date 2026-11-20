"""gen_win16_lane.py <worktree> <outfile> -- run from the repo root with the Python package built:

    python -B src/verify/lanes/testdata/gen_win16_lane.py . <outfile>

Runs the live Python of codebreaker/verify/lanes/win16.py on a real 16-bit module (LZEXPAND.DLL: src/formats/testdata) and records what it answered,
as verify/lanes/win16.cpp must: the fixed inputs, the fingerprints of every exported function on the first ten of them (how each run ended, AX, the
calls and port accesses in order, the memory left, the stack's move, whether SI DI DS were kept, the offsets of the function's own instructions that ran,
the path of its own jumps), the spans of the functions, the directed inputs, the caller-frame places of a helper's C, and the verdict of each exported
function beside itself and beside another function of the module standing in for its rebuild (a rebuild that does something else).
"""
import json
import os
import sys

sys.path.insert(0, ".")
from codebreaker.base import settings as _settings  # noqa: E402
MAIN = os.environ.get("CB_MAIN_CHECKOUT", "C:/Users/mrtit/Documents/GitHub/CodeBreaker")
_settings.ROOT = MAIN
_settings.FILE = os.path.join(MAIN, "settings.json")
_settings._loaded = None
from codebreaker.verify.lanes import win16  # noqa: E402

wt, out = sys.argv[1], sys.argv[2]
path = os.path.abspath(wt + "/src/formats/testdata/lzexpand.dll")
nef = win16.P.loader.open(path)
auto = nef.h.get("autodata_seg")
entries = [(e["seg"], e["offset"]) for e in nef.entries() if e.get("seg")]
places = []
for at in entries:
    if at not in places:
        places.append(at)
where = {i: at for i, at in enumerate(places)}
spans = win16._spans(nef, where)
doc = {"auto": auto, "places": places, "spans": {str(i): [list(sp), n] for i, (sp, n) in spans.items()}}

vectors = win16.vectors()
doc["vectors"] = [v["args"] for v in vectors[:10]]
doc["n_vectors"] = len(vectors)

doc["fingerprints"] = {}
for i, (seg, off) in enumerate(places):
    span = spans[i][0]
    rows = []
    for v in vectors[:10]:
        f = win16.fingerprint(path, seg, off, False, False, v, span=span)
        rows.append({"exit": f.get("exit"), "ret": f.get("ret"), "deps": list(f.get("deps") or ()), "writes": f.get("writes"), "pop": f.get("pop"),
                     "saved": f.get("saved"), "cov": list(f.get("cov") or ()), "trail": [list(t) for t in f.get("trail") or ()], "err": f.get("err")})
    doc["fingerprints"]["%d:%x" % (seg, off)] = rows

sizes = {}                                              # the bytes of arguments each Windows function removes, as the run found them
for i, (seg, off) in enumerate(places):
    for v in vectors[:10]:
        r = win16.P.emulator.run(path, seg=seg, offset=off, args=v["args"], near=False, ports=True)
        for h in r.raw.get("imports_hit") or ():
            if h.get("arg_bytes") is not None:
                sizes[h["api"]] = h["arg_bytes"]
doc["sizes"] = sizes

doc["directed"] = {}
for i, (seg, off) in enumerate(places[:6]):
    d = win16.directed(path, seg, spans[i][0])
    doc["directed"]["%d:%x" % (seg, off)] = [v["args"] for v in d]

doc["frames"] = [
    ["void sub_9_0A62(frame_4,frame_6)\nchar __far *frame_6;\nshort __far *frame_4;\n{\n}", "sub_9_0A62", win16.frame_slots("void sub_9_0A62(frame_4,frame_6)\n"
        "int2 __far *frame_4;\nint1 __far *frame_6;\n{\n}\n", "sub_9_0A62")],
    ["void f(int a)\n{\n}", "f", win16.frame_slots("void f(int a)\n{\n}", "f")],
]

doc["judge"] = {}
data_image = b""
for i, (seg, off) in enumerate(places):
    key = "%d:%04x" % (seg, off)
    span, insns = spans[i]
    other = places[(i + 1) % len(places)]
    for what, (bseg, boff) in (("self", (seg, off)), ("other", other)):
        item = (key, seg, off, False, False, span, insns, bseg, boff)
        v, reason, limits, detail = win16._judge((path, path, data_image, (), {seg: seg, bseg: bseg}), item)
        doc["judge"]["%s %s" % (key, what)] = [v, reason, list(limits), detail]
json.dump(doc, open(out, "w"), indent=1)
print("written", out, len(doc["judge"]), "judgements")
