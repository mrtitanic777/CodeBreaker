"""The C text made two ways, compared: decompile/c_text.py as it is in a git revision (default
master) and as it is in the working tree, on the same decompiler output (the module is decompiled
once; both are given a copy). The gate for a change to c_text.py that must not change its output.

    python tools/ctext_ab.py [--rev REV] [MODULE ...]       (default: a fixed set, 16-bit and 32-bit)

Prints, per module, the functions whose final text differs (and the header, if it does), with
the first differing line; exit code 1 if anything differs. Functions that differ only in spacing
(blank lines, indentation) are counted apart and do not fail the comparison."""
import argparse
import copy
import importlib.util
import os
import subprocess
import sys
import tempfile
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

MODULES16 = ["lzexpand.dll", "keyboard.drv", "comm.drv", "system.drv", "ver.dll", "commdlg.dll", "mmsystem.dll",
             "shell.dll", "user.exe", "gdi.exe", "krnl386.exe", "toolhelp.dll", "olecli.dll", "ddeml.dll"]
MODULES32 = ["lz32.dll", "notepad.exe", "winmine.exe", "comdlg32.dll", "shell32.dll"]


def _old_module(rev):
    src = subprocess.run(["git", "-C", ROOT, "show", "%s:codebreaker/decompile/c_text.py" % rev],
                         capture_output=True, check=True).stdout
    folder = tempfile.mkdtemp(prefix="ctext_ab_")
    path = os.path.join(folder, "c_text_old.py")
    with open(path, "wb") as f:
        f.write(src)
    spec = importlib.util.spec_from_file_location("c_text_old", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def _memoized_decompiler():
    """decompile16 / raw made once per module and handed out as copies (the C text changes what
    it is given)."""
    from codebreaker.decompile import decompiler
    memo = {}
    for name in ("decompile16", "raw"):
        real = getattr(decompiler, name)

        def wrapped(module_path, *a, _real=real, _name=name, **kw):
            k = (_name, module_path)
            if k not in memo:
                memo[k] = _real(module_path, *a, **kw)
            return copy.deepcopy(memo[k])
        setattr(decompiler, name, wrapped)


def _outputs(mod, path, kind, work):
    if kind == "16":
        m = mod._compilable16(path, work)
        return m.header, dict(m.texts)
    from codebreaker.verify.lanes import win32
    header, texts, _order, _by_va, _used, _ecx, _edx = mod.compilable32(path, win32.LANES["pe"].functions(path), work)
    return header, dict(texts)


def _squeezed(s):
    return "".join(s.split())


def _first_diff(a, b):
    al, bl = a.split("\n"), b.split("\n")
    for i, (x, y) in enumerate(zip(al, bl)):
        if x != y:
            return "line %d:\n      %s: %s\n      now: %s" % (i + 1, "was", x[:150], y[:150])
    return "%d lines vs %d" % (len(al), len(bl))


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--rev", default="master")
    ap.add_argument("modules", nargs="*")
    a = ap.parse_args(argv)
    os.environ["CB_NO_CACHE"] = "1"
    from codebreaker.base import corpus
    from codebreaker.decompile import c_text as new
    old = _old_module(a.rev)
    _memoized_decompiler()
    mods = a.modules or (MODULES16 + MODULES32)
    bad = 0
    funcs = 0
    for name in mods:
        path = corpus.retail(name)
        if not path:
            print("%-14s not in the corpus" % name)
            continue
        kind = "32" if name in MODULES32 or name.lower().endswith((".exe",)) and name not in MODULES16 else "16"
        t = time.time()
        with tempfile.TemporaryDirectory(prefix="ctext_ab_") as work:
            try:
                h0, t0 = _outputs(old, path, kind, work)
            except Exception as e:
                print("%-14s the old one failed: %s" % (name, e))
                continue
            h1, t1 = _outputs(new, path, kind, work)
        diffs = [n for n in sorted(set(t0) | set(t1)) if t0.get(n) != t1.get(n)]
        spaces = [n for n in diffs if _squeezed(t0.get(n, "")) == _squeezed(t1.get(n, ""))]
        diffs = [n for n in diffs if n not in spaces]
        funcs += len(t0)
        print("%-14s %4d functions  %s%s  (%.0fs)" % (name, len(t0), "same" if not diffs and h0 == h1 else
                                                       "%d differ%s" % (len(diffs), ", header differs" if h0 != h1 else ""),
                                                       ("; %d only in spacing" % len(spaces)) if spaces else "",
                                                       time.time() - t))
        if h0 != h1:
            print("    header " + _first_diff(h0, h1))
        for n in diffs[:5]:
            print("    %s %s" % (n, _first_diff(t0.get(n, ""), t1.get(n, ""))))
        bad += bool(diffs or h0 != h1)
    print("%d functions in %d modules; %s" % (funcs, len(mods), "ALL THE SAME" if not bad else "%d modules differ" % bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
