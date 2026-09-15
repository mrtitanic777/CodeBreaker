#!/usr/bin/env python3
r"""bodyverify16.py -- per-body compile-verify for the Win16 NE lane (Layer 3: the 16-bit core).

The NE counterpart of bodyverify.py. For one 16-bit function it:
  1. SYNTHESISES a clean C body from the original's call flow (interproc16.reconstruct_at);
  2. COMPILES just that body into a real Win16 NE DLL -- OpenWatcom wcc (16-bit) + wlink against the
     AUTHENTIC Win16 runtime (lib286/win: libentry.obj + clibc.lib + windows.lib), exporting the body;
  3. VERIFIES it against the ORIGINAL on the NE dynamic oracle (ne_dynoracle.probe) under identical
     inputs -- retail body by (seg,offset), rebuilt body by its export ordinal;
  4. compares verify95's codegen-independent fingerprint (ret, exit, imported-call sequence).

This is the 16-bit core (krnl386/gdi/user) lane -- the boot-critical junction between the DOS+VMM base
and Win32. Exported and internal bodies are both addressable (probe takes seg/offset or ordinal).

    wt --target win95 bodyverify16 <module.exe> [--max N]
    wt --target win95 bodyverify16 <module.exe> --seg S --off 0xNNN
"""
import os
import re
import subprocess

from src.engines.behavior import interproc16
from src.targets.win95 import ne_dynoracle as NO, verify95, origintriage as OT


def _ow():
    from src.core import paths
    root = os.path.join(paths.repo_root(), "experiments", "premise-proof", "open-watcom-v2", "rel")
    return {"root": root,
            "wcc": os.path.join(root, "binnt64", "wcc.exe"),
            "wlink": os.path.join(root, "binnt64", "wlink.exe"),
            "wlink_lnk": os.path.join(root, "binnt64", "wlink.lnk"),
            "lib": os.path.join(root, "lib286", "win"),
            "inc": [os.path.join(root, "h", "win"), os.path.join(root, "h")]}


_DEF_RE = re.compile(r"(?m)^\s*(?:WORD|int|unsigned|long|void|LRESULT|BOOL|LONG)\s+"
                     r"(?:__far\s+|__pascal\s+|__export\s+|FAR\s+|PASCAL\s+)*"
                     r"([A-Za-z_]\w*)\s*\(")


def synth16(module, seg, off):
    """Clean C for one 16-bit function, or (None, reason). fname is the emitted function's name."""
    try:
        r = interproc16.reconstruct_at(module, seg, off)
    except Exception as e:
        return None, "synth error: " + type(e).__name__
    if not r.get("reconstructed") or not r.get("emitted_c"):
        return None, r.get("reason", "no synth")
    c = r["emitted_c"]
    m = _DEF_RE.search(c)
    return (m.group(1) if m else "sub_%d_%x" % (seg, off)), c


_C_KW = {"if", "else", "while", "for", "do", "switch", "case", "return", "goto", "break", "continue",
         "sizeof", "void", "int", "char", "short", "long", "unsigned", "signed", "struct", "union",
         "enum", "const", "static", "default", "WORD", "BYTE", "DWORD", "LONG", "BOOL"}


def compile_body16(c_text, fname, outdir):
    """Compile ONE synthesised 16-bit body into a real Win16 NE DLL exporting `fname`. Returns
    (dll_path|None, log)."""
    ow = _ow()
    if not os.path.exists(ow["wcc"]):
        return None, "OpenWatcom 16-bit (wcc) not built"
    os.makedirs(outdir, exist_ok=True)
    # give the exported body the Win16 DLL export decoration (far + pascal + __export)
    c_text = re.sub(r"(?m)^(\s*)(WORD|int|unsigned|long|void)\s+(%s)\s*\(" % re.escape(fname),
                    r"\1\2 __far __pascal __export \3(", c_text, count=1)
    # every called identifier that isn't defined here / a keyword -> a far cdecl stub (caller cleans,
    # so the body's own stack stays balanced regardless of the call's arg count).
    called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", c_text))
    defined = set(re.findall(r"\b(%s)\s*\(" % re.escape(fname), c_text)) | {"LibMain", "WEP"}
    stubs = ["WORD __far __cdecl %s(){return 0;}" % s for s in sorted(called - defined - _C_KW)]
    pre = ["typedef unsigned WORD;", "typedef unsigned char BYTE;", "typedef unsigned long DWORD;",
           "typedef int BOOL;", "typedef long LONG;", "typedef unsigned UINT;",
           "typedef unsigned HANDLE;", "typedef unsigned HWND;", "typedef char __far* LPSTR;",
           "typedef const char __far* LPCSTR;"]
    entry = ["int __far __pascal LibMain(unsigned h,unsigned ds,unsigned hp,char __far*cl){return 1;}",
             "int __far __pascal WEP(int n){return 1;}"]
    src = "\n".join(pre + stubs + entry) + "\n\n" + c_text + "\n"
    cf = os.path.join(outdir, fname + ".c")
    open(cf, "w", newline="\n").write(src)
    env = dict(os.environ)
    env["WATCOM"] = ow["root"]
    env["INCLUDE"] = ";".join(ow["inc"])
    env["WLINK_LNK"] = ow["wlink_lnk"]                        # so `system windows` resolves
    env["PATH"] = os.path.join(ow["root"], "binnt64") + os.pathsep + env.get("PATH", "")
    r = subprocess.run([ow["wcc"], "-bt=windows", "-mc", "-zq", "-w0", fname + ".c"],
                       cwd=outdir, env=env, capture_output=True, text=True, timeout=120)
    if r.returncode != 0:
        return None, "compile: " + (r.stdout + r.stderr).strip().split("\n")[-1][:90]
    lnk = ("system windows dll initinstance memory\nname %s.dll\noption quiet\n"
           "libpath %s\nlibfile libentry.obj\nfile %s.obj\nlibrary clibc.lib,windows.lib\n"
           "export %s\nexport WEP RESIDENT\n" % (fname, ow["lib"], fname, fname))
    open(os.path.join(outdir, "b.lnk"), "w", newline="\n").write(lnk)
    subprocess.run([ow["wlink"], "@b.lnk"], cwd=outdir, env=env, capture_output=True, text=True,
                   timeout=120)
    dll = os.path.join(outdir, fname + ".dll")
    return (dll if os.path.exists(dll) else None), ("ok" if os.path.exists(dll) else "link failed")


