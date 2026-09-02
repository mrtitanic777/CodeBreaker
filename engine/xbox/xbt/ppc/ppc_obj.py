#!/usr/bin/env python3
"""
ppc_obj.py — read a PowerPC PE/COFF object (XDK cl.exe output): per-function
bytes + a bit-level relocation mask. The objextract.py analogue for the 360.

Why a mask at all (the lesson carried from the PC path): a reconstruction can be
opcode-for-opcode correct yet differ in the *bytes* wherever the linker fills in
an address or a call displacement — those depend on final layout, not on the
source. Comparing must ignore exactly those bit-fields. On PowerPC the compiler
tells us where they are, as relocations on the .text section:

    REL24  (0x06)  bl / b        24-bit relative displacement  -> ignore LI field
    REL14  (0x07)  bc            14-bit branch displacement     -> ignore BD field
    REFHI  (0x10)  lis           high half of a 32-bit address  -> ignore imm16
    REFLO  (0x11)  addi/lwz/...  low half of a 32-bit address   -> ignore imm16
    ADDR32 (0x02)  data pointer  whole word                     -> ignore 4 bytes
    PAIR   (0x12)  (metadata for REFHI/REFLO)                    -> no site
    ABSOLUTE(0x00)                                               -> no site

Because every PowerPC instruction is a fixed 4-byte word, the mask is a per-byte
*bit* mask (which bits to ignore), not a whole-byte skip — so the opcode and
register fields of a relocated instruction are still compared. That is stricter
than the NE byte-mask and is what makes a PPC match trustworthy.

    python ppc_obj.py foo.obj              # list functions, bytes, masked sites
"""
import struct
import sys

# IMAGE_REL_PPC_* relocation types (winnt.h)
REL_ABSOLUTE = 0x00
REL_ADDR32   = 0x02
REL_ADDR24   = 0x03
REL_ADDR16   = 0x04
REL_ADDR14   = 0x05
REL_REL24    = 0x06
REL_REL14    = 0x07
REL_TOCREL16 = 0x08
REL_TOCREL14 = 0x09
REL_SECREL   = 0x0B     # 32-bit section-relative
REL_SECREL16 = 0x0F
REL_REFHI    = 0x10
REL_REFLO    = 0x11
REL_PAIR     = 0x12
REL_SECRELLO = 0x13
REL_SECRELHI = 0x14

_IMAGE_SCN_CNT_CODE = 0x00000020
_IMAGE_SCN_MEM_EXECUTE = 0x20000000
_DT_FUNCTION = 0x20     # COFF symbol complex-type "function"


def _rel_byte_masks(rtype):
    """Ignore-bitmask for the 4 bytes of the relocated instruction word (or data
    word), as {byte_index_in_word: ignore_mask}. Empty for non-site types."""
    if rtype in (REL_REL24, REL_ADDR24):
        # opcode = top 6 bits of byte0; LI = 24 bits; AA/LK = low 2 of byte3
        return {0: 0x03, 1: 0xFF, 2: 0xFF, 3: 0xFC}
    if rtype in (REL_REL14, REL_ADDR14):
        # BD = 14 bits in the low half, keep AA/LK (low 2 of byte3)
        return {2: 0xFF, 3: 0xFC}
    if rtype in (REL_ADDR16, REL_REFHI, REL_REFLO, REL_TOCREL16, REL_TOCREL14,
                 REL_SECREL16, REL_SECRELLO, REL_SECRELHI):
        return {2: 0xFF, 3: 0xFF}       # 16-bit immediate in the low half
    if rtype in (REL_ADDR32, REL_SECREL):
        return {0: 0xFF, 1: 0xFF, 2: 0xFF, 3: 0xFF}
    return {}                            # PAIR, ABSOLUTE, etc. — no masked bits


