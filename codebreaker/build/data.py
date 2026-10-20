"""data.py -- a module's initialised data as C, so a rebuilt module carries its own.

The per-function builds borrow the original's data: a 32-bit function's globals are bound to
the original's addresses, a 16-bit one's to offsets in the original's data segment. A whole
module has to bring that data along. This writes it as C the compiler lays out exactly as the
original has it, with every pointer in it written as a pointer.

    from codebreaker.build import data
    d = data.data32(path, function_at)        # 32-bit: each data section as one object
    d = data.data16(path, function_at)        # 16-bit: the automatic data segment
    d.source          the C file defining the data
    d.declarations    what the functions' header needs to refer to it
    d.pointers        pointers written as pointers;  d.unresolved: [(where, why)] left as numbers

32-bit. Each data section becomes an object `codebreaker_data_N`: runs of bytes and, at every
place the module's relocation table marks, a pointer member -- to a reconstructed function by
name, or to a place in a data object. Exported data is an object of its own under its
exported name, cut out of its section (`layout_holds` checks the pieces stayed together). The functions' references are
moved the same way: `relocate_header` rebinds the header's globals, `relocate_text` rewrites
the address constants in a function. Which constants ARE addresses is not guessed from their
value: only a constant that appears among the relocated words inside that function's own code
is rewritten (`address_constants`). A module without a relocation table (most .EXEs) gives no
such evidence and cannot be moved; `data32` says so.

16-bit. Near data is addressed by plain offsets from DS, which no table marks, so the data
segment is not moved: it is written as one object that must be linked FIRST so that it starts
at offset 0 of the automatic data segment, and every offset in the code stays right. Far
pointers stored in it (the segment's relocation records) are written as far pointers to the
function, import or data they name. A selector stored on its own, or a bare offset of a
function, has no C form here: it is left zero and listed in `unresolved`.
"""
import re
import struct

from codebreaker.formats.ne import NEFile
from codebreaker.formats.pe import open_pe

# what a section may hold that the linker or the resource compiler makes anew: exports, imports,
# resources, relocations, debug information
_REMADE = (0, 1, 2, 5, 6)
_CODE = 0x20000020


class Data(object):
    def __init__(self):
        self.source = self.declarations = ""
        self.sections = []                # (number, name, virtual address, size) -- 32-bit
        self.parts = []                   # (C name, virtual address, size, section number): the objects written
        self.pointers = 0
        self.unresolved = []              # (where, why)
        self.imports = {}                 # 16-bit: {symbol: (module, ordinal)} named only by data
        self.movable = True
        self.size = 0
        self.exported = {}                # 16-bit: {(segment, offset): C name} of exported data
        self.pieces = []                  # 16-bit: the segment's pieces, as data16 cut it (see _pieces)
        self.cuts = {}                    # 16-bit: {offset: name} where an exported variable starts

    def place(self, va):
        """The C expression for an address inside a carried section, or None."""
        for symbol, start, size, _n in self.parts:
            if start <= va < start + size:
                return "((char *)&%s + 0x%x)" % (symbol, va - start)
        return None


# ---------------------------------------------------------------------------------------------
# writing an object: bytes, runs of zeros, pointers
# ---------------------------------------------------------------------------------------------
def _rows(blob):
    out = []
    for i in range(0, len(blob), 16):
        row = blob[i:i + 16]
        text = "".join(chr(b) if 0x20 <= b < 0x7F and b not in (0x2A, 0x2F, 0x5C) else "." for b in row)
        out.append("        %-96s/* %s */" % ("".join("0x%02x," % b for b in row), text))
    return out


