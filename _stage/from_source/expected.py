"""expected.py -- the ORIGINAL codebreaker.source.vxd.from_source on REAL VxDs of the Windows 95 corpus and seeded damaged copies of
them (every size field kept sane: see _ne_common/vxdcommon.py), and on seeded segment-tag lists / version resources.

Run as `python expected.py` from this folder (CB_WT names the worktree whose Python is imported; unset: the main checkout).
TEMP is tmp/ and the state folder state_py/ (state_cpp/ for test.exe).
Cases (tab separated; a text field is hex of its UTF-8, '~' empty):
  consts                               _RC_DIR, _ADRC_REF, _INC16_REF, _MASM_RESERVED
  seg_name TAG                         _seg_name(tag)
  object_segs TAGS DDB                 _object_segs(tags, ddb_obj): TAGS the hex of the tags joined by ','
  object_names SPEC                    object_names(v)   (SPEC: V:path|member of the corpus, or B:file of a damaged copy)
  reclass SPEC                         _reclassify_ddb_tail_as_data(D, v): [tail0, the instructions and labels left]
  realseg SPEC                         realseg_source / realseg_def of the driver: [sha1 + length of each]
  ver FILE                             ver_strings(bytes of FILE) / version_rc where the driver has a DDB
  stage                                _stage_res_tools(root): the folders' files and sizes
  embed SPEC                           embed_version_resource(build_le bytes, D, root): the bytes (length, sha1) and the log (sha1)
  result SPEC                          _result(build_le bytes, v, D)
  build SPEC                           build_from_source(v): every field (the logs without temp folder names)
  main OUT ARG...                      the script block (runpy, as __main__): [status, stdout, sha1 of the file it wrote]
"""
import contextlib
import io
import os
import random
import re
import runpy
import shutil
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
os.chdir(HERE)
sys.path.insert(0, os.path.join(os.path.dirname(HERE), "_ne_common"))
import vxdcommon as X  # noqa: E402
from vxdcommon import hexs, sha1  # noqa: E402

X.isolate_env(HERE, state=False)
os.makedirs("inputs", exist_ok=True)

from codebreaker.formats import le  # noqa: E402
from codebreaker.source.vxd import disassembly as dis  # noqa: E402
from codebreaker.source.vxd import from_source as F  # noqa: E402
from codebreaker.source.vxd import le_writer  # noqa: E402

K = X.VC.Cases(HERE)
open("capstone.txt", "w").write(X.VC.CAPSTONE_DLL)                  # the capstone.dll test.exe loads
rnd = random.Random(1115)
ROOT = X.MAIN


def fresh(v):
    D = dis.VxDDisasm(v).run()
    D.v = v
    return D


def norm_log(s):
    return re.sub(r"cb_idiom_\w+", "cb_idiom_X", s or "")


K.add("consts", lambda: [F._RC_DIR, list(F._ADRC_REF), list(F._INC16_REF), sorted(F._MASM_RESERVED)])
for tag in ["LCOD", "ICOD", "PCOD", "LOCK", "lock", "Rep", "BYTE", "AND", "DATA", "", "x", "_X", "a b", "éé", "OFFSET", "PUBLIC", "ASSUME", "MOD", "Seg"]:
    K.add("seg_name\t" + hexs(tag), lambda tag=tag: F._seg_name(tag))

pool = ["LCOD", "ICOD", "PCOD", "LOCK", "\x00\x00\x00\x00", "    ", "", "1AB", "_X", "a b", "éT", "DATA", "BYTE", "repe", "PCOD", "PCOD", "X$?@", "SEG2", "LCOD "]
for n in range(50):
    tags = [rnd.choice(pool) for _ in range(rnd.randrange(0, 7))]
    ddb = rnd.randrange(0, 8)
    K.add("object_segs\t%s\t%d" % (",".join(hexs(t) for t in tags) or "-", ddb),
          lambda tags=tags, ddb=ddb: sorted([k, v[0], v[1]] for k, v in F._object_segs(tags, ddb).items()))
    K.add("realseg_def_names\t%s\t%d" % (",".join(hexs(t) for t in tags) or "-", ddb), lambda tags=tags, ddb=ddb: F.realseg_def("NAME", tags, ddb))

drivers = X.small(40)
specs = [(X.VC.vspec(p, i), name, v) for p, i, name, v in drivers]
n_dmg = len(specs)
for k, (sp, name, v) in enumerate(list(specs[:12])):
    for var, data in enumerate(X.damaged_copies(sp, v, 2, 11000 + k * 29)):
        fn = "inputs/dmg_%d_%d.bin" % (k, var)
        with open(fn, "wb") as f:
            f.write(data)
        try:
            specs.append(("B:" + fn, "DMG", le.LEFile.from_buffer(data)))
        except Exception:  # noqa: BLE001   (a damaged copy that is no LE any more: not a case)
            os.remove(fn)
for sz, p, i, name, v in X.corpus():
    K.add("object_names\t%s" % X.VC.vspec(p, i), lambda v=v: F.object_names(v))
for sp, name, v in specs[n_dmg:]:
    K.add("object_names\t%s" % sp, lambda v=v: F.object_names(v))


def reclass(v):
    D = fresh(v)
    t = F._reclassify_ddb_tail_as_data(D, v)
    return [t, [len(D.insns), sha1(",".join("%d:%d" % k for k in sorted(D.insns)))],
            [len(D.labels), sha1(",".join("%d:%d=%s" % (k[0], k[1], x) for k, x in sorted(D.labels.items())))]]


