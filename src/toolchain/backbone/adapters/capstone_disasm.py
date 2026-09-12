#!/usr/bin/env python3
r"""Capstone disassembler adapters -- x86 16-bit and 32-bit, behind the backbone
Disassembler interface. The inverse engine already trusts capstone (it caught hidden
functions Ghidra missed); this exposes it as a swappable role so a Ghidra or an
objdump adapter can register beside it and be selected by name or by detect(arch=...).
"""
from src.toolchain.backbone.base import Disassembler, Insn

try:
    from capstone import Cs, CS_ARCH_X86, CS_MODE_16, CS_MODE_32
    _HAVE = True
except Exception:
    _HAVE = False


class _CapstoneX86(Disassembler):
    _mode = None
    _bits = None

    def __init__(self):
        self._md = Cs(CS_ARCH_X86, self._mode) if _HAVE else None

    def available(self) -> bool:
        return _HAVE

    def score(self, **h):
        if h.get("arch") not in (None, "x86"):
            return -1
        return 3 if h.get("bits") == self._bits else 0

    def disasm(self, code: bytes, *, base=0, **kw):
        if not _HAVE:
            return []
        return [Insn(i.address, i.size, i.bytes, i.mnemonic, i.op_str)
                for i in self._md.disasm(bytes(code), base)]


class CapstoneX86_16(_CapstoneX86):
    name = "capstone-x86-16"
    version = "capstone x86 (16-bit real mode)"
    _mode = CS_MODE_16 if _HAVE else None
    _bits = 16


class CapstoneX86_32(_CapstoneX86):
    name = "capstone-x86-32"
    version = "capstone x86 (32-bit protected mode)"
    _mode = CS_MODE_32 if _HAVE else None
    _bits = 32


def register(backbone):
    backbone.register(CapstoneX86_16(), default=True)   # 16-bit is the Win95 DLL target
    backbone.register(CapstoneX86_32())
