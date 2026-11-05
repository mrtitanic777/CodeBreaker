"""omf.py -- Intel/Microsoft OMF-86 object modules: read one whole, write the few kinds exact makes.
Answered by the C++ (native/formats/omf: formats.dll); its Python reader is gone (docs/NATIVE.md,
step 4), but for patched and rename_symbols (a spec read by a regular expression; a Python function
applied to every name).
(Also the 32-bit records -- SEGDEF, PUBDEF, LEDATA, LIDATA, FIXUPP with 4-byte offsets -- that a
32-bit compiler of the period writes, e.g. MSVC 2.0's: build/masm.py, invert/msvc2.)

    o = omf.read(data)
        o.seg      {segment name: bytes}            (LEDATA and LIDATA laid out)
        o.segs     [(name, class, length, align)]   in the order defined
        o.groups   {group: [segment names]}
        o.pubs     [(name, segment or None, offset)]
        o.ext      [names]                          (EXTDEF and COMDEF)
        o.comm     {name: (size, far)}              the communal variables (COMDEF)
        o.fix      [(segment, offset, kind, target, displacement)]
        o.fixloc   [the fixup's 4-bit location field], one per o.fix (9, 11, 13: 32-bit kinds)
                   kind: 'off' 'base' 'ptr' 'lo' 'rel' (self-relative); target: ('ext', name) /
                   ('seg', name) / ('grp', name)
        o.raw      the bytes read
    omf.functions(o, seg="_TEXT")  -> [(name, offset, length)] the segment's publics, in order
    omf.code_segments(o)           -> the object's code segments
    omf.holes(o, seg)              -> offsets of the segment the linker fills in
    omf.library(data)              -> [(module name, bytes)] of a .LIB
    omf.records(data)              -> [(type, payload)]
    omf.record(type, payload)      -> one record's bytes, checksum included
    omf.name(s)                    -> a length-prefixed name
"""

from codebreaker.formats import native


# ---- the C++ reader (native/formats/omf): its Python is gone (docs/NATIVE.md, step 4) ----------
def _no_obj(answer, data, *_):
    o = Obj()
    o.__dict__.update(answer)
    o.raw = bytes(data)
    return o


def _no_walk(answer, data, *_):
    items, error = answer
    if isinstance(data, bytearray):
        items = [(i, t, bytearray(body)) for i, t, body in items]

    def records():
        yield from items
        if error is not None:
            raise native._exception(*error)
    return records()


def _no_holes_args(o, seg):
    return None, (o.fix, seg)


def _no_code_segments_args(o):
    return None, (o.segs,)


def _no_functions_args(o, seg="_TEXT"):
    return None, (o.pubs, len(o.seg.get(seg, b"")), o.filled.get(seg, 0), seg)


def _idx(r, q):
    return (((r[q] & 0x7F) << 8) | r[q + 1], q + 2) if r[q] & 0x80 else (r[q], q + 1)


@native.answered("omf.records")
def records(data):
    ...


@native.answered("omf.record", data_arg=1)
def record(t, payload):
    ...


def patched(raw, spec):
    """the module with bytes of a segment's data replaced: spec SEG+OFF=HEXBYTES (in its LEDATA records)"""
    import re
    m = re.fullmatch(r"(\w+)\+([0-9a-fA-F]+)=([0-9a-fA-F]+)", spec)
    return native.call("omf.patched", raw, m.group(1), int(m.group(2), 16), bytes.fromhex(m.group(3)))


@native.answered("omf.name", data_arg=None)
def name(s):
    ...


class Obj(object):
    pass


@native.answered("omf.read", result=_no_obj)
def _read(data):
    ...


def read(data):
    """The object module `data` read whole (an omf.Obj: see the module docstring)."""
    from codebreaker.exact import aout
    if aout.is_aout(data):                                  # (a Version 7 a.out: XENIX's compiler)
        return aout.read(data)
    return _read(data)


@native.answered("omf.library")
def library(data):
    """[(name, module bytes)] of an OMF library (.LIB), in its order; a name met again is NAME#2, #3 ..."""


@native.answered("omf.holes", args_of=_no_holes_args)
def holes(o, seg):
    """the offsets of a segment's bytes the linker fills in"""


@native.answered("omf.code_segments", args_of=_no_code_segments_args)
def code_segments(o):
    """the object's code segments, the first named first (a compiler's NAME_TEXT, or more by #pragma alloc_text)"""


@native.answered("omf.functions", args_of=_no_functions_args)
def functions(o, seg="_TEXT"):
    """[(name, offset, length)] of a code segment, in order. The last ends where the segment's data end (a SEGDEF
    may count a byte more: C 5.0 at times counts one past a closing NOP); code before the first name is
    "(module start)" (an assembly module's)"""


@native.answered("omf.rename_externs")
def rename_externs(data, mapping):
    """the object with external names changed (a compiler that calls its helpers by other names than the
    library the program was linked with)"""


