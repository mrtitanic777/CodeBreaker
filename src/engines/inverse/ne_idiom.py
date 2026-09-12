r"""ne_idiom.py -- re-emit a 16-bit NE code segment as idiomatic Microsoft SDK
cMacros source (cProc / cBegin / cEnd), the KERNEL/USER/GDI analog of the VxD
`vxd_idiom.py`.  The mechanical Track-A form (flat `_TEXT segment`, numeric
labels, cross-function branches pinned as `db`) becomes readable, structured
source that still assembles BYTE-EXACT through the period DDK ML.EXE + the
Windows SDK CMACROS.INC.

Assembly recipe (proven on the KRNL386 RIP validation cluster):
    OPTION M510        ; MASM 5.10 compat -- cMacros v5.20 uses the old `&' paste
    .286p              ; lar/lsl and friends
    ?PLM = 1 / ?WIN = 1
    include cmacros.inc
    sBegin CODE ... sEnd CODE
    ml.exe /c /Ic:\inc /Fom.obj m.asm      (16-bit OMF, no -coff)
_TEXT is extracted from the OMF (LEDATA) and compared with NE relocations masked
-- identical after the linker resolves the fixups, exactly like the VxD path.
"""
import os
import re
import sys
import glob
import shutil
import bisect

from src.disasm import trace
from src.engines.recon import asmrecon
from src.core import paths

# Direct-branch mnemonics whose single operand is a code offset (objdump Intel).
_JMP = {"jmp"}
_JCC = {"je", "jne", "jz", "jnz", "jl", "jle", "jg", "jge", "jb", "jbe", "ja",
        "jae", "js", "jns", "jo", "jno", "jp", "jnp", "jpe", "jpo", "jc", "jnc",
        "jcxz", "jecxz", "loop", "loope", "loopne", "loopz", "loopnz"}
_CALL = {"call"}

_PREAMBLE = [
    "; idiomatic recovered source -- Microsoft SDK cMacros (ne_idiom.py)",
    # OPTION M510: MASM 5.10 compat -- enables the cMacros `&' paste AND makes
    # .386 segments default to use16, so 16-bit code keeps its encoding while
    # 32-bit-operand (66/67-prefix) routines still assemble.
    "OPTION M510",
    "?PLM = 1",
    "?WIN = 1",
    ".xlist",
    "include cmacros.inc",
    ".list",
    "",
    "sBegin  CODE",
    # .386p INSIDE the open segment: sBegin already fixed _TEXT as use16 (16-bit
    # code keeps its encoding); .386p now enables the 386 registers/instructions
    # KERNEL's 32-bit-operand routines use (they get 66/67 prefixes in a use16 seg).
    ".386p",
    "        assumes cs,CODE",
    "        assumes ds,nothing",
    "        assumes es,nothing",
    "",
]


def _branch(objasm):
    """(op, target_off) for a direct jmp/jcc/call to a hex offset, else (None, None)."""
    s = re.sub(r"\s+", " ", objasm).strip()
    m = re.match(r"([a-z]+) 0x([0-9a-f]+)$", s)
    if m and (m.group(1) in _JMP or m.group(1) in _JCC or m.group(1) in _CALL):
        return m.group(1), int(m.group(2), 16)
    return None, None


def _procname(sg, off):
    """A cProc-legal name for the function at `off`: the export name if known,
    else a synthesized `sub_<off>`.  (Byte-exactness never depends on the name.)"""
    nm = None
    try:
        nm = sg.name_at(off)
    except Exception:
        nm = None
    if nm:
        nm = re.sub(r"[^A-Za-z0-9_$?@]", "_", nm)
        if nm[0].isdigit():
            nm = "_" + nm
        return nm
    return "sub_%04x" % off


def emit_range(sg, start, end, funcs=None, seg_i=1):
    """Idiomatic cMacros source text for [start,end).  See _emit() for details."""
    return _emit(sg, start, end, funcs=funcs, seg_i=seg_i)[0]


