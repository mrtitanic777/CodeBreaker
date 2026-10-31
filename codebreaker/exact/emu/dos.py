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


# ---- the DOS memory arena, kept in guest RAM the way DOS keeps it -------------------------------
# Every block is preceded by a 16-byte arena header (memory control block) one paragraph before it:
#   +0 'M' (another block follows) or 'Z' (the last), +1 owner PSP (0 free, 8 DOS), +3 size in
#   paragraphs (not counting the header), +8 name. The next header is at mcb + 1 + size. Programs read
#   this chain themselves (a DOS extender's loader walks it from its PSP - 1 to size the memory it may
#   take), so it is the one source of truth: INT 21h 48h/49h/4Ah work on it directly, as DOS does.
ENVSEG = 0x00C0              # the environment block (its header at ENVSEG - 1 is the first in the chain)
DOSSEG, LOL_OFF, LOL_SIZE = 0x0080, 0x0026, 0x40   # INT 21h 52h's list of lists (first MCB at -2)
MCB_FREE, MCB_DOS = 0, 8


class ArenaError(Exception):
    """a DOS memory error: args[0] is the DOS error code (7 arena trashed, 9 bad block)."""


def _mcb_write(mu, seg, sig, owner, size):
    mu.mem_write(seg * 16, sig + struct.pack("<HH", owner & 0xFFFF, size & 0xFFFF) + bytes(11))


def _mcb_set(mu, seg, sig=None, owner=None, size=None):
    if sig is not None: mu.mem_write(seg * 16, sig)
    if owner is not None: mu.mem_write(seg * 16 + 1, struct.pack("<H", owner & 0xFFFF))
    if size is not None: mu.mem_write(seg * 16 + 3, struct.pack("<H", size & 0xFFFF))


def _mcb_read(mu, seg):
    """-> (sig byte, owner, size) of the header at `seg`; ArenaError(7) if it is not one."""
    h = bytes(mu.mem_read(seg * 16, 5))
    if h[0] not in (0x4D, 0x5A):
        raise ArenaError(7)
    return h[0], h[1] | h[2] << 8, h[3] | h[4] << 8


def mcb_chain(mu, first):
    """-> [(mcb segment, sig, owner, size)], the whole arena from `first` to the 'Z' header."""
    out, seg = [], first
    for _ in range(0x10000):
        sig, owner, size = _mcb_read(mu, seg)
        out.append((seg, sig, owner, size))
        if sig == 0x5A:
            return out
        seg += 1 + size
        if seg > 0xFFFF:
            break
    raise ArenaError(7)


def _mcb_coalesce(mu, first):
    """join each run of free blocks into one (DOS does this before it allocates or resizes)."""
    chain = mcb_chain(mu, first)
    i = 0
    while i < len(chain):
        seg, sig, owner, size = chain[i]
        j = i
        while owner == MCB_FREE and j + 1 < len(chain) and chain[j + 1][2] == MCB_FREE:
            j += 1
            size += 1 + chain[j][3]
            sig = chain[j][1]
        if j > i:
            _mcb_set(mu, seg, sig=bytes([sig]), size=size)
        i = j + 1


def _mcb_largest(mu, first):
    try:
        _mcb_coalesce(mu, first)
        return max([c[3] for c in mcb_chain(mu, first) if c[2] == MCB_FREE] + [0])
    except ArenaError:
        return 0


def _split(mu, seg, sig, size, keep):
    """block at header `seg` (size paragraphs) keeps `keep`; the rest becomes a free block behind it."""
    if size > keep:
        _mcb_write(mu, seg + 1 + keep, bytes([sig]), MCB_FREE, size - keep - 1)
        _mcb_set(mu, seg, sig=b"M", size=keep)


def _mcb_alloc(mu, first, paras, owner, strategy=0):
    """-> the new block's segment, or None when no free block is large enough. The strategy is DOS's
    (INT 21h 58h): 0 first fit, 1 best fit (the smallest that fits), 2 last fit (the top of the last)."""
    try:
        _mcb_coalesce(mu, first)
        chain = mcb_chain(mu, first)
    except ArenaError:
        return None
    fits = [c for c in chain if c[2] == MCB_FREE and c[3] >= paras]
    if not fits:
        return None
    strategy &= 3
    seg, sig, own, size = (fits[0] if strategy == 0 else fits[-1] if strategy == 2
                           else min(fits, key=lambda c: c[3]))
    if strategy == 2 and size > paras:                        # the block at the free block's top end
        _mcb_set(mu, seg, sig=b"M", size=size - paras - 1)
        seg += size - paras
        _mcb_write(mu, seg, bytes([sig]), owner, paras)
        return seg + 1
    _split(mu, seg, sig, size, paras)
    _mcb_set(mu, seg, owner=owner)
    return seg + 1


