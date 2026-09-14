#!/usr/bin/env python3
r"""bodyverify.py -- per-body compile-verify: the automated reconstruction primitive (step 2.3).

This is what replaces the hand-work. For ONE function it:
  1. SYNTHESISES a clean C body from the original's oracle behavior (behavior engine: interproc /
     env-provisioned), faithful-by-construction rather than statically guessed;
  2. COMPILES just that body into a tiny testable module (wcc386 + wlink, callees/globals stubbed);
  3. VERIFIES it against the ORIGINAL on the Unicorn oracle (verify95's codegen-independent
     fingerprint) under identical inputs and symmetric external-stubbing;
  4. HARVESTS it (mark the body verified) iff the reconstruction behaves identically.

Because verification is PER-BODY, it does not need a faithful whole-module build -- each function the
behavior engine can synthesise is verified and banked on its own. Run corpus-wide, this automatically
harvests every auto-achievable body and leaves a QUANTIFIED residue, instead of 1,218 modules of hand
reconstruction.

    wt --target win95 bodyverify <module> [--max N]      # synth+compile+verify each function
    wt --target win95 bodyverify <module> --va 0x1234    # one function
"""
import os
import re
import sys
import subprocess

from src.engines.behavior import tracer, interproc, envprov
from src.targets.win95 import verify95, origintriage as OT

_C_KW = {"if", "else", "while", "for", "do", "switch", "case", "return", "goto", "break",
         "continue", "sizeof", "void", "int", "char", "short", "long", "unsigned", "signed",
         "struct", "union", "enum", "const", "static", "default", "typedef"}


def _ow():
    from src.core import paths
    root = os.path.join(paths.repo_root(), "experiments", "premise-proof", "open-watcom-v2", "rel")
    return {"root": root, "wcc": os.path.join(root, "binnt64", "wcc386.exe"),
            "wlink": os.path.join(root, "binnt64", "wlink.exe"),
            "inc": [os.path.join(root, "h", "nt"), os.path.join(root, "h")]}


_BLOCK_CACHE = {}


def _static_body(module, va):
    """The native decompiler's C for the function at `va` -- clean+faithful for simple LEAF/compute
    functions the call-flow synthesiser skips. Keyed by RVA (va - imagebase)."""
    from src.targets.win95 import sharedruntime as SR, autobuild95 as A
    if module not in _BLOCK_CACHE:
        _BLOCK_CACHE[module] = SR._module_blocks(module)
    blocks = _BLOCK_CACHE[module]
    try:
        rva = va - A._imagebase(A.PE(module))
    except Exception:
        return None
    return blocks.get(rva) or blocks.get(va)


def synth(module, va, arity=4):
    """Clean C body for one function. Ladder: interproc (call-flow) -> env-provisioned (fault paths)
    -> static decompile (leaf/compute). Returns (fname, c_text) or (None, reason). The function is
    renamed to a stable fn_<va> so the compiled body exports a predictable symbol."""
    fname = "fn_%x" % va
    try:
        r = interproc.reconstruct(module, func=hex(va), arity=arity)
        if not r.get("reconstructed"):
            r = envprov.reconstruct(module, func=hex(va), arity=arity)
    except Exception:
        r = {"reconstructed": False}
    if r.get("reconstructed") and r.get("emitted_c"):
        return fname, r["emitted_c"]
    # static leaf/compute fallback: rename the block's own sub_<addr> (its definition name) to fname
    blk = _static_body(module, va)
    if blk:
        m = re.search(r"\bsub_[0-9a-fA-F]+\b", blk)      # first token = this function's own name
        if m:
            blk = blk.replace(m.group(0), fname)
        return fname, blk
    return None, "no synth (leaf skipped by call-flow; static block missing)"


def _stub_refs(c_text, fname):
    """Every called identifier that isn't the function itself, a keyword, or already defined -> a
    zero stub, so the single body compiles standalone."""
    called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", c_text))
    defined = set(re.findall(r"(?m)^\s*[A-Za-z_][\w \*]*\b(\w+)\s*\([^;{]*\)\s*\{", c_text))
    refs = called - defined - _C_KW - {fname}
    return refs


