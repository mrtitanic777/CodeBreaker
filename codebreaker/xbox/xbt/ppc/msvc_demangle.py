"""MSVC C++ name demangler — decode `?foo@bar@@YAHXZ` into readable signatures.

Xbox 360 titles are built with the MSVC PowerPC compiler, so their export
tables, RTTI, and any symbol strings are MSVC-decorated. This turns a decorated
name back into a C++ declaration, which makes every 360 binary far more
readable. Covers the common surface (functions, data, namespaces/classes,
templates, the primitive and pointer/reference/CV type grammar, calling
conventions, access levels, and the operator/ctor/dtor special names); anything
it cannot parse is returned unchanged.

Reimplemented clean-room from the documented MSVC decoration scheme.
"""

import sys

DESCRIPTION = "demangle MSVC C++ decorated names (?foo@bar@@... -> readable signature)"

_PRIM = {
    "X": "void", "D": "char", "C": "signed char", "E": "unsigned char",
    "F": "short", "G": "unsigned short", "H": "int", "I": "unsigned int",
    "J": "long", "K": "unsigned long", "M": "float", "N": "double",
    "O": "long double", "_N": "bool", "_J": "__int64", "_K": "unsigned __int64",
    "_W": "wchar_t", "_S": "char16_t", "_U": "char32_t",
}
_ACCESS = {
    "A": "private: ", "B": "private: ", "C": "private: static ", "D": "private: static ",
    "E": "private: virtual ", "F": "private: virtual ",
    "I": "protected: ", "J": "protected: ", "K": "protected: static ", "L": "protected: static ",
    "M": "protected: virtual ", "N": "protected: virtual ",
    "Q": "public: ", "R": "public: ", "S": "public: static ", "T": "public: static ",
    "U": "public: virtual ", "V": "public: virtual ",
}
_CALLCONV = {"A": "__cdecl", "B": "__cdecl", "C": "__pascal", "E": "__thiscall",
             "G": "__stdcall", "I": "__fastcall"}
_OPS = {
    "0": "{ctor}", "1": "{dtor}", "2": "operator new", "3": "operator delete",
    "4": "operator=", "5": "operator>>", "6": "operator<<", "7": "operator!",
    "8": "operator==", "9": "operator!=", "A": "operator[]", "C": "operator->",
    "D": "operator*", "E": "operator++", "F": "operator--", "G": "operator-",
    "H": "operator+", "I": "operator&", "K": "operator/", "L": "operator%",
    "M": "operator<", "N": "operator<=", "O": "operator>", "P": "operator>=",
    "Q": "operator,", "R": "operator()", "S": "operator~", "T": "operator^",
    "U": "operator|", "V": "operator&&", "W": "operator||",
    "Y": "operator+=", "Z": "operator-=",
}


class _P:
    def __init__(self, s):
        self.s = s
        self.i = 0
        self.names = []          # back-reference table for repeated name fragments

    def eof(self):
        return self.i >= len(self.s)

    def peek(self):
        return self.s[self.i] if self.i < len(self.s) else ""

    def take(self):
        c = self.s[self.i]
        self.i += 1
        return c

    def expect(self, c):
        if self.peek() != c:
            raise ValueError("expected %r at %d" % (c, self.i))
        self.i += 1

    # ---- names ----
    def _fragment(self):
        if self.peek().isdigit():           # back-reference to an earlier fragment
            idx = int(self.take())
            return self.names[idx] if idx < len(self.names) else "?"
        start = self.i
        while not self.eof() and self.peek() != "@":
            self.take()
        frag = self.s[start:self.i]
        self.expect("@")
        self.names.append(frag)
        return frag

    def _special_name(self):
        # after the leading '?'
        c = self.take()
        if c == "?":                        # ??... nested / operator template
            n = self.take()
            return _OPS.get(n, "?" + n)
        return _OPS.get(c, "?" + c)

    def qualified_name(self):
        """Parse a name up to the '@@' terminator; return (display, scope_list)."""
        parts = []
        if self.peek() == "?":
            self.take()
            parts.append(self._special_name())
        else:
            parts.append(self._fragment())
        # remaining scope fragments until '@@'
        scope = []
        while not self.eof() and self.peek() != "@":
            scope.append(self._fragment())
        if self.peek() == "@":
            self.take()                      # first @ (of @@)
        chain = list(reversed(scope)) + [parts[0]]
        return "::".join(chain), scope

    # ---- types ----
    def type(self):
        c = self.peek()
        if c in ("P", "Q", "A", "R"):        # pointer / reference
            self.take()
            cv = self.take()                 # cv class of the pointee (A/B/C/D...)
            inner = self.type()
            suffix = "&" if c in ("A", "R") else "*"
            constq = "const " if cv in ("B", "D") else ""
            return "%s%s %s" % (constq, inner, suffix)
        if c == "_":
            self.take()
            return _PRIM.get("_" + self.take(), "?")
        if c in _PRIM:
            self.take()
            return _PRIM[c]
        if c in ("U", "V", "T", "W"):        # struct/class/union/enum tag
            self.take()
            name, _ = self.qualified_name()
            return name
        if c == "X":
            self.take()
            return "void"
        # unknown: consume one char
        self.take()
        return "?"

    def arg_list(self):
        if self.peek() == "X":               # (void)
            self.take()
            return ["void"]
        args = []
        while not self.eof() and self.peek() not in ("@", "Z"):
            args.append(self.type())
        if self.peek() == "@":
            self.take()
        return args


