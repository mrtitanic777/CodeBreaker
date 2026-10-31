"""ne_writer.py -- a 16-bit Windows (or OS/2) program as assembly source that builds back into it, byte for byte.

    python -m codebreaker ne-source <program> [--out DIR] [--keep-stub]

    from codebreaker.source import ne_writer
    r = ne_writer.reconstruct(path, outdir)
    r["source"], r["image"], r["same"], r["instructions"], r["raw"], r["rounds"]

The way pe_writer writes a 32-bit image, for the segmented kind: one NASM source assembled flat (-f bin),
each segment a section at its place in the file (start=) whose addresses are its own offsets (vstart=0),
followed by its relocation table. Whatever the linker made is written as what it refers to, not as a
number: the segment table's sectors and lengths as expressions of the segments' places, every relocation
chain (LINK writes, in each place a relocation patches, where the next place of the same target is) as the
label of the next place, every record's first place as its label, and an internal reference's offset as
the label in its segment. So the file is the linker's output as written down, and stands without the linker
that made it -- whichever version that was.

Code is what is reached from the entry table, from the program's start, and from every internal reference
into a code segment; what is not reached is data. An instruction is written as its mnemonic where NASM
gives back the original's bytes (checked every round from the assembler's map, as pe_writer does); one that
holds a place a relocation patches is written as its bytes, with those places as expressions and its
mnemonic beside it. The DOS program in front (the stub) comes from its own source when that builds back.
"""
import os
import re
import struct


from codebreaker.base import settings
from codebreaker.formats.ne import NEFile
from codebreaker.source import assembly, image_writer, pe_writer
from codebreaker.translate import l2
from codebreaker.translate.backends import asm
from codebreaker import platforms

_BRANCH = re.compile(r"^(j\w+|call|loop\w*|jcxz|jecxz)$")
_STOP = {"ret", "retf", "jmp", "ljmp", "hlt", "iret", "iretd", "ud2"}
_SEGS = {0x26, 0x2E, 0x36, 0x3E, 0x64, 0x65}
_WIDTH = {2: 2, 3: 4, 5: 2, 11: 6, 13: 4, 0: 1}            # bytes a relocation patches, by source type

_REGS16 = (("ax", "cx", "dx", "bx", "sp", "bp", "si", "di"), ("eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"),
           ("al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"))
_PRELUDE = ["", "; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m), the one",
            "; Microsoft's compiler writes; NASM itself writes the other"]
for _w, _names in zip((1, 2, 0), _REGS16):
    _PRELUDE += ["%%assign _r_%s %d" % (n, k) for k, n in enumerate(_names)]
    _PRELUDE += ["%%assign _w_%s %d" % (n, _w) for n in _names]
_PRELUDE += ["%macro _rr 3", "  %if _w_%2 == 2", "    db 0x66", "  %endif",
             "  db (%1) | 2 | (_w_%2 > 0), 0xC0 | (_r_%2 << 3) | _r_%3", "%endmacro"]
_PRELUDE += ["%%define %s_ _rr 0x%02x," % (m, v) for m, v in sorted(pe_writer._RR.items())]


