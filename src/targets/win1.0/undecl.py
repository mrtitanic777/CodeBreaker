#!/usr/bin/env python3
"""
undecl.py — surface every UNDECLARED identifier in a reconstructed app, per
function, with its evidence lines and an inferred type.

This is the tool for class-4 of the undefined-symbol tail (see the build notes):
locals the lift never emitted (`buf`, `hFile`, `reject`, `siFlag`, ...). Instead
of learning about them one compile error per DOSBox run, this lists all of them
at once, with the evidence that fixes them, so declarations can be applied in
evidence-backed batches and each one stays visible and flaggable.

It is a READER only — it never edits the source. Nothing here guesses beyond
what the code itself shows; where the evidence is thin it says so (confidence
LOW) rather than inventing a type.

Method
  * The set of legitimately-declared outside names is harvested from the REAL
    1985 SDK header (build16/INC30/WINDOWS.H) — file-scope only, so struct
    MEMBER names (x, y, cx, hdc, message, ...) are NOT whitelisted and a local
    that happens to share a member's name is still reported.
  * Types are inferred from the header's own prototypes: `n = SomeApi(...)`
    gives n the API's return type; `SomeApi(..., n, ...)` gives n the declared
    type of that parameter; `n` used as a text buffer with a following size
    argument gives `char n[size]`.

Usage
    python undecl.py reconstructed/control.c            # review list
    python undecl.py reconstructed/control.c --json out.json
    python undecl.py reconstructed/*.c --summary        # counts per app
"""
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
from _wtroot import data_root as _data_root
ROOT = _data_root(HERE)   # DATA root (build16/ reconstructed/ refs/ files/); tools live at HERE
WINH = os.path.join(ROOT, "build16", "INC30", "WINDOWS.H")

KEYWORDS = set("""
auto break case char const continue default do double else entry enum extern
float for fortran goto if int long register return short signed sizeof static
struct switch typedef union unsigned void volatile while near far huge pascal
cdecl _asm asm
""".split())

# Storage/qualifier tokens that may lead a declaration.
LEADERS = set("static register auto const volatile extern unsigned signed "
              "short long struct union enum near far huge NEAR FAR PASCAL "
              "_far _near".split())

BASE_TYPES = set("char int float double void long short unsigned signed".split())

# pointer/calling-convention qualifiers that may sit between the type and the
# declarator name: `char FAR* p;` declares p.
QUALS = set("FAR NEAR HUGE far near huge _far _near _huge PASCAL pascal cdecl "
            "const volatile register".split())


# --------------------------------------------------------------------------
# 1. comment/string blanking (keeps every byte offset and line number intact)
# --------------------------------------------------------------------------
def blank(src):
    """Replace comment and literal bodies with spaces, preserving newlines."""
    out = list(src)
    i, n = 0, len(src)
    while i < n:
        c = src[i]
        if c == '/' and i + 1 < n and src[i + 1] == '*':
            j = src.find('*/', i + 2)
            j = n if j < 0 else j + 2
            for k in range(i, j):
                if out[k] != '\n':
                    out[k] = ' '
            i = j
        elif c == '/' and i + 1 < n and src[i + 1] == '/':
            j = src.find('\n', i)
            j = n if j < 0 else j
            for k in range(i, j):
                out[k] = ' '
            i = j
        elif c in '"\'':
            q, j = c, i + 1
            while j < n and src[j] != q:
                j += 2 if src[j] == '\\' else 1
            j = min(j + 1, n)
            for k in range(i + 1, j - 1):
                if out[k] != '\n':
                    out[k] = ' '
            i = j
        else:
            i += 1
    return ''.join(out)


def strip_directives(text):
    """Blank preprocessor lines (they are not statements) but keep offsets."""
    out = list(text)
    for m in re.finditer(r'(?m)^[ \t]*#.*$', text):
        for k in range(m.start(), m.end()):
            out[k] = ' '
    return ''.join(out)


