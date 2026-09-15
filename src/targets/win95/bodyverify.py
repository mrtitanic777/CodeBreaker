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


def _ondemand_block(module, rva):
    """Decompile ONE function on demand (win95pedec --at RVA) when the module-wide --module discovery
    never classified this tracer-found address as a function head. Returns the emitted source (struct
    defs + the function, so struct-using bodies still compile), or None. PE lane only."""
    lane, exe = OT._detect(module)
    if lane != "pe":
        return None
    from src.core import paths
    dec = os.path.join(paths.repo_root(), "native", "decompiler", "build", "bin", exe)
    if not os.path.exists(dec):
        return None
    try:
        r = subprocess.run([dec, module, "--at", "%x" % rva], capture_output=True, text=True,
                           timeout=120)
    except Exception:
        return None
    text = (r.stdout or "").strip()
    return text if _DEF_RE.search(text) else None          # only if a real function was emitted


_DEF_RE = re.compile(r"(?m)^\s*[A-Za-z_][\w \*]*?\b(?:__fastcall|__cdecl|__stdcall|__watcall)?\s*"
                     r"([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{")


def _defined_name(c):
    m = _DEF_RE.search(c)
    return m.group(1) if m else None


def synth(module, va, arity=4):
    """Clean C body for one function. Ladder: interproc (call-flow) -> env-provisioned (fault paths)
    -> static decompile (leaf/compute). Returns (fname, c_text) or (None, reason), where fname is the
    ACTUAL function name defined in the emitted C (so the compiled body exports the right symbol)."""
    try:
        r = interproc.reconstruct(module, func=hex(va), arity=arity)
        if not r.get("reconstructed"):
            r = envprov.reconstruct(module, func=hex(va), arity=arity)
    except Exception:
        r = {"reconstructed": False}
    if r.get("reconstructed") and r.get("emitted_c"):
        c = _declare_implicits(_beautify(r["emitted_c"]))
        return (_defined_name(c) or "fn_%x" % va), c
    # static leaf/compute fallback: clean the decompiler block into compilable C, KEEPING the
    # signature (so stdcall args still flow to the oracle). Reject blocks with idioms that can't be
    # made statically faithful -- they belong to the behavior engine's dynamic lane, not here.
    blk = _static_body(module, va)
    if not blk:                                          # module-wide discovery missed it: decompile
        try:                                             # this one address on demand (win95pedec --at)
            from src.targets.win95 import autobuild95 as A
            blk = _ondemand_block(module, va - A._imagebase(A.PE(module)))
        except Exception:
            blk = None
    if blk:
        cured = _cure_body(_beautify(blk))
        if cured is None:
            return None, "static block not statically faithful (indirect/leaked-reg/CONCAT/in_)"
        return (_defined_name(cured) or "fn_%x" % va), cured
    return None, "no synth (leaf skipped by call-flow; static + on-demand decompile empty)"


_PROTOS = None


def _beautify(c):
    """Real API names (KERNEL32_LocalAlloc->LocalAlloc) + real Win32 types -- so the harvested body
    is BEAUTIFUL and compiles against the typedef prelude. Evidence-based (import table + SDK)."""
    global _PROTOS
    try:
        from src.targets.win95 import beautify95
        if _PROTOS is None:
            _PROTOS = beautify95._load_protos()
        return beautify95.beautify_text(c, _PROTOS)[0]
    except Exception:
        return c


# decompiler abstractions a static body can't be made faithful with (unresolved indirect call,
# packed-value pseudo-ops, in/unaff inputs). Leaked registers (rN) are NO LONGER rejected -- they are
# frame-backed in compile_body -- so the oracle gets to judge them.
_UNRESOLVABLE = ("indirect(", "CONCAT", "SUB84", "ZEXT", "SEXT", "in_", "unaff_", "extraout_")
_IMPLICIT = re.compile(r"\b(result|eax|ebx|ecx|edx|esi|edi)\b")


