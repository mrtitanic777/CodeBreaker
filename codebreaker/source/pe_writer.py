"""pe_writer.py -- a 32-bit Windows program as assembly source that builds back into it, byte for byte.

    python -m codebreaker pe-source <program> [--out DIR] [--stub SOURCE.ASM]

    from codebreaker.source import pe_writer
    r = pe_writer.reconstruct(path, outdir)
    r["source"], r["image"], r["same"], r["instructions"], r["raw"], r["rounds"]

The whole file is one NASM source assembled flat (-f bin): one section for the headers and one
for every section of the image, each placed at its file offset (start=) and at its address
(vstart=), so a label's value is the address the program sees. Nothing the loader patches is a
number: every place the base-relocation table names is written as the label it points at, the
import table's addresses are written as `label - IMAGE`, and the entry point as the label of the
entry. Calls and jumps go to labels.

Code is what is reached from the entry point, from the starts of functions, and from every
relocated pointer into code (function pointers, the tables of a switch), each path stopping at
an instruction that would cut through a relocated place; what is not reached is data.

An instruction is written as its mnemonic only where the assembler turns that text back into the
original's bytes: every round assembles the whole file, reads where each instruction went (the
assembler's map of labels) and compares its bytes; one that differs, or that the assembler
rejects, is written as its bytes (with its mnemonic beside it) in the next round, until nothing
differs. The DOS program in front of the headers (a stub) is either kept as the original's bytes
or, given --stub, assembled from its own source (dos-source) and included.
"""
import os
import re
import struct
import subprocess

import capstone

from codebreaker.base import settings
from codebreaker.formats import pe_functions
from codebreaker.formats.pe import open_pe
from codebreaker.source import assembly

_BRANCH = re.compile(r"^(j\w+|call|loop\w*|jecxz|jcxz)$")
_STOP = {"ret", "retf", "jmp", "hlt", "iretd", "iret", "ud2"}
_SEGS = {0x26, 0x2E, 0x36, 0x3E, 0x64, 0x65}
_ARITH = {"add", "or", "adc", "sbb", "and", "sub", "xor", "cmp", "push", "imul", "mov", "test"}
_MAX_ROUNDS = 30


_REGS = (("eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"), ("ax", "cx", "dx", "bx", "sp", "bp", "si", "di"),
         ("al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"))
_RR = {"add": 0x00, "or": 0x08, "adc": 0x10, "sbb": 0x18, "and": 0x20, "sub": 0x28, "xor": 0x30, "cmp": 0x38, "mov": 0x88}
_PRELUDE = ["", "; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m), the one",
            "; Microsoft's compiler writes; NASM itself writes the other"]
for _w, _names in zip((1, 2, 0), _REGS):
    _PRELUDE += ["%%assign _r_%s %d" % (n, k) for k, n in enumerate(_names)]
    _PRELUDE += ["%%assign _w_%s %d" % (n, _w) for n in _names]
_PRELUDE += ["%macro _rr 3", "  %if _w_%2 == 2", "    db 0x66", "  %endif",
             "  db (%1) | 2 | (_w_%2 > 0), 0xC0 | (_r_%2 << 3) | _r_%3", "%endmacro"]
_PRELUDE += ["%%define %s_ _rr 0x%02x," % (m, v) for m, v in sorted(_RR.items())]


def nasm():
    exe = settings.get("nasm") or ""
    return exe if os.path.isfile(exe) else ""


