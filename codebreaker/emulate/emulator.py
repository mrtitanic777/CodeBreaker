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


_RETSENT_BASE = 0xB1B10000                                # unique per-call external-return marker
_OUT_HANDLE = 0x0000A100                                  # the handle a stubbed API gives back (+ 4 x its parameter's place)
_HANDLE_OUT_TYPES = ("PHKEY", "PHANDLE", "LPHANDLE")
_NUMBER_OUT_TYPES = ("LPDWORD", "PDWORD", "LPLONG", "PLONG", "PULONG", "LPUINT", "PUINT")
_OUTS = {}


def _callee_pops(uc, ret):
    """Bytes of arguments a call that returns to `ret` leaves for its callee to take off: the
    pushes just before the call -- read back from the code before `ret`, the decoding that ends
    exactly on the call -- unless the caller takes them off itself (`add esp, N` at `ret`). 0
    when the code does not say."""
    try:
        after = bytes(uc.mem_read(ret, 3))
        if after[:2] in (bytes((0x83, 0xC4)), bytes((0x81, 0xC4))):   # add esp, imm: the caller's
            return 0
        before = bytes(uc.mem_read(ret - 64, 64))
    except Exception:
        return 0
    md = _md()
    for start in range(0, 60):
        insns = list(md.disasm(before[start:], ret - 64 + start))
        if not insns or insns[-1].address + insns[-1].size != ret or insns[-1].mnemonic != "call":
            continue
        pushed = 0
        for ins in reversed(insns[:-1]):
            if ins.mnemonic == "push":
                pushed += 4
                continue
            if ins.mnemonic in ("call", "ret", "pop", "leave", "enter", "pushal", "popal") or                     ins.mnemonic.startswith("j") or ins.op_str.startswith("esp"):
                break
        return pushed if pushed <= 64 else 0
    return 0


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


def _memory_hooks(U, rec, mapped, reads=True, static=(), quiet=None, saved=None, fill=False):
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
            bucket.append({"pc": pc, "addr": address, "size": msize, "value": v, "n": rec.get("insn_count", 0)})
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
                if fill:                                    # (a page no one set: a pattern made from
                    uc.mem_write(base, _pattern(base))      # the address, not zeros -- see _pattern)
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


def _pattern(base):
    """What a page nothing set holds, when a run asks for one filled: every byte made from its
    own address, the same on any build. Zeros hid a C function reading the wrong field of a
    caller's structure -- zero there, zero where it should have read, the same either way.
    (Made once per page and kept: a gate run maps the same pages thousands of times.)"""
    got = _PATTERNS.get(base)
    if got is None:
        if len(_PATTERNS) > 4096:
            _PATTERNS.clear()
        got = _PATTERNS[base] = bytes(((base + i) * 0x9E3779B1 >> 13) & 0xFF for i in range(_PAGE))
    return got


_PATTERNS = {}                       # page -> _pattern(page)


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


def _decode_branchy(uc, md, sha, address):
    key = (sha, address)
    if key not in _DECODED:
        from capstone import x86_const as X
        out = ("other", None, (), None)
        try:
            ins = next(md.disasm(bytes(uc.mem_read(address, 16)), address, 1))
            mn = ins.mnemonic

            def operand(op):
                if op.type == X.X86_OP_REG:
                    name = ins.reg_name(op.reg)
                    return ("reg", (_uc_reg(name), _PARENT.get(name, name)), op.size)
                if op.type == X.X86_OP_IMM:
                    return ("imm", op.imm, op.size)
                if op.type == X.X86_OP_MEM and op.mem.segment == 0:
                    m = op.mem
                    return ("mem", (_uc_reg(ins.reg_name(m.base)) if m.base else None,
                                    _uc_reg(ins.reg_name(m.index)) if m.index else None, m.scale, m.disp), op.size)
                return None
            try:
                written = tuple({_PARENT.get(ins.reg_name(r), ins.reg_name(r)) for r in ins.regs_access()[1]})
            except Exception:
                written = ()
            load = None
            if mn in ("mov", "movzx", "movsx") and len(ins.operands) == 2 and ins.operands[0].type == X.X86_OP_REG \
                    and ins.operands[1].type == X.X86_OP_MEM:
                src = operand(ins.operands[1])
                if src:
                    load = (_PARENT.get(ins.reg_name(ins.operands[0].reg)), src, ins.operands[1].size)
            if mn in _FLAGLESS or mn.startswith(("cmov", "set")):
                out = ("keep", None, written, load)
            elif mn.startswith("j") and mn != "jmp" and ins.operands and ins.operands[0].type == X.X86_OP_IMM:
                out = ("jcc", (ins.operands[0].imm & 0xFFFFFFFF, address + ins.size), (), None)
            elif mn in _COMPARES and len(ins.operands) in (1, 2):
                ops = [operand(op) for op in ins.operands]
                if all(o is not None and (o[0] != "reg" or o[1][0] is not None) for o in ops):
                    if len(ops) == 1:                        # dec/inc: compared with what it becomes
                        ops.append(("imm", 1 if mn == "dec" else -1, ops[0][2]))
                    out = ("cmp", (mn, ops, ops[0][2]), written, None)
                else:
                    out = ("other", None, written, None)
            else:
                out = ("other", None, written, None)
        except Exception:
            pass
        _DECODED[key] = out
    return _DECODED[key]


