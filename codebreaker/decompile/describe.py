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
def _cpu(name):
    from codebreaker import platforms
    return platforms.named(name).cpu


def _arch(machine):
    """The decompiler's language for a PE image of this machine type (its platform's), or None."""
    from codebreaker import platforms
    p = platforms.for_machine(machine)
    return p.cpu if p else None


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
    from codebreaker.verify.lanes import win32 as pipeline
    return pipeline.LANES["pe"].functions(module_path)


# module file -> {the C name an import by number is called by: (MODULE, number, bytes of arguments
# it removes or None)} -- what describe32 decided, for whoever declares and links those calls
BY_NUMBER = {}
IN_ECX = {}                             # module file -> names of its functions that take an argument in ECX


def _zeros(outdir, size):
    """A file of at least `size` zero bytes in `outdir`, for describing memory the file does not hold."""
    os.makedirs(outdir, exist_ok=True)                   # (the first time, before anything else is there)
    path = os.path.join(outdir, "zeros.bin")
    if not os.path.isfile(path) or os.path.getsize(path) < size:
        with open(path, "wb") as f:
            f.truncate(max(size, 1 << 20))
    return os.path.abspath(path)


def _register_holds_known(img, fn, i, reg):
    """Whether the register a call at fn.insns[i] goes through was last loaded, earlier in the
    function, with an import's address (`mov esi, [slot]`) or a constant (a function's address):
    the core then calls that function, with its own declaration."""
    for p in reversed(fn.insns[:i]):
        if reg not in p.wr:
            continue
        if p.mn == "mov" and len(p.ops) == 2:
            src = p.ops[1]
            if src[0] == "i":
                return True
            if src[0] == "m" and src[1] is None and src[2] is None:   # (an absolute address: no base, no index)
                return src[4] in img.imports
        return False
    return False


def _pointer_calls32(module_path, funcs):
    """`O <site> <function> <bytes> <model>` for each call through a pointer (a register, a
    variable; not an import's slot): the bytes the code pushed just before it, and who takes them
    off -- the caller (`add esp, N` right after: __cdecl) or the callee (__stdcall). The core
    otherwise calls through a pointer with no arguments at all: msjava.dll's
    `pcVar7 = cond ? CoTaskMemAlloc : CoTaskMemRealloc; (*pcVar7)();`, the original having pushed
    one or two, so the callee took off what the rebuild never pushed."""
    from codebreaker.analyze import image
    try:
        img = image.load(module_path)
    except Exception:
        return []
    wanted, out = set(funcs), []
    for fn in img.each():
        if fn.start not in wanted:
            continue
        # (the pushes a function opens with save registers: never a call's arguments)
        opening = 0
        while opening < len(fn.insns) and fn.insns[opening].mn == "push":
            opening += 1
        for i, ins in enumerate(fn.insns):
            if ins.flow != "call" or ins.target is not None or image.destination(img, ins) is not None:
                continue
            if ins.ops and ins.ops[0][0] == "r" and _register_holds_known(img, fn, i, image.family(ins.ops[0][1])):
                continue                                   # (`mov esi, [lstrcatA's slot] / call esi`: the core knows it)
            nxt = fn.insns[i + 1] if i + 1 < len(fn.insns) else None
            if nxt is not None and nxt.mn == "add" and len(nxt.ops) == 2 and nxt.ops[0][0] == "r" \
                    and image.family(nxt.ops[0][1]) == "esp" and nxt.ops[1][0] == "i":
                # the caller takes them off: exactly as many as it says
                out.append("O\t%x\t%x\t%d\t__cdecl" % (ins.addr, fn.start, nxt.ops[1][1]))
                continue
            pushed, k = 0, i - 1
            while k >= max(opening, i - 40):
                p = fn.insns[k]
                if p.mn == "push":
                    pushed += 4
                elif p.flow or p.mn in ("pop", "leave", "enter", "pushal", "popal") or "esp" in p.wr:
                    break
                k -= 1
            if pushed:
                # (a method called through its object's table -- `mov ecx, esi / call [eax+8]` --
                # takes the object in ECX beside what was pushed: __thiscall, or it is lost)
                this = False
                for p in reversed(fn.insns[max(0, i - 12):i]):
                    if p.flow:
                        break
                    if "ecx" in p.wr:
                        this = True
                        break
                out.append("O\t%x\t%x\t%d\t%s" % (ins.addr, fn.start, pushed, "__thiscall" if this else "__stdcall"))
    return out


def describe32(module_path, funcs, outdir):
    """Write the description of a PE module for the native program: its processor, where its
    bytes sit in memory, and the facts about `funcs` (addresses) and its imports.
    -> (path of the description, {va: name})"""
    from codebreaker.symbols import api_arguments as api_stack
    from codebreaker.symbols import ordinals
    from codebreaker.build import watcom as B
    from codebreaker.formats.pe import open_pe
    pe = open_pe(module_path)
    arch = _arch(pe.machine)
    if not arch:
        raise ValueError("%s: no language for machine type %#x" % (os.path.basename(module_path), pe.machine))
    x86 = arch.startswith("x86:")
    if x86:
        names = function_names(module_path, funcs)
    else:                                       # PowerPC: what run-time type information names
        from codebreaker.symbols import names as known
        own = known.powerpc_names(module_path)
        names = {va: own[va][0] if va in own else "sub_%08X" % va for va in funcs}
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
            # The part of a section the file does not hold (.bss; a section's tail past its raw
            # size) is zeros when the module is loaded: described as such, its globals are the
            # image's, and their sizes reach the C (undescribed, every one was declared 4 bytes:
            # setup32's `dec word ptr [40e36c]` stored over the word beside it).
            if (sec["vsize"] or 0) > size:
                lines.append("bytes\t%x\t%s\t0\t%x" % (pe.image_base + sec["vaddr"] + size,
                                                     _zeros(outdir, sec["vsize"] - size), sec["vsize"] - size))
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
        except Exception as e:
            # (said, not swallowed: without it no function takes its object in ECX, and every
            # C++ method of the module loses it -- msjava.dll's analysis stopped on a function
            # of padding alone, and 116 of its functions passed EnterCriticalSection garbage)
            import sys
            print("describe32: %s: calling conventions not found (%s: %s); no function takes ECX or EDX"
                  % (os.path.basename(module_path), type(e).__name__, e), file=sys.stderr)
            in_ecx, in_edx = set(), set()
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
    # (CB_POINTER_CALLS=1: say what each call through a pointer was pushed. Off: on the 32-bit
    # sample it fixed no program and made msjava.dll worse -- 280 unverified to 377 -- where a
    # pointer call's pushes are not all its arguments; to be made right before it is on)
    if x86 and os.environ.get("CB_POINTER_CALLS") == "1":
        lines += _pointer_calls32(module_path, funcs)
    os.makedirs(outdir, exist_ok=True)
    path = os.path.join(outdir, "description.txt")
    with open(path, "w", newline="\n", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    return path, names


ARCH = _cpu("ne")
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
    used = {sg for sg, _o in out}                            # (segments some export starts in: below)
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
        for place in _far_pointer_targets(nef, s, bytes(code), segs):
            out.setdefault(place, "far")
        # a function nothing visibly reaches (through a table, a pointer computed or kept in
        # data): its entry sequence right after another function's end. Taken for that one's
        # tail, it was erased with it when that one was rebuilt from C (USER had 95)
        for off in prologues_after_ends(bytes(code)):
            how = returns(bytes(code), off)[0]
            out.setdefault((s["idx"], off), how or "far")
    # a code segment nothing above names a function in (reached by its number from outside, or
    # dead) still is one of the module's segments: what it holds starts at 0 -- far or near as
    # how it is reached says (a far fix-up, a near call: above), else as its own return says.
    # (SUWIN's 7:0000 is a near helper -- `ret`, called `call 0` -- and taken for far, its C
    # caller called it far and lost a word to the stack on every call: ten in a loop, then a
    # `retf` into nothing, Illegal Instruction)
    for s in nef.segments():
        if s["kind"] == "CODE" and s["length"] and s["idx"] not in used and (s["idx"], 0) not in out:
            code = bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]]) if s["file_off"] else b""
            out[(s["idx"], 0)] = returns(code, 0)[0] or "far"
    # what the code calls, followed from every start known -- one call is enough when it is in
    # code that is followed (a stray E8 in data is not)
    codes = {s["idx"]: bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]])
             for s in nef.segments() if s["kind"] == "CODE" and s["file_off"]}
    done = set()
    while True:
        new = [(sg, o) for (sg, o) in out if (sg, o) not in done and sg in codes]
        if not new:
            break
        for sg, o in new:
            done.add((sg, o))
            for t in _calls_followed(codes[sg], o):
                out.setdefault((sg, t), "near")
    return out


