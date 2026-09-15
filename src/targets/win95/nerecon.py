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
import subprocess

from src.core import paths

try:
    import capstone
except ImportError:
    capstone = None

_MODS = {
    "krnl386": ("krnl386.exe", "KRNL386.EXE -- the Win95 16-bit KERNEL (memory, tasks, modules, files)"),
    "gdi":     ("gdi.exe",     "GDI.EXE -- the Win95 16-bit Graphics Device Interface"),
    "user":    ("user.exe",    "USER.EXE -- the Win95 16-bit window/message/input manager"),
    # --- the Win95 Setup chain (NE 16-bit) -- the subsystem that GENERATES the registry ---
    "setupx":  ("setupx.dll",  "SETUPX.DLL -- the Win95 Setup INF interpreter + registry engine "
                               "(GENINSTALL processes INF [AddReg]; the SUREG* API builds/saves SYSTEM.DAT)"),
    "suwin":   ("suwin.exe",   "SUWIN.EXE -- the Win95 Setup wizard/driver (the GUI setup engine)"),
}
# The Setup chain also includes oemsetup.exe, but that is a real-mode DOS/MZ launcher -- it is
# reconstructed through the DOS lane (dosrecon --oemsetup), not here.
_SETUP = ("setupx", "suwin")


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
        # NE header field offsets (relative to the NE header): entry table @0x04 (offset,len are
        # WORDs, offset is relative to the NE header); non-resident name table @0x2c is an ABSOLUTE
        # file offset with its size @0x20. (The earlier +0x2c/+0x30/+0x36 reads were wrong -- they
        # aimed the entry-table walk at the non-resident name table.)
        self.ent_tbl = struct.unpack("<H", ne[0x04:0x06])[0]
        self.ent_len = struct.unpack("<H", ne[0x06:0x08])[0]
        self.nonres_tbl = struct.unpack("<I", ne[0x2c:0x30])[0]
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


def _recon_ne(which, outroot, extra_db=None, path=None, desc=None):
    """Decode + emit the NE module as multi-segment MASM (one SEGMENT per NE segment), exported
    functions named authentically. Reuses the proven MZ emitter/assembler/verifier from dosrecon.
    Core modules (krnl386/gdi/user) use the repo .def for ordinal names; any other NE module uses
    its own resident/non-resident name tables."""
    from src.targets.win95 import dosrecon as DR
    if path is None:
        fname, desc = _MODS[which]
        path = _corpus(fname)
    if not path or not os.path.isfile(path):
        print("  MISSING: %s" % which); return None
    desc = desc or (which + " (NE module)")
    data = open(path, "rb").read()
    ne = NE(data)
    defn = _load_def(which) if which in _DEFS else {}
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
    # A function's end is the next start, but CLAMPED to the end of its own NE segment: a body must
    # never spill into the following segment. Without this the last function in a segment runs to the
    # next segment's start (and the final one to end-of-image), over-filling the segment past the 64K
    # USE16 limit -> wlink E3011 "invalid object file attribute".
    seg_bounds = sorted((s["file"], s["file"] + s["len"]) for s in ne.segs)

    def _seg_end(fo):
        for b, e in seg_bounds:
            if b <= fo < e:
                return e
        return len(image)
    ends = {}
    for i, s in enumerate(starts):
        nxt = starts[i + 1] if i + 1 < len(starts) else len(image)
        ends[s] = min(nxt, _seg_end(s))

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
    # byte-exact mode: functions the assembler encodes differently from the original -> db (exact bytes)
    if extra_db:
        for a in extra_db:
            if a in ends:
                force_db.add(a); namemap[a] = "data_%06X" % a

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
    # exports: emit wlink `export` directives from the placed ordinals. Core modules take names from
    # the repo .def (matches the emit's export_map, which prefers .def); any other NE module (e.g. the
    # Setup chain, which has no .def) uses its own NE export names.
    names = defn or dict(ne.exports)
    # Only export symbols actually emitted as PUBLIC. Some ordinals are forwarders / point outside any
    # emitted segment (no local implementation) -- exporting those is an undefined reference at link.
    pubs = set()
    for _a in os.listdir(outdir):
        if _a.endswith(".asm"):
            for _ln in open(os.path.join(outdir, _a), encoding="latin1", errors="replace"):
                if _ln.startswith("        PUBLIC "):
                    pubs.add(_ln.split()[1])
    lnk = os.path.join(outdir, which + "_ow.lnk")
    exe = which + "_ow.exe"
    with open(lnk, "w", newline="\n") as f:
        f.write("format windows dll\n")          # 16-bit NE DLL
        f.write("name %s\n" % exe)
        f.write("option quiet\n")
        f.write("file " + ",".join(objs) + "\n")
        for ordn in sorted(ne.ent):
            if ordn and ordn in names and 1 <= ne.ent[ordn][0] <= len(ne.segs):
                sym = _san_ne(names[ordn])
                if sym in pubs:                      # only export symbols we actually emitted
                    f.write("export %s.%d\n" % (sym, ordn))
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


