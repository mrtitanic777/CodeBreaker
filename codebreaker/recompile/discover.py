"""discover.py -- every function of an Xbox 360 image, for the recompiler.

    from codebreaker.recompile import discover
    funcs, sources, img = discover.discover(path, extra=())
    funcs       {address: size, 0 when unknown}
    sources     how many came from each place: pdata, entry, calls, pointers, constants, seed
    img         the image (formats/powerpc_image.Image)

    python -m codebreaker.recompile.discover <image or .xex> [--list]

A recompiled program calls a function only if it was found: a missed one is a call that does
nothing at run time, which is how a recompiled title crashes. So more is found here than the
decompiler needs (formats/powerpc_image.py finds the .pdata functions, the entry point and every
`bl` target):

  * pointers -- methods reached only through a pointer (virtual tables, function tables): every
    big-endian word of the data sections that is a four-byte-aligned code address, not inside a
    known function, where a function's first instruction plausibly stands;
  * constants -- the same test for addresses the code builds (a `lis` and then an `addi` or
    `ori` on that register): the CRT hands its own leaf functions, which have no .pdata, to
    the run time that way (FlsAlloc's stand-in, the callbacks it registers);
  * seeds -- addresses the caller already knows are called indirectly (from a run), whose own
    callees are then followed like any other's.
"""
import bisect
import struct
import sys

from codebreaker.recompile import disassemble

WORD = 4
_PROLOGUE = {"mflr", "stwu", "stw", "std", "mr", "li", "lis", "or", "addi", "stmw", "b", "clrlwi",
             "rlwinm", "cmpwi", "cmplwi", "lwz", "add"}
_XL_ENDERS = frozenset((16, 528, 18))                     # bclr, bcctr, rfid


def _image(path):
    from codebreaker.formats import powerpc_image, xex
    return powerpc_image.read(xex.basefile(path) if xex.is_xex(path) else path)


def _plausible_start(code, lo, a):
    """A function's first instruction plausibly stands at `a`: it is a prologue-like
    instruction, or any instruction (not a data word) right after the end of a function
    (blr, bctr, a plain branch, padding)."""
    first = next(iter(disassemble.listing(code, lo, start=a - lo, count=1)), None)
    if first is None or first[2] == ".long":
        return False
    if first[2] in _PROLOGUE:
        return True
    if a - lo >= WORD:
        w = struct.unpack_from(">I", code, a - lo - WORD)[0]
        return w in (0, 0x4E800020, 0x4E800420) or (w >> 26 == 18 and not w & 3)
    return False


def from_pointers(img, known):
    """Function starts found as pointers in the data sections (see the module's notes)."""
    lo, hi = img.code
    ext = sorted(known.items())
    starts = [s for s, _n in ext]

    def interior(a):
        i = bisect.bisect_right(starts, a) - 1
        return i >= 0 and ext[i][0] < a < ext[i][0] + ext[i][1]
    code = img.bytes_at(lo, hi - lo)
    out = set()
    for name, va, size, chars in img.sections:
        if chars & 0x20000000 or name == ".text":
            continue
        data = img.bytes_at(va, size)
        for o in range(0, len(data) - 3, 4):
            a = struct.unpack_from(">I", data, o)[0]
            if a & 3 or not lo <= a < hi or a in known or interior(a):
                continue
            if _plausible_start(code, lo, a):
                out.add(a)
    return out


def from_constants(img, known):
    """Function starts found as addresses built in the code: `lis rD, hi` and then
    `addi rX, rD, lo` / `ori rX, rD, lo` (raw opcode bits; see the module's notes)."""
    lo, hi = img.code
    ext = sorted(known.items())
    starts = [s for s, _n in ext]

    def interior(a):
        i = bisect.bisect_right(starts, a) - 1
        return i >= 0 and ext[i][0] < a < ext[i][0] + ext[i][1]
    code = img.bytes_at(lo, hi - lo)
    high, out = {}, set()                      # register -> the value its last lis gave it
    for o in range(0, len(code) - 3, 4):
        w = struct.unpack_from(">I", code, o)[0]
        op, rd, ra, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
        if op == 15 and ra == 0:               # lis rD, imm
            high[rd] = (imm << 16) & 0xFFFFFFFF
            continue
        if op == 14 and ra in high:            # addi rD, rA, simm
            a = (high[ra] + (imm - 0x10000 if imm & 0x8000 else imm)) & 0xFFFFFFFF
        elif op == 24 and rd in high:          # ori rA, rS, uimm (rS is bits 21-25)
            a = high[rd] | imm
        else:
            continue
        if a & 3 or not lo <= a < hi or a in known or interior(a) or a in out:
            continue
        if _plausible_start(code, lo, a):
            out.add(a)
    return out


def discover(path, pointer_scan=True, extra=None):
    img = _image(path)
    funcs, sources = {}, {"pdata": 0, "entry": 0, "calls": 0, "pointers": 0, "constants": 0,
                          "seed": 0}
    for a in extra or ():
        if a not in funcs:
            funcs[a] = 0
            sources["seed"] += 1
    for a, (size, _prolog, _handler) in img.extents.items():
        funcs[a] = size
    sources["pdata"] = len(img.extents)
    if img.entry and img.entry not in funcs:
        funcs[img.entry] = 0
        sources["entry"] = 1
    if pointer_scan:
        for a in sorted(from_pointers(img, {a: s for a, s in funcs.items() if s})):      # (in address order)
            if a not in funcs:
                funcs[a] = 0
                sources["pointers"] += 1
        for a in sorted(from_constants(img, {a: s for a, s in funcs.items() if s})):     # (in address order)
            if a not in funcs:
                funcs[a] = 0
                sources["constants"] += 1
    lo, hi = img.code
    code = img.bytes_at(lo, hi - lo)
    # follow every bl from every function to a fixed point (raw opcode bits: a bl is opcode 18 with LK)
    work, seen = list(funcs), set(funcs)
    while work:
        va = work.pop()
        if not lo <= va < hi:
            continue
        size = funcs.get(va) or 0
        end = min(va + size, hi) if size else min(hi, va + 0x4000)
        off = va - lo
        while lo + off + WORD <= end:
            w = struct.unpack_from(">I", code, off)[0]
            op = w >> 26
            if op == 18:
                li = w & 0x03FFFFFC
                li -= 0x04000000 if li & 0x02000000 else 0
                if not w & 2:
                    t = lo + off + li
                    if w & 1:
                        if t not in seen and lo <= t < hi and t not in img.imports:
                            seen.add(t)
                            funcs.setdefault(t, 0)
                            work.append(t)
                            sources["calls"] += 1
                    elif not size:                     # a plain branch ends a run with no record
                        break
            elif not size and op == 19 and (w >> 1) & 0x3FF in _XL_ENDERS:
                break
            off += WORD
    return funcs, sources, img


def main(argv):
    if not argv:
        print(__doc__.split("\n\n")[0])
        return 2
    funcs, sources, _img = discover(argv[0])
    print("%d functions: %s" % (len(funcs), ", ".join("%s %d" % kv for kv in sources.items())))
    if "--list" in argv:
        for a in sorted(funcs):
            print("  %08X  %d" % (a, funcs[a]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
