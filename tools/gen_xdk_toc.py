#!/usr/bin/env python3
"""Build a navigable index of the Xbox 360 XDK documentation from the CHM's own
table-of-contents (Xbox360SDK.hhc). The .hhc is a single-line HTML "sitemap":
nested <UL>/<LI>, each <LI> carrying an <OBJECT> with a Name (title) and Local (file).
A <UL> that follows an <LI> holds that LI's children -> that nesting is the section tree.

Outputs (into docs/platform/):
  xdk-toc.tsv   depth<TAB>breadcrumb<TAB>title<TAB>file   (one row per topic, in TOC order)
  xdk-toc.txt   indented tree, for eyeballing the structure quickly

Usage:  python tools/gen_xdk_toc.py
Re-run whenever the XDK docs folder changes.
"""
import os, re, html

DOCS = r"C:\Users\mrtit\Desktop\XDK Documentation"
HHC  = os.path.join(DOCS, "Xbox360SDK.hhc")
OUT  = os.path.join(os.path.dirname(__file__), "..", "docs", "platform")

# Token stream we care about, in document order: <UL>, </UL>, and each sitemap OBJECT.
TOKEN = re.compile(r"<UL>|</UL>|<OBJECT\b[^>]*>(.*?)</OBJECT>", re.IGNORECASE | re.DOTALL)
PARAM = re.compile(r'<param\s+name="([^"]*)"\s+value="([^"]*)"', re.IGNORECASE)

def parse(hhc_text):
    """Yield (depth, title, file) in TOC order. depth = enclosing <UL> nesting (>=1)."""
    depth = 0
    for m in TOKEN.finditer(hhc_text):
        tok = m.group(0)
        low = tok[:4].lower()
        if low == "<ul>":
            depth += 1
        elif low == "</ul":
            depth = max(0, depth - 1)
        else:  # OBJECT
            params = dict(PARAM.findall(m.group(1)))
            name = html.unescape(params.get("Name", "").strip())
            local = params.get("Local", "").strip()
            if name:  # skip the empty "site properties" OBJECT (no Name)
                yield depth, name, local

def main():
    with open(HHC, "r", encoding="utf-8", errors="replace") as f:
        text = f.read()

    rows = list(parse(text))
    # Build breadcrumb by tracking the last title seen at each depth.
    trail = {}
    out_rows = []
    for depth, title, local in rows:
        trail[depth] = title
        for d in list(trail):        # forget deeper crumbs once we pop back up
            if d > depth:
                del trail[d]
        crumb = " > ".join(trail[d] for d in range(1, depth) if d in trail)
        out_rows.append((depth, crumb, title, local))

    os.makedirs(OUT, exist_ok=True)
    tsv = os.path.join(OUT, "xdk-toc.tsv")
    with open(tsv, "w", encoding="utf-8") as f:
        f.write("depth\tbreadcrumb\ttitle\tfile\n")
        for depth, crumb, title, local in out_rows:
            f.write("%d\t%s\t%s\t%s\n" % (depth, crumb, title, local))

    txt = os.path.join(OUT, "xdk-toc.txt")
    with open(txt, "w", encoding="utf-8") as f:
        for depth, crumb, title, local in out_rows:
            f.write("%s%s\t[%s]\n" % ("  " * (depth - 1), title, local))

    # Quick stats
    topics = len(out_rows)
    with_file = sum(1 for r in out_rows if r[3])
    maxd = max((r[0] for r in out_rows), default=0)
    print("parsed %d TOC topics (%d with a file), max depth %d" % (topics, with_file, maxd))
    print("wrote %s" % tsv)
    print("wrote %s" % txt)
    print("\nTop-level sections:")
    for depth, crumb, title, local in out_rows:
        if depth == 1:
            print("  - %s  [%s]" % (title, local))

if __name__ == "__main__":
    main()
