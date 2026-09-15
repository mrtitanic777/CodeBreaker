#!/usr/bin/env python3
r"""nerecon.py -- reconstruction organizer for the Win95 16-bit NE GUI core (KRNL386/GDI/USER).

The NE (New Executable) GUI subsystem is the layer between the real-mode DOS base and the desktop.
Unlike the flat MZ/.SYS binaries, an NE module is already segmented (an explicit segment table) and
it EXPORTS its functions by name (the resident/non-resident name tables + the entry table map an
ordinal to a segment:offset). That gives us the AUTHENTIC authored names straight from the binary --
GlobalAlloc, LocalAlloc, BitBlt, TextOut, CreateWindow, SendMessage, ... -- not inferred labels.

This mirrors dosrecon for NE: parse the NE structure, name every exported function authentically,
decode + name the rest, emit one MASM SEGMENT per NE segment (multi-segment, as built), and drive
toward assemble + link (LINK 5.60 emits NE) + byte-verify.

    wt --target win95 nerecon [--krnl386|--gdi|--user|--all] [--build]

Read-only against the corpus; writes under design/recon/ne/.
"""
import os
import re
import sys
import struct

from src.core import paths

try:
    import capstone
except ImportError:
    capstone = None

_MODS = {
    "krnl386": ("krnl386.exe", "KRNL386.EXE -- the Win95 16-bit KERNEL (memory, tasks, modules, files)"),
    "gdi":     ("gdi.exe",     "GDI.EXE -- the Win95 16-bit Graphics Device Interface"),
    "user":    ("user.exe",    "USER.EXE -- the Win95 16-bit window/message/input manager"),
}


def _win95_root():
    root = paths.design_root()
    return os.path.dirname(root) if os.path.basename(root) == "design" else \
        os.path.join(os.path.dirname(paths.repo_root()), "Windows95")


def _corpus(name):
    root = paths.design_root()
    if not os.path.isdir(os.path.join(root, "corpus")):
        root = os.path.join(_win95_root(), "design")
    for dp, _dn, fn in os.walk(os.path.join(root, "corpus")):
        for f in fn:
            if f.lower() == name.lower():
                return os.path.join(dp, f)
    return None


# authentic ordinal->name export tables live as .def files in the Win95 tree
_DEFS = {
    "krnl386": ("core", "kernel", "KRNL386", "kernel.def"),
    "gdi":     ("core", "gdi", "gdi.def"),
    "user":    ("core", "user", "user.def"),
}


def _load_def(which):
    """Parse a module .def EXPORTS section -> {ordinal: Name}. Authentic Win16 API names."""
    parts = _DEFS.get(which)
    if not parts:
        return {}
    p = os.path.join(_win95_root(), *parts)
    if not os.path.isfile(p):
        return {}
    out = {}
    for line in open(p, encoding="latin1", errors="replace"):
        m = re.search(r"^\s*([A-Za-z_][A-Za-z0-9_]*)\s+@(\d+)", line)
        if m:
            out[int(m.group(2))] = m.group(1)
    return out


