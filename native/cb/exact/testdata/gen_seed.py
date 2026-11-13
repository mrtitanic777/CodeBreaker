"""gen_seed.py <outfile> -- run from the repo root with the Python package built (the oracle for seed_test; goes with the Python).

Runs the live Python of codebreaker/exact/seed.py on real projects of the Microsoft-Assemblers-Compilers research and records what it answered, as
exact/seed.cpp must answer it: split() of real C sources, seeds() of five pairs of versions (the count, a SHA-1 of each file's bytes -- the "seeded from"
line's path written with '/' -- and two files in full), and main() --write on a copy of a project (the text, the files written).
"""
import contextlib
import hashlib
import io
import json
import os
import shutil
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", "..", ".."))
MAIN = os.environ.get("CB_MAIN_CHECKOUT", "C:/Users/mrtit/Documents/GitHub/CodeBreaker")
sys.path.insert(0, ROOT)
from codebreaker.base import settings as _settings  # noqa: E402
_settings.ROOT = MAIN
_settings.FILE = os.path.join(MAIN, "settings.json")
_settings._loaded = None
R = "C:/Users/mrtit/Documents/GitHub/Microsoft-Assemblers-Compilers/research/"
from codebreaker.exact import project, seed  # noqa: E402

out = sys.argv[1]
doc = {}


def seeded(text):
    """The file's text with the path of its "seeded from" line written with '/' (the one divergence)."""
    head, rest = text.split("\n", 1)
    return head.replace("\\", "/") + "\n" + rest


def sha(text):
    return hashlib.sha1(text.encode("latin1")).hexdigest()


def capture(fn):
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        rc = fn()
    return [buf.getvalue(), rc]


PAIRS = [("msc-3.0/recon/p1", "msc-3.0/recon/p0"), ("msc-3.0/recon/p0", "msc-3.0/recon/p1"), ("msc-3.0/recon/p2", "msc-3.0/recon/p1"),
         ("lib-2.40", "lib-2.20"), ("link-2.44", "link-2.40")]
doc["seeds"] = {}
for a, b in PAIRS:
    p, o = project.load(R + a), project.load(R + b)
    s = seed.seeds(p, o)
    keys = sorted(s)
    doc["seeds"]["%s <- %s" % (a, b)] = {"count": len(s), "addresses": keys, "sha": [sha(seeded(s[k])) for k in keys],
                                          "full": [[k, seeded(s[k])] for k in keys[:2]]}

doc["split"] = {}
for b in ("msc-3.0/recon/p0", "link-2.40", "lib-2.20"):
    o = project.load(R + b)
    done = 0
    for part in o.sets():
        for at, path in part.sources():
            if path.lower().endswith(".c") and done < 4:
                text = open(path, encoding="latin1").read()
                decls, funcs, types = seed.split(text)
                doc["split"]["%s %s" % (b, os.path.basename(path))] = {
                    "decls": [len(decls), sha("\n".join(decls))], "funcs": [[n, sha(t)] for n, t in funcs.items()], "types": list(types.items())}
                done += 1

tmp = os.path.join(HERE, "..", "..", "..", "..", "state", "work", "w67", "seed-copy")
shutil.rmtree(tmp, ignore_errors=True)
shutil.copytree(R + "lib-2.40", tmp)
pj = json.load(open(os.path.join(tmp, "project.json")))
pj["image"] = os.path.abspath(os.path.join(R + "lib-2.40", pj["image"]))
json.dump(pj, open(os.path.join(tmp, "project.json"), "w"), indent=1)
p = project.load(tmp)
before = sorted(os.listdir(p.src)) if os.path.isdir(p.src) else []
quiet = capture(lambda: seed.main(p, ["--from", R + "lib-2.20"]))
res = capture(lambda: seed.main(p, ["--from", R + "lib-2.20", "--write"]))
after = sorted(os.listdir(p.src))
new = [f for f in after if f not in before]
doc["main"] = {"quiet": quiet, "text": res[0].split("\nwritten:")[0] + "\n", "rc": res[1], "written": res[0].split("written: ")[1].split(" ")[0],
               "new": [[f, sha(seeded(open(os.path.join(p.src, f), "rb").read().decode("latin1")))] for f in new if f.lower().endswith(".c")],
               "headers": [f for f in new if not f.lower().endswith(".c")]}
doc["main_no_from"] = capture(lambda: seed.main(p, []))
shutil.rmtree(tmp, ignore_errors=True)
json.dump(doc, open(out, "w"), indent=1)