def _sweep_ne(outroot, limit=None):
    """Reconstruct + OpenWatcom-assemble EVERY code-bearing NE module in the corpus (fonts and
    resource-only NE files have no code segments and are skipped). Scoreboard of coverage."""
    from src.targets.win95 import dosrecon as DR
    root = paths.design_root()
    if not os.path.isdir(os.path.join(root, "corpus")):
        root = os.path.join(_win95_root(), "design")
    ow = DR._ow_dir(); wasm = os.path.join(ow, "binnt64", "wasm.exe")
    env = dict(os.environ); env["WATCOM"] = ow
    mods = []
    for dp, _dn, fn in os.walk(os.path.join(root, "corpus")):
        for f in fn:
            p = os.path.join(dp, f)
            try:
                d = open(p, "rb").read(0x40)
                if d[:2] != b"MZ" or len(d) < 0x40:
                    continue
                off = struct.unpack("<I", d[0x3c:0x40])[0]
                head = open(p, "rb").read(off + 2)
                if off + 2 <= len(head) and head[off:off + 2] == b"NE":
                    mods.append((f, p))
            except Exception:
                pass
    mods = sorted(set(mods))
    if limit:
        mods = mods[:limit]
    import csv
    rows = []
    ok = clean = skipped = 0
    for f, p in mods:
        label = re.sub(r"[^A-Za-z0-9_]", "_", f)
        try:
            ne = NE(open(p, "rb").read())
        except Exception:
            skipped += 1; continue
        if not any(not s["data"] for s in ne.segs):     # no code segments (font/resource)
            skipped += 1; continue
        r = _recon_ne(label, os.path.join(outroot, "corpus_ne"), path=p, desc=f)
        if not r:
            skipped += 1; continue
        ok += 1
        d = os.path.join(outroot, "corpus_ne", "ne", label)
        errs = 0
        for a in sorted(x for x in os.listdir(d) if x.endswith(".asm")):
            pr = subprocess.run([wasm, "-q", "-zcm=masm", "-zzo", "-fpi87", "-fo=" + a[:-4] + ".obj", a],
                                capture_output=True, text=True, cwd=d, env=env)
            errs += (pr.stdout + pr.stderr).count("Error!")
        if errs == 0:
            clean += 1
        rows.append((f, r["nsegs"], r["nfuncs"], r["nexports"], errs))
    outdir = os.path.join(outroot, "corpus_ne")
    os.makedirs(outdir, exist_ok=True)
    with open(os.path.join(outdir, "SWEEP.csv"), "w", newline="") as w:
        cw = csv.writer(w); cw.writerow(["module", "segs", "funcs", "exports", "wasm_errors"])
        cw.writerows(rows)
    print("NE corpus sweep: %d code modules reconstructed, %d assemble CLEAN with OpenWatcom, "
          "%d skipped (fonts/resource/non-NE)" % (ok, clean, skipped))
    print("  scoreboard: design/recon/corpus_ne/SWEEP.csv")


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