def _fp(probe_result):
    """verify95-shaped fingerprint from an ne_dynoracle probe record."""
    r = probe_result
    deps = tuple(verify95._norm_dep(h.get("api")) for h in r.get("imports_hit", [])
                 if not verify95._is_noise(h.get("api")))
    return {"ret": r.get("ret"), "exit": verify95._exit_norm(r.get("exit")), "deps": deps,
            "truncated": bool(r.get("truncated"))}


def _rebuilt_ordinal(dll, fname):
    """The export ordinal of `fname` in the rebuilt NE DLL (probe addresses the rebuilt by ordinal)."""
    from src.formats.ne import NEFile
    try:
        for name, ordn in NEFile(dll).resident_names():
            if name.upper() == fname.upper():
                return ordn
    except Exception:
        pass
    return 1


def verify_body16(retail, seg, off, arity=4, outdir=None):
    """synth -> compile (real Win16 NE) -> probe retail(seg,off) vs rebuilt(ordinal) -> verdict."""
    fname, c = synth16(retail, seg, off)
    if not fname:
        return {"seg": seg, "off": off, "verdict": "no-synth", "reason": c}
    outdir = outdir or os.path.join(os.path.dirname(__file__), "_bodyverify16", "%d_%x" % (seg, off))
    dll, clog = compile_body16(c, fname, outdir)
    if not dll:
        return {"seg": seg, "off": off, "verdict": "no-compile", "reason": clog, "c": c}
    ordn = _rebuilt_ordinal(dll, fname)
    vecs = verify95._vectors(arity, "ne")
    rs, bs = [], []
    for v in vecs:
        try:
            rs.append(_fp(NO.probe(retail, seg=seg, offset=off, args=v["args"])))
        except Exception as e:
            rs.append({"err": type(e).__name__})
        try:
            bs.append(_fp(NO.probe(dll, ordinal=ordn, args=v["args"])))
        except Exception as e:
            bs.append({"err": type(e).__name__})
    return {"seg": seg, "off": off, "verdict": verify95._body_verdict(rs, bs), "fname": fname}


def run(module, seg=None, off=None, maxn=40):
    from src.targets.win95 import bodyverify as B
    from src.formats.ne import NEFile
    from collections import Counter
    retail = B._retail(module)
    if seg is not None and off is not None:
        r = verify_body16(retail, seg, off)
        print("  %d:%04x  %-12s %s" % (seg, off, r["verdict"], r.get("reason", "")[:70]))
        return
    fns = interproc16.all_functions(NEFile(retail))
    c = Counter()
    for s, o in fns[:maxn]:
        r = verify_body16(retail, s, o)
        c[r["verdict"]] += 1
        print("  %d:%04x  %-12s %s" % (s, o, r["verdict"], r.get("reason", "")[:60]))
    print("  --- " + "  ".join("%s=%s" % (k, v) for k, v in c.most_common()) + " ---")


def main(argv):
    pos = [a for a in argv if not a.startswith("--")]
    if not pos:
        print(__doc__); return
    _v = lambda f: argv[argv.index(f) + 1] if f in argv else None
    seg = _v("--seg"); off = _v("--off"); mx = _v("--max")
    run(pos[0], seg=int(seg) if seg else None,
        off=int(off, 0) if off else None, maxn=int(mx) if mx else 40)


if __name__ == "__main__":
    import sys
    main(sys.argv[1:])
