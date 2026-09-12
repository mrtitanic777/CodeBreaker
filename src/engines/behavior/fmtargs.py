#!/usr/bin/env python3
r"""fmtargs.py -- type a printf-family call's varargs from its format string.

A `wsprintfA(buf, "%s = %ld", name, count)` call is emitted with anonymous `unsigned` varargs
because the callee is variadic -- the SDK proto stops at `...`. But the format string, recovered as
a literal by datarec, says exactly what those arguments are: `%s` is an `LPCSTR`, `%ld` a `LONG`,
`%p` a `void*`. This parses the format string's conversion specifiers and returns a C type per
vararg, so the emitters can annotate `/*LPCSTR*/name, /*LONG*/count` -- real types recovered from
the format contract the compiler could not have checked but the source author wrote.
"""
import re

# printf-family callee -> the argument index that is the format string (0-based).
_FMT_FUNCS = {
    "printf": 0, "wprintf": 0, "vprintf": 0,
    "fprintf": 1, "fwprintf": 1, "sprintf": 1, "swprintf": 1, "vsprintf": 1,
    "_snprintf": 2, "_snwprintf": 2,
    "wsprintfA": 1, "wsprintfW": 1, "wvsprintfA": 1, "wvsprintfW": 1,
    "scanf": 0, "sscanf": 1, "fscanf": 1,
    "StringCchPrintfA": 3, "StringCchPrintfW": 3, "StringCbPrintfA": 3, "StringCbPrintfW": 3,
    "syslog": 1,
}

# one conversion specifier: %[flags][width][.precision][length]<conv>
_SPEC = re.compile(r"%([-+ 0#]*)(\*|\d+)?(?:\.(\*|\d+))?(hh|h|ll|l|L|w|I64|I32|z|t|j)?([diouxXeEfFgGaAcCsSpn%])")

# (length modifier, conversion) -> C type
_ANSI = {"d": "int", "i": "int", "o": "unsigned", "u": "unsigned", "x": "unsigned", "X": "unsigned"}
_WIDE = {"e": "double", "E": "double", "f": "double", "F": "double", "g": "double", "G": "double",
         "a": "double", "A": "double"}


def _type_of(length, conv, wide_default=False):
    if conv == "%":
        return None
    if conv in ("s", "S"):
        wide = (conv == "S") ^ wide_default
        if length in ("l", "w", "I64") or wide:
            return "LPCWSTR"
        return "LPCSTR"
    if conv in ("c", "C"):
        wide = (conv == "C") ^ wide_default
        return "wchar_t" if wide else "char"
    if conv == "p":
        return "void*"
    if conv == "n":
        return "int*"
    if conv in _WIDE:
        return "double"                                     # floats are always promoted to double
    if conv in _ANSI:
        base = _ANSI[conv]
        signed = conv in ("d", "i")
        if length in ("ll", "I64"):
            return "__int64" if signed else "unsigned __int64"
        if length == "l":
            return "long" if signed else "unsigned long"
        if length == "h":
            return "short" if signed else "unsigned short"     # promoted, but this is the intent
        return base
    return "int"


def parse_format(fmt, wide=False):
    """Ordered list of C types, one per conversion specifier in `fmt` (%% and %*width consumed)."""
    types = []
    for flags, width, prec, length, conv in _SPEC.findall(fmt):
        if conv == "%":
            continue
        if width == "*":                                    # %*d -- the width is an int argument
            types.append("int")
        if prec == "*":
            types.append("int")
        t = _type_of(length, conv, wide_default=wide)
        if t:
            types.append(t)
    return types


def fmt_arg_index(name):
    """The format-string argument index for a printf-family callee, or None if not one."""
    return _FMT_FUNCS.get(name)


def type_varargs(name, fmt):
    """(fmt_index, [types]) for a printf-family call given its recovered format string, or None."""
    idx = _FMT_FUNCS.get(name)
    if idx is None:
        return None
    if name.endswith("W"):                                  # explicit Win32 wide variant
        wide = True
    elif name.endswith("A"):                                # explicit Win32 ANSI variant
        wide = False
    else:                                                   # CRT: the wide variants are w{printf,scanf}
        wide = bool(re.search(r"w(printf|scanf)", name))
    return idx, parse_format(fmt, wide=wide)
