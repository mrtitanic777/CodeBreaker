#!/usr/bin/env python3
r"""raise95.py -- the Ghidra-to-authored-C "raise" pass.

Bridges the gap CodeBreaker exists to close: Ghidra decompiles machine code into
*machine-flavored* C (pseudo-types, CONCAT/SUB intrinsics, sub-field packing, uppercased
imports, DAT_ globals). This turns that into *authored-style period C* -- real Win16 types,
windows.h field macros, real API casing, and (given a per-module model) named struct fields
and parameters. Deterministic source-to-source; no full C parser (Ghidra's output is regular).

Three layers, applied in order:
  1. mechanical (context-free): pseudo-types -> Win16 types; CONCAT/SUB/ZEXT/SEXT + `._o_s_`
     field access -> MAKELONG/MAKEWORD/LOWORD/HIWORD/LOBYTE/HIBYTE/casts; _LREAD -> _lread.
  2. structural (per-module map): DAT_<addr> -> struct fields; far-ptr CONCAT22 -> MAKELP.
  3. semantic (light): param_1.. -> real names; `while( true )` -> `for (;;)`.

    raise_c(text, struct_map=None, param_names=None, api_map=None) -> raised text
    python -m src.targets.win95.raise95 <ghidra.c> [out.c]      # whole-file demo
"""
import re
import sys

# --- layer 1 maps ---------------------------------------------------------------------------
_TYPE_MAP = {
    "undefined8": "DWORD", "undefined4": "DWORD", "undefined2": "WORD", "undefined1": "BYTE",
    "undefined": "BYTE", "uint3": "DWORD", "uint": "UINT", "ushort": "WORD", "ulong": "DWORD",
    "uchar": "BYTE", "byte": "BYTE", "sbyte": "char", "bool": "BOOL", "code": "void",
}
# Win16 API real casing: Ghidra shows the NE import table's uppercased form. Seed of the common
# KERNEL/USER/GDI entries; extendable (ultimately auto-derivable from the SDK/DDK INC16 headers).
_API_MAP = {
    "_LREAD": "_lread", "_LWRITE": "_lwrite", "_LOPEN": "_lopen", "_LCREAT": "_lcreat",
    "_LCLOSE": "_lclose", "_LLSEEK": "_llseek", "_HREAD": "_hread", "_HWRITE": "_hwrite",
    "GLOBALALLOC": "GlobalAlloc", "GLOBALFREE": "GlobalFree", "GLOBALLOCK": "GlobalLock",
    "GLOBALUNLOCK": "GlobalUnlock", "GLOBALREALLOC": "GlobalReAlloc", "GLOBALHANDLE": "GlobalHandle",
    "GLOBALSIZE": "GlobalSize", "GLOBALFLAGS": "GlobalFlags", "GLOBALCOMPACT": "GlobalCompact",
    "LOCALALLOC": "LocalAlloc", "LOCALFREE": "LocalFree", "LOCALLOCK": "LocalLock",
    "LOCALUNLOCK": "LocalUnlock", "LOCALREALLOC": "LocalReAlloc",
    "LSTRLEN": "lstrlen", "LSTRCPY": "lstrcpy", "LSTRCAT": "lstrcat", "LSTRCMP": "lstrcmp",
    "OPENFILE": "OpenFile", "GETPROCADDRESS": "GetProcAddress", "GETMODULEHANDLE": "GetModuleHandle",
    "GETMODULEFILENAME": "GetModuleFileName", "GETVERSION": "GetVersion", "GETDOSENVIRONMENT": "GetDOSEnvironment",
    "MAKEPROCINSTANCE": "MakeProcInstance", "FREEPROCINSTANCE": "FreeProcInstance",
    "GETPRIVATEPROFILEINT": "GetPrivateProfileInt", "GETPRIVATEPROFILESTRING": "GetPrivateProfileString",
    "GETTEMPFILENAME": "GetTempFileName", "GETTEMPDRIVE": "GetTempDrive",
    "DOS3CALL": "DOS3Call", "GETCURRENTTASK": "GetCurrentTask", "GETWINFLAGS": "GetWinFlags",
}


