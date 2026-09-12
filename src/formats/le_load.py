#!/usr/bin/env python3
r"""le_load -- VxD (LE/LX, ring-0) loader + flat-code function carver for the behavior tracer.

Produces the SAME tuple tracer._load() returns for a PE -- (path, va0, code, secname, funcs,
sha, imports) -- so the 32-bit behavioral cascade (synth/struct/effect/globals/...) can run on
ring-0 VxD code. VxDs are 32-bit flat images; VMM services are INLINE `INT 20h; dd <service>`
(handled by the tracer's INT-20h stub), so there is no import table here (imports == {}).

Function carving (no PE export table on a VxD): seed starts from the LE start EIP, targets of
`call rel32`, and standard prologues (push ebp; mov ebp, esp), then bound each start by the next.
"""
import hashlib


def _capstone():
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    return md


def carve_flat(code, va0, entry_vas=()):
    """Carve function boundaries from a flat 32-bit code object -> {off: {start,end,name}}."""
    md = _capstone()
    starts = set()
    insns = list(md.disasm(code, va0))
    for i, ins in enumerate(insns):
        if ins.mnemonic == "push" and ins.op_str == "ebp":
            nxt = insns[i + 1] if i + 1 < len(insns) else None
            if nxt and nxt.mnemonic == "mov" and nxt.op_str.replace(" ", "") == "ebp,esp":
                starts.add(ins.address)
        if ins.mnemonic in ("call", "jmp") and ins.op_str.startswith("0x"):
            try:
                t = int(ins.op_str, 16)
            except ValueError:
                t = None
            if t is not None and va0 <= t < va0 + len(code):
                starts.add(t)
    for v in entry_vas:
        if va0 <= v < va0 + len(code):
            starts.add(v)
    if not starts:
        starts.add(va0)
    ordered = sorted(starts)
    funcs = {}
    for j, s in enumerate(ordered):
        e = ordered[j + 1] if j + 1 < len(ordered) else va0 + len(code)
        off = s - va0
        if e - s >= 8:
            funcs[off] = {"start": off, "end": e - va0, "name": ""}
    return funcs


def _entry_vas(lf):
    vas = []
    try:
        if 1 <= lf.eip_obj <= len(lf.objects):
            vas.append(lf.objects[lf.eip_obj - 1]["base"] + lf.eip)
    except Exception:
        pass
    return vas


def le_modules(path):
    """[(name, LEFile)] for a VxD file -- one for a plain LE/LX, all members for a W3 bundle."""
    from src.formats import le
    return le.open_vxds(path)


def primary_exec(lf):
    """The largest executable object of an LE module (its main code section)."""
    execs = [o for o in lf.objects if o["exec"]]
    if not execs:
        raise ValueError("no executable object")
    return max(execs, key=lambda o: o["vsize"])


_SPAN_CAP = 4 * 1024 * 1024    # combine all exec objects into one image if they fit in 4 MB


_STRIDE = 0x10000              # 64K-align each stacked object
_BASE0 = 0x10000


def _exec_image(lf):
    """Build one flat code image covering ALL executable objects of an LE module so every code
    object's functions are traceable, not just the largest. VxD objects are usually relocatable
    (LE object base == 0 for all of them), so we can't trust the header bases -- STACK the objects
    at distinct 64K-aligned synthetic bases. Intra-object call rel32 and prologues still carve
    correctly (position-independent); cross-object/absolute references are handled by the tracer's
    lazy mapping + INT-20h stub. Falls back to the largest object if the stack exceeds _SPAN_CAP."""
    execs = [o for o in lf.objects if o["exec"]]
    if not execs:
        raise ValueError("no executable object")
    layout = []
    cur = _BASE0
    for o in execs:
        b = lf.object_bytes(o)
        if not b:
            continue
        layout.append((cur, b))
        cur += ((len(b) + _STRIDE - 1) // _STRIDE) * _STRIDE
    if not layout:
        raise ValueError("no executable bytes")
    if cur - _BASE0 > _SPAN_CAP:
        o = max(execs, key=lambda x: x["vsize"])
        return _BASE0, lf.object_bytes(o)
    buf = bytearray(cur - _BASE0)
    for at, b in layout:
        buf[at - _BASE0:at - _BASE0 + len(b)] = b
    return _BASE0, bytes(buf)


def le_load(path, module_name=None, member=None):
    """tracer._load tuple for a VxD (all its exec objects) or a named W3 member.

    module_name may carry a "#member" suffix (e.g. "vmm32.vxd#VMM") to pick one module out of a
    W3 bundle -- the core ring-0 VxDs (VMM, VPICD, VDMAD, ...) live inside vmm32.vxd, not as loose
    files, so this is how they are reached."""
    if member is None and module_name and "#" in module_name:
        member = module_name.split("#", 1)[1]
    units = le_modules(path)
    if member is not None:
        units = [(n, lf) for n, lf in units if n.lower() == member.lower()] or units
    name, lf = units[0]
    va0, code = _exec_image(lf)
    funcs = carve_flat(code, va0, _entry_vas(lf))
    sha = hashlib.sha256(code).hexdigest()
    return (path, va0, bytes(code), ".text", funcs, sha, {})