class NE:
    """Parsed NE structure: segments, exports (ordinal->name and ordinal->(seg,off)), imports."""

    def __init__(self, data):
        self.data = data
        self.nepos = struct.unpack("<I", data[0x3c:0x40])[0]
        ne = data[self.nepos:]
        if ne[:2] != b"NE":
            raise ValueError("not an NE module")
        self.seg_cnt = struct.unpack("<H", ne[0x1c:0x1e])[0]
        self.modref_cnt = struct.unpack("<H", ne[0x1e:0x20])[0]
        self.seg_tbl = struct.unpack("<H", ne[0x22:0x24])[0]
        self.resnam_tbl = struct.unpack("<H", ne[0x26:0x28])[0]
        self.modref_tbl = struct.unpack("<H", ne[0x28:0x2a])[0]
        self.impnam_tbl = struct.unpack("<H", ne[0x2a:0x2c])[0]
        self.ent_tbl = struct.unpack("<I", ne[0x2c:0x30])[0]
        self.ent_len = struct.unpack("<H", ne[0x30:0x32])[0]
        self.nonres_tbl = struct.unpack("<I", ne[0x36:0x3a])[0]
        self.nonres_sz = struct.unpack("<H", ne[0x20:0x22])[0]
        self.align = struct.unpack("<H", ne[0x32:0x34])[0] or 9
        self._segs()
        self._entries()
        self._names()
        self._modrefs()
        self._relocs()

    def _segs(self):
        self.segs = []          # (idx, file_off, length, flags, is_data, moveable, has_reloc)
        st = self.nepos + self.seg_tbl
        for i in range(self.seg_cnt):
            off, ln, flg, _mem = struct.unpack("<HHHH", self.data[st + i * 8:st + i * 8 + 8])
            self.segs.append({
                "idx": i + 1, "file": off << self.align, "len": ln, "flags": flg,
                "data": bool(flg & 1), "moveable": bool(flg & 0x10), "reloc": bool(flg & 0x100),
            })

    def _entries(self):
        """Entry table -> {ordinal: (segidx, offset)}. Bundles of fixed/moveable entries."""
        self.ent = {}
        p = self.nepos + self.ent_tbl
        end = p + self.ent_len
        ordn = 1
        d = self.data
        while p < end:
            cnt = d[p]; typ = d[p + 1]; p += 2
            if cnt == 0:
                break
            for _ in range(cnt):
                if typ == 0:                      # unused ordinals
                    ordn += 1; continue
                if typ == 0xFF:                   # moveable: flags, 3Fcd, seg, off (6 bytes)
                    seg = d[p + 3]; off = struct.unpack("<H", d[p + 4:p + 6])[0]; p += 6
                else:                             # fixed: flags, off (3 bytes); seg = typ
                    seg = typ; off = struct.unpack("<H", d[p + 1:p + 3])[0]; p += 3
                self.ent[ordn] = (seg, off); ordn += 1

    def _names(self):
        """resident + non-resident name tables -> {ordinal: name} (the exports' authentic names)."""
        self.exports = {}
        for base, sz in ((self.nepos + self.resnam_tbl, None), (self.nonres_tbl, self.nonres_sz)):
            p = base
            d = self.data
            first = True
            while p < len(d):
                ln = d[p]
                if ln == 0:
                    break
                nm = d[p + 1:p + 1 + ln].decode("latin1", "replace")
                ordv = struct.unpack("<H", d[p + 1 + ln:p + 1 + ln + 2])[0]
                p += 1 + ln + 2
                if not first:                     # first entry of each table is the module name (ord 0)
                    self.exports[ordv] = nm
                first = False

    def _modrefs(self):
        """imported module names (by index) for import relocations."""
        self.modrefs = []
        d = self.data
        for i in range(self.modref_cnt):
            noff = struct.unpack("<H", d[self.nepos + self.modref_tbl + i * 2:
                                         self.nepos + self.modref_tbl + i * 2 + 2])[0]
            p = self.nepos + self.impnam_tbl + noff
            ln = d[p]
            self.modrefs.append(d[p + 1:p + 1 + ln].decode("latin1", "replace"))

    def _relocs(self):
        """Per-segment relocation records, chains walked to individual fixup sites. Each site ->
        (kind, target): kind 'segref' internal segment base (target=seg#), 'ptr' internal far
        (target=(seg#,off)), 'impord' (target=(modidx,ord)), 'impname' (target=(modidx,nameoff))."""
        self.relocs = {}          # segidx -> {site_offset: (atype, kind, target)}
        d = self.data
        for s in self.segs:
            if not s["reloc"]:
                continue
            fbase = s["file"]
            rpos = fbase + s["len"]
            nrel = struct.unpack("<H", d[rpos:rpos + 2])[0]
            sites = {}
            for r in range(nrel):
                rec = d[rpos + 2 + r * 8:rpos + 2 + r * 8 + 8]
                atype = rec[0]; rflag = rec[1]; head = struct.unpack("<H", rec[2:4])[0]
                rtype = rflag & 3
                additive = bool(rflag & 4)
                if rtype == 0:                    # internal ref
                    tseg = rec[4]; toff = struct.unpack("<H", rec[6:8])[0]
                    kind = "segref" if atype == 2 else "ptr"
                    target = tseg if kind == "segref" else (tseg, toff)
                elif rtype == 1:                  # import by ordinal
                    kind = "impord"; target = (struct.unpack("<H", rec[4:6])[0],
                                               struct.unpack("<H", rec[6:8])[0])
                elif rtype == 2:                  # import by name
                    kind = "impname"; target = (struct.unpack("<H", rec[4:6])[0],
                                                struct.unpack("<H", rec[6:8])[0])
                else:                             # OSFIXUP
                    kind = "osfixup"; target = None
                # walk the chain: the word at each site is the next site offset (0xFFFF ends it)
                off = head
                seen = 0
                while off != 0xFFFF and seen < 100000:
                    sites[off] = (atype, kind, target)
                    if additive or off + 2 > s["len"]:
                        break
                    nxt = struct.unpack("<H", d[fbase + off:fbase + off + 2])[0]
                    if nxt == off:
                        break
                    off = nxt; seen += 1
            self.relocs[s["idx"]] = sites

    def export_map(self, defnames=None):
        """{(segidx, offset): name} for exported functions. Prefer the authentic .def ordinal name;
        fall back to the module's own resident/non-resident name table."""
        defnames = defnames or {}
        out = {}
        for ordn, (seg, off) in self.ent.items():
            nm = defnames.get(ordn) or self.exports.get(ordn)
            if nm:
                out[(seg, off)] = nm
        return out


