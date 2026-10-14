"""readable.py -- names in place of numbers and placeholders, in a function's written source.

    from codebreaker.source import readable
    r = readable.Module(texts, string_at)      texts: {function name: C text}, the whole module;
                                               string_at(address) -> the module's string there, or None
    text = r.function(name)                    the text with the names below written in
    r.constants_used                           {NAME: value} of the SDK constants written in
    readable.calls_hint(text)                  "registry: RegOpenKeyExA, RegQueryValueExA" or ""

What is changed, and only where the evidence says so:

    constants   a number passed to a Windows function where the SDK names that parameter's
                values: LocalAlloc(0x40, n) -> LocalAlloc(LMEM_ZEROINIT, n); MessageBoxA(..., 0x24)
                -> MB_YESNO | MB_ICONQUESTION. Only when the value is exactly one constant of the
                parameter's family, or (for flags) exactly an OR of them; else the number stays.
    locals and parameters
                a placeholder (iVar1, uVar2, local_8, a1) passed to Windows functions always as
                the same parameter takes that parameter's name (hWnd, lpText); one that holds
                only the handle a Windows function returns is named for the handle (hwnd, hdc,
                hMem); one counted up in a loop and compared is i, j, k.
    globals     a placeholder global assigned only GetProcAddress(h, "Name") is g_pfnName; one
                assigned only a handle a Windows function returns is g_<handle name>.

Renaming changes no code and an SDK constant is its value, so what was compiled and judged is
what these texts compile to. Nothing is named from a guess about intent: a function's own name
is never invented -- `calls_hint` only says which Windows functions it calls, by area.
"""
import os
import re

from codebreaker.symbols import prototypes

