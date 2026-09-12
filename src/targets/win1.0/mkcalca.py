#!/usr/bin/env python3
"""
mkcalca.py -- carve CALC's byte-exact ASM math/FP subsystem out of the masmlift
oracle (reconstructed/calc.asm) into a standalone, linkable MASM module
reconstructed/calc_fp_asmref.asm.

CALC = C (Windows UI) + one hand-assembly math module, exactly like CARDFILE
(C + INDOS.ASM). masmlift proved the WHOLE seg1 reassembles byte-exact; this
tool extracts just the ASM subsystem so it can be assembled as its own OBJ and
LINK4-merged into _TEXT beside the C (the CARDFILE/CLOCK model).

The subsystem set S (computed the same way as the boundary analysis): every
no-frame function, plus every function that calls the FP core, plus their
no-frame callees (closure). Calls from S to a C function become `extrn L_x:near`;
S functions the C calls become `public L_x`. Labels stay L_XXXX for now -- the
C<->ASM NAME mapping (extrn/public renamed to C symbols) is the wiring step.

Byte-exactness is inherited from the oracle: every emitted line is copied
verbatim from calc.asm (which masmlift --exact verified against the shipped bytes).
"""
import cfg, trace as T, struct, json, sys, re

def compute():
    ne = cfg.open_any("CALC"); ords = T.load_ordinals("ORDINALS.md")
    sg = T.Seg(ne, 1, ords); code = sg.code
    allf = sorted(sg.funcs)
    def frame(a):
        b = code[a:a+3]
        return len(b) >= 3 and b[0] == 0x55 and b[1] == 0x8b and b[2] == 0xec
    def endof(a):
        i = allf.index(a); return allf[i+1] if i+1 < len(allf) else len(code)
    FPCORE = {0x240e,0x271e,0x26be,0x275b,0x27ca,0x2575,0x2e4e,0x31b9}
    def owner(addr):
        cand = [f for f in allf if f <= addr < endof(f)]
        return cand[0] if cand else None
    def callees(a):
        """near-call (e8) targets that are function entries."""
        i, e, out = a, endof(a), set()
        while i < e-2:
            if code[i] == 0xe8:
                t = (i+3+struct.unpack('<h', code[i+1:i+3])[0]) & 0xffff
                if t in sg.funcs: out.add(t)
            i += 1
        return out
    def branch_owners(a):
        """functions this one branches INTO (near/short jmp + Jcc), by owner --
        these can land mid-function, so they cannot be externed; they must be
        pulled into the module."""
        i, e, out = a, endof(a), set()
        while i < e:
            b = code[i]
            t = None
            if b == 0xe9 and i+3 <= e:                       # near jmp rel16
                t = (i+3+struct.unpack('<h', code[i+1:i+3])[0]) & 0xffff; i += 3;
            elif b == 0xeb and i+2 <= e:                     # short jmp rel8
                t = (i+2+struct.unpack('<b', code[i+1:i+2])[0]) & 0xffff; i += 2
            elif 0x70 <= b <= 0x7f and i+2 <= e:             # Jcc rel8
                t = (i+2+struct.unpack('<b', code[i+1:i+2])[0]) & 0xffff; i += 2
            else:
                i += 1; continue
            o = owner(t)
            if o is not None and not (t == o):               # a MID-function target
                out.add(o)
            elif o is not None:
                out.add(o)
        return out
    def calls_core(a):
        return bool(callees(a) & FPCORE)
    S = set(a for a in allf if not frame(a)) | set(a for a in allf if calls_core(a))
    changed = True
    while changed:
        changed = False
        for a in list(S):
            for c in callees(a):
                if c not in S and not frame(c):
                    S.add(c); changed = True
            for o in branch_owners(a):                       # follow jumps into other funcs
                if o not in S:
                    S.add(o); changed = True
    ranges = {a: (a, endof(a)) for a in S}
    externs, publics = set(), set()
    for a in S:
        for c in callees(a):
            if c not in S: externs.add(c)
    for a in allf:
        if a in S: continue
        for c in callees(a):
            if c in S: publics.add(c)
    return sorted(S), ranges, sorted(externs), sorted(publics), code

def in_ranges(off, ranges):
    for a,(lo,hi) in ranges.items():
        if lo <= off < hi: return a
    return None

def main():
    S, ranges, externs, publics, code = compute()
    total = sum(hi-lo for lo,hi in ranges.values())
    # parse the oracle: keep the header (macros) + only lines whose ; off is in S
    src = open("reconstructed/calc.asm", encoding="utf-8", errors="replace").read().splitlines()
    off_re = re.compile(r";\s*([0-9a-fA-F]{3,4})\s+[0-9a-f]")
    lbl_re = re.compile(r"^(L_[0-9a-f]+):")
    import masmlift
    entries = set(ranges.keys())          # every subsystem function entry needs a label
    emitted_lbl = set()
    out = []
    out.append("        title   calca  (CALC math/FP subsystem, byte-exact from the masmlift oracle)")
    out.append("; The hand-assembly module of CALC (software floating point, 12-byte-frame")
    out.append("; stack machine at DS:0x228). Carved from reconstructed/calc.asm by mkcalca.py.")
    out.append("; Bytes are verbatim from the oracle (masmlift --exact byte-verified).")
    out.append("")
    out += masmlift.RETF_MACRO            # MASM 4.0 has no retf mnemonic
    out.append("_TEXT   segment byte public 'CODE'")
    out.append("        assume  cs:_TEXT")
    for e in externs:
        out.append("        extrn   L_%04x:near" % e)
    for p in publics:
        out.append("        public  L_%04x" % p)
    out.append("")
    kept = 0
    for line in src:
        m = off_re.search(line)
        if m:
            off = int(m.group(1), 16)
            if in_ranges(off, ranges) is not None:
                # inject labels for subsystem ENTRIES masmlift never labeled (it only
                # labels branch/call targets; an entry reached only by an inter-module
                # call has no label). Place at the first kept line at/after the entry.
                for e in sorted(entries):
                    lbl = "L_%04x" % e
                    if lbl not in emitted_lbl and e <= off:
                        if e != off:
                            out.append("%s equ $ - %d" % (lbl, off - e))
                        else:
                            out.append("%s:" % lbl)
                        emitted_lbl.add(lbl)
                out.append(line); kept += 1
            continue
        # label lines: keep if the label is an S-function entry or falls in an S range
        lm = lbl_re.match(line.strip())
        if lm:
            addr = int(lm.group(1)[2:], 16)
            if in_ranges(addr, ranges) is not None or addr in ranges:
                out.append(line); emitted_lbl.add(lm.group(1))
            continue
        # skip everything else (other funcs' bodies, segment/end wrappers, macros)
    out.append("")
    out.append("_TEXT   ends")
    out.append("        end")
    open("reconstructed/calc_fp_asmref.asm", "w", newline="\r\n").write("\n".join(out) + "\n")
    print("calca.asm: %d funcs, %d bytes, %d instruction lines, %d extrn, %d public"
          % (len(S), total, kept, len(externs), len(publics)))

if __name__ == "__main__":
    main()
