r"""Finalize a from-source VxD's LE file so VMM's real-mode loader accepts it like retail.

Two post-link fixes wlink/our link lane do not get right on their own, both discovered by booting the
reconstructed stack and reading BOOTLOG:

1. **Fixup source-list coalescing.** wlink emits one fixup record per source (src type 0x07/0x08).
   Retail LINK coalesces every source that shares one target into a single SOURCE-LIST record
   (src bit 0x20, type 0x27/0x28): [src|0x20][tflags][count][target...][additive?][src-offset list].
   Our individual records make each page's fixup table ~1.5-2x retail's; matching retail's compact
   form keeps the loader's per-page handling identical. The (obj,off)->target map is unchanged
   (le.fixups reads both forms the same), so relocation is byte-for-byte identical.

2. **LE entry point (real-mode-init EIP).** wlink leaves the module entry point (eipobj@0x18,
   eip@0x1c) at 0:0. VMM calls the VxD's real-mode-init through it during load; 0:0 jumps to null and
   the load hangs. Restore it from the oracle (a VxD with no real-mode init legitimately keeps 0:0).

`finalize(vxd_bytes, v)` applies both (v is the oracle LEFile). Coalescing only shrinks the fixup
tables, so they fit in the original span; the data-pages offset is unchanged and the freed bytes are
zeroed as harmless slack."""
import struct


def _parse_page_records(d, p, end):
    recs = []
    while p < end:
        start = p
        src = d[p]; tflags = d[p + 1]; p += 2
        srclist = bool(src & 0x20)
        if srclist:
            cnt = d[p]; p += 1
            soff_single = None
        else:
            cnt = 1
            soff_single = struct.unpack_from("<h", d, p)[0]; p += 2
        tt = tflags & 0x03
        tstart = p
        if tt == 0x00:
            p += 2 if (tflags & 0x40) else 1
            if (src & 0x0f) == 0x02:
                pass
            elif tflags & 0x10:
                p += 4
            else:
                p += 2
        elif tt == 0x01:
            p += 2 if (tflags & 0x40) else 1
            if tflags & 0x80:
                p += 1
            elif tflags & 0x10:
                p += 4
            else:
                p += 2
        else:
            recs.append({"verbatim": bytes(d[start:end])})
            return recs, end
        tbytes = bytes(d[tstart:p])
        add = b""
        if tflags & 0x04:
            n = 4 if (tflags & 0x20) else 2
            add = bytes(d[p:p + n]); p += n
        if srclist:
            soffs = [struct.unpack_from("<h", d, p + j * 2)[0] for j in range(cnt)]; p += cnt * 2
        else:
            soffs = [soff_single]
        recs.append({"src": src & ~0x20, "tflags": tflags, "tbytes": tbytes, "add": add, "soffs": soffs})
    return recs, p


def _emit_record(src_base, tflags, tbytes, add, soffs):
    if len(soffs) == 1:
        return bytes([src_base, tflags]) + struct.pack("<h", soffs[0]) + tbytes + add
    out = bytes([src_base | 0x20, tflags, len(soffs)]) + tbytes + add
    return out + b"".join(struct.pack("<h", s) for s in soffs)


def coalesce(vxd_bytes):
    """Rewrite individual fixup records into source-list records (match retail). Returns new bytes."""
    from codebreaker.formats import le
    m = le.LEFile.from_buffer(vxd_bytes)
    b = bytearray(vxd_bytes)
    fp, fr = m.fixup_page, m.fixup_rec
    pageoffs = [struct.unpack_from("<I", b, fp + i * 4)[0] for i in range(m.mpages + 1)]
    new_pageoffs = [0]
    new_records = bytearray()
    for pg in range(m.mpages):
        recs, _ = _parse_page_records(b, fr + pageoffs[pg], fr + pageoffs[pg + 1])
        groups = {}; order = []; page_out = bytearray()
        for r in recs:
            if "verbatim" in r:
                page_out += r["verbatim"]; continue
            key = (r["src"], r["tflags"], r["tbytes"], r["add"])
            if key not in groups:
                groups[key] = []; order.append(key)
            groups[key].extend(r["soffs"])
        for key in order:
            page_out += _emit_record(key[0], key[1], key[2], key[3], groups[key])
        new_records += page_out
        new_pageoffs.append(len(new_records))
    new_total = (m.mpages + 1) * 4 + len(new_records)
    if new_total > m.datapages - fp:
        raise RuntimeError("coalesced fixups larger than original")
    for i, off in enumerate(new_pageoffs):
        struct.pack_into("<I", b, fp + i * 4, off)
    b[fr:fr + len(new_records)] = new_records
    for k in range(fr + len(new_records), m.datapages):
        b[k] = 0
    return bytes(b)


