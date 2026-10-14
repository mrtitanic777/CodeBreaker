"""driver_writer.py -- a virtual device driver as assembly source that builds back into it.

    python -m codebreaker driver-source <driver> [--out DIR]

    from codebreaker.source import driver_writer
    r = driver_writer.reconstruct(path, outdir)
    r["source"], r["image"], r["checks"], r["same"], r["raw"], r["instructions"], r["rounds"]

A driver was written in assembly, so assembly is what its source is. What makes assembly of a
driver SOURCE rather than a listing is that nothing in it is an address: every place the loader
patches (a pointer to a variable, a call into another segment, an entry of a jump table) is
written as the label it refers to, so the file can be assembled, linked at any address and
changed. The image says exactly where those places are (its fix-up records), so nothing about
them is guessed.

The file has one segment per object of the original, with the attributes that object had.
Code is found by following it from the driver's entry points (control procedure, services, API
procedures), from every place a call or jump across objects lands, and from pointers into code;
everything not reached is written as data. Calls to the system and other drivers (`int 20h`
followed by the service number) carry the service's name.

An instruction is written as its mnemonic only when the assembler turns that text back into
the bytes the original has -- which is checked, not assumed (assemblers choose between
equivalent encodings): every distinct instruction is first assembled on its own in a probe file
and its bytes read back from the object file; one that comes back different is written as its
bytes, with the mnemonic beside it. Then the whole file is assembled and linked and each object
compared with the original outside the patched places; an instruction at a place that still
differs is written as bytes too, until nothing differs.
"""
import os
import re
import struct

import capstone

from codebreaker.base import settings
from codebreaker.build import driver as builder
from codebreaker.build import vxdfile
from codebreaker.formats import le
from codebreaker.source import assembly
from codebreaker.symbols import driver_services

_MARK = 0x7A000000                       # stands in for a patched address while decoding
_MAX_ROUNDS = 40
# opcodes common enough that one bad encoding does not mean every instance is bad; a rarer one
# the assembler rejects (a privileged or system instruction) is rejected wherever it appears, so
# all instances of it are written as bytes together
_COMMON = {"mov", "add", "sub", "cmp", "lea", "push", "pop", "call", "jmp", "test", "and", "or",
           "xor", "inc", "dec", "ret", "nop", "mov zx", "movzx", "movsx", "shl", "shr", "adc", "sbb"}


def _attributes(flags):
    words = ["preload" if flags & 0x40 else "loadoncall"]
    words += [w for bit, w in ((0x10, "discardable"), (0x20, "shared"), (0x8000, "iopl"), (0x4000, "conforming"))
              if flags & bit]
    return " ".join(words)


class _Object(object):
    def __init__(self, lf, obj):
        self.i, self.flags, self.size = obj["idx"], obj["flags"], obj["vsize"]
        data = le.object_data(lf, obj)
        self.data = bytearray(data + b"\0" * (self.size - len(data)))
        self.code = bool(self.flags & 4)
        self.wide = bool(self.flags & 0x2000)                # 32-bit code; otherwise real-mode start-up code
        self.labels = set()
        self.fix = {}                    # offset of a patched field -> (kind, target object, target offset)
        self.ins = {}                    # offset -> decoded instruction (code only)
        self.services = {}               # offset of an `int 20h` -> (device, service, is a jump)
        self.raw = bytes(self.data)
        self.memo = {}                   # offset -> what decoding there gave (kept, so nothing is decoded twice)


def _reads_as_text(found):
    """Is what was decoded really a piece of text? (a message table sits in an executable
    object, and letters decode as instructions)"""
    data = b"".join(found[a][0] for a in sorted(found))
    return len(data) >= 6 and sum(1 for b in data if 0x20 <= b < 0x7F or b in (0, 10, 13)) >= 0.85 * len(data)


