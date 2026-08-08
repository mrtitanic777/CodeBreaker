#!/usr/bin/env python3
"""
cc32.py (win95) — compile 32-bit C with MSVC 4.2 and extract the emitted code,
for byte-exact matching of win95 PE reconstructions.

ERA-CORRECT ORACLE: retail Windows 95 / OSR2 (1995-96) 32-bit code was built with
MSVC 2.0/4.x. MSVC 4.2 (1996, compiler internal version 10.20) is now VENDORED in
win95's own reference tree and is the PREFERRED oracle here — the closest available
match to the shipping toolchain. MSVC 5.0 (1997, internal 11.00, shared from
win98/winnt4) remains as a FALLBACK when 4.2 is not staged. `_vc_root()` prefers 4.2,
then 5.0. Codegen from 4.2 should track retail closely, but optimizer/calling-convention
drift is still possible; a byte-exact match is strong evidence, a mismatch is NOT proof
the C is wrong. win95's own EMBEDDED 16-bit code (`.drv`/VxD thunks) uses cc16 instead.

The 32-bit analogue of cc16.py — but far simpler, because MSVC's `CL.EXE`
(the 32-bit C/C++ Optimizing Compiler) is a native Win32 program: it runs
directly, no DOSBox. Given the period compiler + the right
flags, a correct reconstruction of a function compiles to the SAME bytes the
shipped DLL carries — that is the proof (modulo the approximation caveat above).

Two layers, mirroring the NE path (cc16 + objextract + segdiff):

  * `compile_c()` runs CL and returns the COFF `.obj` (+ the `/Fa` asm listing).
  * `Obj(objbytes).functions()` parses the COFF object ourselves — file header,
    section table, relocation records, symbol table — and returns
    `{name: (code_bytes, mask)}` exactly like `objextract` does for 16-bit OMF.
    COFF is cleaner: the relocations are EXPLICIT records (DIR32/REL32), so the
    fixup mask is read straight from the object, no disassembly needed.

The comparison is mask-aware on both sides (`segdiff.diff`): our object masks its
relocation fields; the shipped side masks base-relocs + rel32 branch
displacements (as `carryover` does). Absolute addresses / call displacements
legitimately differ between an object and a linked image, so masking them is what
makes a true opcode-stream match possible.

    python cc32.py --selftest                 # prove the machinery (round-trip + mutation)
    python cc32.py foo.c --flags "/c /O2"     # compile foo.c, list its functions + sizes
    python cc32.py foo.c --match USER32.DLL:_FooFunc@4:0x1234
                                              # diff foo.c's _FooFunc vs the shipped
                                              # USER32 function carved at rva 0x1234
"""
import argparse
import os
import struct
import subprocess
import sys
import tempfile

import segdiff
import target
from engine.foundation import layout

APP_DIR = os.path.dirname(os.path.abspath(__file__))


# --------------------------------------------------------------------------
# locate MSVC 5.0 (from win98re config; default the staged toolchain)
# --------------------------------------------------------------------------

def _vc_root():
    # Explicit override (the oracle bake-off points this at each vendored version in
    # turn to find which compiler reproduces win95's codegen best). Highest priority.
    envr = os.environ.get("WT_VC_ROOT")
    if envr and os.path.exists(os.path.join(envr, "BIN", "CL.EXE")):
        return envr
    # Prefer MSVC 4.2 (msvc42), the era-correct win95 oracle; fall back to MSVC 5.0
    # (msvc50), shared from win98/winnt4. Config keys override the staged defaults.
    import json
    for key in ("msvc42", "msvc50"):
        for base in (APP_DIR, os.getcwd(), os.path.dirname(os.getcwd())):
            p = os.path.join(base, "CodeBreaker.config.json")
            if os.path.exists(p):
                try:
                    v = json.load(open(p)).get(key)
                    if v and os.path.exists(os.path.join(v, "BIN", "CL.EXE")):
                        return v
                except (OSError, ValueError):
                    pass
    # default: the empirically-best win95 oracle. The oraclebake bake-off proved the
    # 2.x C2 backend (v9.x, 1994-95, contemporary with Win95 RTM) reproduces win95's
    # codegen where every 4.x (v10.x) diverges — e.g. the imul operand order at
    # kernel32:0x059a8 that all 4.x miss. So prefer MSVC 2.0/2.1/2.2 (flat msvcNN/VC
    # layout), then fall back to 4.2 (DEVSTUDIO/VC), then 5.0.
    for root in (os.getcwd(), os.path.dirname(APP_DIR)):
        tc = layout.reference_sub("toolchain", base=root)
        for cand in (os.path.join(tc, "msvc20", "VC"),
                     os.path.join(tc, "msvc21", "VC"),
                     os.path.join(tc, "msvc22", "VC"),
                     os.path.join(tc, "msvc42", "DEVSTUDIO", "VC")):
            if os.path.exists(os.path.join(cand, "BIN", "CL.EXE")):
                return cand
    # fallback: the staged MSVC 5.0 toolchain under the project root
    for root in (os.getcwd(), os.path.dirname(APP_DIR)):
        cand = os.path.join(layout.reference_sub("toolchain", base=root), "msvc50", "DEVSTUDIO", "VC")
        if os.path.exists(os.path.join(cand, "BIN", "CL.EXE")):
            return cand
    # last resort: win95 has no vendored MSVC 5.0 of its own — share the sibling
    # data dir's MSVC 5.0 (win98's, then winnt4's). cwd is the top-level win95 data
    # dir, so ../win98 and ../winnt4 are the sibling projects. Approximate oracle;
    # see the module docstring. Duplicating the toolchain buys nothing here.
    for sib in ("win98", "winnt4"):
        cand = os.path.join(layout.reference_sub("toolchain",
                            base=os.path.join(os.path.dirname(os.getcwd()), sib)),
                            "msvc50", "DEVSTUDIO", "VC")
        if os.path.exists(os.path.join(cand, "BIN", "CL.EXE")):
            return cand
    return ""


