r"""msc6_calls.py -- the parameters of a module's internal functions, from how they are called.

Internal (non-exported) functions have no SDK prototype, so the model must guess their
parameter count and far/int widths from a single function's own body -- and gets it
wrong (a far pointer only PASSED THROUGH is never `les`'d locally, so it looks int).

This does a SEGMENT-LEVEL pass first: it reads every function's `retf N` (the exact
total param bytes, a hard constraint for pascal callees), notes which params the body
`les`'s (far, confirmed), and -- the cross-function part -- parses the argument PUSH RUN
at every CALL SITE to detect far-pointer arguments (a far ptr is pushed as a segment
word then an offset word). Merging these three signals yields each internal function's
signature, which run.py hands to the model (msc6.py) as the params.

    from codebreaker.invert import msc6_calls
    sigs = msc6_calls.infer_segment_sigs(nefile, seg, segbytes)   # {func_off: [(name,type),...]}
"""
import re
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
from codebreaker.invert.msc6_function import PROLOGUE, EPI

_MD = Cs(CS_ARCH_X86, CS_MODE_16)
_SEGPUSH = {0x06: "es", 0x0e: "cs", 0x16: "ss", 0x1e: "ds"}   # push <segreg> opcodes


def _func_bounds(segb):
    """(start, end) for every framed function in the segment (prologue .. epilogue+retf)."""
    outs = []; p = segb.find(PROLOGUE); consumed = 0
    while p >= 0:
        if p >= consumed:
            e = segb.find(EPI, p)
            if e >= 0:
                q = e + len(EPI)
                end = q + 3 if segb[q:q+1] == b"\xca" else (q + 1 if segb[q:q+1] in (b"\xcb", b"\xc3") else q)
                outs.append((p, end)); consumed = end
        p = segb.find(PROLOGUE, p + 1)
    return outs


def _retf_total(segb, start, end):
    """Total param BYTES from the callee's `retf N` (pascal), or None (cdecl/unknown)."""
    e = segb.rfind(EPI, start, end)
    if e < 0:
        return None
    q = e + len(EPI)
    if segb[q:q+1] == b"\xca":
        return segb[q+1] | (segb[q+2] << 8)
    return None


def _far_body_offsets(fb):
    """param offsets the body loads as a far pointer -- via `les/lds reg,[bp+off]`, or the
    two-mov idiom `mov reg,[bp+off]; mov es,[bp+off+2]` (offset lane then segment lane)."""
    offs = set()
    for ins in _MD.disasm(bytes(fb), 0):
        if ins.mnemonic in ("les", "lds"):
            m = re.search(r"\[bp \+ (\w+)\]", ins.op_str)
            if m:
                offs.add(int(m.group(1), 0))
        mm = re.match(r"(es|ds), word ptr \[bp \+ (\w+)\]", ins.op_str)
        if ins.mnemonic == "mov" and mm:                  # mov es,[bp+M] -> far param at M-2
            offs.add(int(mm.group(2), 0) - 2)
    return offs


def _long_body_offsets(fb):
    """param offsets the body loads as a 32-bit long/DWORD via the `mov ax,[bp+K];
    mov dx,[bp+K+2]` dx:ax pair idiom (distinct from a far pointer's les/two-mov-into-seg)."""
    offs = set(); insns = list(_MD.disasm(bytes(fb), 0))
    for i in range(len(insns) - 1):
        m1 = re.match(r"ax, word ptr \[bp \+ (\w+)\]$", insns[i].op_str)
        m2 = re.match(r"dx, word ptr \[bp \+ (\w+)\]$", insns[i+1].op_str)
        if insns[i].mnemonic == "mov" and insns[i+1].mnemonic == "mov" and m1 and m2 \
                and int(m2.group(1), 0) == int(m1.group(1), 0) + 2:
            offs.add(int(m1.group(1), 0))
    return offs


