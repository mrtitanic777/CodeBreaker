"""sdk_structs.py -- the layouts of the SDK's structures, read from its headers.

    from codebreaker.symbols import sdk_structs
    s = sdk_structs.layout("RECT")          Struct(name, size, fields) or None
    s.at(8, 4)                              "right": the field that starts at 8 and is 4 bytes
    sdk_structs.pointed_to("LPCRECT")       "RECT": the structure a pointer type points at
    sdk_structs.definition(s)               C text defining it, field for field, at the SDK's offsets

A structure is read from its `typedef struct tagX { ... } X, *PX, FAR *LPX;` in the 32-bit
headers (symbols/prototypes.py finds them). Offsets follow the headers' packing (#include
<pshpackN.h> ... <poppack.h>, natural alignment otherwise). A field of a type this does not
know the size of makes the whole structure unknown -- a wrong offset would name the wrong field.
The definition written out lays the fields at those offsets explicitly (packed to 1, with the
gaps as padding), so it means the same to any compiler.
"""
import os
import re

_SIZE = {"BYTE": 1, "CHAR": 1, "char": 1, "UCHAR": 1, "BOOLEAN": 1, "CCHAR": 1, "TBYTE": 1, "TCHAR": 1,
         "WORD": 2, "SHORT": 2, "short": 2, "USHORT": 2, "WCHAR": 2, "ATOM": 2, "LANGID": 2,
         "double": 8, "DOUBLE": 8, "LONGLONG": 8, "DWORDLONG": 8, "ULONGLONG": 8, "__int64": 8, "FILETIME": 8,
         "LARGE_INTEGER": 8}
_FOUR = ("LONG", "DWORD", "UINT", "INT", "int", "BOOL", "ULONG", "COLORREF", "WPARAM", "LPARAM", "LRESULT",
         "FARPROC", "WNDPROC", "DLGPROC", "HOOKPROC", "float", "FLOAT", "long", "SIZE_T", "LCID", "LCTYPE",
         "ACCESS_MASK", "REGSAM", "HRESULT", "UINT_PTR", "LONG_PTR", "DWORD_PTR", "unsigned", "TIMERPROC")
_SIGNED = {"LONG", "INT", "int", "BOOL", "LPARAM", "LRESULT", "SHORT", "short", "CHAR", "char", "long", "HRESULT",
           "LONG_PTR", "CCHAR", "INT_PTR"}
_DECL = re.compile(r"typedef\s+struct\s+(\w+)?\s*\{([^{}]*)\}\s*([^;]+);", re.S)
_POINTER_TYPEDEF = re.compile(r"typedef\s+(?:const\s+|CONST\s+)?(\w+)\s+(?:FAR\s*|NEAR\s*|far\s*|near\s*)?\*\s*(\w+)\s*;")


class Struct(object):
    def __init__(self, name, size, fields):
        self.name, self.size, self.fields = name, size, fields      # fields: [(offset, size, name, c type)]

    def typed(self, offset, size):
        """(field path, its C type) for the field starting at `offset` that is `size` bytes, or None."""
        for off, n, name, ctype in self.fields:
            if off == offset and n == size and not ctype.startswith("struct ") and "[" not in ctype:
                return name, ctype
            if off <= offset < off + n and ctype.startswith("struct "):
                inner = layout(ctype[7:])
                found = inner.typed(offset - off, size) if inner else None
                if found:
                    return name + "." + found[0], found[1]
        return None

    def at(self, offset, size):
        """The field (a dotted path into a nested structure) starting at `offset` that is `size`
        bytes, or None."""
        for off, n, name, ctype in self.fields:
            if off == offset and n == size:
                return name
            if off <= offset < off + n and ctype.startswith("struct "):
                inner = layout(ctype[7:])
                if inner:
                    sub = inner.at(offset - off, size)
                    if sub:
                        return name + "." + sub
        return None


_db = None


def _size(t, structs):
    t = t.strip()
    if t in _SIZE:
        return _SIZE[t]
    if t in _FOUR or (t[:1] == "H" and t[1:2].isupper()) or t.startswith(("LP", "PC", "NP")) or \
            (t[:1] == "P" and t[1:2].isupper()):
        return 4
    if t in structs and structs[t]:
        return structs[t][0]
    return None


