"""text.py -- what every rewrite of a draft reads its text with: the parts of an if, a block's ends, a test's
parentheses and negation, whether a register is read later, and the names of registers and the library's data."""
import re


REGS = r"\b(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh|es)\b"
WREG = r"(ax|bx|cx|dx|si|di)"


def _strip(c):
    """((X)) -> (X): the parentheses a test is printed in, once"""
    while c.startswith("((") and c.endswith("))"):
        depth = 0
        for k, ch in enumerate(c[1:-1]):
            depth += (ch == "(") - (ch == ")")
            if depth < 0:
                return c
        if depth:
            return c
        c = c[1:-1]
    return c


def _truth(c):
    """a test as the value it tests: (X == 0) -> !X, (X != 0) -> X"""
    c = _strip(c)
    m = re.fullmatch(r"\((.*) != 0\)", c)
    if m:
        return m.group(1)
    m = re.fullmatch(r"\((.*) == 0\)", c)
    if m:
        return "!%s" % m.group(1) if re.fullmatch(r"[\w.\->\[\]()]+", m.group(1)) else "!(%s)" % m.group(1)
    return c


def _balanced(s):
    depth = 0
    for ch in s:
        depth += (ch == "(") - (ch == ")")
        if depth < 0:
            return False
    return depth == 0


def _top_split(c, op):
    """c split at op (&& or ||) outside parentheses, or [c]"""
    out, depth, cur, k = [], 0, "", 0
    while k < len(c):
        ch = c[k]
        depth += (ch == "(") - (ch == ")")
        if depth == 0 and c.startswith(" %s " % op, k):
            out.append(cur)
            cur = ""
            k += len(op) + 2
            continue
        cur += ch
        k += 1
    return out + [cur]


def _not(c):
    c = _strip(c)
    inner = c[1:-1] if c.startswith("(") and c.endswith(")") and _balanced(c[1:-1]) else c
    m = re.fullmatch(r"!\((.*)\)", inner)                   # (!(X): X)
    if m and _balanced(m.group(1)):
        return "(%s)" % m.group(1)
    if re.fullmatch(r"![\w\->\[\].]+", inner):              # (!x: x)
        return inner[1:]
    for op, other in (("&&", "||"), ("||", "&&")):          # (De Morgan, a level down)
        parts = _top_split(inner, op)
        if len(parts) > 1:
            return "(%s)" % (" %s " % other).join(_truth(_not("(" + p + ")")) for p in parts)
    m = re.fullmatch(r"\((.*?) (==|!=|<=|>=|<|>) (.*)\)", c)
    if m and m.group(2) and "(" not in m.group(3).replace("(", "", 0) and m.group(1).count("(") == m.group(1).count(")"):
        flip = {"==": "!=", "!=": "==", "<": ">=", ">=": "<", ">": "<=", "<=": ">"}[m.group(2)]
        if m.group(3) != "0" or m.group(2) not in ("==", "!="):   # (x == 0: !x below; a relation to 0 flips)
            return "(%s %s %s)" % (m.group(1), flip, m.group(3))
    m = re.fullmatch(r"\((.*) == 0\)", c)
    if m:
        return m.group(1)
    m = re.fullmatch(r"\((.*) != 0\)", c)
    if m:
        return "!%s" % m.group(1)
    if re.fullmatch(r"[\w\->\[\].]+", inner):
        return "!%s" % inner
    return "!%s" % c


UCHAR = set()                                               # (bytes the code widens with zeros: unsigned char)
BYTEG = set()                                               # (globals the code reads or writes a byte of: char)


def _opener(lines, k):
    """the line opening the block line k (`}` or `} else {`) closes: the nearest before it at its indentation"""
    ind = len(lines[k]) - len(lines[k].lstrip())
    return next((j for j in range(k - 1, -1, -1) if lines[j].strip() and len(lines[j]) - len(lines[j].lstrip()) == ind), None)


def _in_loop(lines, lo, j, ind):
    """is line j inside a loop or a switch opened after line lo, deeper than the indentation ind"""
    want = len(lines[j]) - len(lines[j].lstrip())
    for x in range(j - 1, lo, -1):
        l = lines[x]
        if not l.strip():
            continue
        d = len(l) - len(l.lstrip())
        if d < want and l.rstrip().endswith("{"):
            if d <= ind:
                return False
            if re.match(r"\s*(?:while|for|do|switch)\b", l) or l.strip() == "do {":
                return True
            want = d
    return False


def _changed(f, text):
    """f(text), or None when it changes nothing (a pass as a variant)"""
    t = f(text)
    return t if t != text else None


def _openers(lines, k):
    """[the line opening each block around line k, the innermost first]"""
    out, want = [], len(lines[k]) - len(lines[k].lstrip())
    for j in range(k - 1, -1, -1):
        x = lines[j]
        if not x.strip():
            continue
        d = len(x) - len(x.lstrip())
        if d < want and x.rstrip().endswith("{"):
            out.append(j)
            want = d
    return out


