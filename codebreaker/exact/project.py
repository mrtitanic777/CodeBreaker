"""project.py -- one program being rebuilt: a directory and its plain-text descriptions.

    project.json   {"image": the original program (a path from the project's directory),
                    "toolchain": profile name, "variant": the compiler's variant,
                    "toolchains": [{"label", "toolchain", "variant"}] instead, when the program's
                        compiler is not in hand and several come near it (a function counts when one
                        of them gives its bytes),
                    "switches": the switches most files had,
                    "dgroup": image offset of the data segment (DGROUP),
                    "code_segments": [image offsets where the code segments begin] (default [0]),
                    "loadseg": the paragraph the image's segment words were made for (default 0),
                    "starts": a file (from the project's directory) listing where every function
                        begins, one hex address a line: a function is then only identical when it
                        reaches the next (it is not shorter than the program's),
                    "code_end": where the code ends (with "starts"),
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
    def __init__(self, folder, override=None):
        self.dir = os.path.abspath(folder)
        with open(os.path.join(self.dir, "project.json")) as f:
            self.cfg = json.load(f)
        if override:                                        # (a set of the project's sources: its own folder,
            self.cfg = dict(self.cfg)                       #  pattern, toolchain)
            self.cfg.pop("also", None)
            self.cfg.update(override)
        c = self.cfg
        self.exe = os.path.normpath(os.path.join(self.dir, c["image"]))
        self.src = os.path.join(self.dir, c.get("src", "src"))
        self.include = [os.path.normpath(os.path.join(self.dir, x)) for x in c.get("include", [])]
        self.dgroup = int(c["dgroup"], 0) if isinstance(c.get("dgroup"), str) else c.get("dgroup", 0)
        num = lambda x: int(x, 0) if isinstance(x, str) else x
        self.code_segments = sorted(num(x) for x in c.get("code_segments", [0]))
        self.loadseg = num(c.get("loadseg", 0))
        self.code_end = num(c.get("code_end", 0))
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
        self.packed = None                                  # (the original as EXEPACK left it, when the image is the
        if c.get("packed"):                                 #  unpacked program: its entry, stack, relocations)
            from codebreaker.exact import exepack
            with open(os.path.normpath(os.path.join(self.dir, c["packed"])), "rb") as f:
                self.packed_file = f.read()
            self.packed = exepack.read(self.packed_file)
        if c.get("code_segments") == "relocs" or c.get("code", "near") == "far" and "code_segments" not in c:
            # (far code: the segments the loader sets, from the words the relocation table points at - those
            # outside the data segment)
            dend = num(c.get("data_end", 0)) or None
            segs = {struct.unpack_from("<H", self.image, a)[0] * 16 for a in self.relocs() if a + 2 <= len(self.image)}
            self.code_segments = sorted({0} | {s for s in segs if s < len(self.image) and
                                               not (self.dgroup <= s < (dend or self.dgroup + 1))})

    def relocs(self):
        """the places in the load image where the loader adds the load segment"""
        if self._relocs is None:
            h = self.header
            self._relocs = {o + s * 16 for o, s in struct.iter_unpack("<HH", self.file[h[12]:h[12] + 4 * h[3]])} if h else set()
        return self._relocs

    def sets(self):
        """[Project] - this one, and the other sets of sources project.json lists ("also": [{"src", "sources",
        "toolchain", ...}]: an assembler's modules beside a compiler's files)"""
        return [self] + [Project(self.dir, x) for x in self.cfg.get("also", [])]

    def code_base(self, a):
        """the image offset of the code segment `a` is in"""
        return max([b for b in self.code_segments if b <= a] or [0])

    def own_end(self):
        """where the program's own code ends (the last of its functions, exact/carve.py)"""
        if getattr(self, "_own_end", None) is None:
            from codebreaker.exact import carve
            fs = carve.functions(self)
            self._own_end = max(a + n for a, n, w in fs) if fs else 0
        return self._own_end

    def starts(self):
        """[image offsets where functions begin], or None"""
        if not self.cfg.get("starts"):
            return None
        p = os.path.join(self.dir, self.cfg["starts"])
        return sorted(int(l.split()[0], 16) for l in open(p) if l.strip() and not l.startswith("#"))

    def toolchain(self):
        return self.toolchains()[0][1]

    def toolchains(self):
        """[(label, Toolchain)]"""
        from codebreaker.exact import toolchain
        c = self.cfg
        if "toolchains" in c:
            return [(x.get("label") or x["toolchain"], toolchain.load(x["toolchain"], x.get("variant"))) for x in c["toolchains"]]
        return [(c["toolchain"], toolchain.load(c["toolchain"], c.get("variant")))]

    def path(self, name):
        return os.path.join(self.src, name)

    def symbols(self):
        s = {}
        p = self.path("SYMBOLS")
        if os.path.exists(p):
            for l in open(p):
                c = l.split()
                if l.startswith("#") or len(c) < 2:
                    continue
                if len(c) >= 3 and c[1] in ("C", "D", "S"):
                    s[c[0]] = (c[1], int(c[2], 16))
                elif re.fullmatch(r"[0-9a-fA-F]+", c[1]):     # (name address: of the code or the data, unsaid)
                    s[c[0]] = ("?", int(c[1], 16))
        return s

    def save_symbols(self, s):
        with open(self.path("SYMBOLS"), "w", newline="\n") as f:
            f.write("# name, C (code: image offset) or D (data: offset in the data segment), address\n")
            for k, (t, a) in sorted(s.items(), key=lambda kv: (kv[1][0], kv[1][1], kv[0])):
                f.write("%s %s %04x\n" % (k, t, a))

    def sources(self):
        """[(address, path)] of the one-function files, and of the whole files src/ORDER lists (name, the address
        of its text: its functions lie one after the other from there)"""
        out = []
        rx = self.cfg.get("sources", r"f([0-9a-f]{5})\.c")     # (project "sources": the pattern of the file names,
        for f in os.listdir(self.src):                          #  group 1 the address)
            m = re.fullmatch(rx, f, re.I)
            if m:
                out.append((int(m.group(1), 16), os.path.join(self.src, f)))
        if self.cfg.get("place_by"):                        # (whole files placed by a list of names and places:
            places = {}                                     #  each at its first function's)
            for l in open(self.path(self.cfg["place_by"]), encoding="utf-8"):
                c = l.split("#")[0].split()
                if len(c) >= 2:
                    m = re.fullmatch(r"[fF]?([0-9a-fA-F]{4,5})", c[1])
                    if m:
                        places[c[0].upper()[:8]] = int(m.group(1), 16)
            for f in sorted(os.listdir(self.src)):
                if not f.lower().endswith(".c"):
                    continue
                t = open(os.path.join(self.src, f), encoding="latin1").read()
                t = re.sub(r"/\*.*?\*/", "", t, flags=re.S)
                for m in re.finditer(r"(?m)^(?:[A-Za-z_][\w \*]*?\s)?\**\s*([A-Za-z_]\w*)\s*\([^;{]*$", t):
                    if m.group(1) in ("if", "while", "for", "switch", "return"):
                        continue
                    if m.group(1).upper()[:8] in places:
                        out.append((places[m.group(1).upper()[:8]], os.path.join(self.src, f)))
                        break
        if self.cfg.get("unplaced"):                        # (files with no place in their name: each is placed by
            for f in sorted(os.listdir(self.src)):          #  the first of its names SYMBOLS knows - address -1)
                if re.fullmatch(self.cfg["unplaced"], f, re.I):
                    out.append((-1, os.path.join(self.src, f)))
        p = self.path("ORDER")
        if os.path.exists(p):
            for l in open(p):
                c = l.split("#")[0].split()
                if len(c) >= 2 and re.fullmatch(r"[0-9a-fA-F]+", c[1]) and os.path.exists(self.path(c[0])):
                    out.append((int(c[1], 16), self.path(c[0])))
        return sorted(out)

    def whole(self, path):
        """does the file's code lie in one piece (a file src/ORDER lists; every file of a project that says
        "whole": true - a PL/M module of several procedures)?"""
        return self.cfg.get("whole", False) or not re.fullmatch(r"f[0-9a-f]{5}\.c", os.path.basename(path))

    def headers(self):
        """{name: bytes} the headers of the sources' folder (the compiler finds them beside the file; the toolchain
        profile's "headers": their extensions, default .h)"""
        ext = tuple(self.toolchain().p.get("headers", [".h"]))
        return {f: open(os.path.join(self.src, f), "rb").read() for f in sorted(os.listdir(self.src)) if f.lower().endswith(ext)}


def load(folder):
    return Project(folder)
