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
_MAX_ROUNDS = 200


class Writer(object):
    def __init__(self, program):
        self.p = p = program
        self.img = bytes(p.image)
        self.origin = 0x100 if p.kind == "com" else 0        # a .COM file sits behind its program prefix
        self.rel = {a - p.base for a in p.relocations}
        load = dos.LOAD
        segs = {(struct.unpack_from("<H", self.img, a)[0] - load) & 0xFFFF for a in self.rel if a + 2 <= len(self.img)}
        # a program with a header of its own (a driver may be linked as one too), or bytes as they load
        self.flat = not (p.kind != "com" and p.spans and p.spans[0][2] == "program")
        self.stack_in = None                                 # the stack's segment, when it is in the load module
        if not self.flat:
            segs |= {(p.cs - load) & 0xFFFF}
            if p.sp and ((p.ss - load) & 0xFFFF) < (len(self.img) + 15) // 16:
                self.stack_in = (p.ss - load) & 0xFFFF
                segs |= {self.stack_in}
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
        self.crossing = set()            # jumps across a place the program was divided at: written as bytes
        self._decode()
        self._divide()

    def _divide(self):
        """A stretch of the program no relocated word divides can be longer than a segment may
        be (COMMAND.COM's code is one such stretch of 75K): the original's segments were set up
        by the program itself, at run time. For the assembler it is divided where it can be --
        at a paragraph no jump or call reaches across and no instruction lies over -- which
        changes no byte: everything inside refers to its own half by label or by number."""
        img_end = len(self.img)
        reach = [(min(at, t[3]), max(at, t[3])) for at, t in self.ins.items() if t[3] is not None]
        inside = {x for at, t in self.ins.items() for x in range(at + 1, at + len(t[0]))}
        again = True
        while again:
            again = False
            for k, s in enumerate(self.segments):
                lo = s * 16
                hi = self.segments[k + 1] * 16 if k + 1 < len(self.segments) else img_end
                if hi - lo <= 0xFFF0:
                    continue
                cut, fewest = None, None
                for b in range((lo + 0xFFF0) & ~15, lo + 0x1000, -16):
                    if b in inside or b in self.rel or b - 1 in self.rel:
                        continue
                    n = sum(1 for a, z in reach if a < b <= z and lo <= a and z < hi)
                    if fewest is None or n < fewest:
                        cut, fewest = b, n
                    if not n:
                        break
                if cut is None:
                    return                                   # it cannot be divided: left as it is
                if fewest:
                    # every place is jumped across (the DOS kernel's code runs in a segment of its
                    # own at run time): the few jumps across the place chosen are written as bytes
                    for at, t in list(self.ins.items()):
                        if t[3] is not None and min(at, t[3]) < cut <= max(at, t[3]):
                            self.ins[at] = t[:3] + (None,) + t[4:]
                            self.crossing.add(at)
                    reach = [(min(at, t[3]), max(at, t[3])) for at, t in self.ins.items() if t[3] is not None]
                self.segments = sorted(set(self.segments) | {cut // 16})
                again = True
                break

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
        if at in self.crossing:
            return None
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
        done = 0                                             # how far the segments before have got
        for k, s in enumerate(self.segments):
            lo = s * 16
            hi = self.segments[k + 1] * 16 if k + 1 < len(self.segments) else len(img)
            # The original's linker packed segments end to end, so the last instruction of one
            # may run a few bytes into the paragraph the next is named by. That next segment then
            # starts where the one before stopped (byte-aligned), and its offsets count from its
            # paragraph all the same.
            over = max(done, lo) - lo
            align = "byte" if over else "para"
            if s == self.stack_in and hi == len(img):        # the stack, in the program's last segment
                out.append("S%04X segment %s stack use16 'STACK'" % (s, align))
            else:
                out.append("S%04X segment %s public use16 'C%04X'" % (s, align, s))
            out.append("        assume cs:S%04X, ds:nothing, es:nothing, ss:nothing" % s)
            if self.origin and s == 0:
                out.append("        org  %d" % self.origin)
            at, run = max(lo + over, self.origin if s == 0 else 0), []      # (DOS supplies what is before a .COM file's origin)

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
            done = at
            if hi in self.labels and hi == len(img):
                out.append("%s:" % self.label(hi))
            if s == self.stack_in and hi == len(img) and p.sp > hi - lo:
                out.append("        db   %d dup (?)" % (p.sp - (hi - lo)))      # the stack's room behind the file's bytes
            out += ["S%04X ends" % s, ""]
        if not self.flat:
            stack = (p.ss - dos.LOAD) & 0xFFFF
            if stack >= self.top:                            # the stack lies behind the load module
                if stack > self.top:
                    out += ["GAP segment para public use16 'GAP'", "        db   %d dup (?)" % ((stack - self.top) * 16),
                            "GAP ends"]
                out += ["STK segment para stack use16 'STACK'", "        db   %d dup (?)" % p.sp, "STK ends", ""]
            # (an entry point outside the load module -- COMMAND.COM's is in its program prefix --
            # has no label to name; it is set in the file's header after linking)
            out.append("end %s" % self.label(self.entry) if 0 <= self.entry < len(img) else "end")
        elif p.kind == "com" and self.origin in self.labels:
            out.append("end %s" % self.label(self.origin))       # where it starts: the linker leaves out what is before
        else:
            out.append("end")
        return "\n".join(out) + "\n", counts

    def pieces(self, good):
        """The image as the pieces the source is made of, for assembly.culprits: each
        instruction written as its mnemonic, each word that holds a segment (whatever it holds
        once built), and the runs of bytes between."""
        out, at, start, n = [], self.origin, self.origin, len(self.img)

        def data(end):
            if end > start:
                out.append((start - self.origin, end - start, None, None, False))
        while at < n:
            if at in self.ins:
                size = len(self.ins[at][0])
                data(at)
                where = self.ins[at][4]
                mask = None
                if where is not None:
                    mask = bytearray(size)
                    mask[where - at:where - at + 2] = b"\1\1"
                    mask = bytes(mask)
                out.append((at - self.origin, size, mask, at if at in good else None, self.ins[at][3] is not None))
                at += size
                start = at
            elif at in self.rel:
                data(at)
                out.append((at - self.origin, 2, b"\1\1", None, False))
                at += 2
                start = at
            else:
                at += 1
        data(n)
        return out

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
            return ["format raw bin", "option quiet, nodefaultlibs", "file %s" % obj]
        # the stack's size is said outright: the linker would give a small one 4096 bytes
        stack = ", stack=%d" % self.p.sp if self.p.sp else ""
        return ["format dos", "option quiet, nodefaultlibs, packcode=0, packdata=0" + stack, "file %s" % obj]


def _load_module(path, kind):
    """(bytes as loaded, relocation offsets, (cs, ip, ss, sp)) of a built file."""
    if kind:
        # read plainly, as stored: the built file IS the program as it runs. (Read the way an
        # original is, a program that moves or unpacks itself at start would be undone twice.)
        with open(path, "rb") as f:
            data = f.read()
        h = dos._mz(data)
        img = bytearray(data[h["header"]:h["size"]])
        relocs = set()
        for i in range(h["nreloc"]):
            off, seg = struct.unpack_from("<HH", data, h["reloc_off"] + 4 * i)
            at = seg * 16 + off
            if at + 2 <= len(img):
                relocs.add(at)
                struct.pack_into("<H", img, at, (struct.unpack_from("<H", img, at)[0] + dos.LOAD) & 0xFFFF)
        return bytes(img), relocs, ((dos.LOAD + h["cs"]) & 0xFFFF, h["ip"], (dos.LOAD + h["ss"]) & 0xFFFF, h["sp"])
    with open(path, "rb") as f:
        return f.read(), set(), None


def reconstruct(module_path, outdir, log=None):
    """Write a program's source, build it, and make it come back as the original (see the top).
    -> a dict: source, image, linked, same, notes, instructions, raw, data, rounds, functions."""
    if dos.kind(module_path) == "kernel":
        return _kernel(module_path, outdir, log)
    p = dos.read(module_path, run_movers=False)         # moving itself is the program's own code
    return _program(p, outdir, re.sub(r"\W", "_", os.path.splitext(p.name)[0]), log)


def _fills_text(layout):
    return "".join(["; the stretches of SYSINIT that LINK /EXEPACK stored as fills: its iterated data\n",
                    "; (`dup`s), as offset length byte, in hex\n"] +
                   ["%05X %04X %02X\n" % f for f in layout.fills])


def _read_fills(path):
    with open(path) as f:
        return [tuple(int(x, 16) for x in line.split()) for line in f if line.strip() and not line.startswith(";")]


def _kernel(module_path, outdir, log=None):
    """IO.SYS: each of its programs written, built and compared as any other (one source file
    each); SYSINIT then packed as LINK /EXEPACK stored it (build/exepack.py, the fills listed
    in sysinit.pack), and loader + kernel + packed SYSINIT joined into the file."""
    from codebreaker.build import exepack
    with open(module_path, "rb") as f:
        original = f.read()
    result = {"source": [], "image": None, "linked": True, "same": False, "rounds": 0, "notes": [], "kind": "kernel",
              "instructions": 0, "raw": 0, "data": 0, "functions": {}, "parts": {}}
    os.makedirs(outdir, exist_ok=True)
    parts, layout = dos.kernel_parts(module_path)
    fills = os.path.join(outdir, "sysinit.pack")
    if not os.path.isfile(fills):
        with open(fills, "w") as f:
            f.write(_fills_text(layout))
    built = {}
    for stem, p in parts:
        r = _program(p, outdir, stem, log)
        result["parts"][stem] = r
        result["source"].append(r["source"])
        for k in ("instructions", "raw", "data"):
            result[k] += r.get(k, 0)
        result["rounds"] = max(result["rounds"], r["rounds"])
        if log:
            log("  %s: %d instructions, %d as bytes; %s" % (stem, r.get("instructions", 0), r.get("raw", 0),
                "builds back the same" if r["same"] else "; ".join(r["notes"]) or "differs"))
        if not r["linked"]:
            result["linked"] = False
            result["notes"].append("%s: %s" % (stem, "; ".join(r["notes"]) or "not built"))
            continue
        built[stem] = r["image"]
    if not result["linked"]:
        return result
    with open(built["exepack"], "rb") as f:
        stub_code = f.read()
    with open(built["sysinit"], "rb") as f:
        exe = f.read()
    h = dos._mz(exe)
    image = exe[h["header"]:h["size"]]
    relocs = [struct.unpack_from("<HH", exe, h["reloc_off"] + 4 * i) for i in range(h["nreloc"])]
    packed, (cs, ip, _ss, _sp) = exepack.pack(image, [seg * 16 + off for off, seg in relocs], (h["cs"], h["ip"]),
                                              (h["ss"], h["sp"]), _read_fills(fills), stub_code, layout.words)
    with open(built["msload"], "rb") as f:
        loader = bytearray(f.read())
    with open(built["msdos"], "rb") as f:
        kernel = f.read()
    # the header the loader's first bytes hold: what it says must be what was built
    head = dos._mz(bytes(loader) + kernel + packed)
    want = {"header": len(loader) + len(kernel), "size": len(loader) + len(kernel) + len(packed), "cs": cs, "ip": ip,
            "nreloc": 0}
    wrong = {k: (head[k], v) for k, v in want.items() if head[k] != v}
    if wrong:
        result["notes"].append("the MZ header in msload does not describe what was built: %s" % ", ".join(
            "%s %#x, built %#x" % (k, a, b) for k, (a, b) in sorted(wrong.items())))
    joined = bytes(loader) + kernel + packed
    out = os.path.join(outdir, os.path.basename(module_path).lower())
    with open(out, "wb") as f:
        f.write(joined)
    result["image"] = out
    result["same"] = joined == original and not wrong
    if not result["same"]:
        n = min(len(joined), len(original))
        first = next((a for a in range(n) if joined[a] != original[a]), n)
        result["notes"].append("the joined file differs from the original at %#x (%d bytes, original %d)" % (
            first, len(joined), len(original)))
        # what the difference is: the same programs, relocation table in another order?
        oh = dos._mz(original)
        if joined[:oh["header"]] == original[:oh["header"]] and len(joined) == len(original):
            mine = dos.unpack_exepack(packed, cs, ip)
            theirs = dos.unpack_exepack(original[oh["header"]:oh["size"]], oh["cs"], oh["ip"])
            if mine[0] == theirs[0] and mine[2:] == theirs[2:] and sorted(mine[1]) == sorted(theirs[1]):
                result["notes"].append("loader, kernel and SYSINIT are the original's, byte for byte, unpacked; "
                                       "only the order of SYSINIT's relocation table differs (LINK listed them "
                                       "in its fixups' order)")
                result["equivalent"] = True
    return result


def _beside(path, make):
    """A file kept beside the source: written from `make()` the first time, read after that
    (so an edited one is what is built). -> its bytes"""
    if not os.path.isfile(path):
        with open(path, "wb") as f:
            f.write(make())
    with open(path, "rb") as f:
        return f.read()


def _as_file(p, outdir, stem, linked, result, log=None):
    """The built program as a file laid out as the original is (build/mzfile.py): its header's
    own data (<stem>.hdr), the load module -- packed as LINK /EXEPACK stored it when the
    original is (build/exepack.py: the fills in <stem>.pack, the unpacker from exepack.asm) --
    and the bytes behind the load module (<stem>.ovl). result["file"], result["file_same"]."""
    from codebreaker.build import exepack, mzfile
    with open(p.path, "rb") as f:
        data = f.read()
    oh = dos._mz(data)
    with open(linked, "rb") as f:
        exe = f.read()
    h = dos._mz(exe)
    module = exe[h["header"]:h["size"]]
    relocs = [struct.unpack_from("<HH", exe, h["reloc_off"] + 4 * i) for i in range(h["nreloc"])]
    # the memory the program needs beyond its load module: as far as its stack reaches (the
    # linker's own figure goes wrong -- FFFF -- for a stack that lies inside the file)
    beyond = max(0, (h["ss"] * 16 + h["sp"] - len(module) + 15) // 16)
    os.makedirs(os.path.join(outdir, "file"), exist_ok=True)
    out = os.path.join(outdir, "file", os.path.basename(p.path).upper())
    if p.packer not in (None, "EXEPACK", "moves itself"):
        # stored by a packer this does not write (PKLITE ...): the program as linked
        with open(out, "wb") as f:
            f.write(exe + p.overlay)
        result.update(file=out, file_same=False)
        result.setdefault("remarks", []).append("the original is stored packed by %s; the file is the program "
                                                "unpacked" % p.packer)
        return
    tpl = _beside(os.path.join(outdir, stem + ".hdr"), lambda: mzfile.template(data[:oh["header"]]))
    overlay = _beside(os.path.join(outdir, stem + ".ovl"), lambda: p.overlay) if p.overlay else b""
    try:
        if p.packer == "EXEPACK":
            layout = exepack.layout(data[oh["header"]:oh["size"]], oh["cs"], oh["ip"])
            fills = os.path.join(outdir, stem + ".pack")
            _beside(fills, lambda: _fills_text(layout).replace("SYSINIT", p.name).encode())
            stub_dir = os.path.join(outdir, "exepack")
            stub_bin = os.path.join(stub_dir, "exepack.bin")
            if not os.path.isfile(stub_bin):
                r = _program(dos.exepack_stub(p.path, layout), stub_dir, "exepack", log)
                if not r["same"]:
                    raise ValueError("the EXEPACK unpacker does not build back")
            with open(stub_bin, "rb") as f:
                stub_code = f.read()
            packed, (cs, ip, ss, sp) = exepack.pack(module, [seg * 16 + off for off, seg in relocs],
                                                    (h["cs"], h["ip"]), (h["ss"], h["sp"]), _read_fills(fills),
                                                    stub_code, layout.words)
            # the memory it asks for beyond the stored module: the unpacked program and its stack
            # (EXEPACK counted both down to whole paragraphs), or the unpacker's copy and stack
            # behind it (counted from the stored module rounded up) -- whichever is more
            program = (len(module) + 15) // 16 + max(0, h["ss"] * 16 + h["sp"] - len(module)) // 16 - len(packed) // 16
            unpacker = ss + sp // 16 - (len(packed) + 15) // 16
            body = mzfile.compose(tpl, packed, [], cs, ip, ss, sp, max(program, unpacker))
        else:
            body = mzfile.compose(tpl, module, relocs, h["cs"], h["ip"], h["ss"], h["sp"], beyond)
    except ValueError as e:
        result.setdefault("notes", []).append("not laid out as the original file: %s" % e)
        body = exe
    with open(out, "wb") as f:
        f.write(body + overlay)
    result.update(file=out, file_same=body + overlay == data)
    if not result["file_same"]:
        n = min(len(body + overlay), len(data))
        first = next((a for a in range(n) if (body + overlay)[a] != data[a]), n)
        result.setdefault("remarks", []).append("the file differs from the original's at %#x (%d bytes, original %d)" % (
            first, len(body + overlay), len(data)))


def _program(p, outdir, stem, log=None):
    """One real-mode program: its source written, assembled, linked and compared, round after
    round, until it builds back as the original (see the top)."""
    result = {"source": None, "image": None, "linked": False, "same": False, "rounds": 0, "notes": [], "kind": p.kind}
    w = Writer(p)
    result["functions"] = {f.address - p.base: f for f in w.analysis.functions.values()}
    os.makedirs(outdir, exist_ok=True)
    src = os.path.join(outdir, stem + ".asm")
    out = os.path.join(outdir, stem + (".com" if p.kind == "com" else ".exe" if not w.flat else ".bin"))
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
            # a jump to a place that is not the start of anything written (the middle of a word
            # that holds a segment, say) has no label to go to: it stays bytes
            lost = set(re.findall(r"Symbol (\w+) is not defined", messages))
            drop |= {at for at in good if w.ins[at][3] is not None and w.label(w.ins[at][3]) in lost}
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
        built, relocs, regs = _load_module(image, not w.flat)
        n = min(len(built), len(original))
        first = next((a for a in range(n) if built[a] != original[a]), None)
        if first is None and len(built) == len(original):
            notes = []
            if not w.flat:
                if relocs != w.rel:
                    notes.append("the relocation table differs (%d entries, original %d)" % (len(relocs), len(w.rel)))
                if regs != (p.cs, p.ip, p.ss, p.sp) and relocs == w.rel:
                    # Where a program starts and where its stack is are four words of the file's
                    # header. A linker fills them in from the source's entry label and stack
                    # segment, and cannot be told a stack that lies inside the program or has
                    # no size, or an entry before the program's first byte; the code and data
                    # being the original's, the four words are written as the original has them.
                    with open(image, "r+b") as f:
                        f.seek(0x0E)
                        f.write(struct.pack("<HH", (p.ss - dos.LOAD) & 0xFFFF, p.sp))
                        f.seek(0x14)
                        f.write(struct.pack("<HH", p.ip, (p.cs - dos.LOAD) & 0xFFFF))
                    _b, _r, regs = _load_module(image, True)
                    result.setdefault("remarks", []).append("entry and stack set in the file's header after linking")
                if regs != (p.cs, p.ip, p.ss, p.sp):
                    notes.append("entry or stack differ: %04x:%04x %04x:%04x, original %04x:%04x %04x:%04x" % (
                        regs + (p.cs, p.ip, p.ss, p.sp)))
            result.update(same=not notes, notes=notes)
            if not w.flat and result["same"] and not getattr(p, "in_kernel", False):
                _as_file(p, outdir, stem, image, result, log)
            break
        # which instructions did not come back as they were -- found instruction by instruction,
        # so one that came back a byte longer is blamed itself and not everything behind it
        blamed = assembly.culprits(bytes(original), bytes(built), w.pieces(good)) & good
        if blamed:
            good -= blamed
            if log:
                log("  round %d: %d instructions did not come back as they were; they go back to bytes" % (
                    rounds, len(blamed)))
            continue
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
        "builds back THE SAME" if r["same"] else "builds back the same program (see below)" if r.get("equivalent")
        else "built, differs" if r["linked"] else "not built"))
    for note in r.get("notes") or ():
        print("  " + note)
    for s in r["source"] if isinstance(r["source"], list) else [r["source"]]:
        print("  source: %s" % s)
    return 0 if r["same"] or r.get("equivalent") else 1
