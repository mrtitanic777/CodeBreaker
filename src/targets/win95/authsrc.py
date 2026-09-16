#!/usr/bin/env python3
r"""authsrc.py -- automated AUTHORED-SOURCE reconstruction for Win95 files.

The deliverable is source as close to Microsoft's original as the binary allows -- readable, named,
in each file's ORIGINAL LANGUAGE -- NOT byte-identical binary transcription. This driver automates
that per file:

  * classify the file's dominant language from the C-calling-convention density (push bp / mov bp,sp
    frames): the real-mode DOS/boot layer is ASM, the Windows Setup layer is C;
  * ASM-dominant -> route to dosrecon, whose emitter already produces named, INT-service-annotated,
    string-xref'd MASM (bring it toward the msboot.asm authored gold standard);
  * C-dominant   -> decompile to C (the native win95 decompiler) and AUTHOR it: substitute behavioral
    function names (reusing dosrecon's naming engine on the same image), annotate INT-service call
    sites (DOS 21h / 2Fh / XMS / EMS from dosrecon's authentic-name tables), and add a provenance
    header. This lifts machine-C toward authored C.

    wt --target win95 authsrc <file> [--out DIR]        # one file
    wt --target win95 authsrc --chain [--out DIR]       # the whole boot->Setup chain + a scoreboard

Names/comments/file-split are not in a stripped binary -- ours are labeled, evidence-based
reconstructions; "authored" here means correct logic in the right language, named and readable.
"""
import os
import re
import sys
import subprocess

from src.core import paths
from src.targets.win95 import dosrecon as DR


def _native(fmt):
    b = {"MZ": "win95mzdec", "NE": "win95dec", "PE": "win95pedec",
         "LE": "win95ledec"}.get(fmt, "win95mzdec")
    p = os.path.join(paths.repo_root(), "native", "decompiler", "build", "bin", b + ".exe")
    return p if os.path.isfile(p) else None