def _mcb_free(mu, first, block):
    """free the block at segment `block` (INT 21h 49h)."""
    try:
        if block < 1: raise ArenaError(9)
        _mcb_read(mu, block - 1)
    except ArenaError:
        raise ArenaError(9)                                   # no header there: not a block
    if all(c[0] != block - 1 for c in mcb_chain(mu, first)):
        raise ArenaError(9)
    _mcb_set(mu, block - 1, owner=MCB_FREE)
    _mcb_coalesce(mu, first)


def _mcb_resize(mu, first, block, paras):
    """resize the block at segment `block` (INT 21h 4Ah) -> None, or the most it could have (error 8)."""
    _mcb_coalesce(mu, first)
    chain = mcb_chain(mu, first)
    k = next((i for i, c in enumerate(chain) if c[0] == block - 1), None)
    if k is None:
        raise ArenaError(9)
    seg, sig, owner, size = chain[k]
    nxt = chain[k + 1] if k + 1 < len(chain) else None
    avail = size + (1 + nxt[3] if nxt and nxt[2] == MCB_FREE else 0)
    if paras > avail:
        return avail
    if paras > size:                                          # grow into the free block behind
        sig, size = nxt[1], avail
        _mcb_set(mu, seg, sig=bytes([sig]), size=size)
    _split(mu, seg, sig, size, paras)
    _mcb_coalesce(mu, first)
    return None


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