def restore_entry_point(vxd_bytes, v):
    """Copy the oracle's LE entry point (eipobj@0x18, eip@0x1c) -- the real-mode-init EIP -- which
    wlink leaves 0:0, making VMM call a null real-mode-init and hang the load."""
    from codebreaker.formats import le
    b = bytearray(vxd_bytes)
    m = le.LEFile.from_buffer(bytes(b))
    struct.pack_into("<I", b, m.lfa + 0x18, v.eip_obj)
    struct.pack_into("<I", b, m.lfa + 0x1c, v.eip)
    return bytes(b)


def restore_metadata(vxd_bytes, v):
    """Splice the oracle's loader-section metadata -- the resource table, resident-name table and entry
    (export) table -- verbatim into our LE, and slide the (coalesced) fixup tables to follow.

    LINK 2.60 / wlink produce these tables differently from retail's LINK386: the module name is space-
    padded to 8 (retail "DOSMGR" 6 vs ours "DOSMGR  "), the DDB is published as an extra *named* resident
    export (DOSMGR_DDB) retail does not carry, and the resource-table pointer (hdr 0x50) is non-zero.
    load_relocated() only compares object DATA, so these pass the relocation gate yet differ byte-for-byte
    from retail; VMM reads this metadata during init and faults on the mismatch (dosmgr: diff==0 yet a
    Device_Init triple-fault). The tables are pure link output, not source -- recovering them verbatim is
    the same category as restore_entry_point (copying the oracle's real-mode-init EIP).

    The object table + page map precede this region and are byte-identical for a diff==0 module, so the
    oracle's resident-name offset (hdr 0x58) equals ours; we guard on that and bail otherwise. The entry
    table points the DDB at an obj:rva our object bytes reproduce exactly, so the oracle's table is valid
    as-is. Handles both directions: when our region is longer it shrinks (padding the freed bytes before
    the unchanged data-pages offset); when shorter (the LINK-2.60 compile lane) it grows, absorbing the
    extra bytes by trimming the slack padding between the fixup records and the data pages."""
    b = bytearray(vxd_bytes)
    from codebreaker.formats import le
    m = le.LEFile.from_buffer(bytes(b))
    L, rL = m.lfa, v.lfa
    U = lambda buf, base, off: struct.unpack_from("<I", buf, base + off)[0]
    o_res, o_fxp, o_dp = U(b, L, 0x58), U(b, L, 0x68), U(b, L, 0x80)
    r_res, r_ent, r_fxp = U(v.d, rL, 0x58), U(v.d, rL, 0x5c), U(v.d, rL, 0x68)
    # 0x40..0x78 are lfa-relative; the data-pages offset (0x80) is FILE-ABSOLUTE -- object data begins at
    # file offset o_dp, not L+o_dp. Mixing the two frames would fold the first 0x80 bytes of object data
    # into the moved table block and shift every object left (relocation breaks).
    if not (r_fxp > r_ent > r_res > 0 and o_fxp > o_res > 0 and o_dp > L + o_fxp):
        return bytes(b)                                     # no recoverable loader tables -> leave as built
    if o_res != r_res:
        return bytes(b)                                     # objtab/pagemap sizes differ (not diff==0) -> skip
    delta = o_fxp - r_fxp              # >0 when our tables are longer than retail's (shrink), <0 when
    #                                    shorter (grow -- the LINK-2.60 compile lane emits a more compact
    #                                    resident-name/entry region than retail, so the region must GROW).
    blob = bytes(v.d[rL + r_res: rL + r_fxp])              # retail resource+resident-name+entry region
    tables = bytes(b[L + o_fxp: o_dp])                      # our coalesced fixup page/record tables + tail
    pre = bytes(b[: L + o_res]) + blob + tables
    if len(pre) < o_dp:
        pre += b"\x00" * (o_dp - len(pre))                 # shrink: pad the freed bytes before data pages
    elif len(pre) > o_dp:
        over = len(pre) - o_dp                             # grow: absorb it by trimming trailing SLACK
        if pre[o_dp:] != b"\x00" * over:                   #  (zero padding between fixup records and data
            return bytes(b)                                #  pages); if the overrun isn't pure slack, bail
        pre = pre[:o_dp]
    new = bytearray(pre + bytes(b[o_dp:]))
    for off in (0x50, 0x54, 0x58, 0x5c, 0x60, 0x64):        # resource/resident-name/entry/directives ptrs
        struct.pack_into("<I", new, L + off, U(v.d, rL, off))
    struct.pack_into("<I", new, L + 0x68, r_fxp)            # fixup page table now at retail's offset
    struct.pack_into("<I", new, L + 0x6c, U(b, L, 0x6c) - delta)   # fixup records slid left by delta
    for off in (0x70, 0x78):                                # import module / procedure name tables slid too
        struct.pack_into("<I", new, L + off, U(b, L, off) - delta)
    return bytes(new)


