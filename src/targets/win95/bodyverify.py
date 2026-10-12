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

from src.core import corpus, toolchain
from src.engines.behavior import tracer, interproc, envprov
from src.targets.win95 import verify95, origintriage as OT

_C_KW = {"if", "else", "while", "for", "do", "switch", "case", "return", "goto", "break",
         "continue", "sizeof", "void", "int", "char", "short", "long", "unsigned", "signed",
         "struct", "union", "enum", "const", "static", "default", "typedef"}

def _ow():
    return {"root": toolchain.openwatcom_root(), "wcc": toolchain.ow_cc("pe"),
            "wlink": toolchain.ow_tool("wlink"), "inc": toolchain.ow_include("pe")}


def _scratch(*parts):
    """Build scratch lives in the temp dir, not beside the source (it grew to many thousands of
    files under src/targets/win95 and slowed every search of the repo)."""
    import tempfile
    return os.path.join(tempfile.gettempdir(), "codebreaker", "bodyverify", *parts)

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
    # CB_DECOMPILER_BIN pins a specific decompiler build dir (e.g. a frozen pre-change copy) so an
    # A/B verdict sweep can compare two decompiler versions on the same oracle. Defaults to build/bin.
    dec = toolchain.decompiler(exe)
    if not dec:
        return None
    # Pass the function end so --at skips its ~1.5s whole-module discovery (whose only use is to find
    # this boundary). The next discovered start above `rva` is a key of the cached --module decompile
    # (_BLOCK_CACHE) -- identical to what discovery would compute. Best-effort: omit on any doubt so
    # --at falls back to its own discovery (unchanged behaviour).
    cmd = [dec, module, "--at", "%x" % rva]
    try:
        from src.targets.win95 import sharedruntime as SR
        if module not in _BLOCK_CACHE:
            _BLOCK_CACHE[module] = SR._module_blocks(module)
        nexts = [k for k in _BLOCK_CACHE[module] if isinstance(k, int) and k > rva]
        if nexts:
            cmd.append("%x" % min(nexts))
    except Exception:
        pass
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
    except Exception:
        return None
    text = (r.stdout or "").strip()
    return text if _DEF_RE.search(text) else None          # only if a real function was emitted

_DEF_RE = re.compile(r"(?m)^\s*[A-Za-z_][\w \*]*?\b(?:__fastcall|__cdecl|__stdcall|__watcall)?\s*"
                     r"([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{")

def _defined_name(c):
    m = _DEF_RE.search(c)
    return m.group(1) if m else None

def synth(module, va, arity=4, fast=False):
    """Clean C body for one function. Ladder: interproc (call-flow) -> env-provisioned (fault paths)
    -> static decompile (leaf/compute). Returns (fname, c_text) or (None, reason), where fname is the
    ACTUAL function name defined in the emitted C (so the compiled body exports the right symbol).
    `fast` skips the interproc/envprov oracle traces (the per-body bottleneck) and takes the static
    decompile for EVERY body, not just leaves -- for the batched grind, where the batch links real
    intra-module callees anyway, so caller behaviour is still exercised without the expensive traces."""
    # FAST PATH for LEAF functions (no calls): interproc/envprov reconstruct the call SEQUENCE of
    # orchestrator functions by running oracle traces (~200ms+ for the two attempts). A leaf makes no
    # calls, so they have nothing to reconstruct and fall through to the static decompile anyway -- the
    # static decompile IS the faithful reconstruction for a leaf. Serve it directly from the cached
    # whole-module decompile (_BLOCK_CACHE), skipping the wasted traces. Only when the static block is
    # present, cures cleanly, and calls nothing (no sub_/API refs) -- any caller still takes the ladder.
    verify95._arm()
    blk0 = _static_body(module, va)
    if blk0:
        cured0 = _cure_body(_beautify(blk0))
        if cured0 is not None:
            fn0 = _defined_name(cured0) or "fn_%x" % va
            if fast or not _stub_refs(cured0, fn0):      # fast: any body; else only a genuine leaf
                return fn0, cured0
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

