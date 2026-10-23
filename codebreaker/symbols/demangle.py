"""demangle.py -- a decorated linker name as the declaration it was made from.

    python -m codebreaker demangle "??0CString@@QAE@PBD@Z"
        public: __thiscall CString::CString(char const *)

    from codebreaker.symbols import demangle
    demangle.declaration("?AddRef@CFoo@@UAGKXZ")   "public: virtual unsigned long __stdcall CFoo::AddRef(void)"
    demangle.short("?AddRef@CFoo@@UAGKXZ")         "CFoo::AddRef"
    demangle.parse("?AddRef@CFoo@@UAGKXZ")         {"name", "kind", "access", "static", "virtual", "far",
                                                    "convention", "returns", "parameters", "const"}
    demangle.plain("_LZRead@12")                   ("LZRead", "__stdcall", 12)      (also "LZRead@12")

A C++ compiler writes a function's whole declaration into its linker name: the classes it is
nested in, whether it is public, static or virtual, its calling convention, its result and every
parameter type. Microsoft's scheme is the one the corpus uses (every C++ export and every MFC
import, 32-bit and 16-bit); the 16-bit compilers use the same scheme with near and far added,
and those are read too. A C name with its convention's decoration (`_name@12`, `@name@8`,
`_name`) is taken apart by plain().

The text produced is the one Microsoft's own tools print, so it can be compared with them.
Anything that cannot be read gives None -- a name is never guessed at.
"""
import re

_BASIC = {"C": "signed char", "D": "char", "E": "unsigned char", "F": "short", "G": "unsigned short",
          "H": "int", "I": "unsigned int", "J": "long", "K": "unsigned long", "M": "float",
          "N": "double", "O": "long double", "X": "void", "Z": "..."}
_EXTENDED = {"D": "__int8", "E": "unsigned __int8", "F": "__int16", "G": "unsigned __int16",
             "H": "__int32", "I": "unsigned __int32", "J": "__int64", "K": "unsigned __int64",
             "N": "bool", "W": "wchar_t"}
_CONVENTION = {"A": "__cdecl", "B": "__cdecl", "C": "__pascal", "D": "__pascal", "E": "__thiscall",
               "F": "__thiscall", "G": "__stdcall", "H": "__stdcall", "I": "__fastcall", "J": "__fastcall"}
_OPERATOR = {
    "2": "operator new", "3": "operator delete", "4": "operator=", "5": "operator>>", "6": "operator<<",
    "7": "operator!", "8": "operator==", "9": "operator!=", "A": "operator[]", "C": "operator->",
    "D": "operator*", "E": "operator++", "F": "operator--", "G": "operator-", "H": "operator+",
    "I": "operator&", "J": "operator->*", "K": "operator/", "L": "operator%", "M": "operator<",
    "N": "operator<=", "O": "operator>", "P": "operator>=", "Q": "operator,", "R": "operator()",
    "S": "operator~", "T": "operator^", "U": "operator|", "V": "operator&&", "W": "operator||",
    "X": "operator*=", "Y": "operator+=", "Z": "operator-=", "_0": "operator/=", "_1": "operator%=",
    "_2": "operator>>=", "_3": "operator<<=", "_4": "operator&=", "_5": "operator|=", "_6": "operator^=",
    "_7": "`vftable'", "_8": "`vbtable'", "_9": "`vcall'", "_A": "`typeof'", "_B": "`local static guard'",
    "_C": "`string'", "_D": "`vbase destructor'", "_E": "`vector deleting destructor'",
    "_F": "`default constructor closure'", "_G": "`scalar deleting destructor'",
    "_H": "`vector constructor iterator'", "_I": "`vector destructor iterator'",
    "_J": "`vector vbase constructor iterator'", "_K": "`virtual displacement map'",
    "_L": "`eh vector constructor iterator'", "_M": "`eh vector destructor iterator'",
    "_N": "`eh vector vbase constructor iterator'", "_O": "`copy constructor closure'",
    "_S": "`local vftable'", "_T": "`local vftable constructor closure'", "_U": "operator new[]",
    "_V": "operator delete[]"}
_QUALIFIER = ("", "const", "volatile", "const volatile")
_ACCESS = ("private", "protected", "public")


class _Unreadable(Exception):
    pass


