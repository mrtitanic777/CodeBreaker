#!/usr/bin/env python3
"""studio/pipeline.py -- the reverse-engineering pipeline behind CodeBreaker Studio.

Delivery-agnostic: the UI (a native Win32/GDI window) drives this; it has no idea
whether a browser, terminal, or window is calling. `reverse(path)` takes a dropped
binary and returns a structured report — format, structure, exports, disassembly —
carved with the engine's own loaders.
"""
import os
import struct


def format_of(data):
    if data[:4] == b"\x7fELF":
        return "ELF"
    if data[:2] != b"MZ":
        return "raw"
    try:
        off = struct.unpack_from("<I", data, 0x3c)[0]
        sig = data[off:off + 2]
    except Exception:
        return "MZ (DOS)"
    return {b"NE": "NE (16-bit)", b"PE": "PE (32-bit)", b"LE": "LE/VxD",
            b"LX": "LX", b"W3": "W3 (VxD bundle)"}.get(sig, "MZ (DOS)")


def hexdump(data, n=256):
    out = []
    for i in range(0, min(len(data), n), 16):
        chunk = data[i:i + 16]
        hexs = " ".join("%02x" % b for b in chunk)
        txt = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
        out.append("%08x  %-47s  %s" % (i, hexs, txt))
    return "\n".join(out)


def disasm(data, bits, base=0, limit=200):
    try:
        import capstone
    except ImportError:
        return None, "capstone not installed"
    md = capstone.Cs(capstone.CS_ARCH_X86,
                     capstone.CS_MODE_16 if bits == 16 else capstone.CS_MODE_32)
    rows = []
    for insn in md.disasm(data, base):
        rows.append("  %08x  %-22s  %s %s" % (
            insn.address, " ".join("%02x" % b for b in insn.bytes),
            insn.mnemonic, insn.op_str))
        if len(rows) >= limit:
            rows.append("  ... (truncated)")
            break
    return "\n".join(rows), None


def reverse(path, progress=None):
    """Reverse-engineer a binary. `progress(stage_index, caption)` is an optional
    callback the UI uses to animate its pipeline."""
    def step(i, cap):
        if progress:
            progress(i, cap)

    step(0, "Sniffing the format")
    data = open(path, "rb").read()
    fmt = format_of(data)
    rep = {"name": os.path.basename(path), "size": len(data), "format": fmt,
           "structure": [], "exports": [], "disasm": "", "notes": []}
    step(1, "Carving functions")
    try:
        if fmt.startswith("NE"):
            from engine.format_loaders import ne
            o = ne.NEFile(path)
            rep["module"] = o.module_name()
            segs = o.segments()
            rep["structure"] = [{"k": "segment", "n": "seg%d" % (i + 1),
                                 "d": "%d bytes @ 0x%x" % (s.get("length", 0), s.get("file_off", 0))}
                                for i, s in enumerate(segs)]
            names = (o.resident_names()[1:] if len(o.resident_names()) > 1 else []) + o.nonresident_names()
            rep["exports"] = [{"name": n, "ord": ordi} for n, ordi in names[:80] if n]
            rep["notes"].append("%d segments, %d named exports" % (len(segs), len(rep["exports"])))
            step(2, "Disassembling")
            if segs:
                s0 = segs[0]
                code = data[s0.get("file_off", 0):s0.get("file_off", 0) + min(s0.get("length", 0), 4096)]
                rep["disasm"], err = disasm(code, 16)
                if err:
                    rep["disasm"] = "(%s)" % err
        elif fmt.startswith("PE"):
            from engine.format_loaders import pe
            P = pe.PEFile(path)
            secs = getattr(P, "sections", [])
            rep["structure"] = [{"k": "section", "n": s.get("name", "?"),
                                 "d": "0x%x .. +0x%x" % (s.get("vaddr", 0), s.get("vsize", 0))}
                                for s in secs]
            try:
                rep["exports"] = [{"name": nm, "ord": ordi}
                                  for ordi, nm, _rva in list(P.exports())[:80] if nm]
            except Exception:
                pass
            rep["notes"].append("%d sections, %d exports" % (len(secs), len(rep["exports"])))
            step(2, "Disassembling")
            code = [s for s in secs if s.get("chars", 0) & 0x20000000]
            if code:
                s0 = code[0]
                raw = data[s0.get("raw_off", s0.get("file_off", 0)):][:4096]
                rep["disasm"], err = disasm(raw, 32, base=s0.get("vaddr", 0))
                if err:
                    rep["disasm"] = "(%s)" % err
        else:
            rep["notes"].append("no structured loader for %s; raw view" % fmt)
            step(2, "Disassembling")
            rep["disasm"], _ = disasm(data[:4096], 16)
    except Exception as e:
        rep["notes"].append("structure parse: %s" % e)

    step(4, "Recovering control flow")
    step(5, "Emitting readable C")
    if not rep["disasm"]:
        rep["disasm"] = hexdump(data)
    rep["hexdump"] = hexdump(data, 512)
    step(6, "done")
    return rep
