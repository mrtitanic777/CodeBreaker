"""windows16.py -- run a function of a 16-bit Windows module.

    from codebreaker.emulate import windows16
    r = windows16.probe("commdlg.dll", seg=1, offset=0x1234, args=[...], near=False)

The module's segments are laid out in protected mode, each behind a selector of its own, with
its relocations applied; every import is a far address in a segment of landing places, so a
call to Windows is seen, named, and returned from with the arguments it was given. The function
is entered the way it is really called (far or near). Memory is kept by the same hooks as the
32-bit emulator's (emulator._memory_hooks). What a run did is judged in verify/lanes/win16.py.
"""
import hashlib
import os
import sys
from codebreaker.emulate import fast_uc
from codebreaker.emulate.emulator import _MAX_INSNS, _PAGE, _memory_hooks
from codebreaker.formats.ne import NEFile
import json


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
_FIXED_SLOT = 101              # the stack's GDT index (the sentinel's next): the same for any module
_MODULE_SLOT = 300             # the module's segment n is at GDT index _MODULE_SLOT + n - 1


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


def segment_base(idx):
    """Where the module's segment `idx` (1-based) is laid out: its own 64K window."""
    return SEG_ORIGIN + idx * SEG_STRIDE


def selector_base(uc, sel):
    """The base the descriptor for `sel` gives, read from the GDT as it is now (0 past its end)."""
    try:
        d = bytes(uc.mem_read(GDT_ADDR + (sel >> 3) * 8, 8))
    except Exception:
        return 0
    return d[2] | d[3] << 8 | d[4] << 16 | d[7] << 24


def _load_ne(uc, U, nef):
    """Map every NE segment into its 64K GDT window, build the GDT (segments + stack + import
    sentinel), and load the segment bytes. Returns (sels, seg_bases, stack_sel, sent_sel).
    sels is indexed by NE segment number (1-based); sels[0] is unused."""
    segs = nef.segments()
    seg_bases = {}
    # The module's segments sit high in the GDT, past every selector a plain number given as an
    # input names (20h, 60h, 100h ...): at index n those numbers were selectors of the module's
    # own code, and a function handed one as a far pointer read its own instructions -- which
    # differ between the original and a rebuild, so 17 of USER's functions "differed" by it.
    harness_list = [(SCRATCH_BASE, 0xFFFF, False)] * (_MODULE_SLOT - 1)
    for s in segs:
        base = segment_base(s["idx"])
        seg_bases[s["idx"]] = base
        uc.mem_map(base, 0x10000)
        body = nef.data[s["file_off"]:s["file_off"] + s["length"]]
        if body:
            uc.mem_write(base, body)
        harness_list.append((base, 0xFFFF, s["kind"] == "CODE"))
    # stack + import-sentinel segments, at the same selectors whatever the module's segment
    # count: a build with another number of segments had another stack selector, and a word
    # equal to one side's SS (a loop counter at 170h) was "SS" there and a number on the other
    # -- USER's sub_35_2008 and five more "differed"
    uc.mem_map(STACK_BASE, 0x10000)
    uc.mem_map(SENT_BASE, 0x10000)
    try:
        uc.mem_map(SCRATCH_BASE, 0x10000)
    except U.UcError:
        pass
    harness_list[_FIXED_SLOT - 1] = (STACK_BASE, 0xFFFF, False)
    harness_list[_FIXED_SLOT] = (SENT_BASE, 0xFFFF, True)    # code: far calls/rets land here
    if len(harness_list) > _N_GDT - 24:                      # (the top indices are the heaps')
        raise ValueError("%d segments: more than the GDT holds" % len(segs))
    sels_all = _build_harness(uc, U, harness_list)
    # sels_all: [null, scratch..., stack, sent, scratch..., seg1, ..., segN]
    sels = {s["idx"]: sels_all[_MODULE_SLOT + s["idx"] - 1] for s in segs}
    stack_sel = sels_all[_FIXED_SLOT]
    sent_sel = sels_all[_FIXED_SLOT + 1]
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


def _kernel_constant(api):
    """The value of one of KERNEL's exports that is a plain number, or None."""
    mod, _, fn = (api or "").upper().partition(".")
    if mod != "KERNEL":
        return None
    return {"@113": 3, "__AHSHIFT": 3, "@114": 8, "__AHINCR": 8}.get(fn)


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
            elif r["kind"] != "OSFIXUP" and r["src"] == 5 and _kernel_constant(namer(r)) is not None:
                # KERNEL's plain numbers (__AHSHIFT 3, __AHINCR 8): written in as the loader does
                tsel, toff = 0, _kernel_constant(namer(r))
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


ENTRY_AX = 0x7E57                 # AX as a function is entered: handed back, it set no result
NEAR_RET = 0xFFEE                 # where a near function entered by the tracer returns to
FAR_RET = 0xFFF0                  # offset in the sentinel segment a far function returns to


_RELOCATED = {}
_LOADED = {}                   # (path, size, mtime) -> (NEFile, {base: fixed-up bytes}, imports)
_TAILS = {}


