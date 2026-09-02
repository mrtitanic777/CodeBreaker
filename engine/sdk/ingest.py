#!/usr/bin/env python3
r"""ingest.py -- SDK/DDK ingestion scanner (Phase 3).

Point it at an SDK/DDK root (headers, libs, binaries, docs, toolchains) and it
returns a structured Inventory: which files are headers / module-defs / import
libs / binaries (DLL/EXE) / toolchain executables / docs. Downstream, apidb.py
builds the API database from the headers+defs, and detect.py maps the toolchain
executables to backbone adapter names.

No network, no execution -- pure filesystem classification. Unknown files are
counted under `other`, never force-fit into a category.
"""
import os

_H = (".h", ".hpp", ".inc")
_DEF = (".def",)
_LIB = (".lib", ".a")
_BIN = (".dll", ".exe", ".drv", ".vxd", ".sys", ".ocx")
_DOC = (".txt", ".md", ".hlp", ".doc", ".rtf", ".htm", ".html")
_SRC = (".c", ".cpp", ".asm", ".rc")
_TOOLS = {"cl.exe", "link.exe", "masm.exe", "ml.exe", "rc.exe", "lib.exe",
          "tlink.exe", "bcc.exe", "wcc.exe", "dosxnt.exe"}


class Inventory:
    def __init__(self, root):
        self.root = root
        self.headers = []; self.defs = []; self.libs = []
        self.binaries = []; self.tools = []; self.docs = []; self.sources = []
        self.other = []

    def add(self, path):
        low = os.path.basename(path).lower()
        ext = os.path.splitext(low)[1]
        if low in _TOOLS:
            self.tools.append(path)
        elif ext in _H:
            self.headers.append(path)
        elif ext in _DEF:
            self.defs.append(path)
        elif ext in _LIB:
            self.libs.append(path)
        elif ext in _BIN:
            self.binaries.append(path)
        elif ext in _SRC:
            self.sources.append(path)
        elif ext in _DOC:
            self.docs.append(path)
        else:
            self.other.append(path)

    def summary(self):
        return {k: len(getattr(self, k)) for k in
                ("headers", "defs", "libs", "binaries", "tools", "sources", "docs", "other")}


def scan(root, max_files=200000):
    """Walk `root` and classify every file. Returns an Inventory."""
    inv = Inventory(os.path.abspath(root))
    n = 0
    for dp, _, fns in os.walk(root):
        for fn in fns:
            inv.add(os.path.join(dp, fn)); n += 1
            if n >= max_files:
                return inv
    return inv


def _selftest():
    from engine.sdk import w95paths
    root = w95paths.src("base", "lzexpand")
    inv = scan(root)
    print("root:", inv.root)
    for k, v in inv.summary().items():
        print("  %-9s %d" % (k, v))
    print("headers:", [os.path.basename(h) for h in inv.headers])
    print("defs:   ", [os.path.basename(d) for d in inv.defs])


if __name__ == "__main__":
    _selftest()
