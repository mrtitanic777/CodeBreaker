#!/usr/bin/env python3
r"""vxd_masm.py -- assemble-and-compare for a recovered VxD object.

The disassembler (vxd_asm) proves the listing is a faithful VIEW of the bytes. This closes the
loop the other way: re-emit an executable object as MASM, assemble it with the period DDK ML.EXE,
and compare the assembled code bytes to the oracle object -- the byte-exact test.

Emission rule that keeps the comparison honest:
  * an instruction with NO fixup is emitted symbolically (mnemonic + operands) and re-encoded by
    ML -- this is what the test measures: does ML reproduce the original compiler's encoding?
  * an instruction WITH a fixup (a cross-object call/ref whose oracle bytes are a 0 placeholder +
    an LE fixup), a VxD service call, and every data byte are emitted as raw `db` -- exact by
    construction, since an object-level assembly has no linker to resolve fixups.

So a byte diff can only come from ML choosing a different legal encoding than the compiler did
(A1 vs 8B for a moffs load, imm8 vs imm32, rel8 vs rel32); the diff report lists exactly those.

    from codebreaker.formats import le
    from codebreaker.engines.inverse import vxd_masm
    v = dict(le.open_vxds("vkd.vxd"))["vkd.vxd"] if False else le.LEFile.from_file("vkd.vxd")
    print(vxd_masm.compare_object(v, 1))     # {'match': True/False, 'ledata_len', 'diffs': [...]}
"""
import os
import re
import shutil
import struct
import subprocess
import tempfile

from codebreaker.engines.inverse.vxd_asm import VxDDisasm
from codebreaker.toolchain.backbone.dosbox import run_dos

# ML.EXE (DDK 6.11c) is a Phar Lap TNT executable -- it needs DOSXNT.EXE, and the mount path must
# be space-free (DOSBox `mount` breaks on spaces). Stage all three into one cached space-free dir.
_TC = os.path.join(os.environ.get("TEMP", "/tmp"), "vxdtc")
_ML_SRC = "binaries/media/Microsoft Windows 95 DDK/MASM611C"
_DOSXNT_SRC = "binaries/toolchains/period/compilers/msvc-1.52c/BIN/DOSXNT.EXE"


def _stage_toolchain(root):
    os.makedirs(_TC, exist_ok=True)
    for src in (os.path.join(root, _ML_SRC, "ML.EXE"), os.path.join(root, _ML_SRC, "ML.ERR"),
                os.path.join(root, _DOSXNT_SRC)):
        dst = os.path.join(_TC, os.path.basename(src))
        if not os.path.exists(dst) and os.path.exists(src):
            shutil.copy(src, dst)
    return _TC


# ---- MASM text for a single instruction -----------------------------------------
_MNEM = {"pushal": "pushad", "popal": "popad"}
_STRING_OPS = {"movsb", "movsw", "movsd", "stosb", "stosw", "stosd", "lodsb", "lodsw", "lodsd",
               "scasb", "scasw", "scasd", "cmpsb", "cmpsw", "cmpsd", "insb", "insw", "insd",
               "outsb", "outsw", "outsd"}


def _hex(m):
    d = m.group(1)
    return ("0" + d if d[0] in "abcdefABCDEF" else d) + "h"


def _masm_ops(ops):
    return re.sub(r"0x([0-9a-fA-F]+)", _hex, ops)


# jmp/jcc have both a 2-byte rel8 (SHORT) and a 5/6-byte rel32 (NEAR) form; MASM auto-picks the
# short one when the target fits, which drifts from a compiler that emitted the long form (and
# vice-versa). Emitting the explicit size pins ML to the oracle's encoding. call is always near;
# jcxz/loop are always short -- neither takes a size override.
_SHORTABLE = {"jmp", "je", "jne", "jz", "jnz", "ja", "jae", "jb", "jbe", "jg", "jge", "jl", "jle",
              "jo", "jno", "js", "jns", "jp", "jnp", "jc", "jnc", "jpe", "jpo", "jna", "jnae",
              "jnb", "jnbe", "jng", "jnge", "jnl", "jnle"}