def compile_body(c_text, fname, outdir):
    """Compile a single synthesised body into a tiny DLL exporting `fname`. Returns dll path|None."""
    ow = _ow()
    if not os.path.exists(ow["wcc"]):
        return None, "OpenWatcom not built"
    os.makedirs(outdir, exist_ok=True)
    refs = _stub_refs(c_text, fname)
    pre = ["typedef unsigned u32;", "typedef unsigned uint;", "typedef unsigned long ulong;",
           "typedef unsigned short ushort;", "typedef unsigned char uchar;",
           "typedef unsigned char undefined1;", "typedef unsigned short undefined2;",
           "typedef unsigned undefined4;", "typedef unsigned undefined;", "typedef int bool;"]
    stubs = ["int %s(){return 0;}" % r for r in sorted(refs)]
    # force an int return type on the body (decompiler emits `return f();` tail-calls even in a
    # void function -> E1051; the oracle reads EAX regardless of the declared type, so int is safe).
    c_text = re.sub(r"\bvoid(\s+(?:__fastcall|__cdecl|__stdcall|__watcall)?\s*%s\s*\()"
                    % re.escape(fname), r"int\1", c_text)
    src = "\n".join(pre) + "\n" + "\n".join(stubs) + "\n" + c_text + "\n"
    cf = os.path.join(outdir, fname + ".c")
    open(cf, "w", newline="\n").write(src)
    env = dict(os.environ); env["WATCOM"] = ow["root"]; env["INCLUDE"] = ";".join(ow["inc"])
    r = subprocess.run([ow["wcc"], "-bt=nt", "-zq", "-w0", fname + ".c"], cwd=outdir, env=env,
                       capture_output=True, text=True, timeout=120)
    if r.returncode != 0:
        return None, "compile: " + (r.stdout + r.stderr).strip().split("\n")[-1][:80]
    obj = open(os.path.join(outdir, fname + ".obj"), "rb").read()
    m = re.search((r"_%s@\d+" % re.escape(fname)).encode(), obj) or re.search(
        (r"_%s\b" % re.escape(fname)).encode(), obj)
    internal = m.group(0).decode() if m else ("_" + fname)
    with open(os.path.join(outdir, "b.lnk"), "w", newline="\n") as f:
        f.write("system nt_dll\nname %s.dll\noption quiet\nfile %s.obj\nexport %s='%s'\n"
                % (fname, fname, fname, internal))
    subprocess.run([ow["wlink"], "@b.lnk"], cwd=outdir, env=env, capture_output=True, text=True)
    dll = os.path.join(outdir, fname + ".dll")
    return (dll if os.path.exists(dll) else None), ("ok" if os.path.exists(dll) else "link failed")


def verify_body(retail, va, arity=4, outdir=None):
    """Full primitive: synth -> compile -> oracle-verify vs the original body. `retail` is a PATH.
    Returns a verdict dict."""
    fname, c = synth(retail, va, arity)
    if not fname:
        return {"va": va, "verdict": "no-synth", "reason": c}
    outdir = outdir or os.path.join(os.path.dirname(__file__), "_bodyverify", "%x" % va)
    dll, clog = compile_body(c, fname, outdir)
    if not dll:
        return {"va": va, "verdict": "no-compile", "reason": clog, "c": c}
    # original: run the retail body; reconstructed: run the compiled body -- same inputs + stubs
    from src.targets.win95 import autobuild95 as A
    pb = A.PE(dll)
    baddr = pb.export_addrs().get(fname)
    bbase = A._imagebase(pb)
    vecs = verify95._vectors(arity, "pe")
    rs = [verify95._fp_pe(retail, va, v) for v in vecs]
    bs = [verify95._fp_pe(dll, bbase + baddr, v) for v in vecs] if baddr else [{"err": "noexport"}]
    return {"va": va, "verdict": verify95._body_verdict(rs, bs), "fname": fname}


def _retail(module):
    if os.path.sep in module or "/" in module:
        return module
    root = OT._corpus_root()
    base = module if "." in module else module + ".dll"
    for dp, _d, fs in os.walk(root):
        for x in fs:
            if x.lower() == base.lower():
                return os.path.join(dp, x)
    return module


def run(module, va=None, maxn=40):
    retail = _retail(module)                             # resolve to a PATH (tracer needs it directly)
    if va is not None:
        r = verify_body(retail, va)
        print("  0x%x  %-12s %s" % (va, r["verdict"], r.get("reason", "")))
        return
    fns = tracer.list_functions(retail)
    from collections import Counter
    c = Counter()
    for off, fva, size in fns[:maxn]:
        r = verify_body(retail, fva)
        c[r["verdict"]] += 1
        print("  0x%-8x %-12s %s" % (fva, r["verdict"], r.get("reason", "")[:60]))
    print("  --- " + "  ".join("%s=%d" % (k, v) for k, v in c.most_common()) + " ---")


def main(argv):
    pos = [x for x in argv if not x.startswith("--")]
    if not pos:
        print(__doc__); return
    va = int(argv[argv.index("--va") + 1], 0) if "--va" in argv else None
    maxn = int(argv[argv.index("--max") + 1]) if "--max" in argv else 40
    print("=== bodyverify %s (synth -> compile -> oracle-verify) ===" % pos[0])
    run(pos[0], va=va, maxn=maxn)


if __name__ == "__main__":
    main(sys.argv[1:])
