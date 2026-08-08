#!/usr/bin/env python3
"""
mfn.py — disassemble ONE ordinal (or address) of a system module, annotated.

The missing convenience tool: trace.py drives off an app .EXE path, but the
system modules live inside files/WF/WIN100.BIN and must be opened with
idord.open_module (base=0) — or, for the mis-located USER/GDI high segments,
segfix.open_module_fixed. This wires those readers into trace.Seg so every
module ordinal can be read with the same annotated output (resolved imports,
DOS AH flags, internal call targets) used for the apps.

Usage:
    python mfn.py MOD ORD              # disassemble the ordinal's body
    python mfn.py MOD 0xADDR SEG       # disassemble an address in a segment
    python mfn.py MOD --entry ORD      # just print the entry-table seg:offset
    python mfn.py MOD --list SEG       # list carved functions in a segment
    python mfn.py MOD --at ORD [N]     # RESYNCED disassembly from the entry
    python mfn.py MOD --brief O O O    # one evidence line per ordinal
    python mfn.py MOD --cskel ORD      # emit a ready-to-refine C skeleton
  options:
    --fixed        open via segfix (DEPRECATED — see idord.open_module)
    --calls        print the resolved call list only (no body)
    --wnd/--dc/--q annotate struct field accesses from reconstructed/win1.h
"""
import sys, re, os
import idord, trace
from engine.foundation import layout


def openmod(mod, fixed):
    if fixed:
        import segfix
        return segfix.open_module_fixed(mod)
    return idord.open_module(mod)


def load_structs(path=None):
    """Parse win1.h's `/* 0xNN */ type name;` lines into
    {STRUCT: {offset: field}} so a raw `[si+0x33]` can be annotated as
    `WND.state`. The header is the single source of truth for every offset the
    project has recovered; this makes the skeleton emitter inherit it for free."""
    if path is None:                       # honor the migrated src/ tree
        path = os.path.join(layout.recon_dir(), "win1.h")
    structs, cur = {}, None
    try:
        src = open(path, encoding="utf-8", errors="replace").read()
    except OSError:
        return structs
    for line in src.splitlines():
        m = re.match(r"\s*typedef struct tag(\w+)", line)
        if m:
            cur = m.group(1).upper(); structs[cur] = {}
            continue
        if cur and line.startswith("}"):
            cur = None; continue
        if cur:
            m = re.match(r"\s*/\* (0x[0-9a-f]+) \*/\s+[\w \*]+?\**(\w+)\s*[;\[]", line)
            if m and not m.group(2).startswith("res"):
                structs[cur][int(m.group(1), 16)] = m.group(2)
    return structs


def annotate_fields(asm, structs, hint):
    if hint is None:
        return ""
    """Append `; WND.state` style notes to an instruction that touches a struct
    field. `hint` picks which structure to read the offsets against."""
    tbl = structs.get(hint)
    if not tbl:
        return ""
    out = []
    for m in re.finditer(r"\[(?:si|di|bx)\+(0x[0-9a-f]+)\]", asm):
        f = tbl.get(int(m.group(1), 16))
        if f:
            out.append(f"{hint}.{f}")
    return ("   -- " + ", ".join(dict.fromkeys(out))) if out else ""