def _masm_line(D, oi, row):
    """MASM text for one instruction, or None if it must be emitted as raw bytes instead (a direct
    branch whose numeric target has no label -- MASM rejects `jmp <number>`)."""
    from codebreaker.engines.inverse.vxd_asm import _render_ops, _direct_target_row, _BRANCHES
    mnem = _MNEM.get(row["mnem"], row["mnem"])
    if mnem.split()[-1] in _STRING_OPS:                    # `lodsb al, [esi]` -> `lodsb` (+ rep)
        return "        " + mnem.rstrip()
    ops, resolved = _render_ops(D, oi, row)
    if mnem in _BRANCHES and not resolved and _direct_target_row(row) is not None:
        return None                                        # direct branch to an unlabelled address
    ops = _masm_ops(ops)
    if mnem in _SHORTABLE and resolved:                    # pin the branch size to the oracle's
        ops = ("SHORT " if row["size"] == 2 else "NEAR PTR ") + ops
    return ("        %-6s %s" % (mnem, ops)).rstrip()


def _db(bs):
    return "        db  " + ", ".join("0%02Xh" % b for b in bs)


_ABS_MEM = re.compile(r"\[\s*(?:0x[0-9a-fA-F]+|[0-9]+)\s*\]")   # [number] -- no base/index register


def has_abs_mem(row):
    """True if an operand is a bare absolute-address memory reference `[0x1234]` / `[0]`. MASM is
    unreliable with these: `mov eax,[disp]` is the 5-byte moffs form it may re-encode to 6 bytes (a
    length drift), and `add [disp],reg` it rejects outright ("immediate operand not allowed", since
    it reads a bracketed bare number as an immediate). Pre-forcing them to raw byte-preserves the
    exact bytes and keeps the assembler out of trouble, so objects converge in a couple of passes."""
    return bool(_ABS_MEM.search(row["ops"]))


_SEG_PREFIX = {0x2E, 0x26, 0x36, 0x3E, 0x64, 0x65}       # CS ES SS DS FS GS override bytes


def has_seg_prefix(row):
    """True if the instruction carries a segment-override prefix (2E/26/36/3E/64/65). Ring-0 VxD
    code uses these to reach V86/other segments, and often carries a redundant CS: prefix that MASM
    silently drops -- a length drift that cascades into every following call/jmp target. Pre-forcing
    byte-preserves the prefix exactly."""
    return bool(row["raw"]) and row["raw"][0] in _SEG_PREFIX


def object_body(D, oi, force_raw=None, base_line=0, extra_labels=None, probe=False, crange=None):
    """The body lines of one object -- instructions symbolically, data + fixup-bearing + service
    instructions as raw db, labels at branch/fixup targets. Returns (lines, linemap) where linemap
    maps (base_line + index-in-lines, 1-based) to the instruction offset. extra_labels adds label
    names at given offsets (e.g. the exported DDB). With probe=True, a `Q<hexoff>:` label precedes
    every instruction; labels emit no bytes, so the code is identical, but the COFF symbol table
    then reveals each instruction's TRUE assembled offset -- so one assembly finds every drifting
    instruction at once (no one-per-pass). crange=(lo,hi) emits only that offset range (one source
    file of a split object); the caller forces any branch leaving the range. Shared by the object
    test, the split-file emitter, and the full link."""
    force_raw = force_raw or set()
    extra_labels = extra_labels or {}
    code = D.code[oi]
    n = len(code)                                          # actual page bytes (may be < vsize when
    lo, hi = crange or (0, n)                              #  the object has a zero-fill/BSS tail)
    L = []
    linemap = {}
    a = lo
    while a < hi:
        if a in extra_labels:
            L.append("public %s" % extra_labels[a])
            L.append("%s label byte" % extra_labels[a])
        if D.labels.get((oi, a)):
            L.append("%s:" % D.labels[(oi, a)])
        row = D.insns.get((oi, a))
        if row is None:                                    # data byte(s)
            b = a
            while b < hi and (oi, b) not in D.insns and not (b > a and D.labels.get((oi, b))) \
                    and not (b > a and b in extra_labels):
                b += 1
            for i in range(a, b, 16):
                L.append(_db(code[i:min(i + 16, b)]))
            a = b
            continue
        if probe:
            L.append("public Q%x" % a)                     # per-instruction offset probe (public so
            L.append("Q%x:" % a)                           #  it lands in the COFF symbol table)
        line = None if (row["fx"] or row["vxdcall"] or a in force_raw) else _masm_line(D, oi, row)
        if line is None:
            L.append(_db(row["raw"]))
        else:
            L.append(line)
            linemap[base_line + len(L)] = a                # this symbolic line -> its offset
        a += row["size"]
    return L, linemap