def _split_args(s):
    """Split a call's argument text on top-level commas (paren/bracket aware)."""
    args, depth, cur = [], 0, ""
    for ch in s:
        if ch in "([{":
            depth += 1; cur += ch
        elif ch in ")]}":
            depth -= 1; cur += ch
        elif ch == "," and depth == 0:
            args.append(cur); cur = ""
        else:
            cur += ch
    if cur.strip() != "" or args:
        args.append(cur)
    return [a.strip() for a in args]


def _rewrite_calls(text, fname_re, fn):
    """Find NAME( ... ) whose NAME matches fname_re (a compiled regex anchored at the token),
    with balanced parens, and replace via fn(name, [args]) -> str. Left-to-right, one pass."""
    out, i, n = "", 0, len(text)
    tok = re.compile(fname_re)
    while i < n:
        m = tok.search(text, i)
        if not m:
            out += text[i:]; break
        j = m.start()
        # whole-token check on the left
        if j > 0 and (text[j - 1].isalnum() or text[j - 1] == "_"):
            out += text[i:m.end()]; i = m.end(); continue
        # m.end() sits just after the '(' (the regex includes it)
        k, depth = m.end(), 1
        while k < n and depth > 0:
            if text[k] == "(":
                depth += 1
            elif text[k] == ")":
                depth -= 1
            k += 1
        inner = text[m.end():k - 1]
        out += text[i:j] + fn(m, _split_args(inner))
        i = k
    return out


def _size_cast(sz):
    return {1: "BYTE", 2: "WORD", 4: "DWORD"}.get(sz, "DWORD")


def _mechanical(text):
    # -- intrinsics (loop to a fixpoint so nested CONCAT/SUB unwrap) --
    for _ in range(8):
        before = text
        # CONCAT22(hi,lo): a DWORD (hi<<16|lo). If cast to a pointer just before -> a far pointer.
        def _concat22(m, a):
            if len(a) != 2:
                return m.group(0) + ",".join(a) + ")"  # leave odd forms
            return "MAKELONG(%s,%s)" % (a[1], a[0])
        text = _rewrite_calls(text, r"CONCAT22\(", _concat22)
        text = _rewrite_calls(text, r"CONCAT11\(",
                              lambda m, a: "MAKEWORD(%s,%s)" % (a[1], a[0]) if len(a) == 2 else m.group(0))
        # SUB<from><to>(v, off): low <to> bytes of (v >> off*8)
        def _sub(m, a):
            to = int(m.group(1)[1]); off = 0
            if len(a) >= 2:
                try:
                    off = int(a[1], 0)
                except ValueError:
                    off = 0
            v = a[0]
            if off == 0:
                return "(%s)(%s)" % (_size_cast(to), v)
            if off == 2 and to == 2:
                return "HIWORD(%s)" % v
            return "(%s)((%s)>>%d)" % (_size_cast(to), v, off * 8)
        text = _rewrite_calls(text, r"SUB(\d)(\d)\(", _sub)
        # ZEXT<from><to>(v)/SEXT: widen
        text = _rewrite_calls(text, r"ZEXT(\d)(\d)\(",
                              lambda m, a: "(%s)(%s)" % (_size_cast(int(m.group(1)[len(m.group(1))-1])), a[0]))
        text = _rewrite_calls(text, r"SEXT(\d)(\d)\(",
                              lambda m, a: "(%s)(%s)" % ({1: "char", 2: "short", 4: "long"}.get(int(m.group(1)[-1]), "long"), a[0]))
        if text == before:
            break
    # -- sub-field packing: IDENT._o_s_ -> field macros --
    def _field(m):
        base, off, sz = m.group(1), int(m.group(2)), int(m.group(3))
        if (off, sz) == (0, 2):
            return "LOWORD(%s)" % base
        if (off, sz) == (2, 2):
            return "HIWORD(%s)" % base
        if (off, sz) == (0, 1):
            return "LOBYTE(%s)" % base
        if (off, sz) == (0, 4):
            return "(DWORD)(%s)" % base
        if sz == 1 and off == 1:
            return "HIBYTE(LOWORD(%s))" % base
        return "(%s)((%s)>>%d)" % (_size_cast(sz), base, off * 8)
    text = re.sub(r"([A-Za-z_]\w*)\._(\d+)_(\d+)_", _field, text)
    # -- types (longest token first so 'undefined2' beats 'undefined') --
    for t in sorted(_TYPE_MAP, key=len, reverse=True):
        text = re.sub(r"\b%s\b" % re.escape(t), _TYPE_MAP[t], text)
    # -- API de-uppercasing --
    for up, real in _API_MAP.items():
        text = re.sub(r"\b%s\b" % re.escape(up), real, text)
    return text