GOTO_LIVE = [True]


def _live_after(lines, k, reg, read=None):
    """is reg (or what `read` matches: its byte too) read after line k before it is given a new value - on any
    path: a value given it inside a nested block (an arm, a loop) is not on every path, so it ends nothing; one
    given at k's own depth, in a bare block, or out of k's block does (lines None: dropped)"""
    rx = re.compile(read or r"\b%s\b" % reg)
    kill = re.compile(r"\s*%s = (?![^;]*\b%s\b)" % (reg, reg))
    stack = []                                              # (the blocks open since k: True a conditional one)
    skip = None                                             # (the depth of an arm beside k's, not on its path)
    killed = None                                           # (the depth of a block that gave it a new value)
    for l in lines[k + 1:]:
        if l is None:
            continue
        opens, closes = l.count("{"), l.count("}")
        if skip is not None:
            skip += opens - closes
            if skip < 0:
                skip = None                                 # (the other arm closed: past the whole if)
            continue
        if re.match(r"\s*\} else\b.*\{\s*$", l) and not stack:   # (leaving k's arm into the other: skipped)
            skip = 0
            continue
        if not stack and re.match(r"\s*return\b", l) and not rx.search(l):
            return False                                    # (the path ends)
        g_ = re.match(r"\s*goto (L_\w+);", l) if GOTO_LIVE[0] and killed is None else None
        if g_ and lines is not None:                        # (a jump elsewhere: read there before it is given?)
            at = next((j for j, x in enumerate(lines) if x is not None and x.strip() == g_.group(1) + ":"), None)
            if at is not None:
                for x in lines[at + 1:at + 40]:
                    if x is None:
                        continue
                    if kill.match(x):
                        break                               # (given there first: not this value)
                    if rx.search(x):
                        return True
                    if re.match(r"\s*(?:return|goto|break|continue)\b", x):
                        break
        if not stack and re.match(r"\s*(?:break|continue);", l):
            skip = 0                                        # (to the end of the block it leaves: the rest of
            continue                                        #  it - a switch's other cases - not on its path)
        if killed is not None and len(stack) < killed:
            killed = None                                   # (out of the block that gave it a new value)
        if kill.match(l):
            if not any(stack):
                return bool(rx.search(l.split("=", 1)[1]))
            if killed is None and rx.search(l.split("=", 1)[1]):   # (given a value on some paths: read only in it)
                return True
            killed = len(stack) if killed is None else min(killed, len(stack))
        elif rx.search(l) and killed is None:
            return True
        for _ in range(closes):
            if stack:
                stack.pop()                                 # (out of k's block: what follows is on its path)
        stack += [l.strip() != "{"] * opens
    return False


SIZES = {"char": 1, "int": 2, "unsigned": 2, "short": 2, "long": 4}
PTRSIZE = {}                                                # (data a draft steps through by 2: a pointer to words)
STRUCTARR = {}                                              # (data an array of structs: name -> (size, first field's type))
ARRAYS = {}                                                 # (data a draft indexes: an array - name: element type)
FILEPTR = {}                                                # (a stream a library macro reads: name -> its type)
_LIBDATA = {}
_HEADERS = {}
_TABLES = {}


def _top_commas(s):
    """s split at its commas outside parentheses and brackets"""
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch in "([":
            depth += 1
        elif ch in ")]":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += ch
    return out + [cur]


def _block_end(lines, k):
    """the index of the line closing the block line k opens (by indentation: the first line back at its indent)"""
    ind = len(lines[k]) - len(lines[k].lstrip())
    for j in range(k + 1, len(lines)):
        if lines[j].strip() and len(lines[j]) - len(lines[j].lstrip()) <= ind:
            return j
    return None


def _else_ifs(lines):
    """`} else if (C) {` chains written out as nested ifs, each inside the else before it"""
    k = 0
    while k < len(lines):
        m = re.match(r"(\s*)\} else if \((.*)\) \{$", lines[k])
        if not m:
            k += 1
            continue
        ind = m.group(1)
        e = k + 1                                           # (the chain's end: the `}` at the if's indent that
        while e < len(lines) and not (lines[e] == ind + "}"):   #  closes the whole chain)
            e += 1
        if e >= len(lines):
            return lines
        inner = [ind + "    if (%s) {" % m.group(2)] + ["    " + l if l.strip() else l for l in lines[k + 1:e]] + [ind + "    }"]
        lines[k:e + 1] = [ind + "} else {"] + inner + [ind + "}"]
        k += 1
    return lines