class _Reader(object):
    """One decorated name being read left to right."""

    def __init__(self, text):
        self.s, self.i = text, 0
        self.names, self.types = [], []          # what a digit refers back to

    def peek(self, n=1):
        return self.s[self.i:self.i + n]

    def take(self):
        if self.i >= len(self.s):
            raise _Unreadable()
        self.i += 1
        return self.s[self.i - 1]

    def number(self):
        negative = self.peek() == "?"
        if negative:
            self.i += 1
        c = self.take()
        if c.isdigit():
            value = int(c) + 1
        else:
            value, self.i = 0, self.i - 1
            while self.peek() != "@":
                c = self.take()
                if not "A" <= c <= "P":
                    raise _Unreadable()
                value = value * 16 + ord(c) - 65
            self.i += 1
        return -value if negative else value

    # ---- names ---------------------------------------------------------------------------
    def identifier(self):
        end = self.s.find("@", self.i)
        if end < 0 or end == self.i:
            raise _Unreadable()
        text, self.i = self.s[self.i:end], end + 1
        return text

    def template(self):
        """`?$name@arguments@` -- its own name, with its arguments."""
        outer = self.names, self.types
        self.names, self.types = [], []
        if self.peek() == "?":                               # an operator template
            self.i += 1
            base = self.operator()
        else:
            base = self.identifier()
            self.names.append(base)
        args = []
        while self.peek() != "@":
            if self.peek() == "$":
                self.i += 1
                kind = self.take()
                if kind != "0":
                    raise _Unreadable()
                args.append(str(self.number()))
            else:
                args.append(self.argument())
        self.i += 1
        self.names, self.types = outer
        text = "%s<%s>" % (base, ",".join(args))
        return text + " " if text.endswith(">>") else text

    def operator(self):
        code = self.take()
        if code == "_":
            code += self.take()
        if code in ("0", "1", "B"):
            return {"0": "{constructor}", "1": "{destructor}", "B": "{conversion}"}[code]
        if code not in _OPERATOR:
            raise _Unreadable()
        return _OPERATOR[code]

    def piece(self):
        """One part of a qualified name."""
        c = self.peek()
        if c.isdigit():
            self.i += 1
            if int(c) >= len(self.names):
                raise _Unreadable()
            return self.names[int(c)]
        if self.peek(2) == "?$":
            self.i += 2
            text = self.template()
        elif c == "?":                                       # a numbered local scope
            self.i += 1
            return "`%d'" % self.number()
        else:
            text = self.identifier()
        if len(self.names) < 10 and text not in self.names:
            self.names.append(text)
        return text

    def qualified(self):
        """`name@scope@...@@` -> [outermost scope, ..., name]"""
        if self.peek() == "?" and self.peek(2) != "?$":
            self.i += 1
            base = self.operator()
        else:
            base = self.piece()
        scopes = []
        while self.peek() != "@":
            if self.i >= len(self.s):
                raise _Unreadable()
            scopes.append(self.piece())
        self.i += 1
        if scopes:
            last = scopes[0].rstrip()
            last = last[:last.index("<")] if "<" in last and not last.startswith("operator") else last
            base = {"{constructor}": last, "{destructor}": "~" + last}.get(base, base)
        return scopes[::-1] + [base]

    # ---- types ---------------------------------------------------------------------------
    def pointee_qualifier(self):
        """The letter after a pointer or reference: what it points at, and how far away.
        -> (const/volatile words, "__far"/"__huge"/"", kind) with kind "data", "function" or "member"."""
        c = self.take()
        if c in "ABCD":
            return _QUALIFIER["ABCD".index(c)], "", "data"
        if c in "EFGH":
            return _QUALIFIER["EFGH".index(c)], "__far", "data"
        if c in "IJKL":
            return _QUALIFIER["IJKL".index(c)], "__huge", "data"
        if c in "67":
            return "", "__far" if c == "7" else "", "function"
        if c in "89":
            return "", "__far" if c == "9" else "", "member function"
        if c in "QRST":
            return _QUALIFIER["QRST".index(c)], "", "member"
        raise _Unreadable()

    def function_type(self, with_this=False):
        """Convention, result, parameters. -> (convention, result as (left, right), [parameters], const words)"""
        this = ""
        # (the first 32-bit compilers wrote a pointer to a member function without the qualifier)
        if with_this and not (self.peek() == "E" and self.peek(2)[1:] not in ("A", "C", "G", "I")):
            this, _far, _kind = self.pointee_qualifier()
        convention = _CONVENTION.get(self.take())
        if convention is None:
            raise _Unreadable()
        if self.peek() == "@":
            self.i += 1
            result = ("", "")
        else:
            result = self.result()
        params = self.parameters()
        return convention, result, params, this

    def result(self):
        if self.peek() == "?":                               # a class returned by value, qualified
            self.i += 1
            q = _QUALIFIER["ABCDEFGH".index(self.take()) % 4]     # E-H: returned through a far pointer
            left, right = self.type()
            return (left + (" " + q if q else ""), right)
        return self.type()

    def parameters(self):
        if self.peek() == "X":
            self.i += 1
            out = ["void"]
        else:
            out = []
            while self.peek() not in ("@", "Z"):
                if self.i >= len(self.s):
                    raise _Unreadable()
                out.append(self.argument())
            if self.take() == "Z":
                out.append("...")
        if self.peek() == "Z":                               # no list of exceptions
            self.i += 1
        return out

    def argument(self):
        """One parameter or template argument as text; long ones can be referred back to."""
        c = self.peek()
        if c.isdigit():
            self.i += 1
            if int(c) >= len(self.types):
                raise _Unreadable()
            return self.types[int(c)]
        start = self.i
        left, right = self.result() if c == "?" else self.type()
        text = _join(left, "", right)
        if self.i - start > 1 and len(self.types) < 10:
            self.types.append(text)
        return text

    def type(self):
        """-> (left, right): the text before and after whatever is being declared."""
        c = self.take()
        if c == "_":
            c = self.take()
            if c not in _EXTENDED:
                raise _Unreadable()
            return (_EXTENDED[c], "")
        if c in _BASIC and c != "Z":
            return (_BASIC[c], "")
        if c in "TUV":
            return ({"T": "union ", "U": "struct ", "V": "class "}[c] + "::".join(self.qualified()), "")
        if c == "W":
            self.take()                                      # the size of the enumeration
            return ("enum " + "::".join(self.qualified()), "")
        if c in "ABPQRS":
            mark = "&" if c in "AB" else "*"
            own = {"B": "volatile", "Q": "const", "R": "volatile", "S": "const volatile"}.get(c, "")
            words, far, kind = self.pointee_qualifier()
            if kind == "member":                             # pointer to a data member
                cls = "::".join(self.qualified())
                left, right = self.type()
                return (_join(left, words, "") + " " + cls + "::" + mark + (" " + own if own else ""), right)
            if kind.endswith("function"):
                cls = "::".join(self.qualified()) + "::" if kind.startswith("member") else ""
                convention, result, params, this = self.function_type(with_this=bool(cls))
                inner = "(%s%s%s%s" % (" ".join(x for x in (far, convention) if x), " " if cls else "", cls, mark)
                if not cls:
                    inner = "(%s%s" % (" ".join(x for x in (far, convention) if x), mark)
                return (_join(result[0], "", "") + " " + inner + (" " + own if own else ""),
                        ")(%s)%s%s" % (",".join(params), this, result[1]))
            if self.peek() == "Y":                           # pointer to an array
                self.i += 1
                dims = [self.number() for _ in range(self.number())]
                left, right = self.type()
                return (_join(left, words, "") + " (" + (far + " " if far else "") + mark + (" " + own if own else ""),
                        ")" + "".join("[%d]" % d for d in dims) + right)
            left, right = self.type()
            if right:                                        # pointer to a pointer to a function
                return (left + mark, right)
            if words and left.endswith(words):               # `* const` said twice
                words = ""
            return (" ".join(x for x in (left, words, far, mark, own) if x), "")
        raise _Unreadable()


