"""origin.py -- was a 16-bit function written in assembly, or made by a C compiler?

    from codebreaker.analyze import origin
    r = origin.module16(path)       r.functions {va: Origin(.name .language .evidence)}, r.summary()
    python -m codebreaker origin <module>

A function is reconstructed in the language it was written in: what a C compiler made, as C;
what a person wrote in assembly, as assembly (Windows' 16-bit core and its drivers are largely
the second). The code says which. Microsoft's C compilers of the period (C 5.1 to 8.0) have a
small, regular repertoire, and a person writing assembly reaches for things they never emit:

  LOOP, JCXZ                         the compiler counts with DEC/JNZ
  LODS/STOS/SCAS/CMPS without REP    the compiler uses string instructions only with REP
  arguments in registers             read before anything wrote them (bar the DS in AX that a
                                     Windows far prologue receives, and what a call leaves:
                                     its result in DX:AX, and BX CX ES it may use)
  a result in the carry flag         callers branch on CF after calling it
  the stack itself moved             a write to SS, or to SP other than restoring it from BP
  its own code segment read          a CS: memory operand (a variable kept beside the code) --
                                     but not a JMP through a CS table: that is a compiled switch
  INT n                              C reaches DOS and the BIOS through library functions
  32-bit registers in 16-bit code    (the C compilers of the period did not make them)
  BP as a plain register             written other than the frame's own `mov bp, sp`
  PUSHF/POPF, CLI/STI, IN/OUT        the machine itself
  a jump stub                        a "function" that is one JMP to code elsewhere: glue

and a C compiler leaves fingerprints of its own -- a call to __chkstk (__aNchkstk/__aFchkstk),
its arithmetic helpers (__aFlmul, __aFuldiv ...), the frame it builds and takes down. One piece
of assembly evidence makes a function assembly; the evidence is kept with the verdict.
"""
import collections

import capstone
import capstone.x86 as X

from codebreaker.decompile import describe

_COMPILER_HELPERS = ("chkstk", "__aFlmul", "__aFldiv", "__aFuldiv", "__aFlrem", "__aFulrem", "__aFlshl",
                     "__aFlshr", "__aFulshr", "__aNlmul", "__aNldiv", "__aNuldiv", "__aNlrem", "__aNulrem",
                     "__aFNalmul", "__aFNauldiv", "__aFahdiff", "__aFahincr", "__fac")
_STRING = {"lodsb", "lodsw", "lodsd", "stosb", "stosw", "stosd", "scasb", "scasw", "scasd", "cmpsb", "cmpsw",
           "cmpsd", "movsb", "movsw", "movsd"}
_MACHINE = {"pushf": "PUSHF", "popf": "POPF", "cli": "CLI", "sti": "STI", "in": "IN", "out": "OUT",
            "lgdt": "LGDT", "lidt": "LIDT", "lmsw": "LMSW", "smsw": "SMSW", "arpl": "ARPL", "lsl": "LSL",
            "lar": "LAR", "verr": "VERR", "verw": "VERW", "hlt": "HLT"}
_REGS32 = {X.X86_REG_EAX, X.X86_REG_EBX, X.X86_REG_ECX, X.X86_REG_EDX, X.X86_REG_ESI, X.X86_REG_EDI,
           X.X86_REG_EBP, X.X86_REG_ESP}


class Origin(object):
    __slots__ = ("va", "name", "language", "evidence", "compiler")

    def __init__(self, va, name):
        self.va, self.name, self.language, self.evidence, self.compiler = va, name, "c", [], []

    def __repr__(self):
        return "<%s %s: %s>" % (self.name, self.language, "; ".join(self.evidence or self.compiler) or "nothing either way")


class Result(object):
    def __init__(self):
        self.functions = {}

    def summary(self):
        n = collections.Counter(o.language for o in self.functions.values())
        why = collections.Counter(e.split(" (")[0] for o in self.functions.values() for e in o.evidence[:1])
        return n, why


def module16(path, more=None):
    """Every function of a 16-bit module, judged. `more`: {va: name} of functions found besides
    the ones the module's own tables give (the decompiler finds the ones only called). -> Result"""
    import os
    from codebreaker.build import watcom
    d = describe.describe16(path, watcom._scratch("origin", os.path.basename(path).lower()))
    names = {va: n for va, n in d.names.items() if va in d.where}
    for va, n in (more or {}).items():
        if va not in names:
            names[va], d.where[va] = n, describe.where(va)
    starts = collections.defaultdict(list)
    for va in names:
        seg, off = d.where[va]
        starts[seg].append(off)
    for seg in starts:
        starts[seg].sort()
    imported = dict(getattr(d, "import_names", {}) or {})
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    md.detail = True
    res = Result()
    for va, name in sorted(names.items()):
        seg, off = d.where[va]
        image = d.images.get(seg, b"")
        later = [o for o in starts[seg] if o > off]
        end = later[0] if later else len(image)
        o = res.functions[va] = Origin(va, name)
        _judge(o, md, image, off, end, imported, set(starts[seg]))
    return res


