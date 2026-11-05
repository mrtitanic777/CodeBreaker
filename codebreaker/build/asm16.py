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

import capstone.x86 as X

from codebreaker.build import data as data16
from codebreaker import platforms


_DECODED = {}                           # (bytes, place) -> the instruction: a segment's code is decoded
                                        # once, not again for every run and pass that goes over it


def _decoded(md, data, at, bits=16):
    k = (data, at, bits)                                # (the same bytes read as 16- or 32-bit code differ)
    if k not in _DECODED:
        if len(_DECODED) > 400000:
            _DECODED.clear()
        _DECODED[k] = next(md.disasm(data, at), None)
    return _DECODED[k]


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
        self.raw_hole_calls = {}                           # offset -> (symbol, opcode): see raw_calls_into_holes
        self.raw_far_entries = set()                       # (segment, offset) a raw far call or pushed pointer
        #                                                    goes to where no function begins: see _raw_far_pointers

        import bisect
        hole_starts = [h[0] for h in self.holes]

        def hole_at(x):                                     # (the holes do not overlap: the last one starting at or before x)
            k = bisect.bisect_right(hole_starts, x) - 1
            return self.holes[k] if k >= 0 and x < self.holes[k][1] else None
        self.publics = [(start, public)] if isinstance(public, str) else sorted(public)
        public = self.publics[0][1]
        self.cs_labels = {}                                 # offset of an instruction -> the place in the run it reads through CS
        s = nef.segments()[seg - 1]
        # a 32-bit code segment (NE flag 2000h: GDI's 36 and 38, its flat thunks) is read as 32-bit
        # code -- read as 16-bit, `66 B8 iw` took the next two bytes for its immediate and the
        # `mov gs, ax` after it became a near call far past the segment's end. It is written into
        # the 16-bit code class all the same, each instruction as its bytes with its 32-bit
        # spelling beside them: the linker keeps a class's 16- and 32-bit segments apart, and a
        # use32 segment went after all of the original's, renumbering both (and it sets no 2000h
        # flag in a Windows module either way -- docs/NATIVE.md, the linker's backlog)
        self.bits = 32 if s["flags"] & 0x2000 else 16
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
        self.code_segments = {x["idx"]: x["length"] for x in segs if x["kind"] == "CODE"}
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
                # KERNEL's huge-pointer constants, __AHSHIFT (113) and __AHINCR (114), taken as a
                # byte or a word: the number the loader writes there in protected mode, 3 and 8.
                # (`db low offset _KERNEL_113` does not assemble: SETUPX's segment 28, its
                # decompressor, fell back to C over it -- and faulted 15% into copying files)
                konst = {113: 3, 114: 8, "__AHSHIFT": 3, "__AHINCR": 8}.get(what) if mod == "KERNEL" else None
                if konst is not None and src in (0, 5):
                    value = (konst + (add if r["additive"] else 0)) & (0xFF if src == 0 else 0xFFFF)
                    sites[off] = (width, ("db   0%02Xh" if src == 0 else "dw   0%04Xh") % value)
                    continue
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
        md = platforms.decoder(self.bits, detail=True)
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
                ins = _decoded(md, blob[at:min(end, at + 16)], at, self.bits)
                if ins is None or at + ins.size > end or hole_at(at + ins.size - 1):
                    break                                   # (nothing runs on into a function rebuilt from C)
                seen.add(at)
                covered = any(o in sites for o in range(at, at + ins.size))   # (its few bytes, looked up)
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
                            if mn in ("jmp", "call") and op.mem.index == 0 and op.mem.base != 0 and self.bits == 16:
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
                    target = ins.operands[0].imm & (0xFFFF if self.bits == 16 else 0xFFFFFFFF)
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
                    elif target >= len(blob):
                        # (past the end of the segment: no code jumps or calls there -- bytes read
                        # as code. NETDI's segment 54 ends in strings far pointers name ("NETDI: ",
                        # "netlog.txt"), described as functions: `ja 316C` in a segment 310A
                        # long refused all 666 of its functions. Followed no further: bytes. A
                        # near call there was taken for one to a function at that place: an
                        # entry no piece defines, a stand-in in another segment, and a link that
                        # failed -- "relocation not in the same segment")
                        break
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
        if calls and self.bits == 32:
            # (a near call or jump out of the run, by its name, would be written as 16-bit code)
            self.why = "a near call or jump out of a 32-bit segment's run (%04x)" % min(calls)
            return
        self.calls = calls
        if self.bits == 16:                                 # (its numbers are imm16s: see there)
            self._far_pointer_offsets(md, symbol_for, internal_selector, seg)
        self._raw_far_pointers(symbol_for, internal_selector)
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

    def _raw_far_pointers(self, symbol_for, internal_selector):
        """The same far pointers where the frame is written as bytes (code not followed, a
        table): a selector fix-up to one of the module's code segments, and in the two bytes
        before it the offset -- of `call far` (9A) or of a pointer stored -- is a number. If it
        is where a function begins, it is that function's offset, by its name (USER's 25:23DF
        `call far 12:085E`, LoadIcon's, in code reached only through a table: with 085E rebuilt
        from C and placed after its frame, the call ran its filler into the next function)."""
        covered = set()
        for a, t in self.ins.items():
            covered.update(range(a, a + len(t[0])))
        for a, (size, _t) in self.symbolic.items():
            covered.update(range(a, a + size))
        for off, tseg in sorted(internal_selector.items()):
            if off not in self.sites:
                continue
            # the offset before the selector (a stored pointer, `call far`), or pushed right
            # after it (`push seg X` / `push 0FF1h`: 68 ss ss 68 oo oo)
            places = [off - 2]
            if self.blob[off - 1:off] == b"\x68" and self.blob[off + 2:off + 3] == b"\x68":
                places = [off + 3]
            for p in places:
                if p < self.start or p + 2 > self.end or p in self.sites or p + 1 in self.sites \
                        or p in covered or p + 1 in covered:
                    continue
                value = struct.unpack_from("<H", self.blob, p)[0]
                name = symbol_for("function", tseg, value)
                if name:
                    self.externs.setdefault(name, "far")
                    self.sites[p] = (2, "dw   offset %s" % name)
                    break
                if p == off + 3 or self.blob[p - 1] in (0x9A, 0xEA):
                    # a call or a pushed pointer into the middle of a function: that place is
                    # an entry of its own (USER 35:035A `call far 43:0234`, inside sub_43_01D4 --
                    # rebuilt from C, the call ran its filler)
                    self.raw_far_entries.add((tseg, value))

    def _pointer_selector(self, md, at, data, selectors):
        """The code segment whose selector makes a far pointer with the number the instruction
        at `at` gives, in one of the two shapes a far pointer is built in -- `push seg X` right
        before `push n`, or the two words stored side by side (`mov [m], n` / `mov [m+2], seg
        X`, either order) -- or None."""
        if data[0] == 0x68 and len(data) == 3:
            if self.blob[at - 3:at - 2] == b"\x68" and (at - 2) in selectors:
                return selectors[at - 2]
            return None
        me = _decoded(md, self.blob[at:at + 16], at, self.bits)
        mem = [o for o in (me.operands if me else ()) if o.type == X.X86_OP_MEM]
        if not mem:
            return None
        m = mem[0].mem
        sizes = {a: len(t[0]) for a, t in self.ins.items()}
        sizes.update({a: n for a, (n, _t) in self.symbolic.items()})   # (the selector's own: a fix-up)
        before = [a for a, n in sizes.items() if a + n == at]
        for other in [at + len(data)] + before:
            if other not in sizes:
                continue
            site = other + sizes[other] - 2
            if site not in selectors:
                continue
            ins = _decoded(md, self.blob[other:other + 16], other, self.bits)
            om = [o for o in (ins.operands if ins else ()) if o.type == X.X86_OP_MEM]
            if om and (om[0].mem.base, om[0].mem.index, om[0].mem.segment) == (m.base, m.index, m.segment) \
                    and om[0].mem.disp == m.disp + 2:
                return selectors[site]
        return None

    def _far_pointer_offsets(self, md, symbol_for, selectors, seg):
        # selectors: {offset of the fix-up: the segment it is the selector of}
        """A far pointer to one of the module's functions put together in the code: its selector
        is a fix-up (or CS), its offset a plain number -- the function's, in the original. Built
        from the number, the pointer would still go where the function WAS (USER's 3:1505
        `mov word ptr [bp-18h], 1587h` with segment 10's selector: the window procedure
        BEAR306, rebuilt from C and placed after its frame, entered at its old place -- the
        frame's filler). So that number is written as the function's offset, by its name."""
        # (the selector is the one beside it -- a few instructions away: a frame may hold
        # selectors of several segments, each with a function at the same offset: USER's
        # `push seg 12; push 0FF1h`, LoadDIBCursorHandler, where 35:0FF1 is a function too)
        near = 24
        own = [a for a, (_d, mn, ops, _t) in self.ins.items()
               if mn == "push" and ops == "cs" or mn == "mov" and ops.endswith(", cs")]
        if not selectors and not own:
            return
        for at, (data, mn, ops, target) in list(self.ins.items()):
            if target is not None or mn not in ("mov", "push") or len(data) < 3:
                continue
            if not (data[0] == 0x68 or 0xB8 <= data[0] <= 0xBF or data[0] == 0xC7 or
                    data[0] in (0x26, 0x2E, 0x36, 0x3E) and len(data) > 3 and data[1] == 0xC7):
                continue                                    # (an imm16 form: push, mov reg / mem)
            value = struct.unpack_from("<H", data, len(data) - 2)[0]
            if not value:
                continue
            segs = {t for o, t in selectors.items() if abs(o - at) <= near}
            if any(abs(a - at) <= near for a in own):
                segs.add(seg)
            names = {symbol_for("function", t, value) for t in segs} - {None}
            if not names:
                # a far pointer to where no function is known to begin: a function nothing
                # else calls (USER's callback at 1:3452, `push seg 1 / push 3452h`, passed to an
                # enumeration -- taken for the tail of sub_1_3412, and gone with it when that
                # was rebuilt from C): that place is an entry of its own. Only in the exact
                # shapes of a far pointer -- a number merely near a selector is not one (taken
                # loosely, 126 of USER's functions went back to assembly for constants)
                t = self._pointer_selector(md, at, data, selectors)
                if t is not None and value < self.code_segments.get(t, 0):
                    self.raw_far_entries.add((t, value))
                continue
            if len(names) != 1:
                continue
            name = names.pop()
            text = self.mnemonic(at)
            new = re.sub(r"(?i)\b0*%Xh$" % value, "offset %s" % name, text)
            if new == text:
                continue
            self.externs.setdefault(name, "far")
            self.symbolic[at] = (len(data), new)            # (written by its name: see write)

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
        for at, (data, _m, _o, target) in self.ins.items() if self.bits == 16 else ():   # (see write)
            if target is None and at not in self.cs_labels:  # (a CS label is checked in the whole)
                out[at] = (self.mnemonic(at), data, bytes(len(data)))
        return out

    def raw_calls_into_holes(self):
        """{name: offset} of each function rebuilt from C (a hole) that a near call or jump in
        the part of the frame NOT followed as code -- written out as its bytes -- goes to: the
        bytes hold the displacement to where the function was, and it is placed elsewhere
        (USER's 25:2460 `E8 59 FB`, call 1FBC, in code reached only through a table)."""
        md = platforms.decoder(self.bits)
        size, rel = (3, "<h") if self.bits == 16 else (5, "<i")
        decoded = set()
        for a, t in self.ins.items():
            decoded.update(range(a, a + len(t[0])))
        holes = {h[0]: h[2] for h in self.holes}
        hole_spans = [(h[0], h[1]) for h in self.holes]
        out = {}
        at = self.start
        while at < self.end:
            if at in decoded or any(lo <= at < hi for lo, hi in hole_spans):
                at += 1
                continue
            stop = at
            while stop < self.end and stop not in decoded and not any(lo <= stop < hi for lo, hi in hole_spans):
                stop += 1
            for ins in md.disasm(bytes(self.blob[at:stop]), at):
                if ins.bytes[0] in (0xE8, 0xE9) and ins.size == size and self.bits == 16:
                    target = (ins.address + size + struct.unpack_from(rel, bytes(ins.bytes), 1)[0]) & 0xFFFF
                    if target in holes:
                        out.setdefault(holes[target], ins.address)
                        self.raw_hole_calls[ins.address] = (holes[target], ins.bytes[0])
            at = stop
        return out

    def write(self, good, lo=None, hi=None):
        """The source, with the instructions in `good` (offsets) as mnemonics and the rest as bytes.
        With `lo`, `hi`: only that piece of it -- one between two functions rebuilt from C, in a
        frame whose places stay where they were: what points out of the piece (a jump, a place
        read through CS, a table's entry) is written as the original's bytes, which stay right."""
        lo = self.start if lo is None else lo
        hi = self.end if hi is None else hi
        if self.bits == 32:                                 # (its instructions as their bytes: see __init__)
            good = set()
        if self.holes and not hasattr(self, "_raw_scanned"):
            # (a call in bytes not followed as code to a function rebuilt from C is written by its
            # name: the function is placed elsewhere, and the displacement in the bytes would not)
            self._raw_scanned = True
            self.raw_calls_into_holes()
            for _a, (sym, _b) in self.raw_hole_calls.items():
                self.externs[sym] = "near"

        def inside(x):
            return lo <= x < hi
        written = self.ins if good is None else [a for a in good if a in self.ins]
        covered = {x for a in written for x in range(a + 1, a + len(self.ins[a][0]))}   # (inside an instruction)
        out = ["; %s, %s: %d:%04x-%04x -- written in assembly, as its author wrote it: what the"
               % (self.module, self.public, self.seg, lo, hi),
               "; loader fixes and the calls out of the function are written as what they refer to;",
               "; an instruction the assembler would encode differently is given as bytes.",
               ".386p", "CODE%d segment byte public use16 'MCODE'" % self.seg,
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
            elif at in self.raw_hole_calls:
                flush()
                sym, b0 = self.raw_hole_calls[at]
                out.append("        %-4s near ptr %s" % ("call" if b0 == 0xE8 else "jmp", sym))
                at += 3 if self.bits == 16 else 5
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
