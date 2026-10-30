"""le_writer.py -- a virtual device driver (LE, and the W3 bundle VMM32.VXD) as assembly source that builds back byte for byte.

    python -m codebreaker le-source <driver> [--out DIR]

    from codebreaker.source import le_writer
    r = le_writer.reconstruct(path, outdir)
    r["source"], r["image"], r["same"], r["instructions"], r["raw"], r["rounds"]

driver_writer.py writes a driver as the driver kit's macro source, built by wasm and wlink: that is the
driver as it was written, and it comes back the same in structure. This writes the FILE, the way
pe_writer and ne_writer do: one NASM source assembled flat (-f bin). The headers and tables are a
section of their own; each object is a section at its first page's place in the file (start=) whose
addresses are the object's offsets (vstart=0), so a label is what a fix-up names. Every fix-up record's
place is written as its label less the start of its page, every internal target as the label it names,
every 32-bit offset the file stores at a fix-up's place (the target, as LINK386 and the driver linker
leave it) as that label, and the fix-up page table as the distances of each page's records from the
first. An object whose pages are not in the file in order, or not all there, is written as its pages'
bytes in the order the file has them.

Code is what is reached from the entry point, the device descriptor's control procedure, the entry
table and every internal fix-up that names a place in an executable object; an instruction is a
mnemonic where NASM gives back its bytes (checked each round from the assembler's map), its bytes
otherwise, and always its bytes when a fix-up patches a place inside it (that place labelled).
A W3 bundle (VMM32.VXD) is its header and each of its drivers, each written as above, its labels named
by the driver.
"""
import os
import re
import struct


from codebreaker.base import settings
from codebreaker.formats import le
from codebreaker.source import assembly, image_writer, pe_writer
from codebreaker.translate import l2
from codebreaker.translate.backends import asm
from codebreaker import platforms

_BRANCH = re.compile(r"^(j\w+|call|loop\w*|jcxz|jecxz)$")
_STOP = {"ret", "retf", "jmp", "ljmp", "hlt", "iret", "iretd", "ud2"}
_SEGS = {0x26, 0x2E, 0x36, 0x3E, 0x64, 0x65}
_WIDTH = {0: 1, 2: 2, 3: 4, 5: 2, 6: 6, 7: 4, 8: 4}
_REG = {n: (k, w) for w, names in ((4, ("eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi")),
                                   (2, ("ax", "cx", "dx", "bx", "sp", "bp", "si", "di")),
                                   (0, ("al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"))) for k, n in enumerate(names)}
_PRELUDE = ["", "; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m), the one",
            "; Microsoft's tools write; NASM itself writes the other"]
_PRELUDE += ["%%assign _r_%s %d" % (n, k) for n, (k, _w) in sorted(_REG.items())]
_PRELUDE += ["%%assign _w_%s %d" % (n, w) for n, (_k, w) in sorted(_REG.items())]
_PRELUDE += ["%macro _rr 3", "  %if (_w_%2 == 2 && __?BITS?__ == 32) || (_w_%2 == 4 && __?BITS?__ == 16)", "    db 0x66",
             "  %endif", "  db (%1) | 2 | (_w_%2 > 0), 0xC0 | (_r_%2 << 3) | _r_%3", "%endmacro"]
_PRELUDE += ["%%define %s_ _rr 0x%02x," % (m, v) for m, v in sorted(pe_writer._RR.items())]


