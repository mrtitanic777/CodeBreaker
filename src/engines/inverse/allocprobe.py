#!/usr/bin/env python3
"""allocprobe.py - recover MC2's frame-offset allocation rule empirically.

Compiles a battery of local-variable C functions through the MC1/MC2 oracle in ONE
dosbox run, then for each function disassembles the prologue framesize (`sub sp,N`)
and every `[bp+disp]` reference, printing the layout so we can correlate
source-local-usage -> assigned offset and recover the allocator rule.
"""
import sys, os, re
from src.formats import objextract
from src.engines.inverse.oracle16 import mc_compile_multi
from capstone import Cs, CS_ARCH_X86, CS_MODE_16

md = Cs(CS_ARCH_X86, CS_MODE_16); md.detail = True

# Battery: each function probes a local-allocation pattern. K&R only, int locals.
BATTERY = r"""
g01(){int a,b; a=(b+1)*2;}
g02(){int a,b,c; a=(b+c)*(b-c);}
g03(){long a,b; a=b+1;}
g04(){long a; int b; a=b;}
g05(){int a; long b,c; a=1;}
g06(){char a,b; a=1; b=2;}
g07(){int a; char b; long c; a=1; b=2; c=3;}
g08(){int a[3]; a[0]=1;}
g09(){int a,b; a=1; {int c; c=a; b=c;}}
g10(p)int p;{int a; a=p+1;}
g11(){int a,b,c; a=b+c+1;}
g12(){int a; a=1; a=a*a+a;}
g13(){int a,b; a=1;b=2;return a*b+a-b;}
g14(){long a,b,c; a=b*c;}
g15(){int a,b,c,d,e; a=b=c=d=e=0;}
"""

def analyze(name, code):
    fs=None; refs=[]  # (offset_in_fn, disp)
    for ins in md.disasm(bytes(code), 0):
        m=ins.mnemonic; op=ins.op_str
        if m=="sub" and op.startswith("sp,"):
            try: fs=int(op.split(",")[1],0)
            except: pass
        # bp-relative memory refs
        for mm in re.finditer(r"\[bp\s*([+\-]\s*0x[0-9a-f]+|[+\-]\s*\d+)?\]", op):
            d=mm.group(1)
            disp=0 if d is None else int(d.replace(" ",""),0)
            refs.append(disp)
    disps=sorted(set(refs))
    return fs, disps

def main():
    obj=mc_compile_multi(BATTERY)
    if not obj:
        print("COMPILE FAILED"); return
    fns=objextract.Obj(obj).functions()
    order=sorted(fns.keys(), key=lambda k:k.upper())
    for k in order:
        code=fns[k][0]
        fs,disps=analyze(k,code)
        pos=[d for d in disps if d>=0]      # locals/temps at bp+ (framesize side)
        print("%-6s fs=%-4s  bp-disps=%s" % (k.lstrip("_"), fs, pos))

if __name__=="__main__": main()
