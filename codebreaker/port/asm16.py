"""asm16.py -- a program's own 16-bit assembly language, made assembly language for a 64-bit machine.

The routines a program wrote in assembly language stay in assembly language: every instruction, label, loop and string
operation as it was, the arithmetic on values in the 16 and 8 bits it had. What a 64-bit machine changes is changed and
nothing else:

  pointers         a register that holds a pointer at that place (it was loaded from a pointer of the C program, is an
                   address, or comes from one) is used whole (rsi for si); one that holds a value keeps its 16 or 8 bits;
                   a value used as an index is widened into a scratch register (r11) first
  structures       [bx+14] is a member of the C program's structure bx points to: its place in the 64-bit layout
  segments         ds, es and their loads go (one flat memory); les/lds are loads of a pointer; far is near
  calls            inside the module the old way, on the stack, each slot 8 bytes; a routine the C program calls gets an
                   entry of the C of today (its arguments in rcx, rdx, r8, r9) that pushes them as the old caller did,
                   cdecl or pascal, and keeps the registers the C of today keeps
  DOS              INT 21h is a call of the platform layer (dos_int21), the registers in a block

The kinds of the registers are followed through each routine (a pass to a fixed point over its jumps and calls); the
C program's types come from its headers - the old layout of each structure (16-bit pointers, far ones 4 bytes) from
the source as it was, the new one as the port lays it out.

    python -m codebreaker.port.asm16 FILE.asm OUT.asm --c PORT_DIR --orig ORIGINAL_SRC
"""
import collections
import re

from codebreaker.port import early_c

R16 = ["ax", "bx", "cx", "dx", "si", "di", "bp", "sp"]
R8 = {"al": "ax", "ah": "ax", "bl": "bx", "bh": "bx", "cl": "cx", "ch": "cx", "dl": "dx", "dh": "dx"}
R64 = {"ax": "rax", "bx": "rbx", "cx": "rcx", "dx": "rdx", "si": "rsi", "di": "rdi", "bp": "rbp", "sp": "rsp"}
SEG = {"ds", "es", "ss", "cs"}
JUMPS = {"jmp", "je", "jne", "jz", "jnz", "jb", "jnb", "jae", "jnae", "ja", "jna", "jbe", "jnbe", "jc", "jnc", "jl",
         "jnl", "jge", "jnge", "jg", "jng", "jle", "jnle", "js", "jns", "jo", "jno", "jp", "jnp", "jpe", "jpo", "loop",
         "loope", "loopne", "loopz", "loopnz", "jcxz"}
STRING = {"lodsb", "lodsw", "stosb", "stosw", "movsb", "movsw", "cmpsb", "cmpsw", "scasb", "scasw"}
SIZES = {"byte": 1, "word": 2, "dword": 4, "qword": 8, "tbyte": 10}


class Asm16Error(Exception):
    pass


# ---------------------------------------------------------------- reading the source

def strip_comment(l):
    q = None
    for i, c in enumerate(l):
        if q:
            if c == q:
                q = None
        elif c in "'\"":
            q = c
        elif c == ";":
            return l[:i]
    return l


def split_ops(s):
    out, d, cur, q = [], 0, "", None
    for c in s:
        if q:
            cur += c
            if c == q:
                q = None
            continue
        if c in "'\"":
            q = c
        elif c in "[(":
            d += 1
        elif c in "])":
            d -= 1
        if c == "," and d == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += c
    if cur.strip():
        out.append(cur.strip())
    return out


class Line:
    def __init__(self, no, text):
        self.no, self.text = no, text
        self.label = self.op = self.prefix = None
        self.args = []
        self.directive = None


def parse(text):
    """-> [Line]: label, prefix (rep...), mnemonic, operands - or a directive"""
    out = []
    for no, raw in enumerate(text.split("\n"), 1):
        l = Line(no, raw)
        s = strip_comment(raw).strip()
        m = re.match(r"([A-Za-z_$@?][\w$@?]*)\s*:(?!:)\s*(.*)$", s)
        if m and not re.match(r"(?i)(es|ds|ss|cs)\s*:", s):
            l.label, s = m.group(1), m.group(2).strip()
        if not s:
            out.append(l)
            continue
        w = s.split(None, 1)
        if len(w) > 1 and w[1].split(None, 1)[0].upper() in ("SEGMENT", "ENDS", "GROUP", "PROC", "ENDP", "LABEL",
                                                               "EQU", "DB", "DW", "DD", "DQ", "DT", "="):
            l.directive = (w[1].split(None, 1)[0].upper(), w[0], (w[1].split(None, 1) + [""])[1])
            out.append(l)
            continue
        if w[0].upper() in ("DB", "DW", "DD", "DQ", "DT", "PUBLIC", "EXTRN", "ASSUME", "TITLE", "END", "ORG", "EVEN",
                            "ALIGN", "INCLUDE", "NAME", "PAGE", "SUBTTL", ".8086", ".186", ".286", ".8087"):
            l.directive = (w[0].upper(), None, w[1] if len(w) > 1 else "")
            out.append(l)
            continue
        op = w[0].lower()
        rest = w[1] if len(w) > 1 else ""
        if op in ("rep", "repe", "repz", "repne", "repnz"):
            l.prefix = op
            w2 = rest.split(None, 1)
            op, rest = w2[0].lower(), (w2[1] if len(w2) > 1 else "")
        l.op = op
        l.args = split_ops(rest)
        out.append(l)
    return out