def _join(left, middle, right):
    text = " ".join(x for x in (left.strip(), middle.strip()) if x) + right
    return re.sub(r"\s+", " ", text).strip()


def _read(name):
    if not name or not name.startswith("?"):
        raise _Unreadable()
    r = _Reader(name[1:])
    if r.peek(4) == "?_C@":
        return {"name": "`string'", "kind": "data", "text": "`string'"}
    names = r.qualified()
    qname = "::".join(names)
    out = {"name": qname, "base": names[-1], "class": "::".join(names[:-1]), "kind": "function",
           "access": "", "static": False, "virtual": False, "far": False, "convention": "",
           "returns": "", "parameters": [], "const": ""}
    if r.i >= len(r.s):
        return dict(out, kind="name", text=qname)
    code = r.take()
    if code in "01234":                                      # a variable
        left, right = r.type()
        if not right and left.endswith("* const"):           # an array, which the scheme writes as a constant pointer
            left = left[:-6]
        words, far, _kind = r.pointee_qualifier() if r.i < len(r.s) else ("", "", "")
        access = _ACCESS[int(code)] if code in "012" else ""
        left = " ".join(x for x in (left, words, far) if x)
        text = (access + ": static " if access else "") + _join(left, qname, right)
        return dict(out, kind="data", access=access, static=bool(access), returns=_join(left, "", right), text=text)
    if code in "67":                                         # a table of virtual functions or bases
        words = _QUALIFIER["ABCDEFGH".index(r.take()) % 4] if r.peek() in tuple("ABCDEFGH") else ""
        owners = []
        while r.i < len(r.s) and r.peek() != "@":
            owners.append("::".join(r.qualified()))
        text = (words + " " if words else "") + qname + ("{for `%s'}" % "'s `".join(owners) if owners else "")
        return dict(out, kind="data", text=text)
    if code in "89":
        return dict(out, kind="name", text=qname)
    if not "A" <= code <= "Z":
        raise _Unreadable()
    n = ord(code) - 65
    member = n < 24
    far = bool(n & 1)
    shape = (n % 8) // 2 if member else 0                    # 0 plain, 1 static, 2 virtual, 3 adjusting thunk
    if shape == 3:
        adjust = r.number()
    convention, result, params, this = r.function_type(with_this=member and shape != 1)
    if names[-1] == "{conversion}":
        names[-1] = "operator " + _join(result[0], "", result[1])
        result, qname = ("", ""), "::".join(names)
    out.update(name=qname, base=names[-1], access=_ACCESS[n // 8] if member else "", static=shape == 1,
               virtual=shape in (2, 3), far=far, convention=convention,
               returns=_join(result[0], "", result[1]), parameters=[] if params == ["void"] else params, const=this)
    lead = ""
    if member:
        lead = out["access"] + ": " + ("static " if shape == 1 else "virtual " if shape in (2, 3) else "")
    how = ("__far " if far else "") + convention
    if "(" in result[0]:                                     # the result is itself a pointer to a function
        middle = "%s %s %s(%s)" % (result[0], how, qname, ",".join(params))
        text = lead + re.sub(r"\s+", " ", middle) + result[1]
    else:
        text = lead + " ".join(x for x in (result[0], how, qname) if x) + "(%s)" % ",".join(params)
    if shape == 3:
        text = "[thunk]:" + text.replace(qname + "(", qname + "`adjustor{%d}' (" % adjust, 1)
    out["text"] = text + this
    return out


def type_name(decorated):
    """A type's name as run-time type information keeps it -- `.?AVCFoo@@` (a class), `.?AU...` (a
    struct) -- as C++ writes it: "CFoo", "Outer::Inner", "std::vector<int>". None if unreadable."""
    if not decorated or not decorated.startswith(".?A") or len(decorated) < 5 or decorated[3] not in "VUTW":
        return None
    try:
        r = _Reader(decorated[4:])
        names = r.qualified()
        return "::".join(names).strip()
    except (_Unreadable, IndexError, ValueError, RecursionError):
        return None


def parse(name):
    """What a decorated C++ name says, as a dict (see the module text), or None."""
    try:
        return _read(name)
    except (_Unreadable, IndexError, ValueError, RecursionError):
        return None


def declaration(name):
    """The declaration a decorated C++ name was made from, or None when it is not one."""
    p = parse(name)
    return p["text"] if p else None


def short(name):
    """Just the qualified name (`CFoo::AddRef`), or None."""
    p = parse(name)
    return p["name"] if p else None


_STDCALL = re.compile(r"_?([A-Za-z_]\w*)@(\d+)\Z")
_FASTCALL = re.compile(r"@([A-Za-z_]\w*)@(\d+)\Z")


def plain(name):
    """A C linker name without its convention's decoration:
    (name, convention or "", bytes of arguments or None)."""
    m = _STDCALL.match(name or "")
    if m:
        return m.group(1), "__stdcall", int(m.group(2))
    m = _FASTCALL.match(name or "")
    if m:
        return m.group(1), "__fastcall", int(m.group(2))
    if name and name.startswith("_") and re.fullmatch(r"_[A-Za-z_]\w*", name):
        return name[1:], "__cdecl", None
    return name, "", None


def readable(name):
    """The best readable form of any linker name: a C++ declaration, a C name without its
    decoration, or the name as it stands."""
    if name and name.startswith("?"):
        return declaration(name) or name
    return plain(name)[0]


def stack_bytes(name):
    """Bytes of arguments the function removes on return, as far as the name alone settles it
    (None when it does not: a caller-cleans function, a class passed by value)."""
    p = parse(name)
    if not p or p["kind"] != "function":
        return plain(name)[2]
    if p["convention"] not in ("__stdcall", "__thiscall") or p["far"]:
        return None                                          # 16-bit: a parameter's size depends on the memory model
    by_value = p["returns"].startswith(("class ", "struct ", "union ")) and not p["returns"].endswith(("*", "&"))
    if by_value and not p["returns"].startswith("class "):
        return None                                          # a small plain structure comes back in registers
    total = 4 if by_value else 0                             # a class comes back through a hidden pointer
    if p["convention"] == "__stdcall" and p["access"] and not p["static"]:
        total += 4                                           # `this` is on the stack
    for t in p["parameters"]:
        if t == "..." or (t.startswith(("class ", "struct ", "union ")) and not t.endswith(("*", "&"))):
            return None
        total += 8 if t in ("double", "__int64", "unsigned __int64") else 4
    return total


def main(argv):
    """python -m codebreaker demangle <name> [<name> ...]"""
    if not argv:
        print(main.__doc__)
        return 2
    for name in argv:
        print(readable(name) if parse(name) or not name.startswith("?") else "%s  (not readable)" % name)
    return 0
