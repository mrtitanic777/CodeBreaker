"""project.py -- one program being rebuilt: a directory and its plain-text descriptions.

    project.json   {"image": the original program (a path from the project's directory),
                    "toolchain": profile name, "variant": the compiler's variant,
                    "switches": the switches most files had,
                    "dgroup": image offset of the data segment (DGROUP),
                    "code": "near" (one code segment; near calls wrap at 64K) | "far",
                    "src": the sources' folder (default "src"),
                    "include": [folders of headers searched before the kit's]}
    src/SYMBOLS    name  C|D|S  address        (C code: image offset; D data: DS offset; S a segment)
    src/fADDR.c    one function each (its first function is at ADDR)
    src/LEDGER     what each function file came to

    p = project.load(folder)
    p.image  p.relocs()  p.symbols()  p.save_symbols(s)  p.toolchain()  p.sources()
"""
import json
import os
import re
import struct


class Project(object):
    def __init__(self, folder):
        self.dir = os.path.abspath(folder)
        with open(os.path.join(self.dir, "project.json")) as f:
            self.cfg = json.load(f)
        c = self.cfg
        self.exe = os.path.normpath(os.path.join(self.dir, c["image"]))
        self.src = os.path.join(self.dir, c.get("src", "src"))
        self.include = [os.path.normpath(os.path.join(self.dir, x)) for x in c.get("include", [])]
        self.dgroup = int(c["dgroup"], 0) if isinstance(c.get("dgroup"), str) else c.get("dgroup", 0)
        self.code_mask = 0xFFFF if c.get("code", "near") == "near" else 0xFFFFF
        self.switches = c.get("switches")
        b = open(self.exe, "rb").read()
        self.file = b
        if b[:2] in (b"MZ", b"ZM"):
            h = struct.unpack_from("<14H", b, 0)
            self.header = h
            self.image = b[h[4] * 16:h[2] * 512 - ((512 - h[1]) if h[1] else 0)]
        else:
            self.header = None
            self.image = b
        self._relocs = None

    def relocs(self):
        """the places in the load image where the loader adds the load segment"""
        if self._relocs is None:
            h = self.header
            self._relocs = {o + s * 16 for o, s in struct.iter_unpack("<HH", self.file[h[12]:h[12] + 4 * h[3]])} if h else set()
        return self._relocs

    def toolchain(self):
        from codebreaker.exact import toolchain
        return toolchain.load(self.cfg["toolchain"], self.cfg.get("variant"))

    def path(self, name):
        return os.path.join(self.src, name)

    def symbols(self):
        s = {}
        p = self.path("SYMBOLS")
        if os.path.exists(p):
            for l in open(p):
                c = l.split()
                if len(c) >= 3 and not l.startswith("#"):
                    s[c[0]] = (c[1], int(c[2], 16))
        return s

    def save_symbols(self, s):
        with open(self.path("SYMBOLS"), "w", newline="\n") as f:
            f.write("# name, C (code: image offset) or D (data: offset in the data segment), address\n")
            for k, (t, a) in sorted(s.items(), key=lambda kv: (kv[1][0], kv[1][1], kv[0])):
                f.write("%s %s %04x\n" % (k, t, a))

    def sources(self):
        """[(address, path)] of the one-function files"""
        out = []
        for f in os.listdir(self.src):
            m = re.fullmatch(r"f([0-9a-f]{5})\.c", f)
            if m:
                out.append((int(m.group(1), 16), os.path.join(self.src, f)))
        return sorted(out)

    def headers(self):
        """{name: bytes} the headers of the sources' folder (the compiler finds them beside the file)"""
        return {f: open(os.path.join(self.src, f), "rb").read() for f in sorted(os.listdir(self.src)) if f.lower().endswith(".h")}


def load(folder):
    return Project(folder)