def classify(path):
    """(fmt, language, nfuncs, c_frames). Format is the reliable era/language signal: PE (Win32) and
    NE (Win16) are C; LE (VxD) is asm; MZ (DOS) is decided by C-calling-convention density."""
    from src.targets.win95 import nativedec
    fmt = nativedec.detect_format(path)
    dec = _native(fmt)
    n = len(subprocess.run([dec, path, "--names"], capture_output=True, text=True).stdout.splitlines()) if dec else 0
    if fmt in ("PE", "NE"):
        lang = "c"
        cf = 0
    elif fmt == "LE":
        lang = "asm"
        cf = 0
    else:                                                             # MZ (DOS): asm kernel vs C tool
        dis = subprocess.run([dec, path, "--disasm"], capture_output=True, text=True).stdout if dec else ""
        cf = dis.count("push bp") + dis.count("push ebp")
        lang = "c" if (n and cf * 100 // max(n, 1) >= 30) else "asm"
    return fmt, lang, n, cf


# --- behavioral name map (reuse dosrecon's engine on the same image) ------------------------
def _namemap(path, fmt):
    """{addr:int -> behavioral name} for the file's functions, via dosrecon's analysis."""
    data = open(path, "rb").read()
    is_iosys, body_end, _lb, image = DR._layout(data)
    if is_iosys:
        image = data
    dec = _native(fmt)
    starts = sorted({int(l.split("\t")[0], 16) for l in
                     subprocess.run([dec, path, "--names"], capture_output=True, text=True).stdout.splitlines()
                     if l.split("\t")[0]})
    starts = [s for s in starts if s < len(image)]
    ends = {s: (starts[i + 1] if i + 1 < len(starts) else len(image)) for i, s in enumerate(starts)}
    strings = DR._string_regions(image)
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    namemap, finfo = {}, {}
    for s in starts:
        e = min(ends[s], s + 0x4000)
        kind, ni, nb, ints, strs, services, callees = DR._analyze_fn(image, s, e, strings, md)
        namemap[s] = DR._beautify_name(s, kind, ints, strs, "")
        finfo[s] = {"services": services, "callees": callees, "strs": strs, "ints": ints, "kind": kind}
    DR._name_by_behavior(namemap, finfo, set(starts))
    # NE modules: name exported functions from the NE entry+name tables (authentic Win16 API names --
    # Win16 GUI code is API-driven, so INT/string behavioral evidence alone leaves most unnamed).
    if fmt == "NE":
        try:
            from src.targets.win95.nerecon import NE
            ne = NE(data)
            for (seg, off), nm in ne.export_map().items():
                if 1 <= seg <= len(ne.segs):
                    fo = ne.segs[seg - 1]["file"] + off
                    namemap[fo] = nm
        except Exception:                                    # noqa: BLE001
            pass
    # the corpus name store wins: a curated name (keyed by body_id -> propagates from any module, or
    # module-local) overrides the engine's inferred name, so names set once render everywhere.
    try:
        from src.targets.win95 import namestore
        module = os.path.basename(path)
        bidm = namestore.bodyid_map(path)
        for s in starts:
            cur = namestore.resolve(module, s, bidm.get(s))
            if cur:
                namemap[s] = cur
    except Exception:                                        # noqa: BLE001 -- store is optional
        pass
    return namemap


# --- INT-service resolution (correlate the AH/AX value at each int site from the disasm) ------
def _int_services(path, fmt):
    """{func_addr:int -> [service label per int site, in order]}. For each `int 0xNN`, scan back for
    the AH/AX it was set with and map to the authentic service name (DOS 21h / 2Fh / XMS / EMS)."""
    dec = _native(fmt)
    dis = subprocess.run([dec, path, "--disasm"], capture_output=True, text=True).stdout
    starts = sorted({int(l.split("\t")[0], 16) for l in
                     subprocess.run([dec, path, "--names"], capture_output=True, text=True).stdout.splitlines()
                     if l.split("\t")[0]})
    import bisect

    def func_of(a):
        i = bisect.bisect_right(starts, a) - 1
        return starts[i] if i >= 0 else None
    out, ah, ax = {}, None, None
    for line in dis.splitlines():
        m = re.match(r"^\s+([0-9A-Fa-f]{4,8})\s+", line)      # "  ADDR  BYTES  text" (indented)
        if not m:
            continue
        addr = int(m.group(1), 16)
        txt = line.split(None, 2)[-1] if len(line.split(None, 2)) == 3 else ""
        mah = re.search(r"mov ah, 0x([0-9a-fA-F]+)", txt)
        max_ = re.search(r"mov ax, 0x([0-9a-fA-F]+)", txt)
        if mah:
            ah = int(mah.group(1), 16)
        if max_:
            ax = int(max_.group(1), 16); ah = (ax >> 8) & 0xFF
        mint = re.search(r"int 0x([0-9a-fA-F]+)", txt)
        if mint:
            cur = func_of(addr)
            if cur is None:
                continue
            out.setdefault(cur, [])
            iv = int(mint.group(1), 16)
            lab = None
            if iv == 0x21 and ah in DR._DOS21:
                lab = "DOS 0x%02X %s" % (ah, DR._DOS21[ah])
            elif iv == 0x2f and ax in DR._INT2F:
                lab = "INT 2Fh %s" % DR._INT2F[ax]
            elif iv == 0x2f and ah == 0x43:
                lab = "XMS"
            elif iv == 0x67:
                lab = "EMS"
            elif iv == 0x10:
                lab = "BIOS video"
            elif iv == 0x13:
                lab = "BIOS disk"
            else:
                lab = "int 0x%02X" % iv
            out[cur].append(lab)
    return out


# --- author the decompiled C ----------------------------------------------------------------
def _simplify_c(c):
    """Clean common decompiler artifacts so bodies read closer to authored C (safe text rewrites)."""
    c = re.sub(r"\(([A-Za-z_]\w*) \^ \1\)", "0", c)                    # (x ^ x) -> 0
    c = re.sub(r"\((\d+) \^ \1\)", "0", c)                             # (4 ^ 4) -> 0
    c = re.sub(r"\b(\w+) \^ \1\b", "0", c)                             # x ^ x   -> 0
    c = c.replace(" + 0 + ", " + ").replace(" - 0 + ", " + ").replace(" + 0;", ";")
    c = re.sub(r" \+ 0\b", "", c)
    return c


_PE_API = re.compile(r"\b([A-Z][A-Za-z0-9]{3,}[AW]?)\(")   # a Win32-API-looking call
_PE_SKIP = {"If", "While", "For", "Switch", "Return", "Sizeof"}


def _pe_namemap(path):
    """PE (Win32) name map (lever 1): exported functions -> their authentic export name; internal
    sub_ functions -> named after the most distinctive Win32 API they call. The decompiler already
    resolves imports to named API calls in the emitted C, so this reads them straight from it."""
    dec = _native("PE")
    names = {}
    for l in subprocess.run([dec, path, "--names"], capture_output=True, text=True).stdout.splitlines():
        p = l.split("\t")
        if len(p) > 1 and p[1].strip():
            try:
                names[int(p[0], 16)] = p[1].strip()               # authentic export name
            except ValueError:
                pass
    c = subprocess.run([dec, path, "--module"], capture_output=True, text=True).stdout
    cur, apis = None, {}
    for line in c.splitlines():
        fm = re.match(r"^[A-Za-z_].*\bsub_([0-9A-Fa-f]{4,8})\(.*\)\s*$", line)
        if fm:
            cur = int(fm.group(1), 16); apis.setdefault(cur, [])
            continue
        if cur is not None:
            for m in _PE_API.finditer(line):
                a = m.group(1)
                if a not in _PE_SKIP and not a.startswith(("sub_", "v")):
                    apis[cur].append(a)
    for addr, calls in apis.items():
        if addr in names or not calls:
            continue
        top = max(set(calls), key=lambda x: (calls.count(x), len(x)))   # most-called, then longest
        names[addr] = "%s_helper_%06X" % (top, addr)
    # lever 2: the corpus name store wins -- a shared-body name (seeded from body_ledger, or curated)
    # propagates here from any module that shares the body.
    try:
        from src.targets.win95 import namestore
        module = os.path.basename(path)
        bidm = namestore.bodyid_map(path)
        for a, bid in bidm.items():
            cur = namestore.resolve(module, a, bid)
            if cur:
                names[a] = cur
    except Exception:                                            # noqa: BLE001
        pass
    return names


def _author_c(path, fmt, out):
    dec = _native(fmt)
    c = subprocess.run([dec, path, "--module"], capture_output=True, text=True).stdout
    # the behavioral-naming + INT-service passes reuse dosrecon's 16-bit real-mode engine; apply them
    # for the 16-bit classes (MZ/NE) and skip cleanly for 32-bit PE (still cleaned + headered).
    names, svcs = {}, {}
    if fmt in ("MZ", "NE"):
        try:
            names = _namemap(path, fmt); svcs = _int_services(path, fmt)
        except Exception:                                             # noqa: BLE001 -- best effort
            names, svcs = {}, {}
    elif fmt == "PE":                                                # lever 1: Win32 export + API naming
        try:
            names = _pe_namemap(path)
        except Exception:                                             # noqa: BLE001
            names = {}

    def sub_name(m):
        return names.get(int(m.group(1), 16), m.group(0))
    c = re.sub(r"sub_([0-9A-Fa-f]{4,8})", sub_name, c)                 # behavioral function names
    c = _simplify_c(c)

    # per-function INT-service annotation: within each function (identified by its _XXXXXX address
    # suffix), replace the int sites in order with the specific resolved service.
    outlines = []
    cur_addr = None
    idx = 0
    for line in c.splitlines():
        fm = re.match(r"^[A-Za-z_].*_([0-9A-Fa-f]{4,8})\(.*\)\s*$", line)
        if fm:
            cur_addr = int(fm.group(1), 16); idx = 0
        im = re.search(r"__asm \{ int (0x[0-9a-fA-F]+) \}", line)
        if im and cur_addr is not None:
            seq = svcs.get(cur_addr, [])
            lab = seq[idx] if idx < len(seq) else (im.group(1) + " service")
            idx += 1
            line = re.sub(r"__asm \{ int 0x[0-9a-fA-F]+ \}",
                          "__asm { int %s }   /* %s */" % (im.group(1), lab), line)
        outlines.append(line)
    c = "\n".join(outlines)

    hdr = ("/* %s -- authored-source reconstruction (CodeBreaker authsrc).\n"
           " * Original language: C. Decompiled from the real Win95 binary and authored: behavioral\n"
           " * function names, resolved INT-service call sites (DOS/2Fh/XMS/EMS/BIOS), artifact\n"
           " * cleanup. Names are evidence-based reconstruction labels, not recovered symbols. */\n\n"
           % os.path.basename(path))
    os.makedirs(os.path.dirname(out), exist_ok=True)
    open(out, "w", encoding="utf-8", errors="replace").write(hdr + c)
    named = sum(1 for v in names.values() if not v.startswith(("sub_", "helper_", "thunk_")))
    return len(names), named


# --- the boot -> Setup chain ----------------------------------------------------------------
_CHAIN = [
    ("VBR",         "recon/bootdisk/msboot.asm", "asm-authored (gold standard)"),
    ("IO.SYS",      "corpus/files/setup/inf/winboot.sys", None),
    ("COMMAND.COM", "corpus/files/setup/inf/command.com", None),
    ("HIMEM.SYS",   "corpus/files/WINDOWS/SYSTEM/himem.sys", None),
    ("CD1.SYS",     "recon/bootdisk/files/CD1.SYS", None),
    ("MSCDEX.EXE",  "corpus/files/WINDOWS/COMMAND/mscdex.exe", None),
    ("FDISK.EXE",   "corpus/files/WINDOWS/COMMAND/fdisk.exe", None),
    ("FORMAT.COM",  "corpus/files/WINDOWS/COMMAND/format.com", None),
    ("OEMSETUP.EXE", "corpus/disc/src/WIN95/OEMSETUP.EXE", None),
    ("SETUPX.DLL",  "corpus/files/setup/inf/setupx.dll", None),
    ("SUWIN.EXE",   "corpus/files/setup/inf/suwin.exe", None),
]


def _dr():
    dr = paths.design_root()
    if os.path.isdir(os.path.join(dr, "corpus")):
        return dr
    sib = os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design")
    return sib if os.path.isdir(os.path.join(sib, "corpus")) else dr


def _all_binaries():
    """Every code-bearing Win95 binary under corpus/files (dedup by name, prefer SYSTEM/)."""
    root = os.path.join(_dr(), "corpus", "files")
    exts = (".exe", ".dll", ".sys", ".com", ".drv", ".vxd", ".cpl", ".386", ".scr")
    found = {}
    for dp, _dn, fns in os.walk(root):
        for fn in fns:
            if fn.lower().endswith(exts):
                key = fn.lower()
                prefer = "system" in dp.lower().replace("\\", "/").split("/")
                if key not in found or (prefer and "system" not in found[key].lower().replace("\\", "/").split("/")):
                    found[key] = os.path.join(dp, fn)
    return [(os.path.basename(p), p, None) for p in sorted(found.values(), key=lambda x: os.path.basename(x).lower())]


def main(argv):
    outroot = os.path.join(_dr(), "recon", "authored")
    chain = do_all = False
    target = None
    limit = 0
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == "--chain":
            chain = True
        elif a == "--all":
            do_all = True
        elif a == "--limit" and i + 1 < len(argv):
            limit = int(argv[i + 1]); i += 1
        elif a == "--out" and i + 1 < len(argv):
            outroot = argv[i + 1]; i += 1
        elif not a.startswith("-"):
            target = a
        i += 1

    items = []
    scoreboard = None
    if do_all:
        items = _all_binaries()
        if limit:
            items = items[:limit]
        scoreboard = os.path.join(outroot, "AUTHORED.csv")
    elif chain:
        for nm, rel, note in _CHAIN:
            items.append((nm, os.path.join(_dr(), rel), note))
    elif target:
        items.append((os.path.basename(target), target, None))
    else:
        print(__doc__); return 0

    rows = []
    quiet = do_all and len(items) > 30
    if not quiet:
        print("%-14s %-4s %-6s %-9s %s" % ("FILE", "LANG", "FUNCS", "NAMED", "authored source"))
    done = 0
    for nm, path, note in items:
        if note and str(path).endswith(".asm"):
            rows.append((nm, "asm", 0, 0, 0, note)); continue
        if not os.path.isfile(path):
            rows.append((nm, "?", 0, 0, 0, "MISSING")); continue
        try:
            fmt, lang, n, cf = classify(path)
            if lang == "c":
                out = os.path.join(outroot, "c", nm.split(".")[0].lower() + ".c")
                tot, named = _author_c(path, fmt, out)
                rows.append((nm, "C/" + fmt, n, tot, named, os.path.relpath(out, _dr())))
                line = "%-14s %-6s %-6d %-9s -> %s" % (nm, "C/" + fmt, n, "%d/%d" % (named, tot), rows[-1][5])
            else:
                rows.append((nm, "asm/" + fmt, n, 0, 0, "dosrecon/asm route"))
                line = "%-14s %-6s %-6d %-9s %s" % (nm, "asm/" + fmt, n, "-", "asm route")
            if not quiet:
                print(line)
        except Exception as e:                                       # noqa: BLE001 -- keep the sweep going
            rows.append((nm, "ERR", 0, 0, 0, str(e)[:60]))
        done += 1
        if quiet and done % 50 == 0:
            print("  ... %d/%d authored" % (done, len(items)))

    if scoreboard:
        import csv
        os.makedirs(os.path.dirname(scoreboard), exist_ok=True)
        with open(scoreboard, "w", newline="") as f:
            w = csv.writer(f); w.writerow(["file", "lang", "funcs", "total", "named", "out_or_note"])
            for r in rows:
                w.writerow(r)
        c_files = sum(1 for r in rows if r[1].startswith("C"))
        asm_files = sum(1 for r in rows if r[1].startswith("asm"))
        errs = sum(1 for r in rows if r[1] == "ERR")
        tot_named = sum(r[4] for r in rows); tot_funcs = sum(r[3] for r in rows)
        print("\n== authsrc corpus scoreboard ==")
        print("  %d files: %d C-authored, %d asm-route, %d errors" % (len(rows), c_files, asm_files, errs))
        print("  C functions named: %d / %d (%.0f%%)"
              % (tot_named, tot_funcs, 100.0 * tot_named / max(tot_funcs, 1)))
        print("  -> %s" % scoreboard)
    print("\nauthored source -> %s" % outroot)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
