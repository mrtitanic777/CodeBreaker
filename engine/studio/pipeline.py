#!/usr/bin/env python3
"""studio/pipeline.py -- the reverse-engineering pipeline behind CodeBreaker Studio.

Delivery-agnostic: the native Studio UI drives this; it has no idea whether a window,
terminal, or anything else is calling. `reverse(path)` takes a dropped binary and does
REAL work with the engine's own loaders and capstone -- no placeholder stages:

  * parse the container (PE / NE) -- sections/segments, imports, exports, entry point
  * disassemble EVERY executable section in full (resyncing past data), not a 4 KB sample
  * carve function entry points from call targets + prologues + the export/entry set
  * for 16/32-bit input, decompile every carved function to readable C with the IR engine
    (il.py) -- EXPERIMENTAL: a decompile of live disassembly, readable but not byte-exact-
    proven. x86-64 is not modeled, so this stage runs for 16/32-bit code only.

The stage list is format-driven and reported to the UI up front, so the progress display
reflects the work that is actually performed -- and nothing that isn't. The C is a decompile
(Ghidra/Hex-Rays class), NOT the byte-exact reconstruction the corpora get from the full
per-binary campaign; the Studio never claims otherwise.
"""
import os
import re
import shutil
import struct
import tempfile

_MAX_INSNS = 4_000_000        # safety ceiling on a single-section linear sweep
_MAX_DECOMPILE = 5000         # cap functions sent through il.py so the UI stays responsive


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


def _imp_sym(r, mods, o):
    """A C identifier for an NE import relocation: MODULE_<name> when the imported-name table
    carries a name; for an ORDINAL import, resolve the ordinal to the real API name via the
    EXPORTING module's export table (impresolve) -> MODULE_<API>, falling back to MODULE_ord<N>
    only when that module's exports aren't available."""
    raw_mod = mods[r["module"] - 1] if mods and 0 < r.get("module", 0) <= len(mods) else "IMP"
    mod = re.sub(r"\W", "_", raw_mod)
    if r.get("kind") == "IMPORTNAME":
        try:
            nm = o.imported_name(r["name_off"])
        except Exception:
            nm = "n%x" % r.get("name_off", 0)
        return "%s_%s" % (mod, re.sub(r"\W", "_", nm) or "sym")
    ordn = r.get("ordinal", 0)
    try:                                                  # ordinal -> real API name (ground truth)
        from engine.symbol_engine import impresolve
        nm = impresolve.resolve(raw_mod, ordn)
    except Exception:
        nm = None
    if nm:
        return "%s_%s" % (mod, re.sub(r"\W", "_", nm))
    return "%s_ord%d" % (mod, ordn)


def _ne_import_map(o, raw, seg, mods):
    """{segment_offset -> C-symbol} for every imported fixup site in a code segment. NE stores
    same-target fixups as a linked chain (each placeholder word points at the next site,
    0xffff terminates), so each relocation record is walked to name all of its sites."""
    out = {}
    try:
        recs = o.relocations(seg)
    except Exception:
        return out
    n = len(raw)
    for r in recs:
        if r.get("kind") not in ("IMPORTNAME", "IMPORTORD"):
            continue
        sym = _imp_sym(r, mods, o)
        site, seen = r.get("offset", 0xffff), set()
        while site != 0xffff and 0 <= site + 1 < n and site not in seen:
            seen.add(site)
            out[site] = sym
            if r.get("additive"):
                break
            site = raw[site] | (raw[site + 1] << 8)   # placeholder word = next chain site
    return out


def _il_asm_text(insns, bits, imports=None):
    """Format a carved function's instructions into the minimal .asm dialect il.py parses:
    `use32` marks 32-bit; intra-function jump targets get `L_<off>:` labels; each line
    carries `; <off> <disasm>`. il.py digests capstone's Intel syntax directly."""
    if len(insns) > 1 and insns[0][2] == "mov" and insns[0][3] in _HOTPATCH:
        insns = insns[1:]          # drop the leading hot-patch no-op so il.py sees a clean
                                   # `push ebp; mov ebp,esp` prologue and elides the frame setup
    lo, hi = insns[0][0], insns[-1][0] + insns[-1][1]
    targets = set()                # computed from ORIGINAL operands (before any import rewrite)
    for a, sz, mn, op, bs in insns:
        if mn[:1] == "j" or mn == "call" or mn.startswith("loop"):
            t = op.strip()
            if t.startswith("0x"):
                try:
                    tv = int(t, 16)
                    if lo <= tv < hi:
                        targets.add(tv)
                except ValueError:
                    pass
    out = ["_TEXT segment use32" if bits == 32 else "_TEXT segment"]
    renames = {}                   # sub_<addr> emitted by il.py  ->  resolved import symbol
    for a, sz, mn, op, bs in insns:
        if a in targets:
            out.append("L_%x:" % a)
        e_mn, e_op = mn, op
        if imports and mn in ("call", "lcall"):
            sym = None
            m = re.match(r"(?:d?word ptr\s*)?\[0x([0-9a-f]+)\]$", op.strip())
            if m:                          # PE: indirect call through an IAT slot -> [0xVA]
                sym = imports.get(int(m.group(1), 16))
            if sym is None:                # NE: far-call fixup site lives in the instr bytes
                site = next((off for off in range(a, a + sz) if off in imports), None)
                if site is not None:
                    sym = imports[site]
            if sym is not None:            # an imported call: give it a unique hex target
                e_mn, e_op = "call", "0x%x" % a   # (its own addr) so il emits sub_<addr>,
                renames["sub_%x" % a] = sym       # which we then rename to the symbol
        insn = ("%s %s" % (e_mn, e_op)).strip()
        out.append("        %-30s ; %x %s" % (insn, a, ("%s %s" % (mn, op)).strip()))
    out += ["_TEXT ends", "        end"]
    return "\n".join(out) + "\n", renames


