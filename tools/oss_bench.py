"""Open-source C projects as a ground truth (docs/STRUCTURING_PLAN.md, phase 0b): their authored
source beside what CodeBreaker makes of their binary.

    python tools/oss_bench.py [PROJECT ...] [--opt FLAGS]

Each project (PROJECTS; cloned under state/oss/<name>) is compiled with OpenWatcom into a 32-bit
DLL exporting every public function -- only its library's .c files, with our own compiler command;
nothing of the project's is run to build it. The DLL goes through the same pipeline as
`python -m codebreaker measure` (verify/reconstruct.py: decompile, rebuild, run beside it). Then,
per function -- named by the link map, statics too -- the authored C and ours are compared:

- the verdict (does ours behave as the original);
- gotos: the authors' (in their source) and ours; ours beyond theirs are the decompiler's.

The report: state/oss_bench.txt. The store: state/oss_bench.sqlite (CB_FUNCTIONS_DB).
"""
import collections
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

OSS = os.path.join(ROOT, "state", "oss")
WORK = os.path.join(ROOT, "state", "oss_build")
# name -> (the library's .c files (globs, relative), defines, include dirs)
PROJECTS = {
    "cJSON": (["cJSON.c", "cJSON_Utils.c"], ["_MSC_VER=1200"], ["."]),   # (its wrappers for a __cdecl malloc)
    "zlib": (["adler32.c", "compress.c", "crc32.c", "deflate.c", "infback.c", "inffast.c", "inflate.c",
              "inftrees.c", "trees.c", "uncompr.c", "zutil.c"], [], ["."]),
    "sds": (["sds.c"], ["__attribute__(x)="], ["."]),
    "tiny-AES-c": (["aes.c"], ["CBC=1", "CTR=1", "ECB=1"], ["."]),
    "inih": (["ini.c"], [], ["."]),
    "lua": (["lapi.c", "lcode.c", "lctype.c", "ldebug.c", "ldo.c", "ldump.c", "lfunc.c", "lgc.c", "llex.c",
             "lmem.c", "lobject.c", "lopcodes.c", "lparser.c", "lstate.c", "lstring.c", "ltable.c", "ltm.c",
             "lundump.c", "lvm.c", "lzio.c", "lauxlib.c", "lbaselib.c", "lcorolib.c", "ldblib.c", "liolib.c",
             "lmathlib.c", "loadlib.c", "loslib.c", "lstrlib.c", "ltablib.c", "lutf8lib.c", "linit.c"],
            ["LUA_32BITS=1"], ["."]),
    "miniz": (["miniz.c", "miniz_tdef.c", "miniz_tinfl.c", "miniz_zip.c"], ["MINIZ_NO_STDIO=1", "MINIZ_EXPORT="], [".", "../../oss_build/_shim"]),   # (an empty miniz_export.h: CMake makes it)
}
# (not yet built by OpenWatcom: cJSON's __cdecl hooks against OW's own malloc; sds's GNU C)
SET_ASIDE = ("cJSON", "sds")
GOTO = re.compile(r"\bgoto\s+\w+\s*;")


def build(name, opt):
    """-> (dll, {address: function name}) -- or raises with the compiler's or linker's complaint"""
    from codebreaker.build import watcom
    files, defines, incs = PROJECTS[name]
    src = os.path.join(OSS, name)
    out = os.path.join(WORK, name)
    os.makedirs(out, exist_ok=True)
    env = watcom.ow_env("pe")
    objs = []
    for f in files:
        obj = os.path.join(out, os.path.splitext(f)[0] + ".obj")
        cmd = [watcom.ow_tool("wcc386"), "-zq", "-bd", "-bt=nt", "-s", "-ecc", "-zastd=c99"] + opt.split() + \
              ["-d%s" % d for d in defines] + ["-i=" + os.path.join(src, i) for i in incs] + \
              ["-i=" + i for i in (lambda x: [x] if isinstance(x, str) else list(x))(watcom.ow_include("pe"))] +               [os.path.join(src, f), "-fo=" + obj]
        r = subprocess.run(cmd, env=env, cwd=out, capture_output=True, text=True)
        if r.returncode:
            raise RuntimeError("%s: %s does not compile: %s" % (name, f, (r.stdout + r.stderr).strip()[-300:]))
        objs.append(obj)
    dll = os.path.join(out, name + ".dll")
    mapf = os.path.join(out, name + ".map")
    link = [watcom.ow_tool("wlink"), "option", "quiet", "system", "nt_dll", "option", "map=" + mapf,
            "option", "statics"] + sum((["file", o] for o in objs), []) + ["name", dll]
    r = subprocess.run(link, env=env, cwd=out, capture_output=True, text=True)
    if r.returncode:
        raise RuntimeError("%s: does not link: %s" % (name, (r.stdout + r.stderr).strip()[-300:]))
    functions = read_map(mapf)
    # again, every public function exported -- a root for the decompiler to start from
    theirs = authored(name)
    publics = sorted({n for n, (_a, pub) in functions.items() if pub and n in theirs})
    r = subprocess.run(link + sum((["export", "%s=_%s" % (n, n)] for n in publics), []), env=env, cwd=out,
                       capture_output=True, text=True)
    if r.returncode:
        raise RuntimeError("%s: does not link with exports: %s" % (name, (r.stdout + r.stderr).strip()[-300:]))
    functions = read_map(mapf)
    return dll, {"%x" % a: n for n, (a, _pub) in functions.items()}