def emit_object_masm(D, oi, segname="_LTEXT", force_raw=None, probe=False, crange=None):
    """Emit one executable object (or, with crange, one offset-range slice of it) as an assemblable
    MASM use32 segment. Returns (text, linemap), where linemap maps a 1-based source line back to
    the instruction offset -- so an assembler error at a line can be traced to the instruction and
    repaired. probe adds per-instruction `Q<off>` labels (see object_body)."""
    head = [".386p", "%s segment dword public use32 'CODE'" % segname, ""]
    body, linemap = object_body(D, oi, force_raw=force_raw, base_line=len(head), probe=probe,
                                crange=crange)
    L = head + body + ["%s ends" % segname, "        end", ""]
    return "\r\n".join(L), linemap


def split_ranges(D, oi, max_ins=6000):
    """Split an executable object into function-aligned offset ranges of about max_ins instructions
    each -- the real Win95 source was many files, not one monolith, and ML cannot assemble a huge
    single source anyway. Cuts fall on function starts (sub_ labels) so no function spans two files."""
    offs = sorted(a for (o, a) in D.insns if o == oi)
    n = len(D.code[oi])
    fs = {off for (o, off), name in D.labels.items() if o == oi and name.startswith("sub_")}
    ranges = []
    lo = 0
    count = 0
    for a in offs:
        if count >= max_ins and a in fs and a > lo:
            ranges.append((lo, a)); lo = a; count = 0
        count += 1
    ranges.append((lo, n))
    return ranges


def cross_range_force(D, oi, lo, hi):
    """Offsets in [lo,hi) whose branch leaves the range -- their target label lives in another file,
    so in an isolated per-file assembly they must be byte-preserved (or, for the buildable tree,
    declared extern and resolved by the linker)."""
    from codebreaker.engines.inverse.vxd_asm import _direct_target_row, _BRANCHES
    out = set()
    for (o, a), row in D.insns.items():
        if o != oi or not (lo <= a < hi):
            continue
        if row["mnem"] in _BRANCHES:
            t = _direct_target_row(row)
            if t is not None and not (lo <= t < hi):
                out.add(a)
    return out


# ---- OMF: reconstruct the assembled segment bytes -------------------------------
def omf_segment_bytes(obj):
    """Reconstruct the (single) segment's bytes from an OMF .obj: place every LEDATA/LIDATA record
    at its enumerated offset. Returns the assembled byte image."""
    buf = bytearray()
    i = 0
    n = len(obj)

    def _put(off, data):
        if off + len(data) > len(buf):
            buf.extend(b"\x00" * (off + len(data) - len(buf)))
        buf[off:off + len(data)] = data

    while i < n:
        rectype = obj[i]
        rlen = struct.unpack_from("<H", obj, i + 1)[0]
        body = obj[i + 3:i + 3 + rlen - 1]                 # minus trailing checksum
        if rectype in (0xA0, 0xA1):                        # LEDATA (16/32-bit offset)
            p = 0
            segidx = body[p]; p += 1
            if segidx >= 0x80:                             # 2-byte index
                p += 1
            if rectype == 0xA1:
                off = struct.unpack_from("<I", body, p)[0]; p += 4
            else:
                off = struct.unpack_from("<H", body, p)[0]; p += 2
            _put(off, body[p:])
        elif rectype in (0xA2, 0xA3):                      # LIDATA (iterated) -- expand
            p = 0
            segidx = body[p]; p += 1
            if segidx >= 0x80:
                p += 1
            if rectype == 0xA3:
                off = struct.unpack_from("<I", body, p)[0]; p += 4
            else:
                off = struct.unpack_from("<H", body, p)[0]; p += 2
            data, _ = _expand_lidata(body, p, rectype == 0xA3)
            _put(off, data)
        i += 3 + rlen - 1 + 1
    return bytes(buf)


