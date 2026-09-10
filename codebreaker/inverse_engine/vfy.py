#!/usr/bin/env python3
"""vfy.py - verify a reconstructed Lattice C .obj against original binary bytes,
modulo relocations (fixup sites are treated as wildcards).

This is the Lattice-C analog of WinTranspiler's byte-exact verify: it proves a
reconstructed function's SOURCE is right when its code bytes match the original
everywhere except at linker-relocated fields (global offsets, call targets).

Usage:
  python vfy.py <recon.obj> <func_pubname> <orig_image.bin> <orig_hex_off> [len]
    - assembles the .obj's PROG segment (LEDATA) + a wildcard mask from FIXUPP,
    - pulls <func_pubname> (a PUBDEF) code+mask,
    - compares to orig_image[orig_off : orig_off+len] with wildcards ignored.

Report: EXACT / EXACT(modulo N fixups) / MISMATCH (with first differing offset).
"""
import sys, struct
from codebreaker.inverse_engine.omf import records, rd_index

def build_segimage(objpath, segclass="PROG"):
    """Return (image bytes, mask[list of bool wildcard], {pubname:off})."""
    data = open(objpath, "rb").read()
    lnames=[""]; segs=[]; pubs={}
    image = bytearray(); mask = bytearray()
    cur_ledata_off = None
    for off, rt, body in records(data):
        if rt == 0x96:
            j=0
            while j < len(body):
                n=body[j]; lnames.append(body[j+1:j+1+n].decode('latin1')); j+=1+n
        elif rt in (0x98,0x99):
            attr=body[0]; seglen=struct.unpack_from("<H",body,1)[0]
            si,j=rd_index(body,3); segs.append(lnames[si] if si<len(lnames) else str(si))
        elif rt in (0xa0,0xa1):  # LEDATA
            seg,j=rd_index(body,0); dataoff=struct.unpack_from("<H",body,j)[0]; j+=2
            payload=body[j:]
            sname=segs[seg-1] if 0<seg<=len(segs) else ""
            if sname==segclass:
                end=dataoff+len(payload)
                if end>len(image):
                    image.extend(b"\x00"*(end-len(image))); mask.extend(b"\x00"*(end-len(mask)))
                image[dataoff:end]=payload
                cur_ledata_off=dataoff
            else:
                cur_ledata_off=None
        elif rt in (0x9c,0x9d):  # FIXUPP
            if cur_ledata_off is None: continue
            j=0
            while j < len(body):
                b0=body[j]
                if not (b0&0x80):  # THREAD subrecord
                    j+=1
                    # thread: method byte already consumed; skip index if present
                    # (rare in our tiny objs) - best effort
                    continue
                locat=(b0<<8)|body[j+1]; j+=2
                datarec_off=locat & 0x03ff
                loc=(locat>>10)&0x0f
                fixdat=body[j]; j+=1
                # frame/target fields (indices) - skip per fixdat encoding
                fbit=(fixdat>>4)&7; tbit=fixdat&3; Pbit=(fixdat>>3)&1
                if not (fixdat&0x80):  # frame not by thread
                    if fbit in (0,1,2): _,j=rd_index(body,j)
                if not (fixdat&0x08):  # target not by thread
                    _,j=rd_index(body,j)
                if not Pbit:  # target displacement present
                    j+=2
                # width of the fixup: loc 1=16bit offset,2=16bit seg,3=32bit ptr,9=32bit off...
                width={0:1,1:2,2:2,3:4,4:1,5:2,9:4}.get(loc,2)
                site=cur_ledata_off+datarec_off
                for k in range(width):
                    if site+k<len(mask): mask[site+k]=1
        elif rt in (0x90,0x91):  # PUBDEF
            g,j=rd_index(body,0); s,j=rd_index(body,j)
            if s==0: j+=2
            while j<len(body):
                n=body[j]; name=body[j+1:j+1+n].decode('latin1'); j+=1+n
                poff=struct.unpack_from("<H",body,j)[0]; j+=2
                _,j=rd_index(body,j); pubs[name]=poff
    return bytes(image), bytes(mask), pubs

def main():
    objp, pub, origp, offs = sys.argv[1:5]
    orig_off=int(offs,16)
    image, mask, pubs = build_segimage(objp)
    if pub not in pubs:
        print("pub %r not found; have: %s"%(pub, list(pubs))); sys.exit(2)
    start=pubs[pub]
    # length: next pub or arg
    if len(sys.argv)>5: ln=int(sys.argv[5],16) if sys.argv[5].startswith('0x') else int(sys.argv[5])
    else:
        laters=sorted(o for o in pubs.values() if o>start)
        ln=(laters[0]-start) if laters else (len(image)-start)
    recon=image[start:start+ln]; m=mask[start:start+ln]
    orig=open(origp,"rb").read()[orig_off:orig_off+ln]
    nfix=sum(m); diffs=[]
    for i in range(min(len(recon),len(orig))):
        if m[i]: continue
        if recon[i]!=orig[i]: diffs.append(i)
    print(f"function {pub}: {ln} bytes, {nfix} fixup-wildcarded")
    print(f"recon: {recon.hex(' ')}")
    print(f"orig : {orig.hex(' ')}")
    print(f"mask : {''.join('^^ ' if x else '.. ' for x in m)}")
    if not diffs: print("RESULT: EXACT" + (f" (modulo {nfix} fixups)" if nfix else ""))
    else: print(f"RESULT: MISMATCH at offsets {[hex(x) for x in diffs[:8]]}")

if __name__=="__main__": main()