def _object(struct_name, name, pieces, far=False):
    """C for one object. pieces: ("bytes", offset, blob) | ("zero", offset, count) |
    ("pointer", offset, C type, C expression, note)."""
    if len(pieces) > 1 and pieces[-1][0] == "zero":          # zeros at the very end need no initialiser
        pieces = pieces[:-1] + [("tail",) + tuple(pieces[-1][1:])]
    members, values = [], []
    for p in pieces:
        kind, off = p[0], p[1]
        if kind == "bytes":
            members.append("    uint1 b_%05x[%d];" % (off, len(p[2])))
            values.append("    {\n%s\n    }," % "\n".join(_rows(p[2])))
        elif kind == "zero":
            members.append("    uint1 z_%05x[%d];" % (off, p[2]))
            values.append("    { 0 },")
        elif kind == "tail":
            members.append("    uint1 z_%05x[%d];" % (off, p[2]))
        else:
            members.append("    %s p_%05x;" % (p[2], off))
            values.append("    %s,%s" % (p[3], ("    /* %s */" % p[4]) if p[4] else ""))
    # packed: each member exactly at its place -- the compiler would otherwise put a word on an
    # even offset and move everything after it (the code reads the data at the original's places)
    return "#pragma pack(push, 1)\nstruct %s {\n%s\n};\n#pragma pack(pop)\nstruct %s %s%s = {\n%s\n};\n" % (
        struct_name, "\n".join(members), struct_name, "__far " if far else "", name, "\n".join(values))


_ZEROS = re.compile(rb"\x00{64,}")


def _pieces(blob, size, pointers):
    """Cut `blob` (padded with zeros to `size`) at the pointer places, and long runs of zeros
    out of the bytes between. pointers: {offset: (width, C type, expression, note)}."""
    out, at = [], 0

    def plain(lo, hi):
        buf, done = bytes(blob[lo:hi]).ljust(hi - lo, b"\0"), 0
        for m in _ZEROS.finditer(buf):
            if m.start() > done:
                out.append(("bytes", lo + done, buf[done:m.start()]))
            out.append(("zero", lo + m.start(), m.end() - m.start()))
            done = m.end()
        if done < len(buf):
            out.append(("bytes", lo + done, buf[done:]))
    for off in sorted(pointers):
        if off < at:
            continue                                         # overlaps the one before: left out
        width, ctype, expr, note = pointers[off]
        plain(at, off)
        out.append(("pointer", off, ctype, expr, note))
        at = off + width
    plain(at, size)
    return out


# ---------------------------------------------------------------------------------------------
# 32-bit
# ---------------------------------------------------------------------------------------------
def relocated_words(pe):
    """{virtual address of a relocated word: the address it holds} from the relocation table."""
    out = {}
    for rva in pe.base_relocs():
        off = pe._rva_to_off(rva)
        if off is not None and off + 4 <= len(pe.data):
            out[pe.image_base + rva] = struct.unpack_from("<I", pe.data, off)[0]
    return out


def address_constants(path, starts):
    """{function address: the set of addresses its code refers to}. `starts` is every function
    address of the module; a function runs to the next one."""
    words = sorted(relocated_words(open_pe(path)).items())
    starts = sorted(starts)
    out, i = {s: set() for s in starts}, 0
    for n, s in enumerate(starts):
        end = starts[n + 1] if n + 1 < len(starts) else s + 0x100000
        while i < len(words) and words[i][0] < s:
            i += 1
        j = i
        while j < len(words) and words[j][0] < end:
            out[s].add(words[j][1])
            j += 1
    return out


