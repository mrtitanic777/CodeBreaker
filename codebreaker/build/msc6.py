"""msc6.py -- Microsoft C 6.0a, the compiler of the 16-bit Windows modules, run for comparison.

    from codebreaker.build import msc6
    msc6.available()
    built = msc6.compile_many({"f": c_text, ...}, header, "/AM /Gsw /Os")   # {"f": (bytes, mask) or None}

The compiler is a DOS program. It is run in DOSBox-X (settings: dosbox_x -- the emulator's
program; msc6 -- the folder holding the compiler's BIN and INCLUDE), every source of a call in
ONE session: starting the emulator costs about two seconds, compiling a function a fraction of
one. Each function is compiled as a file of its own and its code read out of the object file,
with the bytes the linker fills in (an address, a segment, the distance of a call to another
file) marked in `mask` and left out of every comparison -- as build/period.py's other compilers.

What the reconstruction is written in for judging is OpenWatcom's dialect; `dialect()` is the
same text said the way this compiler takes it (its keywords for far and the calling
conventions, no 64-bit integers).
"""
import os
import re
import struct
import subprocess

# /Gc: a function declared without a convention is pascal, as the reconstruction's own default
FLAG_SETS = ("/AM /Gcsw /Os", "/AM /Gcsw /Ox", "/AS /Gcsw /Os", "/AL /Gcsw /Os", "/AM /Gcsw /Od", "/AM /Gc2sw /Os",
             "/AM /Gc2sw /Ox", "/AM /Gcs /Os", "/AS /Gcs /Os")   # (/G2: SUWIN has push imm and leave)


def _tools(compiler="msc6"):
    """(DOSBox-X, the compiler's folder). compiler: "msc6" (Microsoft C 6.0a) or "msvc152"
    (Microsoft C/C++ 8.00c of Visual C++ 1.52c, which built a few of the 16-bit modules); both
    are a BIN\\CL.EXE run the same way."""
    from codebreaker.base import settings
    return settings.get("dosbox_x"), settings.get(compiler)


def available(compiler="msc6"):
    emulator, home = _tools(compiler)
    return bool(emulator and os.path.isfile(emulator) and home and os.path.isfile(os.path.join(home, "BIN", "CL.EXE")))


def object_code(obj):
    """(bytes, mask) of the first code segment of an OMF object file: the data records of that
    segment laid end to end, and 1 for every byte a fix-up record names."""
    code, mask, p = bytearray(), bytearray(), 0
    last = None                                              # (segment, offset of the record's data in `code`)
    while p + 3 <= len(obj):
        kind = obj[p]
        size = struct.unpack_from("<H", obj, p + 1)[0]
        body = obj[p + 3:p + 3 + size - 1]
        p += 3 + size
        if kind in (0xA0, 0xA1):                             # LEDATA: segment, offset, bytes
            i = 0
            seg = body[i]
            i += 1
            if seg & 0x80:
                seg = ((seg & 0x7F) << 8) | body[i]
                i += 1
            width = 4 if kind == 0xA1 else 2
            off = int.from_bytes(body[i:i + width], "little")
            i += width
            if seg != 1:
                last = None
                continue
            if off > len(code):
                code.extend(bytes(off - len(code)))
                mask.extend(bytes(off - len(mask)))
            code[off:off + len(body) - i] = body[i:]
            mask[off:off + len(body) - i] = bytes(len(body) - i)
            last = (seg, off)
        elif kind in (0x9C, 0x9D) and last:                  # FIXUPP: the places in the record before it
            i = 0
            while i < len(body):
                b = body[i]
                if not b & 0x80:                             # a thread: one byte, and an index for most methods
                    i += 1
                    if (b >> 2) & 7 < 4 and i < len(body):
                        i += 2 if body[i] & 0x80 else 1
                    continue
                where = ((b & 3) << 8) | body[i + 1]
                loc = (b >> 2) & 0xF
                fix = body[i + 2]
                i += 3
                if not fix & 0x80 and (fix >> 4) & 7 < 3:    # the frame is given here
                    i += 2 if body[i] & 0x80 else 1
                if not fix & 0x08:                           # the target is given here
                    i += 2 if body[i] & 0x80 else 1
                if not fix & 0x04:                           # with a displacement
                    i += 4 if kind == 0x9D else 2
                n = {0: 1, 1: 2, 2: 2, 3: 4, 4: 1, 5: 2, 9: 4, 11: 6, 13: 4}.get(loc, 2)
                at = last[1] + where
                mask[at:at + n] = b"\1" * min(n, max(0, len(mask) - at))
        elif kind in (0x8A, 0x8B):
            break
    return bytes(code), bytes(mask)


def dialect(header, text):
    """(header, text) as Microsoft C 6 takes them."""
    said = ["#define __far _far", "#define __near _near", "#define __stdcall _pascal", "#define __cdecl _cdecl",
            "#define codebreaker_ss() 0", "#define codebreaker_ds() 0", "#define codebreaker_cs() 0",
            # OpenWatcom's spellings of what Microsoft C 6 has with one underscore, and the far-pointer
            # macros OpenWatcom's i86.h gives (here without a header)
            "#define __based _based", "#define __segname _segname", "#define __export _export",
            "#define __loadds _loadds", "#define __interrupt _interrupt", "#define __saveregs _saveregs",
            # (OpenWatcom's _Packed: the frame structure c_text writes is of words -- no padding to lose)
            "#define _Packed",
            # (Microsoft's _fastcall: arguments in AX, DX, BX -- describe's __fastcall16 model)
            "#define __fastcall _fastcall", "#define __fastcall16near __near __fastcall",
            "#define __fastcall16far __far __fastcall",
            "#define MK_FP(s,o) ((void _far *)(((unsigned long)(unsigned)(s) << 16) | (unsigned)(o)))",
            "#define FP_OFF(p) ((unsigned)(unsigned long)(void _far *)(p))",
            "#define FP_SEG(p) ((unsigned)((unsigned long)(void _far *)(p) >> 16))"]
    keep = []
    for line in header.split("\n"):
        if "#pragma aux" in line or re.match(r"uint2 codebreaker_[sdc]s\(void\);", line.strip()):
            continue
        line = line.replace("unsigned long long", "unsigned long").replace("long long", "long").replace(
            "long double", "double")
        keep.append(line)
    return "\n".join(said + keep) + "\n", text


