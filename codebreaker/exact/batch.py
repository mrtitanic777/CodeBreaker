"""batch.py -- many candidates of a function compiled in one run of the compiler.

A run's cost is mostly the compiler starting its passes, not the function: N candidates in one source cost little
more than one. Each candidate's own names - the function, what it declares extern, the structs it defines - are
given a suffix of its own (__Q<k>) so that they do not meet; the object's names are given back after; each
function of the object is held to the program's code as a lone compile's would be.

A candidate the compiler refuses (the error's line says whose) is left out and the rest compiled again; the ones
left out the caller proves alone (draft/prove.py's _prove mends what it can). Candidates with preprocessor lines other than
#include, or a different file comment (MFLAGS) from the rest, are not batched.
"""
import re

from codebreaker.base import ctext

KEYWORDS = {"extern", "int", "char", "long", "short", "unsigned", "signed", "struct", "union", "double", "float",
            "register", "static", "void", "auto", "typedef", "enum", "sizeof", "return", "if", "else", "while", "for",
            "do", "switch", "case", "default", "break", "continue", "goto"}
SUFFIX = re.compile(r"__Q\d+$")


def split(src):
    """(file comment and #include lines, the rest) - or None when the candidate is not one to batch"""
    pre, body = [], []
    for l in src.split("\n"):
        s = l.strip()
        if not body and (s.startswith("#include") or re.fullmatch(r"/\* \w+: [^*]*\*/", s) or not s):
            pre.append(l)
            continue
        if s.startswith("#"):
            return None
        body.append(l)
    b = ctext.blank("\n".join(body), whole=True, across_lines=True)
    if b.count("{") != b.count("}") or b.count("(") != b.count(")"):
        return None                                         # (a text that does not hold together: its errors would
    return "\n".join(x for x in pre if x.strip()), "\n".join(body)   #  run on into the next one's)


def own_names(body):
    """the names a candidate gives the file: its function, what it declares extern, the structs it defines"""
    names = set()
    m = re.search(r"(?m)^(?!extern\b|struct\b|static\b)(?:[\w *]+\n)?([A-Za-z_]\w*)\(", body)
    if m:
        names.add(m.group(1))
    for l in re.findall(r"(?m)^extern\b([^;]*);", body):
        l = re.sub(r"\bstruct\s+\w+", " ", l)
        for n in re.findall(r"([A-Za-z_]\w*)\s*(?=[(\[,);]|$)", l):
            if n not in KEYWORDS:
                names.add(n)
    names |= set(re.findall(r"\bstruct\s+(\w+)\s*\{", body))
    return names


def renamed(body, k):
    names = own_names(body)
    if not names:
        return body
    rx = re.compile(r"\b(%s)\b" % "|".join(sorted(map(re.escape, names), key=len, reverse=True)))
    out = []
    for piece in re.split(r'("(?:[^"\\]|\\.)*")', body):   # (not inside a string)
        out.append(piece if piece.startswith('"') else rx.sub(lambda m: "%s__Q%d" % (m.group(1), k), piece))
    return "".join(out)


def build(srcs):
    """-> (text, [(first line, last line) of each candidate], [index of each candidate in srcs]) or None"""
    parts = [split(s) for s in srcs]
    ok = [i for i, p in enumerate(parts) if p is not None]
    if len(ok) < 2:
        return None
    pre = parts[ok[0]][0]
    ok = [i for i in ok if parts[i][0] == pre]
    if len(ok) < 2:
        return None
    lines = pre.split("\n") if pre else []
    spans = []
    for k, i in enumerate(ok):
        b = renamed(parts[i][1], k).split("\n")
        first = len(lines) + 1
        lines += b + [""]
        spans.append((first, len(lines)))
    return "\n".join(lines) + "\n", spans, ok


def error_lines(con):
    return {int(m.group(1)) for m in re.finditer(r"\.C\((\d+)\)\s*:(?!\s*warning)", con, re.I)}   # (3.00 names
                                                                                                    #  only warnings)


def compile_many(prj, tc, srcs, rounds=3):
    """-> {index in srcs: (object, the function's index among the object's)} for the candidates compiled together;
    those missing are the caller's to prove alone"""
    from codebreaker.formats import omf
    want = list(range(len(srcs)))
    for _ in range(rounds):
        b = build([srcs[i] for i in want])
        if b is None:
            return {}
        text, spans, ok = b
        idx = [want[i] for i in ok]
        obj, con = tc.compile(text.encode("latin1", errors="replace"), headers=prj.headers(), include=prj.include,
                              switches=prj.switches)
        if obj is not None:
            obj = omf.rename_symbols(obj, lambda n: SUFFIX.sub("", n))
            return {i: (obj, k) for k, i in enumerate(idx)}
        bad = error_lines(con)
        if not bad:
            return {}
        drop = {k for k, (lo, hi) in enumerate(spans) if any(lo <= l <= hi for l in bad)}
        if not drop:
            return {}
        want = [i for k, i in enumerate(idx) if k not in drop]
        if len(want) < 2:
            return {}
    return {}
