#!/usr/bin/env python3
"""
asmrecon32.py — byte-exact reconstruction of 32-bit PE code (the PE analog of
asmrecon.py).

asmrecon.py reconstructs a 16-bit NE function to assembly that RE-ASSEMBLES to
the shipped bytes, via MASM 4.00 under DOSBox with a db-fallback that guarantees
byte-exactness (worst case: all db). This module does the identical thing for the
Win95 32-bit engines — KERNEL32 / USER32 / GDI32 — the only differences being:

  * the section view is `pe_seg.PeSeg` (objdump -mi386, capstone carve) not
    `trace.Seg`;
  * the assembler is MASM 5.10A (`.386` / USE32 flat), driven through the same
    `masmlift` harness via its `masmroot` override; MASM 4.00 has no 386 support;
  * the relocation mask comes from the PE base-reloc table, not NE FIXUPP.

Everything else — the per-instruction listing diff, the two-phase convergence,
the db-fallback — is REUSED from asmrecon so the acceptance test is the same one
the whole no-LLM pipeline already rests on.

    python asmrecon32.py --selftest              # reconstruct a real USER32 fn
    python asmrecon32.py USER32.DLL --func 0x392f
"""
import argparse
import os
import re
import sys

import asmrecon
import masmlift
import objextract
import segdiff
from asmrecon import JMPCC, to_masm, _emit_db, _divergers, _cancelled
from pe_seg import PeSeg

# MASM 5.10A (386-capable), vendored under tools/ — kept separate from the
# proven 16-bit MASM 4.00 root so the NE path is never perturbed. Resolve the
# bundled copy first (frozen EXE unpacks to sys._MEIPASS), then the source tree.
def _masm32root():
    for base in (getattr(sys, "_MEIPASS", None),
                 os.path.dirname(os.path.abspath(__file__))):
        if base:
            cand = os.path.join(base, "masm32bin")
            if os.path.exists(os.path.join(cand, "BIN", "MASM.EXE")):
                return cand
    return os.path.join(os.path.dirname(os.path.abspath(__file__)), "masm32bin")


MASM32ROOT = _masm32root()

_MAXINSN = 15                 # longest legal x86 instruction; longer = objdump
                              # folded a data/zero run into one row -> db it whole


def wrap32(body):
    """Wrap a function body in a 32-bit flat USE32 code segment for MASM 5.10.
    `page ,132` widens the listing so MASM does not wrap a source line mid-way
    through its `; NNNNN` origin-offset comment — the per-instruction diff reads
    that comment, and a wrapped one silently misattributes the offset."""
    return "\n".join(
        ["        title   asmrecon32", "        page    ,132", "        .386"]
        + ["_TEXT   segment dword public use32 'CODE'", "        assume  cs:_TEXT"]
        + body + ["_TEXT   ends", "        end"]) + "\n"


# short-form branch opcodes (rel8, 2 bytes): if the shipped instruction used one,
# emit `SHORT` so MASM reproduces the 2-byte form instead of expanding a forward
# reference to a 6-byte near jump — keeps the branch LIVE and byte-exact.
_SHORT_JCC = set(range(0x70, 0x80)) | {0xEB, 0xE3, 0xE0, 0xE1, 0xE2}


def emit_function32(sg, start, end, force_db):
    """MASM body lines for [start,end) of a PE section. Intra-function branches
    become labels (self-relative, reproduce exactly); force_db rows, branches
    that leave the function, far ptrs and objdump-folded rows become db."""
    rows = [(o, a) for o, a in sg.rows if start <= o < end]
    starts = {o for o, _ in rows}
    labels = {}
    for o, a in rows:
        m = re.match(r"([a-z]+) 0x([0-9a-f]+)$", re.sub(r"\s+", " ", a).strip())
        if m and m.group(1) in JMPCC:
            t = int(m.group(2), 16)
            if start <= t < end and t in starts:
                labels[t] = "L_%05x" % t

    def is_target(o):
        return labels.get(o)

    code = sg.code
    off2idx = {o: i for i, (o, _) in enumerate(rows)}
    row_starts = sorted(o for o, _ in rows)
    lines, pos = [], start
    while pos < end:
        if pos in off2idx:
            i = off2idx[pos]
            off, a = rows[i]
            nxt = min(rows[i + 1][0] if i + 1 < len(rows) else end, end)
            raw = code[off:nxt]
            if off in labels:
                lines.append("%s:" % labels[off])
            masm = to_masm(a, is_target)
            mbr = re.match(r"([a-z]+) 0x([0-9a-f]+)$", re.sub(r"\s+", " ", a).strip())
            external_branch = bool(mbr and mbr.group(1) in JMPCC
                                   and int(mbr.group(2), 16) not in labels)
            # forward branch to an in-function label whose shipped form was short:
            # pin it SHORT so MASM does not grow it to a 6-byte near jump.
            if (mbr and mbr.group(1) in JMPCC and int(mbr.group(2), 16) in labels
                    and raw and raw[0] in _SHORT_JCC):
                op, _, tgt = masm.partition(" ")
                masm = "%s short %s" % (op, tgt.strip())
            far = bool(raw) and raw[0] in (0x9A, 0xEA)     # far call/jmp ptr16:32
            if (off in force_db or external_branch or far or "(bad)" in a
                    or len(raw) > _MAXINSN):
                _emit_db(lines, raw, off, masm)
            else:
                hexc = " ".join("%02x" % b for b in raw[:12]) + (
                    " ..." if len(raw) > 12 else "")
                lines.append("        %-32s ; %05x %s" % (masm, off, hexc))
            pos = nxt
        else:
            nextrow = min(next((o for o in row_starts if o > pos), end), end)
            if pos in labels:
                lines.append("%s:" % labels[pos])
            _emit_db(lines, code[pos:nextrow], pos, "data/uncovered")
            pos = nextrow
    return lines


