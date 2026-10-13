#!/usr/bin/env python3
r"""tracer16.py -- a 16-bit (Win16 NE) behavioral tracer on Unicorn, the segmented-protected-mode
counterpart to the flat 32-bit tracer.py.

Win16 code is 16:16 segmented and referenced by PROTECTED-MODE SELECTORS (not real-mode seg<<4),
so this runs Unicorn in 32-bit protected mode with a hand-built GDT: every NE segment gets a
16-bit descriptor whose base is where we load it, plus a stack selector and an import-sentinel
selector. It loads the segments, applies fixups, sets CS/DS/SS, pushes a Pascal frame under a
far-RET sentinel, runs to the sentinel, and records the same kind of TraceRecord as tracer.py
(path / mem / calls / ret). This is the substrate the behavioral reconstructors need to reach the
16-bit core (USER/GDI/KRNL386), where much of the Win95 behavior actually lives.

This module's GDT/segmentation harness is validated standalone (see `_selftest`) before any real
NE is loaded, because Unicorn's segmented-fetch convention is the fragile part.
"""
import os
import sys

# ---- GDT / memory layout (linear, 32-bit protected mode) -------------------
GDT_ADDR = 0x1000
SEG_ORIGIN = 0x100000          # first NE segment base; each segment gets a 0x10000 (64K) window
SEG_STRIDE = 0x10000
STACK_BASE = 0x900000          # stack segment base
SENT_BASE = 0xA00000           # import-sentinel segment base (far calls here are stubbed)
_PAGE = 0x1000
_MAX_INSNS = 20000


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
def _selftest():
    """Validate the GDT/segmentation harness on a synthetic 16-bit program BEFORE trusting it on
    real NE modules. Program in a code segment: `mov ax, 0x1234 ; retf`. We push a far-ret
    sentinel, run, and expect AX=0x1234 and a clean stop at the sentinel."""
    import unicorn as U
    from unicorn.x86_const import (UC_X86_REG_CS, UC_X86_REG_DS, UC_X86_REG_SS, UC_X86_REG_ES,
                                   UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EAX)
    uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_32)
    CODE, STACK, SENT = SEG_ORIGIN, STACK_BASE, SENT_BASE
    for b in (CODE, STACK, SENT):
        uc.mem_map(b, 0x10000)                            # full 64K segment window
    sels = _build_harness(uc, U, [(CODE, 0xFFFF, True), (STACK, 0xFFFF, False), (SENT, 0xFFFF, True)])
    cs_sel, ss_sel, sent_sel = sels[1], sels[2], sels[3]
    # mov ax, 0x1234 ; retf
    uc.mem_write(CODE, b"\x66\xB8\x34\x12" + b"\xCB")      # operand-size prefix: 16-bit in 32-bit seg
    # actually in a 16-bit (D/B=0) segment, `mov ax,imm16` = B8 34 12 ; retf = CB
    uc.mem_write(CODE, b"\xB8\x34\x12\xCB")
    # stack: push far-ret sentinel (IP=0, CS=sent_sel) so retf lands at SENT:0
    sp = 0xFFF0
    uc.mem_write(STACK + sp, (0).to_bytes(2, "little") + sent_sel.to_bytes(2, "little"))
    uc.reg_write(UC_X86_REG_SS, ss_sel); uc.reg_write(UC_X86_REG_ESP, sp)
    uc.reg_write(UC_X86_REG_DS, ss_sel); uc.reg_write(UC_X86_REG_ES, ss_sel)
    uc.reg_write(UC_X86_REG_CS, cs_sel); uc.reg_write(UC_X86_REG_EIP, 0)
    # emu_start's `begin`/`until` are EFFECTIVE (CS-relative) offsets here: Unicorn adds CS.base
    # for the fetch. begin=0 -> fetch at CS.base(CODE). The retf lands at CS=sent_sel:IP=0, whose
    # effective offset is 0 -- but `until` is matched on the linear PC, so use SENT_BASE.
    for until in (SENT_BASE, 0):
        uc.reg_write(UC_X86_REG_CS, cs_sel); uc.reg_write(UC_X86_REG_EIP, 0)
        uc.reg_write(UC_X86_REG_ESP, sp)
        try:
            uc.emu_start(0, until, count=20)
            ax = uc.reg_read(UC_X86_REG_EAX) & 0xFFFF
            return {"ok": ax == 0x1234, "ax": ax, "until": hex(until), "eip": uc.reg_read(UC_X86_REG_EIP)}
        except U.UcError as e:
            last = {"ok": False, "error": str(e), "until": hex(until), "eip": uc.reg_read(UC_X86_REG_EIP)}
    return last


# --------------------------------------------------------------------------
# NE loading + real-function tracing on the validated harness.
# --------------------------------------------------------------------------
from codebreaker.formats.ne import NEFile                     # noqa: E402

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
        from codebreaker.symbols import symdb
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

    def hook_mem(uc, access, address, size, value, _ud):
        d = rec["mem_writes"] if access in (U.UC_MEM_WRITE,) else rec["mem_reads"]
        if len(d) < 40000:
            d.append({"addr": address, "size": size, "value": value if access == U.UC_MEM_WRITE else None})
            if access == U.UC_MEM_WRITE:                     # what was there before (the hook runs first)
                try:
                    d[-1]["old"] = int.from_bytes(uc.mem_read(address, min(size, 4)), "little")
                except U.UcError:
                    d[-1]["old"] = 0

    def hook_unmapped(uc, access, address, size, value, _ud):
        page = address & ~(_PAGE - 1)
        try:
            uc.mem_map(page, _PAGE)
            return True                                       # lazily map zero page, continue
        except U.UcError:
            return False

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


if __name__ == "__main__":
    print("tracer16 harness self-test:", _selftest())

