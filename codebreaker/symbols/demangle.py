#!/usr/bin/env python3
r"""demangle.py -- decode MSVC-mangled C++ names into readable signatures.

MFC, OLE and every C++ export ship their names MANGLED: `?AddRef@CFoo@@UAGKXZ`. The mangling
encodes the full authored signature -- access, virtual/static, calling convention, class scope,
return type and arguments -- so decoding it recovers real source-level names for a huge number of
functions at once, `public: virtual unsigned long __stdcall CFoo::AddRef(void)`. This implements the
subset of the MSVC scheme that covers period MFC/OLE binaries: qualified names and namespaces,
constructors/destructors/operators, the access + calling-convention decoration, the primitive and
pointer/reference/class/struct/enum type grammar, argument back-references, and data symbols.
"""

_PRIM = {"X": "void", "D": "char", "C": "signed char", "E": "unsigned char", "F": "short",
         "G": "unsigned short", "H": "int", "I": "unsigned int", "J": "long", "K": "unsigned long",
         "M": "float", "N": "double", "O": "long double", "_N": "bool", "_J": "__int64",
         "_K": "unsigned __int64", "_W": "wchar_t", "_D": "__int8", "_H": "__int32"}
_CV = {"A": "", "B": "const ", "C": "volatile ", "D": "const volatile "}
_CC = {"A": "__cdecl", "B": "__cdecl", "C": "__pascal", "E": "__thiscall", "G": "__stdcall",
       "I": "__fastcall"}
# access decoration char -> (access, is_static, is_virtual, is_global)
_ACCESS = {
    "A": ("private", 0, 0, 0), "B": ("private", 0, 0, 0),
    "C": ("private", 1, 0, 0), "D": ("private", 1, 0, 0),
    "E": ("private", 0, 1, 0), "F": ("private", 0, 1, 0),
    "I": ("protected", 0, 0, 0), "J": ("protected", 0, 0, 0),
    "K": ("protected", 1, 0, 0), "L": ("protected", 1, 0, 0),
    "M": ("protected", 0, 1, 0), "N": ("protected", 0, 1, 0),
    "Q": ("public", 0, 0, 0), "R": ("public", 0, 0, 0),
    "S": ("public", 1, 0, 0), "T": ("public", 1, 0, 0),
    "U": ("public", 0, 1, 0), "V": ("public", 0, 1, 0),
    "Y": ("", 0, 0, 1), "Z": ("", 0, 0, 1),
}
_OPS = {"0": "{ctor}", "1": "{dtor}", "2": "operator new", "3": "operator delete",
        "4": "operator=", "5": "operator>>", "6": "operator<<", "7": "operator!",
        "8": "operator==", "9": "operator!=", "A": "operator[]", "C": "operator->",
        "E": "operator++", "F": "operator--", "G": "operator-", "H": "operator+",
        "_G": "`scalar deleting destructor'", "_E": "`vector deleting destructor'"}


