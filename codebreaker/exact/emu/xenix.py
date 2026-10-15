#!/usr/bin/env python3
"""xenixrun.py - run a program of XENIX for the 8086 (Altos ACS 8600, 1982-83) under emulation.

  python xenixrun.py ROOT WORKDIR PROGRAM [ARG ..]

Microsoft's first C linker and librarian for MS-DOS (LINK 2.40, LIB 2.40, 1983-84) were not
compiled on MS-DOS: their code is that of the C compiler of XENIX for the 8086 - the Portable C
Compiler line: /lib/cpp, /lib/c0, /lib/c1, /lib/c2, /bin/as, /bin/ld. A XENIX system with that
compiler survives for the Altos ACS 8600 (Bitsavers; binaries/altos-8600-xenix, git-ignored).
This runs one of its programs, the way dosrun.py runs an MS-DOS one and isis.py an ISIS-II one:
enough of the machine and of the system to compile a file.

The program file is a Version 7 a.out for the 8086: 16 bytes of header (magic 0109h: text and data
in segments of their own; 0107h: in one), text, data. A system call is `out 8800h, ax` - the
Altos traps the port - with the call's number in AX and its arguments in BX, CX, SI, DI; the
result comes back in AX (and BX), with the carry flag set and an error number in AX if it failed.
The numbers are those of Version 7 UNIX.

ROOT is the directory holding the XENIX files (lib/c0 ...); a path the program names is looked
for in WORKDIR first (so /tmp and the files being compiled live there), then in ROOT.
"""
import os, sys, struct, time
from unicorn import Uc, UC_ARCH_X86, UC_MODE_16, UC_HOOK_INSN, UcError
from unicorn.x86_const import *

CS, DS = 0x1000, 0x3000
ENOENT, EBADF, ENOTTY, EINVAL = 2, 9, 25, 22

class Result:
    def __init__(s): s.console = b""; s.status = None; s.calls = []; s.error = None