def file_scope_statements(text):
    """Yield each file-scope statement with brace BODIES dropped, so
    `typedef struct tagX { ... } X;` arrives as one statement and struct
    members never look like file-scope names. A closing brace that ends a
    function DEFINITION resets the buffer (its header is not a declaration
    that continues into the next statement)."""
    depth, cur = 0, []
    for ch in text:
        if ch == '{':
            depth += 1
            continue
        if ch == '}':
            depth = max(0, depth - 1)
            if depth == 0 and re.search(r'\)\s*$', ''.join(cur).strip()):
                cur = []                       # that was a function body
            continue
        if depth:
            continue
        cur.append(ch)
        if ch == ';':
            yield ''.join(cur).strip()
            cur = []


def blank_aggregates(text):
    """Blank the BODY of every struct/union/enum {...} (keeping the braces and
    every byte offset), so an inline aggregate declaration reads as one
    statement instead of being split at its members' semicolons."""
    out = list(text)
    for m in re.finditer(r'\b(?:struct|union|enum)\b[^;{}]*\{', text):
        d, j = 0, m.end() - 1
        while j < len(text):
            if text[j] == '{':
                d += 1
            elif text[j] == '}':
                d -= 1
                if d == 0:
                    break
            j += 1
        for k in range(m.end(), j):
            if out[k] != '\n':
                out[k] = ' '
    return ''.join(out)


def split_top(s, sep=','):
    """Split on `sep` at paren/bracket depth 0."""
    parts, depth, cur = [], 0, []
    for ch in s:
        if ch in '([':
            depth += 1
        elif ch in ')]':
            depth -= 1
        if ch == sep and depth == 0:
            parts.append(''.join(cur)); cur = []
        else:
            cur.append(ch)
    parts.append(''.join(cur))
    return parts


# --------------------------------------------------------------------------
# 2. the 1985 header: file-scope names + prototype table
# --------------------------------------------------------------------------
PROTO = re.compile(
    r'^\s*([A-Za-z_][\w \t\*]*?)\s+(?:FAR\s+)?(?:PASCAL\s+|cdecl\s+)?'
    r'(\*?\s*[A-Za-z_]\w*)\s*\(([^;{]*)\)\s*;')


def scan_header(path):
    """-> (known names at file scope, {func: (rettype, [paramtypes])})"""
    known, protos = set(), {}
    if not os.path.exists(path):
        return known, protos
    raw = open(path, encoding='cp437', errors='replace').read()
    for m in re.finditer(r'(?m)^\s*#\s*define\s+([A-Za-z_]\w*)', raw):
        known.add(m.group(1))
    text = strip_directives(blank(raw))
    # Walk file scope only: characters inside braces (struct/union bodies) are
    # dropped, so MEMBER names never enter the whitelist — a local called `x`
    # or `hdc` must still be declared by the app itself.
    depth, cur = 0, []
    for ch in text:
        if ch == '{':
            depth += 1
            continue
        if ch == '}':
            depth = max(0, depth - 1)
            continue
        if depth:
            continue
        cur.append(ch)
        if ch == ';':
            stmt = ''.join(cur).strip()
            cur = []
            if stmt.startswith('typedef'):
                # must be tested before PROTO: `typedef int (FAR PASCAL
                # *FARPROC)();` otherwise reads as a function declaration.
                known.update(declared_names(stmt))
                continue
            m = PROTO.match(stmt)
            if m:
                ret = ' '.join(m.group(1).split())
                name = m.group(2).replace('*', '').strip()
                params = [' '.join(p.split()) for p in split_top(m.group(3))]
                params = [p for p in params if p and p != 'void']
                known.add(name)
                protos[name] = (ret, params)
                continue
            # typedef / plain declaration: every identifier at declarator
            # position is a file-scope name.
            for nm in declared_names(stmt):
                known.add(nm)
    # struct/union/enum tags declared anywhere
    for m in re.finditer(r'\b(?:struct|union|enum)\s+([A-Za-z_]\w*)', text):
        known.add(m.group(1))
    return known, protos


