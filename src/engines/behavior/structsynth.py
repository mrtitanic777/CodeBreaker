#!/usr/bin/env python3
r"""structsynth.py -- synthesize a novel struct layout from how a pointer's fields are used.

fieldname names fields only when a pointer matches a KNOWN SDK struct. But most structs in an app
are the app's own -- not in any SDK. This recovers those: it tracks the register holding a pointer
argument, collects every `[ptr + offset]` access, and infers each field's size (from the access
width) and type (a field whose loaded value is itself dereferenced is a pointer; otherwise sized
byte/word/dword). It then emits a real C `struct` -- fields at their offsets, gaps filled with
padding -- so `p->f_4` becomes a declared, typed member of a reconstructed type instead of an
anonymous offset.
"""
import re
from src.engines.behavior import tracer

_W = {"byte": 1, "word": 2, "dword": 4}
_CTYPE = {1: "unsigned char", 2: "unsigned short", 4: "unsigned"}
_ACCESS = re.compile(r"(byte|word|dword) ptr \[(e[a-z]{2})(?:\s*\+\s*(0x[0-9a-f]+|\d+))?\]")
_DEREF = re.compile(r"\[(e[a-z]{2})")
_LOADP = re.compile(r"^(e[a-z]{2}),\s*(?:dword ptr )?\[ebp\s*\+\s*(0x[0-9a-f]+|\d+)\]$")
_MOVREG = re.compile(r"^(e[a-z]{2}),\s*(e[a-z]{2})$")


def _rows(module, va):
    import capstone
    _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
    off = va - va0
    end = funcs[off]["end"] if off in funcs else min((o for o in funcs if o > off), default=off + 0x400)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    return [(i.address, i.mnemonic, i.op_str, i.size) for i in md.disasm(code[off:end], va0 + off)]


def synthesize_from_rows(rows, arg=0):
    """Recover {offset: {'size','ptr'}} for the struct pointed to by parameter `arg`. rows: tuples."""
    argoff = 8 + 4 * arg                                     # [ebp+8]=arg0, [ebp+0xc]=arg1, ...
    ptr_regs = set()
    loaded = {}                                              # reg -> struct offset it currently holds
    fields = {}
    for _a, mn, ops, _sz in rows:
        m = _LOADP.match(ops.strip())                        # reg = param  -> reg holds the pointer
        if mn == "mov" and m and int(m.group(2), 0) == argoff:
            ptr_regs.add(m.group(1)); loaded.pop(m.group(1), None); continue
        mv = _MOVREG.match(ops.strip())                      # propagate the pointer through reg copies
        if mn == "mov" and mv:
            if mv.group(2) in ptr_regs:
                ptr_regs.add(mv.group(1))
            elif mv.group(1) in ptr_regs:
                ptr_regs.discard(mv.group(1))
        for w, base, off in _ACCESS.findall(ops):            # a field access [ptr + off]
            if base in ptr_regs:
                o = int(off, 0) if off else 0
                f = fields.setdefault(o, {"size": 0, "ptr": False})
                f["size"] = max(f["size"], _W[w])
                dst = ops.split(",")[0].strip()
                if mn == "mov" and re.match(r"^e[a-z]{2}$", dst):
                    loaded[dst] = o                          # this reg now holds field @o
        for m2 in _DEREF.finditer(ops):                      # a loaded field reg used as a base -> ptr
            r = m2.group(1)
            if r in loaded and r not in ptr_regs:
                fields[loaded[r]]["ptr"] = True
    return fields


def _mask_span(m):
    """(lsb, width) if m is a single contiguous run of 1 bits, else None."""
    if m <= 0:
        return None
    lsb = (m & -m).bit_length() - 1
    v = m >> lsb
    if v & (v + 1):                                          # not 2^w - 1 -> not contiguous
        return None
    return lsb, v.bit_length()


