#!/usr/bin/env python3
r"""ghidra_engine.py -- a second decompiler behind the same verifier.

Ghidra's decompiler and ours fail on different functions. Run through the SAME strict test, each
verifies about a quarter of comctl32 -- but only two thirds of those are the same functions, so
taking whichever verifies recovers a third more than either alone. This module produces Ghidra's
reconstruction of a module in a form the verifier can judge:

  * the module is decompiled once by Ghidra (headless, stock CppExporter) and the export cached;
  * its global variables (`DAT_bff10088`) are bound to the ORIGINAL's addresses, as our own
    decompiler's literals are, so both sides of a comparison read and write the same memory;
  * imported functions get their real calling convention (apiarity);
  * the whole module is compiled as one unit with OpenWatcom, dropping exactly the functions the
    compiler rejects, so every function runs against the others' real code.

Results are stored under the lane name "pe-ghidra" beside our own ("pe"); module_source and the
scoreboard take the better of the two for each function and say which engine it came from.
"""
import hashlib
import os
import re
import subprocess

from src.core import corpus

LANE = "pe-ghidra"
REV = 5                                    # bump when the adaptation below changes what is compiled
_FIXUPS = ("typedef unsigned int pointer32;\ntypedef unsigned int undefined3;\ntypedef void *pointer;\n"
           "typedef unsigned long long ulonglong;\ntypedef long long longlong;\n"
           "#define true 1\n#define false 0\n")


def export_path(module_path):
    d = os.path.join(corpus.design_dir(), "bank", "ghidra")
    os.makedirs(d, exist_ok=True)
    return os.path.join(d, os.path.basename(module_path).lower() + ".c")


def export(module_path, log=None):
    """Ghidra's C export of the module (cached). Returns the .c path, or None if Ghidra failed."""
    out = export_path(module_path)
    if os.path.exists(out) and os.path.exists(os.path.splitext(out)[0] + ".h"):
        return out
    try:
        from src.toolchain.ghidra import ghidra_decomp
        r = ghidra_decomp.decompile_export_one(module_path, out)
    except Exception as e:                             # Ghidra absent or misconfigured
        if log:
            log("ghidra export failed: %s" % e)
        return None
    if log and isinstance(r, dict) and r.get("skip"):
        log("ghidra export skipped: %s" % r["skip"])
    return out if os.path.exists(out) else None


def parse(c_path):
    """(globals lines, {name: text}, [names in order]) from a Ghidra C export."""
    lines = open(c_path, encoding="utf-8", errors="replace").read().split("\n")
    funcs, order, first, i = {}, [], None, 0
    while i < len(lines):
        ln = lines[i]
        if ln and ln[0] not in " \t/{}#" and ln.rstrip().endswith(")") and "(" in ln and ";" not in ln:
            j = i + 1
            while j < len(lines) and lines[j].strip() == "":
                j += 1
            if j < len(lines) and lines[j].startswith("{"):
                k = j
                while k < len(lines) and lines[k] != "}":
                    k += 1
                m = re.search(r"([A-Za-z_]\w*)\s*\([^()]*\)\s*$", ln) or re.search(r"([A-Za-z_]\w*)\s*\(", ln)
                name = m.group(1)
                if first is None:
                    first = i
                if name not in funcs:
                    funcs[name] = "\n".join(lines[i:k + 1])
                    order.append(name)
                i = k + 1
                continue
        i += 1
    return lines[:first or 0], funcs, order