def _expand_lidata(body, p, is32):
    """Expand an OMF LIDATA iterated-data block into raw bytes."""
    out = bytearray()
    rep = struct.unpack_from("<I" if is32 else "<H", body, p)[0]; p += 4 if is32 else 2
    blocks = struct.unpack_from("<H", body, p)[0]; p += 2
    if blocks == 0:
        cnt = body[p]; p += 1
        chunk = body[p:p + cnt]; p += cnt
        out += chunk * rep
    else:
        for _ in range(blocks):
            sub, p = _expand_lidata(body, p, is32)
            out += sub * rep
    return bytes(out), p


# ---- assemble + compare ----------------------------------------------------------
def assemble(masm_text, root):
    tc = _stage_toolchain(root)
    # a large all-raw object is thousands of db lines; give ML headroom (DOSBox at 20k cycles is
    # slow) so a big object's fallback assembly doesn't time out and spuriously "fail".
    tmo = 300 if len(masm_text) > 60000 else 120
    ok, out, work = run_dos({"c": tc},
                            ["c:\\ml.exe /c /Fo m.obj m.asm > asm.txt"],
                            ["m.obj", "asm.txt"], files={"m.asm": masm_text}, timeout=tmo)
    log = ""
    if work and os.path.exists(os.path.join(work, "asm.txt")):
        log = open(os.path.join(work, "asm.txt"), errors="replace").read()
    if work:
        shutil.rmtree(work, ignore_errors=True)
    return (out.get("m.obj"), log)


# --- Native MASM-compatible assembler (nasm --masm) -------------------------
# When a MASM-compatible nasm is installed it drives the byte-exact convergence
# natively -- no DOSBox/ML round-trip -- typically 10-20x faster, and it matches
# the shipped Win95 bytes even on the SIB base/index orderings where ML 6.11c
# differs (so it byte-preserves FEWER instructions than ML). It is resolved from
# config.json's `nasm` (the bundled tools/nasm-masm build; the repo convention),
# then the NASM_MASM env override, then a `nasm-masm`/`nasm` on PATH. When none
# qualifies the code transparently falls back to ML.EXE under DOSBox.
# Build/install instructions: docs/NATIVE_ASSEMBLER.md.
#
# The probe assembles a mixed-case PUBLIC and checks it survives verbatim in the
# object: the convergence locates drift by `public Q<off>` symbols, so a nasm
# that folds public case (older --masm builds did) is rejected here rather than
# silently failing every lookup.
_NASM_PROBE = "bits 32\r\nglobal Q7X\r\nQ7X:\r\n mov eax, ebx\r\n"
_nasm_cache = None