def _discover(image, base, length, md):
    """Discover function starts within one NE segment [base, base+length): 55 8B EC prologues and
    near-call (E8) targets that land inside the segment. Returns a set of file offsets."""
    starts = set()
    end = base + length
    a = base
    while a < end - 2:
        if image[a] == 0x55 and image[a + 1] == 0x8B and image[a + 2] == 0xEC:
            starts.add(a)
        a += 1
    a = base
    while a < end:
        g = list(md.disasm(bytes(image[a:min(a + 8, end)]), a))
        if not g:
            a += 1; continue
        ins = g[0]
        if ins.mnemonic == "call" and ins.op_str.startswith("0x"):
            try:
                t = int(ins.op_str, 16)
                if base <= t < end:
                    starts.add(t)
            except ValueError:
                pass
        a += ins.size
    return starts


def _recon_ne(which, outroot):
    """Decode + emit the NE module as multi-segment MASM (one SEGMENT per NE segment), exported
    functions named authentically. Reuses the proven MZ emitter/assembler/verifier from dosrecon."""
    from src.targets.win95 import dosrecon as DR
    fname, desc = _MODS[which]
    path = _corpus(fname)
    if not path:
        print("  MISSING: %s" % fname); return None
    data = open(path, "rb").read()
    ne = NE(data)
    defn = _load_def(which)
    emap = ne.export_map(defn)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    image = data
    strings = DR._string_regions(image)

    segs = []                     # (seg_file_offset, seg_name) for the emitter
    starts = set()
    force_db = set()
    for s in ne.segs:
        base = s["file"]
        nm = ("DGROUP%d" % s["idx"]) if s["data"] else ("CSEG%d" % s["idx"])
        segs.append((base, nm))
        starts.add(base)
        if s["data"]:
            force_db.add(base)                     # data segment -> emit as db
        else:
            starts |= _discover(image, base, s["len"], md)
    # exported entry points are function starts, named authentically
    namemap = {}
    for (seg, off), name in emap.items():
        if 1 <= seg <= len(ne.segs):
            fo = ne.segs[seg - 1]["file"] + off
            starts.add(fo)
            namemap[fo] = _san_ne(name)
    segs.sort()
    starts = sorted(s for s in starts if s < len(image))
    ends = {s: (starts[i + 1] if i + 1 < len(starts) else len(image)) for i, s in enumerate(starts)}

    # behavioral naming for the non-exported internals (reuse dosrecon's engine)
    finfo = {}
    fkind = {}
    for s in starts:
        e = min(ends[s], s + 0x4000)
        kind, ni, nb, ints, strs, services, callees = DR._analyze_fn(image, s, e, strings, md)
        fkind[s] = kind
        finfo[s] = {"services": services, "callees": callees, "strs": strs, "ints": ints, "kind": kind}
        if s not in namemap:
            namemap[s] = DR._beautify_name(s, kind, ints, strs, "")
    for s in force_db:
        namemap[s] = "seg_data_%06X" % s
        fkind[s] = "data"
    DR._name_by_behavior(namemap, finfo, set(starts))
    # re-assert authentic export names (behavioral naming may not override them, but be explicit)
    for (seg, off), name in emap.items():
        if 1 <= seg <= len(ne.segs):
            namemap[ne.segs[seg - 1]["file"] + off] = _san_ne(name)

    # relocation symbols: at each fixup site, the symbolic reference LINK will regenerate. Only
    # used for the (in-progress) runnable-NE relink path; the default readable build omits them so
    # cross-segment SEG refs (which need all segments in one assembly unit) don't break assembly.
    def _segname(idx):
        s2 = ne.segs[idx - 1]
        return ("DGROUP%d" if s2["data"] else "CSEG%d") % idx
    reloc_syms = {}
    if os.environ.get("NE_RELINK"):
        for s in ne.segs:
            for site, (at, kind, tgt) in ne.relocs.get(s["idx"], {}).items():
                fo = s["file"] + site
                if kind == "segref" and 1 <= tgt <= len(ne.segs):
                    reloc_syms[fo] = "SEG %s" % _segname(tgt)

    outdir = os.path.join(outroot, "ne", which)
    os.makedirs(outdir, exist_ok=True)
    regions = DR._emit_asm(which, os.path.join(outroot, "ne"), image, False, 0, starts, ends,
                           strings, md, namemap, fkind, desc, segs, force_db, reloc_syms)
    print("  emitted %d NE segment(s): %s" % (len(regions), ", ".join(regions)))
    return {"which": which, "path": path, "outdir": outdir, "nsegs": len(segs),
            "nfuncs": len(starts), "nexports": len(emap)}


