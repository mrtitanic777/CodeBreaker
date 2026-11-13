"""gen_w5b3.py <outfile> -- run from the repo root with the Python package built (the oracle for carve_test, bss_test, names_test, link_test and
run_test; goes with the Python).

Runs the live Python of codebreaker/exact/{carve,bss,names,link,run}.py on real projects of the Microsoft-Assemblers-Compilers research
(the exact projects: C 3.00's P0-P3, LIB, LINK, MASM ...) and records what it answered, as the C++ stages must answer it:

  carve    functions() and region() of six projects; main() -v; _hexrx over the profiles' patterns and odd ones
  bss      propose() and lines() of five projects; main()
  names    known(), functions(), carry() of three pairs of versions, main() -v and the SYMBOLS --write makes
  link     library_start, dosname, parts/diagnose/drift on the original and on a damaged copy, the objects of the kit (first_objects, the
           BSS objects), opts_of; crt on the objects of the kit
  run      --toolchains

The kits are the checkout's (settings.json); the research folder is the sibling of the checkout.
"""
import contextlib
import hashlib
import io
import json
import os
import shutil
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", "..", ".."))        # the worktree
MAIN = os.environ.get("CB_MAIN_CHECKOUT", "C:/Users/mrtit/Documents/GitHub/CodeBreaker")
sys.path.insert(0, ROOT)
from codebreaker.base import settings as _settings  # noqa: E402
_settings.ROOT = MAIN                                    # (settings.json, the kits and the state folder are the main checkout's)
_settings.FILE = os.path.join(MAIN, "settings.json")
_settings._loaded = None
R = "C:/Users/mrtit/Documents/GitHub/Microsoft-Assemblers-Compilers/research/"
from codebreaker.exact import bss, carve, link, names, project, run  # noqa: E402

out = sys.argv[1]
doc = {}


def capture(fn):
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        rc = fn()
    return [buf.getvalue(), rc]


def sha(b):
    return hashlib.sha1(b).hexdigest() if b is not None else None


CARVE = ["msc-3.0/recon/p0", "msc-3.0/recon/p1", "lib-3.00", "link-2.44", "msc-1.0x/src/mc1", "masm-5.10"]
doc["carve"] = {}
for name in CARVE:
    p = project.load(R + name)
    lo, hi, skip = carve.region(p)
    doc["carve"][name] = {"functions": [[a, ln, why] for a, ln, why in carve.functions(p)], "region": [lo, hi, [list(x) for x in skip]],
                          "main": capture(lambda: carve.main(p, ["-v"])), "quiet": capture(lambda: carve.main(p, []))}
pats = ["55 8b ec", "55 8b ec 57 56", "55 8b ec (?:33c0|b8....) e8 (....)", "..", "ab c", "a", "(", "", "5 5", ".", "ab.", "(?:..|ff)+?",
        "2e 2a 5b 5d 5c 7c 28 29 3f 2b 7b 7d 5e 24", "AB cd EF", "0g", "g0"]
import re  # noqa: E402
subject = bytes(project.load(R + "msc-3.0/recon/p0").image[:6000])
doc["hexrx"] = []
for pt in pats:
    try:
        rx = re.compile(carve._hexrx(pt), re.S)
        every = [[m.start(), m.end(), m.start(1) if m.groups() else None] for m in rx.finditer(subject)]
        found = {"n": len(every), "first": every[:40], "last": every[-5:]}
    except re.error:
        found = "error"
    doc["hexrx"].append([pt, found])
doc["hexrx_subject"] = hashlib.sha1(subject).hexdigest()
# crafted: starts_add / starts_drop / code_start / no frames, on copies of P0
doc["carve_crafted"] = {}
src = R + "msc-3.0/recon/p0"
for key, edit in (("add_drop", {"starts_add": [16, "0x20"], "starts_drop": ["0x0", 4]}), ("code_start", {"code_start": "0x100"}), ("past", {"starts_add": [60000]})):
    d = os.path.join(HERE, "..", "..", "..", "..", "state", "work", "w5b3-crafted")
    shutil.rmtree(d, ignore_errors=True)
    shutil.copytree(src, d)
    pj = json.load(open(os.path.join(d, "project.json")))
    pj["image"] = os.path.abspath(os.path.join(src, pj["image"]))
    pj.update(edit)
    json.dump(pj, open(os.path.join(d, "project.json"), "w"), indent=1)
    p = project.load(d)
    try:
        doc["carve_crafted"][key] = [[a, ln, why] for a, ln, why in carve.functions(p)]
    except Exception as e:
        doc["carve_crafted"][key] = "error %s" % type(e).__name__
    shutil.rmtree(d, ignore_errors=True)

BSS = ["msc-3.0/recon/p0", "msc-3.0/recon/p2", "lib-3.00", "masm-5.10", "link-2.44"]
doc["bss"] = {}
for name in BSS:
    p = project.load(R + name)
    edata, stack, nm, holes = bss.propose(p)
    doc["bss"][name] = {"propose": [edata, stack, sorted(nm.items()), [[a, ln, list(n)] for a, ln, n in holes]],
                        "lines": bss.lines(p, edata, stack, nm, holes), "main": capture(lambda: bss.main(p, []))}
    doc["bss"][name]["communal"] = p.toolchain().p.get("communal")

