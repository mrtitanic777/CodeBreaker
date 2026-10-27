"""take_apart.py -- what the Workbench does with a program dropped on it.

    from codebreaker.ui import take_apart
    report = take_apart.reverse(path, progress=callback, outbase=folder)

Delivery-agnostic: a window calls it, so could anything else. It does the real work with
CodeBreaker's own readers and decompiler -- no placeholder stages:

  * read the container (PE, NE, LE, DOS) -- sections or segments, imports, exports, entry point
  * disassemble every executable section in full (resyncing past data)
  * find function entry points from call targets, prologues, the exports and the entry
  * decompile the whole program to C with the decompiler (decompile/decompiler.py) -- the same
    text `python -m codebreaker decompile` writes: readable, NOT built or judged. `module` is
    what rebuilds it and runs it beside the original.

`progress` gets ("stages", [names]) once, then ("stage", index, caption) as each begins. With
`outbase`, everything is written to the program's project folder and the report lists it: its
Converted/<name>/ (codebreaker/base/converted.py) when `outbase` is "converted", else a folder
of the same layout under `outbase`: original/ (the program), src/reconstructed.c, reports/
(structure, disassembly, functions, hex dump), README.txt.
"""
import os
import struct

_MAX_INSNS = 4_000_000        # a ceiling on one section's linear sweep


def _pe_bits(data):
    """32 or 64 from the PE optional-header magic (0x10b PE32 / 0x20b PE32+), so the stage
    list is known before the full parse. Defaults to 32 on any malformed header."""
    try:
        off = struct.unpack_from("<I", data, 0x3c)[0]
        return 64 if struct.unpack_from("<H", data, off + 24)[0] == 0x20B else 32
    except Exception:
        return 32


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


def _disasm_section(code, bits, base):
    """Full linear disassembly of a section, resyncing one byte at a time past bytes that
    don't decode (data/padding). Returns a list of (addr, size, mnemonic, op_str, bytes)."""
    import capstone
    mode = {16: capstone.CS_MODE_16, 32: capstone.CS_MODE_32,
            64: capstone.CS_MODE_64}[bits]
    md = capstone.Cs(capstone.CS_ARCH_X86, mode)
    insns, off, n = [], 0, len(code)
    while off < n and len(insns) < _MAX_INSNS:
        progressed = False
        for insn in md.disasm(code[off:], base + off):
            insns.append((insn.address, insn.size, insn.mnemonic, insn.op_str, insn.bytes))
            off = insn.address - base + insn.size
            progressed = True
            if len(insns) >= _MAX_INSNS:
                break
        if not progressed:
            off += 1                      # undecodable byte -> resync
    return insns


def _fmt_insns(insns):
    return "\n".join("  %08x  %-20s  %s %s"
                     % (a, " ".join("%02x" % b for b in bs), mn, op)
                     for (a, sz, mn, op, bs) in insns)


_HOTPATCH = ("edi, edi", "esi, esi", "di, di")   # 2-byte MSVC hot-patch pad before a prologue


def _carve(insns, ranges, seeds):
    """Recover function entry points: the entry/export seeds, every `push (e)bp; mov
    (e)bp,(e)sp` prologue, and every direct-call target that lands on an instruction inside a
    code range. A prologue's canonical start is its hot-patch pad (`mov edi,edi`) when present;
    the `push`/`mov` interior of a prologue is recorded as mid-prologue and never kept as an
    entry, so a call that targets the `push ebp` doesn't split one function into two."""
    def in_code(a):
        return any(lo <= a < hi for lo, hi in ranges)
    starts = {ins[0] for ins in insns}               # valid instruction boundaries
    entries, mid = set(), set()
    for i, (a, sz, mn, op, bs) in enumerate(insns):
        if mn == "push" and op in ("ebp", "bp", "rbp") and i + 1 < len(insns):
            n_mn, n_op = insns[i + 1][2], insns[i + 1][3]
            if n_mn == "mov" and n_op in ("ebp, esp", "bp, sp", "rbp, rsp"):
                start = a
                if i > 0 and insns[i - 1][2] == "mov" and insns[i - 1][3] in _HOTPATCH:
                    start = insns[i - 1][0]           # canonical start is the hot-patch pad
                    mid.add(a)                         # ...so the `push` is now mid-prologue
                entries.add(start)
                mid.add(insns[i + 1][0])              # the `mov (e)bp,(e)sp` is mid-prologue
        elif mn == "call":
            t = op.strip()
            if t.startswith("0x"):
                try:
                    tv = int(t, 16)
                    if in_code(tv) and tv in starts:
                        entries.add(tv)
                except ValueError:
                    pass
    for s in seeds:
        if in_code(s) and s in starts:
            entries.add(s)
    return sorted(entries - mid)


