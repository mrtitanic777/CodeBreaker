#!/usr/bin/env python3
r"""oracle.py -- build THE grading baseline: extract the canonical Win95 OSR2
originals straight from the golden install ISO's cabs into a clean, version-stamped
oracle tree.

Why this exists: reconstructions must be graded against the files as they ship on
the install disc -- NOT the post-install snapshot (files/WINDOWS/SYSTEM), which can
carry contaminant versions setup or later apps dropped in (a real case: a Windows
3.1 lzexpand v3.10 sitting where the OSR2 v4.00.429 belongs). The disc cabs are the
one source of truth.

    python -m engine.foundation.oracle build [<iso>] [<out_dir>]
    python -m engine.foundation.oracle version <file>     # print its FileVersion
    python -m engine.foundation.oracle find <name>        # locate in the oracle

Layout: a flat tree keyed by filename (grading looks up by name), plus manifest.csv
(name,size,sha1,fileversion,source_cabs) and collisions.txt (same name, different
content -- kept as name#<cab>).
"""
import os
import re
import sys
import csv
import glob
import struct
import hashlib
import subprocess
import tempfile

from src.core import paths
ROOT = paths.repo_root()                                      # CodeBreaker/ (canonical resolver)


def _cfg(key, default=""):
    try:
        from src.core import config
        return config.load_config().get(key, default) or default
    except Exception:
        return default


def _sevenzip():
    """7z.exe CLI (config stores the GUI 7zFM.exe; derive the CLI beside it)."""
    fm = _cfg("sevenzip") or r"C:\Program Files\7-Zip\7zFM.exe"
    cli = os.path.join(os.path.dirname(fm), "7z.exe")
    return cli if os.path.exists(cli) else "7z"


def _golden_iso():
    # the unequivocal oracle disc, kept in the Win95 design arena's oracle/ folder
    return _cfg("golden_iso") or os.path.join(
        paths.design_root(), "oracle", "Win95 OSR2.ISO")


def _oracle_dir():
    # the extracted version-stamped oracle tree (generated beside the golden ISO)
    return _cfg("oracle_dir") or os.path.join(paths.design_root(), "oracle", "tree")


# ---- version resource (VS_FIXEDFILEINFO) ---------------------------------------
# Works for both NE and PE: the signature 0xFEEF04BD is followed by struct version
# (WORD), then dwFileVersionMS, dwFileVersionLS (each HIWORD.LOWORD).
_VS_SIG = b"\xbd\x04\xef\xfe"


def fileversion(path):
    """Return 'maj.min.build.rev' from the file's version resource, or '' if none."""
    try:
        d = open(path, "rb").read()
    except Exception:
        return ""
    i = d.find(_VS_SIG)
    if i < 0:
        return ""
    ms, ls = struct.unpack_from("<II", d, i + 8)
    return "%d.%02d.%d.%d" % (ms >> 16, ms & 0xFFFF, ls >> 16, ls & 0xFFFF)


def _sha1(path):
    h = hashlib.sha1()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 16), b""):
            h.update(chunk)
    return h.hexdigest()


# ---- extraction ----------------------------------------------------------------
def _extract_cabs_from_iso(iso, cabdir, sz):
    """Pull PRECOPY + numbered WIN95 cabs out of the ISO into cabdir."""
    os.makedirs(cabdir, exist_ok=True)
    for pat in ("WIN95/WIN95_*.CAB", "WIN95/PRECOPY*.CAB"):
        subprocess.run([sz, "e", iso, "-o" + cabdir, pat, "-y"],
                       capture_output=True, text=True)
    return sorted(glob.glob(os.path.join(cabdir, "*.CAB")) +
                  glob.glob(os.path.join(cabdir, "*.cab")))


def _set_heads(cabs):
    """Win95 cabs are chained sets -- 7z extracts the WHOLE set from any one cab.
    Group by alpha-prefix and return only the lowest-numbered cab of each set, so we
    extract each set once (WIN95_02.. -> one head; PRECOPY1/2 -> one head)."""
    groups = {}
    for c in cabs:
        b = os.path.basename(c)
        m = re.match(r"(.*?)(\d+)\.cab$", b, re.I)
        pref, num = (m.group(1).upper(), int(m.group(2))) if m else (b.upper(), 0)
        groups.setdefault(pref, []).append((num, c))
    return [sorted(pairs)[0][1] for _pref, pairs in sorted(groups.items())]


