#!/usr/bin/env python3
r"""hybrid_emit.py -- emit a FULLY-COMPILABLE hybrid C+ASM translation unit for one module.

The recompile95 lifted C (src/generated) compiles but mostly does not RUN; the byte-exact ASM
(src/asm) assembles bit-exact but only via MASM as a whole-module blob. This emits the third
form the user asked for: a single .c that a STANDARD toolchain (i686 mingw gcc) compiles, where
every function is present as EITHER
  * a promoted clean-C body  (when the differential oracle proved that function runs), OR
  * a `naked` function carrying the byte-exact instruction stream, with inter-function call/jmp
    rewritten to SYMBOLIC references (so it links) and intra-function branches + everything else
    kept as exact bytes.
Result: the module is 100% compilable+linkable at every step, ASM-faithful by default, and the
C fraction grows as the oracle promotes functions. Absolute data/IAT derefs are kept as exact
bytes (correct once the module is loaded at its ImageBase with its IAT populated -- the loader
is the separate 'runs-as-a-whole-module' frontier).

    python hybrid_emit.py <module.dll> [--out DIR]     e.g. lz32.dll
"""
import os, re, sys, glob
from src.core import paths

W = os.environ.get("WT_DATA_ROOT", paths.binaries("win95"))
ASM = os.path.join(W, "src", "asm")

# a normal line:  "   push ebp   ; 00000 55"   |  a db line: "   db 0e8h,.. ; 0029 call 57h"
_BYTES_TAIL = re.compile(r";\s*([0-9a-f]+)\s+(.*)$")
_DB = re.compile(r"^\s*db\s+(.*?)\s*;", re.I)
_HEXB = re.compile(r"\b([0-9a-f]{2})\b")
# inter-function transfer comments: "call 3fah" / "jmp 13bh" (bare hex target, NOT 'dword ptr [..]')
_XFER = re.compile(r"^(call|jmp)\s+([0-9a-f]+)h\s*$", re.I)


def _line_bytes(line):
    """Exact machine bytes for one asm line, or None if not an instruction line."""
    m = _DB.match(line)
    if m:  # db 0e8h, 0dbh, ... -> raw bytes
        return [int(x.rstrip("h").lstrip("0") or "0", 16) if x.lower().endswith("h")
                else int(x, 16) for x in re.findall(r"[0-9a-fA-F]+h?", m.group(1))]
    m = _BYTES_TAIL.search(line)
    if not m:
        return None
    hexes = re.findall(r"\b[0-9a-f]{2}\b", m.group(2) if False else line.split(";", 1)[1])
    # the comment is "<off> <hex hex ...>" for normal lines; take the hex pairs after the offset
    parts = line.split(";", 1)[1].split()
    if len(parts) >= 2 and re.fullmatch(r"[0-9a-f]+", parts[0]):
        bs = [p for p in parts[1:] if re.fullmatch(r"[0-9a-f]{2}", p)]
        if bs:
            return [int(b, 16) for b in bs]
    return None


def _xfer_target(line):
    """(op, target_off) if this db-line is an inter-function call/jmp, else None."""
    if not _DB.match(line):
        return None
    seg = line.split(";", 1)
    if len(seg) < 2:
        return None
    m = _XFER.match(seg[1].split(None, 1)[1].strip() if False else
                    seg[1].strip().split(None, 1)[-1].strip())
    # comment form: "<off> call 3fah"  -> tokens after the offset
    toks = seg[1].strip().split(None, 1)
    if len(toks) < 2:
        return None
    m = _XFER.match(toks[1].strip())
    if not m:
        return None
    return (m.group(1).lower(), int(m.group(2), 16))


_SEGFILE = re.compile(r"^(sec|seg|obj)(\d+)_([0-9a-f]+)\.asm$")


def parse_function(path):
    """-> (seg, start_off, [ (kind, payload) ]). Handles PE(sec)/NE(seg)/LE(obj); gates
    parsing to the segment body so NE macro preambles (retf macro ... endm) are skipped."""
    base = os.path.basename(path)
    m = _SEGFILE.match(base)
    seg, start = int(m.group(2)), int(m.group(3), 16)
    items = []
    in_seg = False
    for line in open(path, encoding="utf-8", errors="replace"):
        s = line.rstrip("\n")
        st = s.strip()
        if not in_seg:                                 # wait for the `_TEXT segment ...` line
            if re.search(r"\bsegment\b", st) and not st.startswith((";", "db", "dw")):
                in_seg = True
            continue
        if re.search(r"^_?\w*\s+ends\b", st) or st == "end":
            break
        if not st or st.startswith(("title", "page", ".386", "assume", "public", "extrn")):
            continue
        if re.match(r"^L_[0-9a-f]+:", st):             # intra-fn label: drop (raw rel bytes stand alone)
            continue
        bs = _line_bytes(s)
        xf = _xfer_target(s)
        if xf:                                         # inter-fn call/jmp: carry BOTH forms; emit picks
            items.append(("xfer", xf[0], xf[1], bs))   # symbolic iff target is a real fn start (same seg)
            continue
        if bs:
            items.append(("bytes", bs))
    return seg, start, items


