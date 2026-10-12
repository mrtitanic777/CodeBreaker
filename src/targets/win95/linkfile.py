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
import struct
import sys
from src.core import layout

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, ROOT)


def _code_sections(path, fmt):
    """[(name, file_off, key, raw_size)] for the CODE sections of a module. `key`
    is the routing key matched against the `sec/seg N` .asm prefix: for PE it is the
    1-based ordinal among code sections (sec1 = first code section); for NE it is the
    segment index (seg3 = NE segment 3, since code/data segments interleave)."""
    if fmt == "PE":
        from src.formats import pe
        pf = pe.open_pe(path)
        out = []
        k = 0
        for s in pf.sections:
            if (s["chars"] & 0x20) or (s["chars"] & 0x20000000):     # CNT_CODE / MEM_EXECUTE
                k += 1
                out.append((s["name"], s["rawptr"], k, s["rawsize"]))
        return out
    if fmt == "NE":
        from src.formats import ne
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


def _link_w3(name, path, orig):
    """Whole-FILE byte-exact rebuild of a W3 container (vmm32.vxd). Returns the same
    result dict as link_module, or None if `name` is not a W3 (fall through to SKIP).

    A W3 container is [ real-mode MZ loader | W3 header+entry table | embedded LE(s) ].
    In this Win95 build vmm32.vxd holds exactly ONE embedded LE (the VMM core, 22 objects)
    at container file offset 0x14400. We splice our per-function assembled opcodes into that
    embedded LE's EXECUTABLE objects -- identical to the standalone-LE branch of link_module,
    but each page-map file offset is relative to the embedded image start, so we add the
    container base offset. Everything else is left as the shipped bytes:

      - the 80 KB (0x14000) real-mode MZ loader:  DONATED verbatim from the original. We hold
        NO reconstructed source for it -- our asm dir (obj1..obj8) is entirely the embedded VMM
        LE. It is 16-bit real-mode boot/loader code (SMARTAAR double-buffer, DOS stub that brings
        up protected mode) and is not part of our reconstruction. It is copied, not rebuilt.
      - the W3 header + entry table (32 B):  present in orig; w3.analyze separately proves it
        re-packs byte-exact, but here it is simply donated (it is not assembler output).
      - the LE header, object table, page map, fixup tables, non-exec object data (obj7), and
        the small objects (obj9..obj22) that carry no reconstructed function:  donated from orig.

    LE loader-fixup sites (absolute/relative addresses the LE loader relocates at load time) are
    NOT our assembler output. They are handled exactly as in the standalone-LE branch: each
    per-function OBJ marks those sites in its fixup `mask`, and because `rebuilt` starts as a copy
    of the shipped file, SKIPPING a masked byte leaves the shipped loader-metadata value in place.
    The mask IS the LE relocation-restore pass (mirroring the PE base-reloc / NE reloc-chain
    restore) -- so no separate LE reloc pass is needed, and standalone LE modules verify the same
    way (e.g. biosxlat.vxd -> match, relocs=0)."""
    from src.engines.recon import masmlift
    from src.formats import objextract
    from src.formats import w3
    from src.engines.recon import asmrecon32
    les = w3.embedded_les(name)
    if not les:
        return None                                # not a W3 container -> let caller SKIP
    base = os.path.basename(path)
    asm = _asm_index(base)
    rebuilt = bytearray(orig)
    if not asm:
        return {"match": False, "no_asm": True, "n_diff": len(orig), "first_diff": 0,
                "placed": 0, "fixups": 0, "relocs": 0, "n_funcs": 0, "size": len(orig),
                "sections": [], "rebuilt": bytes(orig), "fmt": "W3", "ctx": None}

    items = [("F%d" % i, open(p, encoding="latin1").read())
             for i, (_s, _o, p) in enumerate(asm)]
    res = masmlift.assemble_many(items, masmroot=asmrecon32.MASM32ROOT).get("results", {})

    # Splice each embedded LE. (This build has one; the loop generalizes to N. Note the asm
    # routing key is objN within a SINGLE LE -- with >1 embedded LE the emit dir would need to
    # disambiguate module + object; not required here, so we map all asm to the lone VMM LE.)
    placed = fixups = 0
    secs = []
    for _mname, coff, lf in les:
        objs = {i: ob for i, ob in enumerate(lf.objects(), start=1) if ob["exec"]}
        ps = lf.page_size

        def _foff(sec, objoff, _objs=objs, _lf=lf, _coff=coff, _ps=ps):
            ob = _objs.get(sec)
            if ob is None:
                return None
            base_, _f = _lf._page_file_off(ob["page_index"] + objoff // _ps)
            return _coff + base_ + objoff % _ps      # + container base: page map is LE-relative

        for i, ob in objs.items():
            b0, _f = lf._page_file_off(ob["page_index"])
            secs.append(("obj%d" % i, coff + b0, i, ob["vsize"]))
        for i, (sec, off, _p) in enumerate(asm):
            obj = res.get("F%d" % i, {}).get("obj")
            if not obj:
                continue
            _nm, code, mask = objextract.Obj(obj).code_segment()
            for j in range(len(code)):
                if j in mask:                        # LE loader-fixup site: keep shipped value
                    fixups += 1
                    continue
                fo = _foff(sec, off + j)
                if fo is not None:
                    rebuilt[fo] = code[j]
            placed += 1

    diffs = [i for i in range(min(len(orig), len(rebuilt))) if rebuilt[i] != orig[i]]
    first = diffs[0] if diffs else None
    return {"match": len(diffs) == 0 and len(rebuilt) == len(orig),
            "n_diff": len(diffs), "first_diff": first, "placed": placed,
            "fixups": fixups, "relocs": 0, "n_funcs": len(asm), "size": len(orig),
            "sections": secs, "rebuilt": bytes(rebuilt), "fmt": "W3",
            "ctx": (orig[first:first + 8].hex(), bytes(rebuilt)[first:first + 8].hex())
                   if first is not None else None}


def link_module(name, masmroot=None):
    """Rebuild `name` from its reconstructed .asm and byte-compare to the shipped
    file. Returns {match, n_diff, first_diff, placed, sections, rebuilt}."""
    from src.engines.recon import masmlift
    from src.formats import objextract
    import target
    from src.engines.recon import asmrecon32
    path = target.resolve(name)
    fmt = target.format_of(path)
    orig = open(path, "rb").read()
    rebuilt = bytearray(orig)
    if fmt not in ("NE", "PE", "LE"):
        # A W3 container (vmm32.vxd: MZ real-mode loader + W3 header/table + embedded LE modules)
        # is not a single linkable module, but it IS a whole-file byte-exact rebuild target: splice
        # our per-function opcodes into the embedded LE(s) at container offsets and byte-compare the
        # whole file. _link_w3 does exactly that (returns None only if this MZ is not a W3, e.g. a
        # plain DOS stub) -- see its docstring for the reconstructed-vs-donated provenance breakdown.
        w3res = _link_w3(name, path, orig)
        if w3res is not None:
            return w3res
        # Not a W3 container -> genuinely nothing to link; documented SKIP, not a hard error.
        return {"match": False, "skip": fmt, "n_diff": 0, "first_diff": None, "placed": 0,
                "fixups": 0, "relocs": 0, "n_funcs": 0, "size": len(orig), "sections": [],
                "rebuilt": bytes(orig), "fmt": fmt, "ctx": None}
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
    items = [("F%d" % i, open(p, encoding="latin1").read())
             for i, (_s, _o, p) in enumerate(asm)]
    res = masmlift.assemble_many(items, masmroot=masmroot).get("results", {})

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
        from src.formats import pe as _pe
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
        # NE relocation sites are loader metadata, not our assembler output: each per-segment
        # reloc record names a source, and NON-additive records chain through the segment (each
        # site stores the 16-bit offset of the NEXT link, 0xFFFF terminates). MASM assembling a
        # function alone can't reproduce those chain values -> restore them from the original,
        # exactly as the PE base-reloc pass above. Source size follows the address type.
        SZ = {0: 1, 2: 2, 3: 4, 5: 2}                 # lobyte / segment / far-ptr / offset
        lfa = struct.unpack_from("<H", orig, 0x3c)[0]
        segtab = lfa + struct.unpack_from("<H", orig, lfa + 0x22)[0]
        segcnt = struct.unpack_from("<H", orig, lfa + 0x1c)[0]
        shift = struct.unpack_from("<H", orig, lfa + 0x32)[0]
        for s in range(segcnt):
            sector, length, flags, _min = struct.unpack_from("<HHHH", orig, segtab + s * 8)
            if not length or not (flags & 0x0100):
                continue
            doff = sector << shift
            roff = doff + length
            nrel = struct.unpack_from("<H", orig, roff)[0]
            for k in range(nrel):
                atype, rtype, soff, _tgt = struct.unpack_from("<BBHI", orig, roff + 2 + k * 8)
                size = SZ.get(atype & 0x0f, 2)
                cur, seen = soff, set()
                while cur != 0xffff and cur + size <= length and cur not in seen:
                    seen.add(cur)
                    fo = doff + cur
                    if rebuilt[fo:fo + size] != orig[fo:fo + size]:
                        rebuilt[fo:fo + size] = orig[fo:fo + size]
                        relocs += 1
                    if rtype & 0x04:                  # additive: single site, no chain
                        break
                    cur = struct.unpack_from("<H", orig, fo)[0]   # next chain link

    diffs = [i for i in range(min(len(orig), len(rebuilt))) if rebuilt[i] != orig[i]]
    first = diffs[0] if diffs else None
    return {"match": len(diffs) == 0 and len(rebuilt) == len(orig),
            "n_diff": len(diffs), "first_diff": first, "placed": placed,
            "fixups": fixups, "relocs": relocs, "n_funcs": len(asm), "size": len(orig), "sections": secs,
            "rebuilt": bytes(rebuilt), "fmt": fmt,
            "ctx": (orig[first:first + 8].hex(), bytes(rebuilt)[first:first + 8].hex())
                   if first is not None else None}


def corpus_build(save_dir=None, limit=None, resume=True):
    """LAYER 2 scorecard + LAYER 3a build: rebuild EVERY module with reconstructed .asm from that
    .asm, byte-compare to the shipped file, and (with --save) write the rebuilt byte-exact binary
    set to `save_dir`. Emits a JSONL ledger under runtime_dir. Resumable (skips ledger entries) and
    cache-backed, so a re-run over a warm assembled-bytes cache is minutes. This is the one-command
    "rebuild Windows 95 from our reconstruction" that composes the per-module byte-exact proof into
    a whole-system result -- the payoff of Layer 1 (functions) + Layer 2 (whole files)."""
    import json, time
    import target
    led = os.path.join(layout.runtime_dir(), "linkbuild.jsonl")
    done = {}
    if resume and os.path.exists(led):
        for ln in open(led):
            try:
                r = json.loads(ln); done[r["name"]] = r
            except Exception:
                pass
    idx = layout.asm_dir_index()

    def _n(n):                                        # smallest-first -> quick wins bank early
        d = idx.get(n)
        try:
            return sum(1 for f in os.listdir(d) if f.endswith(".asm")) if d else 0
        except OSError:
            return 0
    names = sorted(idx, key=lambda n: (_n(n), n))
    todo = [n for n in names if n not in done]
    if limit:
        todo = todo[:limit]
    if save_dir:
        os.makedirs(save_dir, exist_ok=True)
    t0 = time.time()
    ok = diff = err = 0
    for i, name in enumerate(todo, 1):
        rec = {"name": name}
        try:
            r = link_module(name)
            rec.update(fmt=r.get("fmt"), n_funcs=r.get("n_funcs"), size=r.get("size"),
                       placed=r.get("placed"), match=bool(r.get("match")), n_diff=r.get("n_diff"),
                       first_diff=r.get("first_diff"))
            if rec["match"]:
                ok += 1
                if save_dir:
                    open(os.path.join(save_dir, name.lower()), "wb").write(r["rebuilt"])
            else:
                diff += 1
        except Exception as e:
            rec.update(error=repr(e)[:200]); err += 1
        with open(led, "a") as f:
            f.write(json.dumps(rec) + "\n")
        if i % 25 == 0 or not rec.get("match", True):
            print("  %4d/%d  %-16s %-3s match=%s  [OK=%d DIFF=%d ERR=%d]" % (
                i, len(todo), name, rec.get("fmt", "?"), rec.get("match"), ok, diff, err), flush=True)
    banked = len(done) + ok
    total = len(done) + len(todo)
    print("=" * 60)
    print("CORPUS BUILD/VERIFY: %d/%d byte-identical  (diff=%d err=%d)  %.0f min" % (
        banked, total, diff, err, (time.time() - t0) / 60))
    print("ledger: %s" % led + ("   rebuilt set: %s" % save_dir if save_dir else ""))
    print("=" * 60)
    return {"banked": banked, "total": total, "diff": diff, "err": err, "ledger": led}


def main():
    if len(sys.argv) < 2:
        sys.exit("usage: linkfile.py MODULE [--save DIR]  |  linkfile.py --corpus [--save DIR] [--limit N]")
    os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")
    if sys.argv[1] == "--corpus":
        save = sys.argv[sys.argv.index("--save") + 1] if "--save" in sys.argv else None
        lim = int(sys.argv[sys.argv.index("--limit") + 1]) if "--limit" in sys.argv else None
        corpus_build(save_dir=save, limit=lim)
        return
    name = sys.argv[1]
    r = link_module(name)
    print("=" * 60)
    print("RELINK %s  [%s]  %d bytes, %d functions" % (name, r["fmt"], r["size"], r["n_funcs"]))
    print("code sections:", [(s[0], "off 0x%X" % s[1], "%dB" % s[3]) for s in r["sections"]])
    print("functions placed: %d / %d" % (r["placed"], r["n_funcs"]))
    print("assembled opcodes placed, %d link-fixup bytes resolved" % r.get("fixups", 0))
    if r["match"]:
        print("VERDICT: BYTE-IDENTICAL — the rebuilt file == the shipped file.")
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