def _callee_body(module, rva):
    """Faithful EXACT decompile of one internal callee at `rva` (win95pedec --at). Used for closure
    verification instead of interproc, which TRACE-reconstructs and (a) cannot model a function reached
    only by tail-jmp -- the trace jumps to the fake return frame -- and (b) snaps a non-entry address
    to the enclosing discovered function (reconstructing the wrong body entirely). The static decompile
    at the exact address has neither problem. Returns cured C or None."""
    blk = _ondemand_block(module, rva)
    if not blk:
        return None
    cured = _cure_body(_beautify(blk))
    if cured:
        # A caller may pass a register argument (a stdcall-cleanup count, a dispatch index) that the
        # callee reads but the decompiler did not promote to a parameter, leaving it `name(void)`.
        # Relax that one signature to K&R `name()` (accepts and ignores extra args) so the caller's
        # call compiles; the ignored register never changes the callee's observable behaviour.
        cured = re.sub(r"\(\s*void\s*\)\s*\{", "() {", cured, count=1)
    return cured

def synth_closure(module, va, arity=4, depth=2, _seen=None, _base=None, _top=True):
    """Like synth, but for the TOP function's internal callees (sub_XXXX) it appends their REAL
    decompiled bodies (recursively, to `depth`) instead of leaving them as compile_body's zero stubs.
    This makes the reconstructed call graph -- not a stub returning 0 -- face the oracle, so a function
    whose behaviour flows through a callee is judged on the real reconstruction. Returns (fname, c)."""
    if _seen is None:
        _seen = set()
    if _base is None:
        from src.targets.win95 import autobuild95 as A
        _base = A._imagebase(A.PE(module))
    if _top:
        fname, cc = synth(module, va, arity)
    else:
        cc = _callee_body(module, va - _base)
        fname = _defined_name(cc) if cc else None
    if not fname:
        return None, cc
    parts = [cc]
    if depth > 0:
        for h in sorted(set(re.findall(r"\bsub_([0-9a-fA-F]+)\b", cc))):
            nm = "sub_" + h
            if nm in _seen:
                continue
            _seen.add(nm)
            try:
                _, sub = synth_closure(module, _base + int(h, 16), 4, depth - 1, _seen, _base, False)
                if sub:
                    parts.append(sub)
            except Exception:
                pass
    return fname, "\n".join(parts)

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

_PRE = ["typedef unsigned u32;", "typedef unsigned uint;", "typedef unsigned long ulong;",
        "typedef unsigned short ushort;", "typedef unsigned char uchar;",
        "typedef unsigned char undefined1;", "typedef unsigned short undefined2;",
        "typedef unsigned undefined4;", "typedef unsigned undefined;", "typedef int bool;",
        "typedef unsigned long DWORD;typedef unsigned short WORD;typedef unsigned char BYTE;",
        "typedef long LONG;typedef unsigned UINT;typedef int BOOL;typedef int INT;",
        "typedef void*LPVOID;typedef char*LPSTR;typedef const char*LPCSTR;typedef unsigned*LPDWORD;",
        "typedef void*HANDLE;typedef void*HDC;typedef void*HWND;typedef void*HGDIOBJ;",
        "typedef void*HGLOBAL;typedef void*HLOCAL;typedef void*HINSTANCE;typedef unsigned MMRESULT;",
        "typedef void*HMODULE;typedef unsigned HRESULT;typedef unsigned WPARAM;typedef long LPARAM;",
        "typedef unsigned short*LPWSTR;typedef const unsigned short*LPCWSTR;typedef unsigned*PUINT;"]

