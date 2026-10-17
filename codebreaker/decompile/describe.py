"""describe.py -- take an executable apart and describe it to the decompiler.

    describe32(module_path, addresses, workdir)   a PE module (x86 or PowerPC)
    describe16(module_path, workdir)              a 16-bit Windows module (NE)
    describe_le(image, name, workdir)             a virtual device driver (LE), one member at a time
    describe_dos(module_path, workdir)            a real-mode program (MZ, .COM, driver, IO.SYS)

The native decompiler knows processors, not file formats. A description says which processor,
where the file's bytes sit in memory, and the facts read from the binary rather than guessed:
every function's start and name, how it returns (far or near, how many bytes of arguments it
removes), and every import with the same for it.

A PE module is nearly its own description: sections at their addresses, an import table.
A 16-bit module is a set of segments the loader places and patches, so describing one means
doing what the loader does: each segment gets a selector (segment 1 -> 0x1000, then every 8)
and so an address (selector * 0x10000), every relocation is applied, and a reference to an
imported function becomes a far pointer to an address standing for that function.
"""
import os
import re
import struct

import capstone  # (here, once: imported inside functions, threads of a build raced to a half-made module)


# language id by PE machine type
_ARCH = {0x014C: "x86:LE:32:default:windows", 0x01F2: "PowerPC:BE:64:A2ALT-32addr:homed"}


def _ident(name):
    return re.sub(r"\W", "_", name)


def function_names(module_path, funcs):
    """{va: name} -- an export's own name, else sub_<rva>. A name also used by an import of this
    module stays with the import (the call has to bind to it)."""
    from codebreaker.formats.pe import open_pe
    from codebreaker.build import watcom as B
    pe = open_pe(module_path)
    imported = set(B.import_map(module_path))
    by_rva = {}
    for nm, rva in sorted(pe.export_addrs().items()):
        if re.fullmatch(r"[A-Za-z_]\w*", nm) and nm not in imported:
            by_rva.setdefault(rva, nm)
    return {va: by_rva.get(va - pe.image_base, "sub_%06X" % (va - pe.image_base)) for va in funcs}


def decorated_names(module_path):
    """{address: linker name} for the functions a module exports under a name that is not a C
    name -- a C++ function's decorated name. In the C text such a function is sub_<address>;
    this is the symbol it must be linked as, and symbols/demangle.py makes it a declaration.
    Addresses are as in the description (16-bit: address(segment, offset))."""
    from codebreaker.base import corpus
    plain = re.compile(r"[A-Za-z_]\w*\Z")
    out = {}
    if corpus.lane(module_path) == "ne":
        from codebreaker.formats.ne import NEFile
        nef = NEFile(module_path)
        entries = {e["ordinal"]: e for e in nef.entries() if e.get("kind") in ("FIXED", "MOVEABLE")}
        for nm, ordv in nef.resident_names()[1:] + nef.nonresident_names()[1:]:
            if ordv in entries and not plain.match(nm):
                out.setdefault(address(entries[ordv]["seg"], entries[ordv]["offset"]), nm)
    else:
        from codebreaker.formats.pe import open_pe
        pe = open_pe(module_path)
        for nm, rva in sorted(pe.export_addrs().items()):
            if not plain.match(nm):
                out.setdefault(pe.image_base + rva, nm)
    return out


def import_names16(nef):
    """{symbol: name} for a 16-bit module's imports by number: the symbol the C text and the
    link use (`USER_91`) and the function's real name (`GetDlgItem`), where the exporting
    module or its definition file gives one that is a C name."""
    from codebreaker.symbols import ordinals
    refs = nef.module_refs()
    out = {}
    for s in nef.segments():
        for r in nef.relocations(s):
            if r["kind"] == "IMPORTORD" and 0 < r["module"] <= len(refs):
                symbol = re.sub(r"\W", "_", "%s_%d" % (refs[r["module"] - 1], r["ordinal"]))
                if symbol not in out:
                    out[symbol] = ordinals.identifier(refs[r["module"] - 1], r["ordinal"])
    return {symbol: name for symbol, name in out.items() if name}


def function_starts(module_path):
    """Addresses of a PE module's functions. PowerPC images carry a table of them (.pdata: one
    record per function, its start and length); for x86 they are found by following the code."""
    from codebreaker.formats import powerpc_image
    from codebreaker.formats.pe import open_pe
    if open_pe(module_path).machine == powerpc_image.MACHINE:
        return powerpc_image.read(module_path).functions
    from codebreaker.verify import win32 as pipeline
    return pipeline.LANES["pe"].functions(module_path)


# module file -> {the C name an import by number is called by: (MODULE, number, bytes of arguments
# it removes or None)} -- what describe32 decided, for whoever declares and links those calls
BY_NUMBER = {}
IN_ECX = {}                             # module file -> names of its functions that take an argument in ECX


