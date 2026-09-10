#!/usr/bin/env python3
"""
asmrecon.py — deterministic byte-exact reconstruction of hand-asm (stages C+F).

The 16-bit core (KERNEL/USER/GDI, the .drv/.386 drivers) was hand-written
assembly. The faithful reconstruction is assembly that RE-ASSEMBLES to the
shipped bytes — and that can be produced mechanically, with NO LLM and NO
permuter:

  1. disassemble the carved function (trace.Seg, objdump-verified)
  2. transcribe each instruction to MASM syntax; intra-function branches become
     labels (their self-relative displacement is position-independent, so it
     reproduces exactly); data / external calls become `db`
  3. assemble (masmlift) + diff (segdiff); any instruction MASM does not encode
     identically is rewritten `db <original bytes>` and we retry

The db-fallback GUARANTEES convergence to byte-exact (worst case: all db). The
score is "% live" — how much reassembles as real instructions vs db. This is the
sibling projects' proven technique, here composed from the CodeBreaker tools.

    python asmrecon.py --selftest            # reconstruct a real function, prove exact
    python asmrecon.py KEYBOARD --seg 1 --func 0x0100
"""
import argparse
import re
import sys

import masmlift
from codebreaker.format_loaders import objextract
import segdiff
import target
import trace

JMPCC = {"jmp", "call", "je", "jne", "jz", "jnz", "jl", "jle", "jg", "jge",
         "jb", "jbe", "ja", "jae", "jc", "jnc", "jo", "jno", "js", "jns",
         "jp", "jnp", "jpe", "jpo", "jcxz", "loop", "loope", "loopne",
         "loopz", "loopnz", "jecxz"}


def masm_hex(v):
    s = "%x" % v
    return ("0" + s + "h") if s and s[0] in "abcdef" else (s + "h")


def to_masm(asm, is_target):
    """One objdump (Intel) instruction -> MASM syntax. is_target(off)->label|None."""
    asm = re.sub(r"\s+", " ", asm).strip()
    pre = ""
    mpre = re.match(r"(rep|repz|repe|repnz|repne|lock) (.*)$", asm)
    if mpre:
        pre, asm = mpre.group(1) + " ", mpre.group(2)
    op, _, rest = asm.partition(" ")
    if op in ("lods", "stos", "movs", "scas", "cmps"):
        return pre + op + ("w" if "word ptr" in asm.lower() else "b")
    if op in JMPCC:
        m = re.match(r"0x([0-9a-f]+)$", rest)
        if m:
            lbl = is_target(int(m.group(1), 16))
            if lbl:
                return "%s %s" % (op, lbl)
    rest = (rest.replace("DWORD PTR", "dword ptr").replace("WORD PTR", "word ptr")
                .replace("BYTE PTR", "byte ptr").replace("FWORD PTR", "fword ptr"))
    rest = re.sub(r"0x([0-9a-f]+)", lambda m: masm_hex(int(m.group(1), 16)), rest)
    return (op + " " + rest).strip() if rest else op


RETF_MACRO = ["retf    macro   n", "        ifb     <n>", "        db      0cbh",
              "        else", "        db      0cah", "        dw      n",
              "        endif", "        endm", ""]


def _cancelled():
    try:
        from codebreaker.foundation import progress
        return progress.cancelled()
    except Exception:
        return False


def _dbline(raw, off, note):
    return ("        db      %-28s ; %04x %s"
            % (", ".join("0%02xh" % b if ("%x" % b)[0] in "abcdef" else "%02xh" % b
                         for b in raw), off, note))


def _emit_db(lines, raw, off, note):
    """Append db line(s) for `raw`, chunked at 16 bytes/line so no line exceeds
    MASM's ~128-char limit (objdump collapses zero runs, so a single 'instruction'
    row can span 100+ data bytes)."""
    for c in range(0, len(raw), 16):
        lines.append(_dbline(raw[c:c + 16], off + c, note if c == 0 else ""))


