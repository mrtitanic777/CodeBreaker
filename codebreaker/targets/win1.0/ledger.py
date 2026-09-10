#!/usr/bin/env python3
"""
ledger.py — the coverage ledger: how much of Windows 1.01 is reconstructed.

Measures the project instead of estimating it. Two independent counts:

  FUNCTIONS  every function carved out of the binaries (the denominator) versus
             the C functions written against them, split by depth:
               transcribed — a real body, instruction-derived
               partial     — structure + evidence, body marked
                             "[documented partial]"
               prototype   — declared and described, not implemented
  BYTES      for every address the reconstruction CITES (`seg3:0x0a6f`,
             `USER.151`, …) the size of the carved function at that address,
             summed per module. This is the honest "how much of the binary has
             someone actually looked at" figure.

The address citations are the reconstruction's own comments, which is what makes
this checkable: a file that claims to reconstruct USER.151 has to name it.

Usage:
    python ledger.py            # the full tally
    python ledger.py --gaps MOD # list that module's still-uncited functions,
                                # biggest first — the work queue
"""
import glob, re, sys, collections, os
import idord, trace
from codebreaker.core import layout
from codebreaker.disasm.disasm import open_ne

SYSMODS = ["KERNEL", "GDI", "USER", "SYSTEM", "KEYBOARD",
           "MOUSE", "DISPLAY", "SOUND", "COMM"]

FNDEF = re.compile(r"^[A-Za-z_][A-Za-z0-9_ \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{", re.M)
PROTO = re.compile(r"^(?:static\s+|extern\s+)?[A-Za-z_][\w \t\*]*\b\w+\s*\([^;{]*\)\s*;\s*$", re.M)


def carve(ne):
    """{(seg, off): size} for every function in a module or app."""
    out, o = {}, trace.load_ordinals("ORDINALS.md")
    for i, s in enumerate(ne.segments(), 1):
        if s["kind"] != "CODE":
            continue
        try:
            sg = trace.Seg(ne, i, o)
        except Exception:
            continue
        for start, f in sg.funcs.items():
            out[(i, start)] = max(0, f["end"] - start)
    return out


def module_totals():
    tot = {}
    for m in SYSMODS:
        ne = idord.open_module(m)
        fns = carve(ne)
        tot[m] = dict(ne=ne, fns=fns,
                      bytes=sum(s["length"] for s in ne.segments() if s["kind"] == "CODE"))
    return tot


def classify_sources(files):
    """(transcribed, partial, prototype) counts across a set of .c files."""
    full = part = proto = 0
    for f in files:
        src = open(f, encoding="utf-8", errors="replace").read()
        for m in FNDEF.finditer(src):
            i = src.index("{", m.start()); d = 0; j = i
            while j < len(src):
                if src[j] == "{": d += 1
                elif src[j] == "}":
                    d -= 1
                    if d == 0: break
                j += 1
            body = src[i:j + 1]
            code = re.sub(r"/\*.*?\*/", "", body, flags=re.S).strip()
            stmts = [x for x in re.split(r"[;{}]", code) if x.strip()]
            if "documented partial" in body.lower() or len(stmts) <= 1:
                part += 1
            else:
                full += 1
        # A PLACEHOLDER IS NOT A PROTOTYPE.
        #
        # `Sub_0xNNNN()` (fixrawcalls.py) and `Call_MODULE_ord()` say only that
        # something lives at an address — that is the naming convention for a
        # call with NO confirmed identity, the opposite of understanding. When
        # 50 of them were declared, this count rose by 36 and the "addressed"
        # figure rose with it, reporting progress that had not happened.
        for p in PROTO.findall(src):
            if re.search(r"\b(Sub_0x[0-9a-fA-F]+|Call_[A-Z]+_\d+|Call_0x[0-9a-fA-F]+)\s*\(", p):
                continue
            proto += 1
    return full, part, proto


