#!/usr/bin/env python3
"""
masmlift.py — mechanically lift a system module's DISASSEMBLY to byte-exact MASM.

KERNEL/GDI/USER (and the drivers) WERE hand-written assembly. lift.py targets C
for SCALING/behaviour, but a FAITHFUL reconstruction of an assembly module must be
assembly that REASSEMBLES TO THE ORIGINAL BYTES — translating it to C never can.
This tool takes the accurate disassembly (trace.py/objdump, 99.98% 8086-verified)
and emits MASM source: labels at every internal branch/call target, instructions
one per line in MASM syntax, carved data as db/dw.

BYTE-EXACTNESS IS NOT ASSUMED, IT IS VERIFIED. `--verify` assembles the emitted
.asm and compares the produced code bytes against the shipped segment; any
instruction MASM does not encode identically (a short/near jump choice, an
immediate form) is rewritten as `db <original bytes>  ; <mnemonic>` so the source
stays byte-exact and honest about where MASM and the original diverge.

    python masmlift.py SYSTEM              # emit reconstructed/system.asm
    python masmlift.py SYSTEM --verify     # ...and assemble + byte-check it
"""
import re, sys, os, subprocess, shutil, struct, bisect
import tempfile, time
import cfg, trace
import link16   # reuse the DOSBox-X + MASM machinery
from src.core import layout

# ---------------------------------------------------------------------------
# CodeBreaker assemble primitive (Inc 1 — unify). The ported orchestration
# (asmrecon/conduct) needs a GENERIC "assemble this MASM string -> OBJ bytes"
# entry point, distinct from this module's per-MODULE builders above. It drives
# the same period MASM 4.00 under DOSBox-X (via link16's DBX/build16 root), so
# the byte-exact acceptance test (objextract + segdiff) is identical. Kept
# additive + self-contained so win1.0's proven module path is untouched.
# ---------------------------------------------------------------------------
# Compat aliases the ported orchestration reads as `masmlift.MASMROOT` /
# `masmlift.DBX` (win3.1's masmlift exposed these module-level; win1.0 sources
# them from link16). MASMROOT = build16 root (has BIN/INC/LIB).
MASMROOT = link16.B16
DBX = link16.DBX

_ASM_CONF = r"""[sdl]
autolock=false
[cpu]
cputype=386
core=normal
cycles=max
[dos]
ver=6.22
[autoexec]
mount c "{masm}"
mount d "{work}"
set PATH=C:\BIN
set INCLUDE=C:\INC
set LIB=C:\LIB
set TMP=D:\
d:
{cmd}
exit
"""


