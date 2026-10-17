"""The golden comparison: the decompiler core's C for every function of a fixed corpus, before and
after a change to the core (docs/CORE_ROADMAP.md).

    python tools/golden.py freeze [NAME ...]    describe each module once, keep the description
                                                (the fixed input) and the core's output
    python tools/golden.py check  [NAME ...]    run the core as it is now on the same input:
                                                per module, functions the same / changed / new /
                                                gone, and the time; the differences are written to
                                                state/golden/<module>/diff.txt

The input is frozen with the output, so a change to describe.py does not show here -- this is
the core's own test. Each run is held by the decompiler's governor (CB_MEMORY_MB)."""
import difflib
import json
import os
import re
import shutil
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.base import settings                       # noqa: E402
from codebreaker.decompile import decompiler, describe       # noqa: E402

MINI = os.path.join(ROOT, "state", "mini", "original")
CORPUS = settings.get("corpus") or ""
MODULES = {                                                   # name -> (class, path)
    "USER": ("ne", os.path.join(MINI, "USER.EXE")),
    "GDI": ("ne", os.path.join(MINI, "GDI.EXE")),
    "KRNL386": ("ne", os.path.join(MINI, "KRNL386.EXE")),
    "VGA": ("ne", os.path.join(MINI, "VGA.DRV")),
    "KEYBOARD": ("ne", os.path.join(MINI, "KEYBOARD.DRV")),
    "COMM": ("ne", os.path.join(MINI, "COMM.DRV")),
    "LZEXPAND": ("ne", os.path.join(MINI, "LZEXPAND.DLL")),
    "KERNEL32": ("pe", os.path.join(CORPUS, "WINDOWS", "SYSTEM", "kernel32.dll")),
    "GDI32": ("pe", os.path.join(CORPUS, "WINDOWS", "SYSTEM", "gdi32.dll")),
    "VKD": ("le", os.path.join(CORPUS, "WINDOWS", "SYSTEM", "vkd.vxd")),
    "COMMAND": ("dos", os.path.join(CORPUS, "setup", "inf", "command.com")),
}
STORE = os.path.join(ROOT, "state", "golden")
_MARK = re.compile(r"(?ms)^/\*@@ (\S+) ([0-9a-f]+) \*/\n(.*?)(?=^/\*@@ |\Z)")


def _descriptions(name, kind, path, folder):
    """[(member, description path)] -- made fresh in `folder`."""
    if kind == "ne":
        return [(name, describe.describe16(path, folder).path)]
    if kind == "dos":
        return [(name, describe.describe_dos(path, folder).path)]
    if kind == "pe":
        return [(name, describe.describe32(path, describe.function_starts(path), folder)[0])]
    if kind == "le":
        from codebreaker.formats import le
        return [(n, describe.describe_le(lf, n, os.path.join(folder, re.sub(r"\W", "_", n))).path)
                for n, lf in le.open_vxds(path)]
    raise ValueError(kind)


def _run(desc):
    t = time.time()
    out, err, _whole = decompiler._run_resuming(desc, (), 3600)
    funcs = {m.group(2): m.group(3).strip() for m in _MARK.finditer(out)}
    return funcs, time.time() - t, err


def freeze(names):
    for name in names:
        kind, path = MODULES[name]
        if not os.path.isfile(path):
            print("%-10s no file: %s" % (name, path))
            continue
        folder = os.path.join(STORE, name)
        shutil.rmtree(folder, ignore_errors=True)
        os.makedirs(folder)
        members = _descriptions(name, kind, path, os.path.join(folder, "input"))
        snap = {}
        total = 0.0
        for member, desc in members:
            funcs, secs, _err = _run(desc)
            snap[member] = {"description": desc, "functions": funcs}
            total += secs
        json.dump(snap, open(os.path.join(folder, "golden.json"), "w"), indent=0)
        print("%-10s frozen: %d functions, %.0fs" % (name, sum(len(m["functions"]) for m in snap.values()), total),
              flush=True)


def check(names):
    for name in names:
        g = os.path.join(STORE, name, "golden.json")
        if not os.path.isfile(g):
            print("%-10s not frozen" % name)
            continue
        snap = json.load(open(g))
        same = changed = new = gone = 0
        total = 0.0
        lines = []
        for member, m in snap.items():
            now, secs, err = _run(m["description"])
            total += secs
            old = m["functions"]
            for va in sorted(set(old) | set(now)):
                a, b = old.get(va), now.get(va)
                if a == b:
                    same += 1
                elif a is None:
                    new += 1
                elif b is None:
                    gone += 1
                else:
                    changed += 1
                    lines += ["=== %s %s" % (member, va)] + list(difflib.unified_diff(
                        a.splitlines(), b.splitlines(), lineterm="", n=1))[2:]
        with open(os.path.join(STORE, name, "diff.txt"), "w") as f:
            f.write("\n".join(lines))
        print("%-10s same %d, changed %d, new %d, gone %d  (%.0fs)" % (name, same, changed, new, gone, total),
              flush=True)


if __name__ == "__main__":
    cmd, names = sys.argv[1], [n.upper() for n in sys.argv[2:]] or list(MODULES)
    {"freeze": freeze, "check": check}[cmd](names)