class Writer(object):
    def __init__(self, path):
        self.path = path
        self.ne = ne = NEFile(path)
        self.d = d = ne.data
        self.shift = ne.h["align_shift"]
        self.segs = []
        for s in ne.segments():
            if not s["file_off"]:
                continue
            relocs = ne.relocations(s)
            table = 2 + 8 * len(relocs) if s["flags"] & 0x100 else 0
            self.segs.append(dict(s, relocs=relocs, table=table, code=not s["flags"] & 1))
        self.by_idx = {s["idx"]: s for s in self.segs}
        # every place a relocation patches: (segment, offset) -> (record, width, chained)
        self.sites = {}
        for s in self.segs:
            for k, r in enumerate(s["relocs"]):
                w = _WIDTH.get(r["src"], 2)
                at, seen = r["offset"], set()
                while at not in seen and at + w <= s["length"]:
                    seen.add(at)
                    nxt = struct.unpack_from("<H", d, s["file_off"] + at)[0] if not r["additive"] else 0xFFFF
                    self.sites[(s["idx"], at)] = (k, w, not r["additive"], nxt)
                    if r["additive"] or nxt == 0xFFFF:
                        break
                    at = nxt
        self._decode()

    # -- code ----------------------------------------------------------------------------------
    def _decode(self):
        md = platforms.decoder(16, detail=True)
        self.insns, self.covered = {}, {}
        seeds = []
        for e in self.ne.entries():
            if e.get("kind") in ("FIXED", "MOVEABLE") and e["seg"] in self.by_idx:
                seeds.append((e["seg"], e["offset"]))
        cs = self.ne.h["csip"] >> 16
        if cs in self.by_idx:
            seeds.append((cs, self.ne.h["csip"] & 0xFFFF))
        entries = list(seeds)                                # (where the module is entered: each a function)
        for s in self.segs:
            for r in s["relocs"]:
                if r["kind"] == "INTERNAL" and r["seg"] != 0xFF and r["seg"] in self.by_idx:
                    seeds.append((r["seg"], r["entry"]))
        work = [x for x in seeds if x[0] in self.by_idx and self.by_idx[x[0]]["code"]]
        sitebytes = {}
        for (si, at), (_k, w, _c, _n) in self.sites.items():
            for j in range(w):
                sitebytes[(si, at + j)] = at
        while work:
            si, a = work.pop()
            s = self.by_idx[si]
            while 0 <= a < s["length"] and (si, a) not in self.insns and (si, a) not in self.covered:
                o = s["file_off"] + a
                one = next(md.disasm(self.d[o:min(o + 15, s["file_off"] + s["length"])], a, 1), None)
                if one is None or any((si, a + k) in self.covered for k in range(one.size)):
                    break
                # a relocated place must lie wholly inside the instruction
                starts = {sitebytes[(si, a + k)] for k in range(one.size) if (si, a + k) in sitebytes}
                if any(st < a or st + self.sites[(si, st)][1] > a + one.size for st in starts):
                    break
                self.insns[(si, a)] = one
                for k in range(one.size):
                    self.covered[(si, a + k)] = a
                tgt = self._target(one)
                if tgt is not None and not starts:
                    work.append((si, tgt))
                if one.mnemonic in _STOP:
                    break
                a += one.size
        self.starts = self._function_starts(entries)

    def _function_starts(self, entries):
        """Where each function begins, for the fragment each is in the file's L2 tree: the entry
        points and every place a call lands -- a near call, or a far call through a relocation to a
        segment of the module's own -- where an instruction was found."""
        out = set(entries)
        for (si, a), i in self.insns.items():
            t = self._target(i)
            if i.mnemonic == "call" and t is not None:
                out.add((si, t))
        for (si, at), (k, _w, _c, _n) in self.sites.items():
            st = self.covered.get((si, at))
            r = self.by_idx[si]["relocs"][k]
            if st == at - 1 and self.insns[(si, st)].mnemonic == "lcall" and r["kind"] == "INTERNAL" and \
                    r["seg"] != 0xFF:
                out.add((r["seg"], r["entry"]))
        return {x for x in out if x in self.insns}

    @staticmethod
    def _target(i):
        if _BRANCH.match(i.mnemonic) and i.imm_size and re.match(r"^0x[0-9a-f]+$", i.op_str):
            return int(i.op_str, 16)
        return None

    # -- names ---------------------------------------------------------------------------------
    def label(self, si, a):
        return ("s%d_L%x" if (si, a) in self.insns else "s%d_D%x") % (si, a)

    def ref(self, si, a):
        if (si, a) in self.placed:
            return self.placed[(si, a)]
        if (si, a) in self.covered:
            st = self.covered[(si, a)]
            return "%s+%d" % (self.placed[(si, st)], a - st)
        return "0x%x" % a

    def wanted(self, mark_all):
        want = set()
        for s in self.segs:
            for r in s["relocs"]:
                want.add((s["idx"], r["offset"]))
                if r["kind"] == "INTERNAL" and r["seg"] in self.by_idx:
                    want.add((r["seg"], r["entry"]))
        for (si, at), (_k, _w, chained, nxt) in self.sites.items():
            want.add((si, at))
            if chained and nxt != 0xFFFF:
                want.add((si, nxt))
        for e in self.ne.entries():
            if e.get("kind") in ("FIXED", "MOVEABLE") and e["seg"] in self.by_idx:
                want.add((e["seg"], e["offset"]))
        for (si, a), i in self.insns.items():
            t = self._target(i)
            if t is not None:
                want.add((si, t))
        if mark_all:
            want |= set(self.insns)
        return want

    # -- source --------------------------------------------------------------------------------
    def text(self, si, i, raw):
        a, b = i.address, bytes(i.bytes)
        tgt = self._target(i)
        inside = [k for k in range(i.size) if (si, a + k) in self.sites]
        if not raw and not inside:
            t = self._nasm(si, i, tgt)
            if t:
                return [t]
        out, k = [], 0
        while k < len(b):
            if k in inside:
                _rk, w, chained, nxt = self.sites[(si, a + k)]
                if k:
                    out.append("%s:" % self.placed[(si, a + k)])
                if chained:
                    out.append("dw %s" % ("0xffff" if nxt == 0xFFFF else self.ref(si, nxt)))
                    if w > 2:
                        out.append("db " + ",".join("0x%02x" % c for c in b[k + 2:k + w]))
                else:
                    out.append("db " + ",".join("0x%02x" % c for c in b[k:k + w]))
                k += w
            elif tgt is not None and k == i.imm_offset:
                wd = {4: "dd", 2: "dw", 1: "db"}[i.imm_size]
                out.append("%s %s-($+%d)" % (wd, self.ref(si, tgt), i.imm_size))
                k += i.imm_size
            else:
                stops = [x for x in inside if x > k] + ([i.imm_offset] if tgt is not None and i.imm_offset > k else [])
                stop = min(stops + [len(b)])
                out.append("db " + ",".join("0x%02x" % c for c in b[k:stop]))
                k = stop
        first = next(n for n, x in enumerate(out) if not x.endswith(":"))
        out[first] += "%s; %s %s" % (" " * max(1, 40 - len(out[first])), i.mnemonic, i.op_str)
        return out

    def _nasm(self, si, i, tgt):
        m, ops = i.mnemonic, i.op_str
        if m in ("ljmp", "lcall") or m.startswith("l") and m[1:] in ("call", "jmp"):
            return None
        if assembly.STRING_OPS.match(m) or re.match(r"^(rep |repe |repne )?(movs|cmps|stos|lods|scas|ins|outs)[bwd]$", m):
            return None if any(p in _SEGS for p in i.prefix) else m
        if tgt is not None:
            if m == "call":
                return "call %s" % self.ref(si, tgt) if i.imm_size == 2 and i.size == 3 else None
            if m in ("loop", "loope", "loopne", "jcxz", "jecxz"):
                return "%s %s" % (m, self.ref(si, tgt)) if i.size == 2 else None
            if i.size not in (2, 3, 4):
                return None
            return "%s %s %s" % (m, "short" if i.imm_size == 1 else "near", self.ref(si, tgt))
        rr = re.match(r"^(\w+), (\w+)$", ops)
        if m in pe_writer._RR and rr and all(any(x in g for g in _REGS16) for x in rr.groups()):
            op = bytes(i.bytes)[1 if i.bytes[0] == 0x66 else 0]
            return ("%s_ %s" % (m, ops)) if op & 2 else "%s %s" % (m, ops)
        ops = ops.replace(" ptr ", " ").replace("xmmword ", "oword ").replace("xword ", "tword ")
        ops = re.sub(r"\bst\((\d)\)", r"st\1", ops)
        ops = re.sub(r"\b([cdefgs]s):\[", r"[\1:", ops)
        if i.imm_size == 2 and m in pe_writer._ARITH:
            mm = re.search(r"(-?0x[0-9a-f]+|-?\d+)$", ops)
            if mm:
                v = int(mm.group(1), 0)
                if -128 <= v <= 127 or v >= 0xFF80:
                    ops = ops[:mm.start()] + "strict word " + mm.group(1)
        if i.disp_size == 2:
            br = re.search(r"\[([^\]]*)\]", ops)
            if br and re.search(r"[a-z]{2}\s*[+-]", br.group(1)):
                mm = re.search(r"[+-] (0x[0-9a-f]+|\d+)$", br.group(1))
                if mm and int(mm.group(1), 0) <= 0x7F:
                    ops = ops[:br.start()] + "[word " + br.group(1) + "]" + ops[br.end():]
        return ("%s %s" % (m, ops)).strip()

    def source(self, raw=frozenset(), mark_all=True, stub=None, exact=True):
        want = self.wanted(mark_all)
        self.placed = {}
        for (si, a) in want:
            if si in self.by_idx and ((si, a) in self.insns or (si, a) not in self.covered) and a <= self.by_idx[si]["length"]:
                self.placed[(si, a)] = self.label(si, a)
        for (si, a) in want:
            if (si, a) in self.covered and (si, a) not in self.insns:
                st = self.covered[(si, a)]
                if (si, a) in self.sites and (si, st) in self.insns:
                    self.placed[(si, a)] = "s%d_S%x" % (si, a)       # (a place inside an instruction written as bytes)
                    raw = set(raw) | {(si, st)}
                self.placed.setdefault((si, st), self.label(si, st))
        self._raw = raw
        name = os.path.basename(self.path)
        L, lines_of = l2.Assembly(syntax="nasm", module=name), {}     # (the file as one L2 tree, as it is written)
        L.append("; %s -- assembled with NASM: nasm -f bin -o %s this-file" % (name, name))
        L.append("bits 16")
        L.append("SHIFT equ %d" % self.shift)
        L.append("; where each segment lies in the file: its section's place, and its sector in the segment table")
        L.extend("seg%d_start_file equ 0x%x" % (s["idx"], s["file_off"]) for s in self.segs)
        L.extend(_PRELUDE)
        regions = sorted([(s["file_off"], s["file_off"] + s["length"] + s["table"], s) for s in self.segs], key=lambda r: r[0])
        pos, n, prev = 0, 0, None
        segtab = self.ne.ne_off + self.ne.h["seg_tab"]
        for lo, hi, s in regions + [(len(self.d), len(self.d), None)]:
            if lo > pos:
                L.part("part%d" % n, "outside the segments")
                L.append("")
                L.append("section part%d %s vstart=0x%x align=1" % (n, "start=0x%x" % pos if exact or not prev else "follows=" + prev, pos))
                prev = "part%d" % n
                if pos == 0 and stub:
                    L.append('incbin "%s"' % stub)
                    pos = os.path.getsize(stub)
                self._data_plain(L, pos, lo, segtab)
                n += 1
            if s is None:
                break
            L.part("seg%d" % s["idx"], "segment", {"module": name, "segment": s["idx"], "offset": 0}, s["length"])
            L.append("")
            # (while the rounds look for what NASM does not give back, a section that came out longer
            # must not run into the next: each follows the one before; the last round places them)
            L.append("section seg%d %s vstart=0 align=1" % (s["idx"], "start=seg%d_start_file" % s["idx"] if exact
                                                               else "follows=" + prev))
            prev = "seg%d" % s["idx"]
            L.append("seg%d_start:" % s["idx"])
            self._segment(L, s, lines_of)
            pos = hi
        L.append("")
        self.l2 = L.document()
        return asm.module(self.l2), lines_of

    def _data_plain(self, L, a, end, segtab):
        """Bytes of the file outside the segments; the segment table's sectors and lengths as expressions."""
        n = self.ne.h["n_segs"]
        rows = {segtab + 8 * k: k + 1 for k in range(n)}
        p = a
        while p < end:
            idx = rows.get(p)
            if idx in self.by_idx and p + 8 <= end:
                s = self.by_idx[idx]
                L.append("    dw seg%d_start_file >> SHIFT, seg%d_end%s" % (idx, idx, "" if s["length"] < 0x10000 else " & 0xffff"))
                L.extend("    " + x for x in assembly.data_lines(self.d[p + 4:p + 8]))
                p += 8
                continue
            nxt = min([r for r in rows if r > p] + [end])
            L.extend("    " + x for x in assembly.data_lines(self.d[p:nxt]))
            p = nxt

    def _segment(self, L, s, lines_of):
        si, base, length = s["idx"], s["file_off"], s["length"]
        a = 0
        mine = sorted(x for (sj, x) in self.starts if sj == si and x < length)
        ends = dict(zip(mine, mine[1:] + [length]))          # (a function runs to the next one)
        module = os.path.basename(self.path)
        while a < length:
            if (si, a) in self.insns:
                i = self.insns[(si, a)]
                if a in ends:                                # (each function a fragment of its own)
                    L.part(self.placed.get((si, a)) or self.label(si, a), "function",
                           {"module": module, "segment": si, "offset": a}, ends[a] - a)
                if (si, a) in self.placed:
                    L.append("%s:" % self.placed[(si, a)])
                for t in self.text(si, i, (si, a) in self._raw):
                    lines_of[len(L) + 1] = (si, a)
                    L.append(t if t.endswith(":") else "    " + t)
                a += i.size
                continue
            stop = a + 1
            while stop < length and (si, stop) not in self.insns:
                stop += 1
            self._data(L, si, base, a, stop)
            a = stop
        if (si, length) in self.placed:
            L.append("%s:" % self.placed[(si, length)])
        L.append("seg%d_end:" % si)
        if s["table"]:
            L.append("    dw %d                                   ; relocations" % len(s["relocs"]))
            for r in s["relocs"]:
                p = base + length + 2 + 8 * s["relocs"].index(r)
                rec = self.d[p:p + 8]
                head = "db 0x%02x,0x%02x" % (rec[0], rec[1])
                where = self.ref(si, r["offset"])
                if r["kind"] == "INTERNAL" and r["seg"] != 0xFF and r["seg"] in self.by_idx:
                    L.extend(["    " + head, "    dw " + where, "    db 0x%02x,0x%02x" % (rec[4], rec[5]),
                              "    dw " + self.ref(r["seg"], r["entry"])])
                else:
                    L.extend(["    " + head, "    dw " + where, "    db " + ",".join("0x%02x" % c for c in rec[4:8])])

    def _data(self, L, si, base, a, end):
        cuts = sorted({x for (sj, x) in self.placed if sj == si and a <= x < end} |
                      {x for (sj, x) in self.sites if sj == si and a <= x < end})
        pos = a
        for c in cuts + [end]:
            if c < pos:
                if (si, c) in self.placed:
                    L.append("%s equ $-%d" % (self.placed[(si, c)], pos - c))
                continue
            if c > pos:
                L.extend("    " + x for x in assembly.data_lines(self.d[base + pos:base + c]))
                pos = c
            if c == end:
                break
            if (si, c) in self.placed:
                L.append("%s:" % self.placed[(si, c)])
            if (si, c) in self.sites:
                _k, w, chained, nxt = self.sites[(si, c)]
                if c + w <= end:
                    if chained:
                        L.append("    dw %s" % ("0xffff" if nxt == 0xFFFF else self.ref(si, nxt)))
                        if w > 2:
                            L.extend("    " + x for x in assembly.data_lines(self.d[base + c + 2:base + c + w]))
                    else:
                        L.extend("    " + x for x in assembly.data_lines(self.d[base + c:base + c + w]))
                    pos = c + w