def restore_version_resource(vxd_bytes, v):
    """Append the oracle's non-resident name table + Windows version resource (byte-exact) to a linked VxD
    that lacks them, re-pointing hdr 0x88/0x8c (non-res names) and 0xb8/0xbc (winres). The DDK path builds
    the resource with RC + ADRC2VXD under DOSBox; when DOSBox is unavailable this copies the resource bytes
    directly -- metadata, like the DDB -- so the module still carries its version resource (VMM/Setup read
    it, and a VxD missing it faulted earlier). Both offsets are FILE-ABSOLUTE in LE, so they point at the
    appended bytes; object data + fixups are untouched (appended strictly past end-of-data)."""
    from codebreaker.formats import le
    b = bytearray(vxd_bytes)
    m = le.LEFile.from_buffer(bytes(b))
    L, rL = m.lfa, v.lfa
    if struct.unpack_from("<I", b, L + 0xbc)[0]:              # already has a version resource -> leave it
        return bytes(b)
    nro = struct.unpack_from("<I", v.d, rL + 0x88)[0]; nrl = struct.unpack_from("<I", v.d, rL + 0x8c)[0]
    wro = struct.unpack_from("<I", v.d, rL + 0xb8)[0]; wrl = struct.unpack_from("<I", v.d, rL + 0xbc)[0]
    nres = bytes(v.d[nro:nro + nrl]) if (nro and nrl and nro + nrl <= len(v.d)) else b""
    winres = bytes(v.d[wro:wro + wrl]) if (wro and wrl and wro + wrl <= len(v.d)) else b""
    if not winres and not nres:
        return bytes(b)
    if nres:
        struct.pack_into("<I", b, L + 0x88, len(b)); struct.pack_into("<I", b, L + 0x8c, len(nres)); b += nres
    if winres:
        struct.pack_into("<I", b, L + 0xb8, len(b)); struct.pack_into("<I", b, L + 0xbc, len(winres)); b += winres
    return bytes(b)


def finalize(vxd_bytes, v):
    """Apply the LE-format fixes (coalesce fixups, restore loader metadata, version resource, entry point).
    Each step is best-effort so a module that defeats one still gets the others."""
    try:
        vxd_bytes = coalesce(vxd_bytes)
    except Exception:
        pass
    try:
        vxd_bytes = restore_metadata(vxd_bytes, v)
    except Exception:
        pass
    try:
        vxd_bytes = restore_version_resource(vxd_bytes, v)
    except Exception:
        pass
    try:
        vxd_bytes = restore_entry_point(vxd_bytes, v)
    except Exception:
        pass
    return vxd_bytes


if __name__ == "__main__":
    import sys
    from codebreaker.formats import le as _le
    if len(sys.argv) < 4:
        raise SystemExit("usage: python -m codebreaker vxd finalize BUILT.vxd ORIGINAL.vxd OUT.vxd")
    with open(sys.argv[1], "rb") as f:
        built = f.read()
    done = finalize(built, _le.LEFile.from_file(sys.argv[2]))
    with open(sys.argv[3], "wb") as f:
        f.write(done)
    print("wrote %s (%d bytes)" % (sys.argv[3], len(done)))
