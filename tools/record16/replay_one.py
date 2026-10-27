"""One recorded call through isolated16.check's own path, chained, printing both runs' calls."""
import os, sys, pickle, zlib
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from codebreaker.base import corpus
from codebreaker.emulate import windows16 as emu
from codebreaker.verify.gate16 import isolated16
import memmap16
pkl, folder = sys.argv[1], sys.argv[2]
r = pickle.load(open(pkl, "rb"))
mem = r["memory"]
snap = bytearray(zlib.decompress(open(os.path.join(os.path.dirname(pkl), mem["base"]), "rb").read()))
for i, pg in mem["pages"].items():
    snap[i * 4096:(i + 1) * 4096] = pg
orig = corpus.retail("setupx.dll")
sigs = {k: v for k, v in memmap16.signatures(orig).items() if v[1] == "CODE"}
own = {f["selector"]: sg for sg, f in memmap16.locate(bytes(snap), mem["ldt"][0], mem["ldt"][1], orig, sigs=sigs).items()}
ctx = {k: r[k] for k in ("stack", "sp", "ss", "ds", "es", "regs", "ldt", "data_sel", "data")}
ctx.update(snapshot=bytes(snap), ldt_at=mem["ldt"], own=own, key="dbg")
tables = isolated16._tables(orig)
place = {n: tuple(v) for n, v in tables["place"].items()}
ours = isolated16._built_places(folder)
named = set(place) & set(ours)
funcs = {n: tuple(v) for n, v in tables["funcs"].items()}
print(r["name"], "own", {hex(k): v for k, v in own.items()})
for label, path, where in (("original", orig, place), ("ours", os.path.join(folder, "setupx.dll"), ours)):
    c = dict(ctx, redirect={place[n]: (where[n][0], where[n][1], n) for n in named})
    sg, off = where[r["name"]]
    a = emu.probe(path, seg=sg, offset=off, args=[], near=not r["far"], data_image=r["data"], context=c, insn_cap=200000)
    print(label, a["exit"], "insns", a["insn_count"], "exit regs", a["exit_regs"], a.get("fault"))
    print("   ", [(h.get("api"), h.get("arg_bytes")) for h in a["imports_hit"]][:16])
