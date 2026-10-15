"""asm16.py -- a 16-bit function the C lane cannot give as assembly that links into the rebuild.

    from codebreaker.build import asm16
    f = asm16.Function(nef, seg, start, end, public, symbol_for)
    f.text, f.imports, f.why          (f.text is None when it cannot be written so; f.why says why)
    asm16.check(obj_bytes, f)         -> "" or what differs

The 16-bit system modules were largely written in assembly, and some of their functions say
things C cannot: they move the stack, they take a software interrupt with the registers set up
before it, they read a segment the caller left in ES. When the decompiler's C for one does not
compile, this writes it instead as what it is -- its own instructions -- in a form that links
into the rebuilt module wherever the linker puts it:

  * every place the loader fixes (the segment's relocations: a selector, an offset, a far
    pointer; to a function of the module, its data segment, or an import) is written as the
    symbol it refers to, so the linker fixes it for where things are now;
  * a near call or jump that leaves the function (no relocation exists for those: the target was
    at a known distance in the original segment) is written as `call`/`jmp` to the target's
    symbol;
  * everything else is the original's bytes.

A function that refers to its own segment by number (a jump table read through CS, a short jump
out of itself) depends on where it sits, and is refused: its C stand-in remains -- unless what it
reads is the segment's lead-in (the bytes before its first function, often a variable such as
the data segment's selector), which the build writes the same way and links first into the
segment, so it stays where it was.
"""
import re
import struct

import capstone
import capstone.x86 as X

from codebreaker.build import data as data16