def _cure_body(blk):
    """Param-preserving cure of one decompiler function block -> compilable C, or None if it uses an
    abstraction that cannot be made statically faithful (punt to the behavior engine)."""
    if any(u in blk for u in _UNRESOLVABLE):
        return None
    from src.targets.win95 import raise95
    return _declare_implicits(raise95.map_types(blk))     # undefined4->DWORD, uint->unsigned, ...


_TYPES = (r"(?:unsigned|signed|int|long|short|char|void|DWORD|WORD|BYTE|LONG|UINT|float|double|"
          r"HDC|HGDIOBJ|HANDLE|struct\s+\w+)")


def _declare_implicits(c):
    """Declare the implicit result/register pseudo-vars the emitters can leave undeclared, as locals
    at the top of the function body (so the body compiles without stripping its signature/params)."""
    m = _DEF_RE.search(c)                                 # the FUNCTION header (skip any leading struct)
    if not m:
        return c
    brace = m.end() - 1 if c[m.end() - 1] == "{" else c.index("{", m.end() - 1)
    implicit = [n for n in sorted(set(_IMPLICIT.findall(c)))
                if not re.search(r"\b%s\s+\**%s\b" % (_TYPES, re.escape(n)), c)]
    decl = "".join("  unsigned %s;\n" % r for r in implicit)
    return c[:brace + 1] + "\n" + decl + c[brace + 1:]


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
    # strip decompiler artifacts that look like preprocessor directives ('#350' from switch/line info)
    c_text = re.sub(r"(?m)^\s*#\d.*$", "", c_text)
    # MSVC sized-int keywords the decompiler emits (unsigned __int16 ...) aren't OpenWatcom types and
    # derail the parse (a bad declaration closes the function scope -> E1099 on the next statement).
    # Map them to standard C types, longest first, unsigned-qualified forms before the bare ones.
    for a, b in ((r"unsigned\s+__int64", "unsigned long long"), (r"__int64", "long long"),
                 (r"unsigned\s+__int32", "unsigned int"), (r"__int32", "int"),
                 (r"unsigned\s+__int16", "unsigned short"), (r"__int16", "short"),
                 (r"unsigned\s+__int8", "unsigned char"), (r"__int8", "char")):
        c_text = re.sub(r"\b" + a + r"\b", b, c_text)
    # calling conventions wcc386 doesn't accept (IDA/decompiler-isms): __thiscall -> __stdcall (both
    # put args on the stack, matching the oracle's stdcall frame), and strip __usercall/__userpurge.
    c_text = re.sub(r"\b__thiscall\b", "__stdcall", c_text)
    c_text = re.sub(r"\b__(?:usercall|userpurge|noreturn|far|near|huge)\b", "", c_text)
    # struct field access on incompletely-inferred types (E1031: 'field_2 not in V1_t'). The decompiler
    # names members field_<hexoffset>, so drop the (partial) struct defs and rewrite every field access
    # to a direct dword access at that byte offset -- faithful for dword fields, and always compiles.
    #  a) cast form: ((struct VN_t *)ident)->field_X  -> *(unsigned*)((uchar*)(ident)+0xX)
    c_text = re.sub(r"\(\(struct\s+V\d+_t\s*\*\)\s*([A-Za-z_]\w*)\s*\)\s*->\s*field_([0-9a-fA-F]+)",
                    r"*(unsigned *)((unsigned char*)(\1)+0x\2)", c_text)
    #  b) drop the struct definitions (now largely unreferenced)
    c_text = re.sub(r"(?s)struct\s+V\d+_t\s*\{.*?\}\s*;", "", c_text)
    #  c) any remaining struct-pointer/value type uses -> plain machine words
    c_text = re.sub(r"\bstruct\s+V\d+_t\s*\*", "unsigned char *", c_text)
    c_text = re.sub(r"\bstruct\s+V\d+_t\b", "unsigned", c_text)
    #  d) any remaining `ident->field_X` (now on an untyped pointer) -> the same offset access
    c_text = re.sub(r"([A-Za-z_]\w*)\s*->\s*field_([0-9a-fA-F]+)",
                    r"*(unsigned *)((unsigned char*)(\1)+0x\2)", c_text)
    refs = _stub_refs(c_text, fname)                      # CALLED names -> function stubs
    pre = ["typedef unsigned u32;", "typedef unsigned uint;", "typedef unsigned long ulong;",
           "typedef unsigned short ushort;", "typedef unsigned char uchar;",
           "typedef unsigned char undefined1;", "typedef unsigned short undefined2;",
           "typedef unsigned undefined4;", "typedef unsigned undefined;", "typedef int bool;",
           # Win32 types so beautify's real-typed signatures compile standalone (no windows.h needed;
           # external API calls are stubbed, so these only need to be size-compatible machine words).
           "typedef unsigned long DWORD;typedef unsigned short WORD;typedef unsigned char BYTE;",
           "typedef long LONG;typedef unsigned UINT;typedef int BOOL;typedef int INT;",
           "typedef void*LPVOID;typedef char*LPSTR;typedef const char*LPCSTR;typedef unsigned*LPDWORD;",
           "typedef void*HANDLE;typedef void*HDC;typedef void*HWND;typedef void*HGDIOBJ;",
           "typedef void*HGLOBAL;typedef void*HLOCAL;typedef void*HINSTANCE;typedef unsigned MMRESULT;",
           "typedef void*HMODULE;typedef unsigned HRESULT;typedef unsigned WPARAM;typedef long LPARAM;",
           "typedef unsigned short*LPWSTR;typedef const unsigned short*LPCWSTR;typedef unsigned*PUINT;"]
    stubs = ["int %s(){return 0;}" % r for r in sorted(refs)]
    # declare every identifier the body USES but does not declare (undeclared globals data_XXXX /
    # DAT_ / locals the decompiler left implicit on some path) as a file-scope global -> kills the
    # E1011/E1147 cascade. Excludes callees (stubbed above), params, keywords/types, and the fn name.
    used = set(re.findall(r"\b([A-Za-z_]\w*)\b", c_text))
    declared = set(re.findall(r"\b(?:unsigned|signed|int|long|short|char|void|float|double|struct\s+\w+|"
                              r"DWORD|WORD|BYTE|LONG|UINT|HDC|HANDLE|HGDIOBJ)\s+\**([A-Za-z_]\w*)", c_text))
    dm = _DEF_RE.search(c_text)
    params = set()
    if dm:
        sig = c_text[dm.start():dm.end()]
        params = set(re.findall(r"\b([A-Za-z_]\w*)\s*[,)]", sig))
    _TYPEWORDS = {"unsigned", "signed", "int", "long", "short", "char", "void", "float", "double",
                  "struct", "union", "enum", "const", "static", "return", "if", "else", "while",
                  "for", "do", "switch", "case", "goto", "break", "continue", "sizeof", "default",
                  "u32", "uint", "ulong", "ushort", "uchar", "bool", "undefined", "undefined1",
                  "undefined2", "undefined4", "DWORD", "WORD", "BYTE", "LONG", "UINT", "HDC",
                  "HANDLE", "HGDIOBJ", "__fastcall", "__cdecl", "__stdcall", "__watcall"}
    # goto labels are NOT variables: `goto LABEL_x;` + `LABEL_x:` share C's label namespace, so
    # declaring them as file-scope globals is wrong (and clutters/confuses the scope).
    labels = set(re.findall(r"(?m)^\s*([A-Za-z_]\w*)\s*:(?!:)", c_text))
    labels |= set(re.findall(r"\bgoto\s+([A-Za-z_]\w*)", c_text))
    undecl = sorted(used - refs - declared - params - _TYPEWORDS - labels - {fname})
    # FRAME RECOVERY: leaked physical registers (rN = EBP/ESP/... the lifter left unresolved) are
    # backed by a shared valid scratch buffer, initialised at function entry, so frame accesses
    # (rN +/- off, and LEA-of-local passed to APIs) hit REAL memory and the body runs its true
    # control flow instead of dereferencing a null. The oracle fingerprint is address-independent
    # (call sequence + ret + exit, not pointer values), so this resolves frame-only divergences.
    rregs = sorted(g for g in undecl if re.fullmatch(r"r\d{1,2}", g))
    other = [g for g in undecl if not g.isdigit() and g not in rregs]
    gdecl = ["unsigned %s;" % g for g in other]
    if rregs:
        gdecl.append("static unsigned char _frame[0x8000];")
        gdecl += ["unsigned %s;" % r for r in rregs]
        init = "".join("  %s=(unsigned)(_frame+0x4000);\n" % r for r in rregs)
        dm2 = _DEF_RE.search(c_text)
        if dm2:
            br = dm2.end() - 1 if c_text[dm2.end() - 1] == "{" else c_text.index("{", dm2.end() - 1)
            c_text = c_text[:br + 1] + "\n" + init + c_text[br + 1:]
    # force an int return type on the body (decompiler emits `return f();` tail-calls even in a
    # void function -> E1051; the oracle reads EAX regardless of the declared type, so int is safe).
    c_text = re.sub(r"\bvoid(\s+(?:__fastcall|__cdecl|__stdcall|__watcall)?\s*%s\s*\()"
                    % re.escape(fname), r"int\1", c_text)
    src = "\n".join(pre + stubs + gdecl) + "\n" + c_text + "\n"
    cf = os.path.join(outdir, fname + ".c")
    open(cf, "w", newline="\n").write(src)
    env = dict(os.environ)
    env["WATCOM"] = ow["root"]
    env["INCLUDE"] = ";".join(ow["inc"])
    env["PATH"] = os.path.join(ow["root"], "binnt64") + os.pathsep + env.get("PATH", "")
    # -s: no stack-overflow probe (drops the __CHK reference so the body links self-contained).
    # -zastd=c99: allow declarations to follow statements (decompiler bodies + our frame-init prologue
    # mix decl/stmt freely; C89 rejects that as E1099 "statement must be inside function").
    r = subprocess.run([ow["wcc"], "-bt=nt", "-zq", "-w0", "-s", "-zastd=c99", fname + ".c"],
                       cwd=outdir, env=env, capture_output=True, text=True, timeout=120)
    if r.returncode != 0:
        return None, "compile: " + (r.stdout + r.stderr).strip().split("\n")[-1][:80]
    from src.targets.win95 import autobuild95 as A
    obj = open(os.path.join(outdir, fname + ".obj"), "rb").read()
    # exact public symbol from the OMF PUBDEF records (handles _f@N stdcall / f_ register / _f)
    pubs = A._omf_publics(obj)
    # normalise any convention decoration to the bare name: leading _ (cdecl) or @ (fastcall),
    # trailing _ (watcall), and an @N stdcall/fastcall byte-count suffix -- so the body's own public
    # (e.g. __fastcall '@sub_x@0', __stdcall '_sub_x@N', __watcall 'sub_x_') always resolves.
    def _base(s):
        return s.lstrip("_@").split("@")[0].rstrip("_")
    cand = [s for s in pubs if _base(s) == fname]
    internal = cand[0] if cand else ("_" + fname)
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
    verdict = verify95._body_verdict(rs, bs)
    if verdict == "equivalent":
        _save_verified(retail, fname, c)                 # bank the beautiful, verified source
    return {"va": va, "verdict": verdict, "fname": fname}


