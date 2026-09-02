# reconcount.py — a worked example plugin: tally the reconstruction.
DESCRIPTION = "count reconstructed functions/files (quick project tally)"

import glob
import os
import re
import sys

FNDEF = re.compile(r"^[A-Za-z_][A-Za-z0-9_ \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{", re.M)
MODULE = re.compile(r"MODULE:\s*([A-Za-z0-9_.]+)")


def main():
    root = os.getcwd()
    files = glob.glob(os.path.join(root, "reconstructed", "*.c"))
    by_mod = {}
    total = 0
    for f in files:
        src = open(f, encoding="utf-8", errors="replace").read()
        m = MODULE.search(src)
        mod = m.group(1) if m else os.path.basename(f)
        n = len(FNDEF.findall(src))
        by_mod[mod] = by_mod.get(mod, 0) + n
        total += n
    print("reconstruction tally: %d functions across %d files, %d modules"
          % (total, len(files), len(by_mod)))
    for mod, n in sorted(by_mod.items(), key=lambda kv: -kv[1]):
        print("  %-16s %d" % (mod, n))


if __name__ == "__main__":
    main()