def _emit_data(lines, code, s, e, kind):
    """Emit a DATA region as READABLE data: printable text as `db 'text'`, else
    db hex. Byte-exact either way (same bytes)."""
    if kind != "str":
        _emit_db(lines, code[s:e], s, "data")
        return
    i = s
    while i < e:
        if 0x20 <= code[i] < 0x7F and code[i] != 0x27:
            j = i
            while j < e and 0x20 <= code[j] < 0x7F and code[j] != 0x27:
                j += 1
            lines.append("        db      %-24s ; %04x string"
                         % ("'%s'" % code[i:j].decode("latin1"), i))
            i = j
        else:
            j = i
            while j < e and not (0x20 <= code[j] < 0x7F and code[j] != 0x27):
                j += 1
            _emit_db(lines, code[i:j], i, "data")
            i = j


def emit_function(sg, start, end, force_db, data_spans=()):
    """MASM body lines for the function [start,end). force_db rows, external
    branches and far calls become db. `data_spans` [(s,e,kind)] (safe, row-
    aligned, no branch target inside) are emitted as readable DATA."""
    rows = [(o, a) for o, a in sg.rows if start <= o < end]
    starts = {o for o, _ in rows}
    labels = {}
    for o, a in rows:
        m = re.match(r"([a-z]+) 0x([0-9a-f]+)$", re.sub(r"\s+", " ", a).strip())
        if m and m.group(1) in JMPCC:
            t = int(m.group(2), 16)
            if start <= t < end and t in starts:
                labels[t] = "L_%04x" % t

    def is_target(o):
        return labels.get(o)

    dspans = sorted((s, e, k) for s, e, k in data_spans)

    def span_at(p):
        for s, e, k in dspans:
            if s <= p < e:
                return (s, e, k)
        return None

    def next_data(p):
        return min((s for s, _e, _k in dspans if s > p), default=end)

    code = sg.code
    off2idx = {o: i for i, (o, _) in enumerate(rows)}
    row_starts = sorted(o for o, _ in rows)
    lines = []
    pos = start
    while pos < end:
        sp = span_at(pos)
        if sp:                              # readable DATA region
            s, e, k = sp
            ee = min(e, end)
            if pos in labels:
                lines.append("%s:" % labels[pos])
            _emit_data(lines, code, pos, ee, k)
            pos = ee
            continue
        clamp = next_data(pos)
        if pos in off2idx:
            i = off2idx[pos]
            off, a = rows[i]
            nxt = min(rows[i + 1][0] if i + 1 < len(rows) else end, end, clamp)
            raw = code[off:nxt]
            if off in labels:
                lines.append("%s:" % labels[off])
            masm = to_masm(a, is_target)
            mbr = re.match(r"([a-z]+) 0x([0-9a-f]+)$", re.sub(r"\s+", " ", a).strip())
            external_branch = bool(mbr and mbr.group(1) in JMPCC
                                   and int(mbr.group(2), 16) not in labels)
            # far call/jmp (9A/EA) is an absolute seg:off pointer — inherently
            # external, not a live MASM target here; always db.
            far = bool(raw) and raw[0] in (0x9A, 0xEA)
            # raw longer than any real 16-bit instruction (>8 bytes) means objdump
            # collapsed a run of identical data bytes into this row (its `...`);
            # emitting the mnemonic live would DROP those bytes, so db the whole
            # raw. (This is invisible to the per-instruction check when the instr
            # is a macro like retf, so catch it here.)
            if off in force_db or external_branch or far or "(bad)" in a or len(raw) > 8:
                _emit_db(lines, raw, off, masm)
            else:
                hexc = " ".join("%02x" % b for b in raw[:10]) + (
                    " ..." if len(raw) > 10 else "")
                lines.append("        %-30s ; %04x %s" % (masm, off, hexc))
            pos = nxt
        else:
            # bytes not covered by any instruction row (carve start misaligned
            # with objdump, or embedded data) -> db, so coverage is complete and
            # the all-db fallback is always byte-exact.
            nextrow = min(next((o for o in row_starts if o > pos), end), clamp)
            if pos in labels:
                lines.append("%s:" % labels[pos])
            _emit_db(lines, code[pos:nextrow], pos, "data/uncovered")
            pos = nextrow
    return lines


