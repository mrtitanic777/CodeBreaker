#!/usr/bin/env python3
r"""equiv.py -- DIFFERENTIAL behavioral-equivalence gate (the reconstruction's real test).

The premise is behaves-as-original, but the pipeline has been graded by proxies -- compiles,
byte-divergence, loads -- none of which prove behaviour. bios is the proof the proxies fail: every
byte-checkable structure (control proc, DDB, service handlers, relocations) was IDENTICAL to retail,
yet it faults. Byte analysis is blind to the defect class that matters.

This runs the SAME logical function in OURS and in the ORIGINAL on the SAME sampled machine state,
under Unicorn, and diffs the OBSERVABLE EFFECTS -- return value, memory writes, and the ordered
sequence of external interactions (cross-module / VxD-service `int 20h` calls with their register
arguments). Effects are normalized so the comparison is robust to a function's internal byte layout
(the benign near/short-jump and disp8 shifts): stack and scratch writes are keyed by offset from the
frame base, code addresses by offset from the load base, external calls by the callee's identity
(service id, or target offset) rather than an absolute address. Two functions are EQUIVALENT iff
their effect signatures match on every sampled input; the first divergence pins the defect.

Layout-robust by construction: internal calls (into the same mapped image) are allowed to EXECUTE,
so two implementations that arrange their own code differently but do the same thing still match --
only the boundary-crossing effects and the result are compared.

Not a byte check. A behaviour check. This is the gate a body must pass to be "done".
"""
import os
import struct
import random

_SCRATCH_BASE, _SCRATCH_SIZE = 0x200000, 0x10000
_STACK_TOP, _RET = 0x208000, 0xDEAD0000
_LOAD_VA = 0x400000                                          # both images map here (identical base)
_PAGE = 0x1000
_MAX_INSNS = 20000
_ARGBUF_BASE = 0x300000                                      # where sampled pointer-args point
_OBJ_BASE, _OBJ_STRIDE = 0x10000000, 0x01000000             # relocated object bases (spaced, big)
_SVC_BASE = 0xE1000000                                       # sentinel base for imported VxD services


# ---- relocate a VxD image the way VMM does at load ------------------------------------------
def load_relocated(path, module=None):
    """Load a VxD (LE) and RELOCATE it into a flat multi-object image, applying every LE fixup --
    the same thing VMM does at load. Without this, cross-object dispatch calls and data pointers
    hold their unrelocated file values and run into garbage. Returns
    {regions:[(base,bytes)], obj_base:{idx:base}, ddb_obj, ctrl_off, entries:{name:va}}.

    Each object idx gets a fixed base (_OBJ_BASE + (idx-1)*_OBJ_STRIDE) so ours and the original
    share one address scheme and their effects are comparable; each applies its OWN fixups, so each
    executes its own real control flow. Internal fixups patch a 4-byte offset to the target's VA;
    imported-service fixups patch a per-(module,ordinal) sentinel that reads back as an external call."""
    from src.formats import le
    v = (le.open_vxds(path)[0][1] if module is None else module)
    obj_base = {}
    regions = []
    bufs = {}
    for o in v.objects:
        base = _OBJ_BASE + (o["idx"] - 1) * _OBJ_STRIDE
        obj_base[o["idx"]] = base
        b = bytearray(v.object_bytes(o))
        if len(b) < o["vsize"]:
            b += bytes(o["vsize"] - len(b))                  # BSS tail
        bufs[o["idx"]] = b
    svc = {}
    for (oi, off), fx in v.fixups().items():
        if oi not in bufs or off < 0 or off + 4 > len(bufs[oi]):
            continue
        if fx["kind"] == "internal":
            tva = obj_base.get(fx["obj"], 0) + (fx.get("target") or 0)
        else:                                                # imported VxD/VMM service -> sentinel
            key = (fx.get("module"), fx.get("ordinal"))
            sent = svc.setdefault(key, _SVC_BASE + len(svc) * 0x10)
            tva = sent
        # Self-relative source (srctype 0x08: an E8/E9 call/jmp displacement to another object) must
        # be patched as target - (source+4), NOT as the absolute VA -- otherwise every inter-object
        # call lands in garbage. Absolute sources (0x07: `dd offset`, `push imm`) take the VA.
        if fx.get("srctype") == 0x08:
            src_va = obj_base[oi] + off
            val = (tva - (src_va + 4)) & 0xFFFFFFFF
        else:
            val = tva & 0xFFFFFFFF
        struct.pack_into("<I", bufs[oi], off, val)
    for idx in sorted(bufs):
        regions.append((obj_base[idx], bytes(bufs[idx])))
    db = v.ddb()
    ddb_obj = db["obj"] if db else 1
    ctrl_off = None
    if db:
        # Control_Proc pointer sits at DDB+0x18; read the now-relocated VA and convert to an offset
        ci = None
        for o in v.objects:
            if o["idx"] == ddb_obj:
                ci = o
        if ci is not None:
            rva = db["rva"]
            cva = struct.unpack_from("<I", bufs[ddb_obj], rva + 0x18)[0]
            ctrl_off = cva - obj_base[ddb_obj]
    return {"regions": regions, "obj_base": obj_base, "ddb_obj": ddb_obj,
            "ctrl_off": ctrl_off, "ctrl_va": (obj_base[ddb_obj] + ctrl_off) if ctrl_off is not None else None,
            "svc": {v_: k for k, v_ in svc.items()}}