# --------------------------------------------------------------------------
# 3. declaration recognition
# --------------------------------------------------------------------------
def declared_names(stmt, typenames=None):
    """If `stmt` is a declaration, return the names it declares (else [])."""
    s = ' '.join(stmt.replace('\n', ' ').split())
    s = s.rstrip(';').strip()
    s = s.replace('*', ' * ')          # so `char FAR* p` tokenizes cleanly
    if not s or s.startswith('#'):
        return []
    toks = re.findall(r'[A-Za-z_]\w*|.', s)
    if not toks:
        return []
    first = toks[0]
    is_typedef = first == 'typedef'
    if is_typedef:
        # typedef ... NAME, *LPNAME, *PNAME;  -> every declarator
        out = []
        for part in split_top(s[len('typedef'):]):
            ids = re.findall(r'[A-Za-z_]\w*', part)
            if ids:
                out.append(ids[-1])
        return out
    known_type = (first in LEADERS or first in BASE_TYPES or
                  (typenames is not None and first in typenames) or
                  (typenames is None and re.match(r'^[A-Z][A-Z0-9_]*$', first)))
    if not known_type:
        return []
    if '(' in s and re.search(r'\)\s*$', s) is None and '=' not in s:
        pass  # e.g. "int foo();" handled below by the declarator walk
    # drop the leading type tokens: everything up to the first identifier that
    # is followed by something other than another type token
    body = s
    # remove struct/union/enum tag and (already-blanked) body
    body = re.sub(r'^(struct|union|enum)\b\s*(\w+)?\s*\{\s*\}\s*', '', body)
    body = re.sub(r'^(struct|union|enum)\s+\w+\s+', '', body)
    stripped_agg = body != s
    words = body.split()
    idx = 0
    while idx < len(words) and (words[idx] in LEADERS or words[idx] in BASE_TYPES
                                or words[idx] in QUALS or words[idx] == '*'
                                or (typenames and words[idx] in typenames)
                                or re.match(r'^[A-Z][A-Z0-9_]*$', words[idx])
                                and idx == 0):
        idx += 1
    if idx == 0 and not stripped_agg:
        return []
    rest = ' '.join(words[idx:])
    names = []
    for part in split_top(rest):
        part = part.split('=')[0].strip()
        if not part:
            continue
        # the declarator's name is its first identifier that is not a pointer
        # qualifier — `char FAR* p` declares p, not FAR.
        for tok in re.findall(r'[A-Za-z_]\w*', part.replace('*', ' ')):
            if tok in QUALS:
                continue
            names.append(tok)
            break
    return names


# --------------------------------------------------------------------------
# 4. function extraction
# --------------------------------------------------------------------------
FUNC_HEAD = re.compile(r'([A-Za-z_]\w*)\s*\(([^()]*)\)\s*$')


def functions(text):
    """Yield (name, params[], body_start, body_end) for each definition."""
    depth, i, n = 0, 0, len(text)
    anchor = 0            # start of the current top-level statement
    while i < n:
        ch = text[i]
        if ch == '{':
            if depth == 0:
                head = text[anchor:i].strip()
                m = FUNC_HEAD.search(head)
                if m:
                    start = i
                    d, j = 0, i
                    while j < n:
                        if text[j] == '{':
                            d += 1
                        elif text[j] == '}':
                            d -= 1
                            if d == 0:
                                break
                        j += 1
                    params = []
                    for p in split_top(m.group(2)):
                        ids = re.findall(r'[A-Za-z_]\w*', p)
                        if ids:
                            params.append(ids[-1])
                    yield m.group(1), params, start, j
                    i = j + 1
                    anchor = i
                    continue
            depth += 1
        elif ch == '}':
            depth -= 1
            if depth <= 0:
                depth = 0
                anchor = i + 1
        elif ch == ';' and depth == 0:
            anchor = i + 1
        i += 1