def adapt(module_path, c_path):
    """-> (preamble, {name: text}, [names], {va: name}, [import names used]).

    The preamble is Ghidra's header made acceptable to a C compiler, its globals bound to the
    original's addresses, and imports declared with the convention they really use."""
    from src.engines.behavior import apiarity
    from src.formats.pe import open_pe
    from src.targets.win95 import bodyverify as B
    globs, funcs, order = parse(c_path)
    header = open(os.path.splitext(c_path)[0] + ".h", encoding="utf-8", errors="replace").read()

    def clean(t):
        """Spellings a C compiler does not take: a name Ghidra could not settle
        (`FID_conflict:x`) and the C++-only __thiscall."""
        return re.sub(r"\b__thiscall\b", "", t.replace("FID_conflict:", "FID_conflict_"))
    header = clean(header)
    globs = [clean(g) for g in globs]
    order = [clean(n) for n in order]
    funcs = {clean(n): clean(t) for n, t in funcs.items()}
    # Ghidra names a string after its text, which need not be an identifier
    # (`s_SOFTWARE\Microsoft\..._bfeca530`, `s_%lu%c%03lu_7fe9f17c`). Rename those by address.
    renames = {}
    for i, g in enumerate(globs):
        m = re.match(r"^(string|unicode)\s+(.+_([0-9a-f]{8}))(\[\d*\])?;\s*$", g)
        if m and not re.fullmatch(r"[A-Za-z_]\w*", m.group(2)):
            safe = "%s_%s" % ("s" if m.group(1) == "string" else "u", m.group(3))
            renames[m.group(2)] = safe
            globs[i] = "%s %s%s;" % (m.group(1), safe, m.group(4) or "")
    if renames:
        for old in sorted(renames, key=len, reverse=True):
            for n in funcs:
                if old in funcs[n]:
                    funcs[n] = funcs[n].replace(old, renames[old])
    text = "\n".join(funcs.values())
    hdr = _FIXUPS + header
    hdr = re.sub(r"\{\s*\}\s*;", "{ int _empty; };", hdr)            # C has no empty structs
    # Ghidra declares a function void and then uses its result elsewhere: every one returns int.
    hdr = re.sub(r"(?m)^void\s+(?=(?:__\w+\s+)?[A-Za-z_]\w*\s*\()", "int ", hdr)
    for n in list(funcs):
        funcs[n] = re.sub(r"\Avoid\s+", "int ", funcs[n])
    macros, plain = [], []
    for g in globs:
        m = re.match(r"^(.*?)\b([A-Za-z_]\w*?_([0-9a-f]{8}))(\[\d*\])?;\s*$", g)
        if not m:
            if g.strip() and not g.startswith("#include"):
                plain.append(g)
            continue
        ty, name, addr, arr = m.group(1).strip(), m.group(2), m.group(3), m.group(4)
        if name in funcs:
            continue
        if ty in ("string", "unicode"):                 # text: the name stands for its address
            macros.append("#define %s ((%s *)0x%s)" % (name, "char" if ty == "string" else "unsigned short", addr))
        else:
            macros.append(("#define %s ((%s *)0x%s)" if arr else "#define %s (*(%s *)0x%s)") % (name, ty, addr))
    for a in sorted(set(re.findall(r"\b_DAT_([0-9a-f]{8})\b", text))):  # the same cell at another width
        macros.append("#define _DAT_%s (*(unsigned int *)0x%s)" % (a, a))
    imports = B.import_map(module_path)
    called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", text))
    used = sorted(n for n in called if n in imports and n not in funcs)
    extra = []
    for n in used:
        conv = "__cdecl" if apiarity.stack_bytes(imports[n], n) == 0 else "__stdcall"
        pat = re.compile(r"(?m)^([^\n;(]*?)\b(?:__stdcall\s+|__cdecl\s+)?%s\s*\(([^;]*)\);" % re.escape(n))
        if pat.search(hdr):
            hdr = pat.sub(lambda mm: "%s %s %s(%s);" % (mm.group(1).rstrip(), conv, n, mm.group(2)), hdr, count=1)
        else:
            extra.append("int %s %s();" % (conv, n))
    preamble = "\n".join([hdr] + extra + plain + macros) + "\n"
    pe = open_pe(module_path)
    by_va = {}
    for n in funcs:
        m = re.fullmatch(r"FUN_([0-9a-f]{8})", n)
        if m:
            by_va[int(m.group(1), 16)] = n
    for nm, rva in pe.export_addrs().items():
        if nm in funcs:
            by_va[pe.image_base + rva] = nm
    return preamble, funcs, order, by_va, used


