"""whole_module.py -- compile a module's reconstructed functions as one unit and link a DLL.

A decompiler that emits a whole module gives hundreds of functions in one file, and a few of
them are never valid C. `build` compiles the file, drops the functions the compiler rejects
(each replaced by a stand-in with the same signature, so its callers still link), and repeats
until what is left builds. -> (dll, [functions built], {function dropped: why}).
"""
import hashlib
import os
import re
import subprocess
from codebreaker.base import corpus


def build(module_path, preamble, funcs, order, used, outdir, rounds=30, cflags=None):
    """Compile the whole module, dropping the functions the compiler rejects.
    -> (dll | None, [names built], {dropped name: compiler message})."""
    from codebreaker.build import watcom as B
    ow = B._ow()
    env = B._ow_env(ow)
    imports = B.import_map(module_path)
    os.makedirs(outdir, exist_ok=True)
    names, dropped = list(order), {}
    nostub = set()
    # A caller that passes a callee a different number of arguments than it is defined with is
    # rejected by the compiler -- but only the first such call per callee is reported each run, so
    # a big module took more rounds than there are. Found here they are all known at once.
    dropped.update(B.arity_mismatches([(n, funcs[n]) for n in names]))
    names = [n for n in names if n not in dropped]

    def stand_in(n):
        """A dropped function keeps its place with the same signature and an empty body, so its
        callers still link and their stack stays balanced (they are then marked stub-callee)."""
        head = " ".join(funcs[n].split("{", 1)[0].split())
        return "%s { return 0; }" % head if head.endswith(")") else ""
    for rnd in range(rounds):
        src = (preamble + "\n\n".join("/*@@ %s */\n%s" % (n, funcs[n]) for n in names) + "\n"
               + "/*@@ (stand-ins) */\n" + "\n".join(stand_in(n) for n in order if n in dropped and n not in nostub) + "\n")
        base = "g%d" % rnd
        with open(os.path.join(outdir, base + ".c"), "w", newline="\n", encoding="utf-8") as f:
            f.write(src)
        r = subprocess.run([ow["wcc"]] + (cflags or B._CFLAGS) + ["-e=100000", base + ".c"], cwd=outdir, env=env,
                           capture_output=True, text=True, timeout=1200)
        if r.returncode == 0:
            obj = open(os.path.join(outdir, base + ".obj"), "rb").read()
            with open(os.path.join(outdir, base + ".lnk"), "w", newline="\n") as f:
                # undefsok: a call to a dropped function is left unresolved (it then fails the test)
                f.write(("system nt_dll\nname %s.dll\noption quiet\noption undefsok\n" + B._BASE + "file %s.obj\n") % (base, base)
                        + B._RTL + B._import_lines(obj, set(used), imports))
                # The public symbol of each function as the compiler decorated it: _name (cdecl),
                # _name@N (stdcall), @name@N (fastcall). A function with no such symbol is not exported.
                from codebreaker.targets.win95 import flat_thunk_build as A
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
        progressed = False
        for m in re.finditer(r"\((\d+)\): Error! (E\d+: [^\r\n]*)", errs):
            ln, owner = int(m.group(1)), None
            for st, fn in starts:
                if st <= ln:
                    owner = fn
                else:
                    break
            if owner and owner in funcs:
                bad.setdefault(owner, m.group(2)[:100])
            elif owner == "(stand-ins)":                # a stand-in the compiler will not take
                q = re.search(r"'([A-Za-z_]\w*)'", m.group(2))
                text_lines = src.split("\n")
                line = text_lines[ln - 1] if ln - 1 < len(text_lines) else ""
                hit = (q.group(1) if q and q.group(1) in funcs else
                       next((n for n in dropped if re.search(r"\b%s\s*\(" % re.escape(n), line)), None))
                if hit and hit not in nostub:
                    nostub.add(hit)
                    progressed = True
        if not bad and not progressed:
            break                                       # the preamble itself does not compile
        dropped.update(bad)
        names = [n for n in names if n not in bad]
    return None, [], dropped
