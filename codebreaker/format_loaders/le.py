#!/usr/bin/env python3
r"""le.py -- Linear Executable (LE) loader for Win95 Virtual Device Drivers (.vxd / .386).

VxDs are Ring-0 32-bit drivers in the LE format (not the 16-bit NE of the system DLLs), so
they need their own loader. This parses the LE header, the object table, and extracts each
object's bytes from the data pages, and it locates + decodes the DDB (Device Descriptor Block)
-- the VxD's identity: its 8-char device name, DDK version, device id, and control procedure.
That identity is the anchor for reconstructing the driver as clean MASM (its VxD_Desc_Block,
Begin_Control_Dispatch, and service table).

    from codebreaker.format_loaders import le
    v = le.LEFile.from_file("vmouse.vxd")
    print(v.ddb())                       # {'name': 'VMD', 'version': 0x40a, 'device_id': ...}
    code = v.object_bytes(v.objects[0])  # 32-bit code of the first object
"""
import struct


class LEFile:
    def __init__(s, data, lfa):
        s.d = data; s.lfa = lfa
        u = lambda o: struct.unpack_from("<I", data, lfa + o)[0]
        h = lambda o: struct.unpack_from("<H", data, lfa + o)[0]
        s.sig = data[lfa:lfa + 2]
        s.cpu = h(0x08); s.ostype = h(0x0a)
        s.mpages = u(0x14); s.eip_obj = u(0x18); s.eip = u(0x1c)
        s.pagesize = u(0x28); s.last_page_bytes = u(0x2c)
        s.objtab = lfa + u(0x40); s.nobj = u(0x44); s.pagemap = lfa + u(0x48)
        s.resnames = lfa + u(0x58); s.entrytab = lfa + u(0x5c)
        s.fixup_page = lfa + u(0x68); s.fixup_rec = lfa + u(0x6c)
        s.datapages = u(0x80)                          # offset from top of file (NOT 0x7c)
        s.objects = []
        for i in range(s.nobj):
            vsize, base, flags, pmidx, pcnt = struct.unpack_from("<IIIII", data, s.objtab + i * 24)
            s.objects.append({"idx": i + 1, "vsize": vsize, "base": base, "flags": flags,
                              "pmidx": pmidx, "pcnt": pcnt,
                              "exec": bool(flags & 0x04), "read": bool(flags & 0x01),
                              "write": bool(flags & 0x02)})

    @classmethod
    def from_buffer(cls, data):
        lfa = struct.unpack_from("<I", data, 0x3c)[0]
        if data[lfa:lfa + 2] not in (b"LE", b"LX"):
            raise ValueError("not an LE/LX image (sig=%r)" % data[lfa:lfa + 2])
        return cls(data, lfa)

    @classmethod
    def from_file(cls, path):
        return cls.from_buffer(open(path, "rb").read())

    def object_bytes(s, obj):
        """Raw bytes of one object, assembled from its data pages (contiguous in practice for
        VxDs), truncated to the object's virtual size."""
        off = s.datapages + (obj["pmidx"] - 1) * s.pagesize
        return s.d[off:off + obj["vsize"]]

    def page_to_obj(s, pg):
        """Logical page number (1-based) -> (object, byte offset of the page within the object)."""
        for obj in s.objects:
            if obj["pmidx"] <= pg < obj["pmidx"] + obj["pcnt"]:
                return obj, (pg - obj["pmidx"]) * s.pagesize
        return None, 0

    def fixups(s):
        """Parse the LE fixup page + record tables into {(obj_idx, off_in_obj): fixup}. Each
        fixup is {'kind':'internal', 'obj', 'target'} (an intra-image code/data reference -> a
        label) or {'kind':'import', 'module', 'ordinal'} (a VMM/VxD service imported by
        module:ordinal -> a named call). This is what turns raw operands into symbols."""
        d = s.d; out = {}
        pageoffs = [struct.unpack_from("<I", d, s.fixup_page + i * 4)[0] for i in range(s.mpages + 1)]
        for pg in range(s.mpages):                          # fixups for logical page pg+1
            obj, pbase = s.page_to_obj(pg + 1)
            p = s.fixup_rec + pageoffs[pg]; end = s.fixup_rec + pageoffs[pg + 1]
            while p < end:
                src = d[p]; tflags = d[p + 1]; p += 2
                srclist = bool(src & 0x20)                  # SRC bit5: multiple sources, ONE target
                if srclist:
                    cnt = d[p]; p += 1                      # source count (offset list is at record END)
                else:
                    soff = struct.unpack_from("<h", d, p)[0]; p += 2
                tt = tflags & 0x03
                fx = None
                if tt == 0x00:                              # internal reference
                    objn = struct.unpack_from("<H", d, p)[0] if (tflags & 0x40) else d[p]
                    p += 2 if (tflags & 0x40) else 1
                    if (src & 0x0f) == 0x02:                # 16-bit selector, no offset
                        tv = None
                    elif tflags & 0x10:
                        tv = struct.unpack_from("<I", d, p)[0]; p += 4
                    else:
                        tv = struct.unpack_from("<H", d, p)[0]; p += 2
                    fx = {"kind": "internal", "obj": objn, "target": tv}
                elif tt == 0x01:                            # import by ordinal (VMM/VxD service)
                    mod = struct.unpack_from("<H", d, p)[0] if (tflags & 0x40) else d[p]
                    p += 2 if (tflags & 0x40) else 1
                    if tflags & 0x80:
                        ordv = d[p]; p += 1
                    elif tflags & 0x10:
                        ordv = struct.unpack_from("<I", d, p)[0]; p += 4
                    else:
                        ordv = struct.unpack_from("<H", d, p)[0]; p += 2
                    fx = {"kind": "import", "module": mod, "ordinal": ordv}
                else:                                       # import-by-name / entry -- stop this page
                    break
                if tflags & 0x04:                           # additive value follows the target
                    p += 4 if (tflags & 0x20) else 2
                if srclist:                                 # the SRC offset list lives at the record END
                    offs = [struct.unpack_from("<h", d, p + j * 2)[0] for j in range(cnt)]; p += cnt * 2
                else:
                    offs = [soff]
                if obj is not None:
                    for o in offs:
                        out[(obj["idx"], pbase + o)] = fx
        return out

    def import_modules(s):
        """The imported module names (VMM, VPICD, ...) from the Imported Modules Name Table."""
        import struct as _s
        off = s.lfa + _s.unpack_from("<I", s.d, s.lfa + 0x70)[0]
        cnt = _s.unpack_from("<I", s.d, s.lfa + 0x74)[0]
        names = []; p = off
        for _ in range(cnt):
            n = s.d[p]; names.append(s.d[p + 1:p + 1 + n].decode("latin1", "replace")); p += 1 + n
        return names

    def resident_name(s):
        """The module's own name (first entry of the resident-name table, length-prefixed)."""
        n = s.d[s.resnames]
        return s.d[s.resnames + 1:s.resnames + 1 + n].decode("latin1", "replace")

    def ddb(s):
        """Locate and decode the Device Descriptor Block. Scan each object's bytes for the DDB
        signature: a dword-aligned DDK version (0x03xx/0x04xx) followed by device id, a
        major/minor byte pair, flags, and an 8-byte device name of printable ASCII. Returns the
        decoded DDB or None. The name is what `Declare_Virtual_Device <name>` emits."""
        def decode(obj, b, off):
            name = b[off + 8:off + 16]
            if not name or not all(32 <= c < 127 for c in name):
                return None
            if not (name[0:1].isalpha() or name[0:1].isdigit()):
                return None
            return {"obj": obj["idx"], "rva": off - 4,
                    "version": b[off] | (b[off + 1] << 8),
                    "device_id": b[off + 2] | (b[off + 3] << 8),
                    "major": b[off + 4], "minor": b[off + 5],
                    "flags": b[off + 6] | (b[off + 7] << 8),
                    "name": name.rstrip(b" \x00").decode("latin1", "replace"),
                    "init_order": struct.unpack_from("<I", b, off + 16)[0],
                    "control_proc": struct.unpack_from("<I", b, off + 20)[0]}
        for obj in s.objects:                                  # strict DDK version stamps -- the common case
            b = s.object_bytes(obj)
            for off in range(0, len(b) - 26, 4):
                if (b[off] | (b[off + 1] << 8)) not in (0x030a, 0x0400, 0x040a):
                    continue
                d = decode(obj, b, off)
                if d:
                    return d
        # Fallback: some VxDs stamp a DDK version outside the common set (0x0300 DSVXD,
        # 0x030b DECCORE/VNetSup). Accept any 0x03xx/0x04xx stamp, but require the candidate's
        # device id to equal the one in the LE header (+0xC0) -- this both broadens the version
        # match and rejects a stray version-looking dword (deccore has a 0x03d0/name-like blob
        # whose device id does not match). Runs only when the strict scan found nothing.
        try:
            hdr_dev = struct.unpack_from("<H", s.d, s.lfa + 0xC0)[0]
        except Exception:
            hdr_dev = 0
        if hdr_dev:
            for obj in s.objects:
                b = s.object_bytes(obj)
                for off in range(0, len(b) - 26, 4):
                    ver = b[off] | (b[off + 1] << 8)
                    if (ver >> 8) not in (3, 4):
                        continue
                    if (b[off + 2] | (b[off + 3] << 8)) != hdr_dev:
                        continue
                    d = decode(obj, b, off)
                    if d:
                        return d
        return None