def listing_offsets(path, assembler, args=" /ML T.ASM,T.OBJ,T.LST;"):
    """{label: (segment, offset)} as the program's own assembler placed them (its listing; the assembler runs under
    CodeBreaker's emulator, nothing of it on the host)"""
    import os
    import shutil
    import tempfile
    from codebreaker.exact.emu import dos
    w = tempfile.mkdtemp(prefix="asm16_")
    try:
        shutil.copy(assembler, os.path.join(w, "ASM.EXE"))
        src = open(path, "rb").read().replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")
        open(os.path.join(w, "T.ASM"), "wb").write(src)
        dos.run(os.path.join(w, "ASM.EXE"), w, args, env=b"PATH=C:\\\0", fast=True)
        lst = open(os.path.join(w, "T.LST"), encoding="latin1").read().replace("\r", "")
    finally:
        shutil.rmtree(w, ignore_errors=True)
    out, seg = {}, None
    out["@lines"] = []                                      # ((segment, offset, bytes, the source line's start))
    for l in lst.split("\n"):
        l = l.expandtabs(8)                                 # (MASM 5's listing is laid out with tabs)
        m = re.match(r"^ ([0-9A-F]{4})  ((?:[0-9A-F]{2,4}R? ?)*)\s*(.*)$", l)
        if m and seg and len(l) > 32:
            nbytes = sum(len(t.rstrip("R")) // 2 for t in m.group(2).split())
            out["@lines"].append((seg, int(m.group(1), 16), nbytes, re.sub(r"\s+", " ", l[32:]).strip()[:24],
                                  bytes.fromhex("".join(t.rstrip("R") for t in m.group(2).split()))))
        m = re.match(r"^ ([0-9A-F]{4}) {2,}.*?\b([A-Za-z_$@?][\w$@?]*)\s+SEGMENT\b", l, re.I)
        if m:
            seg = m.group(2)
            continue
        if re.match(r"(?i)^ [0-9A-F]{4}\s+[A-Za-z_$@?][\w$@?]*\s+ENDS\b", l):   # (not a wrapped comment's "ends")
            seg = None
            continue
        m = re.match(r"^ ([0-9A-F]{4})\s+(?:[0-9A-F ]+R?\s+)?\s*([A-Za-z_$@?][\w$@?]*)(?::|\s+LABEL\b)", l, re.I)
        if m and seg:
            out[m.group(2)] = (seg, int(m.group(1), 16))
    return out


# ---------------------------------------------------------------- operands

class Mem:
    def __init__(self, size, seg, sym, regs, disp, text):
        self.size, self.seg, self.sym, self.regs, self.disp, self.text = size, seg, sym, regs, disp, text


R16_CODE = ["ax", "cx", "dx", "bx", "sp", "bp", "si", "di"]
R8_CODE = ["al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"]


def decode_direct(b):
    """(length, mnemonic, [operands]) of the 8086's moves to and from a direct address (8B/89/8A/88 with
    mod 00 r/m 110, A0-A3) written as bytes - None for anything else"""
    if len(b) >= 4 and b[0] in (0x88, 0x89, 0x8A, 0x8B) and (b[1] & 0xC7) == 0x06:
        reg = (R16_CODE if b[0] & 1 else R8_CODE)[(b[1] >> 3) & 7]
        mem = "%s ptr [0%04xh]" % ("word" if b[0] & 1 else "byte", b[2] | b[3] << 8)
        return (4, "mov", [reg, mem] if b[0] & 2 else [mem, reg])
    if len(b) >= 3 and b[0] in (0xA0, 0xA1, 0xA2, 0xA3):
        reg = "ax" if b[0] & 1 else "al"
        mem = "%s ptr [0%04xh]" % ("word" if b[0] & 1 else "byte", b[1] | b[2] << 8)
        return (3, "mov", [reg, mem] if b[0] < 0xA2 else [mem, reg])
    return None


def number(s):
    s = s.strip()
    m = re.fullmatch(r"([0-9][0-9a-fA-F]*)[hH]", s)
    if m:
        return int(m.group(1), 16)
    if re.fullmatch(r"\d+", s):
        return int(s)
    m = re.fullmatch(r"0[xX]([0-9a-fA-F]+)", s)
    if m:
        return int(m.group(1), 16)
    return None


def operand(a, equ=None):
    """('reg', name) ('imm', value text) ('off', symbol, disp) ('mem', Mem) ('sym', name)"""
    s = a.strip()
    low = s.lower()
    if low in R16 or low in R8 or low in SEG:
        return ("reg", low)
    m = re.fullmatch(r"(?i)offset\s+(?:\w+:)?([\w$@?]+)\s*(?:([+-])\s*(\w+))?", s)
    if m:
        d = number(m.group(3)) if m.group(3) else 0
        return ("off", m.group(1), -d if m.group(2) == "-" else d)
    size = None
    m = re.match(r"(?i)(byte|word|dword|qword|tbyte)\s+ptr\s+(.*)$", s)
    if m:
        size, s = SIZES[m.group(1).lower()], m.group(2).strip()
    seg = None
    m = re.match(r"(?i)(es|ds|ss|cs)\s*:\s*(.*)$", s)
    if m:
        seg, s = m.group(1).lower(), m.group(2).strip()
    if "[" in s or size or seg:
        regs, disp, sym = [], 0, None
        inside = " + ".join(re.findall(r"\[([^\]]*)\]", s))
        outside = re.sub(r"\[[^\]]*\]", " ", s)
        for part in re.findall(r"[+-]?\s*[^+\-\s]+", (outside + " + " + inside).replace(" ", "")):
            sign = -1 if part.startswith("-") else 1
            t = part.lstrip("+-")
            if not t:
                continue
            if t.lower() in R16:
                regs.append(t.lower())
            elif number(t) is not None:
                disp += sign * number(t)
            elif equ and t in equ and number(equ[t]) is not None:
                disp += sign * number(equ[t])
            else:
                if sym:
                    raise Asm16Error("two symbols in %r" % a)
                sym = t
        return ("mem", Mem(size, seg, sym, regs, disp, a))
    if number(s) is not None or re.fullmatch(r"'.'", s) or re.fullmatch(r"-\s*\w+", s):
        return ("imm", s)
    if equ and s in equ:
        return ("imm", s)
    if getattr(equ, "data", None) and s in equ.data:        # (a variable named bare: the memory it is)
        return ("mem", Mem(equ.data[s], None, s, [], 0, a))
    return ("sym", s)


class Equates(dict):
    """the module's equates; .data: its variables' sizes (a bare name of one is its memory)"""
    data = {}


# ---------------------------------------------------------------- the C program's types

class CTypes:
    """the globals' types and the structures' layouts, old (as the 8086 had them) and new (as the port has them)"""

    def __init__(self, port_texts, orig_texts):
        self.vars = {}
        for t in port_texts:
            code = early_c.blank(t)
            ds = early_c.depths(code.split("\n"))
            for i, l in enumerate(code.split("\n")):
                s = l.strip()
                if ds[i] or not s.endswith(";") or "(" in s.replace("(*", "") or s.startswith(("typedef", "#")):
                    continue
                s = re.sub(r"=.*;$", ";", s)
                r = early_c.declarators(s)
                if not r or not r[0]:
                    continue
                for n, d in r[1]:
                    self.vars.setdefault(n, (re.sub(r"\b(extern|static|register|const|volatile)\b", "", r[0]).strip(),
                                             d))
        self.old = self._layouts(orig_texts, ptr=2, far=4)
        self.new = self._layouts(port_texts, ptr=8, far=8)
        self.far_ptrs = set()                               # (globals whose pointers were far: 4 bytes each)
        for t in orig_texts:
            for d in re.finditer(r"(?m)^[^;(){}#]*\bfar\s*\*[^;(){}]*?\b(\w+)\s*(?:\[[^\]]*\])?\s*(?:=[^;]*)?;",
                                 early_c.blank(t)):
                self.far_ptrs.add(d.group(1))

    def elem_sizes(self, name):
        """(old, new) bytes of an element of a global array (or of the variable) - None when not known"""
        v = self.vars.get(name)
        if not v:
            return None
        base, d = v
        b = re.sub(r"\s+", " ", re.sub(r"\b(extern|static|register|const|volatile)\b", "", base)).strip()
        if "*" in d:
            return (4 if name in self.far_ptrs else 2), 8
        if b.startswith(("struct", "union")):
            o, n = self.old.get(b), self.new.get(b)
            return (o["@size"], n["@size"]) if o and n else None
        n = {"char": 1, "signed char": 1, "unsigned char": 1, "int16_t": 2, "uint16_t": 2, "short": 2,
             "int32_t": 4, "uint32_t": 4, "long": 4, "int8_t": 1, "uint8_t": 1}.get(b)
        return (n, n) if n else None

    @staticmethod
    def _layouts(texts, ptr, far):
        out = Layouts()
        for t in texts:
            code = early_c.blank(t)
            for m in re.finditer(r"\b(struct|union)\s+(\w+)\s*\{", code):
                d, k = 0, m.end() - 1
                while k < len(code):
                    d += {"{": 1, "}": -1}.get(code[k], 0)
                    if d == 0:
                        break
                    k += 1
                name = "%s %s" % (m.group(1), m.group(2))
                lay = CTypes._body(code[m.end():k], m.group(1) == "union", ptr, far, out, name)
                out.setdefault(name, lay)
        return out

    @staticmethod
    def _body(body, union, ptr, far, out, path=""):
        """the layout of a structure's (or union's) members: a nested struct or union one member of its size"""
        members, d, cur = [], 0, ""
        for c in body:                                      # (the members: ; at the body's own level)
            d += (c == "{") - (c == "}")
            if c == ";" and d == 0:
                members.append(cur)
                cur = ""
            else:
                cur += c
        lay, off = collections.OrderedDict(), 0
        for s in members:
            if not s.strip():
                continue
            nest = re.match(r"\s*(struct|union)\s*\w*\s*\{(.*)\}\s*(.*)$", s, re.S)
            if nest:                                        # (struct { ... } name: its own layout, one member)
                names = [x.strip() for x in nest.group(3).split(",") if x.strip()]
                inner = CTypes._body(nest.group(2), nest.group(1) == "union", ptr, far, out,
                                     "%s.%s" % (path, names[0] if names else "?"))
                names = [re.sub(r"\s*\[.*", "", x) for x in names]
                for nm in names:                            # (its members, for what is inside it)
                    if isinstance(out, Layouts):
                        out.inner["%s.%s" % (path, nm)] = (nest.group(1) == "union", inner)
                        out.setdefault("%s.%s" % (path, nm), inner)
                allptr = all(v[4] for k, v in inner.items() if k != "@size")
                for nm in names:
                    sz = inner["@size"]
                    if sz > 1 and off % 2:
                        off += 1
                    lay[nm] = (0 if union else off, sz, nest.group(1), nm, allptr)
                    off = max(off, sz) if union else off + sz
                continue
            isfar = bool(re.search(r"\bfar\b", s))
            s2 = re.sub(r"\b(far|near|huge)\b", "", s)
            r = early_c.declarators(s2)
            if not r:
                continue
            for n, dcl in r[1]:
                sz = early_c.size_of(r[0], dcl, n, far if isfar else ptr, out)
                if sz is None:
                    sz = 2
                if sz > 1 and off % 2:
                    off += 1
                isptr = "*" in dcl.replace(n, "", 1)
                lay[n] = (0 if union else off, sz, r[0].strip(), dcl, isptr)
                off = max(off, sz) if union else off + sz
        lay["@size"] = off + (off % 2 if off > 1 else 0)
        return lay

    def var(self, name):
        """(base type, declarator) of a C global"""
        return self.vars.get(name)

    def pointee(self, name):
        """the type a pointer variable points to ('struct fcb', 'char') - None when it is no pointer"""
        v = self.vars.get(name)
        if not v:
            return None
        base, d = v
        if "*" in d:
            return re.sub(r"\s+", " ", base).strip()
        return None

    def is_pointer(self, name):
        v = self.vars.get(name)
        return bool(v and "*" in v[1])

    def member_type(self, struct, off_old):
        """what the address of the member that begins at an old offset points to - None"""
        o = self.old.get(struct)
        if not o:
            return None
        for name, v in o.items():
            if name == "@size" or not isinstance(v, tuple) or v[0] != off_old:
                continue
            sub = "%s.%s" % (struct, name)
            if sub in self.old.inner:
                return sub
            stars = v[3].replace(name, "", 1).count("*") if isinstance(v[3], str) else 0
            return v[2] + (" " + "*" * stars if stars else "")
        return None

    def field(self, struct, off_old):
        """the member of a structure at an old offset: (name, old offset, old size, new offset, new size, is pointer,
        its type) - None when not known"""
        o, n = self.old.get(struct), self.new.get(struct)
        if not o or not n:
            return None
        return self._field(struct, o, n, off_old)

    def _field(self, path, o, n, off_old):
        for exact in (True, False):                         # (a member that begins there, else one around it)
            for name, v in o.items():
                if name == "@size":
                    continue
                fo, fs = v[0], v[1]
                if not (fo <= off_old < fo + fs and name in n) or (exact and fo != off_old and
                                                                     "%s.%s" % (path, name) not in self.old.inner):
                    continue
                sub = "%s.%s" % (path, name)
                if sub in self.old.inner and sub in self.new.inner:   # (a structure or union inside: within it)
                    uo, io = self.old.inner[sub]
                    un, inn = self.new.inner[sub]
                    if uo:                                  # (a union: its views, the one with a member there)
                        for vname, vv in io.items():
                            if vname == "@size" or vname not in inn:
                                continue
                            vsub = "%s.%s" % (sub, vname)
                            if vsub in self.old.inner and vsub in self.new.inner:
                                f = self._field(vsub, self.old.inner[vsub][1], self.new.inner[vsub][1],
                                                off_old - fo - vv[0])
                                if f and (f[1] == off_old - fo - vv[0] or not exact):
                                    return (f[0], fo + vv[0] + f[1], f[2], n[name][0] + inn[vname][0] + f[3],
                                            f[4], f[5], f[6])
                        continue
                    f = self._field(sub, io, inn, off_old - fo)
                    if f:
                        return (f[0], fo + f[1], f[2], n[name][0] + f[3], f[4], f[5], f[6])
                    continue
                stars = v[3].replace(name, "", 1).count("*") if isinstance(v[3], str) else 0
                to = v[2] + (" " + "*" * (stars - 1) if stars > 1 else "")
                return (name, fo, fs, n[name][0], n[name][1], v[4], to)
        return None


class Layouts(dict):
    """the structures' layouts; .inner: {"struct S.member": (is a union, its layout)} of those inside them"""
    def __init__(self):
        super().__init__()
        self.inner = {}


# ---------------------------------------------------------------- kinds

class K:
    """what a register holds: 'val' (a number, 16 bits), 'ptr' (an address; to = what it points to), 'opq' (unknown:
    possibly an address - kept whole), 'frame' (sp as it was at entry + old, new offsets), 'seg'"""
    __slots__ = ("kind", "to", "old", "new", "origin")

    def __init__(self, kind, to=None, old=0, new=0, origin=frozenset()):
        self.kind, self.to, self.old, self.new, self.origin = kind, to, old, new, origin

    def wide(self):
        return self.kind in ("ptr", "opq", "frame")

    def key(self):
        return (self.kind, self.to, self.old, self.new)

    def __repr__(self):
        return "%s%s" % (self.kind, ("(%s)" % self.to) if self.to else "")


VAL = K("val")


# routines of the old C library the assembly calls: today's C library's, as each was called
RUNTIME_C = {"__ffree": ("dos_ffree", False, [(4, "ptr")], "void"),
             "__fmalloc": ("dos_fmalloc", False, [(2, "val")], "ptr"),
             "_free": ("free", False, [(2, "ptr")], "void"), "_malloc": ("malloc", False, [(2, "val")], "ptr")}
CALL_WORDS = {"near", "far", "huge", "pascal", "cdecl", "fortran", "_near", "_far", "_pascal", "_cdecl"}


def c_functions(texts):
    """{C name: (pascal, [(the old size of each argument, val|long|ptr|double)], what it returns)} of the functions
    a C program defines - the old way (the arguments' types declared after the head), the macros made of calling
    words (#define NEAR near pascal) as they were"""
    from codebreaker.port import early_c
    macros = {}
    for t in texts:
        for m in re.finditer(r"(?m)^\s*#\s*define\s+(\w+)[ \t]+([\w \t]+?)\s*$", t):
            if set(m.group(2).split()) <= CALL_WORDS:
                macros[m.group(1)] = m.group(2)
    out = {}
    for t in texts:
        code = early_c.blank(t)
        for k, v in macros.items():
            code = re.sub(r"\b%s\b" % k, v, code)
        lines = code.split("\n")
        ds = early_c.depths(lines)
        for i, ln in enumerate(lines):
            m = re.match(r"^([\w\s*]*?)\b([A-Za-z_]\w*)\s*\(([\w\s,]*)\)\s*$", ln)
            if not m or ds[i] != 0 or m.group(2) in early_c.KEYWORDS:
                continue
            j, decls = i + 1, {}
            while j < len(lines) and not lines[j].strip().startswith("{"):
                d = lines[j].strip()
                if not d.endswith(";"):
                    break
                base = re.match(r"((?:\s*\b(?:register|unsigned|signed|long|short|int|char|float|double|"
                                r"struct\s+\w+|union\s+\w+|far|near|huge)\b)+)", d)
                if base:
                    for dcl in early_c.split_top(d[base.end():].rstrip(";")):
                        n = re.search(r"\w+", dcl)
                        if n:
                            decls[n.group(0)] = (base.group(1), dcl)
                j += 1
            if j >= len(lines) or not lines[j].strip().startswith("{"):
                continue
            ret, words = m.group(1), set(m.group(1).split())
            args = []
            for a in [x.strip() for x in m.group(3).split(",") if x.strip()]:
                base, dcl = decls.get(a, ("int", a))
                if "*" in dcl or "[" in dcl:
                    args.append((4 if re.search(r"\b(far|huge)\b", base + dcl.split(a)[0]) else 2, "ptr"))
                elif "double" in base or "float" in base:
                    args.append((8, "double"))
                elif "long" in base:
                    args.append((4, "long"))
                elif re.search(r"\b(struct|union)\b", base):
                    args = None
                    break
                else:
                    args.append((2, "val"))
            if args is None:
                continue
            rk = "ptr" if "*" in ret else "long" if "long" in words else "void" if "void" in words and \
                "*" not in ret else "val"
            to = None                                       # (what the address it gives points to)
            if rk == "ptr":
                base = " ".join(w for w in re.findall(r"\w+", ret.split("*")[0])
                                if w not in CALL_WORDS and w not in ("static", "extern", "register"))
                stars = ret.count("*")
                to = base + (" " + "*" * (stars - 1) if stars > 1 else "") if base else None
            old = out.get(m.group(2))
            if old is None or len(args) >= len(old[1]):
                out[m.group(2)] = ("pascal" in words, args, rk, to)
    return out


def inner_name(label, cnames):
    """the name the ported routine itself has (a routine called from C has the C name for its entry)"""
    n = label[1:] if label.startswith("_") else label
    c = {x.lower(): x for x in cnames}.get(n.lower(), n)
    return ("@" + label) if c == label else label


FIRST_MEMBER = {}                                       # (struct: the type of its first member, a pointer)


def join(a, b):
    if a is None:
        return b
    if a.kind == "ptr" and b is not None and b.kind == "ptr" and a.to != b.to and a.to and b.to:
        for x, y in ((a, b), (b, a)):                       # (a pointer to a structure whose first member is a
            if FIRST_MEMBER.get(y.to) == x.to:              #  pointer: a pointer to such a pointer too - a chain
                return K("ptr", x.to, origin=a.origin | b.origin)   # walked from its head's place)
    if b is None or a.key() == b.key():
        return a if b is None or a.origin >= b.origin else K(a.kind, a.to, a.old, a.new, a.origin | b.origin)
    if a.kind == "val" and b.kind == "val":
        return K("val", origin=a.origin | b.origin)
    if a.kind == "ptr" and b.kind == "ptr":
        return K("ptr", a.to if a.to == b.to else None, origin=a.origin | b.origin)
    if "val" in (a.kind, b.kind):
        o = a if a.kind != "val" else b
        return K("opq", o.to, origin=a.origin | b.origin)
    return K("opq", origin=a.origin | b.origin)


class State:
    def __init__(self):
        self.r = {}                                         # (16-bit name: K)
        self.stack = []                                     # ([old size, new size, K] from entry down)
        self.sp_old = self.sp_new = 0
        self.routines = frozenset()                         # (the routines this code runs for: a ret returns theirs)

    def copy(self):
        s = State()
        s.r = dict(self.r)
        s.stack = [list(x) for x in self.stack]
        s.sp_old, s.sp_new = self.sp_old, self.sp_new
        s.routines = self.routines
        return s

    def key(self):
        return (tuple(sorted((k, v.key(), v.origin) for k, v in self.r.items())),
                tuple((a, b, c.key() if c else None) for a, b, c in self.stack), self.sp_old, self.sp_new,
                self.routines)


def merge(a, b):
    if a is None:
        return b.copy()
    s = a.copy()
    for k in set(a.r) | set(b.r):
        s.r[k] = join(a.r.get(k), b.r.get(k))
    if len(a.stack) == len(b.stack):
        s.stack = [[x[0], x[1], join(x[2], y[2])] for x, y in zip(a.stack, b.stack)]
    s.routines = a.routines | b.routines
    return s


# ---------------------------------------------------------------- the module

class Module:
    offsets = {}                                            # ({label: (segment, offset)}: the old assembler's listing)

    def code_label(self, seg, value):
        """(label, distance) of a code address in a segment, by the old listing - None when it is none"""
        best = None
        for n, so in self.offsets.items():
            if n.startswith('@'):
                continue
            s, o = so
            if s == seg and o <= value and n in self.labels and (best is None or o > best[1]):
                best = (n, o)
        if best is None:
            return None
        nxt = min([so[1] for n, so in self.offsets.items() if not n.startswith('@') and so[0] == seg and
                   so[1] > best[1]] or [1 << 20])
        if value >= nxt:
            return None
        return best[0], value - best[1]

    def place_data(self):
        """each table line of the code: its old offset and length (from the listing, matched in order)"""
        lines = [x for x in self.offsets.get("@lines", [])]
        k = 0
        self.dline = {}                                     # (code index: (offset, bytes))
        for i, l in enumerate(self.code):
            if not (l.directive and l.directive[0] in ("DB", "DW", "DD")):
                continue
            want = re.sub(r"\s+", " ", strip_comment(l.text)).strip()[:24]
            for j in range(k, len(lines)):
                if lines[j][0] == l.seg and lines[j][3][:16] == want[:16]:
                    nxt = next((x[1] for x in lines[j + 1:] if x[0] == l.seg and x[1] > lines[j][1]), None)
                    self.dline[i] = (lines[j][1], (nxt - lines[j][1]) if nxt else lines[j][2])   # (its length: to
                    k = j + 1                                                                   #  the next line)
                    break

    def data_address(self, seg, v):
        """(label, distance) of a number that is an address in a table of the code segment - None when it is none:
        inside a table line, or just before one (a table counted from 1)"""
        best = None
        for i, (o, n) in self.dline.items():
            if self.code[i].seg != seg:
                continue
            if o <= v < o + max(n, 1):
                return "@d_%04x" % o, v - o
            if v < o <= v + 128 and (best is None or o < best[1]):   # (a table counted from a base before it)
                best = ("@d_%04x" % o, o)
        if best:
            return best[0], v - best[1]
        return None

    def table_targets(self):
        """the routines a table of the code segment names (DW place, as the listing places them)"""
        out = set()
        for l in self.code:
            if l.directive and l.directive[0] == "DW":
                for v in split_ops(strip_comment(l.directive[2])):
                    c = self.code_label(getattr(l, "seg", None), number(v)) if number(v) is not None else None
                    if c and c[1] == 0:
                        out.add(c[0])
                    o = re.fullmatch(r"(?i)offset\s+(?:\w+:)?(\w+)", v.strip())
                    if o and o.group(1) in self.labels:
                        out.add(o.group(1))
        return out

    def cs_address(self, i, r):
        """whether register r, given a number at line i, is then used to read the code segment (cs:[r]): the
        number is an address there"""
        for j in range(i + 1, min(i + 40, len(self.code))):
            l = self.code[j]
            if not l.op:
                continue
            if any(re.search(r"(?i)cs\s*:\s*\[[^\]]*\b%s\b" % r, x) for x in l.args):
                return True
            if l.args and l.op in ("mov", "lea", "pop", "les", "lds") and l.args[0].lower() == r:
                return False
            if l.op in ("ret", "retf", "jmp"):
                return False
        return False

    def __init__(self, text, ctypes, cnames=(), pascal=(), cfuncs=None, asm_routines=()):
        """text: the 16-bit source; ctypes: CTypes; cnames: the names the C program calls (to match the routines'
        public names, in capitals for pascal ones); pascal: those it calls the pascal way; cfuncs: the C program's
        functions (c_functions); asm_routines: the public routines of the program's other assembly modules"""
        self.lines = parse(text)
        self.cfuncs = cfuncs or {}
        self.data_used = set()
        for st_ in list(ctypes.old):                        # (the structures that begin with a pointer)
            f = ctypes.field(st_, 0)
            if f and f[5] and f[1] == 0 and f[2] == 2:
                FIRST_MEMBER[st_] = f[6] + " *"

        self.asm_routines = set(asm_routines)
        self.cnames_all = set(cnames)
        self.used_outside = set()                           # (its routines the other modules call)
        self.c_calls = {}
        self.ct = ctypes
        self.public, self.extrn, self.equ = set(), {}, {}
        self.code_seg = set()
        seg = None
        self.labels = {}
        self.code = []                                      # (the instructions of the code segments, in order)
        self.data = []                                      # ((segment, Line))
        for l in self.lines:
            d = l.directive
            if d and d[0] == "SEGMENT":
                seg = d[1]
                if "CODE" in d[2].upper() or seg.upper().endswith("TEXT"):
                    self.code_seg.add(seg)
                continue
            if d and d[0] == "ENDS":
                seg = None
                continue
            if d and d[0] == "PUBLIC":
                self.public |= {x.strip() for x in d[2].split(",") if x.strip()}
                continue
            if d and d[0] == "EXTRN":
                for x in d[2].split(","):
                    if ":" in x:
                        n, k = x.split(":", 1)
                        self.extrn[n.strip()] = k.strip().upper()
                continue
            if d and d[0] in ("EQU", "="):
                self.equ[d[1]] = d[2].strip()
                continue
            if seg in self.code_seg:
                l.seg = seg
                if d and d[0] == "LABEL":
                    l.label = d[1]
                if l.label:
                    self.labels[l.label] = len(self.code)
                if d and d[0] in ("DB", "DW") and self.code and self.code[-1].directive and \
                   self.code[-1].directive[0] == "DB" and number(self.code[-1].directive[2].split(";")[0]) in \
                   (0xCA, 0xC2) and not getattr(self.code[-1], "op", None):
                    prev = self.code[-1]                    # (DB 0CAh, DW n: a return written as its bytes)
                    prev.op, prev.args, prev.directive = ("retf" if number(prev.directive[2]) == 0xCA else "ret"), \
                        [strip_comment(d[2]).strip()], None
                    continue
                if d and d[0] == "DB" and number(strip_comment(d[2]).strip()) in (0xCB, 0xC3):
                    l.op, l.args, l.directive = ("retf" if number(strip_comment(d[2]).strip()) == 0xCB else "ret"), \
                        [], None
                if l.op or l.label or (d and d[0] in ("DB", "DW", "DD")):
                    self.code.append(l)
            elif seg:
                self.data.append((seg, l))
        eq = Equates(self.equ)
        eq.data = {n: {"BYTE": 1, "WORD": 2, "DWORD": 4, "QWORD": 8, "TBYTE": 10}[k] for n, k in self.extrn.items()
                   if k in ("BYTE", "WORD", "DWORD", "QWORD", "TBYTE")}
        for seg_, l in self.data:
            d = l.directive
            if d and d[0] in ("DB", "DW", "DD", "DQ", "DT") and d[1]:
                eq.data[d[1]] = {"DB": 1, "DW": 2, "DD": 4, "DQ": 8, "DT": 10}[d[0]]
        self.equ = eq
        lower = {n.lower(): n for n in cnames}
        self.cname = {}                                     # (a public routine: the name the C program calls)
        for p in self.public:
            if p in self.labels:
                n = p[1:] if p.startswith("_") else p
                self.cname[p] = lower.get(n.lower(), n)
        self.pascal = {p for p in self.cname if self.cname[p] in pascal or not p.startswith("_")}
        self.notes = []

    # -- the kinds, to a fixed point

    def entry_state(self, routine):
        s = State()
        for r in R16:
            s.r[r] = K("opq", origin=frozenset([("in", routine, r)]))
        s.r["sp"] = K("frame", old=0, new=0)
        s.routines = frozenset([routine])
        return s

    def analyse(self):
        self.before = {}
        work = collections.deque()
        for p in self.cname:                                # (a routine called from outside - the C program, the
            if self.cname[p] not in self.cnames_all and p not in self.used_outside and p in self.called:
                continue                                    #  other modules - or by none: an entry; else its
            i = self.labels[p]                              #  callers' states are what it is given)
            self.before[i] = self.entry_state(p)
            work.append(i)
        for p in sorted(self.table_targets()):               # (a routine called through a table of the code: an
            i = self.labels[p]                              #  entry of its own, whoever calls it)
            if i not in self.before:
                self.before[i] = self.entry_state(p)
                work.append(i)
        self.routine_of = {}
        seen = 0
        while work:
            i = work.popleft()
            seen += 1
            if seen > 200000:
                raise Asm16Error("the kinds do not settle")
            st = self.before[i].copy()
            l = self.code[i]
            nxt = []
            if l.op:
                pre_call = st.copy()                        # (the state the callee is entered with: before)
                st, flow = self.step(i, l, st)
            else:
                flow = ["next"]
            for f in flow:
                if f == "next":
                    nxt.append(i + 1)
                elif f == "ret":
                    pass
                elif isinstance(f, tuple) and f[0] == "jump":
                    if f[1] in self.labels:
                        nxt.append(self.labels[f[1]])
                elif isinstance(f, tuple) and f[0] == "call":
                    t = self.labels.get(f[1])
                    if t is not None:
                        cs = pre_call.copy()
                        cs.stack.append([2, 8, K("ret")])   # (the return address)
                        cs.sp_old -= 2
                        cs.sp_new -= 8
                        rel = State()
                        rel.r = dict(cs.r)
                        for r in R16:
                            if cs.r.get(r) and cs.r[r].kind == "frame":
                                rel.r[r] = K("opq")         # (a frame of the caller's is none of the callee's)
                        rel.r["sp"] = K("frame", old=0, new=0)
                        rel.routines = frozenset([f[1]])
                        self._flow(t, rel, work)
                    nxt.append(i + 1)
            for j in nxt:
                if j < len(self.code):
                    self._flow(j, st, work)

    def _flow(self, j, st, work):
        old = self.before.get(j)
        new = merge(old, st)
        if old is None or new.key() != old.key():
            self.before[j] = new
            work.append(j)

    def absolute(self, m):
        """[1f62h]: memory at an old address of the data segment, as a number - the variable there (or m)"""
        if m.sym or m.regs or m.disp < 0x100 or m.seg == "cs":
            return m
        v = self.data_var(m.disp) if self.data_map else None
        if not v:
            return m
        self.data_used.add(v[0])
        return Mem(m.size, None, v[0], [], v[1], m.text)

    def kind_of_mem(self, m, st):
        """the kind of what a memory operand holds, and its new place: (K or None, where)"""
        m = self.absolute(m)
        if m.sym and len(m.regs) == 1 and self.ct.is_pointer(self.c(m.sym)) and \
                "[" in (self.ct.var(self.c(m.sym)) or ("", ""))[1]:
            k_ = st.r.get(m.regs[0])                        # (table[bx]: an element of an array of pointers)
            if not (k_ and k_.wide()):
                return K("ptr", self.ct.pointee(self.c(m.sym))), None
        if m.sym and not m.regs:
            name = self.c(m.sym)
            if (m.sym, m.disp) in self.own_ptr:              # (a slot of the module's own data holding an address)
                return K("ptr"), None
            if self.ct.is_pointer(name) and (m.size in (None, 2, 4)) and m.disp == 0:
                return K("ptr", self.ct.pointee(name)), None
            if self.ct.is_pointer(name) and name in self.ct.far_ptrs and m.disp == 2 and m.size in (None, 2):
                return K("seg"), None                       # (the segment half of a far pointer variable)
            return VAL, None
        if len(m.regs) == 1 and not m.sym:
            b = st.r.get(m.regs[0])
            if b and b.kind == "ptr" and b.to and b.to.startswith("@ds:"):
                return self.kind_of_mem(self.ds_mem(m, b), st)
            if b and b.kind == "ptr" and b.to and b.to.endswith("*") and m.disp % 2 == 0 and m.size in (None, 2):
                return K("ptr", b.to[:-1].strip()), None    # (an element of an array of pointers: one of them)
            if b and b.kind == "ptr" and b.to and b.to.startswith(("struct", "union")) and not b.to.endswith("*"):
                f = self.ct.field(b.to, m.disp)
                if f:
                    name, fo, fs, no, ns, isptr, ftype = f
                    if isptr and m.disp == fo:
                        v = self.ct.vars.get("@field")
                        to = re.sub(r"\b(far|near|huge|extern|static)\b", "", ftype).strip()
                        return K("ptr", to), f
                    if isptr and m.disp == fo + 2 and fs == 4:
                        return K("seg"), f
                    return VAL, f
            if b and b.kind == "frame" and b.old + m.disp < 0:
                e, o = b.old + m.disp, 0                    # (below the entry: what was pushed there, or room)
                for so, sn, k in st.stack:
                    if o - so <= e < o:
                        return (k if k is not None and so == 2 and sn == 8 else VAL), None
                    o -= so
                return VAL, None
            if b and b.kind == "frame":
                e = b.old + m.disp
                for r_ in (st.routines or ()):
                    to = self.param_place(r_).get(e)
                    if to and (m.size in (None, 2, 4)):     # (an argument the C program passes a pointer in)
                        return K("ptr", to), None
                return K("opq", origin=frozenset([("param", m.regs[0], e)])), None
        return VAL, None

    param_types = {}

    def param_place(self, routine):
        """{old offset from the entry's sp: what the argument there points to} - of the C program's calls"""
        args = self.param_types.get(routine)
        if not args:
            return {}
        out, at = {}, self.retsize(routine)
        for size, to in args:
            if to:
                out[at] = to
            at += size
        return out

    def element_sizes(self, k):
        """(old, new) size of what a pointer's kind points to - (1, 1) when not known"""
        to = k.to or ""
        if to.endswith("*"):
            return 2, 8
        if to.startswith(("struct", "union")):
            o, n = self.ct.old.get(to), self.ct.new.get(to)
            if o and n:
                return o["@size"], n["@size"]
        return 1, 1

    data_map = ()                                           # ([(old address in the data segment, name)], sorted)

    def data_var(self, addr):
        """(name, offset inside it) of an old address of the data segment - None when it is none of them"""
        best = None
        for k, (a, n) in enumerate(self.data_map):
            if a <= addr:
                best = k
            else:
                break
        if best is None:
            return None
        a, n = self.data_map[best]
        nxt = self.data_map[best + 1][0] if best + 1 < len(self.data_map) else a + 0x10000
        return (n, addr - a) if addr < nxt else None

    def data_at(self, v):
        """the variable whose old address is exactly v (a number of the code) - None"""
        if v is None or v < 0x100:
            return None
        for a_, n_ in self.data_map:
            if a_ == v:
                return n_
        return None

    def ds_base(self, i, r):
        """whether register r, given a number at line i, is then used as the base of a memory operand of the data
        segment (not cs:) before it is given another value"""
        for j in range(i + 1, min(i + 60, len(self.code))):
            l = self.code[j]
            if not l.op:
                continue
            for x in l.args:
                if re.search(r"\[[^\]]*\b%s\b" % r, x, re.I) and not re.search(r"(?i)cs\s*:", x):
                    return True
            if l.args and l.op in ("mov", "lea", "pop", "les", "lds", "xchg") and l.args[0].lower() == r:
                return False
            if l.op in ("ret", "retf", "jmp", "call"):
                return False
        return False

    def ds_mem(self, m, k):
        """a memory operand at a register given an old address of the data segment: the variable there"""
        addr = int(k.to[4:], 16) + m.disp
        v = self.data_var(addr)
        if not v:
            raise Asm16Error("the data segment's %04xh: no variable known there" % addr)
        self.data_used.add(v[0])
        return Mem(m.size, None, v[0], [], v[1], m.text)

    def callee(self, t):
        """a routine outside the module called: ("asm", its name) - another module's routine, called as it was -
        or ("c", C name, (pascal, [(old size, kind)], returns)) - the C program's or the C library's - or None"""
        if t in self.asm_routines:
            return ("asm", inner_name(t, self.cnames_all))
        if t in RUNTIME_C:
            r = RUNTIME_C[t]
            return ("c", r[0], r[1:])
        lower = {k.lower(): k for k in self.cfuncs}
        if t.startswith("_") and t[1:] in self.cfuncs and not self.cfuncs[t[1:]][0]:
            return ("c", t[1:], self.cfuncs[t[1:]])
        k = lower.get(t.lower())
        if not t.startswith("_") and k and self.cfuncs[k][0]:
            return ("c", k, self.cfuncs[k])
        return None

    @staticmethod
    def slots(sig):
        """the old stack's words of a C function's arguments: [(arg index, size, kind, first slot)], the number of
        slots (the lowest address first: pascal pushed the first argument first)"""
        pascal, args = sig[0], sig[1]
        order = list(range(len(args)))
        if pascal:
            order.reverse()
        out, s = [], 0
        for j in order:
            size, kind = args[j]
            out.append((j, size, kind, s))
            s += size // 2
        return sorted(out), s

    def c_thunk(self, name, sig):
        """the call of a C function from the old code: its arguments, on the stack as the old caller pushed them
        (8 bytes each word), given today's way; what it returns where the old code looks (ax, dx:ax)"""
        args, n = self.slots(sig)
        regs = ["rcx", "rdx", "r8", "r9"]
        extra = max(0, len(args) - 4)
        out = ["", "; %s from the old code: the arguments from the old stack, %s" % (
            name, "taken off by it (pascal)" if sig[0] else "left for the caller"),
            "@@c_%s PROC" % name, "\tpush\tr15", "\tmov\tr15, rsp", "\tand\trsp, -16",
            "\tsub\trsp, %d" % (32 + 8 * (extra + extra % 2))]
        for j, size, kind, sl in args:
            at = "[r15+%d]" % (16 + 8 * sl)
            hi = "[r15+%d]" % (16 + 8 * (sl + 1))
            dst = regs[j] if j < 4 else "rax"
            d32 = {"rcx": "ecx", "rdx": "edx", "r8": "r8d", "r9": "r9d", "rax": "eax"}[dst]
            if kind == "ptr":
                out.append("\tmov\t%s, qword ptr %s" % (dst, at))
            elif kind == "long":
                out += ["\tmovzx\t%s, word ptr %s" % (d32, at), "\tmovzx\tr11d, word ptr %s" % hi,
                        "\tshl\tr11d, 16", "\tor\t%s, r11d" % d32]
            elif kind == "double":
                out.append("\tmov\t%s, qword ptr %s" % (dst, at))
                if j < 4:
                    out.append("\tmovq\txmm%d, %s" % (j, dst))
            else:
                out.append("\tmovsx\t%s, word ptr %s" % (dst, at))
            if j >= 4:
                out.append("\tmov\tqword ptr [rsp+%d], rax" % (32 + 8 * (j - 4)))
        out.append("\tcall\t%s" % name)
        if sig[2] == "long":
            out += ["\tmov\tedx, eax", "\tshr\tedx, 16\t\t; (a long: dx:ax)"]
        out += ["\tmov\trsp, r15", "\tpop\tr15", "\tret\t%d" % (8 * n) if sig[0] and n else "\tret",
                "@@c_%s ENDP" % name]
        return out

    def c(self, sym):
        """the C name of an assembler symbol (_x is C's x)"""
        return sym[1:] if sym.startswith("_") else sym

    def step(self, i, l, st):
        """the state after an instruction, and where control goes"""
        op, a = l.op, [operand(x, self.equ) for x in l.args]
        flow = ["next"]
        if op in ("mov", "pop") and l.args and l.args[0].strip().lower() in ("es", "ds"):
            st.r.pop("@" + l.args[0].strip().lower(), None)   # (another segment: no address's now)

        def setr(r, k):
            if r in R8:
                st.r[R8[r]] = K("val", origin=(st.r.get(R8[r]) or VAL).origin)
            elif r in R16:
                st.r[r] = k

        def get(o):
            if o[0] == "reg":
                return st.r.get(o[1]) if o[1] in R16 else (VAL if o[1] in R8 else K("seg"))
            if o[0] == "off":
                return K("ptr", self.ct.var(self.c(o[1]))[0] if self.ct.var(self.c(o[1])) else None)
            if o[0] == "mem":
                return self.kind_of_mem(o[1], st)[0]
            return VAL

        if op in JUMPS:
            flow = [("jump", l.args[0].split()[-1])]       # (SHORT, NEAR PTR...: the label is the last word)
            if op != "jmp":
                flow.append("next")
            if op.startswith("loop") or op == "jcxz":
                pass
            return st, flow
        if op == "call":
            t = l.args[0].split()[-1]
            for r in ("ax", "cx", "dx", "bx"):
                if t not in self.labels:
                    st.r[r] = VAL if r != "ax" else K("opq")
            ce = self.callee(t) if t not in self.labels else None
            if ce and ce[0] == "c" and ce[2][0]:            # (pascal: the callee took its arguments off)
                words = self.slots(ce[2])[1]
                while words > 0 and st.stack:
                    it = st.stack.pop()
                    st.sp_old += it[0]
                    st.sp_new += it[1]
                    words -= it[0] // 2
            if ce and ce[0] == "c" and ce[2][2] == "long":
                st.r["dx"] = VAL
            if ce and ce[0] == "c" and ce[2][2] == "ptr":    # (the address the C function gives: its type's)
                st.r["ax"] = K("ptr", ce[2][3] if len(ce[2]) > 3 else None)
            if t in self.labels:
                after = self.exit_of(t)
                mods = self.writes(t)
                if after:
                    for r in ("ax", "bx", "cx", "dx", "si", "di", "bp"):
                        if r not in mods:                   # (what the routine never writes is the caller's still)
                            continue
                        if r in after.r:
                            k = after.r[r]
                            if not (k.origin and any(o[0] == "in" for o in k.origin)):
                                st.r[r] = k
            return st, [("call", t)]
        if op in ("ret", "retf", "retn", "iret"):
            for rt in (st.routines or {self.routine(i)}):     # (the routines whose code this is)
                self.exits_new.setdefault(rt, []).append(st.copy())
                if op == "retf":                            # (a far return: a far routine, its arguments 4 on)
                    self.far = set(self.far) | {rt}
            return st, ["ret"]
        if op == "push":
            k = get(a[0]) if a[0][0] in ("reg", "mem") else VAL
            gone = a[0][0] == "reg" and a[0][1] in SEG      # (a segment register: not pushed any more)
            st.stack.append([2, 0 if gone else 8, k])
            st.sp_old -= 2
            st.sp_new -= 0 if gone else 8
            return st, flow
        if op in ("pushf", "pushfw"):
            st.stack.append([2, 8, VAL])
            st.sp_old -= 2
            st.sp_new -= 8
            return st, flow
        if op in ("pop", "popf", "popfw"):
            item = st.stack.pop() if st.stack else [2, 8, VAL]
            k = item[2]
            st.sp_old += 2
            st.sp_new += item[1]
            if op == "pop" and a[0][0] == "reg":
                setr(a[0][1], k)
            return st, flow
        if op in ("mov", "xchg", "lea", "les", "lds") and a and a[0][0] == "reg":
            d = a[0][1]
            if op == "xchg" and len(a) > 1:
                k0, k1 = get(a[0]), get(a[1])
                setr(d, k1)
                if a[1][0] == "reg":
                    setr(a[1][1], k0)
                return st, flow
            if op == "lea":
                to = None
                if a[1][0] == "mem" and len(a[1][1].regs) == 1 and not a[1][1].sym:
                    kb = st.r.get(a[1][1].regs[0])
                    if kb and kb.kind == "ptr" and kb.to and kb.to.startswith(("struct", "union")) and \
                            not kb.to.endswith("*"):
                        to = self.ct.member_type(kb.to, a[1][1].disp)   # (the address of a member: its type's)
                setr(d, K("ptr", to))
                return st, flow
            if op in ("les", "lds"):
                k = get(a[1])
                setr(d, k if k and k.wide() else K("ptr"))
                st.r["@" + op[1:]] = K("seg", origin=frozenset([("segof", d)]))
                return st, flow
            if a[1][0] == "reg" and a[1][1] in ("es", "ds") and d in R16 and st.r.get("@" + a[1][1]):
                partner = [o[1] for o in st.r["@" + a[1][1]].origin if o[0] == "segof"]
                pk = st.r.get(partner[0]) if partner else None
                if pk and pk.wide():                        # (the segment of an address: the address, for its
                    setr(d, pk)                             #  test against 0)
                    return st, flow
            src = a[1]
            if src[0] == "reg" and src[1] == "sp":
                setr(d, K("frame", old=st.sp_old, new=st.sp_new))
            elif d in SEG or (src[0] == "reg" and src[1] in SEG):
                if d in R16:
                    setr(d, K("seg"))
            elif d == "cx":
                setr(d, VAL)                                # (cx addresses nothing on the 8086: a count)
            elif src[0] == "imm" and d in ("bx", "si", "di") and number(l.args[1]) is not None and \
                    ((self.cs_address(i, d) and (self.data_address(l.seg, number(l.args[1])) or
                                                 self.code_label(l.seg, number(l.args[1])))) or
                     self.data_address(l.seg, number(l.args[1])) and number(l.args[1]) >= 0x100):
                # (an address register given a number in a table of the code, or just before one: its address)
                setr(d, K("ptr", "code"))                   # (an address in the code segment: a table)
            elif src[0] == "imm" and d in ("bx", "si", "di") and number(l.args[1]) is not None and self.data_map \
                    and any(a_ == number(l.args[1]) for a_, n_ in self.data_map) and self.ds_base(i, d):
                setr(d, K("ptr", "@ds:%x" % number(l.args[1])))   # (an old address of the data segment: a
                                                                    #  variable's)
            else:
                k_ = get(src) or VAL
                setr(d, VAL if k_.kind == "seg" and src[0] == "mem" else k_)   # (a far address's segment: a number)
            return st, flow
        if op == "sub" and a[0] == ("reg", "sp"):
            n = number(l.args[1]) or 0
            st.stack.append([n, n, VAL])
            st.sp_old -= n
            st.sp_new -= n
            return st, flow
        if op == "add" and a[0] == ("reg", "sp"):
            n = number(l.args[1]) or 0
            st.sp_old += n
            while n > 0 and st.stack:
                item = st.stack.pop()
                n -= item[0]
                st.sp_new += item[1]
            return st, flow
        if op in STRING or op in ("cbw", "cwd", "mul", "imul", "div", "idiv", "lahf"):
            if op in ("lodsb", "lodsw", "cbw", "lahf"):
                st.r["ax"] = VAL
            if op in ("cwd", "mul", "imul", "div", "idiv"):
                st.r["ax"] = VAL
                st.r["dx"] = VAL
            if l.prefix or op in ("loop",):
                st.r["cx"] = VAL
            return st, flow
        if op == "int":
            st.r["ax"] = VAL
            return st, flow
        if a and a[0][0] == "reg" and op in ("add", "sub", "inc", "dec", "neg", "and", "or", "xor", "adc", "sbb",
                                              "not", "shl", "shr", "sal", "sar", "rol", "ror", "rcl", "rcr"):
            d = a[0][1]
            k = st.r.get(d) if d in R16 else VAL
            if d in R8:
                setr(d, VAL)
            elif op == "xor" and len(a) > 1 and a[1] == a[0]:
                setr(d, VAL)
            elif op in ("sub", "cmp") and k and k.wide() and len(a) > 1 and a[1][0] == "imm" and \
                    self.data_at(number(l.args[1])):
                if op == "sub":
                    setr(d, VAL)                            # (an address less a variable's: how far into it)
            elif op in ("add", "sub") and k and k.wide():
                s2 = get(a[1]) if len(a) > 1 else VAL
                if op == "sub" and s2 and s2.wide():
                    setr(d, K("opq", origin=k.origin | s2.origin) if k.kind == "opq" and s2.kind == "opq" else VAL)
                elif k.kind == "frame":
                    n = number(l.args[1]) or 0
                    setr(d, K("frame", old=k.old + (n if op == "add" else -n), new=k.new + (n if op == "add" else -n)))
                else:
                    setr(d, k)
            elif op in ("inc", "dec") and k and k.wide():
                setr(d, k)
            elif op in ("and", "or") and len(a) > 1 and a[1] == a[0]:
                pass                                        # (a test of itself)
            else:
                setr(d, VAL)
            if l.op == "add" and len(a) > 1 and k and k.kind == "val":
                s2 = get(a[1])
                if s2 and s2.wide() and s2.kind != "frame":
                    setr(d, s2)                             # (a value added to a pointer: the pointer)
            return st, flow
        return st, flow

    def writes(self, label, _seen=None):
        """the 16-bit registers a routine (from its label: the lines its jumps reach, and the routines it calls)
        gives a value - a push and pop of one around it not counted"""
        if not hasattr(self, "_writes"):
            self._writes = {}
        if label in self._writes:
            return self._writes[label]
        seen = _seen if _seen is not None else set()
        if label in seen or label not in self.labels:
            return set()
        seen.add(label)
        out, todo, done, pushed = set(), [self.labels[label]], set(), collections.Counter()
        while todo:
            i = todo.pop()
            if i in done or i >= len(self.code):
                continue
            done.add(i)
            l = self.code[i]
            nxt = [i + 1]
            if l.op:
                a0 = l.args[0].lower() if l.args else ""
                r16 = R8.get(a0, a0)
                if l.op == "push" and a0 in R16:
                    pushed[a0] += 1
                elif l.op == "pop" and a0 in R16:
                    if not pushed[a0]:
                        out.add(a0)                         # (a pop of what it did not push: a value given)
                elif l.op in ("mov", "add", "sub", "and", "or", "xor", "adc", "sbb", "inc", "dec", "neg", "not",
                              "shl", "shr", "sal", "sar", "rol", "ror", "rcl", "rcr", "lea", "les", "lds",
                              "xchg") and r16 in R16 and not (l.op in ("and", "or") and len(l.args) == 2 and
                                                               l.args[0].lower() == l.args[1].lower()):
                    out.add(r16)
                    if l.op == "xchg" and len(l.args) == 2 and R8.get(l.args[1].lower(), l.args[1].lower()) in R16:
                        out.add(R8.get(l.args[1].lower(), l.args[1].lower()))
                if l.op in ("lodsb", "lodsw", "cbw", "lahf", "in"):
                    out.add("ax")
                if l.op in ("cwd", "mul", "imul", "div", "idiv"):
                    out |= {"ax", "dx"}
                if l.op in STRING:
                    out |= {"si"} if l.op[:4] == "lods" else {"di"} if l.op[:4] in ("stos", "scas") else {"si", "di"}
                    if l.prefix:
                        out.add("cx")
                if l.op.startswith("loop") or l.op == "int":
                    out |= {"cx"} if l.op.startswith("loop") else {"ax"}
                if l.op == "call":
                    t = l.args[0].split()[-1]
                    out |= self.writes(t, seen) if t in self.labels else {"ax", "bx", "cx", "dx"}
                if l.op in JUMPS:
                    t = l.args[0].split()[-1]
                    if t in self.labels:
                        nxt.append(self.labels[t])
                    if l.op == "jmp":
                        nxt = nxt[1:]
                if l.op in ("ret", "retf", "retn", "iret"):
                    nxt = []
            todo += nxt
        self._writes[label] = out
        return out

    def routine(self, i):
        """the public routine (or the label called) a line belongs to: the nearest such label above it"""
        best = None
        for p, j in self.labels.items():
            if j <= i and (best is None or j > self.labels[best]) and (p in self.cname or p in self.called):
                best = p
        return best

    def exit_of(self, label):
        ex = self.exits.get(label) or self.exits_new.get(label)   # (the round before's, whole; else this one's)
        if not ex:
            return None
        s = None
        for e in ex:
            s = merge(s, e)
        return s

    own_ptr = frozenset()
    exits = {}
    exits_new = {}

    def run(self):
        self.prepare()
        self.called = {l.args[0].split()[-1] for l in self.code if l.op == "call"} | self.table_targets()
        self.far = set(self.far) | {self.routine(i) for i, l in enumerate(self.code) if l.op == "retf"} - {None}
        for rnd in range(4):                                # (then again, knowing the slots that hold addresses)
            for _ in range(4):                              # (the callees' exits, and again: each round looks
                self.exits_new = {}                         #  up the round before's, whole)
                self.analyse()
                self.exits = self.exits_new
            found = self.own_slots()
            if found == self.own_ptr:
                break
            self.own_ptr = found
        self._params()
        return self

    def own_data(self):
        """{label: bytes} of the module's own data (its data segments' labels)"""
        out, cur = {}, None
        for seg, l in self.data:
            d = l.directive
            if d and d[0] in ("DB", "DW", "DD", "DQ", "DT") and d[1]:
                cur = d[1]
                out[cur] = 0
            elif d and d[0] == "LABEL":
                cur = d[1]
                out[cur] = 0
            if d and d[0] in ("DB", "DW", "DD", "DQ", "DT") and cur:
                m = re.match(r"\s*(\d+)\s+DUP", d[2], re.I)
                unit = {"DB": 1, "DW": 2, "DD": 4, "DQ": 8, "DT": 10}[d[0]]
                out[cur] += (int(m.group(1)) if m else len(split_ops(strip_comment(d[2])))) * unit
        return out

    def own_slots(self):
        """the words of the module's own data that are given an address (a whole register stored there): their
        (label, offset) - those alone, where no other access overlaps them"""
        own = self.own_data()
        acc, ptr = collections.defaultdict(set), set()
        for i, l in enumerate(self.code):
            st = self.before.get(i)
            if not st or not l.op:
                continue
            ops = [operand(x, self.equ) for x in l.args]
            for k_, o in enumerate(ops):
                if o[0] == "off" and o[1] in own:
                    acc[o[1]].add((o[2], 0))
                if o[0] != "mem" or not o[1].sym or o[1].sym not in own or o[1].regs:
                    continue
                m = o[1]
                other = ops[1 - k_] if len(ops) == 2 else None
                size = m.size or (1 if other and other[0] == "reg" and other[1] in R8 else 2)
                acc[m.sym].add((m.disp, size))
                if l.op in ("mov", "xchg") and k_ == 0 and other and other[0] == "reg" and other[1] in R16:
                    kr = st.r.get(other[1])
                    if kr and kr.wide() and kr.kind != "frame" and size == 2:
                        ptr.add((m.sym, m.disp))
        good = set()
        for sym, d in ptr:                                  # (none other reaches into the slot)
            if all(not (o < d + 2 and d < o + max(sz, 1)) or (o, sz) in ((d, 2), (d, 0)) for o, sz in acc[sym]):
                if self.used_as_address(sym, d):
                    good.add((sym, d))
        return frozenset(good)

    def used_as_address(self, sym, d):
        """the evidence that a slot holds an address: what is read from it is then used as one (a register's
        base, the si or di of a string instruction), or it is compared with an address"""
        for i, l in enumerate(self.code):
            st = self.before.get(i)
            if not st or not l.op:
                continue
            ops = [operand(x, self.equ) for x in l.args]
            hit = [k for k, o in enumerate(ops) if o[0] == "mem" and o[1].sym == sym and o[1].disp == d and
                   not o[1].regs]
            if not hit:
                continue
            if l.op in ("cmp", "sub") and len(ops) == 2 and ops[1 - hit[0]][0] == "reg":
                k = st.r.get(ops[1 - hit[0]][1])
                if k and k.kind == "ptr" or ops[1 - hit[0]][1] in R16 and                         self.address_use(i + 1, ops[1 - hit[0]][1], 3, set()):
                    return True                             # (compared with an address)
            if l.op == "mov" and hit[0] == 1 and ops[0][0] == "reg" and ops[0][1] in R16 and \
                    self.address_use(i + 1, ops[0][1], 3, set()):
                return True
        return False

    def address_use(self, j0, r, depth, seen):
        """whether register r, from line j0 on (into the routines called, depth levels), is used as an address
        before it is given another value"""
        if j0 in seen:
            return False
        seen.add(j0)
        for j in range(j0, min(j0 + 60, len(self.code))):
            l2 = self.code[j]
            if not l2.op:
                continue
            if any(re.search(r"\[[^\]]*\b%s\b" % r, x, re.I) for x in l2.args):
                return True                                 # (used as a base)
            if l2.op in STRING and ((r == "si" and l2.op[:4] in ("lods", "movs", "cmps")) or
                                    (r == "di" and l2.op[:4] in ("stos", "movs", "cmps", "scas"))):
                return True                                 # (the string instruction's address)
            if l2.op == "call" and depth and l2.args[0].split()[-1] in self.labels:
                if self.address_use(self.labels[l2.args[0].split()[-1]], r, depth - 1, seen):
                    return True
            if l2.args and l2.args[0].lower() == r and l2.op in ("mov", "lea", "les", "lds", "xor"):
                return False                                # (given another value; a pop gives back the one pushed)
            if l2.op in ("ret", "retf", "jmp"):
                return False
        return False

    def own_place(self, sym, disp):
        """an old offset in the module's own data: the new one (the slots that hold addresses 8 bytes now)"""
        return disp + 6 * sum(1 for s2, d in self.own_ptr if s2 == sym and d < disp)

    def prepare(self):
        """before the kinds: the table lines of the code placed (the listing's offsets); a short jump written as
        its two bytes (DB 74h / DB 18h) the jump again, to the label where the listing puts its target"""
        self.place_data()
        jcc = {0x70: "jo", 0x71: "jno", 0x72: "jb", 0x73: "jae", 0x74: "je", 0x75: "jne", 0x76: "jbe", 0x77: "ja",
               0x78: "js", 0x79: "jns", 0x7C: "jl", 0x7D: "jge", 0x7E: "jle", 0x7F: "jg", 0xEB: "jmp"}
        for i, l in enumerate(self.code[:-1]):
            n = self.code[i + 1]
            if not (l.directive and n.directive and l.directive[0] == n.directive[0] == "DB" and i in self.dline
                    and not n.label):
                continue
            op, d = number(strip_comment(l.directive[2]).strip()), number(strip_comment(n.directive[2]).strip())
            if op not in jcc or d is None or op is None or jcc[op] not in JUMPS:
                continue
            to = self.dline[i][0] + 2 + (d - 256 if d >= 128 else d)
            c = self.code_label(l.seg, to)
            if c and c[1] == 0:
                l.op, l.args, l.directive = jcc[op], [c[0]], None
                n.directive = None
                self.notes.append("line %d: a jump written as its bytes - %s %s" % (l.no, jcc[op], c[0]))
        lines = self.offsets.get("@lines", [])
        at = {(x[0], x[1]): x for x in lines}
        for i, l in enumerate(self.code[:-1]):              # (an instruction written as its bytes, running into
            n = self.code[i + 1]                            #  the next line's: the instruction, the next gone)
            if not (l.directive and l.directive[0] == "DB" and i in self.dline and n.op and not n.label):
                continue
            db = bytes(number(x) & 0xFF for x in split_ops(strip_comment(l.directive[2])) if number(x) is not None)
            nxt = at.get((l.seg, self.dline[i][0] + len(db)))
            if not nxt or len(nxt) < 5:
                continue
            ins = decode_direct(db + nxt[4][:4])
            if ins and len(db) < ins[0] == len(db) + len(nxt[4]):
                l.op, l.args, l.directive = ins[1], ins[2], None
                n.op, n.args = None, []
                self.notes.append("line %d: an instruction written as its bytes, with the next line's - %s %s" % (
                    l.no, ins[1], ", ".join(ins[2])))
        return self

    # -- the parameters: where each routine reads its arguments on the stack

    def _params(self):
        """{routine: [(old offset from the entry's sp, old size)]} of the arguments each reads, in order"""
        acc = collections.defaultdict(dict)
        for i, l in enumerate(self.code):
            st = self.before.get(i)
            if not st or not l.op:
                continue
            for x in l.args:
                o = operand(x, self.equ)
                if o[0] != "mem" or len(o[1].regs) != 1:
                    continue
                k = st.r.get(o[1].regs[0])
                if k and k.kind == "frame":
                    e = k.old + o[1].disp
                    r = self.routine(i)
                    size = o[1].size or (4 if l.op in ("les", "lds") else 2)
                    if l.op in ("les", "lds"):
                        size = 4
                    acc[r][e] = max(acc[r].get(e, 0), size)
        self.params = {}
        for r, d in acc.items():
            rs = self.retsize(r)
            out = []
            for e in sorted(d):
                if e < rs:
                    continue
                if out and e < out[-1][0] + out[-1][1]:
                    continue                                # (the upper half of one read whole)
                out.append((e, d[e]))
            self.params[r] = out

    def retsize(self, r):
        return 4 if r in self.far else 2

    def slot(self, r, e):
        """an argument's place: (slot number, offset inside it) of an old offset from the entry's sp"""
        for k, (o, s) in enumerate(self.params.get(r, [])):
            if o <= e < o + s:
                return k, e - o
        raise Asm16Error("%s: no argument at sp+%d" % (r, e))

    # -- the 64-bit text

    far = set()

    def emit(self, longret=(), types=None):
        """the module for ml64: data, an entry of today's C for each routine it calls, the routines"""
        self.used_int21 = False
        self.ptr_args = {}
        self.unreached = []                                 # (lines no path reaches: said in the report)
        out = ["; %s" % "ported by CodeBreaker (port/asm16) for a 64-bit machine: the instructions as they were, the",
               "; registers that hold addresses used whole, members of structures at their new places, segments gone,",
               "; DOS called through the platform layer.", "OPTION CASEMAP:NONE", ""]
        out += ["%s\tEQU\t%s" % (k, v) for k, v in sorted(self.equ.items()) if number(v) is not None]
        ext = []
        self.asm_calls = set()
        self.c_calls = {}
        for n, kind in sorted(self.extrn.items()):
            if kind in ("NEAR", "FAR", "PROC") and (n in self.asm_routines or self.callee(n)):
                continue                                    # (called: by its own name, or its C name, below)
            if self.c(n) in self.aliases:                   # (a place inside another: the other is extern)
                own = "EXTERN %s:BYTE" % self.aliases[self.c(n)][0]
                if own not in ext:
                    ext.append(own)
                continue
            ext.append("EXTERN %s:%s" % (self.c(n), "PROC" if kind in ("NEAR", "FAR", "PROC") else kind))
        body = []
        bases = set()
        import os as _os
        debug = _os.environ.get("CB_ASM16_DEBUG") == "1"    # (every label public: a debugger names them)
        for i, l in enumerate(self.code):
            if (debug or l.label in self.public and l.label in self.labels) and l.label and not l.directive:
                body.append("PUBLIC %s" % self.local(l.label))   # (the other modules call it as it was)
            if l.seg not in bases:
                bases.add(l.seg)
                body.append("@base_%s:\t\t\t; (the start of the segment %s: what its tables count from)" % (
                    l.seg, l.seg))
            if l.directive and l.directive[0] in ("DB", "DW", "DD", "LABEL"):
                body += self.code_data(l)
                continue
            if l.label:
                body.append("%s:" % self.local(l.label))
            if l.op:
                st = self.before.get(i)
                if st is None:
                    body.append("\t; (never reached) %s" % strip_comment(l.text).strip())
                    self.unreached.append(l.no)
                    continue
                try:
                    body += ["\t" + x for x in self.instr(i, l, st)]
                except Asm16Error as e:
                    raise Asm16Error("line %d (%s): %s" % (l.no, l.text.strip(), e))
        if self.used_int21:
            ext.append("EXTERN dos_int21:PROC")
        for n in sorted(self.data_used):                    # (variables reached by their old addresses)
            if n not in self.extrn and n not in self.public:
                e_ = "EXTERN %s:BYTE" % self.sym_any(n)
                if e_ not in ext:
                    ext.append(e_)
        ext += ["EXTERN %s:PROC" % n for n in sorted(self.asm_calls)]
        ext += ["EXTERN %s:PROC" % n for n in sorted(self.c_calls)]
        wrappers = []
        for n, sig in sorted(self.c_calls.items()):
            wrappers += self.c_thunk(n, sig)
        for p in sorted(self.cname, key=lambda p: self.labels[p]):
            wrappers += self.wrapper(p, p in longret)
        if getattr(self, "used_null", False):
            ext.append("EXTERN dos_nullseg:BYTE")
        out += ext + ["", ".data"]
        if self.used_int21:
            out.append("dosregs\tQWORD\t8 DUP (0)\t\t; (the registers given to DOS and back)")
        out += self.data_text()
        out += ["", ".code"]
        out += wrappers
        out += body + ["", "END"]
        return "\n".join(out) + "\n"

    def code_data(self, l):
        """a table in the code segment: as it was - a word that is the place of a routine of the segment, that place
        (counted from the segment's start, as the old one was)"""
        kind, name, rest = l.directive
        rest = strip_comment(rest).strip()
        if kind == "LABEL":
            return ["%s\tLABEL\t%s" % (name, rest)]
        out = ["%s:" % l.label] if l.label else []
        at = getattr(self, "dline", {}).get(self.code.index(l))
        if at:
            out.append("@d_%04x:" % at[0])
        if kind == "DW":
            vals = []
            for v in split_ops(rest):
                c = self.code_label(l.seg, number(v)) if number(v) is not None else None
                o = re.fullmatch(r"(?i)offset\s+(?:\w+:)?(\w+)", v.strip())
                if o and o.group(1) in self.labels:         # (a routine's place, named: from the segment's start)
                    vals.append("%s - @base_%s" % (self.local(o.group(1)), l.seg))
                    continue
                vals.append("%s - @base_%s" % (c[0], l.seg) if c and c[1] == 0 else v)
            return out + ["\tDW\t%s" % ", ".join(vals)]
        return out + ["\t%s\t%s" % (kind, rest)]

    def local(self, label):
        return ("@" + label) if label in self.cname and self.cname[label] == label else label

    def data_text(self):
        out = []
        for seg, l in self.data:
            d = l.directive
            if not d:
                if l.label:
                    out.append("%s:" % l.label)
                continue
            kind, name, rest = d
            if kind in ("PUBLIC", "EXTRN", "ASSUME", "TITLE", "END"):
                continue
            nm = self.c(name) if name and name in self.public else name
            pub = ["PUBLIC %s" % nm] if name and name in self.public else []
            rest = strip_comment(rest).strip()
            if kind == "LABEL":
                out += pub + ["%s\tLABEL\t%s" % (nm, rest)]
            elif kind in ("DB", "DW", "DD", "DQ", "DT") and name and \
                    any(s2 == name for s2, d in self.own_ptr):   # (room for the addresses it holds, 8 bytes each)
                grow = 6 * sum(1 for s2, d in self.own_ptr if s2 == name)
                out += pub + ["%s\t%s\t%s\t\t; (%d bytes more: %d words of it hold addresses)" % (
                    nm, kind, rest, grow, grow // 6), "\tDB\t%d DUP (0)" % grow]
            elif kind in ("DB", "DW", "DD", "DQ", "DT"):
                if kind == "DW" and re.search(r"(?i)\boffset\b", rest):
                    vals = [re.sub(r"(?i)offset\s+(?:\w+:)?", "", v).strip() for v in split_ops(rest)]
                    out += pub + ["%s\tDQ\t%s" % (nm or "", ", ".join(self.c(v) if v.startswith("_") else v
                                                                        for v in vals))]
                else:
                    out += pub + ["%s\t%s\t%s" % (nm or "", kind, rest)]
            elif kind is None and l.directive:
                pass
        return out

    def wrapper(self, p, longret):
        """the entry of today's C: its arguments (rcx, rdx, r8, r9, then the stack) pushed as the old caller pushed
        them, the registers today's C keeps kept"""
        n = len(self.params.get(p, []))
        pascal = p in self.pascal
        saved = ["rbx", "rsi", "rdi", "rbp", "r15"]
        out = ["", "; %s(%s) - %s" % (self.cname[p], ", ".join("a%d" % j for j in range(n)),
                                      "pascal: the first argument pushed first, the routine takes them off" if pascal
                                      else "the last argument pushed first, taken off after"),
               "PUBLIC %s" % self.cname[p], "%s PROC" % self.cname[p]]
        out += ["\tpush\t%s" % r for r in saved]
        regs = ["rcx", "rdx", "r8", "r9"]
        pushed = 0
        ptrs = {k for k, (o, sz) in enumerate(self.params.get(p, [])) if o in self.ptr_args.get(p, set())}
        for k in reversed(range(n)):                        # (slot 0 at the lowest address, as the old stack had it)
            j = (n - 1 - k) if pascal else k
            if j < 4:
                reg = regs[j]
            else:
                off = 8 * len(saved) + 8 * pushed + 8 + 32 + 8 * (j - 4)
                out.append("\tmov\tr11, qword ptr [rsp+%d]" % off)
                reg = "r11"
            if k in ptrs:                                   # (a NULL address read what began the data segment)
                lab = "@@null_%s_%d" % (self.cname[p], k)
                out += ["\ttest\t%s, %s" % (reg, reg), "\tjnz\t%s" % lab, "\tlea\t%s, dos_nullseg" % reg,
                        "%s:" % lab]
                self.used_null = True
            out.append("\tpush\t%s" % reg)
            pushed += 1
        out.append("\tcall\t%s" % self.local(p))
        if not pascal and n:
            out.append("\tadd\trsp, %d" % (8 * n))
        if longret:
            out += ["\tmovzx\teax, ax", "\tshl\tedx, 16", "\tor\teax, edx\t\t; (a long: dx:ax)"]
        out += ["\tpop\t%s" % r for r in reversed(saved)]
        out += ["\tcld", "\tret", "%s ENDP" % self.cname[p]]
        return out

    # -- one instruction

    def reg(self, r, st, wide=None):
        if r in R8:
            return r
        w = st.r.get(r).wide() if wide is None and st.r.get(r) else bool(wide)
        return R64[r] if w else r

    aliases = {}                                            # (C name: (owner, byte offset) - a name the old linker
                                                            #  gave a place inside another variable)

    data_used = set()

    def sym_any(self, s):
        """the C name of a variable of the program, whether the module declares it or not"""
        c = self.c(s)
        if c in self.aliases:
            return self.aliases[c][0]
        return c

    def sym(self, s):
        if s in self.extrn or s in self.public or s in self.data_used:
            c = self.c(s)
            if c in self.aliases:
                owner, off = self.aliases[c]
                return "%s+%d" % (owner, off) if off else owner
            return c
        return s

    def mem(self, m, st, size, i, pre):
        """a memory operand at its new place: text (with its size), the instructions it needs first in pre"""
        sz = {1: "byte", 2: "word", 4: "dword", 8: "qword", 10: "tbyte"}[size] + " ptr "
        m = self.absolute(m)
        disp = m.disp
        parts = []
        scratch = ["r11", "r9"]
        if m.sym:
            name = self.sym(m.sym)
            disp = self.own_place(m.sym, disp)
            if not m.regs:
                return sz + name + ("%+d" % disp if disp else "")
            pre.append("lea\tr10, %s" % name)
            parts.append("r10")
            es_ = self.ct.elem_sizes(self.c(m.sym)) if m.sym in self.extrn or m.sym in self.public else None
            k_ = st.r.get(m.regs[0]) if len(m.regs) == 1 else None
            if es_ and es_[0] != es_[1] and len(m.regs) == 1 and not (k_ and k_.wide()):
                so, sn = es_                                # (an index of the old elements: today's)
                if so & (so - 1):
                    raise Asm16Error("an index into elements of %d bytes" % so)
                pre += ["movzx\tr11d, %s" % m.regs[0], "imul\tr11, r11, %d" % sn,
                        "sar\tr11, %d\t\t; (%d bytes an element then, %d now)" % (so.bit_length() - 1, so, sn)]
                disp = disp // so * sn + disp % so
                return sz + "[r10+r11" + ("%+d" % disp if disp else "") + "]"
        if len(m.regs) == 1 and not m.sym:
            k = st.r.get(m.regs[0])
            if k and k.kind == "ptr" and k.to and k.to.startswith("@ds:"):
                return self.mem(self.ds_mem(m, k), st, size, i, pre)
        for r in m.regs:
            k = st.r.get(r)
            self.as_address(i, k)
            if k and k.kind == "frame":
                rt = self.routine(i)
                e = k.old + disp
                if e >= self.retsize(rt):
                    sl, inner = self.slot(rt, e)
                    new = 8 + 8 * sl + inner
                else:
                    new = self.stack_place(st, e)
                disp = new - k.new
                parts.append(R64[r])
            elif k and k.wide():
                if k.kind == "ptr" and k.to and k.to.endswith("*") and len(m.regs) == 1 and not m.sym:
                    disp = disp * 4                         # (pointers of 2 bytes, 8 now)
                elif k.kind == "ptr" and k.to and k.to.startswith(("struct", "union")) and len(m.regs) == 1 and \
                        not m.sym:
                    f = self.ct.field(k.to, disp)
                    if f:
                        disp = f[3] + (disp - f[1])
                parts.append(R64[r])
            else:
                s = scratch.pop(0)
                pre.append("movzx\t%sd, %s" % (s, r))
                parts.append(s)
        inside = "+".join(parts) + ("%+d" % disp if disp else "")
        return sz + "[" + inside + "]"

    def as_address(self, i, k):
        """an argument of the routine used as an address here: noted (its NULL is the old data segment's start)"""
        if not k:
            return
        for o in k.origin:
            if o[0] == "param":
                self.ptr_args.setdefault(self.routine(i), set()).add(o[2])

    def stack_place(self, st, e):
        """an old offset below the entry's sp: its new one (the pushes and the room made, as they are now)"""
        o = n = 0
        for so, sn, k in st.stack:
            if o - so <= e < o:
                return n - sn + (e - (o - so))              # (the same byte of the item: a push now 8 bytes)
            o -= so
            n -= sn
        raise Asm16Error("sp%+d is not on the stack as it is here" % e)

    def access_size(self, l, a, st, which):
        """the size the instruction's memory operand is used at, now"""
        m = a[which][1]
        k, f = self.kind_of_mem(m, st)
        other = a[1 - which] if len(a) > 1 else None
        if other and other[0] == "reg":
            r = other[1]
            if r in R8:
                return 1
            ko = st.r.get(r)
            if ko and ko.wide() and ko.kind != "frame" and (k and k.wide() or f and f[5] or
                                                             (m.regs and st.r.get(m.regs[0]) and
                                                              st.r.get(m.regs[0]).kind == "frame")):
                return 8
            return 2
        if m.size:
            if m.size == 2 and k and k.kind == "ptr" and f and f[5]:
                return m.size
            return m.size
        return 2

    def instr(self, i, l, st):
        op = l.op
        a = [operand(x, self.equ) for x in l.args]
        pre = []
        R = lambda r, w=None: self.reg(r, st, w)

        def M(which, size=None):
            return self.mem(a[which][1], st, size or self.access_size(l, a, st, which), i, pre)

        def done(*lines):
            return pre + list(lines)

        segop = lambda o: o[0] == "reg" and o[1] in SEG
        if op == "mov" and a[0][0] == "reg" and a[0][1] in R16 and a[1][0] == "reg" and a[1][1] in ("es", "ds") \
                and st.r.get("@" + a[1][1]):
            partner = [o[1] for o in st.r["@" + a[1][1]].origin if o[0] == "segof"]
            pk = st.r.get(partner[0]) if partner else None
            if pk and pk.wide():
                return ["mov\t%s, %s\t\t; (the segment of the address in %s: the address, 0 when it is)" % (
                    R64[a[0][1]], R64[partner[0]], partner[0])]
        if op in ("mov",) and (segop(a[0]) or segop(a[1])):
            return ["; %s\t\t(a segment: one flat memory)" % strip_comment(l.text).strip()]
        if op in ("push", "pop") and segop(a[0]):
            return ["; %s\t\t(a segment: one flat memory)" % strip_comment(l.text).strip()]
        src_at = 0 if op == "push" else 1 if op in ("or", "and", "add", "sub", "cmp", "adc", "sbb", "xor",
                                                     "test") and len(a) == 2 and a[0][0] == "reg" else None
        if src_at is not None and a[src_at][0] == "mem":     # (the segment half of a far address read: none now,
            k_, f_ = self.kind_of_mem(a[src_at][1], st)      #  0 - the address is whole in the other half's place)
            if k_ and k_.kind == "seg":
                if op == "push":
                    return ["push\t0\t\t; (%s: a segment, none now)" % strip_comment(l.text).strip()]
                kd_ = st.r.get(a[0][1])
                d_ = R64[a[0][1]] if a[0][1] in R16 and kd_ and kd_.wide() else a[0][1]
                return ["%s\t%s, 0\t\t; (%s: a segment, none now)" % (op, d_, strip_comment(l.text).strip())]
        if op in JUMPS:
            t = l.args[0].split()[-1]
            if op == "jcxz" or op.startswith("loop"):       # (a short jump only, and the flags kept: through a
                n = "@@j%d" % i                            #  jump beside it to the place, however far)
                return ["movzx\tecx, cx", "%s\t%s_t" % ("jrcxz" if op == "jcxz" else op, n), "jmp\t%s_n" % n,
                        "%s_t:" % n, "jmp\t%s" % t, "%s_n:" % n]
            return ["%s\t%s" % (op, t)]
        if op == "call":
            t = l.args[0].split()[-1]
            if t in self.labels:
                return ["call\t%s" % self.local(t)]
            if a[0][0] == "mem" and a[0][1].seg == "cs":     # (through a table of the code: its entries are the
                pre2 = []                                   #  routines' places from the segment's start)
                where = self.mem(a[0][1], st, 2, i, pre2)
                return pre2 + ["movzx\tr11d, %s" % where, "lea\tr10, @base_%s" % l.seg, "add\tr11, r10",
                               "call\tr11"]
            ce = self.callee(t)
            if ce and ce[0] == "asm":
                self.asm_calls.add(ce[1])
                return ["call\t%s" % ce[1]]
            if ce:
                self.c_calls[ce[1]] = ce[2]
                return ["call\t@@c_%s" % ce[1]]
            raise Asm16Error("a call of %s, outside the module (not yet ported)" % t)
        if op in ("ret", "retn", "retf"):
            if l.args:
                rt = self.routine(i)
                nbytes = number(l.args[0])
                n = len([1 for o, s in self.params.get(rt, []) if o < self.retsize(rt) + nbytes])
                return ["ret\t%d" % (8 * n)]
            return ["ret"]
        if op == "int":
            if number(l.args[0]) != 0x21:
                raise Asm16Error("int %s" % l.args[0])
            self.used_int21 = True
            return ["mov\tqword ptr dosregs, rax", "mov\tqword ptr dosregs+8, rbx", "mov\tqword ptr dosregs+16, rcx",
                    "mov\tqword ptr dosregs+24, rdx", "mov\tqword ptr dosregs+32, rsi",
                    "mov\tqword ptr dosregs+40, rdi", "mov\tr15, rsp\t\t; (DOS: the platform layer, called as today's "
                    "C is)", "and\trsp, -16", "sub\trsp, 32", "lea\trcx, dosregs", "call\tdos_int21", "mov\trsp, r15",
                    "mov\trax, qword ptr dosregs", "mov\trbx, qword ptr dosregs+8", "mov\trcx, qword ptr dosregs+16",
                    "mov\trdx, qword ptr dosregs+24", "bt\tqword ptr dosregs+48, 0\t; (the carry: an error)"]
        if op in ("xlat", "xlatb"):                          # (al from the table at bx: the table's address whole)
            k = st.r.get("bx")
            if not (k and k.wide()):
                raise Asm16Error("xlat with bx not an address")
            return ["xlatb"]
        if op in ("pushf", "pushfw"):
            return ["pushfq"]
        if op in ("popf", "popfw"):
            return ["popfq"]
        if op == "push":
            if a[0][0] == "reg":
                return ["push\t%s" % R64[a[0][1]]]
            if a[0][0] == "mem":
                k, f = self.kind_of_mem(a[0][1], st)
                if k and k.wide():
                    return done("push\t" + M(0, 8))
                return done("movzx\tr11d, " + M(0, 2), "push\tr11")
            if a[0][0] == "off":
                return ["lea\tr11, %s" % self.sym(a[0][1]), "push\tr11"]
            return ["push\t%s" % l.args[0]]
        if op == "pop":
            if a[0][0] == "reg":
                return ["pop\t%s" % R64[a[0][1]]]
            raise Asm16Error("pop to memory")
        if op in ("les", "lds"):
            return done("mov\t%s, %s" % (R64[a[0][1]], M(1, 8)))
        if op == "lea":
            return done("lea\t%s, %s" % (R64[a[0][1]], M(1, 8)[len("qword ptr "):]))
        if op in ("mov", "xchg") and a[0][0] == "reg" and a[1][0] == "reg":
            d, s = a[0][1], a[1][1]
            if s == "sp":
                return ["%s\t%s, rsp" % (op, R64[d])]
            if d in R8 or s in R8:
                return ["%s\t%s, %s" % (op, d, s)]
            ks, kd = st.r.get(s), st.r.get(d)
            w = (ks and ks.wide()) or (op == "xchg" and kd and kd.wide())
            return ["%s\t%s, %s" % (op, R(d, w), R(s, w))]
        if op in ("mov", "xchg") and a[0][0] == "reg" and a[1][0] == "mem":
            d = a[0][1]
            k, f = self.kind_of_mem(a[1][1], st)
            if k and k.kind == "seg" and op == "mov" and d in R16:   # (the segment of a far address read: 0 when
                m_ = a[1][1]                                 #  the address is 0, else not - what it was tested for)
                lo = Mem(m_.size, m_.seg, m_.sym, m_.regs, m_.disp - 2, m_.text)
                where = self.mem(lo, st, 8, i, pre)
                return done("mov\tr11, %s" % where, "test\tr11, r11",
                            "setnz\tr11b\t\t; (%s: the segment of an address - 0 when it is 0)" % (
                                strip_comment(l.text).strip()), "movzx\te%s, r11b" % d if d not in ("si", "di", "bp")
                            else "movzx\te%s, r11b" % d)
            if k and k.kind == "seg":
                return ["; %s\t\t(a segment: one flat memory)" % strip_comment(l.text).strip()]
            if d in R8:
                return done("%s\t%s, %s" % (op, d, M(1, 1)))
            if k and k.wide() and d != "cx":                # (cx: a count, whatever the member was declared)
                return done("%s\t%s, %s" % (op, R64[d], M(1, 8)))
            return done("%s\t%s, %s" % (op, d, M(1, 2)))
        if op in ("mov", "xchg") and a[0][0] == "mem" and a[1][0] == "reg":
            s = a[1][1]
            k, f = self.kind_of_mem(a[0][1], st)
            if k and k.kind == "seg":
                return ["; %s\t\t(a segment: one flat memory)" % strip_comment(l.text).strip()]
            size = self.access_size(l, a, st, 0)
            if size == 2 and f and f[5] and f[2] == 4 and f[4] == 8 and s in R16 and op == "mov":
                return done("movzx\tr11d, %s\t\t; (the offset half of a far address: the address now)" % s,
                            "mov\t%s, r11" % M(0, 8))
            return done("%s\t%s, %s" % (op, M(0, size), R64[s] if size == 8 else s))
        if op == "mov" and a[0][0] == "reg" and a[1][0] == "off":
            dd = self.own_place(a[1][1], a[1][2])
            return ["lea\t%s, %s" % (R64[a[0][1]], self.sym(a[1][1]) + ("%+d" % dd if dd else ""))]
        k_after = None
        if op == "mov" and a[0][0] == "reg" and a[1][0] == "imm" and i + 1 < len(self.code):
            k_after = (self.before.get(i + 1) or State()).r.get(a[0][1])
        if k_after is not None and k_after.kind == "ptr" and k_after.to and k_after.to.startswith("@ds:"):
            v = self.data_var(int(k_after.to[4:], 16))
            self.data_used.add(v[0])
            return ["lea\t%s, %s%s\t\t; (%s: the old address of %s)" % (
                R64[a[0][1]], self.sym(v[0]), "%+d" % v[1] if v[1] else "", l.args[1], self.c(v[0]))]
        if k_after is not None and k_after.kind == "ptr" and k_after.to == "code":
            v = number(l.args[1])
            lab, dist = self.data_address(l.seg, v) or self.code_label(l.seg, v)
            return ["lea\t%s, %s%s\t\t; (%s: an address in the code)" % (R64[a[0][1]], lab,
                                                                     "%+d" % dist if dist else "", l.args[1])]
        if op == "mov" and a[0][0] == "reg" and a[1][0] == "sym" and (
                re.match(r"(?i)seg\s", a[1][1]) or a[1][1].upper() == "DGROUP" or a[1][1] in self.code_seg):
            return ["; %s\t\t(a segment: one flat memory)" % strip_comment(l.text).strip()]
        if op == "mov" and a[0][0] == "reg" and a[1][0] in ("imm", "sym"):
            return ["mov\t%s, %s" % (a[0][1], l.args[1])]
        if op == "mov" and a[0][0] == "mem" and a[1][0] == "off":     # (an address stored: the whole of it)
            dd = self.own_place(a[1][1], a[1][2])
            return done("lea\tr11, %s" % (self.sym(a[1][1]) + ("%+d" % dd if dd else "")),
                        "mov\t%s, r11" % M(0, 8))
        if op == "mov" and a[0][0] == "mem":
            k, f = self.kind_of_mem(a[0][1], st)
            size = a[0][1].size or 2
            if k and k.kind == "ptr" and f and f[5] and number(l.args[1]) == 0:
                size = 8
            return done("mov\t%s, %s" % (M(0, size), l.args[1]))
        if op in ("add", "sub", "and", "or", "xor", "cmp", "test", "adc", "sbb") and a[0][0] == "reg":
            d = a[0][1]
            if d == "sp":
                n = number(l.args[1]) or 0
                return ["%s\trsp, %d" % (op, self.sp_new_bytes(st, n, op))]
            kd = st.r.get(d) if d in R16 else None
            if op == "xor" and a[1] == a[0] and d in R16:
                return ["xor\te%s, e%s" % (d, d)] if d not in ("si", "di", "bp") else ["xor\te%s, e%s" % (d, d)]
            if a[1][0] == "imm" and op == "sub" and d in R16 and not (kd and kd.wide()) and \
                    self.data_at(number(l.args[1])):
                v = self.data_at(number(l.args[1]))         # (an offset less a variable's old address: today's, the
                self.data_used.add(v)                       #  low 16 bits - the same difference)
                return ["lea\tr11, %s\t\t; (%s: the old address of %s)" % (self.sym(v), l.args[1], self.c(v)),
                        "sub\t%s, r11w" % d]
            if a[1][0] == "imm" and op in ("sub", "cmp", "add") and kd and kd.wide() and \
                    self.data_at(number(l.args[1])):
                v = self.data_at(number(l.args[1]))         # (the old address of a variable: its address now)
                self.data_used.add(v)
                return ["lea\tr11, %s\t\t; (%s: the old address of %s)" % (self.sym(v), l.args[1], self.c(v)),
                        "%s\t%s, r11" % (op, R64[d])]
            if d in R8:
                if a[1][0] == "mem":
                    return done("%s\t%s, %s" % (op, d, M(1, 1)))
                return ["%s\t%s, %s" % (op, d, l.args[1])]
            src = a[1]
            if src[0] == "reg":
                ks = st.r.get(src[1]) if src[1] in R16 else None
                if src[1] in R8:
                    return ["%s\t%s, %s" % (op, d, src[1])]
                if kd and kd.wide() and ks and ks.wide():
                    return ["%s\t%s, %s" % (op, R64[d], R64[src[1]])]
                if kd and kd.wide() and kd.kind != "frame" and op in ("add", "sub"):
                    so, sn = self.element_sizes(kd)
                    if so != sn:                            # (an index into an array: the elements' size now)
                        out_ = ["movsx\tr11, %s" % src[1], "imul\tr11, r11, %d" % sn]
                        if so & (so - 1) == 0:
                            out_.append("sar\tr11, %d\t\t; (%d bytes an element then, %d now)" % (
                                so.bit_length() - 1, so, sn))
                        else:
                            raise Asm16Error("an index into elements of %d bytes" % so)
                        return out_ + ["%s\t%s, r11" % (op, R64[d])]
                    return ["movsx\tr11, %s" % src[1], "%s\t%s, r11" % (op, R64[d])]
                if op == "add" and ks and ks.wide() and ks.kind != "frame" and (not kd or not kd.wide()):
                    so, sn = self.element_sizes(ks)
                    if so != sn and so & (so - 1) == 0:     # (an index of the old elements: today's)
                        return ["movzx\te%s, %s" % (d, d), "imul\t%s, %s, %d" % (R64[d], R64[d], sn),
                                "sar\t%s, %d\t\t; (%d bytes an element then, %d now)" % (
                                    R64[d], so.bit_length() - 1, so, sn),
                                "add\t%s, %s\t\t; (a count added to an address: the address)" % (R64[d], R64[src[1]])]
                    return ["movzx\te%s, %s" % (d, d) if d not in ("si", "di", "bp") else "movzx\te%s, %s" % (d, d),
                            "add\t%s, %s\t\t; (a count added to an address: the address)" % (R64[d], R64[src[1]])]
                return ["%s\t%s, %s" % (op, d, src[1])]
            if src[0] == "mem":
                k, f = self.kind_of_mem(src[1], st)
                if kd and kd.wide() and k and k.wide():
                    return done("%s\t%s, %s" % (op, R64[d], M(1, 8)))
                if kd and kd.wide() and kd.kind != "frame" and op in ("add", "sub") and d in R16:
                    where = M(1, 2)                         # (a count from memory added to an address: the whole)
                    return pre + ["movsx\tr8, %s" % where, "%s\t%s, r8" % (op, R64[d])]
                return done("%s\t%s, %s" % (op, d, M(1, 2)))
            if src[0] == "off":
                return ["lea\tr11, %s" % self.sym(src[1]), "%s\t%s, r11" % (op, R64[d])]
            if op == "cmp" and kd and kd.kind == "ptr" and kd.to == "code" and number(l.args[1]) is not None and \
                    (self.data_address(l.seg, number(l.args[1])) or self.code_label(l.seg, number(l.args[1]))):
                lab, dist = self.data_address(l.seg, number(l.args[1])) or self.code_label(l.seg, number(l.args[1]))
                return ["lea\tr11, %s%s" % (lab, "%+d" % dist if dist else ""), "%s\t%s, r11" % (op, R64[d])]
            if kd and kd.wide() and kd.kind == "ptr":
                v = number(l.args[1])
                if v is not None and v >= 0x8000:
                    v -= 0x10000
                return ["%s\t%s, %s" % (op, R64[d], v if v is not None else l.args[1])]
            return ["%s\t%s, %s" % (op, d, l.args[1])]
        if op in ("add", "sub", "and", "or", "xor", "cmp", "test", "adc", "sbb") and a[0][0] == "mem":
            size = 1 if (len(a) > 1 and a[1][0] == "reg" and a[1][1] in R8) else (a[0][1].size or 2)
            k_, f_ = self.kind_of_mem(a[0][1], st)
            if op in ("add", "sub") and size == 2 and k_ and k_.kind == "ptr":   # (an address moved on by a
                if a[1][0] == "imm":                                             #  count: the whole of it)
                    return done("%s\t%s, %s" % (op, M(0, 8), l.args[1]))
                if a[1][0] == "reg" and a[1][1] in R16:
                    where = M(0, 8)                         # (r8: the operand's own place may want r11)
                    return pre + ["movsx\tr8, %s" % a[1][1], "%s\t%s, r8" % (op, where)]
            if op in ("cmp", "test") and size == 2 and a[1][0] == "imm" and number(l.args[1]) == 0:
                k_, f_ = self.kind_of_mem(a[0][1], st)
                if k_ and k_.kind == "ptr":                 # (an address compared with 0: the whole of it)
                    size = 8
            if size == 8:
                return done("%s\t%s, 0" % (op, M(0, 8)))
            return done("%s\t%s, %s" % (op, M(0, size), l.args[1] if a[1][0] != "reg" else a[1][1]))
        if op in ("inc", "dec", "neg", "not") and a[0][0] == "reg":
            d = a[0][1]
            k = st.r.get(d) if d in R16 else None
            return ["%s\t%s" % (op, R64[d] if k and k.wide() and op in ("inc", "dec") else d)]
        if op in ("inc", "dec", "neg", "not") and a[0][0] == "mem":
            k_, f_ = self.kind_of_mem(a[0][1], st)
            if op in ("inc", "dec") and k_ and k_.kind == "ptr" and (a[0][1].size or 2) == 2:
                return done("%s\t%s\t\t; (an address moved on: the whole of it)" % (op, M(0, 8)))
            return done("%s\t%s" % (op, M(0, a[0][1].size or 2)))
        if op in ("shl", "shr", "sal", "sar", "rol", "ror", "rcl", "rcr"):
            if a[0][0] == "mem":
                return done("%s\t%s, %s" % (op, M(0, a[0][1].size or 2), l.args[1]))
            return ["%s\t%s, %s" % (op, a[0][1], l.args[1])]
        if op in ("mul", "imul", "div", "idiv"):
            if a[0][0] == "mem":
                return done("%s\t%s" % (op, M(0, a[0][1].size or 2)))
            return ["%s\t%s" % (op, a[0][1])]
        if op in STRING:
            for r in ("si", "di"):
                if (op[:4] in ("lods", "movs", "cmps") and r == "si") or (op[:4] in ("stos", "movs", "cmps", "scas")
                                                                           and r == "di"):
                    self.as_address(i, st.r.get(r))
            pfx = []
            if l.prefix:
                pfx = ["movzx\tecx, cx", "%s %s" % (l.prefix, op)]
                return pfx
            return [op]
        if op in ("cbw", "cwd", "cld", "std", "stc", "clc", "cmc", "nop", "lahf", "sahf", "cli", "sti"):
            return [op]
        raise Asm16Error("no rule for %s" % op)

    def sp_new_bytes(self, st, n, op):
        """add/sub sp, n: the bytes it is now (the pushes it takes off are 8 bytes each, room made the same)"""
        if op == "sub":
            return n
        items, left, new = list(st.stack), n, 0
        while left > 0 and items:
            so, sn, k = items.pop()
            left -= so
            new += sn
        return new