# The entry sequences of Win16 functions (the compilers' and the hand-written ones' alike)
_PROLOGUES = (b"\x55\x8b\xec",                 # push bp; mov bp, sp
              b"\x45\x55\x8b\xec",             # inc bp; push bp; mov bp, sp   (a far frame)
              b"\x1e\x58\x90",                 # push ds; pop ax; nop          (exported, as in the file)
              b"\x8c\xd8\x90")                 # mov ax, ds; nop
_ENDS = ("ret", "retf", "iret", "jmp", "ljmp")

# The instruction at an offset of a code segment, decoded once. Finding functions, their returns
# and the stubs in front of them walks the same bytes again and again, one instruction at a time:
# COMPOBJ's 75,000 instructions were decoded 752,000 times, a quarter of describing it. Kept per
# segment (the object itself, held so its id is not reused), with a decoder per thread.
_DECODED = {}
_DECODER = __import__("threading").local()

def decoder16(detail=False):
    """A 16-bit x86 decoder (capstone), one of each kind per thread and kept: making one is
    a thousand times dearer than decoding an instruction with it, and describing USER made
    17,000 of them. `detail`: with each instruction's operands and registers."""
    key = "md16_detail" if detail else "md16"
    md = getattr(_DECODER, key, None)
    if md is None:
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
        md.detail = bool(detail)
        setattr(_DECODER, key, md)
    return md



def _one(code, at):
    """The 16-bit instruction at `at` in `code` (as `next(md.disasm(code[at:at + 16], at))`), or None."""
    held = _DECODED.get(id(code))
    if held is None or held[0] is not code:
        held = _DECODED[id(code)] = (code, {})
    seen = held[1]
    if at in seen:
        return seen[at]
    md = getattr(_DECODER, "md", None)
    if md is None:
        md = _DECODER.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    ins = seen[at] = next(md.disasm(bytes(code[at:at + 16]), at), None)
    return ins


def _is_prologue(code, o):
    if any(code.startswith(p, o) for p in _PROLOGUES):
        return True
    if code[o:o + 1] == b"\xc8" and o + 4 <= len(code) and code[o + 3] in (0, 1):   # enter n, 0|1
        return True
    return code[o:o + 1] == b"\xb8" and code.startswith(b"\x45\x55\x8b\xec", o + 3)  # mov ax, DGROUP; inc bp ...


def prologues_after_ends(code):
    """Offsets in one code segment where an entry sequence follows the end of code (a return or
    a jump, then padding): where a function begins, whether or not anything seen reaches it."""
    md = decoder16()
    out, at, n = [], 0, len(code)
    while at < n:
        ins = _one(code, at)
        if ins is None:
            at += 1
            continue
        at += ins.size
        if ins.mnemonic in _ENDS:
            p = at
            while p < n and code[p] in (0x00, 0x90, 0xCC):
                p += 1
            if p < n and _is_prologue(code, p):
                out.append(p)
    return out


def _calls_followed(code, start, limit=20000):
    """Near call targets in the code reached from `start` (fall-through and jumps followed)."""
    md = decoder16()
    calls, seen, work, n = set(), set(), [start], len(code)
    while work and len(seen) < limit:
        at = work.pop()
        while 0 <= at < n and at not in seen:
            ins = _one(code, at)
            if ins is None:
                break
            seen.add(at)
            b0 = ins.bytes[0]
            if b0 == 0xE8 and ins.size == 3:
                t = (at + 3 + struct.unpack_from("<h", bytes(ins.bytes), 1)[0]) & 0xFFFF
                if t < n:
                    calls.add(t)
            elif b0 in (0xEB, 0xE9) or 0x70 <= b0 <= 0x7F or 0xE0 <= b0 <= 0xE3 \
                    or (b0 == 0x0F and 0x80 <= ins.bytes[1] <= 0x8F):
                try:
                    t = int(ins.op_str, 16) & 0xFFFF
                    if t < n:
                        work.append(t)
                except ValueError:
                    pass
            if ins.mnemonic in _ENDS:
                break
            at += ins.size
    return calls


def hidden_functions(nef, known):
    """The guard: entry sequences after an end that are not among `known` {(segment, offset)} --
    functions the description does not have, so that the build would take them for the tail of
    the function before. [(segment, offset)]"""
    out = []
    for s in nef.segments():
        if s["kind"] != "CODE" or not s["file_off"]:
            continue
        code = bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]])
        out += [(s["idx"], o) for o in prologues_after_ends(code) if (s["idx"], o) not in known]
    return out


def _far_pointer_targets(nef, s, code, segs):
    """Places in the module's code segments a far pointer names whose offset is a plain number
    beside a selector fix-up: `call far` / `jmp far` (9A/EA, the offset inline), a pointer
    pushed (`push seg X` / `push n`), or stored as two words side by side (`mov [bp+d], n` /
    `mov [bp+d+2], seg X`, either order). Such a place is a function -- often one nothing calls
    by a near call and the module does not export: a callback (USER's 1:3452, passed to an
    enumeration from 1:36E8). Not seen, it was taken for the tail of the function before it."""
    from codebreaker.build import data as data16
    out = set()
    n = len(code)
    for o, src, _add, r in data16._places(nef, s, code):
        if src != 2 or r.get("kind") != "INTERNAL" or r.get("seg") in (None, 0xFF):
            continue
        t = r["seg"]
        if segs.get(t, {}).get("kind") != "CODE":
            continue
        offs = []
        if o >= 3 and code[o - 3] in (0x9A, 0xEA):                         # call/jmp far off:seg
            offs.append(struct.unpack_from("<H", code, o - 2)[0])
        if o >= 1 and code[o - 1] == 0x68 and o + 5 <= n and code[o + 2] == 0x68:   # push seg / push off
            offs.append(struct.unpack_from("<H", code, o + 3)[0])
        if o >= 3 and code[o - 3] == 0xC7 and code[o - 2] == 0x46:          # mov [bp+d], seg ...
            d = code[o - 1]
            if o >= 8 and code[o - 8] == 0xC7 and code[o - 7] == 0x46 and code[o - 6] == (d - 2) & 0xFF:
                offs.append(struct.unpack_from("<H", code, o - 5)[0])     # ... after mov [bp+d-2], off
            if o + 7 <= n and code[o + 2] == 0xC7 and code[o + 3] == 0x46 and code[o + 4] == (d - 2) & 0xFF:
                offs.append(struct.unpack_from("<H", code, o + 5)[0])     # ... before it
        for v in offs:
            if 0 <= v < segs[t]["length"]:
                out.add((t, v))
    return out