def _read():
    global _db
    if _db is not None:
        return _db
    from codebreaker.symbols import prototypes
    from codebreaker.source import readable
    values = readable._constants()[0]
    raw, pointers = {}, {}
    for folder in prototypes._folders().get("32", []):
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
            text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
            text = re.sub(r"//[^\n]*", " ", text)
            # the packing in force at each point of the file
            packs, stack, pos = [], [], 0
            for m in re.finditer(r"#include\s*<(pshpack(\d)|poppack)\.h>", text):
                if m.group(2):
                    stack.append(int(m.group(2)))
                elif stack:
                    stack.pop()
                packs.append((m.start(), stack[-1] if stack else 8))

            def pack_at(p):
                cur = 8
                for start, value in packs:
                    if start > p:
                        break
                    cur = value
                return cur
            for m in _DECL.finditer(text):
                names = [x.strip().split()[-1].lstrip("*") for x in m.group(3).split(",")]
                body = m.group(2)
                if "union" in body or "#" in body:
                    continue
                main = names[0]
                if main in raw:
                    continue
                raw[main] = (body, pack_at(m.start()))
                for x in m.group(3).split(",")[1:]:
                    if "*" in x:
                        pointers[x.strip().split()[-1].lstrip("*")] = main
                if m.group(1):
                    raw.setdefault(m.group(1), raw[main])
            for m in _POINTER_TYPEDEF.finditer(text):
                pointers.setdefault(m.group(2), m.group(1))
    structs = {}

    def build(name, depth=0):
        if name in structs:
            return structs[name]
        structs[name] = None
        if name not in raw or depth > 6:
            return None
        body, pack = raw[name]
        fields, off, biggest = [], 0, 1
        for decl in body.split(";"):
            decl = re.sub(r"\b(?:CONST|const|volatile|FAR|NEAR|far|near|struct)\b", " ", decl).strip()
            if not decl:
                continue
            m = re.match(r"^(\w+)\s*(\**)\s*([\w\s,\[\]*]+)$", decl)
            if not m:
                return None
            base, star = m.group(1), m.group(2)
            for item in m.group(3).split(","):
                item = item.strip()
                stars = star + "*" * (len(item) - len(item.lstrip("*")))
                item = item.lstrip("*").strip()
                arr = re.match(r"^(\w+)\s*\[\s*(\w+)\s*\]$", item)
                fname, count = (arr.group(1), arr.group(2)) if arr else (item, None)
                if not re.match(r"^\w+$", fname):
                    return None
                if count is not None:
                    count = int(count, 0) if count[:1].isdigit() else values.get(count)
                    if count is None:
                        return None
                if stars:
                    n, ctype = 4, "uint4"
                elif base in raw and base not in _SIZE and base not in _FOUR:
                    inner = build(base, depth + 1)
                    if not inner:
                        return None
                    n, ctype = inner[0], "struct " + base
                else:
                    n = _size(base, structs)
                    if n is None:
                        return None
                    ctype = ("int%d" if base in _SIGNED else "uint%d") % n if n in (1, 2, 4, 8) else None
                    if ctype is None:
                        return None
                align = min(n if not ctype.startswith("struct ") else structs[base][2], pack, 8)
                off = (off + align - 1) // align * align
                biggest = max(biggest, align)
                total = n * (count or 1)
                fields.append((off, n if count is None else total, fname, ctype if count is None else "%s[%d]" % (ctype, count)))
                off += total
        size = (off + biggest - 1) // biggest * biggest
        structs[name] = (size, fields, biggest)
        return structs[name]
    for name in list(raw):
        build(name)
    _db = (structs, pointers)
    return _db


def layout(name):
    structs, _p = _read()
    s = structs.get(name)
    return Struct(name, s[0], s[1]) if s else None


def pointed_to(type_text):
    """The structure a parameter of this type points at, or None ("LPRECT", "CONST RECT *")."""
    structs, pointers = _read()
    t = re.sub(r"\b(?:CONST|const|FAR|NEAR|far|near)\b", " ", type_text or "").strip()
    if t.endswith("*"):
        base = t[:-1].strip()
        return base if structs.get(base) else None
    base = pointers.get(t)
    return base if base and structs.get(base) else None


def definition(s, known=()):
    """C text defining structure `s` (and the structures in it, before it), laid out field for
    field at the SDK's offsets."""
    out, done = [], set(known)

    def emit(name):
        if name in done:
            return
        done.add(name)
        st = layout(name)
        lines, off = [], 0
        for f_off, n, fname, ctype in st.fields:
            if ctype.startswith("struct "):
                emit(ctype[7:])
            if f_off > off:
                lines.append("    uint1 codebreaker_pad_%x[%d];" % (off, f_off - off))
            arr = re.match(r"^(.*)\[(\d+)\]$", ctype)
            base = arr.group(1) if arr else ctype
            base = base[7:] if base.startswith("struct ") else base
            lines.append("    %s %s%s;" % (base, fname, "[%s]" % arr.group(2) if arr else ""))
            off = f_off + n
        if st.size > off:
            lines.append("    uint1 codebreaker_pad_%x[%d];" % (off, st.size - off))
        out.append("#pragma pack(push, 1)\ntypedef struct %s {\n%s\n} %s;\n#pragma pack(pop)\n" % (
            name, "\n".join(lines), name))
    emit(s.name)
    return "".join(out)
