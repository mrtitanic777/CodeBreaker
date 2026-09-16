#!/usr/bin/env python3
r"""authsrc.py -- automated AUTHORED-SOURCE reconstruction for Win95 files.

The deliverable is source as close to Microsoft's original as the binary allows -- readable, named,
in each file's ORIGINAL LANGUAGE -- NOT byte-identical binary transcription. This driver automates
that per file:

  * classify the file's dominant language from the C-calling-convention density (push bp / mov bp,sp
    frames): the real-mode DOS/boot layer is ASM, the Windows Setup layer is C;
  * ASM-dominant -> route to dosrecon, whose emitter already produces named, INT-service-annotated,
    string-xref'd MASM (bring it toward the msboot.asm authored gold standard);
  * C-dominant   -> decompile to C (the native win95 decompiler) and AUTHOR it: substitute behavioral
    function names (reusing dosrecon's naming engine on the same image), annotate INT-service call
    sites (DOS 21h / 2Fh / XMS / EMS from dosrecon's authentic-name tables), and add a provenance
    header. This lifts machine-C toward authored C.

    wt --target win95 authsrc <file> [--out DIR]        # one file
    wt --target win95 authsrc --chain [--out DIR]       # the whole boot->Setup chain + a scoreboard

Names/comments/file-split are not in a stripped binary -- ours are labeled, evidence-based
reconstructions; "authored" here means correct logic in the right language, named and readable.
"""
import os
import re
import sys
import subprocess

from src.core import paths
from src.targets.win95 import dosrecon as DR


def _native(fmt):
    b = {"MZ": "win95mzdec", "NE": "win95dec", "PE": "win95pedec",
         "LE": "win95ledec"}.get(fmt, "win95mzdec")
    p = os.path.join(paths.repo_root(), "native", "decompiler", "build", "bin", b + ".exe")
    return p if os.path.isfile(p) else None