class Obj:
    def __init__(self, data):
        if isinstance(data, (bytes, bytearray)):
            self.d = bytes(data)
        else:
            self.d = open(data, "rb").read()
        d = self.d
        self.machine, self.nsec = struct.unpack_from("<HH", d, 0)
        self.symoff, self.nsym = struct.unpack_from("<II", d, 8)
        self.stroff = self.symoff + self.nsym * 18
        if self.machine != 0x01F2:
            raise ValueError("not a PowerPC-BE COFF object (machine 0x%04X)"
                             % self.machine)
        self._sections()

    def _str(self, off):
        e = self.d.find(b"\x00", self.stroff + off)
        return self.d[self.stroff + off:e].decode("latin1")

    def _name8(self, raw8):
        if raw8[:4] == b"\x00\x00\x00\x00":
            return self._str(struct.unpack("<I", raw8[4:8])[0])
        return raw8.split(b"\x00", 1)[0].decode("latin1")

    def _sections(self):
        self.sec = []
        for i in range(self.nsec):
            b = 20 + i * 40
            name = self._name8(self.d[b:b + 8])
            raw_size, raw_off = struct.unpack_from("<II", self.d, b + 16)
            relptr, = struct.unpack_from("<I", self.d, b + 24)
            nrel, = struct.unpack_from("<H", self.d, b + 32)
            chars, = struct.unpack_from("<I", self.d, b + 36)
            self.sec.append({"name": name, "raw_size": raw_size,
                             "raw_off": raw_off, "relptr": relptr, "nrel": nrel,
                             "chars": chars, "index": i + 1,
                             "code": bool(chars & (_IMAGE_SCN_CNT_CODE |
                                                   _IMAGE_SCN_MEM_EXECUTE))})

    def _relocs(self, sec):
        out = []
        for r in range(sec["nrel"]):
            rb = sec["relptr"] + r * 10
            va, symidx, typ = struct.unpack_from("<IIH", self.d, rb)
            out.append((va, symidx, typ))
        return out

    def _symbols(self):
        out = []
        i = 0
        while i < self.nsym:
            b = self.symoff + i * 18
            name = self._name8(self.d[b:b + 8])
            value, = struct.unpack_from("<I", self.d, b + 8)
            secnum, = struct.unpack_from("<h", self.d, b + 12)
            typ, = struct.unpack_from("<H", self.d, b + 14)
            sclass = self.d[b + 16]
            naux = self.d[b + 17]
            out.append({"name": name, "value": value, "sec": secnum,
                        "type": typ, "sclass": sclass})
            i += 1 + naux
        return out

    def functions(self):
        """{name: (code_bytes, mask)} — mask is {byte_offset: ignore_bitmask},
        offsets relative to the function start. Function boundaries come from the
        symbol table; relocs inside each function become the mask."""
        syms = self._symbols()
        by_index = {s["index"]: s for s in self.sec}
        # function symbols: complex type "function", or any symbol landing in a
        # code section (the XDK marks funcs with DT_FUNCTION; be lenient).
        funcs = []
        for s in syms:
            sec = by_index.get(s["sec"])
            if not sec or not sec["code"]:
                continue
            if s["name"].startswith("."):        # section symbol, not a function
                continue
            is_func = (s["type"] >> 4) == (_DT_FUNCTION >> 4) or s["type"] == _DT_FUNCTION
            if is_func or s["name"]:
                funcs.append(s)
        # boundaries per code section
        out = {}
        for sec in self.sec:
            if not sec["code"]:
                continue
            here = sorted([f for f in funcs if f["sec"] == sec["index"]],
                          key=lambda f: f["value"])
            code = self.d[sec["raw_off"]:sec["raw_off"] + sec["raw_size"]]
            relocs = self._relocs(sec)
            for j, f in enumerate(here):
                start = f["value"]
                end = here[j + 1]["value"] if j + 1 < len(here) else len(code)
                fb = code[start:end]
                mask = {}
                for (va, _si, typ) in relocs:
                    if not (start <= va < end):
                        continue
                    rel = va - start
                    for bi, m in _rel_byte_masks(typ).items():
                        mask[rel + bi] = mask.get(rel + bi, 0) | m
                if f["name"]:
                    out[f["name"]] = (fb, mask)
        return out


