"""arith16.py -- the arithmetic of a 16-bit int kept, where the C of today would do it otherwise.

On the 8086 an int was 16 bits, so an unsigned (16 bits) met by an int made the int unsigned: `u > -1` was false,
`u - 1` with u 0 was 65535. The port keeps every int 16 bits wide (int16_t, uint16_t), which keeps what is stored -
but the C of today makes both of them a 32-bit int before it compares, divides or widens them: `u > -1` is true,
`(long)(u - 1)` is -1. Wherever that changes the answer this pass says, by a cast, what the 8086 did:

  a comparison, / % or >> whose operands the 8086 made unsigned   each operand that is not so already: (uint16_t)
  ... of 16-bit operands of which one is arithmetic (it may have      that operand: (int16_t)(...) or (uint16_t)(...)
      passed 16 bits)
  arithmetic of 16 bits made 32 (a long assigned, given, cast)    the arithmetic, cut to its 16 bits first

Each expression's type is worked out by the 8086's rules from the tree clang makes of the port (clang -Xclang
-ast-dump=json: every expression with its exact place, and the types as the source names them); the casts go in
at those places, the rest of the text as it was.
"""
import json
import os
import re
import subprocess

from codebreaker.base import cache

W16 = {"int16_t": "i16", "short": "i16", "uint16_t": "u16", "unsigned short": "u16", "char": "i16",
       "signed char": "i16", "unsigned char": "i16", "uint8_t": "i16", "int8_t": "i16", "int32_t": "i32",
       "long": "i32", "uint32_t": "u32", "unsigned long": "u32"}
ARITH = {"+", "-", "*", "<<", "&", "|", "^"}
TESTS = {"<", ">", "<=", ">=", "==", "!=", "/", "%", ">>"}
CAST = {"u16": "uint16_t", "i16": "int16_t"}


def dump(cc, src, folder, extra=()):
    """clang's tree of a file of the port (the JSON of -ast-dump), or None"""
    p = subprocess.run([cc, "-fsyntax-only", "-Xclang", "-ast-dump=json", "-fsigned-char", "-std=c11",
                        "-D_CRT_SECURE_NO_WARNINGS", "-D_CRT_NONSTDC_NO_WARNINGS", "-I."] + list(extra) + [src],
                       capture_output=True, cwd=folder)
    try:
        return json.loads(p.stdout.decode("utf-8", "replace"))
    except ValueError:
        return None


def old_type(n, text):
    """the type the 8086's compiler gave an expression: i16 u16 i32 u32 - or None (a pointer, a double, unknown)"""
    k = n.get("kind")
    inner = n.get("inner") or []
    if k in ("ParenExpr",) or (k == "ImplicitCastExpr" and n.get("castKind") in ("LValueToRValue", "NoOp")):
        return old_type(inner[0], text) if inner else None
    if k == "ImplicitCastExpr" and n.get("castKind") == "IntegralCast":
        t = n["type"].get("qualType")
        if t == "int":                                      # (a promotion: what it was, made an int of 16 bits)
            return old_type(inner[0], text) if inner else None
        return W16.get(t)
    if k == "IntegerLiteral":
        s = spelled(n, text) or ""
        if not re.fullmatch(r"(?:0[xX][0-9a-fA-F]+|\d+)[uUlL]*", s):
            s = ""                                          # (a macro's name, not the number's own spelling)
        v = int(n.get("value", "0"))
        if re.search(r"[lL]$", s) or v > 0xFFFF:
            return "i32"
        if v > 0x7FFF:
            return "u16" if re.match(r"0", s) else "i32"   # (0177777, 0xFFFF unsigned; 65535 a long)
        return "i16"
    if k == "CharacterLiteral":
        return "i16"
    if k == "CStyleCastExpr":
        return W16.get(n["type"].get("qualType"))
    if k == "BinaryOperator":
        op = n.get("opcode")
        if op in ("<", ">", "<=", ">=", "==", "!=", "&&", "||"):
            return "i16"
        if op == ",":
            return old_type(inner[1], text) if len(inner) > 1 else None
        if op == "=":
            return old_type(inner[0], text) if inner else None
        a, b = (old_type(x, text) for x in inner[:2]) if len(inner) >= 2 else (None, None)
        if op in ("<<", ">>"):
            return a
        return common(a, b)
    if k == "CompoundAssignOperator":
        return old_type(inner[0], text) if inner else None
    if k == "UnaryOperator":
        op = n.get("opcode")
        if op == "!":
            return "i16"
        if op in ("-", "~", "+", "++", "--"):
            return old_type(inner[0], text) if inner else None
        if op == "*":
            return W16.get(n["type"].get("qualType"))
        return None
    if k == "ConditionalOperator":
        return common(*(old_type(x, text) for x in inner[1:3])) if len(inner) >= 3 else None
    if k in ("DeclRefExpr", "MemberExpr", "ArraySubscriptExpr", "CallExpr"):
        return W16.get(n["type"].get("qualType"))
    return None


