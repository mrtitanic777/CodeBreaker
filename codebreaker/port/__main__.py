"""python -m codebreaker.port SOURCE_DIR OUT_DIR [--compiler gcc|clang|msvc] [--runtime NAME] [--cc PATH]

SOURCE_DIR is read, never written: its C (*.c, *.h, in it or its src/) and its assembler modules (*.asm, in it or
its asm/). OUT_DIR gets the port: the modern sources, the platform layer, a build script, the program built, and
PORT.md - every change made and why, what the compiler still says, what could not be ported.
"""
import collections
import glob
import json
import os
import re
import shutil
import subprocess
import sys

from codebreaker.port import early_c

HERE = os.path.dirname(os.path.abspath(__file__))


def load_compilers():
    c = json.load(open(os.path.join(HERE, "compilers.json")))
    try:                                                    # (the user's own, from settings.json)
        s = json.load(open(os.path.join(HERE, "..", "..", "settings.json")))
        for k, v in s.get("port", {}).get("compilers", {}).items():
            c.setdefault(k, {}).update(v)
    except (OSError, ValueError):
        pass
    return c


def find(names, given=None):
    for c in ([given] if given else []) + names:
        p = shutil.which(c) or next(iter(sorted(glob.glob(c), reverse=True)), None)
        if p:
            return p
    return None


def env_for(cc):
    e = dict(os.environ)                                    # (a compiler's own DLLs beside it, on the path)
    e["PATH"] = os.path.dirname(cc) + os.pathsep + e.get("PATH", "")
    return e


def run(spec, argv, cwd, cc):
    """a compiler's command, after its setup script (MSVC's vcvars) in the same shell"""
    setup = find(spec.get("setup", [])) if spec.get("setup") else None
    if setup:
        argv = ["cmd", "/d", "/c", "call", setup, ">nul", "&&"] + argv
    return subprocess.run(argv, capture_output=True, text=True, env=env_for(cc), cwd=cwd, errors="replace")


def args(tpl, **kw):
    out = []
    for a in tpl:
        if a == "{srcs}":
            out += kw["srcs"]
        else:
            out.append(a.format(**{k: v for k, v in kw.items() if k != "srcs"}))
    return out


def diagnostics(spec, text, folder):
    out = []
    for l in text.splitlines():
        n = re.match(r"^(?P<file>[^:\n]+?(?::[\\/][^:\n]+)?):(?P<line>\d+):(?P<col>\d+): note: (?P<text>.*)$", l.strip())
        if n and out:                                       # (a note: what the complaint before it expected)
            out[-1].setdefault("notes", []).append(n.group("text"))
            continue
        m = re.match(spec["diag"], l.strip())
        if m:
            f = os.path.basename(m.group("file"))
            out.append({"file": f, "line": int(m.group("line")), "col": int(m.group("col") or 0),
                        "kind": m.group("kind"), "text": m.group("text")})
    return out


def read_tree(src):
    files, asm, paths = {}, {}, {}
    for d in (src, os.path.join(src, "src")):
        for p in sorted(glob.glob(os.path.join(d, "*.[ch]"))):
            files[os.path.basename(p)] = open(p, encoding="latin1").read()
    for d in (src, os.path.join(src, "asm"), os.path.join(src, "src")):
        for p in sorted(glob.glob(os.path.join(d, "*.asm"))):
            asm[os.path.basename(p)] = open(p, encoding="latin1").read()
            paths[os.path.basename(p)] = p
    return files, asm, paths


def asm_role(text, profile):
    """what an assembler module of the program is: the old runtime's (it makes public what the runtime defines,
    or every name it makes public is the C library's or the platform layer's), data alone (no instruction in a
    code segment), or code of the program - to be ported as assembly language"""
    from codebreaker.port import asm16
    pubs = set()
    for l in text.splitlines():
        m = re.match(r"\s*PUBLIC\s+(.+)", strip_asm_comment(l), re.I)
        if m:
            pubs |= {x.strip() for x in m.group(1).split(",") if x.strip()}
    known = set(profile["libc"]) | set(profile["platform"]["calls"]) | set(profile["runtime_defines"])
    names = {p[1:] if p.startswith("_") else p for p in pubs}
    if names & set(profile["runtime_defines"]) - {"fopen", "fprintf", "malloc"} or "_main" in pubs:
        return "runtime", sorted(pubs)
    lines, seg, code = asm16.parse(text), None, False
    for l in lines:
        d = l.directive
        if d and d[0] == "SEGMENT":
            seg = d[1] if ("CODE" in d[2].upper() or d[1].upper().endswith("TEXT")) else None
        elif d and d[0] == "ENDS":
            seg = None
        elif seg and l.op:
            code = True
    if not code:
        return "data", sorted(pubs)
    own = [p for p in pubs if re.match(r"_[A-Za-z]", p) and p[1:] not in known or
           re.match(r"[A-Z][A-Z0-9_]*$", p) and p.lower() not in known]   # (C names: _x is C's x, CAPS a pascal one;
    return ("code" if own else "runtime"), sorted(pubs)                  #  __x and cret the runtime's)