def wrap(body):
    return "\n".join(["        title   asmrecon"] + RETF_MACRO
                     + ["_TEXT   segment byte public 'CODE'", "        assume  cs:_TEXT"]
                     + body + ["_TEXT   ends", "        end"]) + "\n"


_LST_OFF = re.compile(r"^\s{0,4}([0-9A-Fa-f]{4})\b")
# origoff comment width is 4 for NE (<=64 KB segments) but grows to 5-6 hex for
# PE sections (asmrecon32 reuses this) — match the whole run, not exactly four.
_LST_CMT = re.compile(r";\s*([0-9a-f]{4,6})\b")


def _divergers(lst_text, our, omask, shipped, smask, spans, start):
    """Per-instruction comparison via the MASM listing. For each emitted
    instruction the LST gives MASM's own offset (`moff`) and our source carries
    the original offset (`ooff`, the `; NNNN` comment); MASM's byte length is the
    delta to the next listed offset. Comparing instruction-to-instruction (not
    whole-buffer) is immune to the shift a length-differ causes downstream, so
    ONE pass finds every true diverger:
      * len(masm) != len(original)         -> length-differer, must db
      * same length but unmasked bytes differ -> re-encode, must db
    A relocation-masked displacement (a live near/far ref) differs but is masked
    on both sides, so it is NOT flagged — it stays live. Returns (lendiff,
    reencode) sets of original offsets: length-differers (fix FIRST — they shift
    the layout and can cascade a neighbouring jump into a different encoding) and
    same-length re-encodes (fix once the layout is aligned)."""
    # Build (masm_off, orig_off) for every BYTE-EMITTING listing line, in order.
    # A byte-emitting line has a leading offset AND a bytefield; label/segment
    # directive lines have an offset but no bytes (skip). MASM wraps a long `db`
    # line so its `; NNNN` origoff comment lands on the CONTINUATION line — pick
    # it up from there, or the db's bytes get misattributed to the prior instr's
    # length (the bug that snowballed length-differers).
    lines = lst_text.splitlines()
    pairs = []
    for idx, line in enumerate(lines):
        mo = _LST_OFF.match(line)
        if not mo:
            continue
        if not re.match(r"\s+[0-9A-Fa-f]{2}\b", line[mo.end():]):
            continue                           # directive/label: no bytefield
        co = _LST_CMT.search(line)
        if not co and idx + 1 < len(lines) and not _LST_OFF.match(lines[idx + 1]):
            co = _LST_CMT.search(lines[idx + 1])   # wrapped comment on next line
        pairs.append((int(mo.group(1), 16), int(co.group(1), 16) if co else None))
    lendiff, reencode = set(), set()
    for i, (moff, ooff) in enumerate(pairs):
        if ooff not in spans:                  # uncovered-data db line: already exact
            continue
        mlen = (pairs[i + 1][0] - moff) if i + 1 < len(pairs) else (len(our) - moff)
        olen = spans[ooff] - ooff
        oi = ooff - start
        if mlen != olen:
            lendiff.add(ooff)
            continue
        for k in range(olen):
            if (moff + k) in omask or (oi + k) in smask:
                continue
            if moff + k >= len(our) or oi + k >= len(shipped):
                break
            if our[moff + k] != shipped[oi + k]:
                reencode.add(ooff)
                break
    return lendiff, reencode