def relocated_data(path):
    """The module's data segment as the loader leaves it -- its fix-ups applied (the selectors
    of far pointers kept in it) -- the bytes a rebuilt module is given in place of its own, so
    both sides start from the same globals. The file's bytes, unfixed, made every far pointer
    in the data a different one on the build's side: run against itself, 23 of USER's 1,613
    functions "differed". -> bytes (empty when it has no data segment)"""
    if path in _RELOCATED:
        return _RELOCATED[path]
    import unicorn as U
    nef = NEFile(path)
    auto = nef.h.get("autodata_seg") or 0
    seg = next((s for s in nef.segments() if s["idx"] == auto), None)
    out = b""
    if seg and seg["file_off"]:
        uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_32)
        sels, seg_bases, _stack, sent_sel = _load_ne(uc, U, nef)
        _apply_relocs(uc, nef, sels, seg_bases, sent_sel, {})
        out = bytes(uc.mem_read(seg_bases[auto], seg["length"]))
    _RELOCATED[path] = out
    return out


# Branch log (trace_function16(branches=...)): the 16-bit form of emulator._branch_hook -- for each
# conditional jump of the function itself, which way it went and what the comparison before it
# compared, for verify/guided.py. What differs from 32-bit code: an instruction is decoded at its
# offset in its segment (a jump's target is an offset), and a memory operand is an offset in a
# segment -- DS, SS for one based on BP, or the one it names -- whose base is read from the GDT.
_DECODED16 = {}                      # (code bytes, offset) -> as emulator._decode_branchy's


def _decode16(md, raw, off):
    key = (raw, off)
    if key not in _DECODED16:
        from capstone import x86_const as X
        from codebreaker.emulate.emulator import _COMPARES, _FLAGLESS, _PARENT, _uc_reg
        out = ("other", None, (), None)
        try:
            ins = next(md.disasm(raw, off, 1))
            mn = ins.mnemonic

            def operand(op):
                if op.type == X.X86_OP_REG:
                    name = ins.reg_name(op.reg)
                    return ("reg", (_uc_reg(name), _PARENT.get(name, name)), op.size)
                if op.type == X.X86_OP_IMM:
                    return ("imm", op.imm, op.size)
                if op.type == X.X86_OP_MEM:
                    m = op.mem
                    base = ins.reg_name(m.base) if m.base else None
                    seg = ins.reg_name(m.segment) if m.segment else ("ss" if base in ("bp", "sp") else "ds")
                    return ("mem", (_uc_reg(base) if base else None,
                                    _uc_reg(ins.reg_name(m.index)) if m.index else None, m.disp, _uc_reg(seg)), op.size)
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
                out = ("jcc", ((ins.operands[0].imm & 0xFFFF), off + ins.size), (), None)
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
        _DECODED16[key] = out
    return _DECODED16[key]


def _branch_hook16(rec, cs_base, lo, hi):
    """A code hook for the offsets [lo, hi) of the segment at `cs_base`: appends (jump, taken,
    compare) to rec["branches"] as emulator._branch_hook does, every address a linear one."""
    import capstone
    from codebreaker.emulate.emulator import _BRANCH_CAP
    from codebreaker.decompile.describe import decoder16
    md = decoder16(detail=True)
    log = rec["branches"] = []
    state = [None, None]                                     # [pending jump, compare in force]
    loaded = {}                                              # register -> (address, size) it was loaded from
    bases = {}

    def segbase(uc, sel):
        if sel not in bases:                                 # (the descriptor's base, from the GDT)
            d = bytes(uc.mem_read(GDT_ADDR + (sel >> 3) * 8, 8))
            bases[sel] = d[2] | d[3] << 8 | d[4] << 16 | d[7] << 24
        return bases[sel]

    def address(uc, mem):
        base, index, disp, seg = mem
        off = (disp + (uc.reg_read(base) if base else 0) + (uc.reg_read(index) if index else 0)) & 0xFFFF
        return segbase(uc, uc.reg_read(seg)) + off

    def value(uc, op):
        kind, what, size = op
        mask = (1 << (8 * size)) - 1
        if kind == "imm":
            return what & mask, None
        if kind == "reg":
            at = loaded.get(what[1])
            return uc.reg_read(what[0]) & mask, (at[0] if at and at[1] == size else None)
        a = address(uc, what)
        return int.from_bytes(bytes(uc.mem_read(a, size)), "little"), a

    def hook(uc, addr, _size, _ud):
        pend = state[0]
        if pend is not None:
            state[0] = None
            if len(log) < _BRANCH_CAP and addr in (pend[1], pend[2]):
                log.append((pend[0], addr == pend[1], pend[3]))
        try:
            raw = bytes(uc.mem_read(addr, 16))
        except Exception:
            raw = bytes(uc.mem_read(addr, (addr | 0xFFF) + 1 - addr))   # (the last bytes of a window)
        kind, detail, written, load = _decode16(md, raw, addr - cs_base)
        if kind == "cmp":
            try:
                (a, at_a), (b, at_b) = value(uc, detail[1][0]), value(uc, detail[1][1])
                state[1] = (detail[0], detail[2], a, b, at_a, at_b)
            except Exception:
                state[1] = None
        elif kind == "jcc":
            if lo <= detail[0] < hi and detail[0] != detail[1]:
                state[0] = (addr, cs_base + detail[0], cs_base + detail[1], state[1])
        elif kind == "other":
            state[1] = None
        for r in written:
            loaded.pop(r, None)
        if load is not None and load[0]:
            try:
                loaded[load[0]] = (address(uc, load[1][1]), load[2])
            except Exception:
                pass
    return hook