def returns(code, off, limit=600, want=None):
    """How the function at `off` returns: ("far" | "near" | None, bytes of arguments removed).

    Read from the function's own first return instruction: `retf` is a far function, `ret` a
    near one, and the operand is what it removes. That is the function's own statement of how
    it must be called; where it was found (entry table, a relocation, a near call) only says
    how SOMEONE calls it. None when no return is met within `limit` instructions."""
    md = decoder16()
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
        ins = _one(code, at)
        if ins is None:
            continue
        n += 1
        if ins.mnemonic in ("ret", "retf", "lret"):
            kind = "near" if ins.mnemonic == "ret" else "far"
            pop = int(ins.op_str, 0) if ins.op_str else 0
            if pop & 1 or pop > 0x7E:
                continue                                 # (no function removes an odd count or so many:
                #                                          bytes that are not code, decoded -- USER 24:09B8)
            if want is None or kind == want:
                return kind, pop
            continue
        target = int(ins.op_str, 0) if ins.op_str[:2] == "0x" and ins.op_str[2:].isalnum() else None
        if ins.mnemonic == "push" and target is not None and off < target < len(code):
            # a return address it pushes -- the stubs push the place of a `retf N` (KRNL386's
            # LoadResource: push 13Dh ... jmp worker; at 13Dh, retf 4) -- only when a return is
            # what is there: a constant pushed as an argument is not one (USER sub_24_00F6's
            # `push 98Bh` was followed into the function after it, to a `ret 0EEBh` in its bytes,
            # and the dialog builder was called with two words fewer than it takes)
            # (or a few straight instructions before it: GetMessage's is `mov ax, -1` / `retf 0Ah`;
            # or a shared tail with a frame of its own: GetWindowText pushes AB36h, `push bp /
            # mov bp, sp ...`, which ends in its `retf 8`)
            if _is_prologue(bytes(code), target):
                work.append(target)
            for k, there in enumerate(md.disasm(bytes(code[target:target + 16]), target)):
                if there.mnemonic in ("ret", "retf"):
                    work.append(target)
                    break
                if k >= 2 or there.mnemonic.startswith("j") or there.mnemonic in ("call", "lcall"):
                    break
        if ins.mnemonic == "jmp":
            if target is not None:                       # (through a table or register: not followed)
                work.append(target)
            continue
        if ins.mnemonic in ("ljmp", "iret"):
            continue                                     # (a far jump leaves: what is after it is someone else's)
        if (ins.mnemonic.startswith("j") or ins.mnemonic in ("loop", "loope", "loopne", "jcxz")) and target is not None:
            work.append(target)
        work.append(ins.address + ins.size)
    return None, -1


def callers_say(d):
    """{(seg, off): "callee" | "caller"} -- who removes a function's arguments, as the original's
    own calls to it show: every call that pushed something before it and is followed by
    `add sp, n` says the caller does; every one that is not says the callee does. Only a function
    whose calls (two at least) all say the same is in it. The callers are the original program's
    own word on how each callee is called -- the callee's instructions can hide it (arguments
    taken off by hand, a shared epilogue)."""
    md = decoder16()
    votes = {}
    for seg, img in d.images.items():
        ins = list(md.disasm(bytes(img), 0))
        for k, i in enumerate(ins[:-1]):
            if i.mnemonic == "call" and i.op_str.startswith("0x") and "," not in i.op_str:
                target = (seg, int(i.op_str, 16) & 0xFFFF)
            elif i.mnemonic == "lcall" and "," in i.op_str and i.op_str.startswith("0x"):
                s, o = (int(x, 16) for x in i.op_str.split(", "))
                target = where((s << 16) | o) if s < 0xE000 else None
            else:
                continue
            if target is None or k == 0 or ins[k - 1].mnemonic != "push":
                continue                                     # (nothing pushed just before: says nothing)
            nxt = ins[k + 1]
            who = "caller" if nxt.mnemonic == "add" and nxt.op_str.startswith("sp, ") else "callee"
            votes.setdefault(target, []).append(who)
    return {t: v[0] for t, v in votes.items() if len(v) >= 2 and len(set(v)) == 1}


class _Into(object):
    """A near jump forwards() followed: where it went."""
    mnemonic, op_str, size = "into", "", 0

    def __init__(self, address):
        self.address = address