def bitfields_from_rows(rows, arg=0):
    """{dword_offset: [(start_bit, width), ...]} for fields accessed via shift/mask (bit-fields)."""
    argoff = 8 + 4 * arg
    ptr_regs, reg, bits = set(), {}, {}
    for _a, mn, ops, _sz in rows:
        m = _LOADP.match(ops.strip())
        if mn == "mov" and m and int(m.group(2), 0) == argoff:
            ptr_regs.add(m.group(1)); continue
        ld = re.match(r"^(e[a-z]{2}),\s*(?:dword ptr )?\[(e[a-z]{2})(?:\s*\+\s*(0x[0-9a-f]+|\d+))?\]$",
                      ops.strip())
        if mn == "mov" and ld and ld.group(2) in ptr_regs:   # reg = field @off
            reg[ld.group(1)] = [int(ld.group(3), 0) if ld.group(3) else 0, 0]
            continue
        parts = [p.strip() for p in ops.split(",")]
        if len(parts) == 2 and parts[0] in reg:
            r = parts[0]
            if mn == "shr":
                try:
                    reg[r][1] += int(parts[1], 0)
                except ValueError:
                    pass
            elif mn == "and":
                try:
                    span = _mask_span(int(parts[1], 0))
                except ValueError:
                    span = None
                if span:
                    bits.setdefault(reg[r][0], set()).add((reg[r][1] + span[0], span[1]))
                reg.pop(r, None)
            elif mn == "mov":
                reg.pop(r, None)                             # reg reused for something else
    return {o: sorted(v) for o, v in bits.items()}


def _emit_bitfield(off, spans):
    """C bit-field members for a dword at `off` covering the recovered spans (gaps kept anonymous)."""
    out, cur = [], 0
    for start, width in spans:
        if start > cur:
            out.append("    unsigned : %d;              /* @0x%x bits %d-%d */" % (start - cur, off, cur, start - 1))
        out.append("    unsigned bf_%x_%d : %d;" % (off, start, width))
        cur = start + width
    if cur < 32:
        out.append("    unsigned : %d;" % (32 - cur))
    return out


def _emit(tag, fields, bits=None):
    if not fields:
        return None
    out = ["struct %s {" % tag]
    cur = 0
    bits = bits or {}
    for off in sorted(fields):
        if off > cur:
            out.append("    unsigned char pad_%x[0x%x];" % (cur, off - cur))
        f = fields[off]
        if off in bits:                                      # this dword is a set of bit-fields
            out += _emit_bitfield(off, bits[off])
            cur = off + 4
            continue
        sz = f["size"] or 4
        typ = "void *" if f["ptr"] else _CTYPE.get(sz, "unsigned")
        out.append("    %-14s f_%x;" % (typ, off))
        cur = off + (4 if f["ptr"] else sz)
    out.append("};")
    return "\n".join(out)


def synthesize(module, va, arg=0):
    """Emit a synthesized C struct for parameter `arg` of a function, or {'ok':False}.

    Defers to fieldname when the accessed offsets match a known SDK struct; else synthesizes novel."""
    try:
        rows = _rows(module, va)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    fields = synthesize_from_rows(rows, arg)
    if not fields:
        return {"ok": False, "reason": "no field accesses on the pointer arg"}
    try:                                                     # known SDK struct? prefer the real name
        from src.engines.behavior import fieldname
        hit = fieldname.best_match(set(fields))
        if hit:
            return {"ok": True, "known": hit[0], "nfields": len(fields),
                    "c": "/* matches SDK struct %s */" % hit[0]}
    except Exception:
        pass
    off = va - int(tracer._load(module)[1]) if module else va
    tag = "sub_%x_arg%d_t" % (off & 0xFFFFFFFF, arg)
    bits = bitfields_from_rows(rows, arg)
    return {"ok": True, "known": None, "nfields": len(fields), "nbitfields": len(bits),
            "c": _emit(tag, fields, bits)}


def synthesize_c(tag, rows, arg=0):
    """Testing helper: emit a struct directly from rows."""
    return _emit(tag, synthesize_from_rows(rows, arg), bitfields_from_rows(rows, arg))
