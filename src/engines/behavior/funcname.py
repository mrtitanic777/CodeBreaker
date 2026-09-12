#!/usr/bin/env python3
r"""funcname.py -- heuristic function naming from the API + string fingerprint.

A floored function is emitted as sub_<off> -- anonymous. But its behavior is evidence of its
PURPOSE: the set of APIs it calls (and strings it references) fingerprints what it does. This
traces the function, collects its resolved API calls, and matches them against a rule table to
propose a name (`ReadRegistryValue`, `LoadFileContents`, `CreateComObject`). A hypothesis, tagged
as such -- never overrides a real symbol, and only offered when the fingerprint is distinctive.
"""
from src.engines.behavior import tracer, interproc as ip

# ordered rules: (required API substrings (all present), suggested name). First match wins.
_RULES = [
    (("RegQueryValue",), "ReadRegistryValue"),
    (("RegSetValue",), "WriteRegistryValue"),
    (("RegOpenKey", "RegCloseKey"), "ReadRegistryKey"),
    (("RegCreateKey",), "CreateRegistryKey"),
    (("CreateFile", "ReadFile"), "ReadFileContents"),
    (("CreateFile", "WriteFile"), "WriteFileContents"),
    (("CreateFileMapping", "MapViewOfFile"), "MapSharedMemory"),
    (("LoadLibrary", "GetProcAddress"), "ResolveProcAddress"),
    (("CoCreateInstance",), "CreateComObject"),
    (("CoTaskMemAlloc",), "AllocTaskMemory"),
    (("CreateWindowEx",), "CreateChildWindow"),
    (("DialogBox",), "ShowDialog"),
    (("MessageBox",), "ShowMessageBox"),
    (("wsprintf",), "FormatString"),
    (("lstrcpy", "lstrcat"), "BuildString"),
    (("lstrlen",), "MeasureString"),
    (("MultiByteToWideChar",), "ToWideChar"),
    (("WideCharToMultiByte",), "ToMultiByte"),
    (("LocalAlloc", "LocalFree"), "AllocAndFreeLocal"),
    (("EnterCriticalSection", "LeaveCriticalSection"), "GuardedSection"),
    (("GetProcAddress",), "GetProcAddress_wrapper"),
    (("SendMessage",), "SendWindowMessage"),
    (("DefWindowProc",), "WindowProc"),
]


def _apis(module, va, arity=4):
    try:
        tr = tracer.trace_function(module, func=hex(va), args=ip._SENT[:arity],
                                   stub_externals=True, insn_cap=8000)
    except Exception:
        return set()
    out = set()
    for c in tr.get("calls", []):
        if c.get("api"):
            out.add(c["api"].split("!")[-1])
    return out


def suggest_from_apis(apis):
    """Rule-match a set of called API names to a proposed function name, or None."""
    if not apis:
        return None
    for req, name in _RULES:
        if all(any(r in a for a in apis) for r in req):
            return name
    return None


def suggest_from_calls(calls):
    """Convenience for an emitter that already has a trace's call records."""
    return suggest_from_apis({c["api"].split("!")[-1] for c in calls if c.get("api")})


def suggest(module, va, arity=4):
    """A hypothesized name for the function from its API fingerprint, or None."""
    return suggest_from_apis(_apis(module, va, arity))
