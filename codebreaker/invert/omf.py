"""omf.py - Intel OMF (.OBJ / .LIB) parser for the Lattice C RE project.

Usage:
  python omf.py <file.obj>            # summarize records of one object module
  python omf.py <file.obj> --raw      # list every record (type,len)
  python omf.py <file.lib> --lib      # walk a library: per-module THEADR + PUBDEFs
  python omf.py <file.lib> --pubs     # just dump every public symbol -> module

Handles the classic 16-bit OMF record set. Library format = concatenated OBJ
modules, each padded to a "page size" boundary, terminated by a LIBEND (0xF1),
with a dictionary at the end (which we ignore; we walk modules linearly).
"""
import sys, struct

REC = {
    0x80:"THEADR", 0x82:"LHEADR", 0x88:"COMENT", 0x8A:"MODEND", 0x8B:"MODEND",
    0x8C:"EXTDEF", 0x8E:"TYPDEF", 0x90:"PUBDEF", 0x91:"PUBDEF", 0x94:"LINNUM",
    0x95:"LINNUM", 0x96:"LNAMES", 0x98:"SEGDEF", 0x99:"SEGDEF", 0x9A:"GRPDEF",
    0x9C:"FIXUPP", 0x9D:"FIXUPP", 0xA0:"LEDATA", 0xA1:"LEDATA", 0xA2:"LIDATA",
    0xA3:"LIDATA", 0xB0:"COMDEF", 0xF0:"LIBHDR", 0xF1:"LIBEND",
}

def rd_str(buf, i):
    n = buf[i]; return buf[i+1:i+1+n].decode('latin1'), i+1+n

def rd_index(buf, i):
    # OMF index: 1 byte if <0x80, else 2 bytes (high bit set on first)
    b = buf[i]
    if b & 0x80:
        return ((b & 0x7f) << 8) | buf[i+1], i+2
    return b, i+1

def records(data, start=0):
    """Yield (offset, rectype, body_bytes). body excludes type/len/checksum."""
    i = start
    n = len(data)
    while i < n:
        rectype = data[i]
        if rectype == 0:  # padding
            i += 1; continue
        rlen = struct.unpack_from("<H", data, i+1)[0]
        body = data[i+3:i+3+rlen-1]   # last byte of rlen region is checksum
        yield i, rectype, body
        i = i + 3 + rlen
        if rectype in (0xF1,):  # LIBEND
            break