class W3File:
    r"""W3 container (e.g. vmm32.vxd): a bundle of concatenated LE images. The header is 'W3',
    a version word, a module count dword, then `count` 16-byte entries (8-char name + dword file
    offset + dword size) starting at +0x10. Win95 builds vmm32.vxd by packing VMM (and, on some
    machines, the statically-loaded VxDs) into one W3 so the loader maps them together."""
    def __init__(s, data, lfa):
        s.d = data; s.lfa = lfa
        s.version = struct.unpack_from("<H", data, lfa + 2)[0]
        s.count = struct.unpack_from("<I", data, lfa + 4)[0]
        s.entries = []
        p = lfa + 0x10
        for _ in range(s.count):
            name = data[p:p + 8].rstrip(b" \x00").decode("latin1", "replace")
            off, size = struct.unpack_from("<II", data, p + 8)
            s.entries.append({"name": name, "off": off, "size": size})
            p += 16

    def modules(s):
        """[(name, LEFile), ...] for each embedded LE image (offsets are the module's LE header)."""
        return [(e["name"], LEFile(s.d, e["off"])) for e in s.entries
                if s.d[e["off"]:e["off"] + 2] in (b"LE", b"LX")]

    @classmethod
    def from_file(cls, path):
        d = open(path, "rb").read()
        return cls(d, struct.unpack_from("<I", d, 0x3c)[0])