def _structural(text, struct_map):
    """DAT_<addr> / _DAT_<addr> -> struct-field expressions from a per-module map. A far-pointer
    CONCAT22 that lands in a pointer context is left as MAKELONG here (curator picks MAKELP)."""
    if not struct_map:
        return text
    # apply longest keys first to avoid prefix collisions
    for name in sorted(struct_map, key=len, reverse=True):
        field = struct_map[name]
        text = re.sub(r"\b_?%s\b" % re.escape(name), field, text)
    return text


def _semantic(text, param_names):
    if param_names:
        for i, nm in enumerate(param_names, 1):
            if nm:
                text = re.sub(r"\bparam_%d\b" % i, nm, text)
    text = re.sub(r"while\(\s*true\s*\)", "for (;;)", text)
    text = re.sub(r"while\s*\(\s*true\s*\)", "for (;;)", text)
    return text


def map_types(s):
    """Rewrite just the Ghidra pseudo-types to Win16 types (for signatures / header decls)."""
    for t in sorted(_TYPE_MAP, key=len, reverse=True):
        s = re.sub(r"\b%s\b" % re.escape(t), _TYPE_MAP[t], s)
    return s


def _load_ne():
    try:
        from src.targets.win95 import neraise95
        return neraise95
    except Exception:
        import importlib.util as _il, os as _os
        _sp = _il.spec_from_file_location("neraise95", _os.path.join(_os.path.dirname(__file__), "neraise95.py"))
        _m = _il.module_from_spec(_sp); _sp.loader.exec_module(_m); return _m


def raise_c(text, struct_map=None, param_names=None, api_map=None,
            far_pairs=None, far_names=None, far_types=None,
            dword_pairs=None, dword_names=None, clean_frames=False):
    """Raise a block of Ghidra decompile C toward authored period C. Layers: mechanical, then
    (NE) far-pointer + 32-bit-pair coalescing, structural DAT_->name, light semantic, and
    (NE) frame-artifact cleanup."""
    if api_map:
        _API_MAP.update(api_map)
    text = _mechanical(text)
    if (far_pairs and far_names) or (dword_pairs and dword_names) or clean_frames:
        ne = _load_ne()
        if far_pairs and far_names:
            text = ne.coalesce_far_pointers(text, far_pairs, far_names, far_types)
        if dword_pairs and dword_names:
            text = ne.coalesce_dwords(text, dword_pairs, dword_names)
    text = _structural(text, struct_map)
    text = _semantic(text, param_names)
    if clean_frames:
        text = ne.clean_frame_artifacts(text)
    return text


def main(argv):
    if not argv:
        print(__doc__); return 2
    src = open(argv[0], encoding="latin1").read()
    out = raise_c(src)
    if len(argv) > 1:
        open(argv[1], "w", encoding="utf-8").write(out)
        print("raised -> %s" % argv[1])
    else:
        sys.stdout.write(out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
