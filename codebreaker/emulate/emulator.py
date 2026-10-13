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
from codebreaker.base import corpus
from codebreaker.base import settings as paths                          # noqa: E402
from codebreaker.formats.pe import open_pe                 # noqa: E402
from codebreaker.formats.pe_functions import PeSeg               # noqa: E402
from codebreaker.formats.pe_functions import PeCarve           # noqa: E402
from codebreaker.formats.ne import NEFile                     # noqa: E402
import json



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


_RETSENT_BASE = 0xB1B10000                                # unique per-call external-return marker


_INSN = {}                # (module file, its hash) -> {address: (text, is a call, direct target)}


def _memory_hooks(U, rec, mapped, reads=True, static=(), quiet=None, saved=None):
    """The memory side of a run, the same for every processor mode: record reads and writes
    (a write with what was there before, so an unchanged byte can be told from an effect), map
    untouched pages on demand so code that reaches into memory keeps running, and stop a run
    that floods memory or maps without end. -> (hook for accesses, hook for unmapped accesses)"""
    from unicorn.x86_const import UC_X86_REG_EIP
    rec.setdefault("lazy_maps", [])

    def hook_mem(uc, access, address, msize, value, _ud):
        # A single `rep movs/stos/scas` with a huge (often uninitialised) count floods memory within
        # already-mapped pages: ONE retired instruction, so hook_code's instruction cap + governor
        # never fire, but hook_mem fires per element. Bound total mem ops per trace and bail -- a
        # deterministic ceiling (op count, not a clock) so the runaway function is skipped.
        if saved is not None and access == U.UC_MEM_WRITE:  # what the page held, to put back after
            for base in {address & ~(_PAGE - 1), (address + max(msize, 1) - 1) & ~(_PAGE - 1)}:
                if base not in saved:
                    try:
                        saved[base] = bytes(uc.mem_read(base, _PAGE))
                    except Exception:
                        pass                            # not there yet: mapping it notes that
        rec["memops"] = rec.get("memops", 0) + 1
        if rec["memops"] > _MAX_MEMOPS:
            rec["truncated"] = True; rec["governed"] = True; uc.emu_stop(); return
        is_write = access == U.UC_MEM_WRITE
        if not is_write and not reads:
            return                                          # counted above; nobody asked what was read
        if is_write:
            for lo, hi in static:                           # code written over: decode it from memory
                if lo <= address < hi:
                    rec["_dirty"] = True
            # every write counts toward the cap, as if recorded; one into `quiet` (the stack: the
            # function's own frame, most of all writes) is then not looked at any further
            seen = rec["writes_seen"] = rec.get("writes_seen", 0) + 1
            if seen > _MAX_MEM or (quiet and quiet[0] <= address < quiet[1]):
                return
        bucket = rec["mem_writes"] if is_write else rec["mem_reads"]
        if len(bucket) < _MAX_MEM:
            pc = uc.reg_read(UC_X86_REG_EIP)
            old = None
            if is_write:
                v = int(value) & 0xFFFFFFFF
                try:                                        # what was there before (hook runs first)
                    old = int.from_bytes(uc.mem_read(address, min(msize, 8)), "little")
                except Exception:
                    old = 0                                 # a page not mapped yet reads as zeros
            else:
                # Unicorn passes value=0 on a READ, but the bytes being read are already in
                # memory at hook time -- read them back to capture the ACTUAL value observed.
                try:
                    v = int.from_bytes(uc.mem_read(address, min(msize, 8)), "little")
                except Exception:
                    v = None
            bucket.append({"pc": pc, "addr": address, "size": msize, "value": v})
            if old is not None:
                bucket[-1]["old"] = old

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
                if saved is not None and base not in saved:
                    saved[base] = None
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

    return hook_mem, hook_unmapped