def _save_verified(retail, fname, c):
    """Write a verified body's beautiful source into the recovered tree, tagged oracle-verified."""
    from src.core import paths
    modbase = os.path.splitext(os.path.basename(retail))[0]
    d = os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design", "recovered",
                     "verified", modbase)
    try:
        os.makedirs(d, exist_ok=True)
        open(os.path.join(d, fname + ".c"), "w", newline="\n").write(
            "/* %s -- ORACLE-VERIFIED reconstruction (behaves identically to retail %s on the\n"
            " * Unicorn oracle). Names/types recovered from the import/export tables + SDK/DDK;\n"
            " * internal callees (sub_) and locals (vN) are inferred, not claimed as recovered. */\n\n"
            % (fname, os.path.basename(retail)) + c + "\n")
    except OSError:
        pass


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


def _ledger_path():
    from src.core import paths
    return os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design", "body_ledger.csv")


def _write_rows(rows):
    """Atomically rewrite the ledger: write a sibling temp file, then os.replace() it into place. A
    crash (or a concurrent reader) can never see a half-written/truncated ledger this way."""
    import csv as _csv
    path = _ledger_path()
    tmp = path + ".tmp.%d" % os.getpid()
    with open(tmp, "w", newline="") as fp:
        w = _csv.DictWriter(fp, fieldnames=rows[0].keys())
        w.writeheader(); w.writerows(rows)
    os.replace(tmp, path)                                 # atomic on Windows + POSIX