class _Driver(object):
    """One LE image inside the file (the whole file, or one driver of a W3 bundle)."""

    def __init__(self, data, lfa, tag):
        self.d, self.tag = data, tag
        self.lf = lf = le.LEFile(data, lfa)
        self.pm = le.page_map(lf)
        self.last = max([n for n, _k in self.pm] or [0])
        self.objs = []
        for o in lf.objects:
            pages = self.pm[o["pmidx"] - 1:o["pmidx"] - 1 + o["pcnt"]]
            nums = [n for n, k in pages]
            whole = all(k == 0 and n for n, k in pages) and nums == list(range(nums[0], nums[0] + len(nums))) if nums else False
            start = lf.datapages + (nums[0] - 1) * lf.pagesize if whole else None
            size = sum(self._page_len(n) for n in nums) if whole else 0
            self.objs.append(dict(o, whole=whole, start=start, size=size, bits=32 if o["flags"] & 0x2000 else 16))
        self.by_idx = {o["idx"]: o for o in self.objs}
        # fix-ups: the place each patches, (object, offset) -> (record index, width, record)
        self.records = le.fixup_records(lf)
        self.sites = {}
        for k, (pg, rec) in enumerate(self.records):
            obj, po = lf.page_to_obj(pg)
            if obj is None:
                continue
            for s in rec["sources"]:
                self.sites.setdefault((obj["idx"], po + s), (k, _WIDTH.get(rec["source"], 4), rec))
        self._decode()

    def _page_len(self, n):
        return self.lf.last_page_bytes if n == self.last and self.lf.last_page_bytes else self.lf.pagesize

    def name(self, oi, a):
        return ("%so%d_L%x" if (oi, a) in self.insns else "%so%d_D%x") % (self.tag, oi, a)

    def _decode(self):
        self.insns, self.covered = {}, {}
        seeds = []
        lf = self.lf
        if lf.eip_obj in self.by_idx:
            seeds.append((lf.eip_obj, lf.eip))
        try:
            for _ordinal, obj, off, _flags in le.entries(lf):
                if obj in self.by_idx:
                    seeds.append((obj, off))
        except Exception:
            pass
        try:
            ddb = le.descriptor(lf)
            if ddb and ddb.get("control"):
                seeds.append(tuple(ddb["control"]))
        except Exception:
            pass
        for (oi, a), (_k, _w, rec) in self.sites.items():
            if rec["kind"] == "internal" and rec.get("target") is not None and rec["obj"] in self.by_idx:
                seeds.append((rec["obj"], rec["target"]))
        sitebytes = {}
        for (oi, at), (_k, w, _r) in self.sites.items():
            for j in range(w):
                sitebytes[(oi, at + j)] = at
        mds = {}
        work = [s for s in seeds if s[0] in self.by_idx and self.by_idx[s[0]]["exec"] and self.by_idx[s[0]]["whole"]]
        while work:
            oi, a = work.pop()
            o = self.by_idx[oi]
            md = mds.get(o["bits"])
            if md is None:
                md = mds[o["bits"]] = platforms.decoder(32 if o["bits"] == 32 else 16, detail=True)
            limit = min(o["vsize"], o["size"])
            while 0 <= a < limit and (oi, a) not in self.insns and (oi, a) not in self.covered:
                off = o["start"] + a
                one = next(md.disasm(self.d[off:o["start"] + min(a + 15, limit)], a, 1), None)
                if one is None or any((oi, a + k) in self.covered for k in range(one.size)):
                    break
                starts = {sitebytes[(oi, a + k)] for k in range(one.size) if (oi, a + k) in sitebytes}
                if any(st < a or st + self.sites[(oi, st)][1] > a + one.size for st in starts):
                    break
                self.insns[(oi, a)] = one
                for k in range(one.size):
                    self.covered[(oi, a + k)] = a
                t = _target(one)
                if t is not None and not starts:
                    work.append((oi, t))
                if one.mnemonic in _STOP:
                    break
                a += one.size


def _target(i):
    if _BRANCH.match(i.mnemonic) and i.imm_size and re.match(r"^0x[0-9a-f]+$", i.op_str):
        return int(i.op_str, 16)
    return None