def read_map(mapf):
    """{C name: (address, public?)} of the code symbols in a wlink map"""
    out = {}
    text = open(mapf, encoding="latin1").read()
    for m in re.finditer(r"^([0-9a-f]{8})([s+*]?)\s+(\S+)\s*$", text, re.M):
        addr, flag, sym = m.groups()
        name = sym.split("@")[0].strip("_")
        out.setdefault(name, (int(addr, 16), flag != "s"))   # (s: a static)
    return out


_AUTHORED = {}


def authored(name):
    """{function name: gotos in its authored C}"""
    if name in _AUTHORED:
        return _AUTHORED[name]
    from codebreaker.decompile import ctree
    files = PROJECTS[name][0]
    out = {}
    for f in files:
        text = open(os.path.join(OSS, name, f), encoding="utf-8", errors="replace").read()
        try:
            src = ctree.Source(text)
        except Exception:
            continue
        for fn in src.find("function_definition"):
            d = fn.child_by_field_name("declarator")
            while d is not None and d.type not in ("function_declarator",):
                d = d.child_by_field_name("declarator")
            ident = d.child_by_field_name("declarator") if d is not None else None
            if ident is None:
                continue
            out[src.at(ident)] = len(src.find("goto_statement", under=fn))
    _AUTHORED[name] = out
    return out


def main(argv):
    from codebreaker.store import functions as F
    from codebreaker.verify import reconstruct
    opt = "-ox"
    if "--opt" in argv:
        opt = argv[argv.index("--opt") + 1]
        argv = [a for a in argv if a not in ("--opt", opt)]
    names = argv or [n for n in PROJECTS if n not in SET_ASIDE]
    db = os.environ.get("CB_FUNCTIONS_DB") or os.path.join(ROOT, "state", "oss_bench.sqlite")
    os.environ["CB_FUNCTIONS_DB"] = db
    fs = F.open()
    lines = []
    tot = collections.Counter()
    for name in names:
        try:
            dll, at = build(name, opt)
        except RuntimeError as e:
            lines.append("%-11s %s" % (name, e))
            print(lines[-1], flush=True)
            continue
        sha, digest = reconstruct.run(dll, fs=fs)
        rows = {r["addr"]: r for r in fs.candidates(sha, digest)}
        theirs = authored(name)
        c = collections.Counter()
        for addr, fn in at.items():
            if fn not in theirs:
                continue                                 # (the runtime library's, not the project's)
            r = rows.get(addr)
            c["functions"] += 1
            c[(r or {}).get("verdict") or "none"] += 1
            ours = len(GOTO.findall((r or {}).get("source") or ""))
            c["their gotos"] += theirs[fn]
            c["our gotos"] += ours
            c["gotos we added"] += max(0, ours - theirs[fn])
        tot.update(c)
        lines.append("%-11s functions %4d  equivalent %4d  consistent %3d  divergent %4d  no-compile %3d  "
                     "gotos: theirs %3d, ours %3d, added %3d" % (
                         name, c["functions"], c["equivalent"], c["consistent"], c["divergent"], c["no-compile"],
                         c["their gotos"], c["our gotos"], c["gotos we added"]))
        print(lines[-1], flush=True)
    lines.append("%-11s functions %4d  equivalent %4d  consistent %3d  divergent %4d  no-compile %3d  "
                 "gotos: theirs %3d, ours %3d, added %3d" % (
                     "TOTAL", tot["functions"], tot["equivalent"], tot["consistent"], tot["divergent"],
                     tot["no-compile"], tot["their gotos"], tot["our gotos"], tot["gotos we added"]))
    with open(os.environ.get("CB_OSS_REPORT") or os.path.join(ROOT, "state", "oss_bench.txt"), "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print(lines[-1])
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