class Writer(object):
    def __init__(self, path):
        self.path = path
        self.pe = pe = open_pe(path)
        self.d = d = pe.data
        self.B = B = pe.image_base
        self.headers = struct.unpack_from("<I", d, pe.e_lfanew + 24 + 60)[0]
        self.secs = []
        for i, s in enumerate(pe.sections):
            va = B + s["vaddr"]
            nxt = B + pe.sections[i + 1]["vaddr"] if i + 1 < len(pe.sections) else va + max(s["vsize"], s["rawsize"])
            self.secs.append(dict(s, va=va, file=bool(s["rawptr"]), extent=nxt - va,
                                  code=bool(s["chars"] & 0x20000020), nasm=self._secname(s["name"], i)))
        self.sites = {}                                      # VA of a relocated dword -> the address it holds
        for r in pe.base_relocs():
            o = pe.file_offset(r)
            if o is not None:
                self.sites[B + r] = struct.unpack_from("<I", d, o)[0]
        self.imports = {}                                    # IAT slot VA -> label
        for mod in pe.import_table():
            for s in mod["symbols"]:
                nm = s["name"] or "%s_%d" % (re.sub(r"\W", "_", mod["module"]), s["ordinal"])
                self.imports[B + s["slot"]] = "__imp_" + re.sub(r"[^\w@?$]", "_", nm)
        self.rva_fields = self._import_rvas()                # file offset of an RVA field -> the RVA
        self.tail = len(d) - max(s["rawptr"] + s["rawsize"] for s in pe.sections)
        self._decode()

    @staticmethod
    def _secname(name, i):
        n = re.sub(r"[^\w.]", "_", name) or "sec%d" % i
        return n if n not in ("hdr", "tail") else n + "_"

    # -- what is where -------------------------------------------------------------------------
    def section_of(self, va):
        for s in self.secs:
            if s["va"] <= va < s["va"] + s["extent"]:
                return s
        return None

    def offset(self, va):
        """File offset of an address in a section's file part, or None."""
        if self.B <= va < self.B + self.headers:
            return va - self.B
        s = self.section_of(va)
        if s and s["file"] and va - s["va"] < s["rawsize"]:
            return s["rawptr"] + va - s["va"]
        return None

    def _import_rvas(self):
        """The RVA fields of the import directory (descriptors, lookup and address tables)."""
        pe, d, out = self.pe, self.d, {}
        va, _size = pe.directory(1)
        off = pe.file_offset(va) if va else None
        k = 0
        while off is not None and off + 20 * (k + 1) <= len(d):
            rec = off + 20 * k
            lookup, _ts, _fw, name, iat = struct.unpack_from("<IIIII", d, rec)
            if not (lookup or name or iat):
                break
            for at, v in ((rec, lookup), (rec + 12, name), (rec + 16, iat)):
                if v:
                    out[at] = v
            for table in (lookup, iat):
                t = pe.file_offset(table) if table else None
                j = 0
                while t is not None:
                    v = struct.unpack_from("<I", d, t + 4 * j)[0]
                    if not v:
                        break
                    if not v & 0x80000000:
                        out[t + 4 * j] = v
                    j += 1
            k += 1
        return out

    # -- code ----------------------------------------------------------------------------------
    def _decode(self):
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        md.detail = True
        self.insns = {}                                      # VA -> instruction
        covered = {}                                         # VA of every byte of an instruction -> its start
        code = [s for s in self.secs if s["code"] and s["file"]]
        def in_code(a):
            return any(s["va"] <= a < s["va"] + s["rawsize"] for s in code)
        seeds = [self.B + self.pe.entry] if self.pe.entry else []
        seeds += sorted(pe_functions.function_spans(self.path))
        seeds += sorted(v for v in self.sites.values() if in_code(v))
        work = [a for a in seeds if in_code(a)]
        while work:
            a = work.pop()
            while in_code(a) and a not in self.insns:
                if a in covered:                             # into the middle of one already decoded
                    break
                o = self.offset(a)
                one = next(md.disasm(self.d[o:o + 15], a, 1), None)
                if one is None or not self._fits(one) or any(covered.get(a + k) is not None for k in range(one.size)) \
                        or not in_code(a + one.size - 1):
                    break
                self.insns[a] = one
                for k in range(one.size):
                    covered[a + k] = a
                m = one.mnemonic
                tgt = self._target(one)
                if tgt is not None and in_code(tgt):
                    work.append(tgt)
                if m in _STOP or m.startswith("int3") or (m == "jmp"):
                    break
                a += one.size
        self.covered = covered

    def _fits(self, i):
        """Every relocated place inside the instruction is its whole 4-byte displacement or immediate."""
        for k in range(i.size):
            if i.address + k in self.sites:
                at = k
                if not ((at == i.disp_offset and i.disp_size == 4) or (at == i.imm_offset and i.imm_size == 4)):
                    return False
                if _BRANCH.match(i.mnemonic) and at == i.imm_offset:
                    return False                             # a relative operand is never relocated
        return True

    @staticmethod
    def _target(i):
        if _BRANCH.match(i.mnemonic) and i.imm_size and re.match(r"^0x[0-9a-f]+$", i.op_str):
            return int(i.op_str, 16)
        return None

    # -- names ---------------------------------------------------------------------------------
    def wanted(self):
        """The addresses that need a label: what relocated places, import fields and branches name."""
        want = set(self.sites.values())
        want.update(self.B + v for v in self.rva_fields.values())
        for i in self.insns.values():
            t = self._target(i)
            if t is not None:
                want.add(t)
        if self.pe.entry:
            want.add(self.B + self.pe.entry)
        return want

    def name(self, va):
        if va in self.imports:
            return self.imports[va]
        return ("L%x" if va in self.insns else "D%x") % va

    def ref(self, va):
        """How the source spells an address: its label, a label plus a distance, or IMAGE plus one."""
        if va in self.placed:
            return self.placed[va]
        if va in self.covered:                               # inside an instruction
            st = self.covered[va]
            return "%s+%d" % (self.placed[st], va - st)
        if self.B <= va < self.B + self.headers or self.section_of(va) is not None:
            return "IMAGE+0x%x" % (va - self.B)
        return "0x%x" % (va & 0xFFFFFFFF)                    # (outside the image: a fixed address)

    # -- source --------------------------------------------------------------------------------
    def text(self, i, raw):
        """An instruction's lines: its mnemonic in NASM's spelling, or its bytes with the
        relocated places and branch distances as expressions (raw)."""
        a, b = i.address, bytes(i.bytes)
        tgt = self._target(i)
        site = next((k for k in range(i.size) if a + k in self.sites), None)
        sites = [k for k in range(i.size) if a + k in self.sites]
        asm = "%s %s" % (i.mnemonic, i.op_str)
        if not raw:
            t = self._nasm(i, tgt, sites)
            if t:
                return [t]
        out, k = [], 0
        while k < len(b):
            if k in sites:
                out.append("dd %s" % self.ref(self.sites[a + k]))
                k += 4
            elif tgt is not None and k == i.imm_offset:
                w = "dd" if i.imm_size == 4 else "dw" if i.imm_size == 2 else "db"
                out.append("%s %s-($+%d)" % (w, self.ref(tgt), i.imm_size))
                k += i.imm_size
            else:
                stop = min([x for x in sites if x > k] + ([i.imm_offset] if tgt is not None and i.imm_offset > k else []) + [len(b)])
                out.append("db " + ",".join("0x%02x" % c for c in b[k:stop]))
                k = stop
        out[0] += "%s; %s" % (" " * max(1, 40 - len(out[0])), asm.strip())
        return out

    def _nasm(self, i, tgt, sites):
        m, ops = i.mnemonic, i.op_str
        if any(p in _SEGS for p in i.prefix) and assembly.STRING_OPS.match(m + "x"[:0]):
            return None
        if m.startswith("l") and m in ("ljmp", "lcall"):
            return None
        sm = assembly.STRING_OPS.match(m)
        if sm or re.match(r"^(rep |repe |repne )?(movs|cmps|stos|lods|scas|ins|outs)[bwd]$", m):
            if any(p in _SEGS for p in i.prefix):
                return None
            return m
        if tgt is not None:
            if m == "call":
                return "call %s" % self.ref(tgt) if i.imm_size == 4 else None
            if m in ("loop", "loope", "loopne", "jecxz", "jcxz"):
                return "%s %s" % (m, self.ref(tgt))
            return "%s %s %s" % (m, "short" if i.imm_size == 1 else "near", self.ref(tgt))
        rr = re.match(r"^(\w+), (\w+)$", ops)
        if m in _RR and rr and all(any(x in g for g in _REGS) for x in rr.groups()):
            op = bytes(i.bytes)[1 if i.bytes[0] == 0x66 else 0]
            return ("%s_ %s" % (m, ops)) if op & 2 else "%s %s" % (m, ops)
        ops = ops.replace(" ptr ", " ").replace("xmmword ", "oword ").replace("xword ", "tword ")
        ops = re.sub(r"\[(\w\w:)?(e\w\w\*[248])", r"[nosplit ", ops)
        ops = re.sub(r"\bst\((\d)\)", r"st\1", ops)
        ops = re.sub(r"\b([cdefgs]s):\[", r"[\1:", ops)
        for k in sites:
            v = self.sites[i.address + k]
            lab = self.ref(v)
            if k == i.disp_offset:
                br = re.search(r"\[[^\]]*\]", ops)
                if not br:
                    return None
                inner = re.sub(r"(?<![\w+])0x%x\b" % v, lab, br.group(0), count=1)
                if inner == br.group(0):
                    return None
                ops = ops[:br.start()] + inner + ops[br.end():]
            else:
                new = re.sub(r"(?<![\w\[+])0x%x$" % v, ("dword " if m == "push" else "") + lab, ops)
                if new == ops:
                    return None
                ops = new
        if i.imm_size in (2, 4) and i.imm_offset not in sites and m in _ARITH:
            mm = re.search(r"(-?0x[0-9a-f]+|-?\d+)$", ops)
            if mm:
                v = int(mm.group(1), 0)
                if -128 <= v <= 127 or v >= (0xFF80 if i.imm_size == 2 else 0xFFFFFF80):
                    ops = ops[:mm.start()] + ("strict word " if i.imm_size == 2 else "strict dword ") + mm.group(1)
        if i.disp_size == 4 and i.disp_offset not in sites:
            br = re.search(r"\[([^\]]*)\]", ops)
            if br and re.search(r"[a-z]{2,3}\s*[+-]", br.group(1)):
                ops = ops[:br.start()] + "[dword " + br.group(1) + "]" + ops[br.end():]
        return ("%s %s" % (m, ops)).strip()

    def source(self, raw=frozenset(), mark_all=True, stub=None):
        """The file's text. -> (text, {line number: instruction VA})"""
        want = self.wanted()
        if mark_all:
            want |= set(self.insns)
        self.placed = {}
        # a label can stand at an instruction's start or anywhere in data
        for va in want:
            if va in self.insns or va not in self.covered:
                if self.section_of(va) is not None or self.B <= va < self.B + self.headers:
                    self.placed[va] = self.name(va)
        for st in {self.covered[v] for v in want if v in self.covered and v not in self.insns}:
            self.placed[st] = self.name(st)
        L, lines_of = [], {}
        L.append("; %s -- assembled with NASM: nasm -f bin -o %s %s" % (
            os.path.basename(self.path), os.path.basename(self.path), "this file"))
        L.append("bits 32")
        L.append("IMAGE equ 0x%x" % self.B)
        L.extend(_PRELUDE)
        L.append("")
        L.append("section hdr start=0 vstart=0x%x align=1" % self.B)
        if stub:
            L.append('incbin "%s"' % stub)
            size = os.path.getsize(stub)
            self._data(L, self.B + size, self.B + self.headers, header=True)
        else:
            self._data(L, self.B, self.B + self.headers, header=True)
        for s in self.secs:
            L.append("")
            if not s["file"]:
                L.append("section %s nobits vstart=0x%x" % (s["nasm"], s["va"]))
                self._bss(L, s)
                continue
            L.append("section %s start=0x%x vstart=0x%x align=1" % (s["nasm"], s["rawptr"], s["va"]))
            a, end = s["va"], s["va"] + s["rawsize"]
            while a < end:
                if a in self.insns:
                    i = self.insns[a]
                    if a in self.placed:
                        L.append("%s:" % self.placed[a])
                    for k, t in enumerate(self.text(i, a in raw)):
                        lines_of[len(L) + 1] = a
                        L.append("    " + t)
                    a += i.size
                else:
                    stop = a + 1
                    while stop < end and stop not in self.insns:
                        stop += 1
                    self._data(L, a, stop)
                    a = stop
            # labels in the part of the section the file does not store (zeros the loader adds): from its end
            for v in sorted(x for x in self.placed if end <= x < s["va"] + s["extent"]):
                L.append("%s equ $+0x%x" % (self.placed[v], v - end))
        if self.tail:
            L.append("")
            L.append("section tail start=0x%x vstart=0 align=1" % (len(self.d) - self.tail))
            L.extend("    " + x for x in assembly.data_lines(self.d[len(self.d) - self.tail:]))
        L.append("")
        return "\n".join(L), lines_of

    def _bss(self, L, s):
        a, end = s["va"], s["va"] + s["extent"]
        for p in sorted(v for v in self.placed if a <= v < end):
            if p > a:
                L.append("    resb 0x%x" % (p - a))
            L.append("%s:" % self.placed[p])
            a = p
        if end > a:
            L.append("    resb 0x%x" % (end - a))

    def _data(self, L, a, end, header=False):
        """Bytes from a to end: labels where they stand, relocated places and RVA fields as
        expressions, the rest as db lines."""
        cuts = sorted({v for v in self.placed if a <= v < end} |
                      {v for v in self.sites if a <= v < end})
        if header and self.pe.entry:
            ep = self.pe.e_lfanew + 24 + 16
            cuts = sorted(set(cuts) | {self.B + ep})
        rv = {}
        for o, v in self.rva_fields.items():
            va = self._va_of_offset(o)
            if va is not None and a <= va < end:
                rv[va] = v
        cuts = sorted(set(cuts) | set(rv))
        pos = a
        for c in cuts + [end]:
            if c < pos:
                if c in self.placed:                         # inside the dword just written
                    L.append("%s equ $-%d" % (self.placed[c], pos - c))
                continue
            if c > pos:
                o = self.offset(pos)
                L.extend("    " + x for x in assembly.data_lines(self.d[o:o + (c - pos)]))
                pos = c
            if c == end:
                break
            if c in self.placed:
                L.append("%s:" % self.placed[c])
            if c in self.sites and c + 4 <= end:
                L.append("    dd %s" % self.ref(self.sites[c]))
                pos = c + 4
            elif c in rv and c + 4 <= end:
                L.append("    dd %s-IMAGE" % self.ref(self.B + rv[c]))
                pos = c + 4
            elif header and self.pe.entry and c == self.B + self.pe.e_lfanew + 40:
                L.append("    dd %s-IMAGE                          ; the entry point" % self.ref(self.B + self.pe.entry))
                pos = c + 4

    def _va_of_offset(self, o):
        for s in self.secs:
            if s["file"] and s["rawptr"] <= o < s["rawptr"] + s["rawsize"]:
                return s["va"] + o - s["rawptr"]
        return None