def _nasm(i, bits, ref):
    """The instruction in NASM's spelling, or None where only its bytes will do."""
    m, ops = i.mnemonic, i.op_str
    if m in ("ljmp", "lcall"):
        return None
    if assembly.STRING_OPS.match(m) or re.match(r"^(rep |repe |repne )?(movs|cmps|stos|lods|scas|ins|outs)[bwdq]$", m):
        return None if any(p in _SEGS for p in i.prefix) else m
    tgt = _target(i)
    if tgt is not None:
        near = 4 if bits == 32 else 2
        if m == "call":
            return "call %s" % ref(tgt) if i.imm_size == near and i.size == 1 + near else None
        if m in ("loop", "loope", "loopne", "jcxz", "jecxz"):
            return "%s %s" % (m, ref(tgt)) if i.size == 2 else None
        if i.imm_size not in (1, near) or (i.imm_size == 1 and i.size != 2):
            return None
        return "%s %s %s" % (m, "short" if i.imm_size == 1 else "near", ref(tgt))
    rr = re.match(r"^(\w+), (\w+)$", ops)
    if m in pe_writer._RR and rr and rr.group(1) in _REG and rr.group(2) in _REG:
        op = bytes(i.bytes)[1 if i.bytes[0] == 0x66 else 0]
        return ("%s_ %s" % (m, ops)) if op & 2 else "%s %s" % (m, ops)
    ops = ops.replace(" ptr ", " ").replace("xmmword ", "oword ").replace("xword ", "tword ")
    ops = re.sub(r"\bst\((\d)\)", r"st\1", ops)
    ops = re.sub(r"\b([cdefgs]s):\[", r"[\1:", ops)
    if bits == 32:
        ops = re.sub(r"\[(\w\w:)?(e\w\w\*[248])", r"[\1nosplit \2", ops)
    if i.imm_size in (2, 4) and m in pe_writer._ARITH:
        mm = re.search(r"(-?0x[0-9a-f]+|-?\d+)$", ops)
        if mm:
            v = int(mm.group(1), 0)
            if -128 <= v <= 127 or v >= (0xFF80 if i.imm_size == 2 else 0xFFFFFF80):
                ops = ops[:mm.start()] + ("strict word " if i.imm_size == 2 else "strict dword ") + mm.group(1)
    if i.disp_size in (2, 4):
        br = re.search(r"\[([^\]]*)\]", ops)
        if br and re.search(r"[a-z]{2,3}\s*[+-]", br.group(1)):
            ops = ops[:br.start()] + "[%s " % ("dword" if i.disp_size == 4 else "word") + br.group(1) + "]" + ops[br.end():]
    return ("%s %s" % (m, ops)).strip()


