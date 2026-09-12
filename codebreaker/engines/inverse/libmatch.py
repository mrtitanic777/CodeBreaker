#!/usr/bin/env python3
"""libmatch.py - identify LC1/LC2 functions that ARE runtime-library code, by
matching every function in a .LIB (modulo relocations) against the linked binary.

A linked EXE's library functions are byte-identical to their .LIB source except at
fixup sites (which the linker resolved). So: for each library function, wildcard its
FIXUPP sites and search the target image for a same-length window that matches on all
non-wildcard bytes. A hit NAMES that target function and proves it needs no source
reconstruction (just link the lib).

Usage: python libmatch.py <lib> <target_image.bin> <ghidra.json>
"""
import sys, struct, json
from codebreaker.engines.inverse.omf import records, rd_index

def iter_modules(data):
    """Yield mstart offsets for each library module, page-aligned (OMF .LIB)."""
    if data[0]!=0xF0:
        yield 0; return
    pagelen=struct.unpack_from("<H",data,1)[0]+3
    i=pagelen
    while i<len(data):
        if data[i]==0xF1: break
        if data[i]!=0x80: i+=1; continue
        yield i
        # advance to end of this module (MODEND), then page-align
        end=i
        for off,rt,body in records(data,i):
            end=off+3+len(body)+1
            if rt in (0x8a,0x8b): break
        i=((end+pagelen-1)//pagelen)*pagelen

def module_functions(data, mstart):
    """Build the PROG image+mask+pubs for ONE module beginning at mstart; return
    list of (pubname, code_bytes, mask_bytes)."""
    lnames=[""]; segs=[]; pubs={}
    image=bytearray(); mask=bytearray(); cur=None
    for off,rt,body in records(data, mstart):
        if rt==0x80 and off!=mstart: break
        if rt==0x96:
            j=0
            while j<len(body): n=body[j]; lnames.append(body[j+1:j+1+n].decode('latin1')); j+=1+n
        elif rt in (0x98,0x99):
            si,j=rd_index(body,3); segs.append(lnames[si] if si<len(lnames) else str(si))
        elif rt in (0xa0,0xa1):
            seg,j=rd_index(body,0); do=struct.unpack_from("<H",body,j)[0]; j+=2; pl=body[j:]
            sn=segs[seg-1] if 0<seg<=len(segs) else ""
            if sn in ("PROG","CODE","BASE"):
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
                pubs[nm]=po
        elif rt in (0x8a,0x8b): break
    # split into functions by pub offsets
    out=[]
    if pubs:
        items=sorted(pubs.items(), key=lambda x:x[1])
        for i,(nm,po) in enumerate(items):
            end=items[i+1][1] if i+1<len(items) else len(image)
            if end>po: out.append((nm, bytes(image[po:end]), bytes(mask[po:end])))
    return out

def masked_eq(a, m, b):
    if len(a)!=len(b): return False
    for i in range(len(a)):
        if not m[i] and a[i]!=b[i]: return False
    return True

def main():
    libp, imgp, ghp = sys.argv[1:4]
    data=open(libp,"rb").read(); img=open(imgp,"rb").read()
    ghfns=json.load(open(ghp))
    # index target functions by (offset,size)
    tf=[]
    for f in ghfns:
        o=int(f['entry'],16)-0x10000
        if 0<=o<len(img): tf.append((o,f['size'],f['name']))
    # collect library functions
    libfns=[]
    for ms in iter_modules(data):
        for nm,code,mask in module_functions(data, ms):
            if len(code)>=6 and code[0] in (0x55,0x53,0x56,0x57,0x8b,0xb8,0xe9,0x2e):
                libfns.append((nm,code,mask))
    # slide-search: find each library function's masked pattern anywhere in the image
    hits={}  # image_off -> (name, len, nfixups)
    for nm,code,mask in libfns:
        L=len(code); c0=code[0]
        pos=img.find(bytes([c0]))
        while pos!=-1 and pos+L<=len(img):
            if masked_eq(code,mask,img[pos:pos+L]):
                if pos not in hits or L>hits[pos][1]:
                    hits[pos]=(nm,L,sum(mask))
            pos=img.find(bytes([c0]), pos+1)
    # coverage: which Ghidra functions start at a library hit?
    named={}
    for o,s,tn in tf:
        if o in hits: named[o]=hits[o]
    # also compute total library bytes covered
    covered=bytearray(len(img))
    for pos,(nm,L,nf) in hits.items():
        for k in range(L): covered[pos+k]=1
    print(f"library {libp}: {len(libfns)} funcs extracted; {len(hits)} placements found in image")
    for o in sorted(named):
        nm,ln,nf=named[o]; print(f"  0x{o+0x10000:05x}  {nm:<12} {ln:4d}B  ({nf} fixups)")
    print(f"\nSUMMARY: {len(named)}/{len(tf)} Ghidra functions matched a LIBRARY function by name")
    print(f"         {sum(covered)} image bytes ({100*sum(covered)//len(img)}%) are identified library code")
    print(f"         {len(tf)-len(named)} Ghidra functions remain = compiler-original")

if __name__=="__main__": main()