# The reconstruction says a callee-pops convention as OpenWatcom's __stdcall, whose arguments go on
# the stack right to left: the C lists them in that order. Microsoft's _pascal pushes them left to
# right, so the same list said as _pascal is backwards -- the Windows API and every pascal function of
# a 1990s module were written with the first-pushed argument first.
_PASCAL = re.compile(r"^(?!\s*#)[^\n]*?__stdcall(?:16(?:far|near))?\b[^;(){}=\n]*?\b(\w+)\s*\(", re.M)
_SEGS = ("unaff_SS", "codebreaker_ss()", "unaff_DS", "codebreaker_ds()")


def pascal_names(*texts):
    """the functions the texts declare or define with a callee-pops (__stdcall) convention"""
    return {m.group(1) for t in texts for m in _PASCAL.finditer(t)}


def _close(text, i):
    """the index past the parenthesis that closes the one before `i` (string and character literals
    skipped), or -1"""
    depth = 1
    while i < len(text):
        c = text[i]
        if c in "\"'":
            j = i + 1
            while j < len(text) and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            i = j + 1
            continue
        depth += c == "("
        depth -= c == ")"
        i += 1
        if not depth:
            return i
    return -1


def _arguments(inner):
    """an argument list cut at its top-level commas"""
    out, depth, start, i = [], 0, 0, 0
    while i < len(inner):
        c = inner[i]
        if c in "\"'":
            j = i + 1
            while j < len(inner) and inner[j] != c:
                j += 2 if inner[j] == "\\" else 1
            i = j + 1
            continue
        depth += c in "([{"
        depth -= c in ")]}"
        if c == "," and not depth:
            out.append(inner[start:i])
            start = i + 1
        i += 1
    if inner.strip():
        out.append(inner[start:])
    return out


def _far_pairs(args):
    """an offset followed by the segment register it is in (right-to-left order: the offset is pushed
    last, so it comes first) is one far pointer, as the source had it"""
    out, k = [], 0
    while k < len(args):
        lo = re.match(r"^\s*\(\s*u?int2\s*\)\s*(.+?)\s*$", args[k]) if k + 1 < len(args) else None
        hi = re.match(r"^\s*\(\s*u?int2\s*\)\s*\(\s*\(\s*u?int4\s*\)\s*(.+?)\s*>>\s*(?:0x10|16)\s*\)\s*$",
                      args[k + 1]) if lo else None
        if lo and hi and lo.group(1).strip("() ") == hi.group(1).strip("() "):
            out.append(" " + lo.group(1))            # a 4-byte value split into its words: the value
            k += 2
            continue
        if k + 1 < len(args) and args[k + 1].strip() in _SEGS:
            off = re.sub(r"^\s*\(\s*u?int2\s*\)\s*", "", args[k]).strip()
            out.append(" (void __far *)%s" % (off if off.startswith("&") else "(char __near *)(%s)" % off))
            k += 2
        else:
            out.append(args[k])
            k += 1
    return out


def pascal_order(text, names):
    """`text` with the arguments of every call to, and the parameters of every definition of, a
    function in `names` in Pascal order: first pushed first, an offset and its segment one far pointer."""
    out, i = [], 0
    call = re.compile(r"\b(\w+)\s*\(")
    while True:
        m = call.search(text, i)
        if not m:
            out.append(text[i:])
            return "".join(out)
        end = _close(text, m.end())
        if m.group(1) not in names or end < 0:
            out.append(text[i:m.end()])
            i = m.end()
            continue
        args = _far_pairs([pascal_order(a, names) for a in _arguments(text[m.end():end - 1])])
        args.reverse()
        out.append(text[i:m.end()] + ", ".join(a.strip() for a in args) + ")")
        i = end


_IMPORT = re.compile(r"^extern [^;\n]*__stdcall\s+([A-Z][A-Z0-9]*)_(\d+)\s*\(\s*\);[ \t]*$", re.M)


def windows_h():
    """the Windows SDK's WINDOWS.H (Visual C++ 1.52c's, the Windows 3.1 one), or None"""
    from codebreaker.base import settings
    home = settings.get("msvc152")
    path = os.path.join(home or "", "INCLUDE", "WINDOWS.H")
    if not home or not os.path.isfile(path):
        return None
    with open(path, encoding="latin1") as f:
        return f.read().replace("\r\n", "\n")


def cdecl_imports(header, sdk=None):
    """`header` with every import by number that is really a C function declared __cdecl, not
    __stdcall: one its module exports under a C name (`_SUFORMATMESSAGEBOX` -- a pascal export is
    in capitals, a cdecl one keeps the compiler's underscore), or one WINDOWS.H declares CDECL
    (wsprintf). The caller removes such a function's arguments (`add sp, 12h` after the call), and
    its argument list is in C order."""
    from codebreaker.symbols import ordinals
    sdk = sdk if sdk is not None else (windows_h() or "")
    cdecl = set(re.findall(r"\bCDECL\s+(\w+)\s*\(", sdk))

    def swap(m):
        nm = ordinals.name(m.group(1), int(m.group(2))) or ""
        if nm.startswith("_") or nm in cdecl:
            return m.group(0).replace("__stdcall", "__cdecl")
        return m.group(0)
    return _IMPORT.sub(swap, header)


