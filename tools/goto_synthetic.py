"""The goto shapes on purpose (docs/STRUCTURING_PLAN.md, phase 0): tools/goto_synthetic/shapes.c
-- small functions written to have each flow shape the structurer leaves a goto in -- built into
a DLL with OpenWatcom (optimized, and not), then decompiled, rebuilt and run beside it by the
same pipeline as `python -m codebreaker measure` (verify/reconstruct.py).

    python tools/goto_synthetic.py [--keep]

Per function and build: the gotos in its C, and the verdict (does it behave as the original).
A change to the structurer is to bring the gotos down and keep every verdict. The store is
state/goto_synthetic.sqlite (CB_FUNCTIONS_DB), made anew each run unless --keep.
"""
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

SRC = os.path.join(ROOT, "tools", "goto_synthetic", "shapes.c")
OUT = os.path.join(ROOT, "state", "goto_synthetic")
NAMES = re.compile(r"EXPORT int __stdcall (\w+)\(")
BUILDS = {"opt": ["-ox", "-s"], "plain": ["-od", "-s"]}


def build(flags, tag):
    """the DLL made from shapes.c with these compiler flags"""
    from codebreaker.build import watcom
    os.makedirs(OUT, exist_ok=True)
    env = watcom.ow_env("pe")
    obj = os.path.join(OUT, "shapes_%s.obj" % tag)
    dll = os.path.join(OUT, "shapes_%s.dll" % tag)
    subprocess.run([watcom.ow_tool("wcc386"), "-zq", "-bd", "-bt=nt"] + flags + [SRC, "-fo=" + obj],
                   check=True, env=env, cwd=OUT)
    names = NAMES.findall(open(SRC, encoding="utf-8").read())
    r = subprocess.run([watcom.ow_tool("wlink"), "option", "quiet", "system", "nt_dll", "file", obj, "name", dll],
                       env=env, cwd=OUT, capture_output=True, text=True)
    if r.returncode or not os.path.exists(dll):
        raise SystemExit("link failed: " + (r.stdout + r.stderr)[-600:])
    return dll, names


def main(argv):
    from codebreaker.store import functions as F
    from codebreaker.verify import reconstruct
    db = os.path.join(ROOT, "state", "goto_synthetic.sqlite")
    if "--keep" not in argv and os.path.exists(db):
        os.remove(db)
    os.environ["CB_FUNCTIONS_DB"] = db
    fs = F.open()
    total = 0
    for tag, flags in BUILDS.items():
        dll, names = build(flags, tag)
        sha, digest = reconstruct.run(dll, fs=fs)
        from codebreaker.formats.pe import open_pe
        pe = open_pe(dll)
        at = {name.lstrip("_").split("@")[0]: "%x" % (pe.image_base + rva) for _o, name, rva in pe.exports()}
        rows = {r["addr"]: r for r in fs.candidates(sha, digest)}
        print("== %s (%s)" % (tag, " ".join(flags)))
        for n in names:
            r = rows.get(at.get(n))
            gotos = len(re.findall(r"\bgoto \w+;", (r or {}).get("source") or ""))
            total += gotos
            print("  %-16s gotos %2d   %s %s" % (n, gotos, (r or {}).get("verdict"), ((r or {}).get("reason") or "")[:60]))
    print("total gotos:", total)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