def strip_asm_comment(l):
    return l.split(";", 1)[0]


def asm_modules(port, out, res, asm, paths, roles, profile, opt, compilers):
    """the program's own assembly language, ported as assembly language (port/asm16) and assembled; its data
    alone as C definitions. -> ([objects to link], {name: its C declaration}, [notes])"""
    from codebreaker.port import asm16
    from codebreaker.base import settings
    texts = [t for n, t in res.items() if n.endswith((".c", ".h"))]
    origs = list(port.originals.values())
    ct = asm16.CTypes(texts, origs)
    calls = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", "\n".join(early_c.blank(t) for t in texts)))
    far = set(re.findall(r"\bfar\b[^;(){}]*?\b(\w+)\s*\(", "\n".join(early_c.blank(t) for t in origs)))
    old_asm = opt("--asm16") or (settings.get("port") or {}).get("asm16") if hasattr(settings, "get") else None
    ml = find(compilers.get("ml64", {}).get("find", []))
    objs, protos, notes = [], {}, []
    for n, t in sorted(asm.items()):
        role = roles[n][0]
        if role == "data":
            for d in asm_data_defs(t, ct):
                port.asm_data.append((n, d))
            for al, where in asm_data_defs.aliases.items():   # (names of one place: one variable)
                owner = re.search(r"[A-Za-z_]\w*", where).group(0)
                od = next((d for m_, d in port.asm_data if re.search(r"\b%s\s*\[" % owner, d)), None)
                port.aliases[al] = (where, od)
            notes.append("%s: data alone - its variables defined in data.c with the C program's types" % n)
            continue
        if role != "code":
            continue
        m = asm16.Module(t, ct, cnames=calls)
        m.far = {p for p, c in m.cname.items() if c in far}
        lib = set(profile["libc"]) | {"strcmp", "strlen", "strcpy", "strcat", "memcpy", "memset", "memcmp"}
        for p, c in list(m.cname.items()):
            if c in lib:                                    # (the program's own routine of a C library name: its
                m.cname[p] = c + "_"                        #  own kept, under a name of its own, in the C too)
                port.renames[c] = c + "_"
                notes.append("%s: its own %s kept (not the C library's), as %s_" % (n, c, c))
        if old_asm and os.path.isfile(old_asm):
            m.offsets = asm16.listing_offsets(paths[n], old_asm)
        try:
            m.run()
            text = m.emit()
        except asm16.Asm16Error as e:
            notes.append("**%s: not ported - %s**" % (n, e))
            continue
        base = os.path.splitext(n)[0] + "64"
        open(os.path.join(out, base + ".asm"), "w", newline="\n").write(text)
        for p, c in m.cname.items():
            ex = m.exit_of(p)
            k = ex.r.get("ax") if ex else None
            ret = "char *" if k is not None and k.wide() and k.kind != "frame" else "int16_t"
            protos[c] = "%s%s%s();" % (ret, "" if ret.endswith("*") else " ", c)
        if not ml:
            notes.append("**%s: ported to %s.asm, but no ml64 to assemble it**" % (n, base))
            continue
        p = subprocess.run([ml, "/nologo", "/c", "/Cp", "/Fo" + base + ".obj", base + ".asm"], capture_output=True,
                           text=True, cwd=out)
        if p.returncode:
            notes.append("**%s: %s.asm does not assemble: %s**" % (n, base, (p.stdout + p.stderr)[-400:]))
            continue
        objs.append(base + ".obj")
        notes.append("%s: ported as assembly language to %s.asm (%d routines the C program calls)" % (
            n, base, len(m.cname)))
    return objs, protos, notes