def _differing(w, data, syms, raw):
    bad = set()
    order = sorted(w.insns)
    for n, key in enumerate(order):
        if key in raw:
            continue
        si, a = key
        i = w.insns[key]
        s = w.by_idx[si]
        at, real = syms.get(w.placed.get(key, ""), (None, None))
        if at is None:
            bad.add(key)
            continue
        nxt = order[n + 1] if n + 1 < len(order) else None
        if nxt and nxt[0] == si and nxt[1] == a + i.size and syms.get(w.placed.get(nxt, "")) is not None \
                and syms[w.placed[nxt]][0] - at != i.size:
            bad.add(key)
            continue
        o, ob = real, s["file_off"] + a
        mine, orig = data[o:o + i.size], w.d[ob:ob + i.size]
        skip = set()
        if w._target(i) is not None:
            skip = set(range(i.imm_offset, i.imm_offset + i.imm_size))
        if len(mine) != len(orig) or any(mine[k] != orig[k] for k in range(len(orig)) if k not in skip):
            bad.add(key)
    return bad


def reconstruct(path, outdir, stub=None, log=None):
    say = log or (lambda s: None)
    if not image_writer.nasm():
        raise SystemExit("no assembler (settings.json: nasm)")
    outdir = os.path.abspath(outdir)
    os.makedirs(outdir, exist_ok=True)
    w = Writer(path)
    name = os.path.basename(path).lower()
    if stub is None:
        stub = _stub(w, outdir, say)
    stub = stub or None

    def write(raw, final):
        return w.source(raw, mark_all=False, stub=stub) if final else w.source(raw, mark_all=True, stub=stub, exact=False)
    r = image_writer.flat(w.d, write, lambda data, labels, raw: _differing(w, data, labels, raw | w._raw),
                          outdir, name, say)
    r.update(instructions=len(w.insns), raw=len(r["raw"] | w._raw))
    return r


def _stub(w, outdir, say):
    class _P(object):
        pass
    p = _P()
    p.d, p.pe = w.d, _P()
    p.pe.e_lfanew = w.ne.ne_off
    return pe_writer._stub(p, outdir, say)


def main(argv):
    """python -m codebreaker ne-source <program> [--out DIR] [--keep-stub]"""
    args = [a for a in argv if not a.startswith("--")]
    if "--out" in argv:
        args.remove(argv[argv.index("--out") + 1])
    if not args:
        print(__doc__)
        return 2
    path = args[0]
    outdir = argv[argv.index("--out") + 1] if "--out" in argv else \
        settings.state("ne_source", os.path.splitext(os.path.basename(path))[0].lower())
    return image_writer.flat_report(path, reconstruct(path, outdir, stub=False if "--keep-stub" in argv else None, log=print))
