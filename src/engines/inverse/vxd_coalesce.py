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
    from src.formats import le
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
    from src.formats import le
    b = bytearray(vxd_bytes)
    m = le.LEFile.from_buffer(bytes(b))
    struct.pack_into("<I", b, m.lfa + 0x18, v.eip_obj)
    struct.pack_into("<I", b, m.lfa + 0x1c, v.eip)
    return bytes(b)


def finalize(vxd_bytes, v):
    """Apply both LE-format fixes (coalesce fixups, restore entry point). Best-effort per step."""
    try:
        vxd_bytes = coalesce(vxd_bytes)
    except Exception:
        pass
    try:
        vxd_bytes = restore_entry_point(vxd_bytes, v)
    except Exception:
        pass
    return vxd_bytes