def cited_addresses(tot):
    """Addresses the reconstruction names, as {module: {(seg, off)}}.

    Two citation forms are honoured: an explicit `segN:0xHHHH`, and an ordinal
    `MOD.N` (resolved through the entry table). Both appear throughout the
    reconstruction's comments.
    """
    cites = collections.defaultdict(set)
    # DELIBERATELY EXCLUDES reconstructed/auto/. Those are massgen.py
    # skeletons: mechanically true about calls and arguments, but nobody has
    # read them. Counting them here would overstate understanding: a skeleton
    # is not a reconstruction, so they get their own line at the bottom of
    # this report instead.
    files = glob.glob(os.path.join(layout.recon_dir(), "*.c")) + glob.glob(os.path.join(layout.recon_sub("system"), "*.c"))
    for f in files:
        src = open(f, encoding="utf-8", errors="replace").read()
        mods = set(re.findall(r"\b(KERNEL|GDI|USER|SYSTEM|KEYBOARD|MOUSE|DISPLAY|SOUND|COMM)\.\d+", src))
        for mo in re.finditer(r"\b(KERNEL|GDI|USER|SYSTEM|KEYBOARD|MOUSE|DISPLAY|SOUND|COMM)\.(\d+)", src):
            m, ordn = mo.group(1), int(mo.group(2))
            if m not in tot:
                continue
            seg, off = trace.find_seg_of_entry(tot[m]["ne"], ordn)
            if seg is not None:
                cites[m].add((seg, off))
        # a bare segN:0xHHHH belongs to whichever module the file is about
        owner = None
        for m in tot:
            if m in mods:
                owner = m if owner is None else owner
        base = f.replace("\\", "/").split("/")[-1]
        for pfx, m in (("kernel_", "KERNEL"), ("gdi_", "GDI"), ("user_", "USER"),
                       ("comm", "COMM"), ("keyboard", "KEYBOARD"), ("mouse", "MOUSE"),
                       ("sound", "SOUND"), ("system", "SYSTEM"), ("display", "DISPLAY")):
            if base.startswith(pfx):
                owner = m
        if owner:
            for mo in re.finditer(r"\bseg(\d+):0x([0-9a-fA-F]{2,4})", src):
                cites[owner].add((int(mo.group(1)), int(mo.group(2), 16)))
            # the driver files cite their own module's ordinals as "@N"
            for mo in re.finditer(r"@(\d+)\b", src):
                seg, off = trace.find_seg_of_entry(tot[owner]["ne"], int(mo.group(1)))
                if seg is not None:
                    cites[owner].add((seg, off))
    return cites