# (function without its A/W ending, lower case; parameter name) -> (prefixes, "flags" | "enum")
FAMILIES = {
    ("messagebox", "uType"): (("MB_",), "flags"), ("messageboxex", "uType"): (("MB_",), "flags"),
    ("messagebeep", "uType"): (("MB_",), "enum"),
    ("localalloc", "uFlags"): (("LMEM_",), "flags"), ("localrealloc", "uFlags"): (("LMEM_",), "flags"),
    ("globalalloc", "uFlags"): (("GMEM_",), "flags"), ("globalrealloc", "uFlags"): (("GMEM_",), "flags"),
    ("heapalloc", "dwFlags"): (("HEAP_",), "flags"), ("heaprealloc", "dwFlags"): (("HEAP_",), "flags"),
    ("heapfree", "dwFlags"): (("HEAP_",), "flags"), ("heapcreate", "flOptions"): (("HEAP_",), "flags"),
    ("virtualalloc", "flAllocationType"): (("MEM_",), "flags"), ("virtualalloc", "flProtect"): (("PAGE_",), "flags"),
    ("virtualfree", "dwFreeType"): (("MEM_",), "flags"), ("virtualprotect", "flNewProtect"): (("PAGE_",), "flags"),
    ("showwindow", "nCmdShow"): (("SW_",), "enum"), ("showwindowasync", "nCmdShow"): (("SW_",), "enum"),
    ("sendmessage", "Msg"): (("WM_",), "enum"), ("postmessage", "Msg"): (("WM_",), "enum"),
    ("senddlgitemmessage", "Msg"): (("WM_",), "enum"), ("defwindowproc", "Msg"): (("WM_",), "enum"),
    ("callwindowproc", "Msg"): (("WM_",), "enum"), ("postthreadmessage", "Msg"): (("WM_",), "enum"),
    ("createwindowex", "dwExStyle"): (("WS_EX_",), "flags"), ("createwindowex", "dwStyle"): (("WS_",), "flags"),
    ("setwindowpos", "uFlags"): (("SWP_",), "flags"),
    ("getwindowlong", "nIndex"): (("GWL_",), "enum"), ("setwindowlong", "nIndex"): (("GWL_",), "enum"),
    ("getclasslong", "nIndex"): (("GCL_",), "enum"), ("setclasslong", "nIndex"): (("GCL_",), "enum"),
    ("getsystemmetrics", "nIndex"): (("SM_",), "enum"), ("getsyscolor", "nIndex"): (("COLOR_",), "enum"),
    ("getsyscolorbrush", "nIndex"): (("COLOR_",), "enum"), ("getdevicecaps", "index"): (("HORZRES", "VERTRES", "LOGPIXELS", "BITSPIXEL", "PLANES", "NUMCOLORS", "RASTERCAPS", "TECHNOLOGY", "SIZEPALETTE"), "enum"),
    ("drawtext", "uFormat"): (("DT_",), "flags"), ("drawtextex", "dwDTFormat"): (("DT_",), "flags"),
    ("peekmessage", "wRemoveMsg"): (("PM_",), "flags"),
    ("trackpopupmenu", "uFlags"): (("TPM_",), "flags"), ("trackpopupmenuex", "fuFlags"): (("TPM_",), "flags"),
    ("enablemenuitem", "uEnable"): (("MF_",), "flags"), ("checkmenuitem", "uCheck"): (("MF_",), "flags"),
    ("appendmenu", "uFlags"): (("MF_",), "flags"), ("insertmenu", "uFlags"): (("MF_",), "flags"),
    ("modifymenu", "uFlags"): (("MF_",), "flags"), ("deletemenu", "uFlags"): (("MF_",), "flags"),
    ("removemenu", "uFlags"): (("MF_",), "flags"),
    ("checkdlgbutton", "uCheck"): (("BST_",), "enum"),
    ("regopenkeyex", "samDesired"): (("KEY_",), "flags"), ("regcreatekeyex", "samDesired"): (("KEY_",), "flags"),
    ("regcreatekeyex", "dwOptions"): (("REG_OPTION_",), "flags"), ("regsetvalueex", "dwType"): (("REG_",), "enum"),
    ("regopenkeyex", "hKey"): (("HKEY_",), "enum"), ("regcreatekeyex", "hKey"): (("HKEY_",), "enum"),
    ("regopenkey", "hKey"): (("HKEY_",), "enum"), ("regcreatekey", "hKey"): (("HKEY_",), "enum"),
    ("regqueryvalue", "hKey"): (("HKEY_",), "enum"), ("regsetvalue", "hKey"): (("HKEY_",), "enum"),
    ("regdeletekey", "hKey"): (("HKEY_",), "enum"), ("regenumkey", "hKey"): (("HKEY_",), "enum"),
    ("createfile", "dwDesiredAccess"): (("GENERIC_",), "flags"), ("createfile", "dwShareMode"): (("FILE_SHARE_",), "flags"),
    ("createfile", "dwFlagsAndAttributes"): (("FILE_ATTRIBUTE_", "FILE_FLAG_"), "flags"),
    ("setfileattributes", "dwFileAttributes"): (("FILE_ATTRIBUTE_",), "flags"),
    ("setfilepointer", "dwMoveMethod"): (("FILE_BEGIN", "FILE_CURRENT", "FILE_END"), "enum"),
    ("formatmessage", "dwFlags"): (("FORMAT_MESSAGE_",), "flags"),
    ("loadimage", "uType"): (("IMAGE_",), "enum"), ("loadimage", "fuLoad"): (("LR_",), "flags"),
    ("copyimage", "uType"): (("IMAGE_",), "enum"), ("copyimage", "fuFlags"): (("LR_",), "flags"),
    ("setbkmode", "iBkMode"): (("TRANSPARENT", "OPAQUE"), "enum"),
    ("getstockobject", "i"): (("WHITE_BRUSH", "LTGRAY_BRUSH", "GRAY_BRUSH", "DKGRAY_BRUSH", "BLACK_BRUSH", "NULL_BRUSH",
                               "HOLLOW_BRUSH", "WHITE_PEN", "BLACK_PEN", "NULL_PEN", "OEM_FIXED_FONT", "ANSI_FIXED_FONT",
                               "ANSI_VAR_FONT", "SYSTEM_FONT", "DEVICE_DEFAULT_FONT", "DEFAULT_PALETTE",
                               "SYSTEM_FIXED_FONT", "DEFAULT_GUI_FONT"), "enum"),
    ("waitforsingleobject", "dwMilliseconds"): (("INFINITE",), "enum"),
    ("waitformultipleobjects", "dwMilliseconds"): (("INFINITE",), "enum"),
    ("setwindowshookex", "idHook"): (("WH_",), "enum"),
    ("redrawwindow", "flags"): (("RDW_",), "flags"),
    ("getwindow", "uCmd"): (("GW_",), "enum"),
    ("seterrormode", "uMode"): (("SEM_",), "flags"),
    ("getfiletype", ""): ((), "enum"),
}
_EXPR = re.compile(r"^#define\s+([A-Z][A-Z0-9_]+)\s+\(\s*([A-Z][A-Z0-9_]*(?:\s*\|\s*[A-Z][A-Z0-9_]*)+)\s*\)\s*(?:/[/*].*)?$", re.M)
_TOP = re.compile(r"^#define\s+([A-Z][A-Z0-9_]+)\s+\(?\s*(?:\(\s*\w+\s*\)\s*)?(-?\s*(?:0x[0-9A-Fa-f]+|\d+))[uUlL]*\s*\)?\s*(?:/[/*].*)?$", re.M)
_db = None