def open_vxds(path):
    """[(name, LEFile), ...] for any VxD file: one entry for a plain LE/LX, all embedded modules
    for a W3 container. The single entry point for feeding the disassembler."""
    d = open(path, "rb").read()
    lfa = struct.unpack_from("<I", d, 0x3c)[0]
    sig = d[lfa:lfa + 2]
    if sig == b"W3":
        return W3File(d, lfa).modules()
    if sig in (b"LE", b"LX"):
        import os as _os
        return [(_os.path.basename(path), LEFile(d, lfa))]
    raise ValueError("not an LE/LX/W3 image (sig=%r)" % sig)


if __name__ == "__main__":
    import sys, glob, os
    args = sys.argv[1:]
    if args and os.path.isdir(args[0]):
        args = sorted(glob.glob(os.path.join(args[0], "*.vxd")) + glob.glob(os.path.join(args[0], "*.386")))
    for p in args:
        try:
            v = LEFile.from_file(p)
            db = v.ddb()
            objs = "+".join("%d%s" % (o["vsize"], "x" if o["exec"] else "d") for o in v.objects)
            print("%-16s %-9s ver=0x%03x id=0x%04x v%d.%d objs=[%s]" % (
                os.path.basename(p), db["name"] if db else "?", db["version"] if db else 0,
                db["device_id"] if db else 0, db["major"] if db else 0, db["minor"] if db else 0, objs))
        except Exception as e:
            print("%-16s ERROR %s" % (os.path.basename(p), str(e)[:50]))
