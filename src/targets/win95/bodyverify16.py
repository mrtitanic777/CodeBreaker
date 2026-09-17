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


_DEF_RE = re.compile(r"(?m)^\s*(?:WORD|int|unsigned|long|void|LRESULT|BOOL|LONG|char|short|"
                     r"unsigned\s+\w+)\s+(?:__far\s+|__pascal\s+|__export\s+|__fastcall\s+|"
                     r"__cdecl\s+|__stdcall\s+|FAR\s+|PASCAL\s+)*([A-Za-z_]\w*)\s*\(")


_NE_BLOCKS = {}


def _ne_blocks(module):
    """{linear_addr: source-block} from win95dec --module (NE). The decompiler keys functions by
    linear address = (seg<<16)|off and names them with the recovered export name (or sub_<lin>)."""
    if module in _NE_BLOCKS:
        return _NE_BLOCKS[module]
    from src.core import paths
    dec = os.path.join(paths.repo_root(), "native", "decompiler", "build", "bin", "win95dec.exe")
    out = {}
    try:
        r = subprocess.run([dec, module, "--module", "0"], capture_output=True, text=True, timeout=300)
        text = r.stdout or ""
    except Exception:
        text = ""
    for m in re.finditer(r"//\s+\w+\s+@\s+0x([0-9A-Fa-f]+)\s*\n", text):
        lin = int(m.group(1), 16)
        b = text.find("{", m.end())
        if b < 0:
            continue
        depth, j = 0, b                                   # brace-match the function body
        while j < len(text):
            if text[j] == "{":
                depth += 1
            elif text[j] == "}":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        if depth == 0:
            # include the signature line (between the @addr comment and the brace)
            sig = text[m.end():b]
            out[lin] = sig + text[b:j + 1]
    _NE_BLOCKS[module] = out
    return out


def synth16(module, seg, off):
    """Clean C for one 16-bit function, or (None, reason). Ladder: interproc16 call-flow -> static
    decompile (win95dec --module block, full logic). fname is the emitted function's name."""
    try:
        r = interproc16.reconstruct_at(module, seg, off)
    except Exception:
        r = {"reconstructed": False}
    if r.get("reconstructed") and r.get("emitted_c"):
        c = r["emitted_c"]
        m = _DEF_RE.search(c)
        return (m.group(1) if m else "sub_%d_%x" % (seg, off)), c
    # static fallback: the full-logic decompiled body (named with its recovered export name)
    blk = _ne_blocks(module).get((seg << 16) | off)
    if blk:
        m = _DEF_RE.search(blk)
        if m:
            return m.group(1), blk
    return None, r.get("reason", "no synth (leaf + static block missing)")


_C_KW = {"if", "else", "while", "for", "do", "switch", "case", "return", "goto", "break", "continue",
         "sizeof", "void", "int", "char", "short", "long", "unsigned", "signed", "struct", "union",
         "enum", "const", "static", "default", "WORD", "BYTE", "DWORD", "LONG", "BOOL", "UINT",
         "HANDLE", "HWND", "LPSTR", "LPCSTR", "undefined", "undefined1", "undefined2", "undefined4",
         "__far", "__near", "__huge", "__pascal", "__cdecl", "__stdcall", "__fastcall", "__export",
         "__thiscall", "__interrupt", "FAR", "PASCAL", "far", "near",
         "__asm", "_asm", "asm", "__emit", "__based", "__self"}


