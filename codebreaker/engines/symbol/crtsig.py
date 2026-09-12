#!/usr/bin/env python3
r"""crtsig.py -- identify CRT and compiler-generated helper functions by signature.

The triage principle: memcpy, _allmul, __ftol, _chkstk, _EH_prolog, far-pointer ops, struct
copies, prologue/epilogue helpers -- NOBODY wrote these for Windows 95. They ship in the CRT
that ships with the compiler, or are emitted by the compiler itself. They must be IDENTIFIED
against the period toolchain and associated with it, not reverse-engineered as authored code.

This builds a FLIRT-style signature DB from a period COFF import/static library (e.g. MSVC 2.0's
LLIBCE.LIB) -- for every function its name + a reloc-masked code prefix -- and matches carved
functions against it. A hit names the function AND attributes its origin (CRT/compiler), removing
it from the reconstruction surface. Precision-first: the prefix is masked at relocation sites and
must match exactly.

    wt --target win95 crtsig <LIB.LIB>               # build + summarize the DB
    wt --target win95 crtsig <LIB.LIB> --match <MODULE>
"""
import os
import struct
import sys

SIG_LEN = 32                      # bytes of reloc-masked prefix used as the signature
MIN_SIG = 8                       # need at least this many real (unmasked) leading bytes


# --- minimal COFF archive + object parsing ----------------------------------
def _ar_members(data):
    """Yield (name, blob) for each archive member after the two linker members."""
    if data[:8] != b"!<arch>\n":
        return
    p = 8
    longnames = b""
    idx = 0
    while p + 60 <= len(data):
        hdr = data[p:p + 60]
        name = hdr[:16].rstrip()
        size = int(hdr[48:58].strip() or b"0")
        body = data[p + 60:p + 60 + size]
        p += 60 + size + (size & 1)               # members are 2-byte aligned
        idx += 1
        if idx <= 2:                               # 1st/2nd = symbol tables
            if name == b"//":
                longnames = body
            continue
        if name.startswith(b"/"):                  # long name -> offset into longnames
            try:
                off = int(name[1:])
                end = longnames.find(b"\n", off)
                name = longnames[off:end].rstrip(b"/")
            except ValueError:
                pass
        else:
            name = name.rstrip(b"/")
        yield name.decode("latin1", "replace"), body


def _coff_funcs(obj):
    """From one COFF object: yield (symbol_name, code_bytes, masked_offsets) for functions in
    the .text section."""
    if len(obj) < 20:
        return
    machine, nsec, _ts, symptr, nsym, _opt, _ch = struct.unpack_from("<HHIIIHH", obj, 0)
    if machine not in (0x14c, 0x14d):              # i386
        return
    secs = []
    for i in range(nsec):
        o = 20 + i * 40
        if o + 40 > len(obj):
            return
        nm = obj[o:o + 8].rstrip(b"\x00").decode("latin1", "replace")
        vsize, vaddr, rawsize, rawptr, relptr, _lptr, nrel, _nln, chars = \
            struct.unpack_from("<IIIIIIHHI", obj, o + 8)
        secs.append(dict(name=nm, rawsize=rawsize, rawptr=rawptr, relptr=relptr, nrel=nrel, idx=i + 1))
    text = next((s for s in secs if s["name"] == ".text"), None)
    if not text or not text["rawsize"]:
        return
    code = obj[text["rawptr"]:text["rawptr"] + text["rawsize"]]
    # relocation offsets within .text (10-byte records: vaddr u32, symidx u32, type u16)
    masked = set()
    for i in range(text["nrel"]):
        ro = text["relptr"] + i * 10
        if ro + 10 > len(obj):
            break
        vaddr, _si, _ty = struct.unpack_from("<IIH", obj, ro)
        for b in range(4):                         # mask 4 bytes at each reloc site (disp32/addr32)
            masked.add(vaddr + b)
    # symbol table -> functions located in .text
    strtab_off = symptr + nsym * 18
    def symname(rec):
        if rec[:4] == b"\x00\x00\x00\x00":
            o = struct.unpack_from("<I", rec, 4)[0]
            e = obj.find(b"\x00", strtab_off + o)
            return obj[strtab_off + o:e].decode("latin1", "replace")
        return rec[:8].rstrip(b"\x00").decode("latin1", "replace")
    funcs = []
    i = 0
    while i < nsym:
        rec = obj[symptr + i * 18:symptr + i * 18 + 18]
        if len(rec) < 18:
            break
        value, secnum, _typ, sclass, naux = struct.unpack_from("<IhHBB", rec, 8)
        if secnum == text["idx"] and sclass in (2, 3) and value < len(code):   # EXTERNAL/STATIC in .text
            funcs.append((symname(rec), value))
        i += 1 + naux
    funcs.sort(key=lambda t: t[1])
    for j, (nm, off) in enumerate(funcs):
        end = funcs[j + 1][1] if j + 1 < len(funcs) else len(code)
        fcode = code[off:end]
        fmask = {m - off for m in masked if off <= m < end}
        if fcode:
            yield nm, fcode, fmask


