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
# (functions that take their arguments in registers -- DOS3Call, NetBIOSCall: AH, DS:DX, ES:DI, CX
# loaded just before the call, and the carry flag back. C sets none of them: a C caller of one is
# not the original's. VER's sub_3_0BD6 renames a file through DOS3Call; its C called it with
# nothing loaded, and the isolated check, answering DOS3Call alike on both sides, saw no difference)
_IN_REGISTERS = {"KERNEL_102", "DOS3CALL", "KERNEL_103", "NETBIOSCALL"}
_MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
_MD.detail = True                                       # (what an instruction reads and writes)


def _described(path, where):
    """(description, {address: name}, {address: bytes it removes -- None: not known})"""
    d = describe.describe16(path, where)
    names, pops = {}, {}
    with open(os.path.join(where, "description.txt"), encoding="utf-8") as f:
        for line in f:
            p = line.rstrip("\n").split("\t")
            if p[0] in ("X", "F") and len(p) > 2:
                names[int(p[1], 16)] = p[2]
            if p[0] in ("X", "F") and len(p) > 3 and re.fullmatch(r"-?\d+", p[3]):
                pops[int(p[1], 16)] = int(p[3]) if int(p[3]) >= 0 else None
    return d, names, pops


def _removes(d, pops):
    """removes(key) for _calls: what the callee at a call's target removes, as described -- or,
    one the describer did not find (the build's functions are found as the original's are, and
    not all of them: USER's sub_1_2293 rebuilt at 1:5810 was not, and every call to it "took
    none"), as its own nearest return says"""
    def removes(key):
        va = key[1] if key[0] == "far" else describe.address(key[1], key[2])
        if va in pops:
            return pops[va]
        s, o = describe.where(va)
        if s not in d.images:
            return None
        pop = describe.returns(d.images[s], o, want="far" if key[0] == "far" else None)[1]
        return pop if pop >= 0 else None
    return removes