def _save_artifacts(rep, srcpath, outbase, texts):
    """Write everything produced into the program's project folder (see the module's notes);
    record the (label, path) artifacts so the UI can offer buttons to open them."""
    from codebreaker.base import converted
    proj = converted.project(srcpath, base=None if outbase == "converted" else outbase)
    arts = []
    if proj.original_file and os.path.isfile(proj.original_file):
        arts.append(("Original binary", proj.original_file))
    written = []
    for label, fname, folder in (("Structure & imports", "structure.txt", proj.reports),
                                 ("Disassembly", "disassembly.asm", proj.reports),
                                 ("Functions", "functions.txt", proj.reports),
                                 ("Reconstructed C", "reconstructed.c", proj.src),
                                 ("Hex dump", "hexdump.txt", proj.reports)):
        body = texts.get(fname)
        if not body:
            continue
        p = os.path.join(folder, fname)
        with open(p, "w", encoding="utf-8", newline="\n") as f:
            f.write(body)
        arts.append((label, p))
        written.append(p)
    proj.record("take-apart", written, notes="the program read, disassembled and decompiled to C "
                "(readable; not built or judged)")
    rep["outdir"] = proj.root
    rep["artifacts"] = arts


def reverse(path, progress=None, outbase=None):
    """Take a program apart. -> report {name, size, format, structure, exports, imports,
    functions, insn_count, notes, disasm[, outdir, artifacts]}"""
    def emit(kind, *a):
        if progress:
            progress(kind, *a)

    with open(path, "rb") as f:
        data = f.read()
    fmt = format_of(data)
    is16 = fmt.startswith(("NE", "MZ"))
    disbits = 16 if is16 else (_pe_bits(data) if fmt.startswith("PE") else 32)
    rep = {"name": os.path.basename(path), "size": len(data), "format": fmt,
           "structure": [], "exports": [], "imports": [], "functions": 0, "insn_count": 0, "notes": []}
    stages = ["Reading the file", "Reading its structure", "Disassembling", "Finding functions",
              "Decompiling to C"]
    emit("stages", stages)
    emit("stage", 0, stages[0])
    struct_lines = ["%s   %s   %s bytes" % (rep["name"], fmt, format(len(data), ","))]
    code_secs, seeds, texts = [], [], {}

    emit("stage", 1, stages[1])
    try:
        if fmt.startswith("PE"):
            from codebreaker.formats.pe import PEFile
            P = PEFile(path)
            disbits = 64 if P.is_plus else 32
            rep["format"] = fmt = "%s (%s)" % ("PE32+" if P.is_plus else "PE32", P.machine_name())
            rep["structure"] = [{"k": "section", "n": s.get("name", "?"),
                                 "d": "0x%x .. +0x%x" % (s.get("vaddr", 0), s.get("vsize", 0))} for s in P.sections]
            struct_lines.append("entry 0x%x   image_base 0x%x   %s" % (P.image_base + P.entry, P.image_base,
                                                                       "DLL" if P.is_dll() else "EXE"))
            struct_lines += ["", "SECTIONS"] + ["  %-8s 0x%08x  +0x%x" % (s["name"], P.image_base + s["vaddr"], s["vsize"])
                                                for s in P.sections]
            try:
                rep["exports"] = [{"name": nm, "ord": o} for o, nm, _rva in list(P.exports())[:200] if nm]
            except Exception:
                pass
            imps = {}
            try:
                for _addr, (dll, nm) in P.imports().items():
                    imps.setdefault(dll, []).append(nm)
            except Exception:
                pass
            rep["imports"] = [{"dll": d, "count": len(v)} for d, v in imps.items()]
            if imps:
                struct_lines += ["", "IMPORTS (%d DLLs)" % len(imps)]
                struct_lines += ["  %-16s %d symbols" % (d, len(v)) for d, v in sorted(imps.items())]
            if rep["exports"]:
                struct_lines += ["", "EXPORTS (%d)" % len(rep["exports"])]
                struct_lines += ["  %s  @%s" % (e["name"], e["ord"]) for e in rep["exports"]]
            code_secs = list(P.code_sections())
            seeds = [P.image_base + P.entry] + [P.image_base + rva for _o, _nm, rva in
                                                (list(P.exports()) if rep["exports"] else [])]
        elif fmt.startswith("NE"):
            from codebreaker.formats.ne import NEFile
            o = NEFile(path)
            rep["module"] = o.module_name()
            segs = o.segments()
            rep["structure"] = [{"k": "segment", "n": "seg%d" % (i + 1),
                                 "d": "%d bytes @ 0x%x" % (s.get("length", 0), s.get("file_off", 0))}
                                for i, s in enumerate(segs)]
            names = (o.resident_names()[1:] if len(o.resident_names()) > 1 else []) + o.nonresident_names()
            rep["exports"] = [{"name": n, "ord": ordinal} for n, ordinal in names[:200] if n]
            struct_lines += ["module %s   %d segments" % (o.module_name(), len(segs)), "", "SEGMENTS"]
            struct_lines += ["  seg%-3d %6d bytes @ 0x%06x" % (i + 1, s.get("length", 0), s.get("file_off", 0))
                             for i, s in enumerate(segs)]
            refs = o.module_refs()
            if refs:
                struct_lines += ["", "MODULE REFS"] + ["  %s" % r for r in refs]
            if rep["exports"]:
                struct_lines += ["", "EXPORTS (%d)" % len(rep["exports"])]
                struct_lines += ["  %s  @%s" % (e["name"], e["ord"]) for e in rep["exports"]]
            for i, s in enumerate(segs):
                fo, ln = s.get("file_off", 0), s.get("length", 0)
                if ln and s.get("kind") == "CODE":
                    code_secs.append((0, data[fo:fo + ln], "seg%d" % (i + 1)))
        elif fmt.startswith(("LE", "W3")):
            from codebreaker.formats import le
            for member, v in le.open_vxds(path):
                struct_lines.append("driver %s: %d objects" % (member, len(v.objects)))
                for i, ob in enumerate(v.objects):
                    if ob.get("flags", 0) & 0x4:                   # executable object
                        code_secs.append((0, v.object_bytes(ob), "%s.obj%d" % (member, i + 1)))
            disbits = 32
        else:
            rep["notes"].append("no structured reader for %s; read as flat 16-bit code" % fmt)
            code_secs = [(0, data, "image")]
    except Exception as e:
        rep["notes"].append("structure: %s" % e)
    texts["structure.txt"] = "\n".join(struct_lines)

    emit("stage", 2, stages[2])
    all_insns, parts, ranges = [], [], []
    try:
        for base, raw, name in code_secs:
            if not raw:
                continue
            insns = _disasm_section(bytes(raw), disbits, base)
            all_insns.append(insns)
            ranges.append((base, base + len(raw)))
            parts.append("; ---- %s  (%d bytes, %d instructions) ----" % (name, len(raw), len(insns)))
            parts.append(_fmt_insns(insns))
    except ImportError:
        rep["notes"].append("capstone is not installed; no disassembly")
    rep["insn_count"] = sum(len(i) for i in all_insns)
    texts["disassembly.asm"] = "\n".join(parts)

    emit("stage", 3, stages[3])
    flat = [ins for i in all_insns for ins in i]
    funcs = _carve(flat, ranges, seeds) if flat else []
    rep["functions"] = len(funcs)
    if funcs:
        texts["functions.txt"] = ("%d function entry points (call targets, prologues, exports)\n\n" % len(funcs)) + \
            "\n".join("  %08x" % a for a in funcs)

    emit("stage", 4, stages[4])
    try:
        from codebreaker.decompile import decompiler
        text, done, total = decompiler.as_c(path)
        texts["reconstructed.c"] = text
        rep["notes"].append("decompiled %d of %d functions to C (readable; not built or judged -- "
                            "`python -m codebreaker module` does that)" % (done, total))
    except Exception as e:
        rep["notes"].append("decompile: %s" % e)

    # (the scan's count, not the decompiler's: it follows calls to code the scan did not take for
    # a function -- COMPOBJ's 506 here, 752 decompiled -- and it had read as more done than found)
    rep["notes"].append("%d code section(s), %s instructions, %d functions found by the first scan"
                        % (len(code_secs), format(rep["insn_count"], ","), rep["functions"]))
    texts["hexdump.txt"] = hexdump(data, 512)
    rep["disasm"] = texts["disassembly.asm"][:20000]
    if outbase:
        try:
            _save_artifacts(rep, path, outbase, texts)
        except Exception as e:
            rep["notes"].append("save: %s" % e)
    emit("stage", len(stages), "done")
    return rep


def main(argv):
    """python -m codebreaker take-apart <program> [--out FOLDER]"""
    from codebreaker.base import corpus
    if not argv:
        print(main.__doc__)
        return 2
    path = argv[0] if os.path.isfile(argv[0]) else corpus.retail(argv[0])
    out = argv[argv.index("--out") + 1] if "--out" in argv else "converted"

    def show(kind, *a):
        if kind == "stage" and a[0] < 5:
            print("  %s" % a[1])
    rep = reverse(path, show, out)
    for n in rep["notes"]:
        print(n)
    print("written to %s" % rep.get("outdir"))
    return 0
