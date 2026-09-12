#!/usr/bin/env python3
"""rtlocals.py - round-trip proof for the framemodel wiring.

For each probe function: oracle-compile -> extract bytes -> mc2inv.recover -> to_c ->
oracle-recompile -> masked byte-compare. Exercises unused-local gaps + temp-shifted
locals, which the old `int v<off>` emission could not reproduce.
"""
import sys, os, re
from src.formats import objextract
from src.engines.inverse import mc2inv
from src.engines.inverse.bvfy import mask_external_branches
from src.engines.inverse.oracle16 import mc_compile_multi
from src.engines.inverse import raiser16

# functions that specifically stress local layout (gaps, temps, order)
BATTERY = r"""
t01(){int a,b; a=1; b=2;}
t02(){int a,b,c; a=1; c=3;}
t04(){int a,b,c,d; a=1; d=4;}
t07(){int a,b,c; a=b+c+1;}
r09(){int a,b; a=1; b=a+a;}
r11(){int a,b; a=5; b=a;}
r12(){int a,b,c; a=1; b=2; c=a*b;}
r13(p)int p;{int a; a=p; return a+1;}
r14(){int a,b; a=1; b=2; return a+b;}
r10(){int a,b,c; a=1; b=2; c=a+b;}
r15(){int a,b,c; a=1; b=2; c=a-b;}
r16(){int a,b; a=3; b=a*a;}
c01(){int a,b; a=5; b=a;}
c02(){int a,b,c; a=11; b=a; c=b;}
c03(){int a,b,c,d; a=11; b=22; c=a; d=44;}
c04(){int a,b,c,d; a=11; b=22; c=33; d=a;}
c05(){int a,b,c; a=11; b=22; c=b;}
c06(){int a,b; a=11; b=a; a=99;}
c07(){int a,b,c,d; a=11; b=a; c=33; d=c;}
"""

def extract_one(csrc):
    """compile a single-function C string, return (bytes, mask_offsets) for its function."""
    obj = raiser16.mc_compile(csrc)
    if not obj: return None, None
    fns = objextract.Obj(obj).functions()
    if not fns: return None, None
    v = next(iter(fns.values()))
    return v[0], (v[1] if len(v)>1 else None)

def masked_eq(a, b, extra_mask):
    if len(a)!=len(b): return False, "len %d/%d"%(len(a),len(b))
    m=bytearray(len(a))
    if extra_mask:
        try:
            for i in extra_mask:
                if 0<=i<len(m): m[i]=1
        except TypeError: pass
    for i,x in enumerate(mask_external_branches(a)):
        if x and i<len(m): m[i]=1
    ok=all(m[i] or a[i]==b[i] for i in range(len(a)))
    diffs=[i for i in range(len(a)) if not m[i] and a[i]!=b[i]]
    return ok, ("BYTE-EXACT" if ok else "diff@%s"%diffs[:6])

def main():
    obj = mc_compile_multi(BATTERY)
    fns = objextract.Obj(obj).functions()
    ok=0; tot=0
    for k in sorted(fns.keys(), key=lambda s:s.upper()):
        code = fns[k][0]
        tot+=1
        try:
            fs,params,stmts,pt,loc = mc2inv.recover(bytes(code))
            csrc = mc2inv.to_c(k.lstrip("_"), fs, params, stmts, pt, loc)
        except Exception as e:
            print("  %-5s INVERSE-FAIL %s"%(k.lstrip("_"), e)); continue
        recomp, mask = extract_one(csrc)
        if recomp is None:
            print("  %-5s RECOMPILE-FAIL"%k.lstrip("_")); continue
        good, note = masked_eq(recomp, code, mask)
        ok += good
        print("  %-5s fs=%-3d loc=%-14s %s"%(k.lstrip("_"), fs, sorted(loc), note))
    print("\nrtlocals: %d/%d round-trip BYTE-EXACT with framemodel local layout"%(ok,tot))

if __name__=="__main__": main()
