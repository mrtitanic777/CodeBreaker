"""carve.py -- where the program's own functions begin and end.

    python -m codebreaker exact PROJECT carve [--write] [-v]

The program's own code is what lies before the library part (exact/library.py), less the kit's
objects linked first. In it a function begins

  - where the compiler's prologue stands (the toolchain profile's "carve": "prologues", byte
    patterns as hex regular expressions; one whose group 1 is a near call's displacement counts only
    when the call goes to the routine "prologue_call" names - C 3.00's __chkstk)
  - where a frame is set up (push bp / mov bp,sp) right after a return or a pad: a function compiled
    without the stack probe (-Gs), or by hand
  - where a near call of the code goes (the code disassembled from each start to the next)

and ends where the next begins (or the program's own code ends). A start the project knows better
can be added or taken away: project "starts_add" / "starts_drop" (image offsets).

    carve.functions(project) -> [(start, length)]
src/FUNCS (--write): one start a line, the length after it.
"""
import re
import struct

from codebreaker.exact import library


def _rel(img, k):
    return struct.unpack_from("<h", img, k)[0]


def region(prj, mp=None):
    """(lo, hi, [(a, b) ranges inside that are not the program's own: the kit's objects])"""
    mp = mp or library.locate(prj)
    hi = mp.start if mp.start is not None else prj.dgroup
    lo = prj.cfg.get("code_start", 0)
    lo = int(lo, 0) if isinstance(lo, str) else lo
    skip = [(p.code, p.end) for p in mp.coded + mp.lone if p.code < hi]
    skip += [(a, b) for a, b, nm in mp.objects]
    return lo, hi, sorted(skip)


def functions(prj, verbose=False):
    img = prj.image
    mp = library.locate(prj)
    lo, hi, skip = region(prj, mp)
    cv = prj.toolchain().p.get("carve", {})
    names = {}
    for p in mp.coded:
        for m in [p.m] + p.also:
            for n, (s, off) in m.pubs.items():
                if s == m.code[0][0]:
                    names.setdefault(n, p.code + off)
    for n, (t, a) in prj.symbols().items():
        if t == "C":
            names.setdefault(n, a)
    inside = lambda a: lo <= a < hi and not any(x <= a < y for x, y in skip)
    starts = set()
    why = {}
    target = names.get(cv.get("prologue_call")) if cv.get("prologue_call") else None
    for rx in cv.get("prologues", []):
        r = re.compile(bytes.fromhex(rx) if False else _hexrx(rx), re.S)
        for m in r.finditer(img, lo, hi):
            a = m.start()
            if not inside(a):
                continue
            if m.groups() and m.group(1) is not None and target is not None:
                k = m.start(1)
                if (k + 2 + _rel(img, k)) & 0xFFFF != target & 0xFFFF:
                    continue
            starts.add(a)
            why.setdefault(a, "prologue")
    if cv.get("frames", True):
        for m in re.finditer(rb"\x55\x8b\xec", img[:hi]):
            a = m.start()
            if a < lo or not inside(a):
                continue
            if a == lo or img[a - 1] in (0xC3, 0xCB, 0x00, 0x90) or a >= 3 and img[a - 3] in (0xC2, 0xCA):
                starts.add(a)
                why.setdefault(a, "frame after a return")
    for a in prj.cfg.get("starts_add", []):
        starts.add(int(a, 0) if isinstance(a, str) else a)
    if cv.get("calls", True):
        _calls(img, starts, inside, prj, why, hi)
    for a in prj.cfg.get("starts_drop", []):
        starts.discard(int(a, 0) if isinstance(a, str) else a)
    st = sorted(starts)
    ends = sorted(set(st[1:]) | {hi} | {x for x, y in skip if x > lo})
    out = []
    for a in st:
        e = min(x for x in ends if x > a)
        z = e
        while z > a + 1 and img[z - 1] == 0:
            z -= 1
        if z < e and (img[z - 1] in (0xC3, 0xCB) or z - a >= 3 and img[z - 3] in (0xC2, 0xCA)):
            e = z                                           # (zeros after the return: a pad to the next module)
        out.append((a, e - a, why.get(a, "")))
    return out


def _hexrx(rx):
    """a pattern of hex bytes ("55 8b ec (?:33c0|b8....) e8 (....)") as a bytes regex: two hex digits a byte,
    '..' any byte, the regex's own ( ) | ? * + left as they are"""
    out = b""
    s = rx.replace(" ", "")
    k = 0
    while k < len(s):
        c = s[k]
        if s[k:k + 2] == "..":
            out += b"."
            k += 2
        elif c in "0123456789abcdefABCDEF" and k + 1 < len(s) and s[k + 1] in "0123456789abcdefABCDEF":
            out += re.escape(bytes([int(s[k:k + 2], 16)]))
            k += 2
        else:
            out += c.encode()
            k += 1
    return out


def _calls(img, starts, inside, prj, why, hi):
    """near calls' targets, from the code disassembled from each known start (until a new one is not found)"""
    from codebreaker.exact import x86
    emu = prj.toolchain().p.get("carve", {}).get("emulator_ints", True)
    done = set()
    while True:
        todo = sorted(starts - done)
        if not todo:
            return
        allst = sorted(starts)
        for a in todo:
            done.add(a)
            nxt = min([x for x in allst if x > a] + [hi])
            for i in x86.body(img, a, nxt, emu, prj.code_base(a)):
                if i.mnemonic == "call" and i.size == 3 and i.bytes[0] == 0xE8:
                    base = prj.code_base(i.address)
                    t = base + ((i.address + 3 + _rel(img, i.address + 1) - base) & 0xFFFF)
                    if inside(t) and t not in starts:
                        starts.add(t)
                        why.setdefault(t, "called from %05x" % i.address)


def main(prj, argv):
    fs = functions(prj, verbose="-v" in argv)
    for a, ln, why in fs:
        if "-v" in argv:
            print("%05x %5d  %s" % (a, ln, why))
    print("%d functions of the program's own, %d bytes" % (len(fs), sum(ln for a, ln, w in fs)))
    if "--write" in argv:
        with open(prj.path("FUNCS"), "w", newline="\n") as f:
            f.write("# where the program's own functions begin, and their lengths (codebreaker exact carve)\n")
            for a, ln, why in fs:
                f.write("%05x %d  %s\n" % (a, ln, why))
    return 0