def _assemble(src, work, name):
    asm = os.path.join(work, name + ".asm")
    out = os.path.join(work, name)
    mp = os.path.join(work, name + ".map")
    with open(asm, "w", newline="\n", encoding="latin1") as f:
        f.write("[map symbols %s]\n" % mp.replace("\\", "/") + src)
    for p in (out, mp):
        if os.path.exists(p):
            os.remove(p)
    r = subprocess.run([nasm(), "-f", "bin", "-w-all", "-o", out, asm], capture_output=True, text=True,
                       errors="replace", cwd=work)
    return (out if r.returncode == 0 and os.path.isfile(out) else None), r.stderr + r.stdout, mp


def _map(mp):
    syms = {}
    with open(mp, encoding="latin1") as f:
        for line in f:
            m = re.match(r"^\s*([0-9A-F]+)\s+([0-9A-F]+)\s+(\S+)\s*$", line)
            if m:
                syms[m.group(3)] = int(m.group(2), 16)
    return syms


def _differing(w, data, syms, raw):
    """The instructions the assembler did not give back: compared where each one went, its
    length (to the next instruction's label) and its bytes, outside the places whose value
    depends on where things are (relocated addresses, branch distances)."""
    bad = set()
    order = sorted(w.insns)
    for n, a in enumerate(order):
        if a in raw:
            continue
        i = w.insns[a]
        at = syms.get(w.placed.get(a, ""))
        s = w.section_of(a)
        if at is None or s is None:
            bad.add(a)
            continue
        nxt = order[n + 1] if n + 1 < len(order) else None
        if nxt == a + i.size and syms.get(w.placed.get(nxt, "")) is not None and                 w.section_of(nxt) is s and syms[w.placed[nxt]] - at != i.size:
            bad.add(a)
            continue
        o, ob = s["rawptr"] + at - s["va"], w.offset(a)
        mine, orig = bytearray(data[o:o + i.size]), bytearray(w.d[ob:ob + i.size])
        skip = {k + j for k in range(i.size) if a + k in w.sites for j in range(4)}
        if w._target(i) is not None:
            skip |= set(range(i.imm_offset, i.imm_offset + i.imm_size))
        if len(mine) != len(orig) or any(mine[k] != orig[k] for k in range(len(orig)) if k not in skip):
            bad.add(a)
    return bad


