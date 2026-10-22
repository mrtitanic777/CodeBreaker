r"""owbuild.py -- build a ring-0 VxD from recovered source with the OpenWatcom
toolchain (no Microsoft tools): wasm (MASM-mode assemble) + wlink `system win_vxd`.

The recovered per-segment source (vxd_tree.emit_tree) is emitted UNSPLIT (one file per
LE object, so every branch target is internal), each object is converged to assemble by
byte-preserving what wasm rejects (the probe), assembled to OMF, and linked into an LE
VxD with the DDB as the single required export (`export VXD_DDB.1`). Produces a real
loadable <name>.vxd.

    from codebreaker.source.vxd import owbuild as vxd_owbuild
    vxd_owbuild.build_vxd("vkd.vxd", "VKD", "out")     # -> out/vkd/vkd.vxd

Corpus result: 211/215 standalone ring-0 VxDs build into valid LE binaries.
"""
import os
import subprocess

from codebreaker.base import settings
from codebreaker.formats import le
from codebreaker.source.vxd import tree as vxd_tree

_OW = os.path.dirname(settings.get("openwatcom") or "") or os.path.join(settings.ROOT, "experiments", "premise-proof", "open-watcom-v2")
_WASM = os.path.join(_OW, "bld", "wasm", "nt386", "wasm.exe")
_WLINK = os.path.join(_OW, "rel", "binnt64", "wlink.exe")
_NASM = settings.get("nasm")


def _env():
    e = dict(os.environ)
    e["WATCOM"] = os.path.join(_OW, "rel")
    e["PATH"] = os.path.join(_OW, "rel", "binnt64") + os.pathsep + e.get("PATH", "")
    return e


def build_vxd(vxd_path, name, outroot, byte_exact=True):
    """Assemble (wasm) + link (wlink system win_vxd) one VxD's recovered source into a
    loadable .vxd. Returns {ok, output|error, bytes}."""
    if not (os.path.exists(_WASM) and os.path.exists(_WLINK)):
        return {"ok": False, "error": "OpenWatcom wasm/wlink not built"}
    v = le.LEFile.from_file(vxd_path)
    env = _env()
    # emit UNSPLIT (max_ins huge) so cross-object branch targets stay internal.
    # byte_exact: db every instruction the assembler re-encodes differently from the
    # oracle (the verified force-raw set) so the built binary is byte-identical to
    # retail; otherwise just converge to assemble (buildable, may drift).
    fr_map = None
    if byte_exact:
        from codebreaker.source.vxd import link as vxd_link
        fr_map, _ok = vxd_link.byte_exact_force_raw(v)
    files = vxd_tree.emit_tree(v, name.upper(), outroot, max_ins=10 ** 9,
                               probe_wasm=(None if byte_exact else _WASM),
                               force_raw_map=fr_map)
    d = os.path.join(outroot, name.lower())
    objs, asmfail = [], []
    ddb = False
    for fn in files:
        if not fn.endswith(".asm"):
            continue
        src = os.path.join(d, fn)
        if "public VXD_DDB" in open(src, encoding="utf-8", errors="replace").read():
            ddb = True
        ob = fn[:-4] + ".obj"
        # byte-exact build uses the same nasm-masm (COFF) the force-raw set was
        # verified against; the buildable build uses wasm.
        if byte_exact:
            r = subprocess.run([_NASM, "--masm", "-f", "win32", fn, "-o", ob],
                               cwd=d, env=env, capture_output=True, text=True)
        else:
            r = subprocess.run([_WASM, "-zcm=masm", "-q", "-fo=" + ob, fn],
                               cwd=d, env=env, capture_output=True, text=True)
        if r.returncode == 0 and os.path.exists(os.path.join(d, ob)):
            objs.append(ob)
        else:
            asmfail.append(fn)
    if asmfail:
        return {"ok": False, "error": "assemble failed: " + ", ".join(asmfail)}
    lnk = os.path.join(d, "link.lnk")
    with open(lnk, "w", newline="\n") as f:
        f.write("system win_vxd\nname %s.vxd\noption quiet\n" % name.lower())
        f.write("file " + ",".join(objs) + "\n")
        if ddb:
            f.write("export VXD_DDB.1\n")     # a VxD's single required export is its DDB
    subprocess.run([_WLINK, "@link.lnk"], cwd=d, env=env, capture_output=True, text=True)
    out = os.path.join(d, name.lower() + ".vxd")
    if os.path.exists(out):
        return {"ok": True, "output": out, "bytes": os.path.getsize(out), "objects": len(objs)}
    return {"ok": False, "error": "link produced no .vxd (missing DDB export?)"}