def find_nasm():
    """Path to a `--masm`-capable, public-case-preserving nasm, or None. Resolution order:
    config.json `nasm`, then the NASM_MASM env var, then `nasm-masm`/`nasm` on PATH. Probed once
    (accepts --masm, emits COFF, keeps public case) and cached. See docs/NATIVE_ASSEMBLER.md."""
    global _nasm_cache
    if os.environ.get("CB_FORCE_ML"):        # force the authentic DDK ML.EXE (DOSBox), not native nasm
        _nasm_cache = ""
        return None
    if _nasm_cache is not None:
        return _nasm_cache or None
    cands = []
    try:
        from codebreaker.core import config
        cands.append(config.load_config().get("nasm"))          # config.json (repo convention; absolute)
    except Exception:
        pass
    cands += [os.environ.get("NASM_MASM"), shutil.which("nasm-masm"), shutil.which("nasm")]
    for cand in cands:
        if not cand or not os.path.exists(cand):
            continue
        try:
            wd = tempfile.mkdtemp()
            ap = os.path.join(wd, "p.asm"); op = os.path.join(wd, "p.obj")
            open(ap, "w", newline="\r\n").write(_NASM_PROBE)
            subprocess.run([cand, "--masm", "-f", "win32", ap, "-o", op],
                           capture_output=True, timeout=30)
            obj = open(op, "rb").read() if os.path.exists(op) else b""
            shutil.rmtree(wd, ignore_errors=True)
            if obj and b"Q7X" in obj:      # --masm accepted, COFF emitted, public case PRESERVED (ML-like)
                _nasm_cache = cand
                return cand
        except Exception:
            pass
    _nasm_cache = ""                                                # probed, unavailable -> ML fallback
    return None


def _assemble_coff_nasm(masm_text, nasm):
    """Assemble one MASM text to a COFF object with nasm --masm. Returns (obj_bytes, log)."""
    wd = tempfile.mkdtemp()
    ap = os.path.join(wd, "m.asm"); op = os.path.join(wd, "m.obj")
    open(ap, "w", newline="\r\n").write(masm_text)
    tmo = 300 if len(masm_text) > 60000 else 120
    try:
        r = subprocess.run([nasm, "--masm", "-f", "win32", ap, "-o", op],
                           capture_output=True, text=True, timeout=tmo)
        log = (r.stderr or "") + (r.stdout or "")
        obj = open(op, "rb").read() if os.path.exists(op) else None
    except Exception as e:
        obj, log = None, "nasm: %s" % e
    finally:
        shutil.rmtree(wd, ignore_errors=True)
    return obj, log


def assemble_coff(masm_text, root):
    """Assemble to a COFF object (so section bytes + a symbol table are available). Returns
    (obj_bytes, log). Uses nasm --masm natively when installed, else ML.EXE under DOSBox."""
    nasm = find_nasm()
    if nasm:
        return _assemble_coff_nasm(masm_text, nasm)
    tc = _stage_toolchain(root)
    tmo = 300 if len(masm_text) > 60000 else 120
    ok, out, work = run_dos({"c": tc},
                            ["c:\\ml.exe -coff -c -Fom.obj m.asm > asm.txt"],
                            ["m.obj", "asm.txt"], files={"m.asm": masm_text}, timeout=tmo)
    log = open(os.path.join(work, "asm.txt"), errors="replace").read() if work and os.path.exists(
        os.path.join(work, "asm.txt")) else ""
    if work:                                               # don't accumulate mkdtemp dirs (a corpus
        shutil.rmtree(work, ignore_errors=True)            #  run makes thousands)
    return out.get("m.obj"), log