def classify(path):
    """(fmt, language, nfuncs, c_frames). Format is the reliable era/language signal: PE (Win32) and
    NE (Win16) are C; LE (VxD) is asm; MZ (DOS) is decided by C-calling-convention density."""
    from src.targets.win95 import nativedec
    fmt = nativedec.detect_format(path)
    dec = _native(fmt)
    n = len(subprocess.run([dec, path, "--names"], capture_output=True, text=True).stdout.splitlines()) if dec else 0
    if fmt in ("PE", "NE"):
        lang = "c"
        cf = 0
    elif fmt == "LE":
        lang = "asm"
        cf = 0
    else:                                                             # MZ (DOS): asm kernel vs C tool
        dis = subprocess.run([dec, path, "--disasm"], capture_output=True, text=True).stdout if dec else ""
        cf = dis.count("push bp") + dis.count("push ebp")
        lang = "c" if (n and cf * 100 // max(n, 1) >= 30) else "asm"
    return fmt, lang, n, cf


# --- behavioral name map (reuse dosrecon's engine on the same image) ------------------------
def _namemap(path, fmt):
    """{addr:int -> behavioral name} for the file's functions, via dosrecon's analysis."""
    data = open(path, "rb").read()
    is_iosys, body_end, _lb, image = DR._layout(data)
    if is_iosys:
        image = data
    dec = _native(fmt)
    starts = sorted({int(l.split("\t")[0], 16) for l in
                     subprocess.run([dec, path, "--names"], capture_output=True, text=True).stdout.splitlines()
                     if l.split("\t")[0]})
    starts = [s for s in starts if s < len(image)]
    ends = {s: (starts[i + 1] if i + 1 < len(starts) else len(image)) for i, s in enumerate(starts)}
    strings = DR._string_regions(image)
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    namemap, finfo = {}, {}
    for s in starts:
        e = min(ends[s], s + 0x4000)
        kind, ni, nb, ints, strs, services, callees = DR._analyze_fn(image, s, e, strings, md)
        namemap[s] = DR._beautify_name(s, kind, ints, strs, "")
        finfo[s] = {"services": services, "callees": callees, "strs": strs, "ints": ints, "kind": kind}
    DR._name_by_behavior(namemap, finfo, set(starts))
    # NE modules: name exported functions from the NE entry+name tables (authentic Win16 API names --
    # Win16 GUI code is API-driven, so INT/string behavioral evidence alone leaves most unnamed).
    if fmt == "NE":
        try:
            from src.targets.win95.nerecon import NE
            ne = NE(data)
            for (seg, off), nm in ne.export_map().items():
                if 1 <= seg <= len(ne.segs):
                    fo = ne.segs[seg - 1]["file"] + off
                    namemap[fo] = nm
        except Exception:                                    # noqa: BLE001
            pass
    # the corpus name store wins: a curated name (keyed by body_id -> propagates from any module, or
    # module-local) overrides the engine's inferred name, so names set once render everywhere.
    try:
        from src.targets.win95 import namestore
        module = os.path.basename(path)
        bidm = namestore.bodyid_map(path)
        for s in starts:
            cur = namestore.resolve(module, s, bidm.get(s))
            if cur:
                namemap[s] = cur
    except Exception:                                        # noqa: BLE001 -- store is optional
        pass
    return namemap


# --- INT-service resolution (correlate the AH/AX value at each int site from the disasm) ------
def _int_services(path, fmt):
    """{func_addr:int -> [service label per int site, in order]}. For each `int 0xNN`, scan back for
    the AH/AX it was set with and map to the authentic service name (DOS 21h / 2Fh / XMS / EMS)."""
    dec = _native(fmt)
    dis = subprocess.run([dec, path, "--disasm"], capture_output=True, text=True).stdout
    starts = sorted({int(l.split("\t")[0], 16) for l in
                     subprocess.run([dec, path, "--names"], capture_output=True, text=True).stdout.splitlines()
                     if l.split("\t")[0]})
    import bisect

    def func_of(a):
        i = bisect.bisect_right(starts, a) - 1
        return starts[i] if i >= 0 else None
    out, ah, ax = {}, None, None
    for line in dis.splitlines():
        m = re.match(r"^\s+([0-9A-Fa-f]{4,8})\s+", line)      # "  ADDR  BYTES  text" (indented)
        if not m:
            continue
        addr = int(m.group(1), 16)
        txt = line.split(None, 2)[-1] if len(line.split(None, 2)) == 3 else ""
        mah = re.search(r"mov ah, 0x([0-9a-fA-F]+)", txt)
        max_ = re.search(r"mov ax, 0x([0-9a-fA-F]+)", txt)
        if mah:
            ah = int(mah.group(1), 16)
        if max_:
            ax = int(max_.group(1), 16); ah = (ax >> 8) & 0xFF
        mint = re.search(r"int 0x([0-9a-fA-F]+)", txt)
        if mint:
            cur = func_of(addr)
            if cur is None:
                continue
            out.setdefault(cur, [])
            iv = int(mint.group(1), 16)
            lab = None
            if iv == 0x21 and ah in DR._DOS21:
                lab = "DOS 0x%02X %s" % (ah, DR._DOS21[ah])
            elif iv == 0x2f and ax in DR._INT2F:
                lab = "INT 2Fh %s" % DR._INT2F[ax]
            elif iv == 0x2f and ah == 0x43:
                lab = "XMS"
            elif iv == 0x67:
                lab = "EMS"
            elif iv == 0x10:
                lab = "BIOS video"
            elif iv == 0x13:
                lab = "BIOS disk"
            else:
                lab = "int 0x%02X" % iv
            out[cur].append(lab)
    return out


# --- author the decompiled C ----------------------------------------------------------------
def _simplify_c(c):
    """Clean common decompiler artifacts so bodies read closer to authored C (safe text rewrites)."""
    c = re.sub(r"\(([A-Za-z_]\w*) \^ \1\)", "0", c)                    # (x ^ x) -> 0
    c = re.sub(r"\((\d+) \^ \1\)", "0", c)                             # (4 ^ 4) -> 0
    c = re.sub(r"\b(\w+) \^ \1\b", "0", c)                             # x ^ x   -> 0
    c = c.replace(" + 0 + ", " + ").replace(" - 0 + ", " + ").replace(" + 0;", ";")
    c = re.sub(r" \+ 0\b", "", c)
    return c


_PE_API = re.compile(r"\b([A-Z][A-Za-z0-9]{3,}[AW]?)\(")   # a Win32-API-looking call
_PE_SKIP = {"If", "While", "For", "Switch", "Return", "Sizeof"}


# --- compilability pass: make the authored C a translation unit OpenWatcom accepts -----------
# The decompiler emits legal-looking C with a handful of mechanical defects Watcom rejects (it is
# stricter than MSVC): pseudo-register temps used without a declaration (E1011), and value-returning
# `return EXPR;` inside a function typed void (E1051/E1010). Both are fixable per function without
# touching the logic, so the recovered source actually BUILDS with our only-OpenWatcom toolchain.
# Unresolved temps are declared + flagged honestly (their binding is what the decompiler lost, not
# the structure); everything else is behaviour-preserving.
_FN_OPEN = re.compile(r"^([A-Za-z_].*?)\b([A-Za-z_]\w*)\s*\(([^;]*)\)\s*$")
_DECL = re.compile(r"^\s*(?:unsigned\s+|signed\s+|const\s+)*"
                   r"(?:void|char|short|int|long|float|double|__int8|__int16|__int32|__int64|"
                   r"struct\s+\w+|[A-Za-z_]\w*_t)\b[\s\*]+([A-Za-z_][\w,\s\*]*);")
_TEMP = re.compile(r"\b([rvaf]\d{1,3}|indirect\d+)\b")      # temps: r/v/a/f pseudo-regs + indirect
_FLAGS = ("CA", "OV", "SF", "ZF", "PF", "AF", "DF", "CF")   # decompiler condition-flag pseudos


def _fix_asm(line):
    """Normalise the decompiler's NASM-ish inline asm to the MASM syntax Watcom's built-in assembler
    wants: size specifiers need `ptr` (`word [si]` -> `word ptr [si]`), and immediates use the `h`
    suffix, not `0x` (`0x1` -> `1h`). Only the text inside an `__asm { ... }` block is touched."""
    def fix(m):
        a = m.group(1)
        a = re.sub(r"\b(byte|word|dword|qword|tbyte|fword)\s+\[", r"\1 ptr [", a)
        a = re.sub(r"\b0x([0-9A-Fa-f]+)\b",
                   lambda h: ("0" if h.group(1)[0].lower() in "abcdef" else "") + h.group(1) + "h", a)
        # string / port instructions carry implicit operands in MASM: the decompiler's explicit
        # operands make Watcom reject them, so reduce to the bare mnemonic (same instruction).
        a = re.sub(r"\b(movs|stos|lods|scas|cmps|ins|outs)([bwd]?)\b[^}]*", r"\1\2", a)
        return "__asm {" + a + "}"
    return re.sub(r"__asm \{([^}]*)\}", fix, line)


def _fn_blocks(lines):
    """Yield (start_idx, sig_idx, open_idx, close_idx) for each top-level function: the sig line,
    its `{`, and the matching `}`. Non-function spans are skipped by the caller."""
    i, n = 0, len(lines)
    while i < n:
        m = _FN_OPEN.match(lines[i].rstrip())
        # a function definition: signature line, then a line that is just `{`
        if m and i + 1 < n and lines[i + 1].strip() == "{" and "=" not in lines[i]:
            depth, j = 0, i + 1
            while j < n:
                depth += lines[j].count("{") - lines[j].count("}")
                if depth == 0:
                    break
                j += 1
            yield (i, i, i + 1, j)
            i = j + 1
        else:
            i += 1


def _sanitize_cxx(c):
    """C++ modules (e.g. fdisk's INET_PARSER) carry MSVC-mangled export names like
    `?NextParam@INET_PARSER@@QAEPADXZ`, which are not legal C identifiers -- they break the parser and
    the compiler. Rewrite every mangled token (starts with `?`, no spaces) to a valid identifier,
    consistently at its definition, calls and comments. The ternary `? ` (space after) is not touched."""
    def fix(m):
        return "cxx_" + re.sub(r"[^A-Za-z0-9_]", "_", m.group(0)[1:])
    c = re.sub(r"\?[A-Za-z0-9_@$?]+", fix, c)
    # stdcall-decorated names carry an `@<N>` byte-count suffix (FreeAllIMEObject@0,
    # _JgGetMiniatureDIB@8) that is not a legal C identifier; fold the `@` to `_` consistently at
    # every definition and call site so the decorated name becomes a plain identifier.
    c = re.sub(r"\b([A-Za-z_]\w*)@(\d+)\b", r"\1_\2", c)
    return c


def make_compilable(c):
    """Rewrite authored decompiler-C into a translation unit OpenWatcom compiles. Returns the text."""
    c = _sanitize_cxx(c)
    lines = c.split("\n")
    edits = {}                                              # line_idx -> replacement line(s)
    inject = {}                                             # open-brace idx -> [decl lines]
    protos = []                                             # forward declarations (fixes E1057/E1062)
    seen_proto = set()
    dupcount = {}                                            # function name -> count, to rename dup defs
    # value-used detection: a void-typed function whose result is consumed in an expression must be
    # promoted to a non-void return type (Watcom E1052 'expression has void type'). A call is
    # value-used when `name(` is preceded by an operator/`=`/`return`/`(` rather than standing alone.
    fn_names = set()
    param_types = {}                                        # function -> [param type strings]
    for _s, sig, _o, _cl in _fn_blocks(lines):
        mm = _FN_OPEN.match(lines[sig].rstrip())
        if mm:
            fn_names.add(mm.group(2))
            pp = mm.group(3).strip()
            param_types[mm.group(2)] = [] if pp in ("", "void") else [
                _param_type(p) for p in _split_args(pp)]
    # value-used in ONE pass over the file (a per-name full-file search was O(functions x filesize),
    # which dominated authoring time on large modules): find every call preceded by a value context.
    value_used = set()
    for mm2 in re.finditer(r"(?:[=(,?:+\-*/%<>&|!~\[]|\breturn)\s*([A-Za-z_]\w*)\s*\(", c):
        value_used.add(mm2.group(1))
    value_used &= fn_names

    for _s, sig, opn, close in _fn_blocks(lines):
        m = _FN_OPEN.match(lines[sig].rstrip())
        if not m:
            continue
        ret, params = m.group(1).strip(), m.group(3)
        fname = m.group(2)
        # promote a void function whose result is consumed -> int (fixes E1052); rewrite its sig line
        promoted = re.match(r"^\s*void\b", ret) and fname in value_used
        if promoted:
            ret = re.sub(r"^\s*void\b", "int", ret)
            edits[sig] = re.sub(r"\bvoid\b", "int", lines[sig], count=1)
        # duplicate DEFINITIONS: the decompiler names several thunk addresses identically (e.g. many
        # `__threadid` slots), which C forbids (Watcom E1034 'already defined'). Keep the first with
        # the shared name (calls, rendered by that name, bind to it) and rename every later definition
        # uniquely -- it becomes an uncalled but compilable copy, preserving the file.
        if fname in seen_proto:
            dupN = dupcount.get(fname, 1) + 1
            dupcount[fname] = dupN
            eff = "%s_r%d" % (fname, dupN)
            base_line = edits.get(sig, lines[sig])
            edits[sig] = re.sub(r"\b" + re.escape(fname) + r"\s*\(", eff + "(", base_line, count=1)
            protos.append("%s %s();" % (ret, eff))
        else:                                               # one prototype per function
            seen_proto.add(fname)
            # empty-param `()` form: declares return type + calling convention (kills the implicit-int
            # E1057/E1062 mismatches) but leaves params unspecified, so the decompiler's noisy call-site
            # arg counts are not checked against it (avoids E1151/E1052 param-count churn).
            protos.append("%s %s();" % (ret, fname))
        body = lines[opn + 1:close]
        # declared names = params + local declarations
        declared = set(re.findall(r"\b([A-Za-z_]\w*)\b\s*(?:,|\)|$)", params))
        declared |= {mm for mm in _TEMP.findall(params)}
        for bl in body:
            dm = _DECL.match(bl)
            if dm:
                for nm in dm.group(1).replace("*", "").split(","):
                    declared.add(nm.strip())
        # 1) undeclared decompiler temps (r-regs, v-vars, a-args, condition flags) -> declare them
        #    (honestly flagged; the decompiler lost their binding, so garbage-but-typed is the floor).
        used = set()
        btext = "\n".join(body)
        for bl in body:
            used |= set(_TEMP.findall(bl))
        used |= {f for f in _FLAGS if re.search(r"\b" + f + r"\b", btext)}
        undecl = sorted(used - declared, key=lambda x: (x[0], x[1:]))
        if undecl:
            decl = ("    unsigned long %s;   /* unresolved decompiler temps (binding not recovered) */"
                    % ", ".join(undecl))
            inject.setdefault(opn, []).append(decl)
        # 2) make every return consistent with the function's type (Watcom E1096 rejects mixing bare
        #    `return;` with `return EXPR;`). A void function: turn value returns into `{ EXPR; return; }`
        #    (braces keep it valid as an inline `if (...)` body). A non-void function (incl. one promoted
        #    above): turn bare returns into `return 0;`.
        if re.match(r"^\s*void\b", ret):
            for k in range(opn + 1, close):
                if re.search(r"\breturn\s+[^;]+;", lines[k]):
                    edits[k] = re.sub(r"\breturn\s+([^;]+);", r"{ \1; return; }", lines[k])
        else:
            for k in range(opn + 1, close):
                if re.search(r"\breturn\s*;", lines[k]):     # any bare return, incl. inline `{...; return;}`
                    edits[k] = re.sub(r"\breturn\s*;", "return 0;", edits.get(k, lines[k]))
    out = []
    for idx, ln in enumerate(lines):
        ln = edits.get(idx, ln)
        if "__asm {" in ln:
            ln = _fix_asm(ln)
        out.append(ln)
        if idx in inject:
            out.extend(inject[idx])
    # a label immediately before `}` has no statement (Watcom E1082); give it an empty one.
    for k in range(len(out) - 1):
        if re.match(r"^\s*[A-Za-z_]\w*:\s*$", out[k]):
            nxt = next((out[j] for j in range(k + 1, len(out)) if out[j].strip()), "")
            if nxt.strip() == "}":
                out[k] = out[k].rstrip() + " ;"
    body = _normalize_call_args("\n".join(out), param_types)
    body = _merge_structs(body)
    # unresolved code-label references: the decompiler names a call/address target it never actually
    # emitted a body for (helper_/sub_/loc_ + address). Declaring them extern lets the reference compile
    # (they resolve at link, or stay honestly external). Skip ones defined here (they have prototypes).
    # unresolved code-label references, two shapes: a bare helper_/sub_/loc_ + address, and a
    # behavioural name carrying a trailing _<address> (e.g. LFN_helper_helper_0002F80). Both are
    # call/address targets the decompiler named but never emitted a body for; declare them extern so
    # the reference compiles. LABEL_ (goto targets) is excluded -- it is a label, not a symbol.
    referenced = set(re.findall(r"\b(?:helper|sub|loc)_[0-9A-Fa-f]{2,}\b", body))
    referenced |= {nm for nm in re.findall(r"\b([A-Za-z_]\w*_[0-9A-Fa-f]{4,})\b", body)
                   if not nm.startswith("LABEL_")}
    externs = ["extern int %s();" % nm for nm in sorted(referenced - fn_names)]
    if protos or externs:
        body = ("/* forward declarations (authored -- lets the file compile in any order) */\n"
                + "\n".join(externs + protos) + "\n\n" + body)
    return body


def _split_args(s):
    """Split an argument/parameter list at top-level commas (ignoring commas nested in (), [], {})."""
    args, depth, cur = [], 0, ""
    for ch in s:
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth -= 1
        if ch == "," and depth == 0:
            args.append(cur.strip()); cur = ""
        else:
            cur += ch
    if cur.strip():
        args.append(cur.strip())
    return args


def _param_type(p):
    """The type portion of one parameter declaration ('struct V10_t *a3' -> 'struct V10_t *')."""
    pm = re.match(r"^(.*?)([A-Za-z_]\w*)\s*$", p.strip())
    return pm.group(1).strip() if (pm and pm.group(1).strip()) else "int"


def _normalize_call_args(c, param_types):
    """The decompiler's per-call and per-definition argument recovery frequently disagree: a call
    passes a different NUMBER of arguments than its callee is defined to take (Watcom E1151), or an
    argument of an incompatible TYPE (E1071). Normalise every call to a locally-defined function to
    the callee's declared signature: drop extra trailing args (never read) or pad missing ones with 0,
    then cast each arg to the declared parameter type so it always agrees. Casts are how the recovered
    call-site type is reconciled with the recovered definition; behaviour-preserving for the common
    over-supply case. Function-pointer params (type contains '(') are left uncast. Definition and
    prototype lines re-emit unchanged."""
    if not param_types:
        return c
    # ONE finditer pass over call-like tokens. The previous version scanned every character with a
    # regex built from an alternation of ALL function names -- O(filesize x names), which made
    # make_compilable take tens of seconds on large modules. Here each candidate token is an O(1)
    # dict lookup; only real call sites are rewritten, everything else is copied verbatim.
    n = len(c)
    out, last = [], 0
    decl0 = re.compile(r"^(?:unsigned |signed |const )*"
                       r"(?:void|char|short|int|long|struct\s+\w+|[A-Za-z_]\w*_t|__int\d+)\b"
                       r"[\s\*]+[A-Za-z_]\w*$")
    for m in re.finditer(r"\b([A-Za-z_]\w*)\s*\(", c):
        if m.start() < last:                              # nested call inside an already-consumed span
            continue
        types = param_types.get(m.group(1))
        if types is None:
            continue
        j = m.end() - 1                                   # index of '('
        depth, k = 0, j
        while k < n:
            if c[k] == "(":
                depth += 1
            elif c[k] == ")":
                depth -= 1
                if depth == 0:
                    break
            k += 1
        if k >= n:
            continue
        args = _split_args(c[j + 1:k])
        # skip the function's own definition / prototype (its "args" are parameter declarations).
        if args and decl0.match(args[0].strip()):
            continue
        want = len(types)
        if len(args) > want:
            args = args[:want]
        elif len(args) < want:
            args += ["0"] * (want - len(args))
        cast = []
        for ai, a in enumerate(args):
            ty = types[ai] if ai < len(types) else ""
            # cast every arg to the declared parameter type (int included): a call before the
            # definition otherwise makes Watcom infer the arg's own type and the definition disagrees
            # (E1071). A function-pointer param is left uncast.
            cast.append("(%s)(%s)" % (ty, a) if ty and "(" not in ty else a)
        out.append(c[last:m.start()])
        out.append(m.group(1) + "(" + ", ".join(cast) + ")")
        last = k + 1
    out.append(c[last:])
    return "".join(out)


def _merge_structs(c):
    """The decompiler re-emits `struct Tag { ... };` at every referencing function, often with a
    DIFFERENT subset of fields each time (Watcom E1019 'already defined', and E1031 'field not found'
    when a later access needs a field an earlier definition lacked). Merge every definition of a tag
    into ONE struct holding the UNION of its fields (deduped by name); the first occurrence carries
    the merged body, the rest are dropped. Also completes each synthesized V<n>_t placeholder with any
    field_<off> the code accesses but no definition declared, so member access always resolves."""
    lines = c.split("\n")
    # pass 1: collect field-line sets per tag, and each definition's span
    defs = {}                                              # tag -> [(start,end)]
    fields = {}                                            # tag -> {fieldname: full_decl_line}
    i, n = 0, len(lines)
    while i < n:
        m = re.match(r"^\s*struct\s+(\w+)\s*\{", lines[i])
        if m:
            tag = m.group(1)
            depth, j = 0, i
            while j < n:
                depth += lines[j].count("{") - lines[j].count("}")
                if depth == 0 and "}" in lines[j]:
                    break
                j += 1
            defs.setdefault(tag, []).append((i, j))
            fm = fields.setdefault(tag, {})
            for k in range(i + 1, j):
                dm = re.search(r"\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*;", lines[k])
                if dm and "{" not in lines[k]:
                    fm.setdefault(dm.group(1), lines[k].rstrip())
            i = j + 1
            continue
        i += 1
    # complete synthesized V<n>_t placeholders with any accessed-but-undeclared field_<off>
    accessed = set(re.findall(r"(?:->|\.)\s*(field_[0-9A-Fa-f]+)\b", c))
    for tag, fm in fields.items():
        if re.match(r"V\d+_t$", tag):
            for fld in sorted(accessed):
                if fld not in fm:
                    fm[fld] = "  unsigned long %s;   /* accessed; offset unrecovered */" % fld
    # pass 2: rewrite -- first def of each tag becomes the merged body, later defs are dropped
    emitted = set()
    out, i = [], 0
    while i < n:
        m = re.match(r"^\s*struct\s+(\w+)\s*\{", lines[i])
        if m and m.group(1) in defs:
            tag = m.group(1)
            _s, e = defs[tag][0]
            span_here = next(((s2, e2) for (s2, e2) in defs[tag] if s2 == i), None)
            if span_here is None:
                out.append(lines[i]); i += 1; continue
            if tag in emitted or i != _s:
                i = span_here[1] + 1                        # drop repeat definition
                continue
            emitted.add(tag)
            out.append(lines[i])
            for decl in fields[tag].values():
                out.append(decl if decl.lstrip().startswith(("unsigned", "signed", "int", "char",
                           "short", "long", "float", "double", "struct", "void", "__int")) else "  " + decl.strip())
            out.append("};")
            i = span_here[1] + 1
            continue
        out.append(lines[i]); i += 1
    return "\n".join(out)


def _pe_namemap(path, module_c=None):
    """PE (Win32) name map (lever 1): exported functions -> their authentic export name; internal
    sub_ functions -> named after the most distinctive Win32 API they call. The decompiler already
    resolves imports to named API calls in the emitted C, so this reads them straight from it.
    module_c: the already-decoded `--module` text, reused to avoid a second decode of the binary."""
    dec = _native("PE")
    names = {}
    for l in subprocess.run([dec, path, "--names"], capture_output=True, text=True).stdout.splitlines():
        p = l.split("\t")
        if len(p) > 1 and p[1].strip():
            try:
                names[int(p[0], 16)] = p[1].strip()               # authentic export name
            except ValueError:
                pass
    c = module_c if module_c is not None else \
        subprocess.run([dec, path, "--module"], capture_output=True, text=True).stdout
    cur, apis = None, {}
    for line in c.splitlines():
        fm = re.match(r"^[A-Za-z_].*\bsub_([0-9A-Fa-f]{4,8})\(.*\)\s*$", line)
        if fm:
            cur = int(fm.group(1), 16); apis.setdefault(cur, [])
            continue
        if cur is not None:
            for m in _PE_API.finditer(line):
                a = m.group(1)
                if a not in _PE_SKIP and not a.startswith(("sub_", "v")):
                    apis[cur].append(a)
    for addr, calls in apis.items():
        if addr in names or not calls:
            continue
        top = max(set(calls), key=lambda x: (calls.count(x), len(x)))   # most-called, then longest
        names[addr] = "%s_helper_%06X" % (top, addr)
    # lever 2: the corpus name store wins -- a shared-body name (seeded from body_ledger, or curated)
    # propagates here from any module that shares the body.
    try:
        from src.targets.win95 import namestore
        module = os.path.basename(path)
        bidm = namestore.bodyid_map(path)
        for a, bid in bidm.items():
            cur = namestore.resolve(module, a, bid)
            if cur:
                names[a] = cur
    except Exception:                                            # noqa: BLE001
        pass
    return names


def _author_c(path, fmt, out):
    dec = _native(fmt)
    c = subprocess.run([dec, path, "--module"], capture_output=True, text=True).stdout
    # the behavioral-naming + INT-service passes reuse dosrecon's 16-bit real-mode engine; apply them
    # for the 16-bit classes (MZ/NE) and skip cleanly for 32-bit PE (still cleaned + headered).
    names, svcs = {}, {}
    if fmt in ("MZ", "NE"):
        try:
            names = _namemap(path, fmt); svcs = _int_services(path, fmt)
        except Exception:                                             # noqa: BLE001 -- best effort
            names, svcs = {}, {}
    elif fmt == "PE":                                                # lever 1: Win32 export + API naming
        try:
            names = _pe_namemap(path, module_c=c)                     # reuse the already-decoded C
        except Exception:                                             # noqa: BLE001
            names = {}

    def sub_name(m):
        return names.get(int(m.group(1), 16), m.group(0))
    c = re.sub(r"sub_([0-9A-Fa-f]{4,8})", sub_name, c)                 # behavioral function names
    c = _simplify_c(c)

    # per-function INT-service annotation: within each function (identified by its _XXXXXX address
    # suffix), replace the int sites in order with the specific resolved service.
    outlines = []
    cur_addr = None
    idx = 0
    for line in c.splitlines():
        fm = re.match(r"^[A-Za-z_].*_([0-9A-Fa-f]{4,8})\(.*\)\s*$", line)
        if fm:
            cur_addr = int(fm.group(1), 16); idx = 0
        im = re.search(r"__asm \{ int (0x[0-9a-fA-F]+) \}", line)
        if im and cur_addr is not None:
            seq = svcs.get(cur_addr, [])
            lab = seq[idx] if idx < len(seq) else (im.group(1) + " service")
            idx += 1
            line = re.sub(r"__asm \{ int 0x[0-9a-fA-F]+ \}",
                          "__asm { int %s }   /* %s */" % (im.group(1), lab), line)
        outlines.append(line)
    c = "\n".join(outlines)

    # compilability pass: make the authored C a translation unit OpenWatcom accepts (our only C
    # toolchain). Best-effort -- never let a transform bug drop the authored output.
    try:
        c = make_compilable(c)
    except Exception:                                                # noqa: BLE001
        pass

    hdr = ("/* %s -- authored-source reconstruction (CodeBreaker authsrc).\n"
           " * Original language: C. Decompiled from the real Win95 binary and authored: behavioral\n"
           " * function names, resolved INT-service call sites (DOS/2Fh/XMS/EMS/BIOS), artifact\n"
           " * cleanup. Names are evidence-based reconstruction labels, not recovered symbols. */\n\n"
           % os.path.basename(path))
    os.makedirs(os.path.dirname(out), exist_ok=True)
    open(out, "w", encoding="utf-8", errors="replace").write(hdr + c)
    named = sum(1 for v in names.values() if not v.startswith(("sub_", "helper_", "thunk_")))
    return len(names), named


# --- the boot -> Setup chain ----------------------------------------------------------------
_CHAIN = [
    ("VBR",         "recon/bootdisk/msboot.asm", "asm-authored (gold standard)"),
    ("IO.SYS",      "corpus/files/setup/inf/winboot.sys", None),
    ("COMMAND.COM", "corpus/files/setup/inf/command.com", None),
    ("HIMEM.SYS",   "corpus/files/WINDOWS/SYSTEM/himem.sys", None),
    ("CD1.SYS",     "recon/bootdisk/files/CD1.SYS", None),
    ("MSCDEX.EXE",  "corpus/files/WINDOWS/COMMAND/mscdex.exe", None),
    ("FDISK.EXE",   "corpus/files/WINDOWS/COMMAND/fdisk.exe", None),
    ("FORMAT.COM",  "corpus/files/WINDOWS/COMMAND/format.com", None),
    ("OEMSETUP.EXE", "corpus/disc/src/WIN95/OEMSETUP.EXE", None),
    ("SETUPX.DLL",  "corpus/files/setup/inf/setupx.dll", None),
    ("SUWIN.EXE",   "corpus/files/setup/inf/suwin.exe", None),
]


def _dr():
    dr = paths.design_root()
    if os.path.isdir(os.path.join(dr, "corpus")):
        return dr
    sib = os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design")
    return sib if os.path.isdir(os.path.join(sib, "corpus")) else dr


def _engine_ver():
    """A signature of the authoring engine (this module + owcheck + the decompiler binaries). When it
    changes, the incremental cache is invalidated so a corpus re-run re-authors with the new engine."""
    import hashlib
    h = hashlib.sha1()
    here = os.path.dirname(os.path.abspath(__file__))
    for f in ("authsrc.py", "owcheck.py"):
        p = os.path.join(here, f)
        if os.path.isfile(p):
            h.update(("%s:%d;" % (f, int(os.path.getmtime(p)))).encode())
    bindir = os.path.join(paths.repo_root(), "native", "decompiler", "build", "bin")
    for b in ("win95mzdec.exe", "win95dec.exe", "win95pedec.exe"):
        p = os.path.join(bindir, b)
        if os.path.isfile(p):
            h.update(("%s:%d;" % (b, int(os.path.getmtime(p)))).encode())
    return h.hexdigest()[:16]


def _src_sha(path):
    import hashlib
    return hashlib.sha1(open(path, "rb").read()).hexdigest()[:16]


def _finish_c(outpath, bits):
    """Compile the authored C with OpenWatcom; if only inline-asm (E1156) blocks the build, neutralise
    the mis-decoded ones and recompile. Returns (compiles: bool, errors: int)."""
    from src.targets.win95 import owcheck
    r = owcheck.compile_c(outpath, bits)
    if not r["ok"] and r["by_code"].get("E1156"):
        r, _n = owcheck.neutralize_unassemblable(outpath, bits)
    return r["ok"], r["errors"]


def _author_job(job):
    """Author + compile-verify ONE binary; a picklable unit of work for the parallel corpus sweep.
    Returns a row dict (also carries the source sha + compile verdict for the cache)."""
    nm, path, outroot = job
    try:
        fmt, lang, n, cf = classify(path)
        if lang != "c":
            return {"nm": nm, "row": (nm, "asm/" + fmt, n, 0, 0, "dosrecon/asm route")}
        out = os.path.join(outroot, "c", nm.split(".")[0].lower() + ".c")
        tot, named = _author_c(path, fmt, out)
        bits = 32 if fmt == "PE" else 16
        ok, errs = _finish_c(out, bits)
        return {"nm": nm, "fmt": fmt, "n": n, "total": tot, "named": named, "compiles": bool(ok),
                "errors": errs, "sha": _src_sha(path), "path": path,
                "row": (nm, "C/" + fmt, n, tot, named,
                        os.path.relpath(out, _dr()) + ("  [build:OK]" if ok else "  [build:%d err]" % errs))}
    except Exception as e:                                            # noqa: BLE001 -- keep the sweep going
        return {"nm": nm, "row": (nm, "ERR", 0, 0, 0, str(e)[:60])}


def _all_binaries():
    """Every code-bearing Win95 binary under corpus/files (dedup by name, prefer SYSTEM/)."""
    root = os.path.join(_dr(), "corpus", "files")
    exts = (".exe", ".dll", ".sys", ".com", ".drv", ".vxd", ".cpl", ".386", ".scr")
    found = {}
    for dp, _dn, fns in os.walk(root):
        for fn in fns:
            if fn.lower().endswith(exts):
                key = fn.lower()
                prefer = "system" in dp.lower().replace("\\", "/").split("/")
                if key not in found or (prefer and "system" not in found[key].lower().replace("\\", "/").split("/")):
                    found[key] = os.path.join(dp, fn)
    return [(os.path.basename(p), p, None) for p in sorted(found.values(), key=lambda x: os.path.basename(x).lower())]


def main(argv):
    outroot = os.path.join(_dr(), "recon", "authored")
    chain = do_all = verify = force = False
    target = None
    limit = 0
    jobs_n = min((os.cpu_count() or 4), 12)                  # parallel workers for the corpus sweep
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == "--chain":
            chain = True
        elif a == "--all":
            do_all = True
        elif a == "--verify":                                # compile each authored C with OpenWatcom
            verify = True
        elif a == "--force":                                 # ignore the incremental cache; redo all
            force = True
        elif a in ("--jobs", "-j") and i + 1 < len(argv):
            jobs_n = int(argv[i + 1]); i += 1
        elif a == "--limit" and i + 1 < len(argv):
            limit = int(argv[i + 1]); i += 1
        elif a == "--out" and i + 1 < len(argv):
            outroot = argv[i + 1]; i += 1
        elif not a.startswith("-"):
            target = a
        i += 1
    verify = verify or do_all or chain                       # corpus/chain runs verify by default

    items = []
    scoreboard = None
    if do_all:
        items = _all_binaries()
        if limit:
            items = items[:limit]
        scoreboard = os.path.join(outroot, "AUTHORED.csv")
    elif chain:
        for nm, rel, note in _CHAIN:
            items.append((nm, os.path.join(_dr(), rel), note))
    elif target:
        items.append((os.path.basename(target), target, None))
    else:
        print(__doc__); return 0

    # incremental cache: skip files already authored + verified-compilable whose source and the engine
    # are unchanged, so a corpus re-run does NOT redo work that is already correct.
    import json
    cache_path = os.path.join(outroot, ".authcache.json")
    cache = {}
    if verify and not force and os.path.isfile(cache_path):
        try:
            cache = json.load(open(cache_path))
        except Exception:                                            # noqa: BLE001
            cache = {}
    ever = _engine_ver() if verify else ""

    rows = []
    quiet = do_all and len(items) > 30
    if not quiet:
        print("%-14s %-4s %-6s %-9s %s" % ("FILE", "LANG", "FUNCS", "NAMED", "authored source"))
    skipped = compiled = c_total = 0

    # Phase 1 (serial, cheap): resolve notes / missing / cache-hits without any decode; the rest
    # become jobs for the parallel workers. A cache hit needs only the source sha (no decompile).
    jobs = []
    for nm, path, note in items:
        if note and str(path).endswith(".asm"):
            rows.append((nm, "asm", 0, 0, 0, note)); continue
        if not os.path.isfile(path):
            rows.append((nm, "?", 0, 0, 0, "MISSING")); continue
        key = os.path.relpath(path, _dr())
        out = os.path.join(outroot, "c", nm.split(".")[0].lower() + ".c")
        ce = cache.get(key)
        if (verify and not force and ce and ce.get("sha") == _src_sha(path) and ce.get("engine") == ever
                and ce.get("compiles") and os.path.isfile(out)):
            c_total += 1; compiled += 1; skipped += 1
            rows.append((nm, "C/" + ce.get("fmt", "PE"), ce.get("n", 0), ce.get("total", 0),
                         ce.get("named", 0), "cached (compiles) -- skipped"))
            continue
        jobs.append((nm, path, outroot))

    # Phase 2 (parallel): author + compile-verify every remaining file across worker processes. The
    # decompiler and wcc are the cost and they are per-file independent, so this scales ~linearly with
    # cores -- the corpus sweep goes from serial-hours to minutes. -j1 forces the serial path.
    def _apply(res):
        nonlocal compiled, c_total
        rows.append(res["row"])
        if "compiles" in res:
            c_total += 1
            if res["compiles"]:
                compiled += 1
            cache[os.path.relpath(res["path"], _dr())] = {
                "sha": res["sha"], "engine": ever, "compiles": res["compiles"],
                "errors": res["errors"], "total": res["total"], "named": res["named"],
                "fmt": res["fmt"], "n": res["n"]}

    done = 0
    if jobs_n > 1 and len(jobs) > 1:
        from concurrent.futures import ProcessPoolExecutor, as_completed
        with ProcessPoolExecutor(max_workers=jobs_n) as ex:
            futs = {ex.submit(_author_job, j): j for j in jobs}
            for fut in as_completed(futs):
                _apply(fut.result()); done += 1
                if not quiet:
                    print("  %s" % (rows[-1][0]))
                elif done % 50 == 0:
                    print("  ... %d/%d authored (%d skipped, %d workers)" % (done, len(jobs), skipped, jobs_n))
    else:
        for j in jobs:
            _apply(_author_job(j)); done += 1
            if quiet and done % 50 == 0:
                print("  ... %d/%d authored (%d skipped)" % (done, len(jobs), skipped))

    if verify:
        try:
            os.makedirs(outroot, exist_ok=True)
            json.dump(cache, open(cache_path, "w"), indent=0, sort_keys=True)
        except Exception:                                            # noqa: BLE001
            pass

    if scoreboard:
        import csv
        os.makedirs(os.path.dirname(scoreboard), exist_ok=True)
        with open(scoreboard, "w", newline="") as f:
            w = csv.writer(f); w.writerow(["file", "lang", "funcs", "total", "named", "out_or_note"])
            for r in rows:
                w.writerow(r)
        c_files = sum(1 for r in rows if r[1].startswith("C"))
        asm_files = sum(1 for r in rows if r[1].startswith("asm"))
        errs = sum(1 for r in rows if r[1] == "ERR")
        tot_named = sum(r[4] for r in rows); tot_funcs = sum(r[3] for r in rows)
        print("\n== authsrc corpus scoreboard ==")
        print("  %d files: %d C-authored, %d asm-route, %d errors" % (len(rows), c_files, asm_files, errs))
        print("  C functions named: %d / %d (%.0f%%)"
              % (tot_named, tot_funcs, 100.0 * tot_named / max(tot_funcs, 1)))
        if verify:
            print("  C files compiling with OpenWatcom: %d / %d (%.0f%%)   [%d skipped as already-correct]"
                  % (compiled, c_total, 100.0 * compiled / max(c_total, 1), skipped))
        print("  -> %s" % scoreboard)
    print("\nauthored source -> %s" % outroot)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
