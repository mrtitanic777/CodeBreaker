#!/usr/bin/env python3
"""cmp.py <recon.obj> <orig.bin> <manifest.json> <PUBNAME> - side-by-side disasm."""
import sys, json
from engine.inverse_engine.vfy import build_segimage
from engine.inverse_engine.bvfy import mask_external_branches
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
_md = Cs(CS_ARCH_X86, CS_MODE_16)

def dis(code):
    out=[]
    for i in _md.disasm(bytes(code),0):
        out.append((i.address,i.size,code[i.address:i.address+i.size].hex(),i.mnemonic+" "+i.op_str))
    return out

def main():
    objp,imgp,manp,pub=sys.argv[1:5]
    image,mask,pubs=build_segimage(objp)
    man=json.load(open(manp))
    off,ln=int(man[pub][0],16),int(man[pub][1])
    start=pubs[pub]
    recon=bytearray(image[start:start+ln])
    orig=bytearray(open(imgp,"rb").read()[off:off+ln])
    m=bytearray(mask[start:start+ln])
    xm=mask_external_branches(recon)
    for i in range(min(len(m),len(xm))):
        if xm[i]: m[i]=1
    print(f"== {pub}  recon={len(recon)}b orig={ln}b ==")
    dr=dis(recon); do=dis(orig)
    print("--- RECON ---")
    for a,s,h,t in dr: print(f"  {a:04x}: {h:<14} {t}")
    print("--- ORIG ---")
    for a,s,h,t in do: print(f"  {a:04x}: {h:<14} {t}")
    # first real diff
    diffs=[i for i in range(min(len(recon),len(orig))) if not m[i] and recon[i]!=orig[i]]
    print("first diff:", hex(diffs[0]) if diffs else "NONE", "len_r=%d len_o=%d"%(len(recon),len(orig)))

if __name__=="__main__": main()
