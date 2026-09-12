#!/usr/bin/env python3
r"""matchbatch.py — Phase-0 byte-match CENSUS for the "compile our C" project.

The one-function proof (matchdecomp candidate -> hand-template -> oracle) showed the loop
closes. This scales it: auto-generate C candidates for a whole CLASS of functions, compile
them in CHUNKS through the period compiler (cc32/MSVC 4.2), byte-compare each against the
shipped bytes, and report the match rate + a categorized histogram of the MISSES. The miss
categories ARE the raiser's prioritized worklist — each names a codegen fact the raiser must
recover (calling convention, return type, frame, spill) to turn a miss into a match.

    wt --target win95 matchbatch                 # census over the identity-function class
    wt --target win95 matchbatch --limit 2000    # cap the sample
    wt --target win95 matchbatch --chunk 200     # functions per cc32 compile (default 200)

Starts with the IDENTITY class (body reduces to `return argN`) — the easiest on-ramp, where
a correct signature alone should match. Misses here isolate pure codegen effects (frame,
cdq/xor-edx for wide returns, stack adjust). Extend `_collect` with new classes as the
raiser grows.
"""
import glob
import os
import re
import sys

from codebreaker.core import layout


def shipped_bytes(asm_path):
    """Function bytes from a per-function .asm — authoritative shipped bytes. Handles
    BOTH emitted forms objextract/masmlift use:
      * instruction lines `<insn> ; <offhex> bb bb ...`  — bytes in the comment
      * data/abs-addr lines `db 0bbh, 0bbh, ... ; <off> <disasm>` — bytes on the LEFT
        (absolute-addressing insns are emitted as db, so real code needs this path)."""
    out = bytearray()
    for line in open(asm_path, encoding="utf-8", errors="replace"):
        code = line.split(";", 1)[0].strip()
        if code.lower().startswith("db "):
            for tok in code[3:].split(","):
                t = tok.strip().rstrip("h").rstrip("H")
                if re.fullmatch(r"[0-9a-fA-F]+", t):
                    out.append(int(t, 16) & 0xFF)
            continue
        if ";" not in line:
            continue
        c = line.split(";", 1)[1].split()
        if not c or not re.fullmatch(r"[0-9a-fA-F]{2,8}", c[0]):
            continue
        for tok in c[1:]:
            if re.fullmatch(r"[0-9a-fA-F]{2}", tok):
                out.append(int(tok, 16))
            else:
                break
    return bytes(out)


def _asm_for(mod, seg, off):
    d = layout.asm_module_dir(mod)
    for cand in ("sec%s_%s.asm" % (seg, off), "sec%d_%05x.asm" % (int(seg), int(off, 16))):
        p = os.path.join(d, cand)
        if os.path.isfile(p):
            return p
    g = glob.glob(os.path.join(d, "sec%s_*%s.asm" % (seg, off)))
    return g[0] if g else None


def _collect(limit):
    """[(module, asm_path, kind, val)] for the auto-generatable easy classes across the
    32-bit (PE) modules, where the lifted body reduces to one statement:
      * ('id',   argN)  — `return argN`      (identity)
      * ('const', N)    — `return <literal>` (constant-return stub)
    PE only — cc32/MSVC targets 32-bit. Leading `u32 vN;` decls are allowed."""
    out = []
    pe = glob.glob("src/generated/**/*.dll.c", recursive=True) + \
        glob.glob("src/generated/**/*.exe.c", recursive=True)
    fn_re = re.compile(
        r"/\* \[AUTO\] \w+\s+\w+ seg(\d+):0x([0-9a-f]+)\s+clean \*/\n"
        r"[a-z0-9_ ]+\**\w+\(([^)]*)\)\n\{([^}]*)\}", re.S)
    for f in pe:
        mod = os.path.basename(f)[:-2]
        txt = open(f, encoding="utf-8", errors="replace").read()
        for m in fn_re.finditer(txt):
            seg, off, _args, body = m.groups()
            stmts = [s.strip() for s in body.strip().split("\n") if s.strip()]
            stmts = [s for s in stmts if not re.fullmatch(r"u32 [\w, ]+;", s)]
            if len(stmts) != 1:
                continue
            s = stmts[0]
            am = re.fullmatch(r"return arg(\d+);", s)
            cm = re.fullmatch(r"return (-?\d+|0x[0-9a-fA-F]+);", s)
            kind_val = ("id", int(am.group(1))) if am else \
                       ("const", int(cm.group(1), 0)) if cm else None
            if not kind_val:
                continue
            ap = _asm_for(mod, seg, off)
            if ap:
                out.append((mod, ap) + kind_val)
            if len(out) >= limit:
                return out
    return out


def _conv(shipped):
    """(convention, stdcall_argcount_or_None) from the shipped return tail."""
    if len(shipped) >= 3 and shipped[-3] == 0xC2:              # ret NN -> __stdcall
        return "__stdcall", int.from_bytes(shipped[-2:], "little") // 4
    if shipped[-1] == 0xC3:                                    # ret -> __cdecl
        return "__cdecl", None
    return None, None