def common(a, b):
    """the 8086's usual arithmetic conversions (int 16 bits, long 32)"""
    if a is None or b is None:
        return None
    if "u32" in (a, b):
        return "u32"
    if "i32" in (a, b):
        return "i32"
    if "u16" in (a, b):
        return "u16"
    return "i16"


def place(n):
    """(start, end) offsets of an expression in the file - None when in a macro or another file"""
    r = n.get("range") or {}
    b, e = r.get("begin", {}), r.get("end", {})
    if "expansionLoc" in b and "expansionLoc" in e:        # (all of it from one use of a macro: that use)
        x, y = b["expansionLoc"], e["expansionLoc"]
        if x.get("offset") is not None and x.get("offset") == y.get("offset"):
            f = x.get("_file") or b.get("_file")
            end = x["offset"] + x.get("tokLen", 0)
            if TEXT[0] is not None and re.match(r"\s*\(", TEXT[0][end:end + 40]):
                return None                                 # (a macro with arguments - assert(...) - not a constant)
            return x["offset"], end, f
        return None
    if "offset" not in b or "offset" not in e or "spellingLoc" in b or "spellingLoc" in e:
        return None
    if b.get("_file") != e.get("_file") or b.get("_file") is None:
        return None
    return b["offset"], e["offset"] + e.get("tokLen", 0), b["_file"]


def spelled(n, text):
    p = place(n)
    return text[p[0]:p[1]] if p and p[2] == MAIN[0] else None


MAIN = [None]
TEXT = [None]                                               # (the text of the file being read)


def mark_files(node, cur):
    """clang names a location's file only where it changes: each location given its own (_file)"""
    if isinstance(node, dict):
        for k, v in node.items():
            if isinstance(v, dict) and ("offset" in v or "file" in v or "spellingLoc" in v):
                if "file" in v and k != "includedFrom":     # (includedFrom names the file that included this one, not
                    cur[0] = v["file"]                      #  the file the location is in: it must not move the cursor -
                v["_file"] = cur[0]                         #  every later location of a header was taken for the includer's)
            mark_files(v, cur)
    elif isinstance(node, list):
        for v in node:
            mark_files(v, cur)


def bare(n):
    while n.get("kind") in ("ParenExpr",) or (n.get("kind") == "ImplicitCastExpr" and
                                                n.get("castKind") in ("LValueToRValue", "NoOp", "IntegralCast")):
        n = (n.get("inner") or [{}])[0]
    return n


def nonneg(n):
    """a constant of no sign: the same number whichever way it is converted"""
    n = bare(n)
    return n.get("kind") in ("IntegerLiteral", "CharacterLiteral") and int(n.get("value", 0)) >= 0


def compound(n, text=None):
    """an operand that is arithmetic of its own that may have gone past 16 bits (or, with & | ^, mixed signs),
    its parentheses passed"""
    n = bare(n)
    if n.get("kind") == "BinaryOperator" and n.get("opcode") in ARITH:
        if n.get("opcode") in ("&", "|", "^"):              # (bits of two 16-bit values stay 16 bits - unless a
            ts = [old_type(x, text or "") for x in (n.get("inner") or [])[:2]]   # signed one meets an unsigned)
            if not ("u16" in ts and any(t == "i16" and not nonneg(x) for t, x in zip(ts, n.get("inner")))):
                return None
        return n
    if n.get("kind") == "UnaryOperator" and n.get("opcode") in ("-", "~") and not \
       bare((n.get("inner") or [{}])[0]).get("kind") in ("IntegerLiteral", "CharacterLiteral"):
        return n
    return None


