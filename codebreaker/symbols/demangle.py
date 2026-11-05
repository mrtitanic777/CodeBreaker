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

The reading itself is C++ (native/symbols/demangle.cc, cbsymbols.dll); this is its thin shim.
"""
import re

from codebreaker.symbols import native


def type_name(decorated):
    """A type's name as run-time type information keeps it -- `.?AVCFoo@@` (a class), `.?AU...` (a
    struct) -- as C++ writes it: "CFoo", "Outer::Inner", "std::vector<int>". None if unreadable."""
    return native.ask("demangle.type_name", decorated) if decorated else None


def parse(name):
    """What a decorated C++ name says, as a dict (see the module text), or None."""
    return native.ask("demangle.parse", name) if name else None


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