def realseg(v):
    D = fresh(v)
    names = F.object_names(v)
    t = F._reclassify_ddb_tail_as_data(D, v)
    a = F.realseg_source(D, v, names, t)
    d = F.realseg_def("N" if not D.ddb else D.ddb["name"], names, D.ddb["obj"] if D.ddb else 1)
    return [len(a), sha1(a), len(d), sha1(d)]


for sp, name, v in specs:
    K.add("reclass\t%s" % sp, lambda v=v: reclass(v))
    K.add("realseg\t%s" % sp, lambda v=v: realseg(v))

# the version resources: every driver's own, and seeded ones
n_ver = 0
for sz, p, i, name, v in X.corpus():
    lfa = v.lfa
    import struct
    woff = struct.unpack_from("<I", v.d, lfa + 0xb8)[0]
    wlen = struct.unpack_from("<I", v.d, lfa + 0xbc)[0]
    if wlen and woff + wlen <= len(v.d) and n_ver < 120:
        fn = "inputs/ver_%d.bin" % n_ver
        with open(fn, "wb") as f:
            f.write(v.d[woff:woff + wlen])
        K.add("ver\t" + fn, lambda fn=fn: F.ver_strings(open(fn, "rb").read()))
        n_ver += 1
keys = [b"FileDescription", b"LegalCopyright", b"InternalName", b"OriginalFilename", b"Other"]
for n in range(30):
    buf = bytearray()
    for _ in range(rnd.randrange(0, 8)):
        if rnd.random() < 0.6:
            buf += rnd.choice(keys) + b"\0" * rnd.randrange(0, 4) + bytes(rnd.choice(b"abc \xe9 (C) 1995-96") for _ in range(rnd.randrange(0, 12))) + b"\0" * rnd.randrange(0, 3)
        else:
            buf += bytes(rnd.randrange(256) for _ in range(rnd.randrange(0, 20)))
    fn = "inputs/verr_%d.bin" % n
    with open(fn, "wb") as f:
        f.write(bytes(buf))
    K.add("ver\t" + fn, lambda fn=fn: F.ver_strings(open(fn, "rb").read()))
for sp, name, v in specs:
    def vrc(v=v):
        D = fresh(v)
        lfa = v.lfa
        woff = struct.unpack_from("<I", v.d, lfa + 0xb8)[0]
        wlen = struct.unpack_from("<I", v.d, lfa + 0xbc)[0]
        return F.version_rc(D, v.d[woff:woff + wlen])
    K.add("vrc\t%s" % sp, vrc)


def stage():
    tools, inc = F._stage_res_tools(ROOT)
    return [[[n, os.path.getsize(os.path.join(d, n))] for n in sorted(os.listdir(d))] for d in (tools, inc)]


K.add("stage", stage)


def built_bytes(v):
    D = fresh(v)
    return D, le_writer.build_le(D, v, D.ddb["name"] if D.ddb else "DEVICE")


def result_d(r):
    keys_ = ["ok", "vxd_len", "oracle_len", "objects", "page_map_identity", "ddb_rva", "oracle_ddb_rva", "device_id", "device_id_ok", "resource_len", "obj1_diffs"]
    return [r[k] for k in keys_] + [sha1(r["vxd"]), sha1(r["asm"]) if r["asm"] is not None else None, sha1(r["def"]) if r["def"] is not None else None,
                                     sha1(norm_log(r["link_log"])), sha1(norm_log(r["res_log"])), r.get("compile_ok"), r.get("link_ok"), r.get("object_flags")]


for sp, name, v in specs:
    def res(v=v):
        D, b = built_bytes(v)
        return result_d(F._result(b, v, D))
    K.add("result\t%s" % sp, res)


def emb(v):
    D, b = built_bytes(v)
    out, log = F.embed_version_resource(b, D, ROOT)
    return [None if out is None else [len(out), sha1(out)], sha1(norm_log(log)), len(log)]


for sp, name, v in specs[:3]:
    K.add("embed\t%s" % sp, lambda v=v: emb(v))
nores = [s for s in specs[:n_dmg] if struct.unpack_from("<I", s[2].d, s[2].lfa + 0xbc)[0] == 0][:1]
for sp, name, v in nores:
    K.add("embed\t%s" % sp, lambda v=v: emb(v))

# the whole build: ML + LINK 2.60 + the container writer; 512-byte-page drivers take the fallback path
b4 = [s for s in specs[:n_dmg] if s[2].pagesize == 0x1000][:3]
b5 = [s for s in specs[:n_dmg] if s[2].pagesize != 0x1000][:2]
dm = specs[n_dmg:n_dmg + 2]
for sp, name, v in b4 + b5 + dm:
    K.add("build\t%s" % sp, lambda v=v: result_d(F.build_from_source(v, root=ROOT)))
K.add("build_named\t%s" % b4[0][0], lambda v=b4[0][2]: result_d(F.build_from_source(v, name="MYNAME", root=ROOT)))

FS = os.path.join(os.path.dirname(F.__file__), "from_source.py")


def run_main(out, args):
    old = sys.argv
    sys.argv = [FS] + args
    buf, status = io.StringIO(), 0
    try:
        with contextlib.redirect_stdout(buf):
            runpy.run_path(FS, run_name="__main__")
    except SystemExit as e:
        status = e.code if isinstance(e.code, int) else (1 if e.code else 0)
    finally:
        sys.argv = old
    extra = None
    if out and os.path.exists(out):
        extra = sha1(open(out, "rb").read())
        os.remove(out)
    return [status, buf.getvalue(), extra]


pa = b4[0][0][2:].split("|")[0]
for out, args in (("", [pa]), ("out_fs.vxd", [pa, "out_fs.vxd"]), ("", [])):
    K.add("\t".join(["main", hexs(out)] + [hexs(a) for a in args]), lambda out=out, args=args: run_main(out, args))
K.close()
print(K.n, "cases")