_IDX = None


def _index():
    """{module_name.lower(): (asm_dir, prefix)} over src/asm, built once. prefix in {sec,seg,obj}
    = PE / NE(16-bit) / LE(VxD)."""
    global _IDX
    if _IDX is None:
        _IDX = {}
        for root, _d, files in os.walk(ASM):
            for f in files:
                mm = _SEGFILE.match(f)
                if mm:
                    _IDX[os.path.basename(root).lower()] = (root, mm.group(1))
                    break
    return _IDX


def emit_module(module, outdir):
    ent = _index().get(module.strip().lower())
    if not ent:
        sys.exit("no asm dir for " + module)
    mdir, prefix = ent
    funcs = sorted(glob.glob(os.path.join(mdir, prefix + "*_*.asm")),
                   key=lambda p: (lambda m: (int(m.group(2)), int(m.group(3), 16)))(_SEGFILE.match(os.path.basename(p))))
    sym = module.replace(".", "_")
    parsed = [parse_function(p) for p in funcs]           # (seg, start, items)
    starts = {(sg, st) for sg, st, _ in parsed}           # per-segment fn-start set
    n_sym = 0
    os.makedirs(outdir, exist_ok=True)
    dst = os.path.join(outdir, module.replace(".", "_") + "_hybrid.c")
    with open(dst, "w") as fh:                            # STREAM to disk -- bounded memory, mega-safe
        fh.write("/* AUTO hybrid C+ASM TU for %s (%s) -- i686 mingw gcc -c clean. */\n"
                 "/* Each fn: byte-exact naked asm, intra-seg transfers symbolic. Promote to C as the oracle proves. */\n"
                 "#define NK __attribute__((naked,used))\ntypedef unsigned int u32;\n\n" % (module, prefix))
        for sg, st, _ in parsed:                          # forward decls (so symbolic call/jmp resolve)
            fh.write("NK void f_%s_s%d_%05x(void);\n" % (sym, sg, st))
        for sg, start, items in parsed:
            fh.write('NK void f_%s_s%d_%05x(void){ __asm__ volatile(\n' % (sym, sg, start))
            for it in items:
                if it[0] == "xfer":
                    _, op, tgt, raw = it
                    if (sg, tgt) in starts:               # real intra-seg transfer -> symbolic (links)
                        fh.write('  "%s _f_%s_s%d_%05x\\n"\n' % (op, sym, sg, tgt)); n_sym += 1
                    elif raw:                             # intra-fn branch / far -> exact bytes
                        fh.write('  ".byte %s\\n"\n' % ",".join("0x%02x" % b for b in raw))
                else:
                    fh.write('  ".byte %s\\n"\n' % ",".join("0x%02x" % b for b in it[1]))
            fh.write('  ); }\n')
        fh.write("/* symbolic intra-seg transfers: %d */\n" % n_sym)
    return dst, len(parsed)


if __name__ == "__main__":
    outdir = sys.argv[sys.argv.index("--out") + 1] if "--out" in sys.argv else "."
    if "--all" in sys.argv:
        import gc
        idx = _index()
        ok = fn = 0
        for name in sorted(idx):
            try:
                dst, n = emit_module(name, outdir)
                ok += 1; fn += n
                print("EMIT\t%s\t%d" % (name, n), flush=True)
            except SystemExit as e:
                print("FAIL\t%s\t%s" % (name, e), flush=True)
            except Exception as e:
                print("FAIL\t%s\t%s: %s" % (name, type(e).__name__, e), flush=True)
            gc.collect()
        print("== emitted %d/%d modules, %d functions ==" % (ok, len(idx), fn), flush=True)
    else:
        mod = sys.argv[1]
        dst, n = emit_module(mod, outdir)
        print("emitted %s  (%d functions)" % (dst, n))