class _P:
    def __init__(self, s):
        self.s, self.i = s, 0
        self.argrefs = []                                   # back-referenceable compound arg types
        self.namerefs = []                                  # back-referenceable name fragments

    def eof(self):
        return self.i >= len(self.s)

    def peek(self):
        return self.s[self.i] if self.i < len(self.s) else ""

    def take(self):
        c = self.s[self.i]; self.i += 1
        return c

    def _ident(self):
        j = self.s.find("@", self.i)
        if j < 0:
            j = len(self.s)
        name = self.s[self.i:j]
        self.i = j
        if self.peek() == "@":
            self.i += 1
        return name

    def _nontype_arg(self):
        """A non-type template argument ($0<number>, ...) -- decoded best-effort, else a placeholder."""
        self.i += 1                                         # consume '$'
        code = self.take() if not self.eof() else ""
        val = ""
        while not self.eof() and self.peek() != "@" and self.peek().isalnum():
            val += self.take()
        if self.peek() == "@":
            self.i += 1
        if code == "0" and val[:1] in "0123456789":         # small integer literal
            return val
        return "/*non-type*/"

    def _name_frag(self):
        """One scope fragment: a template (?$Name@args@), an identifier, or a name-ref digit."""
        c = self.peek()
        if c == "?" and self.i + 1 < len(self.s) and self.s[self.i + 1] == "$":
            self.i += 2                                      # consume '?$'
            base = self._ident()                            # template name, consumes trailing @
            args = []
            while not self.eof() and self.peek() != "@":
                args.append(self._nontype_arg() if self.peek() == "$" else self.type())
            if self.peek() == "@":
                self.i += 1                                 # closing @ of the argument list
            frag = "%s<%s>" % (base, ", ".join(args))
            self.namerefs.append(frag)
            return frag
        if c.isdigit():                                     # back-reference to an earlier fragment
            self.i += 1
            k = int(c)
            return self.namerefs[k] if k < len(self.namerefs) else "?"
        frag = self._ident()
        self.namerefs.append(frag)
        return frag

    def qualified(self):
        """basename@scope@...@@  ->  Scope::...::name (basename is emitted first, scopes reversed)."""
        if self.peek() == "?":                              # special name (operator / ctor / dtor)
            self.i += 1
            if self.peek() == "_":
                code = self.s[self.i:self.i + 2]; self.i += 2
            else:
                code = self.take()
            base = _OPS.get(code, "operator?")
        else:
            base = self._name_frag()
        scopes = []
        while not self.eof() and self.peek() != "@":
            scopes.append(self._name_frag())
        if self.peek() == "@":
            self.i += 1                                     # consume the terminating @@ (second @)
        scopes = list(reversed(scopes))
        cls = scopes[-1] if scopes else ""
        if base == "{ctor}":
            base = cls
        elif base == "{dtor}":
            base = "~" + cls
        return "::".join(scopes + [base]), base, cls

    def type(self):
        c = self.peek()
        if c == "_":
            two = self.s[self.i:self.i + 2]
            if two in _PRIM:
                self.i += 2; return _PRIM[two]
        if c in _PRIM:
            self.i += 1; return _PRIM[c]
        if c in ("P", "Q", "R", "S"):                       # pointer (+cv on the pointee)
            self.i += 1
            cv = _CV.get(self.take(), "")
            inner = self.type()
            return "%s%s *" % (cv, inner)
        if c == "A":                                        # reference
            self.i += 1
            cv = _CV.get(self.take(), "")
            return "%s%s &" % (cv, self.type())
        if c in ("V", "U", "T"):                            # class / struct / union
            self.i += 1
            kw = {"V": "class ", "U": "struct ", "T": "union "}[c]
            nm, _b, _c = self.qualified()
            return kw + nm
        if c == "W":                                        # enum: W<size-char><name>
            self.i += 1
            self.take()                                     # underlying-size char
            nm, _b, _c = self.qualified()
            return "enum " + nm
        if c.isdigit():                                     # back-reference to a prior compound arg
            self.i += 1
            k = int(c)
            return self.argrefs[k] if k < len(self.argrefs) else "?"
        self.i += 1
        return "?"

    def args(self):
        if self.peek() == "X":                              # (void)
            self.i += 1
            return ["void"]
        out = []
        while not self.eof() and self.peek() not in ("@", "Z"):
            start = self.i
            t = self.type()
            if self.s[start] not in _PRIM and self.s[start] != "_":   # compound types are back-refable
                self.argrefs.append(t)
            out.append(t)
        if self.peek() == "@":
            self.i += 1
        return out or ["void"]


def short(sym):
    """Just the qualified name (CFoo::AddRef) of a mangled symbol -- for use as a call target."""
    if not sym or not sym.startswith("?"):
        return None
    try:
        return _P(sym[1:]).qualified()[0]
    except Exception:
        return None


def _clean_type(t):
    """MSVC type spelling -> a clean C type: drop the class/struct/union/enum tag
    keyword (`class CWnd *` -> `CWnd *`) so it reads as an authored C declaration."""
    if not t:
        return t
    for kw in ("class ", "struct ", "union ", "enum "):
        t = t.replace(kw, "")
    return t.strip()


