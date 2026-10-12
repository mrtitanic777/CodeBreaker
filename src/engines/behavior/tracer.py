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

from src.core import cores                          # noqa: E402
from src.core import paths                          # noqa: E402
from src.formats.pe import open_pe                 # noqa: E402
from src.formats.pe_seg import PeSeg               # noqa: E402
from src.formats.pe_trace import PeCarve           # noqa: E402

# Entry-state constants, identical to difftest._emulate so traces are comparable to the
# oracle difftest already trusts.
_SCRATCH_BASE, _SCRATCH_SIZE = 0x200000, 0x10000
# The stack has a region of its own. It used to sit at the top of the scratch region, 2 KB above
# the objects pointer arguments are aimed at, so a function with a large frame or a deep call chain
# wrote over its own inputs.
_STACK_BASE, _STACK_SIZE = 0x300000, 0x40000
_STACK_TOP, _RET = _STACK_BASE + _STACK_SIZE - 0x1000, 0xDEAD0000
_PAGE = 0x1000

# Recording caps -- a pathological loop can execute millions of instructions; a trace is
# evidence, not a full replay log, so we bound it and record that we did.
_MAX_INSNS = 20000
_MAX_MEM = 40000
_MAX_PATH = 20000
# Hard ceiling on distinct pages lazily mapped in ONE trace. A function that scans/writes an
# ever-increasing address range faults on a new page each step; hook_unmapped maps + retries, and
# the instruction never RETIRES -- so the instruction cap never fires, and (because Unicorn's
# mem_map slows as regions accumulate) the trace crawls through maps for a very long time. Bounding
# the map count abandons such a runaway deterministically (by page count, not a clock). 2048 pages
# = 2 MB, still far more than any real function's working set (legit functions touch a handful of
# pages; a scan/copy runaway marches through hundreds+ and is what we want to cut off fast).
_MAX_MAPS = 512
# Hard ceiling on memory ops per trace: bounds a `rep` flood over already-mapped memory (one
# instruction, millions of element accesses) that the instruction caps can't see.
_MAX_MEMOPS = 3_000_000

# Per-FUNCTION instruction governor. A single emulation is capped (_MAX_INSNS), but reconstructing
# one function runs MANY emulations across the cascade (synth trials, struct, effect, envprov, fuzz,
# terminate, ...). A pathological function (e.g. jgdwmie!sub_22c0) can make that explode and spin for
# hours. This is a DETERMINISTIC ceiling on TOTAL instructions emulated per function (not a wall-clock
# timeout): recon.reconstruct_one resets it; hook_code stops emulation once it's exceeded, so the
# cascade runs dry fast and the function is honestly SKIPPED -- one bad function never stalls the run.
_GOV = {"n": 0, "ceil": 0}


def budget_reset(ceiling=0):
    """Start a fresh per-function instruction budget (0 = unlimited)."""
    _GOV["n"] = 0
    _GOV["ceil"] = ceiling


def budget_exceeded():
    return bool(_GOV["ceil"]) and _GOV["n"] > _GOV["ceil"]

# IAT sentinels: a unique, recognizable landing address per import slot (see trace_function).
_SENT_BASE = 0xE1000000
_SENT_STRIDE = 0x10

# Process-wide default for external-call stubbing, set once (e.g. by the command dispatcher when
# an SDK is configured) so EVERY trace in the analysis pipeline inherits it without threading the
# flag through every call site. An explicit argument to trace_function always overrides this.
_DEFAULT_STUB = {"enabled": False, "resolver": None}


def set_default_stub(enabled, resolver=None):
    _DEFAULT_STUB["enabled"] = bool(enabled)
    _DEFAULT_STUB["resolver"] = resolver

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


_MAX_CODE_SPAN = 64 << 20     # give up joining code sections spread wider than this


