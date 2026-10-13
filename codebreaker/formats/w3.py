#!/usr/bin/env python3
r"""
w3.py -- reconstruct a W3 container (VMM32.VXD) and characterize its parts.

VMM32.VXD is not the "~100 bundled VxDs" folklore suggests -- in this build it is:
  [ real-mode MZ loader | W3 header + entry table | one big VMM LE module ].
The W3 header + entry table (the container index) reconstruct byte-exact by
re-packing their fields. Each bundled entry points at a standard LE module that the
existing LE pipeline (conduct -> linkfile / leloader / headergen) already handles.

    python w3.py MODULE        # reconstruct the W3 header + break down the container
"""
import os
from codebreaker.base import registry as cores
import struct
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.append(ROOT)
def embedded_les(module):
    """[(name, file_off, LEFile)] for each LE module bundled inside a W3 container
    (VMM32.VXD). The W3 entry table indexes standard LE images by (name, off, size); each
    is parsed via LEFile.from_buffer (no MZ of its own). Returns [] if MODULE isn't a W3.
    This is the reusable extraction the DDB tooling builds on -- e.g.
        for name, off, lf in embedded_les('vmm32.vxd'):
            services = lf.all_ddb_services()      # every static VxD's service table
    (NB: the base VMM's own service table is non-standard -- see le.find_ddb.)"""
    target = cores.get('target')
    le = cores.get('le')
    d = open(target.resolve(module), "rb").read()
    if d[:2] != b"MZ":
        return []
    lfa = struct.unpack_from("<I", d, 0x3C)[0]
    if d[lfa:lfa + 2] != b"W3":
        return []
    count = struct.unpack_from("<I", d, lfa + 4)[0]
    out = []
    for i in range(count):
        rec = d[lfa + 0x10 + i * 16:lfa + 0x10 + i * 16 + 16]
        name = rec[:8].rstrip(b" \0").decode("latin1", "replace")
        off = struct.unpack_from("<I", rec, 8)[0]
        if d[off:off + 2] in (b"LE", b"LX"):
            try:
                out.append((name, off, le.LEFile.from_buffer(d[off:], 0, path=name)))
            except Exception:
                pass
    return out


def analyze(module):
    target = cores.get('target')
    path = target.resolve(module)
    d = open(path, "rb").read()
    if d[:2] != b"MZ":
        return {"skip": True, "why": "no MZ"}
    lfa = struct.unpack_from("<I", d, 0x3C)[0]
    if d[lfa:lfa + 2] != b"W3":
        return {"skip": True, "why": "not W3"}
    count = struct.unpack_from("<I", d, lfa + 4)[0]

    # re-pack the W3 header (sig + version + count + reserved) + entry table
    out = bytearray()
    out += d[lfa:lfa + 2]
    out += struct.pack("<H", struct.unpack_from("<H", d, lfa + 2)[0])
    out += struct.pack("<I", count)
    out += d[lfa + 8:lfa + 0x10]
    entries = []
    for i in range(count):
        rec = d[lfa + 0x10 + i * 16:lfa + 0x10 + i * 16 + 16]
        name = rec[:8].rstrip(b" \0").decode("latin1", "replace")
        off, size = struct.unpack("<II", rec[8:16])
        out += rec[:8] + struct.pack("<II", off, size)
        entries.append({"name": name, "off": off, "size": size})
    region = d[lfa:lfa + 0x10 + count * 16]
    hdr_ok = bytes(out) == region

    # characterize each bundled LE module
    le = cores.get('le')
    mods = []
    for e in entries:
        inner = d[e["off"]:]
        info = {"name": e["name"], "off": e["off"], "size": e["size"]}
        if inner[:2] in (b"LE", b"LX"):
            try:
                lf = le.LEFile.__new__(le.LEFile)
                lf.data = inner
                lf.e_lfanew = 0
                lf._parse()
                objs = lf.objects()
                info["le"] = {"objects": len(objs), "exec": sum(1 for o in objs if o["exec"]),
                              "pages": lf.n_pages,
                              "code_bytes": sum(o["vsize"] for o in objs if o["exec"])}
            except Exception as ex:
                info["le"] = {"error": str(ex)[:60]}
        mods.append(info)
    return {"lfa": lfa, "realmode": lfa, "count": count, "hdr_ok": hdr_ok,
            "hdr_bytes": len(region), "modules": mods, "total": len(d)}


def main():
    if len(sys.argv) < 2:
        sys.exit("usage: w3.py MODULE")
    os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")
    from codebreaker.base import config
    config.apply_config()
    r = analyze(sys.argv[1])
    if r.get("skip"):
        print("%s: %s" % (sys.argv[1], r["why"])); return
    print("W3 CONTAINER %s  (%d bytes)" % (sys.argv[1], r["total"]))
    print("  real-mode MZ loader:  0x%X bytes" % r["realmode"])
    print("  W3 header + %d entr(y/ies):  %d bytes  -> %s"
          % (r["count"], r["hdr_bytes"], "BYTE-IDENTICAL" if r["hdr_ok"] else "DIFF"))
    for m in r["modules"]:
        le = m.get("le", {})
        desc = ("LE: %d objects (%d exec), %d code bytes" % (le["objects"], le["exec"], le["code_bytes"])
                if "objects" in le else le.get("error", "non-LE"))
        print("  bundled module '%s' @0x%X  size 0x%X  -> %s" % (m["name"], m["off"], m["size"], desc))


if __name__ == "__main__":
    main()