def _emit(sg, start, end, funcs=None, seg_i=1, force_db=(), data_off=()):
    """Emit functions covering [start,end) as idiomatic cMacros source text.

    force_db: offsets to pin as `db` (divergers the reconcile loop found).
    data_off: offsets inside safe-data regions to pin as `db`.

    Returns (text, unresolved) where `unresolved` is the set of segment offsets
    whose bytes are cross-range (extern) branch displacements -- unresolved (0000)
    in an isolated OBJ but correct once the whole segment is present, so range
    verification masks them exactly like a relocation.

    funcs: sorted list of function START offsets in the range (defaults to the
    carve's `sg.funcs`).  Every function is emitted `cProc name,<NEAR>` /
    `cBegin nogen` / body / `cEnd nogen` -- `nogen' adds no bytes, so the body is
    reproduced verbatim while gaining structure, names and symbolic branches.
    """
    if funcs is None:
        funcs = sorted(s for s in sg.funcs if start <= s < end)
    funcs = sorted(funcs)
    if not funcs:
        # a segment the carve found no function starts in (e.g. code that does
        # not open with `push bp'): treat the whole range as one function so it
        # is emitted rather than dropped to an empty segment (0 _TEXT).
        funcs = [start]
    fstart = set(funcs)
    fend = {}
    for i, s in enumerate(funcs):
        fend[s] = funcs[i + 1] if i + 1 < len(funcs) else end

    rows = [(o, a) for o, a in sg.rows if start <= o < end]
    off2idx = {o: i for i, (o, _) in enumerate(rows)}
    row_starts = sorted(o for o, _ in rows)
    code = sg.code

    # --- pass 1: collect every direct-branch target, classify in-range vs extern
    targets = {}                # off -> kind ('func' | 'local' | 'extern')
    for o, a in rows:
        op, tgt = _branch(a)
        if tgt is None:
            continue
        if tgt in fstart:
            targets[tgt] = "func"
        elif start <= tgt < end:
            targets.setdefault(tgt, "local")
        else:
            targets[tgt] = "extern"

    def label_of(tgt):
        k = targets.get(tgt)
        if k == "func":
            return _procname(sg, tgt)
        if k == "local":
            return "L_%04x" % tgt
        if k == "extern":
            return _procname(sg, tgt)          # a cross-range near ref
        return None

    externs = sorted(t for t, k in targets.items() if k == "extern")

    # --- pass 2: emit each function
    body = []
    unresolved = set()
    local_labels = {t for t, k in targets.items() if k == "local"}
    if funcs and funcs[0] > start:            # range opened mid-function: db the lead-in
        asmrecon._emit_db(body, code[start:funcs[0]], start, "data/lead-in")
    for s in funcs:
        e = fend[s]
        name = _procname(sg, s)
        # near vs far from the terminating instruction (retf/ret far -> FAR)
        far = False
        for o, a in rows:
            if s <= o < e and re.match(r"(retf|ret f|lret)", a.strip()):
                far = True
        body.append("cProc   %s,<%s>" % (name, "FAR" if far else "NEAR"))
        body.append("cBegin  nogen")
        # Walk EVERY byte of [s,e): a covered offset emits its instruction (live,
        # symbolic, or db), an uncovered gap is db'd up to the next covered offset
        # -- so no byte is ever dropped even where objdump mis-decodes a run.
        pos = s
        while pos < e:
            if pos in local_labels:
                body.append("L_%04x:" % pos)
            if pos not in off2idx:              # uncovered bytes -> db
                j = bisect.bisect_right(row_starts, pos)
                nxt = min(row_starts[j] if j < len(row_starts) else e, e)
                if nxt <= pos:
                    nxt = e
                asmrecon._emit_db(body, code[pos:nxt], pos, "data/uncovered")
                pos = nxt
                continue
            i = off2idx[pos]
            off, a = rows[i]
            nxt = min(rows[i + 1][0] if i + 1 < len(rows) else e, e)
            raw = code[off:nxt]
            op, tgt = _branch(a)
            far_ins = bool(raw) and raw[0] in (0x9A, 0xEA)
            if off in force_db or off in data_off or far_ins or "(bad)" in a or len(raw) > 8:
                # pinned as data: a diverger, a safe-data region, a far seg:off
                # pointer, or a run objdump collapsed -- emit the shipped bytes.
                asmrecon._emit_db(body, raw, off, asmrecon.to_masm(a, lambda o: None))
                pos = nxt
                continue
            emitted = None
            if op and tgt is not None:
                lbl = label_of(tgt)
                if lbl:
                    short = (len(raw) == 2)     # eb/7x rel8
                    if op in _JMP:
                        emitted = "jmp     short %s" % lbl if short else "jmp     %s" % lbl
                    elif op in _CALL:
                        emitted = "call    %s" % lbl
                    else:                       # jcc: rel8 short, rel16 needs .386
                        emitted = "%-7s %s" % (op, ("short " + lbl) if short else lbl)
                    if targets.get(tgt) == "extern":  # disp bytes are unresolved
                        unresolved.update(range(off + 1, nxt))
            if emitted is not None:
                body.append("        %-30s ; %04x" % (emitted, off))
            else:
                body.append("        %-30s ; %04x" % (asmrecon.to_masm(a, lambda o: None), off))
            pos = nxt
        body.append("cEnd    nogen")
        body.append("")

    ext_lines = ["        externNP %s" % _procname(sg, t) for t in externs]
    text = "\n".join(_PREAMBLE + body + ext_lines + ["sEnd    CODE", "        end", ""])
    return text, unresolved


