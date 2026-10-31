#!/usr/bin/env python3
"""gate.py -- formats.dll against the Python readers: every file through both, every fact compared.

    python native/formats/gate.py [--readers pe,ne,...] [--corpus NAME_OR_DIR ...] [--limit N] [--show N]

Each file of each corpus (default: every corpus settings names) goes through a reader's Python
(codebreaker.formats.<reader>) and its C++ (formats.dll, through codebreaker.formats.native),
and every fact the reader gives is compared: every attribute the object carries and every
method's answer -- or, where the reader raises, the exception's name. Values are compared with
their types (a tuple is not a list, True is not 1) and dicts in their order. A file "accepted"
is one the reader opens without raising. Prints, per reader: files, accepted, identical, and
the seconds both sides took over the whole corpus (file reads included, each side reading).
"""
import argparse
import os
import struct
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, ROOT)

from codebreaker.base import settings  # noqa: E402
from codebreaker.formats import native  # noqa: E402


def ename(e):
    return "struct.error" if isinstance(e, struct.error) else type(e).__name__


def canon(x):
    """x with its types spelled out, so == compares what Python's == lets slip (1 == True, (1,) != [1] ...)."""
    if x is None or isinstance(x, (bool, int, float, str, bytes)):
        return (type(x).__name__, x)
    if isinstance(x, bytearray):
        return ("bytearray", bytes(x))
    if isinstance(x, (list, tuple)):
        return (type(x).__name__, tuple(canon(v) for v in x))
    if isinstance(x, dict):
        return ("dict", tuple((canon(k), canon(v)) for k, v in x.items()))
    if isinstance(x, (set, frozenset)):
        return (type(x).__name__, tuple(sorted((canon(v) for v in x), key=repr)))
    if hasattr(x, "__dict__"):
        return ("object", type(x).__name__.replace("Native", ""), canon({k: v for k, v in vars(x).items() if not k.startswith("_")}))
    return ("other", repr(x))


def ask(fn):
    try:
        return canon(fn())
    except Exception as e:                      # noqa: BLE001 -- an exception is a fact
        return ("raise", ename(e))


# ---- the facts of each reader -----------------------------------------------------------------
def pe_facts(open_pe, path, _data):
    try:
        f = open_pe(path)
    except Exception as e:                      # noqa: BLE001
        return False, [("open", ("raise", ename(e)))]
    out = [("attrs", canon({k: v for k, v in vars(f).items() if not k.startswith("_")}))]
    for m in ("machine_name", "is_dll", "code_bytes", "module_name", "export_addrs", "exports", "imports",
              "code_sections", "base_relocs", "build_meta", "debug_codeview", "optional_header", "export_table",
              "resource_tree", "reloc_blocks", "tls", "debug_entries", "certificates", "clr", "rich"):
        out.append((m, ask(getattr(f, m))))
    out.append(("import_table", ask(f.import_table)))
    out.append(("import_table(delay)", ask(lambda: f.import_table(delay=True))))
    rvas = {0, 1, f.entry, 0xFFFFFFFF, -1}
    for s in f.sections:
        rvas.update((s["vaddr"], s["vaddr"] + s["vsize"], s["vaddr"] + s["rawsize"] - 1))
    for va, _sz in f.datadirs[:16]:
        rvas.add(va)
    for r in sorted(rvas):
        out.append(("_rva_to_off(%d)" % r, ask(lambda: f._rva_to_off(r))))
        out.append(("file_offset(%d)" % r, ask(lambda: f.file_offset(r))))
    n = len(f.data)
    for off in (None, -1, 0, 0x3C, f.e_lfanew, n - 9, n - 1, n, n + 5):
        for fmt in "BHIQ":
            out.append(("_u(%s,%s)" % (fmt, off), ask(lambda: f._u(fmt, off))))
        out.append(("_text(%s)" % off, ask(lambda: f._text(off))))
        out.append(("_text(%s,4)" % off, ask(lambda: f._text(off, 4))))
    for i in list(range(17)) + [-1]:
        out.append(("directory(%d)" % i, ask(lambda: f.directory(i))))
    return True, out


def pe_side(open_pe, is_dotnet):
    def facts(path, data):
        ok, out = pe_facts(open_pe, path, data)
        out.append(("is_dotnet", ask(lambda: is_dotnet(path))))
        return ok, out
    return facts


READERS = {}     # reader -> (the Python side's facts(path, data), the C++ side's)


def _register():
    from codebreaker.formats import pe
    READERS["pe"] = (pe_side(pe.PEFile, pe._is_dotnet), pe_side(native.PEFile, native.is_dotnet))


# ---- the corpus -------------------------------------------------------------------------------
def corpus_files(roots, limit=None):
    out = []
    for root in roots:
        for dp, dirs, files in os.walk(root):
            dirs.sort()
            for fn in sorted(files):
                out.append(os.path.join(dp, fn))
                if limit and len(out) >= limit:
                    return out
    return out


def roots_of(names):
    known = settings.corpora()
    if not names:
        return list(dict.fromkeys(known.values()))
    return [known.get(n, n) for n in names]


def gate(reader, files, show=5):
    py, cpp = READERS[reader]
    accepted = identical = 0
    t_py = t_cpp = 0.0
    shown = 0
    for path in files:
        t0 = time.perf_counter()
        ok_py, f_py = py(path, None)
        t1 = time.perf_counter()
        ok_cpp, f_cpp = cpp(path, None)
        t2 = time.perf_counter()
        t_py += t1 - t0
        t_cpp += t2 - t1
        accepted += ok_py
        if f_py == f_cpp:
            identical += 1
            continue
        if shown < show:
            shown += 1
            a, b = dict(f_py), dict(f_cpp)
            diff = [k for k in list(a) + [k for k in b if k not in a] if a.get(k) != b.get(k)]
            print("  DIFF %s: %s" % (path, ", ".join(diff[:6])))
            for k in diff[:2]:
                print("     py : %.300r" % (a.get(k),))
                print("     c++: %.300r" % (b.get(k),))
    return {"files": len(files), "accepted": accepted, "identical": identical, "py": t_py, "cpp": t_cpp}


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument("--readers", default="")
    ap.add_argument("--corpus", nargs="*", default=[])
    ap.add_argument("--limit", type=int, default=None)
    ap.add_argument("--show", type=int, default=5)
    args = ap.parse_args(argv)
    if native.why_not():
        print("formats.dll not usable:", native.why_not())
        return 2
    _register()
    readers = [r for r in args.readers.split(",") if r] or list(READERS)
    roots = roots_of(args.corpus)
    files = corpus_files(roots, args.limit)
    print("corpus: %d files from %s" % (len(files), ", ".join(roots)))
    print("| reader | files | accepted | identical | Python s | C++ s |")
    print("|---|---|---|---|---|---|")
    bad = 0
    for r in readers:
        res = gate(r, files, args.show)
        bad += res["files"] - res["identical"]
        print("| %s | %d | %d | %d | %.2f | %.2f |" % (r, res["files"], res["accepted"], res["identical"],
                                                       res["py"], res["cpp"]))
        sys.stdout.flush()
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