def forwards(d, seg, off, kind):
    """Bytes of arguments a thunk's caller has removed, for a thunk that puts more arguments
    under its return address and jumps on: `pop bx / pop dx` (the return address), `push`es (the
    words it adds), `push dx / push bx`, then `jmp` / `ljmp` to the function that does the work
    (USER's 1:0110 adds a 0 and goes to BeginPaint's body at 1:75C8). What the target removes,
    less the words the thunk added. -> bytes, or None when it is not such a thunk."""
    code = (getattr(d, "images", None) or {}).get(seg) or d.segment_bytes.get(seg, b"")   # (relocated: a far jump's segment as the placeholder)
    md = decoder16()
    # (read along the way it runs: a near jump inside the segment followed, not listed -- USER's
    # 1:2440 `pop ax / pop dx / xor cx, cx / jmp 244C` into 1:2446's `push cx / push dx / push
    # ax`, then on into the body they share, which ends `retf 0Eh`: its callers push six words,
    # not seven, and the C passed DS for a seventh)
    ins, at = [], off
    while len(ins) < 30 and 0 <= at < len(code):
        i = _one(code, at)
        if i is None:
            break
        if i.mnemonic == "jmp" and re.fullmatch(r"0x[0-9a-f]+", i.op_str) and len(ins) > 0:
            at = int(i.op_str, 16) & 0xFFFF
            ins.append(_Into(at))                        # (where it went: the body, if it is next)
            continue
        ins.append(i)
        at = i.address + i.size
    # (the body's address first put in a register, the rest shared: Win95 USER's entries
    # `mov cx, 2FAh / jmp 6Bh`, and at 6B `pop eax / push 0 / push eax / jmp cx`)
    loaded = {}
    while ins and (ins[0].mnemonic == "into" or ins[0].mnemonic == "mov"
                   and re.fullmatch(r"(ax|bx|cx|dx), 0x[0-9a-f]+", ins[0].op_str)):
        if ins[0].mnemonic == "mov":
            r, v = ins[0].op_str.split(", ")
            loaded[r] = int(v, 16)
        ins = ins[1:]
    words = 2 if kind == "far" else 1
    regs = ("ax", "bx", "cx", "dx", "si", "di", "es")
    # (a far return address taken off whole, in one 32-bit register: Win95 USER's 5:0011
    # (lstrcmpi) `pop eax / push word es:[90h] / push eax / jmp 6E1` -- its callers push four
    # words, not the five the body's `retf 0Ah` removes, and the C passed a sixth and a seventh)
    if kind == "far" and ins and ins[0].mnemonic == "pop" and ins[0].op_str in ("eax", "ebx", "ecx", "edx"):
        words = 1
    elif any(i.mnemonic != "pop" or i.op_str not in regs for i in ins[:words]):
        return None
    if len(ins) < 2 * words + 2:
        return None
    ret = [i.op_str for i in ins[:words]]
    k, added, taken = words, 0, 0
    wide = regs + ("eax", "ebx", "ecx", "edx", "esi", "edi")
    # (and the caller's arguments it takes off, before adding its own or among them -- USER's
    # 1:014C pops one, pushes 0 and 1 or 5 (a `jcxz` choosing), puts the return address back,
    # sets slots in the stack and goes to 1:3E1E: its callers' stack came out four bytes short in
    # the gate; Win95 USER.88 (EndDialog) `pop ecx / pop ax / cwde / push eax / push ecx`, its int
    # widened to a long, and USER's 8:43D6 `pop eax / pop bx / xor ecx, ecx / pop cx / push ecx`)
    _writes = {"cbw": "ax", "cwde": "eax", "cwd": "dx", "cdq": "edx"}

    def quiet(i):                                            # (moves nothing on the stack)
        if i.mnemonic in _writes:                            # (widening in a register)
            return not any(r in _writes[i.mnemonic] or _writes[i.mnemonic] in r for r in ret)
        return i.mnemonic in ("xor", "mov", "sub", "cmp", "test", "and", "or", "movzx", "movsx") \
            and "sp" not in i.op_str.split(",")[0] \
            or (i.mnemonic in ("jcxz", "je", "jne") and re.fullmatch(r"0x[0-9a-f]+", i.op_str)
                and i.address < int(i.op_str, 16) <= i.address + 0x20)
    while k < len(ins) and (ins[k].mnemonic in ("push", "pop", "into") or quiet(ins[k])) and \
            not (ins[k].mnemonic == "push" and ins[k].op_str == ret[-1]):
        if ins[k].mnemonic == "into":                        # (a jump followed: it goes on there)
            k += 1
            continue
        if ins[k].mnemonic == "pop":
            if ins[k].op_str not in wide or ins[k].op_str in ret:
                return None
            taken += 2 if ins[k].op_str.startswith("e") and len(ins[k].op_str) == 3 else 1
        elif ins[k].mnemonic == "push":                      # (a 32-bit push is two words: GDI.612's
            added += 2 if 0x66 in ins[k].bytes[:2] else 1     # `push 0` is 66 68 00000000)
        elif any(r in ins[k].op_str.split(",")[0] for r in ret):
            return None                                      # (its return address changed)
        k += 1
    back = [i.op_str for i in ins[k:k + words] if i.mnemonic == "push"]
    if back != list(reversed(ret)) or not added or k + words >= len(ins):
        return None
    k += words
    while k < len(ins) - 1 and quiet(ins[k]):                # (slots set beneath, then the jump)
        k += 1
    if k >= len(ins):
        return None
    j = ins[k]
    if j.mnemonic == "ljmp" and re.fullmatch(r"0x[0-9a-f]+(, |:)0x[0-9a-f]+", j.op_str):
        s, o = (int(x, 16) for x in re.split(r", |:", j.op_str))
        if s >= 0xE000:
            return None                                      # (another module's: not known here)
        tseg, toff = where((s << 16) | o)
    elif j.mnemonic == "into":                               # (a near jump, followed: the body there)
        tseg, toff = seg, j.address
    elif j.mnemonic == "jmp" and j.op_str in loaded:         # (to the address put in the register)
        tseg, toff = seg, loaded[j.op_str]
    elif j.mnemonic in ("mov", "enter", "push", "inc", "nop"):
        tseg, toff = seg, j.address                          # (no jump: the body goes on right here)
    else:
        return None
    _k, pop = returns(d.segment_bytes.get(tseg, b""), toff, want=kind)
    if pop is None or pop < 0:
        return None
    return max(0, pop - 2 * added + 2 * taken)


def popped_by_hand(code, off, kind):
    """Bytes of arguments a function takes off the stack itself at its start: its return
    address popped (one word near, two far), then more words popped, then the return address
    pushed back. -> bytes, or None when it does not start so."""
    md = decoder16()
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
    if k < len(ins) and ins[k].mnemonic == "call" and ins[k].op_str.startswith("0x") and "," not in ins[k].op_str:
        # a helper does it for it (USER's PostQuitMessage, GetSysColor: `call 8128`, which pops
        # its own return address, this one's, and the arguments, pushes this one's back and
        # `jmp bx` home): the helper's pops past its own return address are this function's
        helper = int(ins[k].op_str, 16) & 0xFFFF
        if helper != off:
            h = list(md.disasm(bytes(code[helper:helper + 40]), helper))[:20]
            hp = []
            j = 0
            while j < len(h) and h[j].mnemonic == "pop" and h[j].op_str in regs:
                hp.append(h[j].op_str)
                j += 1
            if len(hp) > 1 + words:
                while j < len(h) and j < 14 and quiet(h[j]):
                    j += 1
                back = [i.op_str for i in h[j:j + words] if i.mnemonic == "push"]
                rest = h[j + words:j + words + 6]
                home = any(i.mnemonic == "jmp" and i.op_str == hp[0] for i in rest)
                if back == list(reversed(hp[1:1 + words])) and home:
                    return 2 * (len(hp) - 1 - words)
        return None
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


def args_read(code, start, end, kind):
    """The bytes of arguments a function reads -- through BP, after the prologue sets it
    (`enter`, or `push bp / mov bp, sp`) -- or None when it keeps no such frame (its reads are
    not known here)."""
    md = decoder16(detail=True)
    X = capstone.x86
    if windows_far_prologue(code, start):
        return None
    ins = list(md.disasm(bytes(code[start:min(end, start + 0x4000)]), start))
    if ins and ins[0].mnemonic == "enter":
        k = 1
    elif len(ins) > 1 and ins[0].mnemonic == "push" and ins[0].op_str == "bp" and \
            ins[1].mnemonic == "mov" and ins[1].op_str == "bp, sp":
        k = 2
    else:
        return None
    first = 6 if kind == "far" else 4                        # (past the saved BP and the return address)
    most = 0
    for i in ins[k:]:
        if i.mnemonic in ("mov", "lea", "pop", "xchg") and i.op_str.startswith("bp"):
            break                                            # (BP made something else: no longer the frame)
        for op in i.operands:
            if op.type == X.X86_OP_MEM and op.mem.base == X.X86_REG_BP and op.mem.index == 0 and \
                    op.mem.segment in (0, X.X86_REG_SS) and op.mem.disp >= first:
                # (a far pointer loaded or called through is four bytes, whatever Capstone says)
                size = 2 if i.mnemonic == "lea" else \
                    4 if i.mnemonic in ("les", "lds", "lss", "lfs", "lgs", "lcall", "ljmp") else op.size
                most = max(most, op.mem.disp + max(size, 2) - first)
    return most + (most & 1)


def windows_far_prologue(code, off):
    """The prologue of a far function that Windows calls (exports, callbacks): `mov ax,ds` or
    `push ds; pop ax`, `nop`, `inc bp; push bp` -- the odd BP marking a far frame."""
    head = bytes(code[off:off + 6])
    return head[:4] in (b"\x8c\xd8\x90\x45", b"\x1e\x58\x90\x45") or head[:2] == b"\x45\x55"