# --------------------------------------------------------------------------
# verify: assemble via ML + CMACROS.INC (16-bit OMF) and diff _TEXT vs shipped
# --------------------------------------------------------------------------
def _stage_cmacros(root):
    from src.engines.inverse import vxd_masm
    tc = vxd_masm._stage_toolchain(root)
    incdst = os.path.join(tc, "inc")
    os.makedirs(incdst, exist_ok=True)
    inc16 = paths.reference("Microsoft Windows 95 DDK", "INC16")
    for f in glob.glob(inc16 + "/*.INC"):
        d = os.path.join(incdst, os.path.basename(f))
        if not os.path.exists(d):
            shutil.copy(f, d)
    return tc


def assemble(asm_text, root="."):
    """Assemble idiomatic cMacros source with ML + CMACROS.INC (16-bit OMF).
    Returns {obj: bytes|None, lst: str, log: str} (the `lst' drives db-fallback)."""
    from src.toolchain.backbone.dosbox import run_dos
    tc = _stage_cmacros(root)
    _ok, out, work = run_dos(
        {"c": tc},
        [r"c:\ml.exe /c /Ic:\inc /Fom.obj /Flm.lst m.asm > a.txt"],
        ["m.obj", "m.lst", "a.txt"], files={"m.asm": asm_text}, timeout=300)

    def _rd(n):
        p = os.path.join(work, n) if work else None
        return open(p, errors="replace").read() if p and os.path.exists(p) else ""
    return {"obj": out.get("m.obj"), "lst": _rd("m.lst"), "log": _rd("a.txt")}


# --------------------------------------------------------------------------
# DOSBox-free backend: the nasm-masm fork assembles cMacros natively (its
# cmacros_shim.inc replaces CMACROS.INC), ~100x faster than ML-under-DOSBox.
# nasm picks different-but-equivalent encodings than ML on ambiguous cases,
# so the reconcile db-pins nasm's OWN divergers (found by direct byte compare,
# since nasm's listing format differs from ML's).
# --------------------------------------------------------------------------
def _nasm_bin():
    return os.environ.get(
        "NASM_MASM", r"C:\Users\mrtit\Documents\GitHub\nasm-masm\nasm.exe")


def _nasm_inc():
    """A space-free include dir holding cmacros_shim.inc (nasm redirects
    `include cmacros.inc' -> it)."""
    import tempfile
    d = os.path.join(tempfile.gettempdir(), "ne_idiom_nasm_inc")
    os.makedirs(d, exist_ok=True)
    shim = os.path.join(os.path.dirname(_nasm_bin()), "test", "masm",
                        "cmacros_shim.inc")
    dst = os.path.join(d, "cmacros_shim.inc")
    if os.path.exists(shim) and (not os.path.exists(dst)
                                 or os.path.getmtime(shim) > os.path.getmtime(dst)):
        shutil.copy(shim, dst)
    return d


def assemble_nasm(asm_text):
    """Assemble idiomatic cMacros source with the nasm-masm fork (16-bit OMF),
    no DOSBox.  Returns {obj: bytes|None, lst: str, log: str}."""
    import subprocess
    import tempfile
    inc = _nasm_inc()
    work = tempfile.mkdtemp(prefix="ne_nasm_")
    src = os.path.join(work, "m.asm")
    obj = os.path.join(work, "m.obj")
    lst = os.path.join(work, "m.lst")
    open(src, "w", newline="\n").write(asm_text)
    try:
        p = subprocess.run([_nasm_bin(), "--masm", "-f", "obj", "-I", inc + os.sep,
                            "-l", lst, "-o", obj, src],
                           capture_output=True, text=True, timeout=120)
        log = (p.stderr or "") + (p.stdout or "")
    except Exception as e:
        log = repr(e)
    data = open(obj, "rb").read() if os.path.exists(obj) else None
    lstx = open(lst, errors="replace").read() if os.path.exists(lst) else ""
    shutil.rmtree(work, ignore_errors=True)               # don't leak a tempdir per pass
    return {"obj": data, "lst": lstx, "log": log}


