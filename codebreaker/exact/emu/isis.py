#!/usr/bin/env python3
"""isis.py - run an Intel ISIS-II program (8080 code) : enough of the 8080 and of ISIS-II for the
Intel development tools of 1978-82 - PL/M-86, LINK86, LOC86, LIB86, OH86.

  python isis.py [-d0 DIR] [-d1 DIR] ... PROGRAM command tail
  run(program, tail, drives) -> Result(console, status, steps)

The tools are data here: 8080 object files interpreted by this module, nothing of them runs on the
host. Drives :F0: .. :F9: are host directories (file names upper case, as ISIS has them).
ISIS calls (CALL 40h, C = function, DE -> parameter block): OPEN CLOSE DELETE READ WRITE SEEK LOAD
RENAME CONSOL EXIT ATTRIB RESCAN ERROR WHOCON SPATH. Monitor entries: CI CO MEMCK and the rest as
no-ops.
"""
import os, sys

class Result:
    def __init__(s, console, status, steps): s.console, s.status, s.steps = console, status, steps

class Exit(Exception): pass

PAR = [0] * 256
for _i in range(256): PAR[_i] = 1 if bin(_i).count("1") % 2 == 0 else 0

class Machine:
    def __init__(s, drives, tail, echo=False):
        s.m = bytearray(65536); s.r = [0, 0, 0, 0, 0, 0, 0, 0]       # B C D E H L - A
        s.sp = 0xF6C0; s.pc = 0; s.fz = s.fs = s.fp = s.fc = s.fa = 0
        s.drives = drives; s.files = {}; s.console = bytearray(); s.echo = echo
        s.cin = bytearray(b"PROG" + tail.encode("latin1") + b"\r\n"); s.cpos = 4; s.status = None     # RESCAN goes back to the program's name
        s.ops = [None] * 256; s._build()

    # ---- files
    def path(s, name):
        name = name.upper()
        if name.startswith(":") and name[3:4] == ":":
            dev = name[1:3]; rest = name[4:]
            if dev[0] == "F" and dev[1].isdigit(): return os.path.join(s.drives.get(int(dev[1]), s.drives[0]), rest), None
            return None, dev
        return os.path.join(s.drives[0], name), None

    def str_at(s, a):
        m = s.m
        while m[a] == 0x20: a += 1
        e = a
        while (0x30 <= m[e] <= 0x39) or (0x41 <= m[e] <= 0x5A) or (0x61 <= m[e] <= 0x7A) or m[e] in (0x3A, 0x2E): e += 1
        return bytes(m[a:e]).decode("latin1")

    def w(s, a): return s.m[a] | (s.m[(a + 1) & 0xFFFF] << 8)
    def ww(s, a, v): s.m[a] = v & 255; s.m[(a + 1) & 0xFFFF] = (v >> 8) & 255

    def load(s, path, bias=0):
        b = open(path, "rb").read(); p = 0; entry = 0
        while p + 3 <= len(b):
            t = b[p]; n = b[p + 1] | (b[p + 2] << 8); body = b[p + 3:p + 3 + n - 1]; p += 3 + n
            if t == 0x06:
                a = (body[1] | (body[2] << 8)) + bias; d = body[3:]; s.m[a:a + len(d)] = d
            elif t == 0x04: entry = (body[2] | (body[3] << 8)) + bias
            elif t == 0x0E: break
        return entry

    def isis(s):
        f = s.r[1]; pb = (s.r[2] << 8) | s.r[3]; w = s.w; m = s.m
        P = lambda k: w(pb + 2 * k)
        def st(k, v): s.ww(P(k), v)
        if f == 0:                                                # OPEN(aftn^, name, access, echo, status^)
            name = s.str_at(P(1)); acc = P(2); path, dev = s.path(name)
            if dev is not None:
                a = {"CO": 0, "CI": 1}.get(dev)
                if a is None: a = next(k for k in range(2, 99) if k not in s.files); s.files[a] = [None, 0, dev, bytearray(), False]
                s.ww(P(0), a); st(4, 0); return
            if acc == 1 and not os.path.exists(path): st(4, 13); return
            data = bytearray(open(path, "rb").read()) if acc != 2 and os.path.exists(path) else bytearray()
            a = next(k for k in range(2, 99) if k not in s.files); s.files[a] = [path, 0, None, data, acc != 1]
            s.ww(P(0), a); st(4, 0)
        elif f == 1:                                              # CLOSE(aftn, status^)
            a = P(0); fl = s.files.pop(a, None)
            if fl and fl[4] and fl[0]: open(fl[0], "wb").write(bytes(fl[3]))
            st(1, 0)
        elif f == 2:                                              # DELETE(name, status^)
            path, dev = s.path(s.str_at(P(0)))
            if path and os.path.exists(path): os.remove(path); st(1, 0)
            else: st(1, 13)
        elif f == 3:                                              # READ(aftn, buf, count, actual^, status^)
            a, buf, cnt = P(0), P(1), P(2)
            if a == 1:
                e = s.cin.find(b"\n", s.cpos); e = len(s.cin) if e < 0 else e + 1
                d = s.cin[s.cpos:min(e, s.cpos + cnt)]; s.cpos += len(d)
            else:
                fl = s.files.get(a)
                if fl is None: st(4, 2); return
                d = fl[3][fl[1]:fl[1] + cnt] if fl[2] is None else b""; fl[1] += len(d)
            m[buf:buf + len(d)] = d; s.ww(P(3), len(d)); st(4, 0)
        elif f == 4:                                              # WRITE(aftn, buf, count, status^)
            a, buf, cnt = P(0), P(1), P(2); d = bytes(m[buf:buf + cnt])
            if a == 0:
                s.console += d
                if s.echo: sys.stdout.write(d.decode("latin1")); sys.stdout.flush()
            else:
                fl = s.files.get(a)
                if fl is None: st(3, 2); return
                if fl[2] is None:
                    pos = fl[1]
                    if pos > len(fl[3]): fl[3] += bytes(pos - len(fl[3]))
                    fl[3][pos:pos + cnt] = d; fl[1] = pos + cnt; fl[4] = True
            st(3, 0)
        elif f == 5:                                              # SEEK(aftn, mode, block^, byte^, status^)
            a, mode = P(0), P(1); fl = s.files.get(a)
            if fl is None: st(4, 2); return
            blk, byt = w(P(2)), w(P(3)); off = blk * 128 + byt
            if mode == 0: s.ww(P(2), fl[1] // 128); s.ww(P(3), fl[1] % 128)
            elif mode == 1: fl[1] = max(0, fl[1] - off)
            elif mode == 2: fl[1] = off
            elif mode == 3: fl[1] = fl[1] + off
            elif mode == 4: fl[1] = len(fl[3])
            if fl[1] > len(fl[3]) and fl[4]: fl[3] += bytes(fl[1] - len(fl[3]))
            st(4, 0)
        elif f == 6:                                              # LOAD(name, bias, retsw, entry^, status^)
            path, dev = s.path(s.str_at(P(0)))
            if not path or not os.path.exists(path): st(4, 13); return
            bias, retsw, entp, stp = P(1), P(2), P(3), P(4)       # the parameter block may lie where the program loads: read it first
            s.ww(stp, 0); e = s.load(path, bias)
            if retsw != 0: s.pc = e; return True                  # retsw 1: control goes to the program loaded; 0: back to the caller
            s.ww(entp, e)
        elif f == 7:                                              # RENAME(old, new, status^)
            a, _ = s.path(s.str_at(P(0))); b, _ = s.path(s.str_at(P(1)))
            if a and os.path.exists(a): os.replace(a, b); st(2, 0)
            else: st(2, 13)
        elif f == 9:                                              # EXIT
            for a, fl in list(s.files.items()):
                if fl[4] and fl[0]: open(fl[0], "wb").write(bytes(fl[3]))
            raise Exit()
        elif f == 11: s.cpos = 0; st(1, 0)                        # RESCAN
        elif f == 12:                                             # ERROR(errnum, status^)
            s.console += ("ERROR %d USER PC %04X\r\n" % (P(0) & 255, s.w(s.sp))).encode(); st(1, 0)
        elif f == 14:                                             # SPATH(name, info^, status^)
            name = s.str_at(P(0)); path, dev = s.path(name); info = P(1)
            if dev is not None: m[info] = {"CO": 0x0D, "CI": 0x0C, "BB": 0x16, "LP": 0x15, "TO": 0x09, "TI": 0x08}.get(dev, 0x16); m[info + 1:info + 12] = bytes(11)
            else:
                base = os.path.basename(path); nm, _, ex = base.partition(".")
                m[info] = int(name[2]) if name.startswith(":F") else 0
                m[info + 1:info + 7] = nm.encode().ljust(6, b"\0")[:6]; m[info + 7:info + 10] = ex.encode().ljust(3, b"\0")[:3]
                m[info + 10] = 3; m[info + 11] = 0
            st(2, 0)
        elif f in (8, 10, 13): s.ww(P(P.__code__.co_argcount), 0) if False else s.ww(w(pb + 2 * {8: 2, 10: 3, 13: 1}[f]), 0)
        else: raise RuntimeError("ISIS call %d" % f)

    # ---- the 8080
    def _build(s):
        m = s.m; r = s.r; ops = s.ops
        def rd(i): return m[(r[4] << 8) | r[5]] if i == 6 else r[i]
        def wr(i, v):
            if i == 6: m[(r[4] << 8) | r[5]] = v
            else: r[i] = v
        def szp(v): s.fz = 1 if v == 0 else 0; s.fs = v >> 7; s.fp = PAR[v]
        def add(v, c=0):
            a = r[7]; t = a + v + c; s.fa = ((a & 15) + (v & 15) + c) >> 4; s.fc = t >> 8; t &= 255; r[7] = t; szp(t)
        def sub(v, c=0, keep=True):
            a = r[7]; t = a - v - c; s.fa = 1 if ((a & 15) - (v & 15) - c) >= 0 else 0; s.fc = 1 if t < 0 else 0; t &= 255; szp(t)
            if keep: r[7] = t
        def logic(t, ac=0): r[7] = t; s.fc = 0; s.fa = ac; szp(t)
        def push(v): s.sp = (s.sp - 2) & 0xFFFF; m[s.sp] = v & 255; m[(s.sp + 1) & 0xFFFF] = v >> 8
        def pop(): v = m[s.sp] | (m[(s.sp + 1) & 0xFFFF] << 8); s.sp = (s.sp + 2) & 0xFFFF; return v
        def imm8(): v = m[s.pc]; s.pc = (s.pc + 1) & 0xFFFF; return v
        def imm16(): v = m[s.pc] | (m[(s.pc + 1) & 0xFFFF] << 8); s.pc = (s.pc + 2) & 0xFFFF; return v
        def psw(): return (r[7] << 8) | (s.fs << 7) | (s.fz << 6) | (s.fa << 4) | (s.fp << 2) | 2 | s.fc
        def cond(k): return (not s.fz, s.fz, not s.fc, s.fc, not s.fp, s.fp, not s.fs, s.fs)[k]
        def rp(k): return s.sp if k == 3 else (r[2 * k] << 8) | r[2 * k + 1]
        def setrp(k, v):
            if k == 3: s.sp = v
            else: r[2 * k] = v >> 8; r[2 * k + 1] = v & 255
        for op in range(256):
            hi, d, sr = op >> 6, (op >> 3) & 7, op & 7
            if op == 0x76: f = lambda: (_ for _ in ()).throw(Exit())
            elif hi == 1: f = (lambda d=d, sr=sr: wr(d, rd(sr)))
            elif hi == 2:
                f = [lambda sr=sr: add(rd(sr)), lambda sr=sr: add(rd(sr), s.fc), lambda sr=sr: sub(rd(sr)), lambda sr=sr: sub(rd(sr), s.fc),
                     lambda sr=sr: logic(r[7] & rd(sr), ((r[7] | rd(sr)) >> 3) & 1), lambda sr=sr: logic(r[7] ^ rd(sr)), lambda sr=sr: logic(r[7] | rd(sr)),
                     lambda sr=sr: sub(rd(sr), 0, False)][d]
            elif hi == 0:
                k = d >> 1
                if sr == 0: f = lambda: None
                elif sr == 1 and not d & 1: f = lambda k=k: setrp(k, imm16())
                elif sr == 1:
                    def f(k=k):
                        t = ((r[4] << 8) | r[5]) + rp(k); s.fc = t >> 16; r[4] = (t >> 8) & 255; r[5] = t & 255
                elif sr == 2:
                    if d == 0: f = lambda: m.__setitem__((r[0] << 8) | r[1], r[7])
                    elif d == 1: f = lambda: r.__setitem__(7, m[(r[0] << 8) | r[1]])
                    elif d == 2: f = lambda: m.__setitem__((r[2] << 8) | r[3], r[7])
                    elif d == 3: f = lambda: r.__setitem__(7, m[(r[2] << 8) | r[3]])
                    elif d == 4:
                        def f(): a = imm16(); m[a] = r[5]; m[(a + 1) & 0xFFFF] = r[4]
                    elif d == 5:
                        def f(): a = imm16(); r[5] = m[a]; r[4] = m[(a + 1) & 0xFFFF]
                    elif d == 6: f = lambda: m.__setitem__(imm16(), r[7])
                    else: f = lambda: r.__setitem__(7, m[imm16()])
                elif sr == 3: f = (lambda k=k: setrp(k, (rp(k) + 1) & 0xFFFF)) if not d & 1 else (lambda k=k: setrp(k, (rp(k) - 1) & 0xFFFF))
                elif sr == 4:
                    def f(d=d): v = (rd(d) + 1) & 255; wr(d, v); s.fa = 1 if (v & 15) == 0 else 0; szp(v)
                elif sr == 5:
                    def f(d=d): v = (rd(d) - 1) & 255; wr(d, v); s.fa = 0 if (v & 15) == 15 else 1; szp(v)
                elif sr == 6: f = lambda d=d: wr(d, imm8())
                else:
                    if d == 0:
                        def f(): a = r[7]; s.fc = a >> 7; r[7] = ((a << 1) | s.fc) & 255
                    elif d == 1:
                        def f(): a = r[7]; s.fc = a & 1; r[7] = (a >> 1) | (s.fc << 7)
                    elif d == 2:
                        def f(): a = r[7]; c = s.fc; s.fc = a >> 7; r[7] = ((a << 1) | c) & 255
                    elif d == 3:
                        def f(): a = r[7]; c = s.fc; s.fc = a & 1; r[7] = (a >> 1) | (c << 7)
                    elif d == 4:
                        def f():
                            a = r[7]; c = s.fc; adj = 0
                            if (a & 15) > 9 or s.fa: adj = 6
                            if a > 0x99 or c: adj |= 0x60; c = 1
                            s.fa = 1 if (a & 15) + (adj & 15) > 15 else 0; a = (a + adj) & 255; r[7] = a; szp(a); s.fc = c
                    elif d == 5: f = lambda: r.__setitem__(7, r[7] ^ 255)
                    elif d == 6: f = lambda: setattr(s, "fc", 1)
                    else: f = lambda: setattr(s, "fc", s.fc ^ 1)
            else:
                k = d >> 1
                if sr == 0:
                    def f(d=d):
                        if cond(d): s.pc = pop()
                elif sr == 1:
                    if not d & 1:
                        if k == 3:
                            def f(): v = pop(); r[7] = v >> 8; s.fs = (v >> 7) & 1; s.fz = (v >> 6) & 1; s.fa = (v >> 4) & 1; s.fp = (v >> 2) & 1; s.fc = v & 1
                        else: f = lambda k=k: setrp(k, pop())
                    elif d == 1: f = lambda: setattr(s, "pc", pop())
                    elif d == 5: f = lambda: setattr(s, "pc", (r[4] << 8) | r[5])
                    elif d == 7: f = lambda: setattr(s, "sp", (r[4] << 8) | r[5])
                    else: f = lambda: setattr(s, "pc", pop())
                elif sr == 2:
                    def f(d=d):
                        a = imm16()
                        if cond(d): s.pc = a
                elif sr == 3:
                    if d == 0 or d == 1: f = lambda: setattr(s, "pc", imm16())
                    elif d == 2 or d == 3: f = lambda: imm8()
                    elif d == 4:
                        def f(): l, h = m[s.sp], m[(s.sp + 1) & 0xFFFF]; m[s.sp] = r[5]; m[(s.sp + 1) & 0xFFFF] = r[4]; r[5] = l; r[4] = h
                    elif d == 5:
                        def f(): r[2], r[4] = r[4], r[2]; r[3], r[5] = r[5], r[3]
                    else: f = lambda: None
                elif sr == 4:
                    def f(d=d):
                        a = imm16()
                        if cond(d): push(s.pc); s.pc = a
                elif sr == 5:
                    if not d & 1: f = (lambda: push(psw())) if k == 3 else (lambda k=k: push(rp(k)))
                    else:
                        def f(): a = imm16(); push(s.pc); s.pc = a
                elif sr == 6:
                    f = [lambda: add(imm8()), lambda: add(imm8(), s.fc), lambda: sub(imm8()), lambda: sub(imm8(), s.fc),
                         lambda: (lambda v: logic(r[7] & v, ((r[7] | v) >> 3) & 1))(imm8()), lambda: logic(r[7] ^ imm8()), lambda: logic(r[7] | imm8()),
                         lambda: sub(imm8(), 0, False)][d]
                else:
                    def f(d=d): push(s.pc); s.pc = d * 8
            ops[op] = f
        s.pop = pop

    def run(s, limit=2_000_000_000):
        m = s.m; ops = s.ops; n = 0
        try:
            while n < limit:
                pc = s.pc
                if pc == 0x40:
                    if not s.isis(): s.pc = s.pop()
                    n += 1; continue
                if pc >= 0xF800:
                    if pc == 0xF81B: s.r[7] = 0xC0; s.r[0] = 0xF6                    # MEMCK: top of memory in B:A (ASM86 wants at least F6C0)
                    elif pc == 0xF809: s.console.append(s.r[1])                     # CO
                    elif pc == 0xF803: s.r[7] = 0x0D                                # CI
                    s.pc = s.pop(); n += 1; continue
                s.pc = (pc + 1) & 0xFFFF
                ops[m[pc]]()
                n += 1
        except Exit: pass
        s.steps = n
        return n

def run(program, tail, drives, echo=False, limit=2_000_000_000):
    """program: host path of the ISIS object file; tail: the command line after the program name"""
    M = Machine(drives, tail, echo); M.pc = M.load(program); n = M.run(limit)
    return Result(M.console.decode("latin1"), 0 if n < limit else -1, n)

if __name__ == "__main__":
    a = sys.argv[1:]; drives = {}
    while a and a[0].startswith("-d"): drives[int(a[0][2])] = a[1]; a = a[2:]
    drives.setdefault(0, os.path.dirname(os.path.abspath(a[0])))
    r = run(a[0], " " + " ".join(a[1:]), drives, echo=True)
    print("\n[%d instructions]" % r.steps)