def build(module_path, preamble, funcs, order, used, outdir, rounds=12, cflags=None):
    """Compile the whole module, dropping the functions the compiler rejects.
    -> (dll | None, [names built], {dropped name: compiler message})."""
    from src.targets.win95 import bodyverify as B
    ow = B._ow()
    env = B._ow_env(ow)
    imports = B.import_map(module_path)
    os.makedirs(outdir, exist_ok=True)
    names, dropped = list(order), {}

    def stand_in(n):
        """A dropped function keeps its place with the same signature and an empty body, so its
        callers still link and their stack stays balanced (they are then marked stub-callee)."""
        head = " ".join(funcs[n].split("{", 1)[0].split())
        return "%s { return 0; }" % head if head.endswith(")") else ""
    for rnd in range(rounds):
        src = (preamble + "\n\n".join("/*@@ %s */\n%s" % (n, funcs[n]) for n in names) + "\n"
               + "/*@@ (stand-ins) */\n" + "\n".join(stand_in(n) for n in order if n in dropped) + "\n")
        base = "g%d" % rnd
        with open(os.path.join(outdir, base + ".c"), "w", newline="\n", encoding="utf-8") as f:
            f.write(src)
        r = subprocess.run([ow["wcc"]] + (cflags or B._CFLAGS) + ["-e=100000", base + ".c"], cwd=outdir, env=env,
                           capture_output=True, text=True, timeout=1200)
        if r.returncode == 0:
            obj = open(os.path.join(outdir, base + ".obj"), "rb").read()
            with open(os.path.join(outdir, base + ".lnk"), "w", newline="\n") as f:
                # undefsok: a call to a dropped function is left unresolved (it then fails the test)
                f.write("system nt_dll\nname %s.dll\noption quiet\noption undefsok\nfile %s.obj\n" % (base, base)
                        + B._RTL + B._import_lines(obj, set(used), imports))
                # The public symbol of each function as the compiler decorated it: _name (cdecl),
                # _name@N (stdcall), @name@N (fastcall). A function with no such symbol is not exported.
                from src.targets.win95 import autobuild95 as A
                pubs = set(A._omf_publics(obj))
                for n in names:
                    sym = next((s for s in pubs if s == "_" + n or s == n or
                                re.fullmatch(r"[_@]%s@\d+" % re.escape(n), s)), None)
                    if sym:
                        f.write("export %s='%s'\n" % (n, sym))
            subprocess.run([ow["wlink"], "@%s.lnk" % base], cwd=outdir, env=env, capture_output=True, text=True)
            dll = os.path.join(outdir, base + ".dll")
            return (dll if os.path.exists(dll) else None), names, dropped
        starts = [(k + 1, ln[5:-3]) for k, ln in enumerate(src.split("\n"))
                  if ln.startswith("/*@@ ") and ln.endswith(" */")]
        errs = (r.stdout or "") + (r.stderr or "")
        try:
            errs += open(os.path.join(outdir, base + ".err"), errors="replace").read()
        except OSError:
            pass
        bad = {}
        for m in re.finditer(r"\((\d+)\): Error! (E\d+: [^\r\n]*)", errs):
            ln, owner = int(m.group(1)), None
            for st, fn in starts:
                if st <= ln:
                    owner = fn
                else:
                    break
            if owner and owner in funcs:
                bad.setdefault(owner, m.group(2)[:100])
        if not bad:
            break                                       # the preamble itself does not compile
        dropped.update(bad)
        names = [n for n in names if n not in bad]
    return None, [], dropped


CORE_LANE = "pe-core"