def _ret_type(shipped):
    """Recover the RETURN TYPE from the wide-return codegen the compiler emitted:
    a `cdq` (0x99) => signed 64-bit; `xor edx,edx` (0x33 0xd2) => unsigned 64-bit."""
    if b"\x99" in shipped:
        return "__int64", "int"
    if b"\x33\xd2" in shipped:
        return "unsigned __int64", "unsigned int"
    return "int", "int"


def _candidate(i, shipped, kind, val):
    """Generate a C candidate (identity or constant) whose signature + return type match
    what the shipped bytes imply. Returns (csrc, objname) or None if unrecognizable."""
    conv, k = _conv(shipped)
    if conv is None:
        return None
    rtype, atype = _ret_type(shipped)
    name = "_f%d@%d" % (i, 4 * (k or 0)) if conv == "__stdcall" else "_f%d" % i
    if kind == "const":
        K = k or 0
        args = ",".join("int a%d" % j for j in range(1, K + 1))
        if conv == "__stdcall":
            name = "_f%d@%d" % (i, 4 * K)
        return "%s %s f%d(%s){return %d;}\n" % (rtype, conv, i, args, val), name
    # identity
    mm = re.match(rb"\x8b\x44\x24(.)", shipped)                # mov eax,[esp+X]
    retarg = mm[0][3] // 4 if mm else val
    K = k if k else max(retarg, val)
    if not (1 <= retarg <= K):
        return None
    args = ",".join("%s a%d" % (atype, j) for j in range(1, K + 1))
    if conv == "__stdcall":
        name = "_f%d@%d" % (i, 4 * K)
    return "%s %s f%d(%s){return a%d;}\n" % (rtype, conv, i, args, retarg), name


# --- miss categorisation: each label is a raiser requirement -------------------
def categorize(shipped, emitted):
    if emitted is None:
        return "compile/emit-fail"
    if shipped[:2] == b"\x8b\x45" and emitted[:3] == b"\x8b\x44":
        return "frame-ptr (uses [ebp], not [esp])"
    if b"\x99" in shipped and b"\x99" not in emitted:
        return "wide-return: cdq (signed 64-bit)"
    if b"\x33\xd2" in shipped and b"\x33\xd2" not in emitted:
        return "wide-return: xor edx (unsigned 64-bit)"
    if b"\x83\xec" in shipped and b"\x83\xec" not in emitted:
        return "stack-adjust / opt-flag (sub esp)"
    if len(shipped) > len(emitted) + 2:
        return "not-pure-identity (lift under-modeled body)"
    return "other codegen delta"


def main():
    a = sys.argv[1:]
    def opt(flag, d):
        return int(a[a.index(flag) + 1]) if flag in a else d
    limit = opt("--limit", 3000)
    chunk = opt("--chunk", 200)

    import cc32
    print("collecting identity-function targets (PE, 32-bit)...", flush=True)
    targets = _collect(limit)
    print("  collected %d\n" % len(targets), flush=True)

    matched = missed = skipped = 0
    from collections import Counter
    cats = Counter()
    samples = {}

    # chunked: one cc32 compile per `chunk` functions (keeps CL input bounded)
    for base in range(0, len(targets), chunk):
        grp = targets[base:base + chunk]
        srcs, meta = [], []
        for j, (mod, ap, kind, val) in enumerate(grp):
            i = base + j
            sb = shipped_bytes(ap)
            if not sb:
                skipped += 1
                continue
            cand = _candidate(i, sb, kind, val)
            if not cand:
                skipped += 1
                continue
            src, name = cand
            srcs.append(src)
            meta.append((name, mod, sb))
        if not srcs:
            continue
        r = cc32.compile_c("".join(srcs), flags="/c /O2")
        if "obj" not in r:
            skipped += len(meta)
            continue
        emitted = {n: bytes(c) for n, (c, _m) in cc32.Obj(r["obj"]).functions().items()}
        for name, mod, sb in meta:
            got = emitted.get(name) or emitted.get(name.split("@")[0])
            if got is not None and got == sb:
                matched += 1
            else:
                missed += 1
                cat = categorize(sb, got)
                cats[cat] += 1
                samples.setdefault(cat, (mod, sb.hex(" "), got.hex(" ") if got else "NO-EMIT"))
        print("  ...%d/%d compiled" % (min(base + chunk, len(targets)), len(targets)), flush=True)

    tot = matched + missed
    print("\n" + "=" * 66)
    print("PHASE 0 BYTE-MATCH CENSUS  (identity class, oracle: cc32/MSVC 4.2)")
    print("=" * 66)
    print("  attempted:  %d   (skipped %d unparseable)" % (tot, skipped))
    print("  BYTE-EXACT: %d  (%.1f%%)   with zero raiser work" % (matched, 100 * matched / max(1, tot)))
    print("  misses:     %d  (%.1f%%)" % (missed, 100 * missed / max(1, tot)))
    print("-" * 66)
    print("  MISS CATEGORIES  (the raiser's prioritized worklist):")
    for cat, n in cats.most_common():
        mod, s, g = samples[cat]
        print("   %4d  %-38s" % (n, cat))
        print("         e.g. %s  shipped %s -> %s" % (mod, s[:32], g[:32]))
    print("=" * 66)


if __name__ == "__main__":
    main()
