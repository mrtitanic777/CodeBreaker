"""seed.py -- a program's functions given the sources of the same functions in another version of it.

    python -m codebreaker exact PROJECT seed --from OTHER [--write] [-v]

The functions the names stage pairs with OTHER's (the same code but for its places, or found by its calls) are
mostly the same source. For each pair whose function OTHER has in a source file, this program gets a file of
its own, src/fADDR.c (ADDR this program's place): every top-level declaration of OTHER's file (its includes,
macros, types, variables - a function of the file that returns other than an int declared), then the function's
text. OTHER's address-names in it (fXXXXX, g_XXXX) are made this program's where the pairs say where those are.
The ledger then says which are this program's bytes; a file that is not is a draft to start from.

A file this project has already is kept. Without --write the files are only counted.
"""
import os
import re

from codebreaker.exact import names as namesmod, project as projmod

FUNC = re.compile(r"(?m)^((?:static\s+)?(?:(?:unsigned|struct\s+\w+|union\s+\w+|[A-Za-z_]\w*)\s+\**\s*)?\**\s*([A-Za-z_]\w*)\s*\([^;{]*\)\s*)$")


def split(text):
    """(top-level declarations [text], {function name: its text}, {function name: the type it returns})"""
    lines = text.split("\n")
    decls, funcs, types = [], {}, {}
    k = 0
    pending = []                                            # (a remark just before a function: its own)
    while k < len(lines):
        l = lines[k]
        m = FUNC.match(l) if not l.startswith((" ", "\t", "#", "}")) else None
        if m and m.group(2) not in ("if", "while", "for", "switch", "return", "sizeof"):
            j = k
            while j < len(lines) and not lines[j].startswith("{"):      # (K&R: the parameters' declarations)
                if lines[j].rstrip().endswith(";") and j > k and lines[j].startswith(("extern", "static")):
                    break
                j += 1
            if j < len(lines) and lines[j].startswith("{"):
                e = j
                depth = 0
                while e < len(lines):
                    depth += lines[e].count("{") - lines[e].count("}")
                    if depth <= 0 and e > j or depth <= 0 and lines[e].strip() == "}":
                        break
                    e += 1
                name = m.group(2)
                funcs[name] = "\n".join(pending + lines[k:e + 1]) + "\n"
                head = l[:l.index(name)].replace("static", "").strip()
                if head and head != "int":
                    types[name] = head
                pending = []
                k = e + 1
                continue
        if l.startswith("/*") and not l.rstrip().endswith(";"):
            r = [l]
            while "*/" not in lines[k] and k + 1 < len(lines):
                k += 1
                r.append(lines[k])
            pending = r
            k += 1
            continue
        if l.strip():
            decls += pending + [l]
        pending = []
        k += 1
    return decls, funcs, types


def seeds(prj, other):
    nm, dn = namesmod.carry(prj, other)
    pairs, dpairs = namesmod.carry.pairs, namesmod.carry.dpairs
    ocode, odata = namesmod.known(other)
    files = {}
    for part in other.sets():
        for at, path in part.sources():
            if path.lower().endswith(".c"):
                files[path] = split(open(path, encoding="latin1").read())
    where = {}
    for path, (decls, funcs, types) in files.items():
        for n in funcs:
            where.setdefault(n, path)
    inv = {b: a for a, b in pairs.items()}
    cname = lambda n: n[1:] if n.startswith("_") else n
    rename = {}                                             # (OTHER's address-names - for wherever OTHER's names
    for b, a in inv.items():                                #  were taken from - as this program's places)
        n = cname(ocode.get(b, ""))
        if re.fullmatch(r"f[0-9a-f]{5}", n):
            rename[n] = "f%05x" % a
    for a, b in dpairs.items():
        n = cname(odata.get(b, ""))
        if re.fullmatch(r"g_[0-9a-f]{1,4}", n):
            rename[n] = "g_%04x" % a
    rx = re.compile(r"\b(f[0-9a-f]{5}|g_[0-9a-f]{1,4})\b")
    out = {}
    for a, b in sorted(pairs.items()):
        n = cname(ocode.get(b, "f%05x" % b))
        path = where.get(n) or where.get("f%05x" % b)
        if not path:
            continue
        decls, funcs, types = files[path]
        fn = funcs.get(n) or funcs.get("f%05x" % b)
        others = ["extern %s %s();" % (t, f) for f, t in types.items() if f != n and re.search(r"\b%s\s*\(" % f, fn)]
        text = "/* seeded from %s %s */\n" % (os.path.basename(other.exe), os.path.relpath(path, other.dir)) + \
            "\n".join(decls + others) + "\n\n" + fn
        out[a] = rx.sub(lambda m: rename.get(m.group(1), m.group(1)), text)
    return out


def main(prj, argv):
    if "--from" not in argv:
        print("seed: --from OTHER (a project of another version of the program, with sources)")
        return 1
    f = argv[argv.index("--from") + 1]
    other = projmod.load(f if os.path.isabs(f) else os.path.join(os.getcwd(), f))
    out = seeds(prj, other)
    have = {a for a, p in prj.sources()}
    new = {a: t for a, t in out.items() if a not in have}
    print("%d functions seeded from %s (%d the project has a file for already)" % (len(out), os.path.basename(other.exe), len(out) - len(new)))
    if "--write" in argv:
        os.makedirs(prj.src, exist_ok=True)
        for part in other.sets():                           # (the headers the sources include, when this has none)
            for h in os.listdir(part.src):
                if h.lower().endswith((".h", ".inc", ".lit")) and not os.path.exists(prj.path(h)):
                    with open(os.path.join(part.src, h), "rb") as s, open(prj.path(h), "wb") as d:
                        d.write(s.read())
        for a, t in new.items():
            with open(prj.path("f%05x.c" % a), "w", newline="\n") as fh:
                fh.write(t)
        print("written: %d files in %s" % (len(new), prj.src))
    return 0