def _constants():
    """{NAME: value} from the SDK headers, and [(NAME, value, header)] in the order the headers give
    them. A name defined as an OR of others (WS_OVERLAPPEDWINDOW) gets the value of the OR."""
    global _db
    if _db is None:
        values, order, later = {}, [], []
        folders = prototypes._folders()
        for bits in ("32", "16"):
            for folder in folders.get(bits, []):
                if not os.path.isdir(folder):
                    continue
                for fn in sorted(os.listdir(folder)):
                    if not fn.lower().endswith(".h"):
                        continue
                    try:
                        with open(os.path.join(folder, fn), encoding="latin1") as f:
                            text = f.read()
                    except OSError:
                        continue
                    text = re.sub(r"\\[ \t]*\r?\n", " ", text)            # a define continued on the next line
                    for m in _TOP.finditer(text):
                        name = m.group(1)
                        if name in values:
                            continue
                        try:
                            v = int(m.group(2).replace(" ", ""), 0)
                        except ValueError:
                            continue
                        values[name] = v & 0xFFFFFFFF
                        order.append((name, v & 0xFFFFFFFF, fn.lower()))
                    later += [(m.group(1), m.group(2), fn.lower()) for m in _EXPR.finditer(text)]
        for name, expr, header in later:
            parts = [x.strip() for x in expr.split("|")]
            if name not in values and all(x in values for x in parts):
                v = 0
                for x in parts:
                    v |= values[x]
                values[name] = v
                order.append((name, v, header))
        _db = (values, order)
    return _db


_NARROWER = {"WS_": ("WS_EX_",), "MB_": ("MB_ERR_",), "SW_": ("SW_ERASE", "SW_INVALIDATE", "SW_SCROLLCHILDREN"),
             "REG_": ("REG_OPTION_", "REG_CREATED_", "REG_OPENED_", "REG_NOTIFY_", "REG_LEGAL_", "REG_WHOLE_",
                      "REG_REFRESH_", "REG_NO_")}


