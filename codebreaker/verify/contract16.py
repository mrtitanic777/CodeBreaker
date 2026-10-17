"""The calls of each function rebuilt from C beside the original's: per callee, the bytes the
caller takes off the stack after the call. The original's callers say how each callee is to be
called -- one that removes its own arguments is called without the caller taking them off --
and a rebuilt function that takes off what the callee already removed (or leaves what it
does not) leaves the stack wrong for everything after it.

The isolated check (isolated16.py) answers a call as the description says the callee does, so
it agrees with any belief the description has; this reads both modules' instructions as they
are and so sees a wrong belief.

    mismatches(original, folder, image, names) -> {name: why}"""
import collections
import os
import re
import shutil
import tempfile

import capstone

from codebreaker.decompile import describe

_MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)


def _described(path, where):
    d = describe.describe16(path, where)
    names = {}
    with open(os.path.join(where, "description.txt"), encoding="utf-8") as f:
        for line in f:
            p = line.rstrip("\n").split("\t")
            if p[0] in ("X", "F") and len(p) > 2:
                names[int(p[1], 16)] = p[2]
    return d, names


def _calls(img, seg, lo, hi, name_of):
    """[(callee, bytes taken off after it)] of the code in [lo, hi)."""
    ins = list(_MD.disasm(bytes(img[lo:hi]), lo))
    out = []
    for k, i in enumerate(ins):
        if i.mnemonic == "call" and re.fullmatch(r"0x[0-9a-f]+", i.op_str):
            callee = name_of(("near", seg, int(i.op_str, 16) & 0xFFFF))
        elif i.mnemonic == "lcall" and re.fullmatch(r"0x[0-9a-f]+, 0x[0-9a-f]+", i.op_str):
            s, o = (int(x, 16) for x in i.op_str.split(", "))
            callee = name_of(("far", (s << 16) | o))
        else:
            continue                                         # (through a pointer: the callee is not known here)
        nxt = ins[k + 1] if k + 1 < len(ins) else None
        off = 0
        if nxt is not None and nxt.mnemonic == "add" and nxt.op_str.startswith("sp, "):
            amount = nxt.op_str.split(", ")[1]
            off = int(amount, 0) if re.fullmatch(r"-?(0x[0-9a-f]+|\d+)", amount) else "?" + amount
        if callee:
            out.append((callee, off))
    return out


def mismatches(original, folder, image, names):
    """{name: why} for each named function whose calls take off different bytes than the
    original's calls to the same callee (map file and image of the build in `folder`)."""
    tmp = tempfile.mkdtemp(prefix="contract16_")
    try:
        do, xo = _described(original, os.path.join(tmp, "o"))
        du, xu = _described(image, os.path.join(tmp, "u"))
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    starts = collections.defaultdict(list)
    for va in do.names:
        s, o = describe.where(va)
        starts[s].append(o)
    for s in starts:
        starts[s].sort()
    orig_fn, orig_at = {}, {}
    for va, n in do.names.items():
        s, o = describe.where(va)
        later = [x for x in starts[s] if x > o]
        orig_fn[n] = (s, o, later[0] if later else len(do.images.get(s, b"")))
        orig_at[(s, o)] = n

    def orig_name(key):
        if key[0] == "far":
            return xo.get(key[1]) or do.names.get(key[1])
        return orig_at.get(key[1:])

    try:
        text = open(os.path.join(folder, "m.map"), errors="replace").read()
    except OSError:
        return {}
    syms = collections.defaultdict(list)
    for s, o, n in re.findall(r"(?m)^([0-9a-f]{4}):([0-9a-f]{4})\S*\s+(\S+)\s*$", text):
        syms[int(s, 16)].append((int(o, 16), re.sub(r"^_", "", re.sub(r"@\d+$", "", n))))
    ours_at = {}
    for s in syms:
        syms[s].sort()
        for o, n in syms[s]:
            ours_at[(s, o)] = n

    def ours_name(key):
        if key[0] == "far":
            if key[1] in xu:
                return xu[key[1]]
            return ours_at.get(describe.where(key[1]))
        return ours_at.get(key[1:])

    wanted, out = set(names), {}
    for s, lst in syms.items():
        img = du.images.get(s, b"")
        for k, (o, n) in enumerate(lst):
            if n not in wanted or n not in orig_fn:
                continue
            hi = lst[k + 1][0] if k + 1 < len(lst) else len(img)
            os_, oo, oe = orig_fn[n]
            a, b = collections.defaultdict(set), collections.defaultdict(set)
            for c, t in _calls(do.images.get(os_, b""), os_, oo, oe, orig_name):
                a[c].add(t)
            for c, t in _calls(img, s, o, hi, ours_name):
                b[c].add(t)
            for c in sorted(set(a) & set(b)):
                if a[c] != b[c]:
                    out[n] = "after calling %s the original takes off %s bytes, ours %s" % (
                        c, sorted(a[c]), sorted(b[c]))
                    break
    return out