# nasm listing byte line: "<src#> <offset8> <bytes...> <source ; NNNN>"
# (shim-expansion lines carry a `<N>' marker and no offset -- they don't match).
_NLST = re.compile(r"^\s*\d+\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{2})")


def _nasm_divergers(lst, our, omask, shipped, smask, spans, start):
    """asmrecon._divergers for the nasm-masm listing format (line#/src#/offset8).
    Returns (lendiff, reencode) sets of original offsets to db, exactly like the
    ML path: fix length-differers first, then same-length re-encodes."""
    pairs = []
    for ln in lst.splitlines():
        m = _NLST.match(ln)
        if not m:
            continue
        co = re.search(r";\s*([0-9a-f]{4})\b", ln)
        pairs.append((int(m.group(1), 16), int(co.group(1), 16) if co else None))
    lendiff, reencode = set(), set()
    for i, (moff, ooff) in enumerate(pairs):
        if ooff not in spans:
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


def reconcile_range(sg, start, end, funcs=None, root=".", max_passes=60, backend="ml"):
    """Emit [start,end) idiomatic, assemble via ML+CMACROS, and db-fallback each
    diverging instruction until byte-exact -- asmrecon's reconcile technique with
    the cMacros emitter.  A whole range is one assembly unit, so one instruction
    ML re-encodes (length or same-length) shifts the rest: find those via the
    listing and pin them as `db` (byte-exact), keeping everything else live and
    idiomatic.  Returns (asm_text, result, force_db, n_code)."""
    from src.formats import objextract
    from src.engines.recon import asmrecon, segdiff
    shipped = sg.code[start:end]
    seg_mask = getattr(sg, "mask", set()) or set()
    smask = {m - start for m in seg_mask if start <= m < end}
    dspans = asmrecon._safe_data_spans(sg, start, end)
    data_off = set()
    for s, e, _k in dspans:
        for o, _a in sg.rows:
            if s <= o < e:
                data_off.add(o)
    rows = sorted(o for o, _ in sg.rows if start <= o < end and o not in data_off)
    spans = {o: (rows[i + 1] if i + 1 < len(rows) else end)
             for i, o in enumerate(rows)}
    funcs_list = sorted(f for f in (funcs if funcs is not None else sg.funcs)
                        if start <= f < end)

    def _func_of(absoff):
        """The function range [s,e) containing absoff (for whole-function db)."""
        j = bisect.bisect_right(funcs_list, absoff) - 1
        if 0 <= j < len(funcs_list):
            s = funcs_list[j]
            return s, (funcs_list[j + 1] if j + 1 < len(funcs_list) else end)
        return None
    err_re = (re.compile(r":(\d+):\s*(?:error|fatal)", re.I) if backend == "nasm"
              else re.compile(r"\((\d+)\)\s*:\s*(?:error|fatal)", re.I))
    off_re = re.compile(r";\s*([0-9a-f]{4})\b")
    force_db, errored, res, asm = set(), set(), None, ""
    for _ in range(max_passes):
        asm, unresolved = _emit(sg, start, end, funcs=funcs,
                                force_db=force_db, data_off=data_off)
        asm_lines = asm.splitlines()
        r = assemble_nasm(asm) if backend == "nasm" else assemble(asm, root)
        # localize any MASM error to the offending original offset via its `; NNNN'
        # comment and db just that instruction (it is data MASM cannot encode).
        errs = set()
        for m in err_re.finditer((r.get("log") or "") + "\n" + (r.get("lst") or "")):
            ln = int(m.group(1))
            if 1 <= ln <= len(asm_lines):
                cm = off_re.search(asm_lines[ln - 1])
                if cm:
                    errs.add(int(cm.group(1), 16))
        if not r.get("obj") or errs:
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
        omask = set(omask) | {u - start for u in unresolved}
        res = segdiff.diff(our, omask, shipped, smask)
        if res.get("exact"):
            break
        if backend == "nasm":
            lendiff, reencode = _nasm_divergers(r.get("lst") or "", our, omask,
                                                shipped, smask, spans, start)
        else:
            lendiff, reencode = asmrecon._divergers(r["lst"], our, omask, shipped,
                                                    smask, spans, start)
        new_len = lendiff - force_db           # phase 1: fix length-differers first
        if new_len:
            force_db |= new_len
            continue
        new_re = reencode - force_db           # phase 2: same-length re-encodes
        if new_re:
            force_db |= new_re
            continue
        # last resort: the listing can't localize the divergence (a mis-decoded
        # region, e.g. an inline jump table objdump read as code). db the whole
        # function containing the first diff -- pure db is byte-exact in a
        # contiguous emission -- which re-aligns everything after it.
        fd = res.get("first_diff")
        fr = _func_of(start + fd) if fd is not None else None
        if fr:
            rr = {o for o in rows if fr[0] <= o < fr[1]}
            if rr - force_db:
                force_db |= rr
                continue
        break
    code_rows = [o for o in rows if o not in errored]
    return asm, res, force_db - errored, len(code_rows)