def harvest(module, maxn=100000, rows=None, present=None, verified_now=None):
    """Verify every function in `module`, driving off the TRIAGE function list so each verified body
    maps 1:1 to its ledger body_id (aligned crediting). Marks equivalent bodies verified. If `rows`
    is passed it credits into that shared list (for a corpus sweep) and the caller writes the ledger;
    otherwise it writes the ledger itself. Returns counts + credited body_ids."""
    import csv as _csv
    from collections import Counter
    from src.targets.win95 import autobuild95 as A
    retail = _retail(module)
    own = rows is None
    if own:
        rows = list(_csv.DictReader(open(_ledger_path())))
        present = {r["body_id"] for r in rows}
    verified_now = set() if verified_now is None else verified_now
    already = {r["body_id"] for r in rows if r.get("verified") == "1"}
    try:
        base = A._imagebase(A.PE(retail))
    except Exception:
        base = 0
    # trace off the TRACER's function starts (accurate traces), but CREDIT via the triage body_id
    # that CONTAINS each verified address -- the two enumerators' boundaries differ, so match by
    # containment, not exact equality.
    _lane, tfns = OT.triage_module(retail)
    tri = sorted((f["addr"], f["sh"][:16]) for f in (tfns or [])
                 if f.get("addr") is not None and f.get("sh"))

    def _bid_for(rva):
        lo = None
        for a, sh in tri:
            if a == rva:
                return sh
            if a <= rva:
                lo = sh
            else:
                break
        return lo                                        # enclosing triage function's body id
    c = Counter()
    allfns = tracer.list_functions(retail)
    for off, va, size in allfns[:maxn]:
        bid = _bid_for(va - base)
        if not bid or bid not in present or bid in already or bid in verified_now:
            continue
        r = verify_body(retail, va)
        c[r["verdict"]] += 1
        if r["verdict"] == "equivalent":
            verified_now.add(bid)
    if own:
        for r in rows:
            if r["body_id"] in verified_now:
                r["verified"], r["state"] = "1", "verified"
        _write_rows(rows)
    # exhausted = we saw the WHOLE function list (not cut off by the cap): re-running with the same
    # engine would re-try the identical non-verified bodies for the same verdicts, so it's safe to
    # skip this module next run (clear verify_swept.txt whenever synth/cure/lifter improves).
    return {"module": module, "counts": dict(c), "credited": len(verified_now),
            "exhausted": len(allfns) <= maxn}


