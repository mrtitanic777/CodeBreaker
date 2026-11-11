"""exact/emu/dpmi.py: the DPMI host's descriptor table limits (the C++ twin is native/cb/exact/emu/dpmi.cpp)."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from unicorn.x86_const import UC_X86_REG_AX, UC_X86_REG_BX, UC_X86_REG_CX, UC_X86_REG_EFLAGS    # noqa: E402

from codebreaker.exact.emu import dpmi    # noqa: E402


class _Engine(object):
    """just enough of a Unicorn engine for the host's table work: registers, and memory as a dict of bytes"""

    def __init__(self):
        self.regs = {}
        self.mem = {}

    def reg_read(self, r):
        return self.regs.get(r, 0)

    def reg_write(self, r, v):
        self.regs[r] = v

    def mem_map(self, *_a):
        pass

    def mem_write(self, addr, data):
        for i, b in enumerate(data):
            self.mem[addr + i] = b

    def mem_read(self, addr, n):
        return bytes(self.mem.get(addr + i, 0) for i in range(n))


def _host():
    mu = _Engine()
    ctx = type("Ctx", (), {})()
    ctx.mu, ctx.st = mu, {}
    ctx.R = lambda r: mu.reg_read(r)
    ctx.W = lambda r, v: mu.reg_write(r, v & 0xFFFF)

    def carry(on, ax=None):
        f = mu.reg_read(UC_X86_REG_EFLAGS)
        mu.reg_write(UC_X86_REG_EFLAGS, (f | 1) if on else (f & ~1))
    ctx.carry = carry
    host = dpmi.Host()
    host.attach(ctx)
    return host, mu


def test_more_descriptors_than_the_ldt_holds_is_an_error_not_an_overrun():
    """INT 31h 0000 allocated without looking at the table's 512 entries, writing descriptors into the memory after it."""
    host, mu = _host()
    mu.reg_write(UC_X86_REG_AX, 0x0000)
    mu.reg_write(UC_X86_REG_CX, dpmi.LDT_COUNT)          # more than are left after the first 16
    host._int31()
    assert mu.reg_read(UC_X86_REG_EFLAGS) & 1
    assert mu.reg_read(UC_X86_REG_AX) == 0x8011
    assert host.next_ldt == dpmi.LDT_FIRST              # (nothing was taken)


def test_set_descriptor_of_a_gdt_selector_is_refused():
    """INT 31h 000C wrote the descriptor into the LDT at the GDT selector's index."""
    host, mu = _host()
    mu.reg_write(UC_X86_REG_AX, 0x000C)
    mu.reg_write(UC_X86_REG_BX, dpmi.SEL_STUBCODE)
    host._int31()
    assert mu.reg_read(UC_X86_REG_EFLAGS) & 1
    assert mu.reg_read(UC_X86_REG_AX) == 0x8022
