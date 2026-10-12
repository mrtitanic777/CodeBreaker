#!/usr/bin/env python3
r"""
disc_build.py -- assemble a functional self-hosted Windows 98 install disc.

Rebuilds the WIN98 install source by repacking every original cabinet IN PLACE:
each output CAB carries the same file list as the original (so SETUP's LAYOUT.INF
still resolves file->cabinet), but its bytes are (a) our byte-exact reconstructed
binaries where we have them, else (b) the file expanded from the original media.
Loose files (INF control files, SETUP.EXE, ...) are copied through. Then isowrite
lays it out as an ISO. Our binaries are byte-identical, so the installed system is
byte-identical; only the CAB/ISO container is functional-not-identical.

    python disc_build.py --src disc_src/WIN98 --files disc_files \
                         --out win98re.iso [--our staged/bin] [--boot BOOT.IMG]
"""
import glob
import os
import shutil
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, ROOT)


def _resolve(name, our, files_dir):
    """Where to get file `name` from: our reconstructed build first, else the media
    expansion. Returns a path or None."""
    lo = name.lower()
    if our:
        p = os.path.join(our, lo)
        if os.path.isfile(p):
            return p, "ours"
    p = os.path.join(files_dir, name)
    if os.path.isfile(p):
        return p, "media"
    p = os.path.join(files_dir, lo)
    if os.path.isfile(p):
        return p, "media"
    return None, None


def build(src_win98, files_dir, out_iso, our=None, boot=None, volid="WIN98",
          out_root=None, progress=None):
    import cab, cabwrite
    from src.engines.build import isowrite
    out_root = out_root or os.path.join("disc_out")
    win98_out = os.path.join(out_root, "WIN98")
    os.makedirs(win98_out, exist_ok=True)

    n_ours = n_media = n_missing = n_cabs = 0
    # 1) repack each cabinet from its original file list
    for cabpath in sorted(glob.glob(os.path.join(src_win98, "*.[cC][aA][bB]"))):
        try:
            _info, cfiles = cab.read_cab(cabpath)
        except Exception:
            shutil.copy2(cabpath, win98_out)          # unreadable header: pass through
            continue
        payload = []
        complete = True
        for cf in cfiles:
            path, kind = _resolve(cf.name, our, files_dir)
            if path is None:
                complete = False
                break
            payload.append((cf.name, open(path, "rb").read(), kind))
        if complete and payload:                      # repack with our substitutions
            cabwrite.write_cab([(n, d) for n, d, _k in payload],
                               os.path.join(win98_out, os.path.basename(cabpath)))
            n_ours += sum(1 for _n, _d, k in payload if k == "ours")
            n_media += sum(1 for _n, _d, k in payload if k == "media")
            n_cabs += 1
        else:                                         # can't fully gather -> pass original through
            shutil.copy2(cabpath, win98_out)
            n_missing += 1
        if progress:
            progress(n_cabs)

    # 2) copy loose (non-CAB) files: INF control files, SETUP.EXE, etc.
    for f in sorted(os.listdir(src_win98)):
        sp = os.path.join(src_win98, f)
        if os.path.isfile(sp) and not f.lower().endswith(".cab"):
            shutil.copy2(sp, win98_out)

    # 3) lay out the ISO
    size = isowrite.write_iso(out_root, out_iso, volid=volid, boot_img=boot)
    return {"iso": out_iso, "iso_bytes": size, "cabs": n_cabs,
            "files_ours": n_ours, "files_media": n_media, "missing": n_missing}


def main():
    a = sys.argv
    def opt(flag, default=None):
        return a[a.index(flag) + 1] if flag in a else default
    src = opt("--src", os.path.join("disc_src", "WIN98"))
    files_dir = opt("--files", "disc_files")
    out = opt("--out", "win98re.iso")
    our = opt("--our")
    boot = opt("--boot")
    volid = opt("--volid", "WIN98")
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
