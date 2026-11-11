"""image.py -- a module as the analyses see it.

    from codebreaker.analyze import image
    img = image.load("comctl32.dll")        # a 32-bit module, a 16-bit one, or a driver
    img.kind, img.bits                      # "pe" | "ne" | "le",  32 | 16
    img.functions                           # start addresses, sorted
    fn = img.function(va)                   # .insns (by address), .tables (jump tables), .end
    img.read(va, n), img.u32(va)            # the module's bytes at their addresses
    img.imports[addr]                       # "USER32!DefWindowProcA", "USER!DEFWINDOWPROC"
    image.arguments(fn, i, 4)               # what the four pushes before the call at insns[i] hold

One address space per module, whatever its format. A 32-bit module's addresses are its own. A
16-bit module is laid out as decompile/describe.py lays it out for the decompiler (each segment
at selector * 0x10000, relocations applied, each imported function at an address standing for
it), and so is a driver; an address an analysis reports is therefore the address the decompiler
and the function store use.

A function is decoded by following its code from its start: fall through, direct branches and
recognised jump tables, never past the next function (in C++: native/analyze, cbanalyze.dll, through
analyze/native.py, with the capstone.dll Python's package carries). Bytes nothing reaches (tables and padding
inside the code) are not read as instructions, so they produce no false references. Function
starts are the ones V2 already finds (formats/pe_functions.py, describe.py).

An instruction is a small record (`Ins`): mnemonic, operands as plain tuples, the registers it
reads and writes, and where it goes when it is a branch.

    ("r", name, size)                              a register
    ("i", value, size)                             an immediate, masked to its size
    ("m", base, index, scale, disp, seg, size)     memory; disp is the address when base is None
"""
import bisect
import os
import re
import struct

from codebreaker.analyze import native
from codebreaker.base import corpus, settings
from codebreaker.decompile import describe
from codebreaker.formats import detect
from codebreaker.formats import le as le_format
from codebreaker.formats import pe_functions
from codebreaker.formats.ne import NEFile
from codebreaker.formats.pe import open_pe

_FAMILY = {}
for _fam, _names in (("eax", "rax eax ax al ah"), ("ecx", "rcx ecx cx cl ch"), ("edx", "rdx edx dx dl dh"),
                     ("ebx", "rbx ebx bx bl bh"), ("esp", "rsp esp sp spl"), ("ebp", "rbp ebp bp bpl"),
                     ("esi", "rsi esi si sil"), ("edi", "rdi edi di dil")):
    for _n in _names.split():
        _FAMILY[_n] = _fam
_FLOWS = (None, "call", "jmp", "jcc", "ret")     # Ins.flow by the DLL's number


def family(reg):
    """The whole register a register name is part of: al, ax -> eax."""
    return _FAMILY.get(reg, reg)


def signed(value, size):
    """An immediate or displacement of `size` bytes as a signed number."""
    bits = 8 * size
    return value - (1 << bits) if value >> (bits - 1) else value


class Ins(object):
    """One instruction. `flow` is "call", "jmp", "jcc", "ret" or None; `target` the address a
    direct call or branch goes to."""
    __slots__ = ("addr", "size", "mn", "ops", "text", "rd", "wr", "flow", "target")

    def __repr__(self):
        return "%x: %s" % (self.addr, self.text)


class Function(object):
    """One function: `insns` in address order, `tables` its jump tables, `limit` where the next
    function starts, `end` the end of the last instruction reached."""
    __slots__ = ("start", "limit", "end", "name", "insns", "tables", "index")

    def at(self, addr):
        """The position in `insns` of the instruction at `addr`, or None."""
        return self.index.get(addr)


