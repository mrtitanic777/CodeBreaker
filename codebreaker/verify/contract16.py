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

_VARIADIC = {"USER_420", "wsprintf", "USER_421", "wvsprintf"}   # (USER.420 wsprintf: its own count)
_MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
_MD.detail = True                                       # (what an instruction reads and writes)


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
    # (what the prologue pushes is saved registers, not arguments; and a push a jump lands on
    # starts a new count -- as describe counts the bytes pushed for a call through a pointer)
    targets = {i.operands[0].imm & 0xFFFF for i in ins
               if i.group(capstone.CS_GRP_JUMP) and i.operands and i.operands[0].type == capstone.x86.X86_OP_IMM}
    prologue = lo
    for j, i in enumerate(ins[:16]):
        nxt_ = ins[j + 1] if j + 1 < len(ins) else None
        if (i.mnemonic in ("nop", "enter") or i.op_str in ("ax, ds", "bp, sp", "ds, ax")
                # (and OpenWatcom's room for locals, `push ax` a word at a time -- counted as an
                # argument, sub_3_7E58's allocator call "pushed six bytes" for its two)
                or (i.mnemonic == "push" and i.op_str in ("ax", "cx", "dx") and nxt_ is not None
                    and not nxt_.group(capstone.CS_GRP_CALL))
                or (i.mnemonic in ("push", "inc") and i.op_str in ("bp", "ds", "si", "di"))
                or (i.mnemonic == "pop" and i.op_str == "ax")
                or (i.mnemonic == "mov" and i.op_str.startswith("ax, 0x"))
                or (i.mnemonic == "sub" and i.op_str.startswith("sp,"))):
            prologue = i.address + i.size
        else:
            break
    targets.add(prologue)
    for k, i in enumerate(ins):
        # (a target of 0 is printed `0`, not `0x0`: every call to a segment's first function --
        # SUWIN's helpers at 7:0000 and 9:0000 -- went unchecked, and a C caller passing one word
        # of a far pointer's two was never seen)
        mnemonic = i.mnemonic.replace("notrack ", "")      # (OpenWatcom's `3E E8`, a call with a DS prefix:
        if mnemonic == "call" and re.fullmatch(r"0x[0-9a-f]+|\d+", i.op_str):   # Capstone's "notrack call")
            callee = name_of(("near", seg, int(i.op_str, 0) & 0xFFFF))
        elif mnemonic == "lcall" and re.fullmatch(r"(0x[0-9a-f]+|\d+), (0x[0-9a-f]+|\d+)", i.op_str):
            s, o = (int(x, 0) for x in i.op_str.split(", "))
            callee = name_of(("far", (s << 16) | o))
        else:
            continue                                         # (through a pointer: the callee is not known here)
        nxt = ins[k + 1] if k + 1 < len(ins) else None
        pushed = 0                                         # (the argument bytes pushed just before it)
        before = ins[:k]
        # (`push cs / call near`: a far call to a function of the same segment -- CS is the return
        # address's half, no argument: counted as one, every such call of the build's "pushed two
        # more" than the original's)
        if before and before[-1].mnemonic == "push" and before[-1].op_str == "cs":
            before = before[:-1]
        exact = True                                       # (False: counted only from a join on)
        for p in reversed(before):
            if p.address < prologue:
                break
            if p.mnemonic == "push":
                pushed += 4 if p.operands and p.operands[0].size == 4 else 2
                if p.address in targets:
                    exact = p.address == prologue          # (a jump lands here: another path's
                    break                                  # pushes may come before -- MSC shares
                continue                                   # a call's tail, `push ds / push 1150h /
            if p.address in targets:                       # jmp` into the middle of its pushes)
                exact = p.address == prologue
                break
            if p.group(capstone.CS_GRP_CALL) or p.group(capstone.CS_GRP_JUMP) or                     p.mnemonic in ("ret", "retf", "pop", "leave", "enter", "add", "sub") and                     (p.mnemonic not in ("add", "sub") or p.op_str.startswith("sp")):
                break
            try:
                _r, w = p.regs_access()
            except capstone.CsError:
                break
            if capstone.x86.X86_REG_SP in w:
                break
        off = 0
        if nxt is not None and nxt.mnemonic == "add" and nxt.op_str.startswith("sp, "):
            amount = nxt.op_str.split(", ")[1]
            off = int(amount, 0) if re.fullmatch(r"-?(0x[0-9a-f]+|\d+)", amount) else "?" + amount
        else:
            # (or taken off by popping them into a register no one reads: `pop bx`, `pop cx` --
            # the original's way after a one-word cdecl call; read as nothing, the build's
            # `add sp, 2` after the same call "took off two bytes the original did not")
            for q in ins[k + 1:k + 1 + max(0, pushed // 2)]:
                if q.mnemonic == "pop" and q.op_str in ("bx", "cx") and off < pushed:
                    off += 2
                else:
                    break
        if callee:
            out.append((callee, off, pushed if exact else -pushed))   # (negative: at least that)
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
            # (the build's own map first: the describer names a function of the build by where it
            # is in the build -- SETUPX's allocator sub_24_0F1E was "sub_24_052E" there, no call to
            # it compared, and sub_3_7E58 calling it with no size was never seen. An import has no
            # place in the map: its name is the describer's)
            return ours_at.get(describe.where(key[1])) or xu.get(key[1])
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
            pa, pb = collections.defaultdict(set), collections.defaultdict(set)
            low = collections.defaultdict(int)            # (the original's at-least counts)
            for c, t, p in _calls(do.images.get(os_, b""), os_, oo, oe, orig_name):
                a[c].add(t)
                if p >= 0:
                    pa[c].add(p)
                else:
                    low[c] = max(low[c], -p)
            for c, t, p in _calls(img, s, o, hi, ours_name):
                b[c].add(t)
                if p >= 0:                                # (ours counted from a join: not compared)
                    pb[c].add(p)
            for c in sorted(set(a) & set(b)):
                if a[c] != b[c]:
                    out[n] = "after calling %s the original takes off %s bytes, ours %s" % (
                        c, sorted(a[c]), sorted(b[c]))
                    break
                # (and fewer argument bytes pushed than the original pushes: SUWIN's sub_8_0634
                # gave sub_8_0A6A one near pointer for its two far ones -- the second, a 260-byte
                # buffer the caller never reads, dropped -- and a path was written over its frame)
                # (or more: a call is the callee's, the same bytes from any caller -- unless the
                # callee takes a varying number, `...`, as wsprintf does)
                if pa[c] and pb[c] and (max(pb[c]) < min(pa[c]) or
                                        pa[c] != pb[c] and len(pa[c]) == 1 and len(pb[c]) == 1 and c not in _VARIADIC):
                    out[n] = "it pushes %s argument bytes for %s, the original %s" % (
                        sorted(pb[c]), c, sorted(pa[c]))
                    break
    return out