def result_bytes(code, start, end, passes=None, voided=None):
    """What a function returns, by its instructions: 2 when on a way to one of its returns the
    last thing to set AX is its own (past the prologue's `mov ax, ds` / `push ds; pop ax` /
    `mov ax, DGROUP` ahead of `mov ds, ax`, and not a `pop ax` -- AX given back as it came) or
    a call (what the callee left there goes back as it came); 4 when DX is set there as well.
    0 when no return is seen to carry anything. Walked back from each return through its
    straight run of instructions.
    0 as well when a way to a return sets AX nowhere past that prologue: AX is then the data
    segment the prologue loaded -- a register as it came, not a result -- and a function that
    returns nothing on one way returns nothing at all (LZEXPAND's LZDONE: `dec word ptr [32h] /
    jne` straight to the return, the other way copying globals through AX) -- unless on another
    way AX is set and not read again before the return (LMOUSE31's CPLAPPLET: `mov ax, 0 / jmp`
    to the return; DS on its other way is a nonzero answer as well). `voided`, given a
    list, has what the other ways would have made it appended (results() keeps that where a
    caller reads AX after all), and `passes` is emptied."""
    import capstone
    md = decoder16(detail=True)
    ins = list(md.disasm(code[start:end], start))
    # (Windows' own prologue loads DS seventh or eighth: `mov ax, ds` or `push ds; pop ax`, `nop`,
    # `inc bp; push bp; mov bp, sp; push ds; mov ds, ax`)
    windows = windows_far_prologue(code, start)
    head = [i for i in ins[:8 if windows else 6] if i.mnemonic == "mov" and i.op_str == "ds, ax"]
    prologue = head[0].address + head[0].size if head else start
    # (AX there is the data segment -- the caller's, which Windows makes the module's own, or
    # DGROUP's `mov ax, seg` -- not an argument loaded into DS)
    k = ins.index(head[0]) if head else 0
    ds_in_ax = bool(head) and (windows or k > 0 and ins[k - 1].mnemonic == "mov" and
                               re.fullmatch(r"ax, (0x[0-9a-f]+|\d+)", ins[k - 1].op_str) is not None)
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
    def _reads_dx(p, r):
        return p.mnemonic == "int" or bool(dx & set(r))      # (an `int` reads what it is handed: DOS's DS:DX)

    def _read_after(a):
        # DX read from what sets AX on to the return, along the way it runs (a conditional
        # jump's fall-through; a jump followed): USER's 21:58E6 `mov ds, dx`, GDI's 4:168F
        # `cmp dx, di` -- DX still in use there, not handed back
        for _n in range(24):
            if a not in at:
                return True
            p = ins[at[a]]
            if p.mnemonic in ("ret", "retf"):
                return False
            try:
                if _reads_dx(p, p.regs_access()[0]):
                    return True
            except capstone.CsError:
                return True
            if p.group(capstone.CS_GRP_CALL):
                return True
            a = p.operands[0].imm & 0xFFFF if p.mnemonic == "jmp" and p.operands and \
                p.operands[0].type == capstone.CS_OP_IMM else p.address + p.size
        return True

    def _segment_in_dx(a):
        # Before what sets AX, along the straight run into it: the first thing to set DX is a
        # segment register put there (`mov dx, es`) -- the selector of a far pointer that goes
        # back in DX:AX (KRNL386's `mov es, sel / mov dx, es / mov ax, es:[off]`) -- or an
        # argument nothing reads again before the return (SUWIN's 3:319C `mov ax, [bp+6] /
        # mov dx, [bp+8] / ... sub ax, cx / add ax, 1FEh / leave / retf`: a pointer into the
        # segment it was given; a compiler writes no DX it will not use). A number or a sum
        # there is as often scratch, and is left out.
        p = ins[at[a]] if a in at else None
        try:
            read = p is None or _reads_dx(p, p.regs_access()[0]) or _read_after(a)
        except capstone.CsError:
            read = True
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
                src = p.op_str.split(", ")[-1] if p.mnemonic == "mov" else ""
                return src in ("es", "ds", "ss", "cs") or (src.startswith("word ptr [bp + ") and not read)
            read = read or _reads_dx(p, _r)                  # (DX read on the way: scratch)
        return False

    # (`bare`: a way to a return sets no AX past the prologue; `evident`: on one, what sets AX
    # last is not read again before the return -- `mov ax, 0 / jmp done` -- a value set for the
    # caller, where one read on the way -- `mov ax, [34h] / mov [44h], ax` -- was a scratch copy)
    best, bare, evident = 0, False, False
    for i in ins:
        if i.mnemonic not in ("ret", "retf"):
            continue
        # back from the return along every way into it, to what last set AX on each
        # (DX set too makes it four bytes only right at the end -- `mov ax, lo / mov dx, hi` --
        # or by what sets AX itself: DX on the way is as often a scratch register)
        work, seen = [(p, False, 0, False, False) for p in into.get(i.address, [])], set()
        while work:
            a, wide, steps, opaque, used = work.pop()
            if a in at and ds_in_ax and a < prologue and not opaque:
                bare = True                                  # (into the prologue, AX still its DS)
            if (a, wide, opaque, used) in seen or a not in at or a < prologue:
                continue
            seen.add((a, wide, opaque, used))
            p = ins[at[a]]
            # (an interrupt's results are not all in what Capstone says it writes)
            opaque = opaque or p.mnemonic in ("int", "into", "int3")
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
                    evident = evident or not used
                continue
            w = wide or bool(dx & set(written) and steps < 3 and (p.mnemonic != "pop" or not saves_dx))
            u = used or bool(ax & set(_r))
            work += [(q, w, steps + (0 if p.mnemonic in ("pop", "leave", "push") else 1), opaque, u)
                     for q in into.get(a, [])]
    if bare and not evident and (best or passes):
        if voided is not None:
            voided.append(best or 2)
        if passes is not None:
            del passes[:]
        return 0
    return best


_UNSIGNED_BOUND = {"ja": 1, "jbe": 1, "jae": 0, "jb": 0}     # index <= bound (1) or < bound (0)


def jump_tables(d):
    """`J` lines: each switch jump through a table in its own code segment, `jmp word ptr
    cs:[reg + table]`, with the places its table sends to. The analysis finds the table by the
    range it can prove for the index -- and the range it proves can be wrong: USER's 1:03A5
    indexes by a byte offset known to be even (the code tested the low four bits, then shifted),
    the analysis took odd offsets too, read garbage words as places, and followed them through
    half the segment (sub_1_032E: 1.5 GB). Here the table is read as the code bounds it: the
    compare that guards the jump (`cmp reg, n` / `ja`), the index a byte offset unless doubled
    before (`shl bx, 1` / `add bx, bx`) -- and no further than where the code it sends to begins."""
    import capstone
    md = decoder16()
    lines = []
    for sg, code in d.segment_bytes.items():
        at = 0
        while True:
            at = code.find(b"\x2e\xff", at)
            if at < 0:
                break
            ins = next(md.disasm(code[at:at + 8], at), None)
            at += 1
            mm = re.fullmatch(r"word ptr cs:\[(bx|si|di) \+ 0x([0-9a-f]+)\]", ins.op_str if ins else "")
            if not ins or ins.mnemonic != "jmp" or not mm:
                continue
            reg, table = mm.group(1), int(mm.group(2), 16)
            # the guard: back from the jump, the last conditional branch on an unsigned compare
            back = []
            for start in range(max(0, ins.address - 24), ins.address):   # (a decode in step with the jump)
                cand = list(md.disasm(code[start:ins.address], start))
                if cand and cand[-1].address + cand[-1].size == ins.address \
                        and sum(c.size for c in cand) == ins.address - start:
                    back = cand
                    break
            if not back:
                continue
            bound, doubled, step = None, False, 2
            for k in range(len(back) - 1, -1, -1):
                i = back[k]
                if i.mnemonic in ("shl", "sal") and i.op_str == "%s, 1" % reg or \
                        i.mnemonic == "add" and i.op_str == "%s, %s" % (reg, reg):
                    doubled = True
                if i.mnemonic in _UNSIGNED_BOUND and k > 0 and back[k - 1].mnemonic == "cmp":
                    m2 = re.fullmatch(r"\w+, (0x[0-9a-f]+|\d+)", back[k - 1].op_str)
                    if m2:
                        bound = int(m2.group(1), 0) + _UNSIGNED_BOUND[i.mnemonic] - 1   # (largest index)
                    break
            if bound is None or bound < 0 or bound > 0x400:
                continue
            count = bound + 1 if doubled else bound // 2 + 1
            targets = []
            for k in range(count):
                p = table + 2 * k
                if p + 2 > len(code):
                    break
                t = struct.unpack_from("<H", code, p)[0]
                if table <= t < table + 2 * count or t >= len(code):
                    break                                      # (into the table itself: past its end)
                targets.append(t)
            if targets:
                lines.append("J\t%x\t%s" % (address(sg, ins.address), ",".join("%x" % address(sg, t) for t in targets)))
    return lines