def _safe_data_spans(sg, start, end):
    """Safe data regions within [start,end): from reachability (no branch target
    inside), snapped to row boundaries, then RE-checked so snapping never absorbs
    a branch target. Guarantees no needed label is dropped -> byte-exact safe."""
    try:
        from codebreaker.disasm_engine import reachability
        regs = reachability.safe_data_regions(sg)
        bt = reachability.branch_targets(sg)
    except Exception:
        return []
    rowset = sorted(o for o, _ in sg.rows)
    import bisect
    out = []
    for s, e, k in regs:
        if e <= start or s >= end:
            continue
        lo = rowset[bisect.bisect_right(rowset, s) - 1] if rowset and rowset[0] <= s else s
        hi_i = bisect.bisect_left(rowset, e)
        hi = rowset[hi_i] if hi_i < len(rowset) else end
        lo, hi = max(lo, start), min(hi, end)
        if hi <= lo or any(lo <= t < hi for t in bt):     # snapping hit a target -> skip
            continue
        out.append((lo, hi, k))
    return out


def reconcile(sg, start, end, seg_mask, max_passes=200, verbose=True):
    """Reconstruct [start,end) to byte-exact via db-fallback. Safe data regions
    are emitted as readable data and excluded from the code live count. Returns
    (asm_text, result, force_db, n_code_instr)."""
    shipped = sg.code[start:end]
    smask = {m - start for m in seg_mask if start <= m < end}
    dspans = _safe_data_spans(sg, start, end)
    data_off = set()
    for s, e, _k in dspans:
        for o, _a in sg.rows:
            if s <= o < e:
                data_off.add(o)
    rows = sorted(o for o, _ in sg.rows if start <= o < end and o not in data_off)
    spans = {o: (rows[i + 1] if i + 1 < len(rows) else end) for i, o in enumerate(rows)}
    err_re = re.compile(r"\((\d+)\)\s*:\s*(?:error|fatal)", re.I)
    off_re = re.compile(r";\s*([0-9a-f]{4})\b")
    force_db = set()
    errored = set()          # offsets MASM can't assemble = definitively DATA
    res = None
    for _ in range(max_passes):
        if _cancelled():                       # Cancel pressed: bail immediately
            break
        body = emit_function(sg, start, end, force_db, dspans)
        asm = wrap(body)
        asm_lines = asm.splitlines()
        r = masmlift.assemble(asm)
        # localize MASM errors to the offending instruction(s) via the `; NNNN`
        # offset comment on each emitted line, and db just those -- NOT the whole
        # function -- so live instructions are preserved.
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
                errored |= new          # MASM errored on these -> data, not code
                continue
            if len(force_db) >= len(rows):      # can't localize: last-resort db-all
                break
            force_db = set(rows)
            continue
        _nm, our, omask = objextract.Obj(r["obj"]).code_segment()
        res = segdiff.diff(our, omask, shipped, smask)
        if res["exact"]:
            break
        # two-phase per-instruction compare via the listing. Phase 1: db all
        # LENGTH-differers and re-assemble — this restores the original layout,
        # so a neighbouring near-jump MASM would otherwise re-encode comes out
        # right and stays LIVE (minimises db). Phase 2 (only once no length
        # differs): db the true same-length re-encodes. Converges in a few passes
        # with near-minimal db.
        lendiff, reencode = _divergers(r["lst"], our, omask, shipped, smask, spans, start)
        new_len = lendiff - force_db
        if new_len:
            force_db |= new_len
            continue
        new_re = reencode - force_db
        if not new_re:
            break
        force_db |= new_re
    # MASM-error instructions are data (objdump garbage MASM can't assemble):
    # exclude them from the CODE count and the reported code-db so live% reflects
    # real instructions only. They stay db'd in the asm -> byte-exact unchanged.
    code_rows = [o for o in rows if o not in errored]
    return asm, res, force_db - errored, len(code_rows)