def run(root, work, prog, args, stdin=b"", trace=False, limit=400_000_000, hooks=None):
    """-> Result (console output of fd 1 and 2, exit status)"""
    def host(path, create=False):
        p = path.decode("latin1") if isinstance(path, bytes) else path
        rel = p.lstrip("/").replace("..", "_")
        w = os.path.join(work, *rel.split("/")) if rel else work
        if create or os.path.exists(w): return w
        r = os.path.join(root, *rel.split("/")) if rel else root
        return r if os.path.exists(r) else w
    b = open(host(prog), "rb").read()
    magic, tsz, dsz, bsz, ssz, entry, _, flag = struct.unpack_from("<8H", b, 0)
    if magic not in (0x0107, 0x0108, 0x0109): raise SystemExit("%s is not an a.out file (%04x)" % (prog, magic))
    text = b[16:16 + tsz]; data = b[16 + tsz:16 + tsz + dsz]
    mu = Uc(UC_ARCH_X86, UC_MODE_16); mu.mem_map(0, 0x100000)
    split = magic == 0x0109; cs = CS; ds = DS if split else CS
    mu.mem_write(cs * 16, text); mu.mem_write(ds * 16 + (0 if split else tsz), data)
    brk = [(dsz + bsz) if split else (tsz + dsz + bsz)]
    # the stack: argc, argv[], 0, envp[], 0, the strings - at the top of the data segment
    argv = [prog.encode()] + [a.encode() if isinstance(a, str) else a for a in args]
    sp = 0xFF00; strs = b""; ptrs = []
    for a in argv: ptrs.append(len(strs)); strs += a + b"\0"
    sbase = sp - len(strs) - (len(strs) & 1); mu.mem_write(ds * 16 + sbase, strs)
    vec = struct.pack("<H", len(argv)) + b"".join(struct.pack("<H", sbase + p) for p in ptrs) + struct.pack("<HH", 0, 0)
    sp = sbase - len(vec); sp -= sp & 1; mu.mem_write(ds * 16 + sp, vec)
    for r, v in ((UC_X86_REG_CS, cs), (UC_X86_REG_DS, ds), (UC_X86_REG_ES, ds), (UC_X86_REG_SS, ds), (UC_X86_REG_SP, sp), (UC_X86_REG_BP, 0)): mu.reg_write(r, v)
    res = Result(); fds = {0: ["in", 0], 1: ["out"], 2: ["out"]}
    R = lambda r: mu.reg_read(r)
    def cstr(off):
        s = b""
        while True:
            c = bytes(mu.mem_read(ds * 16 + off + len(s), 64)); k = c.find(b"\0")
            if k >= 0: return s + c[:k]
            s += c
    def ret(ax=0, bx=None, err=None):
        fl = R(UC_X86_REG_EFLAGS)
        if err is not None: mu.reg_write(UC_X86_REG_AX, err); mu.reg_write(UC_X86_REG_EFLAGS, fl | 1)
        else:
            mu.reg_write(UC_X86_REG_AX, ax & 0xFFFF); mu.reg_write(UC_X86_REG_EFLAGS, fl & ~1)
            if bx is not None: mu.reg_write(UC_X86_REG_BX, bx & 0xFFFF)
    def newfd(f):
        n = next(k for k in range(3, 64) if k not in fds); fds[n] = ["file", f]; return n
    def stat_buf(p):
        st = os.stat(p); mode = (0o040000 if os.path.isdir(p) else 0o100000) | 0o644
        # V7 struct stat: dev, ino, mode, nlink, uid, gid, rdev (7 words), size (long), atime, mtime, ctime (longs)
        long_ = lambda v: struct.pack("<HH", (v >> 16) & 0xFFFF, v & 0xFFFF)
        return struct.pack("<7H", 1, 2, mode, 1, 0, 0, 0) + long_(st.st_size) + long_(int(st.st_mtime)) * 3
    def syscall(uc, port, size, value, _):
        if port != 0x8800: return
        n = value & 0xFFFF; bx, cx, si, di = R(UC_X86_REG_BX), R(UC_X86_REG_CX), R(UC_X86_REG_SI), R(UC_X86_REG_DI)
        if trace: res.calls.append((n, bx, cx, si, di))
        if n == 1: res.status = bx; uc.emu_stop(); return
        if n == 3:                                                  # read(fd, buf, count)
            f = fds.get(bx)
            if not f: return ret(err=EBADF)
            if f[0] == "in": d = stdin[f[1]:f[1] + si]; f[1] += len(d)
            elif f[0] == "file": d = f[1].read(si)
            else: return ret(err=EBADF)
            mu.mem_write(ds * 16 + cx, d); return ret(len(d))
        if n == 4:                                                  # write(fd, buf, count)
            f = fds.get(bx); d = bytes(mu.mem_read(ds * 16 + cx, si))
            if not f: return ret(err=EBADF)
            if f[0] == "out": res.console += d
            elif f[0] == "file": f[1].write(d)
            return ret(len(d))
        if n == 5:                                                  # open(name, mode)
            p = host(cstr(bx))
            if not os.path.isfile(p): return ret(err=ENOENT)
            return ret(newfd(open(p, "rb" if cx == 0 else "r+b")))
        if n == 8:                                                  # creat(name, mode)
            p = host(cstr(bx), create=True); os.makedirs(os.path.dirname(p), exist_ok=True)
            return ret(newfd(open(p, "w+b")))
        if n == 6:                                                  # close(fd)
            f = fds.pop(bx, None)
            if f and f[0] == "file": f[1].close()
            return ret(0) if f else ret(err=EBADF)
        if n == 10:                                                 # unlink(name)
            p = host(cstr(bx))
            if os.path.isfile(p) and os.path.abspath(p).startswith(os.path.abspath(work)): os.remove(p); return ret(0)
            return ret(0) if os.path.exists(p) else ret(err=ENOENT)
        if n == 19:                                                 # lseek(fd, long offset (high, low), whence)
            f = fds.get(bx)
            if not f or f[0] != "file": return ret(err=EBADF)
            off = (cx << 16 | si); off -= (off & 0x80000000) << 1
            f[1].seek(off, di); pos = f[1].tell(); return ret(pos & 0xFFFF, bx=pos >> 16)
        if n == 17:                                                 # break(address)
            brk[0] = bx; return ret(0)
        if n in (18, 28):                                           # stat(name, buf) / fstat(fd, buf)
            if n == 18:
                p = host(cstr(bx))
                if not os.path.exists(p): return ret(err=ENOENT)
            else:
                f = fds.get(bx)
                if not f: return ret(err=EBADF)
                if f[0] != "file": mu.mem_write(ds * 16 + cx, struct.pack("<7H", 1, 1, 0o020622, 1, 0, 0, 0) + bytes(16)); return ret(0)
                p = f[1].name
            mu.mem_write(ds * 16 + cx, stat_buf(p)); return ret(0)
        if n == 33: return ret(0) if os.path.exists(host(cstr(bx))) else ret(err=ENOENT)      # access
        if n == 20: return ret(86)                                  # getpid
        if n == 13: t = int(time.time()); return ret(t >> 16, bx=t & 0xFFFF)                   # time
        if n == 48: return ret(0)                                   # signal
        if n == 54: return ret(err=ENOTTY)                          # ioctl: nothing is a terminal
        if n in (24, 47): return ret(0)                             # getuid, getgid
        if n == 60: return ret(0o22)                                # umask
        if n == 15: return ret(0)                                   # chmod
        res.error = "system call %d (bx=%04x cx=%04x si=%04x di=%04x) is not provided" % (n, bx, cx, si, di); uc.emu_stop()
    mu.hook_add(UC_HOOK_INSN, syscall, None, 1, 0, UC_X86_INS_OUT)
    if text[:4] == b"\xeb\x7e\xeb\xfe":
        # XENIX 2.3's other way in: the program's first bytes are a jump over a row of `jmp .`,
        # and a system call is a far call to the one at 0002 (the number in AX as before); a far
        # call to the one at 0004 asks for the stack to be grown down to BX. The system catches
        # both there; so does this, and returns as the far call expects.
        from unicorn import UC_HOOK_CODE
        def gate(uc, addr, size, _):
            which = addr - cs * 16
            if which == 2: syscall(uc, 0x8800, 2, R(UC_X86_REG_AX), None)
            else: mu.reg_write(UC_X86_REG_AX, R(UC_X86_REG_BX)); mu.reg_write(UC_X86_REG_EFLAGS, R(UC_X86_REG_EFLAGS) & ~1)
            if res.status is not None or res.error: return
            sp_ = R(UC_X86_REG_SP); ip, seg = struct.unpack("<HH", bytes(mu.mem_read(ds * 16 + sp_, 4)))
            mu.reg_write(UC_X86_REG_SP, (sp_ + 4) & 0xFFFF); mu.reg_write(UC_X86_REG_CS, seg); mu.reg_write(UC_X86_REG_IP, ip)
        mu.hook_add(UC_HOOK_CODE, gate, None, cs * 16 + 2, cs * 16 + 5)
    if hooks:
        # (to watch a program at work: {an address of its text: a function called, with the machine and the two
        # segments, each time the instruction there is about to be done})
        from unicorn import UC_HOOK_CODE
        for at, fn in hooks.items(): mu.hook_add(UC_HOOK_CODE, (lambda f: lambda uc, addr, size, _: f(uc, cs, ds))(fn), None, cs * 16 + at, cs * 16 + at)
    try: mu.emu_start(cs * 16 + entry, 0xFFFFF, count=limit)
    except UcError as e:
        res.error = "%s at %04x:%04x" % (e, R(UC_X86_REG_CS), R(UC_X86_REG_IP))
    for f in fds.values():
        if f[0] == "file": f[1].close()
    if res.status is None and res.error is None: res.error = "did not finish (instruction limit, or it stopped at %04x)" % R(UC_X86_REG_IP)
    return res

def main():
    a = [x for x in sys.argv[1:] if x != "--trace"]
    r = run(a[0], a[1], a[2], a[3:], trace="--trace" in sys.argv)
    sys.stdout.write(r.console.decode("latin1"))
    for c in r.calls[-40:]: print("  call %d bx=%04x cx=%04x si=%04x di=%04x" % c)
    print("[exit status %s%s]" % (r.status, "; " + r.error if r.error else ""))

if __name__ == "__main__":
    main()
