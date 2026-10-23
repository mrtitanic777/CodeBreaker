"""Run the calls record.py recorded again, the original's function beside a build's, on what each
call had: python replay.py <rec tag> <build folder> [module=setupx.dll] [names,...]
A call recorded with the whole of memory is run chained: its callees run too, on that memory.
Prints each function: same / differs (why) / not run, and the count of each."""
import glob
import os
import pickle
import sys
import zlib
from collections import defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
from codebreaker.base import corpus, settings
from codebreaker.verify import isolated16
sys.path.insert(0, HERE)
import memmap16

tag, folder = sys.argv[1], sys.argv[2]
module = sys.argv[3] if len(sys.argv) > 3 and sys.argv[3] != "-" else "setupx.dll"
only = set(sys.argv[4].split(",")) if len(sys.argv) > 4 else None
original = corpus.retail(module)
bases = {}
_sigs = {}


def own_code(snap, ldt):
    """{selector: code segment} of the module's own code in a recorded memory."""
    if "s" not in _sigs:
        _sigs["s"] = {k: v for k, v in memmap16.signatures(original).items() if v[1] == "CODE"}
    found = memmap16.locate(snap, ldt[0], ldt[1], original, sigs=_sigs["s"])
    return {f["selector"]: sg for sg, f in found.items()}


def snapshot(mem, rec_dir):
    if mem["base"] not in bases:
        bases.clear()                                       # (one 64 MB base at a time)
        bases[mem["base"]] = zlib.decompress(open(os.path.join(rec_dir, mem["base"]), "rb").read())
    out = bytearray(bases[mem["base"]])
    for i, page in mem["pages"].items():
        out[i * 4096:(i + 1) * 4096] = page
    return bytes(out)


calls = defaultdict(list)
for p in sorted(glob.glob(os.path.join(settings.state("record16"), "rec", tag, "*.pkl"))):
    r = pickle.load(open(p, "rb"))
    if only and r["name"] not in only:
        continue
    calls[r["name"]].append(p)
tables = isolated16._tables(original)
count = defaultdict(int)
for name in sorted(calls):
    for p in calls[name]:
        r = pickle.load(open(p, "rb"))
        ctx = {k: r[k] for k in ("stack", "sp", "ss", "ds", "es", "regs", "ldt", "data_sel", "data")}
        ctx["key"] = "%s:%s" % (tag, os.path.basename(p))
        mem = r.get("memory")
        if mem:
            ctx["snapshot"] = snapshot(mem, os.path.dirname(p))
            ctx["ldt_at"] = mem["ldt"]
            ctx["own"] = r.get("own") or own_code(ctx["snapshot"], mem["ldt"])
        v = {"args": r["words"][:max(0, r["pop"]) // 2], "context": ctx}
        for got in isolated16.check(original, folder, [name], inputs=[v], workers=1, tables=tables,
                                    chain=bool(mem)):
            count[got[1]] += 1
            print("%-28s %-3s %-8s %s" % (got[0], os.path.basename(p).rsplit("_", 1)[1][:-4], got[1], got[2][:160]), flush=True)
print(dict(count))