def _caller_cleanup(ins, j):
    """the bytes of arguments the caller takes off right after the call ins[j] -- `add sp, n`, or
    `pop bx` / `pop cx` a word each (never DX, where a result's high word comes back) -- or 0"""
    nxt = ins[j + 1] if j + 1 < len(ins) else None
    if nxt is None:
        return 0
    m = re.fullmatch(r"sp, (0x[0-9a-f]+|\d+)", nxt.op_str) if nxt.mnemonic == "add" else None
    if m:
        n = int(m.group(1), 0)
        return n if 0 < n < 0x100 and n % 2 == 0 else 0
    words = 0
    for p in ins[j + 1:j + 4]:
        if p.mnemonic == "pop" and p.op_str in ("bx", "cx"):
            words += 1
        else:
            break
    return 2 * words


def _leave_follows(ins, j):
    """nothing after the call ins[j] touches the stack before the frame's LEAVE"""
    import capstone
    for q in ins[j + 1:j + 12]:
        if q.mnemonic == "leave" or (q.mnemonic == "mov" and q.op_str == "sp, bp"):
            return True
        if q.mnemonic in ("push", "pop", "pushf", "popf", "enter", "ret", "retf") or \
                q.group(capstone.CS_GRP_CALL) or q.group(capstone.CS_GRP_JUMP):
            return False
        try:
            _r, w = q.regs_access()
        except capstone.CsError:
            return False
        if capstone.x86.X86_REG_SP in w:
            return False
    return False


def _pushed_before(ins, j, targets, X):
    """the bytes pushed just before the call ins[j] (back to a jump target, the prologue, or
    anything else that moves SP); `push cs` of a near call to a far function not counted"""
    import capstone
    pushed = 0
    before = ins[:j]
    if before and before[-1].mnemonic == "push" and before[-1].op_str == "cs":
        before = before[:-1]
    for p in reversed(before):
        if p.mnemonic == "push":
            pushed += 4 if p.operands and p.operands[0].size == 4 else 2
            if p.address in targets:
                break
            continue
        if p.group(capstone.CS_GRP_CALL) or p.group(capstone.CS_GRP_JUMP) or \
                p.mnemonic in ("ret", "retf", "pop", "leave", "enter") or p.address in targets:
            break
        try:
            _r, w = p.regs_access()
        except capstone.CsError:
            break
        if X.X86_REG_SP in w:
            break
    return pushed


def call_overrides(d):
    """`O` lines: for each call through a pointer (a driver's dispatch table, a callback), the
    words the code pushed for it just before -- the callee is unknown, so the analysis cannot
    tell what it takes, and drops what was pushed. Removed by the callee (Pascal: Windows'
    drivers and callbacks) unless the caller takes them off itself right after (`add sp, n`)."""
    import capstone
    md = decoder16(detail=True)
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
                if not i.group(capstone.CS_GRP_CALL) or not i.operands:
                    continue
                if i.operands[0].type == X.X86_OP_IMM:
                    # A direct call's callee is described -- but a C function's description says
                    # only that it removes nothing, not what it takes: its caller says that, in the
                    # cleanup right after the call (`add sp, 6`; at /Os `pop bx`/`pop cx` for each
                    # word). SUWIN's sub_2_0A92 passed sub_8_0F24 a far pointer and a character
                    # (`add sp, 6`); without this the call came out with no arguments at all.
                    # (CB_CALLER_ARGS=0 turns it off: on SUWIN as many functions are the original's
                    # bytes with it as without -- 37 -- once each is compiled in its own source file;
                    # Setup on the 15 MINI.CAB modules rebuilt with it ran to the desktop)
                    took = _caller_cleanup(ins, j) if os.environ.get("CB_CALLER_ARGS") != "0" else 0
                    if not took and os.environ.get("CB_CALLER_ARGS") != "0" and i.mnemonic == "call" \
                            and _leave_follows(ins, j):
                        # (or none: Microsoft C 8 leaves a call's arguments to the frame's LEAVE when
                        # the function ends there -- SETUPX's 4:16D8 pushes sub_4_1470 six words and
                        # never takes them off. The pushes are the arguments, when the callee is one
                        # that leaves them to its caller: its own return says so)
                        target = i.operands[0].imm & 0xFFFF
                        if returns(d.segment_bytes.get(sg, b""), target)[1] == 0:
                            took = _pushed_before(ins, j, targets, X)
                    if took:
                        # (`push cs; call` is a far call to a function of the same file: the pushed CS
                        # is the return address's segment, not an argument)
                        far = i.mnemonic == "lcall" or (j > 0 and ins[j - 1].mnemonic == "push" and ins[j - 1].op_str == "cs")
                        lines.append("O\t%x\t%x\t%d\t%s" % (address(sg, i.address), va, took,
                                                            "__cdecl16" + ("far" if far else "near")))
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
    own, passes, voided = {}, {}, {}
    for sg, offs in by_seg.items():
        code = d.segment_bytes.get(sg, b"")
        offs = sorted(offs)
        for k, off in enumerate(offs):
            p, v = [], []
            own[(sg, off)] = result_bytes(code, off, offs[k + 1] if k + 1 < len(offs) else len(code), p, v)
            passes[(sg, off)] = p
            if v:
                voided[(sg, off)] = v[0]
    if voided and getattr(d, "images", None):
        # (one with a way that sets no AX, whose result a caller reads all the same: a result)
        for place in callers_read_ax(d, set(voided)):
            own[place] = voided[place]
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
    # (off unless CB_CALLER_RESULTS=1: with it, USER's C no longer starts Windows 95 Setup -- "not
    # enough memory to run Setup" -- and which of the functions it makes four-byte does that is not
    # yet found; SETUPX, which it was for, goes into the Setup that boots as its instructions)
    if os.environ.get("CB_CALLER_RESULTS") == "1" and getattr(d, "images", None):
        edges = []
        for place in callers_read_dx(d, edges):              # (what the callers read after it)
            if place in size and size[place] < 4:
                size[place] = 4
        writes = _writes_dx(d, {callee for _c, callee in edges})
        for _round in range(8):                              # (and what a four-byte caller hands back
            changed = False                                  #  as it is: its callee's DX:AX too --
            for caller, callee in edges:                     #  one that sets DX at all: KEYBOARD's
                if size.get(caller, 0) >= 4 and callee in size and size[callee] < 4 \
                        and callee in writes:                #  9:0367 leaves it as its caller had it)
                    size[callee], changed = 4, True
            if not changed:
                break
    return {va: size.get(where(va), 0) for va in d.names}