def _straight_run(md, image, lo, address, size):
    """(how many instructions, {address: text}, their addresses, is any of them 00 00) for the
    `size` bytes of code at `address`, or False when they do not decode into exactly that."""
    addrs, texts, total, zero = [], {}, 0, False
    for ins in md.disasm(image[address - lo:address - lo + size], address):
        addrs.append(ins.address)
        texts[ins.address] = ("%s %s" % (ins.mnemonic, ins.op_str)).strip()
        total += ins.size
        zero = zero or bytes(ins.bytes) == bytes(2)
    if total != size or not addrs:
        return False
    return len(addrs), texts, tuple(addrs), zero


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
                   quiet_stack=False, reuse=False, fast=False, lean=False,
                   pads=None):
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
    skip = [None]
    fast = bool(fast) and not _GOV["ceil"]

    def hook_code(uc, address, csize, _ud):
        if skip[0] is not None:
            done, skip[0] = skip[0], None
            if done == address:
                return
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
            # (00 00 is a whole instruction, two bytes long: any other length is not it)
            two = b""
            if csize == 2 or not 0 < csize <= 15:
                two = None
                if "_dirty" not in rec:                     # as in the file: no need to ask the emulator
                    for lo, hi, image, _table in statics:
                        if lo <= address < hi - 1:
                            if (address >> 12) not in init_pages and ((address + 1) >> 12) not in init_pages:
                                two = image[address - lo:address - lo + 2]
                            break
                if two is None:
                    two = uc.mem_read(address, 2)
            if two == b"\x00\x00":
                zeros[0] += 1
                if zeros[0] == 2 and stop_wild:
                    # the caller only wants to know THAT this happened: what follows is the
                    # harness's doing, and it is the slowest thing a run can do
                    rec["bogus_stubs"] = 1
                    rec["truncated"] = True
                    uc.emu_stop(); return
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
        # ... and so does reaching a landing pad (`pads`, a range the caller filled its pretend
        # objects' pointers with): a call through a pointer, recorded with WHICH pad it reached,
        # so that a call through the wrong field of an object is a different call.
        hit = sentinel_map.get(address) if stub_externals else None
        if hit is None and pads and pads[0] <= address < pads[1]:
            hit = ("CALLBACK", "%d" % ((address - pads[0]) // 4))
        if hit is not None:
            dll, name = hit
            api = "%s!%s" % (dll, name)
            rec.setdefault("api_deps", [])
            if api not in rec["api_deps"]:
                rec["api_deps"].append(api)
            argc = 0 if dll == "CALLBACK" else sig_resolver(dll, name) if sig_resolver else None
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
            known = info = raw = None
            if "_dirty" not in rec:
                for lo, hi, image, table in statics:
                    if lo <= address < hi:
                        if (address >> 12) not in init_pages and ((address + 14) >> 12) not in init_pages:
                            known, info = table, table.get(address)
                            raw = image[address - lo:address - lo + min(csize or 15, 15)]
                        break
            if info is None:
                try:
                    ins = next(md.disasm(raw if raw is not None else uc.mem_read(address, min(csize or 15, 15)),
                                         address))
                    tgt = None
                    if ins.mnemonic == "call":
                        # static (immediate) target if direct; register/mem calls resolve on next insn
                        import capstone
                        for op in ins.operands:
                            if op.type == capstone.x86.X86_OP_IMM:
                                tgt = op.imm & 0xFFFFFFFF
                    info = (("%s %s" % (ins.mnemonic, ins.op_str)).strip(), ins.mnemonic == "call", tgt)
                except StopIteration:
                    info = ("?", False, None)
                if known is not None:
                    known[address] = info
            rec["mnemonics"][address] = info[0]
            if info[1]:
                tgt = info[2]
                rec["calls"].append({"pc": address, "target": tgt, "indirect": tgt is None,
                                     "into_func": bool(tgt is not None
                                                       and func_lo <= tgt < func_hi)})
                pending[0] = len(rec["calls"]) - 1

    # FAST: told of a whole straight run of instructions at once, not of each. Calling back per
    # instruction is most of what a run costs, and a straight run of the file's own code holds
    # no surprise: its instructions are known before it starts. Anything that is not that -- code
    # not as in the file, a run of zeros, the last steps before the limit -- is handed to
    # hook_code, one instruction at a time, from there to the end of the run (`switch`).
    # What a fast run does not keep: `path` and `calls`.
    seen_blocks = set()
    last = [None, ()]       # the straight run being executed, and the instructions first seen in it
    switch = [False]

    def hook_block(uc, address, size, _ud):
        if switch[0]:
            return
        if (stub_externals and address in sentinel_map) or (pads and pads[0] <= address < pads[1]):
            last[0] = None
            hook_code(uc, address, 2 if address in sentinel_map else 1, None)
            if not rec["truncated"] and uc.reg_read(UC_X86_REG_EIP) == address:
                skip[0] = address                           # not returned from: go on with care
                switch[0] = True
                uc.emu_stop()
            return
        info = None
        if "_dirty" not in rec:
            for lo, hi, image, table in statics:
                if lo <= address and address + size <= hi:
                    if (address >> 12) not in init_pages and ((address + size - 1) >> 12) not in init_pages:
                        info = table.get((address, size))
                        if info is None:
                            info = table[(address, size)] = _straight_run(md, image, lo, address, size)
                    break
        c = rec["insn_count"]
        if not info or info[3] or c + info[0] > insn_cap:
            last[0] = None
            switch[0] = True
            uc.emu_stop()
            return
        zeros[0] = 0
        rec["insn_count"] = c + info[0]
        key = (address, size)
        if key in seen_blocks:
            last[0], last[1] = info, ()
        else:
            seen_blocks.add(key)
            mn, texts = rec["mnemonics"], info[1]
            new = [a for a in info[2] if a not in mn]
            for a in new:
                mn[a] = texts[a]
            last[0], last[1] = info, new

    def settle():
        """A fast run that stopped inside a straight run: the instructions after the one it
        stopped at were counted on entry and did not happen."""
        if fast and not switch[0] and last[0] is not None:
            eip = uc.reg_read(UC_X86_REG_EIP)
            info = last[0]
            if eip in info[1]:
                rec["insn_count"] -= sum(1 for a in info[2] if a > eip)
                for a in last[1]:
                    if a > eip:
                        del rec["mnemonics"][a]
            last[0] = None

    hook_mem, hook_unmapped = _memory_hooks(U, rec, mapped, reads, [(s[0], s[1]) for s in statics],
                                            (_STACK_BASE, _STACK_BASE + _STACK_SIZE) if quiet_stack else None, saved)

    # LEAN: writes kept as (address, size, value, pc) with nothing asked of the emulator for each.
    # What a byte held before the run is read once per page (`pages0`), not once per write; the
    # pc is only asked for where it is used (to tell the harness's own write in a run of zeros,
    # which a fast run never executes); and writes to the stack are not reported at all.
    pages0 = rec["pages0"] = {} if lean else None
    lean_ranges = [(s[0], s[1]) for s in statics]
    lean_seen = [0]

    def hook_write(uc, access, address, msize, value, _ud):
        base = address & ~(_PAGE - 1)
        if base not in pages0 or ((address + msize - 1) & ~(_PAGE - 1)) not in pages0:
            for b in (base, (address + max(msize, 1) - 1) & ~(_PAGE - 1)):
                if b not in pages0:
                    try:
                        held = bytes(uc.mem_read(b, _PAGE))
                        if saved is not None and b not in saved:
                            saved[b] = held
                    except Exception:
                        held = bytes(_PAGE)                  # not there yet: it reads as zeros
                    pages0[b] = held
        for lo, hi in lean_ranges:
            if lo <= address < hi:
                rec["_dirty"] = True
        lean_seen[0] += 1
        if lean_seen[0] <= _MAX_MEM:
            rec["mem_writes"].append((address, msize, int(value) & 0xFFFFFFFF,
                                      uc.reg_read(UC_X86_REG_EIP) if switch[0] or not fast else 0))

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

    hooks = [uc.hook_add(U.UC_HOOK_BLOCK, hook_block) if fast else uc.hook_add(U.UC_HOOK_CODE, hook_code),
             # (reads nobody asked for are not even reported: every repeat of a `rep` instruction
             # comes through hook_code, so the step limit bounds them without a count of accesses)
             uc.hook_add(U.UC_HOOK_MEM_READ | U.UC_HOOK_MEM_WRITE if reads else U.UC_HOOK_MEM_WRITE, hook_mem)
             if not lean else uc.hook_add(U.UC_HOOK_MEM_WRITE, hook_write, None, 0, _STACK_BASE - 1),
             uc.hook_add(U.UC_HOOK_MEM_READ_UNMAPPED | U.UC_HOOK_MEM_WRITE_UNMAPPED
                         | U.UC_HOOK_MEM_FETCH_UNMAPPED, hook_unmapped),
             uc.hook_add(U.UC_HOOK_INTR, hook_intr)]
    if lean:
        hooks.append(uc.hook_add(U.UC_HOOK_MEM_WRITE, hook_write, None, _STACK_BASE + _STACK_SIZE, 0xFFFFFFFF))

    try:
        if fast:
            uc.emu_start(fva, _RET)
            if switch[0]:                                    # from here on, one instruction at a time
                hooks.append(uc.hook_add(U.UC_HOOK_CODE, hook_code))
                uc.ctl_flush_tb()
                uc.emu_start(uc.reg_read(UC_X86_REG_EIP), _RET, count=insn_cap + 1)
        else:
            uc.emu_start(fva, _RET, count=insn_cap + 1)
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

    # mnemonics keyed by int va -> stringify for JSON friendliness
    rec["mnemonics"] = {("0x%x" % k): v for k, v in rec["mnemonics"].items()}
    if lean:
        rec["writes_seen"] = lean_seen[0]
    if reuse:                                                # the machine, as it was, for the next run
        try:
            if lean:                                         # (its writes were not followed)
                uc.mem_write(_STACK_BASE, bytes(_STACK_SIZE))
            for h in hooks:
                uc.hook_del(h)
            for base, data in saved.items():
                if data is None:
                    uc.mem_unmap(base, _PAGE)
                else:
                    uc.mem_write(base, data)
            while len(_MACHINES) >= _MAX_MACHINES:
                _MACHINES.pop(next(iter(_MACHINES)))
            _MACHINES[mkey] = mach
        except Exception:
            pass                                             # not put back: the next run makes its own
    return rec


# Built-in stdcall argument counts for common Win32 APIs. These are public, documented ABI
# facts (the number of dword arguments each API takes) -- not inferences about the binary -- so
# the environment can clean the stack correctly for the frequent cases with no SDK configured.
# Extend as real targets surface more. wsprintf-family (cdecl/variadic) are intentionally absent
# (cdecl cleanup is 0, the default). Keyed by bare name; A/W variants share a count.
_WIN32_ARGC = {
    # kernel32
    "GetLastError": 0, "SetLastError": 1, "GlobalAlloc": 2, "GlobalFree": 1, "GlobalLock": 1,
    "GlobalUnlock": 1, "GlobalReAlloc": 3, "GlobalSize": 1, "GlobalHandle": 1, "GlobalFlags": 1,
    "LocalAlloc": 2, "LocalFree": 1, "LocalLock": 1, "LocalUnlock": 1, "lstrlen": 1, "lstrcpy": 2,
    "lstrcpyn": 3, "lstrcat": 2, "lstrcmp": 2, "lstrcmpi": 2, "GetModuleHandle": 1,
    "GetProcAddress": 2, "LoadLibrary": 1, "LoadLibraryEx": 3, "FreeLibrary": 1, "CloseHandle": 1,
    "GetCurrentProcess": 0, "GetCurrentProcessId": 0, "GetCurrentThreadId": 0, "GetTickCount": 0,
    "Sleep": 1, "GetVersion": 0, "GetVersionEx": 1, "HeapAlloc": 3, "HeapFree": 3, "HeapReAlloc": 4,
    "GetProcessHeap": 0, "MultiByteToWideChar": 6, "WideCharToMultiByte": 8, "GetModuleFileName": 3,
    "OutputDebugString": 1, "InterlockedIncrement": 1, "InterlockedDecrement": 1,
    "InterlockedExchange": 2, "EnterCriticalSection": 1, "LeaveCriticalSection": 1,
    "InitializeCriticalSection": 1, "DeleteCriticalSection": 1, "GetProfileInt": 3,
    "GetPrivateProfileInt": 4, "WritePrivateProfileString": 4, "GetPrivateProfileString": 6,
    "TlsAlloc": 0, "TlsGetValue": 1, "TlsSetValue": 2, "TlsFree": 1, "GetCommandLine": 0,
    # advapi32
    "RegOpenKey": 3, "RegOpenKeyEx": 5, "RegQueryValue": 4, "RegQueryValueEx": 6, "RegCloseKey": 1,
    "RegCreateKey": 3, "RegCreateKeyEx": 9, "RegSetValue": 5, "RegSetValueEx": 6, "RegEnumKey": 4,
    "RegEnumKeyEx": 8, "RegEnumValue": 8, "RegDeleteKey": 2, "RegDeleteValue": 2, "RegFlushKey": 1,
    # user32
    "GetDC": 1, "ReleaseDC": 2, "GetDesktopWindow": 0, "GetSystemMetrics": 1, "LoadString": 4,
    "LoadCursor": 2, "LoadIcon": 2, "MessageBox": 4, "CharNext": 1, "CharPrev": 2, "CharUpper": 1,
    "CharLower": 1, "GetKeyboardLayout": 1, "GetSysColor": 1, "GetWindowDC": 1,
    # gdi32
    "GetStockObject": 1, "DeleteObject": 1, "GetObject": 3, "SelectObject": 2, "DeleteDC": 1,
    "CreateCompatibleDC": 1, "GetDeviceCaps": 2, "SetBkMode": 2, "SetTextColor": 2,
    "CreateCompatibleBitmap": 3,
}


def _builtin_argc(name):
    n = (name or "")
    if n in _WIN32_ARGC:
        return _WIN32_ARGC[n]
    if n and n[-1] in ("A", "W") and n[:-1] in _WIN32_ARGC:   # ANSI/Wide variant
        return _WIN32_ARGC[n[:-1]]
    return None


# ---- GDT / memory layout (linear, 32-bit protected mode) -------------------
GDT_ADDR = 0x1000
SEG_ORIGIN = 0x100000          # first NE segment base; each segment gets a 0x10000 (64K) window
SEG_STRIDE = 0x10000
STACK_BASE = 0x900000          # stack segment base
SENT_BASE = 0xA00000           # import-sentinel segment base (far calls here are stubbed)


def _desc(base, limit, code, dpl=0, big=False):
    """Build one 8-byte GDT descriptor. 16-bit (D/B=0) byte-granular segment by default."""
    d = bytearray(8)
    d[0] = limit & 0xFF
    d[1] = (limit >> 8) & 0xFF
    d[2] = base & 0xFF
    d[3] = (base >> 8) & 0xFF
    d[4] = (base >> 16) & 0xFF
    # access: P | DPL | S(1) | type ; code=0xA(exec/read) data=0x2(read/write)
    d[5] = 0x80 | ((dpl & 3) << 5) | 0x10 | (0xA if code else 0x2)
    # flags nibble (G=0,D/B,L=0,AVL=0) | limit[19:16]
    d[6] = ((0x4 if big else 0x0) << 4) | ((limit >> 16) & 0xF)
    d[7] = (base >> 24) & 0xFF
    return bytes(d)


def _sel(index, rpl=0):
    return (index << 3) | (rpl & 3)


SCRATCH_BASE = 0xB00000        # fallback segment every "unknown" selector points at
_N_GDT = 512                   # dense GDT: fill every selector index so loads/far-rets never #GP


def _build_harness(uc, U, segs):
    """segs: list of (base, limit, is_code). Returns selector list (index 0 = null). Maps the GDT,
    sets GDTR.

    DENSE GDT: real NE/stack/sentinel segments occupy indices 1..k; EVERY other index up to
    _N_GDT is filled with a present descriptor pointing at a shared scratch segment. Win16 code
    routinely `pop ds`/`pop es`/`retf` to selectors we didn't assign (a caller's saved DS, a
    movable-segment selector, a null arg); with a sparse GDT those loads raise #GP and Unicorn
    aborts the whole trace. Making every selector present turns those into harmless
    scratch-segment accesses so the trace continues and the behavior is still captured."""
    try:
        uc.mem_map(SCRATCH_BASE, 0x10000)
    except U.UcError:
        pass                                             # already mapped
    scratch = _desc(SCRATCH_BASE, 0xFFFF, False)         # present, data r/w
    gdt = bytearray(8)                                   # null descriptor at index 0
    sels = [0]
    for (base, limit, is_code) in segs:
        sels.append(_sel(len(sels)))
        gdt += _desc(base, limit, is_code)
    while len(gdt) // 8 < _N_GDT:                         # pad every remaining index with scratch
        gdt += scratch
    uc.mem_map(GDT_ADDR, ((len(gdt) + _PAGE - 1) // _PAGE) * _PAGE)
    uc.mem_write(GDT_ADDR, bytes(gdt))
    from unicorn.x86_const import UC_X86_REG_GDTR
    uc.reg_write(UC_X86_REG_GDTR, (0, GDT_ADDR, len(gdt) - 1, 0))
    return sels


# --------------------------------------------------------------------------
# NE loading + real-function tracing on the validated harness.
# --------------------------------------------------------------------------

_IMPORT_SENT_STRIDE = 0x10                                    # per-import slot inside SENT segment


def _load_ne(uc, U, nef):
    """Map every NE segment into its 64K GDT window, build the GDT (segments + stack + import
    sentinel), and load the segment bytes. Returns (sels, seg_bases, stack_sel, sent_sel).
    sels is indexed by NE segment number (1-based); sels[0] is unused."""
    segs = nef.segments()
    seg_bases = {}
    harness_list = []                                        # (base, limit, is_code) per NE seg
    for s in segs:
        base = SEG_ORIGIN + s["idx"] * SEG_STRIDE
        seg_bases[s["idx"]] = base
        uc.mem_map(base, 0x10000)
        body = nef.data[s["file_off"]:s["file_off"] + s["length"]]
        if body:
            uc.mem_write(base, body)
        harness_list.append((base, 0xFFFF, s["kind"] == "CODE"))
    # stack + import-sentinel segments appended after the NE segments
    uc.mem_map(STACK_BASE, 0x10000)
    uc.mem_map(SENT_BASE, 0x10000)
    harness_list.append((STACK_BASE, 0xFFFF, False))
    harness_list.append((SENT_BASE, 0xFFFF, True))            # code: far calls/rets land here
    sels_all = _build_harness(uc, U, harness_list)
    # sels_all: [null, seg1, seg2, ..., segN, stack, sent]
    sels = {s["idx"]: sels_all[s["idx"]] for s in segs}
    stack_sel = sels_all[len(segs) + 1]
    sent_sel = sels_all[len(segs) + 2]
    return sels, seg_bases, stack_sel, sent_sel


def _import_namer(nef):
    """Return namer(import_record) -> 'MODULE.FUNC' using the NE module-ref table + the Win16
    ordinal->name oracle (symdb, built from the SDK LIB16 libraries). Falls back to
    'MODULE.@ord' when the ordinal isn't in the SDK tables."""
    try:
        modrefs = nef.module_refs()
    except Exception:
        modrefs = []
    ord16 = {}
    try:
        from codebreaker.emulate import emulator as symdb
        ord16 = symdb._load().get("ord16", {})
    except Exception:
        ord16 = {}

    def namer(r):
        mi = r.get("module")
        mod = modrefs[mi - 1] if mi and 0 < mi <= len(modrefs) else "?"
        if r["kind"] == "IMPORTORD":
            o = r.get("ordinal")
            nm = ord16.get(mod.upper(), {}).get(o)
            return "%s.%s" % (mod, nm or ("@%d" % o))
        if r["kind"] == "IMPORTNAME":
            try:
                return "%s.%s" % (mod, nef.imported_name(r.get("name_off", 0)))
            except Exception:
                return "%s.?" % mod
        return "%s.OSFIXUP" % mod
    return namer


def _apply_relocs(uc, nef, sels, seg_bases, sent_sel, imports):
    """Apply per-segment fixups. INTERNAL refs resolve to (selector:offset) in our GDT; external
    IMPORT refs get a unique sentinel far address (sent_sel:slot) recorded in `imports` for
    stubbing. Handles the NE source types we see in practice: 2=SEGMENT(selector), 5=OFFSET(16),
    3=POINTER(far 32), 0=LOBYTE; walks non-additive chains, adds for additive."""
    ents = None
    namer = _import_namer(nef)
    for s in nef.segments():
        base = seg_bases[s["idx"]]
        for r in nef.relocations(s):
            # resolve the target (selector, offset)
            if r["kind"] == "INTERNAL":
                tseg = r["seg"]
                if tseg == 0xFF:                              # moveable -> entry-table ordinal
                    if ents is None:
                        ents = nef.entries()
                    e = ents[r["entry"] - 1] if 0 < r["entry"] <= len(ents) else None
                    if not e or e.get("kind") == "UNUSED":
                        continue
                    tsel = sels.get(e["seg"], 0); toff = e["offset"]
                else:
                    tsel = sels.get(tseg, 0); toff = r["entry"]
            else:                                             # IMPORTORD / IMPORTNAME / OSFIXUP
                slot = len(imports) + 1
                toff = slot * _IMPORT_SENT_STRIDE
                tsel = sent_sel
                imports[toff] = dict(kind=r["kind"], module=r.get("module"),
                                     ordinal=r.get("ordinal"), name_off=r.get("name_off"),
                                     api=namer(r))
            _patch_chain(uc, base, r, tsel, toff)


def _patch_chain(uc, base, r, tsel, toff):
    src = r["src"]
    off = r["offset"]
    def rd16(a): return int.from_bytes(uc.mem_read(a, 2), "little")
    def wr16(a, v): uc.mem_write(a, (v & 0xFFFF).to_bytes(2, "little"))
    def put(a):
        if src == 2:      wr16(a, tsel)                       # SEGMENT selector
        elif src == 5:    wr16(a, toff)                       # OFFSET 16
        elif src == 3:    wr16(a, toff); wr16(a + 2, tsel)    # POINTER far 32 (off:sel)
        elif src == 0:    uc.mem_write(a, bytes([toff & 0xFF]))  # LOBYTE
        else:             wr16(a, toff)                        # best-effort
    if r["additive"]:
        a = base + off
        if src == 2:   wr16(a, (rd16(a) + tsel) & 0xFFFF)
        elif src == 3: wr16(a, (rd16(a) + toff) & 0xFFFF); wr16(a + 2, (rd16(a + 2) + tsel) & 0xFFFF)
        else:          wr16(a, (rd16(a) + toff) & 0xFFFF)
        return
    seen = 0
    while off != 0xFFFF and seen < 4096:                      # walk the fixup chain
        a = base + off
        nxt = rd16(a) if src != 3 else rd16(a)                # next-link is the low 16 bits
        put(a)
        off = nxt
        seen += 1


NEAR_RET = 0xFFEE                 # where a near function entered by the tracer returns to
FAR_RET = 0xFFF0                  # offset in the sentinel segment a far function returns to


def trace_function16(path, ordinal=None, seg=None, offset=None, args=None, arity=0,
                     insn_cap=_MAX_INSNS, import_stub=None, prelude=None, near=False, data_image=None):
    """Trace one exported 16-bit function. Identify it by `ordinal` (entry-table) or explicit
    (seg, offset). `args` are 16-bit Pascal args (pushed left-to-right); a far-RET sentinel sits
    under them. Returns a TraceRecord dict like tracer.trace_function.

    import_stub(hit, uc, U) -> dict|None lets a caller model Win16 API calls (the dynamic NE
    oracle): return {"ax":.., "dx":.., "pop":arg_bytes} to set the DX:AX result and pop the callee's
    Pascal args (keeping the caller's stack balanced). Default (None) keeps the old AX=0, pop-none
    behavior. `hit` carries the resolved "MODULE.FUNC" in hit["api"] plus args_stack.

    near=True enters a NEAR function: only a return offset is pushed, and returning to it ends the
    run. data_image (bytes) is written over the start of the data segment before the run -- used
    to give a rebuilt module the ORIGINAL's data, so both sides start from the same globals."""
    import unicorn as U
    from unicorn.x86_const import (UC_X86_REG_CS, UC_X86_REG_DS, UC_X86_REG_SS, UC_X86_REG_ES,
                                   UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EAX, UC_X86_REG_EDX)
    nef = NEFile(path)
    uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_32)
    sels, seg_bases, stack_sel, sent_sel = _load_ne(uc, U, nef)
    imports = {}
    _apply_relocs(uc, nef, sels, seg_bases, sent_sel, imports)

    if ordinal is not None:
        ents = nef.entries()
        e = ents[ordinal - 1] if 0 < ordinal <= len(ents) else None
        if not e or e.get("kind") == "UNUSED":
            return {"exit": "bad-ordinal", "ordinal": ordinal}
        seg, offset = e["seg"], e["offset"]
    cs_sel = sels[seg]; cs_base = seg_bases[seg]
    auto = nef.h.get("autodata_seg") or 0
    ds_sel = sels.get(auto, stack_sel)                        # DGROUP; fall back to stack
    args = [int(a) & 0xFFFF for a in (args or [0] * arity)]

    def _enter(sseg, soff, sargs):
        """Set up a Pascal far frame for one call: push args left-to-right, then a far-ret sentinel,
        and point CS:IP at (sseg:soff) with DS/ES = DGROUP. Returns the effective code offset."""
        sp = 0xFFF0
        for a in sargs:
            sp = (sp - 2) & 0xFFFF
            uc.mem_write(STACK_BASE + sp, (int(a) & 0xFFFF).to_bytes(2, "little"))
        if near:                                             # near call: just the return offset
            sp = (sp - 2) & 0xFFFF; uc.mem_write(STACK_BASE + sp, NEAR_RET.to_bytes(2, "little"))
        else:
            # a far return goes to a place of its own in the sentinel segment. (It used to go to
            # offset 0 of the function's own segment, which ran whatever was there.)
            sp = (sp - 2) & 0xFFFF; uc.mem_write(STACK_BASE + sp, sent_sel.to_bytes(2, "little"))
            sp = (sp - 2) & 0xFFFF; uc.mem_write(STACK_BASE + sp, FAR_RET.to_bytes(2, "little"))
        entry_sp[0] = sp
        uc.reg_write(UC_X86_REG_SS, stack_sel); uc.reg_write(UC_X86_REG_ESP, sp)
        uc.reg_write(UC_X86_REG_DS, ds_sel); uc.reg_write(UC_X86_REG_ES, ds_sel)
        uc.reg_write(UC_X86_REG_CS, sels[sseg]); uc.reg_write(UC_X86_REG_EIP, soff)
        return soff

    entry_sp = [0]
    if data_image and seg_bases.get(auto) is not None:
        uc.mem_write(seg_bases[auto], bytes(data_image[:0x10000]))
    _enter(seg, offset, args)

    rec = {"schema": "cb.behavior.trace16/1", "module": os.path.basename(path),
           "seg": seg, "offset": offset, "ordinal": ordinal,
           "input": {"args": args, "arity": len(args)}, "insn_count": 0, "path": [],
           "mem_reads": [], "mem_writes": [], "calls": [], "imports_hit": [],
           "truncated": False, "exit": None, "ret": None, "ret32": None}
    base_to_seg = {b: i for i, b in seg_bases.items()}
    rec["layout"] = {"seg_bases": dict(seg_bases), "ds_base": seg_bases.get(auto),
                     "autodata_seg": auto, "seg_stride": SEG_STRIDE,
                     "stack_base": STACK_BASE, "scratch_base": SCRATCH_BASE, "sent_base": SENT_BASE}

    def hook_code(uc, address, size, _ud):
        if rec["insn_count"] >= insn_cap:
            rec["truncated"] = True; uc.emu_stop(); return
        if near and address == cs_base + NEAR_RET:           # the near function returned
            rec["returned"] = True; uc.emu_stop(); return
        if address == SENT_BASE + FAR_RET:                   # the far function returned
            rec["returned"] = True; uc.emu_stop(); return
        rec["insn_count"] += 1
        if len(rec["path"]) < 20000:
            rec["path"].append(address)
        # import sentinel landing: a far call/jmp reached SENT -> stub a far return (AX=0)
        if SENT_BASE <= address < SENT_BASE + 0x10000:
            slot = address - SENT_BASE
            spn = uc.reg_read(UC_X86_REG_ESP) & 0xFFFF
            ip = int.from_bytes(uc.mem_read(STACK_BASE + spn, 2), "little")
            cs = int.from_bytes(uc.mem_read(STACK_BASE + spn + 2, 2), "little")
            # capture the Pascal args pushed above the far-return addr (for caller reconstruction)
            args_stack = [int.from_bytes(uc.mem_read(STACK_BASE + ((spn + 4 + 2 * k) & 0xFFFF), 2),
                                         "little") for k in range(12)]
            hit = dict(imports.get(slot, {"slot": slot}))
            hit["args_stack"] = args_stack
            rec["imports_hit"].append(hit)
            res = import_stub(hit, uc, U) if import_stub else None
            ax = (res or {}).get("ax", 0) & 0xFFFF
            dx = (res or {}).get("dx", 0) & 0xFFFF
            pop = (res or {}).get("pop", 0)
            uc.reg_write(UC_X86_REG_ESP, (spn + 4 + pop) & 0xFFFF)   # far-ret addr + callee's pascal args
            uc.reg_write(UC_X86_REG_EAX, (dx << 16) | ax)
            uc.reg_write(UC_X86_REG_EDX, dx)
            uc.reg_write(UC_X86_REG_CS, cs); uc.reg_write(UC_X86_REG_EIP, ip)

    hook_mem, hook_unmapped = _memory_hooks(U, rec, set())

    uc.hook_add(U.UC_HOOK_CODE, hook_code)
    uc.hook_add(U.UC_HOOK_MEM_READ | U.UC_HOOK_MEM_WRITE, hook_mem)
    uc.hook_add(U.UC_HOOK_MEM_READ_UNMAPPED | U.UC_HOOK_MEM_WRITE_UNMAPPED
                | U.UC_HOOK_MEM_FETCH_UNMAPPED, hook_unmapped)

    # prelude: run setup calls first (sharing this uc's DGROUP state + the import_stub ctx), then
    # discard their trace so only the main function's memory/imports are captured.
    for pc in (prelude or []):
        pseg, poff = pc.get("seg"), pc.get("offset")
        if pc.get("ordinal") is not None:
            pe = nef.entries()[pc["ordinal"] - 1]
            pseg, poff = pe["seg"], pe["offset"]
        _enter(pseg, poff, [int(a) & 0xFFFF for a in pc.get("args", [])])
        try:
            uc.emu_start(poff, SENT_BASE, count=insn_cap + 1)
        except U.UcError:
            pass
    if prelude:
        rec["mem_reads"] = []; rec["mem_writes"] = []; rec["imports_hit"] = []
        rec["path"] = []; rec["insn_count"] = 0; rec["prelude_ran"] = len(prelude)
        _enter(seg, offset, args)

    # snapshot the register file the main function sees at entry (the implicit inputs behind
    # Ghidra's in_/unaff_ pseudo-vars). Meaningful for an in-context run (real caller via prelude).
    from unicorn.x86_const import (UC_X86_REG_EBX, UC_X86_REG_ECX, UC_X86_REG_ESI, UC_X86_REG_EDI,
                                   UC_X86_REG_EBP)
    rec["entry_regs"] = {
        "AX": uc.reg_read(UC_X86_REG_EAX) & 0xFFFF, "BX": uc.reg_read(UC_X86_REG_EBX) & 0xFFFF,
        "CX": uc.reg_read(UC_X86_REG_ECX) & 0xFFFF, "DX": uc.reg_read(UC_X86_REG_EDX) & 0xFFFF,
        "SI": uc.reg_read(UC_X86_REG_ESI) & 0xFFFF, "DI": uc.reg_read(UC_X86_REG_EDI) & 0xFFFF,
        "BP": uc.reg_read(UC_X86_REG_EBP) & 0xFFFF, "CS": uc.reg_read(UC_X86_REG_CS),
        "DS": uc.reg_read(UC_X86_REG_DS), "ES": uc.reg_read(UC_X86_REG_ES),
        "SS": uc.reg_read(UC_X86_REG_SS)}

    try:
        uc.emu_start(offset, SENT_BASE, count=insn_cap + 1)
        rec["exit"] = "truncated" if rec["truncated"] else "ret" if rec.get("returned") else "stopped"
    except U.UcError as e:
        rec["exit"] = "exception: %s" % e
        # fault detail: disassemble the instruction at the faulting CS:EIP (helps triage which
        # 16-bit construct the segmented emulator still can't handle).
        try:
            import capstone
            feip = uc.reg_read(UC_X86_REG_EIP) & 0xFFFF
            fcs = uc.reg_read(UC_X86_REG_CS)
            csbase = next((b for sidx, b in seg_bases.items() if sels.get(sidx) == fcs), None)
            if csbase is not None:
                fb = bytes(uc.mem_read(csbase + feip, 16))
                ins = next(capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16).disasm(fb, feip), None)
                rec["fault"] = {"cs": fcs, "eip": feip,
                                "insn": ("%s %s" % (ins.mnemonic, ins.op_str)).strip() if ins else None,
                                "bytes": fb[:8].hex()}
        except Exception:
            pass
    rec["ret"] = uc.reg_read(UC_X86_REG_EAX) & 0xFFFF
    # how far the stack pointer moved: 2 or 4 for the return address, plus any arguments removed
    rec["sp_delta"] = ((uc.reg_read(UC_X86_REG_ESP) & 0xFFFF) - entry_sp[0]) & 0xFFFF
    rec["code_base"] = cs_base
    from unicorn.x86_const import UC_X86_REG_ESI as _SI, UC_X86_REG_EDI as _DI
    rec["exit_regs"] = {"SI": uc.reg_read(_SI) & 0xFFFF, "DI": uc.reg_read(_DI) & 0xFFFF,
                        "DS": uc.reg_read(UC_X86_REG_DS)}
    rec["ret32"] = ((uc.reg_read(UC_X86_REG_EDX) & 0xFFFF) << 16) | rec["ret"]
    rec["n_imports"] = len(imports)
    return rec


ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)


HEAP_BASE = 0xB00000        # first far-heap block (distinct from seg/stack/scratch/sentinel windows)
HEAP_STRIDE = 0x10000
SAFE_ARG = _sel(200)      # a selector that names a real, writable scratch segment

                            # returns to the sentinel regardless, so surplus args are harmless


class _Ctx(object):
    """Per-run oracle state: a far-heap allocator that installs real GDT descriptors so GlobalAlloc
    returns genuinely distinct far selectors (not the shared scratch pad), and fed file input."""
    def __init__(self, uc, U):
        self.uc, self.U = uc, U
        self.n = 0
        self.heaps = {}                    # selector -> (base, size)
        self.feed = b""                    # bytes handed to _lread
        self.feed_pos = 0

    def alloc_heap(self, size=0x10000):
        # take a GDT index near the top (the harness padded those with scratch; overwrite with a
        # real data descriptor pointing at a fresh linear window) and map that window.
        idx = _N_GDT - 8 - self.n
        base = HEAP_BASE + self.n * HEAP_STRIDE
        self.n += 1
        try:
            self.uc.mem_map(base, ((size + 0xFFF) // 0x1000) * 0x1000)
        except self.U.UcError:
            pass
        self.uc.mem_write(GDT_ADDR + idx * 8, bytes(_desc(base, 0xFFFF, False)))
        sel = _sel(idx)
        self.heaps[sel] = (base, size)
        return sel


# Well-known Win16 KERNEL.EXE ordinals -- fallback when the SDK ordinal oracle (symdb) doesn't
# resolve them (it can come back as 'KERNEL.@15'). These are the stable KERNEL export ordinals.
KERNEL_ORD = {
    15: "GLOBALALLOC", 16: "GLOBALREALLOC", 17: "GLOBALFREE", 18: "GLOBALLOCK",
    19: "GLOBALUNLOCK", 20: "GLOBALSIZE", 21: "GLOBALHANDLE", 22: "GLOBALFLAGS",
    23: "LOCKSEGMENT", 24: "UNLOCKSEGMENT", 26: "GLOBALDOSALLOC",
    81: "_LCLOSE", 82: "_LREAD", 83: "_LCREAT", 84: "_LLSEEK", 85: "_LOPEN", 86: "_LWRITE",
}


def _api(hit):
    """Bare API function name, upper-cased (drop the 'MODULE.' prefix); resolve KERNEL @ordinals."""
    api = (hit.get("api") or "").upper()
    fn = api.split(".", 1)[1] if "." in api else api
    if fn.startswith("@") and api.startswith("KERNEL"):
        try:
            fn = KERNEL_ORD.get(int(fn[1:]), fn)
        except ValueError:
            pass
    return fn


def default_stub(hit, uc, U, ctx):
    """Model the Win16 APIs an LZ-class module calls. Returns {ax, dx, pop}. `pop` = total Pascal
    arg bytes the callee removes (WORD=2, DWORD / far ptr = 4)."""
    fn = _api(hit)
    a = hit.get("args_stack", [])
    if fn in ("GLOBALALLOC", "GLOBALDOSALLOC"):        # (wFlags:W, dwBytes:D) -> HGLOBAL(sel)
        return {"ax": ctx.alloc_heap(), "pop": 6}
    if fn in ("GLOBALLOCK",):                            # (hMem:W) -> far ptr sel:0000  (DX:AX)
        return {"ax": 0, "dx": a[0] if a else 0, "pop": 2}
    if fn in ("GLOBALFREE", "GLOBALUNLOCK", "GLOBALHANDLE"):
        return {"ax": 0, "pop": 2}
    if fn in ("_LOPEN", "_LCREAT"):                      # -> HFILE (pretend a valid handle)
        return {"ax": 0x0080, "pop": 6}
    if fn in ("_LCLOSE",):
        return {"ax": 0, "pop": 2}
    if fn in ("_LLSEEK",):                               # (hf:W, off:D, org:W) -> LONG new pos
        return {"ax": (a[1] if len(a) > 1 else 0), "dx": (a[2] if len(a) > 2 else 0), "pop": 8}
    if fn in ("_LREAD", "_HREAD"):                       # (hf:W, buf:far, cb:W) -> bytes read
        cb = a[3] if len(a) > 3 else 0
        return {"ax": cb, "pop": 8}
    if fn in ("_LWRITE", "_HWRITE"):
        cb = a[3] if len(a) > 3 else 0
        return {"ax": cb, "pop": 8}
    return {"ax": 0}                                     # unknown: benign 0 (stack may drift)


def probe(dll, ordinal=None, seg=None, offset=None, args=None, stub=default_stub, insn_cap=None,
          prelude=None, cdecl=False, near=False, data_image=None):
    """Run one NE function under the controlled Win16 environment. Returns the tracer16 TraceRecord
    (with rec['layout'] + rec['imports_hit'] + rec['mem_*'] + rec['ret']).

    cdecl=True reverses `args` before pushing: the tracer pushes Pascal order (left-to-right), but
    INTERNAL functions are often __cdecl16far (args pushed right-to-left), so their args must be
    reversed to land in the right slots (this is how the LZDecompress reset path was reached)."""
    if cdecl and args:
        args = list(reversed(args))
    ctx_box = {}

    def _stub(hit, uc, u):
        if "ctx" not in ctx_box:
            ctx_box["ctx"] = _Ctx(uc, u)
        res = stub(hit, uc, u, ctx_box["ctx"]) or {}
        if "pop" not in res:                                 # remove the arguments, as the real one would
            from codebreaker.symbols import api_arguments as api16
            size = api16.of(hit.get("api"))
            hit["arg_bytes"] = size
            if size:
                res = dict(res, pop=size)
        else:
            hit["arg_bytes"] = res["pop"]
        return res

    kw = {}
    if insn_cap:
        kw["insn_cap"] = insn_cap
    rec = trace_function16(dll, ordinal=ordinal, seg=seg, offset=offset,
                             args=args, arity=len(args or []), import_stub=_stub,
                             prelude=prelude, near=near, data_image=data_image, **kw)
    rec["heaps"] = {"base": HEAP_BASE, "stride": HEAP_STRIDE}
    ctx = ctx_box.get("ctx")
    rec["heap_sels"] = {s: b for s, (b, _sz) in ctx.heaps.items()} if ctx else {}
    return rec


_REF = ("docs", "reference")
_CACHE = {}


def _find(fname):
    """Locate a reference JSON across the repo/data split. Resolves against the active data corpus
    (paths.data_root / WT_DATA_ROOT) and the cwd -- no in-repo corpus copy."""
    from codebreaker.base import settings as paths
    env = os.environ.get("WT_DATA_ROOT")
    roots = []
    if env:
        roots += [env, os.path.join(env, "win95")]
    try:
        roots.append(paths.data_root())
    except Exception:
        pass
    roots += [os.getcwd(), os.path.join(os.getcwd(), "win95")]
    for r in roots:
        p = os.path.join(r, *_REF, fname)
        if os.path.exists(p):
            return p
    return None


def _load_symbols():
    if _CACHE:
        return _CACHE
    sdk = {}
    p = _find("sdk_symbols.json")
    if p:
        try:
            sdk = json.load(open(p, encoding="utf-8"))
        except (OSError, ValueError):
            sdk = {}
    cv = {}
    p = _find("build311_codeview_syms.json")
    if p:
        try:
            cv = json.load(open(p, encoding="utf-8"))
        except (OSError, ValueError):
            cv = {}
    # flatten: name -> arity (from lib @N across all modules), and the codeview name set
    arities = {}
    for dll, syms in sdk.get("32", {}).items():
        for name, a in syms.items():
            if isinstance(a, int) and name not in arities:
                arities[name] = a
    cvnames = set()
    for _mod, names in cv.items():
        cvnames.update(names)
    proto = sdk.get("proto", {})
    arity_lib = arities
    # 16-bit OMF IMPDEF map carries ordinals: {MODULE: {name: ordinal_or_expname}}. Invert to
    # {MODULE: {ordinal: name}} so an ordinal-only export can be named (the 32-bit COFF libs are
    # by-name, so they contribute no ordinal map -- 32-bit exports are named from the PE table).
    ord16 = {}
    for mod, syms in sdk.get("16", {}).items():
        inv = {}
        for name, ov in syms.items():
            if isinstance(ov, int):
                inv.setdefault(ov, name)
        if inv:
            ord16[mod.upper()] = inv
    # case-insensitive index: 16-bit export names are UPPERCASE, SDK names are proper-case
    ci = {k.lower(): k for k in proto}
    ci_arity = {k.lower(): k for k in arity_lib}
    ci_cv = {n.lower() for n in cvnames}
    _CACHE.update(proto=proto, struct=sdk.get("struct", {}), arity_lib=arity_lib,
                  cvnames=cvnames, ci=ci, ci_arity=ci_arity, ci_cv=ci_cv, ord16=ord16)
    return _CACHE


def struct(name):
    return _load_symbols()["struct"].get(name)
