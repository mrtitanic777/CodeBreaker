#!/usr/bin/env python3
"""
pe_trace.py — the 32-bit (PE) function carve: `trace.Seg` for the PE world.

`trace.Seg` carves 16-bit NE segments via objdump; this does the same job for
the target's PE modules (e.g. KERNEL32/USER32/GDI32) using
**capstone** (32-bit x86). It gives the PE side what the NE side already has:
function starts, per-function extents, and *resolved* calls — so a reconstruction
can cite `0xADDR` and be checked, and the ledger can one day credit PE bytes.

Function starts come from three sources, most-authoritative first:
  1. the export table (RVAs the binary itself declares) — ground truth;
  2. direct `call <imm>` targets seen in a linear sweep — real entry points;
  3. standard prologues: `push ebp; mov ebp,esp` (55 8B EC) and the MS hot-patch
     `mov edi,edi; push ebp; mov ebp,esp` (8B FF 55 8B EC).
Extents run start-to-next-start (as in trace.Seg).

Calls resolve via `pe.imports()` (a `call [IAT_slot]` -> `DLL!func`) and the
export/known-start set (a `call <imm>` -> a named export or a local `sub_VA`).
That is what makes the carve checkable rather than a raw listing.

Doctrine: this is *evidence extraction*, not understanding. It says where the
functions are and what they call; a human still reads them. Capstone is a
modern decoder — cross-check anything surprising against objdump/Ghidra.

    python pe_trace.py USER32.DLL --list            # functions + call counts
    python pe_trace.py USER32.DLL --func 0xbff9xxxx # one function, resolved
    python pe_trace.py KERNEL32.DLL --stats
"""
import re
from engine.foundation import cores
import sys

import capstone

target = cores.get('target')
from engine.format_loaders.pe import open_pe