def _prep_body(c_text, fname):
    """Transform ONE synthesised body into compilable C and compute its external surface. Returns
    (c_text, refs, other_globals, rregs): the prepared source (with frame-init + int-return applied),
    the called names needing stubs, the undeclared globals it uses, and the leaked physical registers
    it needs frame-backed. The caller assembles typedefs + stubs + shared frame + globals + bodies --
    so compile_body (one) and batch_compile (many) share identical preparation."""
    # strip decompiler artifacts that look like preprocessor directives ('#350' from switch/line info)
    c_text = re.sub(r"(?m)^\s*#\d.*$", "", c_text)
    for a, b in ((r"unsigned\s+__int64", "unsigned long long"), (r"__int64", "long long"),
                 (r"unsigned\s+__int32", "unsigned int"), (r"__int32", "int"),
                 (r"unsigned\s+__int16", "unsigned short"), (r"__int16", "short"),
                 (r"unsigned\s+__int8", "unsigned char"), (r"__int8", "char")):
        c_text = re.sub(r"\b" + a + r"\b", b, c_text)
    c_text = re.sub(r"\b__thiscall\b", "__stdcall", c_text)
    c_text = re.sub(r"\b__(?:usercall|userpurge|noreturn|far|near|huge)\b", "", c_text)
    c_text = re.sub(r"\(\(struct\s+V\d+_t\s*\*\)\s*([A-Za-z_]\w*)\s*\)\s*->\s*field_([0-9a-fA-F]+)",
                    r"*(unsigned *)((unsigned char*)(\1)+0x\2)", c_text)
    c_text = re.sub(r"(?s)struct\s+V\d+_t\s*\{.*?\}\s*;", "", c_text)
    c_text = re.sub(r"\bstruct\s+V\d+_t\s*\*", "unsigned char *", c_text)
    c_text = re.sub(r"\bstruct\s+V\d+_t\b", "unsigned", c_text)
    c_text = re.sub(r"([A-Za-z_]\w*)\s*->\s*field_([0-9a-fA-F]+)",
                    r"*(unsigned *)((unsigned char*)(\1)+0x\2)", c_text)
    refs = _stub_refs(c_text, fname)
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
    labels = set(re.findall(r"(?m)^\s*([A-Za-z_]\w*)\s*:(?!:)", c_text))
    labels |= set(re.findall(r"\bgoto\s+([A-Za-z_]\w*)", c_text))
    undecl = sorted(used - refs - declared - params - _TYPEWORDS - labels - {fname})
    rregs = sorted(g for g in undecl if re.fullmatch(r"r\d{1,2}", g))
    other = [g for g in undecl if not g.isdigit() and g not in rregs]
    if rregs:                                             # inject the frame-init prologue into THIS body
        init = "".join("  %s=(unsigned)(_frame+0x4000);\n" % r for r in rregs)
        dm2 = _DEF_RE.search(c_text)
        if dm2:
            br = dm2.end() - 1 if c_text[dm2.end() - 1] == "{" else c_text.index("{", dm2.end() - 1)
            c_text = c_text[:br + 1] + "\n" + init + c_text[br + 1:]
    c_text = re.sub(r"\bvoid(\s+(?:__fastcall|__cdecl|__stdcall|__watcall)?\s*%s\s*\()"
                    % re.escape(fname), r"int\1", c_text)
    return c_text, refs, other, rregs

def _ow_env(ow):
    return toolchain.ow_env("pe")

def _omf_export_name(obj, fname):
    from src.targets.win95 import autobuild95 as A
    pubs = A._omf_publics(obj)
    def _base(s):
        return s.lstrip("_@").split("@")[0].rstrip("_")
    cand = [s for s in pubs if _base(s) == fname]
    return cand[0] if cand else ("_" + fname)