def trace_function16(path, ordinal=None, seg=None, offset=None, args=None, arity=0,
                     insn_cap=_MAX_INSNS, import_stub=None, prelude=None, near=False, data_image=None,
                     callees=None, data_end=None, context=None, mem_init=None, branches=None, bp=None,
                     ports=False, shadow=None):
    """Trace one exported 16-bit function. Identify it by `ordinal` (entry-table) or explicit
    (seg, offset). `args` are 16-bit Pascal args (pushed left-to-right); a far-RET sentinel sits
    under them. Returns a TraceRecord dict like tracer.trace_function.

    import_stub(hit, uc, U) -> dict|None lets a caller model Win16 API calls (the dynamic NE
    oracle): return {"ax":.., "dx":.., "pop":arg_bytes} to set the DX:AX result and pop the callee's
    Pascal args (keeping the caller's stack balanced). Default (None) keeps the old AX=0, pop-none
    behavior. `hit` carries the resolved "MODULE.FUNC" in hit["api"] plus args_stack.

    near=True enters a NEAR function: only a return offset is pushed, and returning to it ends the
    run. data_image (bytes) is written over the start of the data segment before the run -- used
    to give a rebuilt module the ORIGINAL's data, so both sides start from the same globals.

    callees: {(seg, offset): (name, bytes of arguments it removes, far)} -- the module's other
    functions, not run: reaching one is a call recorded (rec["calls_out"]: name and argument
    words) and answered at once with a result made from them, the same on any build. A function
    is then judged on what it does itself, whatever its callees' state.

    context: a call recorded in the running system (a breakpoint at the function's entry), replayed
    -- {"stack": its stack segment's bytes, "sp": SP at entry, "ss": SS, "ds", "es": selectors,
    "regs": {"AX": ..}, "ldt": {selector: (bytes, is_code)}, "data_sel": the module's own data
    selector there}. The recorded stack is the run's stack, at the same SP (its return address
    replaced by the tracer's own); the selectors the system had name the same bytes here (an LDT
    of them): SS's the stack, the module's data selector its data segment, the rest the recorded
    segments' copies, and any other one scratch. `args` are then not pushed.

    mem_init: [(where, bytes)] written before the run, `where` a linear address or ("D", offset):
    an offset in the module's data segment, wherever this build has it.

    branches: (low, high) offsets in the function's segment -- also log each conditional jump
    there: which way it went and what was compared (rec["branches"]; _branch_hook16).

    bp: BP as the function is entered (else as the emulator starts it, 0) -- for a helper that
    works in its caller's frame (verify/lanes/win16.py, CALLER_BP).

    ports: `in` and `out` reach the UARTs and interrupt controllers of emulate/ports16.py, fresh
    for the run, and what was done to them is rec["io"] ([(step, "in"|"out", port, value)]);
    else an `in` reads 0 and an `out` goes nowhere.

    shadow: an emulate/shadow.py Shadow16 kept up beside the run (the path explorer's): told the
    arguments' places, stepped before each instruction of the module's code, told of each call
    the environment answers. The run is the same run with it or without."""
    import unicorn as U
    from unicorn.x86_const import (UC_X86_REG_CS, UC_X86_REG_DS, UC_X86_REG_SS, UC_X86_REG_ES,
                                   UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EAX, UC_X86_REG_EDX,
                                   UC_X86_REG_EBP)
    # (a module is parsed and fixed up once per process: each run after the first writes the
    # fixed-up segments as they were -- walking every fix-up chain again was most of a run's setup)
    st = os.stat(path)
    known = _LOADED.get((path, st.st_size, st.st_mtime_ns))
    nef = known[0] if known else NEFile(path)
    uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_32)
    fast_uc.speed_up(uc)                                     # (the same reads, for less: emulate/fast_uc.py)
    sels, seg_bases, stack_sel, sent_sel = _load_ne(uc, U, nef)
    if known:
        for base, body in known[1].items():
            uc.mem_write(base, body)
        imports = {k: dict(v) for k, v in known[2].items()}
    else:
        imports = {}
        _apply_relocs(uc, nef, sels, seg_bases, sent_sel, imports)
        lengths = {s["idx"]: s["length"] for s in nef.segments() if s["file_off"]}
        _LOADED[(path, st.st_size, st.st_mtime_ns)] = (
            nef, {seg_bases[i]: bytes(uc.mem_read(seg_bases[i], n)) for i, n in lengths.items() if n},
            {k: dict(v) for k, v in imports.items()})

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

    alias = _recorded_ldt(uc, U, context, seg_bases.get(auto)) if context else {}

    def _enter(sseg, soff, sargs):
        """Set up a Pascal far frame for one call: push args left-to-right, then a far-ret sentinel,
        and point CS:IP at (sseg:soff) with DS/ES = DGROUP. Returns the effective code offset."""
        if context:
            return _enter_recorded(sseg, soff)
        sp = 0xFFF0
        arg_places[:] = []
        for a in sargs:
            sp = (sp - 2) & 0xFFFF
            uc.mem_write(STACK_BASE + sp, (int(a) & 0xFFFF).to_bytes(2, "little"))
            arg_places.append((STACK_BASE + sp, int(a) & 0xFFFF))
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
        uc.reg_write(UC_X86_REG_EAX, ENTRY_AX)               # (a function that sets no result returns it)
        if bp is not None:
            uc.reg_write(UC_X86_REG_EBP, bp & 0xFFFF)
        uc.reg_write(UC_X86_REG_CS, sels[sseg]); uc.reg_write(UC_X86_REG_EIP, soff)
        return soff

    def _enter_recorded(sseg, soff):
        """The recorded call's own frame: its stack at its SP, its registers, its selectors."""
        from unicorn.x86_const import (UC_X86_REG_EBX, UC_X86_REG_ECX, UC_X86_REG_ESI,
                                       UC_X86_REG_EDI, UC_X86_REG_EBP)
        uc.mem_write(STACK_BASE, bytes(context["stack"][:0x10000]).ljust(0x10000, b"\0"))
        sp = context["sp"] & 0xFFFF
        if near:
            uc.mem_write(STACK_BASE + sp, NEAR_RET.to_bytes(2, "little"))
        else:
            uc.mem_write(STACK_BASE + sp, FAR_RET.to_bytes(2, "little"))
            uc.mem_write(STACK_BASE + sp + 2, sent_sel.to_bytes(2, "little"))
        entry_sp[0] = sp
        regs = context.get("regs") or {}
        for r, k in ((UC_X86_REG_EAX, "AX"), (UC_X86_REG_EBX, "BX"), (UC_X86_REG_ECX, "CX"),
                     (UC_X86_REG_EDX, "DX"), (UC_X86_REG_ESI, "SI"), (UC_X86_REG_EDI, "DI"),
                     (UC_X86_REG_EBP, "BP")):
            # (AX the tracer's mark, not the recorded one: a function that sets no result hands
            # back what AX held, and the recorded word read as a result -- sub_22_4008, a copy
            # loop, "returned 1228h")
            uc.reg_write(r, ENTRY_AX if k == "AX" else regs.get(k, 0) & 0xFFFF)
        # (SS the tracer's own: a stack selector must be of the running level, and the run's is
        # 0; the recorded one, in the LDT, names the same bytes for a pointer into the stack)
        uc.reg_write(UC_X86_REG_SS, stack_sel)
        uc.reg_write(UC_X86_REG_ESP, sp)
        uc.reg_write(UC_X86_REG_DS, alias.get(context.get("ds"), ds_sel))
        uc.reg_write(UC_X86_REG_ES, alias.get(context.get("es"), ds_sel))
        uc.reg_write(UC_X86_REG_CS, sels[sseg]); uc.reg_write(UC_X86_REG_EIP, soff)
        return soff

    entry_sp = [0]
    arg_places = []                                          # (where each argument word is pushed, its value)
    if data_image and seg_bases.get(auto) is not None:
        uc.mem_write(seg_bases[auto], bytes(data_image[:0x10000]))
    if data_end and seg_bases.get(auto) is not None:
        # (past the original's data, the same on both sides: zeros to where its segment ends --
        # what the loader gives -- then a pattern to 64K. A build's own data segment is laid out
        # longer, and a read past the end -- a pointer near FFFFh -- found the build's bytes on
        # one side and nothing on the other: USER's sub_15_1772 and four more "differed")
        base = seg_bases[auto]
        start = len(data_image) if data_image else data_end[0]
        for page in range((base + start) & ~0xFFF, base + 0x10000, 0x1000):
            try:
                uc.mem_map(page, 0x1000)
            except U.UcError:
                pass                                         # (mapped already)
        uc.mem_write(base + start, bytes(max(0, data_end[1] - start)))
        tail = _TAILS.get((base, data_end[1]))
        if tail is None:
            tail = _TAILS[(base, data_end[1])] = bytes(((base + a) * 0x9E3779B1 >> 13) & 0xFF   # (emulator._pattern's)
                                                       for a in range(data_end[1], 0x10000))
        uc.mem_write(base + data_end[1], tail)
    for where, data in mem_init or ():
        if isinstance(where, tuple) and (seg_bases.get(auto) is None or where[1] + len(data) > 0x10000):
            continue                                         # (no data segment, or past its end)
        at = seg_bases[auto] + where[1] if isinstance(where, tuple) else where
        try:
            uc.mem_write(at, bytes(data))
        except U.UcError:
            pass                                             # (nothing mapped there: not an input)
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

    code_at = {sels[i] & ~3: seg_bases[i] for i in seg_bases if i in sels and i != auto}   # (selector -> base)
    intercept = {}
    places = {(cseg, coff): what[0] for (cseg, coff), what in (callees or {}).items()}
    for (cseg, coff), what in (callees or {}).items():
        if cseg in seg_bases and (cseg, coff) != (seg, offset):
            intercept[seg_bases[cseg] + coff] = what
    rec["calls_out"] = []

    def answer(address):
        """A call into another of the module's functions, recorded and answered."""
        name, pop, far = intercept[address]
        if pop < 0:                                          # (how many it removes is not known:
            rec["unsure"] = True                             # the stack after it is not either)
            pop = 0
        spn = uc.reg_read(UC_X86_REG_ESP) & 0xFFFF
        ip = int.from_bytes(uc.mem_read(STACK_BASE + spn, 2), "little")
        cs = int.from_bytes(uc.mem_read(STACK_BASE + spn + 2, 2), "little") if far else uc.reg_read(UC_X86_REG_CS)
        first = spn + (4 if far else 2)
        words = [int.from_bytes(uc.mem_read(STACK_BASE + ((first + 2 * k) & 0xFFFF), 2), "little")
                 for k in range(pop // 2)]
        rec["calls_out"].append({"name": name, "args": words, "sp": spn, "n": rec["insn_count"],
                                 "ret_ip": ip})
        # (the made-up result is made from the words as they compare between builds: a place in
        # the stack, or the stack's or data's selector, legitimately differs -- USER's eleven
        # RegisterClass callers kept a result made from their frame's address, and "differed")
        ss_now, ds_now = uc.reg_read(UC_X86_REG_SS), uc.reg_read(UC_X86_REG_DS)
        same = []
        for i, w in enumerate(words):
            nxt = words[i + 1] if i + 1 < len(words) else None
            fn = None
            if nxt is not None:                              # (a callback: the function, wherever it sits)
                fn = places.get(((nxt >> 3) - _MODULE_SLOT + 1, w))
            same.append("&" + fn if fn else "SS" if w == ss_now else "DS" if w == ds_now
                        else "stack" if nxt == ss_now or 0xE000 <= w < 0xFFF0 else w)
        h = int(hashlib.sha1(("%s%r" % (name, same)).encode()).hexdigest()[:8], 16)
        # (a place in the caller's stack handed over, `&local, SS`, the callee fills: a word made
        # up the same way. Left as it was, the caller read what its frame held before -- another
        # thing in each build: LookupMenuHandle's result, written by sub_10_009C)
        if shadow is not None:
            shadow.after_call(uc, False, 0, STACK_BASE + first, len(words), "callee")
        for i, w in enumerate(words[:-1]):
            if words[i + 1] == ss_now:
                uc.mem_write(STACK_BASE + w, (((h >> 7) ^ (0x9E37 * (i + 1))) & 0xFFFF).to_bytes(2, "little"))
                if shadow is not None:
                    shadow.written(STACK_BASE + w, 2)
        uc.reg_write(UC_X86_REG_EAX, h & 0xFFFF)
        uc.reg_write(UC_X86_REG_EDX, (h >> 16) & 0xFFFF)
        uc.reg_write(UC_X86_REG_ESP, (first + pop) & 0xFFFF)
        uc.reg_write(UC_X86_REG_CS, cs)
        uc.reg_write(UC_X86_REG_EIP, ip)

    def hook_code(uc, address, size, _ud):
        if OWN_BASE <= address < OWN_BASE + 0x1000000 and context:
            # (the module's own code reached through a recorded far pointer: that function of this
            # side's build run, by name; or, not a function's start, answered as foreign code is)
            n, off = (address - OWN_BASE) >> 16, address & 0xFFFF
            tgt = (context.get("redirect") or {}).get((n, off))
            if tgt:
                rec["imports_hit"].append({"api": "callback " + tgt[2], "n": rec["insn_count"]})
                uc.reg_write(UC_X86_REG_CS, sels[tgt[0]])
                uc.reg_write(UC_X86_REG_EIP, tgt[1])
                return
        if address in intercept and rec["insn_count"]:
            answer(address)
            return
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
            hit["n"] = rec["insn_count"]
            hit["ret_ip"] = ip
            hit["ret_cs"] = cs                               # (the caller's own code segment)
            # (and what a far pointer into one of the module's code segments points at: up to 32
            # bytes, to the first zero -- a string kept in the code, at its own offset in each build)
            for k in range(len(args_stack) - 1):
                base = code_at.get(args_stack[k + 1] & ~3)
                if base is not None:
                    try:
                        hit.setdefault("pointed", {})[k] = bytes(uc.mem_read(base + args_stack[k], 32)).split(b"\0")[0]
                    except U.UcError:
                        pass
            called = uc.reg_read(UC_X86_REG_CS)
            if context and called != sent_sel:
                called |= 3                                  # (loaded as conforming code: RPL 0)
                # (a far call into the recorded system's code: named by where it went, the same in
                # any build, and its arguments removed as that code's own `retf n` removes them --
                # left on the stack, the frame below them came apart: sub_22_33B8 "lost" DI)
                size = _retf_bytes(context, called, slot)
                hit = {"api": "far %04x:%04x" % (called, slot), "slot": slot, "foreign": True,
                       "args_stack": args_stack, "n": rec["insn_count"], "ret_ip": ip, "arg_bytes": size}
                rec["imports_hit"].append(hit)
                h = int(hashlib.sha1(hit["api"].encode()).hexdigest()[:8], 16)
                res = {"ax": h & 0xFFFF, "dx": (h >> 16) & 0xFFFF, "pop": size}
            else:
                rec["imports_hit"].append(hit)
                res = import_stub(hit, uc, U) if import_stub else None
            ax = (res or {}).get("ax", 0) & 0xFFFF
            dx = (res or {}).get("dx", 0) & 0xFFFF
            pop = (res or {}).get("pop", 0)
            uc.reg_write(UC_X86_REG_ESP, (spn + 4 + pop) & 0xFFFF)   # far-ret addr + callee's pascal args
            uc.reg_write(UC_X86_REG_EAX, (dx << 16) | ax)
            uc.reg_write(UC_X86_REG_EDX, dx)
            uc.reg_write(UC_X86_REG_CS, cs); uc.reg_write(UC_X86_REG_EIP, ip)
            if shadow is not None:
                # (an unmodelled call's result is an input, `ret`; a modelled one's is made from
                # its arguments, which the shadow does not follow into the model)
                free = bool(hit.get("unmodelled"))
                shadow.after_call(uc, free, (dx << 16) | ax, STACK_BASE + ((spn + 4) & 0xFFFF), pop // 2,
                                  None if free else "call:%s" % hit.get("api"))
        elif shadow is not None:
            shadow.step(uc, address, shadow.segbase(uc, uc.reg_read(UC_X86_REG_CS)))

    hook_mem, hook_unmapped = _memory_hooks(U, rec, set(), fill=not os.environ.get("CB_GATE_ZERO"))

    def hook_limited(uc, access, address, msize, value, ud):
        # A word read at offset FFFFh runs past the end of its segment: on the processor a
        # protection fault, here (Unicorn checks no limit) a byte of whatever lies after the
        # segment in this run's layout -- another in each build. It ends the run as a fault, on
        # either side alike, and is not itself recorded. (Every segment's window is 64K-aligned.)
        if rec.get("limit_fault"):
            return
        if (address & 0xFFFF) + msize > 0x10000 and address < SNAP_BASE:   # (a recorded memory's
                                                             # segments sit where they sat, not 64K apart)
            rec["limit_fault"] = True
            uc.emu_stop()
            return
        return hook_mem(uc, access, address, msize, value, ud)

    def hook_intr(uc, intno, _ud):
        # `int 21h` in a Windows module is DOS3Call by another door -- the same service, which
        # Windows documents as the faster way in; a module's own code picks one by a flag its
        # start-up sets (LZEXPAND's sub_11_0042 tests CS:[71h]), and a rebuild in C calls
        # DOS3Call. So it is answered as that import is, recorded as that call. Any other
        # interrupt, or a processor exception, ends the run as it did without this hook.
        if intno == 0x21 and import_stub is not None:
            hit = {"api": "KERNEL.@102", "slot": None, "args_stack": [], "n": rec["insn_count"], "int": 0x21}
            rec["imports_hit"].append(hit)
            res = import_stub(hit, uc, U) or {}
            ax, dx = res.get("ax", 0) & 0xFFFF, res.get("dx", 0) & 0xFFFF
            uc.reg_write(UC_X86_REG_EAX, (dx << 16) | ax)
            uc.reg_write(UC_X86_REG_EDX, dx)
            if shadow is not None:
                free = bool(hit.get("unmodelled"))
                shadow.after_call(uc, free, (dx << 16) | ax, None, 0, None if free else "call:KERNEL.@102")
            return
        rec["interrupt"] = intno
        uc.emu_stop()

    uc.hook_add(U.UC_HOOK_CODE, hook_code)
    uc.hook_add(U.UC_HOOK_INTR, hook_intr)
    if branches:
        uc.hook_add(U.UC_HOOK_CODE, _branch_hook16(rec, cs_base, branches[0], branches[1]), None,
                    cs_base + branches[0], cs_base + branches[1] - 1)
    uc.hook_add(U.UC_HOOK_MEM_READ | U.UC_HOOK_MEM_WRITE, hook_limited)
    uc.hook_add(U.UC_HOOK_MEM_READ_UNMAPPED | U.UC_HOOK_MEM_WRITE_UNMAPPED
                | U.UC_HOOK_MEM_FETCH_UNMAPPED, hook_unmapped)
    devices = None
    if ports:
        from codebreaker.emulate import ports16
        devices = ports16.Devices(clock=lambda: rec["insn_count"])
        ports16.hook(uc, U, devices)

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
        if devices:
            devices.log.clear()                              # (the devices keep their state)
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

    if shadow is not None:
        shadow.enter([] if context else list(arg_places))
    failed = None
    try:
        uc.emu_start(offset, SENT_BASE, count=insn_cap + 1)
        # (neither returned nor stopped by a hook: emu_start's own instruction count ran out --
        # just short of the cap, the skipped calls counting too. That is a run cut off, as much
        # as one the hook stops: "stopped" made a long loop in each build look like an ending)
        rec["exit"] = "ret" if rec.get("returned") else "truncated"
        if rec.get("limit_fault"):
            rec["exit"] = "exception: past the end of a segment"
        if rec.get("interrupt") is not None:                 # (hook_intr: as Unicorn says it unhooked)
            failed = "Unhandled CPU exception (UC_ERR_EXCEPTION)"
    except U.UcError as e:
        failed = str(e)
    if failed:
        rec["exit"] = "exception: %s" % failed
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
    if devices:
        rec["io"] = list(devices.log)
    return rec


_RETF = {}


def _retf_bytes(context, sel, offset):
    """The bytes of arguments the recorded system's code at sel:offset removes: its first `retf n`
    (0 for a plain retf, or none found in the next 8K)."""
    base = (context.get("_code_bases") or {}).get(sel)
    snap = context.get("snapshot")
    if base is None or not snap:
        return 0
    at = base - SNAP_BASE + offset
    key = (context.get("key"), at)
    if key not in _RETF:
        import capstone
        from codebreaker.decompile.describe import decoder16
        md = decoder16()
        got = 0
        for ins in md.disasm(bytes(snap[at:at + 0x2000]), offset):
            if ins.mnemonic in ("retf", "lret"):
                got = int(ins.op_str, 0) if ins.op_str else 0
                break
        _RETF[key] = got
    return _RETF[key]


LDT_ADDR = 0x1F00000           # a recorded call's LDT
FOREIGN_BASE = 0x2000000       # the recorded segments' copies, 64K apart
SNAP_BASE = 0x10000000         # a recorded call's whole memory
OWN_BASE = 0x8000000           # the module's own code as the recorded memory's selectors name it


def _recorded_ldt(uc, U, context, data_base):
    """An LDT holding a recorded call's selectors (each index of it present: a selector not
    recorded names scratch). Returns {recorded selector: the one to load for it here}.

    With context["snapshot"] (the whole of memory at the call, and context["ldt_at"]: where the
    LDT was and its limit) every selector the system had names its bytes there -- a selector a
    callee hands back as much as one among the arguments."""
    from unicorn.x86_const import UC_X86_REG_LDTR
    rec = dict(context.get("ldt") or {})
    snap = context.get("snapshot")
    if snap:
        uc.mem_map(SNAP_BASE, (len(snap) + 0xFFFF) & ~0xFFFF)
        uc.mem_write(SNAP_BASE, bytes(snap))
        at, lim = context["ldt_at"]
        n = (lim + 1) // 8
        table = bytearray(_desc(SCRATCH_BASE, 0xFFFF, False, dpl=3) * n)
        for i in range(1, n):
            d = snap[at + 8 * i:at + 8 * i + 8]
            if len(d) < 8 or not (d[5] & 0x80 and d[5] & 0x10):
                continue                                     # (not present, or not code/data)
            base = d[2] | d[3] << 8 | d[4] << 16 | d[7] << 24
            limit = d[0] | d[1] << 8
            if base + limit >= len(snap):
                continue
            if d[5] & 8:
                # (code of the running system -- Windows', another module's, a callback: not run,
                # but called as an import is, in the sentinel segment, answered and returned
                # from. Conforming, so a far call from the tracer's level 0 to an RPL 3 selector
                # is allowed; run where it was, a level-3 segment, it faulted on both sides)
                e = bytearray(_desc(SENT_BASE, 0xFFFF, True))
                e[5] = 0x80 | 0x10 | 0x0E
                table[8 * i:8 * i + 8] = bytes(e)
                continue
            table[8 * i:8 * i + 8] = _desc(SNAP_BASE + base, limit, False, dpl=3)
        rec = {}
    else:
        top = max([s >> 3 for s in rec] + [context["ss"] >> 3, (context.get("data_sel") or 0) >> 3, 1])
        table = bytearray(_desc(SCRATCH_BASE, 0xFFFF, False, dpl=3) * (top + 1))
    if len(table) // 8 <= max(context["ss"], context.get("data_sel") or 0) >> 3:
        table += _desc(SCRATCH_BASE, 0xFFFF, False, dpl=3) * (
            (max(context["ss"], context.get("data_sel") or 0) >> 3) + 1 - len(table) // 8)

    def put(sel, base, code):
        i = sel >> 3
        table[8 * i:8 * i + 8] = _desc(base, 0xFFFF, code, dpl=3)
    for k, (sel, (body, code)) in enumerate(sorted(rec.items())):
        if not sel & 4:
            continue
        base = FOREIGN_BASE + k * SEG_STRIDE
        uc.mem_map(base, SEG_STRIDE)
        uc.mem_write(base, bytes(body[:SEG_STRIDE]))
        put(sel, base, code)
    put(context["ss"], STACK_BASE, False)
    if context.get("data_sel") and data_base is not None:
        put(context["data_sel"], data_base, False)
    for sel, sg in (context.get("own") or {}).items() if snap else ():
        # (the module's own code segments: a window each, where a far call is sent on by name)
        try:
            uc.mem_map(OWN_BASE + sg * SEG_STRIDE, SEG_STRIDE)
        except U.UcError:
            pass
        e = bytearray(_desc(OWN_BASE + sg * SEG_STRIDE, 0xFFFF, True))
        e[5] = 0x80 | 0x10 | 0x0E
        table[8 * (sel >> 3):8 * (sel >> 3) + 8] = bytes(e)
    size = (len(table) + _PAGE - 1) // _PAGE * _PAGE
    uc.mem_map(LDT_ADDR, size)
    uc.mem_write(LDT_ADDR, bytes(table))
    uc.reg_write(UC_X86_REG_LDTR, (0, LDT_ADDR, len(table) - 1, 0x82))
    if snap:
        context["_code_bases"] = {(i << 3) | 7: SNAP_BASE + (snap[at + 8 * i + 2] | snap[at + 8 * i + 3] << 8 |
                                                           snap[at + 8 * i + 4] << 16 | snap[at + 8 * i + 7] << 24)
                                  for i in range(1, n) if snap[at + 8 * i + 5] & 0x98 == 0x98}
    out = {s: s for s in rec if s & 4}
    if snap:
        out = {(i << 3) | 7: (i << 3) | 7 for i in range(1, len(table) // 8)}
    out[context["ss"]] = context["ss"]
    if context.get("data_sel"):
        out[context["data_sel"]] = context["data_sel"]
    return out


ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)


HEAP_BASE = 0xB00000        # first far-heap block (distinct from seg/stack/scratch/sentinel windows)
HEAP_STRIDE = 0x10000
SAFE_ARG = _sel(200)      # a selector that names a real, writable scratch segment
STACK_SEL = _sel(_FIXED_SLOT)   # the stack's selector, whatever the module (_load_ne)

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


def _api(hit):
    """Bare API function name, upper-cased (drop the 'MODULE.' prefix); resolve KERNEL @ordinals
    (the knowledge base's win16 kernel_ordinal: KERNEL's numbers for the functions modelled, for
    when the call is reported by number, 'KERNEL.@15')."""
    api = (hit.get("api") or "").upper()
    fn = api.split(".", 1)[1] if "." in api else api
    if fn.startswith("@") and api.startswith("KERNEL"):
        try:
            from codebreaker import knowledge
            fn = knowledge.table("win16", "kernel_ordinal").get(str(int(fn[1:])), fn)
        except ValueError:
            pass
    return fn


def default_stub(hit, uc, U, ctx):
    """Model the Win16 APIs an LZ-class module calls, as the knowledge base's win16 api_model says
    (knowledge/models/win16.json). Returns {ax, dx, pop}. `pop` = total Pascal arg bytes the
    callee removes (WORD=2, DWORD / far ptr = 4)."""
    from codebreaker import knowledge
    model = knowledge.api_model("win16", _api(hit))
    if model is None:
        return {"ax": 0}                                     # unknown: benign 0 (stack may drift)
    a = hit.get("args_stack", [])
    out = {}
    for reg in ("ax", "dx", "pop"):
        if reg in model:
            v = model[reg]
            if v == "heap":                                  # a new selector of the stand-in heap
                v = ctx.alloc_heap()
            elif isinstance(v, str) and v.startswith("arg"):  # the Nth word of the arguments, as pushed
                n = int(v[3:])
                v = a[n] if len(a) > n else 0
            out[reg] = v
    return out


def probe(dll, ordinal=None, seg=None, offset=None, args=None, stub=default_stub, insn_cap=None,
          prelude=None, cdecl=False, near=False, data_image=None, callees=None, data_end=None,
          context=None, stub_ret=None, mem_init=None, branches=None, bp=None, ports=False, shadow=None):
    """Run one NE function under the controlled Win16 environment. Returns the tracer16 TraceRecord
    (with rec['layout'] + rec['imports_hit'] + rec['mem_*'] + rec['ret']).

    cdecl=True reverses `args` before pushing: the tracer pushes Pascal order (left-to-right), but
    INTERNAL functions are often __cdecl16far (args pushed right-to-left), so their args must be
    reversed to land in the right slots (this is how the LZDecompress reset path was reached).

    stub_ret: what a Windows function `stub` does not model returns in DX:AX (else 0). mem_init,
    branches, bp, ports, shadow: as trace_function16."""
    if cdecl and args:
        args = list(reversed(args))
    ctx_box = {}

    def _stub(hit, uc, u):
        if "ctx" not in ctx_box:
            ctx_box["ctx"] = _Ctx(uc, u)
        res = stub(hit, uc, u, ctx_box["ctx"]) or {}
        hit["unmodelled"] = "pop" not in res                 # (its result: stub_ret, for the path explorer)
        if "pop" not in res and stub_ret is not None:        # (not modelled: the result asked for)
            res = dict(res, ax=stub_ret & 0xFFFF, dx=(stub_ret >> 16) & 0xFFFF)
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
                             prelude=prelude, near=near, data_image=data_image, callees=callees,
                             data_end=data_end, context=context, mem_init=mem_init, branches=branches, bp=bp,
                             ports=ports, shadow=shadow, **kw)
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
