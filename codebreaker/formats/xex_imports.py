"""xex_imports.py -- an Xbox 360 module's imports, one per line: library, ordinal, name, kind, slot, thunk.

    python -m codebreaker xex-imports <module.xex>
    from codebreaker.formats import xex_imports
    xex_imports.imports(path)       [(library, ordinal, name, kind, slot address, thunk address or None)]

Works for any module, a title or a system one (any load address). Names come from
symbols/console_ordinals.json; an ordinal missing there is named <library>_<ordinal>.

Each imported function has two records: a 4-byte slot (filled by the loader) and a 16-byte code
thunk (which the recompiler turns into its `__imp__Name` function). An imported variable has only
the slot. The import header (0x000103FF) is: the total size, the size of the string table, the
number of strings, the library names (each padded to 4 bytes), then import blocks until the total
is used -- and a library can have several blocks (dashboard 17559's dash.xex has two for xam).
A block names its library by index in its byte at 0x25 (early devkit dashboards -- 2255, 2258,
2832 -- set other bits in the byte before it), then lists the addresses of its records; a
record's word in the image has the ordinal in its low 16 bits and, for a thunk, a non-zero top byte.
"""
import struct
import sys

IMPORT_HEADER = 0x000103FF


def _names():
    from codebreaker import knowledge                       # (console_ordinals.json, as the knowledge base has it)
    table = knowledge.table("xbox360", "ordinal_names")
    return {lib: {int(k): v for k, v in names.items()} for lib, names in table.items() if not lib.startswith("_")}


def imports(path):
    from codebreaker.formats import powerpc_image, xex
    with open(path, "rb") as f:
        data = f.read()
    o = {key: value for _at, key, value in xex.optional_headers(data)}.get(IMPORT_HEADER)
    if o is None:
        return []
    img = powerpc_image.read(xex.basefile(path))
    total, strsize, nlibs = struct.unpack_from(">III", data, o)
    libs, p = [], o + 12
    while len(libs) < nlibs:
        end = data.index(b"\0", p)
        libs.append(data[p:end].decode("latin-1"))
        p += (end - p + 1 + 3) & ~3
    p = o + 12 + strsize
    found = {}
    while p < o + total:
        size = struct.unpack_from(">I", data, p)[0]
        if size == 0:
            break
        index, count = struct.unpack_from(">HH", data, p + 0x24)
        lib = libs[index & 0xFF]
        for rec in struct.unpack_from(">%dI" % count, data, p + 0x28):
            word = img.word(rec)
            if word is None:
                continue
            entry = found.setdefault((lib, word & 0xFFFF), {})
            entry["thunk" if word >> 24 else "slot"] = rec
        p += size
    names = _names()
    out = []
    for (lib, ordinal), e in sorted(found.items()):
        name = names.get(lib, {}).get(ordinal)
        kind = "function" if "thunk" in e else "variable"
        out.append((lib, ordinal, name or "%s_%d" % (lib.split(".")[0], ordinal), kind, e.get("slot"), e.get("thunk")))
    return out


def main(argv):
    if not argv:
        print(__doc__.split("\n\n")[0] + "\n\n" + __doc__.split("\n\n")[1])
        return 2
    print("library\tordinal\tname\tkind\tslot\tthunk")
    for lib, ordinal, name, kind, slot, thunk in imports(argv[0]):
        print("%s\t%d\t%s\t%s\t%s\t%s" % (lib, ordinal, name, kind, "0x%08X" % slot if slot else "",
                                          "0x%08X" % thunk if thunk else ""))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