def wine_typing(header, texts, exports=None):
    """(header, {name: text}) with every import by number that Wine's .spec types (symbols/wine16.py)
    declared with its parameters and its result's width, and each call of it said with one argument
    to a parameter -- a 4-byte parameter's two words joined (high first, as a pascal call pushes them;
    low first in a C call's list). The texts are in Pascal order (pascal_order). An import with a call
    whose arguments fit neither way keeps its declaration without parameters. `exports`: (module,
    ordinal) -> wine16.Export, for tests."""
    from codebreaker.symbols import wine16
    look = exports or (lambda mod, n: wine16.exports(mod).get(n))
    typed = {}
    for m in _IMPORT.finditer(header):
        e = look(m.group(1), int(m.group(2)))
        if e and e.args is not None and e.convention in ("pascal", "cdecl"):
            typed["%s_%s" % (m.group(1), m.group(2))] = e
    call = re.compile(r"\b(%s)\s*\(" % "|".join(sorted(typed)) if typed else r"(?!x)x")

    def calls(text):
        for mm in call.finditer(text):
            end = _close(text, mm.end())
            if end > 0:
                yield mm, end, _arguments(text[mm.end():end - 1])
    fits = {}
    for text in texts.values():
        for mm, _end, args in calls(text):
            e = typed[mm.group(1)]
            ok = len(args) in (len(e.args), sum(wine16.WIDTH[a] // 2 for a in e.args))
            fits[mm.group(1)] = fits.get(mm.group(1), True) and ok
    use = {nm: e for nm, e in typed.items() if fits.get(nm, True)}

    def said(text):
        out, i = [], 0
        for mm, end, args in calls(text):
            if mm.group(1) not in use or mm.start() < i:     # (a call inside the arguments of one said: said with them)
                continue
            e = use[mm.group(1)]
            args = [said(a.strip()) for a in args]
            if len(args) != len(e.args):
                joined, k = [], 0
                for kind in e.args:
                    if wine16.WIDTH[kind] == 4:
                        hi, lo = (args[k], args[k + 1]) if e.convention == "pascal" else (args[k + 1], args[k])
                        v = "(((uint4)(uint2)(%s) << 16) | (uint2)(%s))" % (hi, lo)
                        joined.append("(void __far *)%s" % v if kind in wine16.POINTER else v)
                        k += 2
                    else:
                        joined.append(args[k])
                        k += 1
                args = joined
            out.append(text[i:mm.end()] + ", ".join(args) + ")")
            i = end
        out.append(text[i:])
        return "".join(out)

    def declare(m):
        nm = "%s_%s" % (m.group(1), m.group(2))
        if nm not in use:
            return m.group(0)
        e = use[nm]
        conv = "__stdcall" if e.convention == "pascal" else "__cdecl"
        return "extern %s __far %s %s(%s);" % ("uint2" if e.ret16 else "uint4", conv, nm,
                                               ", ".join(wine16.c_type(a) for a in e.args) or "void")
    return _IMPORT.sub(declare, header), {fn: said(t) for fn, t in texts.items()}


def wine_definition(text, export):
    """`text` (as decompiled: parameters in stack order, a 4-byte one as its low word then its high)
    with the parameters of an exported function typed as Wine's .spec has them (`export`, a
    wine16.Export): each 4-byte parameter's two words one parameter -- a far pointer or a long --
    used as such where the two are passed on together, and as FP_OFF/FP_SEG (or its halves) where
    one is used alone. Unchanged when the words do not add up to the spec's parameters."""
    from codebreaker.symbols import wine16
    if not export or export.args is None:
        return text
    m = re.match(r"(?s)(.*?\b\w+\s*\()([\w\s,]*)(\)\s*\n)(.*?)(\{.*)", text)
    if not m:
        return text
    head, plist, close, decls, body = m.groups()
    params = [p.strip() for p in plist.split(",") if p.strip()]
    kinds = list(reversed(export.args)) if export.convention == "pascal" else list(export.args)
    if len(params) != sum(wine16.WIDTH[k] // 2 for k in kinds) or len(params) == len(kinds):
        return text
    out, k = [], 0
    for kind in kinds:
        if wine16.WIDTH[kind] == 2:
            out.append(params[k])
            k += 1
            continue
        lo, hi = params[k], params[k + 1]
        k += 2
        out.append(lo)
        ptr = kind in wine16.POINTER
        whole = "\x00%d\x00" % k                     # (the pair, kept out of the way while its halves change)
        body = re.sub(r"(?<![\w.])%s\s*,\s*%s\b" % (re.escape(lo), re.escape(hi)), whole, body)
        body = re.sub(r"(?<![\w.])%s\b" % re.escape(hi), ("FP_SEG(%s)" if ptr else "((uint2)((uint4)%s >> 16))") % whole, body)
        body = re.sub(r"(?<![\w.])%s\b" % re.escape(lo), ("FP_OFF(%s)" if ptr else "((uint2)%s)") % whole, body)
        body = body.replace(whole, lo)
        decls = re.sub(r"^\s*\w+\s+%s\s*;[ \t]*\n" % re.escape(hi), "", decls, flags=re.M)
        decls = re.sub(r"^(\s*)\w+(\s+)%s\s*;" % re.escape(lo), r"\1%s\2%s;" % (wine16.c_type(kind), lo), decls, flags=re.M)
    return head + ",".join(out) + close + decls + body


def sdk_header(header, sdk=None):
    """WINDOWS.H, then `header` (whose imports by number keep their own declarations: a call that
    cannot be said through the SDK's prototype -- sdk_calls -- stays as it was)"""
    sdk = sdk if sdk is not None else windows_h()
    return sdk + "\n" + header if sdk else header


_WIDE = re.compile(r"FAR\s*\*|\bLP|\bH?PSTR\b|\bLONG\b|\bDWORD\b|\bFARPROC\b|\bSEGPTR\b|\bCOLORREF\b|\bLRESULT\b")
_POINTERS = re.compile(r"FAR\s*\*|\bLP(?!ARAM\b)|\bH?PSTR\b|\bFARPROC\b|\bSEGPTR\b")


def sdk_prototypes(sdk=None):
    """{API name: [parameter type, ...]} of every WINAPI function WINDOWS.H declares on a line, or
    None for one with a variable argument list; a function that returns nothing has its name in the
    set `.void` of the result"""
    sdk = sdk if sdk is not None else windows_h()
    out = _Prototypes()
    for m in re.finditer(r"^([^\n#]*?)\bWINAPI\s+(\w+)\s*\(([^()]*)\)\s*;", sdk or "", re.M):
        params = [p.strip() for p in m.group(3).split(",")]
        out[m.group(2)] = None if "..." in params else [] if params in (["void"], ["VOID"], [""]) else params
        if re.fullmatch(r"\s*(void|VOID)\s*", m.group(1)):
            out.void.add(m.group(2))
    return out


class _Prototypes(dict):
    def __init__(self):
        super().__init__()
        self.void = set()


def sdk_calls(text, protos):
    """`text` (in Pascal order) with each call of an import by number that the SDK declares said by
    its name, `USER_33(...)` -> `GetClientRect(...)` -- the module's maker called the API through its
    prototype, which decides how every argument is passed. A call whose arguments are one to a
    parameter is renamed; one whose arguments are one to a word of the parameters has each 4-byte
    parameter's two words (high first, as pushed) joined -- MAKELP for a pointer, MAKELONG for a
    number; any other call keeps its number and its declaration without parameters."""
    from codebreaker.symbols import ordinals
    out, i = [], 0
    call = re.compile(r"\b([A-Z][A-Z0-9]*)_(\d+)\s*\(")
    while True:
        m = call.search(text, i)
        if not m:
            out.append(text[i:])
            return "".join(out)
        end = _close(text, m.end())
        nm = ordinals.name(m.group(1), int(m.group(2)))
        params = protos.get(nm) if nm else None
        if params is not None and nm in getattr(protos, "void", ()) and not (
                re.search(r"(^|[;{}])\s*$", text[:m.start()]) and re.match(r"\s*;", text[end:])):
            params = None                            # (a value taken from a function of no value: not the API)
        if end < 0 or params is None:
            out.append(text[i:m.end()])
            i = m.end()
            continue
        args = [sdk_calls(a, protos).strip() for a in _arguments(text[m.end():end - 1])]
        wide = [bool(_WIDE.search(p)) for p in params]
        if len(args) == len(params):
            said = args
        elif len(args) == len(params) + sum(wide):
            said, k = [], 0
            for p, w in zip(params, wide):
                if w:
                    hi, lo = args[k], args[k + 1]
                    said.append(("MAKELP(%s, %s)" if _POINTERS.search(p) else "MAKELONG(%s, %s)") % (
                        (hi, lo) if _POINTERS.search(p) else (lo, hi)))
                    k += 2
                else:
                    said.append(args[k])
                    k += 1
        else:
            out.append(text[i:m.end()])
            i = m.end()
            continue
        out.append(text[i:m.start()] + nm + "(" + ", ".join(said) + ")")
        i = end


_DECL = re.compile(r"^\s+(?:[\w]+\s+)+\**\s*(\w+)\s*;\s*$")
_ASSIGN = re.compile(r"^(\s*)(\w+) = (.+);\s*$")
_LOOP = re.compile(r"^\s*(?:\}\s*)?(?:while|for|do)\b")


def fold_temporaries(text):
    """`text` with each temporary the decompiler made -- a local assigned once and used once, in the
    statement right after, in the same block -- put back into the expression it came from:
    `n = lstrlen(s); p = LocalAlloc(0x40, n + 1);` -> `p = LocalAlloc(0x40, lstrlen(s) + 1);`,
    `iVar1 = f(x); return iVar1;` -> `return f(x);`. The source wrote one expression; the compiler
    evaluates it in the source's order (a pascal call's arguments first to last), which the
    temporaries change. Not into a loop's condition (that is evaluated again each time round)."""
    lines = text.split("\n")
    locals_ = set()
    body = 0
    for k, ln in enumerate(lines):
        if ln.strip() == "{" and not body:
            body = k + 1
            continue
        if body and k >= body:
            m = _DECL.match(ln)
            if m and "(" not in ln and "=" not in ln:
                locals_.add(m.group(1))
            elif re.match(r"^\s+(?:\w+\s+)+\**\s*\w+ = [^;]*;\s*$", ln):
                continue                                 # (one declared with its value: not a temporary)
            elif ln.strip():
                break
    changed = True
    while changed:
        changed = False
        for k, ln in enumerate(lines):
            m = _ASSIGN.match(ln)
            if not m or m.group(2) not in locals_:
                continue
            name, expr = m.group(2), m.group(3)
            word = re.compile(r"(?<![\w.>])%s\b(?!\s*\[)" % re.escape(name))
            decl = _decl_line(lines, name)
            uses = [j for j, other in enumerate(lines) if j not in (k, decl) and word.search(other)]
            nxt = next((i for i in range(k + 1, len(lines)) if lines[i].strip()), None)
            if not uses or nxt is None:
                continue
            again = _ASSIGN.match(lines[nxt])
            constant = re.fullmatch(r"-?(?:0x[0-9a-fA-F]+|\d+)", expr.strip())
            if again and again.group(2) == name and len(word.findall(again.group(3))) == 1:
                j = nxt                                  # `v = f(); v = g(v + 1);`: the first value dies there
            elif len(uses) == 1 and uses[0] == nxt:
                j = nxt
            elif constant and len(uses) == 1 and _same_block(lines, k, uses[0]):
                j = uses[0]                              # a constant moves past what comes between
            else:
                continue
            target = _ASSIGN.match(lines[j])
            head, tail = (lines[j][:target.start(3)], target.group(3) + ";") if target and \
                target.group(2) == name and j == nxt and again else ("", lines[j])
            if len(word.findall(tail)) != 1 or _LOOP.match(lines[j]) or re.match(r"\s*\}", lines[j]):
                continue
            indent = len(lines[j]) - len(lines[j].lstrip())
            if indent != len(m.group(1)):
                continue
            simple = re.fullmatch(r"[\w.]+|0x[0-9a-fA-F]+|-?\d+|[\w.]+\([^()]*\)", expr.strip())
            lines[j] = head + word.sub(lambda _m: expr if simple else "(%s)" % expr, tail, count=1)
            del lines[k]
            changed = True
            break
    out = "\n".join(lines)
    for name in sorted(locals_):                     # a declaration left with nothing to declare
        if len(re.findall(r"(?<![\w.>])%s\b" % re.escape(name), out)) == 1:
            out = re.sub(r"\n\s+(?:[\w]+\s+)+\**\s*%s\s*;[ \t]*(?=\n)" % re.escape(name), "", out, count=1)
    return out


def join_far_parameters(text):
    """`text` with each pair of a function's own parameters that is used only as a pair -- the
    lower one (the offset) then the next (its segment), side by side in an argument list -- made
    the one far pointer the source had: `f(a1,a2) int2 a1; int2 a2; ... lstrlen(a1,a2)` ->
    `f(a1) void __far *a1; ... lstrlen(a1)`. The words go on the stack as they did; what changes is
    that the compiler knows them for one value (it pushes them straight from the frame, not through
    registers, and a prototype takes them as the pointer they are)."""
    m = re.match(r"(?s)(.*?\b\w+\s*\()([\w\s,]*)(\)\s*\n)(.*?)(\{.*)", text)
    if not m:
        return text
    head, plist, close, decls, body = m.groups()
    params = [p.strip() for p in plist.split(",") if p.strip()]
    joined = []
    for lo, hi in zip(params, params[1:]):
        if lo in joined or hi in joined or any(lo in j or hi in j for j in joined):
            continue
        pair = re.compile(r"\b%s\s*,\s*%s\b" % (re.escape(lo), re.escape(hi)))
        alone = re.compile(r"(?<![\w.])(%s|%s)\b" % (re.escape(lo), re.escape(hi)))
        pairs = len(pair.findall(body))
        if pairs and len(alone.findall(body)) == 2 * pairs and re.search(r"^\s*\w+\s+%s\s*;" % re.escape(hi), decls, re.M):
            joined.append((lo, hi))
    if not joined:
        return text
    for lo, hi in joined:
        params.remove(hi)
        body = re.sub(r"\b%s\s*,\s*%s\b" % (re.escape(lo), re.escape(hi)), lo, body)
        decls = re.sub(r"^\s*\w+\s+%s\s*;[ \t]*\n" % re.escape(hi), "", decls, flags=re.M)
        decls = re.sub(r"^(\s*)\w+(\s+)%s\s*;" % re.escape(lo), r"\1void __far *\2%s;" % lo, decls, flags=re.M)
    return head + ",".join(params) + close + decls + body


_SIZES = {"char": 1, "int1": 1, "uint1": 1, "xunknown1": 1, "bool": 1, "undefined": 1, "undefined1": 1,
          "int2": 2, "uint2": 2, "xunknown2": 2, "undefined2": 2, "short": 2,
          "int4": 4, "uint4": 4, "xunknown4": 4, "undefined4": 4, "long": 4}
_FRAME = re.compile(r"\n[ \t]*_Packed struct \{\n((?:[ \t]*[\w ]+\s+\w+\s*(?:\[\d+\])?\s*;\n)+)[ \t]*\} codebreaker_frame;")
_MEMBER = re.compile(r"([\w ]+?)\s+(\w+?_([0-9a-f]+))\s*(?:\[(\d+)\])?\s*;")
_AT = re.compile(r"(&)?\(\*\((\w+) \*\)\(\(uint1 \*\)codebreaker_frame\.(\w+) \+ (0x[0-9a-f]+|\d+)\)\)")
_INDEX = re.compile(r"codebreaker_frame\.(\w+)\[(0x[0-9a-f]+|\d+)\]")
_PTR = re.compile(r"\(codebreaker_frame\.(\w+) \+ (-?(?:0x[0-9a-f]+|\d+))\)")
_BARE = re.compile(r"codebreaker_frame\.(\w+)\b")


def _number(text):
    """a C number as the decompiler writes it: 0x... hexadecimal, else decimal (a leading zero is not octal here, and not an error)"""
    return int(text, 16) if text.lstrip("-").lower().startswith("0x") else int(text)


def split_frame(text):
    """`text` with the decompiler's frame structure -- every local of a function in one packed
    structure of byte arrays, reached as `(*(int2 *)((uint1 *)codebreaker_frame.axStack_12 + 8))` --
    taken apart into the locals a source declares: a variable for each place read or written (of the
    widest type it is read as), a buffer for each place whose address is taken (to the next variable).
    The structure keeps the frame's layout for a build; a compiler given locals lays them out itself,
    as it did the source's. Declared nearest the frame pointer first (search16 tries other orders).
    A function whose frame is reached in a way not read here is left as it was."""
    fm = _FRAME.search(text)
    if not fm:
        return text
    members = {}
    for mm in _MEMBER.finditer(fm.group(1)):
        typ, name, hexoff, count = mm.group(1).strip(), mm.group(2), mm.group(3), mm.group(4)
        members[name] = (-int(hexoff, 16), typ, int(count) if count else 1)
    if not members:
        return text
    body = text[fm.end():]
    scalars, starts = {}, set()
    for amp, typ, mem, k in _AT.findall(body):
        if mem not in members or typ not in _SIZES:
            return text
        at = members[mem][0] + _number(k)
        scalars[at] = max(scalars.get(at, 0), _SIZES[typ])
        if amp:
            starts.add(at)
    for mem, k in _INDEX.findall(body):
        if mem not in members:
            return text
        base, typ, _n = members[mem]
        at = base + _number(k) * _SIZES.get(typ, 1)
        scalars[at] = max(scalars.get(at, 0), _SIZES.get(typ, 1))
    for mem, k in _PTR.findall(body):                 # (a member's address moved on: a place of its own)
        if mem not in members:
            return text
        starts.add(members[mem][0] + _number(k))
    rest = _PTR.sub("", _INDEX.sub("", _AT.sub("", body)))
    for mem in _BARE.findall(rest):
        if mem not in members:
            return text
        starts.add(members[mem][0])
    low = min(b for b, _t, _n in members.values())
    high = max(b + _SIZES.get(t, 1) * n for b, t, n in members.values())
    points = sorted(set(scalars) | starts)
    if not points:
        return text
    var = {}
    for i, at in enumerate(points):
        end = points[i + 1] if i + 1 < len(points) else high
        span = end - at
        name = "local_%x" % -at if at < 0 else "local_p%x" % at
        if at in starts and (at not in scalars or span > scalars[at]):
            var[at] = (name, "xunknown1", span)                      # a buffer: its address is passed on
        else:
            size = scalars.get(at, span)
            if size > span:
                return text                                           # (reads that overlap: not locals)
            var[at] = (name, {1: "xunknown1", 2: "int2", 4: "int4"}.get(size, "xunknown1"), 1 if size in (1, 2, 4) else span)

    def place(at):
        """the variable holding `at`, and how far into it"""
        b = max(p for p in points if p <= at)
        return var[b], at - b

    def at_text(m):
        amp, typ, mem, k = m.groups()
        (name, vtyp, n), d = place(members[mem][0] + _number(k))
        if not d and n == 1 and vtyp == typ:
            ref = name
        elif not d and n == 1 and _SIZES.get(vtyp) == _SIZES[typ]:
            ref = "(*(%s *)&%s)" % (typ, name)
        else:
            ref = "(*(%s *)((uint1 *)&%s + %d))" % (typ, name, d)
        return "&" + ref if amp else ref

    def index_text(m):
        mem, k = m.groups()
        base, typ, _n = members[mem]
        (name, vtyp, n), d = place(base + _number(k) * _SIZES.get(typ, 1))
        return name if not d and n == 1 and vtyp == typ else "(*(%s *)((uint1 *)&%s + %d))" % (typ, name, d)

    def bare_text(m):
        base, typ, _n = members[m.group(1)]
        (name, _vt, _n2), d = place(base)
        return "((%s *)((uint1 *)&%s + %d))" % (typ, name, d) if d else "((%s *)&%s)" % (typ, name)
    def ptr_text(m):
        base, typ, _n = members[m.group(1)]
        (name, _vt, _n2), d = place(base + _number(m.group(2)))
        return "((%s *)((uint1 *)&%s + %d))" % (typ, name, d) if d else "((%s *)&%s)" % (typ, name)
    body = _BARE.sub(bare_text, _PTR.sub(ptr_text, _INDEX.sub(index_text, _AT.sub(at_text, body))))
    decls = "".join("\n  %s %s%s;" % (t, n, "[%d]" % c if c > 1 or t == "xunknown1" and c > 1 else "")
                    for _at, (n, t, c) in sorted(var.items(), key=lambda kv: -kv[0]))
    return text[:fm.start()] + decls + body


def _same_block(lines, a, b):
    """lines a < b are in one block: nothing between them closes it, opens one, or is a label"""
    indent = len(lines[a]) - len(lines[a].lstrip())
    for ln in lines[a + 1:b]:
        s = ln.strip()
        if not s:
            continue
        if len(ln) - len(ln.lstrip()) != indent or s.endswith(("{", ":")) or s.startswith("}"):
            return False
    return True


def _decl_line(lines, name):
    for i, ln in enumerate(lines):
        m = _DECL.match(ln)
        if m and m.group(1) == name:
            return i
    return -1


_DEREF = re.compile(r"\*\s*\(\s*((?:unsigned |signed )?\w+)\s*\*\s*\)\s*(0x[0-9a-fA-F]+|\d+)\b")


def ds_globals(text):
    """(declarations, text): every read or write through a constant near address, `*(T *)0x63b2`,
    made one through a variable of the data segment, `(*(T *)codebreaker_d63b2)`. Microsoft C
    addresses a variable directly (`mov ax, [63b2]`, the offset a fix-up); a constant pointer it loads
    into a register first -- the source had the variable. One variable per address, read as each
    place reads it."""
    seen = set()

    def var(m):
        typ, at = m.group(1), _number(m.group(2))
        if at < 0x10 or typ in ("void", "code"):
            return m.group(0)
        nm = "codebreaker_d%04x" % at
        seen.add(nm)
        return "(*(%s *)%s)" % (typ, nm)
    out = _DEREF.sub(var, text)
    return "".join("extern char %s[];\n" % n for n in sorted(seen)), out


def function_code(obj, name):
    """(bytes, mask) of the public function `name` in an OMF object file -- cut from its code segment
    between its public name and the next -- or None. A header's own functions (static helpers, the
    reconstruction's stubs) come first in the segment, so the first code segment is not the function."""
    from codebreaker.formats import omf
    want = name.lstrip("_@").lower()             # (a C name _f, a fastcall one @f)
    for pub, (code, holes) in omf.function_bytes(obj).items():
        if pub.lstrip("_@").lower() == want:
            mask = bytearray(len(code))
            for at in holes:
                if at < len(mask):
                    mask[at] = 1
            return bytes(code), bytes(mask)
    return None


def _native(compiler):
    """(the compiler's folder, a work folder), both by short paths, when `compiler` runs on Windows
    itself -- else None. Visual C++ 1.52c's CL, C1 and C2 (C/C++ 8.00c) are Win32 console programs
    (KERNEL32 only; the Phar Lap stub is for DOS), and run on 64-bit Windows given two things:
    paths short enough for a buffer in CL (a long one overruns it: CL.EXE+0xC46B reads part of the
    path as a pointer) and an environment small enough for its pass launcher (with a modern
    environment it says "Out of memory" before starting C2). The short paths are directory
    junctions, made the first time (settings: native16_tools, native16_work; native16 = off keeps
    the emulator). Microsoft C 6.0a is a DOS program: always the emulator."""
    from codebreaker.base import settings
    if compiler != "msvc152" or str(settings.get("native16") or "").lower() in ("off", "0", "false", "no"):
        return None
    _emulator, home = _tools(compiler)
    if not home or not os.path.isfile(os.path.join(home, "BIN", "CL.EXE")):
        return None
    profile = os.environ.get("USERPROFILE") or os.path.expanduser("~")
    tools = settings.get("native16_tools") or os.path.join(profile, ".cbt")
    root = settings.get("native16_work") or os.path.join(profile, ".cbw")
    if not os.path.isdir(tools):
        subprocess.run(["cmd", "/c", "mklink", "/J", tools, os.path.abspath(home)],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    os.makedirs(root, exist_ok=True)
    if not os.path.isfile(os.path.join(tools, "BIN", "CL.EXE")):
        return None
    return tools, root


def _native_session(work, header, files, flags, tools, root, timeout):
    """`files` compiled by the compiler run on Windows itself, one CL a file, in a short work folder;
    the objects and messages copied back into `work` as an emulator session leaves them."""
    import hashlib
    import shutil
    short = os.path.join(root, hashlib.sha1(os.path.abspath(work).encode()).hexdigest()[:8])
    for folder in (work, short):
        os.makedirs(folder, exist_ok=True)
        for f in os.listdir(folder):
            if f.lower().endswith((".obj", ".c", ".txt", ".h")):
                os.remove(os.path.join(folder, f))
    with open(os.path.join(short, "M.H"), "w", newline="\r\n", encoding="latin1", errors="replace") as f:
        f.write(header)
    env = {"PATH": os.path.join(tools, "BIN"), "INCLUDE": os.path.join(tools, "INCLUDE"), "TMP": short, "TEMP": short,
           "SystemRoot": os.environ.get("SystemRoot", r"C:\Windows")}
    for stem, text in files:
        with open(os.path.join(short, stem + ".C"), "w", newline="\r\n", encoding="latin1", errors="replace") as f:
            f.write('#include "m.h"\n\n' + text + "\n")
        try:
            r = subprocess.run([os.path.join(tools, "BIN", "CL.EXE"), "/nologo", "/c", "/W0"] + flags.split() +
                               ["/Fo%s.OBJ" % stem, stem + ".C"], cwd=short, env=env, capture_output=True, timeout=timeout)
            said = (r.stdout or b"") + (r.stderr or b"")
        except subprocess.TimeoutExpired:
            said = b"(timed out)"
        with open(os.path.join(work, stem + ".TXT"), "wb") as f:
            f.write(said)
        if os.path.isfile(os.path.join(short, stem + ".OBJ")):
            shutil.copyfile(os.path.join(short, stem + ".OBJ"), os.path.join(work, stem + ".OBJ"))
    return True


def _session(work, header, files, flags, compiler, timeout):
    """One emulator session compiling `files` [(stem, C text)] -> whether it ran to its end. A compiler
    that runs on Windows itself (_native) is run so instead -- no emulator."""
    native = _native(compiler)
    if native:
        return _native_session(work, header, files, flags, native[0], native[1], timeout)
    from codebreaker.build import dos_tools
    _emulator, home = _tools(compiler)
    os.makedirs(work, exist_ok=True)
    for f in os.listdir(work):
        if f.lower().endswith((".obj", ".c", ".txt")):
            os.remove(os.path.join(work, f))
    with open(os.path.join(work, "M.H"), "w", newline="\r\n", encoding="latin1", errors="replace") as f:
        f.write(header)
    lines = []
    for stem, text in files:
        with open(os.path.join(work, stem + ".C"), "w", newline="\r\n", encoding="latin1", errors="replace") as f:
            f.write('#include "m.h"\n\n' + text + "\n")
        lines.append("cl /nologo /c /W0 %s /Fo%s.OBJ %s.C > %s.TXT" % (flags, stem, stem, stem))
    dos_tools.run({"c": home, "d": work}, lines, [], timeout=timeout, work=work,
                  cpu={"core": "dynamic", "cputype": "386", "cycles": "max"},
                  variables={"PATH": r"C:\BIN", "INCLUDE": r"C:\INCLUDE", "LIB": r"C:\LIB"})
    return os.path.isfile(os.path.join(work, "done.txt"))


def _read(path):
    if os.path.isfile(path):
        with open(path, "rb") as f:
            return f.read()
    return None


BATCH = 40          # functions to a source file when each is cut out by name


def compile_many(sources, header, flags, work=None, timeout=900, compiler="msc6", names=None, batch=BATCH,
                 together=None):
    """Compile each of `sources` {key: C text} with `header` before it. -> {key: (bytes, mask) or None}
    names: {key: function name} -- each key's code is that public function's (function_code), not the
    object's first code segment.

    With names, functions share source files: `together` [[key, ...], ...] says which (the source
    files of the original, as far as they are known), else `batch` at a time in their order. Starting
    the compiler's passes (CL, C1, C2) is most of the cost of compiling a function in the emulator.
    What a function compiles to depends on its file: a far call to a function defined in the same
    file is `push cs; call` (near, the segment known), to one elsewhere `lcall` -- so a function's
    result is kept (cached) for its whole file, and a file with one function not yet known is compiled
    again whole. A file the compiler refuses has the functions its errors are in compiled on their own,
    the rest together, so one function cannot sink the rest."""
    from codebreaker.base import cache, settings
    if not available(compiler):
        raise RuntimeError("%s is not set up (settings.json: dosbox_x, %s)" % (compiler, compiler))
    out, todo = {}, {}
    _emulator, home = _tools(compiler)
    stamp = cache.tool(os.path.join(home, "BIN", "CL.EXE"))
    named = [k for k in sources if names and k in names]
    if names and batch > 1:
        files_of = [[k for k in g if k in sources] for g in together] if together else \
            [named[i:i + batch] for i in range(0, len(named), batch)]
        seen = {k for g in files_of for k in g}
        files_of = [g for g in files_of if g] + [[k] for k in named if k not in seen]
    else:
        files_of = [[k] for k in named]
    context = {}
    for g in files_of:
        whole = cache.key(*[sources[k] for k in g]) if len(g) > 1 else ""
        for k in g:
            context[k] = whole
    for key, text in sources.items():
        k = cache.key(stamp, flags, header, text, "function " + names[key] if names and key in names else "",
                      context.get(key, ""))
        had = cache.get("msc6", k)
        if had is not None:
            out[key] = None if had == b"-" else (had[4:4 + int.from_bytes(had[:4], "little")],
                                                 had[4 + int.from_bytes(had[:4], "little"):])
        todo[key] = (k, text)
    for g in files_of:                               # (a file not wholly known is compiled again whole)
        if all(k in out for k in g):
            for k in g:
                todo.pop(k, None)
    for key in list(todo):
        if key not in context and key in out:
            todo.pop(key)
    if not todo:
        return out
    keep_files = [[k for k in g if k in todo] for g in files_of]
    keep_files = [g for g in keep_files if g]
    work = os.path.abspath(work or settings.state(compiler, "work"))
    keys = list(todo)
    built, unsettled = {}, set()             # (unsettled: no verdict -- the session was cut off -- so not cached)
    single = [k for k in keys if not names or k not in names]
    groups = keep_files if names else []
    # A refused file: the functions its errors are in go on their own, the rest together again --
    # round after round, for the compiler stops at a file's first errors and a second bad function
    # shows only once the first is out (one emulator session a round).
    for _round in range(6):
        if not groups:
            break
        files, starts = _batch_files(groups, todo, "B")
        finished = _session(work, header, files, flags, compiler, timeout)
        again = []
        for i, g in enumerate(groups):
            obj = _read(os.path.join(work, "B%d.OBJ" % i))
            if obj is not None:
                built.update((k, function_code(obj, names[k])) for k in g)
            elif not finished:
                unsettled.update(g)
            elif len(g) == 1:
                built[g[0]] = None                   # (refused on its own: it does not compile)
            else:
                bad = _refused(os.path.join(work, "B%d.TXT" % i), starts[i])
                if not bad:
                    again += [[k] for k in g]        # (errors not told apart: each on its own)
                    continue
                again += [[k] for k in bad]
                rest = [k for k in g if k not in bad]
                if rest:
                    again.append(rest)
        groups = again
    for g in groups:
        unsettled.update(g)                          # (still refused after every round: no verdict)
    if single:
        files = [("F%d" % j, todo[k][1]) for j, k in enumerate(single)]
        finished = _session(work, header, files, flags, compiler, timeout)
        for j, k in enumerate(single):
            obj = _read(os.path.join(work, "F%d.OBJ" % j))
            if obj is None:
                built[k] = None
                if not finished:
                    unsettled.add(k)
            elif names and k in names:
                built[k] = function_code(obj, names[k])
            else:
                code, mask = object_code(obj)
                built[k] = (code, mask) if code else None
    for key, (k, _text) in todo.items():
        got = out[key] = built.get(key)
        if got:
            cache.put("msc6", k, len(got[0]).to_bytes(4, "little") + got[0] + got[1])
        elif key in built and key not in unsettled:
            cache.put("msc6", k, b"-")                       # the compiler refused it (not: the session was cut off)
    return out


def _batch_files(groups, todo, prefix):
    """([(stem, text)], [[(first line, key)]]): each group's functions in one file, and the line each
    begins on (the file starts with the #include and a blank line)"""
    files, starts = [], []
    for i, g in enumerate(groups):
        at, here = 3, []
        for k in g:
            here.append((at, k))
            at += todo[k][1].count("\n") + 2         # (its lines, and the blank one between)
        files.append(("%s%d" % (prefix, i), "\n\n".join(todo[k][1] for k in g)))
        starts.append(here)
    return files, starts


def _refused(path, starts):
    """the keys of the functions a refused file's errors are in -- [] when an error is not in the file
    itself (the header's: then none can be told from another)"""
    try:
        text = open(path, encoding="latin1").read()
    except OSError:
        return []
    bad = []
    for name, line in re.findall(r"^(\S+)\((\d+)\) : (?:fatal )?error", text, re.M):
        if not re.match(r"[BR]\d+\.C$", os.path.basename(name), re.I):
            return []
        at = int(line)
        k = max((s for s in starts if s[0] <= at), default=None)
        if k and k[1] not in bad:
            bad.append(k[1])
    return bad