class Image(object):
    def __init__(self, path, kind, bits):
        self.path, self.kind, self.bits = path, kind, bits
        self.name = os.path.basename(path).lower()
        self.spans = []                # (va, bytes, executable, writable, name, initialised length)
        self.names = {}                # function start -> name
        self.limits = {}               # function start -> where its bytes may run to
        self.imports = {}              # address standing for an import -> "MODULE!name"
        self.exports = {}              # address -> exported name
        self.roots = []                # where execution can enter from outside
        self.pointers = None           # {address: (kind, target)} -- where the file says the loader writes
                                       # an address, and which; kind "abs32" | "far" | "selector" |
                                       # "offset" (None: the file does not say)
        self.device = None             # a driver's device descriptor block
        self.discovered = set()        # function starts found here that the description did not have
        self.base = 0                  # a 32-bit module's preferred load address
        self.data_segment = 0          # 16-bit: base address of the automatic data segment
        self.functions = []
        self._starts = []
        self._fn = {}
        self._memo = {}
        self._rows = {}                # decoded functions the DLL handed back, not yet made objects
        self._native = None

    @property
    def flat(self):
        """A flat 32-bit image (PE): one address space from its base, run in user mode."""
        return detect.flat(self.kind)

    # ---- the bytes ---------------------------------------------------------------------------
    def _finish(self):
        self.spans.sort(key=lambda s: s[0])
        self._starts = [s[0] for s in self.spans]
        self.functions = sorted(self.names)

    def span(self, va):
        """The span holding `va`, or None."""
        i = bisect.bisect_right(self._starts, va) - 1
        if i >= 0:
            s = self.spans[i]
            if va < s[0] + len(s[1]):
                return s
        return None

    def read(self, va, n):
        s = self.span(va)
        return bytes(s[1][va - s[0]:va - s[0] + n]) if s else b""

    def u16(self, va):
        b = self.read(va, 2)
        return struct.unpack("<H", b)[0] if len(b) == 2 else None

    def u32(self, va):
        b = self.read(va, 4)
        return struct.unpack("<I", b)[0] if len(b) == 4 else None

    def is_code(self, va):
        s = self.span(va)
        return bool(s and s[2])

    def is_data(self, va):
        """In the module, and not at an address standing for an import."""
        return self.span(va) is not None and va not in self.imports

    def initialised(self, va):
        """Does the file hold a value for this address (as opposed to memory it only reserves)?"""
        s = self.span(va)
        return bool(s and va - s[0] < s[5])

    def owner(self, va):
        """The start of the function whose bytes hold `va`, or None."""
        i = bisect.bisect_right(self.functions, va) - 1
        if i >= 0 and va < self.limits[self.functions[i]]:
            return self.functions[i]
        return None

    def label(self, va):
        """A name for an address: the function, the import, or the address itself."""
        return self.names.get(va) or self.imports.get(va) or "%x" % va

    def memo(self, key, make):
        """Something an analysis works out once per image and others reuse."""
        if key not in self._memo:
            self._memo[key] = make(self)
        return self._memo[key]

    # ---- instructions (decoded in C++: native/analyze, image.cc) -------------------------------
    def _handle(self):
        if self._native is None:
            self._native = native.Handle(self)
        return self._native

    def _function(self, row):
        fn = Function()
        fn.start, fn.limit, fn.end = row[0], row[1], row[2]
        fn.name = self.names.get(fn.start, "%x" % fn.start)
        insns = []
        for addr, size, mn, text, ops, rd, wr, flow, target in row[3]:
            ins = Ins()
            ins.addr, ins.size, ins.mn, ins.text = addr, size, mn, text
            ins.ops = tuple(tuple(o) for o in ops)
            ins.rd, ins.wr = frozenset(rd), frozenset(wr)
            ins.flow, ins.target = _FLOWS[flow], target
            insns.append(ins)
        fn.insns = insns
        fn.index = {ins.addr: i for i, ins in enumerate(insns)}
        fn.tables = row[4]
        return fn

    def function(self, start):
        """The decoded function starting at `start` (decoded once)."""
        fn = self._fn.get(start)
        if fn is None:
            row = self._rows.pop(start, None) or self._handle().ask("image.function", start)
            fn = self._fn[start] = self._function(row)
        return fn

    def linear(self, va, n):
        """Up to `n` instructions read straight on from `va` (no branch followed): [Ins]."""
        return self._function(self._handle().ask("image.linear", (n << 40) | va)).insns

    def each(self):
        for start in list(self.functions):
            yield self.function(start)

    def _auto_name(self, va):
        if self.kind == "pe":
            return "sub_%06X" % (va - self.base)
        if self.kind == "ne":
            return "sub_%d_%04X" % describe.where(va)
        return "sub_%d_%05X" % ((va - describe.LE_BASE) // describe.LE_STRIDE + 1, (va - describe.LE_BASE) % describe.LE_STRIDE)

    def discover(self):
        """Add the function starts the module itself gives away beyond those it was loaded with
        (which are the ones a scan of the bytes agrees on):

          * the target of a direct call made by code reached from a known start -- a function
            called from one place only is not among the scanned ones;
          * an address the file marks as an address (a relocation) that points at code no known
            function reaches: a virtual function, a callback, a handler handed to the system.

        Both are repeated until nothing new turns up. Every function is decoded on the way.
        -> the starts added"""
        got = self._handle().ask("image.discover")
        for va in got["discovered"]:
            self.names[va] = self._auto_name(va)
        self.discovered |= set(got["discovered"])
        self.functions[:] = [f for f, _l in got["functions"]]
        for f, limit in got["functions"]:
            self.limits[f] = limit
        self._fn.clear()
        self._rows = {row[0]: row for row in got["decoded"]}
        self._native.names(self)
        return self.discovered

    def covering(self, va):
        """The decoded instruction whose bytes hold `va`, or None when no function reaches it."""
        f = self.owner(va)
        if f is None:
            return None
        fn = self.function(f)
        lo, hi = 0, len(fn.insns)
        while lo < hi:
            mid = (lo + hi) // 2
            if fn.insns[mid].addr <= va:
                lo = mid + 1
            else:
                hi = mid
        return fn.insns[lo - 1] if lo and va < fn.insns[lo - 1].addr + fn.insns[lo - 1].size else None


# ---- arguments at a call ---------------------------------------------------------------------
def definition(fn, i, reg, depth=14):
    """What register `reg` holds just before fn.insns[i], looking back through straight-line
    code: an operand tuple, ("lea", base, index, scale, disp) for an address, ("call", target)
    for a result, or None when it cannot be said."""
    fam = family(reg)
    for j in range(i - 1, max(-1, i - 1 - depth), -1):
        ins = fn.insns[j]
        if ins.flow in ("jmp", "ret"):
            return None
        if ins.flow == "call":
            if fam in ("eax", "ecx", "edx"):
                return ("call", ins.target if ins.target is not None else _slot(ins)) if fam == "eax" else None
            continue
        if fam not in ins.wr:
            continue
        ops = ins.ops
        if len(ops) == 2 and ops[0][0] == "r" and family(ops[0][1]) == fam:
            if ins.mn == "mov" or ins.mn == "movzx":
                if ops[1][0] == "r":
                    return definition(fn, j, ops[1][1], depth - (i - j)) or ("r", ops[1][1], ops[1][2])
                return ops[1]
            if ins.mn == "lea" and ops[1][0] == "m":
                return ("lea",) + ops[1][1:5]
            if ins.mn in ("xor", "sub") and ops[1][0] == "r" and ops[1][1] == ops[0][1]:
                return ("i", 0, ops[0][2])
        if ins.mn == "pop" and j and fn.insns[j - 1].mn == "push":          # push K / pop reg
            return fn.insns[j - 1].ops[0] if fn.insns[j - 1].ops else None
        return None
    return None


def _slot(ins):
    """The address an indirect call or jump reads its destination from (`call [slot]`), or None."""
    if ins.ops and ins.ops[0][0] == "m" and ins.ops[0][1] is None and ins.ops[0][2] is None:
        return ins.ops[0][4]
    return None


def destination(img, ins):
    """Where a call or jump goes: the function, or the address standing for the import; None
    when it is decided at run time."""
    if ins.target is not None:
        return ins.target
    slot = _slot(ins)
    if slot is not None and slot in img.imports:
        return slot
    return None


def arguments(fn, i, n):
    """The operands of the `n` pushes before fn.insns[i], nearest first (so the first argument
    first for C and stdcall, the last first for Pascal). A pushed register is replaced by what
    it was loaded with when that is in sight. The list is shorter when the code stops being
    a run of pushes."""
    out = []
    for j in range(i - 1, max(-1, i - 60), -1):
        if len(out) >= n:
            break
        ins = fn.insns[j]
        if ins.flow in ("call", "jmp", "ret") or ins.mn in ("pop", "leave", "enter", "pushal", "popal"):
            break
        if ins.mn == "push" and ins.ops:
            op = ins.ops[0]
            if op[0] == "r":
                op = definition(fn, j, op[1]) or op
            out.append(op)
        elif ins.mn in ("add", "sub") and ins.ops and ins.ops[0][0] == "r" and family(ins.ops[0][1]) == "esp":
            break
    return out


# ---- loading ---------------------------------------------------------------------------------
_NE_NAMES = {}
_NE_FILES = {"KERNEL": ("krnl386.exe", "kernel.exe"), "USER": ("user.exe",), "GDI": ("gdi.exe",)}


def _ne_export_names(module):
    """{ordinal: name} of the 16-bit module that exports under `module`, from its own name tables."""
    key = module.upper()
    if key not in _NE_NAMES:
        names = {}
        for cand in _NE_FILES.get(key, ()) + tuple(key.lower() + e for e in (".dll", ".exe", ".drv")):
            p = corpus.retail(cand)
            if p and corpus.lane(p) == "ne":
                try:
                    nef = NEFile(p)
                    names = {o: n for n, o in nef.resident_names()[1:] + nef.nonresident_names()[1:]}
                    break
                except (ValueError, IndexError, struct.error):
                    pass
        _NE_NAMES[key] = names
    return _NE_NAMES[key]


def _load_pe(path):
    pe = open_pe(path)
    if pe.machine != 0x014C:
        raise ValueError("%s: not an x86 module" % os.path.basename(path))
    img = Image(path, "pe", 32)
    base = pe.image_base
    img.base = base
    for s in pe.sections:
        size = max(s["vsize"], s["rawsize"])
        held = min(s["rawsize"], s["vsize"] or s["rawsize"]) if s["rawptr"] else 0
        raw = pe.data[s["rawptr"]:s["rawptr"] + held]
        if size:
            img.spans.append((base + s["vaddr"], raw + bytes(size - len(raw)), bool(s["chars"] & 0x20000020),
                              bool(s["chars"] & 0x80000000), s["name"], len(raw)))
    carve = pe_functions.PeCarve(path)
    for va, f in carve.funcs.items():
        img.names[va] = f["name"] if f["name"] and re.fullmatch(r"[A-Za-z_?@$][\w?@$]*", f["name"]) \
            else "sub_%06X" % (va - base)
        img.limits[va] = f["end"]
    img.exports = dict(carve.exports)
    for slot, (dll, name) in pe.imports().items():
        img.imports[slot] = "%s!%s" % (os.path.splitext(dll)[0].upper(), name)
    img.roots = sorted(set(carve.exports) | ({base + pe.entry} if pe.entry else set()))
    relocs = pe.base_relocs()
    img._finish()
    img.pointers = {base + r: ("abs32", img.u32(base + r)) for r in relocs} if relocs else None
    return img


def _load_ne(path, work):
    nef = NEFile(path)
    d = describe.describe16(path, work)
    img = Image(path, "ne", 16)
    segs = {s["idx"]: s for s in nef.segments()}
    for idx, data in d.images.items():
        s = segs[idx]
        if data:
            img.spans.append((describe.address(idx, 0), data, s["kind"] == "CODE", s["kind"] != "CODE",
                              "%s%d" % (s["kind"].lower(), idx), s["length"] if s["file_off"] else 0))
    by_seg = {}
    for va, (seg, off) in d.where.items():
        by_seg.setdefault(seg, []).append(off)
        img.names[va] = d.names[va]
    for seg, offs in by_seg.items():
        offs.sort()
        for k, off in enumerate(offs):
            img.limits[describe.address(seg, off)] = describe.address(seg, offs[k + 1] if k + 1 < len(offs)
                                                                      else d.code.get(seg, 0))
    for k, name in enumerate(d.imports):
        m = re.fullmatch(r"(.+)_(\d+)", name)
        if m:
            real = _ne_export_names(m.group(1)).get(int(m.group(2)))
            name = "%s!%s" % (m.group(1).upper(), real or "#" + m.group(2))
        img.imports[(describe.EXTERN_SELECTOR << 16) + 16 * k] = name
    roots = set()
    for e in nef.entries():
        if e.get("kind") in ("FIXED", "MOVEABLE"):
            va = describe.address(e["seg"], e["offset"])
            if va in img.names:
                roots.add(va)
                if not img.names[va].startswith("sub_"):
                    img.exports[va] = img.names[va]
    cs, ip = nef.h["csip"] >> 16, nef.h["csip"] & 0xFFFF
    if cs and describe.address(cs, ip) in img.names:
        roots.add(describe.address(cs, ip))
    img.roots = sorted(roots)
    img.data_segment = describe.address(d.data_segment, 0) if d.data_segment else 0
    # where the loader writes an address: every place a relocation record (and its chain) names
    img.pointers = {}
    entries = {e["ordinal"]: e for e in nef.entries() if e.get("kind") in ("FIXED", "MOVEABLE")}
    refs, extern = nef.module_refs(), {n: k for k, n in enumerate(d.imports)}
    for s in nef.segments():
        for r in nef.relocations(s):
            if r["kind"] == "INTERNAL":
                e = entries.get(r["entry"]) if r["seg"] == 0xFF else {"seg": r["seg"], "offset": r["entry"]}
                target = describe.address(e["seg"], e["offset"]) if e else None
            elif r["kind"] in ("IMPORTORD", "IMPORTNAME"):
                mod = refs[r["module"] - 1] if 0 < r["module"] <= len(refs) else "?"
                nm = ("%s_%d" % (mod, r["ordinal"])) if r["kind"] == "IMPORTORD" else nef.imported_name(r["name_off"])
                k = extern.get(re.sub(r"\W", "_", nm))
                target = None if k is None else (describe.EXTERN_SELECTOR << 16) + 16 * k
            else:
                continue
            off, hops = r["offset"], 0
            while off != 0xFFFF and off + 1 < s["length"] and hops < 0x10000:
                img.pointers[describe.address(s["idx"], off)] = (
                    {3: "far", 2: "selector", 5: "offset"}.get(r["src"], "byte"), target)
                if r["additive"]:
                    break
                off, hops = nef.u16(s["file_off"] + off), hops + 1
    img._finish()
    return img


def _load_le(path, member, work):
    found = le_format.open_vxds(path)
    if member:
        found = [(n, lf) for n, lf in found if n.lower() == member.lower()]
    if not found:
        raise ValueError("%s: no driver image%s" % (os.path.basename(path), " named " + member if member else ""))
    name, lf = found[0]
    d = describe.describe_le(lf, name, work)
    img = Image(path, "le", 32)
    if member:
        img.name = "%s#%s" % (img.name, member.lower())
    executable = {va for va, _n in d.spans.values()}
    writable = {describe.le_address(o["idx"], 0) for o in lf.objects if o["write"]}
    with open(d.path, encoding="utf-8") as f:
        for line in f:
            p = line.rstrip("\n").split("\t")
            if p[0] == "bytes":
                va, off, size = int(p[1], 16), int(p[3], 16), int(p[4], 16)
                with open(p[2], "rb") as blob:
                    blob.seek(off)
                    data = blob.read(size)
                img.spans.append((va, data, va in executable, va in writable or va not in executable,
                                  "object%d" % ((va - describe.LE_BASE) // describe.LE_STRIDE + 1), len(data)))
            elif p[0] == "F":
                img.names[int(p[1], 16)] = p[2]
            elif p[0] == "I":
                img.imports[int(p[1], 16)] = "%s!%s" % ("VMM" if p[2].startswith("VMM_") else "VXD", p[2])
    img._finish()
    # what the device descriptor block names: the control procedure, the API procedures and the
    # service table are entered from outside and are function starts
    try:
        ddb = lf.ddb()
    except (IndexError, struct.error, ValueError):   # a damaged table (the reader's FormatError)
        ddb = None
    img.device = ddb
    try:
        img.pointers = {describe.le_address(obj, off): ("abs32", img.u32(describe.le_address(obj, off)))
                        for (obj, off), fx in lf.fixups().items()
                        if fx and fx["kind"] == "internal" and fx.get("srctype") == 7}
    except (IndexError, struct.error, ValueError):
        img.pointers = None
    if ddb:
        at = describe.le_address(ddb["obj"], ddb["rva"])
        entries = [img.u32(at + o) for o in (0x18, 0x1C, 0x20)]
        table, count = img.u32(at + 0x30), img.u32(at + 0x34)
        if table and count and count < 0x400 and img.span(table):
            entries += [img.u32(table + 4 * k) for k in range(count)]
        for va in entries:
            if va and img.is_code(va):
                img.names.setdefault(va, "sub_%d_%05X" % ((va - describe.LE_BASE) // describe.LE_STRIDE + 1,
                                                           (va - describe.LE_BASE) % describe.LE_STRIDE))
                img.roots.append(va)
    img.functions = sorted(img.names)
    for k, va in enumerate(img.functions):
        s = img.span(va)
        nxt = img.functions[k + 1] if k + 1 < len(img.functions) else None
        end = s[0] + len(s[1]) if s else va
        img.limits[va] = nxt if nxt is not None and nxt < end else end
    img.roots = sorted(set(img.roots))
    return img


def kind_of(path):
    """'pe', 'ne' or 'le' for what load() reads (a bundle of drivers, vmm32.vxd's W3, is read as
    its drivers), else the file's lane (base/corpus.lane)."""
    return "le" if detect.format_of(path) == "W3" else corpus.lane(path)


def load(module, work=None):
    """The image of a module the corpus names (or a path). A driver inside a bundle is named
    "vmm32.vxd#vmm"."""
    name, _, member = module.partition("#")
    path = corpus.resolve(name)
    kind = kind_of(path)
    if kind not in ("pe", "ne", "le"):
        raise ValueError("%s: not a 32-bit module, a 16-bit module or a driver" % os.path.basename(path))
    work = work or settings.state("analyze", "work", os.path.basename(path).lower() + ("-" + member if member else ""))
    os.makedirs(work, exist_ok=True)
    img = _load_pe(path) if kind == "pe" else _load_ne(path, work) if kind == "ne" else _load_le(path, member, work)
    img.discover()
    return img
