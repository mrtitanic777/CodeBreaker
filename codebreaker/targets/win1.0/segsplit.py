#!/usr/bin/env python3
"""
segsplit.py — split a merged app reconstruction into its original per-segment
modules, the way Microsoft actually shipped it.

Every Windows 1.0 app was a multi-CODE-segment NE: CALENDAR has 9 code
segments, CONTROL 9, PAINT 32. Our reconstruction merges each app into one big
.c. That is (a) unfaithful to the binary's structure and (b) more than the 1985
compiler's front end can digest in one pass ("Stack overflow" on the large
modules). Splitting back into segment modules fixes both: each function already
carries the segment it was recovered from (segN:0xoffset, in its banner), so the
partition is read from the evidence, not invented.

OUTPUT (for APP.c -> build dir):
    APP.H         includes + macros + typedefs + structs + all prototypes
                  (non-static) + `extern` for every file-scope global
    APP_G.C       the one definition of each global (static dropped)
    APP_S<n>.C    the functions recovered from code segment <n>
Each APP_S<n>.C and APP_G.C #includes APP.H. Functions lose `static` so a
near/far call from another segment module resolves at link — which is exactly
what a cross-segment call was in the original NE.

A function's home segment is the FIRST `segN:` tag in its banner+header. A
function with no tag goes to APP_S0.C (reported, not guessed).

This runs on the ANSI source (a function body's `{` still follows `)`), BEFORE
ansi2knr; the caller then runs ansi2knr on each emitted piece.

Usage:  python segsplit.py reconstructed/calendar.c OUTDIR [--stem calendar]
"""
import os, re, sys


def scan_top_level(src):
    """Yield (kind, start, end) for each top-level unit, comment/string aware.
      kind='func'  a function definition (header + body)
      kind='other' anything else at depth 0 (decl, global, typedef, struct,
                   prototype, #directive, stray text)
    A unit's text is src[start:end]. 'other' runs are split at the ';' or '}'
    that ends them so each declaration is its own unit; 'func' spans header..'}'.
    """
    i, n = 0, len(src)
    depth = 0
    unit_start = 0
    # position of the last depth-0 '(' ... ')' close, to tell func `){` apart
    while i < n:
        c = src[i]
        # comments
        if c == '/' and i + 1 < n and src[i+1] == '*':
            j = src.find('*/', i + 2)
            i = (j + 2) if j >= 0 else n
            continue
        if c == '/' and i + 1 < n and src[i+1] == '/':
            j = src.find('\n', i)
            i = (j) if j >= 0 else n
            continue
        # strings / chars
        if c == '"' or c == "'":
            q = c
            i += 1
            while i < n:
                if src[i] == '\\':
                    i += 2; continue
                if src[i] == q:
                    i += 1; break
                i += 1
            continue
        if c == '#' and (i == 0 or src[i-1] == '\n'):
            # a preprocessor line — one 'other' unit. A line ending in `\`
            # continues onto the next, so a multi-line #define (WRITE's
            # WRITE_DIALOG_BODY) is ONE unit; taking only the first line left
            # the macro's body standing as bare code in the emitted module.
            j = src.find('\n', i)
            while j >= 0 and src[:j].rstrip('\r').endswith('\\'):
                j = src.find('\n', j + 1)
            end = (j + 1) if j >= 0 else n
            # A directive can trail a comment that runs past its line —
            # `#include <winexp.h>   /* why ...` continued below. Consuming
            # only the first line left the scanner resuming INSIDE that
            # comment, where every following `{` was misread and whole
            # functions ended up in the shared header instead of a module.
            seg_txt = src[i:end]
            op = seg_txt.rfind('/*')
            if op >= 0 and seg_txt.find('*/', op) < 0:
                close = src.find('*/', i + op + 2)
                if close >= 0:
                    end = close + 2
                    nl = src.find('\n', end)
                    end = (nl + 1) if nl >= 0 else len(src)
            if src[unit_start:i].strip():
                yield ('other', unit_start, i)
            yield ('other', i, end)
            unit_start = end
            i = end
            continue
        if c == '{':
            if depth == 0:
                # decide: function body vs initializer/struct/enum/union
                head = src[unit_start:i]
                if _is_func_head(head):
                    end = _match_brace(src, i)
                    yield ('func', unit_start, end)
                    unit_start = end
                    i = end
                    continue
                else:
                    # a brace that belongs to a declaration (struct/enum/init);
                    # skip to its matching close, stay in the same 'other' unit
                    i = _match_brace(src, i)
                    continue
            depth += 1
            i += 1
            continue
        if c == '}':
            depth -= 1
            i += 1
            continue
        if c == ';' and depth == 0:
            yield ('other', unit_start, i + 1)
            unit_start = i + 1
            i += 1
            continue
        i += 1
    if src[unit_start:].strip():
        yield ('other', unit_start, n)