def _cl_env(vc):
    env = os.environ.copy()
    bindir = os.path.join(vc, "BIN")
    env["PATH"] = bindir + os.pathsep + env.get("PATH", "")
    env["INCLUDE"] = os.path.join(vc, "INCLUDE")
    env["LIB"] = os.path.join(vc, "LIB")
    return env, bindir


# --------------------------------------------------------------------------
# compile
# --------------------------------------------------------------------------

def compile_c(csrc, flags="/c /O2", stem="t", timeout=120):
    """Compile C with the vendored CL (MSVC 4.2, else 5.0). Returns {obj: bytes,
    cod: str, log: str, flags: str}. `flags` should include /c (compile only). The
    backend (C1/C2) + any PDB/DIS support DLLs must be reachable — the staged BIN has
    them (see reference/toolchain/README)."""
    vc = _vc_root()
    if not vc or not os.path.exists(os.path.join(vc, "BIN", "CL.EXE")):
        raise RuntimeError("MSVC 4.2/5.0 not found. Set it with:  win95re config msvc42 <path-to-VC>")
    env, bindir = _cl_env(vc)
    out = {"flags": flags}
    with tempfile.TemporaryDirectory(prefix="cc32_") as td:
        cpath = os.path.join(td, stem + ".c")
        with open(cpath, "w", encoding="latin1") as f:
            f.write(csrc)
        cmd = ([os.path.join(bindir, "CL.EXE"), "/nologo"] + flags.split()
               + ["/Fo" + stem + ".obj", "/Fa" + stem + ".cod", stem + ".c"])
        try:
            r = subprocess.run(cmd, cwd=td, env=env, capture_output=True,
                               text=True, timeout=timeout)
            out["log"] = (r.stdout or "") + (r.stderr or "")
        except subprocess.TimeoutExpired:
            out["log"] = "TIMEOUT"
            return out
        for ext, key in ((".obj", "obj"), (".cod", "cod")):
            p = os.path.join(td, stem + ext)
            if os.path.exists(p):
                mode = "rb" if key == "obj" else "r"
                with open(p, mode, **({} if key == "obj" else {"errors": "replace"})) as f:
                    out[key] = f.read()
    return out


# --------------------------------------------------------------------------
# COFF object parser  (IMAGE_FILE_HEADER / sections / relocations / symbols)
# --------------------------------------------------------------------------

_SCN_CNT_CODE = 0x00000020
# i386 relocation types that patch a 4-byte field in the code stream.
_REL4 = {0x0006,  # IMAGE_REL_I386_DIR32   absolute 32-bit
         0x0007,  # IMAGE_REL_I386_DIR32NB absolute 32-bit, image-relative
         0x0014}  # IMAGE_REL_I386_REL32   pc-relative 32-bit (call/jmp)


