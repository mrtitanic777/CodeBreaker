#!/usr/bin/env python3
r"""tracer.py -- run one win95 function in Unicorn and record a structured TraceRecord.

This is the behavioral engine's observation primitive. It reuses the SAME entry-state
setup that recompile95/tools/difftest.py proved correct (map the code section page-aligned,
a scratch window, a stack with stdcall args under a RET sentinel, run to the sentinel,
read EAX), but instead of only checking the final EAX it installs hooks and records
*what happened*: the instruction path, every memory read/write, detected calls, faults,
and the return value. That record is the raw evidence the rest of the engine reasons over.

Two deliberate upgrades over difftest's oracle:
  * LAZY MAPPING. difftest maps only the code section + one scratch window, so a function
    that dereferences a pointer into unmapped memory faults and difftest discards it --
    which is why difftest only covers PURE LEAF functions. Here, an unmapped access maps a
    zeroed page and continues, and the fault itself is recorded as evidence. This lets the
    tracer observe the memory-touching majority (reads return 0; that is a first-approximation
    environment the active-experiment layer will later refine with real inputs).
  * DECODED PATH. each executed instruction is decoded once (one reused capstone engine) so
    the record carries mnemonics and real call/branch targets, not just bare addresses.

No fabrication contract (same as the rest of CodeBreaker): everything in a TraceRecord is
something that was actually observed during the run. A fault, a truncation, or a non-return
exit is recorded as itself, never smoothed over.
"""
import hashlib
import os
import sys

_ENGINE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_ROOT = os.path.dirname(_ENGINE)
if _ROOT not in sys.path:
    sys.path.insert(0, _ROOT)                                # for engine.* imports

from engine.foundation import cores                          # noqa: E402
from engine.format_loaders.pe import open_pe                 # noqa: E402
from engine.format_loaders.pe_seg import PeSeg               # noqa: E402
from engine.format_loaders.pe_trace import PeCarve           # noqa: E402

# Entry-state constants, identical to difftest._emulate so traces are comparable to the
# oracle difftest already trusts.
_SCRATCH_BASE, _SCRATCH_SIZE = 0x200000, 0x10000
_STACK_TOP, _RET = 0x208000, 0xDEAD0000
_PAGE = 0x1000

# Recording caps -- a pathological loop can execute millions of instructions; a trace is
# evidence, not a full replay log, so we bound it and record that we did.
_MAX_INSNS = 20000
_MAX_MEM = 40000
_MAX_PATH = 20000

_MD = None


def _md():
    """One detailed capstone engine, reused across instructions (constructing per-insn is the
    cost that dominates -- same lesson difftest._md() records)."""
    global _MD
    if _MD is None:
        import capstone
        _MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        _MD.detail = True
    return _MD


def _sha256(path):
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def _arities(module):
    """sigrecover.arities(module) lives in recompile95/tools; import it lazily and tolerate
    its absence (a corpus/data-shape dependency, not a hard one for tracing)."""
    tools = os.path.join(_ENGINE, "recompile95", "tools")
    if tools not in sys.path:
        sys.path.insert(0, tools)
    try:
        import sigrecover
        return sigrecover.arities(module) or {}
    except Exception:
        return {}


def list_functions(module):
    """(off, va, size) for each recovered function in the module's first code section,
    smallest offset first. The caller picks one to trace."""
    target = cores.get("target")
    path = target.resolve(module)
    pe = open_pe(path)
    va0, code, secname = pe.code_sections()[0]
    sg = PeSeg(module, section=secname, carve=PeCarve(path))
    out = []
    for off, f in sorted(sg.funcs.items()):
        size = f["end"] - off
        if size >= 8:
            out.append((off, va0 + off, size))
    return out