def describe32(module_path, funcs, outdir):
    """Write the description of a PE module for the native program: its processor, where its
    bytes sit in memory, and the facts about `funcs` (addresses) and its imports.
    -> (path of the description, {va: name})"""
    from codebreaker.symbols import api_arguments as api_stack
    from codebreaker.symbols import ordinals
    from codebreaker.build import watcom as B
    from codebreaker.formats.pe import open_pe
    pe = open_pe(module_path)
    arch = _ARCH.get(pe.machine)
    if not arch:
        raise ValueError("%s: no language for machine type %#x" % (os.path.basename(module_path), pe.machine))
    x86 = arch.startswith("x86:")
    names = function_names(module_path, funcs) if x86 else {va: "sub_%08X" % va for va in funcs}
    lines = ["arch\t%s" % arch]
    if not x86:
        # PowerPC: the image says where its bytes sit, which stubs are imports, and which
        # register save/restore runs are part of their callers (formats/powerpc_image.py)
        from codebreaker.formats import powerpc_image
        img = powerpc_image.read(module_path)
        for va, off, size in img.spans:
            lines.append("bytes\t%x\t%s\t%x\t%x" % (va, os.path.abspath(module_path), off, size))
        for va, name in sorted(img.imports.items()):
            lines.append("X\t%x\t%s\t-1" % (va, name))
        for va, name in sorted(img.inline.items()):
            lines.append("F\t%x\t%s\t-1\tinline" % (va, name))
    else:
        for sec in pe.sections:
            size = min(sec["rawsize"], sec["vsize"] or sec["rawsize"])
            if size and sec["rawptr"]:
                lines.append("bytes\t%x\t%s\t%x\t%x" % (pe.image_base + sec["vaddr"],
                                                        os.path.abspath(module_path), sec["rawptr"], size))
    mod = api_stack._Mod(module_path) if x86 else None
    # Functions that take an argument in ECX and nothing else in a register: a C++ member
    # function's object. Read from the code (ECX is used before anything is put in it), and said
    # to the decompiler, which otherwise shows the object as a variable nobody set -- in the
    # function and, worse, not passed at all by its callers.
    in_ecx, in_edx = set(), set()
    if x86:
        try:
            from codebreaker.analyze import conventions, image
            found = conventions.functions(image.load(module_path))
            in_ecx = {va for va, fact in found.items() if fact.get("convention") == "thiscall"}
            in_edx = {va for va, fact in found.items() if fact.get("convention") == "fastcall"}   # ECX and EDX
        except Exception:
            in_ecx = set()
    IN_ECX[module_path] = {names[va] for va in funcs if va in in_ecx}
    for va in funcs:
        pop = None
        if x86:
            try:
                pop = api_stack._scan(mod, va - pe.image_base, 4)
            except Exception:
                pop = None
        lines.append("F\t%x\t%s\t%d%s" % (va, names[va], -1 if pop is None else pop,
                                             "\t__thiscall" if va in in_ecx and pop is not None else
                                             "\t__fastcall" if va in in_edx and pop is not None else ""))
    # An import by number is called by its real name when the module that exports it (or that
    # module's definition file) gives it one that is a C name nothing else here already has;
    # how many bytes of arguments it takes is read from the export with that number.
    taken = dict.fromkeys(set(B.import_map(module_path)) | set(names.values())) if x86 else {}
    numbered = BY_NUMBER[module_path] = {}
    for slot, (dll, name) in sorted((pe.imports() if x86 else {}).items()):
        stem = os.path.splitext(dll)[0].upper()
        if not name or name.startswith("#"):
            number = int(name[1:]) if name and name[1:].isdigit() else 0
            real = ordinals.identifier(stem, number)
            if real and taken.setdefault(real, (stem, number)) == (stem, number):
                pop = api_stack.header_bytes(real)
                pop = api_stack.stack_bytes(stem, name) if pop is None else pop
                lines.append("I\t%x\t%s\t%d" % (slot, real, -1 if pop is None else pop))
                numbered[real] = (stem, number, pop)
            else:
                # no name for it -- but the module that exports it still says how many bytes of
                # arguments it removes, and without that the call is recovered with none
                pop = api_stack.stack_bytes(stem, name) if name else None
                lines.append("I\t%x\t%s\t%d" % (slot, _ident("%s_ord_%s" % (stem, (name or "#0")[1:])),
                                                -1 if pop is None else pop))
                numbered[_ident("%s_ord_%s" % (stem, (name or "#0")[1:]))] = (stem, number, pop)
            continue
        pop = api_stack.stack_bytes(stem, name) if x86 else None
        lines.append("I\t%x\t%s\t%d" % (slot, _ident(name), -1 if pop is None else pop))
    os.makedirs(outdir, exist_ok=True)
    path = os.path.join(outdir, "description.txt")
    with open(path, "w", newline="\n", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    return path, names


ARCH = "x86:LE:16:Protected Mode:default"
FIRST_SELECTOR = 0x1000
EXTERN_SELECTOR = 0xE000           # imported functions stand at EXTERN_SELECTOR:16*n
# KERNEL's exports that are plain numbers, the same in every protected-mode Windows (by
# ordinal, and by name): a huge pointer's selector step
_KERNEL_CONSTANT = {"KERNEL_113": 3, "__AHSHIFT": 3, "KERNEL_114": 8, "__AHINCR": 8}


def _bare(name):
    return name.upper() if name.upper().startswith("KERNEL_") else name.upper().split(".")[-1]


IMPORT_SELECTOR = 0xD000          # an import used as a selector (KERNEL's __0040H ...): IMPORT_SELECTOR + n


def selector(seg):
    return FIRST_SELECTOR + 8 * (seg - 1)


def address(seg, off):
    return (selector(seg) << 16) + off


def where(va):
    """(segment, offset) of an address inside the module's segments."""
    return ((va >> 16) - FIRST_SELECTOR) // 8 + 1, va & 0xFFFF


def inside(d, va):
    """Is `va` in one of the described module's code segments?"""
    seg, off = where(va)
    return ((va >> 16) - FIRST_SELECTOR) % 8 == 0 and off < d.code.get(seg, 0)


class Description(object):
    def __init__(self, path, names, where, imports):
        self.path, self.names, self.where, self.imports = path, names, where, imports


def _near_call_targets(code):
    """Offsets inside one code segment that two or more `call rel16` sites agree on. A stray
    0xE8 in data does not coincide with another, so an agreed target is a real function start."""
    sites = {}
    n = len(code)
    for i in range(n - 2):
        if code[i] == 0xE8:
            tgt = (i + 3 + struct.unpack_from("<h", code, i + 1)[0]) & 0xFFFF
            if tgt < n:
                sites.setdefault(tgt, set()).add(i)
    return {t for t, s in sites.items() if len(s) >= 2}


def functions(nef):
    """{(segment, offset): "far" | "near"} -- every function start that can be established."""
    segs = {s["idx"]: s for s in nef.segments()}
    out = {}
    for e in nef.entries():
        if e.get("kind") in ("FIXED", "MOVEABLE") and segs.get(e.get("seg"), {}).get("kind") == "CODE":
            out[(e["seg"], e["offset"])] = "far"
    # the module's entry point (a library's LibEntry: what Windows runs when it loads it -- it
    # makes the local heap and sets the module up) is a function, called far by the loader
    cs, ip = nef.h.get("csip", 0) >> 16, nef.h.get("csip", 0) & 0xFFFF
    if cs and segs.get(cs, {}).get("kind") == "CODE":
        out[(cs, ip)] = "far"
    # a code segment nothing above names a function in (reached by its number from outside, or
    # dead) still is one of the module's segments: what it holds starts at 0
    used = {sg for sg, _o in out}
    for s in nef.segments():
        if s["kind"] == "CODE" and s["length"] and s["idx"] not in used:
            out[(s["idx"], 0)] = "far"
    for s in nef.segments():
        if s["kind"] != "CODE":
            continue
        for r in nef.relocations(s):
            if r.get("kind") == "INTERNAL" and r.get("seg") not in (None, 0xFF) \
                    and segs.get(r["seg"], {}).get("kind") == "CODE" and r.get("src") == 3:
                out.setdefault((r["seg"], r.get("entry", 0)), "far")
        code = nef.data[s["file_off"]:s["file_off"] + s["length"]]
        for off in _near_call_targets(code):
            out.setdefault((s["idx"], off), "near")
    return out


def returns(code, off, limit=600, want=None):
    """How the function at `off` returns: ("far" | "near" | None, bytes of arguments removed).

    Read from the function's own first return instruction: `retf` is a far function, `ret` a
    near one, and the operand is what it removes. That is the function's own statement of how
    it must be called; where it was found (entry table, a relocation, a near call) only says
    how SOMEONE calls it. None when no return is met within `limit` instructions."""
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    # The code is followed, not read straight down: functions share epilogues (Windows exports
    # beside each other jump into one `retf 8` that may lie BEFORE them), and straight down from
    # a jump is often someone else's code and someone else's return. The nearest return by the
    # paths the function takes is its own.
    from collections import deque
    # (one instruction at a time, every path abreast: the return fewest instructions away)
    work, seen, n = deque([off]), set(), 0
    while work and n < limit:
        at = work.popleft()
        if not 0 <= at < len(code) or at in seen:
            continue
        seen.add(at)
        ins = next(md.disasm(bytes(code[at:at + 16]), at), None)
        if ins is None:
            continue
        n += 1
        if ins.mnemonic in ("ret", "retf", "lret"):
            kind = "near" if ins.mnemonic == "ret" else "far"
            if want is None or kind == want:
                pop = int(ins.op_str, 0) if ins.op_str else 0
                return kind, pop
            continue
        target = int(ins.op_str, 0) if ins.op_str[:2] == "0x" and ins.op_str[2:].isalnum() else None
        if ins.mnemonic == "push" and target is not None and off < target < len(code):
            work.append(target)                          # a return address it pushes (push 198b; jmp worker)
        if ins.mnemonic == "jmp":
            if target is not None:                       # (through a table or register: not followed)
                work.append(target)
            continue
        if (ins.mnemonic.startswith("j") or ins.mnemonic in ("loop", "loope", "loopne", "jcxz")) and target is not None:
            work.append(target)
        work.append(ins.address + ins.size)
    return None, -1


def popped_by_hand(code, off, kind):
    """Bytes of arguments a function takes off the stack itself at its start: its return
    address popped (one word near, two far), then more words popped, then the return address
    pushed back. -> bytes, or None when it does not start so."""
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    ins = list(md.disasm(bytes(code[off:off + 40]), off))[:20]
    words = 2 if kind == "far" else 1
    regs = ("ax", "bx", "cx", "dx", "si", "di", "bp", "es")

    def quiet(i):                                            # (leaves the stack and the flow alone)
        return i.mnemonic not in ("push", "pop", "call", "lcall", "ret", "retf", "jmp", "ljmp", "int", "enter",
                                  "leave", "pushf", "popf", "pusha", "popa") \
            and not i.mnemonic.startswith("j") and "sp" not in i.op_str
    k = 0
    while k < len(ins) and k < 3 and quiet(ins[k]):          # (IsIconic: `mov cl, 20h / test ax, ...` first)
        k += 1
    pops = []
    while k < len(ins) and ins[k].mnemonic == "pop" and ins[k].op_str in regs:
        pops.append(ins[k].op_str)
        k += 1
    if len(pops) <= words:
        return None
    while k < len(ins) and k < 12 and quiet(ins[k]):         # (GetAsyncKeyState: `xor ah, ah` between)
        if any(r in ins[k].op_str.split(",")[0] for r in pops[:words]):
            return None                                      # (its return address changed: not given back)
        k += 1
    back = [i.op_str for i in ins[k:k + words] if i.mnemonic == "push"]
    # (the return address pushed back as it came: the last popped of it first)
    if back != list(reversed(pops[:words])):
        return None
    return 2 * (len(pops) - words)


def windows_far_prologue(code, off):
    """The prologue of a far function that Windows calls (exports, callbacks): `mov ax,ds` or
    `push ds; pop ax`, `nop`, `inc bp; push bp` -- the odd BP marking a far frame."""
    head = bytes(code[off:off + 6])
    return head[:4] in (b"\x8c\xd8\x90\x45", b"\x1e\x58\x90\x45") or head[:2] == b"\x45\x55"


def result_bytes(code, start, end, passes=None):
    """What a function returns, by its instructions: 2 when on a way to one of its returns the
    last thing to set AX is its own (past the prologue's `mov ax, ds` / `push ds; pop ax` /
    `mov ax, DGROUP` ahead of `mov ds, ax`, and not a `pop ax` -- AX given back as it came) or
    a call (what the callee left there goes back as it came); 4 when DX is set there as well.
    0 when no return is seen to carry anything. Walked back from each return through its
    straight run of instructions."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    md.detail = True
    ins = list(md.disasm(code[start:end], start))
    prologue = next((i.address + i.size for i in ins[:6] if i.mnemonic == "mov" and i.op_str == "ds, ax"), start)
    targets = set()
    for i in ins:
        if i.group(capstone.CS_GRP_JUMP) and i.operands and i.operands[0].type == capstone.CS_OP_IMM:
            targets.add(i.operands[0].imm & 0xFFFF)
    X = capstone.x86
    ax = {X.X86_REG_AX, X.X86_REG_AL, X.X86_REG_AH, X.X86_REG_EAX}
    dx = {X.X86_REG_DX, X.X86_REG_DL, X.X86_REG_DH, X.X86_REG_EDX}
    # every way into each instruction: the one before it (unless that one jumps away or
    # returns) and every jump to it
    at = {i.address: k for k, i in enumerate(ins)}
    into = {}
    for k, i in enumerate(ins):
        if k + 1 < len(ins) and i.mnemonic not in ("jmp", "ljmp", "ret", "retf", "iret"):
            into.setdefault(ins[k + 1].address, []).append(i.address)
        if i.group(capstone.CS_GRP_JUMP) and i.operands and i.operands[0].type == capstone.CS_OP_IMM:
            into.setdefault(i.operands[0].imm & 0xFFFF, []).append(i.address)
    # (a `pop dx` hands DX a value -- `push ax ... pop dx` -- unless the function saves DX
    # itself, when it is DX given back)
    saves_dx = any(i.mnemonic == "push" and i.op_str in ("dx", "edx") for i in ins)
    if passes is not None:                                   # a jump on into another function's
        for i in ins:                                        # body: its result is that one's
            if i.mnemonic == "jmp" and i.operands and i.operands[0].type == capstone.CS_OP_IMM                     and not start <= (i.operands[0].imm & 0xFFFF) < end:
                passes.append(("into", i.operands[0].imm & 0xFFFF))
    def _segment_in_dx(a):
        # Before what sets AX, along the straight run into it: the first thing to set DX is a
        # segment register put there (`mov dx, es`) -- the selector of a far pointer that goes
        # back in DX:AX (KRNL386's `mov es, sel / mov dx, es / mov ax, es:[off]`). A number or
        # a sum there is as often scratch, and is left out.
        for _n in range(8):
            prev = into.get(a, [])
            if len(prev) != 1 or prev[0] not in at or prev[0] < prologue:
                return False
            a = prev[0]
            p = ins[at[a]]
            if p.group(capstone.CS_GRP_CALL):
                return False
            try:
                _r, w = p.regs_access()
            except capstone.CsError:
                return False
            if dx & set(w):
                return p.mnemonic == "mov" and p.op_str.split(", ")[-1] in ("es", "ds", "ss", "cs")
        return False

    best = 0
    for i in ins:
        if i.mnemonic not in ("ret", "retf"):
            continue
        # back from the return along every way into it, to what last set AX on each
        # (DX set too makes it four bytes only right at the end -- `mov ax, lo / mov dx, hi` --
        # or by what sets AX itself: DX on the way is as often a scratch register)
        work, seen = [(p, False, 0) for p in into.get(i.address, [])], set()
        while work:
            a, wide, steps = work.pop()
            if (a, wide) in seen or a not in at or a < prologue:
                continue
            seen.add((a, wide))
            p = ins[at[a]]
            if p.group(capstone.CS_GRP_CALL):
                if passes is not None:                      # (its size is the callee's: see results())
                    near = p.operands and p.operands[0].type == capstone.CS_OP_IMM and p.bytes[0] == 0xE8
                    passes.append((p.operands[0].imm & 0xFFFF) if near else None)
                    best = max(best, 4 if wide else 0)
                else:
                    best = max(best, 4 if wide else 2)      # the callee's result, passed on
                continue
            try:
                _r, written = p.regs_access()
            except capstone.CsError:
                continue
            if ax & set(written):
                if p.mnemonic != "pop":
                    best = max(best, 4 if wide or (dx & set(written)) or _segment_in_dx(p.address) else 2)
                continue
            w = wide or bool(dx & set(written) and steps < 3 and (p.mnemonic != "pop" or not saves_dx))
            work += [(q, w, steps + (0 if p.mnemonic in ("pop", "leave", "push") else 1)) for q in into.get(a, [])]
    return best


def call_overrides(d):
    """`O` lines: for each call through a pointer (a driver's dispatch table, a callback), the
    words the code pushed for it just before -- the callee is unknown, so the analysis cannot
    tell what it takes, and drops what was pushed. Removed by the callee (Pascal: Windows'
    drivers and callbacks) unless the caller takes them off itself right after (`add sp, n`)."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    md.detail = True
    X = capstone.x86
    by_seg = {}
    for va in d.names:
        sg, off = where(va)
        by_seg.setdefault(sg, []).append((off, va))
    lines = []
    for sg, lst in by_seg.items():
        code = d.segment_bytes.get(sg, b"")
        lst.sort()
        for k, (off, va) in enumerate(lst):
            end = lst[k + 1][0] if k + 1 < len(lst) else len(code)
            ins = list(md.disasm(code[off:end], off))
            targets = {i.operands[0].imm & 0xFFFF for i in ins
                       if i.group(capstone.CS_GRP_JUMP) and i.operands and i.operands[0].type == X.X86_OP_IMM}
            # (what the prologue pushes is saved registers, not arguments: the count stops there)
            prologue = off
            for i in ins[:12]:
                if (i.mnemonic in ("nop", "enter") or i.op_str in ("ax, ds", "bp, sp", "ds, ax")
                        or (i.mnemonic in ("push", "inc") and i.op_str in ("bp", "ds", "si", "di"))
                        or (i.mnemonic == "pop" and i.op_str == "ax")
                        or (i.mnemonic == "mov" and i.op_str.startswith("ax, 0x"))
                        or (i.mnemonic == "sub" and i.op_str.startswith("sp,"))):
                    prologue = i.address + i.size
                else:
                    break
            targets.add(prologue)
            for j, i in enumerate(ins):
                if not i.group(capstone.CS_GRP_CALL) or not i.operands or i.operands[0].type == X.X86_OP_IMM:
                    continue                                 # (a direct call: its callee is described)
                if i.mnemonic == "lcall" and i.operands[0].type == X.X86_OP_IMM:
                    continue
                pushed = 0
                for p in reversed(ins[:j]):
                    if p.mnemonic == "push":
                        pushed += 2 if p.operands and p.operands[0].size == 2 else 4 if p.operands and p.operands[0].size == 4 else 2
                        if p.address in targets:
                            break
                        continue
                    if p.group(capstone.CS_GRP_CALL) or p.group(capstone.CS_GRP_JUMP) or p.mnemonic in ("ret", "retf", "pop", "leave", "enter") \
                            or p.address in targets:
                        break
                    try:
                        _r, w = p.regs_access()
                    except capstone.CsError:
                        break
                    if X.X86_REG_SP in w:
                        break
                if not pushed:
                    continue
                far = i.mnemonic == "lcall" or (i.mnemonic == "call" and "dword" in i.op_str)
                nxt = ins[j + 1] if j + 1 < len(ins) else None
                caller_pops = nxt is not None and nxt.mnemonic == "add" and nxt.op_str.startswith("sp,")
                model = ("__cdecl16" if caller_pops else "__stdcall16") + ("far" if far else "near")
                lines.append("O\t%x\t%x\t%d\t%s" % (address(sg, i.address), va, pushed, model))
    return lines


def results(d):
    """{address: bytes it returns} for every function described: its own (result_bytes), or for
    one that passes on a callee's result, the callee's -- four bytes passed on are four bytes
    (a near callee in its segment; any other call counts as two)."""
    by_seg = {}
    for va in d.names:
        sg, off = where(va)
        by_seg.setdefault(sg, []).append(off)
    own, passes = {}, {}
    for sg, offs in by_seg.items():
        code = d.segment_bytes.get(sg, b"")
        offs = sorted(offs)
        for k, off in enumerate(offs):
            p = []
            own[(sg, off)] = result_bytes(code, off, offs[k + 1] if k + 1 < len(offs) else len(code), p)
            passes[(sg, off)] = p
    size = dict(own)
    for _round in range(8):                                   # (wrappers of wrappers)
        changed = False
        for place, ps in passes.items():
            for t in ps:
                if isinstance(t, tuple):                     # ("into", offset): the function it runs on in
                    inside = max([o for o in by_seg[place[0]] if o <= t[1]] or [None])
                    got = size.get((place[0], inside), 0) if inside is not None else 0
                    if not got:
                        continue
                else:
                    got = size.get((place[0], t), 0) if t is not None else 2
                    got = got or 2
                if got > size[place]:
                    size[place], changed = got, True
        if not changed:
            break
    return {va: size.get(where(va), 0) for va in d.names}


def model_line(d, va, name, default="near"):
    """The description line for a function at `va`."""
    seg, off = where(va)
    code = d.segment_bytes.get(seg, b"")
    if (seg, off) in getattr(d, "far_called", ()):           # (found later, called far: see describe16)
        default = "far"
    # How the original calls it is the evidence (an export, a far fix-up: far); the nearest
    # return of that kind then says what it removes. With no such evidence, its own nearest
    # return says both.
    if default == "far" or windows_far_prologue(code, off):
        kind, pop = returns(code, off, want="far")
        kind = "far"
    else:
        kind, pop = returns(code, off)
    # A function that removes its own arguments says so in its return (`retf 6`); one that ends
    # in a plain return leaves them to the caller -- unless it took them off by hand first
    # (USER's IsChild: `pop ax; pop dx` its return address, `pop bx; pop cx` its two arguments,
    # `push dx; push ax` the return address back, then a plain `retf`).
    if pop == 0:
        pop = popped_by_hand(code, off, kind or default) or 0
    conv = "__stdcall16" if pop > 0 else "__cdecl16" if pop == 0 else "__stdcall16"
    # what it returns, as its instructions show (where it runs to: the next function's start)
    result = getattr(d, "results", {}).get(va)
    if result is None:                                       # (one found later: on its own)
        later = [o for v in getattr(d, "names", {}) for (sg, o) in [where(v)] if sg == seg and o > off]
        ps = []
        result = result_bytes(code, off, min(later) if later else len(code), ps)
        known = getattr(d, "results", {})
        for t in ps:                                         # a callee's result passed on: its size
            if isinstance(t, tuple):
                continue
            got = known.get(address(seg, t), 0) if t is not None and seg in d.segment_bytes else 2
            result = max(result, got or 2)
    return "F\t%x\t%s\t%d\t%s%s%s" % (va, name, pop, conv, kind or default, "\t%d" % result if result else "")


def describe16(module_path, outdir):
    """Write the description (and the relocated image it points at). -> Description"""
    from codebreaker.formats.ne import NEFile
    nef = NEFile(module_path)
    segs = nef.segments()
    entries = {e["ordinal"]: e for e in nef.entries() if e.get("kind") in ("FIXED", "MOVEABLE")}
    refs = nef.module_refs()
    images = {}
    for s in segs:
        data = bytearray(nef.data[s["file_off"]:s["file_off"] + s["length"]]) if s["file_off"] else bytearray()
        size = max(len(data), min(s["minalloc"], 0x10000))
        images[s["idx"]] = data + bytearray(size - len(data))
    imports, order = {}, []                                  # name -> stand-in address

    def extern(name):
        if name not in imports:
            imports[name] = (EXTERN_SELECTOR << 16) + 16 * len(order)
            order.append(name)
        return imports[name]

    selectors = {}                                           # import name -> its own made-up selector
    used_as = {}                                             # import name -> "segment" | "offset"

    def selector_of(name):
        if name not in selectors:
            selectors[name] = IMPORT_SELECTOR + len(selectors)
        return selectors[name]

    for s in segs:
        img = images[s["idx"]]
        for r in nef.relocations(s):
            kind = r["kind"]
            if kind == "INTERNAL":
                if r["seg"] == 0xFF:                         # by entry-table ordinal
                    e = entries.get(r["entry"])
                    if not e:
                        continue
                    tseg, toff = e["seg"], e["offset"]
                else:
                    tseg, toff = r["seg"], r["entry"]
                tsel = selector(tseg)
            elif kind in ("IMPORTORD", "IMPORTNAME"):
                mod = refs[r["module"] - 1] if 0 < r["module"] <= len(refs) else "?"
                nm = ("%s_%d" % (mod, r["ordinal"])) if kind == "IMPORTORD" else nef.imported_name(r["name_off"])
                a = extern(re.sub(r"\W", "_", nm))
                tsel, toff = a >> 16, a & 0xFFFF
            else:
                continue                                     # floating-point emulator fixups
            # Where the same target is needed in several places the file chains them: each place
            # holds the offset of the next, ending at 0xFFFF. An "additive" record is one place.
            off, hops = r["offset"], 0
            while off != 0xFFFF and off + 1 < len(img) and hops < 0x10000:
                nxt = struct.unpack_from("<H", img, off)[0]
                src = r["src"]
                add = nxt if r["additive"] else 0
                if src == 3 and off + 3 < len(img):          # far pointer: offset, selector
                    struct.pack_into("<HH", img, off, (toff + add) & 0xFFFF, tsel)
                elif src == 2:                               # selector only
                    # an imported selector is a segment of its own: what is read through it is
                    # told apart, and named by its import (not one shared made-up selector)
                    if kind in ("IMPORTORD", "IMPORTNAME"):
                        sel = selector_of(re.sub(r"\W", "_", nm))
                        used_as[re.sub(r"\W", "_", nm)] = "segment"
                    else:
                        sel = tsel
                    struct.pack_into("<H", img, off, sel)
                elif src == 5 and kind in ("IMPORTORD", "IMPORTNAME") and _KERNEL_CONSTANT.get(_bare(nm)) is not None:
                    # a number that is the same in every protected-mode Windows (__AHSHIFT 3,
                    # __AHINCR 8: a huge pointer's step): the number itself -- a made-up one
                    # would be folded into the arithmetic as if it were the value
                    struct.pack_into("<H", img, off, (_KERNEL_CONSTANT[_bare(nm)] + add) & 0xFFFF)
                elif src == 5 and kind in ("IMPORTORD", "IMPORTNAME"):
                    # an import's offset alone: a value KERNEL exports (__0040H, __WINFLAGS ...),
                    # which the loader writes in. A value of its own, so it is told apart
                    struct.pack_into("<H", img, off, (selector_of(re.sub(r"\W", "_", nm)) + add) & 0xFFFF)
                    used_as.setdefault(re.sub(r"\W", "_", nm), "offset")
                elif src == 5:                               # offset only
                    struct.pack_into("<H", img, off, (toff + add) & 0xFFFF)
                elif src == 0:                               # low byte
                    img[off] = (toff + add) & 0xFF
                if r["additive"]:
                    break
                off, hops = nxt, hops + 1

    os.makedirs(outdir, exist_ok=True)
    blob = os.path.abspath(os.path.join(outdir, "describe.bin"))
    lines, pos = ["arch\t%s" % ARCH], 0
    with open(blob, "wb") as f:
        for s in segs:
            img = images[s["idx"]]
            if not img:
                continue
            f.write(img)
            lines.append("bytes\t%x\t%s\t%x\t%x" % (address(s["idx"], 0), blob, pos, len(img)))
            pos += len(img)
    data_seg = nef.h.get("autodata_seg")
    if data_seg:
        lines.append("reg\tDS\t%x" % selector(data_seg))
    exported = {}
    for nm, ordv in nef.resident_names()[1:] + nef.nonresident_names()[1:]:
        e = entries.get(ordv)
        if e and re.fullmatch(r"[A-Za-z_]\w*", nm):
            exported.setdefault((e["seg"], e["offset"]), nm)
    names, where, funcs = {}, {}, []
    # A far call whose offset the linker wrote and whose selector the loader fills in (a fix-up
    # of the selector word alone) calls a far function: `call far seg:off` in the relocated code.
    code_sel = {selector(s["idx"]): s["idx"] for s in segs if s["kind"] == "CODE"}
    far_called = set()
    for s in segs:
        if s["kind"] != "CODE":
            continue
        img = images[s["idx"]]
        i = img.find(b"\x9a")
        while 0 <= i < len(img) - 4:
            o, sl = struct.unpack_from("<HH", img, i + 1)
            if sl in code_sel:
                far_called.add((code_sel[sl], o))
            i = img.find(b"\x9a", i + 1)
    for (seg, off), dist in sorted(functions(nef).items()):
        if (seg, off) in far_called:
            dist = "far"
        va = address(seg, off)
        nm = exported.get((seg, off)) or "sub_%d_%04X" % (seg, off)
        if nm in imports:
            nm = "sub_%d_%04X" % (seg, off)
        names[va], where[va] = nm, (seg, off)
        funcs.append((va, nm, dist))
    d = Description("", names, where, order)
    d.far_called = far_called                                                 # (seg, off) a far call reaches
    d.import_names = import_names16(nef)                                       # symbol -> the import's real name
    d.selector_imports = {sel: (name, used_as.get(name, "segment"))       # made-up value -> (import symbol,
                          for name, sel in selectors.items()}               #   its selector or its offset)
    d.code = {s["idx"]: s["length"] for s in segs if s["kind"] == "CODE"}     # segment -> bytes of code
    d.images = {k: bytes(v) for k, v in images.items()}                       # segment -> its bytes, relocated
    d.data_segment = data_seg or 0
    # (as the loader has them: the stored bytes, then zeros to the size it allocates -- a
    # function's last instruction can end in them: `retf 4` stored as CA 04, its 00 not stored)
    d.segment_bytes = {s["idx"]: bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]])
                       + bytes(max(0, min(s["minalloc"] or 0x10000, 0x10000) - s["length"]))
                       for s in segs if s["kind"] == "CODE" and s["file_off"]}
    d.results = results(d)
    for va, nm, dist in funcs:
        lines.append(model_line(d, va, nm, default=dist))
    if not os.environ.get("CB_NO_CALL_OVERRIDES"):
        lines += call_overrides(d)
    from codebreaker.symbols import api_arguments as api16
    for nm in order:                 # Windows API: far, and the callee removes its arguments --
        size = api16.of(nm)          # how many bytes is read from the module that exports it
        lines.append("X\t%x\t%s\t%d\t__stdcall16far" % (imports[nm], nm, -1 if size is None else size))
    path = os.path.join(outdir, "description.txt")
    with open(path, "w", newline="\n", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    d.path = path
    return d


# Virtual device drivers (LE) ---------------------------------------------------------------------
LE_ARCH = "x86:LE:32:default:windows"
LE_BASE, LE_STRIDE = 0x01000000, 0x01000000      # object n sits at LE_BASE + (n-1) * LE_STRIDE
LE_SLOTS = 0x0F000000                            # one pointer per service called, read as that service


def le_address(obj, off):
    return LE_BASE + (obj - 1) * LE_STRIDE + off


def describe_le(lf, name, outdir):
    """Describe one LE image (`lf`, from formats.le) to the decompiler. -> Description

    A driver's objects are relocatable, so each is given an address of its own and the
    fix-ups the loader would apply are applied: a 32-bit reference to another place in the
    image gets that place's address. Ring-0 code calls the system and other drivers with
    `int 20h` followed by a service number -- six bytes the loader rewrites into a call; they
    are rewritten here into a call through a pointer that stands for that service, so the
    decompiler sees a named call instead of an interrupt and four bytes of nonsense."""
    import struct
    images = {}
    for o in lf.objects:
        b = bytearray(lf.object_bytes(o))
        images[o["idx"]] = b + bytearray(max(0, o["vsize"] - len(b)))
    execs = {o["idx"] for o in lf.objects if o["exec"]}
    starts = set()
    try:
        fixups = lf.fixups()
    except Exception:
        fixups = {}
    for (obj, off), fx in fixups.items():
        img = images.get(obj)
        if img is None or fx is None or off < 0 or off + 4 > len(img):
            continue
        if fx["kind"] != "internal" or fx.get("target") is None or fx["obj"] not in images:
            continue
        target = le_address(fx["obj"], fx["target"])
        if fx.get("srctype") == 7:                           # 32-bit offset
            struct.pack_into("<I", img, off, target & 0xFFFFFFFF)
        elif fx.get("srctype") == 8:                         # 32-bit self-relative (a call or jump)
            struct.pack_into("<I", img, off, (target - (le_address(obj, off) + 4)) & 0xFFFFFFFF)
            if fx["obj"] in execs:
                starts.add(target)
    # service calls: CD 20 <service:16> <device:16>
    services, order = {}, []
    for obj in sorted(execs):
        img = images[obj]
        i = img.find(b"\xCD\x20")
        while i != -1 and i + 6 <= len(img):
            svc, dev = struct.unpack_from("<HH", img, i + 2)
            if 0 < dev < 0x600 and (svc & 0x7FFF) < 0x400:
                key = (dev, svc & 0x7FFF)
                if key not in services:
                    services[key] = LE_SLOTS + 4 * len(order)
                    order.append(key)
                img[i:i + 2] = b"\xFF\x25" if svc & 0x8000 else b"\xFF\x15"   # jmp / call [slot]
                struct.pack_into("<I", img, i + 2, services[key])
                i = img.find(b"\xCD\x20", i + 6)
            else:
                i = img.find(b"\xCD\x20", i + 2)
    # where functions start: the control procedure, self-relative targets, agreed near calls
    try:
        ddb = lf.ddb()
    except Exception:
        ddb = None
    if ddb:
        img = images.get(ddb["obj"])
        at = ddb["rva"] + 4 + 20
        if img is not None and at + 4 <= len(img):
            cp = struct.unpack_from("<I", img, at)[0]
            if any(le_address(o, 0) <= cp < le_address(o, len(images[o])) for o in execs):
                starts.add(cp)
    for obj in sorted(execs):
        img, sites = images[obj], {}
        for i in range(len(img) - 4):
            if img[i] == 0xE8:
                t = (i + 5 + struct.unpack_from("<i", img, i + 1)[0])
                if 0 <= t < len(img):
                    sites.setdefault(t, set()).add(i)
        starts |= {le_address(obj, t) for t, ss in sites.items() if len(ss) >= 2}
    os.makedirs(outdir, exist_ok=True)
    blob = os.path.abspath(os.path.join(outdir, "image_le.bin"))
    lines, pos = ["arch\t%s" % LE_ARCH], 0
    with open(blob, "wb") as f:
        for obj in sorted(images):
            if images[obj]:
                f.write(images[obj])
                lines.append("bytes\t%x\t%s\t%x\t%x" % (le_address(obj, 0), blob, pos, len(images[obj])))
                pos += len(images[obj])
    names = {va: "sub_%d_%05X" % ((va - LE_BASE) // LE_STRIDE + 1, (va - LE_BASE) % LE_STRIDE) for va in sorted(starts)}
    for va in sorted(names):
        lines.append("F\t%x\t%s\t-1" % (va, names[va]))
    from codebreaker.symbols import driver_services
    for dev, svc in order:
        lines.append("I\t%x\t%s\t-1" % (services[(dev, svc)], driver_services.label(dev, svc)))
    d = Description(os.path.join(outdir, "description.txt"), names, {}, [])
    d.spans = {obj: (le_address(obj, 0), len(images[obj])) for obj in execs}
    d.device = (ddb or {}).get("name") or name
    with open(d.path, "w", newline="\n", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    return d


# DOS programs (MZ) -------------------------------------------------------------------------------
DOS_ARCH = "x86:LE:16:Real Mode:default"


def describe_dos(module_path, outdir):
    """Describe a real-mode program to the decompiler. -> Description

    The reader (formats/dos.py) gives the bytes as they sit in memory -- unpacked when the file
    stores them packed, a .COM behind its program prefix, a device driver, the parts of IO.SYS
    each where it runs -- and the analysis (analyze/dos_functions.py) every function start that
    can be established, with whether it returns far or near and how many bytes its return
    removes. The Description also carries `program`, `analysis` and `interrupts`
    ({function name: [interrupt calls]}) so the interrupt calls in the output can be named."""
    from codebreaker.analyze import dos_functions
    from codebreaker.formats import dos
    prog = dos.read(module_path)
    found = dos_functions.discover(prog)
    os.makedirs(outdir, exist_ok=True)
    blob = os.path.abspath(os.path.join(outdir, "image_dos.bin"))
    with open(blob, "wb") as f:
        f.write(prog.image)
    lines = ["arch\t%s" % DOS_ARCH]
    for lo, n, _label in prog.spans:
        lines.append("bytes\t%x\t%s\t%x\t%x" % (lo, blob, lo - prog.base, n))
    lines.append("reg\tDS\t%x" % prog.ds)
    names = {}
    for va in sorted(found.functions):
        fn = found.functions[va]
        names[va] = fn.name
        lines.append("F\t%x\t%s\t%d\t%s%s" % (fn.address, fn.name, fn.removes,
                                               "__stdcall16" if fn.removes > 0 else "__cdecl16", fn.distance))
    d = Description(os.path.join(outdir, "description.txt"), names, {}, [])
    d.spans = {k + 1: (lo, n) for k, (lo, n, _label) in enumerate(prog.spans)}
    d.image, d.base, d.program, d.analysis = prog.image, prog.base, prog, found
    d.interrupts = {}
    for call in found.interrupts:
        if call.function is not None:
            d.interrupts.setdefault(call.function.name, []).append(call)
    with open(d.path, "w", newline="\n", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    return d