def _swept_path():
    return os.path.join(os.path.dirname(_ledger_path()), "verify_swept.txt")


# ---------------------------------------------------------------------------
# Parallel sweep: workers verify whole modules concurrently and RETURN their verified body-ids to
# the parent, which is the SOLE ledger writer (single-threaded, atomic) -- so parallelism gives a
# ~core-count speedup with zero ledger-contention (the corruption that killed the earlier detached
# attempt). Module-level parallelism reuses each module's cached decompile inside its worker.
# ---------------------------------------------------------------------------
_W = {}


def _winit(target_dir, present, already):
    from src.core import cores
    cores.set_target(target_dir)
    _W["present"] = present
    _W["already"] = already


def _wjob(args):
    """Worker: verify every function in one module; return (module, [verified body_ids], exhausted,
    counts). No ledger access -- the parent credits and writes."""
    module, maxn = args
    from collections import Counter
    from src.targets.win95 import autobuild95 as A
    present, already = _W["present"], _W["already"]
    try:
        retail = _retail(module)
        try:
            base = A._imagebase(A.PE(retail))
        except Exception:
            base = 0
        _lane, tfns = OT.triage_module(retail)
        tri = sorted((f["addr"], f["sh"][:16]) for f in (tfns or [])
                     if f.get("addr") is not None and f.get("sh"))

        def _bid_for(rva):
            lo = None
            for a, sh in tri:
                if a == rva:
                    return sh
                if a <= rva:
                    lo = sh
                else:
                    break
            return lo
        c = Counter(); got = set()
        allfns = tracer.list_functions(retail)
        for off, va, size in allfns[:maxn]:
            bid = _bid_for(va - base)
            if not bid or bid not in present or bid in already or bid in got:
                continue
            outdir = os.path.join(os.path.dirname(__file__), "_bodyverify",
                                  "w%d" % os.getpid(), "%x" % va)
            r = verify_body(retail, va, outdir=outdir)
            c[r["verdict"]] += 1
            if r["verdict"] == "equivalent":
                got.add(bid)
        return (module, sorted(got), len(allfns) <= maxn, dict(c))
    except Exception as e:
        return (module, [], False, {"error": type(e).__name__})


