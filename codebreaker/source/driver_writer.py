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
import json
import os
import re
import struct

import capstone

from codebreaker.base import settings
from codebreaker.build import driver as builder
from codebreaker.formats import le
from codebreaker.symbols import driver_services

_MARK = 0x7A000000                       # stands in for a patched address while decoding
_CC = ("o", "no", "b", "ae", "e", "ne", "be", "a", "s", "ns", "p", "np", "l", "ge", "le", "g")
_SPELL = {"pushal": "pushad", "popal": "popad", "int3": "int 3", "retn": "ret", "pushfd": "pushfd",
          "fwait": "wait", "iretd": "iretd", "xlatb": "xlatb"}
_STRING = re.compile(r"^(rep |repe |repne )?(movs|cmps|stos|lods|scas|ins|outs)([bwd])\b")
_HEX = re.compile(r"\b0x([0-9a-f]+)\b")
_MAX_ROUNDS = 40


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


def _reads_as_text(found):
    """Is what was decoded really a piece of text? (a message table sits in an executable
    object, and letters decode as instructions)"""
    data = b"".join(found[a][0] for a in sorted(found))
    return len(data) >= 6 and sum(1 for b in data if 0x20 <= b < 0x7F or b in (0, 10, 13)) >= 0.85 * len(data)