def masked_eq(a, b, mask):
    """True if a == b ignoring the bits set in `mask` ({offset: ignore_bits})."""
    if len(a) != len(b):
        return False
    for i in range(len(a)):
        m = mask.get(i, 0)
        if (a[i] & ~m) != (b[i] & ~m):
            return False
    return True


DESCRIPTION = "read/write PowerPC-BE COFF objects (XDK cl.exe format): dump or emit a linkable .obj"

# COFF constants for writing (winnt.h)
_SYM_CLASS_EXTERNAL = 2
_SYM_CLASS_STATIC = 3
_CH_TEXT = 0x60400020          # CODE|EXECUTE|READ, 16-byte align (matches XDK .text)
_CH_PDATA = 0x40300040         # INITIALIZED_DATA|READ, 4-byte align


def _name_field(name, strings):
    """8-byte name field: inline if <=8 bytes, else \\0\\0\\0\\0 + u32 string-table
    offset. `strings` is a bytearray whose first 4 bytes are its own length."""
    b = name.encode("latin1")
    if len(b) <= 8:
        return b + b"\x00" * (8 - len(b))
    off = len(strings)
    strings += b + b"\x00"
    return b"\x00\x00\x00\x00" + struct.pack("<I", off)


def write_obj(name, code, relocs=None, pdata=None, machine=0x01F2):
    """Emit a PowerPC-BE COFF object with one `.text` (holding `code`, big-endian)
    and one function symbol `name`. `relocs` is [(offset, rtype, symbol)] naming
    external symbols the code references (each becomes an undefined external +
    a relocation record). Optional `pdata` bytes become a `.pdata` section.

    Standard PE/COFF layout (no XDK COMDAT/watermark quirks); round-trips through
    the Obj reader above, which is how it's verified."""
    relocs = relocs or []
    strings = bytearray(struct.pack("<I", 4))     # string table: starts as just its size

    # --- symbol table (built first so relocs can reference symbol indices) ---
    # index bookkeeping counts aux records (they occupy a slot each).
    syms = []                                      # (name, value, secnum, type, sclass, aux_bytes|None)
    symindex = {}
    nsec = 2 if pdata else 1

    def add_sym(nm, value, secnum, typ, sclass, aux=None):
        idx = sum(1 + (1 if s[5] is not None else 0) for s in syms)
        symindex[nm] = idx
        syms.append((nm, value, secnum, typ, sclass, aux))
        return idx

    # .text section symbol + its section-definition aux record
    text_aux = struct.pack("<IHHIHBxxx", len(code), len(relocs), 0, 0, 0, 0)
    add_sym(".text", 0, 1, 0, _SYM_CLASS_STATIC, text_aux)
    if pdata:
        pd_aux = struct.pack("<IHHIHBxxx", len(pdata), 0, 0, 0, 0, 0)
        add_sym(".pdata", 0, 2, 0, _SYM_CLASS_STATIC, pd_aux)
    # the defined function
    add_sym(name, 0, 1, _DT_FUNCTION, _SYM_CLASS_EXTERNAL)
    # undefined externals referenced by relocations
    for _off, _rt, sym in relocs:
        if sym not in symindex:
            add_sym(sym, 0, 0, _DT_FUNCTION, _SYM_CLASS_EXTERNAL)   # sec 0 = UNDEFINED

    nsym = sum(1 + (1 if s[5] is not None else 0) for s in syms)

    # --- file layout offsets ---
    hdr_len = 20 + nsec * 40
    text_raw = hdr_len
    text_rel = text_raw + len(code)
    off = text_rel + len(relocs) * 10
    if pdata:
        pdata_raw = off
        off = pdata_raw + len(pdata)
    symptr = off

    # --- COFF header ---
    out = bytearray()
    out += struct.pack("<HHIIIHH", machine, nsec, 0, symptr, nsym, 0, 0x0000)

    # --- section headers ---
    out += _name_field(".text", strings)
    out += struct.pack("<IIIIIIHHI", 0, 0, len(code), text_raw,
                       text_rel if relocs else 0, 0, len(relocs), 0, _CH_TEXT)
    if pdata:
        out += _name_field(".pdata", strings)
        out += struct.pack("<IIIIIIHHI", 0, 0, len(pdata), pdata_raw,
                           0, 0, 0, 0, _CH_PDATA)

    # --- section data + relocations ---
    out += code
    for o, rt, sym in relocs:
        out += struct.pack("<IIH", o, symindex[sym], rt)
    if pdata:
        out += pdata

    # --- symbol table ---
    for nm, value, secnum, typ, sclass, aux in syms:
        out += _name_field(nm, strings)
        out += struct.pack("<IhHBB", value, secnum, typ, sclass, 1 if aux else 0)
        if aux:
            out += aux

    # --- string table ---
    struct.pack_into("<I", strings, 0, len(strings))
    out += strings
    return bytes(out)