def _sig(code, mask):
    """Reloc-masked prefix signature (first SIG_LEN bytes, masked bytes zeroed) + the real-byte
    count, or None if too short / too masked."""
    pref = bytearray(code[:SIG_LEN])
    real = 0
    for i in range(len(pref)):
        if i in mask:
            pref[i] = 0
        else:
            real += 1
    if len(pref) < MIN_SIG or real < MIN_SIG:
        return None
    return bytes(pref)


def build(lib_path):
    """Return {signature_prefix: name} for the library's functions (name collisions kept first)."""
    data = open(lib_path, "rb").read()
    db = {}
    for _mname, obj in _ar_members(data):
        try:
            for nm, code, mask in _coff_funcs(obj):
                s = _sig(code, mask)
                if s and s not in db:
                    db[s] = nm
        except Exception:
            continue
    return db


def _pe_masked_prefix(data, relocs_rva, fn_rva, fn_off, size):
    """Reloc-masked prefix of a carved PE function: mask base-reloc sites (absolute addrs) AND
    call/jmp rel32 displacement sites (E8/E9 -> next 4 bytes), matching how the lib masks COFF
    relocs. Returns the SIG_LEN-byte masked prefix."""
    pref = bytearray(data[fn_off:fn_off + SIG_LEN])
    n = len(pref)
    mask = set()
    for r in relocs_rva:                              # base relocs inside the prefix window
        o = r - fn_rva
        if 0 <= o < n:
            for b in range(4):
                if o + b < n:
                    mask.add(o + b)
    i = 0
    while i < n - 4:                                   # call/jmp rel32 displacement sites
        if pref[i] in (0xE8, 0xE9):
            for b in range(1, 5):
                mask.add(i + b)
            i += 5
        else:
            i += 1
    for o in mask:
        if o < n:
            pref[o] = 0
    return bytes(pref)


def build_from_pe(module):
    """Build CRT signatures from a SHIPPED CRT DLL's export table (e.g. crtdll.dll) -- these are
    the Win95-EXACT CRT functions, built with the actual Win95 compiler, so their reloc-masked
    prefixes match the statically-linked CRT copies in other modules far better than an external
    MSVC lib of a slightly different version. Returns {signature: name}."""
    here = os.path.dirname(os.path.abspath(__file__))
    cb = os.path.dirname(os.path.dirname(here))
    if os.path.join(cb, "targets", "win95") not in sys.path:
        sys.path.insert(0, os.path.join(cb, "targets", "win95"))
    import pe32util as H
    pe = H.carve(module).pe
    br = getattr(pe, "base_relocs", None)
    try:
        rl = br() if callable(br) else (br or [])
    except Exception:
        rl = []
    relocs = set(x[0] if isinstance(x, (tuple, list)) else x for x in rl)
    db = {}
    for ent in pe.exports():
        rva, name = ent[2], ent[1]
        off = pe._rva_to_off(rva)
        if off is None:
            continue
        sig = _pe_masked_prefix(pe.data, relocs, rva, off, SIG_LEN)
        # require enough real (non-masked) leading bytes
        if sum(1 for b in sig if b) >= MIN_SIG and sig not in db:
            db[sig] = name
    return db


def match_module(db, module):
    """Identify carved functions in `module` that match a CRT/compiler signature. Returns
    {rva: name}."""
    here = os.path.dirname(os.path.abspath(__file__))          # .../codebreaker/engines/symbol
    cb = os.path.dirname(os.path.dirname(here))                # .../codebreaker
    sys.path.insert(0, os.path.join(cb, "targets", "win95"))
    import pe32util as H
    import classify32
    pe = H.carve(module).pe
    br = getattr(pe, "base_relocs", None)
    try:
        rl = br() if callable(br) else (br or [])
    except Exception:
        rl = []
    # base_relocs may be RVAs or (rva,type) tuples -> normalize to a set of RVAs
    relocs = set(x[0] if isinstance(x, (tuple, list)) else x for x in rl)
    out = {}
    for r in classify32.classify_module(module):
        rva, size = r["rva"], r.get("size", 0) or 0
        off = pe._rva_to_off(rva)
        if off is None or size < MIN_SIG:
            continue
        sig = _pe_masked_prefix(pe.data, relocs, rva, off, size)
        if sig in db:
            out[rva] = db[sig]
    return out


from codebreaker.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    lib = argv[0]
    db = build(lib)
    print("=== crtsig DB from %s: %d signatures, %d names ===" %
          (os.path.basename(lib), len(db), len(set(db.values()))))
    match = _opt(argv, "--match")
    if match:
        from codebreaker.core import config
        config.apply_config()
        hits = match_module(db, match)
        print("  %s: %d functions IDENTIFIED as CRT/compiler code (remove from reconstruction):"
              % (match, len(hits)))
        for rva in sorted(hits):
            print("    sub_%05x  %s" % (rva, hits[rva]))
    else:
        helpers = [n for n in sorted(set(db.values())) if n.lstrip("_").lower().startswith(
            ("allmul", "aulldiv", "alldiv", "allrem", "ftol", "chkstk", "eh_", "local_unwind",
             "except_handler", "memcpy", "memset", "strcpy", "strlen", "malloc"))]
        print("  sample CRT/compiler-helper sigs:", ", ".join(helpers[:20]))


if __name__ == "__main__":
    main()