def sweep_parallel(modules, workers=10, maxper=100000, skip_swept=True):
    """Corpus sweep across `workers` processes. The parent is the only ledger writer; it checkpoints
    atomically as workers return, so a stop at any point keeps every credited body."""
    import csv as _csv
    import multiprocessing as mp
    from src.core import paths
    rows = list(_csv.DictReader(open(_ledger_path())))
    present = {r["body_id"] for r in rows}
    already = {r["body_id"] for r in rows if r.get("verified") == "1"}
    swept = _load_swept() if skip_swept else set()
    if skip_swept:
        modules = [m for m in modules if os.path.basename(m).lower() not in swept]
    verified_now = set(); newly_swept = []; done = 0

    def _flush():
        for r in rows:
            if r["body_id"] in verified_now:
                r["verified"], r["state"] = "1", "verified"
        _write_rows(rows)
        if newly_swept:
            with open(_swept_path(), "a") as fp:
                fp.write("".join(s + "\n" for s in newly_swept))

    tdir = paths.target_dir("win95")
    jobs = [(m, maxper) for m in modules]
    with mp.Pool(workers, initializer=_winit, initargs=(tdir, present, already)) as pool:
        for module, ids, exhausted, counts in pool.imap_unordered(_wjob, jobs):
            done += 1
            for bid in ids:
                verified_now.add(bid)
            if skip_swept and exhausted:
                newly_swept.append(os.path.basename(module).lower())
            if done % max(1, workers) == 0 or ids:       # checkpoint often (cheap, atomic)
                _flush(); newly_swept = []
            print("  [%d/%d] %-16s +%d  %s" % (done, len(jobs), os.path.basename(module),
                  len(ids), "  ".join("%s=%d" % kv for kv in sorted(counts.items()))), flush=True)
    _flush()
    return {"credited": len(verified_now), "skipped": len(swept),
            "total_verified": sum(1 for r in rows if r.get("verified") == "1")}


def _load_swept():
    try:
        return set(l.strip().lower() for l in open(_swept_path()) if l.strip())
    except OSError:
        return set()


def sweep_corpus(modules, maxper=100000, skip_swept=False):
    """Run harvest across several modules, crediting into one shared ledger write. Resumable: already
    -verified bodies skip; with skip_swept, whole modules already processed are skipped so each run
    advances to NEW modules instead of re-decompiling finished ones."""
    import csv as _csv
    rows = list(_csv.DictReader(open(_ledger_path())))
    present = {r["body_id"] for r in rows}
    verified_now = set()
    per = []
    swept = _load_swept() if skip_swept else set()
    if skip_swept:
        modules = [m for m in modules if os.path.basename(m).lower() not in swept]

    def _flush():                                        # persist after each module so a timeout
        for r in rows:                                   # never loses completed modules' credits
            if r["body_id"] in verified_now:
                r["verified"], r["state"] = "1", "verified"
        _write_rows(rows)                                 # atomic (temp + os.replace)

    newly_swept = []
    for m in modules:
        try:
            r = harvest(m, maxn=maxper, rows=rows, present=present, verified_now=verified_now)
            per.append((m, r["counts"]))
            if skip_swept and r.get("exhausted"):
                newly_swept.append(os.path.basename(m).lower())
        except Exception as e:
            per.append((m, {"error": type(e).__name__}))
        _flush()                                         # incremental checkpoint
        if newly_swept:                                  # persist swept list incrementally too
            with open(_swept_path(), "a") as fp:
                fp.write("".join(s + "\n" for s in newly_swept))
            swept.update(newly_swept); newly_swept = []
    return {"modules": per, "credited": len(verified_now), "skipped": len(swept),
            "total_verified": sum(1 for r in rows if r.get("verified") == "1")}


