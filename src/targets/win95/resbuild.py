#!/usr/bin/env python3
"""resbuild.py -- the resource-only build lane for Win95 DLLs that contain NO code.

A handful of Win95 "DLLs" are pure resource containers -- no .text / code segment,
no entry point, no exported functions -- just icons, bitmaps, strings and version
info (e.g. moricons.dll, the Program Manager icon library; homebase.dll; hpvres.dll).
There is no source to reconstruct, so the compile frontier (owbuild/autocure) reports
them as fns=0. They are still real DLLs we must be able to REBUILD.

This lane reproduces them faithfully: every resource is preserved VERBATIM (its exact
type id, name id, language and raw bytes) as a raw-data statement in a generated .rc,
so the resource directory -- including the RT_GROUP_ICON -> RT_ICON references -- comes
out identical. It then links a code-less DLL (a one-symbol stub object) and binds the
resources in with wrc, using the from-source OpenWatcom tools (wcc/wcc386 + wrc + wlink).

    wt --target win95 resbuild <module.dll> [--out DIR]

Verify = re-parse the built DLL and diff its resource set (per type/name/lang: size and
bytes) against the original; a clean lane reproduces every resource exactly.
"""
import argparse
import os
import subprocess
import sys

from src.formats import exeinfo
from src.targets.win95 import owbuild


def _flatten(info):
    """[(type_id, name, lang, foff, size)] for every resource, in directory order."""
    out = []
    res = info.get("resources") or {}
    for ty in res.get("types", []):
        for it in ty.get("items", []):
            out.append((int(it.get("type_id", ty.get("id", 0))),
                        it.get("name", ""), it.get("lang"),
                        int(it["foff"]), int(it["size"])))
    return out


def _name_token(name):
    """RC name token: a number where the resource id is numeric, else a quoted string."""
    s = str(name)
    if s.startswith("#"):
        s = s[1:]
    if s.isdigit():
        return s
    return '"%s"' % s.replace('"', '')


def _lang_pair(lang):
    """(primary, sub) from a Win32 LANGID, or None for neutral/absent."""
    if not lang:
        return None
    return (lang & 0x3FF, (lang >> 10) & 0x3F)


def build(module_path, outdir=None):
    info = exeinfo.parse(module_path)
    fmt = info.get("format")
    if fmt not in ("PE", "NE"):
        return {"ok": False, "error": "resbuild handles PE/NE only, got %s" % fmt}
    items = _flatten(info)
    if not items:
        return {"ok": False, "error": "no resources found"}

    base = os.path.splitext(os.path.basename(module_path))[0]
    from src.targets.win95 import buildall
    bld = outdir or os.path.join(buildall._design(), "scaffold", base)
    resdir = os.path.join(bld, "res")
    os.makedirs(resdir, exist_ok=True)
    cls = "ne" if fmt == "NE" else "pe"
    bt = "windows" if fmt == "NE" else "nt"

    # 1) extract every resource's raw bytes verbatim
    raw = open(module_path, "rb").read()
    rc_lines = []
    cur_lang = "NONE"
    for idx, (tid, name, lang, foff, size) in enumerate(items):
        data = raw[foff:foff + size]
        fn = "r%04d.bin" % idx
        with open(os.path.join(resdir, fn), "wb") as f:
            f.write(data)
        # group consecutive resources by language (PE); NE has none -> neutral
        lp = _lang_pair(lang)
        key = "%d,%d" % lp if lp else "NONE"
        if key != cur_lang:
            if lp:
                rc_lines.append("LANGUAGE 0x%02X, 0x%02X" % lp)
            cur_lang = key
        # `nameID typeID filename` -- numeric typeID keeps wrc from re-parsing a
        # standard type, so the bytes are stored exactly as they were.
        rc_lines.append('%s %d "res/%s"' % (_name_token(name), tid, fn))

    rc = os.path.join(bld, base + ".rc")
    with open(rc, "w", newline="\n") as f:
        f.write("/* resbuild: verbatim resource reproduction of %s (%d resources) */\n"
                % (os.path.basename(module_path), len(items)))
        f.write("\n".join(rc_lines) + "\n")

    # 2) build with the from-source OpenWatcom tools
    env = owbuild._ow_env(cls)
    log = []
    cc = "wcc386" if cls == "pe" else "wcc"
    ccflags = (["-bt=nt", "-q", "-s"] if cls == "pe"
               else ["-bt=windows", "-mm", "-q", "-s"])
    # a code-less DLL still needs one object to link. For NE (Win16) the DLL
    # startup (LIBENTRY) calls a user LibMain/WEP, so provide trivial ones -- the
    # DLL has no real init. For PE a single data symbol is enough.
    stub_c = os.path.join(bld, "resstub.c")
    with open(stub_c, "w", newline="\n") as f:
        if cls == "ne":
            f.write("#include <windows.h>\n"
                    "int FAR PASCAL LibMain(HINSTANCE h, WORD ds, WORD heap, LPSTR cmd)\n"
                    "{ (void)h;(void)ds;(void)heap;(void)cmd; return 1; }\n"
                    "int FAR PASCAL WEP(int n) { (void)n; return 1; }\n")
        else:
            f.write("int __resonly_marker = 0;\n")
    if owbuild._run([cc] + ccflags + ["resstub.c"], bld, env, log) != 0:
        return {"ok": False, "error": "stub compile failed", "log": log, "dir": bld}

    # .rc -> .res
    if owbuild._run(["wrc", "-q", "-r", "-bt=" + bt, base + ".rc"], bld, env, log) != 0:
        return {"ok": False, "error": "wrc failed", "log": log, "dir": bld}

    out = base + ".dll"
    with open(os.path.join(bld, "link.lnk"), "w", newline="\n") as f:
        f.write("system %s\n" % ("windows_dll" if cls == "ne" else "nt_dll"))
        f.write("name %s\n" % out)
        f.write("option quiet\n")
        f.write("option resource=%s.res\n" % base)
        f.write("file resstub\n")
    if owbuild._run(["wlink", "@link.lnk"], bld, env, log) != 0:
        return {"ok": False, "error": "wlink failed", "log": log, "dir": bld}

    outpath = os.path.join(bld, out)
    if not os.path.isfile(outpath):
        return {"ok": False, "error": "link produced no %s" % out, "log": log, "dir": bld}

    # 3) verify: same resource set (per type/name: size + bytes) as the original
    verify = _verify(module_path, outpath, items)
    return {"ok": True, "output": outpath, "size": os.path.getsize(outpath),
            "resources": len(items), "verify": verify, "log": log, "dir": bld}


