#!/usr/bin/env python3
r"""
linkfile.py — relink a reconstructed module into a byte-IDENTICAL file (Layer 3).

Per-function reconstruction proves each function assembles to the shipped bytes.
This is the next rung: assemble the WHOLE code section from our per-function .asm,
splice it into the ORIGINAL file's headers + data / import / relocation / resource
sections, and verify the rebuilt file is byte-for-byte identical to the shipped
one. That is the proof this is a REBUILDABLE Windows 95, not just a disassembly.

The link resolves what an assembler can't from one function at a time:
  - OBJ fixup sites (inter-function `call/jmp rel32`, data refs): each per-function
    .obj leaves a placeholder + fixup record; we keep the module's layout value.
  - Base relocations: absolute-address dwords (jump/pointer tables) are patched
    from the module's own .reloc table at its preferred base.
Every byte NOT in those two sets is genuine opcode from our source and must equal
the shipped binary exactly; the linked file is then byte-for-byte identical.

    python linkfile.py WRITE.EXE          # rebuild + byte-verify one module
    python linkfile.py NOTEPAD.EXE --save reconstructed/relinked/
"""
import os
import re
import sys

from engine.foundation import layout

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, ROOT)


def _code_sections(path, fmt):
    """[(name, file_off, key, raw_size)] for the CODE sections of a module. `key`
    is the routing key matched against the `sec/seg N` .asm prefix: for PE it is the
    1-based ordinal among code sections (sec1 = first code section); for NE it is the
    segment index (seg3 = NE segment 3, since code/data segments interleave)."""
    if fmt == "PE":
        from engine.format_loaders import pe
        pf = pe.open_pe(path)
        out = []
        k = 0
        for s in pf.sections:
            if (s["chars"] & 0x20) or (s["chars"] & 0x20000000):     # CNT_CODE / MEM_EXECUTE
                k += 1
                out.append((s["name"], s["rawptr"], k, s["rawsize"]))
        return out
    if fmt == "NE":
        from engine.format_loaders import ne
        nf = ne.open_ne(path) if hasattr(ne, "open_ne") else ne.NEFile(path)
        return [("seg%d" % s["idx"], s["file_off"], s["idx"], s["length"])
                for s in nf.segments() if s.get("kind") == "CODE"]
    if fmt == "LE":
        import le
        lf = le.open_le(path)
        out = []
        for i, ob in enumerate(lf.objects(), start=1):    # objN key = 1-based obj index
            if ob["exec"]:
                base, _flag = lf._page_file_off(ob["page_index"])
                out.append(("obj%d" % i, base, i, ob["vsize"]))
        return out
    raise ValueError("linkfile: unsupported format %s" % fmt)


def _asm_dir(base):
    """The reconstructed-asm directory for `base`. Emit dirs are named either by the
    full filename (notepad.exe, gdi32.dll) or by the extensionless stem (krnl386,
    gdi, user) depending on how the module was conducted, so try both and pick the
    one that actually holds .asm (never silently return an empty dir -> false link)."""
    stem = base.rsplit(".", 1)[0]
    idx = layout.asm_dir_index()                    # {module-dir name: path}, any depth
    for cand in (base.lower(), stem.lower()):
        d = idx.get(cand)
        if d and any(f.endswith(".asm") for f in os.listdir(d)):
            return d
    return None


def _asm_index(base):
    """[(sec_idx, within-section off, .asm path)] sorted. sec_idx is the 1-based
    CODE-section index from the `secN_<off>.asm` filename (sec1 = first code
    section). Single-code-section modules use sec1 throughout."""
    d = _asm_dir(base)
    out = []
    if d:
        for f in os.listdir(d):
            if not (f.endswith(".asm") and "_" in f):
                continue
            pre, tail = f.rsplit("_", 1)
            try:
                off = int(tail[:-4], 16)
            except ValueError:
                continue
            m = re.match(r"(?:se[cg]|obj)(\d+)", pre)     # PE sec / NE seg / LE obj
            out.append((int(m.group(1)) if m else 1, off, os.path.join(d, f)))
    out.sort()
    return out