def _san_ne(name):
    return re.sub(r"[^A-Za-z0-9_]", "_", name)


def _merge_module(which, ne, outdir):
    """Merge the per-segment .asm files into ONE module .asm (all SEGMENT blocks together) so that
    inter-segment `SEG segname` references resolve within a single assembly unit. Returns its path."""
    order = []
    for s in ne.segs:
        nm = ("DGROUP%d" if s["data"] else "CSEG%d") % s["idx"]
        p = os.path.join(outdir, nm + ".asm")
        if os.path.isfile(p):
            order.append((nm, p))
    defined = set()                       # labels defined across all segments (to drop stale EXTRN)
    for nm, p in order:
        for line in open(p, encoding="latin1"):
            m = re.match(r"([A-Za-z_][A-Za-z0-9_]*) label near", line)
            if m:
                defined.add(m.group(1))
    out = os.path.join(outdir, which + ".asm")
    with open(out, "w") as w:
        w.write("; %s.asm -- full module (all NE segments together for inter-segment SEG refs)\n"
                "        .386p\n\n" % which)
        for nm, p in order:
            inseg = False
            for line in open(p, encoding="latin1"):
                if re.match(r"\S+\s+SEGMENT ", line):
                    inseg = True
                if inseg:
                    if line.strip().startswith("END") and not line.strip().startswith("ENDS"):
                        continue
                    w.write(line)
                    if re.match(r"\S+\s+ENDS", line):
                        inseg = False
        w.write("\n        END\n")
    return out


def _write_def(which, ne, defn, outdir):
    """Emit a module .def (LIBRARY + EXPORTS by ordinal) for LINK 5.60. Export only the ordinals we
    actually placed as PUBLIC symbols (entry-table ordinal mapped to a named function), so every
    export resolves."""
    p = os.path.join(outdir, which + ".def")
    modname = {"krnl386": "KERNEL", "gdi": "GDI", "user": "USER"}[which]
    with open(p, "w") as w:
        w.write("LIBRARY %s\n" % modname)
        w.write("EXETYPE WINDOWS\n")
        w.write("EXPORTS\n")
        for ordn in sorted(ne.ent):
            if ordn == 0 or ordn not in defn:
                continue
            seg, off = ne.ent[ordn]
            if 1 <= seg <= len(ne.segs):
                w.write("    %s @%d\n" % (_san_ne(defn[ordn]), ordn))
    return p


