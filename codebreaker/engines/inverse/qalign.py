#!/usr/bin/env python3
"""qalign.py - align a quad file and its compiled object per-function, yielding
(name, quad_records, code_bytes) triples: the ground-truth (input,output) pairs of
the MC2 code generator. Foundation for mc2model (forward quad->bytes model).

Usage: python qalign.py <corp.q/corpq.bin> <corp.OBJ>
"""
import sys, struct
from codebreaker.engines.inverse.omf import records, rd_index

def parse_quads(qpath):
    """Return list of functions, each = list of (opcode, payload_bytes)."""
    d = open(qpath, "rb").read()
    # name pool at tail
    i = len(d)
    while i > 0 and (32 <= d[i-1] < 127 or d[i-1] == 0): i -= 1
    end = i
    funcs = []; cur = None
    o = 0x5c
    while o + 2 <= end:
        op, ln = d[o], d[o+1]
        if op == 0xff: break
        rec = (op, d[o+2:o+2+ln])
        if op == 0x23:  # FUNC-BEGIN
            cur = []; funcs.append(cur)
        if cur is not None: cur.append(rec)
        o += 2 + ln
    return funcs

def parse_obj_funcs(objpath):
    """Return [(name, off, bytes)] for PROG functions, in address order."""
    data = open(objpath,"rb").read()
    lnames=[""]; segs=[]; code=b""; pubs=[]
    for off,rt,body in records(data):
        if rt==0x96:
            j=0
            while j<len(body): n=body[j]; lnames.append(body[j+1:j+1+n].decode('latin1')); j+=1+n
        elif rt in (0x98,0x99):
            si,j=rd_index(body,3); segs.append(lnames[si] if si<len(lnames) else str(si))
        elif rt in (0xa0,0xa1):
            seg,j=rd_index(body,0); do=struct.unpack_from("<H",body,j)[0]; j+=2; pl=body[j:]
            sn=segs[seg-1] if 0<seg<=len(segs) else ""
            if sn=="PROG":
                if do+len(pl)>len(code): code=code+b"\x00"*(do+len(pl)-len(code))
                code=code[:do]+pl+code[do+len(pl):]
        elif rt in (0x90,0x91):
            g,j=rd_index(body,0); s,j=rd_index(body,j)
            if s==0: j+=2
            while j<len(body):
                n=body[j]; nm=body[j+1:j+1+n].decode('latin1'); j+=1+n
                po=struct.unpack_from("<H",body,j)[0]; j+=2; _,j=rd_index(body,j)
                pubs.append((nm,po))
    pubs=[p for p in pubs if not p[0].isupper() or True]
    pubs.sort(key=lambda x:x[1])
    out=[]
    for i,(nm,po) in enumerate(pubs):
        end=pubs[i+1][1] if i+1<len(pubs) else len(code)
        out.append((nm, po, bytes(code[po:end])))
    return out

def main():
    quads=parse_quads(sys.argv[1])
    objf=parse_obj_funcs(sys.argv[2])
    # obj pubs include data symbols; keep those whose bytes look like a function (start 55/53/8b)
    objf=[f for f in objf if f[2][:1] in (b"\x55",b"\x53",b"\x8b",b"\xb8")]
    print("quad funcs: %d | obj funcs: %d"%(len(quads),len(objf)))
    for i,(nm,po,code) in enumerate(objf):
        q = quads[i] if i<len(quads) else []
        ops=" ".join("%02x"%op for op,_ in q)
        print("\n=== %s  (%d bytes) ==="%(nm,len(code)))
        print("  quads: "+ops)
        print("  code : "+code.hex(' '))

if __name__=="__main__": main()