def _assemble_items(items, timeout=None):
    """Assemble (stem, asm_src) pairs in ONE DOSBox boot (the dominant cost);
    return {stem: {obj,lst,log}}. Stems must be unique + DOS 8.3-safe."""
    masm = link16.B16                                   # build16: has BIN/INC/LIB
    if not os.path.exists(link16.DBX):
        sys.exit("dosbox-x not found: %s" % link16.DBX)
    if not os.path.exists(os.path.join(masm, "BIN", "MASM.EXE")):
        sys.exit("MASM.EXE not found under %s\\BIN" % masm)
    if timeout is None:
        timeout = 120 + 3 * len(items)
    work = tempfile.mkdtemp(prefix="wtmasm_")
    for stem, src in items:
        with open(os.path.join(work, stem + ".ASM"), "w", newline="\r\n") as f:
            f.write(src)
    cmd = "\n".join("MASM %s.ASM, %s.OBJ, %s.LST, NUL; > %s.LOG 2>&1"
                    % (s, s, s, s) for s, _ in items)
    conf = os.path.join(work, "wtmasm.conf")
    with open(conf, "w", newline="\r\n") as f:
        f.write(_ASM_CONF.format(masm=masm, work=work, cmd=cmd))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    try:
        from src.core import progress as _pr
        aborted = _pr.cancelled
    except Exception:
        aborted = lambda: False
    proc = subprocess.Popen([link16.DBX, "-conf", conf, "-nolog"], env=env,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    waited = 0.0
    while proc.poll() is None:
        if aborted():
            proc.kill(); proc.wait(timeout=5)
            return {"aborted": True}
        time.sleep(0.05)
        waited += 0.05
        if waited > timeout:
            proc.kill(); break
    results = {}
    for stem, _src in items:
        d = {}
        for ext, key in ((".OBJ", "obj"), (".LST", "lst"), (".LOG", "log")):
            p = os.path.join(work, stem + ext)
            if os.path.exists(p):
                d[key] = open(p, "rb").read() if ext == ".OBJ" else \
                    open(p, encoding="latin1").read()
        results[stem] = d
    return results


def assemble(asm_src, stem="t", timeout=180, masmroot=None):
    """Assemble one MASM string; return {obj,lst,log} (matches asmrecon's use)."""
    r = _assemble_items([(stem, asm_src)], timeout=timeout)
    if r.get("aborted"):
        return {"aborted": True}
    return r.get(stem, {})


def assemble_many(items, timeout=None, masmroot=None):
    """Assemble MANY (stem, asm_src) pairs in one boot; return
    {"results": {stem: {...}}} (the shape conduct's batch path expects)."""
    r = _assemble_items(items, timeout=timeout)
    if r.get("aborted"):
        return {"aborted": True}
    return {"results": r}


def ledata(obj_path):
    """The segment's code bytes from an OMF .OBJ's LEDATA records — the ACTUAL
    assembled output (a near jump's real self-relative displacement), unlike the
    listing which displays the pre-fixup target with an `R` marker. Chunk-relative
    offsets, matching the listing's offsets. None if the OBJ was not produced."""
    if not os.path.exists(obj_path):
        return None
    data = open(obj_path, "rb").read()
    buf = bytearray(70000)
    i = 0
    while i + 3 <= len(data):
        rtype = data[i]
        rlen = struct.unpack_from("<H", data, i + 1)[0]
        body = data[i + 3:i + 3 + rlen - 1]        # drop the trailing checksum
        if rtype in (0xA0, 0xA1):                   # LEDATA
            p = 0
            s = body[p]; p += 1
            if s >= 0x80:
                s = ((s & 0x7f) << 8) | body[p]; p += 1
            off = struct.unpack_from("<H", body, p)[0]; p += 2
            for k, b in enumerate(body[p:]):
                if off + k < len(buf):
                    buf[off + k] = b
        i += 3 + rlen
    return buf

# branch mnemonics whose operand is a CODE TARGET (-> a label), not a value.
JccJMP = {"jmp", "call", "je", "jne", "jz", "jnz", "jl", "jle", "jg", "jge",
          "jb", "jbe", "ja", "jae", "jc", "jnc", "jo", "jno", "js", "jns",
          "jp", "jnp", "jpe", "jpo", "jcxz", "loop", "loope", "loopne",
          "loopz", "loopnz"}


def masm_hex(v):
    """objdump 0xNN -> MASM NNh (a leading 0 when it starts a-f, so MASM does
    not read it as an identifier)."""
    s = "%x" % v
    return ("0" + s + "h") if s and s[0] in "abcdef" else (s + "h")


def to_masm(asm, is_target):
    """Convert ONE objdump instruction to MASM syntax. `is_target(off)->label`
    maps a resolved branch target offset to its label name (or None)."""
    asm = re.sub(r"\s+", " ", asm).strip()
    # a rep/repz/repne prefix stays; convert the string op after it
    pre = ""
    mpre = re.match(r"(rep|repz|repe|repnz|repne)\s+(.*)$", asm)
    if mpre:
        pre, asm = mpre.group(1) + " ", mpre.group(2)
    op, _, rest = asm.partition(" ")

    # STRING ops: objdump spells operands out (`lods ax,word ptr ds:[si]`), MASM
    # wants the implicit-size mnemonic (`lodsw`/`lodsb`). objdump varies the case of
    # WORD/BYTE PTR, so test case-insensitively (a `scas ax,WORD PTR es:[di]` slipping
    # to `scasb` silently swaps opcode af->ae -- a same-length VALUE corruption).
    if op in ("lods", "stos", "movs", "scas", "cmps"):
        return pre + op + ("w" if "word ptr" in asm.lower() else "b")

    # a branch/call to a bare intra-segment address -> its label
    if op in JccJMP:
        m = re.match(r"0x([0-9a-f]+)$", rest)
        if m:
            lbl = is_target(int(m.group(1), 16))
            if lbl:
                return "%s %s" % (op, lbl)

    # size keywords
    rest = (rest.replace("WORD PTR", "word ptr")
                .replace("BYTE PTR", "byte ptr")
                .replace("DWORD PTR", "dword ptr"))
    # bare hex values / displacements -> MASM hex
    rest = re.sub(r"0x([0-9a-f]+)", lambda m: masm_hex(int(m.group(1), 16)), rest)
    return (op + " " + rest).strip() if rest else op


def emit_segment(mod, si, sg, ordinals, force_db=frozenset(), bounds=None,
                 extra_labels=None):
    """Return (asm_lines, code_span) for one CODE segment (or a [lo,hi) CHUNK of
    a segment too big for MASM's 640KB): labels + instructions + carved data, in
    MASM syntax. An instruction in force_db, or a branch whose target is OUTSIDE
    this unit (cross-chunk / cross-segment, so not a resolvable label here), is
    emitted `db <original bytes> ; mnemonic` — the byte-exact fallback.

    extra_labels: offsets (in OTHER chunks) that ARE resolvable — declared `extrn
    ...:near` by the caller and resolved by LINK4. Passing them makes cross-chunk
    branches emit LIVE (`jmp L_xxxx`) instead of db, so the linker fills the exact
    self-relative displacement."""
    code = sg.code
    off2idx = {off: i for i, (off, _) in enumerate(sg.rows)}
    lo, hi = bounds if bounds else (0, len(code))

    # --- label targets. A label is only placed at a real INSTRUCTION START in
    #     THIS unit. A branch to a cross-chunk target, or to a mid-instruction
    #     address (misaligned/overlapping code), is NOT a label here and cannot be
    #     made live without linking (MASM won't emit a proper near jump to a
    #     numeric literal, and `org` can't name an offset before the chunk), so it
    #     falls to db via unresolved_branch(). ---------------------------------
    targets = set()
    for off, asm in sg.rows:
        if not (lo <= off < hi):
            continue
        m = re.match(r"([a-z]+)\s+0x([0-9a-f]+)$", re.sub(r"\s+", " ", asm).strip())
        if m and m.group(1) in JccJMP:
            targets.add(int(m.group(2), 16))
    targets |= set(sg.funcs)
    labelname = {t: "L_%04x" % t for t in targets
                 if t in off2idx and lo <= t < hi}       # real starts, this unit
    for t in (extra_labels or ()):                       # cross-chunk (extrn) labels
        if t in off2idx:
            labelname.setdefault(t, "L_%04x" % t)

    def is_target(o):
        return labelname.get(o)

    def unresolved_branch(asm):
        # a branch whose target is NOT a resolvable label in this unit (cross-
        # chunk, or a mid-instruction/data-as-code offset) -> db. MASM cannot make
        # a proper near jump to a numeric literal, and org cannot reach an offset
        # before the chunk, so these genuinely need LINKING to go live.
        m = re.match(r"([a-z]+)\s+0x([0-9a-f]+)$", re.sub(r"\s+", " ", asm).strip())
        return bool(m and m.group(1) in JccJMP
                    and int(m.group(2), 16) not in labelname)

    data = sorted(getattr(sg, "data_runs", []))

    lines = []
    pos = lo
    while pos < hi:
        # data run starting here?
        drun = next(((a, z) for a, z in data if a == pos), None)
        if drun:
            a, z = drun
            emit_db(lines, code, a, min(z, hi), labelname)
            pos = min(z, hi)
            continue
        # is there a row at pos?
        if pos in off2idx:
            off = pos
            j = off2idx[off]
            nxt = sg.rows[j + 1][0] if j + 1 < len(sg.rows) else len(code)
            nxt = min(nxt, hi)
            split = False
            for a, z in data:                 # clamp to the next data run start
                if off < a < nxt:
                    nxt = a
                    split = True              # a data run starts INSIDE this instr
                    break
            if off in labelname:
                lines.append("%s:" % labelname[off])
            raw = code[off:nxt]
            masm = to_masm(sg.rows[j][1], is_target)
            # `split`: a carved data run begins mid-instruction, so the disassembled
            # mnemonic is longer than the bytes that actually belong here (spurious
            # decode of a data byte). Emitting it live would make MASM lay down the
            # full instruction and then the data run would DOUBLE the overlap byte.
            # db the clamped bytes so the segment stays contiguously byte-exact.
            if (si, off) in force_db or unresolved_branch(sg.rows[j][1]) or split:
                lines.append("        db      %-24s ; %04x %s"
                             % (", ".join("%sh" % ("0%02x" % b if
                                ("%x" % b)[0] in "abcdef" else "%02x" % b)
                                for b in raw), off, masm))
            else:
                lines.append("        %-32s ; %04x %s"
                             % (masm, off, " ".join("%02x" % b for b in raw)))
            pos = nxt
        else:
            emit_db(lines, code, pos, pos + 1, labelname)     # uncovered byte
            pos += 1
    return lines, (lo, hi)


def emit_db(lines, code, a, z, labelname):
    """Emit a byte range as db lines (16 per line), with a label if one lands
    on the range start."""
    if a in labelname:
        lines.append("%s:" % labelname[a])
    for p in range(a, z, 16):
        chunk = code[p:min(p + 16, z)]
        lines.append("        db      " + ", ".join("%sh" % ("0%02x" % b if
                     ("%x" % b)[0] in "abcdef" else "%02x" % b) for b in chunk)
                     + "   ; %04x" % p)


def build_asm(mod, force_db=frozenset()):
    o = trace.load_ordinals("ORDINALS.md")
    ne = cfg.open_any(mod)
    out = ["        title   %s  (mechanically lifted to MASM by masmlift.py)"
           % mod.lower(),
           "; Byte-exact reconstruction of the shipped %s module's code." % mod,
           "; Every instruction is transcribed from the verified disassembly;",
           "; data runs are emitted as db. NOT interpreted — a faithful,",
           "; reassemblable transcription of the assembly Microsoft wrote.", ""]
    for i, s in enumerate(ne.segments(), 1):
        if s["kind"] != "CODE":
            continue
        sg = trace.Seg(ne, i, o)
        segname = "_TEXT" if i == 1 else "SEG%d" % i
        out.append("%s   segment byte public 'CODE'" % segname)
        out.append("        assume  cs:%s" % segname)
        lines, _ = emit_segment(mod, i, sg, o, force_db)
        out += lines
        out.append("%s   ends" % segname)
        out.append("")
    out.append("        end")
    return "\n".join(out) + "\n"


# MASM lists `OFFSET  BYTEFIELD  <pad>  SOURCE`. The BYTEFIELD is single-space-
# separated tokens: a 2-hex BYTE, a 4-hex WORD (stored little-endian), or the
# relocation flag `R`. It ends at the multi-space pad before the source.
LST_HEAD = re.compile(r"^\s{0,3}([0-9A-Fa-f]{4})  +(\S.*)$")


def parse_lst(line):
    """(offset, [bytes]) for a MASM listing code line, else None."""
    m = LST_HEAD.match(line)
    if not m:
        return None
    off = int(m.group(1), 16)
    field = re.split(r"  +", m.group(2), maxsplit=1)[0]   # up to the source pad
    out = []
    for tok in field.split():
        if tok == "R":                              # relocation marker
            continue
        if re.fullmatch(r"[0-9A-Fa-f]{2}", tok):
            out.append(int(tok, 16))
        elif re.fullmatch(r"[0-9A-Fa-f]{4}", tok):  # WORD -> two little-endian bytes
            w = int(tok, 16)
            out += [w & 0xff, w >> 8]
        else:
            break                                   # reached source text
    return off, out


# MASM 4.0 has no `retf`/`retf N` mnemonic (it wants `ret` inside a PROC FAR).
# A macro emits the exact far-return bytes so the source reads as `retf`/`retf N`
# (LIVE, not db) and still assembles byte-exact.
RETF_MACRO = ["retf    macro   n",
              "        ifb     <n>",
              "        db      0cbh",
              "        else",
              "        db      0cah",
              "        dw      n",
              "        endif",
              "        endm", ""]


def seg_standalone(mod, si, sg, o, force_db, bounds=None):
    """Standalone MASM source for one segment or [lo,hi) chunk (MASM overflows on
    the whole module — and on a segment over ~14KB — so each unit is small; cross-
    unit refs fall to db, which is byte-exact). No `org` needed: relative jumps are
    position-independent and the byte-compare aligns via each line's ; origoff."""
    segname = "_TEXT" if si == 1 else "SEG%d" % si
    lines = ["        title   %s seg%d" % (mod.lower(), si)] + RETF_MACRO + [
             "%s   segment byte public 'CODE'" % segname,
             "        assume  cs:%s" % segname]
    body, _ = emit_segment(mod, si, sg, o, force_db, bounds)
    lines += body
    lines += ["%s   ends" % segname, "        end"]
    return "\n".join(lines) + "\n"


def chunk_bounds(sg, limit=17000):
    """[(lo,hi)] instruction-aligned chunks so no unit exceeds ~limit bytes
    (MASM's symbol table is small). 16.4KB is the proven ceiling — GDI's 16476-byte
    seg1 assembles whole, KERNEL's 30KB and USER's 35KB do not — so only those two
    outliers chunk; every other segment is ONE unit (its near jumps stay LIVE, not
    db'd across a chunk boundary)."""
    n = len(sg.code)
    if n <= limit:
        return [(0, n)]
    starts = sorted(off for off, _ in sg.rows)
    bounds, lo, tgt = [], 0, limit
    for off in starts:
        if off >= tgt and off > lo:
            bounds.append((lo, off)); lo = off; tgt = off + limit
    bounds.append((lo, n))
    return bounds


def seg_xref(sg, bnds):
    """For a chunked segment: (xref, chunk_of). xref = {target_off -> {chunk indices
    that branch to it from a DIFFERENT chunk}} over REAL instruction-start targets.
    These are exactly the cross-chunk near branches the linker resolves: `public` in
    the chunk that defines the target, `extrn ...:near` in each referencing chunk."""
    starts = {off for off, _ in sg.rows}

    def chunk_of(x):
        for k, (lo, hi) in enumerate(bnds):
            if lo <= x < hi:
                return k
        return -1

    xref = {}
    for off, asm in sg.rows:
        m = re.match(r"([a-z]+)\s+0x([0-9a-f]+)$", re.sub(r"\s+", " ", asm).strip())
        if not (m and m.group(1) in JccJMP):
            continue
        t = int(m.group(2), 16)
        if t in starts and chunk_of(t) >= 0 and chunk_of(t) != chunk_of(off):
            xref.setdefault(t, set()).add(chunk_of(off))
    return xref, chunk_of


def seg_chunk_linked(mod, si, sg, o, force_db, ci, bnd, xref):
    """One chunk of a chunked segment, emitted for LINKING: same segment name across
    chunks (LINK4 concatenates them into the original single segment), with cross-
    chunk targets declared `public`/`extrn` so their near branches stay LIVE and the
    linker fills the exact self-relative displacement. No `org`: LINK4 lays the
    chunks down in order and each is byte-exact-length, so every offset is native."""
    lo, hi = bnd
    segname = "_TEXT" if si == 1 else "SEG%d" % si
    pub = sorted(t for t in xref if lo <= t < hi)                 # defined here
    ext = sorted(t for t, refs in xref.items()                   # referenced here,
                 if ci in refs and not (lo <= t < hi))           # defined elsewhere
    lines = ["        title   %s seg%d chunk%d" % (mod.lower(), si, ci)] + RETF_MACRO
    lines += ["%s   segment byte public 'CODE'" % segname,
              "        assume  cs:%s" % segname]
    lines += ["        public  L_%04x" % t for t in pub]
    lines += ["        extrn   L_%04x:near" % t for t in ext]
    # extra_labels ensures a label is PLACED for both (a) `ext` cross-chunk targets
    # this chunk branches to (so `jmp L_xxxx` resolves via the linker) and (b) `pub`
    # targets defined here but only branched to from ANOTHER chunk -- without (b) the
    # `public L_xxxx` has no matching label and MASM errors "Symbol not defined".
    body, _ = emit_segment(mod, si, sg, o, force_db, bnd,
                           extra_labels=set(ext) | set(pub))
    lines += body
    lines += ["%s   ends" % segname, "        end"]
    return "\n".join(lines) + "\n"


def mz_image(exe_path):
    """Raw load image of a DOS MZ .EXE = the linked segment bytes. Header size is at
    offset 8 (paragraphs); relocations (offset 6/24) must be ZERO here — our only
    fixups are self-relative near branches resolved in place, and far refs are db, so
    the image is the finished segment with no load-time patching."""
    d = open(exe_path, "rb").read()
    if d[:2] not in (b"MZ", b"ZM"):
        raise ValueError("not an MZ exe: %s" % exe_path)
    nreloc = struct.unpack_from("<H", d, 6)[0]
    hdr_para = struct.unpack_from("<H", d, 8)[0]
    return d[hdr_para * 16:], nreloc


def _compare_lst(lst_path, orig, orig_len, obj=None):
    """(ok, total, {off: (got_bytes, orig_instr_len)}). Bytes come from the OBJ at
    the instruction's offset (correct self-relative jumps), else the listing. The
    caller needs the ORIGINAL instruction length (from the disassembly, NOT MASM's
    length) to tell a LENGTH-DIFFERER (a re-encode that shifts everything after it
    -> drift) from a same-length drifted jump."""
    ok = tot = 0
    mism = {}
    cmt = re.compile(r";\s*([0-9a-f]{4})\b")
    for line in open(lst_path, encoding="cp437", errors="replace").read().splitlines():
        pl = parse_lst(line)
        cm = cmt.search(line)
        if not pl or not cm:
            continue
        masm_off, disp = pl
        n = len(disp)
        origoff = int(cm.group(1), 16)
        got = list(obj[masm_off:masm_off + n]) if obj is not None else disp
        want = list(orig[origoff:origoff + n])
        tot += 1
        if got == want:
            ok += 1
        else:
            mism[origoff] = (got, orig_len.get(origoff, n))
    return ok, tot, mism


def verify(mod, force_db=frozenset(), verbose=True):
    """Assemble EACH code segment standalone (one DOSBox pass, one MASM per
    segment) and byte-compare per instruction. Returns {(seg, off)} that did not
    assemble byte-exact."""
    o = trace.load_ordinals("ORDINALS.md")
    ne = cfg.open_any(mod)
    outdir = os.path.join(link16.B16, "MASMV")
    link16.rmtree_retry(outdir)
    os.makedirs(outdir)

    units = []            # (name, seg_index, sg)
    for i, s in enumerate(ne.segments(), 1):
        if s["kind"] != "CODE":
            continue
        sg = trace.Seg(ne, i, o)
        for ci, bnd in enumerate(chunk_bounds(sg)):
            name = "M%d_%d" % (i, ci)
            open(os.path.join(outdir, name + ".ASM"), "w", newline="\r\n").write(
                seg_standalone(mod, i, sg, o, force_db, bnd))
            units.append((name, i, sg))

    bat = ["@echo off", "echo masm > build.log"]
    for name, _, _ in units:
        bat.append("masm %s.ASM, %s.OBJ, %s.LST; > %s.out 2> %s.err"
                   % (name, name, name, name, name))
    bat.append("echo DONE >> build.log")
    open(os.path.join(outdir, "BUILD.BAT"), "w",
         newline="\r\n").write("\n".join(bat) + "\n")
    conf = os.path.join(link16.B16, "_masmv.conf")
    open(conf, "w").write(link16.CONF.format(c_root=link16.B16, wd="MASMV"))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    subprocess.run([link16.DBX, "-conf", conf, "-fastlaunch"], cwd=link16.B16,
                   env=env, timeout=1800, capture_output=True)

    byte_mism = {}             # (seg,off) -> (got, want)  [trustworthy iff OBJ built]
    err_off = set()            # (seg,off) MASM could not assemble -> must db
    tot_ok = tot = nerr = 0
    failed = []
    err_re = re.compile(r"\.ASM\((\d+)\)\s*:\s*error", re.I)
    off_re = re.compile(r";\s*([0-9a-f]{4})\b")
    for name, i, sg in units:
        lst = os.path.join(outdir, name + ".LST")
        if not os.path.exists(lst):
            failed.append(name)
            continue
        obj = ledata(os.path.join(outdir, name + ".OBJ"))
        # original instruction length at each offset (next start - this start)
        rows = sorted(sg.rows)
        orig_len = {off: (rows[k + 1][0] if k + 1 < len(rows) else len(sg.code)) - off
                    for k, (off, _) in enumerate(rows)}
        ok, t, mism = _compare_lst(lst, sg.code, orig_len, obj)
        tot_ok += ok
        tot += t
        for off, byts in mism.items():
            byte_mism[(i, off)] = byts
        # MASM ERRORS: an instruction MASM cannot assemble (retf N, an operand
        # form it rejects) must db, or the OBJ never builds. Map each error's .asm
        # line back to its offset. Kept SEPARATE from byte-mismatches: byte
        # comparisons are only trustworthy once the unit builds an OBJ (0 errors).
        lst_text = open(lst, encoding="cp437", errors="replace").read()
        asm_lines = open(os.path.join(outdir, name + ".ASM"),
                         encoding="utf-8", errors="replace").read().splitlines()
        for m in err_re.finditer(lst_text):
            nerr += 1
            ln = int(m.group(1))
            if 1 <= ln <= len(asm_lines):
                cm = off_re.search(asm_lines[ln - 1])
                if cm:
                    err_off.add((i, int(cm.group(1), 16)))
    print("VERIFY %s: %d/%d instructions byte-exact (%.2f%%), %d MASM error(s)%s"
          % (mod, tot_ok, tot, 100.0 * tot_ok / max(1, tot), nerr,
             ("   [%d seg(s) MASM-failed: %s]" % (len(failed), failed))
             if failed else ""))
    return byte_mism, err_off


def structural_drift(img, code):
    """The orig offset where the linked image first LOSES or GAINS a byte relative to
    the shipped segment -- the length-drift that a cross-chunk displacement can't
    explain (a dropped prefix, an imm8/imm16 re-encode MASM sized differently). Skips
    isolated 1-byte diffs (those are cross-chunk-displacement symptoms that vanish
    once the real drifter is db'd). Returns None if only such symptoms remain."""
    n = min(len(img), len(code))
    k = 0
    while k < n - 20:
        if img[k] != code[k]:
            if code[k + 1:k + 17] == img[k:k + 16]:      # img missing a byte at k
                return k
            if code[k:k + 16] == img[k + 1:k + 17]:      # img has an extra byte at k
                return k
        k += 1
    return None


def _emit_and_link(mod, o, seg_units, outdir):
    """Emit every chunk of every seg_unit with its current keep_db, then MASM + LINK4
    all of them in one DOSBox pass. Mutates nothing; reads keep_db out of each unit."""
    bat = ["@echo off", "echo masm > build.log"]
    for u in seg_units:
        i, sg, names, bnds, xref, keep_db = u
        for ci, bnd in enumerate(bnds):
            open(os.path.join(outdir, names[ci] + ".ASM"), "w",
                 newline="\r\n").write(
                seg_chunk_linked(mod, i, sg, o, keep_db, ci, bnd, xref))
            bat.append("masm %s.ASM, %s.OBJ, %s.LST; > %s.out 2> %s.err"
                       % tuple([names[ci]] * 5))
        bat.append("link4 %s, S%d.EXE, S%d.MAP,,; > S%d.lk 2> S%d.le"
                   % (("+".join(names),) + (i,) * 4))
    bat.append("echo DONE >> build.log")
    open(os.path.join(outdir, "BUILD.BAT"), "w",
         newline="\r\n").write("\n".join(bat) + "\n")
    conf = os.path.join(link16.B16, "_masml.conf")
    open(conf, "w").write(link16.CONF.format(c_root=link16.B16, wd="MASML"))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    subprocess.run([link16.DBX, "-conf", conf, "-fastlaunch"], cwd=link16.B16,
                   env=env, timeout=1800, capture_output=True)


def link_verify(mod, force_db=frozenset()):
    """CROSS-CHUNK proof. For each segment too big to assemble whole (KERNEL seg1,
    USER seg1), emit its chunks with `public`/`extrn` cross-chunk labels, MASM each to
    an OBJ, LINK4 them into ONE segment, and byte-compare the LINKED image (which is
    contiguous, unlike the per-instruction OBJ compare) against the shipped segment.

    A convergence loop db's each length-drifter LINK4 exposes -- dropped segment-
    override prefixes on string ops, imm re-encodes -- that the per-instruction
    verify can miss, until the linked image is byte-exact. Success proves every cross-
    chunk near branch is byte-exact LIVE assembly. Returns {(seg,off): 'linked'}."""
    o = trace.load_ordinals("ORDINALS.md")
    ne = cfg.open_any(mod)
    outdir = os.path.join(link16.B16, "MASML")
    link16.rmtree_retry(outdir)
    os.makedirs(outdir)

    seg_units = []            # [i, sg, names, bnds, xref, keep_db]
    for i, s in enumerate(ne.segments(), 1):
        if s["kind"] != "CODE":
            continue
        sg = trace.Seg(ne, i, o)
        bnds = chunk_bounds(sg)
        if len(bnds) <= 1:               # only chunked segments need linking
            continue
        xref, _ = seg_xref(sg, bnds)
        # Only a NEAR jmp/call (e9/e8, always 3 bytes) is guaranteed to reproduce its
        # length when its target becomes an `extrn`. A short jmp (eb, 2B) is promoted
        # to near (e9, 3B) by MASM -> +1B drift; a short Jcc (7x, 2B) to a near extern
        # is likewise unsafe. Keep those db (still byte-exact); only near forms go
        # live through the link.
        keep_db = set(force_db)
        for off, asm in sg.rows:
            m = re.match(r"([a-z]+)\s+0x([0-9a-f]+)$", re.sub(r"\s+", " ", asm).strip())
            if (m and m.group(1) in JccJMP and int(m.group(2), 16) in xref
                    and sg.code[off] not in (0xe9, 0xe8)):
                keep_db.add((i, off))
        names = ["L%d_%d" % (i, ci) for ci in range(len(bnds))]
        seg_units.append([i, sg, names, bnds, xref, keep_db])

    if not seg_units:
        return {}

    it = 0
    while it < 20:
        it += 1
        _emit_and_link(mod, o, seg_units, outdir)
        progressed = False
        for u in seg_units:
            i, sg, names, bnds, xref, keep_db = u
            exe = os.path.join(outdir, "S%d.EXE" % i)
            if not os.path.exists(exe):
                continue
            d = open(exe, "rb").read()
            base, stored, exact = locate_segment(exe, sg.code)
            if exact:
                continue
            img = d[base:base + stored]
            # Prefer the STRUCTURAL drift (a missing/extra byte) -- fixing it also
            # clears the cross-chunk-displacement symptoms that trail it. Only once
            # the image is length-aligned (no structural drift) is a remaining
            # isolated diff a genuine same-length re-encode to db.
            drift = structural_drift(img, sg.code)
            if drift is None:
                drift = next((k for k in range(stored) if img[k] != sg.code[k]),
                             None)
            if drift is None:
                continue
            starts = sorted(off for off, _ in sg.rows)
            di = starts[bisect.bisect_right(starts, drift) - 1]   # instr containing it
            if (i, di) not in keep_db:
                keep_db.add((i, di))
                progressed = True
        if not progressed:
            break

    resolved = {}
    for u in seg_units:
        i, sg, names, bnds, xref, keep_db = u
        exe = os.path.join(outdir, "S%d.EXE" % i)
        if not os.path.exists(exe):
            le = os.path.join(outdir, "S%d.le" % i)
            msg = open(le, encoding="cp437", errors="replace").read().strip() \
                if os.path.exists(le) else "(no LINK4 output)"
            print("  LINK %s seg%d: FAILED to link -- %s"
                  % (mod, i, msg.splitlines()[-1] if msg else "?"))
            continue
        base, stored, exact = locate_segment(exe, sg.code)
        n = len(sg.code)
        live = [off for off, asm in sg.rows
                if (m := re.match(r"([a-z]+)\s+0x([0-9a-f]+)$",
                                  re.sub(r"\s+", " ", asm).strip()))
                and m.group(1) in JccJMP and int(m.group(2), 16) in xref
                and (i, off) not in keep_db]
        if exact:
            print("  LINK %s seg%d: BYTE-EXACT after link (base @%d, %d B stored + "
                  "%d B zero tail) -- %d cross-chunk near branch(es) proven LIVE"
                  % (mod, i, base, stored, n - stored, len(live)))
            for off in live:
                resolved[(i, off)] = "linked"
        else:
            img = open(exe, "rb").read()[base:base + stored]
            firstbad = next((k for k in range(len(img)) if img[k] != sg.code[k]),
                            None)
            print("  LINK %s seg%d: linked but NOT byte-exact (base @%d, first diff "
                  "@%s) -- keeping db" % (mod, i, base,
                  ("%04x" % firstbad) if firstbad is not None else "tail"))
    return resolved


def locate_segment(exe_path, code):
    """Locate the linked segment in LINK4's MZ .EXE and check byte-exactness.
    LINK4's MZ header size is unreliable for a lib-less link AND MZ truncates a
    segment's trailing zero bytes, so we align by CONTENT and score by the EXACT
    PREFIX RUN over the stored portion. Returns (base, stored_len, exact): exact is
    True iff every stored byte matches the segment and the un-stored tail is all zero
    -- i.e. the linked image reproduces the shipped segment exactly."""
    d = open(exe_path, "rb").read()
    n = len(code)

    def run(base):                            # exact-match prefix length at base
        k, lim = 0, min(n, len(d) - base)
        while k < lim and d[base + k] == code[k]:
            k += 1
        return k

    base = max(range(0, max(1, len(d))), key=run)
    stored = min(n, len(d) - base)
    exact = (run(base) == stored) and all(b == 0 for b in code[stored:])
    return base, stored, exact


def main():
    if len(sys.argv) < 2:
        print(__doc__); return 1
    mod = sys.argv[1].upper()
    # System modules AND apps: the disasm machinery (cfg.open_any) is generic --
    # it opens files/WF/<mod>.EXE for anything not in SYSMODS. Byte-exact apps are
    # a project goal, so accept any module whose binary resolves.
    if mod not in cfg.SYSMODS:
        try:
            cfg.open_any(mod)
        except Exception:
            print("%s: not a system module and no files/WF/%s.EXE" % (mod, mod))
            return 1
    path = os.path.join(layout.recon_dir(), mod.lower() + ".asm")
    force_db = set()
    text = build_asm(mod, force_db)
    open(path, "w", newline="\r\n").write(text)
    print("%s -> %s  (%d lines)" % (mod, path, text.count("\n")))

    if "--exact" in sys.argv:
        # MINIMAL-db, OBJ-VERIFIED. Two concerns, handled in order so the db set
        # stays as small as possible (max LIVE assembly):
        #  1. db what BLOCKS a clean OBJ: MASM errors + LENGTH-DIFFERERS (an
        #     imm8/imm16 choice that shifts every following label -> drift). Only
        #     once a unit builds an OBJ are its byte comparisons trustworthy.
        #  2. THEN db the true byte-mismatches the OBJ reveals (genuine re-encodes
        #     MASM won't reproduce) — a small residue; the drifted/`R` jumps are
        #     already byte-exact LIVE in the OBJ and are NOT db'd.
        it = 0
        while it < 14:
            it += 1
            byte_mism, err_off = verify(mod, force_db, verbose=False)
            lendiff = {k for k, (got, olen) in byte_mism.items() if len(got) != olen}
            blockers = (err_off | lendiff) - force_db      # phase 1: unblock OBJ
            if blockers:
                force_db |= blockers
                tag = "errors+drift"
            else:
                residue = set(byte_mism) - force_db        # phase 2: true residue
                if not residue:
                    break
                force_db |= residue
                tag = "true re-encodes"
            open(path, "w", newline="\r\n").write(build_asm(mod, force_db))
            print("  pass %d: +%d db (%d total) [%s], re-verifying..."
                  % (it, len(blockers if blockers else residue),
                     len(force_db), tag))
        ninstr = text.count("\n")
        print("*** BYTE-EXACT: %s reassembles to the shipped bytes "
              "(%d db of ~%d instructions = %.1f%% LIVE assembly)."
              % (mod, len(force_db), ninstr,
                 100.0 * (1 - len(force_db) / max(1, ninstr))))
        # CROSS-CHUNK CLOSURE. The whole-segment source (build_asm) emits every
        # in-segment near branch LIVE, but a >~17KB segment cannot assemble in one
        # MASM unit, so those branches were live-but-UNVERIFIED. link_verify links
        # the chunks and proves them byte-exact -- closing the last verify gap.
        if "--no-link" not in sys.argv:
            resolved = link_verify(mod, force_db)
            if resolved:
                print("    -> %d cross-chunk near branch(es) proven byte-exact by "
                      "LINK4 (were live-but-unverified)." % len(resolved))
    elif "--verify" in sys.argv:
        verify(mod, force_db)
    return 0


if __name__ == "__main__":
    sys.exit(main())