def demangle_sig(sym):
    """Structured signature for an MSVC-mangled name, ready to feed a decompiler's
    per-function prototype. Returns None if not mangled / undecodable, else a dict:

        name     qualified name (CWnd::SetText)
        base     final component (SetText, ~CWnd, operator=)
        cls      enclosing class/namespace ("" if free function)
        kind     "func" | "data"
        access   "public"/"protected"/"private"/"" ; static/virtual: bool
        cc       calling convention (__thiscall / __stdcall / ...) or ""
        ret      return C type ("" when none: ctors/dtors/operator new-delete)
        args     list of explicit arg C types ([] for (void))
        this_type  "<cls> *" for a non-static member (the implicit `this`), else None
        type     for kind=="data": the variable's C type

    Types are cleaned C spellings (no `class`/`struct` tag keyword)."""
    if not sym or not sym.startswith("?"):
        return None
    try:
        p = _P(sym[1:])
        qname, base, cls = p.qualified()
        deco = p.peek()
        if deco.isdigit():                                  # data symbol: ?name@@3<type><cv>
            p.i += 1
            ty = _clean_type(p.type())
            return {"name": qname, "base": base, "cls": cls, "kind": "data",
                    "type": ty, "access": "", "static": False, "virtual": False,
                    "cc": "", "ret": None, "args": [], "this_type": None}
        if deco not in _ACCESS:
            return {"name": qname, "base": base, "cls": cls, "kind": "func",
                    "access": "", "static": False, "virtual": False, "cc": "",
                    "ret": None, "args": [], "this_type": None}   # name only, no type info
        p.i += 1
        access, is_static, is_virtual, is_global = _ACCESS[deco]
        is_member = not is_static and not is_global
        if is_member:
            p.take()                                        # `this` cv-qualifier (kept in string form only)
        cc = _CC.get(p.take(), "")
        no_ret = base.startswith(("~", "operator new", "operator delete")) or cls == base
        ret = "" if no_ret else _clean_type(p.type())
        args = [_clean_type(t) for t in p.args()]
        if args == ["void"]:
            args = []
        return {"name": qname, "base": base, "cls": cls, "kind": "func",
                "access": access, "static": bool(is_static), "virtual": bool(is_virtual),
                "cc": cc, "ret": ret, "args": args,
                "this_type": (cls + " *") if (is_member and cls) else None}
    except Exception:
        return None


def demangle(sym):
    """Readable C++ signature for an MSVC-mangled name, or None if not mangled / unsupported."""
    if not sym or not sym.startswith("?"):
        return None
    try:
        p = _P(sym[1:])
        qname, base, cls = p.qualified()
        deco = p.peek()
        if deco.isdigit():                                  # data symbol: ?name@@3<type><cv>
            p.i += 1
            ty = p.type()
            return ("%s %s" % (ty, qname)).strip()
        if deco not in _ACCESS:
            return qname                                    # a name with no (decodable) type info
        p.i += 1
        access, is_static, is_virtual, is_global = _ACCESS[deco]
        this_cv = ""
        if not is_static and not is_global:                 # member: `this` cv-qualifier
            this_cv = _CV.get(p.take(), "")
        cc = _CC.get(p.take(), "")
        ret = "" if base.startswith(("~", "operator new", "operator delete")) or cls == base else p.type()
        arglist = p.args()
        parts = []
        if access:
            parts.append(access + ":")
        if is_static:
            parts.append("static")
        if is_virtual:
            parts.append("virtual")
        if ret:
            parts.append(ret)
        if cc:
            parts.append(cc)
        sig = " ".join(parts)
        tail = (" " + this_cv.strip()) if this_cv else ""
        return "%s %s(%s)%s" % (sig, qname, ", ".join(arglist), tail) if sig else \
               "%s(%s)%s" % (qname, ", ".join(arglist), tail)
    except Exception:
        return None