def _relink(which, outroot):
    """Merge segments + .def -> LINK 5.60 -> NE module (the runnable-image path). Requires the
    per-segment .asm already emitted with NE_RELINK (symbolic SEG refs)."""
    ne = NE(open(_corpus(_MODS[which][0]), "rb").read())
    defn = _load_def(which)
    outdir = os.path.join(outroot, "ne", which)
    ml = os.path.join(paths.repo_root(), "toolchains", "period", "assemblers", "masm-6.13", "ML.EXE")
    link = os.path.join(paths.repo_root(), "toolchains", "period", "linkers", "link-5.60", "LINK.EXE")
    _write_def(which, ne, defn, outdir)
    import subprocess
    # assemble each per-segment .asm to its own obj (the merged file would exceed MASM's line limit)
    objs = []
    total_err = 0
    for s in ne.segs:
        nm = ("DGROUP%d" if s["data"] else "CSEG%d") % s["idx"]
        asm = os.path.join(outdir, nm + ".asm")
        if not os.path.isfile(asm):
            continue
        obj = os.path.join(outdir, nm + ".obj")
        r = subprocess.run([ml, "/c", "/Fo", obj, asm], capture_output=True, text=True, cwd=outdir)
        e = (r.stdout + r.stderr).count("error A")
        total_err += e
        if e:
            open(os.path.join(outdir, nm + ".asmlog"), "w").write(r.stdout + r.stderr)
        if os.path.isfile(obj):
            objs.append(nm + ".obj")
    print("  assembled %d segment objs, %d errors" % (len(objs), total_err))
    if total_err or not objs:
        return
    exe = os.path.join(outdir, which + "_rebuilt.exe")
    # response file (many objs overflow LINK's command-line limit)
    rsp = os.path.join(outdir, which + ".lnk")
    with open(rsp, "w") as w:
        w.write(" +\n".join(objs) + "\n")
        w.write(os.path.basename(exe) + "\n")   # output
        w.write("NUL\n")                         # map
        w.write("\n")                            # libraries
        w.write(which + ".def\n")                # module definition
    lk = subprocess.run([link, "/NOLOGO", "@" + which + ".lnk"],
                        capture_output=True, text=True, cwd=outdir)
    open(os.path.join(outdir, which + ".linklog"), "w").write(lk.stdout + lk.stderr)
    ok = os.path.isfile(exe)
    print("  LINK 5.60: %s%s" % ("produced " + os.path.basename(exe) if ok else "FAILED",
                                 " (%d bytes)" % os.path.getsize(exe) if ok else ""))
    if ok:
        sig = open(exe, "rb").read(2)
        print("  output signature: %r" % sig)


def _relink_ow(which, outroot):
    """Runnable-NE via the from-source OpenWatcom toolchain: wasm assembles each segment, wlink
    (system windows_dll) links them into an NE, with exports from the module .def. No MS tools."""
    import subprocess
    from src.targets.win95 import dosrecon as DR
    ne = NE(open(_corpus(_MODS[which][0]), "rb").read())
    defn = _load_def(which)
    outdir = os.path.join(outroot, "ne", which)
    ow = DR._ow_dir()
    wasm = os.path.join(ow, "binnt64", "wasm.exe")
    wlink = os.path.join(ow, "binnt64", "wlink.exe")
    env = dict(os.environ); env["WATCOM"] = ow
    objs = []
    total_err = 0
    for s in ne.segs:
        nm = ("DGROUP%d" if s["data"] else "CSEG%d") % s["idx"]
        asm = os.path.join(outdir, nm + ".asm")
        if not os.path.isfile(asm):
            continue
        obj = nm + ".obj"
        p = subprocess.run([wasm, "-q", "-zcm=masm", "-zzo", "-fpi87", "-fo=" + obj, nm + ".asm"],
                           capture_output=True, text=True, cwd=outdir, env=env)
        e = (p.stdout + p.stderr).count("Error!")
        total_err += e
        if e:
            open(os.path.join(outdir, nm + ".owlog"), "w").write(p.stdout + p.stderr)
        if os.path.isfile(os.path.join(outdir, obj)):
            objs.append(obj)
    print("  wasm: %d segment objs, %d errors" % (len(objs), total_err))
    if total_err or not objs:
        return
    # exports: emit wlink `export` directives from the placed ordinals
    modname = {"krnl386": "KERNEL", "gdi": "GDI", "user": "USER"}[which]
    lnk = os.path.join(outdir, which + "_ow.lnk")
    exe = which + "_ow.exe"
    with open(lnk, "w", newline="\n") as f:
        f.write("format windows dll\n")          # 16-bit NE DLL
        f.write("name %s\n" % exe)
        f.write("option quiet\n")
        f.write("file " + ",".join(objs) + "\n")
        for ordn in sorted(ne.ent):
            if ordn and ordn in defn and 1 <= ne.ent[ordn][0] <= len(ne.segs):
                f.write("export %s.%d\n" % (_san_ne(defn[ordn]), ordn))
    lk = subprocess.run([wlink, "@" + which + "_ow.lnk"], capture_output=True, text=True,
                        cwd=outdir, env=env)
    open(os.path.join(outdir, which + "_ow.linklog"), "w").write(lk.stdout + lk.stderr)
    p = os.path.join(outdir, exe)
    if os.path.isfile(p):
        import struct
        d = open(p, "rb").read(); nepos = struct.unpack("<I", d[0x3c:0x40])[0]
        sig = d[nepos:nepos + 2] if nepos < len(d) - 1 else b"??"
        print("  wlink: produced %s (%d bytes, sig=%r)" % (exe, len(d), sig))
    else:
        errs = lk.stdout.count("Error!") + lk.stderr.count("Error!")
        print("  wlink: FAILED (%d errors) -- see %s_ow.linklog" % (errs, which))