class Writer(object):
    def __init__(self, lf):
        self.lf = lf
        self.d = le.descriptor(lf) or {}
        self.objects = {o["idx"]: _Object(lf, o) for o in lf.objects}
        for o, a, kind, to, ta in le.references(lf):
            if o in self.objects and to in self.objects:
                self.objects[o].fix[a] = (kind, to, ta)
                self.objects[to].labels.add(ta)
        if self.d:
            self.objects[self.d["obj"]].labels.add(self.d["offset"])
        self.names = {}
        device = self.d.get("name", "DRV").strip()
        for label, (o, a) in sorted(le.entry_points(lf).items()):
            name = "%s_%s" % (device, label)
            if label.startswith("service_") and self.d.get("device_id"):
                # a service this driver offers has a name of its own in the kit
                known = driver_services.label(self.d["device_id"], int(label[8:]))
                if re.fullmatch(r"[A-Za-z_]\w*", known) and not re.match(r"(VMM|VxD)_[0-9A-F]{4}", known):
                    name = known
            self.names.setdefault((o, a), name)
            self.objects[o].labels.add(a)
        if self.d:
            self.names[(self.d["obj"], self.d["offset"])] = "%s_DDB" % self.d["name"].strip()
        self._within = None
        self._decode()
        # places inside an instruction, and every name that will be needed, before writing starts
        self._within = {(i, x): off for i, ob in self.objects.items() for off, t in ob.ins.items()
                        for x in range(off + 1, off + len(t[0]))}
        for ob in list(self.objects.values()):
            ob.labels = {a for a in ob.labels if 0 <= a <= ob.size}
            for kind, to, ta in list(ob.fix.values()):
                self.label(to, ta)

    def label(self, o, a):
        """The name of a place. A place nothing can be written AT -- inside a patched field or
        an instruction, or outside its object altogether -- is the nearest place before it that
        can, plus the distance."""
        ob = self.objects[o]
        base = a
        if not 0 <= a <= ob.size:
            base = 0 if a < 0 or a > 0x7FFFFFFF else ob.size
            a = a - (1 << 32) if a > 0x7FFFFFFF else a
        elif self._within is not None and (o, a) in self._within:
            base = self._within[(o, a)]
        else:
            for back in (1, 2, 3):
                if a - back in ob.fix:
                    base = a - back
                    break
        if base != a:
            ob.labels.add(base)
            return "%s %s %d" % (self.label(o, base), "+" if a > base else "-", abs(a - base))
        return self.names.get((o, a)) or "L%d_%05X" % (o, a)

    # -- finding the code ------------------------------------------------------------------------
    def _decode(self):
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        md.detail = True
        work = [(o, a) for (o, a) in le.entry_points(self.lf).values()]
        for ob in self.objects.values():
            for a, (kind, to, ta) in ob.fix.items():
                if self.objects[to].code and (kind == 8 or not ob.code or True):
                    work.append((to, ta) if kind == 8 else (to, ta, "maybe"))
        tried = set()
        while work:
            item = work.pop()
            o, a = item[0], item[1]
            maybe = len(item) > 2
            ob = self.objects.get(o)
            if ob is None or not ob.code or not ob.wide or (o, a) in tried or a in ob.ins:
                continue
            tried.add((o, a))
            found, more, ok = self._follow(md, ob, a)
            if maybe and (not ok or _reads_as_text(found)):
                continue                                     # a pointer into code that is not code
            for off, ins in found.items():
                ob.ins[off] = ins
            for t in more:
                ob.labels.add(t)
                work.append((o, t))
        # drop instructions another instruction's bytes run into (data read as code by mistake)
        for ob in self.objects.values():
            covered = {}
            for off in sorted(ob.ins):
                n = len(ob.ins[off][0])
                # ...or that something points into the middle of
                if any(x in covered for x in range(off, off + n)) or \
                        any(x in ob.labels for x in range(off + 1, off + n)):
                    del ob.ins[off]
                    continue
                for x in range(off, off + n):
                    covered[x] = off
            # ...and jumps to places that are not the start of anything (so not code either)
            again = True
            while again:
                again = False
                inside = {x for off in ob.ins for x in range(off + 1, off + len(ob.ins[off][0]))}
                inside |= {a + k for a in ob.fix for k in (1, 2, 3)}
                for off in list(ob.ins):
                    target = ob.ins[off][4]
                    if target is not None and target in inside:
                        del ob.ins[off]
                        again = True
            ob.labels -= {a + k for a in ob.fix for k in (1, 2, 3)}

    def _one(self, md, ob, a):
        """The instruction at `a`, decoded once: ((bytes, mnemonic, operands, "", branch target
        or None), where control goes on: the next offset, or None when it does not) -- or None
        when nothing can be decoded there."""
        hit = ob.memo.get(a, 0)
        if hit != 0:
            return hit
        data, out = ob.raw, None
        if data[a:a + 2] == b"\xCD\x20" and a + 6 <= ob.size and not any(x in ob.fix for x in range(a, a + 6)):
            svc, dev = struct.unpack_from("<HH", data, a + 2)
            if 0 < dev < 0x8000 and (svc & 0x7FFF) < 0x800:
                ob.services[a] = (dev, svc & 0x7FFF, bool(svc & 0x8000))
                out = ((data[a:a + 6], "service", "", "service", None), None if svc & 0x8000 else a + 6)
        if out is None:
            # patched fields read as a marker, so the operand they belong to can be recognised
            window = bytearray(data[a:a + 16])
            for x in range(a, min(a + 13, ob.size)):
                if x in ob.fix:
                    struct.pack_into("<I", window, x - a, _MARK + (x & 0xFFFFF))
            ins = next(md.disasm(bytes(window), a), None)
            if ins is not None and a + ins.size <= ob.size:
                inside = [x for x in range(a, a + ins.size) if x in ob.fix]
                if not (any(x + 4 > a + ins.size for x in inside) or any(a - k in ob.fix for k in (1, 2, 3))):
                    target = None
                    groups = ins.groups
                    if (capstone.x86.X86_GRP_JUMP in groups or capstone.x86.X86_GRP_CALL in groups) and ins.operands \
                            and ins.operands[0].type == capstone.x86.X86_OP_IMM and not inside:
                        target = ins.operands[0].imm & 0xFFFFFFFF
                        if target >= ob.size:
                            target = None
                    ends = ins.mnemonic in ("ret", "retn", "retf", "iretd", "iret", "jmp", "hlt")
                    out = ((data[a:a + ins.size], ins.mnemonic, ins.op_str, "", target), None if ends else a + ins.size)
        ob.memo[a] = out
        return out

    def _follow(self, md, ob, start):
        """Decode from `start` until control cannot continue. -> ({offset: instruction},
        [targets in this object], reached a clean end)"""
        found, more, a = {}, [], start
        while 0 <= a < ob.size and a not in ob.ins and a not in found:
            one = self._one(md, ob, a)
            if one is None:
                return found, more, False
            found[a] = one[0]
            if one[0][4] is not None:
                more.append(one[0][4])
            if one[1] is None:
                return found, more, True
            a = one[1]
        return found, more, True

    # -- writing -------------------------------------------------------------------------------
    def mnemonic(self, ob, off):
        """An instruction as the text an assembler takes, or None when it has no such text
        here (a service call, or more than one patched field)."""
        data, mnem, ops, _sig, target = ob.ins[off]
        n = len(data)
        if mnem == "service":
            return None
        fixes = [x for x in range(off, off + n) if x in ob.fix]
        if len(fixes) > 1:
            return None
        m = assembly.STRING_OPS.match(mnem + " ")
        if m and not fixes:
            return ((m.group(1) or "") + m.group(2) + m.group(3)).strip()
        if target is not None:
            # the distance is said, so the assembler keeps the original's size
            if mnem in ("loop", "loope", "loopne", "jecxz", "jcxz"):
                return "%-6s %s" % (mnem, self.label(ob.i, target))
            how = "short" if n == 2 else "near ptr"
            return "%-6s %s %s" % (mnem, how, self.label(ob.i, target))
        o = ops
        if fixes:
            kind, to, ta = ob.fix[fixes[0]]
            marker = "0x%x" % (_MARK + (fixes[0] & 0xFFFFF))
            if marker not in o:
                return None
            name = self.label(to, ta)
            if kind == 8:
                o = o.replace(marker, name)
            else:
                in_memory = re.search(r"\[[^\]]*%s[^\]]*\]" % re.escape(marker), o)
                o = o.replace(marker, name if in_memory else "offset " + name)
        o = assembly.operands(o)
        return ("%-6s %s" % (assembly.mnemonic(mnem), o)).rstrip()

    def _raw(self, ob, off):
        """An instruction as its bytes, each patched field as the label it holds."""
        data, mnem, ops, _sig, _target = ob.ins[off]
        n = len(data)
        if mnem == "service":
            dev, svc, jump = ob.services[off]
            return ["        int  20h", "        dd   0%08Xh    ; %s%s" % (
                struct.unpack_from("<I", data, 2)[0], "jump to " if jump else "", driver_services.label(dev, svc))]
        lines, run, x = [], [], off

        def flush():
            if run:
                lines.append("        db   " + ", ".join("0%02Xh" % b for b in run))
                del run[:]
        while x < off + n:
            if x in ob.fix:
                kind, to, ta = ob.fix[x]
                name = self.label(to, ta)
                if kind == 8 and run and run[-1] in (0xE8, 0xE9):
                    op = run.pop()
                    flush()
                    lines.append("        %s %s" % ("call" if op == 0xE8 else "jmp ", name))
                elif kind == 8 and len(run) > 1 and run[-2] == 0x0F and 0x80 <= run[-1] <= 0x8F:
                    cc = assembly.CONDITIONS[run.pop() - 0x80]
                    run.pop()
                    flush()
                    lines.append("        j%-3s near ptr %s" % (cc, name))
                else:
                    flush()
                    lines.append("        dd   offset %s%s" % (name, " - ($ + 4)" if kind == 8 else ""))
                x += 4
            else:
                run.append(data[x - off])
                x += 1
        flush()
        text = self.mnemonic(ob, off) or ("%s %s" % (mnem, ops)).strip()
        if lines:
            lines[0] = "%-44s ; %s" % (lines[0], text)
        return lines

    def source(self, good=None):
        """-> (the assembly text, counts, {source line number: (object, offset)}). `good` is the
        set of (object, offset) of the instructions to write as mnemonics; None writes every one
        that has a text. The line map says which instruction each mnemonic line came from, so an
        assembler error on a line names the instruction to write as bytes instead."""
        line_of = {}
        name = self.d.get("name", "DRIVER").strip()
        out = ["; %s -- a virtual device driver, reconstructed from the image by CodeBreaker." % name,
               ";",
               "; One segment per object of the original. Nothing here is an address: every place the",
               "; loader patches is written as the label it refers to. Labels are L<object>_<offset in the",
               "; original>; names the image itself carries (entry points, the descriptor block) are used",
               "; where there are any. Instructions written as bytes are ones the assembler would encode",
               "; differently from the original; their mnemonic is in the comment.",
               "", ".386p", ".model flat", ""]
        counts = {"instructions": 0, "raw": 0, "data": 0}
        for i in sorted(self.objects):
            ob = self.objects[i]
            cls = "O%d%s" % (i, "CODE" if ob.code else "DATA")
            out += ["; object %d: %d bytes, %s" % (i, ob.size, _attributes(ob.flags)),
                    "S%d segment dword public %s '%s'" % (i, "use32" if ob.wide else "use16", cls)]
            a, run = 0, []

            def flush():
                out.extend(assembly.data_lines(run))
                del run[:]
            while a < ob.size:
                if a in ob.labels and a in ob.fix and a not in ob.ins and ob.fix[a][0] == 8 and len(run) > 1 \
                        and run[-2] == 0x0F and 0x80 <= run[-1] <= 0x8F:
                    cc = assembly.CONDITIONS[run.pop() - 0x80]            # the same, for a conditional jump
                    run.pop()
                    flush()
                    kind, to, ta = ob.fix[a]
                    out.append("        j%-3s near ptr %s" % (cc, self.label(to, ta)))
                    out.append("%s equ $ - 4" % self.label(i, a))
                    counts["data"] -= 2
                    a += 4
                    continue
                if a in ob.labels and a in ob.fix and a not in ob.ins and ob.fix[a][0] == 8 and run and run[-1] in (0xE8, 0xE9):
                    # A call or jump among the data whose distance field is itself referred to (code
                    # that rewrites the jump): written as the instruction, so the loader's patch
                    # is kept -- a `dd` of the distance to another object loses it -- and the
                    # label is the four bytes before the instruction's end.
                    op = run.pop()
                    flush()
                    kind, to, ta = ob.fix[a]
                    out.append("        %s %s" % ("call" if op == 0xE8 else "jmp ", self.label(to, ta)))
                    out.append("%s equ $ - 4" % self.label(i, a))
                    counts["data"] -= 1
                    a += 4
                    continue
                if a in ob.labels:
                    flush()
                    if (i, a) == (self.d.get("obj"), self.d.get("offset")):
                        out.append("public %s" % self.label(i, a))
                    out.append("%s:" % self.label(i, a))
                if a in ob.ins and a not in ob.fix:          # (bytes the loader patches from their first are an address, not an instruction)
                    flush()
                    text = self.mnemonic(ob, a) if (good is None or (i, a) in good) else None
                    counts["instructions"] += 1
                    if text is None:
                        counts["raw"] += ob.ins[a][1] != "service"
                        out += self._raw(ob, a)
                    else:
                        out.append("        " + text)
                        line_of[len(out)] = (i, a)
                    a += len(ob.ins[a][0])
                elif a in ob.fix:
                    flush()
                    kind, to, ta = ob.fix[a]
                    out.append("        dd   offset %s%s" % (self.label(to, ta), " - ($ + 4)" if kind == 8 else ""))
                    a += 4
                else:
                    run.append(ob.data[a])
                    counts["data"] += 1
                    a += 1
            flush()
            if ob.size in ob.labels:
                out.append("%s:" % self.label(i, ob.size))
            out += ["S%d ends" % i, ""]
        out.append("end")
        return "\n".join(out) + "\n", counts, line_of

    def classes(self):
        return [("O%d%s" % (i, "CODE" if ob.code else "DATA"), _attributes(ob.flags))
                for i, ob in sorted(self.objects.items())]

    # -- which instructions the assembler gives back as they were --------------------------------
    def probe(self, outdir):
        """-> the set of (object, offset) of instructions whose text assembles to the original's
        bytes (source/assembly.py). Jumps and calls inside an object are not probed (their bytes
        depend on where things are); the build as a whole is what checks those."""
        cand, good = {}, set()
        for i, ob in self.objects.items():
            for off, (data, _m, _o, _sig, target) in ob.ins.items():
                text = self.mnemonic(ob, off)
                if text is None:
                    continue
                if target is not None:
                    good.add((i, off))
                    continue
                mask = bytearray(len(data))
                for x in range(off, off + len(data)):
                    if x in ob.fix:
                        mask[x - off:x - off + 4] = b"\1\1\1\1"
                cand[(i, off)] = (text, bytes(b for b, k in zip(data, mask) if not k), bytes(mask))
        near = {self.label(to, ta) for ob in self.objects.values() for kind, to, ta in ob.fix.values() if kind == 8}
        names = {self.label(i, a) for i, ob in self.objects.items() for a in list(ob.labels)}
        ok = assembly.probe(list(cand.values()), {n for n in names if "+" not in n}, outdir, mode="flat32", near=near)
        return good | {at for at, c in cand.items() if c in ok}