def _family(prefixes, header=None):
    """[(NAME, value)] of the constants with these prefixes -- from `header` (the function's own
    header) when it defines any, so MB_ for MessageBox is winuser.h's, not winnls.h's."""
    _values, order = _constants()
    out = [(name, v, h) for name, v, h in order
           if any(name.startswith(p) if p.endswith("_") else name == p or name.startswith(p) for p in prefixes)
           and not any(name.startswith(x) for p in prefixes for x in _NARROWER.get(p, ()))
           and not name.endswith(("MASK", "_FIRST", "_LAST", "_MAX", "_MIN", "_RESERVED"))]
    own = [(n, v) for n, v, h in out if header and h == header]
    return own or [(n, v) for n, v, _h in out]


def prefixes_of(base, parameter):
    fam = FAMILIES.get((base, parameter))
    return fam[0] if fam else ()


def constant(function, parameter, value):
    """The SDK's name for `value` passed as `parameter` of `function`, or None."""
    base = re.sub(r"(?<=[a-z])[AW]$", "", function or "").lower()
    fam = FAMILIES.get((base, parameter))
    if not fam or not fam[0]:
        return None
    proto = _prototype(function)
    members = _family(fam[0], proto and proto.get("header", "").lower())
    value &= 0xFFFFFFFF
    exact = [n for n, v in members if v == value]
    if exact:
        return exact[0]                                       # the first the headers give that value
    if fam[1] != "flags" or value == 0:
        return None
    left, picked = value, []
    for n, v in sorted(members, key=lambda nv: (-bin(nv[1]).count("1"), -nv[1])):
        if v and v & left == v:
            picked.append((v, n))
            left &= ~v
        if not left:
            break
    if left or len(picked) < 2:
        return None
    if prefixes_of(base, parameter) == ("WS_",):              # a child window's 0x20000/0x10000 are WS_GROUP and
        child = value & 0x40000000                            # WS_TABSTOP, a top-level one's the minimize and
        mean = {0x20000: "WS_GROUP" if child else "WS_MINIMIZEBOX",           # maximize boxes (the SDK's own
                0x10000: "WS_TABSTOP" if child else "WS_MAXIMIZEBOX"}         # reading of those bits)
        picked = [(v, mean.get(v, n)) for v, n in picked]
        members = [(n, w) for n, w in members if w not in mean or n == mean[w]]
    for v, n in picked:                                       # a part two names share: which was meant is
        if len({m for m, w in members if w == v}) > 1:        # not in the number, so the number stays
            return None
    return " | ".join(n for _v, n in sorted(picked, reverse=True))


# ---- reading calls in C text ------------------------------------------------------------------------
_CALL = re.compile(r"\b([A-Za-z_]\w*)\s*\(")
_NUMBER = re.compile(r"^\(?\s*(?:\(\s*[A-Za-z_][\w ]*\*?\s*\)\s*)?(-?(?:0x[0-9a-fA-F]+|\d+))[uUlL]*\s*\)?$")


def calls(text):
    """[(function, start of its argument list, end, [argument texts])] for every call in text."""
    out = []
    for m in _CALL.finditer(text):
        name, i = m.group(1), m.end()
        if name in ("if", "while", "for", "switch", "return", "sizeof"):
            continue
        depth, start, args = 1, i, []
        j = i
        while j < len(text) and depth:
            c = text[j]
            if c in "([":
                depth += 1
            elif c in ")]":
                depth -= 1
                if not depth:
                    args.append(text[start:j])
                    break
            elif c == "," and depth == 1:
                args.append(text[start:j])
                start = j + 1
            elif c == '"':
                j += 1
                while j < len(text) and text[j] != '"':
                    j += 2 if text[j] == "\\" else 1
            j += 1
        if depth == 0:
            out.append((name, i, j, args))
    return out


def _prototype(name):
    for candidate in (name, re.sub(r"(?<=[a-z])[AW]$", "", name)):
        p = prototypes.of(candidate)
        if p:
            return p
    return None