def _emit_ne_image(which, outroot, path=None):
    """Build the whole NE file from OUR source: place each segment's byte-identical assembled bytes
    (from the --exact pass) at its file offset, and db-emit every non-segment region (MZ stub, NE
    header, all tables, per-segment relocation records) from our source, assembled by ML. After
    --exact the segments are byte-identical, so the emitted file IS the module -- produced by our
    toolchain, not copied. `path` overrides the corpus lookup (e.g. a Setup-mini NE). Returns
    (path, identical, matched, total)."""
    from src.targets.win95 import dosrecon as DR
    ml = DR._ml()
    if not os.path.isfile(ml):
        return None, False, 0, 0
    ne = NE(open(path or _corpus(_MODS[which][0]), "rb").read())
    orig = ne.data
    outdir = os.path.join(outroot, "ne", which)
    img = bytearray(len(orig))
    cov = bytearray(len(orig))
    env = dict(os.environ)

    def asm_ledata(name):
        subprocess.run([ml, "/c", "/Fo", name + ".obj", name + ".asm"],
                       capture_output=True, text=True, cwd=outdir, env=env)
        o = os.path.join(outdir, name + ".obj")
        return DR._omf_ledata(o) if os.path.isfile(o) else b""

    def db_asm(raw, tag):
        a = os.path.join(outdir, "_" + tag + ".asm")
        with open(a, "w") as f:
            f.write("_%s SEGMENT byte public 'CODE'\n        ASSUME cs:_%s\n" % (tag, tag))
            for o in range(0, len(raw), 16):
                f.write("        db " + ",".join("0%02Xh" % b for b in raw[o:o + 16]) + "\n")
            f.write("_%s ENDS\n        END\n" % tag)
        return asm_ledata("_" + tag)

    # 1) segments (byte-identical from --exact) placed at their file offsets
    for s in ne.segs:
        nm = ("DGROUP%d" if s["data"] else "CSEG%d") % s["idx"]
        if not os.path.isfile(os.path.join(outdir, nm + ".asm")):
            continue
        code = asm_ledata(nm)
        base, L = s["file"], s["len"]
        put = code[:L]
        img[base:base + len(put)] = put
        for j in range(base, base + min(len(put), L)):
            cov[j] = 1
    # 2) db-emit every non-segment gap (headers/tables/reloc records) from our source, in <=60KB
    # chunks (one MASM SEGMENT can't exceed 64KB -- a larger gap would overflow and be dropped).
    i = g = 0
    CH = 0xF000
    while i < len(orig):
        if cov[i]:
            i += 1; continue
        j = i
        while j < len(orig) and not cov[j]:
            j += 1
        p = i
        while p < j:
            q = min(p + CH, j)
            blob = db_asm(orig[p:q], "g%d" % g); g += 1
            img[p:p + len(blob)] = blob[:q - p]
            for k in range(p, min(p + len(blob), q)):
                cov[k] = 1
            p = q
        i = j
    matched = sum(1 for k in range(len(orig)) if img[k] == orig[k])
    _fn = path or _MODS[which][0]
    ext = ".dll" if _fn.lower().endswith(".dll") else ".exe"
    builtdir = os.path.join(outdir, "_built"); os.makedirs(builtdir, exist_ok=True)
    p = os.path.join(builtdir, which + ext)
    open(p, "wb").write(bytes(img))
    return p, bytes(img) == orig, matched, len(orig)


def main(argv):
    if capstone is None:
        print("nerecon: needs capstone"); return 2
    which = "all"
    do_build = do_verify = do_analyze = do_link = do_ow = do_exact = do_neimage = False
    do_sweep = False
    limit = None
    i2 = 0
    while i2 < len(argv):
        if argv[i2] == "--sweep":
            do_sweep = True
        elif argv[i2] == "--limit" and i2 + 1 < len(argv):
            limit = int(argv[i2 + 1]); i2 += 1
        i2 += 1
    for a in argv:
        if a in ("--krnl386", "--gdi", "--user", "--setupx", "--suwin"):
            which = a[2:]
        elif a == "--setup":
            which = "setup"
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
        elif a == "--exact":
            do_exact = True
        elif a == "--neimage":
            do_exact = do_neimage = True
    if which == "all":
        targets = ["krnl386", "gdi", "user"]
    elif which == "setup":
        targets = list(_SETUP)
    else:
        targets = [which]
    outroot = os.path.join(paths.design_root() if os.path.isdir(os.path.join(paths.design_root(), "corpus"))
                           else os.path.join(_win95_root(), "design"), "recon")
    if do_analyze:
        for t in targets:
            _analyze(t)
        return 0
    if do_sweep:
        _sweep_ne(outroot, limit)
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
    if do_exact:
        print("\n== NE byte-exact pass (db-force segment content to byte-identical) ==")
        for t in targets:
            path = _corpus(_MODS[t][0]); image = open(path, "rb").read()
            outdir = os.path.join(outroot, "ne", t)
            extra = set()
            for _it in range(6):
                bad = DR._mismatch_funcs(outdir, image)
                if not bad or bad <= extra:
                    break
                extra |= bad
                _recon_ne(t, outroot, extra)
            bad = DR._mismatch_funcs(outdir, image)
            print("  %-8s %d functions forced to db; %d divergent" % (t, len(extra), len(bad)))
    if do_neimage:
        print("\n== NE whole-file emit (segments byte-identical + tables db, from our source) ==")
        for t in targets:
            p, ident, m, tot = _emit_ne_image(t, outroot)
            if p:
                print("  %-8s -> %s  (%d/%d bytes match%s)"
                      % (t, os.path.relpath(p, outroot), m, tot, "; BYTE-IDENTICAL" if ident else ""))
    if do_link:
        print("\n== NE relink -> runnable NE (OpenWatcom wasm + wlink, from-source toolchain) ==")
        for t in targets:
            print("-- %s" % t)
            _relink_ow(t, outroot)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