def _stub(w, outdir, say):
    """The DOS program in front of the headers, from its own source (dos-source) when that
    comes back as the original's bytes; None (its bytes kept) when it does not, or when it is
    only the 128-byte "cannot be run in DOS mode" kind too small to be worth it."""
    d = w.d
    last, pages = struct.unpack_from("<HH", d, 2)
    size = (pages - 1) * 512 + (last or 512) if pages else 0
    if not 0x80 < size <= w.pe.e_lfanew:
        return None
    from codebreaker.source import dos_writer
    sd = os.path.join(outdir, "stub")
    os.makedirs(sd, exist_ok=True)
    carved = os.path.join(sd, "original", "STUB.EXE")
    os.makedirs(os.path.dirname(carved), exist_ok=True)
    with open(carved, "wb") as f:
        f.write(d[:size])
    try:
        r = dos_writer.reconstruct(carved, sd)
    except Exception as e:                                   # (a stub the DOS lane cannot take: its bytes)
        say("the stub: %s" % e)
        return None
    built = r.get("file")
    if built and r.get("file_same"):
        say("the stub: %d bytes, from its own source (%s)" % (size, r["source"]))
        return os.path.abspath(built)
    say("the stub does not come back from its source; its bytes are kept")
    return None


def reconstruct(path, outdir, stub=None, log=None):
    say = log or (lambda s: None)
    if not nasm():
        raise SystemExit("no assembler (settings.json: nasm)")
    outdir = os.path.abspath(outdir)
    os.makedirs(outdir, exist_ok=True)
    w = Writer(path)
    name = os.path.basename(path).lower()
    if stub is None:
        stub = _stub(w, outdir, say)
    stub = stub or None
    raw, rounds, built = set(), 0, None
    while rounds < _MAX_ROUNDS:
        rounds += 1
        src, lines_of = w.source(raw, mark_all=True, stub=stub)
        built, msg, mp = _assemble(src, outdir, name)
        if not built:
            bad = {lines_of.get(int(n) - 1) for n in re.findall(r":(\d+): error", msg)} - {None}
            say("round %d: %d instructions the assembler rejects" % (rounds, len(bad)))
            if not bad:
                raise SystemExit("assembler: " + msg[-2000:])
            raw |= bad
            continue
        bad = _differing(w, open(built, "rb").read(), _map(mp), raw)
        say("round %d: %d instructions come back different" % (rounds, len(bad)))
        if not bad:
            break
        raw |= bad
    src, _lines = w.source(raw, mark_all=False, stub=stub)
    built, msg, _mp = _assemble(src, outdir, name)
    same = bool(built) and open(built, "rb").read() == w.d
    if built and not same:
        a, b = open(built, "rb").read(), w.d
        k = next((j for j in range(min(len(a), len(b))) if a[j] != b[j]), min(len(a), len(b)))
        say("first difference at file offset 0x%x (sizes %d, %d)" % (k, len(a), len(b)))
    return {"source": os.path.join(outdir, name + ".asm"), "image": built, "same": same,
            "instructions": len(w.insns), "raw": len(raw), "rounds": rounds, "log": msg[-2000:]}


def main(argv):
    """python -m codebreaker pe-source <program> [--out DIR] [--stub FILE]"""
    args = [a for a in argv if not a.startswith("--")]
    for opt in ("--out", "--stub"):
        if opt in argv:
            args.remove(argv[argv.index(opt) + 1])
    if not args:
        print(__doc__)
        return 2
    path = args[0]
    outdir = argv[argv.index("--out") + 1] if "--out" in argv else \
        settings.state("pe_source", os.path.splitext(os.path.basename(path))[0].lower())
    stub = os.path.abspath(argv[argv.index("--stub") + 1]) if "--stub" in argv else None
    stub = False if "--keep-stub" in argv else stub
    r = reconstruct(path, outdir, stub=stub, log=print)
    n = max(1, r["instructions"])
    print("%s: %d instructions, %d (%d%%) written as bytes; %d round%s; %s" % (
        os.path.basename(path), r["instructions"], r["raw"], 100 * r["raw"] // n, r["rounds"],
        "" if r["rounds"] == 1 else "s", "builds back THE SAME" if r["same"] else "built, differs" if r["image"] else "not built"))
    print("  source: %s" % r["source"])
    return 0 if r["same"] else 1