def statements(body):
    """Split a function body into statement texts with their start offsets."""
    out, cur, start, depth = [], [], 0, 0
    for i, ch in enumerate(body):
        if ch in '([':
            depth += 1
        elif ch in ')]':
            depth -= 1
        cur.append(ch)
        if depth == 0 and ch in ';{}':
            out.append((start, ''.join(cur)))
            cur, start = [], i + 1
    if cur:
        out.append((start, ''.join(cur)))
    return out


# --------------------------------------------------------------------------
# 5. type inference from evidence
# --------------------------------------------------------------------------
def call_sites(text, name):
    """Yield (func, argindex, argtext) for every call that passes `name`."""
    for m in re.finditer(r'([A-Za-z_]\w*)\s*\(', text):
        fn, o = m.group(1), m.end() - 1
        depth, j = 0, o
        while j < len(text):
            if text[j] == '(':
                depth += 1
            elif text[j] == ')':
                depth -= 1
                if depth == 0:
                    break
            j += 1
        args = split_top(text[o + 1:j])
        for k, a in enumerate(args):
            if re.search(r'(?<![\w.])&?\s*%s\b' % re.escape(name), a):
                yield fn, k, a.strip()


def infer(name, ev_text, protos):
    """-> (type_string, confidence, why)"""
    # assigned the result of a known API — the strongest evidence there is, and
    # it must be tested FIRST: `q = AnsiNext(q)` makes q an LPSTR, not the
    # caller-allocated buffer the parameter rule below would suggest.
    m = re.search(r'(?<![\w])%s\s*=\s*(?:\([^)]*\)\s*)?([A-Za-z_]\w*)\s*\('
                  % re.escape(name), ev_text)
    if m and m.group(1) in protos:
        return (protos[m.group(1)][0], "HIGH",
                "assigned from %s(), which returns %s"
                % (m.group(1), protos[m.group(1)][0]))
    # used with member selection -> an aggregate; name the members seen
    mem = sorted(set(re.findall(r'(?<![\w])%s\s*(?:\.|->)\s*([A-Za-z_]\w*)'
                                % re.escape(name), ev_text)))
    if mem:
        cast = re.search(r'=\s*\(\s*([A-Za-z_]\w*)\s+FAR\s*\*\s*\)', ev_text)
        if cast:
            return (cast.group(1), "MED",
                    "aggregate: members .%s used; cast to %s FAR* at the same site"
                    % (", .".join(mem), cast.group(1)))
        return ("struct { /* members: .%s */ }" % ", .".join(mem), "LOW",
                "aggregate: members .%s used; no type name in view"
                % ", .".join(mem))
    # array use with an explicit size argument in a text API
    for fn, k, a in call_sites(ev_text, name):
        if fn in protos and k < len(protos[fn][1]):
            ptype = protos[fn][1][k]
            if ptype in ('LPSTR', 'LPCSTR') and a.strip() == name:
                # look for a following size argument in the same call
                m = re.search(re.escape(fn) + r'\s*\(([^;]*)\)', ev_text)
                if m:
                    args = split_top(m.group(1))
                    if k + 1 < len(args):
                        sz = args[k + 1].strip()
                        if re.match(r'^(0x[0-9a-fA-F]+|\d+)$', sz):
                            return ("char %s[%s]" % (name, sz), "HIGH",
                                    "%s() arg %d is LPSTR with size %s"
                                    % (fn, k + 1, sz))
                return ("char %s[128]" % name, "MED",
                        "%s() arg %d is LPSTR (no size argument in view)"
                        % (fn, k + 1))
    # passed as a typed parameter
    for fn, k, a in call_sites(ev_text, name):
        if fn in protos and k < len(protos[fn][1]):
            t = protos[fn][1][k]
            if a.startswith('&'):
                return (t.replace('LPSTR', 'char') + ' *', "MED",
                        "address passed to %s() arg %d (%s)" % (fn, k + 1, t))
            return (t, "HIGH", "passed to %s() arg %d, declared %s"
                    % (fn, k + 1, t))
    # indexed -> array of unknown extent
    if re.search(r'(?<![\w.])%s\s*\[' % re.escape(name), ev_text):
        return ("char %s[/* extent NOT recovered */]" % name, "LOW",
                "used as an array; no size evidence")
    # compared/assigned a plain number
    if re.search(r'(?<![\w.])%s\s*[=!<>]=?\s*(0x[0-9a-fA-F]+|\d+)'
                 % re.escape(name), ev_text):
        return ("WORD", "LOW", "only numeric assignment/compare — width UNVERIFIED")
    return ("WORD", "LOW", "no type evidence in the recovered code — AMBIGUOUS")


