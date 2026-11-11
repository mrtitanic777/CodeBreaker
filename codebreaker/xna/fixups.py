"""fixups.py -- what ILSpy writes for an XNA game that is not valid C#, rewritten as the C# it stands for.

    from codebreaker.xna import fixups
    text, n = fixups.ref_casts(text)        ((Color)(ref white)).A = 64   ->   (white).A = 64
    text, n, left = fixups.constructors(text)
                                            base._002Ector(a);  ->  `: base(a)` on the constructor;
                                            v._002Ector(a);     ->  v = new T(a);   (T from v's declaration)
    text, n = fixups.data_holders(text)     a compiler's static-data holder (<PrivateImplementationDetails>,
                                            renamed by an obfuscator): its private blob structs internal
    fixups.apply(folder)                    all of them, over every .cs file: {"ref casts", "constructors",
                                            "data holders", "left": [(file, line, why)]}

Each rewrite gives the same program: a cast of a value to its own type is no cast (the `ref` in it
is how the decompiler marks a mutable struct's member being reached in place), and a base or this
constructor call at the top of a constructor is what an initializer is (it runs before the body
either way). What a rewrite cannot place with certainty is left as it is and listed.

These came from the Arcadecraft decompile (../Arcadecraft/tools/fix_*.py), where they made 65 000
lines of decompiled C# compile again.
"""
import os
import re

_LVALUE = r"(?:[A-Za-z_]\w*(?:\s*\[[^\[\]]*\])?)(?:\s*\.\s*[A-Za-z_]\w*(?:\s*\[[^\[\]]*\])?)*"
_REF_CAST = re.compile(r"\(\s*[A-Za-z_][\w.]*\s*\)\s*\(\s*ref\s+(" + _LVALUE + r")\s*\)")
_CTOR = re.compile(r"(?P<base>base|this)\._002Ector\s*\("
                   r"|\(\([A-Za-z_][A-Za-z0-9_.]*\)this\)\._002Ector\s*\("
                   r"|\(?(?P<var>[^\W\d]\w*)\)?\._002Ector\s*\(")        # (an obfuscator's names are any letters)


def ref_casts(text):
    total, prev = 0, None
    while prev != text:                                   # nested: ((IDisposable)(Enumerator)(ref e))
        prev = text
        text, n = _REF_CAST.subn(r"\1", text)
        total += n
    return text, total


def _matching(text, open_paren):
    depth = 0
    for i in range(open_paren, len(text)):
        if text[i] == "(":
            depth += 1
        elif text[i] == ")":
            depth -= 1
            if depth == 0:
                return text[open_paren + 1:i], i + 1
    return None, -1


def _enclosing_brace(text, pos):
    depth = 0
    for i in range(pos - 1, -1, -1):
        if text[i] == "}":
            depth += 1
        elif text[i] == "{":
            if depth == 0:
                return i
            depth -= 1
    return -1


_STATEMENTS = ("if", "while", "for", "foreach", "using", "lock", "switch", "catch", "fixed")


def _statement_keyword(text, close):
    """The word before the parenthesis that the `)` at `close` closes (`if` for `if (a)`), or ''."""
    depth = 0
    for i in range(close, -1, -1):
        if text[i] == ")":
            depth += 1
        elif text[i] == "(":
            depth -= 1
            if depth == 0:
                m = re.search(r"([A-Za-z_]\w*)\s*$", text[:i])
                return m.group(1) if m else ""
    return ""


def _declared_type(text, var, before):
    """The type of the nearest declaration of `var` before `before` (the decompiler writes one per line)."""
    rx = re.compile(r"(?m)^[ \t]*([A-Za-z_][A-Za-z0-9_<>,.\[\] ]*?)[ \t]+" + re.escape(var) + r"[ \t]*(?:=(?!=)|;)")
    hits = list(rx.finditer(text, 0, before))
    return hits[-1].group(1).strip() if hits else None


def constructors(text):
    if "_002Ector" not in text:
        return text, 0, []
    edits, left = [], []
    for m in _CTOR.finditer(text):
        paren = text.index("(", m.end() - 1)
        args, after = _matching(text, paren)
        if args is None:
            continue
        line = text.count("\n", 0, m.start()) + 1
        if m.group("base") or m.group("var") is None:
            kind = m.group("base") or "base"              # ((Base)this).ctor is the base constructor
            brace = _enclosing_brace(text, m.start())
            close = text.rfind(")", 0, brace) if brace > 0 else -1
            if close <= 0:
                left.append((line, "no constructor parameter list before it"))
                continue
            if text[close + 1:brace].strip() or _statement_keyword(text, close) in ("base", "this"):   # `: base()` there
                left.append((line, "the constructor already has an initializer"))
                continue
            if _statement_keyword(text, close) in _STATEMENTS:   # `if (a) {` is no constructor
                left.append((line, "it is inside a statement block, not at the top of a constructor"))
                continue
            edits.append((close + 1, close + 1, " : %s(%s)" % (kind, args)))
            semi = after
            while semi < len(text) and text[semi] in " \t":
                semi += 1
            if semi < len(text) and text[semi] == ";":
                after = semi + 1
            start = text.rfind("\n", 0, m.start()) + 1
            end = text.find("\n", after)                  # the call may span lines: through the line its `)` is on
            end = len(text) if end < 0 else end
            if not text[start:m.start()].strip() and not text[after:end].strip():
                edits.append((start, min(end + 1, len(text)), ""))      # the call is alone on its lines: they go
            else:
                edits.append((m.start(), after, ""))                    # it shares a line: only the call goes
        else:
            var = m.group("var")
            typ = _declared_type(text, var, m.start())
            if not typ:
                left.append((line, "no declaration of %s before it" % var))
                continue
            edits.append((m.start(), after, "%s = new %s(%s)" % (var, typ, args)))
    applied, last = 0, len(text) + 1
    for s, e, r in sorted(edits, key=lambda x: x[0], reverse=True):
        if e > last:
            continue
        text = text[:s] + r + text[e:]
        last = s
        applied += 1
    return text, applied, left


def data_holders(text):
    """The compiler's holder of static array data (<PrivateImplementationDetails>; an obfuscator renames it):
    ILSpy writes its fields as `internal static StructN X /* Not supported: data(...) */` over private blob
    structs, which C# refuses (CS0052), and turns the code that used the data into array literals. The blob
    structs become internal; nothing reads them. -> (text, structs changed)"""
    if "/* Not supported: data(" not in text:
        return text, 0
    return re.subn(r"\bprivate(\s+struct\s+\w+)", r"internal\1", text)


def apply(folder):
    report = {"ref casts": 0, "constructors": 0, "data holders": 0, "left": []}
    for base, _dirs, files in os.walk(folder):
        for f in files:
            if not f.endswith(".cs"):
                continue
            p = os.path.join(base, f)
            with open(p, encoding="utf-8-sig", newline="") as fh:        # line endings come back as they went in
                text = fh.read()
            fixed, n_ref = ref_casts(text)
            fixed, n_ctor, left = constructors(fixed)
            fixed, n_data = data_holders(fixed)
            report["data holders"] += n_data
            report["ref casts"] += n_ref
            report["constructors"] += n_ctor
            report["left"] += [(os.path.relpath(p, folder), ln, why) for ln, why in left]
            if fixed != text:
                with open(p, "w", encoding="utf-8", newline="") as fh:
                    fh.write(fixed)
    return report