def reconcile32(sg, start, end, seg_mask, max_passes=200):
    """Reconstruct PE [start,end) to byte-exact via db-fallback. Returns
    (asm_text, result, force_db, n_code_instr) — same contract as
    asmrecon.reconcile."""
    shipped = sg.code[start:end]
    smask = {m - start for m in seg_mask if start <= m < end}
    rows = sorted(o for o, _ in sg.rows if start <= o < end)
    spans = {o: (rows[i + 1] if i + 1 < len(rows) else end)
             for i, o in enumerate(rows)}
    err_re = re.compile(r"\((\d+)\)\s*:\s*(?:error|fatal)", re.I)
    off_re = re.compile(r";\s*([0-9a-f]{4,6})\b")
    force_db = set()
    errored = set()               # offsets MASM can't assemble = data, not code
    res = None
    for _ in range(max_passes):
        if _cancelled():
            break
        body = emit_function32(sg, start, end, force_db)
        asm = wrap32(body)
        asm_lines = asm.splitlines()
        r = masmlift.assemble(asm, masmroot=MASM32ROOT)
        errs = set()
        for m in err_re.finditer(r.get("log", "") + "\n" + r.get("lst", "")):
            ln = int(m.group(1))
            if 1 <= ln <= len(asm_lines):
                cm = off_re.search(asm_lines[ln - 1])
                if cm:
                    errs.add(int(cm.group(1), 16))
        if "obj" not in r or errs:
            new = errs - force_db
            if new:
                force_db |= new
                errored |= new
                continue
            if len(force_db) >= len(rows):
                break
            force_db = set(rows)
            continue
        _nm, our, omask = objextract.Obj(r["obj"]).code_segment()
        res = segdiff.diff(our, omask, shipped, smask)
        if res["exact"]:
            break
        lendiff, reencode = _divergers(r["lst"], our, omask, shipped, smask,
                                       spans, start)
        new_len = lendiff - force_db
        if new_len:
            force_db |= new_len
            continue
        new_re = reencode - force_db
        if not new_re:
            break
        force_db |= new_re
    code_rows = [o for o in rows if o not in errored]
    return asm, res, force_db - errored, len(code_rows)


def reconcile_many(sg, funcs, seg_mask, max_passes=200):
    """Batched PE reconcile: same per-function result as reconcile32(), one
    DOSBox boot per pass across the whole chunk. Returns list parallel to funcs."""
    jobs = []
    for start, end in funcs:
        shipped = sg.code[start:end]
        smask = {m - start for m in seg_mask if start <= m < end}
        rows = sorted(o for o, _ in sg.rows if start <= o < end)
        spans = {o: (rows[i + 1] if i + 1 < len(rows) else end)
                 for i, o in enumerate(rows)}
        jobs.append(dict(
            emit=(lambda fdb, s=start, e=end: emit_function32(sg, s, e, fdb)),
            shipped=shipped, smask=smask, rows=rows, spans=spans, start=start))
    return asmrecon.batch_reconcile(jobs, wrap32, MASM32ROOT, max_passes)


def _run(module, func_off=None, section=None, out=None):
    sg = PeSeg(module, section=section)
    if func_off is None:                       # smallest substantive carved fn
        cands = sorted((f["end"] - f["start"], s) for s, f in sg.funcs.items()
                       if 16 <= f["end"] - f["start"] <= 120)
        if not cands:
            sys.exit("no suitable function in %s" % module)
        func_off = cands[0][1]
    fn = sg.funcs.get(func_off)
    if not fn:
        sys.exit("no function carved at %s +%#x" % (module, func_off))
    start, end = fn["start"], fn["end"]
    asm, res, force_db, n = reconcile32(sg, start, end, sg.mask)
    live = n - len(force_db)
    nm = fn["name"] or "sub_%x" % (sg.va + start)
    print("%s %s +%#06x  %s  [%d bytes, %d instrs]"
          % (module, sg.name, start, nm, end - start, n))
    print("  verdict: %s" % ("BYTE-EXACT" if res and res["exact"] else "NOT EXACT"))
    print("  live: %d/%d instructions (%.0f%%), %d db"
          % (live, n, 100.0 * live / max(1, n), len(force_db)))
    if out:
        open(out, "w", newline="\r\n").write(asm)
        print("  wrote %s" % out)
    return asm, res


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("module", nargs="?")
    ap.add_argument("--func", type=lambda x: int(x, 0))
    ap.add_argument("--section")
    ap.add_argument("--out")
    ap.add_argument("--selftest", action="store_true")
    args = ap.parse_args()

    if args.selftest:
        print("asmrecon32 selftest on USER32.DLL\n")
        _asm, res = _run("USER32.DLL")
        ok = res is not None and res["exact"]
        print("\nSELFTEST:", "PASS - real PE function reconstructed byte-exact"
              if ok else "FAIL")
        sys.exit(0 if ok else 1)

    if not args.module:
        ap.error("give a module or --selftest")
    _asm, res = _run(args.module, args.func, args.section, args.out)
    sys.exit(0 if res and res["exact"] else 1)


if __name__ == "__main__":
    main()
