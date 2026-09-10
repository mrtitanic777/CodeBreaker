#!/usr/bin/env python3
"""raiser16.py - byte-exact C reconstruction backend, powered by the INVERSE ENGINE.

THE MERGE: instead of black-box search (lift a hypothesis, compile, diff, permute),
this drives `mc2inv` (the deterministic bytes->C inverse of the MC2 codegen model) to
produce C directly, then VERIFIES it by recompiling with the real MC1/MC2 oracle and
checking the bytes match the original (relocations masked). No search in the loop.

  reconstruct(image, off, size) -> (c_source, verified: bool, note)

Usage: python raiser16.py <image.bin> <hexoff> <len>          # one function
       python raiser16.py <image.bin> --batch <worklist.json> # many; report rate
"""
import sys, os, json, subprocess, struct, re
from codebreaker.inverse_engine import mc2inv
from codebreaker.inverse_engine.oracle16 import mc_compile
from codebreaker.format_loaders import objextract
from codebreaker.inverse_engine.bvfy import mask_external_branches

def masked_eq(recon, mask, orig):
    if len(recon)!=len(orig): return False
    return all(mask[i] or recon[i]==orig[i] for i in range(len(recon)))

def reconstruct(image, off, size, verify=True):
    code = bytes(image[off:off+size])
    if len(code) < 4:
        return None, False, "too-small (not a function)"
    try:
        fs, params, stmts, pt, loc = mc2inv.recover(code)  # pt = int|long|float (param type)
    except Exception as e:
        return None, False, "inverse-failed: %s"%e
    name = "f%05x"%off                                    # empty stmts -> empty-body function f(){}
    globs = sorted(set(re.findall(r"g_[0-9a-f]+", " ".join(stmts))))
    sig = "%s(%s)"%(name, ", ".join(params)) if params else "%s()"%name
    decl = ("%s %s;"%(pt, ", ".join(params))) if params else ""
    # framemodel-driven local layout: int at each referenced offset, char filler for
    # unused-local gaps + trailing region, so the recompiled frame is byte-identical.
    ordered, _ = mc2inv.framemodel.emit_local_decls(loc or [], fs, None, mc2inv.detect_copies(stmts))
    ldecl = "".join(mc2inv.framemodel.fmt_decl(*d)+" " for d in ordered)
    stmts = ([ldecl.strip()] if ldecl else []) + stmts
    pre = ("int %s;\n"%", ".join(globs)) if globs else ""
    csrc = "%s%s\n%s\n{\n%s\n}\n"%(pre, sig, decl, "\n".join("    "+s for s in stmts))
    if not verify:
        return csrc, None, "unverified"
    obj = mc_compile(csrc)
    if not obj: return csrc, False, "compile-failed"
    try:
        fns = objextract.Obj(obj).functions()
    except Exception as e:
        return csrc, False, "objextract: %s"%e
    cand=None; candmask=None
    for k,v in fns.items():
        if k.upper().lstrip("_")==name.upper(): cand=v[0]; candmask=(v[1] if len(v)>1 else None); break
    if cand is None and fns:
        v=next(iter(fns.values())); cand=v[0]; candmask=(v[1] if len(v)>1 else None)
    if cand is None: return csrc, False, "no-func-in-obj"
    m = bytearray(len(cand))
    if candmask is not None:
        try:
            it=list(candmask)
            if it and all(isinstance(x,int) for x in it) and not isinstance(candmask,(bytes,bytearray)):
                for i in it:                          # set/list of relocated offsets
                    if 0<=i<len(m): m[i]=1
            else:
                for i in range(min(len(m),len(candmask))):  # byte-array mask
                    if candmask[i]: m[i]=1
        except TypeError: pass
    for i,x in enumerate(mask_external_branches(cand)):       # external call/jmp displacements
        if x and i<len(m): m[i]=1
    ok = masked_eq(cand, m, code)
    return csrc, ok, ("BYTE-EXACT (%d fixups)"%sum(m) if ok else "mismatch len %d/%d"%(len(cand),len(code)))

def main():
    img = open(sys.argv[1],"rb").read()
    if "--batch" in sys.argv:
        wl = json.load(open(sys.argv[sys.argv.index("--batch")+1]))
        ok=att=0
        for f in wl:
            off=int(f["off"],16); size=f["size"]
            csrc, v, note = reconstruct(img, off, size)
            att+=1; ok+= (v is True)
            print("  %-16s %-30s %s"%(f["name"], note, "OK" if v else ""))
        print("\nINVERSE-ENGINE RECONSTRUCTION: %d/%d functions byte-exact C"%(ok,att))
    else:
        off=int(sys.argv[2],16); size=int(sys.argv[3])
        csrc, v, note = reconstruct(img, off, size)
        print("verified:", v, "|", note); print(); print(csrc)

if __name__=="__main__": main()