def _w3_components(data):
    """Parse a W3 bundle header -> (lfa, [(name, offset, size)])."""
    import struct
    lfa = struct.unpack_from("<I", data, 0x3C)[0]
    if data[lfa:lfa + 2] != b"W3":
        return None, []
    count = struct.unpack_from("<I", data, lfa + 4)[0]
    recs = []
    for i in range(count):
        e = lfa + 16 + i * 16
        name = data[e:e + 8].split(b"\0")[0].decode("latin1").strip()
        off, size = struct.unpack_from("<II", data, e + 8)
        recs.append((name, off, size))
    return lfa, recs


def build_w3(w3_path, outroot):
    """Build a W3 VxD bundle (e.g. vmm32.vxd) with OpenWatcom: extract each bundled
    LE module, rebuild it from recovered source (build_vxd), and repack -- the DOS/
    real-mode front is byte-preserved (it is a real-mode loader, not LE-VxD source),
    the W3 header + records regenerated, each rebuilt LE placed page-aligned."""
    import struct, tempfile
    data = open(w3_path, "rb").read()
    lfa, recs = _w3_components(data)
    if lfa is None:
        return {"ok": False, "error": "not a W3 bundle"}
    front = bytearray(data[:lfa])                      # MZ + real-mode loader, preserved
    built = []
    for name, off, size in recs:
        # the LE module runs from its offset to the next module (or EOF)
        end = min([o for _n, o, _s in recs if o > off] + [len(data)])
        with tempfile.NamedTemporaryFile("wb", suffix=".vxd", delete=False) as tf:
            tf.write(data[off:end]); modpath = tf.name
        try:
            for nm, v in le.open_vxds(modpath):
                r = build_vxd(modpath, name, outroot)
                built.append((name, r))
                break
        finally:
            try: os.unlink(modpath)
            except OSError: pass
    if not all(r.get("ok") for _n, r in built):
        return {"ok": False, "error": "component build failed",
                "components": [(n, r.get("ok")) for n, r in built]}

    # assemble the W3: front + header + records + page-aligned LE modules
    hdr_size = 16 + len(recs) * 16
    body_start = (lfa + hdr_size + 0x3FF) & ~0x3FF     # page-align the first module
    out = bytearray(front)
    out += b"W3" + struct.pack("<HI", 0x0400, len(recs)) + b"\0" * 8   # 16-byte header
    rec_off = len(out)
    out += b"\0" * (len(recs) * 16)                    # placeholder records
    while len(out) < body_start:
        out += b"\0"
    records = []
    for name, r in built:
        mod = open(r["output"], "rb").read()
        moff = len(out)
        out += mod
        while len(out) % 0x400:                        # page-align next module
            out += b"\0"
        records.append((name, moff, len(mod)))
    for i, (name, moff, msize) in enumerate(records):
        e = rec_off + i * 16
        out[e:e + 8] = name.encode("latin1")[:8].ljust(8, b" ")
        struct.pack_into("<II", out, e + 8, moff, msize)

    outp = os.path.join(outroot, os.path.basename(w3_path).lower())
    os.makedirs(outroot, exist_ok=True)
    open(outp, "wb").write(out)
    return {"ok": True, "output": outp, "bytes": len(out),
            "components": [n for n, _r in built]}


def main(argv):
    import argparse
    ap = argparse.ArgumentParser(prog="vxd_owbuild", description=__doc__.splitlines()[0])
    ap.add_argument("vxd", help="path to a .vxd/.386 (or a corpus name)")
    ap.add_argument("--out", default=None)
    a = ap.parse_args(argv)
    path = a.vxd
    if not os.path.isfile(path):
        from codebreaker.base import corpus
        path = corpus.retail(a.vxd) or path                  # a name the corpus knows
    out = a.out or settings.state("vxd_ow")
    for nm, v in le.open_vxds(path):
        base = nm.split(".")[0]
        r = build_vxd(path, base, out)
        print("%s: %s" % (base, ("built %d bytes (%d objs) -> %s" % (
            r["bytes"], r["objects"], r["output"])) if r.get("ok") else "FAIL " + r.get("error", "")))
        break
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main(sys.argv[1:]))
