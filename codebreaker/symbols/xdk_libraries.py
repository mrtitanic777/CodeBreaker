"""xdk_libraries.py -- naming the XDK library functions an Xbox 360 program was statically linked with.

    python -m codebreaker xdk-match <program.xex or image> [<XDK lib folder>] [-o names.json]
    from codebreaker.symbols import xdk_libraries
    found = xdk_libraries.match(image_path, lib_folder)     # {address: {"names", "libs", "size"}}

Every COFF archive (.lib) in the XDK's library folder is read, each function's machine code taken
from its object with the bits the linker patches (the fields its relocations name) masked out, and
the program's code searched for the same words. A program linked against the same XDK and
compiler holds the release libraries' functions byte for byte apart from those fields, so a match
names the function for certain -- once a signature is long enough to place (at least three
unmasked words) and lands in exactly one place.

A match counts only where a function can start: a .pdata entry, or after an instruction control
does not fall through (a branch, a return, a trap, padding). A short library function can equal
the tail of a program function word for word; such a hit is refused unless the library symbol is
itself a secondary entry of its section (the register save / restore runs and other hand-written
routines are entered by falling into them).

The library folder defaults to the setting `xdk_libs`, else the XDK's usual install
(Program Files (x86)/Microsoft Xbox 360 SDK/lib/xbox). The names are the XDK's: the output stays
with the program's project, never in the repository.
"""
import json
import os
import struct
import sys
from collections import defaultdict

PPCBE = 0x01F2
# IMAGE_REL_PPC_* (the low byte of a relocation's type)
REL_ADDR32, REL_ADDR24, REL_ADDR16, REL_ADDR14 = 0x2, 0x3, 0x4, 0x5
REL_REL24, REL_REL14, REL_ADDR32NB = 0x6, 0x7, 0xA
REL_SECREL, REL_REFHI, REL_REFLO, REL_PAIR = 0xB, 0x10, 0x11, 0x12
REL_SECRELLO, REL_SECRELHI, REL_GPREL = 0x13, 0x14, 0x15
DEFAULT_LIBS = r"C:\Program Files (x86)\Microsoft Xbox 360 SDK\lib\xbox"


def lib_folder():
    from codebreaker.base import settings
    return settings.get("xdk_libs") or DEFAULT_LIBS


def archive_members(data):
    """(name, bytes) of each object in a COFF archive (`!<arch>`)."""
    if data[:8] != b"!<arch>\n":
        return
    pos, longnames = 8, b""
    while pos + 60 <= len(data):
        hdr = data[pos:pos + 60]
        name = hdr[:16].decode("latin-1").rstrip()
        size = int(hdr[48:58].decode().strip() or 0)
        body = data[pos + 60:pos + 60 + size]
        pos += 60 + size + (size & 1)
        if name in ("/", "/<ECSYMBOLS>/"):
            continue
        if name == "//":
            longnames = body
            continue
        if name.startswith("/") and name[1:].isdigit():
            off = int(name[1:])
            name = longnames[off:longnames.find(b"\0", off)].decode("latin-1")
        yield name.rstrip("/"), body


class Coff(object):
    """A COFF object, as the Xbox 360 compilers write them (the structures little-endian, the code big)."""

    def __init__(self, data):
        self.data = data
        machine, nsec, _t, symptr, nsym, opt, _c = struct.unpack_from("<HHIIIHH", data, 0)
        self.ok = machine == PPCBE and nsym > 0
        if not self.ok:
            return
        self.sections = []
        for i in range(nsec):
            o = 20 + opt + i * 40
            _vs, _va, rawsize, rawptr, relptr, _l, nrel, _n, chars = struct.unpack_from("<IIIIIIHHI", data, o + 8)
            self.sections.append(dict(size=rawsize, ptr=rawptr, relptr=relptr, nrel=nrel, chars=chars))
        strtab = symptr + nsym * 18
        self.symbols, i = [], 0
        while i < nsym:
            o = symptr + i * 18
            raw = data[o:o + 8]
            if raw[:4] == b"\0\0\0\0":
                so = strtab + struct.unpack_from("<I", raw, 4)[0]
                name = data[so:data.find(b"\0", so)].decode("latin-1")
            else:
                name = raw.rstrip(b"\0").decode("latin-1")
            value, secnum, typ, cls, naux = struct.unpack_from("<IhHBB", data, o + 8)
            self.symbols.append(dict(name=name, value=value, sec=secnum, type=typ, cls=cls))
            self.symbols.extend([None] * naux)
            i += 1 + naux

    def relocations(self, sec):
        return [struct.unpack_from("<IIH", self.data, sec["relptr"] + k * 10) for k in range(sec["nrel"])]