def _match_brace(src, i):
    """Given src[i]=='{', return index just past the matching '}'. Comment /
    string aware."""
    depth = 0
    n = len(src)
    while i < n:
        c = src[i]
        if c == '/' and i + 1 < n and src[i+1] == '*':
            j = src.find('*/', i + 2); i = (j + 2) if j >= 0 else n; continue
        if c == '/' and i + 1 < n and src[i+1] == '/':
            j = src.find('\n', i); i = j if j >= 0 else n; continue
        if c == '"' or c == "'":
            q = c; i += 1
            while i < n:
                if src[i] == '\\': i += 2; continue
                if src[i] == q: i += 1; break
                i += 1
            continue
        if c == '{': depth += 1
        elif c == '}':
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    return n


def _strip_cmts(s):
    s = re.sub(r"/\*.*?\*/", " ", s, flags=re.S)
    s = re.sub(r"//[^\n]*", " ", s)
    return s


def _is_func_head(head):
    """Does the text just before a depth-0 '{' look like a function header?
    ANSI form: ends (ignoring comments/space) with ')'. Excludes '=' (init),
    struct/enum/union tags, and typedef."""
    h = _strip_cmts(head).rstrip()
    if not h.endswith(')'):
        return False
    # must contain a name '(' somewhere and not be a control keyword
    if re.search(r"\b(if|for|while|switch)\s*\($", h):
        return False
    return bool(re.search(r"[A-Za-z_]\w*\s*\(", h))


SEGTAG = re.compile(r"seg(\d+):0x", re.I)


# A top-level unit begins right after the PREVIOUS unit's ';', so it normally
# opens with that declaration's trailing comment and/or this one's banner. An
# anchored /^\s*static/ therefore never fires — which silently left `static` on
# every global and every function, so nothing could be referenced across the
# segment modules. Skip leading comments before looking for the keyword.
def _chunk_suffix(ci, total):
    """Chunk suffix: '' for a lone chunk, else A..Z then AA..ZZ (Excel-column
    style). The old chr('A'+ci) overflowed past 'Z' into punctuation — ci=27
    gave a backslash, and `SPL1\\.C` is not a filename — once a segment chunked
    into more than 26 pieces (spooler seg1 at small --maxbytes). Single-letter
    for the first 26, so small apps are unchanged."""
    if total == 1:
        return ""
    s, n = "", ci
    while True:
        s = chr(ord('A') + n % 26) + s
        n = n // 26 - 1
        if n < 0:
            return s


def _split_commas(s):
    """Split on commas at bracket/paren depth 0 (declarator lists)."""
    parts, depth, cur = [], 0, []
    for ch in s:
        if ch in "([":
            depth += 1
        elif ch in ")]":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append("".join(cur)); cur = []
        else:
            cur.append(ch)
    parts.append("".join(cur))
    return parts


def drop_static(text):
    """Remove a leading `static` storage class, keeping any comments above it.

    Scanned rather than matched: the obvious regex for "whitespace and comments,
    repeated" backtracks catastrophically on a unit that opens with a large
    banner and no `static` (clipbrd.c hung segsplit outright). This is linear.
    """
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c.isspace():
            i += 1
        elif c == '/' and i + 1 < n and text[i+1] == '*':
            j = text.find('*/', i + 2)
            i = n if j < 0 else j + 2
        elif c == '/' and i + 1 < n and text[i+1] == '/':
            j = text.find('\n', i)
            i = n if j < 0 else j + 1
        else:
            break
    if text.startswith('static', i):
        j = i + len('static')
        if j >= n or not (text[j].isalnum() or text[j] == '_'):
            while j < n and text[j] in ' \t':
                j += 1
            return text[:i] + text[j:]
    return text