def asm_data_defs(text, ct):
    """the variables of an assembler module of data alone (placed by the old linker): C definitions, with the
    C program's types where it declares them"""
    from codebreaker.port import asm16
    lines = asm16.parse(text)
    pubs = set()
    for l in lines:
        d = l.directive
        if d and d[0] == "PUBLIC":
            pubs |= {x.strip() for x in d[2].split(",") if x.strip()}
    places, off, seg = [], 0, None                          # ([C name, old offset, bytes of its own])
    for l in lines:
        d = l.directive
        if d and d[0] == "SEGMENT":
            seg, off = d[1], 0
            continue
        if d and d[0] == "LABEL" and d[1] in pubs:
            places.append([d[1][1:] if d[1].startswith("_") else d[1], (seg, off), 0])
        elif d and d[0] in ("DB", "DW", "DD"):
            m = re.match(r"\s*(\d+)\s+DUP", d[2], re.I)
            n = (int(m.group(1)) if m else len(asm16.split_ops(d[2]))) * {"DB": 1, "DW": 2, "DD": 4}[d[0]]
            if places and places[-1][1][0] == seg:
                places[-1][2] += n
            off += n
    old_size = lambda base, dcl, n: early_c.size_of(base, dcl, n, 2, {}) or 2
    out, aliases, done = [], {}, set()
    for i, (c, at, own) in enumerate(places):
        if c in done:
            continue
        v = ct.var(c)
        nv = ct.var(places[i + 1][0]) if i + 1 < len(places) else None
        same_next = bool(v and nv and re.sub(r"\s", "", nv[0]) == re.sub(r"\s", "", v[0]) and
                         ("*" in nv[1]) == ("*" in re.sub(r"\[\s*\]", "", v[1])))
        if v and re.search(r"\[\s*\]", v[1]) and (own == 0 or same_next):   # (an array that is the names after
            # it: the old linker gave them one place)
            base, dcl = v                                   #  linker gave them one place)
            el = re.sub(r"\[\s*\]", "", dcl).strip()
            es = old_size(base, el, c)
            elt = re.sub(r"\s", "", base + el.replace(c, ""))
            nxt, end = [], None
            for c2, at2, own2 in places[i + 1:]:
                v2 = ct.var(c2)
                if at2[0] != at[0]:
                    break
                d2 = re.sub(r"\s", "", v2[0] + re.sub(r"\[\s*\]", "", v2[1]).replace(c2, "")) if v2 else None
                rel = at2[1] - at[1]
                if d2 == elt and v2 and re.search(r"\[\s*\]", v2[1]) and rel == 0:
                    nxt.append((c2, None))                  # (the same array, by another name)
                elif d2 == elt and v2 and rel % es == 0 and re.search(r"\[\s*\]", v2[1]):
                    nxt.append((c2, ("&", rel // es)))      # (an array that is the rest of it, from there)
                elif d2 == elt and v2 and rel % es == 0:
                    nxt.append((c2, rel // es))             # (one of its elements)
                else:
                    end = at2[1]
                    break
            extent = (end - at[1]) if end is not None else max(es, sum(p[2] for p in places[i:i + len(nxt) + 1]))
            ks = [k if isinstance(k, int) else k[1] for c2, k in nxt if k is not None]
            count = max(1, extent // es, max(ks + [0]) + 1)
            out.append("%s %s;" % (base, dcl.replace("[]", "[%d]" % count).strip()))
            for c2, k2 in nxt:
                aliases[c2] = c if k2 is None else ("(&%s[%d])" % (c, k2[1]) if isinstance(k2, tuple) else
                                                    "%s[%d]" % (c, k2))
                done.add(c2)
            continue
        if v:
            base, dcl = v
            unit = old_size(base, re.sub(r"\[\s*\]", "", dcl), c)
            dcl = re.sub(r"\[\s*\]", "[%d]" % max(1, own // max(1, unit)), dcl)
            out.append("%s %s;" % (base, dcl.strip()))
        else:
            out.append("char %s[%d];\t\t/* (no C declaration says its type) */" % (c, max(own, 1)))
    asm_data_defs.aliases = aliases
    return out


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 2
    src, out = argv[0], argv[1]
    from codebreaker.base import parallel
    parallel.throttle(below_normal=True)                    # (the machine first; the compilers inherit it)
    opt = lambda k, d=None: argv[argv.index(k) + 1] if k in argv else d
    comp = opt("--compiler", "gcc")
    profile = json.load(open(os.path.join(HERE, "runtimes", opt("--runtime", "dos_c_early") + ".json")))
    compilers = load_compilers()
    spec = compilers[comp]
    cc = find(spec["find"], opt("--cc"))
    if os.path.abspath(out).startswith(os.path.abspath(src) + os.sep) or os.path.abspath(out) == os.path.abspath(src):
        print("the port is written beside the source, never into it: give another OUT_DIR")
        return 2
    files, asm, paths = read_tree(src)
    port = early_c.Port(files, profile)
    roles = {n: asm_role(t, profile) for n, t in asm.items()}
    os.makedirs(out, exist_ok=True)
    for n in ("dosrt.h", "dosrt.c"):
        shutil.copy(os.path.join(HERE, "platform", n), os.path.join(out, n))
    with open(os.path.join(out, "dosrt_config.h"), "w", newline="\n") as f:   # (the old runtime's ways)
        f.write("/* dosrt_config.h -- the old runtime's ways (made by CodeBreaker's port from the profile %s) */\n"
                "#define DOSRT_TEXT_MODE %d\n" % (profile["name"], 1 if profile.get("text_mode") else 0))

    def write():
        res = port.modernize()
        for n, t in port.headers().items():
            if t is not None:
                res[n] = t
        for n, t in res.items():
            open(os.path.join(out, n), "w", encoding="latin1", newline="\n").write(t)
        return res

    objs, asm_notes = [], []
    if any(r[0] in ("code", "data") for r in roles.values()):
        res = write()                                       # (the C as far as it goes: the types the assembly needs)
        objs, protos, asm_notes = asm_modules(port, out, res, asm, paths, roles, profile, opt, compilers)
        port.asm_protos = protos
    rounds, diags = 0, []
    while True:
        res = write()
        diags = []
        if cc is None:
            break
        for n in sorted(res):
            if n.endswith(".c"):
                p = run(spec, args(spec["check"], cc=cc, inc=".", src=n), out, cc)
                diags += diagnostics(spec, p.stdout + p.stderr, out)
        rounds += 1
        if rounds > 8 or not early_c.mend(port, res, diags):
            break
    if port.int16:                                          # (the 8086's arithmetic, where the C of today's differs)
        arith = arith16_pass(port, out, res, compilers)
        if arith:
            res = write()
    srcs = sorted(n for n in res if n.endswith(".c")) + ["dosrt.c"] + objs
    exe = os.path.join(out, "program.exe" if os.name == "nt" else "program")
    cases = opt("--cases")
    sanitized = []
    if cases:                                               # (what only a run shows: the sanitizer, the cases)
        sanitized = sanitize_loop(port, out, write, srcs, compilers, comp, cases)
        res = write()
    built, blog = False, ""
    if cc:
        p = run(spec, args(spec["build"], cc=cc, inc=".", out=exe, srcs=srcs), out, cc)
        built, blog = p.returncode == 0, p.stdout + p.stderr
    with open(os.path.join(out, "build.sh"), "w", newline="\n") as f:
        f.write("#!/bin/sh\n# the port built by %s (made by CodeBreaker's port)\ncd \"$(dirname \"$0\")\"\n%s\n" %
                (comp, " ".join(args(spec["build"], cc=os.path.basename(cc or comp), inc=".", out=os.path.basename(exe),
                                     srcs=srcs))))
    report(out, src, comp, cc, port, roles, diags, rounds, built, blog, profile, sanitized, asm_notes)
    errs = sum(1 for d in diags if d["kind"] == "error")
    print("%s: %d files ported, %d left out (the old runtime), %d rounds; %d errors, %d warnings; %s" % (
        out, len([n for n in res if n.endswith(".c")]), len(port.left_out), rounds, errs,
        sum(1 for d in diags if d["kind"] == "warning"), "BUILT " + exe if built else "not built"))
    return 0 if built else 1


def arith16_pass(port, out, res, compilers):
    """arith16 over the port's C (clang reads it): each line it changed kept as a rule; -> how many casts"""
    from codebreaker.port import arith16
    cc = find(compilers["clang"]["find"])
    if not cc:
        port.mended["the 16-bit arithmetic NOT checked (no clang to read the port): comparisons of unsigned and "
                    "signed 16-bit values may differ"].append("-")
        return 0
    files = sorted(n for n in res if n.endswith(".c"))
    new = arith16.run(cc, out, files)
    n = 0
    for f, t in new.items():
        for a, b in zip(res[f].split("\n"), t.split("\n")):
            if a != b:
                port.line_fixes[(f, a)] = b
                n += b.count("_t)") - a.count("_t)")
                port.mended["the 8086's 16-bit arithmetic kept (an unsigned met by an int was unsigned; a sum was "
                            "cut to 16 bits before it was compared or made a long): a cast"].append(f)
    return n


def sanitize_loop(port, out, write, srcs, compilers, comp, cases, rounds=6):
    """the port built with AddressSanitizer and run on the cases until it reports nothing; -> the rounds' notes"""
    from codebreaker.port import sanitize, verify
    from codebreaker.base import parallel
    for name in ([comp] if comp != "msvc" else []) + ["clang", "gcc"]:
        spec = compilers.get(name)
        cc = find(spec["find"]) if spec else None
        if cc and (name != "gcc" or os.name != "nt"):       # (MinGW's gcc has no sanitizer runtime)
            break
    else:
        return ["no compiler with AddressSanitizer found: the cases were not run sanitized"]
    rt = sanitize.runtime_dir(cc)
    d = os.path.join(out, ".sanitize")
    os.makedirs(d, exist_ok=True)
    exe = os.path.join(d, "program.exe" if os.name == "nt" else "program")
    notes = []
    for r in range(rounds):
        parallel.wait_for_memory("sanitized build")         # (the sanitizer's builds and runs want memory)
        write()
        tpl = [a for a in spec["build"] if a not in ("-O1", "/O1")]
        tpl = tpl[:1] + ["-g", "-fsanitize=address"] + tpl[1:]
        p = run(spec, args(tpl, cc=cc, inc=".", out=exe, srcs=srcs), out, cc)
        if p.returncode:
            notes.append("the sanitized build failed: %s" % (p.stdout + p.stderr)[-300:])
            break
        env = {"PATH": (rt + os.pathsep if rt else "") + os.environ.get("PATH", ""), "ASAN_OPTIONS": "detect_leaks=0"}
        reports = verify.run_cases(cases, exe, env)
        done = sanitize.mend(port, port.out, reports)
        bad = [c for c, t in reports if "AddressSanitizer" in t]
        if not done:
            notes.append("round %d: %d cases run sanitized, %s" % (
                r + 1, len(reports), "no fault" if not bad else "faults not mended: " + ", ".join(bad)))
            break
        notes += ["round %d: %s" % (r + 1, why) for v, why in done]
    import shutil as _sh
    _sh.rmtree(d, ignore_errors=True)
    return notes


def report(out, src, comp, cc, port, roles, diags, rounds, built, blog, profile, sanitized=(), asm_notes=()):
    L = ["# Port of `%s`" % os.path.basename(os.path.normpath(src)), "",
         "Made by CodeBreaker's port from `%s` (read only), for **%s** (`%s`)." % (src, comp, cc or "not found"), "",
         "**%s.** %d rounds of the compiler's complaints mended." % ("Built" if built else "Not built", rounds), ""]
    L += ["## Left out: the old runtime", "", "| file | why |", "|---|---|"]
    L += ["| %s | %s |" % (n, w) for n, w in sorted(port.left_out.items())]
    for n, (role, pubs) in sorted(roles.items()):
        why = {"runtime": "assembler routines of the old runtime (%s): the C library and the platform layer stand in",
               "data": "data alone (%s)",
               "code": "the program's own assembly language (%s): ported as assembly language (below)"}[role]
        L.append("| %s | %s |" % (n, why % (", ".join(pubs) or "nothing public")))
    L += ["", "## Changes", "", "| change | how many | where (first) |", "|---|---|---|"]
    for k, v in sorted(port.notes.items(), key=lambda kv: -len(kv[1])):
        L.append("| %s | %d | %s |" % (k, len(v), ", ".join(v[:4]) + (" ..." if len(v) > 4 else "")))
    if asm_notes:
        L += ["", "## The program's assembly language", ""] + ["- %s" % x for x in asm_notes]
    if sanitized:
        L += ["", "## Run under AddressSanitizer (the verify cases)", ""] + ["- %s" % s for s in sanitized]
    L += ["", "## Behaviour that may differ", ""]
    L += ["- **%s**: %s" % (k, v) for k, v in profile["notes"].items()]
    L += ["- **int** is %d bits in the original and 32 here: arithmetic that relied on 16-bit wrap-around differs "
          "(structure members keep 16 bits); the verifier (verify.py) is what shows it." % profile["widths"]["int"]]
    if profile.get("char_signed"):
        L.append("- **char** was signed: built with -fsigned-char where the compiler has it.")
    left = [d for d in diags if d["kind"] in ("error", "warning")]
    L += ["", "## What the compiler still says (%d)" % len(left), ""]
    cnt = collections.Counter(re.sub(r"'[^']*'", "'..'", d["text"]) for d in left)
    L += ["- %d x %s" % (n, t) for t, n in cnt.most_common(30)]
    if not built and blog:
        L += ["", "## Build", "", "```", blog[-3000:], "```"]
    open(os.path.join(out, "PORT.md"), "w", encoding="utf-8").write("\n".join(L) + "\n")


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
