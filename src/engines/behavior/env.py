#!/usr/bin/env python3
r"""env.py -- the controlled environment's knowledge layer: API signatures for stub cleanup.

The tracer can stub external (API) calls, but to return correctly for a STDCALL callee it must
pop the right number of argument dwords -- which it can only know from the API's signature. This
module supplies that, reusing CodeBreaker's existing SDK engine (engine/sdk/apidb): point
config.json's `behavior_sdk` at an include directory and its prototypes drive the cleanup.

`argc(dll, name)` resolves an argument count by, in order:
  1. the SDK ApiDb (if `behavior_sdk` is configured and the prototype is found), then
  2. a stdcall name-decoration heuristic (`_Foo@16` / `Foo@16` -> 16 bytes -> 4 args), then
  3. None (caller falls back to its default cleanup).

With nothing configured, API calls are still NAMED (that comes from the PE import table, not
here) -- only the exact stdcall cleanup needs a signature source.
"""
import os
import re

_DECOR = re.compile(r"@(\d+)$")
_DB = None
_TRIED = False

# Built-in stdcall argument counts for common Win32 APIs. These are public, documented ABI
# facts (the number of dword arguments each API takes) -- not inferences about the binary -- so
# the environment can clean the stack correctly for the frequent cases with no SDK configured.
# Extend as real targets surface more. wsprintf-family (cdecl/variadic) are intentionally absent
# (cdecl cleanup is 0, the default). Keyed by bare name; A/W variants share a count.
_WIN32_ARGC = {
    # kernel32
    "GetLastError": 0, "SetLastError": 1, "GlobalAlloc": 2, "GlobalFree": 1, "GlobalLock": 1,
    "GlobalUnlock": 1, "GlobalReAlloc": 3, "GlobalSize": 1, "GlobalHandle": 1, "GlobalFlags": 1,
    "LocalAlloc": 2, "LocalFree": 1, "LocalLock": 1, "LocalUnlock": 1, "lstrlen": 1, "lstrcpy": 2,
    "lstrcpyn": 3, "lstrcat": 2, "lstrcmp": 2, "lstrcmpi": 2, "GetModuleHandle": 1,
    "GetProcAddress": 2, "LoadLibrary": 1, "LoadLibraryEx": 3, "FreeLibrary": 1, "CloseHandle": 1,
    "GetCurrentProcess": 0, "GetCurrentProcessId": 0, "GetCurrentThreadId": 0, "GetTickCount": 0,
    "Sleep": 1, "GetVersion": 0, "GetVersionEx": 1, "HeapAlloc": 3, "HeapFree": 3, "HeapReAlloc": 4,
    "GetProcessHeap": 0, "MultiByteToWideChar": 6, "WideCharToMultiByte": 8, "GetModuleFileName": 3,
    "OutputDebugString": 1, "InterlockedIncrement": 1, "InterlockedDecrement": 1,
    "InterlockedExchange": 2, "EnterCriticalSection": 1, "LeaveCriticalSection": 1,
    "InitializeCriticalSection": 1, "DeleteCriticalSection": 1, "GetProfileInt": 3,
    "GetPrivateProfileInt": 4, "WritePrivateProfileString": 4, "GetPrivateProfileString": 6,
    "TlsAlloc": 0, "TlsGetValue": 1, "TlsSetValue": 2, "TlsFree": 1, "GetCommandLine": 0,
    # advapi32
    "RegOpenKey": 3, "RegOpenKeyEx": 5, "RegQueryValue": 4, "RegQueryValueEx": 6, "RegCloseKey": 1,
    "RegCreateKey": 3, "RegCreateKeyEx": 9, "RegSetValue": 5, "RegSetValueEx": 6, "RegEnumKey": 4,
    "RegEnumKeyEx": 8, "RegEnumValue": 8, "RegDeleteKey": 2, "RegDeleteValue": 2, "RegFlushKey": 1,
    # user32
    "GetDC": 1, "ReleaseDC": 2, "GetDesktopWindow": 0, "GetSystemMetrics": 1, "LoadString": 4,
    "LoadCursor": 2, "LoadIcon": 2, "MessageBox": 4, "CharNext": 1, "CharPrev": 2, "CharUpper": 1,
    "CharLower": 1, "GetKeyboardLayout": 1, "GetSysColor": 1, "GetWindowDC": 1,
    # gdi32
    "GetStockObject": 1, "DeleteObject": 1, "GetObject": 3, "SelectObject": 2, "DeleteDC": 1,
    "CreateCompatibleDC": 1, "GetDeviceCaps": 2, "SetBkMode": 2, "SetTextColor": 2,
    "CreateCompatibleBitmap": 3,
}


def _builtin_argc(name):
    n = (name or "")
    if n in _WIN32_ARGC:
        return _WIN32_ARGC[n]
    if n and n[-1] in ("A", "W") and n[:-1] in _WIN32_ARGC:   # ANSI/Wide variant
        return _WIN32_ARGC[n[:-1]]
    return None


def _db():
    global _DB, _TRIED
    if not _TRIED:
        _TRIED = True
        try:
            from src.core import config
            inc = config.load_config().get("behavior_sdk", "")
            if inc and os.path.isdir(inc):
                from src.toolchain.sdk import apidb
                _DB = apidb.ApiDb().add_dir(inc)
        except Exception:
            _DB = None
    return _DB


def argc(dll, name):
    """Argument count for an imported API, or None if unknown. Sources, in order: the SDK apidb
    (config `behavior_sdk`), the built-in Win32 table, then the stdcall `@N` name decoration."""
    db = _db()
    if db is not None:
        try:
            p = db.params(name)
            if p is not None:
                if len(p) == 1 and str(p[0]).lower().find("void") >= 0:
                    return 0                    # a lone 'void' means zero args
                return len(p)
        except Exception:
            pass
    b = _builtin_argc(name)
    if b is not None:
        return b
    m = _DECOR.search(name or "")                # stdcall decoration encodes the byte count
    if m:
        return int(m.group(1)) // 4
    return None


def resolver():
    """A callable (dll, name) -> argc|None for tracer.trace_function(sig_resolver=...)."""
    return argc


def stubbing_enabled():
    """Whether to stub external calls across the analysis pipeline. On by default: the built-in
    Win32 table + SDK + `@N` decoration clean the stack correctly for known APIs, and stubbing is
    strictly better than running into zero-filled memory even when an arg count is unknown
    (cleanup then defaults to 0/cdecl, and the call is flagged). Set config `behavior_stub` to
    false to force it off."""
    try:
        from src.core import config
        v = config.load_config().get("behavior_stub", True)
        return v is not False and str(v).lower() not in ("false", "0", "off", "no")
    except Exception:
        return True


def apply_defaults():
    """Set the tracer's process-wide stub default from config, so every trace in the pipeline
    inherits it. Called once by the behavior command dispatcher."""
    from src.engines.behavior import tracer
    tracer.set_default_stub(stubbing_enabled(), resolver())
