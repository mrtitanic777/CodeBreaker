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


SPENT = [0.0]               # seconds inside the readers (the comparison's own work not counted)


def timed(fn, *args):
    t0 = time.perf_counter()
    try:
        return fn(*args)
    finally:
        SPENT[0] += time.perf_counter() - t0


def ask(fn):
    try:
        return canon(timed(fn))
    except Exception as e:                      # noqa: BLE001 -- an exception is a fact
        return ("raise", ename(e))


# ---- the facts of each reader -----------------------------------------------------------------
def pe_facts(open_pe, path, _data):
    try:
        f = timed(open_pe, path)
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


def pe_all(path, data):
    from codebreaker.formats import pe
    ok, out = pe_facts(pe.open_pe, path, data)
    out.append(("is_dotnet", ask(lambda: pe.is_dotnet(path))))
    return ok, out


def ne_module_facts(f, prefix=""):
    from codebreaker.formats import ne
    out = [(prefix + "attrs", canon({k: v for k, v in vars(f).items() if not k.startswith("_")}))]
    for m in ("resident_names", "nonresident_names", "module_name", "module_refs", "entries", "export_ordinals",
              "resources", "segments"):
        out.append((prefix + m, ask(getattr(f, m))))
    try:
        segs = f.segments()
    except Exception:                           # noqa: BLE001
        segs = []
    for s in segs:
        out.append((prefix + "relocations(%d)" % s["idx"], ask(lambda: f.relocations(s))))
    out.append((prefix + "relocations(nodict)", ask(lambda: f.relocations({"flags": 0x100}))))
    h = f.h
    out.append((prefix + "imported_names", ask(lambda: [f.imported_name(f.u16(f.ne_off + h["mod_tab"] + 2 * i))
                                                        for i in range(h["n_modrefs"])])))
    for off in (-1, 0, 1, 5000, 10 ** 6):
        out.append((prefix + "imported_name(%d)" % off, ask(lambda: f.imported_name(off))))
    n = len(f.data)
    for off in (0, 0x3C, f.ne_off, n - 3, n - 2, n - 1, n, -2):
        out.append((prefix + "u16(%d)" % off, ask(lambda: f.u16(off))))
        out.append((prefix + "u32(%d)" % off, ask(lambda: f.u32(off))))
    out.append((prefix + "_read_name_table(ne_off)", ask(lambda: f._read_name_table(f.ne_off))))
    out.append((prefix + "_rsrc_string", ask(lambda: f._rsrc_string(f.ne_off + h["rsrc_tab"], 6))))
    out.append((prefix + "_tables_end", ask(lambda: ne._tables_end(f))))
    out.append((prefix + "standalone", ask(lambda: ne.standalone(f))))
    return out


def ne_all(path, data):
    from codebreaker.formats import ne
    out = []
    with open(path, "rb") as fh:
        whole = fh.read()
    out.append(("embedded", ask(lambda: ne.embedded(whole))))
    out.append(("embedded(0)", ask(lambda: ne.embedded(whole, 0))))
    try:
        f = timed(ne.NEFile, path)
        ok = True
        out += ne_module_facts(f)
    except Exception as e:                      # noqa: BLE001
        ok = False
        out.append(("open", ("raise", ename(e))))
    try:
        mods = timed(ne.embedded, whole)
    except Exception:                           # noqa: BLE001
        mods = []
    if mods:
        for off, _name in mods[:8]:             # each module read from its own header (no MZ stub)
            try:
                m = timed(ne.NEFile.from_buffer, whole, off, path)
                out += ne_module_facts(m, "buf%x." % off)
            except Exception as e:              # noqa: BLE001
                out.append(("buf%x" % off, ("raise", ename(e))))
        if path.lower().endswith(".bin"):       # a Windows 1.x boot image: modules over resident + overlay
            try:
                for name, m in timed(ne.open_boot_image, path).items():
                    out += ne_module_facts(m, "boot.%s." % name)
            except Exception as e:              # noqa: BLE001
                out.append(("boot", ("raise", ename(e))))
    return ok, out


READERS = {"pe": pe_all, "ne": ne_all}     # reader -> its facts(path, data), asked once of each side


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


CALLS = [0]                 # questions the C++ was asked (the Python side must ask none)
_real_call = native.call


def _counted_call(*a, **k):
    CALLS[0] += 1
    return _real_call(*a, **k)


native.call = _counted_call


def side(reader, on, facts, path):
    """facts(path) with the reader's switch set: on = the C++, off = the Python."""
    key = "CB_NATIVE_FORMATS_" + reader.upper()
    old = os.environ.get(key)
    os.environ[key] = "1" if on else "0"
    before = CALLS[0]
    try:
        return facts(path, None)
    finally:
        if not on and CALLS[0] != before:
            raise AssertionError("the Python side of %s asked the C++ (%s)" % (reader, path))
        if on:
            SIDE_CALLS[0] += CALLS[0] - before
        if old is None:
            del os.environ[key]
        else:
            os.environ[key] = old


SIDE_CALLS = [0]


def gate(reader, files, show=5):
    facts = READERS[reader]
    SIDE_CALLS[0] = 0
    accepted = identical = 0
    t_py = t_cpp = 0.0
    shown = 0
    for path in files:
        t0 = SPENT[0]
        ok_py, f_py = side(reader, False, facts, path)
        t1 = SPENT[0]
        ok_cpp, f_cpp = side(reader, True, facts, path)
        t2 = SPENT[0]
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
    return {"files": len(files), "accepted": accepted, "identical": identical, "py": t_py, "cpp": t_cpp,
            "asked": SIDE_CALLS[0]}


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
    readers = [r for r in args.readers.split(",") if r] or list(READERS)
    roots = roots_of(args.corpus)
    files = corpus_files(roots, args.limit)
    print("corpus: %d files from %s" % (len(files), ", ".join(roots)))
    print("| reader | files | accepted | identical | Python s | C++ s | questions asked of the C++ |")
    print("|---|---|---|---|---|---|---|")
    bad = 0
    for r in readers:
        res = gate(r, files, args.show)
        bad += res["files"] - res["identical"] + (0 if res["asked"] else 1)
        print("| %s | %d | %d | %d | %.2f | %.2f | %d |" % (r, res["files"], res["accepted"], res["identical"],
                                                            res["py"], res["cpp"], res["asked"]))
        sys.stdout.flush()
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