def main():
    tot = module_totals()
    cites = cited_addresses(tot)

    if "--gaps" in sys.argv:
        m = sys.argv[sys.argv.index("--gaps") + 1].upper()
        seen = cites.get(m, set())
        rows = sorted(((sz, seg, off) for (seg, off), sz in tot[m]["fns"].items()
                       if (seg, off) not in seen), reverse=True)
        print(f"# {m}: {len(rows)} functions not yet cited by any reconstruction, "
              f"largest first")
        for sz, seg, off in rows[:40]:
            print(f"  seg{seg:<3}:0x{off:04x}  {sz:6d} B")
        return

    print("SYSTEM MODULES        fns   cited    code B    cited B   byte cover")
    tf = tc = tb = tcb = 0
    for m in SYSMODS:
        fns = tot[m]["fns"]; seen = cites.get(m, set())
        hit = [k for k in fns if k in seen]
        cb = sum(fns[k] for k in hit)
        tf += len(fns); tc += len(hit); tb += tot[m]["bytes"]; tcb += cb
        print(f"  {m:9s} {len(fns):7d} {len(hit):7d} {tot[m]['bytes']:9d} {cb:10d}"
              f" {100*cb/max(1,tot[m]['bytes']):9.1f}%")
    print(f"  {'TOTAL':9s} {tf:7d} {tc:7d} {tb:9d} {tcb:10d} {100*tcb/tb:9.1f}%")

    sysc = classify_sources(glob.glob(os.path.join(layout.recon_sub("system"), "*.c")))
    appc = classify_sources(glob.glob(os.path.join(layout.recon_dir(), "*.c")))
    appfns = 0
    for f in glob.glob("files/WF/*.EXE"):
        try:
            appfns += len(carve(open_ne(f)))
        except Exception:
            pass
    print("\nWRITTEN C            transcribed  partial  proto  addressed   of layer")
    for nm, c, den in (("SYSTEM", sysc, tf), ("APPS", appc, appfns)):
        a = sum(c)
        print(f"  {nm:9s} {c[0]:14d} {c[1]:8d} {c[2]:6d} {a:10d} {100*a/max(1,den):9.1f}%")
    a = sum(sysc) + sum(appc); den = tf + appfns
    print(f"  {'TOTAL':9s} {sysc[0]+appc[0]:14d} {sysc[1]+appc[1]:8d} "
          f"{sysc[2]+appc[2]:6d} {a:10d} {100*a/den:9.1f}%")

    o = trace.load_ordinals("ORDINALS.md")
    ident = lambda v: v and re.match(r"^[A-Za-z_]\w*$", v)
    named = len([k for k, v in o.items() if ident(v)])
    raw = sum(len(re.findall(r"Call_[A-Z]+_\d+\(", open(f, encoding="utf-8", errors="replace").read()))
              for f in glob.glob(os.path.join(layout.recon_dir(), "*.c")) + glob.glob(os.path.join(layout.recon_sub("system"), "*.c")))
    print(f"\nORDINALS named: {named}    raw call markers left: {raw}"
          f"  ({100*(1231-raw)/1231:.1f}% of app call sites resolved)")


if __name__ == "__main__":
    main()


def auto_coverage():
    """Report the massgen.py skeletons SEPARATELY, and never as progress."""
    import os
    tot = read = 0
    for f in glob.glob(os.path.join(layout.generated_dir(), "*.c")):
        src = open(f, encoding="utf-8", errors="replace").read()
        # anchored to line start: the marker also appears in each
        # function's `return` comment, which would double the count
        n = len(re.findall(r"^/\* \[AUTO\] ", src, re.M))
        tot += n
        # a skeleton promoted to a real reconstruction has its [AUTO] removed
        read += len(re.findall(r"^/\* (?!\[AUTO\])\w+  \w+ seg", src, re.M))
    if tot:
        print()
        print("AUTO skeletons (massgen.py): %d generated, %d promoted by a human"
              % (tot, read))
        print("  These are NOT counted as reconstructed. They carry the binary's"
              " true call")
        print("  graph, argument counts and global usage — everything except"
              " understanding.")


def lift_coverage():
    """Report the lift.py mechanical transliterations SEPARATELY.

    reconstructed/auto_lift/ is a subdirectory, so the `reconstructed/*.c` and
    `reconstructed/system/*.c` globs used everywhere above never see it — it is
    excluded from every transcribed/addressed/cited count by construction, the
    same way reconstructed/auto/ is. This line is the only place it is counted,
    and it is counted as exactly what it is.
    """
    tot = 0
    for f in glob.glob(os.path.join(layout.recon_sub("auto_lift"), "*.c")):
        src = open(f, encoding="utf-8", errors="replace").read()
        tot += len(re.findall(r"^long \w+_s\d+_[0-9a-f]+\(", src, re.M))
    if tot:
        print()
        print("LIFTED (lift.py): %d functions mechanically transliterated to C" % tot)
        print("  reconstructed/auto_lift/ — DERIVED per-instruction, not interpreted.")
        print("  Compilable C with neutral names (loc_N, arg1, sub_XXXX). NOT counted")
        print("  as transcribed: a function graduates only when a human names it.")


if __name__ == "__main__":
    try:
        auto_coverage()
        lift_coverage()
    except Exception:
        pass