PAIRS = [("lib-2.40", "lib-2.20"), ("link-2.44", "link-2.40"), ("msc-3.0/recon/p2", "msc-3.0/recon/p1")]
doc["names"] = {}
for a, b in PAIRS:
    pa, pb = project.load(R + a), project.load(R + b)
    nm, dn = names.carry(pa, pb)
    doc["names"]["%s <- %s" % (a, b)] = {
        "names": sorted([k, v[0], v[1]] for k, v in nm.items()), "data": sorted([k, v[0], v[1]] for k, v in dn.items()),
        "pairs": [list(x) for x in names.carry.pairs.items()], "dpairs": [list(x) for x in names.carry.dpairs.items()],
        "main": capture(lambda: names.main(pa, ["--from", R + b, "-v"])), "quiet": capture(lambda: names.main(pa, ["--from", R + b]))}
for name in ("lib-2.40", "link-2.44", "msc-3.0/recon/p0", "masm-1.00/recon/exact/c100"):
    p = project.load(R + name)
    doc["names"]["known " + name] = [[sorted(x.items()) for x in names.known(p)], [sorted(x.items()) for x in names.known(p, starts=set())],
                                    names.functions(p)[:40], [names.read(p, a, n)[:6] for a, n in names.functions(p)[:2]]]
# --write into a copy
tmp = os.path.join(HERE, "..", "..", "..", "..", "state", "work", "w5b3-names")
shutil.rmtree(tmp, ignore_errors=True)
shutil.copytree(R + "lib-2.40", tmp)
pj = json.load(open(os.path.join(tmp, "project.json")))
pj["image"] = os.path.abspath(os.path.join(R + "lib-2.40", pj["image"]))
json.dump(pj, open(os.path.join(tmp, "project.json"), "w"), indent=1)
p = project.load(tmp)
before = open(p.path("SYMBOLS"), "rb").read().decode("latin1") if os.path.exists(p.path("SYMBOLS")) else None
res = capture(lambda: names.main(p, ["--from", R + "lib-2.20", "--write"]))
doc["names_write"] = {"before": before, "after": open(p.path("SYMBOLS"), "rb").read().decode("latin1"), "main": [res[0].replace(os.path.abspath(tmp).replace("\\", "/"), "<D>").replace(tmp, "<D>"), res[1]]}
shutil.rmtree(tmp, ignore_errors=True)

LINKS = ["msc-3.0/recon/p0", "msc-3.0/recon/p1", "lib-3.00"]
doc["link"] = {}
for name in LINKS:
    p = project.load(R + name)
    rec = {"library_start": link._library_start(p), "opts": link.opts_of(p), "dosnames": [[n, link.dosname(n)] for n in ("_exit", "exit", "BINMODE", "a b.c", "###", "longername12", "")]}
    fo = link.first_objects(p)
    rec["first_objects"] = [[n, sha(b)] for n, b in fo]
    bo = link.bss_objects(p)
    rec["bss_objects"] = [[n, sha(b)] for n, b in bo]
    rec["bss_after"] = bool(getattr(link.bss_objects, "after", False))
    lib, miss = link.crt(p, fo + bo)
    rec["crt"] = [[[m, sha(b)] for m, b in lib], miss, list(link.crt.conflicts)]
    exe = p.packed_file if p.packed else p.file
    rec["parts"] = None
    try:
        h, ib, rb = link.parts(exe)
        rec["parts"] = [list(h), sha(ib), len(ib), [list(x) for x in rb[:5]], len(rb)]
    except Exception as e:
        rec["parts"] = "%s" % type(e).__name__
    if not p.packed:
        rec["diagnose_same"] = link.diagnose(p, exe, exe)
        bad = bytearray(exe)                                  # (a damaged copy: bytes of the code, of the library part and of the data)
        h = struct.unpack_from("<14H", exe, 0)
        base = h[4] * 16
        for off in (0x40, 0x41, p.own_end() + 5, p.dgroup + 7, p.dgroup + 8, p.dgroup + 300):
            bad[base + off] ^= 0x55
        bad = bytes(bad)
        rec["damaged_sha"] = sha(bad)
        rec["diagnose_damaged"] = link.diagnose(p, exe, bad)
        shifted = exe[:base + p.dgroup + 20] + b"\x01\x02" + exe[base + p.dgroup + 20:]   # (the data moved by two bytes)
        rec["diagnose_shifted"] = link.diagnose(p, exe, shifted)
        rec["drift"] = link.drift(p.image[p.dgroup:], p.image[p.dgroup:][:100] + b"\x07\x07" + p.image[p.dgroup:][100:])
    doc["link"][name] = rec
a = bytes((i * 7 + (i >> 3)) & 0xFF for i in range(600))
doc["drift_synthetic"] = {"a": sha(a), "cases": []}
for cut in (100, 301, 450):
    b = a[:cut] + b"\xAA\xBB" + a[cut:]
    doc["drift_synthetic"]["cases"].append([cut, link.drift(a, b), link.drift(a, b, 4, 8, 64)])
doc["run_toolchains"] = capture(lambda: run.main(["--toolchains"]))
json.dump(doc, open(out, "w"), sort_keys=True, separators=(",", ":"))