def _fix_ctor_dtor(display):
    """Turn a trailing {ctor}/{dtor} placeholder into the class name / ~name."""
    parts = display.split("::")
    cls = parts[-2] if len(parts) >= 2 else parts[0]
    if display.endswith("{ctor}"):
        return "::".join(parts[:-1] + [cls])
    if display.endswith("{dtor}"):
        return "::".join(parts[:-1] + ["~" + cls])
    return display


def _function(p, access, display):
    conv = _CALLCONV.get(p.take(), "__cdecl")
    is_cd = display.endswith("{ctor}") or display.endswith("{dtor}")
    ret = ""
    if is_cd:
        if p.peek() == "@":
            p.take()                          # ctor/dtor have no return type
        display = _fix_ctor_dtor(display)
    else:
        ret = p.type()
    args = p.arg_list()
    argstr = ", ".join(args) if args else "void"
    if is_cd:
        return "%s%s %s(%s)" % (access, conv, display, argstr)
    return "%s%s %s %s(%s)" % (access, ret, conv, display, argstr)


def demangle(name):
    """Return a readable C++ declaration for a decorated `name`, or `name`
    itself if it is not a recognisable MSVC decorated name."""
    if not name or name[0] != "?":
        return name
    try:
        p = _P(name[1:])
        display, _scope = p.qualified_name()
        if p.eof():
            return display
        fc = p.peek()
        if fc in ("Y", "Z"):                  # free / global function
            p.take()
            return _function(p, "", display)
        if fc in _ACCESS:                     # member function
            access = _ACCESS[fc]
            p.take()
            p.take()                          # this-cv qualifier
            return _function(p, access, display)
        if fc in "0123456789":                # data symbol
            p.take()
            t = p.type() if not p.eof() else ""
            return ("%s %s" % (t, display)).strip()
        return display
    except (ValueError, IndexError):
        return name


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: msvc_demangle <?decorated@name@@...> [...]\n"
              "       msvc_demangle --selftest")
        return
    if a[0] == "--selftest":
        cases = {
            "?foo@bar@@YAHXZ": "int __cdecl bar::foo(void)",
            "?x@@3HA": "int x",
            "?g@@YAXPAH@Z": "void __cdecl g(int *)",
            "?Update@Player@Game@@QAEXH@Z": "public: void __thiscall Game::Player::Update(int)",
        }
        bad = 0
        for m, want in cases.items():
            got = demangle(m)
            ok = want in got or got == want
            if not ok:
                bad += 1
                print("  FAIL %s -> %s (want ~%s)" % (m, got, want))
        print("demangle self-test: %s" % ("OK" if not bad else "%d failed" % bad))
        sys.exit(1 if bad else 0)
    for m in a:
        print("%s\n  -> %s" % (m, demangle(m)))


if __name__ == "__main__":
    main()