def _differences(w, built, good):
    """Instructions written as mnemonics that did not come back as the original's bytes.
    -> (set of (object, offset), [objects that differ])"""
    bad, differing = set(), []
    objs = {o["idx"]: o for o in built.objects}
    for i, ob in w.objects.items():
        if i not in objs:
            differing.append(i)
            continue
        data = le.object_data(built, objs[i])
        patched = set()
        for a in ob.fix:
            patched.update(range(a, a + 4))
        n = min(len(data), ob.size)
        if objs[i]["vsize"] == ob.size and all(data[a] == ob.data[a] for a in range(n) if a not in patched):
            continue
        differing.append(i)
        pieces, a, start = [], 0, 0

        def data_piece(lo, hi):
            if hi > lo:
                pieces.append((lo, hi - lo, bytes(1 if x in patched else 0 for x in range(lo, hi)), None, False))
        while a < ob.size:
            if a in ob.ins and (i, a) in good:
                data_piece(start, a)
                length = len(ob.ins[a][0])
                mask = bytearray(1 if x in patched else 0 for x in range(a, a + length))
                jump = ob.ins[a][4] is not None
                if jump:                                     # the distance: right once the rest is
                    width = 1 if length == 2 else 4
                    mask[length - width:] = b"\1" * width
                pieces.append((a, length, bytes(mask), (i, a), jump))
                a += length
                start = a
            else:
                a += len(ob.ins[a][0]) if a in ob.ins else 1
        data_piece(start, ob.size)
        bad |= assembly.culprits(bytes(ob.data), bytes(data), pieces)
    return bad, differing