def emit_cskel(mod, ne, ordinals, rest, argv):
    """Emit a ready-to-refine C skeleton for ANY system function.

    trace.py's --cskel only understands window procedures in applications. This
    one works on any module routine: it recovers the PASCAL argument count from
    the `retf N`, lists every call in order resolved to its real name, keeps the
    branch structure as labelled comments, and annotates struct field accesses
    from win1.h. What is left for a human is the logic — which is the part that
    actually needs judgement.
    """
    def int0(x): return int(x, 0)
    tgt = rest[0]
    ordn = None
    if tgt.startswith("0x"):
        start = int0(tgt); seg = int0(rest[1]) if len(rest) > 1 else 1
    else:
        ordn = int0(tgt)
        seg, start = trace.find_seg_of_entry(ne, ordn)
        if seg is None:
            print(f"ordinal {tgt} not in {mod} entry table"); sys.exit(1)
    # Field annotation is OPT-IN: applying the WND layout to a function that
    # is really walking a menu block would invent structure that is not there.
    hint = None
    if "--wnd" in argv: hint = "WND"
    if "--dc" in argv:  hint = "DC"
    if "--q" in argv:   hint = "MSGQUEUE"
    structs = load_structs()

    sg = trace.Seg(ne, seg, ordinals)
    rows = trace.disasm_whole(sg.code[start:], base=start)

    # --- pass 1: find the end, the arg bytes, the branch targets ------------
    end, argbytes, targets, maxslot = None, 0, set(), 0
    for off, asm in rows:
        m = re.match(r"j\w+\s+0x([0-9a-f]+)$", asm)
        if m: targets.add(int(m.group(1), 16))
        m = re.match(r"retf?\s+0x([0-9a-f]+)", asm)
        if m: argbytes = int(m.group(1), 16)
        for mm in re.finditer(r"\[bp\+(0x[0-9a-f]+)\]", asm):
            maxslot = max(maxslot, int(mm.group(1), 16))
        if asm.startswith("ret"):
            end = off; break
    nargs = argbytes // 2
    # Functions that go through USER's shared far-proc prolog end in a plain
    # `ret` — the prolog owns the `retf N` — so retf tells us nothing. Fall back
    # to the highest [bp+N] frame slot the body reads: arguments start at
    # [bp+0x06], so slot 0x0e means (0x0e-6)/2 + 1 = 5 words.
    if argbytes == 0 and maxslot >= 6:
        nargs = (maxslot - 6) // 2 + 1
        argnote = f"no retf N (shared prolog); {nargs} args inferred from frame slots up to [bp+0x{maxslot:02x}]"
    else:
        argnote = f"retf {argbytes} => {nargs} word argument(s), PASCAL order"
    img = ne.segments()[seg-1].get("image", "?")
    name = ordinals.get(f"{mod}.{ordn}") if ordn is not None else None

    print(f"/* AUTO-SKELETON  mfn.py --cskel   {mod}.{ordn}  "
          f"seg{seg}[{img}]:0x{start:04x}..0x{(end or 0):04x}")
    print(f" * {argnote}.")
    print(" * Calls resolved below are the binary's; the logic is not written yet.")
    print(" */")
    args = ", ".join(f"WORD a{i+1}" for i in range(nargs)) or "void"
    print(f"LONG FAR PASCAL {name or f'{mod}_{ordn}'}({args})\n{{")

    for off, asm in rows:
        if end is not None and off > end:
            break
        if off in targets:
            print(f"L_{off:04x}:")
        note = ""
        if asm.startswith("call") or asm.startswith("jmp"):
            k, d = sg.resolve_call(off, asm)
            if k == "sys":
                lbl, nm = d
                note = f"    {nm or lbl}(...);"
                if not nm: note += f"   /* {lbl} — UNIDENTIFIED, leave raw */"
            elif k == "int":  note = f"    sub_{d:04x}(...);"
            elif k == "far":  note = f"    /* -> {d} */"
        m = re.match(r"mov    ax,0x([0-9a-f]+)$", asm)
        if m and int(m.group(1), 16) in trace.WM:
            note = f"    /* message {trace.WM[int(m.group(1),16)]} */"
        m = re.match(r"mov    ah,0x([0-9a-f]+)", asm)
        if m and int(m.group(1), 16) in trace.DOS21:
            note = f"    /* DOS {trace.DOS21[int(m.group(1),16)]} */"
        if note:
            print(f"{note:<44}/* {off:04x}: {asm} */")
        else:
            fld = annotate_fields(asm, structs, hint)
            if fld or re.match(r"(cmp|test|and|or|shl|shr|add|sub)\s", asm):
                print(f"    /* {off:04x}: {asm}{fld} */")
    print("    return 0;   /* [documented partial — fill in the logic] */")
    print("}")