def _writes_dx(d, places):
    """{(seg, off)} of `places` whose own code (to the next function's start) writes DX other than
    by popping back what it pushed -- or calls out, which may set it"""
    md = decoder16(detail=True)
    dx = {capstone.x86.X86_REG_DX, capstone.x86.X86_REG_DL, capstone.x86.X86_REG_DH, capstone.x86.X86_REG_EDX}
    starts = {}
    for va in d.names:
        sg, o = where(va)
        starts.setdefault(sg, []).append(o)
    out = set()
    for sg, off in places:
        img = d.images.get(sg)
        if img is None:
            continue
        later = sorted(o for o in starts.get(sg, []) if o > off)
        end = later[0] if later else len(img)
        saved = False
        reach = off                                      # (the furthest a jump seen so far goes)
        for i in md.disasm(bytes(img[off:end]), off):
            if i.group(capstone.CS_GRP_JUMP) and i.operands and i.operands[0].type == capstone.CS_OP_IMM:
                reach = max(reach, i.operands[0].imm & 0xFFFF)
            if i.mnemonic in ("ret", "retf") and reach <= i.address:
                break                                    # (its end: what follows is another's)
            if i.mnemonic == "push" and i.op_str == "dx":
                saved = True
                continue
            if i.mnemonic == "pop" and i.op_str == "dx" and saved:
                continue
            if i.group(capstone.CS_GRP_CALL) and i.mnemonic == "call" and "[" not in i.op_str:
                out.add((sg, off))                           # (a callee of its own may set it)
                break
            try:
                if dx & set(i.regs_access()[1]):
                    out.add((sg, off))
                    break
            except capstone.CsError:
                continue
    return out


def callers_read_dx(d, edges=None):
    """{(seg, off)} of the functions a call to which is followed, before anything else could set
    it, by a read of DX -- the callee returns four bytes, DX:AX, whatever its own instructions
    let one tell: a compiler reads DX after a call only for a long or a far pointer it returned
    (SETUPX's 4:6C9C: `lcall SUREGCREATEKEY / or dx, ax`, the key's LONG result, read as an int and
    a DX nothing in the C had set). `edges`, given a list, has [(caller, callee)] added for each
    call whose AX and DX go back to the caller's own caller as they are (nothing writes either
    before the return, a jump to it followed): if the caller returns four bytes, so does the
    callee (SUREGQUERYVALUEEX hands back sub_31_002A's)."""
    md = decoder16(detail=True)
    dx = {capstone.x86.X86_REG_DX, capstone.x86.X86_REG_DL, capstone.x86.X86_REG_DH, capstone.x86.X86_REG_EDX}
    ax = {capstone.x86.X86_REG_AX, capstone.x86.X86_REG_AL, capstone.x86.X86_REG_AH, capstone.x86.X86_REG_EAX}
    out = set()
    code_segs = getattr(d, "code", None) or {}
    starts = {}
    for va in d.names:
        sg, o = where(va)
        starts.setdefault(sg, []).append(o)
    for sg in starts:
        starts[sg].sort()
    import bisect
    for seg, img in d.images.items():
        if code_segs and seg not in code_segs:
            continue                                         # (a data segment: no calls in it)
        ins = list(md.disasm(bytes(img), 0))
        at = {i.address: n for n, i in enumerate(ins)}
        for k, i in enumerate(ins[:-1]):
            if edges is not None and (i.mnemonic in ("call", "lcall")) and seg in starts:
                own = starts[seg][bisect.bisect_right(starts[seg], i.address) - 1] \
                    if bisect.bisect_right(starts[seg], i.address) else None
                n, hops, through = k + 1, 0, None
                while own is not None and n < len(ins) and hops < 12:
                    q = ins[n]
                    if q.mnemonic in ("ret", "retf"):
                        through = True
                        break
                    if q.mnemonic == "jmp" and q.op_str.startswith("0x") and int(q.op_str, 16) in at:
                        n, hops = at[int(q.op_str, 16)], hops + 1
                        continue
                    if q.group(capstone.CS_GRP_JUMP) or q.group(capstone.CS_GRP_CALL):
                        break
                    try:
                        _r, w = q.regs_access()
                    except capstone.CsError:
                        break
                    if (ax | dx) & set(w):
                        break
                    n, hops = n + 1, hops + 1
                if through:
                    tgt = None
                    if i.mnemonic == "call" and i.op_str.startswith("0x") and "," not in i.op_str:
                        tgt = (seg, int(i.op_str, 16) & 0xFFFF)
                    elif i.mnemonic == "lcall" and "," in i.op_str and i.op_str.startswith("0x"):
                        s, o = (int(x, 16) for x in i.op_str.split(", "))
                        tgt = where((s << 16) | o) if s < 0xE000 else None
                    if tgt is not None:
                        edges.append(((seg, own), tgt))
            if i.mnemonic == "call" and i.op_str.startswith("0x") and "," not in i.op_str:
                target = (seg, int(i.op_str, 16) & 0xFFFF)
            elif i.mnemonic == "lcall" and "," in i.op_str and i.op_str.startswith("0x"):
                s, o = (int(x, 16) for x in i.op_str.split(", "))
                target = where((s << 16) | o) if s < 0xE000 else None
            else:
                continue
            if target is None:
                continue
            # (not when the caller set DX itself just before: what it reads may be its own, kept --
            # KEYBOARD's 9:0367 saves AX and BX and leaves DX alone -- nor for a _fastcall callee,
            # whose DX may be its argument)
            set_before = False
            for q in ins[max(0, k - 6):k]:
                try:
                    if dx & set(q.regs_access()[1]):
                        set_before = True
                except capstone.CsError:
                    pass
            if set_before or (target[0] in getattr(d, "segment_bytes", {}) and
                              register_arguments(d.segment_bytes[target[0]], target[1])):
                continue
            for q in ins[k + 1:k + 8]:
                if q.group(capstone.CS_GRP_JUMP) or q.group(capstone.CS_GRP_CALL) or q.group(capstone.CS_GRP_RET) \
                        or q.mnemonic in ("int", "iret", "leave"):
                    break
                try:
                    r, w = q.regs_access()
                except capstone.CsError:
                    break
                if q.mnemonic == "xor" and q.op_str in ("dx, dx",) or q.mnemonic == "sub" and q.op_str == "dx, dx":
                    break                                    # (cleared: written, not read)
                if dx & set(r):
                    out.add(target)
                    break
                if dx & set(w):
                    break
    return out


_KEPT_BY_C = {"si": "si", "esi": "si", "di": "di", "edi": "di", "ds": "ds"}


def register_helper(code, off, end=None):
    """Is the function at `off` a helper whose effect is on its caller's registers: a short
    straight run to a plain `ret` -- no frame, no branch, no call -- that leaves SI, DI or DS
    changed, registers a C function keeps for its caller? KEYBOARD's sub_9_0A6B, `push ax / push
    bx / mov ax, ds / add ax, 8 / mov ds, ax / xor si, si / pop bx / pop ax / ret`, moves
    OEMTOANSI's source pointer DS:SI on to the next 64K of a huge buffer. As a function of its
    own its C can only `return`, and the caller's C goes on through the old pointer (its copy
    past 64K went back over the start). Such a helper is decompiled as part of each caller --
    the description's `inline` -- so the caller's C moves its own pointer."""
    md = decoder16(detail=True)
    stop = off + 48 if end is None else min(end, off + 48)
    pushed, changed = [], set()
    for k, i in enumerate(md.disasm(bytes(code[off:stop]), off)):
        if k >= 16:
            return False
        if i.mnemonic == "ret" and not i.op_str:
            return not pushed and bool(changed)
        if i.group(capstone.CS_GRP_JUMP) or i.group(capstone.CS_GRP_CALL) or i.group(capstone.CS_GRP_RET) or \
                i.group(capstone.CS_GRP_INT) or i.mnemonic in ("retf", "iret", "enter", "leave", "int", "into"):
            return False
        try:
            reads, writes = i.regs_access()
        except capstone.CsError:
            return False
        names = {i.reg_name(r) for r in reads} | {i.reg_name(r) for r in writes}
        if names & {"bp", "ebp", "sp", "esp"} - ({"sp", "esp"} if i.mnemonic in ("push", "pop") else set()):
            return False                                     # (a frame, or its caller's: not this)
        if i.mnemonic == "push":
            pushed.append(i.op_str)
            continue
        if i.mnemonic == "pop":
            if not pushed or pushed.pop() != i.op_str:
                return False
            changed.discard(_KEPT_BY_C.get(i.op_str))
            continue
        changed |= {_KEPT_BY_C[i.reg_name(r)] for r in writes if i.reg_name(r) in _KEPT_BY_C}
    return False


