"""make_cases.py -- the golden data of native/cb/port/old_c_data_test.cpp.

Runs the ORIGINAL Python (codebreaker.port.pre_ansi.Port) over real source trees and hand-written ones, one pass
at a time and as the chain Port.modernize runs, and writes for each case:
  testdata/old_c/<case>/manifest.json    the Port's state when the passes start (files, ds, bss, reach, profile)
  testdata/old_c/<case>/originals/<f>    the sources as given
  testdata/old_c/<case>/files/<f>        the files the Port holds after __init__ that differ from the originals
  testdata/old_c/<case>/expected.json    what each pass leaves: changed files (digests), the note lists, ...
Usage: python make_cases.py [--full-dump DIR]   (the full texts of every changed file go to DIR for debugging)
It re-runs itself with PYTHONHASHSEED=0 (the port's own passes iterate sets of names). Set P2_EXTRA=ALL (or a comma list of
trees of the ports folder) to write the cases of those trees to cases/ beside this file instead (CB_P2_EXTRA names that folder).
"""
import hashlib
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", "..", "..", ".."))      # (the checkout: native/cb/port/testdata/old_c)
sys.path.insert(0, ROOT)
from codebreaker.port import pre_ansi                                     # noqa: E402
from codebreaker.port import __main__ as pm                               # noqa: E402

PORTS = os.environ.get("CB_PORTS", r"C:\Users\mrtit\Documents\GitHub\Microsoft-Assemblers-Compilers\ports")   # (the real trees)
RUNTIMES = os.path.join(ROOT, "codebreaker", "port", "runtimes")
OUT = HERE
SYN = os.path.join(OUT, "synthetic")

METHODS = ["agree_globals", "fixed_allocators", "array_bounds", "word_stacks", "merge_tables", "numbered_tables",
           "size_tables", "pool_blocks", "local_copies", "byte_places", "shared_blocks", "word_copies"]
CHAIN = ["word_copies", "shared_blocks", "byte_places", "local_copies", "pool_blocks", "size_tables",
         "numbered_tables", "array_bounds", "word_stacks", "merge_tables", "fixed_allocators", "agree_globals"]
LISTS = ["agree_notes", "long_notes", "alloc_notes", "table_notes", "notes_word_copies", "notes_shared",
         "notes_byte_places", "notes_local_copies", "notes_pools", "notes_size_tables"]

REAL = {"msc-3.00-p3": "dos_msc3", "msc-3.00-p0": "dos_msc3"}
SYNTHETIC = {"agree": "dos_msc3", "bounds": "dos_msc3", "allocs": "dos_msc3", "tables": "dos_msc3", "words": "dos_msc3",
             "tables2": "dos_msc3", "words2": "dos_msc3", "tables3": "dos_msc3", "tables3b": "dos_msc3"}
# the same sources under other settings: {case: (sources, environment)}
VARIANTS = {"agree-rules-off": ("agree", {"CB_PORT_RULES": "-agree_ptr_lists,-agree_ptr_defs,-agree_longs"})}
# synthetic cases that name variables by their place: {case: {name: address}}
DS = {"bounds": {"g_123c": 0x123c, "g_13cc": 0x13cc, "g_13d6": 0x13d6}}


def fnv(s):
    h = 0xcbf29ce484222325
    for b in s.encode("utf-8"):
        h = ((h ^ b) * 0x100000001b3) & 0xFFFFFFFFFFFFFFFF
    return "%016x" % h


def profile(name):
    return json.load(open(os.path.join(RUNTIMES, name + ".json")))


def make(files, prof, ds, bss, reach):
    p = pre_ansi.Port(dict(files), prof)
    p.ds = dict(ds)
    p.bss = dict(bss)
    p.reach = dict(reach)
    return p


def record(p, before, tag, full):
    changed = {n: fnv(p.files[n]) for n in sorted(p.files) if p.files[n] != before.get(n)}
    rec = {"tag": tag, "changed": changed,
           "lists": {k: list(getattr(p, k, [])) for k in LISTS},
           "notes": [[k, list(v)] for k, v in p.notes.items()],
           "aliases": [[k, v[0], v[1]] for k, v in p.aliases.items()],
           "extra": {f: sorted(d) for f, d in p.extra_decls.items() if d},
           "rules": [[r[0], r[1] if isinstance(r[1], str) else "<fn>"] for r in p.decl_rules],
           "bss": dict(sorted(p.bss.items()))}
    if full:
        d = os.path.join(full, tag)
        for n in changed:
            os.makedirs(d, exist_ok=True)
            open(os.path.join(d, n), "w", encoding="utf-8", newline="").write(p.files[n])
    return rec