# --------------------------------------------------------------------------
# 6. driver
# --------------------------------------------------------------------------
def analyse(path, known, protos):
    raw = open(path, encoding='utf-8', errors='replace').read()
    lines = raw.splitlines()
    text = blank_aggregates(strip_directives(blank(raw)))
    # file-scope names + local #defines
    fglobals = set(re.findall(r'(?m)^\s*#\s*define\s+([A-Za-z_]\w*)', raw))
    typenames = set(n for n in known if re.match(r'^[A-Z][A-Za-z0-9_]*$', n))
    funcs = list(functions(text))
    spans = [(s, e) for _, _, s, e in funcs]

    def in_func(off):
        return any(s <= off < e for s, e in spans)

    # Two passes over file scope: the app's OWN typedefs first (a global
    # declared `static CARDHEADER CurCardHead;` is only parseable once
    # CARDHEADER is known to be a type), then the declarations themselves.
    stmts = list(file_scope_statements(text))
    for stmt in stmts:
        if stmt.startswith('typedef'):
            typenames.update(declared_names(stmt))
    for stmt in stmts:
        if not stmt.startswith('typedef'):
            fglobals.update(declared_names(stmt, typenames))
    for name, _, _, _ in funcs:
        fglobals.add(name)

    def lineno(off):
        return text.count('\n', 0, off) + 1

    report = []
    for fname, params, s, e in funcs:
        body = text[s:e + 1]
        declared = set(params)
        # inline aggregate locals — `struct { ... } d;` — are picked up here
        # because the statement splitter treats braces as terminators.
        for m in re.finditer(r'\b(?:struct|union|enum)\b\s*\w*\s*\{[^{}]*\}'
                             r'\s*([^;{}]*);', body):
            for part in split_top(m.group(1)):
                ids = re.findall(r'[A-Za-z_]\w*', part.split('=')[0])
                if ids:
                    declared.add(ids[-1])
        used = {}       # name -> list of (offset, statement)
        for off, stmt in statements(body):
            names = declared_names(stmt, typenames)
            if names:
                declared.update(names)
                continue
            # blank a leading `label:` (a goto target defines it, it is not a
            # use of a variable). Length-preserving so offsets stay exact.
            # A statement can carry several: `default:` and `deflt:` both label
            # the same return, so strip them in a loop. `case X:` stops it —
            # X is a constant that must still be checked.
            # `case 0x68:` is stepped OVER (its constant still has to be
            # checked) so that a goto label sitting under it — the real shape
            # in cardfile — is still recognised as a label.
            pos = 0
            while True:
                m = re.compile(r'\s*case\b[^:]*:(?!:)').match(stmt, pos)
                if m:
                    pos = m.end(); continue
                m = re.compile(r'\s*([A-Za-z_]\w*)(\s*:(?!:))').match(stmt, pos)
                if not m or m.group(1) == 'default':
                    if m:                      # `default:` — step over it too
                        pos = m.end(); continue
                    break
                seg = ''.join(' ' if c != '\n' else '\n'
                              for c in stmt[m.start(1):m.end(2)])
                stmt = stmt[:m.start(1)] + seg + stmt[m.end(2):]
                pos = m.end(2)
            # blank goto targets (same length, so offsets stay exact)
            stmt = re.sub(r'(\bgoto\s+)([A-Za-z_]\w*)',
                          lambda m: m.group(1) + ' ' * len(m.group(2)), stmt)
            # (?<![\w]) keeps the `x` of 0x0111 and the `L` of 0L out of the
            # identifier stream — they are parts of numeric literals.
            for m in re.finditer(r'(?<![\w])[A-Za-z_]\w*', stmt):
                nm = m.group(0)
                pre = stmt[:m.start()].rstrip()
                post = stmt[m.end():].lstrip()
                if pre.endswith('.') or pre.endswith('->'):
                    continue
                if post.startswith('('):
                    continue                              # a call
                if nm in KEYWORDS or nm in known or nm in fglobals:
                    continue
                if nm in typenames:
                    continue
                used.setdefault(nm, []).append((s + off + m.start(), stmt))
        missing = {n: v for n, v in used.items() if n not in declared}
        if not missing:
            continue
        entries = []
        for nm, hits in sorted(missing.items(), key=lambda kv: -len(kv[1])):
            ev = ' '.join(h[1] for h in hits)
            t, conf, why = infer(nm, ev, protos)
            entries.append({
                "name": nm, "type": t, "confidence": conf, "why": why,
                "count": len(hits),
                "lines": [lineno(o) for o, _ in hits][:6],
            })
        report.append({"function": fname, "line": lineno(s),
                       "undeclared": entries})
    # scope hint: a name missing from more than one function, or named by the
    # ds_0xNNN convention, belongs at file scope, not in a single body.
    seen = {}
    for f in report:
        for e in f["undeclared"]:
            seen.setdefault(e["name"], []).append(f["function"])
    for f in report:
        for e in f["undeclared"]:
            fns = seen[e["name"]]
            e["scope"] = ("global" if len(fns) > 1 or
                          re.match(r'^ds_0x[0-9a-f]+', e["name"]) else "local")
            e["in_functions"] = fns
    return report, lines