def parse_module(data, start=0, lnames=None, segnames=None, verbose=True):
    """Parse one module starting at `start`; return next offset (after MODEND)."""
    lnames = lnames if lnames is not None else [""]  # index 1-based
    segs = segnames if segnames is not None else []
    extdefs = []
    pubdefs = []
    theadr = None
    i = start
    for off, rt, body in records(data, start):
        name = REC.get(rt, f"?{rt:02X}")
        if rt == 0x80:  # THEADR
            theadr, _ = rd_str(body, 0)
            if verbose: print(f"  @{off:06x} THEADR  '{theadr}'")
        elif rt == 0x96:  # LNAMES
            j = 0
            while j < len(body):
                s, j = rd_str(body, j); lnames.append(s)
            if verbose: print(f"  @{off:06x} LNAMES  {lnames[1:]}")
        elif rt in (0x98,0x99):  # SEGDEF
            attr = body[0]; j = 1
            # if alignment field (ACBP) high bits==0 there may be extra; keep simple
            seglen = struct.unpack_from("<H", body, j)[0]; j += 2
            segname_idx, j = rd_index(body, j)
            classname_idx, j = rd_index(body, j)
            overlay_idx, j = rd_index(body, j)
            sname = lnames[segname_idx] if segname_idx < len(lnames) else f"#{segname_idx}"
            cname = lnames[classname_idx] if classname_idx < len(lnames) else f"#{classname_idx}"
            segs.append(sname)
            if verbose: print(f"  @{off:06x} SEGDEF  {sname} class={cname} len={seglen} acbp={attr:02x}")
        elif rt == 0x9A:  # GRPDEF
            gidx, j = rd_index(body, 0)
            gname = lnames[gidx] if gidx < len(lnames) else f"#{gidx}"
            members = []
            while j < len(body):
                typ = body[j]; j += 1
                si, j = rd_index(body, j)
                members.append(segs[si-1] if 0 < si <= len(segs) else f"seg#{si}")
            if verbose: print(f"  @{off:06x} GRPDEF  {gname} = {members}")
        elif rt == 0x8C:  # EXTDEF
            j = 0
            while j < len(body):
                s, j = rd_str(body, j)
                ti, j = rd_index(body, j)
                extdefs.append(s)
            if verbose: print(f"  @{off:06x} EXTDEF  {extdefs[-5:]}{' ...' if len(extdefs)>5 else ''}")
        elif rt in (0x90,0x91):  # PUBDEF
            j = 0
            grp, j = rd_index(body, j)
            seg, j = rd_index(body, j)
            if seg == 0:
                j += 2  # base frame word when seg index 0
            this_rec = []
            while j < len(body):
                s, j = rd_str(body, j)
                poff = struct.unpack_from("<H", body, j)[0]; j += 2
                ti, j = rd_index(body, j)
                pubdefs.append((s, poff)); this_rec.append((s, poff))
            if verbose:
                print(f"  @{off:06x} PUBDEF  " + ", ".join(f"{s}@{po:#x}" for s,po in this_rec))
        elif rt == 0x88:  # COMENT
            ctype = body[0]; cclass = body[1] if len(body)>1 else 0
            txt = body[2:].decode('latin1','replace')
            if verbose: print(f"  @{off:06x} COMENT  class={cclass:02x} {txt!r}")
        elif rt in (0xA0,0xA1):  # LEDATA
            seg, j = rd_index(body, 0)
            dataoff = struct.unpack_from("<H", body, j)[0]; j += 2
            payload = body[j:]
            sname = segs[seg-1] if 0 < seg <= len(segs) else f"seg#{seg}"
            if verbose: print(f"  @{off:06x} LEDATA  {sname}+{dataoff:#x} {len(payload)} bytes")
        elif rt in (0x9C,0x9D):  # FIXUPP
            if verbose: print(f"  @{off:06x} FIXUPP  {len(body)} bytes")
        elif rt in (0x8A,0x8B):  # MODEND
            if verbose: print(f"  @{off:06x} MODEND")
            return off + 3 + len(body) + 1, lnames, segs, extdefs, pubdefs
        else:
            if verbose: print(f"  @{off:06x} {name} ({len(body)} bytes)")
    return len(data), lnames, segs, extdefs, pubdefs

def cmd_obj(path, raw=False):
    data = open(path,"rb").read()
    if raw:
        for off, rt, body in records(data):
            print(f"@{off:06x} {REC.get(rt,f'?{rt:02X}'):8} len={len(body)+1}")
        return
    print(f"== {path} ({len(data)} bytes) ==")
    parse_module(data, 0)