class PeCarve:
    def __init__(self, path, progress_cb=None):
        # progress_cb(bytes_swept) is called periodically during the (dominant)
        # linear capstone sweep, so a caller can drive a disassembly progress bar.
        self._progress_cb = progress_cb
        self.pe = open_pe(target.resolve(path))
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        self.md.detail = True
        self.sections = self.pe.code_sections()          # [(va, bytes, name)]
        self.imports = self.pe.imports()                 # {iat_va: (dll, name)}
        self.exports = {}                                 # export_va -> name
        for o, n, rva in self.pe.exports():
            self.exports[self.pe.image_base + rva] = n or ("Ord%d" % o)
        self._sweep()
        self.starts = self._find_starts()
        self.funcs = self._carve()

    # -- section helpers --------------------------------------------------
    def _sec_of(self, va):
        for base, data, _name in self.sections:
            if base <= va < base + len(data):
                return base, data
        return None, None

    def _sweep(self):
        """One linear capstone pass per code section. Collects every decoded
        instruction (va -> (mnem, op_str, size, call_target)) and the set of
        direct-call targets. Linear sweep drifts on embedded data — same caveat
        as trace.Seg — so starts are corroborated by exports/prologues too."""
        self.insn = {}
        self.call_targets = set()
        cb = self._progress_cb
        swept = 0                                 # bytes swept across prior sections
        cnt = 0
        for base, data, _name in self.sections:
            n = len(data)
            off = 0
            while off < n:
                made = 0
                for ins in self.md.disasm(data[off:], base + off):
                    tgt = None
                    if ins.mnemonic == "call" and ins.operands:
                        op = ins.operands[0]
                        if op.type == capstone.x86.X86_OP_IMM:
                            tgt = op.imm
                            self.call_targets.add(tgt)
                    self.insn[ins.address] = (ins.mnemonic, ins.op_str, ins.size, ins, tgt)
                    off = ins.address - base + ins.size
                    made += 1
                    cnt += 1
                    if cb and cnt % 2000 == 0:     # report every ~2000 instructions
                        cb(swept + off)
                if made == 0:
                    # capstone hit a byte it can't decode (data-in-code) and
                    # stopped; skip it and resync, or the rest of the section —
                    # and every function in it — is lost (oleaut32 carved 0).
                    off += 1
            swept += n
        if cb:
            cb(swept)

    def _find_starts(self):
        starts = set(self.exports)
        # entry point
        if self.pe.entry:
            starts.add(self.pe.image_base + self.pe.entry)
        # direct-call targets that land in a code section
        for t in self.call_targets:
            if self._sec_of(t)[0] is not None:
                starts.add(t)
        # prologue scan
        for base, data, _name in self.sections:
            n = len(data)
            for i in range(n - 4):
                b = data[i:i + 5]
                if b[:3] == b"\x55\x8b\xec":                     # push ebp; mov ebp,esp
                    starts.add(base + i)
                elif b[:5] == b"\x8b\xff\x55\x8b\xec":           # mov edi,edi; push ebp; mov ebp,esp
                    starts.add(base + i)
        # keep only addresses we actually decoded an instruction at
        return {s for s in starts if s in self.insn}

    def _carve(self):
        order = sorted(self.starts)
        out = {}
        for i, s in enumerate(order):
            # end = next start in the SAME section, else section end
            base, data = self._sec_of(s)
            secend = base + len(data)
            end = secend
            if i + 1 < len(order) and order[i + 1] < secend:
                end = order[i + 1]
            out[s] = {"start": s, "end": end, "name": self.exports.get(s)}
        return out

    # -- queries ----------------------------------------------------------
    def resolve_call(self, ins):
        """A capstone call/jmp insn -> a name, or None if not a call/branch."""
        if ins.mnemonic not in ("call", "jmp") or not ins.operands:
            return None
        op = ins.operands[0]
        if op.type == capstone.x86.X86_OP_IMM:
            t = op.imm
            if t in self.exports:
                return self.exports[t]
            if t in self.funcs or self._sec_of(t)[0] is not None:
                return "sub_%x" % t
            return None
        if op.type == capstone.x86.X86_OP_MEM:
            m = op.mem
            # absolute [disp] with no base/index = an IAT slot (or a global /
            # loader-connected thunk pointer). capstone gives disp SIGNED, so
            # mask to 32-bit before matching the IAT map. A hit is a named
            # import; a miss is an internal indirect (e.g. the per-module
            # QT_Thunk pointer) and honestly stays unresolved.
            if m.base == 0 and m.index == 0:
                imp = self.imports.get(m.disp & 0xFFFFFFFF)
                if imp:
                    return "%s!%s" % imp
                return None
        return None

    def calls_in(self, start):
        """Resolved call/jmp targets inside the function carved at `start`."""
        f = self.funcs.get(start)
        if not f:
            return []
        out = []
        va = start
        while va < f["end"]:
            rec = self.insn.get(va)
            if rec is None:
                va += 1
                continue
            _mn, _op, size, ins, _tgt = rec
            nm = self.resolve_call(ins)
            if nm:
                out.append(nm)
            va += size
        return out

    def disasm_func(self, start):
        f = self.funcs.get(start)
        if not f:
            return []
        rows, va = [], start
        while va < f["end"]:
            rec = self.insn.get(va)
            if rec is None:
                va += 1
                continue
            mn, op, size, _ins, _t = rec
            rows.append((va, mn, op))
            va += size
        return rows


def main():
    argv = sys.argv[1:]
    if not argv:
        sys.exit(__doc__)
    mod = argv[0]
    c = PeCarve(mod)

    if "--stats" in argv:
        named = sum(1 for f in c.funcs.values() if f["name"])
        print(f"{mod}: {len(c.funcs)} functions carved "
              f"({named} = exported, {len(c.funcs) - named} internal), "
              f"{len(c.exports)} exports, {len(c.imports)} import slots, "
              f"{len(c.call_targets)} distinct call targets")
        return
    if "--func" in argv:
        va = int(argv[argv.index("--func") + 1], 16)
        for a, mn, op in c.disasm_func(va):
            print(f"  {a:#010x}: {mn:<7} {op}")
        calls = c.calls_in(va)
        print("  calls:", ", ".join(sorted(set(calls))) or "(none)")
        return
    # default: --list
    rows = sorted(c.funcs.values(), key=lambda f: -(f["end"] - f["start"]))
    print(f"# {mod}: {len(c.funcs)} functions (largest first)")
    for f in rows[:40]:
        nm = f["name"] or ("sub_%x" % f["start"])
        calls = c.calls_in(f["start"])
        print(f"  {f['start']:#010x}  {f['end'] - f['start']:6d} B  "
              f"{nm:<28} calls={len(calls)}")


if __name__ == "__main__":
    main()
