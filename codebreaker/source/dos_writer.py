"""dos_writer.py -- a real-mode program as assembly source that builds back into it.

    python -m codebreaker dos-source <program> [--out DIR]

    from codebreaker.source import dos_writer
    r = dos_writer.reconstruct(path, outdir)
    r["source"], r["image"], r["same"], r["notes"], r["instructions"], r["raw"], r["rounds"]

The real-mode base of the system was written in assembly, so assembly is what its source is.
The file has one segment for every segment the program itself names (each word its relocation
table marks holds one, and so do the entry point and the stack), and every such word is
written as the segment it names (`dw seg S0605`), so the program can be assembled and linked
at any address. Code is what analyze/dos_functions.py found by following the program; the rest
is written as data, text as text. Interrupt calls carry the name of the service.

What is NOT recovered, because a real-mode image does not state it: which data an offset
inside a segment refers to. `mov ax, ds:[1234h]` stays a number, so data cannot be moved within
its segment without changing those by hand.

An instruction is written as its mnemonic only when the assembler turns that text back into the
original's bytes (source/assembly.py checks each one); the file is then assembled and linked
and the result compared with the original: every byte of the load module, the relocation
table, the entry point and the stack. Kinds of file: MZ programs (unpacked first when the file
stores them packed -- the rebuilt program is the unpacked one), .COM files, and device drivers
that are plain images.
"""
import os
import re
import struct

import capstone

from codebreaker.analyze import dos_functions
from codebreaker.base import settings
from codebreaker.formats import dos
from codebreaker.source import assembly
from codebreaker.symbols import dos_interrupts

_MARK = 0x7AB3                           # stands in for a relocated segment word while decoding
_MAX_ROUNDS = 30


