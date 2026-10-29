"""pcode.py -- an instruction's p-code, from the decompiler's own SLEIGH specification.

    lifter = pcode.lifter("x86-16-real")       # None when the lifting program is not there
    ins = lifter.lift(code_bytes, offset)      # Lifted: .length, .ops, ...; None if it does not decode

The judge's path explorer (verify/concolic.py through emulate/shadow.py) follows what an
instruction does to the values made from a function's inputs. What an instruction does is
already written down once, and checked against the processor instruction by instruction
(docs/SLEIGH_VERIFY.md): the specification the decompiler lifts with. So the explorer asks for
the same p-code -- native/decompiler/pcodecheck.cc in its lift mode, one long-running process
per Python process, every instruction lifted once (kept by its bytes and offset).

An op is (OPCODE, out, inputs): a varnode is (space, offset, size) with space "r" register,
"u" unique, "c" constant, "m" ram; out is None for none; a user operation's first input is its
name (a str, "segment"). A 16-bit instruction is lifted at its offset in its segment: a near
jump's target is an offset there too (the lift mode puts each in a 64K window of its own, the
window dropped here).

The program is found as CB_PCODECHECK names it, else beside the decompiler (the setting
decompiler_bin); one built before the lift mode existed is not used (lifter() is None)."""
import os
import subprocess
import threading


class Lifted(object):
    """One instruction's p-code, and what the shadow needs to know of it without running it:
    the register bytes it reads and writes, whether it reaches memory, a user operation or
    a branch (anything that must be followed op by op)."""
    __slots__ = ("length", "ops", "reads", "writes", "full")

    def __init__(self, length, ops):
        self.length, self.ops = length, ops
        reads, writes, full = set(), set(), False
        for opc, out, ins in ops:
            if opc in ("LOAD", "STORE", "CALLOTHER", "CBRANCH", "BRANCHIND", "CALLIND", "RETURN"):
                full = True
            for v in ins:
                if isinstance(v, tuple):
                    if v[0] == "r":
                        reads.update(range(v[1], v[1] + v[2]))
                    elif v[0] == "m":
                        full = True
            if out is not None:
                if out[0] == "r":
                    writes.update(range(out[1], out[1] + out[2]))
                elif out[0] == "m":
                    full = True
        self.reads, self.writes, self.full = frozenset(reads), frozenset(writes), full


def _varnode(text, window):
    if text == "-":
        return None
    if text.startswith("@"):
        return text[1:]
    space, off, size = text.split(":")
    off = int(off, 16)
    if space == "m" and window:
        off &= 0xFFFF
    return (space, off, int(size))


class Lifter(object):
    def __init__(self, exe, sla, mode):
        self.mode = mode
        self.window = mode == "x86-16-real"
        self.p = subprocess.Popen([exe, sla, mode + "-lift"], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                  stderr=subprocess.DEVNULL, text=True, bufsize=1)
        head = self.p.stdout.readline().split()
        if not head or head[0] != "regs":
            self.p.kill()
            raise OSError("not a lifting pcodecheck")
        self.regs = {}                                       # name -> (offset, size)
        for item in head[1:]:
            name, off, size = item.split(":")
            self.regs[name] = (int(off, 16), int(size))
        self.memo = {}
        self.lock = threading.Lock()

    def lift(self, code, offset):
        """The p-code of the instruction at the start of `code` (bytes; 16 are enough for x86),
        at `offset`. -> Lifted, or None when it does not decode."""
        key = (bytes(code), offset)
        got = self.memo.get(key, False)
        if got is not False:
            return got
        with self.lock:
            self.p.stdin.write("%s %x\n" % (key[0].hex(), offset))
            self.p.stdin.flush()
            line = self.p.stdout.readline().rstrip("\n")
        out = None
        if line and not line.startswith("err"):
            length, _, body = line.partition(" ")
            ops = []
            for text in body.split(";") if body else ():
                parts = text.split(" ")
                ops.append((parts[0], _varnode(parts[1], self.window),
                            tuple(_varnode(p, self.window) for p in parts[2:])))
            out = Lifted(int(length), tuple(ops))
        self.memo[key] = out
        return out


_LIFTERS = {}


def _exe():
    exe = os.environ.get("CB_PCODECHECK")
    if exe:
        return exe
    from codebreaker.base import settings
    return os.path.join(settings.get("decompiler_bin") or "", "pcodecheck.exe")


def lifter(mode="x86-16-real"):
    """The process's lifter for `mode`, started on first use; None when there is none to start
    (no program, or one without the lift mode)."""
    if mode not in _LIFTERS:
        from codebreaker.decompile import decompiler
        exe = _exe()
        sla = os.path.join(decompiler._languages(), "x86.sla")
        try:
            _LIFTERS[mode] = Lifter(exe, sla, mode) if os.path.exists(exe) and os.path.exists(sla) else None
        except (OSError, ValueError):
            _LIFTERS[mode] = None
    return _LIFTERS[mode]


def stamp():
    """What the lifted p-code depends on (for keys of anything kept from it): the program and the
    specification (base/cache.py tool)."""
    from codebreaker.base import cache
    from codebreaker.decompile import decompiler
    return cache.tool(_exe()), cache.tool(os.path.join(decompiler._languages(), "x86.sla"))