def _signature(ins):
    """What makes two instructions the same KIND to an assembler: prefixes, opcode, and the
    form of the operand byte (register or memory, with or without an index byte)."""
    modrm = ins.modrm
    form = "%d%s" % (modrm >> 6, "s" if modrm & 7 == 4 and modrm >> 6 != 3 else "d" if modrm >> 6 == 0 and modrm & 7 == 5 else "")
    return "%s|%s|%s|%d" % (bytes(x for x in ins.prefix if x).hex(), bytes(x for x in ins.opcode if x).hex() or "00",
                            form if ins.modrm or len(ins.bytes) > 1 else "", len(ins.operands))


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
        self._decode()

    def label(self, o, a):
        ob = self.objects[o]
        for back in (1, 2, 3):                               # a place inside a patched field
            if a - back in ob.fix:
                ob.labels.add(a - back)
                return "%s + %d" % (self.label(o, a - back), back)
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

    def _follow(self, md, ob, start):
        """Decode from `start` until control cannot continue. -> ({offset: (bytes, mnemonic,
        operands, signature, branch target or None)}, [targets in this object], reached a clean end)"""
        found, more, a = {}, [], start
        data = bytes(ob.data)
        while 0 <= a < ob.size and a not in ob.ins and a not in found:
            if data[a:a + 2] == b"\xCD\x20" and a + 6 <= ob.size and not any(x in ob.fix for x in range(a, a + 6)):
                svc, dev = struct.unpack_from("<HH", data, a + 2)
                if 0 < dev < 0x8000 and (svc & 0x7FFF) < 0x800:
                    ob.services[a] = (dev, svc & 0x7FFF, bool(svc & 0x8000))
                    found[a] = (data[a:a + 6], "service", "", "service", None)
                    if svc & 0x8000:                         # the jump form does not come back
                        return found, more, True
                    a += 6
                    continue
            # patched fields read as a marker, so the operand they belong to can be recognised
            window = bytearray(data[a:a + 16])
            for x in range(a, min(a + 16, ob.size)):
                if x in ob.fix and x + 4 <= a + 16:
                    struct.pack_into("<I", window, x - a, _MARK + (x & 0xFFFFF))
            ins = next(md.disasm(bytes(window), a), None)
            if ins is None or a + ins.size > ob.size:
                return found, more, False
            inside = [x for x in range(a, a + ins.size) if x in ob.fix]
            if any(x + 4 > a + ins.size for x in inside) or any((x - 3 <= y < x) for x in range(a, a + ins.size)
                                                                for y in ob.fix if y < a):
                return found, more, False                    # a patched field straddles the instruction
            target = None
            groups = set(ins.groups)
            is_branch = capstone.x86.X86_GRP_JUMP in groups or capstone.x86.X86_GRP_CALL in groups
            if is_branch and ins.operands and ins.operands[0].type == capstone.x86.X86_OP_IMM and not inside:
                target = ins.operands[0].imm & 0xFFFFFFFF
                if target < ob.size:
                    more.append(target)
                else:
                    target = None
            found[a] = (data[a:a + ins.size], ins.mnemonic, ins.op_str, _signature(ins), target)
            if ins.mnemonic in ("ret", "retn", "retf", "iretd", "iret", "jmp", "hlt"):
                return found, more, True
            a += ins.size
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
        m = _STRING.match(mnem + " ")
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
        o = _HEX.sub(lambda mm: "0%sh" % mm.group(1).upper(), o)
        return ("%-6s %s" % (_SPELL.get(mnem, mnem), o)).rstrip()

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
                    cc = _CC[run.pop() - 0x80]
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
        """-> (the assembly text, counts). `good` is the set of (object, offset) of the
        instructions to write as mnemonics; None writes every one that has a text."""
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
                for k in range(0, len(run), 16):
                    out.append("        db   " + ", ".join("0%02Xh" % b for b in run[k:k + 16]))
                del run[:]
            while a < ob.size:
                if a in ob.labels:
                    flush()
                    if (i, a) == (self.d.get("obj"), self.d.get("offset")):
                        out.append("public %s" % self.label(i, a))
                    out.append("%s:" % self.label(i, a))
                if a in ob.ins:
                    flush()
                    text = self.mnemonic(ob, a) if (good is None or (i, a) in good) else None
                    counts["instructions"] += 1
                    if text is None:
                        counts["raw"] += ob.ins[a][1] != "service"
                        out += self._raw(ob, a)
                    else:
                        out.append("        " + text)
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
        return "\n".join(out) + "\n", counts

    def classes(self):
        return [("O%d%s" % (i, "CODE" if ob.code else "DATA"), _attributes(ob.flags))
                for i, ob in sorted(self.objects.items())]

    # -- which instructions the assembler gives back as they were --------------------------------
    def probe(self, outdir):
        """-> the set of (object, offset) of instructions whose text assembles to the original's
        bytes. Each distinct text is assembled once, at a place of its own in a probe file, and
        read back from the object file. Jumps and calls inside an object are not probed (their
        bytes depend on where things are); the build as a whole is what checks those."""
        slots, where = {}, {}
        for i, ob in self.objects.items():
            for off, (data, _m, _o, _sig, target) in ob.ins.items():
                text = self.mnemonic(ob, off)
                if text is None:
                    continue
                if target is not None:
                    where[(i, off)] = None
                    continue
                mask = bytearray(len(data))
                for x in range(off, off + len(data)):
                    if x in ob.fix:
                        mask[x - off:x - off + 4] = b"\1\1\1\1"
                key = (text, bytes(b for b, k in zip(data, mask) if not k), bytes(mask))
                slots.setdefault(key, len(slots))
                where[(i, off)] = key
        known = _load_probes()
        todo = [k for k in slots if _probe_key(k) not in known]
        bad_lines = set()
        for _attempt in range(12):
            if not todo:
                break
            calls = set()
            for i, ob in self.objects.items():
                for a, (kind, to, ta) in ob.fix.items():
                    if kind == 8:
                        calls.add(self.label(to, ta))
            names = sorted({self.label(i, a) for i, ob in self.objects.items() for a in ob.labels})
            # flat model: a label in a code segment is addressed like any other, without the
            # code-segment prefix the assembler would otherwise put before it
            lines = [".386p", ".model flat", "PD segment dword public use32 'PXCODE'"]
            lines += ["%s label %s" % (n, "near" if n in calls else "byte") for n in names]
            lines += ["PD ends", "PC segment dword public use32 'PCODE'"]
            line_of = {}
            for n, key in enumerate(todo):
                lines.append("        org  %d" % (32 * n))
                line_of[len(lines) + 1] = key
                lines.append("        " + (key[0] if key not in bad_lines else "nop"))
            lines += ["PC ends", "end"]
            src = os.path.join(outdir, "probe.asm")
            with open(src, "w", newline="\r\n", encoding="latin1") as f:
                f.write("\n".join(lines) + "\n")
            obj, messages = builder.assemble(src, outdir)
            if obj:
                got = _ledata(open(obj, "rb").read())
                for n, key in enumerate(todo):
                    text, plain, mask = key
                    out = got.get(32 * n, b"")
                    same = key not in bad_lines and len(out) == len(mask) and \
                        bytes(b for b, k in zip(out, mask) if not k) == plain
                    known[_probe_key(key)] = same
                todo = []
                break
            failed = {line_of[int(m.group(1))] for m in re.finditer(r"\((\d+)\)\s*:\s*Error", messages)
                      if int(m.group(1)) in line_of}
            if not failed:
                for key in todo:
                    known[_probe_key(key)] = False
                todo = []
                break
            bad_lines |= failed
        _save_probes(known)
        return {at for at, key in where.items() if key is None or known.get(_probe_key(key))}


def _probe_key(key):
    return "%s|%s|%s" % (key[0], key[1].hex(), key[2].hex())


def _load_probes():
    try:
        with open(settings.state("driver_probes.json"), encoding="utf-8") as f:
            return json.load(f)
    except (OSError, ValueError):
        return {}


def _save_probes(known):
    with open(settings.state("driver_probes.json"), "w", encoding="utf-8") as f:
        json.dump(known, f, indent=0, sort_keys=True)