def _decompile_il(all_insns, bits, tmp, sec_imports=None):
    """Run the IR engine (il.py) over every carved function -> readable C. This is a
    decompile of LIVE disassembly, not a byte-exact reconstruction -- readable but
    unverified. Imported calls are renamed from the NE import tables (see sec_imports, a
    per-section {offset: symbol} map). Returns (c_text, lifted, skipped, capped, externs)."""
    from engine.ir_engine import il
    out, lifted, skipped, capped, externs = [], 0, 0, False, {}
    for si, (base, end, insns) in enumerate(all_insns):
        if not insns:
            continue
        imports = sec_imports[si] if sec_imports and si < len(sec_imports) else None
        ents = _carve(insns, [(base, end)], [])
        byoff = {a: i for i, (a, *_r) in enumerate(insns)}
        entset = set(ents)
        for a in ents:
            if a not in byoff:
                continue
            if lifted + skipped >= _MAX_DECOMPILE:
                capped = True
                break
            i = byoff[a]
            fi = []
            for j in range(i, len(insns)):
                fi.append(insns[j])
                if insns[j][2] in ("ret", "retf"):
                    break
                if j + 1 < len(insns) and insns[j + 1][0] in entset and j > i:
                    break
            if len(fi) < 3:
                continue
            asm, renames = _il_asm_text(fi, bits, imports)
            with open(tmp, "w", encoding="latin1") as f:
                f.write(asm)
            try:
                c = il.lift_asm_function(tmp, "sub_%x" % a)
                for k, v in renames.items():
                    c = re.sub(r"\b%s\b" % re.escape(k), v, c)
                    externs[v] = True
                out.append(c)
                lifted += 1
            except Exception:
                skipped += 1
        if capped:
            break
    return "\n".join(out), lifted, skipped, capped, sorted(externs)


def _save_artifacts(rep, srcpath, outbase, texts):
    """Write everything produced into a fresh per-file project folder; record the
    (label, path) artifacts so the UI can offer buttons to open them."""
    stem = os.path.splitext(os.path.basename(srcpath))[0] or "binary"
    proj = os.path.join(outbase, stem)
    n = 2
    while os.path.exists(proj):
        proj = os.path.join(outbase, "%s_%d" % (stem, n)); n += 1
    os.makedirs(proj, exist_ok=True)
    arts = []
    orig = os.path.join(proj, os.path.basename(srcpath))
    try:
        shutil.copy2(srcpath, orig); arts.append(("Original binary", orig))
    except Exception:
        pass
    for label, fname in (("Structure & imports", "structure.txt"),
                         ("Disassembly", "disassembly.asm"),
                         ("Functions", "functions.txt"),
                         ("Reconstructed C", "reconstructed.c"),
                         ("Hex dump", "hexdump.txt")):
        body = texts.get(fname)
        if not body:
            continue
        p = os.path.join(proj, fname)
        with open(p, "w", encoding="utf-8", newline="\n") as f:
            f.write(body)
        arts.append((label, p))
    rep["outdir"] = proj
    rep["artifacts"] = arts