class Writer(object):
    def __init__(self, path):
        self.path = path
        with open(path, "rb") as f:
            self.d = d = f.read()
        lfa = struct.unpack_from("<I", d, 0x3C)[0]
        self.w3 = d[lfa:lfa + 2] == b"W3"
        if self.w3:
            w3 = le.W3File(d, lfa)
            self.drivers = [_Driver(d, e["off"], "m%d_" % k) for k, e in enumerate(w3.entries)
                            if d[e["off"]:e["off"] + 2] in (b"LE", b"LX")]
        else:
            self.drivers = [_Driver(d, lfa, "")]

    @property
    def insn_count(self):
        return sum(len(dr.insns) for dr in self.drivers)

    def _place(self, dr, raw):
        want = set()
        for (oi, at), (_k, w, rec) in dr.sites.items():
            want.add((oi, at))
            if rec["kind"] == "internal" and rec.get("target") is not None:
                want.add((rec["obj"], rec["target"]))
        for (oi, a), i in dr.insns.items():
            t = _target(i)
            if t is not None:
                want.add((oi, t))
        if self._mark_all:
            want |= set(dr.insns)
        dr.placed = {}
        for (oi, a) in want:
            o = dr.by_idx.get(oi)
            if o is None or not o["whole"] or a > o["size"]:
                continue
            if (oi, a) in dr.insns or (oi, a) not in dr.covered:
                dr.placed[(oi, a)] = dr.name(oi, a)
        for (oi, a) in want:
            if (oi, a) in dr.covered and (oi, a) not in dr.insns and (oi, a) in dr.sites:
                st = dr.covered[(oi, a)]
                dr.placed[(oi, a)] = "%so%d_S%x" % (dr.tag, oi, a)
                dr.placed.setdefault((oi, st), dr.name(oi, st))

    def ref(self, dr, oi, a):
        if (oi, a) in dr.placed:
            return dr.placed[(oi, a)]
        if (oi, a) in dr.covered and (oi, dr.covered[(oi, a)]) in dr.placed:
            st = dr.covered[(oi, a)]
            return "%s+%d" % (dr.placed[(oi, st)], a - st)
        o = dr.by_idx.get(oi)
        if o is not None and o["whole"]:
            return "%so%d_start+0x%x" % (dr.tag, oi, a)
        return "0x%x" % a

    def source(self, raw=frozenset(), mark_all=True, exact=True):
        self._mark_all = mark_all
        for dr in self.drivers:
            self._place(dr, raw)
        regions = []                                         # (file offset, end, kind, driver, what)
        for dr in self.drivers:
            for o in dr.objs:
                if o["whole"] and o["size"]:
                    regions.append((o["start"], o["start"] + o["size"], "obj", dr, o))
            lf = dr.lf
            offs = [struct.unpack_from("<I", self.d, lf.fixup_page + 4 * i)[0] for i in range(lf.mpages + 1)]
            if offs[-1]:
                regions.append((lf.fixup_rec, lf.fixup_rec + offs[-1], "fixrec", dr, offs))
                regions.append((lf.fixup_page, lf.fixup_page + 4 * (lf.mpages + 1), "fixpage", dr, offs))
        regions.sort(key=lambda r: r[0])
        name = os.path.basename(self.path)
        L, lines_of = l2.Assembly(syntax="nasm", module=name), {}     # (the file as one L2 tree, as it is written)
        L.append("; %s -- assembled with NASM: nasm -f bin -o %s this-file" % (name, name))
        L.extend(_PRELUDE)
        for dr in self.drivers:
            for o in dr.objs:
                if o["whole"] and o["size"]:
                    L.append("%so%d_file equ 0x%x" % (dr.tag, o["idx"], o["start"]))
        pos, n, prev = 0, 0, None
        for lo, hi, kind, dr, what in regions + [(len(self.d), len(self.d), None, None, None)]:
            if lo < pos:                                     # (overlapping tables: written with what they overlap)
                continue
            if lo > pos:
                L.part("part%d" % n, "outside the objects", length=lo - pos)
                L.append("")
                L.append("section part%d %s vstart=0x%x align=1" % (n, "start=0x%x" % pos if exact or not prev else "follows=" + prev, pos))
                prev = "part%d" % n
                L.extend("    " + x for x in assembly.data_lines(self.d[pos:lo]))
                n += 1
            if kind is None:
                break
            if kind == "obj":
                L.part("%so%d" % (dr.tag, what["idx"]), "object", {"module": name, "segment": what["idx"], "offset": 0},
                       what["size"])
            else:
                L.part(dr.tag + kind, "fixups", length=hi - lo)
            L.append("")
            if kind == "obj":
                o = what
                sec = "%so%d" % (dr.tag, o["idx"])
                L.append("section %s %s vstart=0 align=1" % (sec, ("start=%s_file" % sec) if exact or not prev else "follows=" + prev))
                prev = sec
                L.append("bits %d" % o["bits"])
                L.append("%s_start:" % sec)
                self._object(L, dr, o, raw, lines_of)
            elif kind == "fixrec":
                sec = "%sfixrec" % dr.tag
                L.append("section %s %s vstart=0 align=1" % (sec, "start=0x%x" % lo if exact or not prev else "follows=" + prev))
                prev = sec
                self._fixrec(L, dr, what)
            else:
                sec = "%sfixpage" % dr.tag
                L.append("section %s %s vstart=0 align=1" % (sec, "start=0x%x" % lo if exact or not prev else "follows=" + prev))
                prev = sec
                for k in range(len(what)):
                    L.append("    dd %sfr_p%d-%sfr_p0" % (dr.tag, k, dr.tag))
            pos = hi
        L.append("")
        self.l2 = L.document()
        return asm.module(self.l2), lines_of

    def _object(self, L, dr, o, raw, lines_of):
        oi, base, size = o["idx"], o["start"], o["size"]
        limit = size
        a = 0
        while a < limit:
            if (oi, a) in dr.insns:
                i = dr.insns[(oi, a)]
                if (oi, a) in dr.placed:
                    L.append("%s:" % dr.placed[(oi, a)])
                for t in self._text(dr, oi, i, (dr.tag, oi, a) in raw, o["bits"]):
                    lines_of[len(L) + 1] = (dr.tag, oi, a)
                    L.append(t if t.endswith(":") else "    " + t)
                a += i.size
                continue
            stop = a + 1
            while stop < limit and (oi, stop) not in dr.insns:
                stop += 1
            self._data(L, dr, oi, base, a, stop)
            a = stop
        if (oi, size) in dr.placed:
            L.append("%s:" % dr.placed[(oi, size)])
        for v in sorted(x for (oj, x) in dr.placed if oj == oi and x > size):
            L.append("%s equ $+0x%x" % (dr.placed[(oi, v)], v - size))

    def _site_value(self, dr, oi, at, rec, w, b):
        """How the bytes at a fix-up's place are written: the label of the target the file holds there."""
        if rec["kind"] == "internal" and rec["source"] == 7 and w == 4 and rec.get("target") is not None:
            if struct.unpack("<I", b)[0] == rec["target"] and rec["obj"] in dr.by_idx:
                return "dd " + self.ref(dr, rec["obj"], rec["target"])
        return "db " + ",".join("0x%02x" % c for c in b)

    def _text(self, dr, oi, i, raw, bits):
        a, b = i.address, bytes(i.bytes)
        tgt = _target(i)
        inside = [k for k in range(i.size) if (oi, a + k) in dr.sites]
        if not raw and not inside:
            t = _nasm(i, bits, lambda v: self.ref(dr, oi, v))
            if t:
                return [t]
        out, k = [], 0
        while k < len(b):
            if k in inside:
                _rk, w, rec = dr.sites[(oi, a + k)]
                if k:
                    out.append("%s:" % dr.placed[(oi, a + k)])
                out.append(self._site_value(dr, oi, a + k, rec, w, b[k:k + w]))
                k += w
            elif tgt is not None and k == i.imm_offset:
                wd = {4: "dd", 2: "dw", 1: "db"}[i.imm_size]
                out.append("%s %s-($+%d)" % (wd, self.ref(dr, oi, tgt), i.imm_size))
                k += i.imm_size
            else:
                stops = [x for x in inside if x > k] + ([i.imm_offset] if tgt is not None and i.imm_offset > k else [])
                stop = min(stops + [len(b)])
                out.append("db " + ",".join("0x%02x" % c for c in b[k:stop]))
                k = stop
        first = next(n for n, x in enumerate(out) if not x.endswith(":"))
        out[first] += "%s; %s %s" % (" " * max(1, 40 - len(out[first])), i.mnemonic, i.op_str)
        return out

    def _data(self, L, dr, oi, base, a, end):
        cuts = sorted({x for (oj, x) in dr.placed if oj == oi and a <= x < end} |
                      {x for (oj, x) in dr.sites if oj == oi and a <= x < end})
        pos = a
        for c in cuts + [end]:
            if c < pos:
                if (oi, c) in dr.placed:
                    L.append("%s equ $-%d" % (dr.placed[(oi, c)], pos - c))
                continue
            if c > pos:
                L.extend("    " + x for x in assembly.data_lines(self.d[base + pos:base + c]))
                pos = c
            if c == end:
                break
            if (oi, c) in dr.placed:
                L.append("%s:" % dr.placed[(oi, c)])
            if (oi, c) in dr.sites:
                _k, w, rec = dr.sites[(oi, c)]
                if c + w <= end:
                    L.append("    " + self._site_value(dr, oi, c, rec, w, self.d[base + c:base + c + w]))
                    pos = c + w

    def _fixrec(self, L, dr, offs):
        """The fix-up records, page by page: each place as its label less its page's start, each
        internal target as its label."""
        lf = dr.lf
        by_page = {}
        for k, (pg, rec) in enumerate(dr.records):
            by_page.setdefault(pg, []).append(rec)
        for pg in range(lf.mpages + 1):
            L.append("%sfr_p%d:" % (dr.tag, pg))
            if pg == lf.mpages:
                break
            obj, po = lf.page_to_obj(pg + 1)
            obj = dr.by_idx.get(obj["idx"]) if obj else None
            for rec in by_page.get(pg + 1, []):
                raw = rec["raw"]
                src, fl = raw[0], raw[1]
                many = bool(src & 0x20)
                p = 2
                parts = ["db 0x%02x,0x%02x" % (src, fl)]
                if many:
                    parts.append("db %d" % raw[2])
                    p = 3
                else:
                    s = rec["sources"][0]
                    lab = dr.placed.get((obj["idx"], po + s)) if obj else None
                    parts.append("dw %s-0x%x" % (lab, po) if lab and obj["whole"] else "dw 0x%04x" % (s & 0xFFFF))
                    p = 4
                wide = 2 if fl & 0x40 else 1
                parts.append(("dw 0x%04x" if wide == 2 else "db 0x%02x") % int.from_bytes(raw[p:p + wide], "little"))
                p += wide
                rest = raw[p:]
                if rec["kind"] == "internal" and rec.get("target") is not None and rec["obj"] in dr.by_idx \
                        and dr.by_idx[rec["obj"]]["whole"]:
                    n = 4 if fl & 0x10 else 2
                    parts.append(("dd %s" if n == 4 else "dw %s") % self.ref(dr, rec["obj"], rec["target"]))
                    rest = rest[n:]
                if many:
                    srcs = rest[-2 * len(rec["sources"]):]
                    mid = rest[:-2 * len(rec["sources"])]
                    if mid:
                        parts.append("db " + ",".join("0x%02x" % c for c in mid))
                    for s in rec["sources"]:
                        lab = dr.placed.get((obj["idx"], po + s)) if obj else None
                        parts.append("dw %s-0x%x" % (lab, po) if lab and obj["whole"] else "dw 0x%04x" % (s & 0xFFFF))
                elif rest:
                    parts.append("db " + ",".join("0x%02x" % c for c in rest))
                L.extend("    " + x for x in parts)


