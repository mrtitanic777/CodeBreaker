#!/usr/bin/env python3
r"""le.py -- Linear Executable (LE) loader for Win95 Virtual Device Drivers (.vxd / .386).

VxDs are Ring-0 32-bit drivers in the LE format (not the 16-bit NE of the system DLLs), so
they need their own loader. This parses the LE header, the object table, and extracts each
object's bytes from the data pages, and it locates + decodes the DDB (Device Descriptor Block)
-- the VxD's identity: its 8-char device name, DDK version, device id, and control procedure.
That identity is the anchor for reconstructing the driver as clean MASM (its VxD_Desc_Block,
Begin_Control_Dispatch, and service table).

    from engine.format_loaders import le
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

    def resident_name(s):
        """The module's own name (first entry of the resident-name table, length-prefixed)."""
        n = s.d[s.resnames]
        return s.d[s.resnames + 1:s.resnames + 1 + n].decode("latin1", "replace")

    def ddb(s):
        """Locate and decode the Device Descriptor Block. Scan each object's bytes for the DDB
        signature: a dword-aligned DDK version (0x03xx/0x04xx) followed by device id, a
        major/minor byte pair, flags, and an 8-byte device name of printable ASCII. Returns the
        decoded DDB or None. The name is what `Declare_Virtual_Device <name>` emits."""
        for obj in s.objects:
            b = s.object_bytes(obj)
            for off in range(0, len(b) - 26, 4):
                ver = b[off] | (b[off + 1] << 8)
                if ver not in (0x030a, 0x0400, 0x040a, 0x0400):   # DDK version stamps
                    continue
                name = b[off + 8:off + 16]
                if not name or not all(32 <= c < 127 for c in name):
                    continue
                if not (name[0:1].isalpha() or name[0:1].isdigit()):
                    continue
                devid = b[off + 2] | (b[off + 3] << 8)
                init_order = struct.unpack_from("<I", b, off + 16)[0]
                ctrl = struct.unpack_from("<I", b, off + 20)[0]
                return {"obj": obj["idx"], "rva": off - 4, "version": ver, "device_id": devid,
                        "major": b[off + 4], "minor": b[off + 5], "flags": b[off + 6] | (b[off + 7] << 8),
                        "name": name.rstrip(b" \x00").decode("latin1", "replace"),
                        "init_order": init_order, "control_proc": ctrl}
        return None


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