def run(exe, workdir, cmdline="", stdin=b"", max_blocks=int(os.environ.get("DOSRUN_MAX_BLOCKS", "60000000")), trace_calls=False, env=b"PATH=\0", instrument=None, instrument_when=None, driver=None, psp=None, top=None, fast=False, fs=None, dpmi=None):
    # dpmi: a DPMI host (exact/emu/dpmi.py). None/False keeps plain real mode (unchanged). True builds
    # a host; an object is used as given. The host adds the INT 2Fh/1687h + INT 31h + protected-mode
    # switch on top of this same engine and routes protected-mode INT 21h back through int21() below.
    # fast: no hook on every basic block (the blocks are not counted or kept - Result.blocks is empty); a run
    # that does not end is stopped by a time limit instead (FAST_LIMIT seconds). Most of a compile's time
    # went to that hook.
    PSP = psp or globals()["PSP"]                # (the PSP's segment: some passes need to be loaded low, some high)
    TOP = top or globals()["TOP"]                # (the top of memory: some programs fail above 512K)
    from unicorn import Uc, UC_ARCH_X86, UC_MODE_16, UC_HOOK_INTR, UC_HOOK_BLOCK, UC_HOOK_CODE, UcError
    from unicorn.x86_const import UC_X86_REG_AX, UC_X86_REG_BX, UC_X86_REG_CX, UC_X86_REG_DX, UC_X86_REG_SI, UC_X86_REG_DI, UC_X86_REG_SP, UC_X86_REG_IP, UC_X86_REG_CS, UC_X86_REG_DS, UC_X86_REG_ES, UC_X86_REG_SS, UC_X86_REG_EFLAGS, UC_X86_REG_CR0, UC_X86_REG_EIP
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
    envseg = ENVSEG
    # (after the strings: the program's own path, as DOS gives it - a DOS extender finds the program it
    #  is to load there; the drive is the working folder's)
    rel = os.path.relpath(os.path.abspath(exe), os.path.abspath(workdir))
    if rel.startswith(".."): rel = os.path.basename(exe)
    envb = env + b"\0" + b"\x01\0" + b"C:\\" + rel.replace("/", "\\").upper().encode("latin1") + b"\0"
    mu.mem_write(envseg * 16, envb)
    # the DOS memory arena, in guest RAM as DOS keeps it (see the MCB helpers below): the environment,
    # DOS's own block over any gap up to the program, and the program's block to the top of memory
    envparas = min((len(envb) + 15) // 16, PSP - 1 - envseg)
    _mcb_write(mu, envseg - 1, b"M", PSP, envparas)
    gap = PSP - 1 - (envseg + envparas)
    if gap > 0:
        _mcb_write(mu, envseg + envparas, b"M", MCB_DOS, gap - 1)
    _mcb_write(mu, PSP - 1, b"Z", PSP, TOP - PSP)
    lol = bytearray(LOL_SIZE)                         # INT 21h 52h's list of lists: the first MCB just before it
    struct.pack_into("<H", lol, 0, envseg - 1)       # (the rest - drive tables, device chain - not kept: zero)
    mu.mem_write(DOSSEG * 16 + LOL_OFF - 2, bytes(lol))
    psp = bytearray(256)
    psp[0:2] = b"\xCD\x20"; struct.pack_into("<H", psp, 2, TOP); struct.pack_into("<H", psp, 0x2C, envseg)
    struct.pack_into("<H", psp, 0x16, PSP)                  # (parent: itself, as the shell's)
    psp[0x18:0x2C] = b"\x01\x01\x01\x00\x02" + b"\xFF" * 15     # the job file table: CON CON CON AUX PRN
    struct.pack_into("<HHH", psp, 0x32, 20, 0x18, PSP)      # its size and far pointer
    psp[0x50:0x53] = b"\xCD\x21\xCB"                        # the INT 21h / RETF entry
    tail = (" " + cmdline if cmdline else "").encode("latin1")[:126]
    psp[0x80] = len(tail); psp[0x81:0x81 + len(tail)] = tail; psp[0x81 + len(tail)] = 0x0D
    mu.mem_write(PSP * 16, bytes(psp))
    for r, v in ((UC_X86_REG_CS, cs + load), (UC_X86_REG_IP, ip), (UC_X86_REG_SS, ss + load), (UC_X86_REG_SP, sp),
                 (UC_X86_REG_DS, PSP), (UC_X86_REG_ES, PSP), (UC_X86_REG_AX, 0), (UC_X86_REG_EFLAGS, 0x202)):
        mu.reg_write(r, v)

    st = dict(blocks=set(), n=0, console=bytearray(), exit=None, stdin=bytearray(stdin), dta=PSP * 16 + 0x80,
              vec={}, calls=collections.Counter())          # (memory blocks: the MCB arena in guest RAM)
    st["cur_psp"] = PSP              # INT 21h 50h/51h/62h
    st["strategy"] = 0               # INT 21h 58h: first fit
    st["pm"] = False                 # protected mode active (the DPMI host turns this on at the switch)
    st["desc"] = {}                  # selector (sel & 0xFFFC: RPL off, LDT bit kept) -> (base, limit, access)
    def lin(seg, off):
        # a (segment/selector : offset) -> the linear byte. Real mode: segment*16 + offset (UNCHANGED,
        # so every existing caller is byte-for-byte identical). Protected mode: descriptor base + offset.
        if st["pm"]:
            d = st["desc"].get(seg & 0xFFFC)
            if d is not None:
                return d[0] + off
        return seg * 16 + off
    files = {}; nexth = [5]
    def newh():                                             # the next handle; past 19 (where a C library's table of
        if nexth[0] < 20: nexth[0] += 1; return nexth[0] - 1    # files ends) the lowest free one, as DOS gives them
        return next(h for h in range(5, 255) if h not in files)
    R = lambda r: mu.reg_read(r)
    def W(r, v): mu.reg_write(r, v & 0xFFFF)
    def cstr(seg, off):
        a = lin(seg, off); out = bytearray()
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
    first_mcb = envseg - 1
    def alloc(paras):                                       # -> the block's segment, or None (no room)
        return _mcb_alloc(mu, first_mcb, paras, PSP)
    def largest(): return _mcb_largest(mu, first_mcb)

    def int21():
        ax = R(UC_X86_REG_AX); ah = ax >> 8; al = ax & 0xFF
        bx, cx, dx = R(UC_X86_REG_BX), R(UC_X86_REG_CX), R(UC_X86_REG_DX)
        # ds/es: the DPMI host, reflecting a protected-mode INT 21h to real mode (INT 31h 0300), sets
        # these overrides to the selectors whose descriptors map the client's real-mode segments, so
        # lin() resolves the right linear byte without a mid-run reg_write (which would corrupt the
        # segment's hidden base). Unset everywhere else -> the real registers, byte-for-byte as before.
        ds = st["ov_ds"] if st.get("ov_ds") is not None else R(UC_X86_REG_DS)
        es = st["ov_es"] if st.get("ov_es") is not None else R(UC_X86_REG_ES)
        def Wseg(name, v):                                  # a segment the call returns: when reflected from
            if st.get("ov_" + name) is not None: st["out_" + name] = v   # protected mode it goes back into the
            else: W(UC_X86_REG_ES if name == "es" else UC_X86_REG_DS, v)  # real-mode call structure, not a register
        if os.environ.get("DOSRUN_DEBUG"): print("int21 ax=%04x bx=%04x cx=%04x dx=%04x" % (ax, bx, cx, dx), file=sys.stderr)
        if ah == 0x4C or ah == 0x00: st["exit"] = al if ah == 0x4C else 0; raise DosExit()
        if ah in (0x02, 0x06):
            if ah == 0x06 and (dx & 0xFF) == 0xFF: carry(False); W(UC_X86_REG_AX, ax & 0xFF00); return
            st["console"].append(dx & 0xFF); return
        if ah == 0x09:
            a = lin(ds, dx)
            while True:
                c = mu.mem_read(a, 1)[0]
                if c == 0x24: break
                st["console"].append(c); a += 1
            return
        if ah == 0x0A:                                           # buffered line input
            a = lin(ds, dx); mx = mu.mem_read(a, 1)[0]
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
        if ah == 0x1A: st["dta"] = lin(ds, dx); return
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
            s_, o_ = st["vec"].get(al, (0, 0)); Wseg("es", s_); W(UC_X86_REG_BX, o_); return
        if ah == 0x2A: W(UC_X86_REG_CX, 1989); W(UC_X86_REG_DX, 0x0110); W(UC_X86_REG_AX, (ah << 8)); return
        if ah == 0x2B:                                           # set date: accepted when valid (DESQview's check is not)
            ok_ = 1980 <= cx <= 2099 and 1 <= dx >> 8 <= 12 and 1 <= dx & 0xFF <= 31
            W(UC_X86_REG_AX, (ah << 8) | (0 if ok_ else 0xFF)); return
        if ah == 0x2D: W(UC_X86_REG_AX, (ah << 8) | (0 if cx >> 8 < 24 and cx & 0xFF < 60 and dx >> 8 < 60 else 0xFF)); return
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
            mu.mem_write(lin(ds, dx), bytes(data)); carry(False, len(data)); return
        if ah == 0x40:
            data = bytes(mu.mem_read(lin(ds, dx), cx))
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
        if ah == 0x47: mu.mem_write(lin(ds, R(UC_X86_REG_SI)), b"\0"); carry(False); return
        if ah in (0x48, 0x49, 0x4A):                             # memory: the MCB arena in guest RAM
            try:
                if ah == 0x48:
                    s_ = _mcb_alloc(mu, first_mcb, bx, PSP, st["strategy"])
                    if s_ is None: W(UC_X86_REG_BX, _mcb_largest(mu, first_mcb)); carry(True, 8)
                    else: carry(False, s_)
                elif ah == 0x49:
                    _mcb_free(mu, first_mcb, es); carry(False)
                else:
                    got = _mcb_resize(mu, first_mcb, es, bx)
                    if got is None: carry(False)
                    else: W(UC_X86_REG_BX, got); carry(True, 8)
            except ArenaError as e:
                carry(True, e.args[0])
            return
        if ah == 0x58:                                           # the allocation strategy (UMBs: none, not linked)
            if al == 0: carry(False, st["strategy"])
            elif al == 1: st["strategy"] = bx & 0xFF; carry(False)
            elif al == 2: carry(False, 0)
            elif al == 3: carry(False)
            else: carry(True, 1)
            return
        if ah == 0x67: carry(False); return                      # set handle count: the handle table has room
        if ah == 0x52:                                           # list of lists: the first MCB at ES:[BX-2]
            Wseg("es", DOSSEG); W(UC_X86_REG_BX, LOL_OFF); return
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
        if ah == 0x62 or ah == 0x51: W(UC_X86_REG_BX, st["cur_psp"]); return
        if ah == 0x50: st["cur_psp"] = bx; return               # set the current PSP
        if ah in (0x26, 0x55):                                   # create a (child) PSP at DX, from the current one
            new_ = bytearray(mu.mem_read(st["cur_psp"] * 16, 256))
            if ah == 0x55:
                struct.pack_into("<H", new_, 2, R(UC_X86_REG_SI)); struct.pack_into("<H", new_, 0x16, st["cur_psp"])
                struct.pack_into("<HHH", new_, 0x32, 20, 0x18, dx)
                st["cur_psp"] = dx
            mu.mem_write(dx * 16, bytes(new_))
            if ah == 0x55: W(UC_X86_REG_AX, (ah << 8) | 0xF0)
            return
        if ah == 0x0D or ah == 0x68: return
        if ah == 0x87: W(UC_X86_REG_AX, ax & 0xFF00); return        # no such function on MS-DOS (a C run-time asks: XENIX answers it)
        if ah == 0x4B:                                           # EXEC: the child runs to its end in an emulator of its own
            name = cstr(ds, dx); hp = hostpath(name)
            if not fsisfile(hp): carry(True, 2); return
            if al == 3:                                          # load an overlay: the image (relocated) at a segment, not run
                lseg, rfac = struct.unpack("<HH", bytes(mu.mem_read(lin(es, bx), 4)))
                if fs: ob = fs.read(hp)
                else:
                    with ropen(hp, "rb") as f_: ob = f_.read()
                if ob[:2] in (b"MZ", b"ZM"):
                    (lp_, pg_, nr_, hp_) = struct.unpack_from("<4H", ob, 2); ro_ = struct.unpack_from("<H", ob, 0x18)[0]
                    oimg = bytearray(ob[hp_ * 16:pg_ * 512 - ((512 - lp_) if lp_ else 0)])
                    for k in range(nr_):
                        o_, s_ = struct.unpack_from("<HH", ob, ro_ + 4 * k); a_ = s_ * 16 + o_
                        struct.pack_into("<H", oimg, a_, (struct.unpack_from("<H", oimg, a_)[0] + rfac) & 0xFFFF)
                else: oimg = ob
                mu.mem_write(lseg * 16, bytes(oimg))
                if os.environ.get("DOSRUN_DEBUG"): print("overlay %s at %04x reloc %04x: %d bytes" % (name, lseg, rfac, len(oimg)), file=sys.stderr)
                carry(False); return
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
            mu.mem_write(PSP * 16 + 0x48, bytes(2)); Wseg("ds", PSP); W(UC_X86_REG_SI, 0x48); carry(False); return
        raise RuntimeError("INT 21h AH=%02X not implemented (AX=%04X)" % (ah, ax))

    def intr(uc, intno, _):
        if host is not None and host.handle(intno): return  # DPMI: INT 2Fh/1687h, INT 31h, the switch
        dos_int(intno)
    def dos_int(intno):                                     # the executive's own service of an interrupt
        uc = mu
        if intno == 0x21: int21()
        elif intno == 0x20: st["exit"] = 0; raise DosExit()
        elif intno in (0x10, 0x1A, 0x12, 0x11, 0x16, 0x2F): pass
        elif intno == 0x15:                                  # BIOS system services: no extended memory by INT 15h
            if os.environ.get("DOSRUN_DEBUG"): print("int15 ax=%04x" % R(UC_X86_REG_AX), file=sys.stderr)
            if R(UC_X86_REG_AX) >> 8 == 0x88: W(UC_X86_REG_AX, 0); carry(False)   # (what a DPMI host - Windows - says)
            else: W(UC_X86_REG_AX, (0x86 << 8) | (R(UC_X86_REG_AX) & 0xFF)); carry(True)
        elif st["vec"].get(intno, (0, 0)) != (0, 0):         # a handler the program installed itself (an 8087 emulator's INT 34h-3Dh):
            sp_ = (R(UC_X86_REG_SP) - 6) & 0xFFFF            # do what the processor does, and go on there
            mu.mem_write(R(UC_X86_REG_SS) * 16 + sp_, struct.pack("<HHH", R(UC_X86_REG_IP), R(UC_X86_REG_CS), R(UC_X86_REG_EFLAGS) & 0xFFFF))
            W(UC_X86_REG_SP, sp_); s_, o_ = st["vec"][intno]
            mu.reg_write(UC_X86_REG_EFLAGS, R(UC_X86_REG_EFLAGS) & ~0x300)
            W(UC_X86_REG_CS, s_); W(UC_X86_REG_IP, o_); st["again"] = True; uc.emu_stop()
        else:
            at = pc()
            raise RuntimeError("INT %02Xh not implemented (at %04x:%08x = %x: %s)" % (
                intno, R(UC_X86_REG_CS), R(UC_X86_REG_EIP), at, bytes(mu.mem_read(at, 12)).hex()))
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
    def pc():
        # the linear address the CPU is at (for messages): real mode CS*16 + IP; protected mode (CR0.PE -
        # the CPU's own mode, which a host's switch code may not have reached yet) the selector's base + EIP
        if host is not None and R(UC_X86_REG_CR0) & 1:
            d = st["desc"].get(R(UC_X86_REG_CS) & 0xFFFC)
            return (d[0] if d else 0) + R(UC_X86_REG_EIP)
        return R(UC_X86_REG_CS) * 16 + R(UC_X86_REG_IP)
    fast = fast and not instrument and not instrument_when and not trace_calls and not driver
    host = None
    if dpmi:
        if dpmi is True:
            from codebreaker.exact.emu import dpmi as _dpmi
            host = _dpmi.Host()
        else:
            host = dpmi
        class _Ctx(object):
            pass
        ctx = _Ctx()
        ctx.mu, ctx.st, ctx.int21, ctx.R, ctx.W, ctx.carry = mu, st, int21, R, W, carry
        ctx.alloc, ctx.largest, ctx.load, ctx.psp = alloc, largest, load, PSP
        ctx.dos_int = dos_int
        ctx.free = lambda seg: _mcb_free(mu, first_mcb, seg)
        ctx.resize = lambda seg, paras: _mcb_resize(mu, first_mcb, seg, paras)
        ctx.ArenaError = ArenaError
        host.attach(ctx)
    _until = 0x1000000 if host is not None else 0x10FFFF   # protected-mode code/tables live above 1 MB
    mu.hook_add(UC_HOOK_INTR, intr_safe)
    if not fast: mu.hook_add(UC_HOOK_BLOCK, block)
    lim = {"timeout": int(FAST_LIMIT * 1e6)} if fast else {}
    watchdog = None
    if fast and host is not None:
        # A DOS extender restarts the CPU at every raw mode switch (hundreds a run), and Unicorn's own
        # timeout costs a timer thread per emu_start (~24 ms each on Windows: most of such a run). So
        # one watchdog thread stops the whole run at the limit instead (emu_stop is thread-safe).
        import threading
        limit = max(FAST_LIMIT, float(os.environ.get("DOSRUN_DPMI_LIMIT", "300")))
        def _expire():
            st["timedout"] = True; mu.emu_stop()
        watchdog = threading.Timer(limit, _expire); watchdog.daemon = True
        lim = {}
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
        if watchdog is not None: watchdog.start()
        mu.emu_start((cs + load) * 16 + ip, _until, **lim)
        while st.get("again") and st["exit"] is None and not err and not st.get("timedout"):
            # (Unicorn's 16-bit mode takes a start address as CS*16 + IP and sets IP = start - CS*16 in
            #  either CPU mode, so this resumes at CS:IP in protected mode too - IP below 64 KB)
            st["again"] = False; mu.emu_start(R(UC_X86_REG_CS) * 16 + R(UC_X86_REG_IP), _until, **lim)
        if fast and st["exit"] is None and not err and (st.get("timedout") or watchdog is None and time.time() - t_start >= FAST_LIMIT * 0.9):
            err.append(RuntimeError("time limit at %05x%s; console so far: %r" % (pc() - load * 16, (" (cr0 %x cs:eip %04x:%08x)" % (R(UC_X86_REG_CR0), R(UC_X86_REG_CS), R(UC_X86_REG_EIP))) if host is not None else "", bytes(st["console"])[-400:])))
        if st.get("resume") and st["exit"] is None and not err:
            st["resume"] = False; instrument(mu, load)
            mu.emu_start(R(UC_X86_REG_CS) * 16 + R(UC_X86_REG_IP), 0x10FFFF)
    except StopIteration: pass
    except UcError as e:
        err.append(RuntimeError("cpu fault %s at %05x" % (e, pc())))
    finally:
        if watchdog is not None: watchdog.cancel()
    for e in files.values():
        if e["f"]: e["f"].close()
    if err and st["exit"] is None: raise err[0]
    return Result(st["exit"], bytes(st["console"]), st["blocks"], load, None, st["n"], st["calls"])

if __name__ == "__main__":
    r = run(sys.argv[1], sys.argv[2], sys.argv[3] if len(sys.argv) > 3 else "")
    sys.stdout.write(r.console.decode("latin1"))
    print("\n[exit %s, %d basic blocks executed, %d distinct]" % (r.exit, r.steps, len(r.blocks)))