def build(iso=None, out=None):
    iso = iso or _golden_iso()
    out = out or _oracle_dir()
    sz = _sevenzip()
    if not os.path.exists(iso):
        print("golden ISO not found: %s" % iso)
        return 2
    os.makedirs(out, exist_ok=True)
    print("oracle: iso=%s\n        out=%s\n        7z=%s" % (iso, out, sz), flush=True)

    with tempfile.TemporaryDirectory(prefix="oracle_cabs_") as cabdir:
        cabs = _extract_cabs_from_iso(iso, cabdir, sz)
        heads = _set_heads(cabs)
        print("oracle: %d cabs in %d set(s) -> extracting heads: %s"
              % (len(cabs), len(heads), ", ".join(os.path.basename(h) for h in heads)),
              flush=True)
        # name -> {sha1 -> (size, ver, [cabs])}
        index = {}
        for cab in heads:
            cabname = os.path.basename(cab)
            with tempfile.TemporaryDirectory(prefix="oracle_x_") as xd:
                # extract with all cabs present in cabdir so 7z can follow the set;
                # -o points into a scratch dir, flat.
                subprocess.run([sz, "e", cab, "-o" + xd, "-y", "-aoa"],
                               capture_output=True, text=True)
                for fn in os.listdir(xd):
                    fp = os.path.join(xd, fn)
                    if not os.path.isfile(fp):
                        continue
                    h = _sha1(fp)
                    key = fn.lower()
                    rec = index.setdefault(key, {})
                    if h not in rec:
                        rec[h] = {"size": os.path.getsize(fp), "ver": fileversion(fp),
                                  "cabs": [], "src": fp, "name": fn}
                        # copy the first occurrence into the oracle now
                        variants = len(rec)
                        dest_name = fn if variants == 1 else "%s#%s" % (fn, cabname)
                        with open(fp, "rb") as s, open(os.path.join(out, dest_name), "wb") as o:
                            o.write(s.read())
                        rec[h]["dest"] = dest_name
                    if cabname not in rec[h]["cabs"]:
                        rec[h]["cabs"].append(cabname)

    # manifest + collisions
    rows = []
    collisions = []
    for name in sorted(index):
        variants = index[name]
        if len(variants) > 1:
            collisions.append((name, variants))
        for h, meta in variants.items():
            rows.append((meta["dest"], meta["size"], h, meta["ver"],
                         ";".join(sorted(meta["cabs"]))))
    with open(os.path.join(out, "manifest.csv"), "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["name", "size", "sha1", "fileversion", "source_cabs"])
        w.writerows(sorted(rows))
    if collisions:
        with open(os.path.join(out, "collisions.txt"), "w") as f:
            for name, variants in collisions:
                f.write("%s: %d distinct contents\n" % (name, len(variants)))
                for h, meta in variants.items():
                    f.write("   %s  %d bytes  v%s  <- %s\n"
                            % (meta["dest"], meta["size"], meta["ver"] or "-",
                               ";".join(sorted(meta["cabs"]))))
    versioned = sum(1 for r in rows if r[3])
    print("oracle: %d files (%d distinct-name collisions), %d with version stamp -> %s"
          % (len(rows), len(collisions), versioned, out), flush=True)
    return 0


def find(name):
    out = _oracle_dir()
    hits = glob.glob(os.path.join(out, name)) + glob.glob(os.path.join(out, name + "#*"))
    if not hits:
        # case-insensitive fallback
        for fn in os.listdir(out) if os.path.isdir(out) else []:
            if fn.lower() == name.lower() or fn.lower().startswith(name.lower() + "#"):
                hits.append(os.path.join(out, fn))
    for h in hits:
        print("%8d  v%-12s %s" % (os.path.getsize(h), fileversion(h) or "-", h))
    return 0 if hits else 1


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    cmd = argv[0]
    if cmd == "build":
        return build(argv[1] if len(argv) > 1 else None,
                     argv[2] if len(argv) > 2 else None)
    if cmd == "version":
        print(fileversion(argv[1]) or "(no version resource)")
        return 0
    if cmd == "find":
        return find(argv[1])
    print(__doc__)
    return 2


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