def _differing(w, data, syms, raw):
    bad = set()
    for dr in w.drivers:
        order = sorted(dr.insns)
        for n, key in enumerate(order):
            tk = (dr.tag,) + key
            if tk in raw:
                continue
            oi, a = key
            i = dr.insns[key]
            o = dr.by_idx[oi]
            at, real = syms.get(dr.placed.get(key, ""), (None, None))
            if at is None:
                bad.add(tk)
                continue
            nxt = order[n + 1] if n + 1 < len(order) else None
            if nxt and nxt[0] == oi and nxt[1] == a + i.size and dr.placed.get(nxt) in syms and \
                    syms[dr.placed[nxt]][0] - at != i.size:
                bad.add(tk)
                continue
            mine, orig = data[real:real + i.size], w.d[o["start"] + a:o["start"] + a + i.size]
            skip = set(range(i.imm_offset, i.imm_offset + i.imm_size)) if _target(i) is not None else set()
            if len(mine) != len(orig) or any(mine[k] != orig[k] for k in range(len(orig)) if k not in skip):
                bad.add(tk)
    return bad


def reconstruct(path, outdir, log=None):
    say = log or (lambda s: None)
    if not image_writer.nasm():
        raise SystemExit("no assembler (settings.json: nasm)")
    outdir = os.path.abspath(outdir)
    os.makedirs(outdir, exist_ok=True)
    w = Writer(path)
    name = os.path.basename(path).lower()
    r = image_writer.flat(w.d, lambda raw, final: w.source(raw, mark_all=not final, exact=final),
                          lambda data, labels, raw: _differing(w, data, labels, raw), outdir, name, say, new_only=True)
    r.update(instructions=w.insn_count, raw=len(r["raw"]))
    return r


def main(argv):
    """python -m codebreaker le-source <driver> [--out DIR]"""
    args = [a for a in argv if not a.startswith("--")]
    if "--out" in argv:
        args.remove(argv[argv.index("--out") + 1])
    if not args:
        print(__doc__)
        return 2
    path = args[0]
    outdir = argv[argv.index("--out") + 1] if "--out" in argv else \
        settings.state("le_source", os.path.splitext(os.path.basename(path))[0].lower())
    return image_writer.flat_report(path, reconstruct(path, outdir, log=print))