def _mask(typ):
    """The bits of an instruction word a relocation of this type fills (None: the whole word)."""
    t = typ & 0xFF
    if t in (REL_REL24, REL_ADDR24):
        return 0x03FFFFFC
    if t in (REL_REL14, REL_ADDR14):
        return 0x0000FFFC
    if t in (REL_REFHI, REL_REFLO, REL_ADDR16, REL_SECRELLO, REL_SECRELHI, REL_GPREL):
        return 0x0000FFFF
    return None


def functions_in(obj):
    """(name, code, mask words, offset in its section) for every function in an object's code sections."""
    for si, sec in enumerate(obj.sections, start=1):
        if not sec["chars"] & 0x20 or sec["size"] < 8:            # IMAGE_SCN_CNT_CODE
            continue
        code = obj.data[sec["ptr"]:sec["ptr"] + sec["size"]]
        n = len(code) // 4
        mask = [0xFFFFFFFF] * n
        skip = False
        for va, _sym, typ in obj.relocations(sec):
            if skip:                                               # a PAIR carries data, not a place
                skip = False
                continue
            if typ & 0xFF == REL_PAIR:
                continue
            if typ & 0xFF == REL_REFHI:
                skip = True
            m = _mask(typ)
            w = (va & ~3) // 4
            if w >= n:
                continue
            if m is None and va & 3:                               # an unaligned 32-bit field: two words
                mask[w] = 0
                if w + 1 < n:
                    mask[w + 1] = 0
            else:
                mask[w] &= ~(m if m is not None else 0xFFFFFFFF) & 0xFFFFFFFF
        syms = sorted((s for s in obj.symbols if s and s["sec"] == si and s["cls"] in (2, 3)
                       and not s["name"].startswith((".", "$")) and (s["type"] & 0x20 or s["cls"] == 2)),
                      key=lambda s: s["value"])
        for k, s in enumerate(syms):
            start = s["value"]
            end = syms[k + 1]["value"] if k + 1 < len(syms) else len(code)
            if end - start >= 8 and not start & 3:
                yield s["name"], code[start:end], mask[start // 4:end // 4], start


def _trim(code, mask):
    """Without the zero words that pad a function to its alignment."""
    n = len(code) // 4
    while n and code[4 * n - 4:4 * n] == b"\0\0\0\0":
        n -= 1
    return code[:4 * n], mask[:n]


def _falls_through(w):
    op = w >> 26
    if w == 0 or op == 18:                                        # padding; b / bl / ba
        return False
    if op == 19 and (w >> 1) & 0x3FF in (16, 528) and (w >> 21) & 0x14 == 0x14:   # unconditional blr / bctr
        return False
    if op == 3 or (op == 31 and (w >> 1) & 0x3FF == 4):           # twi / tw
        return False
    return True


def signatures(folder):
    """{(words, mask): [names, libraries, secondary entry?]} from every .lib in `folder`, cached (the
    cache is named by every library's name, size and time, so a changed XDK is read again)."""
    import hashlib
    import pickle
    from codebreaker.base import settings
    libs = sorted(f for f in os.listdir(folder) if f.lower().endswith(".lib"))
    stamp = hashlib.sha1(repr([(f, os.path.getsize(os.path.join(folder, f)), int(os.path.getmtime(os.path.join(folder, f))))
                               for f in libs]).encode()).hexdigest()[:16]
    cache = settings.state("xdk", "signatures-%s.pickle" % stamp)
    try:
        with open(cache, "rb") as f:
            return pickle.load(f)
    except (OSError, EOFError, pickle.UnpicklingError):
        pass
    sigs = _read_signatures(folder)
    os.makedirs(os.path.dirname(cache), exist_ok=True)
    with open(cache + ".part", "wb") as f:
        pickle.dump(sigs, f, protocol=pickle.HIGHEST_PROTOCOL)
    os.replace(cache + ".part", cache)
    return sigs


def _read_signatures(folder):
    sigs = {}
    for fname in sorted(os.listdir(folder)):
        if not fname.lower().endswith(".lib"):
            continue
        with open(os.path.join(folder, fname), "rb") as f:
            raw = f.read()
        for _name, body in archive_members(raw):
            if len(body) < 20:
                continue
            obj = Coff(body)
            if not obj.ok:
                continue
            for name, code, mask, offset in functions_in(obj):
                code, mask = _trim(code, mask)
                if len(mask) < 4:
                    continue
                key = (struct.unpack(">%dI" % len(mask), code), tuple(mask))
                entry = sigs.setdefault(key, [set(), set(), False])
                entry[0].add(name)
                entry[1].add(fname)
                entry[2] |= offset != 0
    return sigs


def match(image_path, folder=None):
    """{address: {"names": [...], "libs": [...], "size": bytes}} for the program's library functions."""
    from codebreaker.formats import powerpc_image, xex
    path = xex.basefile(image_path) if xex.is_xex(image_path) else image_path
    img = powerpc_image.read(path)
    text = next((s for s in img.sections if s[0].strip("\0") == ".text"), None)
    if text is None:
        raise ValueError("%s: no .text section" % image_path)
    t0, size = text[1], text[2]
    tbytes = img.bytes_at(t0, size)
    words = struct.unpack(">%dI" % (len(tbytes) // 4), tbytes[:len(tbytes) // 4 * 4])
    index = defaultdict(list)
    for i in range(len(words) - 1):
        index[(words[i], words[i + 1])].append(i)
    starts = {(a - t0) // 4 for a in img.extents}
    found = defaultdict(lambda: {"names": set(), "libs": set(), "size": 0})
    for (sig, mask), (names, libs, secondary) in signatures(folder or lib_folder()).items():
        if sum(1 for m in mask if m == 0xFFFFFFFF) < 3:
            continue                                               # too generic to place
        # anchored on its rarest pair of unmasked words in this program (a prologue's pair is everywhere);
        # a pair the program does not have at all rules the signature out at once
        anchor, fewest = None, None
        for i in range(len(mask) - 1):
            if mask[i] == mask[i + 1] == 0xFFFFFFFF:
                n = len(index.get((sig[i], sig[i + 1]), ()))
                if fewest is None or n < fewest:
                    anchor, fewest = i, n
                    if n == 0:
                        break
        if anchor is None or not fewest:
            continue
        hits = []
        for p in index.get((sig[anchor], sig[anchor + 1]), ()):
            s = p - anchor
            if s < 0 or s + len(sig) > len(words):
                continue
            if all((words[s + i] & mask[i]) == (sig[i] & mask[i]) for i in range(len(sig))):
                if not secondary and s not in starts and s > 0 and _falls_through(words[s - 1]):
                    continue                                       # the tail of another function
                hits.append(s)
        if len(hits) == 1:
            f = found[t0 + hits[0] * 4]
            f["names"] |= names
            f["libs"] |= libs
            f["size"] = max(f["size"], len(sig) * 4)
    return {a: {"names": sorted(v["names"]), "libs": sorted(v["libs"]), "size": v["size"]} for a, v in sorted(found.items())}


def main(argv):
    if not argv or argv[0].startswith("-"):
        print(__doc__.split("\n\n")[0] + "\n\n" + __doc__.split("\n\n")[1])
        return 2
    out = argv[argv.index("-o") + 1] if "-o" in argv else None
    rest = [a for i, a in enumerate(argv) if a != "-o" and (i == 0 or argv[i - 1] != "-o")]
    folder = rest[1] if len(rest) > 1 else lib_folder()
    if not os.path.isdir(folder):
        print("no XDK library folder: %s (setting xdk_libs)" % folder)
        return 1
    found = match(rest[0], folder)
    text_size = sum(v["size"] for v in found.values())
    per_lib = defaultdict(int)
    for v in found.values():
        per_lib[v["libs"][0]] += 1
    print("%s: %d library functions named (%s bytes)" % (os.path.basename(rest[0]), len(found), format(text_size, ",")))
    for lib, n in sorted(per_lib.items(), key=lambda x: -x[1])[:15]:
        print("  %6d  %s" % (n, lib))
    if out:
        with open(out, "w", encoding="utf-8") as f:
            json.dump({"%08X" % a: v for a, v in found.items()}, f, indent=1)
        print("  -> %s" % out)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
