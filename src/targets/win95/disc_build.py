#!/usr/bin/env python3
r"""
disc_build.py -- assemble a functional self-hosted Windows 95 install disc.

Rebuilds the WIN95 install source by repacking every original cabinet IN PLACE:
each output CAB carries the same file list as the original (so SETUP's LAYOUT.INF
still resolves file->cabinet), but its bytes are (a) our byte-exact reconstructed
binaries where we have them, else (b) the file expanded from the original media.
Loose files (INF control files, SETUP.EXE, ...) are copied through. Then isowrite
lays it out as an ISO. Our binaries are byte-identical, so the installed system is
byte-identical; only the CAB/ISO container is functional-not-identical.

    python disc_build.py --src disc/src/WIN95 --files disc/files \
                         --out win95re.iso [--our staged/bin] [--boot BOOT.IMG]
"""
import glob
import os
import shutil
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, ROOT)


def _cab_chain(cabpath):
    """(set_id, cab_index, prev, nxt) from a cabinet's header; prev/nxt are
    (cabinet_name, disk_name) tuples or None. Preserves cabinet-SET linkage so a
    repacked spanning set (PRECOPY1<->PRECOPY2) still chains for SETUP's precopy."""
    import struct
    d = open(cabpath, "rb").read()
    flags, set_id, cab_index = struct.unpack_from("<HHH", d, 30)
    off = 36
    if flags & 0x0004:                                # reserve present
        cbH = struct.unpack_from("<H", d, off)[0]
        off += 4 + cbH

    def cstr(o):
        e = d.index(b"\0", o)
        return d[o:e].decode("latin1", "replace"), e + 1
    prev = nxt = None
    if flags & 0x0001:
        c, off = cstr(off); dk, off = cstr(off); prev = (c, dk)
    if flags & 0x0002:
        c, off = cstr(off); dk, off = cstr(off); nxt = (c, dk)
    return set_id, cab_index, prev, nxt


_INDEX_CACHE = {}


def _dir_index(d):
    """Cached {basename.lower(): path} over every file under `d`, at ANY depth.
    This is what lets a nested install-tree corpus (files/WINDOWS/SYSTEM/…, or our
    image/WINDOWS/…) resolve by bare basename the same way a flat dir would — module
    names are unique, so a filename maps to one path wherever it physically sits. Same
    idea as target._files_index(), generalized to any root so it serves both the media
    corpus and the `our` reconstruction tree without copying 133 MB into a flat view."""
    if not d:
        return {}
    key = os.path.abspath(d)
    idx = _INDEX_CACHE.get(key)
    if idx is None:
        idx = {}
        for dp, _dns, fns in os.walk(d):
            for fn in fns:
                idx.setdefault(fn.lower(), os.path.join(dp, fn))
        _INDEX_CACHE[key] = idx
    return idx


def _resolve(name, our, files_dir):
    """Where to get file `name` from: our reconstructed build first, else the media
    expansion. Both roots may be a flat dir OR a nested install-tree; the flat join is
    tried first (cheap) and a recursive basename index bridges the nested case.
    Returns (path, kind) or (None, None)."""
    lo = name.lower()
    if our:
        p = os.path.join(our, lo)                     # flat `our` fast-path
        if os.path.isfile(p):
            return p, "ours"
        p = _dir_index(our).get(lo)                   # nested image/ tree
        if p:
            return p, "ours"
    p = os.path.join(files_dir, name)                 # flat media fast-path
    if os.path.isfile(p):
        return p, "media"
    p = os.path.join(files_dir, lo)
    if os.path.isfile(p):
        return p, "media"
    p = _dir_index(files_dir).get(lo)                 # nested files/WINDOWS/SYSTEM/… tree
    if p:
        return p, "media"
    return None, None


