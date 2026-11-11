"""shape.py -- the same function written in the other shapes a person might have written it in.

    for start, end, text in shape.switches(body, shadow):   an if-chain on one value as a switch
    new_body = shape.returns(head, decls, body, shadow)       `r = x; ... return r;` as `return x;`

A decompiler gives every function one shape: tests in chains of if and else, one variable that
collects the result, one return at the end. A compiler that optimises sees through small
differences of spelling (the order of a comparison's sides, the order of declarations) and gives
the same bytes for all of them -- but not through these: a `switch` is compiled as a switch, and
a function that returns from the middle is laid out differently from one that does not. Each
rewrite here changes no meaning; verify/authored/repair.py tries them and keeps what brings the bytes
nearer the original's.

`shadow` is the body with its strings and comments blanked (repair._blank), the same length:
where things are is found in it, what is written comes from the body.
"""
import re

_NUM = r"(?:-?0x[0-9a-fA-F]+|-?\d+)"
_NAME = r"[A-Za-z_]\w*"


def _close(shadow, i, pair="{}"):
    """The index of the bracket that closes the one at `i`, or -1."""
    depth = 0
    for j in range(i, len(shadow)):
        if shadow[j] == pair[0]:
            depth += 1
        elif shadow[j] == pair[1]:
            depth -= 1
            if depth == 0:
                return j
    return -1


def statements(shadow, lo, hi):
    """[(start, end)] of the statements between `lo` and `hi`, an if with all its elses as one."""
    out, i = [], lo
    while i < hi:
        while i < hi and shadow[i].isspace():
            i += 1
        if i >= hi:
            break
        start, depth = i, 0
        loop = re.match(r"do\b", shadow[i:hi]) is not None
        while i < hi:
            c = shadow[i]
            if c == "(":
                depth += 1
            elif c == ")":
                depth -= 1
            elif c == ";" and depth == 0:
                i += 1
                break
            elif c == "{" and depth == 0:
                j = _close(shadow, i)
                if j < 0 or j >= hi:
                    return out                         # not well formed: say nothing more
                i = j + 1
                rest = shadow[i:hi]
                if re.match(r"\s*else\b", rest):
                    i += re.match(r"\s*else\b", rest).end()
                    continue
                if loop and re.match(r"\s*while\b", rest):
                    continue                           # do { } while (...);
                break
            i += 1
        out.append((start, i))
    return out


def switches(body, shadow):
    """(start, end, text) for each chain `if (v == A) {..} else if (v == B) {..} [else {..}]`:
    the same as a switch. Not when a branch holds a `break` (it would leave the switch instead
    of the loop it was in), nor when a value is tested twice."""
    for m in re.finditer(r"\bif \((%s) == (%s)\) \{" % (_NAME, _NUM), shadow):
        before = shadow[:m.start()].rstrip()
        if before.endswith("else") or before[-1:] not in ";{}":
            continue                                   # inside a chain, or not a statement
        v, cases, default = m.group(1), [], None
        at, value, brace = m.start(), m.group(2), m.end() - 1
        while True:
            end = _close(shadow, brace)
            if end < 0:
                cases = []
                break
            cases.append((value, brace, end))
            at = end + 1
            again = re.match(r"\s*else if \(%s == (%s)\) \{" % (re.escape(v), _NUM), shadow[at:])
            if again:
                value, brace = again.group(1), at + again.end() - 1
                continue
            last = re.match(r"\s*else \{", shadow[at:])
            if last:
                brace = at + last.end() - 1
                end = _close(shadow, brace)
                if end < 0:
                    cases = []
                    break
                default, at = (brace, end), end + 1
            break
        try:
            values = [int(c[0], 0) & 0xFFFFFFFF for c in cases]
        except ValueError:                             # an octal constant (012): int() does not read it, so the chain stays
            continue
        if len(cases) < 2 or len(set(values)) != len(values):
            continue
        if re.search(r"\bbreak\b", shadow[m.start():at]):
            continue
        lines = ["switch (%s) {" % v]
        for value, b0, b1 in cases:
            lines.append("case %s:\n{%s}\nbreak;" % (value, body[b0 + 1:b1]))
        if default:
            lines.append("default:\n{%s}" % body[default[0] + 1:default[1]])
        lines.append("}")
        yield m.start(), at, "\n".join(lines)