# ---- one run --------------------------------------------------------------------------------
def run_image(regions, entry_va, regs=None, stack_args=(), mem_init=(), insn_cap=_MAX_INSNS):
    """Execute a RELOCATED image (a list of (base, bytes) object regions) from entry_va under
    Unicorn until it returns to the RET sentinel. Return an effect record. Calls whose target
    lands inside any region EXECUTE (so cross-object dispatch works); anything else (a service
    sentinel, an unmapped target) is stubbed to return 0 and recorded as an external interaction.

    regs -- initial x86 registers ({"eax": msg, ...}); VxD control procs take the message in EAX.
    """
    import unicorn as U
    from unicorn.x86_const import (UC_X86_REG_ESP, UC_X86_REG_EAX, UC_X86_REG_EBX,
                                   UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_ESI,
                                   UC_X86_REG_EDI, UC_X86_REG_EBP, UC_X86_REG_EIP,
                                   UC_X86_REG_EFLAGS)
    REG = {"eax": UC_X86_REG_EAX, "ebx": UC_X86_REG_EBX, "ecx": UC_X86_REG_ECX,
           "edx": UC_X86_REG_EDX, "esi": UC_X86_REG_ESI, "edi": UC_X86_REG_EDI,
           "ebp": UC_X86_REG_EBP}
    uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_32)
    ranges = []                                              # (base, base+len) of every code region
    mapped = set()
    for base, data in regions:
        msize = (len(data) + _PAGE - 1) & ~(_PAGE - 1)
        uc.mem_map(base, max(msize, _PAGE)); uc.mem_write(base, bytes(data))
        ranges.append((base, base + len(data)))
        for pg in range(base, base + max(msize, _PAGE), _PAGE):
            mapped.add(pg)
    uc.mem_map(_SCRATCH_BASE, _SCRATCH_SIZE); mapped.add(_SCRATCH_BASE)
    for addr, dat in mem_init:
        a = addr & ~(_PAGE - 1)
        while a < addr + len(dat):
            if a not in mapped:
                uc.mem_map(a, _PAGE); mapped.add(a)
            a += _PAGE
        uc.mem_write(addr, bytes(dat))

    esp = _STACK_TOP
    for a in reversed(list(stack_args)):
        esp -= 4; uc.mem_write(esp, int(a & 0xFFFFFFFF).to_bytes(4, "little"))
    esp -= 4; uc.mem_write(esp, _RET.to_bytes(4, "little"))
    uc.reg_write(UC_X86_REG_ESP, esp)
    for r, val in (regs or {}).items():
        if r in REG:
            uc.reg_write(REG[r], int(val) & 0xFFFFFFFF)

    def in_image(addr):
        return any(lo <= addr < hi for lo, hi in ranges)

    obase = ranges[0][0]                                     # normalize code offsets against obj1's base

    def norm(addr):
        if _STACK_TOP - 0x4000 <= addr <= _STACK_TOP + 0x100:
            return ("stk", addr - _STACK_TOP)
        if _SCRATCH_BASE <= addr < _SCRATCH_BASE + _SCRATCH_SIZE:
            return ("scr", addr - _SCRATCH_BASE)
        for i, (lo, hi) in enumerate(ranges):
            if lo <= addr < hi:
                return ("obj%d" % i, addr - lo)
        if _ARGBUF_BASE <= addr < _ARGBUF_BASE + 0x10000:
            return ("arg", addr - _ARGBUF_BASE)
        if _SVC_BASE <= addr < _SVC_BASE + 0x100000:
            return ("svc", (addr - _SVC_BASE) // 0x10)
        return ("abs", addr)

    rec = {"ret": None, "carry": None, "writes": [], "svc": [], "ext": [], "exit": None, "insns": 0}

    def is_retaddr(v):
        """A return address a `call` pushed: an in-image pointer immediately preceded by a call
        (E8 rel32 at v-5, or an FF /2 indirect call ending at v). These are call machinery, not
        semantic arguments, and they shift by a byte with benign code-layout changes -- normalize
        them to one sentinel so they don't register as differences."""
        if not in_image(v):
            return False
        try:
            if uc.mem_read(v - 5, 1)[0] == 0xE8:
                return True
            b = uc.mem_read(v - 7, 7)
            for j in range(len(b) - 1):
                if b[j] == 0xFF and (b[j + 1] & 0x38) == 0x10 and (v - 7 + j) + 2 <= v:
                    pass                                     # (indirect calls vary in length; E8 is the
            return False                                    #  common case and enough for the ret-addr class)
        except Exception:
            return False

    def stack_args(n=6):
        """The top n stack dwords at a call, each normalized -- the callee's ARGUMENTS. Capturing
        these makes a service/external call's INPUTS part of the interaction signature, so passing a
        differently-located pointer (e.g. a DDB at a rotated offset) registers as a real difference.
        Return addresses (call machinery) are normalized to a sentinel so benign call-site shifts
        don't register."""
        sp = uc.reg_read(UC_X86_REG_ESP)
        out = []
        for i in range(n):
            try:
                v = int.from_bytes(uc.mem_read(sp + i * 4, 4), "little")
                out.append(("ret",) if is_retaddr(v) else norm(v))
            except Exception:
                out.append(("?", 0))
        return tuple(out)

    def hook_code(uc, address, csize, _ud):
        rec["insns"] += 1
        try:
            ins = bytes(uc.mem_read(address, min(csize, 16)))
        except Exception:
            return
        if len(ins) >= 2 and ins[0] == 0xCD and ins[1] == 0x20:      # int 20h VxD service
            sid = int.from_bytes(uc.mem_read(address + 2, 4), "little")
            rec["svc"].append((sid, norm(uc.reg_read(UC_X86_REG_EAX) & 0xFFFFFFFF),
                               norm(uc.reg_read(UC_X86_REG_EBX) & 0xFFFFFFFF),
                               norm(uc.reg_read(UC_X86_REG_ECX) & 0xFFFFFFFF),
                               norm(uc.reg_read(UC_X86_REG_EDX) & 0xFFFFFFFF), stack_args()))
            # Benign success stub, applied identically to ours and the original so the comparison is
            # fair. A Jmp-type service (ordinal bit 0x8000 -> VxDJmp/VMMJmp) transfers control away and
            # returns to the THUNK'S CALLER, so pop the return address; a Call-type service returns to
            # the instruction after the `int 20h; dd id` (address+6). Modelling this is what lets an
            # init path (Device_Init) run to a clean RET instead of hitting the `int3` service padding.
            uc.reg_write(UC_X86_REG_EAX, 0)
            eflags = uc.reg_read(UC_X86_REG_EFLAGS) & ~1     # clear CF (success)
            uc.reg_write(UC_X86_REG_EFLAGS, eflags)
            if sid & 0x8000:                                 # Jmp service -> return to the caller
                sp = uc.reg_read(UC_X86_REG_ESP)
                ret_to = int.from_bytes(uc.mem_read(sp, 4), "little")
                uc.reg_write(UC_X86_REG_ESP, sp + 4)
                uc.reg_write(UC_X86_REG_EIP, ret_to)
            else:
                uc.reg_write(UC_X86_REG_EIP, address + 6)
            return
        if ins[0] == 0xE8:                                            # call rel32
            tgt = (address + 5 + int.from_bytes(ins[1:5], "little", signed=True)) & 0xFFFFFFFF
            if not in_image(tgt):
                # the call has NOT pushed its return address yet (we intercept at the call insn), so
                # the current stack top already holds the callee's first argument
                rec["ext"].append(("call", norm(tgt), stack_args()))
                uc.reg_write(UC_X86_REG_EAX, 0)
                uc.reg_write(UC_X86_REG_EIP, address + 5)

    def hook_mem_w(uc, access, address, msize, value, _ud):
        rec["writes"].append((norm(address), msize, value & ((1 << (msize * 8)) - 1)))

    def hook_unmapped(uc, access, address, msize, value, _ud):
        base = address & ~(_PAGE - 1)
        if base not in mapped:
            try:
                uc.mem_map(base, _PAGE); mapped.add(base); return True
            except Exception:
                return False
        return False

    uc.hook_add(U.UC_HOOK_CODE, hook_code)
    uc.hook_add(U.UC_HOOK_MEM_WRITE, hook_mem_w)
    uc.hook_add(U.UC_HOOK_MEM_READ_UNMAPPED | U.UC_HOOK_MEM_WRITE_UNMAPPED
                | U.UC_HOOK_MEM_FETCH_UNMAPPED, hook_unmapped)
    try:
        uc.emu_start(entry_va, _RET, count=insn_cap + 1)
        rec["exit"] = "ret"
        rec["ret"] = uc.reg_read(UC_X86_REG_EAX) & 0xFFFFFFFF
        rec["carry"] = uc.reg_read(UC_X86_REG_EFLAGS) & 1     # VxD control procs signal status in CF
    except U.UcError as e:
        rec["exit"] = "fault:%s" % e
    if rec["insns"] > insn_cap:
        rec["exit"] = "timeout"
    return rec


def effect_sig(rec):
    """The comparable behavioural signature: exit reason, return value, the SET of normalized
    memory writes (last value wins per slot), and the ORDERED external/service interactions."""
    last = {}
    for slot, sz, val in rec["writes"]:
        if slot[0] == "stk":
            continue                                         # transient stack frame: private, not
            #                                                  caller-observable -- excluding it drops
            #                                                  the false-positive class of shifted
            #                                                  return-addresses / spilled self-pointers
        last[(slot, sz)] = val
    return (rec["exit"], rec["ret"], rec.get("carry"), tuple(sorted(last.items())),
            tuple(rec["svc"]), tuple(rec["ext"]))


# ---- input sampling -------------------------------------------------------------------------
def sample_inputs(reg_names, n, seed=0, messages=()):
    """Sample input states. `messages` seeds specific EAX values (VxD control messages) so the
    dispatch arms are actually exercised. Each sample sets the given registers + a controlled
    pointer buffer at _ARGBUF_BASE (so a pointer arg reads defined memory)."""
    rng = random.Random(seed)
    edge = [0, 1, 2, 0xFFFFFFFF, 0x80000000, _ARGBUF_BASE]
    out = []
    seedmsgs = list(messages) or [0]
    for i in range(n):
        regs = {}
        for r in reg_names:
            if r == "eax" and i < len(seedmsgs):
                regs[r] = seedmsgs[i]
            elif i < len(edge):
                regs[r] = edge[i]
            else:
                regs[r] = rng.choice(edge + [rng.randint(0, 0xFFFFFFFF)])
        buf = bytes(rng.randint(0, 255) for _ in range(64))
        out.append({"regs": regs, "mem_init": [(_ARGBUF_BASE, buf)]})
    return out


# ---- the gate -------------------------------------------------------------------------------
def equiv_images(ours_img, orig_img, ours_entry=None, orig_entry=None,
                 reg_names=("eax", "ebx", "ecx", "edx"), stack_arity=0, samples=24, seed=0,
                 messages=(), insn_cap=_MAX_INSNS):
    """Behavioural-equivalence verdict between two RELOCATED images (from load_relocated). Runs the
    same sampled machine state through each from its entry (default: its DDB Control_Proc) and diffs
    effect signatures. Returns {equivalent, samples, diverged_at, detail}."""
    oe = ours_entry if ours_entry is not None else ours_img["ctrl_va"]
    re_ = orig_entry if orig_entry is not None else orig_img["ctrl_va"]
    if oe is None or re_ is None:                             # no resolvable Control_Proc -> can't run
        return {"equivalent": None, "informative": 0, "diverged_at": None,
                "detail": None, "reason": "no-ctrl-proc"}
    inputs = sample_inputs(reg_names, samples, seed, messages)
    informative = 0                                          # samples where the ORIGINAL ran cleanly
    for inp in inputs:
        stk = [0] * stack_arity
        # The original defines valid behaviour: if it doesn't reach a clean RET for this input, the
        # environment is inadequate (e.g. a garbage VM handle Device_Init dereferences) and the input
        # is not a fair test -- skip it. Only compare where the oracle itself terminates cleanly.
        rr = run_image(orig_img["regions"], re_, inp["regs"], stk, inp["mem_init"], insn_cap)
        if rr["exit"] != "ret":
            continue
        informative += 1
        ro = run_image(ours_img["regions"], oe, inp["regs"], stk, inp["mem_init"], insn_cap)
        if effect_sig(ro) != effect_sig(rr):
            return {"equivalent": False, "informative": informative, "diverged_at": inp["regs"],
                    "detail": {"ours": _brief(ro), "orig": _brief(rr),
                               "first_field": _first_field(effect_sig(ro), effect_sig(rr))}}
    return {"equivalent": True, "informative": informative, "total": len(inputs),
            "diverged_at": None, "detail": None}


def equiv_vxd(ours_path, orig_path, **kw):
    """Convenience: load+relocate both VxDs and run the control-proc equivalence gate."""
    return equiv_images(load_relocated(ours_path), load_relocated(orig_path), **kw)


def _brief(rec):
    return {"exit": rec["exit"], "ret": rec["ret"], "carry": rec.get("carry"),
            "n_writes": len(rec["writes"]), "svc": rec["svc"][:6], "ext": rec["ext"][:6],
            "insns": rec["insns"]}


def _first_field(a, b):
    for name, x, y in zip(("exit", "ret", "carry", "writes", "svc", "ext"), a, b):
        if x != y:
            return name
    return "?"