def _uc_reg(name):
    from unicorn import x86_const
    return getattr(x86_const, "UC_X86_REG_" + name.upper(), None)


def _address(uc, mem):
    base, index, scale, disp = mem
    return (disp + (uc.reg_read(base) if base else 0) + (uc.reg_read(index) * scale if index else 0)) & 0xFFFFFFFF


def _operand_value(uc, op, loaded):
    """(value, the address it came from or None): a register's is where it was last loaded from,
    if it was loaded from memory in this function and not written since (`loaded`)."""
    kind, what, size = op
    mask = (1 << (8 * size)) - 1
    if kind == "imm":
        return what & mask, None
    if kind == "reg":
        at = loaded.get(what[1])
        return uc.reg_read(what[0]) & mask, (at if at and at[1] == size else (None, 0))[0]
    a = _address(uc, what)
    return int.from_bytes(bytes(uc.mem_read(a, size)), "little"), a


def _branch_hook(rec, md, sha, lo, hi):
    """A code hook for [lo, hi): appends (jump address, taken, compare) to rec["branches"], where
    compare is (mnemonic, size, first operand's value, second's, the address the first came from
    or None, the second's) when the last instruction before the jump that set the flags compared
    two values, else None. A register operand's address is the memory it was loaded from."""
    log = rec["branches"] = []
    state = [None, None]                                     # [pending jump, compare in force]
    loaded = {}                                              # 32-bit register -> (address, size) it was loaded from

    def hook(uc, address, _size, _ud):
        pend = state[0]
        if pend is not None:
            state[0] = None
            if len(log) < _BRANCH_CAP and address in (pend[1], pend[2]):
                log.append((pend[0], address == pend[1], pend[3]))
        kind, detail, written, load = _decode_branchy(uc, md, sha, address)
        if kind == "cmp":
            try:
                (a, at_a), (b, at_b) = (_operand_value(uc, detail[1][0], loaded),
                                        _operand_value(uc, detail[1][1], loaded))
                state[1] = (detail[0], detail[2], a, b, at_a, at_b)
            except Exception:
                state[1] = None
        elif kind == "jcc":
            if lo <= detail[0] < hi and detail[0] != detail[1]:
                state[0] = (address, detail[0], detail[1], state[1])
        elif kind == "other":
            state[1] = None
        for r in written:
            loaded.pop(r, None)
        if load is not None and load[0]:
            try:
                loaded[load[0]] = (_address(uc, load[1][1]), load[2])
            except Exception:
                pass
    return hook


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
    md = _md()
    func_lo, func_hi = fva, va0 + end

    sec_lo, sec_hi = secbase, secbase + max(size, _PAGE)
    pending = [None]        # index of a call whose actual destination is the next instruction
    zeros = [0]             # consecutive 00-00 ("add [eax],al") instructions -> crawling zeros
    skip = [None]
    fast = bool(fast) and not _GOV["ceil"] and shadow is None

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
                    if shadow is not None:
                        shadow.after_call(uc, False, 0)
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
            if not argc:
                # A callback, or an import nothing here gives the argument count of (a driver's
                # NDIS.SYS calls), removes what its call site pushed when its caller does not take
                # them off (as Windows' callbacks and stdcall imports do). Left on the stack, the
                # original got over it (its frame pointer puts ESP back) and the rebuild, keeping
                # no frame pointer, returned to whatever was there: pjlmon's sub_001D2A, to 0.
                # (Only removed: what each side recorded of the call stays as it was -- worked out
                # from each side's own code, the two would not record alike.)
                try:
                    cleanup = _callee_pops(uc, int.from_bytes(uc.mem_read(uc.reg_read(UC_X86_REG_ESP), 4), "little")) or cleanup
                except Exception:
                    pass
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
                # What the API gives back through a pointer -- a handle (RegOpenKeyExA's PHKEY), a
                # number (RegQueryInfoKeyA's count of subkeys) -- is given: the same on every run,
                # so the caller goes on with what it would have from Windows. Left out, each build
                # went on with whatever its own stack held there (shlwapi: a stale return address
                # against 0).
                if argc and dll != "CALLBACK":
                    for i, given in _outs(name):
                        if i >= argc:
                            continue
                        p = int.from_bytes(uc.mem_read(esp_now + 4 + 4 * i, 4), "little")
                        if p:
                            try:
                                uc.mem_write(p, (given if given is not None else _OUT_HANDLE + 4 * i).to_bytes(4, "little"))
                                if shadow is not None:
                                    shadow.written(p, 4)
                            except Exception:
                                pass
                # unique per-call return marker (interproc v2 value-flow), else the fixed stub_ret
                nstub = rec.get("stubbed_externals", 0)
                rv = (_RETSENT_BASE + nstub) if stub_ret_unique else stub_ret
                uc.reg_write(UC_X86_REG_EAX, rv & 0xFFFFFFFF)
                uc.reg_write(UC_X86_REG_EIP, ret_addr)
                rec["stubbed_externals"] = nstub + 1
                if shadow is not None:                       # (its result: stub_ret, an input of the run)
                    shadow.after_call(uc, not stub_ret_unique, rv)
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
        if shadow is not None:
            shadow.step(uc, address)

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

    # NATIVE (emulate/native_trace.py, CB_NATIVE_TRACE=0 to turn off): the same hooks in C++,
    # attached in the same order, keeping the same record -- no Python per instruction. A run
    # kept beside a shadow stays here.
    nrun = None
    if shadow is None and not os.environ.get("WT_DBG"):
        from codebreaker.emulate import native_trace
        if native_trace.enabled():
            nrun = native_trace.Run(
                uc, statics=statics, init_pages=init_pages, mapped=mapped, sentinels=sentinel_map,
                sentinel_from=(mach["sentinels"], stub_at if stub_at and stub_externals else None),
                saved=saved, sha=sha, gov=_GOV, fast=fast, lean=lean, reads=reads,
                quiet=(_STACK_BASE, _STACK_BASE + _STACK_SIZE) if quiet_stack else None, stop_wild=stop_wild,
                stub=bool(stub_externals), unique=stub_ret_unique, branches=branches, pads=pads,
                insn_cap=insn_cap, stub_ret=stub_ret, stub_cleanup=stub_cleanup, func=(func_lo, func_hi),
                sec=(sec_lo, sec_hi), lean_ranges=(_STACK_BASE - 1, _STACK_BASE + _STACK_SIZE),
                sig_resolver=sig_resolver, outs=_outs, cache=mach)
    if nrun is not None:
        hooks = []
        start, switched, add_code = (lambda b, u, count=0: nrun.start(uc, b, u, count)), nrun.switched, nrun.attach_code

        def settle():
            nrun.settle(rec)
    else:
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
        if branches:
            hooks.append(uc.hook_add(U.UC_HOOK_CODE, _branch_hook(rec, md, sha, func_lo, func_hi), None,
                                     func_lo, func_hi - 1))
        start, switched = uc.emu_start, (lambda: switch[0])

        def add_code():
            hooks.append(uc.hook_add(U.UC_HOOK_CODE, hook_code))

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

    if nrun is not None:
        nrun.collect(rec)
    # mnemonics keyed by int va -> stringify for JSON friendliness
    rec["mnemonics"] = {("0x%x" % k): v for k, v in rec["mnemonics"].items()}
    if lean:
        rec["writes_seen"] = nrun.lean_seen if nrun is not None else lean_seen[0]
    if reuse:                                                # the machine, as it was, for the next run
        try:
            if lean:                                         # (its writes were not followed)
                uc.mem_write(_STACK_BASE, bytes(_STACK_SIZE))
            for h in hooks:
                uc.hook_del(h)
            if nrun is not None:
                nrun.detach()
            for base, data in saved.items():
                if data is None:
                    uc.mem_unmap(base, _PAGE)
                else:
                    uc.mem_write(base, data)
            if nrun is not None:                             # (the pages its hooks saved, after Python's)
                nrun.restore()
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