def data32(path, function_at, named=None):
    """The data sections of a 32-bit module as C. `function_at(va)` -> the name of the
    reconstructed function at an address, or None. `named` is {address: C name} for places that
    must be objects of their own (exported data): a section is then written as several objects
    one after another, which only works if the compiler keeps them together -- `layout_holds`
    checks that on the compiled object."""
    pe = open_pe(path)
    d = Data()
    base = pe.image_base
    remade = {pe.datadirs[i][0] for i in _REMADE if i < len(pe.datadirs) and pe.datadirs[i][0]}
    d.movable = bool(pe.base_relocs())
    named = dict(named or {})
    for n, s in enumerate(pe.sections, 1):
        size = max(s["vsize"], s["rawsize"]) if s["vsize"] else s["rawsize"]
        if s["chars"] & _CODE or s["vaddr"] in remade or not size or s["name"] in (".rsrc", ".reloc", ".idata", ".edata"):
            continue
        start = base + s["vaddr"]
        d.sections.append((n, s["name"], start, size))
        cuts = [start] + sorted(va for va in named if start < va < start + size) + [start + size]
        for lo, hi in zip(cuts, cuts[1:]):
            d.parts.append((named.get(lo) or "codebreaker_data_%d%s" % (n, "_%x" % (lo - start) if lo > start else ""),
                            lo, hi - lo, n))
    words = relocated_words(pe)
    out = ["/* The initialised data of %s: each data section laid out as in the original. A member"
           % pe.path.replace("\\", "/").rsplit("/", 1)[-1].lower(),
           " * p_NNNNN is a pointer the original's relocation table marks. */", ""]
    decls = []
    for symbol, start, size, n in d.parts:
        s = pe.sections[n - 1]
        skip = start - (base + s["vaddr"])
        blob = pe.data[s["rawptr"] + skip:s["rawptr"] + max(skip, min(s["rawsize"], skip + size))]
        pointers = {}
        for va in range(start, start + len(blob) - 3):
            if va not in words:
                continue
            target = words[va]
            fn = function_at(target)
            if fn:
                pointers[va - start] = (4, "void *", "(void *)%s" % fn, "")
            elif d.place(target):
                pointers[va - start] = (4, "void *", d.place(target), "")
            else:
                d.unresolved.append(("%s+0x%x" % (s["name"], va - start + skip),
                                     "points at 0x%x, which is neither a reconstructed function nor carried data" % target))
        d.pointers += len(pointers)
        d.size += size
        decls.append("struct layout_of_%s; extern struct layout_of_%s %s;" % (symbol, symbol, symbol))
        out.append("/* section %s + 0x%x: 0x%x bytes at 0x%x in the original */" % (s["name"], skip, size, start))
        out.append(_object("layout_of_%s" % symbol, symbol, _pieces(blob, size, pointers)))
    d.declarations = "\n".join(decls) + "\n"
    d.source = "\n".join(out)
    return d


def _public_offsets(obj):
    """{symbol: (segment index, offset)} from an object file's public-name records."""
    out, i = {}, 0

    def index(p):
        return (((obj[p] & 0x7F) << 8) | obj[p + 1], p + 2) if obj[p] & 0x80 else (obj[p], p + 1)
    while i + 3 <= len(obj):
        kind, end = obj[i], i + 3 + (obj[i + 1] | (obj[i + 2] << 8))
        if end > len(obj):
            break
        if kind in (0x90, 0x91, 0xB6, 0xB7):
            _group, p = index(i + 3)
            seg, p = index(p)
            if seg == 0:
                p += 2
            wide = 4 if kind & 1 else 2
            while p < end - 1:
                n = obj[p]
                name = obj[p + 1:p + 1 + n].decode("latin-1")
                p += 1 + n
                out[name] = (seg, int.from_bytes(obj[p:p + wide], "little"))
                _type, p = index(p + wide)
        i = end
    return out


def layout_holds(obj, d):
    """Whether, in the compiled data object, the objects a section was written as lie one after
    another as they do in the original."""
    where = _public_offsets(obj)
    first = {}
    for symbol, start, _size, n in d.parts:
        at = where.get("_" + symbol)
        if at is None:
            return False
        if n not in first:
            first[n] = (at, start)
        (seg, off), start0 = first[n]
        if at[0] != seg or at[1] - off != start - start0:
            return False
    return True


_GLOBAL = re.compile(r"(?m)^(#define \w+ \(\*\([^()]*\*\))0x([0-9a-f]+)\)$")
_CONSTANT = re.compile(r"(?<![\w.])(-?)0x([0-9a-fA-F]+)\b")


def relocate_header(header, d):
    """Rebind the header's globals from the original's addresses to the carried data.
    -> (header, [addresses that are in no carried section])"""
    lost = []

    def fix(m):
        place = d.place(int(m.group(2), 16))
        if not place:
            lost.append(int(m.group(2), 16))
            return m.group(0)
        return "%s%s)" % (m.group(1), place)
    return d.declarations + _GLOBAL.sub(fix, header), lost


def relocate_text(text, addresses, d, function_at):
    """Rewrite the address constants of one function. `addresses` is what its code refers to
    (see address_constants). -> (text, [addresses that could not be placed])"""
    lost = []

    def fix(m):
        value = int(m.group(2), 16)
        if m.group(1):
            value = -value & 0xFFFFFFFF
        if value not in addresses:
            return m.group(0)
        fn = function_at(value)
        if fn:
            return "((int4)%s)" % fn
        place = d.place(value)
        if place:
            return "((int4)%s)" % place
        lost.append(value)
        return m.group(0)
    head, brace, body = text.partition("{")
    return head + brace + _CONSTANT.sub(fix, body), lost