def compile_body(c_text, fname, outdir):
    """Compile a single synthesised body into a tiny DLL exporting `fname`. Returns dll path|None."""
    ow = _ow()
    if not os.path.exists(ow["wcc"]):
        return None, "OpenWatcom not built"
    os.makedirs(outdir, exist_ok=True)
    c_text, refs, other, rregs = _prep_body(c_text, fname)
    stubs = ["int %s(){return 0;}" % r for r in sorted(refs)]
    gdecl = ["unsigned %s;" % g for g in other]
    if rregs:
        gdecl.append("static unsigned char _frame[0x8000];")
        gdecl += ["unsigned %s;" % r for r in rregs]
    src = "\n".join(_PRE + stubs + gdecl) + "\n" + c_text + "\n"
    cf = os.path.join(outdir, fname + ".c")
    open(cf, "w", newline="\n").write(src)
    env = _ow_env(ow)
    r = subprocess.run([ow["wcc"], "-bt=nt", "-zq", "-w0", "-s", "-zastd=c99", fname + ".c"],
                       cwd=outdir, env=env, capture_output=True, text=True, timeout=120)
    if r.returncode != 0:
        return None, "compile: " + (r.stdout + r.stderr).strip().split("\n")[-1][:80]
    obj = open(os.path.join(outdir, fname + ".obj"), "rb").read()
    internal = _omf_export_name(obj, fname)
    with open(os.path.join(outdir, "b.lnk"), "w", newline="\n") as f:
        f.write("system nt_dll\nname %s.dll\noption quiet\nfile %s.obj\nexport %s='%s'\n"
                % (fname, fname, fname, internal))
    subprocess.run([ow["wlink"], "@b.lnk"], cwd=outdir, env=env, capture_output=True, text=True)
    dll = os.path.join(outdir, fname + ".dll")
    return (dll if os.path.exists(dll) else None), ("ok" if os.path.exists(dll) else "link failed")

def batch_compile(items, outdir):
    """Compile MANY synthesised bodies into ONE DLL exporting all of them -- a single wcc386 + wlink
    instead of two subprocess spawns per body (the ~45%/body compile cost). `items` = [(fname, c_text)].
    Returns (dll_path, [fname...exported]) or (None, []) if even a single body won't compile alone.
    Callees NOT in the batch are stubbed; a name that IS a batch member links to its real body (so an
    intra-batch call runs the real callee -- never less faithful, and the oracle still judges vs retail).
    On a batch compile/link failure the caller bisects down to compile_body, so one bad body can't sink
    the rest."""
    ow = _ow()
    if not os.path.exists(ow["wcc"]) or not items:
        return None, []
    os.makedirs(outdir, exist_ok=True)
    preps = []
    for fname, c_text in items:
        try:
            preps.append((fname, _prep_body(c_text, fname)))
        except Exception:
            pass
    if not preps:
        return None, []
    members = {fn for fn, _ in preps}
    all_refs, all_other, all_rregs, bodies = set(), set(), set(), []
    for fname, (ct, refs, other, rregs) in preps:
        all_refs |= refs; all_other |= set(other); all_rregs |= set(rregs); bodies.append(ct)
    stub_refs = sorted(all_refs - members)               # intra-batch calls link to the real member
    stubs = ["int %s(){return 0;}" % r for r in stub_refs]
    gdecl = ["unsigned %s;" % g for g in sorted(all_other)]
    if all_rregs:
        gdecl.append("static unsigned char _frame[0x8000];")
        gdecl += ["unsigned %s;" % r for r in sorted(all_rregs)]
    # a member called with args but declared (void) -> K&R () so the intra-batch call compiles
    merged = "\n\n".join(bodies)
    merged = re.sub(r"\(\s*void\s*\)\s*\{", "() {", merged) if len(members) > 1 else merged
    src = "\n".join(_PRE + stubs + gdecl) + "\n" + merged + "\n"
    import hashlib
    # sha1, not hash(): Python salts str hashes per process, so the old name never repeated
    base = "batch_" + hashlib.sha1("\n".join(sorted(members)).encode()).hexdigest()[:10]
    cf = os.path.join(outdir, base + ".c")
    open(cf, "w", newline="\n").write(src)
    env = _ow_env(ow)
    r = subprocess.run([ow["wcc"], "-bt=nt", "-zq", "-w0", "-s", "-zastd=c99", base + ".c"],
                       cwd=outdir, env=env, capture_output=True, text=True, timeout=300)
    if r.returncode != 0:
        return None, []
    obj = open(os.path.join(outdir, base + ".obj"), "rb").read()
    exports = []
    with open(os.path.join(outdir, base + ".lnk"), "w", newline="\n") as f:
        f.write("system nt_dll\nname %s.dll\noption quiet\nfile %s.obj\n" % (base, base))
        for fn in sorted(members):
            f.write("export %s='%s'\n" % (fn, _omf_export_name(obj, fn)))
            exports.append(fn)
    subprocess.run([ow["wlink"], "@%s.lnk" % base], cwd=outdir, env=env, capture_output=True, text=True)
    dll = os.path.join(outdir, base + ".dll")
    return (dll, exports) if os.path.exists(dll) else (None, [])

