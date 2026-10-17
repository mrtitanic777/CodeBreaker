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
        m = re.match(r"^ ([0-9A-F]{4})  ((?:[0-9A-F]{2,4}R? ?)*)\s*(.*)$", l)
        if m and seg and len(l) > 32:
            nbytes = sum(len(t.rstrip("R")) // 2 for t in m.group(2).split())
            out["@lines"].append((seg, int(m.group(1), 16), nbytes, re.sub(r"\s+", " ", l[32:]).strip()[:24]))
        m = re.match(r"^ ([0-9A-F]{4}) {2,}.*?\b([A-Za-z_$@?][\w$@?]*)\s+SEGMENT\b", l)
        if m:
            seg = m.group(2)
            continue
        if re.search(r"\bENDS\b", l):
            seg = None
            continue
        m = re.match(r"^ ([0-9A-F]{4})\s+(?:[0-9A-F ]+R?\s+)?\s*([A-Za-z_$@?][\w$@?]*)(?::|\s+LABEL\b)", l)
        if m and seg:
            out[m.group(2)] = (seg, int(m.group(1), 16))
    return out


# ---------------------------------------------------------------- operands

class Mem:
    def __init__(self, size, seg, sym, regs, disp, text):
        self.size, self.seg, self.sym, self.regs, self.disp, self.text = size, seg, sym, regs, disp, text


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
    return ("sym", s)


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

    @staticmethod
    def _layouts(texts, ptr, far):
        out = {}
        for t in texts:
            code = early_c.blank(t)
            for m in re.finditer(r"\b(struct|union)\s+(\w+)\s*\{", code):
                d, k = 0, m.end() - 1
                while k < len(code):
                    d += {"{": 1, "}": -1}.get(code[k], 0)
                    if d == 0:
                        break
                    k += 1
                lay = CTypes._body(code[m.end():k], m.group(1) == "union", ptr, far, out)
                out.setdefault("%s %s" % (m.group(1), m.group(2)), lay)
        return out

    @staticmethod
    def _body(body, union, ptr, far, out):
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
                inner = CTypes._body(nest.group(2), nest.group(1) == "union", ptr, far, out)
                names = [x.strip() for x in nest.group(3).split(",") if x.strip()]
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

    def field(self, struct, off_old):
        """the member of a structure at an old offset: (name, old offset, old size, new offset, new size, is pointer,
        its type) - None when not known"""
        o, n = self.old.get(struct), self.new.get(struct)
        if not o or not n:
            return None
        for name, v in o.items():
            if name == "@size":
                continue
            fo, fs = v[0], v[1]
            if fo <= off_old < fo + fs and name in n:
                return (name, fo, fs, n[name][0], n[name][1], v[4], v[2])
        return None


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


def join(a, b):
    if a is None:
        return b
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

    def __init__(self, text, ctypes, cnames=(), pascal=()):
        """text: the 16-bit source; ctypes: CTypes; cnames: the names the C program calls (to match the routines'
        public names, in capitals for pascal ones); pascal: those it calls the pascal way"""
        self.lines = parse(text)
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
        entries = [self.labels[p] for p in self.cname]
        work = collections.deque()
        for p in self.cname:
            i = self.labels[p]
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

    def kind_of_mem(self, m, st):
        """the kind of what a memory operand holds, and its new place: (K or None, where)"""
        if m.sym and not m.regs:
            name = self.c(m.sym)
            if (m.sym, m.disp) in self.own_ptr:              # (a slot of the module's own data holding an address)
                return K("ptr"), None
            if self.ct.is_pointer(name) and (m.size in (None, 2, 4)) and m.disp == 0:
                return K("ptr", self.ct.pointee(name)), None
            return VAL, None
        if len(m.regs) == 1 and not m.sym:
            b = st.r.get(m.regs[0])
            if b and b.kind == "ptr" and b.to and b.to.startswith(("struct", "union")):
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
            if b and b.kind == "frame":
                return K("opq", origin=frozenset([("param", m.regs[0], b.old + m.disp)])), None
        return VAL, None

    def c(self, sym):
        """the C name of an assembler symbol (_x is C's x)"""
        return sym[1:] if sym.startswith("_") else sym

    def step(self, i, l, st):
        """the state after an instruction, and where control goes"""
        op, a = l.op, [operand(x, self.equ) for x in l.args]
        flow = ["next"]

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
                setr(d, K("ptr"))
                return st, flow
            if op in ("les", "lds"):
                k = get(a[1])
                setr(d, k if k and k.wide() else K("ptr"))
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
            else:
                setr(d, get(src) or VAL)
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
        """before the kinds: the table lines of the code placed (the listing's offsets)"""
        self.place_data()
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
        ext = []
        for n, kind in sorted(self.extrn.items()):
            ext.append("EXTERN %s:%s" % (self.c(n), "PROC" if kind in ("NEAR", "FAR", "PROC") else kind))
        body = []
        bases = set()
        import os as _os
        debug = _os.environ.get("CB_ASM16_DEBUG") == "1"    # (every label public: a debugger names them)
        for i, l in enumerate(self.code):
            if debug and l.label and not l.directive:
                body.append("PUBLIC %s" % self.local(l.label))
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
        wrappers = []
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

    def sym(self, s):
        if s in self.extrn or s in self.public:
            return self.c(s)
        return s

    def mem(self, m, st, size, i, pre):
        """a memory operand at its new place: text (with its size), the instructions it needs first in pre"""
        sz = {1: "byte", 2: "word", 4: "dword", 8: "qword", 10: "tbyte"}[size] + " ptr "
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
                if k.kind == "ptr" and k.to and k.to.startswith(("struct", "union")) and len(m.regs) == 1 and not m.sym:
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
        if op in ("mov",) and (segop(a[0]) or segop(a[1])):
            return ["; %s\t\t(a segment: one flat memory)" % strip_comment(l.text).strip()]
        if op in ("push", "pop") and segop(a[0]):
            return ["; %s\t\t(a segment: one flat memory)" % strip_comment(l.text).strip()]
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
            return done("%s\t%s, %s" % (op, M(0, size), R64[s] if size == 8 else s))
        if op == "mov" and a[0][0] == "reg" and a[1][0] == "off":
            dd = self.own_place(a[1][1], a[1][2])
            return ["lea\t%s, %s" % (R64[a[0][1]], self.sym(a[1][1]) + ("%+d" % dd if dd else ""))]
        k_after = None
        if op == "mov" and a[0][0] == "reg" and a[1][0] == "imm" and i + 1 < len(self.code):
            k_after = (self.before.get(i + 1) or State()).r.get(a[0][1])
        if k_after is not None and k_after.kind == "ptr" and k_after.to == "code":
            v = number(l.args[1])
            lab, dist = self.data_address(l.seg, v) or self.code_label(l.seg, v)
            return ["lea\t%s, %s%s\t\t; (%s: an address in the code)" % (R64[a[0][1]], lab,
                                                                     "%+d" % dist if dist else "", l.args[1])]
        if op == "mov" and a[0][0] == "reg" and a[1][0] in ("imm", "sym"):
            return ["mov\t%s, %s" % (a[0][1], l.args[1])]
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
                if kd and kd.wide() and kd.kind != "opq" and op in ("add", "sub"):
                    return ["movsx\tr11, %s" % src[1], "%s\t%s, r11" % (op, R64[d])]
                if op == "add" and ks and ks.wide() and ks.kind != "frame" and (not kd or not kd.wide()):
                    return ["movzx\te%s, %s" % (d, d) if d not in ("si", "di", "bp") else "movzx\te%s, %s" % (d, d),
                            "add\t%s, %s\t\t; (a count added to an address: the address)" % (R64[d], R64[src[1]])]
                return ["%s\t%s, %s" % (op, d, src[1])]
            if src[0] == "mem":
                k, f = self.kind_of_mem(src[1], st)
                if kd and kd.wide() and k and k.wide():
                    return done("%s\t%s, %s" % (op, R64[d], M(1, 8)))
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
            return done("%s\t%s, %s" % (op, M(0, size), l.args[1] if a[1][0] != "reg" else a[1][1]))
        if op in ("inc", "dec", "neg", "not") and a[0][0] == "reg":
            d = a[0][1]
            k = st.r.get(d) if d in R16 else None
            return ["%s\t%s" % (op, R64[d] if k and k.wide() and op in ("inc", "dec") else d)]
        if op in ("inc", "dec", "neg", "not") and a[0][0] == "mem":
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