class Writer(object):
    def __init__(self, program):
        self.p = p = program
        self.img = bytes(p.image)
        self.origin = 0x100 if p.kind == "com" else 0        # a .COM file sits behind its program prefix
        self.rel = {a - p.base for a in p.relocations}
        load = dos.LOAD
        segs = {(struct.unpack_from("<H", self.img, a)[0] - load) & 0xFFFF for a in self.rel if a + 2 <= len(self.img)}
        self.flat = p.kind != "mz"
        if not self.flat:
            segs |= {(p.cs - load) & 0xFFFF}
        self.top = (len(self.img) + 15) // 16
        self.segments = sorted({s for s in segs if s < self.top} | {0})
        self.analysis = a = dos_functions.discover(p)
        self.names = {f.address - p.base: f.name for f in a.functions.values()}
        self.entry = ((((p.cs - load) & 0xFFFF) << 4) + p.ip) if not self.flat else None
        self.labels = set(self.names)
        if self.entry is not None:
            self.labels.add(self.entry)
        self.ins = {}                    # offset in the image -> (bytes, mnemonic, operands, target, relocated word at)
        self.calls = {}                  # offset of an `int` -> its name
        for i in a.interrupts:
            s = dos_interrupts.name(i.number, i.ah, i.al)
            self.calls[i.address - p.base] = s.name + ("(%s)" % s.arguments if s.arguments and s.exact else "")
        self._decode()

    def segment_of(self, at):
        best = 0
        for s in self.segments:
            if s * 16 <= at:
                best = s
        return best

    def label(self, at):
        return self.names.get(at) or "L_%05X" % at

    def _decode(self):
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
        md.detail = True
        marks = self.analysis._mark
        img = self.img
        for at in range(len(img)):
            if marks[at] != 1:
                continue
            window = bytearray(img[at:at + 16])
            where = None
            for x in range(at, min(at + 15, len(img))):
                if x in self.rel:
                    struct.pack_into("<H", window, x - at, _MARK)
                    where = x
                    break
            ins = next(md.disasm(bytes(window), at & 0xFFFF), None)
            if ins is None or at + ins.size > len(img):
                continue
            if where is not None and not (at <= where and where + 2 <= at + ins.size):
                where = None
            if any(x in self.rel for x in range(at, at + ins.size) if x != where):
                continue                                     # more than one relocated word: written as bytes
            target = None
            groups = set(ins.groups)
            if (capstone.x86.X86_GRP_JUMP in groups or capstone.x86.X86_GRP_CALL in groups) and len(ins.operands) == 1 \
                    and ins.operands[0].type == capstone.x86.X86_OP_IMM and where is None and \
                    ins.bytes[0] not in (0x9A, 0xEA):
                delta = (ins.operands[0].imm - ((at + ins.size) & 0xFFFF)) & 0xFFFF
                if delta & 0x8000:
                    delta -= 0x10000
                t = at + ins.size + delta
                if 0 <= t < len(img) and self.segment_of(t) == self.segment_of(at):
                    target = t
                    self.labels.add(t)
            self.ins[at] = (img[at:at + ins.size], ins.mnemonic, ins.op_str, target, where)
        # an instruction something jumps into the middle of is not written as one
        for at in sorted(self.ins):
            n = len(self.ins[at][0])
            if any(x in self.labels for x in range(at + 1, at + n)):
                del self.ins[at]
        again = True
        while again:
            again = False
            inside = {x for at in self.ins for x in range(at + 1, at + len(self.ins[at][0]))}
            for at in list(self.ins):
                if self.ins[at][3] is not None and self.ins[at][3] in inside:
                    del self.ins[at]
                    again = True

    # -- text ----------------------------------------------------------------------------------
    def seg_name(self, value):
        """A relocated word's value (relative to where the program is loaded) as an expression."""
        base = max([s for s in self.segments if s <= value] or [0])
        return "seg S%04X%s" % (base, " + %d" % (value - base) if value != base else "")

    def _seg_at(self, at):
        return self.seg_name((struct.unpack_from("<H", self.img, at)[0] - dos.LOAD) & 0xFFFF)

    def mnemonic(self, at):
        data, mnem, ops, target, where = self.ins[at]
        m = assembly.STRING_OPS.match(mnem + " ")
        if m and where is None and ":" not in ops.replace("es:[di]", ""):
            return ((m.group(1) or "") + m.group(2) + m.group(3)).strip()
        if target is not None:
            if mnem in ("loop", "loope", "loopne", "jcxz", "jecxz"):
                return "%-6s %s" % (mnem, self.label(target))
            return "%-6s %s %s" % (mnem, "short" if len(data) == 2 else "near ptr", self.label(target))
        o = ops
        if where is not None:
            marker = "0x%x" % _MARK
            if marker not in o:
                return None
            o = o.replace(marker, self._seg_at(where))
        # a bare number in brackets is an address in the data segment, which the assembler
        # reads as a number unless the segment is said
        o = re.sub(r"(?<![a-z]:)\[(0x[0-9a-f]+)\]", r"ds:[\1]", o)
        return ("%-6s %s" % (assembly.mnemonic(mnem), assembly.operands(o))).rstrip()

    def _raw(self, at):
        data, mnem, ops, _target, _where = self.ins[at]
        lines, run = [], []

        def flush():
            if run:
                lines.append("        db   " + ", ".join("0%02Xh" % b for b in run))
                del run[:]
        x = at
        while x < at + len(data):
            if x in self.rel:
                flush()
                lines.append("        dw   " + self._seg_at(x))
                x += 2
            else:
                run.append(data[x - at])
                x += 1
        flush()
        text = self.mnemonic(at) or ("%s %s" % (mnem, ops)).strip()
        if lines:
            lines[0] = "%-44s ; %s" % (lines[0], text)
        return lines

    def source(self, good=None):
        p = self.p
        out = ["; %s -- reconstructed from the program image by CodeBreaker." % p.name, ";",
               "; One segment for each segment the program names. A word holding a segment is written as",
               "; that segment, so the program can be loaded anywhere; offsets inside a segment are the",
               "; original's numbers. Labels are L_<position in the load module>. Instructions written as",
               "; bytes are ones the assembler would encode differently; their mnemonic is in the comment.",
               "", ".386p", ""]
        counts = {"instructions": 0, "raw": 0, "data": 0}
        img = self.img
        for k, s in enumerate(self.segments):
            lo = s * 16
            hi = self.segments[k + 1] * 16 if k + 1 < len(self.segments) else len(img)
            out.append("S%04X segment para public use16 'C%04X'" % (s, s))
            out.append("        assume cs:S%04X, ds:nothing, es:nothing, ss:nothing" % s)
            if self.origin and s == 0:
                out.append("        org  %d" % self.origin)
            at, run = lo, []

            def flush():
                out.extend(assembly.data_lines(run))
                del run[:]
            while at < hi:
                if at in self.labels:
                    flush()
                    out.append("%s:" % self.label(at))
                if at in self.ins:
                    flush()
                    text = self.mnemonic(at) if (good is None or at in good) else None
                    counts["instructions"] += 1
                    note = self.calls.get(at)
                    if text is None:
                        counts["raw"] += 1
                        out += self._raw(at)
                    else:
                        out.append("        " + text + ("    ; " + note if note else ""))
                    at += len(self.ins[at][0])
                elif at in self.rel:
                    flush()
                    out.append("        dw   " + self._seg_at(at))
                    at += 2
                else:
                    run.append(img[at])
                    counts["data"] += 1
                    at += 1
            flush()
            if hi in self.labels and hi == len(img):
                out.append("%s:" % self.label(hi))
            out += ["S%04X ends" % s, ""]
        if not self.flat:
            stack = (p.ss - dos.LOAD) & 0xFFFF
            if stack >= self.top:                            # the stack lies behind the load module
                if stack > self.top:
                    out += ["GAP segment para public use16 'GAP'", "        db   %d dup (?)" % ((stack - self.top) * 16),
                            "GAP ends"]
                out += ["STK segment para stack use16 'STACK'", "        db   %d dup (?)" % p.sp, "STK ends", ""]
            out.append("end %s" % self.label(self.entry))
        else:
            out.append("end")
        return "\n".join(out) + "\n", counts

    def candidates(self):
        """{offset: (text, bytes outside the relocated word, mask)} of what can be probed."""
        out = {}
        for at, (data, _m, _o, target, where) in self.ins.items():
            if target is not None:
                continue
            text = self.mnemonic(at)
            if text is None:
                continue
            mask = bytearray(len(data))
            if where is not None:
                mask[where - at:where - at + 2] = b"\1\1"
            out[at] = (text, bytes(b for b, k in zip(data, mask) if not k), bytes(mask))
        return out

    def link_lines(self, obj):
        if self.p.kind == "com":
            return ["format dos com", "option quiet, nodefaultlibs", "file %s" % obj]
        if self.flat:
            return ["format raw bin", "option quiet, nodefaultlibs, nostack", "file %s" % obj]
        return ["format dos", "option quiet, nodefaultlibs, packcode=0, packdata=0", "file %s" % obj]


