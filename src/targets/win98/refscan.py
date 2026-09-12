#!/usr/bin/env python3
"""
refscan.py — tally an external reference source tree and score its relevance to
the Windows 98 reconstruction.

The RefTree tree is an external reference source tree you supply. It is NOT
Windows 98 source — NT and 9x are different kernels — so most of it is noise for
this project. But specific subtrees carry real cross-reference value, because
9x and NT share ancestry in exactly a few places:

  * the **16-bit WOW / MVDM** sources — the 16-bit USER/GDI/KERNEL that Win98's
    KRNL386/USER/GDI descend from (same code family as 3.1's leaked WOW16);
  * the **USER/GDI Ring-3 client** code (`ntuser/client`, `ntgdi/client`) —
    dialog/menu/edit/combo/DDE logic whose algorithms match USER32/GDI32;
  * the **shell lineage** — shell32/comctl32/comdlg32 were born in Win98 and the
    NT versions are direct descendants.

Everything else — the NT kernel (ntos), win32k kernel-mode side, Active
Directory (ds), IIS/IE (inetsrv/inetcore), Terminal Server, NT networking — has
little to no bearing on retail Win98 and must not inflate a "how much helps 95"
number. So this tool reports the tree TWICE: the full size, and the
relevance-weighted slice.

Doctrine note (CONTEXT.md): even the HIGH-relevance code is *hypothesis*. NT's
USER32 client is not Win98's USER.EXE; it says what to look for, never what is
true. This tally sizes the haystack, it does not bless any needle.

    python3 refscan.py "/path/to/reference-tree"
    python3 refscan.py <tree> --md reports/refscan_reftree.md
"""
import argparse
import os
import sys

# Source-like extensions we count lines for; everything else is tallied by
# file count only (binaries, resources, build junk).
SRC_EXT = {".c", ".cpp", ".cxx", ".cc", ".h", ".hpp", ".hxx", ".asm", ".inc",
           ".def", ".spec", ".rc", ".idl", ".thk", ".w", ".s"}

# Relevance classifier. First matching rule wins; matched against the lowercased
# path relative to the tree root (forward-slashed). Tuned for an NT5 tree.
# (bucket, [substrings]) — order matters, most-specific first.
RULES = [
    # HIGH = direct 9x lineage. The 16-bit Windows source under mvdm/wow16 is
    # essentially the 3.1 KERNEL/GDI/USER/COMMDLG/DDEML/.DRV tree that Win98's
    # 16-bit core descends from; plus the USER/GDI Ring-3 client and the thunk
    # code. Note: bare /mvdm/ is the DOS-VM box (command.com, v86) — that is
    # MS-DOS, NOT the 16-bit Windows core, so it is MED not HIGH.
    ("HIGH", [
        "/mvdm/wow16/", "/mvdm/wow32/", "/wow16/", "/wow32/",
        "/ntuser/client", "/ntgdi/client",
        "/thunk", "genthunk", "flatthnk", "wowthunk", "/wthunk",
    ]),
    ("HIGH-SHELL", [
        "/shell/comctl32", "/shell/comdlg32", "/shell/shell32",
        "/shell/cpls", "/shell/shdocvw", "/shell/browseui",
        "/shell32", "/comctl32", "/comdlg32",
    ]),
    ("MED", [
        # shared Win32 algorithms / same API, different build; and the DOS-VM
        # + generic mvdm layer (relevant to Win98's DOS box, not its core).
        "/mvdm/", "/ntuser/", "/ntgdi/", "/mmsystem", "/winmm",
        "/multimedia/", "/win32/", "/base/win32", "/base/client",
        "/published/", "/printscan/gdi", "/shell/",
    ]),
    ("LOW", [
        "/base/", "/printscan/", "/com/", "/mergedcomponents/", "/tools/",
        "/sdktools/", "/loc/", "/developer/", "/enduser/", "/inc",
    ]),
    ("NONE", [
        # NT-only architecture: kernel-mode win32k, NT exec, AD, IIS/IE, net.
        "/win32k", "/ntos/", "/ds/", "/termsrv/", "/inetsrv/", "/inetcore/",
        "/net/", "/admin/", "/drivers/",
    ]),
]

BUCKETS = ["HIGH", "HIGH-SHELL", "MED", "LOW", "NONE", "UNCLASSIFIED"]


def classify(relpath):
    p = "/" + relpath.replace("\\", "/").lower()
    for bucket, needles in RULES:
        if any(n in p for n in needles):
            return bucket
    return "UNCLASSIFIED"


_SKIP_DIRS = {".git", ".svn", "obj", "objd", "objfre", "objchk",
              "objfre_wxp_x86", "objchk_wxp_x86"}
_MAX_LINECOUNT_BYTES = 8 * 1024 * 1024   # don't slurp multi-MB generated files


def count_lines(path, size):
    # read in chunks so a stray huge source file can't blow memory or time
    try:
        n = 0
        with open(path, "rb", buffering=1 << 20) as f:
            while True:
                chunk = f.read(1 << 20)
                if not chunk:
                    break
                n += chunk.count(b"\n")
        return n + 1
    except OSError:
        return 0


