"""early_c_round_trip.py - round-trip proof for the framemodel wiring.

For each probe function: oracle-compile -> extract bytes -> mc2inv.recover -> to_c ->
oracle-recompile -> masked byte-compare. Exercises unused-local gaps + temp-shifted
locals, which the old `int v<off>` emission could not reproduce.
"""
from codebreaker.compare import mask as masks
from codebreaker.invert import omf as objextract
from codebreaker.invert import early_c as mc2inv
from codebreaker.invert.early_c_check import external_branches as mask_external_branches
from codebreaker.invert.compilers import mc_compile_multi
from codebreaker.invert import compilers

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
    obj = compilers.mc_compile(csrc)
    if not obj: return None, None
    fns = objextract.Obj(obj).functions()
    if not fns: return None, None
    v = next(iter(fns.values()))
    return v[0], (v[1] if len(v)>1 else None)

def masked_eq(a, b, extra_mask):
    if len(a)!=len(b): return False, "len %d/%d"%(len(a),len(b))
    try: m=masks.of(extra_mask or (), len(a))
    except TypeError: m=bytearray(len(a))
    diffs=masks.differing(a, b, m, mask_external_branches(a))
    ok=not diffs
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