def _if_parts(lines, k):
    """(cond, then, else, end) of the if at line k - `if (C) {`, its arms' lines, the line after it - or None"""
    m = re.match(r"(\s*)if \((.*)\) \{$", lines[k])
    if not m:
        return None
    ind = m.group(1)
    j = k + 1
    while j < len(lines) and not lines[j].startswith(ind + "}"):
        if lines[j].strip() and not lines[j].startswith(ind + " "):
            return None
        j += 1
    if j >= len(lines):
        return None
    then = lines[k + 1:j]
    if lines[j] == ind + "}":
        return m.group(2), then, None, j + 1
    if lines[j] != ind + "} else {":
        return None
    e = j + 1
    while e < len(lines) and not lines[e].startswith(ind + "}"):
        if lines[e].strip() and not lines[e].startswith(ind + " "):
            return None
        e += 1
    if e >= len(lines) or lines[e] != ind + "}":
        return None
    return m.group(2), then, lines[j + 1:e], e + 1


def _unlabelled(arm, other=None):
    """arm without the label the other arm's goto went to (the only label it loses: others are wanted)"""
    g = re.match(r"\s*goto (L_\w+);$", other[-1]) if other else None
    if other is not None and not g:
        return arm
    return [l for l in arm if not re.match(r"\s*L_\w+:$", l) or (g and l.strip() != g.group(1) + ":")]


def _conj(op, a, b):
    """a op b, an operand that is the other kind of join parenthesized (|| inside &&)"""
    wrap = lambda x: "(%s)" % x if op == "&&" and len(_top_split(_strip(x) if x.startswith("((") else x, "||")) > 1 else x
    return "%s %s %s" % (wrap(a), op, wrap(b))


def _same_arm(x, y):
    """do the arms x and y run the same: equal, or x the statements y begins with and then a goto to y's label
    there (the compiler's one copy of a tail both reach) - and the label then wanted by none else"""
    if x == y:
        return True
    if not x or not re.match(r"\s*goto (L_\w+);$", x[-1]):
        return False
    lab = re.match(r"\s*goto (L_\w+);$", x[-1]).group(1)
    at = next((j for j, l in enumerate(y) if l.strip() == lab + ":"), None)
    return at is not None and x[:-1] == y[:at] and _unlabelled(x[:-1]) == x[:-1]


def _plain_store(l, reg):
    """`X = E;` with reg read in E alone and E giving nothing a value: reg's value read before the store"""
    m = re.fullmatch(r"\s*([^=;]+?) = ([^;]+);\s*", l)
    return bool(m) and not re.search(r"\b%s\b" % reg, m.group(1)) and not re.search(r"\+\+|--|[^=!<>]=[^=]", m.group(2)) \
        and not re.search(r"\w\s*\(", m.group(2) + m.group(1))


RANGES = set()                                              # (the ranges a loaded register is tested against: (lo, hi))
LONGINFO = {}                                               # (the function's longs the code shows: unsigned ones, its value)


def _declare_pointer(src, x, ty):
    """the source with x (an int parameter or local) declared a pointer to ty, or None"""
    head, sep, body = split_body(src)
    for part in ("head", "body"):
        t = head if part == "head" else body
        m = re.search(r"(?m)^([ \t]*)(register\s+)?int ((?:\w+, )*)%s((?:, \w+)*);$" % re.escape(x), t)
        if m:
            rest = (m.group(3) + m.group(4).lstrip(", ")).strip(", ")
            new = ("%s%sint %s;\n" % (m.group(1), m.group(2) or "", rest) if rest else "") + "%s%s%s *%s;" % (m.group(1), m.group(2) or "", ty, x)
            t = t[:m.start()] + new + t[m.end():]
            out = (t + sep + body) if part == "head" else (head + sep + t)
            if ty == "int":                                 # (its byte offsets, now in words: x + N/2)
                out = re.sub(r"\(\(int \*\)\(\(char \*\)%s \+ (\d+)\)\)" % re.escape(x),
                             lambda m2: "(%s + %d)" % (x, int(m2.group(1)) // 2) if int(m2.group(1)) % 2 == 0 else m2.group(0), out)
                out = re.sub(r"\b%s\[\((.+?) << 1\) \+ (\d+)\]" % re.escape(x),     # (an index the code doubles)
                             lambda m2: "%s[%s + %d]" % (x, m2.group(1), int(m2.group(2)) // 2) if int(m2.group(2)) % 2 == 0 else m2.group(0), out)
                out = re.sub(r"\b%s\[\((.+?) << 1\)\]" % re.escape(x), r"%s[\1]" % x, out)
            return out
    return None


BYTEPTR = set()                                             # (the byte-wide places the code reads: (pointer, offset))


def split_body(text):
    """(head, "{", body) at the function body's own brace - a line of "{" alone, after its signature - not a
    struct's that a pass declared before it"""
    m = re.search(r"(?m)^\{\s*$", text)
    if not m:
        return text.partition("{")
    return text[:m.start()], "{", text[m.start() + 1:]