def _verify(orig_path, built_path, orig_items):
    """Compare the built DLL's resources against the original: how many reproduce
    byte-for-byte. Keyed by (type_id, name) -- language may be renormalized by wrc."""
    try:
        binfo = exeinfo.parse(built_path)
    except Exception as e:
        return {"parsed": False, "error": str(e)}
    braw = open(built_path, "rb").read()
    oraw = open(orig_path, "rb").read()
    built = {}
    for tid, name, lang, foff, size in _flatten(binfo):
        built.setdefault((tid, str(name).lstrip("#").upper()), braw[foff:foff + size])
    exact = 0
    for tid, name, lang, foff, size in orig_items:
        key = (tid, str(name).lstrip("#").upper())
        if key in built and built[key] == oraw[foff:foff + size]:
            exact += 1
    return {"parsed": True, "orig": len(orig_items), "built": len(built),
            "byte_exact": exact}


def main(argv):
    ap = argparse.ArgumentParser(prog="resbuild", description=__doc__.splitlines()[0])
    ap.add_argument("module", help="path to a resource-only DLL (or a corpus name)")
    ap.add_argument("--out", default=None, help="build directory")
    a = ap.parse_args(argv)

    path = a.module
    if not os.path.isfile(path):
        # resolve a bare name against the corpus, preferring WINDOWS/SYSTEM
        from src.targets.win95 import buildall
        hits = []
        for dp, _, fs in os.walk(buildall._corpus()):
            for fn in fs:
                if fn.lower() == a.module.lower():
                    hits.append(os.path.join(dp, fn))
        hits.sort(key=lambda p: (0 if "system" in p.lower() else 1))
        if not hits:
            print("not found:", a.module); return 2
        path = hits[0]

    r = build(path, a.out)
    if not r.get("ok"):
        print("resbuild FAILED:", r.get("error"))
        for l in (r.get("log") or [])[-12:]:
            print("   ", l)
        return 1
    v = r["verify"]
    print("resbuild OK: %s (%d bytes, %d resources) -> %s"
          % (os.path.basename(r["output"]), r["size"], r["resources"], r["output"]))
    if v.get("parsed"):
        print("   verify: %d/%d resources reproduced byte-exact"
              % (v["byte_exact"], v["orig"]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
