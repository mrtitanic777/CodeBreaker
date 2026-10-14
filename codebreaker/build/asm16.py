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
import capstone
import capstone.x86 as X

from codebreaker.build import data as data16


class Function(object):
    def __init__(self, nef, seg, start, end, public, symbol_for, cs_ok=0):
        """`seg` the segment number (1-based), [start, end) the function in it, `public` the
        symbol to define, `symbol_for(kind, *what)` -> symbol or None, kind one of
        ("function", seg, off), ("segment", seg) [any symbol in that code segment],
        ("data", segment) [a data segment's first byte], ("import", module, ordinal or name). `cs_ok`: offsets of the segment below it may be read
        through CS (the segment's lead-in, which the build keeps where it was)."""
        self.text, self.imports, self.why, self.externs = None, {}, "", {}
        s = nef.segments()[seg - 1]
        blob = bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]]) if s["file_off"] else b""
        if not (0 <= start < end <= len(blob)):
            self.why = "its bytes are not in the file"
            return
        self.start, self.end, self.blob = start, end, blob
        segs = nef.segments()
        entries = {e["ordinal"]: e for e in nef.entries() if e.get("kind") in ("FIXED", "MOVEABLE")}
        refs = nef.module_refs()
        sites = {}                                         # offset -> (width, directive text)
        for off, src, add, r in data16._places(nef, s, blob):
            if not start <= off < end:
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
        self.sites = sites
        # the instructions: near calls and jumps leaving the function, and references to its own segment
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
        md.detail = True
        calls = {}                                          # offset -> (size, text)
        at = start
        while at < end:
            ins = next(md.disasm(blob[at:min(end, at + 16)], at), None)
            if ins is None:
                at += 1
                continue
            covered = any(at <= o < at + ins.size for o in sites)
            if not covered:
                for op in ins.operands:
                    if op.type == X.X86_OP_MEM and op.mem.segment == X.X86_REG_CS and not (
                            op.mem.base == 0 and op.mem.index == 0 and 0 <= op.mem.disp < cs_ok):
                        self.why = "it reads its own segment through CS (%s %s at %04x)" % (ins.mnemonic, ins.op_str, at)
                        return
                groups = set(ins.groups)
                if (X.X86_GRP_JUMP in groups or X.X86_GRP_CALL in groups) and ins.operands \
                        and ins.operands[0].type == X.X86_OP_IMM and ins.bytes[0] not in (0x9A, 0xEA):
                    target = ins.operands[0].imm & 0xFFFF
                    if not start <= target < end:
                        if ins.bytes[0] not in (0xE8, 0xE9):
                            self.why = "a short jump out of itself (%s at %04x)" % (ins.mnemonic, at)
                            return
                        sym = symbol_for("function", seg, target)
                        if not sym:
                            self.why = "a near %s to %04x, where no function has a symbol" % (ins.mnemonic, target)
                            return
                        self.externs[sym] = "near"
                        calls[at] = (ins.size, "%-4s near ptr %s" % ("call" if ins.bytes[0] == 0xE8 else "jmp", sym))
            at += ins.size
        self.calls = calls
        self.text = self._write(nef.module_name(), seg, public)

    def _write(self, module, seg, public):
        out = ["; %s, %s: %d:%04x-%04x -- the original's instructions; what the loader fixes, and"
               % (module, public, seg, self.start, self.end),
               "; the near calls and jumps out of the function, are written as what they refer to.",
               ".286p", "CODE%d segment byte public 'CODE'" % seg]
        out += ["        extrn %s:%s" % (s, k) for s, k in sorted(self.externs.items()) if s != public]
        out += ["        public %s" % public, "%s:" % public]
        run = []

        def flush():
            for i in range(0, len(run), 16):
                out.append("        db   " + ", ".join("0%02Xh" % b for b in run[i:i + 16]))
            del run[:]
        at = self.start
        while at < self.end:
            if at in self.sites:
                flush()
                width, text = self.sites[at]
                out.append("        " + text)
                at += width
            elif at in self.calls:
                flush()
                size, text = self.calls[at]
                out.append("        " + text)
                at += size
            else:
                run.append(self.blob[at])
                at += 1
        flush()
        out += ["CODE%d ends" % seg, "end", ""]
        return "\n".join(out)


def check(obj, f):
    """The assembled object against the original: every byte the same but those the loader or
    the linker sets. -> "" or where they differ."""
    from codebreaker.build import msc6
    code, mask = msc6.object_code(obj)
    want = f.blob[f.start:f.end]
    if len(code) != len(want):
        return "%d bytes, the original %d" % (len(code), len(want))
    fixed = set()
    for at, (width, _t) in f.sites.items():
        fixed.update(range(at - f.start, at - f.start + width))
    for at, (size, _t) in f.calls.items():
        fixed.update(range(at - f.start + 1, at - f.start + size))
    bad = [i for i in range(len(want)) if code[i] != want[i] and i not in fixed and not mask[i]]
    return ("differs at %04x" % (f.start + bad[0])) if bad else ""