def build(src_win95, files_dir, out_iso, our=None, boot=None, volid="WIN95",
          out_root=None, progress=None, subst_cabs=None):
    """subst_cabs: if given (a set of upper-case CAB basenames), OUR binaries are substituted ONLY into
    those cabinets; every other cabinet is repacked from the original media only. Lets a caller put our
    binaries into the INSTALLED cabinets while leaving the Setup-runtime cabinets (MINI/PRECOPY*)
    original -- so Setup runs on stock code and our binaries are exercised on first boot."""
    import cab, cabwrite, isowrite
    out_root = out_root or os.path.join("disc", "out")
    win95_out = os.path.join(out_root, "WIN95")
    os.makedirs(win95_out, exist_ok=True)

    n_ours = n_media = n_missing = n_cabs = 0
    # 1) repack each cabinet from its original file list
    for cabpath in sorted(glob.glob(os.path.join(src_win95, "*.[cC][aA][bB]"))):
        try:
            _info, cfiles = cab.read_cab(cabpath)
        except Exception:
            shutil.copy2(cabpath, win95_out)          # unreadable header: pass through
            continue
        # gate substitution to the requested cabinets (None = all)
        cab_our = our if (subst_cabs is None or
                          os.path.basename(cabpath).upper() in subst_cabs) else None
        payload = []
        complete = True
        for cf in cfiles:
            path, kind = _resolve(cf.name, cab_our, files_dir)
            if path is None:
                complete = False
                break
            payload.append((cf.name, open(path, "rb").read(), kind))
        if complete and payload:                      # repack with our substitutions
            set_id, cab_index, prev, nxt = _cab_chain(cabpath)
            cabwrite.write_cab([(n, d) for n, d, _k in payload],
                               os.path.join(win95_out, os.path.basename(cabpath)),
                               set_id=set_id, cab_index=cab_index, prev=prev, nxt=nxt)
            n_ours += sum(1 for _n, _d, k in payload if k == "ours")
            n_media += sum(1 for _n, _d, k in payload if k == "media")
            n_cabs += 1
        else:                                         # can't fully gather -> pass original through
            shutil.copy2(cabpath, win95_out)
            n_missing += 1
        if progress:
            progress(n_cabs)

    # 2) copy loose (non-CAB) files: INF control files, SETUP.EXE, etc.
    for f in sorted(os.listdir(src_win95)):
        sp = os.path.join(src_win95, f)
        if os.path.isfile(sp) and not f.lower().endswith(".cab"):
            shutil.copy2(sp, win95_out)

    # 3) lay out the ISO
    size = isowrite.write_iso(out_root, out_iso, volid=volid, boot_img=boot)
    return {"iso": out_iso, "iso_bytes": size, "cabs": n_cabs,
            "files_ours": n_ours, "files_media": n_media, "missing": n_missing}


def main():
    a = sys.argv
    def opt(flag, default=None):
        return a[a.index(flag) + 1] if flag in a else default
    src = opt("--src", os.path.join("disc", "src", "WIN95"))
    files_dir = opt("--files", os.path.join("disc", "files"))
    out = opt("--out", "win95re.iso")
    our = opt("--our")
    boot = opt("--boot")
    volid = opt("--volid", "WIN95")
    os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")
    from src.core import config
    config.apply_config()
    if not os.path.isdir(files_dir):
        sys.exit("expanded media dir not found: %s (run the CAB expansion first)" % files_dir)
    print("assembling self-hosted disc: src=%s files=%s -> %s" % (src, files_dir, out))
    n = [0]
    def prog(c):
        n[0] = c
        if c % 25 == 0:
            print("  repacked %d cabs..." % c, flush=True)
    r = build(src, files_dir, out, our=our, boot=boot, volid=volid, progress=prog)
    print("=" * 60)
    print("DISC: %s  (%d bytes, %d cabs repacked)" % (r["iso"], r["iso_bytes"], r["cabs"]))
    print("  files from OUR reconstruction: %d" % r["files_ours"])
    print("  files from media (data/not-yet-reconstructed): %d" % r["files_media"])
    if r["missing"]:
        print("  MISSING (not found in either): %d" % r["missing"])
    print("=" * 60)


if __name__ == "__main__":
    main()