# ---- names for placeholders -----------------------------------------------------------------------------
_PLACEHOLDER = re.compile(r"^(?:[a-z]{1,4}Var\d+|local_[0-9a-fA-F]+|a\d+(?:_\d+)?|param_\d+|in_\w+|extraout_\w+)$")
_GLOBAL = re.compile(r"\b(?:[a-z]{1,4}Ram[0-9a-f]+|DAT_[0-9a-f]+|PTR_\w+)\b")
_HANDLE_NAMES = {"HWND": "hwnd", "HDC": "hdc", "HGLOBAL": "hMem", "HLOCAL": "hMem", "HMENU": "hmenu",
                 "HINSTANCE": "hinst", "HMODULE": "hmod", "HBRUSH": "hbr", "HPEN": "hpen", "HFONT": "hfont",
                 "HBITMAP": "hbm", "HICON": "hicon", "HCURSOR": "hcur", "HKEY": "hkey", "HANDLE": "h",
                 "HRGN": "hrgn", "HPALETTE": "hpal", "HGDIOBJ": "hobj", "HACCEL": "haccel", "HRSRC": "hrsrc",
                 "HFILE": "hf", "HDWP": "hdwp", "HIMAGELIST": "himl", "HTREEITEM": "hitem", "HPROPSHEETPAGE": "hpage"}
_KEYWORDS = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum",
             "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed",
             "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
             "far", "near", "huge", "pascal", "cdecl", "i", "j", "k"}


def _assignments(text, var):
    return re.findall(r"(?<![\w.>])%s\s*=(?!=)\s*([^;]+);" % re.escape(var), text)


def _local_names(text):
    """{placeholder: name} for one function's locals and parameters."""
    used = set(re.findall(r"\b[A-Za-z_]\w*\b", text))
    as_param = {}                                             # var -> {(function, parameter name)}
    for fn, _a, _b, args in calls(text):
        p = _prototype(fn)
        if not p:
            continue
        for k, arg in enumerate(args):
            arg = arg.strip()
            m = re.match(r"^(?:\([^()]*\)\s*)?([A-Za-z_]\w*)$", arg)
            if m and _PLACEHOLDER.match(m.group(1)) and k < len(p["parameters"]) and p["parameters"][k][1]:
                as_param.setdefault(m.group(1), set()).add(p["parameters"][k][1])
    names = {}
    for var in sorted(set(re.findall(r"\b[A-Za-z_]\w*\b", text))):
        if not _PLACEHOLDER.match(var):
            continue
        wanted = None
        given = _assignments(text, var)
        sources = {re.match(r"^\s*(?:\([^()]*\)\s*)?([A-Za-z_]\w*)\s*\(", g).group(1) for g in given
                   if re.match(r"^\s*(?:\([^()]*\)\s*)?([A-Za-z_]\w*)\s*\(", g)}
        if given and len(sources) == 1 and len(sources) == len(set(g.strip() for g in given)) or \
                (given and len(sources) == 1 and all(re.match(r"^\s*(?:\([^()]*\)\s*)?%s\s*\(" % re.escape(next(iter(sources))), g) for g in given)):
            p = _prototype(next(iter(sources)))
            if p and p["returns"] in _HANDLE_NAMES:
                wanted = _HANDLE_NAMES[p["returns"]]
        if wanted is None and len(as_param.get(var, ())) == 1:
            wanted = next(iter(as_param[var]))
        if wanted is None and re.search(r"\b%s\s*=\s*%s\s*\+\s*1\s*[;)]" % (re.escape(var), re.escape(var)), text) and \
                re.search(r"\b%s\s*<" % re.escape(var), text):
            wanted = "i"
        if not wanted:
            continue
        base, n = wanted, 2
        while wanted in used or wanted in names.values() or wanted in _KEYWORDS and wanted not in ("i", "j", "k"):
            if base == "i" and wanted in ("i", "j"):
                wanted = {"i": "j", "j": "k"}[wanted]
                continue
            wanted = "%s%d" % (base, n)
            n += 1
        names[var] = wanted
        used.add(wanted)
    return names