def compile_body16(c_text, fname, outdir):
    """Compile ONE synthesised 16-bit body into a real Win16 NE DLL exporting `fname`. Returns
    (dll_path|None, log)."""
    ow = _ow()
    if not os.path.exists(ow["wcc"]):
        return None, "OpenWatcom 16-bit (wcc) not built"
    os.makedirs(outdir, exist_ok=True)
    # --- cure the raw decompiler output into compilable 16-bit C ---------------------------------
    c_text = re.sub(r"(?m)^\s*#\d.*$", "", c_text)                     # strip #<line> artifacts
    # sanitize characters that are legal in decorated/mangled symbols but not in C identifiers:
    # `@` (Watcom __fastcall leading @, e.g. an imported `KERNEL_@4` ordinal call) and `$`. Never
    # touch `?` -- it is the ternary operator. Do this before any identifier analysis so the call/
    # stub/decl passes see clean names (an `@` split a name and produced an invalid declarator +
    # an E1147 cascade).
    c_text = c_text.replace("@", "_").replace("$", "_")
    c_text = re.sub(r"\b__int8\b", "char", c_text)                     # MSVC sized ints -> standard
    c_text = re.sub(r"\bunsigned\s+__int16\b", "unsigned", c_text)
    c_text = re.sub(r"\b__int16\b", "int", c_text)
    c_text = re.sub(r"\bunsigned\s+__int32\b", "unsigned long", c_text)
    c_text = re.sub(r"\b__int32\b", "long", c_text)
    c_text = re.sub(r"\b(?:__thiscall|__usercall|__userpurge)\b", "", c_text)  # unknown conventions
    # the body signature: replace its convention with the DLL export decoration
    c_text = re.sub(r"(?m)^(\s*)([A-Za-z_][\w ]*?)\b(?:__fastcall|__cdecl|__stdcall|__pascal)?\s*(%s)\s*\("
                    % re.escape(fname), r"\1\2 __far __pascal __export \3(", c_text, count=1)
    called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", c_text))
    defined = set(re.findall(r"\b(%s)\s*\(" % re.escape(fname), c_text)) | {"LibMain", "WEP"}
    stubs = ["WORD __far __cdecl %s(){return 0;}" % s for s in sorted(called - defined - _C_KW)]
    pre = ["typedef unsigned WORD;", "typedef unsigned char BYTE;", "typedef unsigned long DWORD;",
           "typedef int BOOL;", "typedef long LONG;", "typedef unsigned UINT;",
           "typedef unsigned undefined;", "typedef unsigned char undefined1;",
           "typedef unsigned undefined2;", "typedef unsigned long undefined4;",
           "typedef unsigned HANDLE;", "typedef unsigned HWND;", "typedef char __far* LPSTR;",
           "typedef const char __far* LPCSTR;"]
    # FRAME RECOVERY: the NE lifter leaves 16-bit registers r20..r27 (ax..di) unresolved; back them
    # with a shared scratch buffer so register-relative accesses hit real memory (the oracle
    # fingerprint is address-independent), and declare any other undeclared identifiers as globals.
    used = set(re.findall(r"\b([A-Za-z_]\w*)\b", c_text))
    decl_types = re.findall(r"\b(?:WORD|BYTE|DWORD|int|unsigned|long|short|char|void)\s+\**([A-Za-z_]\w*)", c_text)
    rregs = sorted(g for g in used if re.fullmatch(r"r\d{1,2}", g))
    labels = set(re.findall(r"(?m)^\s*([A-Za-z_]\w*)\s*:(?!:)", c_text)) | set(re.findall(r"\bgoto\s+(\w+)", c_text))
    # STRUCT SYNTHESIS: the raise engine casts through `(struct V<n>_t *)x` and reads `->field`, but
    # leaves V<n>_t and its fields undefined. Declaring them as scalar globals (below) makes
    # `((struct V11_t*)v11)->field_3B2` fail E1031 (no such member). Instead emit a real struct for
    # each V<n>_t tag carrying every member accessed anywhere (union is safe -- Watcom only needs the
    # member to exist), and keep the tags + members OUT of the scalar-global set. Mirrors authsrc's
    # _merge_structs for the single-body synth path.
    struct_tags = sorted(set(re.findall(r"\bstruct\s+(V\d+_t)\b", c_text)))
    members = sorted(set(re.findall(r"(?:->|\.)\s*([A-Za-z_]\w*)", c_text)))
    struct_defs = []
    for tag in struct_tags:
        body = "".join("  WORD %s;\n" % m for m in members) or "  WORD _pad;\n"
        struct_defs.append("struct %s {\n%s};" % (tag, body))
    undecl = sorted(used - set(decl_types) - _C_KW - labels - set(rregs) - set(called)
                    - set(struct_tags) - set(members)
                    - {fname, "LibMain", "WEP", "result"})
    gdecl = ["WORD %s;" % g for g in undecl if not g.isdigit()] + ["WORD result;"]
    frame = []
    if rregs:
        frame = ["static unsigned char _frame[0x4000];"] + ["WORD %s;" % r for r in rregs]
    entry = ["int __far __pascal LibMain(unsigned h,unsigned ds,unsigned hp,char __far*cl){return 1;}",
             "int __far __pascal WEP(int n){return 1;}"]
    # init the frame registers at the top of the body
    if rregs:
        mm = _DEF_RE.search(c_text)
        if mm:
            br = c_text.index("{", mm.end() - 1)
            init = "".join("  %s=(WORD)_frame+0x2000;\n" % r for r in rregs)
            c_text = c_text[:br + 1] + "\n" + init + c_text[br + 1:]
    src = "\n".join(pre + struct_defs + gdecl + frame + stubs + entry) + "\n\n" + c_text + "\n"
    cf = os.path.join(outdir, fname + ".c")
    open(cf, "w", newline="\n").write(src)
    env = dict(os.environ)
    env["WATCOM"] = ow["root"]
    env["INCLUDE"] = ";".join(ow["inc"])
    env["WLINK_LNK"] = ow["wlink_lnk"]                        # so `system windows` resolves
    env["PATH"] = os.path.join(ow["root"], "binnt64") + os.pathsep + env.get("PATH", "")
    r = subprocess.run([ow["wcc"], "-bt=windows", "-mc", "-zq", "-w0", "-zastd=c99", fname + ".c"],
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