def _pe_code_image(module, path, pe):
    """(va0, code, secname, funcs) covering EVERY executable section of a PE.

    A PE can have several code sections (kernel32: _FREQASM, .text, _INIT). Taking only the first
    left every function in the others untraceable -- the address fell outside `code`, the trace
    raised, and the body was reported "inconclusive" -- which hid about 91% of kernel32's code from
    verification. The sections are laid out at their real addresses in one buffer starting at the
    lowest; the gaps between them (data sections) are zero-filled, which is exactly what an unmapped
    page reads as on demand, so nothing outside the code changes. Function offsets are relative to
    that buffer, as callers expect (`va0 + off`)."""
    secs = pe.code_sections()                                  # [(va, bytes, name)]
    carve = PeCarve(path)
    lo = min(va for va, _raw, _nm in secs)
    hi = max(va + len(raw) for va, raw, _nm in secs)
    if len(secs) > 1 and hi - lo > _MAX_CODE_SPAN:
        secs = secs[:1]                                        # pathological layout: first only
        lo, hi = secs[0][0], secs[0][0] + len(secs[0][1])
    code = bytearray(hi - lo)
    funcs = {}
    for va, raw, name in secs:
        base = va - lo
        code[base:base + len(raw)] = raw
        sg = PeSeg(module, section=name, carve=carve)
        for off, f in sg.funcs.items():
            funcs[base + off] = {"end": base + f["end"]}
    return lo, code, "+".join(nm for _va, _raw, nm in secs), funcs


def _load(module):
    """(path, va0, code, secname, funcs, sha, imports) for a module, carved once and cached.
    imports is {iat_slot_va: (dll, name)} from the PE import directory (may be empty)."""
    target = cores.get("target")
    # A "name#member" selector addresses one module inside a W3 bundle (the core VxDs live in
    # vmm32.vxd, not as loose files). Resolve the bundle file by its base name; keep the full
    # selector as the cache key and pass it to the LE loader to pick the member.
    base_module = module.split("#", 1)[0]
    is_member = "#" in module
    path = target.resolve(base_module)
    cache_key = module if is_member else path
    ce = _PE_CACHE.get(cache_key)
    if ce is None:
        # VxD (LE/LX, ring-0) modules: flat 32-bit code, no PE structures. Route to the LE
        # loader/carver, which returns this same tuple shape. VMM services are inline INT 20h
        # (stubbed in trace_function), so imports is {}. PE path below is unchanged.
        fmt = None
        try:
            fmt = target.format_of(path)
        except Exception:
            fmt = None
        if is_member or fmt in ("LE", "LX"):        # a W3 member is always an LE image
            from src.formats import le_load as _le_load
            ce = _le_load.le_load(path, module)
            _PE_CACHE[cache_key] = ce
            return ce
        pe = open_pe(path)
        va0, code, secname, funcs = _pe_code_image(module, path, pe)
        try:
            imports = dict(pe.imports())
        except Exception:
            imports = {}
        ce = (path, va0, bytes(code), secname, funcs, _sha256(path), imports)
        _PE_CACHE[path] = ce
    return ce


def _arities(module):
    """sigrecover.arities(module) lives in recompile95/tools; import it lazily and tolerate
    its absence (a corpus/data-shape dependency, not a hard one for tracing)."""
    tools = paths.engine_dir("recompile95", "tools")
    if tools not in sys.path:
        sys.path.insert(0, tools)
    try:
        import sigrecover
        return sigrecover.arities(module) or {}
    except Exception:
        return {}


def list_functions(module):
    """(off, va, size) for each recovered function across the module's code sections, smallest
    offset first (offsets are relative to the lowest code section). The caller picks one to trace."""
    _, va0, code, secname, funcs, _, _imp = _load(module)
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
    _, va0, code, secname, allfuncs, _, _imp = _load(module)
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
            if holder:
                off = holder[0]
            elif 0 <= off < len(code):
                # An explicit VA/offset the recovery didn't mark as a function (e.g. an EXPORT not
                # reached by call-graph traversal). The caller knows it's a function, so accept it
                # as a standalone start: end = the next recovered function start, else section end.
                nexts = sorted(o for o in funcs if o > off)
                return off, va0, code, (nexts[0] if nexts else len(code)), secname
            else:
                raise ValueError("%s: no function at offset 0x%x (VA 0x%x)"
                                 % (module, off, va0 + off))
    return off, va0, code, funcs[off]["end"], secname


_RETSENT_BASE = 0xB1B10000                                # unique per-call external-return marker


