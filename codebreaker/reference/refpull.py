#!/usr/bin/env python3
"""
refpull.py — copy the target-relevant slice of an external source tree into
reference/, preserving structure, with a manifest.

Companion to refscan.py: refscan *measures* relevance, refpull *extracts* it.
It reuses the exact same classifier, so what you pull is exactly what the tally
scored. Default is the direct-9x-lineage gold (HIGH + HIGH-SHELL) — the
`base/mvdm/wow16` 16-bit KERNEL/GDI/USER/COMMDLG/DDEML/.DRV tree and the
shell32/comctl32/comdlg32 sources. Pass --tiers to widen (e.g. add MED, the
shared-Win32 algorithms).

Doctrine: this material is *hypothesis*, never truth (CONTEXT.md). It lands in
reference/, which the ledger never counts and .gitignore never commits.

    python3 refpull.py "/path/to/reference-tree" reference/reftree
    python3 refpull.py <tree> reference/reftree --tiers HIGH HIGH-SHELL MED
"""
import argparse
from codebreaker.foundation import cores
import os
import shutil
import sys

refscan = cores.get('refscan')


def pull(tree, dest, tiers):
    tree = os.path.abspath(tree)
    os.makedirs(dest, exist_ok=True)
    tierset = set(tiers)
    manifest = []
    n_files = 0
    n_bytes = 0

    def walk(dirpath):
        nonlocal n_files, n_bytes
        try:
            entries = list(os.scandir(dirpath))
        except OSError:
            return
        for e in entries:
            try:
                if e.is_dir(follow_symlinks=False):
                    if e.name.lower() not in refscan._SKIP_DIRS:
                        walk(e.path)
                    continue
            except OSError:
                continue
            rel = os.path.relpath(e.path, tree)
            bucket = refscan.classify(os.path.dirname(rel))
            if bucket not in tierset:
                continue
            dst = os.path.join(dest, rel)
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            try:
                shutil.copy2(e.path, dst)
                sz = os.path.getsize(dst)
            except OSError as ex:
                sys.stderr.write(f"  skip {rel}: {ex}\n")
                continue
            manifest.append((bucket, rel.replace("\\", "/"), sz))
            n_files += 1
            n_bytes += sz
            if n_files % 1000 == 0:
                sys.stderr.write(f"  ...{n_files:,} files copied\n")
                sys.stderr.flush()

    walk(tree)

    manifest.sort()
    mpath = os.path.join(dest, "MANIFEST.tsv")
    with open(mpath, "w", encoding="utf-8") as f:
        f.write(f"# pulled from {tree}\n")
        f.write(f"# tiers: {', '.join(sorted(tierset))}\n")
        f.write(f"# {n_files} files, {n_bytes} bytes\n")
        f.write("bucket\tpath\tbytes\n")
        for bucket, rel, sz in manifest:
            f.write(f"{bucket}\t{rel}\t{sz}\n")
    return n_files, n_bytes, mpath


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("tree", help="external source tree (e.g. RefTree)")
    ap.add_argument("dest", help="destination under reference/ (created)")
    ap.add_argument("--tiers", nargs="+", default=["HIGH", "HIGH-SHELL"],
                    help="relevance buckets to pull (default: HIGH HIGH-SHELL)")
    args = ap.parse_args()
    n, b, mp = pull(args.tree, args.dest, args.tiers)
    print(f"# pulled {n:,} files ({b / 1024 / 1024:.0f} MB) -> {args.dest}")
    print(f"# manifest: {mp}")


if __name__ == "__main__":
    main()