def _resolve_func(module, func):
    """Map a user-supplied func selector to (off, va0, code, end, secname).

    func may be: None (first function), a section offset or VA as hex ('0x...') or int,
    or a decimal offset. A VA (>= va0) is converted to an offset; an offset is taken as-is.
    """
    target = cores.get("target")
    path = target.resolve(module)
    pe = open_pe(path)
    va0, code, secname = pe.code_sections()[0]
    sg = PeSeg(module, section=secname, carve=PeCarve(path))
    funcs = {off: f for off, f in sg.funcs.items() if (f["end"] - off) >= 8}
    if not funcs:
        raise ValueError("%s: no functions recovered in section %s" % (module, secname))
    if func is None:
        off = min(funcs)
    else:
        val = int(func, 0) if isinstance(func, str) else int(func)
        off = (val - va0) if val >= va0 else val
        if off not in funcs:
            # snap to the enclosing function if the address lands inside one
            holder = [o for o, f in funcs.items() if o <= off < f["end"]]
            if not holder:
                raise ValueError("%s: no function at offset 0x%x (VA 0x%x)"
                                 % (module, off, va0 + off))
            off = holder[0]
    return off, va0, code, funcs[off]["end"], secname


def trace_function(module, func=None, args=None, arity=None, insn_cap=_MAX_INSNS):
    """Run one function under Unicorn with full hooks; return a TraceRecord (a plain dict).

    module  -- e.g. "gdi32.dll"; resolved through the active target like difftest.
    func    -- function selector (see _resolve_func); None = first function.
    args    -- explicit list of dword stdcall args; if None, `arity` zero dwords are pushed.
    arity   -- number of stdcall dword slots to push when args is None; defaults to the
               recovered arity for the function, else 4.
    """
    import unicorn as U
    from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EAX, UC_X86_REG_EIP

    target = cores.get("target")
    path = target.resolve(module)
    off, va0, code, end, secname = _resolve_func(module, func)
    fva = va0 + off
    if args is None:
        n = arity if arity is not None else (_arities(module).get(off) or 4)
        args = [0] * int(n)

    uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_32)
    secbase = va0 & ~(_PAGE - 1)
    size = (len(code) + (va0 - secbase) + (_PAGE - 1)) & ~(_PAGE - 1)
    uc.mem_map(secbase, max(size, _PAGE))
    uc.mem_write(va0, bytes(code))
    uc.mem_map(_SCRATCH_BASE, _SCRATCH_SIZE)
    mapped = {secbase, _SCRATCH_BASE}                        # page-group bases we've mapped

    # Build the stdcall frame: args in order above a RET sentinel the emulation halts on.
    esp = _STACK_TOP
    for a in reversed(args):
        esp -= 4
        uc.mem_write(esp, int(a & 0xFFFFFFFF).to_bytes(4, "little"))
    esp -= 4
    uc.mem_write(esp, _RET.to_bytes(4, "little"))
    uc.reg_write(UC_X86_REG_ESP, esp)
    entry_esp = esp

    rec = {
        "schema": "cb.behavior.trace/1",
        "module": os.path.basename(module),                 # clean label (module may be a path)
        "module_path": path,                                # the resolved file actually traced
        "target_sha256": _sha256(path),
        "section": secname,
        "func_off": off,
        "func_va": fva,
        "func_end_va": va0 + end,
        "input": {"args": [int(a & 0xFFFFFFFF) for a in args], "arity": len(args)},
        "entry": {"esp": entry_esp},
        "insn_count": 0,
        "path": [],
        "mnemonics": {},            # va -> "mov eax, [ecx]"  (first time each va is seen)
        "calls": [],                # {"pc":.., "target":.., "into_func":bool}
        "mem_reads": [],            # {"pc":.., "addr":.., "size":.., "value":..}
        "mem_writes": [],           # {"pc":.., "addr":.., "size":.., "value":..}
        "lazy_maps": [],            # {"pc":.., "addr":.., "access":"read|write|fetch"} faults
        "truncated": False,
        "exit": None,               # "ret" | "exception: ..."
        "ret": None,                # EAX at the RET sentinel
    }
    md = _md()
    func_lo, func_hi = fva, va0 + end

    def hook_code(uc, address, csize, _ud):
        c = rec["insn_count"]
        if c >= insn_cap:
            rec["truncated"] = True
            uc.emu_stop()
            return
        rec["insn_count"] = c + 1
        if len(rec["path"]) < _MAX_PATH:
            rec["path"].append(address)
        if address not in rec["mnemonics"]:
            try:
                ins = next(md.disasm(uc.mem_read(address, min(csize or 15, 15)), address))
                rec["mnemonics"][address] = ("%s %s" % (ins.mnemonic, ins.op_str)).strip()
                if ins.mnemonic == "call":
                    # resolve an immediate (direct) call target; register/mem calls stay None
                    import capstone
                    tgt = None
                    for op in ins.operands:
                        if op.type == capstone.x86.X86_OP_IMM:
                            tgt = op.imm & 0xFFFFFFFF
                    rec["calls"].append({"pc": address, "target": tgt,
                                         "into_func": bool(tgt is not None
                                                           and func_lo <= tgt < func_hi)})
            except StopIteration:
                rec["mnemonics"][address] = "?"

    def hook_mem(uc, access, address, msize, value, _ud):
        bucket = rec["mem_writes"] if access == U.UC_MEM_WRITE else rec["mem_reads"]
        if len(bucket) < _MAX_MEM:
            pc = uc.reg_read(UC_X86_REG_EIP)
            # on a read, Unicorn passes value=0; read it back post-hoc is unreliable, so record
            # the write value as given and leave read value as observed (0-fill where lazy-mapped).
            bucket.append({"pc": pc, "addr": address, "size": msize,
                           "value": int(value) & 0xFFFFFFFF if access == U.UC_MEM_WRITE else None})

    def hook_unmapped(uc, access, address, msize, value, _ud):
        # Lazily map the faulting page (zeroed) and continue -- this is the difference that lets
        # us trace memory-touching functions. Record the fault as first-class evidence.
        base = address & ~(_PAGE - 1)
        if base not in mapped:
            try:
                uc.mem_map(base, _PAGE)
                mapped.add(base)
            except U.UcError:
                return False                                 # truly can't map -> let it fault
        kind = {U.UC_MEM_READ_UNMAPPED: "read", U.UC_MEM_WRITE_UNMAPPED: "write",
                U.UC_MEM_FETCH_UNMAPPED: "fetch"}.get(access, "?")
        if len(rec["lazy_maps"]) < _MAX_MEM:
            pc = uc.reg_read(UC_X86_REG_EIP)
            rec["lazy_maps"].append({"pc": pc, "addr": address, "page": base,
                                     "access": kind, "size": msize})
        return True                                          # retry the access now that it's mapped

    uc.hook_add(U.UC_HOOK_CODE, hook_code)
    uc.hook_add(U.UC_HOOK_MEM_READ | U.UC_HOOK_MEM_WRITE, hook_mem)
    uc.hook_add(U.UC_HOOK_MEM_READ_UNMAPPED | U.UC_HOOK_MEM_WRITE_UNMAPPED
                | U.UC_HOOK_MEM_FETCH_UNMAPPED, hook_unmapped)

    try:
        uc.emu_start(fva, _RET, count=insn_cap + 1)
        rec["exit"] = "truncated" if rec["truncated"] else "ret"
        rec["ret"] = uc.reg_read(UC_X86_REG_EAX)
    except U.UcError as e:
        rec["exit"] = "exception: %s" % e
        try:
            rec["ret"] = uc.reg_read(UC_X86_REG_EAX)
        except Exception:
            pass

    # mnemonics keyed by int va -> stringify for JSON friendliness
    rec["mnemonics"] = {("0x%x" % k): v for k, v in rec["mnemonics"].items()}
    return rec