def _batch_or_bisect(items, outdir, tag="b"):
    """Compile `items` [(fname, c_text)] into as few DLLs as possible: try the whole batch; on failure
    split in half and recurse, down to a single body (compile_body). Returns [(dll, [fname...])] -- the
    bad bodies simply end up as failed singletons, never sinking their neighbours."""
    if not items:
        return []
    dll, exports = batch_compile(items, os.path.join(outdir, tag))
    if dll:
        return [(dll, exports)]
    if len(items) == 1:
        fn, c = items[0]
        d, _ = compile_body(c, fn, os.path.join(outdir, tag + "_1"))
        return [(d, [fn])] if d else []
    mid = len(items) // 2
    return (_batch_or_bisect(items[:mid], outdir, tag + "L") +
            _batch_or_bisect(items[mid:], outdir, tag + "R"))


def verify_module_batched(retail, vas=None, arity=4, outdir=None, bank=True, fast=True):
    """Behavioural-verify MANY functions of one module with batched compilation (one wcc386+wlink per
    ~batch instead of per body). synth each -> batch_compile (bisecting around failures) -> oracle each
    export vs retail -> bank equivalents. Returns a verdict Counter. Intra-batch calls link to the real
    member, so a body that calls a sibling is verified against the real sibling (closure-like) for free.
    This is the throughput path for the corpus grind; closure>0 refinement of the residue is separate."""
    from collections import Counter
    from src.targets.win95 import autobuild95 as A
    import pe32util as H
    outdir = outdir or _scratch("batch", os.path.basename(retail).lower())
    os.makedirs(outdir, exist_ok=True)
    c = H.carve(retail); base = c.pe.image_base
    if vas is None:
        vas = sorted(c.funcs)
    cnt = Counter()
    # synth every body; keep (fname, synth_c, va). Dedup by fname (a shared body reached at two vas).
    items = []; seen = {}; synth_c = {}
    for va in vas:
        try:
            fn, cc = synth(retail, va, arity, fast=fast)
        except Exception:
            cnt["no-synth"] += 1; continue
        if not fn:
            cnt["no-synth"] += 1; continue
        if fn in seen:
            continue
        seen[fn] = va; synth_c[fn] = cc; items.append((fn, cc))
    subs = _batch_or_bisect(items, outdir)
    compiled = set()
    vecs = verify95._vectors(arity, "pe")
    for dll, exports in subs:
        if not dll:
            continue
        pb = A.PE(dll); bbase = A._imagebase(pb); addrs = pb.export_addrs()
        for fn in exports:
            ba = addrs.get(fn)
            if not ba:
                continue
            compiled.add(fn)
            va = seen[fn]
            rs = [verify95._fp_pe(retail, va, v) for v in vecs]
            bs = [verify95._fp_pe(dll, bbase + ba, v) for v in vecs]
            verdict = verify95._body_verdict(rs, bs)
            cnt[verdict] += 1
            if verdict == "equivalent" and bank:
                _save_verified(retail, fn, synth_c[fn])
    cnt["no-compile"] += len([fn for fn in seen if fn not in compiled])
    return cnt


def verify_body(retail, va, arity=4, outdir=None, closure=0):
    """Full primitive: synth -> compile -> oracle-verify vs the original body. `retail` is a PATH.
    Returns a verdict dict. `closure` > 0 ALSO tries a run that inlines the real internal-callee bodies
    to that depth (see synth_closure) -- the remedy for the call-graph-stubbing artifact, where a
    function whose behaviour flows through a callee is spuriously divergent against zero stubs. The
    closure run is STRICTLY NON-REGRESSING: it is attempted only when the stub run did not already pass
    and is adopted only if it passes, so inlining a callee body that fails to compile (common for deep
    or struct-heavy callees) can never downgrade a result."""
    res = _verify_one(retail, va, arity, outdir, 0)
    if closure > 0 and res.get("verdict") not in ("equivalent", "consistent"):
        clo = _verify_one(retail, va, arity, (outdir + "_cl") if outdir else None, closure)
        if clo.get("verdict") in ("equivalent", "consistent"):
            return clo
    return res