def cmd_lib(path, pubs_only=False):
    data = open(path,"rb").read()
    # LIBHDR (0xF0): record len defines page/block size for module alignment
    assert data[0] == 0xF0, "not a library (expected 0xF0)"
    pagelen = struct.unpack_from("<H", data, 1)[0] + 3
    print(f"== library {path} ({len(data)} bytes) page/block size {pagelen} ==")
    i = pagelen  # first module starts after the header block
    nmod = 0; npub = 0
    while i < len(data):
        if data[i] == 0xF1:  # LIBEND / dictionary
            print(f"-- LIBEND @ {i:#x} ({nmod} modules, {npub} publics) --"); break
        if data[i] != 0x80:  # not a THEADR -> alignment padding
            i += 1; continue
        nxt, ln, sg, ex, pb = parse_module(data, i, verbose=not pubs_only)
        theadr = None
        # recover theadr quickly
        for off, rt, body in records(data, i):
            if rt == 0x80:
                theadr, _ = rd_str(body, 0); break
            break
        if pubs_only:
            for s,po in pb:
                print(f"{s}\t{theadr}")
        nmod += 1; npub += len(pb)
        # align next module up to page boundary
        i = ((nxt + pagelen - 1)//pagelen)*pagelen
    print(f"total: {nmod} modules, {npub} publics")

# what the linker fills in, by the kind of place a fix-up names: bytes it covers
_WIDTH = {0: 1, 1: 2, 2: 2, 3: 4, 4: 1, 5: 2, 9: 4, 11: 6, 13: 4}


def functions(data):
    """{public name: (bytes, {offsets the linker fills in})} of an object file's code segment,
    cut between consecutive public names. A name keeps the compiler's leading underscore."""
    lnames, segs, seg_bytes, seg_mask, pubs, last = [""], [], {}, {}, {}, None
    for _at, kind, body in records(data):
        wide = kind & 1
        if kind == 0x96:                                     # LNAMES
            p = 0
            while p < len(body):
                name, p = rd_str(body, p)
                lnames.append(name)
        elif kind in (0x98, 0x99):                           # SEGDEF
            p = 1 + (3 if body[0] >> 5 == 0 else 0) + (4 if wide else 2)
            name_i, p = rd_index(body, p)
            class_i, p = rd_index(body, p)
            name = lnames[name_i] if name_i < len(lnames) else "?%d" % name_i
            segs.append((name, lnames[class_i] if class_i < len(lnames) else ""))
            seg_bytes.setdefault(name, bytearray())
            seg_mask.setdefault(name, set())
        elif kind in (0xA0, 0xA1):                           # LEDATA
            seg_i, p = rd_index(body, 0)
            off = int.from_bytes(body[p:p + (4 if wide else 2)], "little")
            p += 4 if wide else 2
            chunk = bytes(body[p:])
            name = segs[seg_i - 1][0] if 0 < seg_i <= len(segs) else "_TEXT"
            buf = seg_bytes.setdefault(name, bytearray())
            if len(buf) < off + len(chunk):
                buf.extend(bytes(off + len(chunk) - len(buf)))
            buf[off:off + len(chunk)] = chunk
            last = (name, off)
        elif kind in (0x9C, 0x9D) and last:                  # FIXUPP: the places of the last data record
            name, base = last
            mask, p = seg_mask.setdefault(name, set()), 0
            while p < len(body):
                first = body[p]
                if not first & 0x80:                         # a THREAD: no place of its own
                    _i, p = rd_index(body, p + 1)
                    continue
                locat = (first << 8) | body[p + 1]
                for k in range(_WIDTH.get((locat >> 10) & 0x0F, 2)):
                    mask.add(base + (locat & 0x3FF) + k)
                fixdata, p = body[p + 2], p + 3
                if not fixdata & 0x80 and (fixdata >> 4) & 7 in (0, 1, 2):
                    _i, p = rd_index(body, p)
                if not fixdata & 0x08:
                    _i, p = rd_index(body, p)
                if not fixdata & 0x04:
                    p += 4 if wide else 2
        elif kind in (0x90, 0x91):                           # PUBDEF
            _grp, p = rd_index(body, 0)
            seg_i, p = rd_index(body, p)
            if seg_i == 0:
                p += 2
            segname = segs[seg_i - 1][0] if 0 < seg_i <= len(segs) else "?"
            while p < len(body):
                name, p = rd_str(body, p)
                off = int.from_bytes(body[p:p + (4 if wide else 2)], "little")
                p += 4 if wide else 2
                _t, p = rd_index(body, p)
                pubs[name] = (segname, off)
    code_seg = next((n for n, c in segs if "CODE" in c.upper() or n.upper().endswith("TEXT")), segs[0][0] if segs else None)
    if code_seg is None:
        return {}
    code, mask = bytes(seg_bytes.get(code_seg, b"")), seg_mask.get(code_seg, set())
    here = sorted((off, nm) for nm, (sg, off) in pubs.items() if sg == code_seg)
    out = {}
    for i, (off, nm) in enumerate(here):
        end = here[i + 1][0] if i + 1 < len(here) else len(code)
        out[nm] = (code[off:end], {m - off for m in mask if off <= m < end})
    return out


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(__doc__); sys.exit(1)
    p = sys.argv[1]; args = sys.argv[2:]
    if "--lib" in args: cmd_lib(p, pubs_only=False)
    elif "--pubs" in args: cmd_lib(p, pubs_only=True)
    else: cmd_obj(p, raw=("--raw" in args))
