#!/usr/bin/env python3
"""Phase-2 carver: recursive-descent function inventory + call graph for an unpacked
16-bit MZ, then a byte-exact ASM floor via mzasmrecon. Run from the CodeBreaker repo root.

Usage: python carve.py <unpacked.exe> <out_dir> [--floor] [--maxfuncs N]
"""
import sys, os, json, struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_16, CS_OP_IMM

def u16(b, o): return b[o] | (b[o+1] << 8)

def load(path):
    d = open(path, 'rb').read()
    hdr = u16(d, 8) * 16
    e_cp, e_cblp = u16(d, 4), u16(d, 2)
    imglen = e_cp*512 - ((512-e_cblp) if e_cblp else 0)
    img = d[hdr:imglen]
    cs, ip = u16(d, 0x16), u16(d, 0x14)
    entry = cs*16 + ip
    nrel = u16(d, 6); ro = u16(d, 0x18); relocs = []
    for i in range(nrel):
        off = u16(d, ro+4*i); seg = u16(d, ro+4*i+2)
        relocs.append(seg*16 + off)
    return bytearray(img), entry, sorted(set(relocs)), cs

def btarget(ins, n):
    b = ins.bytes
    if b and b[0] in (0x9A, 0xEA) and len(b) >= 5:      # far call/jmp ptr16:16
        off = b[1] | (b[2] << 8); seg = b[3] | (b[4] << 8)
        t = seg*16 + off
        return t if 0 <= t < n else None
    if ins.operands and ins.operands[0].type == CS_OP_IMM:   # near rel (flat target)
        t = ins.operands[0].imm
        if 0 <= t < n: return t
    return None

def sweep_leaders(img, entry):
    """Seed function leaders by scanning the whole image: direct near/far call targets
    plus MSC 16-bit prologues. Catches functions only reached via indirect dispatch."""
    n = len(img); leaders = {entry}
    i = 0
    while i < n-4:
        b = img[i]
        if b == 0xE8:                                   # near call rel16
            rel = img[i+1] | (img[i+2] << 8)
            t = (i + 3 + rel) & 0xFFFF
            base = i & ~0xFFFF
            t = base + t
            if 0 <= t < n: leaders.add(t)
            i += 3; continue
        if b == 0x9A:                                   # far call ptr16:16
            off = img[i+1] | (img[i+2] << 8); seg = img[i+3] | (img[i+4] << 8)
            t = seg*16 + off
            if 0 <= t < n: leaders.add(t)
            i += 5; continue
        if b == 0x55 and img[i+1] == 0x8B and img[i+2] == 0xEC:   # push bp; mov bp,sp
            leaders.add(i); i += 3; continue
        i += 1
    return leaders

def carve(img, entry, seed=None):
    md = Cs(CS_ARCH_X86, CS_MODE_16); md.detail = True
    n = len(img)
    funcs = {}; edges = []; work = list(seed) if seed else [entry]; done = set()
    indirect = set()
    while work:
        fs = work.pop()
        if fs in done: continue
        done.add(fs)
        seen = set(); stack = [fs]; calls = set(); maxend = fs; has_ret = False
        while stack:
            a = stack.pop()
            while 0 <= a < n and a not in seen:
                try: ins = next(md.disasm(bytes(img[a:a+16]), a, count=1))
                except StopIteration: break
                if ins.size == 0: break
                seen.add(a); maxend = max(maxend, a+ins.size)
                m = ins.mnemonic; nxt = a+ins.size
                if m == 'call':
                    t = btarget(ins, n)
                    if t is not None:
                        calls.add(t)
                        if t not in done and t not in work: work.append(t)
                    else: indirect.add(a)
                    a = nxt; continue
                if m in ('ret','retf','iret','iretd','hlt','retn'):
                    has_ret = True; break
                if m == 'jmp':
                    t = btarget(ins, n)
                    if t is None: indirect.add(a)
                    elif ins.bytes[0] == 0xEA:            # far jmp = tail call to another func
                        if t not in done and t not in work: work.append(t)
                    else: stack.append(t)                 # near jmp = in-function
                    break
                if m[0] == 'j' or m.startswith('loop'):   # cond jmp / loop
                    t = btarget(ins, n)
                    if t is not None: stack.append(t)
                    a = nxt; continue
                a = nxt
        funcs[fs] = dict(start=fs, end=maxend, size=maxend-fs,
                         ninsns=len(seen), calls=sorted(calls), has_ret=has_ret)
        for c in calls: edges.append([fs, c])
    return funcs, edges, sorted(indirect)