def _analyze(which):
    fname, desc = _MODS[which]
    path = _corpus(fname)
    if not path:
        print("  MISSING: %s" % fname); return None
    ne = NE(open(path, "rb").read())
    defn = _load_def(which)
    emap = ne.export_map(defn)
    codesegs = [s for s in ne.segs if not s["data"]]
    print("== %s ==" % which)
    print("  %s" % desc)
    print("  segments: %d (%d code, %d data); modrefs: %s"
          % (ne.seg_cnt, len(codesegs), ne.seg_cnt - len(codesegs), ne.modrefs or "(none)"))
    print("  entry-table ordinals: %d ; .def names: %d ; functions named authentically: %d"
          % (len(ne.ent), len(defn), len(emap)))
    print("  sample: %s" % ", ".join(sorted(set(n for n in emap.values()))[:14]))
    return ne


def main(argv):
    if capstone is None:
        print("nerecon: needs capstone"); return 2
    which = "all"
    do_build = do_verify = do_analyze = do_link = do_ow = False
    for a in argv:
        if a in ("--krnl386", "--gdi", "--user"):
            which = a[2:]
        elif a == "--all":
            which = "all"
        elif a == "--build":
            do_build = True
        elif a == "--verify":
            do_build = do_verify = True
        elif a == "--analyze":
            do_analyze = True
        elif a == "--link":
            do_link = True
        elif a == "--ow":
            do_ow = True
    targets = ["krnl386", "gdi", "user"] if which == "all" else [which]
    outroot = os.path.join(paths.design_root() if os.path.isdir(os.path.join(paths.design_root(), "corpus"))
                           else os.path.join(_win95_root(), "design"), "recon")
    if do_analyze:
        for t in targets:
            _analyze(t)
        return 0
    if do_link:
        os.environ["NE_RELINK"] = "1"
    from src.targets.win95 import dosrecon as DR
    results = []
    for t in targets:
        print("== nerecon %s ==" % t)
        r = _recon_ne(t, outroot)
        if r:
            print("  %d segments, %d functions, %d authentic export names"
                  % (r["nsegs"], r["nfuncs"], r["nexports"]))
            r["which_dir"] = os.path.join("ne", t)
            results.append(r)
    if do_build or do_verify or do_ow:
        rr = [{"which": os.path.join("ne", r["which"]), "path": r["path"]} for r in results]
    if do_ow:
        DR._assemble_ow(outroot, rr)
    if do_build or do_verify:
        DR._assemble(outroot, rr)
    if do_verify:
        DR._verify(outroot, rr)
    if do_link:
        print("\n== NE relink -> runnable NE (OpenWatcom wasm + wlink, from-source toolchain) ==")
        for t in targets:
            print("-- %s" % t)
            _relink_ow(t, outroot)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
