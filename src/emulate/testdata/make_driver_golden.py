"""Runs the Python emulate.driver on real VxDs and writes what the C++ test compares: src/emulate/testdata/driver.golden.json.

Run from the main checkout (it has the native DLLs):
    python -B src/emulate/testdata/make_driver_golden.py <worktree>
"""
import json
import sys

sys.path.insert(0, '.')
from codebreaker.emulate import driver as d

wt = sys.argv[1]
M64 = (1 << 64) - 1


def fold(values):
    h = 0xCBF29CE484222325
    for v in values:
        for k in range(8):
            h ^= (v >> (8 * k)) & 0xFF
            h = (h * 0x100000001B3) & M64
    return h


def summary(rec):
    ws = rec["mem_writes"]
    return {
        "exit": rec["exit"], "eax": rec["eax"], "flags": rec["flags"], "registers": list(rec["registers"]),
        "esp_delta": rec["esp_delta"], "insns": rec["insns"], "outside": rec["outside"], "truncated": rec["truncated"],
        "calls": [[c[0], c[1], list(c[2]), list(c[3])] for c in rec["calls"]],
        "ports": [list(p) for p in rec["ports"]],
        "cover_len": len(rec["cover"]), "cover_fold": fold(sorted(rec["cover"])),
        "writes_len": len(ws), "writes_fold": fold([v for w in ws for v in (w["addr"], w["size"], w["value"])]),
        "reads_len": len(rec["mem_reads"]), "reads_fold": fold([r["addr"] for r in rec["mem_reads"]]),
    }


def memory():
    seq = bytes(range(256))
    return [(d.VM, bytes(0x100)), (d.CLIENT, bytes(0x100)), (d.BUFFER, seq + seq), (d.OBJECT, bytes(0x400)),
            (d.SAFE, b"\xc3" * 16)]


vectors = {
    "zeros": dict(registers={"ebx": d.VM, "ebp": d.CLIENT}, stack=[], memory=memory()),
    "words": dict(registers={"eax": 1, "ebx": d.VM, "ecx": 0x1234, "edx": d.BUFFER, "esi": d.OBJECT, "edi": d.SAFE, "ebp": d.CLIENT},
                  stack=[1, 2, 3], memory=memory()),
    "service": dict(registers={"eax": 0, "ebx": d.VM, "ebp": d.CLIENT}, stack=[0, d.BUFFER], memory=memory(), service_eax=0x55, service_carry=1, flags=1),
    "cap": dict(registers={"ebx": d.VM, "ebp": d.CLIENT}, stack=[], memory=memory(), cap=50),
}

out = {}
for name in ("bios.vxd", "splitter.vxd", "srammtd.vxd"):
    image = d.load(wt + "/src/formats/testdata/" + name)
    entry = {
        "digest": image.digest, "shape": list(image.shape),
        "entries": [[k, v] for k, v in image.entries.items()],
        "regions": [[a, len(b), fold(list(b))] for a, b in image.regions],
        "spans": [list(s) for s in image.spans],
        "runs": {},
    }
    for label, address in list(image.entries.items())[:14]:
        for vname, kw in vectors.items():
            try:
                rec = d.run(image, address, **kw)
            except Exception as e:
                entry["runs"]["%s %s" % (label, vname)] = {"error": type(e).__name__}
                continue
            entry["runs"]["%s %s" % (label, vname)] = summary(rec)
    out[name] = entry

with open(wt + '/src/emulate/testdata/driver.golden.json', 'w') as f:
    json.dump(out, f, sort_keys=True, separators=(",", ":"))
print({k: len(v["runs"]) for k, v in out.items()})