def main():
    a = sys.argv[1:]
    if not a:
        print(__doc__); sys.exit(1)
    mod = a[0].upper()
    fixed = "--fixed" in a
    rest = [x for x in a[1:] if not x.startswith("--")]
    ne = openmod(mod, fixed)
    ordinals = trace.load_ordinals("ORDINALS.md")

    def int0(x): return int(x, 0)

    if "--entry" in a:
        for t in rest:
            print(mod, t, trace.find_seg_of_entry(ne, int0(t)))
        return
    if "--list" in a:
        seg = int0(rest[0]) if rest else 1
        sg = trace.Seg(ne, seg, ordinals)
        print(f"# {len(sg.funcs)} functions in {mod} seg{seg}\n")
        for s in sorted(sg.funcs):
            trace.print_function(sg, s, body=False)
        return

    if "--cskel" in a:
        emit_cskel(mod, ne, ordinals, rest, a)
        return

    if "--at" in a:
        # RESYNCED disassembly from an exact entry point.
        #
        # trace.Seg disassembles a whole segment linearly from offset 0. Any data
        # embedded in the code stream (jump tables, the 2-byte descriptors of the
        # shared-prolog convention, string literals) desynchronises the decoder
        # for everything after it, which is why some entry offsets report "not an
        # instruction boundary" — the linear pass never landed on them. Starting
        # the decode AT the entry offset instead is always correct, because an
        # exported entry point is by definition the first byte of an instruction.
        tgt = rest[0]
        ordn = None
        if tgt.startswith("0x"):
            start = int0(tgt); seg = int0(rest[1]) if len(rest) > 1 else 1
        else:
            ordn = int0(tgt)
            seg, start = trace.find_seg_of_entry(ne, ordn)
            if seg is None:
                print(f"ordinal {tgt} not in {mod} entry table"); sys.exit(1)
        n = int0(a[a.index("--at") + 1]) if len(a) > a.index("--at") + 1 \
            and a[a.index("--at") + 1].lstrip("-").isdigit() else 60
        sg = trace.Seg(ne, seg, ordinals)
        code = sg.code[start:]
        rows = trace.disasm_whole(code, base=start)
        img = ne.segments()[seg-1].get("image", "?")
        print(f"# {mod}.{ordn if ordn is not None else '?'} seg{seg}[{img}]:0x{start:04x}"
              f"  (resynced from the entry point)")
        for off, asm in rows[:n]:
            line = f"    {off:04x}:  {asm}"
            if asm.startswith("call") or asm.startswith("jmp"):
                k, d = sg.resolve_call(off, asm)
                if k == "sys":   line += f"      ; {d[0]}" + (f" = {d[1]}" if d[1] else "")
                elif k == "far": line += f"      ; -> {d}"
                elif k == "int": line += f"      ; -> 0x{d:04x}"
            m = re.match(r"mov    ah,0x([0-9a-f]+)", asm)
            if m and int(m.group(1), 16) in trace.DOS21:
                line += f"      ; DOS {trace.DOS21[int(m.group(1),16)]}"
            m = re.match(r"mov    ax,0x([0-9a-f]+)$", asm)
            if m and int(m.group(1), 16) in trace.WM:
                line += f"      ; {trace.WM[int(m.group(1),16)]}"
            print(line)
            if asm.startswith("ret"):
                break
        return

    if "--brief" in a:
        # several ordinals at once, one compact line each: the shape of the body
        # (calls resolved to names, message/DOS constants, struct offsets it
        # touches, and its retf) — enough to recognise most APIs on sight.
        import re as _re
        segs = {}
        for t in rest:
            ordn = int0(t)
            seg, off = trace.find_seg_of_entry(ne, ordn)
            if seg is None:
                print(f"{mod}.{ordn}: no entry"); continue
            if seg not in segs:
                try: segs[seg] = trace.Seg(ne, seg, ordinals)
                except Exception as e: segs[seg] = None
            sg = segs[seg]
            if sg is None or off not in sg.off2idx:
                print(f"{mod}.{ordn} seg{seg}:0x{off:04x}: not an instruction boundary")
                continue
            out = []
            for offs, asm in sg.rows[sg.off2idx[off]:][:90]:
                if asm.startswith("call") or asm.startswith("jmp"):
                    k, d = sg.resolve_call(offs, asm)
                    if k == "sys":   out.append("call " + (d[1] or d[0]))
                    elif k == "far": out.append("call " + str(d))
                elif _re.match(r"mov    a[xhl],0x", asm) or \
                     _re.search(r"(ds:0x[0-9a-f]+|\[(si|di|bx)\+0x[0-9a-f]+\])", asm) or \
                     _re.match(r"(cmp|test|and|or)\s", asm):
                    out.append(asm)
                if asm.startswith("ret"):
                    out.append("**" + asm + "**"); break
            img = ne.segments()[seg-1].get("image", "?")
            print(f"\n== {mod}.{ordn}  seg{seg}[{img}]:0x{off:04x}")
            print("   " + " | ".join(out[:16]))
        return

    tgt = rest[0]
    if tgt.startswith("0x"):
        seg = int0(rest[1]) if len(rest) > 1 else 1
        start = int0(tgt)
    else:
        seg, start = trace.find_seg_of_entry(ne, int0(tgt))
        if seg is None:
            print(f"ordinal {tgt} not in {mod} entry table"); sys.exit(1)
        print(f"# {mod}.{tgt} -> seg{seg}:0x{start:04x}")
    sg = trace.Seg(ne, seg, ordinals)
    if "--calls" in a:
        for row in sg.calls_in(start):
            print(row)
        return
    trace.print_function(sg, start, body=True)


if __name__ == "__main__":
    main()