def link_module(name, masmroot=None):
    """Rebuild `name` from its reconstructed .asm and byte-compare to the shipped
    file. Returns {match, n_diff, first_diff, placed, sections, rebuilt}."""
    from engine.format_loaders import objextract
    import target, masmlift, asmrecon32
    path = target.resolve(name)
    fmt = target.format_of(path)
    orig = open(path, "rb").read()
    rebuilt = bytearray(orig)
    base = os.path.basename(path)
    asm = _asm_index(base)
    if not asm:                                # no reconstructed .asm -> NOT a link:
        return {"match": False, "no_asm": True, "n_diff": len(orig), "first_diff": 0,
                "placed": 0, "fixups": 0, "relocs": 0, "n_funcs": 0, "size": len(orig),
                "sections": _code_sections(path, fmt), "rebuilt": bytes(orig),
                "fmt": fmt, "ctx": None}
    if masmroot is None:                       # NE = 16-bit MASM; PE/LE = 32-bit MASM
        masmroot = masmlift.MASMROOT if fmt == "NE" else asmrecon32.MASM32ROOT

    secs = _code_sections(path, fmt)          # (name, file_off, key, size)
    secmap = {key: foff for (_nm, foff, key, _sz) in secs}
    placed = 0
    # Assemble each function .asm ONCE and drop its bytes at its file offset, routing
    # each to its OWN code section (secN prefix). Fixup-mask sites are skipped: a fixup
    # is where the OBJ carries an address the LINKER resolves against the symbol layout
    # (inter-function call/jmp rel32, data refs). Assembled alone, each .obj leaves
    # those as placeholders; we rebuild at the shipped layout/base, so the resolved
    # value equals the byte already present (rebuilt started as the orig). Everything
    # NOT masked is genuine opcode from our source and must match the shipped bytes.
    # Assemble in chunks: one DOSBox boot reliably handles only ~40 MASM runs
    # (the autoexec command string / DOSBox stability caps it — conduct uses the
    # same CHUNK). Passing all functions in one boot silently yields ZERO .OBJ,
    # which would leave `rebuilt` an untouched copy of the shipped file and FAKE a
    # byte-identical verdict. Global F%d keys keep res lookups stable across chunks.
    CHUNK = 40
    items = [("F%d" % i, open(p, encoding="latin1").read())
             for i, (_s, _o, p) in enumerate(asm)]
    res = {}
    for c in range(0, len(items), CHUNK):
        res.update(masmlift.assemble_many(items[c:c + CHUNK],
                                          masmroot=masmroot).get("results", {}))

    # LE objects are page-mapped: a byte's file offset must go through the page map
    # (pages can be non-contiguous on disk), so resolve per-byte. PE/NE code sections
    # are contiguous, so their file offset is just the section base + within offset.
    le_ctx = None
    if fmt == "LE":
        import le
        lf = le.open_le(path)
        objs = {i: ob for i, ob in enumerate(lf.objects(), start=1) if ob["exec"]}
        le_ctx = (lf, objs, lf.page_size)

    def _foff(sec, objoff):
        if le_ctx:
            lf_, objs_, ps = le_ctx
            ob = objs_.get(sec)
            if ob is None:
                return None
            base_, _f = lf_._page_file_off(ob["page_index"] + objoff // ps)
            return base_ + objoff % ps
        return secmap.get(sec, secs[0][1]) + objoff

    fixups = 0
    for i, (sec, off, _p) in enumerate(asm):
        obj = res.get("F%d" % i, {}).get("obj")
        if not obj:
            continue
        _nm, code, mask = objextract.Obj(obj).code_segment()
        for j in range(len(code)):
            if j in mask:                       # linker-resolved site: leave layout value
                fixups += 1
                continue
            fo = _foff(sec, off + j)
            if fo is not None:
                rebuilt[fo] = code[j]
        placed += 1

    # Apply base relocations from the module's own .reloc table. Absolute-address
    # dwords in code (jump tables, function-pointer tables) are patched by the
    # LINKER to the image's actual base — they are not assembler output. MASM's
    # per-function OBJ can't know the final layout and bakes a stale value; the
    # .reloc table names exactly these sites, and at the preferred base each
    # resolves to the shipped dword. This is the link's relocation pass.
    relocs = 0
    if fmt == "PE":
        from engine.format_loaders import pe as _pe
        pf2 = _pe.open_pe(path)
        smap = [(s["rawptr"], s["vaddr"], max(s.get("vsize", 0), s["rawsize"]))
                for s in pf2.sections]

        def _rva2off(rva):
            for rpt, vaa, sz in smap:
                if vaa <= rva < vaa + sz:
                    return rpt + (rva - vaa)
            return None
        for rva in pf2.base_relocs():
            fo = _rva2off(rva)
            if fo is not None and rebuilt[fo:fo + 4] != orig[fo:fo + 4]:
                rebuilt[fo:fo + 4] = orig[fo:fo + 4]
                relocs += 1
    elif fmt == "NE":
        # NE segment relocations. On disk, a fixup source holds a CHAIN pointer (or
        # an additive value), NOT the loaded address — the loader patches it. Our
        # assembled OBJ emits the base operand instead, so restore the shipped on-
        # disk bytes at every reloc source (conduct proves the function byte-exact
        # WITH these sites masked; the PE path above does the same for base relocs).
        from engine.format_loaders import ne as _ne
        nf2 = _ne.open_ne(path) if hasattr(_ne, "open_ne") else _ne.NEFile(path)
        WIDTH = {0: 1, 2: 2, 3: 4, 5: 2, 11: 6, 13: 4}   # NE source-type -> byte width
        for s in nf2.segments():
            if s.get("kind") != "CODE":
                continue
            sfo = s["file_off"]
            for rec in nf2.relocations(s):
                w = WIDTH.get(rec["src"], 2)
                if rec.get("additive"):
                    positions = [rec["offset"]]
                else:                                    # walk the fixup chain via orig
                    positions, pos, guard = [], rec["offset"], 0
                    while pos != 0xFFFF and guard < 200000:
                        positions.append(pos)
                        if sfo + pos + 1 >= len(orig):
                            break
                        pos = orig[sfo + pos] | (orig[sfo + pos + 1] << 8)
                        guard += 1
                for pos in positions:
                    fo = sfo + pos
                    if 0 <= fo and fo + w <= len(rebuilt) and rebuilt[fo:fo + w] != orig[fo:fo + w]:
                        rebuilt[fo:fo + w] = orig[fo:fo + w]
                        relocs += 1

    diffs = [i for i in range(min(len(orig), len(rebuilt))) if rebuilt[i] != orig[i]]
    first = diffs[0] if diffs else None
    # Honest verdict: BYTE-IDENTICAL requires that EVERY carved function was
    # actually assembled from our source and placed. placed<n_funcs means some
    # functions failed to assemble (e.g. a dropped DOSBox chunk) and their bytes
    # are shipped-passthrough — a zero-diff there proves nothing about our source.
    all_placed = (placed == len(asm))
    return {"match": len(diffs) == 0 and len(rebuilt) == len(orig) and all_placed,
            "all_placed": all_placed,
            "n_diff": len(diffs), "first_diff": first, "placed": placed,
            "fixups": fixups, "relocs": relocs, "n_funcs": len(asm), "size": len(orig), "sections": secs,
            "rebuilt": bytes(rebuilt), "fmt": fmt,
            "ctx": (orig[first:first + 8].hex(), bytes(rebuilt)[first:first + 8].hex())
                   if first is not None else None}


def main():
    if len(sys.argv) < 2:
        sys.exit("usage: linkfile.py MODULE [--save DIR]")
    os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")
    name = sys.argv[1]
    r = link_module(name)
    print("=" * 60)
    print("RELINK %s  [%s]  %d bytes, %d functions" % (name, r["fmt"], r["size"], r["n_funcs"]))
    print("code sections:", [(s[0], "off 0x%X" % s[1], "%dB" % s[3]) for s in r["sections"]])
    print("functions placed: %d / %d" % (r["placed"], r["n_funcs"]))
    print("assembled opcodes placed, %d link-fixup bytes resolved" % r.get("fixups", 0))
    if r["match"]:
        print("VERDICT: BYTE-IDENTICAL — the rebuilt file == the shipped file.")
    elif not r.get("all_placed", True):
        print("VERDICT: INCOMPLETE — only %d/%d functions assembled+placed from our "
              "source; the rest are shipped-passthrough, so a zero diff proves "
              "nothing. (Assembly likely dropped a chunk.)" % (r["placed"], r["n_funcs"]))
        if r["n_diff"]:
            print("  (%d byte(s) also differ, first at 0x%X)" % (r["n_diff"], r["first_diff"]))
    else:
        print("VERDICT: %d byte(s) differ (first at 0x%X)" % (r["n_diff"], r["first_diff"]))
        if r["ctx"]:
            print("  shipped %s  vs  rebuilt %s" % r["ctx"])
    print("=" * 60)
    if "--save" in sys.argv:
        d = sys.argv[sys.argv.index("--save") + 1]
        os.makedirs(d, exist_ok=True)
        p = os.path.join(d, os.path.basename(name).lower())
        open(p, "wb").write(r["rebuilt"])
        print("saved rebuilt file:", os.path.abspath(p))


if __name__ == "__main__":
    main()
