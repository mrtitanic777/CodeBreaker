#!/usr/bin/env python3
"""objfn.py -- extract a named function's code + fixup mask from an OMF .OBJ, and
compare (masked) against a window of a target image. The byte-exact verify primitive.

    python objfn.py <obj> <pubname>                      # dump function bytes+mask
    python objfn.py <obj> <pubname> <target.exe> <hexoff># masked-compare to image@off
"""
import sys, os, struct
CB = r"C:\Users\mrtit\Documents\GitHub\CodeBreaker"
sys.path.insert(0, CB)
from src.engines.inverse.omf import records, rd_index

def extract(objpath):
    data = open(objpath, "rb").read()
    lnames=[""]; segs=[]; pubs={}; image=bytearray(); mask=bytearray(); cur=None
    text_idx = None
    for off, rt, body in records(data, 0):
        if rt == 0x96:
            j=0
            while j<len(body): n=body[j]; lnames.append(body[j+1:j+1+n].decode('latin1')); j+=1+n
        elif rt in (0x98,0x99):
            si,j=rd_index(body,3); nm=lnames[si] if si<len(lnames) else str(si)
            segs.append(nm)
            if nm in ("_TEXT","CODE","PROG"): text_idx = len(segs)  # 1-based seg index
        elif rt in (0xa0,0xa1):
            seg,j=rd_index(body,0); do=struct.unpack_from("<H",body,j)[0]; j+=2; pl=body[j:]
            sn=segs[seg-1] if 0<seg<=len(segs) else ""
            if sn in ("_TEXT","CODE","PROG"):
                end=do+len(pl)
                if end>len(image): image.extend(b"\0"*(end-len(image))); mask.extend(b"\0"*(end-len(mask)))
                image[do:end]=pl; cur=do
            else: cur=None
        elif rt in (0x9c,0x9d):
            if cur is None: continue
            j=0
            while j<len(body):
                b0=body[j]
                if not (b0&0x80): j+=1; continue
                locat=(b0<<8)|body[j+1]; j+=2; dro=locat&0x3ff; loc=(locat>>10)&0xf
                fd=body[j]; j+=1
                if not (fd&0x80):
                    fb=(fd>>4)&7
                    if fb in (0,1,2): _,j=rd_index(body,j)
                if not (fd&0x08): _,j=rd_index(body,j)
                if not ((fd>>2)&1): j+=2
                w={0:1,1:2,2:2,3:4,4:1,5:2,9:4}.get(loc,2)
                for k in range(w):
                    if cur+dro+k<len(mask): mask[cur+dro+k]=1
        elif rt in (0x90,0x91):
            g,j=rd_index(body,0); s,j=rd_index(body,j)
            if s==0: j+=2
            while j<len(body):
                n=body[j]; nm=body[j+1:j+1+n].decode('latin1'); j+=1+n
                po=struct.unpack_from("<H",body,j)[0]; j+=2; _,j=rd_index(body,j)
                if s == text_idx:            # only public *code* symbols
                    pubs[nm]=po
    return image, mask, pubs

def fn_bytes(objpath, name):
    image, mask, pubs = extract(objpath)
    if name not in pubs:
        cands = [k for k in pubs if name in k]
        raise SystemExit(f"'{name}' not in pubs: {sorted(pubs)}; near={cands}")
    starts = sorted(pubs.values())
    po = pubs[name]; idx = starts.index(po)
    end = starts[idx+1] if idx+1 < len(starts) else len(image)
    return bytes(image[po:end]), bytes(mask[po:end]), po

if __name__ == "__main__":
    obj, name = sys.argv[1], sys.argv[2]
    code, m, po = fn_bytes(obj, name)
    print(f"{name} @obj:{po:#x} len={len(code)} fixups={sum(m)}")
    print("  code:", code.hex())
    print("  mask:", "".join("1" if b else "." for b in m))
    if len(sys.argv) >= 5:
        tgt = open(sys.argv[3], "rb").read()
        hdr = struct.unpack_from("<H", tgt, 8)[0]*16
        off = int(sys.argv[4], 16)
        win = tgt[hdr+off: hdr+off+len(code)]
        diffs = [i for i in range(len(code)) if not m[i] and i < len(win) and code[i]!=win[i]]
        print(f"  target@{off:#x}: {win.hex()}")
        if len(win) < len(code):
            print(f"  LENGTH MISMATCH: obj {len(code)} vs window {len(win)}")
        elif not diffs:
            print(f"  *** BYTE-EXACT MATCH (modulo {sum(m)} fixup bytes) ***")
        else:
            print(f"  {len(diffs)} mismatches at (non-fixup) offsets: "
                  + " ".join(f"{d:#x}(obj {code[d]:02x}/tgt {win[d]:02x})" for d in diffs[:16]))