def _calls(img, seg, lo, hi, name_of, removes=lambda key: None):
    """[(callee, bytes taken off after it, argument bytes pushed for it -- negative: at least that,
    None: not known -- whether the frame's LEAVE follows)] of the code in [lo, hi). removes(key):
    the bytes a callee removes itself (its `retf n`), or None."""
    ins = list(_MD.disasm(bytes(img[lo:hi]), lo))
    out = []
    # (what the prologue pushes is saved registers, not arguments; and a push a jump lands on
    # starts a new count -- as describe counts the bytes pushed for a call through a pointer)
    targets = {i.operands[0].imm & 0xFFFF for i in ins
               if i.group(capstone.CS_GRP_JUMP) and i.operands and i.operands[0].type == capstone.x86.X86_OP_IMM}
    prologue = lo
    loaded = False                                         # (DS set from AX: the Windows prolog done)
    for j, i in enumerate(ins[:16]):
        nxt_ = ins[j + 1] if j + 1 < len(ins) else None
        # (`mov ax, ds` and `mov ax, <data segment>` are the prolog's only before DS is loaded from
        # AX: after it, GDI's sub_1_59D2 rebuilt -- `mov ds, ax / mov ax, ds / push ax / push 594h`,
        # a far pointer to its data -- "pushed two bytes" for the four it pushes)
        if (i.mnemonic in ("nop", "enter") or i.op_str in ("bp, sp", "ds, ax")
                or (not loaded and i.op_str == "ax, ds")
                # (and OpenWatcom's room for locals, `push ax` a word at a time -- counted as an
                # argument, sub_3_7E58's allocator call "pushed six bytes" for its two)
                or (i.mnemonic == "push" and i.op_str in ("ax", "cx", "dx") and nxt_ is not None
                    and not nxt_.group(capstone.CS_GRP_CALL))
                or (i.mnemonic in ("push", "inc") and i.op_str in ("bp", "ds", "si", "di"))
                or (i.mnemonic == "pop" and i.op_str == "ax")
                or (not loaded and i.mnemonic == "mov" and i.op_str.startswith("ax, 0x"))
                or (i.mnemonic == "sub" and i.op_str.startswith("sp,"))):
            prologue = i.address + i.size
            loaded = loaded or (i.mnemonic == "mov" and i.op_str == "ds, ax")
        else:
            break
    targets.add(prologue)
    stack_at = _stack(ins, seg, prologue, removes)
    for k, i in enumerate(ins):
        # (a target of 0 is printed `0`, not `0x0`: every call to a segment's first function --
        # SUWIN's helpers at 7:0000 and 9:0000 -- went unchecked, and a C caller passing one word
        # of a far pointer's two was never seen)
        # (OpenWatcom's `3E E8`, a call with a DS prefix: Capstone's "notrack call")
        key = _callee_key(i, seg)
        if key is None:
            continue                                         # (through a pointer: the callee is not known here)
        callee = name_of(key)
        after = k + 1
        while after < len(ins) and ins[after].mnemonic == "nop":
            after += 1                                     # (OpenWatcom's `nop` after a near call)
        nxt = ins[after] if after < len(ins) else None
        total, exact = stack_at.get(k, (0, False))         # (pushed and not yet taken off; False:
                                                           #  counted only from a join on)
        off = 0
        rest = after                                       # (the first instruction past the cleanup)
        if nxt is not None and nxt.mnemonic == "add" and nxt.op_str.startswith("sp, "):
            amount = nxt.op_str.split(", ")[1]
            off = int(amount, 0) if re.fullmatch(r"-?(0x[0-9a-f]+|\d+)", amount) else "?" + amount
            rest = after + 1
        else:
            # (or taken off by popping them into a register no one reads: `pop bx`, `pop cx` --
            # the original's way after a one-word cdecl call; read as nothing, the build's
            # `add sp, 2` after the same call "took off two bytes the original did not")
            for q in ins[after:after + max(0, total // 2)]:
                if q.mnemonic == "pop" and q.op_str in ("bx", "cx") and off < total:
                    off += 2
                    rest += 1
                else:
                    break
        at_leave = False
        if total:
            # (and then the frame's LEAVE, nothing touching the stack before it: SP is set back from
            # BP whatever was taken off -- Microsoft C 8 itself leaves a call's arguments there when
            # the function ends next (SETUPX's 2:5A3C, `lcall _SUFORMATMESSAGEBOX / xor ax, ax /
            # leave`), OpenWatcom takes them off: the same. Told here; compared as the same.)
            for q in ins[rest:rest + 12]:
                if q.mnemonic == "leave" or (q.mnemonic == "mov" and q.op_str == "sp, bp") or \
                        (q.mnemonic == "lea" and q.op_str.startswith("sp, [bp")):   # (OpenWatcom's, before its pops)
                    at_leave = True
                    break
                if q.mnemonic in ("push", "pop", "pushf", "popf", "enter", "ret", "retf") or \
                        q.group(capstone.CS_GRP_CALL) or q.group(capstone.CS_GRP_JUMP):
                    break
                try:
                    _r, w = q.regs_access()
                except capstone.CsError:
                    break
                if capstone.x86.X86_REG_SP in w:
                    break
        if callee:
            # (the bytes that are this call's: what the callee removes itself, else what the caller
            # takes off after it, else -- the frame's LEAVE taking them -- all still pushed; a
            # callee that removes none and is followed by no cleanup says nothing of its own)
            took = removes(key) or 0
            args = took if took > 0 else off if isinstance(off, int) and off > 0 else total if at_leave else None
            if args is None:
                pushed = None
            elif args <= total:
                pushed = args
            else:
                # (negative: at least that -- and none counted from a join is no count at all: USER's
                # sub_6_1A72 jumps into its call to sub_1_38E4 from three places, "pushing none")
                pushed = total if exact else -total if total else None
            out.append((callee, off, pushed, at_leave))
    return out


def _callee_key(i, seg):
    """("near", seg, off) / ("far", address) of a direct call, or None (through a pointer)"""
    mnemonic = i.mnemonic.replace("notrack ", "")
    if mnemonic == "call" and re.fullmatch(r"0x[0-9a-f]+|\d+", i.op_str):
        return ("near", seg, int(i.op_str, 0) & 0xFFFF)
    if mnemonic == "lcall" and re.fullmatch(r"(0x[0-9a-f]+|\d+), (0x[0-9a-f]+|\d+)", i.op_str):
        s, o = (int(x, 0) for x in i.op_str.split(", "))
        return ("far", (s << 16) | o)
    return None


def _stack(ins, seg, prologue, removes):
    """{index of a call: (bytes pushed and not yet taken off, whether that is all of them)} -- the
    pushes since the prologue, less what was popped, taken off (`add sp, n`) or removed by a callee
    that removes its own (removes(key)). A call's pushes are not all its own: the bytes before
    them can be an enclosing call's -- `GlobalAlloc(0x2042, GlobalSize(h))` pushes the flags, then
    GlobalSize's handle -- and counted back only to the call before, USER's GETCLIPBOARDDATA
    "pushed four" for GlobalAlloc's six; counted back to a join, a call taking no arguments was
    given the next call's (USER's sub_9_0907 pushes sub_9_0930's far pointer before calling
    sub_1_8270, whose result is sub_9_0930's first argument). A join of paths (a jump's target)
    has what every jump to it had, if they agree; else (or one not seen: a jump back, a table's)
    the count starts over there, as no longer all of them."""
    out, saved = {}, {}
    pend, exact, live = [], False, False
    for k, i in enumerate(ins):
        a = i.address
        if a < prologue:
            continue
        if a == prologue:
            pend, exact, live = [], True, True
        elif a in saved or not live:
            seen = saved.pop(a, []) + ([(pend, exact)] if live else [])
            if seen and all(e for _p, e in seen) and len({sum(p) for p, _e in seen}) == 1:
                pend, exact = list(seen[0][0]), True
            else:
                pend, exact = [], False
            live = True
        m = i.mnemonic.replace("notrack ", "")
        if m == "push":
            pend.append(4 if i.operands and i.operands[0].size == 4 else 2)
            if i.op_str == "cs" and k + 1 < len(ins) and ins[k + 1].mnemonic == "call":
                pend[-1] = 0                                     # (`push cs / call near`: the return
            continue                                             #  address's half, no argument)
        if m in ("pushf", "pushfw"):
            pend.append(2)
            continue
        if m in ("pusha", "pushaw"):
            pend.append(16)
            continue
        if m in ("pop", "popf", "popfw", "popa", "popaw"):
            n = 16 if m.startswith("popa") else 2
            while n > 0 and pend:
                n -= pend.pop() or 2
            continue
        if m in ("add", "sub") and re.fullmatch(r"sp, (0x[0-9a-f]+|\d+)", i.op_str):
            n = int(i.op_str.split(", ")[1], 0)
            if m == "sub":
                pend.append(n)                                   # (room for a structure passed by value)
            else:
                while n > 0 and pend:
                    n -= pend.pop() or 2
            continue
        if i.group(capstone.CS_GRP_CALL):
            if pend and pend[-1] == 0:
                pend.pop()
            out[k] = (sum(pend), exact)
            key = _callee_key(i, seg)
            if key is None:
                nxt = ins[k + 1] if k + 1 < len(ins) else None
                if not (nxt is not None and nxt.mnemonic == "add" and nxt.op_str.startswith("sp")):
                    pend, exact = [], False                      # (through a pointer: it may remove
                continue                                         #  what was pushed, or not)
            n = removes(key) or 0
            while n > 0 and pend:
                n -= pend.pop() or 2
            continue
        if i.group(capstone.CS_GRP_JUMP):
            t = i.operands[0].imm & 0xFFFF if i.operands and i.operands[0].type == capstone.x86.X86_OP_IMM else None
            if t is not None and t > a:
                saved.setdefault(t, []).append((list(pend), exact))
            if m in ("jmp", "ljmp"):
                live = False
            continue
        if i.group(capstone.CS_GRP_RET) or m in ("ret", "retf", "iret", "int3"):
            live = False
            continue
        try:
            _r, w = i.regs_access()
        except capstone.CsError:
            pend, exact = [], False
            continue
        if capstone.x86.X86_REG_SP in w:
            pend, exact = [], False                              # (`mov sp, bp`, LEAVE: the frame's end)
    return out


def mismatches(original, folder, image, names):
    """{name: why} for each named function whose calls take off different bytes than the
    original's calls to the same callee (map file and image of the build in `folder`)."""
    tmp = tempfile.mkdtemp(prefix="contract16_")
    try:
        do, xo, po = _described(original, os.path.join(tmp, "o"))
        du, xu, pu = _described(image, os.path.join(tmp, "u"))
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
            for c, t, p, lv in _calls(do.images.get(os_, b""), os_, oo, oe, orig_name, _removes(do, po)):
                a[c].add("leave" if lv else t)
                if p is None:
                    continue
                if p >= 0:
                    pa[c].add(p)
                else:
                    low[c] = max(low[c], -p)
            for c, t, p, lv in _calls(img, s, o, hi, ours_name, _removes(du, pu)):
                b[c].add("leave" if lv else t)
                if p is not None and p >= 0:              # (ours counted from a join: not compared)
                    pb[c].add(p)
            for c in sorted(set(a) & set(b)):
                if c in _IN_REGISTERS:
                    out[n] = "it calls %s, which takes its arguments in registers" % c
                    break
                ta, tb = a[c] - {"leave"}, b[c] - {"leave"}   # (before a LEAVE: whatever was taken off)
                if ta and tb and ta != tb:
                    # (each set may hold "leave" beside the byte counts -- and a count read from a
                    # register, "?bx": sorted as text, or the report itself fails. It did, on every
                    # USER build after the LEAVE rule came in, and with it the whole check)
                    out[n] = "after calling %s the original takes off %s bytes, ours %s" % (
                        c, sorted(a[c], key=str), sorted(b[c], key=str))
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