_BOOT_CRITICAL = ["gdi32.dll", "user32.dll", "kernel32.dll", "advapi32.dll", "shell32.dll",
                  "comdlg32.dll", "comctl32.dll", "winmm.dll", "version.dll", "lz32.dll"]


def main(argv):
    # positional args = module names; exclude flags AND the values that follow --max/--va
    _valued = {"--max", "--va", "--workers"}
    pos, skip = [], False
    for i, x in enumerate(argv):
        if skip:
            skip = False; continue
        if x in _valued:
            skip = True; continue
        if x.startswith("--"):
            continue
        pos.append(x)
    maxn = int(argv[argv.index("--max") + 1]) if "--max" in argv else 40
    workers = int(argv[argv.index("--workers") + 1]) if "--workers" in argv else 10
    if "--broad" in argv:                                # every PE corpus module (resumable sweep)
        root = OT._corpus_root()
        seen, mods = set(), []
        for dp, _d, fs in os.walk(root):
            for fn in fs:
                if fn.lower().endswith((".dll", ".exe", ".drv", ".ocx", ".cpl")) and fn.lower() not in seen:
                    p = os.path.join(dp, fn)
                    if OT._detect(p)[0] == "pe":
                        seen.add(fn.lower()); mods.append(p)
        mp = maxn if "--max" in argv else 100000
        if "--parallel" in argv:                         # N worker processes (default 10 of 12 cores)
            print("=== bodyverify --broad --parallel (%d PE modules, %d workers) ===" % (len(mods), workers), flush=True)
            r = sweep_parallel(mods, workers=workers, maxper=mp, skip_swept=True)
        else:
            r = sweep_corpus(mods, maxper=mp, skip_swept=True)
            print("=== bodyverify --broad (%d PE modules) ===" % len(mods))
        print("  bodies newly credited: %d" % r["credited"])
        print("  modules skipped (swept): %d" % r.get("skipped", 0))
        print("  TOTAL VERIFIED:        %d" % r["total_verified"])
        return
    if "--sweep" in argv:
        mods = pos or _BOOT_CRITICAL
        r = sweep_corpus(mods, maxper=maxn if "--max" in argv else 100000)
        print("=== bodyverify --sweep (automatic harvest) ===")
        for m, cts in r["modules"]:
            print("  %-14s %s" % (m, "  ".join("%s=%d" % (k, v) for k, v in sorted(cts.items()))))
        print("  bodies newly credited this sweep: %d" % r["credited"])
        print("  TOTAL VERIFIED in ledger:         %d" % r["total_verified"])
        return
    if not pos:
        print(__doc__); return
    if "--harvest" in argv:
        r = harvest(pos[0], maxn=maxn)
        print("=== bodyverify --harvest %s ===" % pos[0])
        print("  " + "  ".join("%s=%d" % (k, v) for k, v in sorted(r["counts"].items())))
        print("  bodies credited VERIFIED in ledger: %d" % r["credited"])
        return
    va = int(argv[argv.index("--va") + 1], 0) if "--va" in argv else None
    print("=== bodyverify %s (synth -> compile -> oracle-verify) ===" % pos[0])
    run(pos[0], va=va, maxn=maxn)


if __name__ == "__main__":
    main(sys.argv[1:])