# ---------------------------------------------------------------------------------------------
# 16-bit
# ---------------------------------------------------------------------------------------------
def _places(nef, seg, blob):
    """Every relocated place of a segment: [(offset, source kind, addend, record)], following
    the chains the file keeps (each place holds the offset of the next; 0xFFFF ends one)."""
    out = []
    for r in nef.relocations(seg):
        off, hops = r["offset"], 0
        while off != 0xFFFF and off + 1 < len(blob) and hops < 0x10000:
            word = struct.unpack_from("<H", blob, off)[0]
            out.append((off, r["src"], word if r["additive"] else 0, r))
            if r["additive"]:
                break
            off, hops = word, hops + 1
    return out


def data16(path, function_at, far_segments=(), segment_symbol=None):
    """The automatic data segment of a 16-bit module as C. `function_at((segment, offset))` ->
    the reconstructed function there, or None; `far_segments` are the other segments the build
    carries as `codebreaker_segment_N`."""
    nef = NEFile(path)
    d = Data()
    auto = nef.h.get("autodata_seg")
    segs = nef.segments()
    if not auto or auto > len(segs):
        d.source = "/* %s has no automatic data segment. */\nchar codebreaker_data = 1;\n" % nef.module_name()
        return d
    s = segs[auto - 1]
    blob = bytearray(nef.data[s["file_off"]:s["file_off"] + s["length"]] if s["file_off"] else b"")
    size = max(len(blob), min(s["minalloc"], 0x10000)) if s["file_off"] else min(s["minalloc"], 0x10000)
    entries = {e["ordinal"]: e for e in nef.entries() if e.get("kind") in ("FIXED", "MOVEABLE")}
    refs = nef.module_refs()
    pointers, externs = {}, []
    for off, src, add, r in _places(nef, s, bytes(blob)):
        width = {3: 4, 2: 2, 5: 2, 0: 1}.get(src, 2)
        blob[off:off + width] = bytes(width)                 # the file keeps a chain link there, not a value
        where = "data+0x%04x" % off
        if r["kind"] in ("IMPORTORD", "IMPORTNAME"):
            mod = refs[r["module"] - 1] if 0 < r["module"] <= len(refs) else "?"
            if r["kind"] == "IMPORTORD":
                symbol = "%s_%d" % (re.sub(r"\W", "_", mod), r["ordinal"])
                d.imports[symbol] = (mod, r["ordinal"])
            else:
                symbol = re.sub(r"\W", "_", nef.imported_name(r["name_off"]))
                d.imports[symbol] = (mod, nef.imported_name(r["name_off"]))
            if src == 3 and not add:
                pointers[off] = (4, "code __far *", "(code __far *)%s" % symbol, "")
                externs.append(symbol)
            elif src == 5 and not add:                   # its offset: KERNEL's __WINFLAGS, a value
                pointers[off] = (2, "uint2", "(uint2)%s" % symbol, "")
                externs.append(symbol)
            elif src == 2:                               # its selector (KERNEL's __0040H)
                pointers[off] = (2, "__segment", "(__segment)%s" % symbol, "")
                externs.append(symbol)
            else:
                d.unresolved.append((where, "part of the address of %s" % symbol))
            continue
        if r["kind"] != "INTERNAL":
            d.unresolved.append((where, "a floating-point fixup"))
            continue
        if r["seg"] == 0xFF:
            e = entries.get(r["entry"])
            if not e:
                d.unresolved.append((where, "names entry %d, which the module does not have" % r["entry"]))
                continue
            tseg, toff = e["seg"], e["offset"]
        else:
            tseg, toff = r["seg"], r["entry"]
        toff = (toff + add) & 0xFFFF
        fn = function_at((tseg, toff))
        if src == 5 and tseg == auto:                        # an offset into this very segment: a number
            struct.pack_into("<H", blob, off, toff)
        elif src == 3 and fn:
            pointers[off] = (4, "code __far *", "(code __far *)%s" % fn, "")
        elif src == 3 and tseg == auto:
            pointers[off] = (4, "void __far *", "(void __far *)((char *)&codebreaker_data + 0x%x)" % toff, "")
        elif src == 3 and tseg in far_segments:
            pointers[off] = (4, "void __far *", "(void __far *)(codebreaker_segment_%d + 0x%x)" % (tseg, toff), "")
        elif src == 2 and tseg == auto:                      # a segment's selector, which the loader
            pointers[off] = (2, "__segment", "(__segment)&codebreaker_data", "")   # writes: its own,
        elif src == 2 and tseg in far_segments:                                    # a segment carried,
            pointers[off] = (2, "__segment", "(__segment)codebreaker_segment_%d" % tseg, "")
        elif src == 2 and segment_symbol and segment_symbol(tseg):                 # a code segment's
            pointers[off] = (2, "__segment", "(__segment)&%s" % segment_symbol(tseg),
                             "the selector of segment %d" % tseg)
        else:
            what = {5: "an offset in segment %d" % tseg,
                    3: "a far pointer to %d:%04x" % (tseg, toff)}.get(src, "a fixup of kind %d" % src)
            d.unresolved.append((where, what + ", which has no C form here"))
    d.pointers = len(pointers)
    d.size = size
    head = ["/* The automatic data segment of %s: 0x%x bytes (0x%x of them in the file)." % (
        nef.module_name(), size, len(blob)),
        " * It must be the first thing linked into the data segment: the code addresses it by offset. */"]
    head += ["extern uint4 __far __stdcall %s();" % x for x in sorted(set(externs))]
    # data the module exports (KERNEL's THHOOK): the segment is cut there, and from there on it
    # is a variable of the export's own name -- the parts follow one another in the segment
    names = {}
    for kind in ("resident_names", "nonresident_names"):
        got = getattr(nef, kind)
        for name, ordinal in (got() if callable(got) else got) or ():
            if ordinal:
                names[ordinal] = name
    cuts = {e["offset"]: names.get(o, "export_%d" % o) for o, e in entries.items()
            if e.get("seg") == auto and 0 < e["offset"] < size}
    pieces = _cut(_pieces(bytes(blob), size, pointers), sorted(cuts))
    d.pieces, d.cuts = pieces, {o: re.sub(r"\W", "_", n) for o, n in cuts.items()}
    d.exported = {}
    parts, start = [], 0
    for at in sorted(cuts) + [size]:
        part = [piece for piece in pieces if start <= piece[1] < at]
        name = "codebreaker_data" if start == 0 else re.sub(r"\W", "_", cuts[start])
        if part:
            parts.append(_object("codebreaker_data_%s" % (name if start else "segment"), name, part))
            if start:
                d.exported[(auto, start)] = name
        start = at
    # (a far pointer into one of the module's other data segments names its bytes: declared here,
    # not left to the C's header -- with no C function using segment 41, SETUPX's data did not
    # compile, and the build fell back to a data segment of one byte)
    named = sorted({int(n) for n in re.findall(r"\bcodebreaker_segment_(\d+)\b", "\n".join(parts))})
    head = head + ['extern uint1 __based(__segname("CODEBREAKER_SEGMENT_%d")) codebreaker_segment_%d[];' % (n, n)
                   for n in named]
    d.source = "\n".join(head) + "\n" + "\n".join(parts)
    return d


def _cut(pieces, offsets):
    """Pieces (see _pieces) cut so that each offset starts one."""
    out = []
    for p in pieces:
        kind, off = p[0], p[1]
        length = len(p[2]) if kind == "bytes" else p[2] if kind == "zero" else None
        inner = [c for c in offsets if length and off < c < off + length]
        if not inner:
            out.append(p)
            continue
        bounds = [off] + inner + [off + length]
        for lo, hi in zip(bounds, bounds[1:]):
            out.append(("bytes", lo, p[2][lo - off:hi - off]) if kind == "bytes" else ("zero", lo, hi - lo))
    return out


def far_segments(segments):
    """C for the other segments a 16-bit module's code reads from: {number: bytes}. Each is a
    segment of its own, starting with it: code (and assembly above all) reaches into it by the
    original's offsets."""
    out = []
    for n, image in sorted(segments.items()):
        out.append('uint1 __based(__segname("CODEBREAKER_SEGMENT_%d")) codebreaker_segment_%d[] = {\n%s\n};'
                   % (n, n, "\n".join(_rows(bytes(image)))))
    return "\n".join(out) + ("\n" if out else "")
