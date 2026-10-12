#!/usr/bin/env python3
"""buildall.py -- the corpus-wide reconstruction frontier for Win95's DLLs. For every
DLL it: reads the export surface straight from the binary (-> a .def), notes the
decompiled function count + how readable/app-heavy it is, generates an owbuild.json
build scaffold, and -- where curated source exists -- runs the OpenWatcom build. One
scoreboard shows exactly where each DLL stands so the frontier can be driven up.

    wt --target win95 buildall [--limit N] [--only substr] [--class ne|pe] [--build]

Stages per module (each recorded in BUILDALL.csv):
  exports   read from the binary's own tables (NE resident/nonresident, PE export dir)
  decompile function count + %readable (from the *_lane sweep, if present)
  scaffold  .def + owbuild.json written to <design>/scaffold/<module>/
  build     if base/<module>/owbuild.json (curated) exists and --build: owbuild -> C

Scaffolds are staging (design/), never the one source tree; curation promotes a
module into base/<module>/ with real split C.
"""
import argparse
import csv
import json
import os
import struct
import sys

from src.core import paths
from src.targets.win95 import nativedec


def _corpus():
    from src.core import corpus
    return corpus.root()


def _design():
    c = _corpus()
    return os.path.dirname(os.path.dirname(c)) if c.lower().rstrip("\\/").endswith("files") \
        else os.path.dirname(c)


# --- export extraction straight from the binary ---------------------------------
def exports_ne(d):
    """[(name, ordinal)] from an NE module's resident + nonresident name tables."""
    e = struct.unpack_from("<H", d, 0x3C)[0]
    out = []

    def read_table(off, first_is_module):
        p = off
        idx = 0
        while p < len(d):
            ln = d[p]
            if ln == 0:
                break
            name = d[p + 1:p + 1 + ln].decode("latin1", "replace")
            ordn = struct.unpack_from("<H", d, p + 1 + ln)[0]
            if not (first_is_module and idx == 0):      # entry 0 = module/description
                out.append((name, ordn))
            p += 1 + ln + 2
            idx += 1

    rn = e + struct.unpack_from("<H", d, e + 0x26)[0]    # resident names (from NE hdr)
    read_table(rn, True)
    nr = struct.unpack_from("<I", d, e + 0x2C)[0]        # nonresident names (from file start)
    if 0 < nr < len(d):
        read_table(nr, True)
    return out


def exports_pe(d):
    """[(name, ordinal)] from a PE32 module's export directory."""
    e = struct.unpack_from("<I", d, 0x3C)[0]
    if d[e:e + 4] != b"PE\0\0":
        return []
    opt = e + 24
    magic = struct.unpack_from("<H", d, opt)[0]
    if magic != 0x10B:
        return []
    ndir = struct.unpack_from("<I", d, opt + 92)[0]
    if ndir < 1:
        return []
    exp_rva = struct.unpack_from("<I", d, opt + 96)[0]
    if not exp_rva:
        return []
    nsec = struct.unpack_from("<H", d, e + 6)[0]
    optsz = struct.unpack_from("<H", d, e + 20)[0]
    sh = opt + optsz
    secs = []
    for i in range(nsec):
        s = sh + i * 40
        secs.append((struct.unpack_from("<I", d, s + 12)[0], struct.unpack_from("<I", d, s + 8)[0],
                     struct.unpack_from("<I", d, s + 16)[0], struct.unpack_from("<I", d, s + 20)[0]))

    def r2o(rva):
        for va, vs, rs, ro in secs:
            if va <= rva < va + max(vs, rs):
                return ro + (rva - va)
        return None

    exp_size = struct.unpack_from("<I", d, opt + 100)[0]
    ed = r2o(exp_rva)
    if ed is None:
        return []
    base = struct.unpack_from("<I", d, ed + 16)[0]
    nfunc = struct.unpack_from("<I", d, ed + 20)[0]            # Export Address Table size
    nname = struct.unpack_from("<I", d, ed + 24)[0]
    eat = r2o(struct.unpack_from("<I", d, ed + 28)[0])
    an = r2o(struct.unpack_from("<I", d, ed + 32)[0])
    ao = r2o(struct.unpack_from("<I", d, ed + 36)[0])
    out = []
    namedidx = set()
    # Named exports (unchanged): the name-pointer table + name-ordinal table. An ordinal-
    # only module (nname=0, no name table) has an/ao == None here -- that path is handled
    # by the EAT walk below, NOT by bailing out (the old `return []` dropped every export
    # of MFC30/riched32/msjt3032, which export entirely by ordinal).
    if an is not None and ao is not None:
        for i in range(nname):
            nrva = struct.unpack_from("<I", d, an + i * 4)[0]
            no = r2o(nrva)
            if no is None:
                continue
            end = d.find(b"\0", no)
            name = d[no:end].decode("latin1", "replace")
            oi = struct.unpack_from("<H", d, ao + i * 2)[0]
            namedidx.add(oi)
            out.append((name, oi + base))
    # Ordinal-only exports: every Export Address Table slot with no name-table entry.
    # Skip empty slots (unused ordinals) and forwarders (RVA inside the export directory,
    # which re-export another DLL's symbol rather than defining a function here).
    if eat is not None:
        for j in range(nfunc):
            if j in namedidx:
                continue
            frva = struct.unpack_from("<I", d, eat + j * 4)[0]
            if not frva or exp_rva <= frva < exp_rva + exp_size:
                continue
            out.append(("", base + j))
    return out