def reverse(path, progress=None, outbase=None):
    """Reverse-engineer a binary for real. `progress` gets ("stages", [names]) once, then
    ("stage", index, caption) as each stage begins. With `outbase`, artifacts are written to
    a per-file project folder and the report carries `outdir` + `artifacts`."""
    def emit(kind, *a):
        if progress:
            progress(kind, *a)

    data = open(path, "rb").read()
    fmt = format_of(data)
    is16 = fmt.startswith("NE") or fmt.startswith("MZ") or fmt.startswith("LE")
    disbits = 16 if is16 else (_pe_bits(data) if fmt.startswith("PE") else 32)
    decompilable = disbits in (16, 32)      # il.py models 16/32-bit x86, not x86-64
    rep = {"name": os.path.basename(path), "size": len(data), "format": fmt,
           "structure": [], "exports": [], "imports": [], "functions": 0,
           "insn_count": 0, "notes": []}

    stages = ["Reading & sniffing format", "Parsing structure",
              "Disassembling code", "Carving functions"]
    if decompilable:
        stages.append("Decompiling to C (experimental)")
    emit("stages", stages)

    emit("stage", 0, "Reading & sniffing format")
    struct_lines = ["%s   %s   %s bytes" % (rep["name"], fmt, format(len(data), ","))]
    code_secs = []          # [(base, raw_bytes, name)]
    seeds = []              # entry point + export addresses (function seeds)
    sec_import_by_name = {}  # NE only: section name -> {offset: import symbol}
    global_import_map = None  # PE: {IAT VA -> import symbol}, applies to every code section
    texts = {}

    emit("stage", 1, "Parsing structure")
    try:
        if fmt.startswith("PE"):
            from engine.format_loaders import pe
            P = pe.PEFile(path)
            disbits = 64 if P.is_plus else 32
            rep["format"] = fmt = "%s (%s)" % ("PE32+" if P.is_plus else "PE32", P.machine_name())
            rep["structure"] = [{"k": "section", "n": s.get("name", "?"),
                                 "d": "0x%x .. +0x%x" % (s.get("vaddr", 0), s.get("vsize", 0))}
                                for s in P.sections]
            struct_lines.append("entry 0x%x   image_base 0x%x   %s"
                                % (P.image_base + P.entry, P.image_base,
                                   "DLL" if P.is_dll() else "EXE"))
            struct_lines += ["", "SECTIONS"] + ["  %-8s 0x%08x  +0x%x"
                             % (s["name"], P.image_base + s["vaddr"], s["vsize"]) for s in P.sections]
            try:
                rep["exports"] = [{"name": nm, "ord": o}
                                  for o, nm, _rva in list(P.exports())[:200] if nm]
            except Exception:
                pass
            imps = {}
            try:
                for _addr, (dll, nm) in P.imports().items():
                    imps.setdefault(dll, []).append(nm)
            except Exception:
                pass
            rep["imports"] = [{"dll": d, "count": len(v)} for d, v in imps.items()]
            # {IAT VA -> C symbol}: by-name imports carry the API name; by-ordinal (#N) resolve
            # via the exporting DLL's export table (impresolve), else fall back to MODULE_ordN.
            try:
                from engine.symbol_engine import impresolve
                gm = {}
                for va, (dll, nm) in P.imports().items():
                    modid = re.sub(r"\W", "_", dll.rsplit(".", 1)[0]).upper()
                    if isinstance(nm, str) and nm.startswith("#"):
                        api = None
                        try:
                            api = impresolve.resolve(dll, int(nm[1:]))
                        except Exception:
                            api = None
                        api = api or ("ord" + nm[1:])
                    else:
                        api = nm
                    gm[va] = "%s_%s" % (modid, re.sub(r"\W", "_", str(api)))
                if gm:
                    global_import_map = gm
            except Exception:
                pass
            if imps:
                struct_lines += ["", "IMPORTS (%d DLLs)" % len(imps)]
                for d, v in sorted(imps.items()):
                    struct_lines.append("  %-16s %d symbols" % (d, len(v)))
            if rep["exports"]:
                struct_lines += ["", "EXPORTS (%d)" % len(rep["exports"])]
                struct_lines += ["  %s%s" % (e["name"], "  @%s" % e["ord"] if e["ord"] is not None else "")
                                 for e in rep["exports"]]
            code_secs = list(P.code_sections())
            seeds = [P.image_base + P.entry] + [P.image_base + rva for _o, nm, rva in
                                                (list(P.exports()) if rep["exports"] else [])]
        elif fmt.startswith("NE"):
            from engine.format_loaders import ne
            o = ne.NEFile(path)
            rep["module"] = o.module_name()
            segs = o.segments()
            rep["structure"] = [{"k": "segment", "n": "seg%d" % (i + 1),
                                 "d": "%d bytes @ 0x%x" % (s.get("length", 0), s.get("file_off", 0))}
                                for i, s in enumerate(segs)]
            names = (o.resident_names()[1:] if len(o.resident_names()) > 1 else []) + o.nonresident_names()
            rep["exports"] = [{"name": n, "ord": ordi} for n, ordi in names[:200] if n]
            struct_lines += ["module %s   %d segments" % (o.module_name(), len(segs)), "", "SEGMENTS"]
            struct_lines += ["  seg%-3d %6d bytes @ 0x%06x" % (i + 1, s.get("length", 0), s.get("file_off", 0))
                             for i, s in enumerate(segs)]
            try:
                refs = o.module_refs()
                if refs:
                    struct_lines += ["", "MODULE REFS"] + ["  %s" % r for r in refs]
            except Exception:
                pass
            if rep["exports"]:
                struct_lines += ["", "EXPORTS (%d)" % len(rep["exports"])]
                struct_lines += ["  %s%s" % (e["name"], "  @%s" % e["ord"] if e["ord"] is not None else "")
                                 for e in rep["exports"]]
            mods = o.module_refs()
            for i, s in enumerate(segs):
                fo, ln = s.get("file_off", 0), s.get("length", 0)
                if ln:
                    segraw = data[fo:fo + ln]
                    nm = "seg%d" % (i + 1)
                    code_secs.append((0, segraw, nm))
                    if s.get("kind") == "CODE":
                        imap = _ne_import_map(o, segraw, s, mods)
                        if imap:
                            sec_import_by_name[nm] = imap
        else:
            rep["notes"].append("no structured loader for %s; flat 16-bit view" % fmt)
            code_secs = [(0, data, "image")]
    except Exception as e:
        rep["notes"].append("structure parse: %s" % e)
    texts["structure.txt"] = "\n".join(struct_lines)

    emit("stage", 2, "Disassembling code")
    all_insns, disasm_parts, ranges, all_imports = [], [], [], []
    try:
        for base, raw, name in code_secs:
            if not raw:
                continue
            insns = _disasm_section(bytes(raw), disbits, base)
            all_insns.append((base, base + len(raw), insns))
            all_imports.append(sec_import_by_name.get(name) or global_import_map)
            ranges.append((base, base + len(raw)))
            disasm_parts.append("; ---- %s  (%d bytes, %d instructions) ----"
                                % (name, len(raw), len(insns)))
            disasm_parts.append(_fmt_insns(insns))
    except ImportError:
        rep["notes"].append("capstone not installed; disassembly skipped")
    rep["insn_count"] = sum(len(i) for _a, _b, i in all_insns)
    texts["disassembly.asm"] = "\n".join(disasm_parts)

    emit("stage", 3, "Carving functions")
    flat = [ins for _a, _b, i in all_insns for ins in i]
    funcs = _carve(flat, ranges, seeds) if flat else []
    rep["functions"] = len(funcs)
    if funcs:
        texts["functions.txt"] = ("%d function entry points (call-target + prologue scan)\n\n"
                                  % len(funcs)) + "\n".join("  %08x" % a for a in funcs)

    if decompilable and all_insns:
        emit("stage", 4, "Decompiling to C (experimental)")
        ctext, li, sk, capped, externs = "", 0, 0, False, []
        tmp = os.path.join(tempfile.gettempdir(), "cb_il_%d.asm" % os.getpid())
        try:
            ctext, li, sk, capped, externs = _decompile_il(all_insns, disbits, tmp, all_imports)
        except Exception as e:
            rep["notes"].append("decompile: %s" % e)
        finally:
            try:
                os.remove(tmp)
            except OSError:
                pass
        rep["notes"].append("IR engine (il.py) decompiled %d function(s) to readable C, %d skipped%s "
                            "(EXPERIMENTAL -- readable but not byte-exact-proven)"
                            % (li, sk, " (capped)" if capped else ""))
        if externs:
            rep["notes"].append("named %d imported symbol(s) from the NE import table" % len(externs))
        if ctext.strip():
            header = (
                "/* EXPERIMENTAL decompilation by the CodeBreaker IR engine (il.py) -- the same\n"
                "   engine behind the byte-exact reconstructed corpora, here run on LIVE\n"
                "   disassembly. It is READABLE but UNVERIFIED: not proven byte-exact against the\n"
                "   original (that needs the per-binary reconstruction campaign). Treat it as a\n"
                "   high-quality decompile -- like Ghidra/Hex-Rays output: a guide, not ground\n"
                "   truth. x86-64 is not modeled, so this stage runs for 16/32-bit code only. */\n\n")
            if externs:
                header += ("/* Imported calls are named from the module's import tables (NE module-\n"
                           "   reference + imported-name, or PE IAT); ordinal imports are resolved to\n"
                           "   the exporting module's export names, else MODULE_ordN. */\n"
                           + "".join("extern int %s();\n" % e for e in externs) + "\n")
            texts["reconstructed.c"] = header + ctext

    rep["notes"].append("%d code section(s), %s instructions, %d functions"
                        % (len(code_secs), format(rep["insn_count"], ","), rep["functions"]))
    texts["hexdump.txt"] = hexdump(data, 512)
    rep["disasm"] = texts["disassembly.asm"][:20000]        # preview kept in-report

    if outbase:
        try:
            _save_artifacts(rep, path, outbase, texts)
        except Exception as e:
            rep["notes"].append("save: %s" % e)
    emit("stage", len(stages), "done")
    return rep
