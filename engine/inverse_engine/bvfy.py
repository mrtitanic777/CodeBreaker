#!/usr/bin/env python3
"""bvfy.py - batch verify: compile one recon .obj, verify MANY functions vs original,
update a ledger. The scalable crank driver (compile once, verify all).

Usage: python bvfy.py <recon.obj> <orig_image.bin> <manifest.json> [ledger.json]
manifest.json: { "PUBNAME": ["0xHEXOFF", length], ... }
"""
import sys, json
from engine.inverse_engine.vfy import build_segimage
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
_md = Cs(CS_ARCH_X86, CS_MODE_16)

def mask_external_branches(code):
    """Return a bytearray mask marking the displacement bytes of any relative
    call/jmp whose target lands OUTSIDE this function (calls to sibling functions,
    tail-jumps to other functions). Their relative offset depends on link layout,
    so it must be wildcarded. Intra-function jumps (targets inside `code`) are LEFT
    intact so real control-flow bytes still get verified."""
    m = bytearray(len(code))
    for insn in _md.disasm(code, 0):
        mn = insn.mnemonic
        if mn == "call" or mn == "jmp" or mn.startswith("j") or mn.startswith("loop"):
            # relative branch? operand is an immediate address
            ops = insn.op_str
            if ops.startswith("0x"):
                try: tgt = int(ops, 16)
                except ValueError: continue
                if tgt < 0 or tgt >= len(code):     # target outside this function
                    # displacement bytes = the tail of the instruction (1 for short, 2 for near)
                    dl = 2 if insn.size >= 4 or code[insn.address] in (0xe8,0xe9) else 1
                    if code[insn.address] in (0xeb,) or (0x70 <= code[insn.address] <= 0x7f):
                        dl = 1
                    for k in range(dl):
                        m[insn.address + insn.size - dl + k] = 1
    return m

def main():
    objp, imgp, manp = sys.argv[1:4]
    ledp = sys.argv[4] if len(sys.argv)>4 else None
    image, mask, pubs = build_segimage(objp)
    orig = open(imgp,"rb").read()
    man = json.load(open(manp))
    ledger = {}
    if ledp:
        try: ledger=json.load(open(ledp))
        except: ledger={}
    ok=bad=0; results=[]
    for name,(offhex,ln) in man.items():
        off=int(offhex,16); ln=int(ln)
        if name not in pubs:
            results.append((name,"NO-PUB",0)); bad+=1; continue
        start=pubs[name]
        recon=image[start:start+ln]; m=bytearray(mask[start:start+ln]); o=orig[off:off+ln]
        # also wildcard sibling/external relative-call offsets (layout-dependent)
        xm=mask_external_branches(recon)
        for i in range(min(len(m),len(xm))):
            if xm[i]: m[i]=1
        nfix=sum(m); diffs=[i for i in range(min(len(recon),len(o))) if not m[i] and recon[i]!=o[i]]
        if not diffs and len(recon)==ln:
            results.append((name,"EXACT",nfix)); ok+=1
            ledger[offhex]={"name":name,"len":ln,"status":"EXACT","fixups":nfix}
        else:
            results.append((name,"MISMATCH@%s"%(hex(diffs[0]) if diffs else 'len'),nfix)); bad+=1
    for name,st,nf in results:
        print(f"  {name:<10} {st:<16} {'('+str(nf)+' fixups)' if nf else ''}")
    print(f"\nBATCH: {ok} EXACT, {bad} failed  ({ok}/{ok+bad})")
    if ledp:
        json.dump(ledger, open(ledp,"w"), indent=1)
        print(f"ledger: {len(ledger)} functions verified EXACT total -> {ledp}")

if __name__=="__main__": main()