class Obj:
    """A COFF .obj. `functions()` -> {name: (code_bytes, mask)}."""

    def __init__(self, data):
        self.data = data
        (self.machine, self.nsec, _ts, self.symptr, self.nsym,
         _optsz, _chars) = struct.unpack("<HHIIIHH", data[:20])
        self.sections = []
        off = 20
        for _ in range(self.nsec):
            (name, _vsz, _va, rawsz, rawptr, relptr, _lnptr,
             nrel, _nln, chars) = struct.unpack("<8sIIIIIIHHI", data[off:off + 40])
            self.sections.append(dict(
                name=name.rstrip(b"\0").decode("latin1"),
                rawsz=rawsz, rawptr=rawptr, relptr=relptr, nrel=nrel, chars=chars))
            off += 40
        self._strtab = data[self.symptr + self.nsym * 18:] if self.symptr else b""

    def _sym_name(self, raw8):
        if raw8[:4] == b"\0\0\0\0":
            soff = struct.unpack("<I", raw8[4:8])[0]
            end = self._strtab.find(b"\0", soff)
            return self._strtab[soff:end].decode("latin1")
        return raw8.rstrip(b"\0").decode("latin1")

    def _code_sections(self):
        """[(index1based, bytes, mask)] for EVERY code section. MSVC 5.0 emits each
        function in its own `.text` section (COMDAT, for linker dead-stripping), so
        there is rarely a single .text — all must be collected. Mask = byte offsets
        covered by a 4-byte relocation field, read straight from the reloc records."""
        out = []
        for i, s in enumerate(self.sections, 1):
            if s["name"].lower().startswith(".text") or (s["chars"] & _SCN_CNT_CODE):
                code = self.data[s["rawptr"]:s["rawptr"] + s["rawsz"]]
                mask = set()
                p = s["relptr"]
                for _ in range(s["nrel"]):
                    vaddr, _symidx, typ = struct.unpack("<IIH", self.data[p:p + 10])
                    p += 10
                    if typ in _REL4:
                        for k in range(4):
                            mask.add(vaddr + k)
                out.append((i, code, mask))
        return out

    def _text_symbols(self, sec_index):
        """[(offset, name)] for real symbols defined in the code section, skipping
        the section symbol and aux records."""
        out = []
        p = self.symptr
        i = 0
        while i < self.nsym:
            rec = self.data[p:p + 18]
            raw8 = rec[:8]
            value, secnum, _typ, sclass, naux = struct.unpack("<IhHBB", rec[8:18])
            name = self._sym_name(raw8)
            if secnum == sec_index and not name.startswith("."):
                # function/label defined in .text; StorageClass EXTERNAL(2)/STATIC(3)
                if sclass in (2, 3):
                    out.append((value, name))
            p += 18 * (1 + naux)
            i += 1 + naux
        return out

    def functions(self):
        """{name: (bytes, mask)} — for every code section, slice between consecutive
        defined symbols (mirrors objextract.functions for OMF). Handles both a
        single .text with many functions and MSVC's one-.text-per-function layout."""
        out = {}
        for idx, code, mask in self._code_sections():
            syms = sorted(set(self._text_symbols(idx)))
            if not syms:
                continue
            for i, (off, nm) in enumerate(syms):
                end = syms[i + 1][0] if i + 1 < len(syms) else len(code)
                if end <= off:
                    continue
                body = code[off:end]
                # MSVC pads each COMDAT function section to alignment with int3
                # (0xCC) or nop (0x90); trim it so the extracted body is the
                # function only — the shipped carve has the exact length, so
                # padding would otherwise read as a spurious length difference.
                b = len(body)
                while b > 0 and body[b - 1] in (0xCC, 0x90):
                    b -= 1
                out[nm] = (body[:b], {m - off for m in mask if off <= m < b})
        return out


def masked(code, mask):
    """Return code with masked byte positions zeroed (for display/hashing)."""
    b = bytearray(code)
    for k in mask:
        if 0 <= k < len(b):
            b[k] = 0
    return bytes(b)


# --------------------------------------------------------------------------
# shipped side: a retail PE function's bytes + mask (base-relocs + rel32)
# --------------------------------------------------------------------------

def shipped_pe_function(module, rva):
    """(bytes, mask) for the carved retail function at `rva` in a PE module.
    Mask = base-reloc absolute fields + relative branch displacements — the same
    fixup classes carryover masks, so the compare is opcode-stream only."""
    import capstone
    from engine.format_loaders import pe_trace
    path = target.resolve(module)
    c = pe_trace.PeCarve(path)
    base = c.pe.image_base
    start = base + rva
    f = c.funcs.get(start)
    if not f:
        raise ValueError("no carved function at rva 0x%x in %s "
                         "(try `petrace %s` to list starts)" % (rva, module, module))
    end = f["end"]
    raw = None
    for sva, data, _nm in c.sections:
        if sva <= start < sva + len(data):
            raw = bytes(data[start - sva:end - sva]); break
    if raw is None:
        raise ValueError("function bytes not in a code section")
    mask = set()
    for va in (base + r for r in c.pe.base_relocs()):
        if start <= va < end:
            for k in range(4):
                mask.add(va - start + k)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    for insn in md.disasm(raw, 0):
        bs, a, sz = insn.bytes, insn.address, insn.size
        if not bs:
            continue
        if bs[0] in (0xE8, 0xE9) and sz >= 5:
            mask.update(range(a + 1, a + 5))
        elif bs[0] == 0x0F and sz >= 6 and 0x80 <= bs[1] <= 0x8F:
            mask.update(range(a + 2, a + 6))
        elif (bs[0] == 0xEB or 0x70 <= bs[0] <= 0x7F or bs[0] in (0xE0, 0xE1, 0xE2, 0xE3)) and sz >= 2:
            mask.add(a + 1)
    return raw, mask