def _judge(o, md, image, start, end, imported, starts=()):
    written, prologue_ax, frame = set(), False, False
    first_ins = next(md.disasm(image[start:end], start), None)
    if first_ins is not None and first_ins.mnemonic in ("jmp", "ljmp") and end - start <= first_ins.size + 2:
        o.evidence.append("a jump stub (%04x %s %s)" % (start, first_ins.mnemonic, first_ins.op_str))
        o.language = "assembly"
        return
    first = True
    for ins in md.disasm(image[start:end], start):
        mn = ins.mnemonic
        ops = ins.operands
        try:
            regs_read, regs_written = ins.regs_access()
        except capstone.CsError:
            regs_read, regs_written = [], []
        if first and ins.bytes[:2] == b"\x8c\xd8":                # mov ax, ds (Windows far prologue)
            prologue_ax = True
        first = False
        if mn in ("loop", "loope", "loopne", "jcxz", "jecxz"):
            o.evidence.append("%s (%04x)" % (mn.upper(), ins.address))
        elif mn in _STRING and not (ins.prefix[0] in (0xF2, 0xF3)) and mn not in ("movsb", "movsw", "movsd"):
            o.evidence.append("%s without REP (%04x)" % (mn.upper(), ins.address))
        elif mn == "int":
            o.evidence.append("INT %s (%04x)" % (ins.op_str, ins.address))
        elif mn in _MACHINE:
            o.evidence.append("%s (%04x)" % (_MACHINE[mn], ins.address))
        explicit = [op.reg for op in ops if op.type == X.X86_OP_REG] +                    [r for op in ops if op.type == X.X86_OP_MEM for r in (op.mem.base, op.mem.index)]
        if any(r in _REGS32 for r in explicit):                # (not the stack pointer push and call imply)
            o.evidence.append("32-bit register (%04x %s %s)" % (ins.address, mn, ins.op_str))
        for op in ops:
            # (a JMP through a table in the code segment is how the compiler makes a switch)
            if op.type == X.X86_OP_MEM and op.mem.segment == X.X86_REG_CS and mn != "jmp":
                o.evidence.append("reads its code segment (%04x)" % ins.address)
        if X.X86_REG_SS in regs_written and mn != "pop" or (mn == "pop" and ins.op_str == "ss"):
            o.evidence.append("loads SS (%04x)" % ins.address)
        if X.X86_REG_SP in regs_written and mn == "mov" and ins.op_str != "sp, bp":
            o.evidence.append("moves SP (%04x)" % ins.address)
        if mn == "mov" and ins.op_str == "bp, sp":
            frame = True
        elif X.X86_REG_BP in regs_written and mn not in ("push", "pop", "inc", "dec", "leave", "enter") and not frame:
            o.evidence.append("BP as a plain register (%04x)" % ins.address)
        # registers read before written (arguments in registers)
        for r in regs_read:
            nm = ins.reg_name(r)
            if nm in ("ax", "bx", "cx", "dx", "si", "di", "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh", "es") \
                    and nm[:1] + "x" not in written and nm not in written and mn not in ("push", "xor", "sub") \
                    and not (prologue_ax and nm == "ax") and nm != "es":
                o.evidence.append("argument in %s (%04x)" % (nm.upper(), ins.address))
        for r in regs_written:
            nm = ins.reg_name(r)
            written.add(nm)
            if len(nm) == 2 and nm[1] in "lh":
                written.add(nm[0] + "x")
        if mn in ("call", "lcall"):                           # what a call leaves: its result in DX:AX,
            written.update(("ax", "bx", "cx", "dx", "es"))   # and the registers it may use
        elif mn == "int":
            written.update(("ax", "bx", "cx", "dx", "si", "di", "es"))
        if mn == "call" and ops and ops[0].type == X.X86_OP_IMM:
            target = imported.get(ops[0].imm)
            if target and any(h.lower() in target.lower() for h in _COMPILER_HELPERS):
                o.compiler.append("calls %s" % target)
        # a jump out of the function into the middle of another: code shared between entry
        # points (set a register, jump to the common part) -- no C compiler does that
        if (mn == "jmp" or mn.startswith("j")) and ops and ops[0].type == X.X86_OP_IMM                 and not start <= ops[0].imm < end and ops[0].imm not in starts and starts:
            o.evidence.append("jumps into the middle of other code (%04x -> %04x)" % (ins.address, ops[0].imm))
        if mn in ("ret", "retf", "iret") or mn == "jmp":
            if mn == "iret":
                o.evidence.append("IRET (%04x)" % ins.address)
            if mn in ("ret", "retf"):
                pass

    # de-duplicate, keep the first few kinds
    seen, ev = set(), []
    for e in o.evidence:
        k = e.split(" (")[0]
        if k not in seen:
            seen.add(k)
            ev.append(e)
    o.evidence = ev
    o.language = "assembly" if ev else "c"


def main(argv):
    """python -m codebreaker origin <module>   which functions were written in assembly"""
    from codebreaker.base import corpus
    import os
    if not argv:
        print(main.__doc__)
        return 2
    path = argv[0] if os.path.isfile(argv[0]) else corpus.retail(argv[0])
    r = module16(path)
    n, why = r.summary()
    print("%s: %d functions -- %d C, %d assembly" % (os.path.basename(path), len(r.functions), n["c"], n["assembly"]))
    for k, v in why.most_common(12):
        print("  %4d  %s" % (v, k))
    if "--list" in argv:
        for o in r.functions.values():
            print("  %-28s %-9s %s" % (o.name, o.language, "; ".join(o.evidence[:3] or o.compiler[:2])))
    return 0