def batch_reconcile(jobs, wrap_fn, masmroot, max_passes=200):
    """Reconcile MANY functions together, assembling every not-yet-converged one
    in ONE DOSBox boot per pass (masmlift.assemble_many). The db-fallback and the
    two-phase per-instruction diff are IDENTICAL to reconcile(); only the
    assembler call is shared across functions — which removes the per-function
    DOSBox-boot cost that dominates wall-clock.

    jobs: list of dicts, each {emit: callable(force_db)->body_lines, shipped,
    smask, rows, spans, start}. wrap_fn wraps a body into a full .asm module;
    masmroot picks the assembler (MASM 4.00 for NE, 5.10A for PE). Returns a list
    parallel to jobs of (asm, result, force_db, n_code_instr) — same tuple
    reconcile() returns per function."""
    import masmlift
    import shutil
    err_re = re.compile(r"\((\d+)\)\s*:\s*(?:error|fatal)", re.I)
    off_re = re.compile(r";\s*([0-9a-f]{4,6})\b")
    st = [dict(job=j, force_db=set(), errored=set(), res=None, done=False,
               asm=None, lines=None) for j in jobs]
    for _p in range(max_passes):
        if _cancelled():
            break
        pend = [s for s in st if not s["done"]]
        if not pend:
            break
        items = []
        for i, s in enumerate(pend):
            try:
                body = s["job"]["emit"](s["force_db"])
                s["asm"] = wrap_fn(body)
                s["lines"] = s["asm"].splitlines()
                items.append(("F%d" % i, s["asm"]))
            except Exception:                  # a bad function can't kill the chunk
                s["done"] = True; s["res"] = None
        out = masmlift.assemble_many(items, masmroot=masmroot)
        if out.get("aborted"):
            break
        results = out.get("results", {})
        progressed = False
        for i, s in enumerate(pend):
            if s["done"]:                      # emit-failed this pass; skip
                continue
            j = s["job"]
            r = results.get("F%d" % i, {})
            errs = set()
            for m in err_re.finditer(r.get("log", "") + "\n" + r.get("lst", "")):
                ln = int(m.group(1))
                if 1 <= ln <= len(s["lines"]):
                    cm = off_re.search(s["lines"][ln - 1])
                    if cm:
                        errs.add(int(cm.group(1), 16))
            if "obj" not in r or errs:
                new = errs - s["force_db"]
                if new:
                    s["force_db"] |= new; s["errored"] |= new; progressed = True
                    continue
                if s["force_db"] >= set(j["rows"]):
                    s["done"] = True
                    continue
                s["force_db"] = set(j["rows"]); progressed = True
                continue
            _nm, our, omask = objextract.Obj(r["obj"]).code_segment()
            s["res"] = segdiff.diff(our, omask, j["shipped"], j["smask"])
            if s["res"]["exact"]:
                s["done"] = True
                continue
            lendiff, reencode = _divergers(r["lst"], our, omask, j["shipped"],
                                           j["smask"], j["spans"], j["start"])
            new_len = lendiff - s["force_db"]
            if new_len:
                s["force_db"] |= new_len; progressed = True
                continue
            new_re = reencode - s["force_db"]
            if new_re:
                s["force_db"] |= new_re; progressed = True
                continue
            # stuck but not exact: escalate to all-db (byte-exact by construction)
            if s["force_db"] != set(j["rows"]):
                s["force_db"] = set(j["rows"]); progressed = True
            else:
                s["done"] = True
        shutil.rmtree(out.get("work", ""), ignore_errors=True)
        if not progressed:
            break
    out_list = []
    for s in st:
        j = s["job"]
        code_rows = [o for o in j["rows"] if o not in s["errored"]]
        out_list.append((s["asm"], s["res"], s["force_db"] - s["errored"],
                         len(code_rows)))
    return out_list


