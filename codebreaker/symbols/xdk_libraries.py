"""xdk_libraries.py -- naming the XDK library functions an Xbox 360 program was statically linked with.

    python -m codebreaker xdk-match <program.xex or image> [<XDK lib folder>] [-o names.json]
    from codebreaker.symbols import xdk_libraries
    found = xdk_libraries.match(image_path, lib_folder)     # {address: {"names", "libs", "size"}}

Every COFF archive (.lib) in the XDK's library folder is read (symbols/libraries), each function's
machine code taken from its object with the bits the linker patches (the fields its relocations
name) masked out, and the program's code searched for the same words. A program linked against the same XDK and
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

from codebreaker.symbols import libraries

DEFAULT_LIBS = r"C:\Program Files (x86)\Microsoft Xbox 360 SDK\lib\xbox"


def lib_folder():
    from codebreaker.base import settings
    return settings.get("xdk_libs") or DEFAULT_LIBS


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


REV = 2                                 # how the libraries are read (a mask: the bits left out): part of the cache's name


def signatures(folder):
    """{(words, bits left out of each): [names, libraries, secondary entry?]} from every .lib in
    `folder`, cached (the cache is named by every library's name, size and time, so a changed XDK is
    read again)."""
    import hashlib
    import pickle
    from codebreaker.base import settings
    libs = sorted(f for f in os.listdir(folder) if f.lower().endswith(".lib"))
    stamp = hashlib.sha1(repr([(f, os.path.getsize(os.path.join(folder, f)), int(os.path.getmtime(os.path.join(folder, f))))
                               for f in libs]).encode()).hexdigest()[:16]
    cache = settings.state("xdk", "signatures-%d-%s.pickle" % (REV, stamp))
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
    for old in os.listdir(os.path.dirname(cache)):        # (those of another XDK or REV: never read again)
        if old.startswith("signatures-") and old != os.path.basename(cache):
            try:
                os.remove(os.path.join(os.path.dirname(cache), old))
            except OSError:
                pass
    return sigs


def _read_signatures(folder):
    sigs = {}
    for fname in sorted(os.listdir(folder)):
        if not fname.lower().endswith(".lib"):
            continue
        kind, found = libraries.functions(os.path.join(folder, fname))
        if kind != "powerpc":
            continue
        for name, code, ignore, offset in found:
            code, ignore = _trim(code, ignore)
            if len(ignore) < 4:
                continue
            key = (struct.unpack(">%dI" % len(ignore), code), tuple(ignore))
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
    index = libraries.Index(words)
    starts = {(a - t0) // 4 for a in img.extents}
    found = defaultdict(lambda: {"names": set(), "libs": set(), "size": 0})
    for (sig, ignore), (names, libs, secondary) in signatures(folder or lib_folder()).items():
        if sum(1 for m in ignore if not m) < 3:
            continue                                               # too generic to place
        hits = [s for s in index.find(sig, ignore)                 # (not the tail of another function)
                if secondary or s in starts or s == 0 or not _falls_through(words[s - 1])]
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