def _match_module_flags(image, lf):
    """Set the built file's LE module-flags field to the original's. The field (header offset
    0x10) tells the loader the kind of module; the Win3.x VxDs in WIN386.EXE carry a value
    (0x8020) OpenWatcom's linker does not emit, and it is metadata, not anything the code
    produces, so a faithful rebuild carries the original's."""
    want = le.header_fields(lf)["module_flags"]
    with open(image, "rb") as f:
        data = bytearray(f.read())
    lfa = struct.unpack_from("<I", data, 0x3C)[0]
    if data[lfa:lfa + 2] in (b"LE", b"LX") and struct.unpack_from("<I", data, lfa + 0x10)[0] != want:
        struct.pack_into("<I", data, lfa + 0x10, want)
        with open(image, "wb") as f:
            f.write(data)


def _nonresident_names(lf):
    """Whether the LE has a non-resident name table inside its file."""
    at, n = struct.unpack_from("<II", lf.d, lf.lfa + 0x88)
    return bool(at and n and at + n <= len(lf.d))


def reconstruct(module_path, outdir, member=0, log=None):
    """Write a driver's source, build it, and make it come back as the original (see the top).
    -> a dict: source, image, linked, checks, notes, same, instructions, raw, data, rounds."""
    name, lf = le.members(module_path)[member]
    w = Writer(lf)
    device = w.d.get("name", name).strip() or "DRIVER"
    stem = re.sub(r"\W", "_", device.lower())
    os.makedirs(outdir, exist_ok=True)
    src = os.path.join(outdir, stem + ".asm")
    out = os.path.join(outdir, stem + ".vxd")
    good = w.probe(outdir)
    result = {"source": src, "image": None, "linked": False, "rounds": 0, "notes": []}
    # the driver's .def -- its description and exports, which the linker puts in the
    # non-resident name table -- when the original has one; kept as written once it is there
    definitions = None
    def_path = os.path.join(outdir, stem + ".def")
    if os.path.isfile(def_path):
        with open(def_path, encoding="latin1") as f:
            definitions = f.read()
    elif _nonresident_names(lf):
        definitions = vxdfile.definitions(lf.d, lf.lfa)
        with open(def_path, "w", newline="\r\n", encoding="latin1") as f:
            f.write("; %s -- the definitions it was linked with, from its non-resident name table\n" % device
                    + "VXD %s\n" % device + definitions)
    if definitions:
        result["definitions"] = def_path
    for rounds in range(1, _MAX_ROUNDS + 1):
        text, counts, line_of = w.source(good)
        with open(src, "w", newline="\r\n", encoding="latin1") as f:
            f.write(text)
        result.update(rounds=rounds, instructions=counts["instructions"], raw=counts["raw"], data=counts["data"])
        obj, messages = assembly.assemble(src, outdir)
        if not obj:
            failed = {line_of[int(m.group(1))] for m in re.finditer(r"\((\d+)\)\s*:\s*Error", messages)
                      if int(m.group(1)) in line_of}
            # an uncommon opcode the assembler rejects (sidt, lgdt, ...) is rejected everywhere it
            # appears, so drop every instance of it at once rather than one per round
            drop = set(failed)
            ops = {w.objects[i].ins[a][1] for (i, a) in failed} - _COMMON
            if ops:
                drop |= {(i, a) for i in w.objects for a in w.objects[i].ins
                         if (i, a) in good and w.objects[i].ins[a][1] in ops}
            if not drop:
                result["notes"] = ["assembler: " + (messages.strip().splitlines() or ["failed"])[-1][:200]]
                break
            good -= drop
            continue
        named = bool(le.exports(lf) and le.exports(lf)[0][0])
        image, messages = builder.link(obj, device, out, classes=w.classes(), named=named,
                                       dynamic=le.header_fields(lf)["module_flags"] == le.MODULE_DYNAMIC,
                                       definitions=definitions)
        if not image:
            result["notes"] = ["linker: " + (messages.strip().splitlines() or ["failed"])[-1][:200]]
            break
        _match_module_flags(image, lf)                       # a flag the linker will not emit (Win3.x LE)
        built = le.members(image)[0][1]
        result.update(image=image, linked=True)
        bad, differing = _differences(w, built, good)
        if log and differing:
            log("  round %d: objects %s differ; %d instructions go back to bytes" % (rounds, differing, len(bad)))
        if not differing or not bad:
            checks, notes, same = builder.compare(built, lf)
            result.update(checks=checks, notes=notes, same=same, differing_objects=differing)
            break
        good -= bad
    return result