def _rename(text, names):
    if not names:
        return text
    pattern = re.compile(r"(?<![\w.>])(%s)\b" % "|".join(re.escape(k) for k in sorted(names, key=len, reverse=True)))
    out, i = [], 0
    for m in re.finditer(r'"(?:\\.|[^"\\])*"', text):                # never inside a string
        out.append(pattern.sub(lambda g: names[g.group(1)], text[i:m.start()]))
        out.append(m.group(0))
        i = m.end()
    out.append(pattern.sub(lambda g: names[g.group(1)], text[i:]))
    return "".join(out)


class Module(object):
    def __init__(self, texts, string_at=None):
        self.texts = texts
        self.string_at = string_at or (lambda address: None)
        self.constants_used = {}
        self.globals = self._global_names()

    def _global_names(self):
        assigned = {}
        for text in self.texts.values():
            for g in set(_GLOBAL.findall(text)):
                for value in _assignments(text, g):
                    assigned.setdefault(g, []).append(value.strip())
        names, taken = {}, set()
        for text in self.texts.values():
            taken |= set(re.findall(r"\b[A-Za-z_]\w*\b", text))
        for g, values in sorted(assigned.items()):
            wanted = None
            procs = {self._proc_name(v) for v in values if "GetProcAddress" in v} - {None}
            if procs and len(procs) == 1 and all("GetProcAddress" in v for v in values):
                wanted = "g_pfn" + next(iter(procs))
            else:
                called = {re.match(r"^(?:\([^()]*\)\s*)?([A-Za-z_]\w*)\s*\(", v).group(1) for v in values
                          if re.match(r"^(?:\([^()]*\)\s*)?([A-Za-z_]\w*)\s*\(", v)}
                if len(called) == 1 and len(called) == len({v.split("(")[0] for v in values}) and \
                        all(re.match(r"^(?:\([^()]*\)\s*)?%s\s*\(" % re.escape(next(iter(called))), v) for v in values):
                    p = _prototype(next(iter(called)))
                    if p and p["returns"] in _HANDLE_NAMES:
                        h = _HANDLE_NAMES[p["returns"]]
                        wanted = "g_" + h
            if not wanted:
                continue
            base, n = wanted, 2
            while wanted in taken or wanted in names.values():
                wanted = "%s%d" % (base, n)
                n += 1
            names[g] = wanted
        return names

    def _proc_name(self, value):
        """The function GetProcAddress(h, name) asks for: a string in the text, or the module's
        string at the address the text passes."""
        m = re.search(r'GetProcAddress\w*\s*\([^,]*,\s*(?:\([^()]*\)\s*)?(?:"(\w+)"|(-?(?:0x[0-9a-fA-F]+|\d+)))\s*\)', value)
        if not m:
            return None
        if m.group(1):
            return m.group(1)
        found = self.string_at(int(m.group(2), 0) & 0xFFFFFFFF)
        return found if found and re.match(r"^[A-Za-z_]\w*$", found) else None

    def function(self, name):
        text = self.texts[name]
        text = self._constants(text)
        text = _rename(text, dict(_local_names(text), **self.globals))
        return text

    def _constants(self, text):
        edits = []
        for fn, _a, _b, args in calls(text):
            p = _prototype(fn)
            if not p:
                continue
            pos = _a
            for k, arg in enumerate(args):
                start = pos
                pos += len(arg) + 1
                if k >= len(p["parameters"]):
                    break
                m = _NUMBER.match(arg.strip())
                if not m:
                    continue
                named = constant(fn, p["parameters"][k][1], int(m.group(1), 0))
                if named:
                    lead = len(arg) - len(arg.lstrip())
                    edits.append((start + lead, start + lead + len(arg.strip()), named))
                    values, _o = _constants()
                    for part in named.split(" | "):
                        self.constants_used[part] = values[part]
        for s, e, named in sorted(edits, reverse=True):
            text = text[:s] + named + text[e:]
        return text

    def header(self, text):
        """The module's header with the globals renamed as in the functions, and the SDK constants
        written into them defined."""
        return _rename(text, self.globals) + ("\n" + self.definitions() if self.constants_used else "")

    def definitions(self):
        """#define lines for the SDK constants written in, with the values the SDK gives them."""
        return "".join("#ifndef %s\n#define %s 0x%X\n#endif\n" % (n, n, v) for n, v in sorted(self.constants_used.items()))