def _selfcheck():
    """Emit an object with a bl to an external, read it back, and confirm the
    function bytes, the relocation, and the ignore-mask all round-trip; then
    check masked_eq ignores exactly the reloc's LI field."""
    import importlib
    ppcasm = (importlib.import_module("ppcasm") if "ppcasm" in sys.modules
              else __import__("ppcasm"))
    base = 0
    asm = "mflr r0\nbl 0x0\naddi r3, r3, 0x1\nblr\n"
    code = ppcasm.assemble(asm, base)
    # the bl is at offset 4; mark it a REL24 to an external "helper"
    obj = write_obj("myfunc", code, relocs=[(4, REL_REL24, "helper")])
    o = Obj(obj)
    if o.machine != 0x01F2:
        return False, "machine wrong"
    funcs = o.functions()
    if "myfunc" not in funcs:
        return False, "function symbol not read back: %r" % list(funcs)
    fb, mask = funcs["myfunc"]
    if fb != code:
        return False, "function bytes not preserved"
    # the REL24 site must mask the LI field at offset 4 (bytes 4..7)
    if mask.get(4) != 0x03 or mask.get(5) != 0xFF or mask.get(7) != 0xFC:
        return False, "REL24 mask wrong at bl site: %r" % {k: mask[k] for k in sorted(mask)}
    # two builds differing only in the bl displacement compare equal under mask
    other = bytearray(code)
    other[4:8] = struct.pack(">I", (18 << 26) | (0x400 & 0x03FFFFFC) | 1)   # different LI+LK
    if not masked_eq(fb, bytes(other), mask):
        return False, "masked_eq failed to ignore the relocated field"
    # address-load (lis/addi @hi/@lo) -> REFHI/REFLO + PAIR, masking both imm16s
    code2, rel2 = _assemble_with_relocs(
        ppcasm, "lis r3, gTable@hi\naddi r3, r3, gTable@lo\nblr\n")
    types = sorted(t for _o, t, _s in rel2)
    if types != [REL_REFHI, REL_REFLO, REL_PAIR, REL_PAIR]:
        return False, "address-load reloc types wrong: %r" % [hex(t) for t in types]
    o2 = Obj(write_obj("loadaddr", code2, relocs=rel2))
    _fb2, m2 = o2.functions()["loadaddr"]
    if not (m2.get(2) == 0xFF and m2.get(3) == 0xFF and       # lis imm16
            m2.get(6) == 0xFF and m2.get(7) == 0xFF):         # addi imm16
        return False, "REFHI/REFLO mask wrong: %r" % {k: hex(m2[k]) for k in sorted(m2)}
    return True, ("emit .obj -> read back: REL24 + REFHI/REFLO address-load relocs, "
                  "masks + masked_eq")