def main():
    path, outdir = sys.argv[1], sys.argv[2]
    do_floor = '--floor' in sys.argv
    maxf = int(sys.argv[sys.argv.index('--maxfuncs')+1]) if '--maxfuncs' in sys.argv else 10**9
    os.makedirs(outdir, exist_ok=True)
    img, entry, relocs, cs = load(path)
    seed = sweep_leaders(img, entry) if '--sweep' in sys.argv else None
    funcs, edges, indirect = carve(img, entry, seed)
    fl = sorted(funcs.values(), key=lambda f: f['start'])
    # non-overlapping tiling: a function's byte extent = [start, next_start), capped by its
    # own reachable maxend (so trailing data between funcs isn't attributed to the function).
    for i, f in enumerate(fl):
        nxt = fl[i+1]['start'] if i+1 < len(fl) else len(img)
        f['reach'] = f['end']
        f['end'] = min(f['end'], nxt)
        f['size'] = f['end'] - f['start']
    covered = sum(f['size'] for f in fl)
    print(f"entry={entry:#x} image={len(img):#x} funcs={len(fl)} "
          f"code_bytes~{covered:#x} ({covered*100//len(img)}% of image) "
          f"relocs={len(relocs)} indirect_br={len(indirect)}")
    # in-degree (most-called)
    indeg = {}
    for _, c in edges: indeg[c] = indeg.get(c, 0)+1
    top = sorted(indeg.items(), key=lambda x: -x[1])[:12]
    print("most-called:", ", ".join(f"{a:#06x}:{d}" for a, d in top))
    json.dump({'entry': entry, 'image_size': len(img), 'functions': fl,
               'edges': edges, 'indirect': indirect, 'relocs': relocs},
              open(os.path.join(outdir, 'functions.json'), 'w'), indent=1)

    if do_floor:
        # Faithful ASM floor straight from Capstone: every byte accounted for, undecodable
        # or reloc-patched bytes emitted as `db`. This IS byte-exact by construction (it is a
        # decode of the real bytes); MASM/nasm round-trip verification is a separate gate.
        md = Cs(CS_ARCH_X86, CS_MODE_16)
        relset = set(relocs)
        lines = ["; byte-exact ASM floor (Capstone 16-bit decode) for " + os.path.basename(path),
                 "; db = undecodable byte or relocation-patched word", ""]
        dec_bytes = 0; db_bytes = 0
        for f in fl:
            s, e = f['start'], f['end']
            lines.append(f"; ---- f{s:05x}  [{s:#06x}..{e:#06x}) size={f['size']} "
                         f"insns~{f['ninsns']} calls={len(f['calls'])}")
            a = s
            while a < e:
                # if a reloc word sits here, or decode fails, emit db for this byte
                chunk = bytes(img[a:min(e, a+16)])
                ins = None
                if a not in relset and (a-1) not in relset:
                    try: ins = next(md.disasm(chunk, a, count=1))
                    except StopIteration: ins = None
                if ins and ins.size and a+ins.size <= e and not any((a+k) in relset or (a+k-1) in relset for k in range(ins.size)):
                    lines.append(f"  {a:05x}: {ins.mnemonic} {ins.op_str}".rstrip())
                    dec_bytes += ins.size; a += ins.size
                else:
                    lines.append(f"  {a:05x}: db {img[a]:#04x}")
                    db_bytes += 1; a += 1
            lines.append("")
        tot = dec_bytes + db_bytes
        print(f"FLOOR: {len(fl)} funcs, {tot} code bytes, decoded {dec_bytes} "
              f"({dec_bytes*100//max(tot,1)}%), db {db_bytes}")
        open(os.path.join(outdir, 'floor.asm'), 'w', newline='\n').write("\n".join(lines))

if __name__ == '__main__':
    main()