def seg_of(text):
    m = SEGTAG.search(text)
    return int(m.group(1)) if m else None


def func_name_of(functext):
    b = _strip_cmts(functext)
    m = re.search(r"([A-Za-z_][\w \t\*]*?)\b([A-Za-z_]\w*)\s*\(", b)
    return m.group(2) if m else None


def _fn_already_far(quals):
    """Does this qualifier run already make the FUNCTION far (or PASCAL)?

    Only FAR/PASCAL AFTER the last `*` qualifies the function — a FAR before or
    among the pointer stars binds to the RETURN TYPE. `char FAR *StrBrk` is a
    NEAR function returning a far pointer, so it is NOT already far, and a
    cross-segment call to it emitted a near "Intersegment self-relative fixup".
    """
    tail = quals.rsplit("*", 1)[-1]        # after the last pointer star
    return bool(re.search(r"\b(FAR|far|PASCAL|pascal)\b", tail))


def prototype_of(functext, far=False):
    """Build a forward prototype (name + return type + FAR/PASCAL, empty K&R
    param list) from a function definition, so a caller in another segment
    module links with the right calling convention. Returns (name, proto).

    `far` adds FAR when the function is CALLED FROM ANOTHER SEGMENT. That is not
    decoration: a near call cannot reach another segment, and the 1985 linker
    reports it ("Intersegment self-relative fixup"). Microsoft's own CARDFILE
    header declares exactly these functions FAR — `int FAR WriteCardFile();` —
    so this reproduces the period convention rather than inventing one.
    """
    b = _strip_cmts(functext)
    m = re.search(r"([A-Za-z_][\w \t\*]*?)\b([A-Za-z_]\w*)\s*\(", b)
    if not m:
        return None
    quals = re.sub(r"^\s*(static|extern)\b\s*", "", m.group(1)).strip()
    quals = " ".join(quals.split())
    name = m.group(2)
    if not quals:
        return None
    if far and not _fn_already_far(quals):
        quals += " FAR"                    # after the last *, so it is the
                                           # FUNCTION that is far, not the return
    return (name, "%s %s();" % (quals, name))


def make_far(functext):
    """Insert FAR into a function DEFINITION's header (idempotent)."""
    b = _strip_cmts(functext)
    m = re.search(r"([A-Za-z_][\w \t\*]*?)\b([A-Za-z_]\w*)\s*\(", b)
    if not m or _fn_already_far(m.group(1)):
        return functext
    # locate the same name in the real text (comments intact) and put FAR ahead
    name = m.group(2)
    hit = re.search(r"(?<![\w])" + re.escape(name) + r"\s*\(", functext)
    if not hit:
        return functext
    return functext[:hit.start()] + "FAR " + functext[hit.start():]


def called_names(functext):
    """Every identifier used as a call target in a function body."""
    b = _strip_cmts(functext)
    return set(re.findall(r"(?<![\w.])([A-Za-z_]\w*)\s*\(", b))


def _extern_decl(defn):
    """Turn a global-variable DEFINITION into an `extern` declaration:
    drop leading static/near/far, drop any initializer, keep array bounds."""
    d = _strip_cmts(defn).strip()
    d = re.sub(r"=\s*.*$", "", d, flags=re.S).strip()      # drop initializer
    d = d.rstrip(";").strip()
    d = re.sub(r"^\s*(static|extern)\b\s*", "", d)
    return "extern " + d + ";"


def _is_prototype(other):
    o = _strip_cmts(other)
    return "(" in o and ";" in o and "=" not in o.split("(")[0]


def _is_global_var(other):
    o = _strip_cmts(other).strip()
    if not o.endswith(";"):
        return False
    if "(" in o.split("=")[0]:        # a prototype
        return False
    if re.match(r"^\s*(typedef|struct|union|enum)\b", o):
        return False
    return bool(re.match(r"^\s*(static\s+)?[A-Za-z_]", o))