def _ledata(obj):
    """{offset: bytes} of the data records of the LAST segment an object file defines (the probe
    file's code), joined where one record continues the one before."""
    chunks, i, n, segs = {}, 0, len(obj), 0
    while i + 3 <= n:
        kind = obj[i]
        end = i + 3 + (obj[i + 1] | (obj[i + 2] << 8))
        if end > n:
            break
        if kind in (0x98, 0x99):
            segs += 1
        elif kind in (0xA0, 0xA1):
            q = i + 3
            seg = obj[q]
            q += 1
            if kind == 0xA1:
                off = struct.unpack_from("<I", obj, q)[0]
                q += 4
            else:
                off = struct.unpack_from("<H", obj, q)[0]
                q += 2
            chunks.setdefault(seg, {})[off] = obj[q:end - 1]
        i = end
    out = {}
    last = None
    for off, data in sorted(chunks.get(segs, {}).items()):
        if last is not None and last + len(out[last]) == off:
            out[last] += data
        else:
            out[off] = bytes(data)
            last = off
    return out


def _differences(w, built, good):
    """Instructions written as mnemonics that sit where the built image's bytes differ from the
    original's (outside the patched places), per object from its first difference on while the
    sizes still agree. -> (set of (object, offset), [objects that differ])"""
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
        diff = [a for a in range(n) if a not in patched and data[a] != ob.data[a]]
        if not diff and objs[i]["vsize"] == ob.size:
            continue
        differing.append(i)
        aligned = objs[i]["vsize"] == ob.size                # same size: every difference is in place
        for at in (diff if aligned else diff[:1] or [n]):
            for a in ob.ins:
                if a <= at < a + len(ob.ins[a][0]) and (i, a) in good:
                    bad.add((i, a))
        if not aligned and not [x for x in bad if x[0] == i]:
            # the first difference is not in an instruction written as text: the size went wrong
            # earlier; give up the nearest one before it
            before = [a for a in ob.ins if a <= (diff[0] if diff else n) and (i, a) in good]
            if before:
                bad.add((i, max(before)))
    return bad, differing


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
    for rounds in range(1, _MAX_ROUNDS + 1):
        text, counts = w.source(good)
        with open(src, "w", newline="\r\n", encoding="latin1") as f:
            f.write(text)
        result.update(rounds=rounds, instructions=counts["instructions"], raw=counts["raw"], data=counts["data"])
        obj, messages = builder.assemble(src, outdir)
        if not obj:
            lines = text.split("\n")
            failed = set()
            for m in re.finditer(r"\((\d+)\)\s*:\s*Error", messages):
                failed.add(lines[int(m.group(1)) - 1].strip() if int(m.group(1)) <= len(lines) else "")
            drop = {(i, a) for (i, a) in good if w.mnemonic(w.objects[i], a) in failed}
            if not drop:
                result["notes"] = ["assembler: " + (messages.strip().splitlines() or ["failed"])[-1][:200]]
                break
            good -= drop
            continue
        image, messages = builder.link(obj, device, out, classes=w.classes(),
                                       dynamic=le.header_fields(lf)["module_flags"] == le.MODULE_DYNAMIC)
        if not image:
            result["notes"] = ["linker: " + (messages.strip().splitlines() or ["failed"])[-1][:200]]
            break
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


def main(argv):
    """python -m codebreaker driver-source <driver> [--out DIR]

    Write a driver's assembly source from its image, build it and check the build against the
    original."""
    from codebreaker.base import corpus
    args = [a for a in argv if not a.startswith("--")]
    if not args:
        print(main.__doc__)
        return 2
    path = args[0] if os.path.isfile(args[0]) else corpus.retail(args[0])
    if not path:
        raise SystemExit("no driver %r" % args[0])
    outdir = argv[argv.index("--out") + 1] if "--out" in argv else \
        settings.state("driver_source", os.path.splitext(os.path.basename(path))[0].lower())
    r = reconstruct(path, outdir, log=print)
    n = max(1, r.get("instructions", 0))
    print("%s: %d instructions, %d (%d%%) written as bytes; %d bytes of data; %d round%s" % (
        os.path.basename(path), r.get("instructions", 0), r.get("raw", 0), 100 * r.get("raw", 0) // n,
        r.get("data", 0), r["rounds"], "" if r["rounds"] == 1 else "s"))
    if r.get("checks"):
        print("  built: %s" % ", ".join("%s %s" % (k, "same" if v else "DIFFERENT") for k, v in r["checks"].items()))
    for note in r.get("notes") or ():
        print("  " + note)
    print("  source: %s" % r["source"])
    return 0 if r.get("checks") and all(r["checks"].values()) else 1