def _report(name, r):
    n = max(1, r.get("instructions", 0))
    print("%s: %d instructions, %d (%d%%) written as bytes; %d bytes of data; %d round%s" % (
        name, r.get("instructions", 0), r.get("raw", 0), 100 * r.get("raw", 0) // n,
        r.get("data", 0), r["rounds"], "" if r["rounds"] == 1 else "s"))
    if r.get("checks"):
        print("  built: %s" % ", ".join("%s %s" % (k, "same" if v else "DIFFERENT") for k, v in r["checks"].items()))
    for note in r.get("notes") or ():
        print("  " + note)
    print("  source: %s" % r["source"])
    return bool(r.get("checks")) and all(r["checks"].values())


def main(argv):
    """python -m codebreaker driver-source <driver> [--out DIR]

    Write a driver's assembly source from its image, build it and check the build against the
    original. A container of drivers (VMM32.VXD, WIN386.EXE) has every member written."""
    from codebreaker.base import corpus
    args = [a for a in argv if not a.startswith("--")]
    if not args:
        print(main.__doc__)
        return 2
    path = args[0] if os.path.isfile(args[0]) else corpus.retail(args[0])
    if not path:
        raise SystemExit("no driver %r" % args[0])
    base = argv[argv.index("--out") + 1] if "--out" in argv else \
        settings.state("driver_source", os.path.splitext(os.path.basename(path))[0].lower())
    members = le.members(path)
    ok = True
    for i, (member, _lf) in enumerate(members):
        outdir = base if len(members) == 1 else os.path.join(base, re.sub(r"\W", "_", member.lower()))
        r = reconstruct(path, outdir, member=i, log=print)
        ok = _report(member if len(members) > 1 else os.path.basename(path), r) and ok
    return 0 if ok else 1