def match(csrc, func, module, rva, flags="/c /O2"):
    """Compile csrc, extract `func`, diff mask-aware vs the shipped PE function.
    Returns (result_dict, our_bytes, our_mask, shipped_bytes, shipped_mask)."""
    r = compile_c(csrc, flags=flags)
    if "obj" not in r:
        raise RuntimeError("compile produced no OBJ:\n" + r.get("log", "")[:800])
    funcs = Obj(r["obj"]).functions()
    if func not in funcs:
        raise RuntimeError("function %r not in object; got: %s"
                           % (func, ", ".join(sorted(funcs)[:20])))
    ob, om = funcs[func]
    sb, sm = shipped_pe_function(module, rva)
    res = segdiff.diff(ob, om, sb, sm)
    return res, ob, om, sb, sm


# --------------------------------------------------------------------------
# selftest — prove the machinery without a retail anchor
# --------------------------------------------------------------------------

_GROUP = r"""
int helper(int x)  { return x + 1; }
int caller(int y)  { return helper(y) * 2 + helper(y + 1); }
"""


def selftest():
    print("cc32 selftest\n")
    vc = _vc_root()
    if not vc:
        print("  MSVC 4.2/5.0 not configured; skipping"); return True
    print("  using VC toolchain:", vc)
    ok = True
    g = compile_c(_GROUP, flags="/c /O2")
    if "obj" not in g:
        print("  COMPILE FAILED:\n" + g.get("log", "")[:600]); return False
    funcs = Obj(g["obj"]).functions()
    print("  compiled; functions in object:", ", ".join(sorted(funcs)))
    if "_caller" not in funcs:
        print("  FAIL: _caller not found (name-decoration mismatch?)"); return False
    cb, cm = funcs["_caller"]
    # (a) round-trip: a function matches itself exactly (mask-aware)
    res = segdiff.diff(cb, cm, cb, cm)
    print("  (a) round-trip _caller vs itself:",
          "BYTE-EXACT" if res["exact"] else "DIFF (BUG)")
    ok = ok and res["exact"]
    # (b) a mutation is detected and located
    g2 = compile_c(_GROUP.replace("* 2 +", "* 3 +"), flags="/c /O2")
    cb2, cm2 = Obj(g2["obj"]).functions()["_caller"]
    res2 = segdiff.diff(cb2, cm2, cb, cm)
    print("  (b) mutated _caller vs original:",
          "differs (good)" if not res2["exact"] else "MATCH (BUG: should differ)")
    ok = ok and not res2["exact"]
    # (c) relocations were captured (caller calls helper -> >=1 rel32 masked)
    print("  (c) _caller masked bytes (rel32 call sites): %d" % len(cm))
    ok = ok and len(cm) > 0
    print("\nSELFTEST:", "PASS" if ok else "FAIL")
    return ok


# --------------------------------------------------------------------------

def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("csrc", nargs="?", help="C source file to compile")
    ap.add_argument("--flags", default="/c /O2", help="CL flags (must include /c)")
    ap.add_argument("--match", help="FUNC:MODULE:RVA — diff FUNC vs shipped PE "
                    "function at RVA (e.g. _Foo@4:USER32.DLL:0x1234)")
    ap.add_argument("--func", help="with a plain compile: show only this function")
    ap.add_argument("--selftest", action="store_true")
    args = ap.parse_args()

    if args.selftest:
        sys.exit(0 if selftest() else 1)
    if not args.csrc:
        ap.print_help(); sys.exit(2)
    csrc = open(args.csrc, encoding="latin1").read()

    if args.match:
        func, module, rva = args.match.split(":")
        res, ob, om, sb, sm = match(csrc, func, module, int(rva, 0), flags=args.flags)
        print("# match %s  vs  %s @ %s   (flags: %s)"
              % (func, module, rva, args.flags))
        print(segdiff.render(res, ob, sb, om, sm))
        sys.exit(0 if res["exact"] else 1)

    r = compile_c(csrc, flags=args.flags)
    if "obj" not in r:
        print("compile failed:\n" + r.get("log", "")); sys.exit(1)
    funcs = Obj(r["obj"]).functions()
    print("# %s -> %d function(s), flags: %s" % (args.csrc, len(funcs), args.flags))
    for nm, (code, mask) in sorted(funcs.items()):
        if args.func and nm != args.func:
            continue
        print("  %-28s %5d bytes  %3d masked (reloc fields)" % (nm, len(code), len(mask)))


if __name__ == "__main__":
    main()