def _verify_one(retail, va, arity, outdir, closure):
    fname, c = synth_closure(retail, va, arity, closure) if closure > 0 else synth(retail, va, arity)
    if not fname:
        return {"va": va, "verdict": "no-synth", "reason": c}
    outdir = outdir or _scratch(os.path.basename(retail).lower(), "%x" % va)
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
    out = {"va": va, "verdict": verdict, "fname": fname}
    if verdict == "inconclusive":                        # say WHY nothing could be observed
        errs = sorted({"retail: " + a["err"] for a in rs if "err" in a} |
                      {"rebuilt: " + b["err"] for b in bs if "err" in b})
        out["reason"] = "; ".join(errs)
    return out

def _save_verified(retail, fname, c):
    """Write a verified body's beautiful source into the recovered tree, tagged oracle-verified."""
    modbase = os.path.splitext(os.path.basename(retail))[0]
    d = os.path.join(corpus.design_dir(), "recovered", "verified", modbase)
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
    """A module name or path -> the retail binary's path (src.core.corpus picks the installed
    copy). An unknown name is returned unchanged so the caller's error names it."""
    return corpus.retail(module) or module
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
    print("  --- " + "  ".join("%s=%s" % (k, v) for k, v in c.most_common()) + " ---")

def _ledger_path():
    return os.path.join(corpus.design_dir(), "body_ledger.csv")

def _write_rows(rows):
    """Atomically rewrite the ledger: write a sibling temp file, then os.replace() it into place. A
    crash (or a concurrent reader) can never see a half-written/truncated ledger this way. On Windows
    os.replace fails with PermissionError (WinError 5) if ANY process has the destination open for
    read at that instant (a monitor reading the ledger, an indexer, antivirus) -- a transient that must
    not kill a multi-hour sweep, so retry with backoff before giving up."""
    import csv as _csv
    import time as _time
    path = _ledger_path()
    tmp = path + ".tmp.%d" % os.getpid()
    with open(tmp, "w", newline="") as fp:
        w = _csv.DictWriter(fp, fieldnames=rows[0].keys())
        w.writeheader(); w.writerows(rows)
    for attempt in range(12):
        try:
            os.replace(tmp, path)                         # atomic on Windows + POSIX
            return
        except PermissionError:
            if attempt == 11:
                try:
                    os.remove(tmp)
                except OSError:
                    pass
                raise
            _time.sleep(0.25 * (attempt + 1))             # brief backoff; the lock is momentary

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

# The old sweep_parallel (a multiprocessing.Pool over modules) was removed: on Windows a worker
# stuck in the in-process oracle has no wall-clock limit, so the pool hung for hours. Corpus
# sweeps are `python -m src.targets.win95.bank pe|ne` (one subprocess per unit, with a timeout).


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
        if "--parallel" in argv:
            print("--parallel was removed; run: python -m src.targets.win95.bank pe"); return
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
            print("  %-14s %s" % (m, "  ".join("%s=%s" % (k, v) for k, v in sorted(cts.items()))))
        print("  bodies newly credited this sweep: %d" % r["credited"])
        print("  TOTAL VERIFIED in ledger:         %d" % r["total_verified"])
        return
    if not pos:
        print(__doc__); return
    if "--harvest" in argv:
        r = harvest(pos[0], maxn=maxn)
        print("=== bodyverify --harvest %s ===" % pos[0])
        print("  " + "  ".join("%s=%s" % (k, v) for k, v in sorted(r["counts"].items())))
        print("  bodies credited VERIFIED in ledger: %d" % r["credited"])
        return
    va = int(argv[argv.index("--va") + 1], 0) if "--va" in argv else None
    print("=== bodyverify %s (synth -> compile -> oracle-verify) ===" % pos[0])
    run(pos[0], va=va, maxn=maxn)

if __name__ == "__main__":
    main(sys.argv[1:])
