#!/usr/bin/env python3
"""
midage.py — read the Midageproject symbol data as a naming-hypothesis oracle.

Midageproject (reference/midage, cloned from github.com/Midageproject) parsed
the surviving Win9x `.SYM` files into per-component YAML: each module lists, per
Windows version, its source_tree (original MS .asm/.c filenames) and its
segments, each segment carrying named symbols at offsets.

This turns one component into a flat table:  segment  offset  name

USED AS HYPOTHESIS ONLY (CONTEXT.md). Two caveats — and one big advantage over
the 95 project:
  * VERSION (the advantage). For the 16-bit/32-bit CORE (krnl386/user/gdi,
    kernel32/...) Midage's core data IS **Windows 98** — so for this project it
    is finally version-matched, not a distant cousin as it was for 95/OSR2.5.
    Names, segment layout and source-file structure should map closely, and
    OFFSETS have a real chance of lining up. Still not a free pass: Midage's
    "Windows 98 Retail" is 98 First Edition, and our corpus is 98 **Second
    Edition** — FE->SE drift is small but real, so confirm before trusting an
    exact offset.
  * These are names Microsoft's build gave its own symbols; attach one to a
    retail 98 SE address only after the bytes agree (match by segment shape /
    call graph / reference source, never by trusting the offset blindly).

Dependency-free: a line parser for Midage's regular 2-space YAML, no pyyaml.

Usage:
    python midage.py <component.yaml> --list-versions
    python midage.py <component.yaml> [--version "Windows 98SE Retail"]  # flat table
    python midage.py <component.yaml> --version "Windows 98 Retail" --grep Alloc
"""
import argparse
import os
import sys


def _hex(s):
    s = s.strip().strip("'\"").strip()
    try:
        return int(s, 16)
    except ValueError:
        return None


def parse(path):
    """{version: {'source_tree': [...], 'segments': [ {num,name,offset,
    symbols:[(name, offset)]} ]}}  — parsed from Midage's indented YAML."""
    out = {}
    cur_ver = None
    cur_seg = None
    section = None            # 'source_tree' | 'segments' | 'symbols'
    pend_sym = None           # (name) awaiting its offset
    with open(path, encoding="utf-8", errors="replace") as f:
        for raw in f:
            line = raw.rstrip("\n")
            if not line.strip():
                continue
            indent = len(line) - len(line.lstrip(" "))
            t = line.strip()

            if t.startswith("- version:"):
                cur_ver = t[len("- version:"):].strip().strip("'\"")
                out[cur_ver] = {"source_tree": [], "segments": []}
                cur_seg = None
                section = None
                continue
            if cur_ver is None:
                continue

            if t == "source_tree:":
                section = "source_tree"; continue
            if t == "segments:":
                section = "segments"; continue
            if t == "symbols:":
                section = "symbols"; continue

            if section == "source_tree" and t.startswith("- "):
                out[cur_ver]["source_tree"].append(t[2:].strip())
                continue

            # a new segment record: "- num: '1'"  (then name:/offset: follow)
            if section == "segments" and t.startswith("- num:"):
                cur_seg = {"num": t.split(":", 1)[1].strip().strip("'\""),
                           "name": "", "offset": None, "symbols": []}
                out[cur_ver]["segments"].append(cur_seg)
                continue
            # a new symbol record under a segment: "- name: foo"
            if section == "symbols" and t.startswith("- name:") and cur_seg is not None:
                pend_sym = t.split(":", 1)[1].strip().strip("'\"")
                continue

            # scalar fields
            if t.startswith("name:") and cur_seg is not None and section == "segments":
                cur_seg["name"] = t.split(":", 1)[1].strip().strip("'\"")
            elif t.startswith("offset:") and section == "segments" and cur_seg is not None \
                    and cur_seg["offset"] is None:
                cur_seg["offset"] = _hex(t.split(":", 1)[1])
            elif t.startswith("offset:") and section == "symbols" and pend_sym is not None:
                cur_seg["symbols"].append((pend_sym, _hex(t.split(":", 1)[1])))
                pend_sym = None
    return out


def pick_version(vers, want):
    if want:
        return want if want in vers else None
    # Prefer our exact target (98 Second Edition) first, then 98 FE, then the
    # nearest neighbours; "Windows 95" is the last-resort fallback.
    for pref in ("Windows 98SE Retail", "Windows 98 Retail", "Windows 98 Debug",
                 "Windows ME Retail", "Windows 95"):
        if pref in vers:
            return pref
    return next(iter(vers), None)


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("component", help="path to a Midage db/components/*.yaml")
    ap.add_argument("--version", help="which Windows version's table")
    ap.add_argument("--list-versions", action="store_true")
    ap.add_argument("--grep", help="only symbols whose name contains this (case-insens)")
    args = ap.parse_args()

    if not os.path.exists(args.component):
        # convenience: accept a bare module name
        cand = os.path.join("reference/midage/db/components", args.component)
        cand = cand if cand.endswith(".yaml") else cand + ".yaml"
        if os.path.exists(cand):
            args.component = cand
        else:
            sys.exit(f"not found: {args.component}")

    data = parse(args.component)
    if args.list_versions or not data:
        print(f"# {os.path.basename(args.component)}: versions")
        for v in data:
            nseg = len(data[v]["segments"])
            nsym = sum(len(s["symbols"]) for s in data[v]["segments"])
            print(f"  {v:<24} {nseg:>4} segments  {nsym:>6} symbols  "
                  f"{len(data[v]['source_tree'])} source files")
        return

    ver = pick_version(data, args.version)
    if ver is None:
        sys.exit(f"version {args.version!r} not present; try --list-versions")
    d = data[ver]
    g = args.grep.lower() if args.grep else None
    print(f"# {os.path.basename(args.component)}  [{ver}]  "
          f"(HYPOTHESIS — confirm vs 98 SE bytes; offsets are {ver})")
    if d["source_tree"]:
        print(f"# source_tree ({len(d['source_tree'])}): "
              f"{', '.join(d['source_tree'][:16])}"
              f"{' …' if len(d['source_tree']) > 16 else ''}")
    print(f"{'segment':<16} {'seg_off':>10} {'sym_off':>9}  name")
    n = 0
    for s in d["segments"]:
        for nm, off in s["symbols"]:
            if g and g not in nm.lower():
                continue
            so = f"0x{s['offset']:08x}" if s["offset"] is not None else "?"
            yo = f"0x{off:08x}" if off is not None else "?"
            print(f"{s['name']:<16} {so:>10} {yo:>9}  {nm}")
            n += 1
    print(f"# {n} symbols" + (f" matching {args.grep!r}" if g else ""))


if __name__ == "__main__":
    main()