class Function(object):
    def __init__(self, nef, seg, start, end, public, symbol_for, cs_ok=0, extra_entries=(), whole=False, holes=()):
        """`seg` the segment number (1-based), [start, end) the function in it -- or the run of
        functions, written together as their source file had them -- `public` the symbol to
        define, or [(offset, symbol)] for each function of a run, `symbol_for(kind, *what)` ->
        symbol or None, kind one of
        ("function", seg, off), ("segment", seg) [any symbol in that code segment],
        ("data", segment) [a data segment's first byte], ("import", module, ordinal or name). `cs_ok`: offsets of the segment below it may be read
        through CS (the segment's lead-in, which the build keeps where it was). `extra_entries`:
        other places of the run called into from outside (each must also be among `public`).
        `whole`: the run is a whole segment, which the build keeps where it was -- any place
        of it may be read through CS by its number. `holes`: [(start, end, symbol)] of the
        functions inside it rebuilt from C, which the build places after it: their bytes here are
        filler, and a call to one is a call by its name."""
        self.text, self.imports, self.why, self.externs = None, {}, "", {}
        self.missing, self.tables = set(), set()              # (set again once the code is followed)
        self.far_missing = set()                            # (segment, offset) a far call or jump reaches mid-run
        self.reaches = None                                 # refused for jumping into the middle of this offset's function
        self.holes = sorted(holes)

        def hole_at(x):
            return next((h for h in self.holes if h[0] <= x < h[1]), None)
        self.publics = [(start, public)] if isinstance(public, str) else sorted(public)
        public = self.publics[0][1]
        self.cs_labels = {}                                 # offset of an instruction -> the place in the run it reads through CS
        s = nef.segments()[seg - 1]
        blob = bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]]) if s["file_off"] else b""
        # what the loader allocates past the stored bytes is zeros it then fixes up: a far jump at
        # the very end of a segment has its selector there (GDI's segment 15)
        if blob and s["minalloc"] > len(blob) and s["minalloc"] - len(blob) <= 16:
            if end == len(blob):
                end = s["minalloc"]
            blob = blob + bytes(s["minalloc"] - len(blob))
        if not (0 <= start < end <= len(blob)):
            self.why = "its bytes are not in the file"
            return
        self.start, self.end, self.blob = start, end, blob
        segs = nef.segments()
        entries = {e["ordinal"]: e for e in nef.entries() if e.get("kind") in ("FIXED", "MOVEABLE")}
        refs = nef.module_refs()
        sites = {}                                         # offset -> (width, directive text)
        internal_selector = {}                             # offset -> segment: a selector of the module's own
        for off, src, add, r in data16._places(nef, s, blob):
            if not start <= off < end or hole_at(off):
                continue
            width = {0: 1, 2: 2, 3: 4, 5: 2}.get(src)
            if width is None or off + width > end:
                self.why = "a fix-up of kind %d at %04x" % (src, off)
                return
            if r["kind"] in ("IMPORTORD", "IMPORTNAME"):
                mod = refs[r["module"] - 1] if 0 < r["module"] <= len(refs) else None
                what = r["ordinal"] if r["kind"] == "IMPORTORD" else nef.imported_name(r["name_off"])
                sym = symbol_for("import", mod, what) if mod else None
                if not sym:
                    self.why = "an import (%s.%s) with no symbol" % (mod, what)
                    return
                self.imports[sym] = (mod, what)
                self.externs[sym] = "far"
            elif r["kind"] == "INTERNAL":
                if r["seg"] == 0xFF:
                    e = entries.get(r["entry"])
                    if not e:
                        self.why = "a fix-up to entry %d, which the module does not have" % r["entry"]
                        return
                    tseg, toff = e["seg"], e["offset"]
                else:
                    tseg, toff = r["seg"], r["entry"]
                toff = (toff + add) & 0xFFFF
                kind = segs[tseg - 1]["kind"] if 0 < tseg <= len(segs) else ""
                if kind == "DATA":                         # a data segment: the auto one or one carried
                    sym = symbol_for("data", tseg)
                    if sym:
                        self.externs[sym] = "byte"
                        if src != 2 and toff:
                            sym = "%s + %d" % (sym, toff)
                elif src == 2:
                    sym = symbol_for("segment", tseg)
                    if sym:
                        self.externs[sym] = "far"
                else:
                    sym = symbol_for("function", tseg, toff)
                    if sym:
                        self.externs[sym] = "far"
                if not sym:
                    self.why = "a fix-up to %d:%04x, where nothing has a symbol" % (tseg, toff)
                    return
            else:
                self.why = "an operating-system fix-up (floating point) at %04x" % off
                return
            text = {2: "dw   seg %s", 3: "dd   %s", 5: "dw   offset %s", 0: "db   low offset %s"}[src] % sym
            if add and r["kind"] != "INTERNAL" and src != 2:
                text += " + %d" % add
            sites[off] = (width, text)
            if r["kind"] == "INTERNAL" and src == 2:
                internal_selector[off] = tseg
        self.sites = sites
        # the instructions: near calls and jumps leaving the function, and references to its own segment
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
        md.detail = True
        calls = {}                                          # offset -> (size, text)
        self.ins = {}                                       # offset -> (bytes, mnemonic, operands, label target)
        self.symbolic = {}                                  # offset -> (size, text): an instruction whose fixup has a name
        sym_of = {}                                         # site offset -> the symbol text of its directive
        for off, (width, text) in sites.items():
            sym_of[off] = text.split(None, 1)[1] if " " in text else text
        # The instructions are found by following the code from its entry points (each function
        # of the run, and any other place called into from outside): what nothing reaches is
        # data and stays bytes. A jump through a table in the run is followed into the table.
        work = sorted({o for o, _n in self.publics} | set(extra_entries or ()))
        seen, self.missing, self.tables = set(), set(), set()
        while work:
            at = work.pop()
            while start <= at < end and at not in seen and not hole_at(at):
                ins = next(md.disasm(blob[at:min(end, at + 16)], at), None)
                if ins is None or at + ins.size > end or hole_at(at + ins.size - 1):
                    break                                   # (nothing runs on into a function rebuilt from C)
                seen.add(at)
                covered = any(at <= o < at + ins.size for o in sites)
                b0 = ins.bytes[0]
                mn = ins.mnemonic
                if covered:
                    # the forms with a symbolic spelling: a far call or jump to a symbol, a
                    # symbol's selector or offset loaded or pushed
                    site = at + 1
                    if b0 in (0x9A, 0xEA) and ins.size == 5 and sites.get(site, (0,))[0] == 4:
                        name = sym_of[site].split()[-1]
                        self.symbolic[at] = (5, "%s far ptr %s" % ("call" if b0 == 0x9A else "jmp", name))
                    elif b0 in (0x9A, 0xEA) and ins.size == 5 and (at + 3) in internal_selector:
                        # a far call into the module: the file relocates only its selector; its offset
                        # is plain bytes -- the target's, which moves with it: the whole is the name
                        tseg = internal_selector[at + 3]
                        toff = struct.unpack_from("<H", blob, at + 1)[0]
                        name = symbol_for("function", tseg, toff)
                        if not name:
                            self.far_missing.add((tseg, toff))
                            self.why = "a far %s to %d:%04x, where no function has a symbol" % (mn, tseg, toff)
                            if tseg == seg:
                                self.missing.add(toff)
                            return
                        self.externs[name] = "far"
                        self.symbolic[at] = (5, "%s far ptr %s" % ("call" if b0 == 0x9A else "jmp", name))
                        del sites[at + 3]
                    elif (0xB8 <= b0 <= 0xBF or b0 == 0x68) and ins.size == 3 and sites.get(site, (0,))[0] == 2:
                        reg = ("ax", "cx", "dx", "bx", "sp", "bp", "si", "di")[b0 - 0xB8] if b0 != 0x68 else None
                        what = sym_of[site]                 # `seg X` or `offset X`
                        self.symbolic[at] = (3, ("mov  %s, %s" % (reg, what)) if reg else ("push %s" % what))
                else:
                    for op in ins.operands:
                        if op.type != X.X86_OP_MEM or op.mem.segment != X.X86_REG_CS:
                            continue
                        disp = op.mem.disp & 0xFFFF
                        if op.mem.base == 0 and op.mem.index == 0 and 0 <= disp < cs_ok:
                            continue                        # the segment's lead-in: where it was
                        if whole and op.mem.base == 0 and op.mem.index == 0:
                            continue                        # the whole segment is where it was
                        if hole_at(disp):
                            self.why = "it reads a function rebuilt from C through CS (%s %s at %04x)" % (
                                mn, ins.op_str, at)
                            self.reaches = disp             # (that one is the assembly's own: a table)
                            return
                        if start <= disp < end and ins.size >= 3:
                            self.cs_labels[at] = disp       # a table or a variable of the run: a label
                            if mn in ("jmp", "call") and op.mem.index == 0 and op.mem.base != 0:
                                # a jump or call table: its words are places in the run to go on from
                                # (it ends, at the latest, where the first place it sends to
                                # after it begins: past that is the code it goes on to)
                                t, first = disp, end
                                while start <= t < min(end - 1, first) and t not in seen:
                                    target = struct.unpack_from("<H", blob, t)[0]
                                    if not start <= target < end:
                                        break
                                    if target > disp:
                                        first = min(first, target)
                                    self.tables.add(t)
                                    work.append(target)
                                    t += 2
                            continue
                        if whole:
                            continue
                        self.why = "it reads its own segment through CS (%s %s at %04x)" % (mn, ins.op_str, at)
                        return
                groups = set(ins.groups)
                target = None
                if not covered and (X.X86_GRP_JUMP in groups or X.X86_GRP_CALL in groups) and ins.operands \
                        and ins.operands[0].type == X.X86_OP_IMM and b0 not in (0x9A, 0xEA):
                    target = ins.operands[0].imm & 0xFFFF
                    h = hole_at(target)
                    if h and target == h[0] and b0 in (0xE8, 0xE9):
                        self.externs[h[2]] = "near"          # a function rebuilt from C: by its name
                        calls[at] = (ins.size, "%-4s near ptr %s" % ("call" if b0 == 0xE8 else "jmp", h[2]))
                    elif h:
                        self.why = "a jump into the middle of %s, rebuilt from C (%s at %04x)" % (h[2], mn, at)
                        self.reaches = target
                        return
                    elif start <= target < end:
                        self.ins[at] = (bytes(ins.bytes), mn, ins.op_str, target)
                        work.append(target)
                    elif b0 not in (0xE8, 0xE9):
                        self.why = "a short jump out of itself (%s at %04x)" % (mn, at)
                        self.reaches = target
                        return
                    else:
                        sym = symbol_for("function", seg, target)
                        if not sym:
                            self.missing.add(target)
                        else:
                            self.externs[sym] = "near"
                            calls[at] = (ins.size, "%-4s near ptr %s" % ("call" if b0 == 0xE8 else "jmp", sym))
                elif not covered:
                    self.ins[at] = (bytes(ins.bytes), mn, ins.op_str, None)
                if mn in ("ret", "retf", "iret", "jmp", "ljmp", "hlt") and not (mn == "jmp" and target is not None
                                                                                and start <= target < end and False):
                    break                                   # the code does not go on past it
                at += ins.size
        if self.missing:
            self.why = "a near call or jump to %s, where no function has a symbol" % ", ".join(
                "%04x" % t for t in sorted(self.missing))
            self.reaches = min(self.missing)
            return
        self.calls = calls
        self.module, self.seg, self.public = nef.module_name(), seg, public
        # a jump into the middle of an instruction: that instruction is not written as one
        # (each instruction's own few bytes looked up in the set: not every target for each)
        targets = {t[3] for t in self.ins.values() if t[3] is not None}
        for a in list(self.ins):
            if any(x in targets for x in range(a + 1, a + len(self.ins[a][0]))):
                del self.ins[a]
        # a place read through CS is a label too: no instruction may run over it
        read = set(self.cs_labels.values())
        for a in list(self.ins):
            if any(x in read for x in range(a + 1, a + len(self.ins[a][0]))):
                del self.ins[a]
        for a in [a for a in self.cs_labels if a not in self.ins]:
            del self.cs_labels[a]
        self.labels = {t[3] for t in self.ins.values() if t[3] is not None and t[3] in self.ins}
        for a in [a for a, t in self.ins.items() if t[3] is not None and t[3] not in self.labels]:
            del self.ins[a]                                 # (its target is not an instruction start)
        self.labels |= set(self.cs_labels.values())
        held = set()
        for t in self.tables:                               # what a jump table holds: labels, as words
            self.labels.add(struct.unpack_from("<H", blob, t)[0])
            held |= {t, t + 1}
        for a in list(self.ins):
            if any(x in held for x in range(a, a + len(self.ins[a][0]))):
                del self.ins[a]
        self.text = self.write(set())

    def mnemonic(self, at):
        """The instruction at `at` as the assembler reads it, or None."""
        from codebreaker.source import assembly
        data, mn, ops, target = self.ins[at]
        m = assembly.STRING_OPS.match(mn + " ")
        if m and ":" not in ops.replace("es:[di]", ""):
            return ((m.group(1) or "") + m.group(2) + m.group(3)).strip()
        if target is not None:
            label = "L_%04X" % target
            if mn in ("loop", "loope", "loopne", "jcxz", "jecxz"):
                return "%-6s %s" % (mn, label)
            return "%-6s %s %s" % (mn, "short" if len(data) == 2 else "near ptr", label)
        if at in self.cs_labels:                            # what it reads through CS, by its label
            disp = self.cs_labels[at]
            ops = re.sub(r"\b0x%x\b" % disp, "L_%04X" % disp, ops) if disp else ops.replace("cs:[", "cs:[L_0000 + ", 1)
        ops = re.sub(r"(?<![a-z]:)\[(0x[0-9a-f]+)\]", r"ds:[\1]", ops)
        return ("%-6s %s" % (assembly.mnemonic(mn), assembly.operands(ops))).rstrip()

    def _stays_inside(self, at, lo, hi):
        """Whether what the instruction at `at` points at (a jump's target, a place it reads
        through CS) is inside [lo, hi): if not, it is written as bytes in a piece."""
        target = self.ins[at][3]
        if target is not None and not lo <= target < hi:
            return False
        disp = self.cs_labels.get(at)
        return disp is None or lo <= disp < hi

    def _used(self, label, lo, hi):
        """Whether a label is referred to from inside [lo, hi) (so a piece writes it)."""
        if not lo <= label < hi:
            return False
        refs = getattr(self, "_refs", None)
        if refs is None:                                    # (who refers to each label, once)
            refs = self._refs = {}
            for a, t in self.ins.items():
                if t[3] is not None:
                    refs.setdefault(t[3], []).append(a)
            for a, d in self.cs_labels.items():
                refs.setdefault(d, []).append(a)
            for t in self.tables:
                refs.setdefault(struct.unpack_from("<H", self.blob, t)[0], []).append(t)
        return any(lo <= a < hi for a in refs.get(label, ()))

    def candidates(self):
        """{offset: (text, bytes, mask)} of the instructions the assembler's spelling is probed for."""
        out = {}
        for at, (data, _m, _o, target) in self.ins.items():
            if target is None and at not in self.cs_labels:  # (a CS label is checked in the whole)
                out[at] = (self.mnemonic(at), data, bytes(len(data)))
        return out

    def write(self, good, lo=None, hi=None):
        """The source, with the instructions in `good` (offsets) as mnemonics and the rest as bytes.
        With `lo`, `hi`: only that piece of it -- one between two functions rebuilt from C, in a
        frame whose places stay where they were: what points out of the piece (a jump, a place
        read through CS, a table's entry) is written as the original's bytes, which stay right."""
        lo = self.start if lo is None else lo
        hi = self.end if hi is None else hi

        def inside(x):
            return lo <= x < hi
        written = self.ins if good is None else [a for a in good if a in self.ins]
        covered = {x for a in written for x in range(a + 1, a + len(self.ins[a][0]))}   # (inside an instruction)
        out = ["; %s, %s: %d:%04x-%04x -- written in assembly, as its author wrote it: what the"
               % (self.module, self.public, self.seg, lo, hi),
               "; loader fixes and the calls out of the function are written as what they refer to;",
               "; an instruction the assembler would encode differently is given as bytes.",
               ".386p", "CODE%d segment byte public use16 'CODE'" % self.seg,
               "        assume cs:CODE%d, ds:nothing, es:nothing, ss:nothing" % self.seg]
        mine = {name for o, name in self.publics if inside(o)}
        others = {name: "near" for o, name in self.publics if not inside(o)}   # (other pieces of the frame)
        out += ["        extrn %s:%s" % (sym, k) for sym, k in sorted({**others, **self.externs}.items()) if sym not in mine]
        out += ["        public %s" % name for o, name in self.publics if inside(o)]
        entry = {o: name for o, name in self.publics if inside(o)}
        # an instruction something enters in its middle (hand-written overlapping entries:
        # `cmp ax, 0B1xxh` chains entered one byte in, where they read `mov cl, xx`) is written
        # as its bytes -- the same bytes, and the place inside it gets its label
        targets = set(entry) | {x for x in self.labels if inside(x)}
        split = {a for a in self.ins if inside(a) and any(t in targets for t in range(a + 1, a + len(self.ins[a][0])))}
        run = []

        def flush():
            for i in range(0, len(run), 16):
                out.append("        db   " + ", ".join("0%02Xh" % b for b in run[i:i + 16]))
            del run[:]
        at = lo
        while at < hi:
            h = next((x for x in self.holes if x[0] == at), None)
            if h:
                flush()
                out.append("        db   %d dup (0CCh)                         ; %s, rebuilt from C, is placed"
                           " after this" % (h[1] - h[0], h[2]))
                at = h[1]
                continue
            if at in entry:
                flush()
                out.append("%s:" % entry[at])
            if at in self.labels and self._used(at, lo, hi):
                flush()
                out.append("L_%04X:" % at)
            if at in self.symbolic and good is not None:
                flush()
                size, text = self.symbolic[at]
                out.append("        " + text)
                at += size
            elif at in self.sites:
                flush()
                width, text = self.sites[at]
                out.append("        " + text)
                at += width
            elif at in self.tables and inside(struct.unpack_from("<H", self.blob, at)[0]):
                flush()
                t = struct.unpack_from("<H", self.blob, at)[0]
                # an entry is a label where an instruction of this piece is written; anywhere
                # else (a function rebuilt from C, the middle of an instruction: a table guessed
                # too long reads on into data) it is the number it is -- the same bytes
                held = t in self.ins and (good is None or t in good) and t not in covered                     and not any(h[0] <= t < h[1] for h in self.holes)
                out.append("        dw   L_%04X" % t if held else "        dw   0%04Xh" % t)
                at += 2
            elif at in self.calls:
                flush()
                size, text = self.calls[at]
                out.append("        " + text)
                at += size
            elif at in self.ins and at in good and at not in split and self._stays_inside(at, lo, hi):
                flush()
                out.append("        " + self.mnemonic(at))
                at += len(self.ins[at][0])
            elif at in self.ins and at not in split:
                flush()
                data = self.ins[at][0]
                out.append("        db   %-40s ; %s %s" % (", ".join("0%02Xh" % b for b in data), self.ins[at][1],
                                                          self.ins[at][2]))
                at += len(data)
            else:
                run.append(self.blob[at])
                at += 1
        flush()
        out += ["CODE%d ends" % self.seg, "end", ""]
        text = "\n".join(out)
        # (a table read on into what is code or data: an entry whose place got no label is the
        # number it is -- the same two bytes)
        defined = set(re.findall(r"(?m)^(L_[0-9A-F]{4}):", text))
        return re.sub(r"(?m)^(        dw   )(L_([0-9A-F]{4}))$",
                      lambda mm: mm.group(0) if mm.group(2) in defined else "%s0%sh" % (mm.group(1), mm.group(3)), text)