def trace_function(module, func=None, args=None, arity=None, insn_cap=_MAX_INSNS, mem_init=None,
                   stub_externals=None, stub_ret=0, stub_cleanup=0, sig_resolver=None,
                   stub_ret_unique=False, overlay=None):
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
    from unicorn.x86_const import (UC_X86_REG_ESP, UC_X86_REG_EAX, UC_X86_REG_EIP,
                                   UC_X86_REG_ECX, UC_X86_REG_EDX)

    if stub_externals is None:                               # inherit the process default
        stub_externals = _DEFAULT_STUB["enabled"]
    if sig_resolver is None:
        sig_resolver = _DEFAULT_STUB["resolver"]
    path, _, _, _, _, sha, imports = _load(module)
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
    uc.mem_map(_STACK_BASE, _STACK_SIZE)
    mapped = {secbase, _SCRATCH_BASE}                        # page-group bases we've mapped
    mapped.update(range(_STACK_BASE, _STACK_BASE + _STACK_SIZE, _PAGE))

    # When stubbing externals, plant a unique SENTINEL address in each IAT slot so a
    # `call [iat_slot]` resolves to a NAMED import (not a null pointer), and we can look up its
    # stdcall arg count for correct stack cleanup. Naming works with no SDK; cleanup needs one.
    sentinel_map = {}                                        # sentinel addr -> (dll, name)
    mem_init = list(mem_init or [])
    # `overlay` is another module whose code is mapped alongside at its own addresses. A rebuilt
    # body refers to the ORIGINAL's code and data by literal address (a function pointer, a thunk
    # target); without the original there, those addresses read as zeros in the rebuilt run only,
    # and the two runs differ for a reason that has nothing to do with the body.
    if overlay:
        try:
            _op, ova0, ocode, _on, _of, _os, oimports = _load(overlay)
            own_hi = secbase + max(size, _PAGE)
            if ova0 + len(ocode) <= secbase or ova0 >= own_hi:      # never over the traced image
                mem_init.insert(0, (ova0, ocode))
                imports = dict(oimports, **imports)
        except Exception:
            pass
    if stub_externals and imports:
        for i, (iat_va, (dll, name)) in enumerate(sorted(imports.items())):
            sent = _SENT_BASE + i * _SENT_STRIDE
            mem_init.append((iat_va, sent.to_bytes(4, "little")))
            sentinel_map[sent] = (dll, name)
        span = (len(imports) * _SENT_STRIDE + (_PAGE - 1)) & ~(_PAGE - 1)
        mem_init.append((_SENT_BASE, bytes(span)))           # map+zero the sentinel landing region

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
    zeros = [0]             # consecutive 00-00 ("add [eax],al") instructions -> crawling zeros

    def hook_code(uc, address, csize, _ud):
        # per-function instruction governor: once this function's cumulative emulation across the
        # whole cascade exceeds the ceiling, stop immediately so the function is abandoned (skipped),
        # never spinning for hours on a pathological body.
        _GOV["n"] += 1
        if _GOV["ceil"] and _GOV["n"] > _GOV["ceil"]:
            rec["truncated"] = True
            rec["governed"] = True
            uc.emu_stop()
            return
        c = rec["insn_count"]
        if c >= insn_cap:
            rec["truncated"] = True
            uc.emu_stop()
            return
        rec["insn_count"] = c + 1
        # A landing in a 00-padding region (executing `add [eax],al`, which real code never does)
        # means a call/jmp went through a pointer that resolved to nothing -- an import our IAT
        # scan didn't cover, or an uninitialised function pointer. Rather than crawl the zeros to
        # the cap, STUB-RETURN from it (like an external call): pop the return address and resume
        # in the caller with EAX=0. This recovers functions whose only obstacle was a bogus
        # indirect call. Bounded so a pathological case still terminates.
        try:
            if uc.mem_read(address, 2) == b"\x00\x00":
                zeros[0] += 1
                if zeros[0] == 2 and rec.get("bogus_stubs", 0) < 64:
                    esp_now = uc.reg_read(UC_X86_REG_ESP)
                    ret_addr = int.from_bytes(uc.mem_read(esp_now, 4), "little")
                    uc.reg_write(UC_X86_REG_ESP, (esp_now + 4) & 0xFFFFFFFF)
                    uc.reg_write(UC_X86_REG_EAX, 0)
                    uc.reg_write(UC_X86_REG_EIP, ret_addr)
                    rec["bogus_stubs"] = rec.get("bogus_stubs", 0) + 1
                    zeros[0] = 0
                    return
                if zeros[0] > 16:
                    rec["truncated"] = True
                    rec["ran_into_zeros"] = True
                    uc.emu_stop(); return
            else:
                zeros[0] = 0
        except Exception:
            zeros[0] = 0
        # EXTERNAL-LANDING STUB. Execution reaching an IAT sentinel means an import was entered
        # -- by `call [iat]` OR by `jmp [iat]` (a tail call / thunk). Either way [esp] holds the
        # return address to resume at, so emulate the API's return instead of crawling into the
        # zero-filled sentinel page (the cause of most "runaway loop" truncations). Catching JMP
        # here, not just CALL, is the fix.
        if stub_externals and address in sentinel_map:
            dll, name = sentinel_map[address]
            api = "%s!%s" % (dll, name)
            rec.setdefault("api_deps", [])
            if api not in rec["api_deps"]:
                rec["api_deps"].append(api)
            argc = sig_resolver(dll, name) if sig_resolver else None
            cleanup = 4 * argc if argc is not None else stub_cleanup
            try:
                esp_now = uc.reg_read(UC_X86_REG_ESP)
                ret_addr = int.from_bytes(uc.mem_read(esp_now, 4), "little")
                uc.reg_write(UC_X86_REG_ESP, (esp_now + 4 + cleanup) & 0xFFFFFFFF)
                # Every import entered, in order, with the arguments it was given (as many as its
                # signature says; none when the signature is unknown). `api_deps` only says WHICH
                # imports were reached -- this says what they were called with.
                seq = rec.setdefault("api_calls", [])
                if len(seq) < 256:
                    seq.append((api, None if argc is None else
                                [int.from_bytes(uc.mem_read(esp_now + 4 + 4 * i, 4), "little")
                                 for i in range(argc)]))
                # unique per-call return marker (interproc v2 value-flow), else the fixed stub_ret
                nstub = rec.get("stubbed_externals", 0)
                rv = (_RETSENT_BASE + nstub) if stub_ret_unique else stub_ret
                uc.reg_write(UC_X86_REG_EAX, rv & 0xFFFFFFFF)
                uc.reg_write(UC_X86_REG_EIP, ret_addr)
                rec["stubbed_externals"] = nstub + 1
                if pending[0] is not None:                   # annotate the call record, if via CALL
                    cr = rec["calls"][pending[0]]
                    cr.update(resolved_target=address, external=True, api=api, stubbed=True,
                              ret_sentinel=(rv if stub_ret_unique else None))
                    try:                                     # snapshot candidate args above ret addr
                        cr["args_stack"] = [int.from_bytes(uc.mem_read(esp_now + 4 + 4*i, 4), "little")
                                            for i in range(6)]
                        cr["ecx"] = uc.reg_read(UC_X86_REG_ECX)   # thiscall this / fastcall arg0
                        cr["edx"] = uc.reg_read(UC_X86_REG_EDX)   # fastcall arg1
                    except Exception:
                        pass
                    if argc is not None:
                        cr["argc"] = argc
                    pending[0] = None
                return                          # skip the external; resume in the caller
            except Exception:
                pass
        # The first instruction after a CALL is the callee entry -- resolve indirect (register/
        # memory) intra-module targets the decoder can't read statically, and classify them.
        if pending[0] is not None:
            cr = rec["calls"][pending[0]]
            cr["resolved_target"] = address
            cr["external"] = not (sec_lo <= address < sec_hi)
            try:                                             # candidate args sit above the ret addr
                esp_c = uc.reg_read(UC_X86_REG_ESP)
                cr["args_stack"] = [int.from_bytes(uc.mem_read(esp_c + 4 + 4*i, 4), "little")
                                    for i in range(6)]
                cr["ecx"] = uc.reg_read(UC_X86_REG_ECX)      # thiscall this / fastcall arg0
                cr["edx"] = uc.reg_read(UC_X86_REG_EDX)      # fastcall arg1
            except Exception:
                pass
            pending[0] = None
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
        # A single `rep movs/stos/scas` with a huge (often uninitialised) count floods memory within
        # already-mapped pages: ONE retired instruction, so hook_code's instruction cap + governor
        # never fire, but hook_mem fires per element. Bound total mem ops per trace and bail -- a
        # deterministic ceiling (op count, not a clock) so the runaway function is skipped.
        rec["memops"] = rec.get("memops", 0) + 1
        if rec["memops"] > _MAX_MEMOPS:
            rec["truncated"] = True; rec["governed"] = True; uc.emu_stop(); return
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
        # Lazily map the faulting page(s) (zeroed) and continue -- this is the difference that lets
        # us trace memory-touching functions. Record the fault as first-class evidence.
        # Map EVERY page the access spans (a read/write crossing into the next unmapped page would
        # otherwise re-fault at the same address forever). Two deterministic bails (no wall clock):
        #   * total pages mapped exceeds _MAX_MAPS  -> a runaway scan/copy over an ever-growing range
        #   * faults that map no NEW page keep recurring -> a fault we can't satisfy (loop)
        if os.environ.get("WT_DBG"):
            rec["_hu"] = rec.get("_hu", 0) + 1
            if rec["_hu"] % 2000 == 1:
                print("  [hu #%d] addr=0x%x size=%s mapped=%d stuck=%s" %
                      (rec["_hu"], address, msize, len(mapped), rec.get("_stuck", 0)), flush=True)
        lo = address & ~(_PAGE - 1)
        hi = (address + max(msize, 1) - 1) & ~(_PAGE - 1)
        newly = 0
        for base in range(lo, hi + _PAGE, _PAGE):
            if base in mapped:
                continue
            _GOV["n"] += 1                                   # maps count toward the per-function budget
            if len(mapped) >= _MAX_MAPS or (_GOV["ceil"] and _GOV["n"] > _GOV["ceil"]):
                rec["truncated"] = True; rec["governed"] = True; uc.emu_stop(); return False
            try:
                uc.mem_map(base, _PAGE); mapped.add(base); newly += 1
            except U.UcError:
                return False                                 # truly can't map -> let it fault
        kind = {U.UC_MEM_READ_UNMAPPED: "read", U.UC_MEM_WRITE_UNMAPPED: "write",
                U.UC_MEM_FETCH_UNMAPPED: "fetch"}.get(access, "?")
        if len(rec["lazy_maps"]) < _MAX_MEM:
            rec["lazy_maps"].append({"pc": uc.reg_read(UC_X86_REG_EIP), "addr": address,
                                     "page": lo, "access": kind, "size": msize})
        if newly == 0:                                       # couldn't make progress -> guard a loop
            rec["_stuck"] = rec.get("_stuck", 0) + 1
            if rec["_stuck"] > 64:
                rec["truncated"] = True; rec["governed"] = True; uc.emu_stop(); return False
        else:
            rec["_stuck"] = 0
        return True                                          # retry the access now that it's mapped

    def hook_intr(uc, intno, user):
        # VxD/VMM service call: `INT 20h ; dd <VxD_ID<<16|service>`. There is no IDT here, so a
        # VxD service is an EXTERNAL call -> stub it: skip the 4-byte inline service dword and
        # continue, leaving EAX as the (unknown) service return. INT 0x20 is the VMM dispatch;
        # any other interrupt in flat ring-0 code we can't model, so halt cleanly.
        if intno == 0x20:
            eip = uc.reg_read(UC_X86_REG_EIP)          # points just past the 2-byte INT 20h
            rec.setdefault("vxd_services", []).append(eip)
            uc.reg_write(UC_X86_REG_EIP, eip + 4)      # step over the inline service id dword
            return
        uc.emu_stop()

    uc.hook_add(U.UC_HOOK_CODE, hook_code)
    uc.hook_add(U.UC_HOOK_MEM_READ | U.UC_HOOK_MEM_WRITE, hook_mem)
    uc.hook_add(U.UC_HOOK_MEM_READ_UNMAPPED | U.UC_HOOK_MEM_WRITE_UNMAPPED
                | U.UC_HOOK_MEM_FETCH_UNMAPPED, hook_unmapped)
    uc.hook_add(U.UC_HOOK_INTR, hook_intr)

    try:
        uc.emu_start(fva, _RET, count=insn_cap + 1)
        rec["exit"] = "truncated" if rec["truncated"] else "ret"
        rec["ret"] = uc.reg_read(UC_X86_REG_EAX)
        try:                                                 # x87 return (float functions -> st0)
            from unicorn.x86_const import UC_X86_REG_ST0
            rec["st0"] = float(uc.reg_read(UC_X86_REG_ST0))
        except Exception:
            pass
    except U.UcError as e:
        rec["exit"] = "exception: %s" % e
        try:
            rec["ret"] = uc.reg_read(UC_X86_REG_EAX)
        except Exception:
            pass

    # mnemonics keyed by int va -> stringify for JSON friendliness
    rec["mnemonics"] = {("0x%x" % k): v for k, v in rec["mnemonics"].items()}
    return rec