def callers_read_ax(d, places):
    """{(seg, off)} of `places` a call to which is followed, before anything sets it, by a read of
    AX (`or ax, ax`, `mov [x], ax`, `push ax`): what it leaves there is a result its caller uses."""
    md = decoder16(detail=True)
    ax = {capstone.x86.X86_REG_AX, capstone.x86.X86_REG_AL, capstone.x86.X86_REG_AH, capstone.x86.X86_REG_EAX}
    code_segs = getattr(d, "code", None) or {}
    out = set()
    for seg, img in d.images.items():
        if code_segs and seg not in code_segs:
            continue
        ins = list(md.disasm(bytes(img), 0))
        for k, i in enumerate(ins[:-1]):
            if i.mnemonic == "call" and i.op_str.startswith("0x") and "," not in i.op_str:
                target = (seg, int(i.op_str, 16) & 0xFFFF)
            elif i.mnemonic == "lcall" and "," in i.op_str and i.op_str.startswith("0x"):
                s, o = (int(x, 16) for x in i.op_str.split(", "))
                target = where((s << 16) | o) if s < 0xE000 else None
            else:
                continue
            if target not in places or target in out:
                continue
            for q in ins[k + 1:k + 8]:
                if q.group(capstone.CS_GRP_JUMP) or q.group(capstone.CS_GRP_CALL) or q.group(capstone.CS_GRP_RET) \
                        or q.mnemonic in ("int", "iret", "leave"):
                    break
                ops = [o.strip() for o in q.op_str.split(",")]
                if q.mnemonic in ("xor", "sub") and len(ops) == 2 and ops[0] == ops[1]:
                    if ops[0] in ("ax", "al", "ah", "eax"):
                        break                                # (cleared: written, not read)
                    continue
                try:
                    r, w = q.regs_access()
                except capstone.CsError:
                    break
                if ax & set(r):
                    out.add(target)
                    break
                if ax & set(w):
                    break
    return out


_FASTCALL_REGS = {"ax": "ax", "al": "ax", "ah": "ax", "eax": "ax", "dx": "dx", "dl": "dx", "dh": "dx", "edx": "dx",
                  "bx": "bx", "bl": "bx", "bh": "bx", "ebx": "bx"}


def register_arguments(code, off):
    """Whether the function at `off` takes arguments in AX, DX or BX -- Microsoft C's _fastcall: its
    straight-line start (to the first jump, call or return) reads one of them (or AL, DL, BL) before
    it writes it. SUWIN's 7:0000 begins `push bp / mov bp, sp / push ax` -- its char argument, kept in
    its frame -- and its callers load AX just before the call."""
    md = decoder16(detail=True)
    written = set()
    for i in md.disasm(bytes(code[off:off + 64]), off):
        if i.group(capstone.CS_GRP_JUMP) or i.group(capstone.CS_GRP_CALL) or i.group(capstone.CS_GRP_RET) \
                or i.mnemonic in ("int", "iret", "retf"):
            return False
        try:
            reads, writes = i.regs_access()
        except capstone.CsError:
            return False
        ops = [o.strip() for o in i.op_str.split(",")]
        same = i.mnemonic in ("xor", "sub", "sbb") and len(ops) == 2 and ops[0] == ops[1]
        if not same:
            for r in reads:
                full = _FASTCALL_REGS.get(i.reg_name(r))
                if full and full not in written:
                    return True
        for r in writes:
            full = _FASTCALL_REGS.get(i.reg_name(r))
            if full:
                written.add(full)
    return False


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
    fwd = forwards(d, seg, off, kind or default)
    if fwd is not None:
        pop, kind = fwd, "far" if (kind or default) == "far" else kind
    # (and what it took off by hand at its start comes on top of what its return removes: USER's
    # call-hook dispatcher 1:8005 pops one word by hand, then `retf 8` -- five words in all)
    hand = popped_by_hand(code, off, kind or default) or 0
    if hand and pop >= 0:
        pop += hand
    conv = "__stdcall16" if pop > 0 else "__cdecl16" if pop == 0 else "__stdcall16"
    # (CB_FASTCALL=0 turns it off; near ones only -- a far _fastcall function cannot be compiled
    # with /Gw, the Windows prolog loading DS through AX: "illegal far _fastcall function")
    if os.environ.get("CB_FASTCALL") != "0" and (kind or default) == "near" and register_arguments(code, off):
        conv = "__fastcall16"
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
    _DECODED.clear()                                         # (another module's segments: let them go)
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
    d.inlined = set()                                        # helpers made part of their callers
    starts16 = {}
    for va, _nm, _dist in funcs:
        starts16.setdefault(where[va][0], []).append(where[va][1])
    for va, nm, dist in funcs:
        seg, off = where[va]
        later = [o for o in starts16.get(seg, ()) if o > off]
        if dist == "near" and (seg, off) not in exported and os.environ.get("CB_INLINE_HELPERS") != "0" and \
                register_helper(d.segment_bytes.get(seg, b""), off, min(later) if later else None):
            d.inlined.add(va)
            lines.append("F\t%x\t%s\t-1\tinline" % (va, nm))
            continue
        lines.append(model_line(d, va, nm, default=dist))
    if not os.environ.get("CB_NO_CALL_OVERRIDES"):
        lines += call_overrides(d)
    lines += jump_tables(d)
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
LE_ARCH = _cpu("le")
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
DOS_ARCH = _cpu("dos")


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


# NES -------------------------------------------------------------------------------------------
def describe_nes(module_path, outdir):
    """Write the description of a NES cartridge for the native program: the 6502, the program
    ROM where the CPU sees it at power-on (formats/nes.py), and its functions -- the three
    vectors' and every JSR target. -> (path of the description, {va: name})"""
    from codebreaker import platforms
    from codebreaker.formats import nes
    rom = nes.read(module_path)
    os.makedirs(outdir, exist_ok=True)
    image = os.path.join(outdir, "cpu.bin")
    with open(image, "wb") as f:
        f.write(bytes(rom.memory()[0x8000:]))
    names = {rom.vectors["reset"]: "reset"}
    names.setdefault(rom.vectors["nmi"], "nmi")
    names.setdefault(rom.vectors["irq"], "irq")
    lines = ["arch\t%s" % platforms.named("nes").cpu, "bytes\t8000\t%s\t0\t8000" % os.path.abspath(image)]
    by_va = {}
    for va in rom.functions():
        by_va[va] = names.get(va, "sub_%04X" % va)
        lines.append("F\t%x\t%s\t-1" % (va, by_va[va]))
    path = os.path.join(outdir, "description.txt")
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines) + "\n")
    return path, by_va
