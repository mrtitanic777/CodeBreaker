r"""emulator.py -- run one 32-bit function in Unicorn and record a structured TraceRecord.

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
from codebreaker.base import corpus
from codebreaker.base import settings as paths                          # noqa: E402
from codebreaker.formats.pe import open_pe                 # noqa: E402
from codebreaker.formats.pe_functions import PeSeg               # noqa: E402
from codebreaker.formats.pe_functions import PeCarve           # noqa: E402
from codebreaker import platforms


# Entry-state constants, identical to difftest._emulate so traces are comparable to the
# oracle difftest already trusts.
_SCRATCH_BASE, _SCRATCH_SIZE = 0x200000, 0x10000
# The stack has a region of its own. It used to sit at the top of the scratch region, 2 KB above
# the objects pointer arguments are aimed at, so a function with a large frame or a deep call chain
# wrote over its own inputs.
_STACK_BASE, _STACK_SIZE = 0x300000, 0x40000
SAVED_SENTINELS = (0xB0B0B0B0, 0x51515151, 0xD1D1D1D1, 0xBDBDBDBD)      # EBX, ESI, EDI, EBP at entry
_STACK_TOP, _RET = _STACK_BASE + _STACK_SIZE - 0x1000, 0xDEAD0000
_PAGE = 0x1000

# Recording caps -- a pathological loop can execute millions of instructions; a trace is
# evidence, not a full replay log, so we bound it and record that we did.
_MAX_INSNS = 20000
_MAX_MEM = 40000

# Per-FUNCTION instruction governor. A single emulation is capped (_MAX_INSNS), but reconstructing
# one function runs MANY emulations across the cascade (synth trials, struct, effect, envprov, fuzz,
# terminate, ...). A pathological function (e.g. jgdwmie!sub_22c0) can make that explode and spin for
# hours. This is a DETERMINISTIC ceiling on TOTAL instructions emulated per function (not a wall-clock
# timeout): recon.reconstruct_one resets it; hook_code stops emulation once it's exceeded, so the
# cascade runs dry fast and the function is honestly SKIPPED -- one bad function never stalls the run.
_GOV = {"n": 0, "ceil": 0}


# IAT sentinels: a unique, recognizable landing address per import slot (see trace_function).
_SENT_BASE = 0xE1000000
_SENT_STRIDE = 0x10

# Process-wide default for external-call stubbing, set once (e.g. by the command dispatcher when
# an SDK is configured) so EVERY trace in the analysis pipeline inherits it without threading the
# flag through every call site. An explicit argument to trace_function always overrides this.
_DEFAULT_STUB = {"enabled": False, "resolver": None}


_MD = None


def _md():
    """One detailed capstone engine, reused across instructions (constructing per-insn is the
    cost that dominates -- same lesson difftest._md() records)."""
    global _MD
    if _MD is None:
        _MD = platforms.decoder(32, detail=True)
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
_LOADED = {}              # the name a module was asked for by -> the same, without asking the disk again
_RESOLVED = {}


_CODE_RANGES = {}         # module file -> [(lo, hi)] of its code sections, as loaded
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
    _CODE_RANGES[path] = [(va, va + len(raw)) for va, raw, _nm in secs]
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
    # A "name#member" selector addresses one module inside a W3 bundle (the core VxDs live in
    # vmm32.vxd, not as loose files). Resolve the bundle file by its base name; keep the full
    # selector as the cache key and pass it to the LE loader to pick the member.
    if module in _LOADED:
        return _LOADED[module]
    base_module = module.split("#", 1)[0]
    is_member = "#" in module
    path = corpus.resolve(base_module)
    cache_key = module if is_member else path
    ce = _PE_CACHE.get(cache_key)
    if ce is None:
        pe = open_pe(path)
        va0, code, secname, funcs = _pe_code_image(module, path, pe)
        try:
            imports = dict(pe.imports())
        except Exception:
            imports = {}
        ce = (path, va0, bytes(code), secname, funcs, _sha256(path), imports)
        _PE_CACHE[path] = ce
    _LOADED[module] = ce
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


def _resolve_func(module, func):
    """Map a user-supplied func selector to (off, va0, code, end, secname).

    func may be: None (first function), a section offset or VA as hex ('0x...') or int,
    or a decimal offset. A VA (>= va0) is converted to an offset; an offset is taken as-is.
    """
    memo = (module, func)
    if memo in _RESOLVED:
        return _RESOLVED[memo]
    _RESOLVED[memo] = out = _resolve_func_(module, func)
    return out


def _resolve_func_(module, func):
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


_OUT_HANDLE = 0x0000A100                                  # the handle a stubbed API gives back (+ 4 x its parameter's place)
_HANDLE_OUT_TYPES = ("PHKEY", "PHANDLE", "LPHANDLE")
_NUMBER_OUT_TYPES = ("LPDWORD", "PDWORD", "LPLONG", "PLONG", "PULONG", "LPUINT", "PUINT")
_OUTS = {}


def _outs(name):
    """[(place, value)] of an API's parameters through which it gives something back, by its
    declaration: a handle (value None: _OUT_HANDLE by its place) or a number (0)."""
    if name not in _OUTS:
        try:
            from codebreaker import knowledge
            proto = knowledge.prototype("win32", name) or {}
        except Exception:                                   # (no headers here: nothing is given back)
            proto = {}
        out = []
        for i, (ty, _n) in enumerate(proto.get("parameters") or []):
            t = ty.split()[-1]
            if t in _HANDLE_OUT_TYPES:
                out.append((i, None))
            elif t in _NUMBER_OUT_TYPES:
                out.append((i, 0))
        _OUTS[name] = out
    return _OUTS[name]


_INSN = {}                # (module file, its hash) -> {address: (text, is a call, direct target)}


# Branch log (trace_function(branches=True)): for each conditional jump of the function itself,
# which way it went and what the comparison just before it compared. An input generator reads
# this to aim at the side of a branch no input has taken yet (verify/guided.py).
_BRANCH_CAP = 4096                   # branch events kept per run
_COMPARES = ("cmp", "test", "sub", "and", "or", "dec", "inc", "add", "xor")
# instructions that leave the flags alone: a compare before them still decides the jump after
_FLAGLESS = ("mov", "movzx", "movsx", "lea", "push", "pop", "nop", "xchg", "not", "cdq", "cwde", "cbw", "bswap")
_PARENT = {}                         # capstone register name -> the 32-bit register it is part of
for _r in "abcd":
    for _n in (_r + "l", _r + "h", _r + "x", "e" + _r + "x"):
        _PARENT[_n] = "e" + _r + "x"
for _r in ("si", "di", "bp", "sp"):
    _PARENT[_r] = _PARENT["e" + _r] = "e" + _r
# (sha, address) -> (kind, detail, registers written, load) where kind is "jcc" (detail: target,
# next), "cmp" (detail: mnemonic, operands, size), "keep" (flags untouched) or "other"; load is
# (register, memory operand, size) for a load from memory into a register, else None
_DECODED = {}


def _uc_reg(name):
    from unicorn import x86_const
    return getattr(x86_const, "UC_X86_REG_" + name.upper(), None)


_MACHINES = {}            # (module, hash, overlay, stubbing) -> a machine with it laid out, between runs
_MAX_MACHINES = 4


def _machine(U, path, sha, va0, code, secbase, size, imports, overlay, stub_externals, first):
    """A fresh emulator with the module laid out: its image, the original beside it (`overlay`),
    the caller's memory (`first`), and a landing place in every import slot.

    When stubbing externals, a unique SENTINEL address is planted in each IAT slot so a
    `call [iat_slot]` resolves to a NAMED import (not a null pointer), and its stdcall argument
    count can be looked up for correct stack cleanup."""
    uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_32)
    uc.mem_map(secbase, max(size, _PAGE))
    uc.mem_write(va0, bytes(code))
    uc.mem_map(_SCRATCH_BASE, _SCRATCH_SIZE)
    uc.mem_map(_STACK_BASE, _STACK_SIZE)
    mapped = {secbase, _SCRATCH_BASE}                        # page-group bases we've mapped
    mapped.update(range(_STACK_BASE, _STACK_BASE + _STACK_SIZE, _PAGE))
    sentinel_map = {}                                        # sentinel addr -> (dll, name)
    mem_init = list(first)
    # The module's code as it is in the file: an instruction there is decoded once per process,
    # not once per run -- unless something was written over it (then memory is what is read).
    statics = [(lo, hi, code[lo - va0:hi - va0], _INSN.setdefault((path, sha, lo), {}))
               for lo, hi in _CODE_RANGES.get(path) or [(va0, va0 + len(code))]]
    laid = None
    # `overlay` is another module whose code is mapped alongside at its own addresses. A rebuilt
    # body refers to the ORIGINAL's code and data by literal address (a function pointer, a thunk
    # target); without the original there, those addresses read as zeros in the rebuilt run only,
    # and the two runs differ for a reason that has nothing to do with the body.
    if overlay:
        try:
            _op, ova0, ocode, _on, _of, _os, oimports = _load(overlay)
            own_hi = secbase + max(size, _PAGE)
            if ova0 + len(ocode) <= secbase or ova0 >= own_hi:      # never over the traced image
                laid = (ova0, ocode)
                mem_init.insert(0, laid)
                statics += [(lo, hi, ocode[lo - ova0:hi - ova0], _INSN.setdefault((_op, _os, lo), {}))
                            for lo, hi in _CODE_RANGES.get(_op) or [(ova0, ova0 + len(ocode))]]
                imports = dict(oimports, **imports)
        except Exception:
            pass
    slots = {}                                               # page -> [(address, bytes)] of import slots
    if stub_externals and imports:
        for i, (iat_va, (dll, name)) in enumerate(sorted(imports.items())):
            sent = _SENT_BASE + i * _SENT_STRIDE
            mem_init.append((iat_va, sent.to_bytes(4, "little")))
            slots.setdefault(iat_va & ~(_PAGE - 1), []).append(mem_init[-1])
            if (iat_va + 3) & ~(_PAGE - 1) != iat_va & ~(_PAGE - 1):
                slots.setdefault((iat_va + 3) & ~(_PAGE - 1), []).append(mem_init[-1])
            sentinel_map[sent] = (dll, name)
        span = (len(imports) * _SENT_STRIDE + (_PAGE - 1)) & ~(_PAGE - 1)
        mem_init.append((_SENT_BASE, bytes(span)))           # map+zero the sentinel landing region
    # pages given other contents before the run are not "as in the file"
    init_pages = set()
    for entry in mem_init:
        if entry is not laid:
            init_pages.update(range(entry[0] >> 12, ((entry[0] + max(len(entry[1]), 1) - 1) >> 12) + 1))
    # pre-load memory (e.g. a controlled struct buffer a pointer arg points at); pages that lie
    # side by side are mapped together -- every mapping makes every later one slower
    need = set()
    for addr, data in mem_init:
        a = addr
        while a < addr + len(data):
            base = a & ~(_PAGE - 1)
            if base not in mapped and not (secbase <= base < secbase + max(size, _PAGE)) \
                    and not (_SCRATCH_BASE <= base < _SCRATCH_BASE + _SCRATCH_SIZE):
                need.add(base)
            a = base + _PAGE
    run = None
    for base in sorted(need) + [None]:
        if run and base == run[0] + run[1]:
            run[1] += _PAGE
            continue
        if run:
            uc.mem_map(run[0], run[1])
        run = [base, _PAGE] if base is not None else None
    mapped |= need
    for addr, data in mem_init:
        uc.mem_write(addr, bytes(data))
    return {"uc": uc, "sentinels": sentinel_map, "statics": statics, "mapped": mapped, "slots": slots,
            "init_pages": init_pages, "context": uc.context_save()}


def trace_function(module, func=None, args=None, arity=None, insn_cap=_MAX_INSNS, mem_init=None,
                   stub_externals=None, stub_ret=0, stub_cleanup=0, sig_resolver=None,
                   stub_ret_unique=False, overlay=None, sentinels=None, reads=True, stop_wild=False,
                   quiet_stack=False, reuse=False, fast=False, lean=False, ecx=None, edx=None,
                   pads=None, branches=False, stub_at=None, shadow=None):
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
    branches -- also log each conditional jump of the function itself: which way it went and
               what the comparison before it compared (rec["branches"]; see _branch_hook).
    shadow  -- an emulate/shadow.py Shadow32 kept up beside the run (the path explorer's): stepped
               before each instruction, told of each import or callback answered and of what the
               answer wrote. One instruction at a time (not `fast`); the run is the same run.
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

    secbase = va0 & ~(_PAGE - 1)
    size = (len(code) + (va0 - secbase) + (_PAGE - 1)) & ~(_PAGE - 1)
    mem_init = list(mem_init or [])
    # A machine with the module laid out is kept between runs (reuse=True) and put back exactly
    # as it was after each: laying a module out costs more than most runs of a function do.
    mkey = (path, sha, overlay, bool(stub_externals), bool(fast))
    mach = _MACHINES.pop(mkey, None) if reuse else None
    saved = {} if reuse else None                            # page -> what it held (None: not there before)
    if mach is None:
        mach = _machine(U, path, sha, va0, code, secbase, size, imports, overlay, stub_externals,
                        () if reuse else mem_init)
    uc, sentinel_map, statics = mach["uc"], mach["sentinels"], mach["statics"]
    if stub_at and stub_externals:
        # functions entered as if they were imports: {address: (dll, name)} -- a flat thunk's
        # stubs, which go on into 16-bit code through a pointer only Windows fills in
        sentinel_map = {**sentinel_map, **stub_at}
    mapped = set(mach["mapped"]) if reuse else mach["mapped"]
    init_pages = set(mach["init_pages"])
    for addr, data in mem_init:
        init_pages.update(range(addr >> 12, ((addr + max(len(data), 1) - 1) >> 12) + 1))
    if reuse:
        uc.context_restore(mach["context"])
        again = set()
        for addr, data in mem_init:                          # this run's memory, over the kept machine
            a = addr
            while a < addr + len(data):
                base = a & ~(_PAGE - 1)
                if base not in saved:
                    if base not in mapped and not (secbase <= base < secbase + max(size, _PAGE)) \
                            and not (_SCRATCH_BASE <= base < _SCRATCH_BASE + _SCRATCH_SIZE):
                        uc.mem_map(base, _PAGE); mapped.add(base)
                        saved[base] = None
                    else:
                        saved[base] = bytes(uc.mem_read(base, _PAGE))
                if base in mach["slots"]:
                    again.add(base)
                a = base + _PAGE
            uc.mem_write(addr, bytes(data))
        for base in again:                                   # import slots are written last, as ever
            for a, data in mach["slots"][base]:
                uc.mem_write(a, data)
        top = (_STACK_TOP - 4 * (len(args) + 1)) & ~(_PAGE - 1)
        for base in range(top, _STACK_TOP + 1, _PAGE):
            if base not in saved and base in mapped:
                saved[base] = bytes(uc.mem_read(base, _PAGE))

    # Build the stdcall frame: args in order above a RET sentinel the emulation halts on.
    esp = _STACK_TOP
    for a in reversed(args):
        esp -= 4
        uc.mem_write(esp, int(a & 0xFFFFFFFF).to_bytes(4, "little"))
    esp -= 4
    uc.mem_write(esp, _RET.to_bytes(4, "little"))
    uc.reg_write(UC_X86_REG_ESP, esp)
    # Sentinels in the registers a function must preserve: one that hands a register back changed
    # is caught even though its caller here holds nothing of value there. (A function's callers
    # keep live values in these; a rebuilt body that loses one breaks them, not itself.)
    from unicorn.x86_const import UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP
    for _r, _v in zip((UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP), sentinels or SAVED_SENTINELS):
        uc.reg_write(_r, _v)
    if ecx is not None:                                      # an argument in ECX: a member function's object
        uc.reg_write(UC_X86_REG_ECX, int(ecx) & 0xFFFFFFFF)
    if edx is not None:
        uc.reg_write(UC_X86_REG_EDX, int(edx) & 0xFFFFFFFF)
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
    func_lo, func_hi = fva, va0 + end

    sec_lo, sec_hi = secbase, secbase + max(size, _PAGE)
    fast = bool(fast) and not _GOV["ceil"] and shadow is None
    rec["pages0"] = {} if lean else None

    # The hooks are C++ (native/tracer: cbtrace.dll; emulate/native_trace.py), attached to this
    # Unicorn in trace_function's order, keeping the record -- no Python per instruction, and a
    # shadow (the path explorer's) stepped there too.
    from codebreaker.emulate import native_shadow, native_trace
    native_trace.require()
    nrun = native_trace.Run(
        uc, statics=statics, init_pages=init_pages, mapped=mapped, sentinels=sentinel_map,
        sentinel_from=(mach["sentinels"], stub_at if stub_at and stub_externals else None),
        saved=saved, sha=sha, gov=_GOV, fast=fast, lean=lean, reads=reads,
        quiet=(_STACK_BASE, _STACK_BASE + _STACK_SIZE) if quiet_stack else None, stop_wild=stop_wild,
        stub=bool(stub_externals), unique=stub_ret_unique, branches=branches, pads=pads,
        insn_cap=insn_cap, stub_ret=stub_ret, stub_cleanup=stub_cleanup, func=(func_lo, func_hi),
        sec=(sec_lo, sec_hi), lean_ranges=(_STACK_BASE - 1, _STACK_BASE + _STACK_SIZE),
        sig_resolver=sig_resolver, outs=_outs, cache=mach, shadow=native_shadow.handle_of(shadow))
    start, switched, add_code = (lambda b, u, count=0: nrun.start(uc, b, u, count)), nrun.switched, nrun.attach_code

    def settle():
        nrun.settle(rec)

    try:
        if fast:
            start(fva, _RET)
            if switched():                                   # from here on, one instruction at a time
                add_code()
                uc.ctl_flush_tb()
                start(uc.reg_read(UC_X86_REG_EIP), _RET, count=insn_cap + 1)
        else:
            start(fva, _RET, count=insn_cap + 1)
        settle()
        rec["exit"] = "truncated" if rec["truncated"] else "ret"
        # The emulator can stop without raising (a divide by zero does): it only RETURNED if it
        # stopped at the return sentinel. Anything else is a fault, wherever it stopped.
        if not rec["truncated"] and uc.reg_read(UC_X86_REG_EIP) != _RET:
            rec["exit"] = "exception: stopped at 0x%x" % uc.reg_read(UC_X86_REG_EIP)
        rec["ret"] = uc.reg_read(UC_X86_REG_EAX)
        # The registers a function must hand back unchanged (they are 0 on entry here), and where
        # the stack pointer ended up relative to entry: 4 = cdecl, 4 + N = stdcall with N bytes.
        try:
            from unicorn.x86_const import (UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI,
                                           UC_X86_REG_EBP)
            rec["saved_regs"] = tuple(uc.reg_read(x) for x in (UC_X86_REG_EBX, UC_X86_REG_ESI,
                                                               UC_X86_REG_EDI, UC_X86_REG_EBP))
            rec["esp_delta"] = (uc.reg_read(UC_X86_REG_ESP) - entry_esp) & 0xFFFFFFFF
        except Exception:
            pass
        try:                                                 # x87 return (float functions -> st0)
            from unicorn.x86_const import UC_X86_REG_ST0
            rec["st0"] = float(uc.reg_read(UC_X86_REG_ST0))
        except Exception:
            pass
    except U.UcError as e:
        settle()
        rec["exit"] = "exception: %s" % e
        try:
            rec["ret"] = uc.reg_read(UC_X86_REG_EAX)
        except Exception:
            pass

    nrun.collect(rec)
    # mnemonics keyed by int va -> stringify for JSON friendliness
    rec["mnemonics"] = {("0x%x" % k): v for k, v in rec["mnemonics"].items()}
    if lean:
        rec["writes_seen"] = nrun.lean_seen
    if reuse:                                                # the machine, as it was, for the next run
        try:
            if lean:                                         # (its writes were not followed)
                uc.mem_write(_STACK_BASE, bytes(_STACK_SIZE))
            nrun.detach()
            for base, data in saved.items():
                if data is None:
                    uc.mem_unmap(base, _PAGE)
                else:
                    uc.mem_write(base, data)
            nrun.restore()                                   # (the pages its hooks saved, after Python's)
            while len(_MACHINES) >= _MAX_MACHINES:
                _MACHINES.pop(next(iter(_MACHINES)))
            _MACHINES[mkey] = mach
        except Exception:
            pass                                             # not put back: the next run makes its own
    return rec


_HARNESS = "add byte ptr [eax], al"     # (a call through nothing lands in zeros: the stand-in's write)


def record(rec, sentinels=None):
    """trace_function's record as the one run record (emulate/run.py): EAX (and x87 ST0) the
    results, the imports and callbacks entered with their arguments, EBX ESI EDI EBP against the
    sentinels they were given (`sentinels`, else SAVED_SENTINELS), every instruction run (first
    seen first). A write made on the way out of zeros is the harness's. A lean run's writes carry
    what their bytes held before the run (its pages as first written)."""
    from codebreaker.emulate.run import Call, Run, Write
    ws = rec.get("mem_writes") or []
    mn = rec.get("mnemonics") or {}
    pages = rec.get("pages0")
    writes = []
    if pages is not None:
        for a, n, v, pc in ws:
            old = 0
            for i in range(min(n, 8)):
                b = a + i
                old |= pages[b & ~(_PAGE - 1)][b & (_PAGE - 1)] << (8 * i)
            writes.append(Write(a, n, v, old, bool(pc) and mn.get("0x%x" % pc) == _HARNESS))
    else:
        for w in ws:
            writes.append(Write(w["addr"], w["size"], w["value"] or 0, (w.get("old") or 0) if "old" in w else None,
                                mn.get("0x%x" % w["pc"]) == _HARNESS))
    saved = rec.get("saved_regs")
    preserved = None if saved is None else dict(zip(("EBX", "ESI", "EDI", "EBP"),
                                                    zip(tuple(sentinels or SAVED_SENTINELS), saved)))
    if preserved is not None and len(saved) != len(tuple(sentinels or SAVED_SENTINELS)):
        preserved = None
    return Run(rec.get("exit"), results={"EAX": rec.get("ret"), "ST0": rec.get("st0")}, writes=writes,
               capped=rec.get("writes_seen", len(ws)) >= _MAX_MEM,
               calls=[Call(api, args, None) for api, args in rec.get("api_calls") or ()],
               stack=rec.get("esp_delta"), preserved=preserved,
               path=tuple(int(k, 16) if isinstance(k, str) else k for k in mn),
               steps=rec.get("insn_count"), truncated=rec.get("truncated"), raw=rec)


def recorded(module, **kw):
    """The platform's runner (platforms/win32.py emulator.run): trace_function(module, **kw) as a Run."""
    return record(trace_function(module, **kw), kw.get("sentinels"))


def _builtin_argc(name):
    """Argument count from the knowledge base's win32 api_model (knowledge/models/win32.json:
    documented counts for common APIs, A/W variants sharing one), or None."""
    from codebreaker import knowledge
    n = (name or "")
    model = knowledge.api_model("win32", n)
    if model is None and n and n[-1] in ("A", "W"):          # ANSI/Wide variant
        model = knowledge.api_model("win32", n[:-1])
    return model.get("argc") if model else None