def scan(root, progress_every=20000):
    root = os.path.abspath(root)
    # per top-level component: [files, src_files, src_lines, bytes,
    #                           gold_lines (HIGH+HIGH-SHELL), shared_lines (MED)]
    comp = {}
    bucket = {b: [0, 0, 0, 0] for b in BUCKETS}
    ext = {}
    seen = [0]

    def walk(dirpath):
        try:
            entries = list(os.scandir(dirpath))
        except OSError:
            return
        for e in entries:
            try:
                if e.is_dir(follow_symlinks=False):
                    if e.name.lower() not in _SKIP_DIRS:
                        walk(e.path)
                    continue
            except OSError:
                continue
            rel = os.path.relpath(e.path, root)
            top = rel.split(os.sep)[0] if os.sep in rel else "(root)"
            b = classify(os.path.dirname(rel))
            try:
                sz = e.stat(follow_symlinks=False).st_size
            except OSError:
                sz = 0
            ext_ = os.path.splitext(e.name)[1].lower()
            c = comp.setdefault(top, [0, 0, 0, 0, 0, 0])
            c[0] += 1
            c[3] += sz
            bk = bucket[b]
            bk[0] += 1
            bk[3] += sz
            ec = ext.setdefault(ext_, [0, 0])
            ec[0] += 1
            if ext_ in SRC_EXT and sz <= _MAX_LINECOUNT_BYTES:
                ln = count_lines(e.path, sz)
                c[1] += 1
                c[2] += ln
                bk[1] += 1
                bk[2] += ln
                ec[1] += ln
                if b in ("HIGH", "HIGH-SHELL"):
                    c[4] += ln
                elif b == "MED":
                    c[5] += ln
            seen[0] += 1
            if progress_every and seen[0] % progress_every == 0:
                sys.stderr.write(f"  ...{seen[0]:,} files scanned\n")
                sys.stderr.flush()

    walk(root)
    return comp, bucket, ext


def human(n):
    for unit in ("B", "KB", "MB", "GB"):
        if n < 1024:
            return f"{n:.0f}{unit}"
        n /= 1024
    return f"{n:.1f}TB"


def report(root, comp, bucket, ext, out=sys.stdout):
    def w(s=""):
        out.write(s + "\n")

    w(f"# refscan — {root}")
    w()
    tot_files = sum(c[0] for c in comp.values())
    tot_src = sum(c[1] for c in comp.values())
    tot_lines = sum(c[2] for c in comp.values())
    tot_bytes = sum(c[3] for c in comp.values())
    w(f"**Whole tree:** {tot_files:,} files · {tot_src:,} source files · "
      f"{tot_lines:,} source lines · {human(tot_bytes)}")
    w()

    w("## Relevance to Windows 98 (weighted slice)")
    w()
    w("| bucket | meaning | files | src files | src lines | bytes |")
    w("|---|---|--:|--:|--:|--:|")
    meaning = {
        "HIGH": "16-bit WOW/MVDM + USER/GDI Ring-3 client — direct 9x lineage",
        "HIGH-SHELL": "shell32/comctl32/comdlg32 — born in Win98",
        "MED": "USER/GDI/win32/mm — shared algorithms, different build",
        "LOW": "base/com/tools — occasional reference",
        "NONE": "NT kernel/AD/IIS/net/win32k — no bearing on 9x",
        "UNCLASSIFIED": "unmatched — inspect",
    }
    for b in BUCKETS:
        f, s, ln, by = bucket[b]
        w(f"| {b} | {meaning[b]} | {f:,} | {s:,} | {ln:,} | {human(by)} |")
    hi = sum(bucket[b][2] for b in ("HIGH", "HIGH-SHELL"))
    usable = hi + bucket["MED"][2]
    w()
    pct_hi = 100 * hi / tot_lines if tot_lines else 0
    pct_use = 100 * usable / tot_lines if tot_lines else 0
    w(f"**HIGH (direct-lineage) = {hi:,} source lines ({pct_hi:.1f}% of tree).**  ")
    w(f"**HIGH+MED (plausibly useful) = {usable:,} source lines "
      f"({pct_use:.1f}% of tree).**  ")
    w(f"The other ~{100 - pct_use:.0f}% is NT-only and should be ignored for 95.")
    w()

    w("## By top-level component")
    w()
    w("Sorted by **95-gold** (HIGH + HIGH-SHELL source lines) — the direct-9x-"
      "lineage code in each component. `shared` = MED lines (same Win32 API, "
      "different build).")
    w()
    w("| component | src lines | 95-gold | shared | files | bytes |")
    w("|---|--:|--:|--:|--:|--:|")
    for top in sorted(comp, key=lambda t: (-comp[t][4], -comp[t][5])):
        c = comp[top]
        gold = f"**{c[4]:,}**" if c[4] else "0"
        w(f"| {top} | {c[2]:,} | {gold} | {c[5]:,} | {c[0]:,} | {human(c[3])} |")
    w()

    w("## By language / extension (top 20 by lines)")
    w()
    w("| ext | files | lines |")
    w("|---|--:|--:|")
    for e, (fc, ln) in sorted(ext.items(), key=lambda kv: -kv[1][1])[:20]:
        w(f"| {e or '(none)'} | {fc:,} | {ln:,} |")


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("tree", help="reference source tree to tally")
    ap.add_argument("--md", metavar="FILE", help="also write the report to FILE")
    args = ap.parse_args()
    comp, bucket, ext = scan(args.tree)
    report(args.tree, comp, bucket, ext)
    if args.md:
        os.makedirs(os.path.dirname(args.md) or ".", exist_ok=True)
        with open(args.md, "w", encoding="utf-8") as f:
            report(args.tree, comp, bucket, ext, out=f)
        print(f"\n# wrote {args.md}")


if __name__ == "__main__":
    main()