def edits_of(tree, text, path):
    """[(offset, text to put there)] - the casts that keep the 8086's arithmetic"""
    MAIN[0] = path
    TEXT[0] = text
    out = []

    done = set()

    def cast(n, t, wrap):
        p = place(n)
        if not p or p[2] != path:
            return
        s = text[p[0]:p[1]]
        if s.startswith("(%s)" % CAST[t]):
            return
        if (p[0], p[1], t, wrap) in done:                   # (this very cast, made twice: once)
            return
        done.add((p[0], p[1], t, wrap))
        if wrap:
            out.append((p[0], "(%s)(" % CAST[t]))
            out.append((p[1], ")"))
        else:
            out.append((p[0], "(%s)" % CAST[t]))

    def walk(n):
        if not isinstance(n, dict):
            return
        inner = n.get("inner") or []
        k = n.get("kind")
        if k == "BinaryOperator" and n.get("opcode") in TESTS and len(inner) >= 2:
            a, b = old_type(inner[0], text), old_type(inner[1], text)
            c = a if n.get("opcode") == ">>" else common(a, b)
            if c in ("u16", "i16"):
                for x, t in ((inner[0], a), (inner[1], b)):
                    if n.get("opcode") == ">>" and x is inner[1]:
                        continue
                    y = compound(x, text)
                    if c == "u16" and t == "i16" and not nonneg(x):   # (the 8086 made it unsigned; here it
                        cast(y or x, "u16", bool(y) or not simple(x, text))   #  stays signed)
                    elif y is not None:                     # (16 bits of arithmetic, cut as the 8086's were)
                        cast(y, c, True)
        if k in ("ImplicitCastExpr", "CStyleCastExpr") and n.get("castKind") == "IntegralCast" and inner:
            to = W16.get(n["type"].get("qualType"))
            if to in ("i32", "u32"):
                y = compound(inner[0], text)
                t = old_type(inner[0], text)
                if y is not None and t in ("u16", "i16"):   # (16-bit arithmetic made a long: cut first)
                    cast(y, t, True)
        for x in inner:
            walk(x)

    for top in tree.get("inner", []):
        walk(top)
    return out


def simple(n, text):
    s = spelled(n, text) or ""
    return bool(re.fullmatch(r"[\w.\->\[\]]+|-?\d+|0[xX][0-9a-fA-F]+", s.strip()))


def apply(text, edits):
    """the text with the insertions made: at one offset a ")" goes before what opens there, and what opens there keeps
    the order it was given in (an outer cast before the one inside it). Every insertion is made: two casts that end at one
    place each have their ")" (a set of the insertions kept one of them: unbalanced), and the order of two that open at one
    place was the set's, not the walk's"""
    out, done = [], 0
    for _, at, s in sorted(((i, e[0], e[1]) for i, e in enumerate(edits)), key=lambda x: (x[1], x[2] != ")", x[0])):
        out.append(text[done:at])
        out.append(s)
        done = at
    out.append(text[done:])
    return "".join(out)


def _headers_key(folder):
    """what every file of the port includes: its headers' texts (one changed: every file's result again)"""
    import hashlib
    h = hashlib.sha256()
    for n in sorted(os.listdir(folder)):
        if n.endswith(".h"):
            h.update(n.encode() + b"\0" + open(os.path.join(folder, n), "rb").read())
    return h.hexdigest()


KINDS = {                                                   # (where each kind of result comes from: these functions
    "arith16": {"arith16": ("dump", "mark_files", "_files", "edits_of", "apply")},   #  and all they use)
    "records": {"arith16": ("dump", "mark_files"), "oldlayout": ("byte_records",)},
    "oldlayout": {"arith16": ("dump", "mark_files", "_files"), "oldlayout": ("edits_of", "apply")},
}
_CODE = {}