def _parse_arg_run(insns, ci):
    """Walk backward from the call at index `ci` over the pushes; return a list of arg
    descriptors [(words, is_far), ...] in PUSH order (first-pushed first)."""
    run = []
    j = ci - 1
    while j >= 0 and insns[j].mnemonic == "push":
        run.append(insns[j]); j -= 1
    run.reverse()                                         # now in push (source) order
    args = []; k = 0
    while k < len(run):
        ins = run[k]; nxt = run[k+1] if k + 1 < len(run) else None
        far = False
        if ins.bytes[0] in _SEGPUSH and nxt is not None:  # push <segreg>; push <off> = far ptr
            far = True
        elif nxt is not None:                             # push [bp+K+2]; push [bp+K] = far param hi/lo
            m1 = re.search(r"\[bp \+ (\w+)\]", ins.op_str)
            m2 = re.search(r"\[bp \+ (\w+)\]", nxt.op_str)
            if m1 and m2 and int(m1.group(1), 0) == int(m2.group(1), 0) + 2:
                far = True
        if far:
            args.append((2, True)); k += 2
        else:
            args.append((1, False)); k += 1
    return args


def infer_segment_sigs(f, seg, segb):
    """Infer {func_off: [(name,'int'|'far'|'long'), ...]} for every framed function in the
    segment, merging retf N (total), body les (far), and call-site far-pointer detection."""
    bounds = _func_bounds(segb)
    starts = {s for s, _ in bounds}
    retf = {s: _retf_total(segb, s, e) for s, e in bounds}
    far_body = {s: _far_body_offsets(segb[s:e]) for s, e in bounds}
    long_body = {s: _long_body_offsets(segb[s:e]) for s, e in bounds}
    far_call = {s: set() for s, _ in bounds}             # far param offsets seen at call sites

    # scan the whole segment's instructions once; resolve near calls to callee starts and
    # parse their argument runs. A near call `e8 rel16` targets addr_after + rel16.
    insns = list(_MD.disasm(bytes(segb), 0))
    a2i = {ins.address: i for i, ins in enumerate(insns)}
    for i, ins in enumerate(insns):
        tgt = None
        if ins.bytes[0] == 0xe8 and ins.mnemonic == "call":            # near call
            tgt = ins.address + ins.size + _rel(ins)
        elif ins.mnemonic == "call" and ins.bytes[0] == 0x0e:           # (unlikely) push cs handled elsewhere
            pass
        if tgt is None or tgt not in starts:
            continue
        args = _parse_arg_run(insns, i)
        # map far args to callee param offsets. Pascal: first-pushed arg is the HIGHEST
        # offset; the last-pushed sits at [bp+6]. Walk offsets from 6 upward in REVERSE
        # push order so arg positions line up with stack slots.
        off = 6
        for words, is_far in reversed(args):
            if is_far:
                far_call[tgt].add(off)
            off += words * 2

    sigs = {}
    for s, e in bounds:
        total = retf.get(s)
        far = far_body.get(s, set()) | far_call.get(s, set())
        lng = long_body.get(s, set()) - far     # far wins if a slot somehow matched both
        # bound: retf N words (pascal) else the max bp-offset the body reads
        limit = 6 + total if total else _maxbp(segb[s:e]) + 2
        params = []; off = 6
        while off < limit:
            if off in far and off + 4 <= limit:
                params.append(("p%d" % len(params), "far")); off += 4
            elif off in lng and off + 4 <= limit:
                params.append(("p%d" % len(params), "long")); off += 4
            else:
                params.append(("p%d" % len(params), "int")); off += 2
        if params:
            sigs[s] = params
    return sigs


def _rel(ins):
    v = ins.bytes[1] | (ins.bytes[2] << 8)
    return v - 0x10000 if v >= 0x8000 else v


def _maxbp(fb):
    m = 4
    for ins in _MD.disasm(bytes(fb), 0):
        for mm in re.finditer(r"\[bp \+ (\w+)\]", ins.op_str):
            o = int(mm.group(1), 0)
            if o >= 6:
                m = max(m, o)
    return m
