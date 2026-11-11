"""expected.py -- the ORIGINAL codebreaker.source.vxd.owbuild on REAL VxDs of the Windows 95 corpus (the smallest standalone drivers),
W3 bundles made of them, and seeded damaged inputs.

Run as `python expected.py` from this folder (CB_WT names the worktree whose Python is imported; unset: the main checkout).
TEMP is tmp/ and the state folder state_py/ (state_cpp/ for test.exe); what is built goes under ow_out/ (emptied before the C++ run).
Cases (tab separated; a text field is hex of its UTF-8, '~' empty):
  consts                              the tools' paths relative to the OpenWatcom folder
  env                                 _env(): WATCOM, the first PATH entry, whether the rest of the environment is kept
  w3comp FILE                         _w3_components(bytes of FILE) (inputs/): [lfa, [[name, offset, size]]] or the error type
  build PATH NAME BYTEEXACT DIR       build_vxd(path, name, DIR, byte_exact): [ok, output (relative to DIR), error, bytes, objects, sha1 of the .vxd]
  build_w3 FILE DIR                   build_w3(FILE, DIR): the result and the sha1 of the file it wrote
  main DIR ARG...                     main(argv): [status, stdout (the state folder as <STATE>), the files under DIR]
"""
import contextlib
import io
import os
import random
import shutil
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
os.chdir(HERE)
sys.path.insert(0, os.path.join(os.path.dirname(HERE), "_ne_common"))
import vxdcommon as X  # noqa: E402
from vxdcommon import hexs, sha1  # noqa: E402

X.isolate_env(HERE)
shutil.rmtree("ow_out", ignore_errors=True)
os.makedirs("ow_out", exist_ok=True)
os.makedirs("inputs", exist_ok=True)

from codebreaker.formats import le  # noqa: E402
from codebreaker.source.vxd import disassembly as dis  # noqa: E402
from codebreaker.source.vxd import owbuild as O  # noqa: E402
from codebreaker.source.vxd import tree as T  # noqa: E402

K = X.VC.Cases(HERE)
open("capstone.txt", "w").write(X.VC.CAPSTONE_DLL)                  # the capstone.dll test.exe loads
rnd = random.Random(1114)


def files_d(folder):
    out = []
    for dp, _dn, fn in os.walk(folder):
        for n in fn:
            p = os.path.join(dp, n)
            data = open(p, "rb").read()
            if n.endswith(".obj"):                       # (a COFF object holds the time it was made: the TimeDateStamp at offset 4)
                data = data[:4] + b"\0\0\0\0" + data[8:]
            out.append([os.path.relpath(p, folder).replace("\\", "/"), sha1(data)])
    return sorted(out)


def rel(p, base):
    return os.path.relpath(p, base).replace("\\", "/")


K.add("consts", lambda: [rel(O._WASM, O._OW), rel(O._WLINK, O._OW), os.path.basename(O._NASM)])


def envcase():
    e = O._env()
    first = e["PATH"].split(os.pathsep)[0]
    keep = all(e.get(k) == v for k, v in os.environ.items() if k not in ("PATH", "WATCOM"))
    return [rel(e["WATCOM"], O._OW), rel(first, O._OW), keep, e["PATH"].split(os.pathsep, 1)[1] == os.environ.get("PATH", "")]


K.add("env", envcase)

drivers = X.small(8)


def w3_bundle(paths, names=None, count=None):
    """a W3 file of the standalone drivers: a 0x80-byte MZ front, the header, the records, the modules (page aligned)"""
    datas = [open(p, "rb").read() for p in paths]
    n = len(datas)
    front = bytearray(0x80)
    front[0:2] = b"MZ"
    struct.pack_into("<I", front, 0x3C, 0x80)
    hdr = b"W3" + struct.pack("<HI", 0x0400, n if count is None else count) + b"\0" * 8
    body_start = (0x80 + 16 + 16 * n + 0x3FF) & ~0x3FF
    out = bytearray(front) + hdr + b"\0" * (16 * n)
    while len(out) < body_start:
        out += b"\0"
    recs = []
    for i, d in enumerate(datas):
        off = len(out)
        out += d
        while len(out) % 0x400:
            out += b"\0"
        recs.append(((names[i] if names else os.path.basename(paths[i]).split(".")[0].upper()).encode("latin1")[:8].ljust(8, b" "), off, len(d)))
    for i, (nm, off, size) in enumerate(recs):
        e = 0x90 + i * 16
        out[e:e + 8] = nm
        struct.pack_into("<II", out, e + 8, off, size)
    return bytes(out)


