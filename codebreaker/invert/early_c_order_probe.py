"""early_c_order_probe.py - characterize MC2's emission-order scheduling (load hoist / store defer)."""
import re
from codebreaker.formats import omf
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
from codebreaker.invert.compilers import mc_compile_multi
md = Cs(CS_ARCH_X86, CS_MODE_16)

def stream(src):
    obj = mc_compile_multi(src+"\n")
    code = next(iter(omf.function_bytes(obj).values()))[0]
    ops=[]
    for ins in md.disasm(bytes(code), 0):
        if ins.address < 0xf: continue
        mn=ins.mnemonic; o=ins.op_str; parts=o.split(", ")
        if mn=="mov" and parts[0] in ("ax","bx","cx","dx") and (parts[1].startswith("0x") or parts[1].isdigit()):
            ops.append("LD %s=%s"%(parts[0], parts[1]))
        elif mn=="mov" and parts[0] in ("ax","bx","cx","dx") and "[bp" in parts[1]:
            m=re.search(r"bp \+ (0x[0-9a-f]+|\d+)|\[bp\]",parts[1]); off=int(m.group(1),0) if (m and m.group(1)) else 0
            ops.append("%s=[%d]"%(parts[0],off))
        elif mn in ("mov","add","sub") and "[bp" in parts[0]:
            m=re.search(r"bp \+ (0x[0-9a-f]+|\d+)|\[bp\]",parts[0]); off=int(m.group(1),0) if (m and m.group(1)) else 0
            ops.append("ST[%d]=%s"%(off, parts[1]))
        elif mn in ("inc","dec","imul","idiv"): ops.append("%s %s"%(mn,o))
    return ops

TESTS=[
 "s1(){int a,b,c,d; a=11; b=22; c=a; d=44;}",
 "s2(){int a,b,c; a=11; b=a; c=33;}",
 "s3(){int a,b,c; a=11; c=a; b=22;}",
 "s4(){int a,b,c,d; a=11; b=22; c=33; d=a;}",
 "s5(){int a,b,c; a=11; b=22; c=33;}",
 "s6(){int a,b; a=11; b=a;}",
 "s7(){int a,b,c; a=11; b=22; c=a;}",
 "s8(){int a,b,c,d; a=11; b=a; c=33; d=c;}",
]
if __name__=="__main__":
    for src in TESTS:
        print("%-44s %s"%(src, stream(src)))