def check_piece(obj, f, lo, hi):
    """A piece of a frame (see Function.write) against the original. -> "" or what differs."""
    from codebreaker.build import msc6
    code, mask = msc6.object_code(obj)
    if len(code) != hi - lo:
        return "%d bytes, the original %d" % (len(code), hi - lo)
    start, end = f.start, f.end
    f.start, f.end = lo, hi
    try:
        bad = differences(code, mask, f)
    finally:
        f.start, f.end = start, end
    return ("differs at %04x" % bad[0]) if bad else ""


def check(obj, f):
    """The assembled object against the original: every byte the same but those the loader or
    the linker sets. -> "" or where they differ."""
    from codebreaker.build import msc6
    code, mask = msc6.object_code(obj)
    want = f.blob[f.start:f.end]
    if len(code) != len(want):
        return "%d bytes, the original %d" % (len(code), len(want))
    bad = differences(code, mask, f)
    return ("differs at %04x" % bad[0]) if bad else ""


def differences(code, mask, f):
    """The offsets (in the segment) where the assembled bytes are not the original's, the places
    the loader or the linker sets aside."""
    want = f.blob[f.start:f.end]
    fixed = set()
    for at, (width, _t) in f.sites.items():
        fixed.update(range(at - f.start, at - f.start + width))
    for at, (size, _t) in list(f.calls.items()) + list(f.symbolic.items()):
        fixed.update(range(at - f.start + 1, at - f.start + size))
    for t in f.tables:
        fixed.update((t - f.start, t - f.start + 1))
    for lo, hi, _n in f.holes:
        fixed.update(range(lo - f.start, hi - f.start))
    return [f.start + i for i in range(min(len(code), len(want))) if code[i] != want[i] and i not in fixed and not mask[i]]