def assemble_coff_batch(masm_list, root, budget=300000, maxfiles=32):
    """Assemble many MASM texts to COFF in as FEW DOSBox sessions as possible -- run_dos takes a list
    of commands, so N `ml.exe` calls share one BLD.BAT. Texts are grouped so each session stays under
    `budget` total chars and `maxfiles` files (a text over budget runs solo). Returns [(obj, log)]
    aligned with masm_list. This is where a corpus's cold-start cost lives: 883 objects' first pass
    becomes a few dozen sessions instead of 883. With native nasm there is no cold start, so each
    text is assembled directly and the batching below is only used for the ML/DOSBox fallback."""
    nasm = find_nasm()
    if nasm:
        return [_assemble_coff_nasm(m, nasm) for m in masm_list]
    tc = _stage_toolchain(root)
    results = [(None, "")] * len(masm_list)
    batches, cur, cl = [], [], 0
    for i in sorted(range(len(masm_list)), key=lambda k: len(masm_list[k])):
        sz = len(masm_list[i])
        if sz > budget:
            batches.append([i]); continue
        if cur and (cl + sz > budget or len(cur) >= maxfiles):
            batches.append(cur); cur, cl = [], 0
        cur.append(i); cl += sz
    if cur:
        batches.append(cur)
    for batch in batches:
        files, bat, arts = {}, [], []
        for j, idx in enumerate(batch):
            fn = "m%d" % j
            files[fn + ".asm"] = masm_list[idx]
            bat.append("c:\\ml.exe -coff -c -Fo%s.obj %s.asm > %s.txt" % (fn, fn, fn))
            arts += [fn + ".obj", fn + ".txt"]
        tot = sum(len(masm_list[i]) for i in batch)
        _, out, work = run_dos({"c": tc}, bat, arts, files=files, timeout=min(600, 90 + tot // 400))
        for j, idx in enumerate(batch):
            log = out.get("m%d.txt" % j, b"")
            results[idx] = (out.get("m%d.obj" % j), log.decode("latin1", "replace") if log else "")
        if work:
            shutil.rmtree(work, ignore_errors=True)
    return results


def coff_text_syms(obj):
    """From a COFF object with one code section, return (section_bytes, {symbol_name: offset}) where
    each symbol's offset is section-relative. Used to read the TRUE assembled offset of every
    probe-labelled instruction."""
    nsec = struct.unpack_from("<H", obj, 2)[0]
    symptr = struct.unpack_from("<I", obj, 8)[0]
    nsym = struct.unpack_from("<I", obj, 12)[0]
    strtab = symptr + nsym * 18
    secs = []
    for i in range(nsec):
        h = 20 + i * 40
        rawsize = struct.unpack_from("<I", obj, h + 16)[0]
        rawptr = struct.unpack_from("<I", obj, h + 20)[0]
        flags = struct.unpack_from("<I", obj, h + 36)[0]
        secs.append((i + 1, rawsize, rawptr, flags))
    code = [s for s in secs if (s[3] & 0x20) and s[1] > 0] or \
        sorted(secs, key=lambda s: s[1], reverse=True)[:1]
    secnum, rawsize, rawptr, _ = code[0]
    text = obj[rawptr:rawptr + rawsize]
    syms = {}
    i = 0
    while i < nsym:
        off = symptr + i * 18
        name8 = obj[off:off + 8]
        if name8[:4] == b"\x00\x00\x00\x00":
            so = struct.unpack_from("<I", name8, 4)[0]
            end = obj.index(b"\x00", strtab + so)
            name = obj[strtab + so:end].decode("latin1", "replace")
        else:
            name = name8.rstrip(b"\x00").decode("latin1", "replace")
        value, sec, typ, sclass, naux = struct.unpack_from("<IhHBB", obj, off + 8)
        if sec == secnum:
            syms[name] = value
        i += 1 + naux
    return text, syms


_ERRLINE = re.compile(r"m\d*\.asm[:(](\d+)\)?:?\s*(?:error|fatal)", re.I)   # ML m.asm(N): / m<N>.asm / nasm m.asm:N:


def _converge_range(D, oi, root, lo, hi, oracle, max_repair, first=None):
    """Drive one offset-range [lo,hi) of an object to a byte-exact assembly via probe labels. A
    branch leaving the range is byte-preserved (its target label is in another file). Returns
    {sym, raw, match, force, text} where text is the range's assembled bytes (== oracle[lo:hi] when
    exact). `first`=(obj, log, linemap) seeds iteration 0 with an already-assembled first pass (its
    force MUST be the initial cross_range_force) -- so a caller can batch every range's first pass in
    one DOSBox and hand the result here; the repair passes, which depend on the previous, assemble
    normally."""
    offs = [a for (o, a) in D.insns if o == oi and lo <= a < hi]
    inrange = set(offs)
    force = cross_range_force(D, oi, lo, hi)               # branches out of the range -> byte-preserve
    for it in range(max_repair):
        if it == 0 and first is not None:
            obj, log, linemap = first                     # batched first pass (assembled with `force`)
        else:
            masm, linemap = emit_object_masm(D, oi, force_raw=force, probe=True, crange=(lo, hi))
            obj, log = assemble_coff(masm, root)
        if not obj:
            bad = {linemap[int(m.group(1))] for m in _ERRLINE.finditer(log) if int(m.group(1)) in linemap}
            if not bad or bad <= force:
                break                                      # -> range raw fallback
            force |= bad
            continue
        text, syms = coff_text_syms(obj)
        newbad = set()
        for a in offs:
            if a in force:
                continue
            ao = syms.get("Q%x" % a)
            if ao is None:
                continue
            olen = D.insns[(oi, a)]["size"]
            if bytes(text[ao:ao + olen]) != bytes(oracle[a:a + olen]):
                newbad.add(a)
        exact = len(text) >= (hi - lo) and bytes(text[:hi - lo]) == bytes(oracle[lo:hi])
        if exact or not newbad:
            fr = force & inrange
            return {"sym": len(inrange) - len(fr), "raw": len(fr), "match": exact,
                    "force": force, "text": bytes(text[:hi - lo])}
        force |= newbad
    force |= inrange                                       # range fallback: byte-preserve all of it
    masm, _ = emit_object_masm(D, oi, force_raw=force, crange=(lo, hi))
    obj, log = assemble_coff(masm, root)
    text = coff_text_syms(obj)[0] if obj else b""
    exact = obj is not None and len(text) >= (hi - lo) and bytes(text[:hi - lo]) == bytes(oracle[lo:hi])
    return {"sym": 0, "raw": len(inrange), "match": exact, "force": force,
            "text": bytes(text[:hi - lo]) if obj else b""}


def compare_object(v, oi, root=".", max_repair=8, max_ins=6000):
    """Assemble-and-compare object oi against the oracle, maximising GENUINE re-encodes (no wholesale
    byte-copy). Every instruction is emitted symbolically and probe-labelled; one COFF assembly then
    reveals, via each label's true offset, every instruction ML re-encoded differently (drift or a
    commutative/SIB/direction mirror) -- only those are byte-preserved. Large objects are SPLIT into
    function-aligned files of ~max_ins instructions (as the real multi-file source was, and because
    ML cannot assemble a huge monolithic source); each file converges independently and its bytes
    are checked against its oracle range. Returns the verdict + the symbolic vs forced-raw split."""
    D = VxDDisasm(v).run()
    root = os.path.abspath(root)
    oracle = D.code[oi]
    offs = sorted(a for (o, a) in D.insns if o == oi)
    ranges = [(0, len(oracle))] if len(offs) <= max_ins else split_ranges(D, oi, max_ins)
    sym = raw = 0
    match = True
    fr_all = set()
    parts = []
    for lo, hi in ranges:
        r = _converge_range(D, oi, root, lo, hi, oracle, max_repair)
        sym += r["sym"]; raw += r["raw"]; match = match and r["match"]; fr_all |= r["force"]
        parts.append(r["text"])
    asm = b"".join(parts)
    return {"ok": True, "match": match, "oracle_len": len(oracle), "asm_len": len(asm),
            "n_diffs": 0 if match else 1, "symbolic_insns": sym, "forced_raw": raw,
            "force_raw_set": fr_all, "raw_fallback": sym == 0 and (sym + raw) > 0,   # code, all forced
            "n_files": len(ranges), "_asm": asm[:len(oracle)], "_oracle": oracle, "D": D}


def compare_module(v, root=".", max_repair=8, max_ins=6000):
    """Assemble-and-compare every executable object of a VxD. The FIRST assembly pass of every range
    (of every object) is batched into as few DOSBox sessions as possible (assemble_coff_batch); only
    ranges that do not converge on that first pass take further, per-range repair assemblies. Since
    most ranges are byte-exact on the first pass, this turns a module's dozens of cold DOSBox starts
    into a couple. Returns per-object reports + a module verdict (byte-exact iff every exec object
    rebuilds byte-for-byte)."""
    D = VxDDisasm(v).run()
    root = os.path.abspath(root)
    ranges = []                                            # (oi, lo, hi, oracle, linemap)
    masms = []                                             # each range's first-pass (probe) MASM
    for o in v.objects:
        if not o["exec"]:
            continue
        oi = o["idx"]
        oracle = D.code[oi]
        offs = sorted(a for (oo, a) in D.insns if oo == oi)
        rr = [(0, len(oracle))] if len(offs) <= max_ins else split_ranges(D, oi, max_ins)
        for lo, hi in rr:
            force0 = cross_range_force(D, oi, lo, hi)
            masm, linemap = emit_object_masm(D, oi, force_raw=force0, probe=True, crange=(lo, hi))
            ranges.append((oi, lo, hi, oracle, linemap))
            masms.append(masm)
    firsts = assemble_coff_batch(masms, root)              # batched first pass
    per_obj = {}
    for (oi, lo, hi, oracle, linemap), (obj, log) in zip(ranges, firsts):
        res = _converge_range(D, oi, root, lo, hi, oracle, max_repair, first=(obj, log, linemap))
        per_obj.setdefault(oi, []).append(res)
    reports = []
    for o in v.objects:
        if not o["exec"]:
            continue
        oi = o["idx"]
        oracle = D.code[oi]
        parts = per_obj.get(oi, [])
        sym = sum(p["sym"] for p in parts)
        raw = sum(p["raw"] for p in parts)
        match = all(p["match"] for p in parts) if parts else True
        asm = b"".join(p["text"] for p in parts)
        fr_all = set().union(*[p["force"] for p in parts]) if parts else set()
        reports.append({"ok": True, "obj": oi, "match": match, "oracle_len": len(oracle),
                        "asm_len": len(asm), "n_diffs": 0 if match else 1, "symbolic_insns": sym,
                        "forced_raw": raw, "force_raw_set": fr_all, "n_files": len(parts),
                        "raw_fallback": sym == 0 and (sym + raw) > 0, "_asm": asm[:len(oracle)],
                        "_oracle": oracle})
    allok = all(r["match"] for r in reports)
    return {"byte_exact": allok, "objects": reports,
            "symbolic": sum(r["symbolic_insns"] for r in reports),
            "forced_raw": sum(r["forced_raw"] for r in reports),
            "raw_fallback_objs": sum(1 for r in reports if r["raw_fallback"])}


if __name__ == "__main__":
    import sys
    from codebreaker.formats import le
    path = sys.argv[1]
    arg2 = sys.argv[2] if len(sys.argv) > 2 else "--module"
    modname = sys.argv[3] if len(sys.argv) > 3 else None
    mods = le.open_vxds(path)
    v = dict(mods).get(modname, mods[0][1]) if modname else mods[0][1]
    if arg2 == "--module":
        rep = compare_module(v)
        for r in rep["objects"]:
            if not r.get("ok"):
                print("  obj %-2d  ASSEMBLY FAILED: %s" % (r["obj"], r.get("error")))
            else:
                print("  obj %-2d  %5d bytes  symbolic=%-5d forced_raw=%-3d  %s" % (
                    r["obj"], r["oracle_len"], r["symbolic_insns"], r["forced_raw"],
                    "BYTE-EXACT" if r["match"] else "MISMATCH (%d diffs)" % r["n_diffs"]))
        print("== module %s: %s (symbolic=%d forced_raw=%d) ==" % (
            os.path.basename(path), "BYTE-EXACT" if rep["byte_exact"] else "NOT byte-exact",
            rep["symbolic"], rep["forced_raw"]))
    else:
        r = compare_object(v, int(arg2))
        print(r if not r.get("ok") else "obj %s: %s (symbolic=%d forced_raw=%d diffs=%d)" % (
            arg2, "BYTE-EXACT" if r["match"] else "MISMATCH", r["symbolic_insns"],
            r["forced_raw"], r["n_diffs"]))