def _load_module(path, kind):
    """(bytes as loaded, relocation offsets, (cs, ip, ss, sp)) of a built file."""
    if kind == "mz":
        q = dos.read(path)
        return bytes(q.image), {a - q.base for a in q.relocations}, (q.cs, q.ip, q.ss, q.sp)
    with open(path, "rb") as f:
        return f.read(), set(), None


def reconstruct(module_path, outdir, log=None):
    """Write a program's source, build it, and make it come back as the original (see the top).
    -> a dict: source, image, linked, same, notes, instructions, raw, data, rounds, functions."""
    p = dos.read(module_path)
    result = {"source": None, "image": None, "linked": False, "same": False, "rounds": 0, "notes": [], "kind": p.kind}
    if p.kind == "kernel":
        result["notes"] = ["a three-part kernel image: no source is written for it yet"]
        return result
    w = Writer(p)
    result["functions"] = {f.address - p.base: f for f in w.analysis.functions.values()}
    os.makedirs(outdir, exist_ok=True)
    stem = re.sub(r"\W", "_", os.path.splitext(p.name)[0])
    src = os.path.join(outdir, stem + ".asm")
    out = os.path.join(outdir, stem + (".com" if p.kind == "com" else ".exe" if p.kind == "mz" else ".bin"))
    cand = w.candidates()
    ok = assembly.probe(list(cand.values()), {"S%04X" % s for s in w.segments}, outdir, mode="real16")
    good = {at for at, c in cand.items() if c in ok} | {at for at, t in w.ins.items() if t[3] is not None}
    original = w.img[w.origin:] if w.flat else w.img
    result["source"] = src
    for rounds in range(1, _MAX_ROUNDS + 1):
        text, counts = w.source(good)
        with open(src, "w", newline="\r\n", encoding="latin1") as f:
            f.write(text)
        result.update(rounds=rounds, instructions=counts["instructions"], raw=counts["raw"], data=counts["data"])
        obj, messages = assembly.assemble(src, outdir)
        if not obj:
            lines = text.split("\n")
            failed = {lines[int(m.group(1)) - 1].split("    ;")[0].strip()
                      for m in re.finditer(r"\((\d+)\)\s*:\s*Error", messages) if int(m.group(1)) <= len(lines)}
            drop = {at for at in good if w.mnemonic(at) in failed}
            if not drop:
                result["notes"] = ["assembler: " + (messages.strip().splitlines() or ["failed"])[-1][:200]]
                break
            good -= drop
            continue
        image, messages = assembly.link(w.link_lines(obj), out, outdir)
        if not image:
            result["notes"] = ["linker: " + (messages.strip().splitlines() or ["failed"])[-1][:200]]
            break
        result.update(image=image, linked=True)
        built, relocs, regs = _load_module(image, p.kind)
        n = min(len(built), len(original))
        first = next((a for a in range(n) if built[a] != original[a]), None)
        if first is None and len(built) == len(original):
            notes = []
            if p.kind == "mz":
                if relocs != w.rel:
                    notes.append("the relocation table differs (%d entries, original %d)" % (len(relocs), len(w.rel)))
                if regs != (p.cs, p.ip, p.ss, p.sp):
                    notes.append("entry or stack differ: %04x:%04x %04x:%04x, original %04x:%04x %04x:%04x" % (
                        regs + (p.cs, p.ip, p.ss, p.sp)))
            result.update(same=not notes, notes=notes)
            break
        at = (first if first is not None else n) + w.origin
        before = [a for a in good if a <= at]
        hit = [a for a in before if a <= at < a + len(w.ins[a][0])] or ([max(before)] if before else [])
        if not hit:
            result["notes"] = ["the built program differs at %#x and no instruction accounts for it" % at]
            break
        good -= set(hit)
        if log:
            log("  round %d: differs at %#x; %s goes back to bytes" % (rounds, at, w.mnemonic(hit[0])))
    return result