def split(src, stem, segmap=None):
    units = list(scan_top_level(src))
    header_bits, global_defs, externs = [], [], []
    pre_defines = []
    saw_windows_h = False
    saw_winexp = False
    seg_funcs = {}
    untagged = []
    gen_protos = []
    proto_names = set()
    first_func_seen = False
    for kind, a, b in units:
        text = src[a:b]
        if kind == 'func':
            first_func_seen = True
            s = seg_of(text)
            if s is None and segmap:
                # inline segN: banners were stripped from the source; recover the
                # home segment from the sidecar map (built by gen_segmap.py from
                # the pre-strip source — exactly what seg_of returned before).
                s = segmap.get(func_name_of(text))
            if s is None:
                untagged.append(text)
                seg_funcs.setdefault(0, []).append(text)
            else:
                seg_funcs.setdefault(s, []).append(text)
            continue
        # 'other'
        stripped = _strip_cmts(text).strip()
        if not stripped:
            header_bits.append(text)               # keep comments/banners
            continue
        if stripped.startswith('#'):
            # the 1985 windows.h has NO include guard; each emitted module
            # includes it once from its own template, so drop it here to avoid
            # a double inclusion (which overflows the C 3.0 front end).
            if re.match(r"#\s*include\s*[<\"]windows\.(h|inc)[>\"]", stripped, re.I):
                saw_windows_h = True
                continue
            # <winexp.h> gets the same treatment: emitted once per module in
            # the prelude, never parked in the shared header. It must follow
            # windows.h (it uses its types), and if two merged sources each
            # carry it the shared header would declare every lstr*/_l* twice.
            if re.match(r"#\s*include\s*[<\"]winexp\.h[>\"]", stripped, re.I):
                saw_winexp = True
                continue
            # A #define that PRECEDES <windows.h> is switching the header on:
            # CONTROL's `#define OEMRESOURCE` is what makes OBM_UPARROW exist.
            # It has to be re-emitted ahead of the include in every module, not
            # buried in the shared header, which is included after it.
            if not saw_windows_h and re.match(r"#\s*define\b", stripped):
                pre_defines.append(stripped)
                continue
            header_bits.append(text)
            continue
        if _is_global_var(text):
            global_defs.append(text)
            externs.append(_extern_decl(text))
            continue
        if _is_prototype(text):
            # drop leading `static` so the shared prototype is external
            header_bits.append(drop_static(text))
            continue
        # typedef / struct / enum / macro-ish — shared as-is
        header_bits.append(text)
    # ---- FAR-ness, derived from the call graph -------------------------
    # A call that crosses a segment boundary cannot be near. Which functions
    # those are is not a choice: it is read off the partition we just made.
    name_seg = {}
    for seg, fns in seg_funcs.items():
        for fn in fns:
            nm = func_name_of(fn)
            if nm:
                name_seg[nm] = seg
    far_names = set()
    for seg, fns in seg_funcs.items():
        for fn in fns:
            for callee in called_names(fn):
                if callee in name_seg and name_seg[callee] != seg:
                    far_names.add(callee)
    # A segment that had to be CHUNKED across several OBJs is still one segment
    # at run time, so calls between its chunks are near and correct. But a
    # function reached from a chunk the linker places in a different physical
    # segment is not — and which chunk lands where is the linker's business,
    # not ours. Microsoft resolved this by declaring every shared entry point
    # FAR (INDEX.H: `int FAR WriteCardFile();`), so any function called from a
    # module other than the one defining it is treated the same way here.
    mod_of = {}
    for seg, fns in seg_funcs.items():
        for i, fn in enumerate(fns):
            nm = func_name_of(fn)
            if nm:
                mod_of[nm] = (seg, id(fns[i]))
    for seg, fns in seg_funcs.items():
        for fn in fns:
            me = func_name_of(fn)
            for callee in called_names(fn):
                if callee in name_seg and callee != me and                         mod_of.get(callee, (None, None))[1] != id(fn):
                    far_names.add(callee)
    for seg, fns in seg_funcs.items():
        for i, fn in enumerate(fns):
            if func_name_of(fn) in far_names:
                fns[i] = make_far(fn)
    for seg in sorted(seg_funcs):
        for fn in seg_funcs[seg]:
            pr = prototype_of(fn, far=func_name_of(fn) in far_names)
            if pr and pr[0] not in proto_names:
                proto_names.add(pr[0])
                gen_protos.append(pr[1])
    # The app's OWN forward declarations were carried into header_bits (the
    # _is_prototype branch) as-is — and a near one there SUPPRESSES the FAR
    # prototype generated above (proto_names / `existing` skips a name already
    # declared). So a cross-segment callee whose app-declared prototype is near
    # is called near while its definition is far: exactly the linker's
    # "Intersegment self-relative fixup". FAR-ify those prototypes here so the
    # single surviving declaration matches the definition.
    for i, t in enumerate(header_bits):
        if _is_prototype(t):
            nm = func_name_of(t)
            if nm in far_names:
                header_bits[i] = make_far(t)
    if saw_winexp:
        pre_defines.append("__WINEXP__")     # marker, turned into the include
        # NOATOM trims BOTH winexp.h's atom typedefs AND windows.h's atom API
        # (they share the #ifndef). That is safe — and worth ~40 symbols off the
        # C 3.0 front-end table, which is what tips CONTROL's big modules into
        # "Stack overflow" — ONLY if the app never calls the atom API. Detect
        # that from the source; PAINT uses GetAtomName, so it keeps the full
        # header (and gets its size relief from smaller chunks instead).
        ATOM_API = re.compile(r"\b(GetAtomName|AddAtom|FindAtom|DeleteAtom|"
                              r"GetAtomHandle|InitAtomTable|MAKEINTATOM)\b")
        if not ATOM_API.search(_strip_cmts(src)):
            pre_defines.append("__NOATOM__")
    # Trim windows.h sections the app never uses, to keep the C 3.0 P1
    # symbol/macro table under its limit as an app fills out. Each is emitted
    # ONLY when the source references nothing from that section, so it can never
    # remove a declaration the code needs (same principle as NOATOM above).
    _stripped = _strip_cmts(src)
    SAFE_NO = [
        ("NODRAWTEXT",    r"\bDrawText\b|\bGrayString\b|\bDT_[A-Z]"),
        ("NOMETAFILE",    r"\bMetaFile\b|\bHMETAFILE\b"),
        ("NOWH",          r"\bSetWindowsHook\b|\bUnhookWindowsHook\b|\bWH_[A-Z]"),
        ("NONCMESSAGES",  r"\bWM_NC[A-Z]"),
        ("NOSYSMETRICS",  r"\bGetSystemMetrics\b|\bSM_C[A-Z]"),
        ("NOGDICAPMASKS", r"\bRC_[A-Z]"),
        ("NOKANJI",       r"\bIsDBCSLeadByte\b|\bAnsiNextKanji\b"),
        ("NOSOUND",       r"\bOpenSound\b|\bStartSound\b|\bCloseSound\b|\bSetVoice"),
    ]
    for _name, _pat in SAFE_NO:
        if not re.search(_pat, _stripped):
            pre_defines.append("#define " + _name)
    return (header_bits, global_defs, externs, seg_funcs, untagged,
            gen_protos, pre_defines)


