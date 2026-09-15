#!/usr/bin/env python3
"""Carve a 16-bit MZ image: recursive-descent disassembly, function inventory,
call graph, code/data split, and string harvest. No assembler required.

Usage: python carve.py <unpacked.exe> [outdir]
Writes <outdir>/{functions.txt, callgraph.txt, strings.txt, summary.json}.
"""
import struct, sys, os, json, re
from capstone import Cs, CS_ARCH_X86, CS_MODE_16

MD = Cs(CS_ARCH_X86, CS_MODE_16); MD.detail = True
NEAR_CALL, FAR_CALL = 0xE8, 0x9A
JMP_NEAR, JMP_SHORT = 0xE9, 0xEB
CFLOW_STOP = {"ret", "retf", "iret", "jmp"}
COND = {"je","jne","jz","jnz","jl","jle","jg","jge","jb","jbe","ja","jae","jc","jnc",
        "jo","jno","js","jns","jp","jnp","jcxz","loop","loope","loopne"}


def load(path):
    b = open(path, 'rb').read()
    hdr = struct.unpack('<H', b[8:10])[0] * 16
    nrel = struct.unpack('<H', b[6:8])[0]
    reloff = struct.unpack('<H', b[24:26])[0]
    ip, cs = struct.unpack('<H', b[20:22])[0], struct.unpack('<H', b[22:24])[0]
    relocs = [struct.unpack('<HH', b[reloff + i*4:reloff + i*4 + 4]) for i in range(nrel)]
    img = b[hdr:]
    return img, cs*16 + ip, relocs


def carve(img, entry, relocs):
    seen = set()            # instruction start addresses decoded
    funcs = set([entry])    # function entry points
    edges = []              # (caller_func, callee_addr)
    calls_far = set()
    worklist = [entry]
    fn_of = {}              # addr -> owning function (approx, by discovery)

    def decode_run(start, owner):
        addr = start
        while 0 <= addr < len(img) and addr not in seen:
            ins = next(MD.disasm(img[addr:addr+16], addr), None)
            if ins is None:
                break
            seen.add(addr)
            fn_of[addr] = owner
            op = ins.bytes[0]
            m = re.match(r"^0x([0-9a-fA-F]+)$", ins.op_str.strip())
            tgt = int(m.group(1), 16) if m else None
            if op == NEAR_CALL and tgt is not None:
                edges.append((owner, tgt))
                if tgt not in funcs:
                    funcs.add(tgt); worklist.append(tgt)
            elif op == FAR_CALL:
                calls_far.add(addr)
            elif ins.mnemonic in ("jmp",) and tgt is not None:
                # tail-jump within image: treat target as code, continue there
                if tgt not in seen:
                    worklist.append(("cont", tgt, owner))
                break
            elif ins.mnemonic in COND and tgt is not None:
                if tgt not in seen:
                    worklist.append(("cont", tgt, owner))
            if ins.mnemonic in CFLOW_STOP:
                break
            addr += ins.size

    while worklist:
        item = worklist.pop()
        if isinstance(item, tuple):
            _, a, owner = item
            decode_run(a, owner)
        else:
            decode_run(item, item)

    # size each function = bytes from its entry to the next function entry it flows into
    fentries = sorted(funcs)
    return fentries, edges, seen, calls_far


def code_data_split(fentries, seen, img):
    # heuristic boundary: end of the last decoded instruction reachable as code
    last_code = 0
    for a in seen:
        ins = next(MD.disasm(img[a:a+16], a), None)
        if ins:
            last_code = max(last_code, a + ins.size)
    return last_code


def strings(img, lo, hi, minlen=4):
    out = []
    for m in re.finditer(rb'[\x20-\x7e]{%d,}' % minlen, img[lo:hi]):
        out.append((lo + m.start(), m.group().decode('latin1')))
    return out


def main():
    path = sys.argv[1]
    outdir = sys.argv[2] if len(sys.argv) > 2 else os.path.join(
        "workspace", "disasm", os.path.splitext(os.path.basename(path))[0])
    os.makedirs(outdir, exist_ok=True)
    img, entry, relocs = load(path)
    fentries, edges, seen, calls_far = carve(img, entry, relocs)
    code_end = code_data_split(fentries, seen, img)
    # DGROUP hint from most common reloc target-word (segment) region
    dgroup = min((seg*16 + off for seg, off in relocs), default=0)

    # function sizes
    fe = fentries + [max(code_end, fentries[-1] + 1)]
    sizes = {fe[i]: fe[i+1] - fe[i] for i in range(len(fentries))}
    fanout = {}
    for c, t in edges:
        fanout.setdefault(c, set()).add(t)

    with open(os.path.join(outdir, "functions.txt"), "w") as f:
        f.write(f"# {os.path.basename(path)}  entry={entry:#06x}  functions={len(fentries)}\n")
        for a in fentries:
            tag = " <ENTRY>" if a == entry else ""
            f.write(f"{a:#06x}  size={sizes.get(a,0):5d}  callees={len(fanout.get(a,[])):3d}{tag}\n")
    with open(os.path.join(outdir, "callgraph.txt"), "w") as f:
        for c in sorted(fanout):
            for t in sorted(fanout[c]):
                f.write(f"{c:#06x} -> {t:#06x}\n")
    strs = strings(img, code_end, len(img))
    with open(os.path.join(outdir, "strings.txt"), "w", encoding="utf-8") as f:
        for off, s in strs:
            f.write(f"{off:#06x}\t{s}\n")

    summary = {
        "file": os.path.abspath(path), "entry": entry, "image_len": len(img),
        "functions": len(fentries), "call_edges": len(edges),
        "far_calls": len(calls_far), "code_bytes_reached": len(seen),
        "code_end": code_end, "dgroup_lin": dgroup, "strings": len(strs),
    }
    json.dump(summary, open(os.path.join(outdir, "summary.json"), "w"), indent=2)
    print(json.dumps(summary, indent=2))
    print("->", outdir)


if __name__ == "__main__":
    main()