def write(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    open(path, "w", encoding="utf-8", newline="").write(text)


def case(name, files, prof_name, ds, full, env=None):
    for k, v in (env or {}).items():
        os.environ[k] = v
    prof = profile(prof_name)
    names = sorted({"g_" + h for t in files.values() for h in re.findall(r"\bg_([0-9a-f]{4})\b", t)})
    ds = dict(ds) or {n: int(n[2:], 16) for n in names}
    bss = {n: 10 for n in names}
    base = os.path.join(OUT, name)
    for n, t in files.items():
        write(os.path.join(base, "originals", n), t)
    p = make(files, prof, ds, bss, {})
    for n, t in p.files.items():
        if t != files.get(n):
            write(os.path.join(base, "files", n), t)
    manifest = {"env": env or {}, "profile": prof_name, "pointer_bits": prof.get("widths", {}).get("pointer", 16),
                "files": list(p.files), "ds": [[k, v] for k, v in ds.items()], "bss": bss, "env": env or {},
                "notes": [[k, list(v)] for k, v in p.notes.items()],
                "aliases": [[k, v[0], v[1]] for k, v in p.aliases.items()],
                "extra": {f: sorted(d) for f, d in p.extra_decls.items() if d},
                "rules": [[r[0], r[1]] for r in p.decl_rules if isinstance(r[1], str)]}
    write(os.path.join(base, "manifest.json"), json.dumps(manifest, indent=1))
    before = dict(p.files)
    out = []
    for m in METHODS:
        q = make(files, prof, ds, bss, {})
        getattr(q, m)()
        out.append(record(q, before, name + "." + m, full))
    q = make(files, prof, ds, bss, {})
    for m in CHAIN:
        getattr(q, m)()
        out.append(record(q, before, name + ".chain." + m, full))
    write(os.path.join(base, "expected.json"), json.dumps(out, indent=1))
    fired = [r["tag"].split(".", 1)[1] for r in out if r["changed"] and ".chain." not in r["tag"]]
    print(name, "fires:", ", ".join(fired) or "-")
    for k in (env or {}):
        del os.environ[k]


def main():
    if os.environ.get("PYTHONHASHSEED") != "0":      # (the port's own passes iterate sets of names: one order, for the golden data)
        import subprocess
        env = dict(os.environ, PYTHONHASHSEED="0")
        sys.exit(subprocess.call([sys.executable] + sys.argv, env=env))
    full = None
    if "--full-dump" in sys.argv:
        full = sys.argv[sys.argv.index("--full-dump") + 1]
    global OUT
    extra = os.environ.get("P2_EXTRA")
    real = dict(REAL)
    if extra:      # (more trees of the ports folder, to try the passes on: their data goes to cases/, not kept)
        OUT = os.path.join(HERE, "cases")
        real = {}
        for d in sorted(os.listdir(PORTS)):
            cfg = os.path.join(PORTS, d, "modern", "dosrt_config.h")
            if (extra == "ALL" or d in extra.split(",")) and os.path.exists(cfg) and os.path.isdir(os.path.join(PORTS, d, "original")):
                m = re.search(r"profile (\w+)", open(cfg).readline())
                if m:
                    real[d] = m.group(1)
    for t, prof in real.items():
        files, asm, paths = pm.read_tree(os.path.join(PORTS, t, "original"))
        case(t, files, prof, {}, full)
    for t, prof in ({} if extra else SYNTHETIC).items():
        d = os.path.join(SYN, t)
        files = {}
        for n in sorted(os.listdir(d)):
            files[n] = open(os.path.join(d, n), encoding="utf-8", newline="").read()
        case("syn-" + t, files, prof, DS.get(t, {}), full)
    for v, (src, env) in ({} if extra else VARIANTS).items():
        d = os.path.join(SYN, src)
        files = {n: open(os.path.join(d, n), encoding="utf-8", newline="").read() for n in sorted(os.listdir(d))}
        case("syn-" + v, files, SYNTHETIC[src], DS.get(src, {}), full, env)


if __name__ == "__main__":
    main()