w3s = {
    "w3_two.vxd": w3_bundle([drivers[0][0], drivers[1][0]]),
    "w3_three.vxd": w3_bundle([drivers[2][0], drivers[3][0], drivers[4][0]], names=["AAA", "A LONG NAME", "C"]),
    "w3_one.vxd": w3_bundle([drivers[5][0]]),
}
bad = {
    "w3_nosig.vxd": b"MZ" + b"\0" * 0x3A + struct.pack("<I", 0x80) + b"\0" * 0x40 + b"XX" + b"\0" * 100,
    "w3_short.vxd": b"MZ" + b"\0" * 10,
    "w3_hugecount.vxd": w3s["w3_two.vxd"][:0x84] + struct.pack("<I", 0x7fffffff) + w3s["w3_two.vxd"][0x88:0x400],
    "w3_zero.vxd": w3s["w3_two.vxd"][:0x84] + struct.pack("<I", 0) + w3s["w3_two.vxd"][0x88:0x400],
    "w3_trunc.vxd": w3s["w3_two.vxd"][:0xa0],
    "w3_badlfa.vxd": w3s["w3_two.vxd"][:0x3c] + struct.pack("<I", 0xffffff) + w3s["w3_two.vxd"][0x40:0x400],
}
for r in range(6):
    bad["w3_rand%d.vxd" % r] = bytes(rnd.randrange(256) for _ in range(rnd.choice([0x40, 0x50, 0x200])))
for nm, data in list(w3s.items()) + list(bad.items()):
    with open("inputs/" + nm, "wb") as f:
        f.write(data)
    K.add("w3comp\tinputs/" + nm, lambda nm=nm: (lambda r: [r[0], [list(x) for x in r[1]]])(O._w3_components(open("inputs/" + nm, "rb").read())))

n_b = [0]


def build_case(path, name, byte_exact):
    n_b[0] += 1
    folder = "ow_out/b%d" % n_b[0]

    def f():
        r = O.build_vxd(path, name, folder, byte_exact)
        out = r.get("output")
        return [r["ok"], rel(out, folder) if out else None, r.get("error"), r.get("bytes"), r.get("objects"),
                sha1(open(out, "rb").read()) if out and os.path.exists(out) else None]
    K.add("build\t%s\t%s\t%d\t%s" % (path, hexs(name), 1 if byte_exact else 0, folder), f)


for p, i, nm, v in drivers[:3]:
    for be in (True, False):
        build_case(p, nm.split(".")[0], be)
for p, i, nm, v in drivers[3:5]:
    build_case(p, nm.split(".")[0].upper(), True)
build_case(os.path.join(HERE, "no", "such.vxd"), "NOSUCH", True)                      # the file is not there
build_case(os.path.join(HERE, "inputs", "w3_nosig.vxd"), "BAD", True)                  # not a VxD
for k, (p, i, nm, v) in enumerate(drivers[:3]):                                        # seeded damaged copies
    for var, data in enumerate(X.damaged_copies(p, v, 2, 7000 + k * 17)):
        fn = os.path.join(HERE, "inputs", "dmg_%d_%d.bin" % (k, var))
        with open(fn, "wb") as f:
            f.write(data)
        build_case(fn, "DMG%d%d" % (k, var), True)

n_w = [0]
for nm in ("w3_two.vxd", "w3_one.vxd", "w3_nosig.vxd", "w3_trunc.vxd", "w3_three.vxd"):
    n_w[0] += 1
    folder = "ow_out/w%d" % n_w[0]

    def f(nm=nm, folder=folder):
        r = O.build_w3("inputs/" + nm, folder)
        out = r.get("output")
        comps = [c if isinstance(c, str) else list(c) for c in r["components"]] if "components" in r else None
        return [r["ok"], r.get("error"), rel(out, folder) if out else None, r.get("bytes"), comps,
                sha1(open(out, "rb").read()) if out and os.path.exists(out) else None]
    K.add("build_w3\tinputs/%s\t%s" % (nm, folder), f)


def run_main(out, args):
    buf, status = io.StringIO(), 0
    try:
        with contextlib.redirect_stdout(buf), contextlib.redirect_stderr(io.StringIO()):
            status = O.main(args)
    except SystemExit as e:
        status = e.code if isinstance(e.code, int) else (1 if e.code else 0)
    text = buf.getvalue().replace(os.environ["CB_STATE"], "<STATE>")
    return [status, text if status == 0 else "", files_d(out) if out and os.path.isdir(out) else None]


p0 = drivers[6][0]
for out, args in (("ow_out/m1", [p0, "--out", "ow_out/m1"]), ("ow_out/m2", [os.path.basename(drivers[7][0]), "--out=ow_out/m2"]), ("", []),
                  ("", [p0, "--out"]), ("", [os.path.join(HERE, "no", "such.vxd")]), ("", [p0, "extra"]), ("", ["--nosuch"]),
                  ("ow_out/m3", [os.path.join(HERE, "inputs", "w3_nosig.vxd"), "--out", "ow_out/m3"])):
    K.add("\t".join(["main", hexs(out)] + [hexs(a) for a in args]), lambda out=out, args=args: run_main(out, args))
K.close()
print(K.n, "cases")