def extract_exports(path):
    try:
        d = open(path, "rb").read()
    except OSError:
        return None, []
    fmt = nativedec.detect_format(path)
    if fmt == "NE":
        return fmt, exports_ne(d)
    if fmt == "PE":
        return fmt, exports_pe(d)
    return fmt, []


def write_def(module, fmt, exports, outdir):
    lines = ["LIBRARY %s" % module.upper().split(".")[0]]
    if fmt == "NE":
        lines.append("EXETYPE WINDOWS")
    lines.append("EXPORTS")
    for name, ordn in sorted(exports, key=lambda x: x[1]):
        lines.append("    %-24s @%d" % (name, ordn))
    p = os.path.join(outdir, module.split(".")[0] + ".def")
    with open(p, "w", newline="\n") as f:
        f.write("\n".join(lines) + "\n")
    return p


def _lane_stats(design, fmt, module):
    """(functions, pct_readable) from the *_lane sweep's .c for this module, if present."""
    lane = {"NE": "ne_lane", "PE": "pe_lane", "LE": "le_lane", "MZ": "mz_lane"}.get(fmt)
    if not lane:
        return 0, None
    c = os.path.join(design, lane, module + ".c")
    if not os.path.isfile(c):
        return 0, None
    text = open(c, encoding="utf-8", errors="replace").read()
    funcs = text.count("__fastcall")
    asm = text.count("__asm")
    # bounded readability: share of "clean" units among functions + inline-asm blocks
    pct = round(100.0 * funcs / (funcs + asm), 1) if funcs else None
    return funcs, pct


