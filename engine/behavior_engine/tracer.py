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


# Carving a PE (PeCarve disassembles the whole code section) is the dominant per-trace cost.
# The behavioral commands call trace_function HUNDREDS of times per function (sweeps, probes,
# binary searches), so memoize the expensive load per resolved path.
_PE_CACHE = {}


def _load(module):
    """(path, va0, code, secname, funcs, sha) for a module, carved once and cached."""
    target = cores.get("target")
    path = target.resolve(module)
    ce = _PE_CACHE.get(path)
    if ce is None:
        pe = open_pe(path)
        va0, code, secname = pe.code_sections()[0]
        sg = PeSeg(module, section=secname, carve=PeCarve(path))
        funcs = {off: {"end": f["end"]} for off, f in sg.funcs.items()}
        ce = (path, va0, bytes(code), secname, funcs, _sha256(path))
        _PE_CACHE[path] = ce
    return ce


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
    _, va0, code, secname, funcs, _ = _load(module)
    out = []
    for off, f in sorted(funcs.items()):
        size = f["end"] - off
        if size >= 8:
            out.append((off, va0 + off, size))
    return out


def _resolve_func(module, func):
    """Map a user-supplied func selector to (off, va0, code, end, secname).

    func may be: None (first function), a section offset or VA as hex ('0x...') or int,
    or a decimal offset. A VA (>= va0) is converted to an offset; an offset is taken as-is.
    """
    _, va0, code, secname, allfuncs, _ = _load(module)
    funcs = {off: f for off, f in allfuncs.items() if (f["end"] - off) >= 8}
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


def trace_function(module, func=None, args=None, arity=None, insn_cap=_MAX_INSNS, mem_init=None,
                   stub_externals=False, stub_ret=0, stub_cleanup=0):
    """Run one function under Unicorn with full hooks; return a TraceRecord (a plain dict).

    module  -- e.g. "gdi32.dll"; resolved through the active target like difftest.
    func    -- function selector (see _resolve_func); None = first function.
    args    -- explicit list of dword stdcall args; if None, `arity` zero dwords are pushed.
    arity   -- number of stdcall dword slots to push when args is None; defaults to the
               recovered arity for the function, else 4.
    mem_init-- optional list of (addr, bytes) to write into memory before running (pages mapped
               as needed). Lets a caller point a pointer argument at a controlled buffer -- the
               basis of struct/pointer-field reconstruction.
    stub_externals -- when a call transfers to an address OUTSIDE the module's code section (an
               import thunk / API), don't run into zero-filled memory: emulate a return to the
               caller with EAX=stub_ret and pop stub_cleanup extra bytes (0 = cdecl; a stdcall
               API needs 4*argc, ideally supplied later from the SDK signatures). The external
               call is recorded as a dependency. This is the first step of the controlled
               environment -- enough to get past benign API calls and discover dependencies;
               it does not model the API's real effects.
    """
    import unicorn as U
    from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EAX, UC_X86_REG_EIP

    path, _, _, _, _, sha = _load(module)
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

    # pre-load caller-supplied memory (e.g. a controlled struct buffer a pointer arg points at)
    for addr, data in (mem_init or []):
        a = addr
        while a < addr + len(data):
            base = a & ~(_PAGE - 1)
            if base not in mapped and not (secbase <= base < secbase + max(size, _PAGE)) \
                    and not (_SCRATCH_BASE <= base < _SCRATCH_BASE + _SCRATCH_SIZE):
                uc.mem_map(base, _PAGE); mapped.add(base)
            a = base + _PAGE
        uc.mem_write(addr, bytes(data))

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
        "target_sha256": sha,
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

    sec_lo, sec_hi = secbase, secbase + max(size, _PAGE)
    pending = [None]        # index of a call whose actual destination is the next instruction

    def hook_code(uc, address, csize, _ud):
        c = rec["insn_count"]
        if c >= insn_cap:
            rec["truncated"] = True
            uc.emu_stop()
            return
        rec["insn_count"] = c + 1
        # The first instruction executed after a call IS the callee's entry -- this resolves
        # indirect (register/memory) call targets the decoder can't read statically, and
        # classifies every target as intra-module or external.
        if pending[0] is not None:
            cr = rec["calls"][pending[0]]
            cr["resolved_target"] = address
            ext = not (sec_lo <= address < sec_hi)
            cr["external"] = ext
            pending[0] = None
            if ext and stub_externals:
                # emulate a return instead of executing the external target. At the callee's
                # entry [esp] holds the return address the call pushed.
                try:
                    esp_now = uc.reg_read(UC_X86_REG_ESP)
                    ret_addr = int.from_bytes(uc.mem_read(esp_now, 4), "little")
                    uc.reg_write(UC_X86_REG_ESP, (esp_now + 4 + stub_cleanup) & 0xFFFFFFFF)
                    uc.reg_write(UC_X86_REG_EAX, stub_ret & 0xFFFFFFFF)
                    uc.reg_write(UC_X86_REG_EIP, ret_addr)
                    cr["stubbed"] = True
                    rec["stubbed_externals"] = rec.get("stubbed_externals", 0) + 1
                    return                      # skip the external; resume in the caller
                except Exception:
                    pass
        if len(rec["path"]) < _MAX_PATH:
            rec["path"].append(address)
        if address not in rec["mnemonics"]:
            try:
                ins = next(md.disasm(uc.mem_read(address, min(csize or 15, 15)), address))
                rec["mnemonics"][address] = ("%s %s" % (ins.mnemonic, ins.op_str)).strip()
                if ins.mnemonic == "call":
                    # static (immediate) target if direct; register/mem calls resolve on next insn
                    import capstone
                    tgt = None
                    for op in ins.operands:
                        if op.type == capstone.x86.X86_OP_IMM:
                            tgt = op.imm & 0xFFFFFFFF
                    rec["calls"].append({"pc": address, "target": tgt, "indirect": tgt is None,
                                         "into_func": bool(tgt is not None
                                                           and func_lo <= tgt < func_hi)})
                    pending[0] = len(rec["calls"]) - 1
            except StopIteration:
                rec["mnemonics"][address] = "?"

    def hook_mem(uc, access, address, msize, value, _ud):
        is_write = access == U.UC_MEM_WRITE
        bucket = rec["mem_writes"] if is_write else rec["mem_reads"]
        if len(bucket) < _MAX_MEM:
            pc = uc.reg_read(UC_X86_REG_EIP)
            if is_write:
                v = int(value) & 0xFFFFFFFF
            else:
                # Unicorn passes value=0 on a READ, but the bytes being read are already in
                # memory at hook time -- read them back to capture the ACTUAL value observed.
                try:
                    v = int.from_bytes(uc.mem_read(address, min(msize, 8)), "little")
                except Exception:
                    v = None
            bucket.append({"pc": pc, "addr": address, "size": msize, "value": v})

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