def verify_range(sg, start, end, funcs=None, root=".", backend="ml"):
    """db-fallback [start,end) to byte-exact idiomatic cMacros source.  Returns a
    result dict: ok, n_code (instructions), db (pinned), live, and the asm.
    backend='nasm' uses the fast DOSBox-free nasm-masm fork (byte-exact via its
    own db-fallback); 'ml' uses period ML+CMACROS under DOSBox."""
    asm, res, force_db, n_code = reconcile_range(sg, start, end, funcs=funcs, root=root, backend=backend)
    exact = bool(res and res.get("exact"))
    out = {"ok": exact, "n": end - start, "n_code": n_code,
           "db": len(force_db), "live": n_code - len(force_db), "asm": asm}
    if not exact and res is not None:
        out["res"] = {k: v for k, v in res.items() if k != "asm"}
    return out


if __name__ == "__main__":
    os.chdir(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
    sys.path.insert(0, ".")
    from src.formats import ne as ne_mod
    from src.disasm import trace as _t
    from src.core import cores
    cores.set_target("engine/targets/win95")
    target = cores.get('target')
    module = sys.argv[1] if len(sys.argv) > 1 else "KRNL386.EXE"
    seg_i = int(sys.argv[2]) if len(sys.argv) > 2 else 1
    lo = int(sys.argv[3], 0) if len(sys.argv) > 3 else 0x04a4
    hi_arg = sys.argv[4] if len(sys.argv) > 4 else "0x4fa"   # a number or "whole"
    save = os.environ.get("NE_SAVE")                          # write the .asm here
    try:
        path = target.resolve(module)
    except Exception:
        hits = glob.glob(paths.data_root() + "/files/WINDOWS/**/" + module, recursive=True) or \
               glob.glob(paths.data_root() + "/files/WINDOWS/**/" + module.lower(), recursive=True)
        path = hits[0] if hits else module
    ne = ne_mod.NEFile(path)
    o = _t.load_ordinals("ORDINALS.md")
    # apply recovered real internal names if a names.json exists for this module
    # (see debug_names.py) -- a pure label rename, byte-exactness is unaffected.
    override = {}
    tag = os.path.splitext(os.path.basename(module))[0].lower()
    names_json = os.path.join(paths.data_root() + "/src/idiomatic", tag, "names.json")
    if os.path.exists(names_json):
        from src.engines.inverse import debug_names
        override = debug_names.load_override(names_json)
        print("applying %d recovered real names from %s" % (
            sum(len(v) for v in override.values()), names_json))
    sg = None
    for i, s in enumerate(ne.segments(), 1):
        if i == seg_i and s.get("kind") == "CODE":
            sg = _t.Seg(ne, i, o, name_override=override.get(i))
            break
    if sg is None:
        print("no code segment", seg_i); sys.exit(1)
    hi = len(sg.code) if hi_arg == "whole" else int(hi_arg, 0)
    r = verify_range(sg, lo, hi)
    print("=== %s seg%d [0x%x,0x%x) ===" % (module, seg_i, lo, hi))
    if r["ok"]:
        pct = (100.0 * r["live"] / r["n_code"]) if r["n_code"] else 100.0
        print("*** BYTE-EXACT (%d bytes) -- %d/%d instructions live idiomatic, "
              "%d db-pinned (%.1f%% live) ***" % (r["n"], r["live"], r["n_code"],
                                                  r["db"], pct))
        if save:
            os.makedirs(os.path.dirname(os.path.abspath(save)), exist_ok=True)
            open(save, "w", newline="\r\n").write(r["asm"])
            print("saved idiomatic source -> %s (%d lines)" % (save, r["asm"].count("\n")))
    else:
        print("NOT exact:", {k: v for k, v in r.items() if k != "asm"})