_AREAS = (("registry", ("RegOpen", "RegCreate", "RegQuery", "RegSet", "RegClose", "RegDelete", "RegEnum", "RegFlush")), ("files", ("CreateFile", "ReadFile", "WriteFile", "CloseHandle", "_lopen", "_lread",
                                             "_lwrite", "_lclose", "OpenFile", "SetFilePointer", "FindFirstFile", "DeleteFile")),
          ("memory", ("LocalAlloc", "LocalFree", "GlobalAlloc", "GlobalFree", "GlobalLock", "GlobalUnlock", "HeapAlloc",
                      "HeapFree", "VirtualAlloc", "VirtualFree", "LocalLock", "LocalUnlock", "LocalReAlloc", "GlobalReAlloc")),
          ("windows", ("CreateWindow", "DestroyWindow", "ShowWindow", "SetWindowPos", "MoveWindow", "GetWindowRect",
                       "GetClientRect", "InvalidateRect", "UpdateWindow", "SetWindowLong", "GetWindowLong", "SetWindowText",
                       "GetWindowText", "GetParent", "GetDlgItem", "EnableWindow")),
          ("messages", ("SendMessage", "PostMessage", "SendDlgItemMessage", "DefWindowProc", "CallWindowProc", "GetMessage",
                        "PeekMessage", "DispatchMessage", "TranslateMessage")),
          ("drawing", ("BeginPaint", "EndPaint", "GetDC", "ReleaseDC", "SelectObject", "DeleteObject", "TextOut", "DrawText",
                       "BitBlt", "PatBlt", "FillRect", "CreatePen", "CreateSolidBrush", "SetTextColor", "SetBkColor",
                       "SetBkMode", "LineTo", "MoveToEx", "Rectangle", "CreateCompatibleDC", "StretchBlt", "ExtTextOut")),
          ("strings", ("lstrcpy", "lstrcat", "lstrlen", "lstrcmp", "lstrcmpi", "wsprintf", "wvsprintf", "LoadString",
                       "CharNext", "CharPrev", "CharUpper", "CharLower", "MultiByteToWideChar", "WideCharToMultiByte")),
          ("modules", ("LoadLibrary", "FreeLibrary", "GetProcAddress", "GetModuleHandle", "GetModuleFileName")),
          ("dialogs", ("DialogBox", "CreateDialog", "EndDialog", "MessageBox", "CheckDlgButton", "IsDlgButtonChecked",
                       "SetDlgItemText", "GetDlgItemText", "SetDlgItemInt", "GetDlgItemInt")),
          ("menus", ("CreateMenu", "CreatePopupMenu", "AppendMenu", "InsertMenu", "EnableMenuItem", "CheckMenuItem",
                     "TrackPopupMenu", "GetSubMenu", "DestroyMenu", "LoadMenu")))


def calls_hint(text):
    """Which areas of the Windows API a function works with, and the functions: or ""."""
    seen = []
    for fn, _a, _b, _args in calls(text):
        if fn not in seen and _prototype(fn):
            seen.append(fn)
    if not seen:
        return ""
    areas = [area for area, heads in _AREAS if any(f.startswith(heads) for f in seen)]
    shown = ", ".join(seen[:5]) + (", ..." if len(seen) > 5 else "")
    return ("%s: %s" % (" and ".join(areas[:2]), shown)) if areas else "calls " + shown