def verify(module_path, funcs=None, store=None, outdir=None, log=None, engine="export"):
    """Judge a second decompiler's reconstruction of each function. -> ({key: result}, preamble),
    results as from pipeline.verify, each also carrying "source" (the function's text).

    engine="export": Ghidra run headless, its C export adapted (lane "pe-ghidra").
    engine="core":   the Ghidra decompiler core embedded in CodeBreaker and fed our facts
                     (lane "pe-core") -- see ghidra_core.py."""
    from src.formats.pe import open_pe
    from src.targets.win95 import bodyverify as B, verify95 as V
    from src.targets.win95.verification import pipeline
    pipeline._arm()
    L = pipeline.LANES["pe"]
    if funcs is None:
        funcs = L.functions(module_path)
    cflags = None
    if engine == "core":
        from src.targets.win95.verification import ghidra_core
        if not ghidra_core.available():
            return {}, ""
        lane, rev = CORE_LANE, ghidra_core.REV
        work = B._scratch(lane, os.path.basename(module_path).lower())
        preamble, gfuncs, order, by_va, used = ghidra_core.adapt(module_path, L.functions(module_path), work)
        cflags = ghidra_core.CFLAGS
    else:
        lane, rev = LANE, REV
        c_path = export(module_path, log)
        if not c_path:
            return {}, ""
        preamble, gfuncs, order, by_va, used = adapt(module_path, c_path)
    digest = pipeline.profile("pe")[0]
    whole = hashlib.sha1((preamble + "".join(gfuncs[n] for n in order)).encode("utf-8", "replace")).hexdigest()
    sig = pipeline._sha(digest, lane, rev, whole)
    mod_sha = V._mod_sha(module_path) or ""
    known = store.get_many(lane, mod_sha) if (store and mod_sha) else {}
    results = {}
    fresh = [va for va in funcs if (known.get(L.key(va)) or {}).get("sig") != sig]
    for va in funcs:
        row = known.get(L.key(va))
        if row and row.get("sig") == sig:
            n = by_va.get(va)
            results[L.key(va)] = {"verdict": row["verdict"], "fname": row.get("fname") or "",
                                  "reason": row.get("reason") or "", "limits": row.get("limits") or "",
                                  "detail": row.get("detail") or "", "cached": True,
                                  "source": gfuncs.get(n) if n else None}
    if fresh:
        outdir = outdir or B._scratch(lane, os.path.basename(module_path).lower())
        # Only functions with a known address are built. The rest are library routines Ghidra
        # recognised by signature (__allshl, _memcpy): they belong to the C runtime, which is linked,
        # and defining them again collides with it.
        placed = set(by_va.values())
        dll, built, dropped = build(module_path, preamble, gfuncs, [n for n in order if n in placed],
                                    used, outdir, cflags=cflags)
        ex, base = {}, 0
        if dll:
            pb = open_pe(dll)
            ex, base = pb.export_addrs(), pb.image_base
        vecs, more = V._vectors(4, "pe"), V._confirm_vectors(4, "pe")
        # functions that reach a dropped one (now a stand-in), directly or through others
        tainted = set()
        if dropped:
            pat = re.compile(r"\b(%s)\s*\(" % "|".join(re.escape(d) for d in dropped))
            calls = {n: set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", gfuncs[n])) & set(built) for n in built}
            tainted = {n for n in built if pat.search(gfuncs[n].split("{", 1)[-1])}
            grew = True
            while grew:
                grew = False
                for n in built:
                    if n not in tainted and calls[n] & tainted:
                        tainted.add(n)
                        grew = True
        for va in fresh:
            k, n = L.key(va), by_va.get(va)
            if not n:
                results[k] = {"verdict": "no-synth", "fname": "", "reason": "ghidra found no function here"}
            elif n in dropped or n not in ex:
                results[k] = {"verdict": "no-compile", "fname": n, "reason": dropped.get(n, ""),
                              "source": gfuncs[n]}
            else:
                info = {}
                verdict, reason, limits = V.compare(
                    lambda v, a=va: V._fp_pe(module_path, a, v),
                    lambda v, a=base + ex[n]: V._fp_pe(dll, a, v, rebuilt=module_path), vecs, more,
                    insns=V.insn_count(module_path, va),
                    directed=lambda cov, rch, a=va: V.directed_vectors(module_path, a, cov, reached=rch),
                    info=info)
                if n in tainted and verdict in pipeline.GOOD:
                    limits = sorted(set(limits) | {"stub-callee"})
                results[k] = {"verdict": verdict, "fname": n, "reason": reason, "limits": ",".join(limits),
                              "detail": info.get("detail", ""), "source": gfuncs[n]}
        if store and mod_sha:
            module = os.path.basename(module_path).lower()
            store.put_many((lane, module, mod_sha, k, r.get("fname", ""), sig, r["verdict"],
                            r.get("reason", ""), engine, r.get("limits", ""), digest, r.get("detail", ""))
                           for k, r in results.items() if not r.get("cached"))
    for r in results.values():
        r.setdefault("cached", False)
    return results, preamble