def main(argv):
    ap = argparse.ArgumentParser(prog="buildall", description=__doc__.splitlines()[0])
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--only", default=None)
    ap.add_argument("--ext", default="dll",
                    help="comma-separated extensions to build (default dll; e.g. exe or dll,exe,drv,cpl)")
    ap.add_argument("--class", dest="cls", choices=("ne", "pe"), default=None)
    ap.add_argument("--build", action="store_true", help="run owbuild where curated source exists")
    ap.add_argument("--cure", action="store_true",
                    help="decompile fresh, autocure, and compile each module (the compile frontier)")
    ap.add_argument("--link", action="store_true",
                    help="after a clean compile, autolink the module into a DLL (PE; scaffold)")
    ap.add_argument("--jobs", type=int, default=0, help="parallel workers (default: min(12, cpus))")
    ap.add_argument("--timeout", type=int, default=240, help="per-module decompile timeout")
    a = ap.parse_args(argv)

    corpus, design = _corpus(), _design()
    scaffold_root = os.path.join(design, "scaffold")
    os.makedirs(scaffold_root, exist_ok=True)

    # code-bearing files to enumerate: DLLs by default; --ext widens to executables
    # and driver-class modules (all really NE/PE/LE/MZ under the hood).
    exts = tuple("." + e.lstrip(".").lower() for e in (a.ext or "dll").split(","))
    dlls = {}
    for dp, _, fs in os.walk(corpus):
        for fn in fs:
            if not fn.lower().endswith(exts):
                continue
            if a.only and a.only.lower() not in fn.lower():
                continue
            p = os.path.join(dp, fn)
            key = fn.lower()
            prefer = "system" in dp.lower().replace("\\", "/").split("/")
            if key not in dlls or (prefer and "system" not in dlls[key].lower().replace("\\", "/").split("/")):
                dlls[key] = p
    names = sorted(dlls)
    if a.limit:
        names = names[:a.limit]

    curated_base = os.path.join(paths.data_root(), "Windows95", "base")
    import tempfile
    import threading
    from concurrent.futures import ThreadPoolExecutor

    # Only one "giant" module (mapi, msjt3032 -- 2k+ functions, a ~3-minute
    # single-TU wcc compile) may compile at a time. Two of them overlapping under
    # 12-wide load starves each other and one can spuriously fail; serializing the
    # giants keeps the board deterministic without slowing the small modules.
    giant_lock = threading.Lock()
    GIANT_FUNCS = 1500

    def _work(key):
        """Per-module frontier work (thread-safe; each spawns its own subprocesses)."""
        path = dlls[key]
        mod = key
        fmt, exports = extract_exports(path)
        if a.cls and (fmt or "").lower() != a.cls:
            return None
        sd = os.path.join(scaffold_root, mod.split(".")[0])
        os.makedirs(sd, exist_ok=True)
        def_ok = False
        if exports:
            write_def(mod, fmt, exports, sd)
            def_ok = True
            json.dump({"class": "ne" if fmt == "NE" else "pe",
                       "target": "windows" if fmt == "NE" else "nt",
                       "model": "m" if fmt == "NE" else None,
                       "system": "windows_dll" if fmt == "NE" else "nt_dll",
                       "output": mod, "def": mod.split(".")[0] + ".def", "modules": [],
                       "status": "scaffold -- needs curated source"},
                      open(os.path.join(sd, "owbuild.json"), "w"), indent=2)

        status, cure, build, funcs, readable = "scaffolded", "", "", 0, ""
        # a headerless real-mode blob (detect_format None) decompiles via nativedec's
        # synthetic-MZ wrap -> compile it as 16-bit (NE flags).
        cfmt = fmt if fmt in ("NE", "PE", "LE", "MZ") else "NE"
        if a.cure and (fmt in ("NE", "PE", "LE", "MZ") or fmt is None):
            try:
                from src.targets.win95 import autocure
                # decompile FRESH (current emitter), cure, compile -- one pass, no
                # dependence on a stale *_lane sweep.
                # sigs=False: one decode pass (skip the --names + demangle pass we
                # don't need for the compile frontier) -- ~2x faster on PE.
                ctext = nativedec.decompile_module(path, timeout=a.timeout, sigs=False)
                funcs = ctext.count("__fastcall")
                if not funcs and fmt in ("NE", "PE"):
                    # A code-less resource container (icons/bitmaps/strings, no
                    # .text / entry / exports): nothing to compile, but it is still
                    # a real DLL -- rebuild it from its resources (verbatim).
                    from src.targets.win95 import resbuild
                    rb = resbuild.build(path, sd)
                    if rb.get("ok"):
                        status = "resbuilt"; build = "res"
                        v = rb.get("verify", {})
                        cure = "res:%d/%d" % (v.get("byte_exact", 0), rb.get("resources", 0))
                if funcs:
                    cured, _st = autocure.cure_text(ctext)
                    cf = os.path.join(sd, mod.split(".")[0] + ".c")
                    with open(cf, "w", encoding="utf-8", newline="\n") as f:
                        f.write(cured)
                    # An NE module far larger than a 16-bit code segment (64 KB) can
                    # never compile as one TU (E1118). Split it into multiple TUs up
                    # front -- the DLL was itself multi-segment -- rather than waste a
                    # multi-minute doomed single-TU compile.
                    if cfmt == "NE" and len(cured) > 200000:
                        from src.targets.win95 import nesplit
                        sr = nesplit.compile_split(cured, mod.split(".")[0],
                                                   os.path.join(sd, "split"))
                        cc = {"ok": sr.get("ok"), "errors": len(sr.get("failed", []))}
                        cure = ("compiles(split:%d)" % sr.get("chunks", 0)
                                if sr.get("ok") else "split-err:%d" % len(sr.get("failed", [])))
                    elif funcs >= GIANT_FUNCS:
                        with giant_lock:            # serialize the multi-minute giants
                            cc = autocure._compile(cf, cfmt)
                        cure = "compiles" if cc.get("ok") else "err:%d" % cc.get("errors", -1)
                    else:
                        cc = autocure._compile(cf, cfmt)
                        cure = "compiles" if cc.get("ok") else "err:%d" % cc.get("errors", -1)
                    if cc.get("ok"):
                        status = "compiles"
                        # optional: link the compiled module (PE scaffold). A DLL
                        # links via its export table (autolink); an EXE links to an
                        # entry point with no exports (autolink_exe).
                        if a.link and fmt == "PE":
                            from src.targets.win95 import owbuild
                            imps = __import__("src.formats.exeinfo", fromlist=["exeinfo"]).parse(path).get("imports", [])
                            is_exe = mod.lower().endswith(".exe")
                            if is_exe:
                                lr = owbuild.autolink_exe(mod, fmt, imps, cf, sd)
                            elif exports:
                                enames = [n for n, _o in exports]
                                lr = owbuild.autolink(mod, fmt, enames, imps, cf, sd)
                            else:
                                lr = {"ok": False}
                            build = "linked" if lr.get("ok") else "link-fail"
                            if lr.get("ok"):
                                status = "linked"
            except Exception as ex:
                cure = "cure-error:" + type(ex).__name__
        curated = os.path.isfile(os.path.join(curated_base, mod.split(".")[0], "owbuild.json"))
        if curated and a.build:
            try:
                from src.targets.win95 import owbuild
                r = owbuild.build(mod.split(".")[0], do_diff=False)
                status = "built" if r.get("ok") else "build-fail"
                build = "ok" if r.get("ok") else (r.get("error", "")[:40])
            except Exception as ex:
                status = "build-error"; build = type(ex).__name__
        elif curated:
            status = "curated"
        return (mod, fmt or "?", len(exports), funcs, readable, "def" if def_ok else "",
                status, cure, build)

    jobs = a.jobs or min(12, (os.cpu_count() or 4))
    rows = []
    done = 0
    from concurrent.futures import as_completed
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        futs = {ex.submit(_work, k): k for k in names}
        for fut in as_completed(futs):          # print as each finishes (live progress)
            try:
                row = fut.result()
            except Exception:
                row = None
            if row is None:
                continue
            rows.append(row)
            done += 1
            print("[%d/%d] %-20s %-3s exp=%-4d fns=%-5d %s %s %s"
                  % (done, len(names), row[0], row[1], row[2], row[3],
                     row[6], row[7], row[8]), flush=True)

    # keep the DLL scoreboard as BUILDALL.csv; other ext sets get their own file.
    tag = "" if (a.ext or "dll") == "dll" else "_" + (a.ext or "dll").replace(",", "-")
    out = os.path.join(design, "BUILDALL%s.csv" % tag)
    with open(out, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["module", "format", "exports", "functions", "pct_readable", "def", "status", "cure", "build"])
        w.writerows(rows)

    from collections import Counter
    st = Counter(r[6] for r in rows)
    tot_exp = sum(r[2] for r in rows)
    print("\n=== %d DLLs: %s ===" % (len(rows), dict(st)))
    print("total exports catalogued: %d ; scoreboard -> %s" % (tot_exp, out))


if __name__ == "__main__":
    main(sys.argv[1:])