def rename_symbols(data, fn):
    """the object with every external and public name nm made fn(nm) (EXTDEF, COMDEF, PUBDEF): the names a batch
    of candidates was compiled under given back.  formats.dll walks the records: it names the symbols, fn renames
    each (in the order the records hold them), and it writes the object back."""
    names, failed = native.call("omf.symbol_names", data)
    renamed = []
    for n in names:                                         # (fn, the encoding and the length are checked in the order
        new = fn(n)                                         #  the single walk met them; a damaged record raises after)
        if len(new.encode("latin1")) > 255:
            raise ValueError("bytes must be in range(0, 256)")
        renamed.append(new)
    if failed:
        raise native._exception(*failed)
    return native.call("omf.rename_symbols", data, renamed)


@native.answered("omf.raw_object", data_arg=None)
def raw_object(title, code, pubs, data=b"", dpubs=None, text="_TEXT", dalign=1):
    """an object module of the program's own bytes: a run of its code (and of its data), with the public names
    others call it by - a stand-in for a part no source and no library module gives. Its words for the linker
    are the program's already, so it stands only where it stood."""


@native.answered("omf.bss_object", data_arg=None)
def bss_object(names, start, end, title="bssvars", seg="_BSS", para=False):
    """an object module of variables without a value, each a public name at its place in _BSS (start..end): the
    linker places communal names by its own table's order (a hash of the names), and ours are not the original
    names - so they are given their places outright and nothing communal is left to place"""


@native.answered("omf.names_of")
def names_of(data):
    """(the public names an object module defines, the external names it needs)"""


@native.answered("omf.publics")
def publics(data):
    """Exact public symbol names from an OMF object's PUBDEF records (0x90/0x91). Length-prefixed
    names, so no greedy-match errors -- the authoritative `_name@N` decorations wcc386 emitted.
    It reads only those records, so it reads objects read() cannot (names_of: the full reader)."""


@native.answered("omf.externs")
def externs(data):
    """External symbol names an OMF object refers to (EXTDEF records, 0x8C)."""


# ---- record by record, for the readers that walk the records themselves (invert's quad_align)
@native.answered("omf.walk", result=_no_walk)
def walk(data, start=0):
    """(offset, type, payload) of each record from `start`, the payload without its checksum: zero bytes
    between records (a library's padding) passed over, the walk ended by a LIBEND"""


index = _idx                                                # an OMF index at q: (value, the offset past it)


# ---- one object's code, cut at its publics (build/masm, build/msc6, the ne_asm comparers, invert's probes)
# what the linker fills in, by a fix-up's location field: the bytes it covers
_WIDTH = {0: 1, 1: 2, 2: 2, 3: 4, 4: 1, 5: 2, 9: 4, 11: 6, 13: 4}


@native.answered("omf._code")
def _code(data):
    """(code segment's name, its bytes, {offsets the linker fills in}, {public: (segment, offset)}): the code
    segment is the first whose class is CODE or whose name ends TEXT; its bytes run to the end of its data;
    a fix-up covers the bytes of its kind."""


@native.answered("omf.code_segment")
def code_segment(data):
    """(name, bytes, {offsets the linker fills in}) of an object file's code segment."""


@native.answered("omf.function_bytes")
def function_bytes(data):
    """{public name: (bytes, {offsets the linker fills in})} of an object file's code segment, cut between
    consecutive public names. A name keeps the compiler's leading underscore."""


# ---- a listing: python -m codebreaker.formats.omf <file.obj | file.lib> [--raw]
_KIND = {0x80: "THEADR", 0x82: "LHEADR", 0x88: "COMENT", 0x8A: "MODEND", 0x8B: "MODEND", 0x8C: "EXTDEF",
         0x8E: "TYPDEF", 0x90: "PUBDEF", 0x91: "PUBDEF", 0x94: "LINNUM", 0x95: "LINNUM", 0x96: "LNAMES",
         0x98: "SEGDEF", 0x99: "SEGDEF", 0x9A: "GRPDEF", 0x9C: "FIXUPP", 0x9D: "FIXUPP", 0xA0: "LEDATA",
         0xA1: "LEDATA", 0xA2: "LIDATA", 0xA3: "LIDATA", 0xB0: "COMDEF", 0xF0: "LIBHDR", 0xF1: "LIBEND"}


def main(argv):
    """python -m codebreaker.formats.omf <file.obj | file.lib> [--raw]
    each module's segments, publics and externals; --raw every record (type, length)"""
    if not argv:
        print(main.__doc__)
        return 2
    with open(argv[0], "rb") as f:
        data = f.read()
    if "--raw" in argv:
        for off, t, body in walk(data):
            print("@%06x %-8s len=%d" % (off, _KIND.get(t, "?%02X" % t), len(body) + 1))
        return 0
    for nm, module in library(data):
        o = read(module)
        print("== %s (%d bytes)" % (nm, len(module)))
        for s, cls, ln, _al in o.segs:
            print("  segment %-12s class=%-8s %6d bytes" % (s, cls, ln))
        for p, s, off in o.pubs:
            print("  public  %-24s %s:%04x" % (p, s if s is not None else "-", off))
        for e in o.ext:
            print("  extern  %s" % e)
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main(sys.argv[1:]))
