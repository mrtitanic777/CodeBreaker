#!/usr/bin/env python3
"""dos.py - run a real-mode DOS program under Unicorn, in this process, with just enough of DOS behind
INT 21h for the compilers, assemblers and linkers of the 1980s: files (handles and FCBs), memory
blocks, console, EXEC, version, time. (From Microsoft-Assemblers-Compilers research/masm-1.00/
reversing/tools/dosrun.py, which ran every period tool of those rebuilds.)

It is an ORACLE and a TRACER, not part of the reconstruction: the original binary assembles real
source here, and every basic block it executes is recorded, so the reconstruction can be checked
against behaviour (which function runs for which statement; what the output is).

  python tools/dosrun.py <MASM.EXE> <workdir> "TEST.ASM,TEST.OBJ,TEST.LST;"   # run, print console
As a module:  r = run(exe, workdir, cmdline) -> Result(exit, console, blocks, load_seg, files)

`blocks` is the set of linear addresses of executed basic blocks; (addr - load_seg*16) is an
offset into the program's load image (for an EXEPACK'd program: into the UNPACKED image, since
the unpacker runs here too and unpacks in place).
"""
import os, sys, struct, time, collections

PSP = 0x0100                 # PSP segment; the program loads at PSP + 0x10 = 0x0110
TOP = 0xA000                 # first segment past conventional memory
FAST_LIMIT = float(os.environ.get("DOSRUN_FAST_LIMIT", "20"))   # (seconds a fast run may take: one that never ends)
Result = collections.namedtuple("Result", "exit console blocks load_seg files steps calls")

class DosExit(Exception): pass


class MemFS:
    """the files a run sees, kept in memory: what the program writes stays here (its passes share them), what it
    reads and was not written comes from the disk - read once, kept for every run after (DISK) - except under the
    folders hidden (an include folder given whole each run). No file is written to the disk"""
    DISK = {}                                               # (path -> (mtime, size, bytes)) - the tools' own files

    def __init__(self, hide=()):
        self.files = {}
        self.gone = set()
        self.hide = [os.path.normcase(os.path.abspath(h)) + os.sep for h in hide]

    def key(self, p):
        return os.path.normcase(os.path.abspath(p))

    def _hidden(self, k):
        return any(k.startswith(h) for h in self.hide)

    def disk(self, p):
        k = self.key(p)
        st_ = os.stat(p)
        got = MemFS.DISK.get(k)
        if got and got[0] == st_.st_mtime and got[1] == st_.st_size:
            return got[2]
        b = ropen(p, "rb").read()
        MemFS.DISK[k] = (st_.st_mtime, st_.st_size, b)
        return b

    def isfile(self, p):
        k = self.key(p)
        if k in self.files:
            return True
        return k not in self.gone and not self._hidden(k) and os.path.isfile(p)

    def getsize(self, p):
        k = self.key(p)
        return len(self.files[k]) if k in self.files else os.path.getsize(p)

    def read(self, p):
        k = self.key(p)
        return bytes(self.files[k]) if k in self.files else self.disk(p)

    def open(self, p, mode="rb"):
        k = self.key(p)
        if "w" in mode:
            self.files[k] = bytearray()
            self.gone.discard(k)
        elif k not in self.files:
            if not self.isfile(p):
                raise FileNotFoundError(p)
            self.files[k] = bytearray(self.disk(p))
        return MemFile(self.files[k])

    def put(self, p, b):
        self.files[self.key(p)] = bytearray(b)
        self.gone.discard(self.key(p))

    def remove(self, p):
        k = self.key(p)
        if k in self.files:
            del self.files[k]
        elif not self.isfile(p):
            raise FileNotFoundError(p)
        self.gone.add(k)

    def replace(self, a, b):
        self.put(b, self.read(a))
        self.remove(a)


class MemFile:
    """a file of a MemFS: the operations the DOS calls use"""
    def __init__(self, buf):
        self.buf, self.pos = buf, 0

    def read(self, n=-1):
        end = len(self.buf) if n is None or n < 0 else min(len(self.buf), self.pos + n)
        d = bytes(self.buf[self.pos:end])
        self.pos = max(self.pos, end)
        return d

    def write(self, d):
        if self.pos > len(self.buf):
            self.buf.extend(b"\0" * (self.pos - len(self.buf)))
        self.buf[self.pos:self.pos + len(d)] = d
        self.pos += len(d)
        return len(d)

    def seek(self, off, whence=0):
        self.pos = max(0, off if whence == 0 else self.pos + off if whence == 1 else len(self.buf) + off)
        return self.pos

    def tell(self):
        return self.pos

    def truncate(self, size=None):
        del self.buf[self.pos if size is None else size:]

    def flush(self):
        pass

    def close(self):
        pass