def emit(outdir, stem, header_bits, global_defs, externs, seg_funcs, untagged,
         gen_protos=(), maxbytes=6000, pre_defines=(), seg_maxbytes=None):
    seg_maxbytes = seg_maxbytes or {}
    os.makedirs(outdir, exist_ok=True)
    U = stem.upper()
    # DOS 8.3: the longest basename is <U><maxseg>, which must be <= 8 chars.
    longest = len(U) + max([len(str(s)) for s in seg_funcs] + [1])
    if longest > 8:
        raise SystemExit("stem %r too long: <stem><seg> would be %d chars (>8). "
                         "Pass a shorter --stem." % (U, longest))
    control_defs = [d for d in pre_defines if not d.startswith("__")]
    want_winexp = "__WINEXP__" in pre_defines
    want_noatom = "__NOATOM__" in pre_defines
    prelude = "".join(d + "\n" for d in control_defs)
    # NOATOM (set in split() only for apps that never call the atom API) trims
    # winexp.h's atom typedefs AND windows.h's atom block — ~40 fewer symbols in
    # the C 3.0 front-end table, which is what clears CONTROL's "Stack overflow".
    # It is NOT set for PAINT, which uses GetAtomName: disabling it there removed
    # the declaration and the call silently compiled C-convention (unresolved).
    if want_noatom:
        prelude += "#define NOATOM\n"
    prelude += "#include <windows.h>\n"
    if want_winexp:
        prelude += "#include <winexp.h>\n"
    prelude += '#include "%s.h"\n\n' % U

    # Pre-compute the code chunking NOW (the same _chunk_by_size the module
    # writer uses below), so the header can be built from actual module
    # membership. This is what lets us declare ONLY the functions that are
    # really called across a module boundary — the rest never need a global
    # prototype, and emitting one for all 54 of CONTROL's functions overflowed
    # the C 3.0 front end's symbol table on every module. Microsoft's own source
    # didn't declare every function globally either, so this is more faithful,
    # not just smaller.
    seg_chunks = {}
    defmod = {}                       # function name -> the module that defines it
    for seg in sorted(seg_funcs):
        chunks = _chunk_by_size(seg_funcs[seg], seg_maxbytes.get(seg, maxbytes))
        placed = []
        for ci, chunk in enumerate(chunks):
            suffix = _chunk_suffix(ci, len(chunks))
            base = "%s%d%s" % (U, seg, suffix)
            placed.append((base, chunk))
            for fn in chunk:
                nm = func_name_of(fn)
                if nm:
                    defmod[nm] = base
        seg_chunks[seg] = placed
    # who references each function, and from which module. A function used by
    # ADDRESS (as a callback: `MakeProcInstance(About,..)`, `wc.lpfnWndProc =
    # ClockWndProc`) is not a call() but still needs a prototype in the module
    # that references it — otherwise it is an undefined identifier there. So we
    # look for EVERY occurrence of a defined function's NAME, not just call
    # syntax. (called_names alone missed the callbacks and broke clock.)
    allfns = set(defmod)
    callers = {}
    for seg, placed in seg_chunks.items():
        for base, chunk in placed:
            body = _strip_cmts("".join(chunk))
            present = set(re.findall(r"(?<![\w.])([A-Za-z_]\w*)", body))
            for nm in present & allfns:
                callers.setdefault(nm, set()).add(base)
    # a function needs a shared prototype iff it is referenced from a module
    # OTHER than the one defining it (cross-module use — call OR address-of,
    # which includes every cross-segment reference). Same-module uses resolve
    # locally (via the per-module forward declarations emitted below).
    need_proto = {f for f, cs in callers.items()
                  if f in defmod and any(c != defmod[f] for c in cs)}

    # The SAME reduction for GLOBALS. CONTROL's shared header declared all 86
    # externs; only 11 are referenced from more than one module. The other 75
    # bloated every module's C 3.0 symbol table (which is what "Stack overflow"
    # is). A global used by a single module gets its extern THERE instead; only
    # cross-module globals stay in the shared header. Map each extern to its
    # name, then see which modules reference it.
    # Expand a multi-declarator extern (`extern HWND hsbRed, hsbGreen, hsbBlue;`)
    # into one extern PER NAME, so each is placed by its own reference set. A
    # single regex over the joined form caught only the last name, leaving the
    # others declared nowhere -> "undefined". Definitions in the globals module
    # keep their combined form; only these declarations are split.
    single_externs = []               # (name, "extern TYPE decl;")
    for e in externs:
        s2 = _strip_cmts(e).strip().rstrip(";")
        s2 = re.sub(r"^\s*extern\s+", "", s2)
        segs = _split_commas(s2)
        first = segs[0].strip()
        # the first comma-segment holds TYPE + first declarator; peel the
        # trailing declarator ("[*]name[array]") off, the rest is the type.
        dm = re.search(r"((?:\*\s*)*[A-Za-z_]\w*\s*(?:\[[^\]]*\])?)\s*$", first)
        if not dm:
            continue
        typ = first[:dm.start(1)].strip()
        decls = [dm.group(1).strip()] + [s.strip() for s in segs[1:]]
        for decl in decls:
            nm = re.search(r"([A-Za-z_]\w*)", decl.replace("*", " "))
            if nm and typ:
                single_externs.append((nm.group(1),
                                       "extern %s %s;" % (typ, decl)))
    ext_name = {txt: nm for nm, txt in single_externs}
    gref = {}                         # global name -> set of modules referencing it
    for seg, placed in seg_chunks.items():
        for base, chunk in placed:
            body = _strip_cmts("".join(chunk))
            present = set(re.findall(r"(?<![\w.])([A-Za-z_]\w*)", body))
            for e, nm in ext_name.items():
                if nm in present:
                    gref.setdefault(nm, set()).add(base)
    xmod_globals = {nm for nm, ms in gref.items() if len(ms) > 1}
    # externs to emit in the shared header (cross-module) vs per-module (local).
    # Work over the per-NAME single externs so a multi-name declaration can be
    # split across the header and a module as its usage requires.
    header_externs = [txt for nm, txt in single_externs if nm in xmod_globals]
    local_externs = {}                # module base -> [extern text]
    for nm, txt in single_externs:
        if nm not in xmod_globals and nm in gref:
            for base in gref[nm]:
                local_externs.setdefault(base, []).append(txt)

    hpath = os.path.join(outdir, U + ".H")
    with open(hpath, "w", encoding="utf-8") as f:
        f.write("/* %s.h  --  shared declarations for the %s segment modules.\n"
                " * Generated by segsplit.py: prototypes + extern globals so the\n"
                " * app's code segments compile and link as separate modules,\n"
                " * matching the original multi-segment NE. Include <windows.h>\n"
                " * BEFORE this header. */\n"
                "#ifndef %s_H\n#define %s_H\n\n" % (U, stem, U, U))
        for t in header_bits:
            f.write(t)
        # prototype ONLY for functions called across a module boundary, so a
        # caller in another module links with the right (near/FAR PASCAL)
        # calling convention. Skip names already prototyped in the preamble.
        existing = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(",
                                  _strip_cmts("".join(header_bits))))
        def _pname(p):
            m = re.search(r"([A-Za-z_]\w*)\s*\(\s*\)\s*;", p)
            return m.group(1) if m else None
        gp = [p for p in gen_protos
              if _pname(p) and _pname(p) in need_proto and _pname(p) not in existing]
        if gp:
            f.write("\n\n/* ---- prototypes for cross-module-called functions "
                    "(%d of %d) ---- */\n" % (len(gp), len(defmod)))
            for p in gp:
                f.write(p + "\n")
        f.write("\n\n/* ---- cross-module file-scope globals (%d of %d; the rest\n"
                " *      are externed locally in the one module that uses them) ---- */\n"
                % (len(header_externs), len(externs)))
        for e in header_externs:
            f.write(e + "\n")
        f.write("\n#endif\n")
    # globals module(s). Chunked exactly like the code segments: the C 3.0
    # front end runs out of table space on the shared header's symbols PLUS a
    # few hundred definitions in one file — which is all "Stack overflow" means
    # here. Splitting the DATA definitions across OBJs is as faithful as
    # splitting the code: they all land in the one automatic data segment.
    made_g = []
    gchunks = _chunk_by_size(global_defs, seg_maxbytes.get('G', maxbytes))
    for gi, gchunk in enumerate(gchunks):
        gsuffix = _chunk_suffix(gi, len(gchunks))
        gbase = U + "G" + gsuffix
        if len(gbase) > 8:
            raise SystemExit("name %r exceeds 8.3; use a shorter stem" % gbase)
        gpath = os.path.join(outdir, gbase + ".C")
        with open(gpath, "w", encoding="utf-8") as f:
            f.write(prelude)
            part = "" if len(gchunks) == 1 else (" (part %d/%d)"
                                                 % (gi + 1, len(gchunks)))
            f.write("/* the one definition of each file-scope global%s (static\n"
                    " * dropped so the extern in %s.h binds across the modules) */\n\n"
                    % (part, U))
            for d in gchunk:
                f.write(drop_static(d).lstrip() + "\n")
        made_g.append(gbase + ".C")
    # per-segment modules. A big code segment overflows the 1985 compiler's
    # front end if fed as one file, so chunk it into sub-modules — faithful,
    # since a single NE segment was routinely built from several OBJs. All
    # chunks share the same _TEXT segment at link time.
    made = [U + ".H"] + made_g
    for seg in sorted(seg_chunks):
        placed = seg_chunks[seg]
        for ci, (base, chunk) in enumerate(placed):
            if len(base) > 8:
                raise SystemExit("name %r exceeds 8.3; use a shorter stem" % base)
            spath = os.path.join(outdir, base + ".C")
            with open(spath, "w", encoding="utf-8") as f:
                f.write(prelude)
                part = "" if len(placed) == 1 else (" (part %d/%d)" % (ci + 1, len(placed)))
                f.write("/* code segment %d of %s%s, recovered from the NE. */\n\n"
                        % (seg, stem, part))
                # Forward-declare THIS module's own functions before any code.
                # A function called before it is defined within the same file
                # would otherwise get K&R's implicit `int near` declaration,
                # which then clashes with a `void FAR` definition ("redefinition"
                # — CONTROL's Prt_OnCommand). These are only the handful defined
                # here, so they add almost nothing to the symbol table; the
                # cross-module ones live in the shared header.
                # globals used only by this module: extern them here, not in the
                # shared header (keeps every OTHER module's symbol table small).
                le = local_externs.get(base, [])
                if le:
                    f.write("/* globals used only in this module */\n")
                    for e in le:
                        f.write(e + "\n")
                    f.write("\n")
                local = []
                for fn in chunk:
                    nm = func_name_of(fn)
                    if nm and nm not in need_proto:      # header already has those
                        # the definition text already carries FAR where make_far
                        # added it in split(), so prototype_of reads it back.
                        pr = prototype_of(fn)
                        if pr:
                            local.append(pr[1])
                if local:
                    f.write("/* forward declarations for this module's functions */\n")
                    for p in local:
                        f.write(p + "\n")
                    f.write("\n")
                for fn in chunk:
                    # drop a leading `static` so cross-segment calls resolve
                    f.write(drop_static(fn) + "\n\n")
            made.append(base + ".C")
    return made