def code_key(kind):
    """the code a kind of result comes from: its functions and all they name (of the two passes), by their source -
    a change to another of the passes' rules not this kind's results' (a kind not known: both passes whole)"""
    if kind in _CODE:
        return _CODE[kind]
    import ast
    import hashlib
    here = os.path.dirname(os.path.abspath(__file__))
    h = hashlib.sha256(kind.encode())
    if kind not in KINDS:
        for m in ("arith16.py", "oldlayout.py"):
            h.update(open(os.path.join(here, m), "rb").read())
        _CODE[kind] = h.hexdigest()
        return _CODE[kind]
    defs = {}
    for m in ("arith16", "oldlayout"):
        src = open(os.path.join(here, m + ".py"), encoding="utf-8").read()
        for node in ast.parse(src).body:
            if isinstance(node, (ast.FunctionDef, ast.ClassDef)):
                names = [node.name]
            elif isinstance(node, ast.Assign):
                names = [t.id for t in node.targets if isinstance(t, ast.Name)]
            elif isinstance(node, ast.AnnAssign) and isinstance(node.target, ast.Name):
                names = [node.target.id]
            else:
                continue
            refs = set()
            for x in ast.walk(node):
                if isinstance(x, ast.Name):
                    refs.add((m, x.id))
                elif isinstance(x, ast.Attribute) and isinstance(x.value, ast.Name) and \
                        x.value.id in ("arith16", "oldlayout"):
                    refs.add((x.value.id, x.attr))
            for n in names:
                defs[(m, n)] = (ast.get_source_segment(src, node) or "", refs)
    todo, seen = [(m, n) for m, ns in KINDS[kind].items() for n in ns], set()
    while todo:
        x = todo.pop()
        if x in seen or x not in defs:
            continue
        seen.add(x)
        todo += defs[x][1]
    for x in sorted(seen):
        h.update(repr(x).encode() + b"\0" + defs[x][0].encode() + b"\0")
    _CODE[kind] = h.hexdigest()
    return _CODE[kind]


def reach_key(folder, f):
    """the file and every header of the port's it reaches (an unchanged header's file not read again)"""
    import hashlib
    h = hashlib.sha256()
    todo, seen = [f], set()
    while todo:
        x = todo.pop()
        if x in seen:
            continue
        seen.add(x)
        try:
            t = open(os.path.join(folder, x), "rb").read()
        except OSError:
            continue
        h.update(x.encode() + b"\0" + t + b"\0")
        todo += [m.decode("latin1") for m in re.findall(rb'#\s*include\s*[<"]([^>"]+)[>"]', t)]
    return h.hexdigest()


def pass_key(cc, folder, f, kind, *extra, headers=None):
    """a pass's result for one file, by all it depends on: the file and the headers it reaches, the compiler, the
    code of the pass that result comes from (headers: no longer used - every header of the port)"""
    import hashlib
    h = hashlib.sha256()
    h.update(code_key(kind).encode())
    h.update(repr((os.path.abspath(cc), kind, extra)).encode())
    h.update(reach_key(folder, f).encode())
    return h.hexdigest()


def main_file(files, f):
    """the name clang gave file f among the files it named (separators and a leading ./ aside): not any name that
    merely ends with it - xa.h is not a.h"""
    def norm(n):
        n = n.replace("\\", "/")
        while n.startswith("./"):
            n = n[2:]
        return n
    return next((v for v in files if norm(v) == norm(f)), f)


def run(cc, folder, files):
    """{file: its text with the casts} for the port's C files (those clang could read) - a file's result kept,
    by its text and its headers': the same file again, not read again"""
    out = {}
    hk = _headers_key(folder)
    for f in files:
        key = pass_key(cc, folder, f, "arith16", headers=hk)
        got = cache.value("port-passes", key)
        if got is not None:
            if got.get("new") is not None:
                out[f] = got["new"]
            continue
        tree = dump(cc, f, folder)
        if not tree:
            continue
        mark_files(tree, [None])
        path = main_file(_files(tree), f)
        text = open(folder + "/" + f, encoding="latin1").read()
        e = edits_of(tree, text, path)
        new = apply(text, e) if e else None
        if new is not None:
            out[f] = new
        cache.keep("port-passes", key, {"new": new})
        del tree
    return out


def _files(node, seen=None):
    seen = seen if seen is not None else []
    if isinstance(node, dict):
        if isinstance(node.get("file"), str) and node["file"] not in seen:
            seen.append(node["file"])
        for v in node.values():
            _files(v, seen)
    elif isinstance(node, list):
        for v in node:
            _files(v, seen)
    return seen