def ropen(path, mode="rb", **kw):
    """open(), retried: on a busy Windows machine (virus scanner, indexer) a file just created or
    closed can refuse to open for a moment (EINVAL / EACCES). A failed open here would reach the
    emulated program as a DOS error and change what it does, so wait instead of reporting it."""
    import time
    for k in range(40):
        try: return open(path, mode, **kw)
        except FileNotFoundError: raise
        except OSError:
            if k == 39: raise
            time.sleep(0.05 * (k + 1))

def run(exe, workdir, cmdline="", stdin=b"", max_blocks=int(os.environ.get("DOSRUN_MAX_BLOCKS", "60000000")), trace_calls=False, env=b"PATH=\0", instrument=None, instrument_when=None, driver=None, psp=None, top=None, fast=False, fs=None):
    # fast: no hook on every basic block (the blocks are not counted or kept - Result.blocks is empty); a run
    # that does not end is stopped by a time limit instead (FAST_LIMIT seconds). Most of a compile's time
    # went to that hook.
    PSP = psp or globals()["PSP"]                # (the PSP's segment: some passes need to be loaded low, some high)
    TOP = top or globals()["TOP"]                # (the top of memory: some programs fail above 512K)
    from unicorn import Uc, UC_ARCH_X86, UC_MODE_16, UC_HOOK_INTR, UC_HOOK_BLOCK, UC_HOOK_CODE, UcError
    from unicorn.x86_const import (UC_X86_REG_AX, UC_X86_REG_BX, UC_X86_REG_CX, UC_X86_REG_DX, UC_X86_REG_SI,
        UC_X86_REG_DI, UC_X86_REG_BP, UC_X86_REG_SP, UC_X86_REG_IP, UC_X86_REG_CS, UC_X86_REG_DS,
        UC_X86_REG_ES, UC_X86_REG_SS, UC_X86_REG_EFLAGS)
    # fs: a MemFS - the files the program opens, makes, removes kept in memory (the disk only read)
    fsopen = fs.open if fs else ropen
    fsisfile = fs.isfile if fs else os.path.isfile
    fssize = fs.getsize if fs else os.path.getsize
    fsremove = fs.remove if fs else os.remove
    fsreplace = fs.replace if fs else os.replace
    b = fs.read(exe) if fs else ropen(exe, "rb").read()
    assert b[:2] in (b"MZ", b"ZM"), "not an MZ executable"
    (lastpage, pages, nreloc, hdrp, minalloc, maxalloc, ss, sp, _ck, ip, cs, relofs) = struct.unpack_from("<12H", b, 2)
    size = pages * 512 - ((512 - lastpage) if lastpage else 0)
    image = bytearray(b[hdrp * 16:size])
    load = PSP + 0x10
    for k in range(nreloc):
        off, seg = struct.unpack_from("<HH", b, relofs + 4 * k)
        a = seg * 16 + off
        struct.pack_into("<H", image, a, (struct.unpack_from("<H", image, a)[0] + load) & 0xFFFF)
    # the guest memory is a host buffer: compiled reconstructed C (tools/cbuild.py) works on it directly
    import ctypes
    membuf = ctypes.create_string_buffer(0x200000)
    mu = Uc(UC_ARCH_X86, UC_MODE_16); mu.mem_map_ptr(0, 0x110000, 7, ctypes.addressof(membuf))
    mu.mem_write(load * 16, bytes(image))
    # environment block + PSP
    envseg = 0x00C0
    mu.mem_write(envseg * 16, env + b"\0" + b"\x01\0" + b"C:\\MASM.EXE\0")
    psp = bytearray(256)
    psp[0:2] = b"\xCD\x20"; struct.pack_into("<H", psp, 2, TOP); struct.pack_into("<H", psp, 0x2C, envseg)
    tail = (" " + cmdline if cmdline else "").encode("latin1")[:126]
    psp[0x80] = len(tail); psp[0x81:0x81 + len(tail)] = tail; psp[0x81 + len(tail)] = 0x0D
    mu.mem_write(PSP * 16, bytes(psp))
    for r, v in ((UC_X86_REG_CS, cs + load), (UC_X86_REG_IP, ip), (UC_X86_REG_SS, ss + load), (UC_X86_REG_SP, sp),
                 (UC_X86_REG_DS, PSP), (UC_X86_REG_ES, PSP), (UC_X86_REG_AX, 0), (UC_X86_REG_EFLAGS, 0x202)):
        mu.reg_write(r, v)

    st = dict(blocks=set(), n=0, console=bytearray(), exit=None, stdin=bytearray(stdin), dta=PSP * 16 + 0x80,
              mcb=[[PSP, TOP - PSP, True]], vec={}, calls=collections.Counter())   # memory blocks: [seg, paras, used]
    files = {}; nexth = [5]
    def newh():                                             # the next handle; past 19 (where a C library's table of
        if nexth[0] < 20: nexth[0] += 1; return nexth[0] - 1    # files ends) the lowest free one, as DOS gives them
        return next(h for h in range(5, 255) if h not in files)
    R = lambda r: mu.reg_read(r)
    def W(r, v): mu.reg_write(r, v & 0xFFFF)
    def cstr(seg, off):
        a = seg * 16 + off; out = bytearray()
        while True:
            c = mu.mem_read(a, 1)[0]
            if c == 0: break
            out.append(c); a += 1
        return out.decode("latin1")
    def hostpath(name):
        n = name.replace("/", "\\").upper()
        if len(n) > 1 and n[1] == ":": n = n[2:]
        n = n.lstrip("\\")
        return os.path.join(workdir, *[p for p in n.split("\\") if p not in ("", ".")])
    def carry(on, ax=None):
        f = R(UC_X86_REG_EFLAGS)
        mu.reg_write(UC_X86_REG_EFLAGS, (f | 1) if on else (f & ~1))
        if ax is not None: W(UC_X86_REG_AX, ax)
    def alloc(paras):
        for blk in st["mcb"]:
            if not blk[2] and blk[1] >= paras:
                if blk[1] > paras: st["mcb"].insert(st["mcb"].index(blk) + 1, [blk[0] + paras, blk[1] - paras, False])
                blk[1] = paras; blk[2] = True; return blk[0]
        return None
    def largest(): return max([blk[1] for blk in st["mcb"] if not blk[2]] + [0])
    def merge():
        m = st["mcb"]; i = 0
        while i + 1 < len(m):
            if not m[i][2] and not m[i+1][2]: m[i][1] += m[i+1][1]; del m[i+1]
            else: i += 1

    def int21():
        ax = R(UC_X86_REG_AX); ah = ax >> 8; al = ax & 0xFF
        bx, cx, dx, ds, es = R(UC_X86_REG_BX), R(UC_X86_REG_CX), R(UC_X86_REG_DX), R(UC_X86_REG_DS), R(UC_X86_REG_ES)
        if os.environ.get("DOSRUN_DEBUG"): print("int21 ax=%04x bx=%04x cx=%04x dx=%04x" % (ax, bx, cx, dx), file=sys.stderr)
        if ah == 0x4C or ah == 0x00: st["exit"] = al if ah == 0x4C else 0; raise DosExit()
        if ah in (0x02, 0x06):
            if ah == 0x06 and (dx & 0xFF) == 0xFF: carry(False); W(UC_X86_REG_AX, ax & 0xFF00); return
            st["console"].append(dx & 0xFF); return
        if ah == 0x09:
            a = ds * 16 + dx
            while True:
                c = mu.mem_read(a, 1)[0]
                if c == 0x24: break
                st["console"].append(c); a += 1
            return
        if ah == 0x0A:                                           # buffered line input
            a = ds * 16 + dx; mx = mu.mem_read(a, 1)[0]
            line = bytearray()
            while st["stdin"]:
                c = st["stdin"].pop(0)
                if c == 0x0A: continue
                if c == 0x0D: break
                line.append(c)
            line = line[:max(mx - 1, 0)]
            st["console"] += bytes(line) + b"\r"                 # (DOS echoes the line, and the carriage return)
            mu.mem_write(a + 1, bytes([len(line)]) + bytes(line) + b"\r"); return
        if ah in (0x01, 0x07, 0x08):
            c = st["stdin"].pop(0) if st["stdin"] else 0x0D
            W(UC_X86_REG_AX, (ah << 8) | c); return
        if ah == 0x0B: W(UC_X86_REG_AX, (ah << 8) | (0xFF if st["stdin"] else 0)); return
        if ah == 0x0E: W(UC_X86_REG_AX, (ah << 8) | 3); return
        if ah == 0x19: W(UC_X86_REG_AX, (ah << 8) | int(os.environ.get("DOSRUN_DRIVE", "2"))); return   # current drive C: (DOSRUN_DRIVE=3: D:)
        if ah == 0x1A: st["dta"] = ds * 16 + dx; return
        # ---- DOS 1.x file control blocks (programs from before the handle calls of DOS 2)
        # FCB: +0 drive, +1 name[8], +9 ext[3], +0C current block, +0E record size, +10 file size,
        #      +20 current record, +21 random record (4 bytes)
        if ah in (0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x21, 0x22, 0x23, 0x24, 0x27, 0x28):
            fa = ds * 16 + dx
            if mu.mem_read(fa, 1)[0] == 0xFF: fa += 7            # extended FCB
            rb = lambda o, n=1: bytes(mu.mem_read(fa + o, n))
            rw = lambda o: int.from_bytes(rb(o, 2), "little")
            ww = lambda o, v: mu.mem_write(fa + o, int(v & 0xFFFF).to_bytes(2, "little"))
            nm = rb(1, 8).decode("latin1").rstrip(); ex = rb(9, 3).decode("latin1").rstrip()
            hp = hostpath(nm + ("." + ex if ex else ""))
            fcbs = st.setdefault("fcb", {})
            setal = lambda v: W(UC_X86_REG_AX, (ah << 8) | (v & 0xFF))
            def size_now(f): cur = f.tell(); f.seek(0, 2); n_ = f.tell(); f.seek(cur); return n_
            if ah in (0x0F, 0x16):
                try:
                    if ah == 0x0F:
                        if not fsisfile(hp): setal(0xFF); return
                        f = fsopen(hp, "r+b")
                    else: f = fsopen(hp, "w+b")
                except OSError: setal(0xFF); return
                fcbs[fa] = f
                ww(0x0C, 0); ww(0x0E, 0x80); mu.mem_write(fa + 0x10, size_now(f).to_bytes(4, "little"))
                mu.mem_write(fa + 0x20, b"\0"); setal(0); return
            if ah == 0x10:
                f = fcbs.pop(fa, None)
                if f: f.close()
                setal(0); return
            if ah in (0x11, 0x12):
                if ah == 0x11 and "?" not in nm + ex and fsisfile(hp):
                    d_ = bytearray(32); d_[0] = rb(0)[0]; d_[1:12] = rb(1, 11)
                    d_[0x1C:0x20] = fssize(hp).to_bytes(4, "little"); mu.mem_write(st["dta"], bytes(d_)); setal(0)
                else: setal(0xFF)
                return
            if ah == 0x13:
                try: fsremove(hp); setal(0)
                except OSError: setal(0xFF)
                return
            f = fcbs.get(fa)
            if f is None: setal(0xFF if ah in (0x23,) else 1); return
            rs = rw(0x0E) or 0x80
            rnd = int.from_bytes(rb(0x21, 4), "little") if rs < 64 else int.from_bytes(rb(0x21, 3), "little")
            def seqpos(): return (rw(0x0C) * 128 + rb(0x20)[0]) * rs
            def setseq(rec): ww(0x0C, rec // 128); mu.mem_write(fa + 0x20, bytes([rec % 128]))
            def setsize(): mu.mem_write(fa + 0x10, size_now(f).to_bytes(4, "little"))
            if ah == 0x23: mu.mem_write(fa + 0x21, ((size_now(f) + rs - 1) // rs).to_bytes(4, "little")[:4 if rs < 64 else 3]); setal(0); return
            if ah == 0x24: mu.mem_write(fa + 0x21, (rw(0x0C) * 128 + rb(0x20)[0]).to_bytes(4, "little")[:4 if rs < 64 else 3]); return
            if ah in (0x14, 0x21):                               # read one record (sequential / random)
                if ah == 0x21: setseq(rnd)
                f.seek(seqpos()); d_ = f.read(rs)
                if not d_: setal(1); return
                mu.mem_write(st["dta"], d_ + b"\0" * (rs - len(d_)))
                if ah == 0x14: setseq(rw(0x0C) * 128 + rb(0x20)[0] + 1)
                setal(0 if len(d_) == rs else 3); return
            if ah in (0x15, 0x22):                               # write one record
                if ah == 0x22: setseq(rnd)
                f.seek(seqpos()); f.write(bytes(mu.mem_read(st["dta"], rs))); f.flush(); setsize()
                if ah == 0x15: setseq(rw(0x0C) * 128 + rb(0x20)[0] + 1)
                setal(0); return
            if ah == 0x27:                                       # random block read: CX records
                f.seek(rnd * rs); d_ = f.read(cx * rs); n_ = (len(d_) + rs - 1) // rs
                if d_: mu.mem_write(st["dta"], d_ + b"\0" * (n_ * rs - len(d_)))
                W(UC_X86_REG_CX, n_); rnd += n_; setseq(rnd)
                mu.mem_write(fa + 0x21, rnd.to_bytes(4, "little")[:4 if rs < 64 else 3])
                setal(0 if len(d_) == cx * rs else (1 if not d_ or len(d_) % rs == 0 else 3)); return
            if ah == 0x28:                                       # random block write: CX records (0: set the size)
                f.seek(rnd * rs)
                if cx == 0: f.truncate(rnd * rs)
                else: f.write(bytes(mu.mem_read(st["dta"], cx * rs)))
                f.flush(); setsize(); rnd += cx; setseq(rnd)
                mu.mem_write(fa + 0x21, rnd.to_bytes(4, "little")[:4 if rs < 64 else 3]); setal(0); return
        if ah == 0x17:                                           # rename by FCB: old name at +1, new name at +11h
            fa = ds * 16 + dx
            if mu.mem_read(fa, 1)[0] == 0xFF: fa += 7
            nm = lambda o: (bytes(mu.mem_read(fa + o, 8)).decode("latin1").rstrip(), bytes(mu.mem_read(fa + o + 8, 3)).decode("latin1").rstrip())
            (n1, e1), (n2, e2) = nm(1), nm(0x11)
            try: fsreplace(hostpath(n1 + ("." + e1 if e1 else "")), hostpath(n2 + ("." + e2 if e2 else ""))); W(UC_X86_REG_AX, 0x1700)
            except OSError: W(UC_X86_REG_AX, 0x17FF)
            return
        if ah == 0x29:                                           # parse a file name at DS:SI into the FCB at ES:DI
            si_ = R(UC_X86_REG_SI); di_ = R(UC_X86_REG_DI); a_ = ds * 16 + si_; q = a_
            rd1 = lambda x: mu.mem_read(x, 1)[0]
            if al & 1:
                while rd1(q) in b" \t;,=+": q += 1
            else:
                while rd1(q) in b" \t": q += 1
            drive = 0
            if rd1(q + 1) == 0x3A and chr(rd1(q)).isalpha(): drive = (rd1(q) & 0x1F); q += 2
            def field(n):
                nonlocal q
                out = bytearray(); wild = False
                while len(out) < n:
                    c = rd1(q)
                    if c == 0x2A: out += b"?" * (n - len(out)); q += 1; wild = True; break
                    if c <= 0x20 or c in b".\"/\\[]:<>|+=;,": break
                    if c == 0x3F: wild = True
                    out.append(c - 32 if 0x61 <= c <= 0x7A else c); q += 1
                while rd1(q) > 0x20 and rd1(q) not in b".\"/\\[]:<>|+=;,": q += 1
                return bytes(out) + b" " * (n - len(out)), wild
            name_, w1 = field(8); ext_ = b"   "; w2 = False
            if rd1(q) == 0x2E: q += 1; ext_, w2 = field(3)
            mu.mem_write(es * 16 + di_, bytes([drive]) + name_ + ext_)
            W(UC_X86_REG_SI, si_ + (q - a_)); W(UC_X86_REG_AX, (ah << 8) | (1 if (w1 or w2) else 0)); return
        if ah == 0x25: st["vec"][al] = (ds, dx); return
        if ah == 0x35:
            s_, o_ = st["vec"].get(al, (0, 0)); W(UC_X86_REG_ES, s_); W(UC_X86_REG_BX, o_); return
        if ah == 0x2A: W(UC_X86_REG_CX, 1989); W(UC_X86_REG_DX, 0x0110); W(UC_X86_REG_AX, (ah << 8)); return
        if ah == 0x2C: W(UC_X86_REG_CX, 0x0C00); W(UC_X86_REG_DX, 0); return
        if ah == 0x30: W(UC_X86_REG_AX, 0x1E03); W(UC_X86_REG_BX, 0); W(UC_X86_REG_CX, 0); return   # DOS 3.30
        if ah == 0x33: W(UC_X86_REG_DX, dx & 0xFF00); return
        if ah == 0x37: W(UC_X86_REG_DX, (dx & 0xFF00) | 0x2F); W(UC_X86_REG_AX, ax & 0xFF00); return
        if ah == 0x38: carry(False); return
        if ah == 0x3C or (ah == 0x3D) or ah == 0x5B:
            name = cstr(ds, dx); up = name.upper().rstrip(":")
            if os.environ.get("DOSRUN_DEBUG"): print("open ah=%02x al=%02x %r -> %r" % (ah, al, name, hostpath(name)), file=sys.stderr)
            if up in ("CON", "NUL", "PRN", "AUX", "LST"):
                h = newh(); files[h] = dict(dev=up, f=None); carry(False, h); return
            hp = hostpath(name)
            try:
                if ah == 0x3D:
                    if not fsisfile(hp): carry(True, 2); return
                    f = fsopen(hp, "r+b" if (al & 3) else "rb")
                else: f = fsopen(hp, "w+b")
            except OSError: carry(True, 5); return
            h = newh(); files[h] = dict(dev=None, f=f, name=name); carry(False, h); return
        if ah == 0x3E:
            e = files.pop(bx, None)
            if e and e["f"]: e["f"].close()
            carry(False); return
        if ah == 0x3F:
            if bx == 0 or (bx in files and files[bx]["dev"] == "CON"):
                data = bytearray()
                while st["stdin"] and len(data) < cx:
                    c = st["stdin"].pop(0); data.append(c)
                    if c == 0x0A: break
            elif bx in files and files[bx]["f"]: data = files[bx]["f"].read(cx)
            else: data = b""
            mu.mem_write(ds * 16 + dx, bytes(data)); carry(False, len(data)); return
        if ah == 0x40:
            data = bytes(mu.mem_read(ds * 16 + dx, cx))
            if bx in (1, 2) or (bx in files and files[bx]["dev"] == "CON"): st["console"] += data
            elif bx in files and files[bx]["f"]:
                if cx == 0: files[bx]["f"].truncate()
                else: files[bx]["f"].write(data)
            carry(False, cx); return
        if ah == 0x41:
            try: fsremove(hostpath(cstr(ds, dx))); carry(False)
            except OSError: carry(True, 2)
            return
        if ah == 0x42:
            if bx in files and files[bx]["f"]:
                f = files[bx]["f"]; off = (cx << 16) | dx
                if al and off & 0x80000000: off -= 1 << 32
                f.seek(off, al); p = f.tell()
                W(UC_X86_REG_DX, p >> 16); carry(False, p & 0xFFFF)
            else: W(UC_X86_REG_DX, 0); carry(False, 0)
            return
        if ah == 0x43: carry(not fsisfile(hostpath(cstr(ds, dx))), 2 if not fsisfile(hostpath(cstr(ds, dx))) else None); W(UC_X86_REG_CX, 0x20); return
        if ah == 0x44:
            if al == 0:
                dev = bx <= 4 or (bx in files and files[bx]["dev"])
                W(UC_X86_REG_DX, 0x80D3 if dev else 0x0000); carry(False)
            else: carry(False, 0)
            return
        if ah == 0x45 or ah == 0x46: carry(False, bx); return
        if ah == 0x47: mu.mem_write(ds * 16 + R(UC_X86_REG_SI), b"\0"); carry(False); return
        if ah == 0x48:
            s_ = alloc(bx)
            if s_ is None: W(UC_X86_REG_BX, largest()); carry(True, 8)
            else: carry(False, s_)
            return
        if ah == 0x49:
            for blk in st["mcb"]:
                if blk[0] == es: blk[2] = False
            merge(); carry(False); return
        if ah == 0x4A:
            m = st["mcb"]
            for i, blk in enumerate(m):
                if blk[0] == es:
                    avail = blk[1] + (m[i+1][1] if i + 1 < len(m) and not m[i+1][2] else 0)
                    if bx > avail: W(UC_X86_REG_BX, avail); carry(True, 8); return
                    if i + 1 < len(m) and not m[i+1][2]: del m[i+1]
                    if avail > bx: m.insert(i + 1, [blk[0] + bx, avail - bx, False])
                    blk[1] = bx; carry(False); return
            carry(True, 9); return
        if ah == 0x4E:                                           # find first: an exact name only
            name = cstr(ds, dx); hp = hostpath(name)
            if "*" in name or "?" in name or not fsisfile(hp): carry(True, 18); return
            dta = bytearray(43); dta[0x15] = 0x20
            struct.pack_into("<I", dta, 0x1A, fssize(hp))
            base = os.path.basename(hp).upper().encode("latin1")[:12]
            dta[0x1E:0x1E + len(base)] = base
            mu.mem_write(st["dta"], bytes(dta)); carry(False, 0); return
        if ah == 0x4F: carry(True, 18); return
        if ah == 0x56:                                           # rename DS:DX to ES:DI
            old, new = hostpath(cstr(ds, dx)), hostpath(cstr(es, R(UC_X86_REG_DI)))
            if not fsisfile(old): carry(True, 2); return
            if fsisfile(new): carry(True, 5); return
            fsreplace(old, new); carry(False); return
        if ah == 0x57: W(UC_X86_REG_CX, 0); W(UC_X86_REG_DX, 0x1221); carry(False); return
        if ah == 0x59: W(UC_X86_REG_AX, 2); return
        if ah == 0x62 or ah == 0x51: W(UC_X86_REG_BX, PSP); return
        if ah == 0x0D or ah == 0x68: return
        if ah == 0x87: W(UC_X86_REG_AX, ax & 0xFF00); return        # no such function on MS-DOS (a C run-time asks: XENIX answers it)
        if ah == 0x4B:                                           # EXEC: the child runs to its end in an emulator of its own
            name = cstr(ds, dx); hp = hostpath(name)
            if not fsisfile(hp): carry(True, 2); return
            blk = es * 16 + bx; envs, toff, tseg = struct.unpack("<HHH", bytes(mu.mem_read(blk, 6)))
            n = mu.mem_read(tseg * 16 + toff, 1)[0]; tail = bytes(mu.mem_read(tseg * 16 + toff + 1, n)).decode("latin1")
            cenv = env
            if envs:
                a = envs * 16; e = bytearray()
                while bytes(mu.mem_read(a, 2)) != b"\0\0" and len(e) < 4096: e.append(mu.mem_read(a, 1)[0]); a += 1
                cenv = bytes(e) + b"\0"
            for h in files.values():                             # what the parent has written must be on the disk for the child
                try: h.flush()
                except Exception: pass
            r = run(hp, workdir, tail.lstrip(" "), stdin=bytes(st["stdin"]), max_blocks=max_blocks, env=cenv, psp=PSP, fast=fast, fs=fs)
            st["console"] += r.console; st["child"] = r.exit or 0
            if os.environ.get("DOSRUN_DEBUG"): print("exec %s%s -> %s  env %r" % (name, tail, r.exit, cenv[:300]), file=sys.stderr)
            carry(False); return
        if ah == 0x4D: W(UC_X86_REG_AX, st.get("child", 0) & 0xFF); return
        if ah == 0x63:                                           # the table of lead bytes (two-byte characters): none
            mu.mem_write(PSP * 16 + 0x48, bytes(2)); W(UC_X86_REG_DS, PSP); W(UC_X86_REG_SI, 0x48); carry(False); return
        raise RuntimeError("INT 21h AH=%02X not implemented (AX=%04X)" % (ah, ax))

    def intr(uc, intno, _):
        if intno == 0x21: int21()
        elif intno == 0x20: st["exit"] = 0; raise DosExit()
        elif intno in (0x10, 0x1A, 0x12, 0x11, 0x16, 0x2F): pass
        elif st["vec"].get(intno, (0, 0)) != (0, 0):         # a handler the program installed itself (an 8087 emulator's INT 34h-3Dh):
            sp_ = (R(UC_X86_REG_SP) - 6) & 0xFFFF            # do what the processor does, and go on there
            mu.mem_write(R(UC_X86_REG_SS) * 16 + sp_, struct.pack("<HHH", R(UC_X86_REG_IP), R(UC_X86_REG_CS), R(UC_X86_REG_EFLAGS) & 0xFFFF))
            W(UC_X86_REG_SP, sp_); s_, o_ = st["vec"][intno]
            mu.reg_write(UC_X86_REG_EFLAGS, R(UC_X86_REG_EFLAGS) & ~0x300)
            W(UC_X86_REG_CS, s_); W(UC_X86_REG_IP, o_); st["again"] = True; uc.emu_stop()
        else: raise RuntimeError("INT %02Xh not implemented" % intno)
    err = []
    def intr_safe(uc, intno, u):
        try: intr(uc, intno, u)
        except DosExit: uc.emu_stop()
        except Exception as e: err.append(e); uc.emu_stop()
    def block(uc, addr, sz, _):
        st["blocks"].add(addr); st["n"] += 1
        if st.get("arm") and instrument_when(uc, addr, load):      # the program proper has started:
            st["arm"] = False; st["resume"] = True; uc.emu_stop(); return   # attach the heavy hooks now
        if st["n"] > max_blocks: err.append(RuntimeError("block limit at image %05x; console so far: %r" % (addr - load * 16, bytes(st["console"])[-400:]))); uc.emu_stop()
    fast = fast and not instrument and not instrument_when and not trace_calls and not driver
    mu.hook_add(UC_HOOK_INTR, intr_safe)
    if not fast: mu.hook_add(UC_HOOK_BLOCK, block)
    lim = {"timeout": int(FAST_LIMIT * 1e6)} if fast else {}
    if trace_calls:
        def code(uc, addr, sz, _):
            b0 = uc.mem_read(addr, 1)[0]
            if b0 in (0xE8, 0x9A): st["calls"][addr] += 1
        mu.hook_add(UC_HOOK_CODE, code)
    # Extra hooks (tools/replay.py). With `instrument_when` they are attached only once that
    # predicate fires - a self-unpacking stub does not survive memory hooks.
    if instrument and not instrument_when: instrument(mu, load)
    st["arm"] = bool(instrument and instrument_when)
    try:
        st["membuf"] = membuf
        if driver:                                           # tools/hybrid.py runs the CPU itself
            driver(mu, load, st, err, (cs + load) * 16 + ip)
            raise StopIteration
        t_start = time.time()
        mu.emu_start((cs + load) * 16 + ip, 0x10FFFF, **lim)
        while st.get("again") and st["exit"] is None and not err:
            st["again"] = False; mu.emu_start(R(UC_X86_REG_CS) * 16 + R(UC_X86_REG_IP), 0x10FFFF, **lim)
        if fast and st["exit"] is None and not err and time.time() - t_start >= FAST_LIMIT * 0.9:
            err.append(RuntimeError("time limit at %05x; console so far: %r" % (R(UC_X86_REG_CS) * 16 + R(UC_X86_REG_IP) - load * 16, bytes(st["console"])[-400:])))
        if st.get("resume") and st["exit"] is None and not err:
            st["resume"] = False; instrument(mu, load)
            mu.emu_start(R(UC_X86_REG_CS) * 16 + R(UC_X86_REG_IP), 0x10FFFF)
    except StopIteration: pass
    except UcError as e:
        err.append(RuntimeError("cpu fault %s at %05x" % (e, R(UC_X86_REG_CS) * 16 + R(UC_X86_REG_IP))))
    for e in files.values():
        if e["f"]: e["f"].close()
    if err and st["exit"] is None: raise err[0]
    return Result(st["exit"], bytes(st["console"]), st["blocks"], load, None, st["n"], st["calls"])

if __name__ == "__main__":
    r = run(sys.argv[1], sys.argv[2], sys.argv[3] if len(sys.argv) > 3 else "")
    sys.stdout.write(r.console.decode("latin1"))
    print("\n[exit %s, %d basic blocks executed, %d distinct]" % (r.exit, r.steps, len(r.blocks)))