def reconcile_many(sg, funcs, seg_mask, max_passes=200):
    """Batched NE reconcile: same per-function result as reconcile(), one boot
    per pass across the whole chunk. Returns list parallel to funcs."""
    jobs = []
    for start, end in funcs:
        shipped = sg.code[start:end]
        smask = {m - start for m in seg_mask if start <= m < end}
        dspans = _safe_data_spans(sg, start, end)
        data_off = set()
        for s, e, _k in dspans:
            for o, _a in sg.rows:
                if s <= o < e:
                    data_off.add(o)
        rows = sorted(o for o, _ in sg.rows if start <= o < end and o not in data_off)
        spans = {o: (rows[i + 1] if i + 1 < len(rows) else end)
                 for i, o in enumerate(rows)}
        jobs.append(dict(
            emit=(lambda fdb, s=start, e=end, ds=dspans: emit_function(sg, s, e, fdb, ds)),
            shipped=shipped, smask=smask, rows=rows, spans=spans, start=start))
    return batch_reconcile(jobs, wrap, masmlift.MASMROOT, max_passes)


def _pick():
    """Find a real, substantive (16..120 byte) carved function to reconstruct."""
    from codebreaker.format_loaders import ne as ne_mod
    o = trace.load_ordinals("ORDINALS.md")
    for name in ("MOUSE", "KEYBOARD", "SYSTEM", "GDI", "USER"):
        try:
            ne = ne_mod.NEFile(target.resolve(name))
        except Exception:
            continue
        for i, s in enumerate(ne.segments(), 1):
            if s.get("kind") != "CODE":
                continue
            sg = trace.Seg(ne, i, o)
            cands = sorted((f["end"] - f["start"], f["start"]) for f in sg.funcs.values()
                           if 16 <= f["end"] - f["start"] <= 120)
            if cands:
                return name, i, cands[0][1]
    return None, None, None


def _run(module, seg_index, func_off):
    from codebreaker.format_loaders import ne as ne_mod
    o = trace.load_ordinals("ORDINALS.md") if hasattr(trace, "load_ordinals") else {}
    ne = ne_mod.NEFile(target.resolve(module))
    sg = trace.Seg(ne, seg_index, o)
    _sb, seg_mask = segdiff.shipped_ne_segment(target.resolve(module), seg_index)
    if func_off is None:                       # smallest carved function in this seg
        cands = sorted(((f["end"] - f["start"], f["start"]) for f in sg.funcs.values()
                        if f["end"] > f["start"]))
        if not cands:
            sys.exit("no carved functions in %s seg%d" % (module, seg_index))
        func_off = cands[0][1]
    fn = sg.funcs.get(func_off)
    if not fn:
        sys.exit("no function carved at %s seg%d 0x%04x" % (module, seg_index, func_off))
    start, end = fn["start"], fn["end"]
    asm, res, force_db, n = reconcile(sg, start, end, seg_mask)
    live = n - len(force_db)
    print("%s seg%d func 0x%04x  [%d bytes, %d instrs]" % (module, seg_index, start,
          end - start, n))
    print("  verdict: %s" % ("BYTE-EXACT" if res and res["exact"] else "not exact"))
    print("  live: %d/%d instructions (%.0f%%), %d db"
          % (live, n, 100.0 * live / max(1, n), len(force_db)))
    return asm, res


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("module", nargs="?")
    ap.add_argument("--seg", type=int, default=1)
    ap.add_argument("--func", type=lambda x: int(x, 0))
    ap.add_argument("--selftest", action="store_true")
    ap.add_argument("--out", help="write the reconstructed .asm here")
    args = ap.parse_args()

    if args.selftest:
        module, seg, foff = _pick()
        if not module:
            sys.exit("no suitable NE function found for selftest")
        print("asmrecon selftest on %s\n" % module)
        _asm, res = _run(module, seg, foff)
        ok = res is not None and res["exact"]
        print("\nSELFTEST:", "PASS - real function reconstructed byte-exact"
              if ok else "FAIL")
        sys.exit(0 if ok else 1)

    if not args.module:
        ap.error("give a module or --selftest")
    asm, res = _run(args.module, args.seg, args.func)
    if args.out:
        open(args.out, "w", newline="\r\n").write(asm)
        print("  wrote %s" % args.out)
    sys.exit(0 if res and res["exact"] else 1)


if __name__ == "__main__":
    main()