def _chunk_by_size(funcs, maxbytes):
    """Greedily pack functions into chunks whose total source size stays under
    maxbytes (but always at least one function per chunk)."""
    chunks, cur, cursz = [], [], 0
    for fn in funcs:
        if cur and cursz + len(fn) > maxbytes:
            chunks.append(cur); cur, cursz = [], 0
        cur.append(fn); cursz += len(fn)
    if cur:
        chunks.append(cur)
    return chunks or [[]]


def main():
    src_path = sys.argv[1]
    outdir = sys.argv[2]
    stem = None
    if "--stem" in sys.argv:
        stem = sys.argv[sys.argv.index("--stem") + 1]
    if not stem:
        stem = os.path.splitext(os.path.basename(src_path))[0]
    maxbytes = 6000
    if "--maxbytes" in sys.argv:
        maxbytes = int(sys.argv[sys.argv.index("--maxbytes") + 1])
    segmap = None
    if "--segmap" in sys.argv:
        mp = sys.argv[sys.argv.index("--segmap") + 1]
        segmap = {}
        for line in open(mp, encoding="utf-8"):
            parts = line.split()
            if len(parts) == 2:
                segmap[parts[0]] = int(parts[1])
    # --segmax "3=3000,G=4000": per-segment chunk size for segments whose single
    # unit overflows MS C 3.0 P1 (the globals module is key 'G'). Segments not
    # listed use --maxbytes (which the adaptive driver sets huge = one unit).
    seg_maxbytes = {}
    if "--segmax" in sys.argv:
        for pair in sys.argv[sys.argv.index("--segmax") + 1].split(","):
            if "=" in pair:
                k, v = pair.split("=")
                seg_maxbytes[int(k) if k.lstrip('-').isdigit() else k] = int(v)
    src = open(src_path, encoding="utf-8", errors="replace").read()
    hb, gd, ex, sf, unt, gp, pdefs = split(src, stem, segmap)
    made = emit(outdir, stem, hb, gd, ex, sf, unt, gp, maxbytes,
                pre_defines=pdefs, seg_maxbytes=seg_maxbytes)
    print("%s -> %d modules" % (src_path, len(made)))
    print("  globals: %d   prototypes+banners in .h" % len(gd))
    for seg in sorted(sf):
        tag = ("seg%d" % seg) if seg else "UNTAGGED(seg0)"
        print("  %-14s %d functions" % (tag, len(sf[seg])))
    if unt:
        print("  *** %d function(s) had NO seg tag -> S0 (check these):" % len(unt))
        for t in unt[:8]:
            name = re.search(r"([A-Za-z_]\w*)\s*\(", _strip_cmts(t))
            print("      " + (name.group(1) if name else "?"))
    print("  files: " + ", ".join(made))


if __name__ == "__main__":
    main()