def main(argv):
    """python -m codebreaker dos-source <program> [--out DIR]

    Write a real-mode program's assembly source from its image, build it and compare the build
    with the original."""
    from codebreaker.base import corpus
    args = [a for a in argv if not a.startswith("--")]
    if not args:
        print(main.__doc__)
        return 2
    path = args[0] if os.path.isfile(args[0]) else corpus.retail(args[0])
    if not path:
        raise SystemExit("no program %r" % args[0])
    outdir = argv[argv.index("--out") + 1] if "--out" in argv else \
        settings.state("dos_source", os.path.splitext(os.path.basename(path))[0].lower())
    r = reconstruct(path, outdir, log=print)
    n = max(1, r.get("instructions", 0))
    print("%s: %d instructions, %d (%d%%) written as bytes; %d bytes of data; %d round%s; %s" % (
        os.path.basename(path), r.get("instructions", 0), r.get("raw", 0), 100 * r.get("raw", 0) // n,
        r.get("data", 0), r["rounds"], "" if r["rounds"] == 1 else "s",
        "builds back THE SAME" if r["same"] else "built, differs" if r["linked"] else "not built"))
    for note in r.get("notes") or ():
        print("  " + note)
    print("  source: %s" % r["source"])
    return 0 if r["same"] else 1