def _tail(body, shadow, start, end, r, edits):
    """Collect, for the statement at [start, end), every `r = x;` that is the last thing done
    before control reaches the end of the function."""
    text = shadow[start:end]
    m = re.match(r"%s = ([^;{}]+);\s*$" % re.escape(r), text)
    if m and "{" not in text:
        edits.append((start, end, "return %s;" % body[start + m.start(1):start + m.end(1)].strip()))
        return
    if not re.match(r"if\s*\(", text):
        return
    at = start + text.index("(")
    close = _close(shadow, at, "()")
    while close >= 0:
        brace = close + 1
        while brace < end and shadow[brace].isspace():
            brace += 1
        if brace >= end or shadow[brace] != "{":
            return
        stop = _close(shadow, brace)
        if stop < 0:
            return
        inner = statements(shadow, brace + 1, stop)
        if inner:
            _tail(body, shadow, inner[-1][0], inner[-1][1], r, edits)
        rest = re.match(r"\s*else\b\s*", shadow[stop + 1:end])
        if not rest:
            return
        at = stop + 1 + rest.end()
        if shadow[at:at + 1] == "{":                    # else { ... }
            stop = _close(shadow, at)
            inner = statements(shadow, at + 1, stop) if stop >= 0 else []
            if inner:
                _tail(body, shadow, inner[-1][0], inner[-1][1], r, edits)
            return
        nxt = re.match(r"if\s*\(", shadow[at:end])     # else if (...)
        if not nxt:
            return
        at += nxt.end() - 1
        close = _close(shadow, at, "()")


def returns(head, decls, body, shadow):
    """The body with every `r = x;` that only the final `return r;` follows written `return x;`,
    or None when there is none. Only when `r` has the type the function returns: handing a value
    back through a narrower variable cuts it down, and returning it directly would not."""
    top = statements(shadow, 0, len(shadow.rstrip().rstrip("}")))
    if len(top) < 2:
        return None
    last = re.match(r"return (%s);\s*$" % _NAME, shadow[top[-1][0]:top[-1][1]])
    if not last:
        return None
    r = last.group(1)
    own = re.search(r"(?m)^\s*(\w+)\s+%s;\s*$" % re.escape(r), "\n".join(decls)) or \
        re.search(r"[(,]\s*(\w+)\s+%s\s*[,)]" % re.escape(r), head)
    if not own or own.group(1) != _returned(head):
        return None
    edits = []
    _tail(body, shadow, top[-2][0], top[-2][1], r, edits)
    if not edits:
        return None
    for start, end, new in sorted(edits, reverse=True):
        body = body[:start] + new + body[end:]
    return body


def _returned(head):
    """The type the function returns, from its heading (the word before its convention and name)."""
    lines = [ln for ln in head.split("\n") if "(" in ln]
    if not lines:
        return None
    words = lines[-1].split("(")[0].split()
    words = [w for w in words if not w.startswith("__")]
    return words[-2] if len(words) >= 2 else None


def folds(decls, body, shadow):
    """(start, end, text) for each `t = f(...);` followed at once by `if (t <compare> N) {`, where
    that is all `t` is for: written `if (f(...) <compare> N) {`. A decompiler names every value it
    sees; a person tests the call. Only for a variable of the type a function returns (int4), so
    that nothing is cut down or widened on the way, and only when the two lines are its only
    mentions."""
    plain = {m.group(1) for d in decls for m in [re.match(r"\s*int4 (%s);\s*$" % _NAME, d)] if m}
    line = re.compile(r"(?m)^(?P<in>[ \t]*)(?P<t>%s) = (?P<call>%s\([^;\n]*\));\n[ \t]*if \((?P=t) (?P<op>==|!=|<=|>=|<|>) "
                      r"(?P<n>%s)\) \{" % (_NAME, _NAME, _NUM))
    for m in line.finditer(shadow):
        t = m.group("t")
        if t not in plain or len(re.findall(r"\b%s\b" % re.escape(t), shadow)) != 2:
            continue
        call = body[m.start("call"):m.end("call")]
        if _close(shadow, m.start("call") + shadow[m.start("call"):].index("("), "()") != m.end("call") - 1:
            continue                                   # more than the one call on that line
        yield m.start(), m.end(), "%sif (%s %s %s) {" % (m.group("in"), call, m.group("op"), m.group("n"))