def main():
    argv = sys.argv[1:]
    jsonout = None
    if '--json' in argv:
        i = argv.index('--json')
        if i + 1 < len(argv) and not argv[i + 1].startswith('-'):
            jsonout = argv[i + 1]
            del argv[i + 1]                      # not an input file
        else:
            jsonout = "undecl.json"
    args = [a for a in argv if not a.startswith('--')]
    flags = [a for a in argv if a.startswith('--')]
    if not args:
        print(__doc__); return 1
    known, protos = scan_header(WINH)
    if not known:
        print("!! could not read %s — no header whitelist" % WINH)
    total = 0
    allrep = {}
    for path in args:
        rep, lines = analyse(path, known, protos)
        allrep[os.path.basename(path)] = rep
        cnt = sum(len(f["undeclared"]) for f in rep)
        total += cnt
        if '--summary' in flags:
            print("%-16s %3d undeclared in %d functions"
                  % (os.path.basename(path), cnt, len(rep)))
            continue
        print("=" * 78)
        print("%s — %d undeclared identifier(s) across %d function(s)"
              % (path, cnt, len(rep)))
        print("=" * 78)
        for f in rep:
            print("\n%s()   (line %d)" % (f["function"], f["line"]))
            for e in f["undeclared"]:
                print("    %-14s %-28s [%s/%s]  x%d  lines %s"
                      % (e["name"], e["type"], e["confidence"],
                         e["scope"], e["count"],
                         ",".join(str(x) for x in e["lines"])))
                if len(e["in_functions"]) > 1:
                    print("        also missing in: %s"
                          % ", ".join(x for x in e["in_functions"]
                                      if x != f["function"]))
                print("        evidence: %s" % e["why"])
                for ln in e["lines"][:2]:
                    if ln - 1 < len(lines):
                        print("        %5d | %s" % (ln, lines[ln - 1].strip()[:88]))
    if jsonout:
        with open(jsonout, "w") as fh:
            json.dump(allrep, fh, indent=1)
        print("\n(json -> %s)" % jsonout)
    print("\nTOTAL: %d undeclared identifiers" % total)
    return 0


if __name__ == "__main__":
    sys.exit(main())
