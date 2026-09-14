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


# --- 16-bit OMF library reader (MSC 6.0a / MSVC 1.5x .LIB) -------------------
# The 32-bit path above reads COFF (._ar_members/_coff_funcs). The 16-bit CRT ships
# as Intel OMF -- a different container entirely -- so NE/DOS lanes need this reader
# to get a real CRT byte-prefix DB (previously they had sigscan shapes only). We
# extract each PUBDEF function's code from its segment's assembled LEDATA and key it
# by a 16-bit-masked prefix (near/far call+jmp displacements zeroed), matching how
# the query side masks a linked 16-bit function.

def _omf_index(b, i):
    """OMF index field: 1 byte if < 0x80, else 2 bytes (big-endian, 15-bit)."""
    v = b[i]
    if v & 0x80:
        return ((v & 0x7f) << 8) | b[i + 1], i + 2
    return v, i + 1


def mask_prefix16(code, sig_len=SIG_LEN):
    """16-bit reloc-masked prefix: zero the displacement bytes of near (E8/E9, rel16)
    and far (9A/EA, seg:off) call/jmp -- the bytes that differ between an unlinked
    .obj and a linked module. Deterministic opcode walk, so the DB and the query
    mask identical positions on identical code. Returns None if too short / too
    masked (needs >= MIN_SIG real bytes)."""
    pref = bytearray(code[:sig_len])
    n = len(pref)
    if n < MIN_SIG:
        return None
    masked = 0
    i = 0
    while i < n:
        op = pref[i]
        if op in (0xE8, 0xE9):                       # near call/jmp rel16
            for k in range(1, 3):
                if i + k < n:
                    pref[i + k] = 0; masked += 1
            i += 3
        elif op in (0x9A, 0xEA):                     # far call/jmp seg:off
            for k in range(1, 5):
                if i + k < n:
                    pref[i + k] = 0; masked += 1
            i += 5
        else:
            i += 1
    if n - masked < MIN_SIG:
        return None
    return bytes(pref)


def _omf_modules(data):
    """Yield each object module blob from an OMF .LIB (or the whole buffer if it is a
    bare .OBJ). Modules are page-aligned; the library ends at an 0xF1 record."""
    if not data or data[0] != 0xF0:
        yield data
        return
    page = struct.unpack_from("<H", data, 1)[0] + 3
    n = len(data)
    p = page
    while p < n:
        if data[p] == 0xF1:                          # library end
            break
        start = p
        while p + 3 <= n:
            t = data[p]
            ln = struct.unpack_from("<H", data, p + 1)[0]
            p += 3 + ln
            if t in (0x8A, 0x8B):                    # MODEND
                break
        yield data[start:p]
        if page and (p % page):                      # next module is page-aligned
            p += page - (p % page)


def _omf_records(mod):
    p, n = 0, len(mod)
    while p + 3 <= n:
        t = mod[p]
        ln = struct.unpack_from("<H", mod, p + 1)[0]
        yield t, mod[p + 3:p + 3 + ln - 1]           # body without the checksum byte
        p += 3 + ln


def _build_omf_module(mod, db):
    lnames = [""]                                     # 1-indexed
    segs = []                                         # [dict(class_idx,length,buf)]
    pubs = []                                         # [(seg_index, name, offset)]
    for t, b in _omf_records(mod):
        if t == 0x96:                                # LNAMES
            i = 0
            while i < len(b):
                l = b[i]; i += 1
                lnames.append(b[i:i + l].decode("latin1", "replace")); i += l
        elif t == 0x98:                              # SEGDEF (16-bit)
            i = 0; acbp = b[i]; i += 1
            if (acbp >> 5) & 7 == 0:                  # absolute segment: frame+offset
                i += 3
            length = struct.unpack_from("<H", b, i)[0]; i += 2
            _segname, i = _omf_index(b, i)
            class_idx, i = _omf_index(b, i)
            if (acbp >> 1) & 1:                       # 'big' -> 64K
                length = 0x10000
            segs.append(dict(class_idx=class_idx, length=length,
                             buf=bytearray(min(length, 0x10000))))
        elif t == 0xA0:                              # LEDATA (16-bit)
            i = 0; seg, i = _omf_index(b, i)
            off = struct.unpack_from("<H", b, i)[0]; i += 2
            chunk = b[i:]
            if 1 <= seg <= len(segs):
                buf = segs[seg - 1]["buf"]
                if off + len(chunk) > len(buf):
                    buf.extend(b"\x00" * (off + len(chunk) - len(buf)))
                buf[off:off + len(chunk)] = chunk
        elif t == 0x90:                              # PUBDEF (16-bit)
            i = 0
            _grp, i = _omf_index(b, i)
            seg, i = _omf_index(b, i)
            if seg == 0:                             # base frame present when seg==0
                i += 2
            while i < len(b):
                l = b[i]; i += 1
                name = b[i:i + l].decode("latin1", "replace"); i += l
                off = struct.unpack_from("<H", b, i)[0]; i += 2
                _typ, i = _omf_index(b, i)
                pubs.append((seg, name, off))
    # emit a signature for each public function in a CODE-class segment
    for seg_idx in sorted(set(s for s, _, _ in pubs)):
        if not (1 <= seg_idx <= len(segs)):
            continue
        ci = segs[seg_idx - 1]["class_idx"]
        cls = lnames[ci] if 0 <= ci < len(lnames) else ""
        if "CODE" not in cls.upper():
            continue
        buf = segs[seg_idx - 1]["buf"]
        items = sorted((o, nm) for s, nm, o in pubs if s == seg_idx)
        for j, (off, name) in enumerate(items):
            end = items[j + 1][0] if j + 1 < len(items) else len(buf)
            sig = mask_prefix16(bytes(buf[off:end]))
            if sig and sig not in db:
                db[sig] = name


def build_omf(lib_path):
    """Return {16-bit-masked signature: name} for an OMF .LIB's public functions."""
    data = open(lib_path, "rb").read()
    db = {}
    for mod in _omf_modules(data):
        try:
            _build_omf_module(mod, db)
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
    here = os.path.dirname(os.path.abspath(__file__))          # .../src/engines/symbol
    cb = os.path.dirname(os.path.dirname(here))                # .../src
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


from src.cli.common import opt as _opt


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
        from src.core import config
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