def _assemble_with_relocs(ppcasm, src, base=0):
    """Assemble asm text; a `bl`/`b` to a name that is not a locally-defined
    label becomes a placeholder branch + a REL24 relocation against that external
    symbol. Returns (code_bytes, [(offset, rtype, symbol)])."""
    lines = src.splitlines()

    def strip(ln):
        return ln.split("#", 1)[0].split(";", 1)[0].strip()

    labels, a = {}, base
    for ln in lines:                         # pass 1: label addresses
        s = strip(ln)
        if not s:
            continue
        if s.endswith(":"):
            labels[s[:-1]] = a
        else:
            a += 4
    out, relocs, addr = bytearray(), [], base
    for ln in lines:                         # pass 2: assemble
        s = strip(ln)
        if not s or s.endswith(":"):
            continue
        parts = s.replace(",", " ").split()
        mn, ops = parts[0].lower(), parts[1:]
        if "@" in s:                             # sym@hi / sym@lo address load
            sym = rtype = None
            newops = []
            for op in ops:
                if "@" in op:
                    at = op.index("@")
                    sym = op[:at]
                    rest = op[at + 1:]
                    memsuf = rest[rest.index("("):] if "(" in rest else ""
                    suffix = (rest[:rest.index("(")] if "(" in rest else rest).lower()
                    rtype = REL_REFHI if suffix in ("hi", "ha", "h") else REL_REFLO
                    newops.append("0" + memsuf)
                else:
                    newops.append(op)
            if sym is not None:
                w = ppcasm.assemble_one(mn + " " + " ".join(newops), addr, labels)
                # MS COFF requires each REFHI/REFLO be followed by a PAIR record
                relocs.append((addr - base, rtype, sym))
                relocs.append((addr - base, REL_PAIR, sym))
                out += struct.pack(">I", w)
                addr += 4
                continue
        if mn in ("bl", "b", "bla", "ba") and ops and ops[0] not in labels:
            is_num = True
            try:
                int(ops[0], 0)
            except ValueError:
                is_num = False
            if not is_num:                   # external symbol -> placeholder + reloc
                w = ppcasm.assemble_one("%s 0x%X" % (mn, addr), addr)   # branch-to-self, disp 0
                relocs.append((addr - base, REL_REL24, ops[0]))
                out += struct.pack(">I", w)
                addr += 4
                continue
        w = ppcasm.assemble_one(s, addr, labels)
        if w is None:
            continue
        out += struct.pack(">I", w)
        addr += 4
    return bytes(out), relocs


def main():
    a = sys.argv[1:]
    if a and a[0] == "--selftest":
        ok, msg = _selfcheck()
        print(("PASS: " if ok else "FAIL: ") + msg)
        sys.exit(0 if ok else 1)
    if len(a) >= 2 and a[0] == "emit":
        # ppcobj emit <asm.s> <out.obj> [--name N]  — assemble + wrap as a .obj.
        # A `bl <label>` to a label not defined in the file becomes a REL24
        # relocation against an undefined external of that name.
        import importlib
        ppcasm = (importlib.import_module("ppcasm") if "ppcasm" in sys.modules
                  else __import__("ppcasm"))
        src = open(a[1], encoding="latin1").read()
        name = a[a.index("--name") + 1] if "--name" in a else "func"
        code, relocs = _assemble_with_relocs(ppcasm, src)
        obj = write_obj(name, code, relocs=relocs)
        open(a[2], "wb").write(obj)
        print("wrote %s: %d bytes, function '%s', %d relocation(s)"
              % (a[2], len(obj), name, len(relocs)))
        return
    pos = [t for t in a if not t.startswith("-")]
    if pos and pos[0] == "dump":                 # GUI passes the verb explicitly
        pos = pos[1:]
    if not pos:
        sys.exit("usage: ppcobj [dump] <foo.obj>       dump a COFF object\n"
                 "       ppcobj emit <asm.s> <out.obj> [--name N]\n"
                 "       ppcobj --selftest")
    o = Obj(pos[0])
    print("machine=0x%04X  %d sections  %d symbols" % (o.machine, o.nsec, o.nsym))
    for name, (fb, mask) in o.functions().items():
        print("\n%s: %d bytes, %d masked byte(s)" % (name, len(fb), len(mask)))
        for i in range(0, len(fb), 4):
            w = fb[i:i + 4]
            mm = "".join("%02x" % mask.get(i + k, 0) for k in range(4))
            flag = "  <- reloc" if any(mask.get(i + k) for k in range(4)) else ""
            print("  +%03X  %s   mask=%s%s" % (i, w.hex(), mm, flag))


if __name__ == "__main__":
    main()
