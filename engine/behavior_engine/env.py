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


def _db():
    global _DB, _TRIED
    if not _TRIED:
        _TRIED = True
        try:
            from engine.foundation import config
            inc = config.load_config().get("behavior_sdk", "")
            if inc and os.path.isdir(inc):
                from engine.sdk import apidb
                _DB = apidb.ApiDb().add_dir(inc)
        except Exception:
            _DB = None
    return _DB


def argc(dll, name):
    """Argument count for an imported API, or None if unknown."""
    db = _db()
    if db is not None:
        try:
            p = db.params(name)
            if p is not None:
                # a lone 'void' parameter list means zero args
                if len(p) == 1 and str(p[0]).lower().find("void") >= 0:
                    return 0
                return len(p)
        except Exception:
            pass
    m = _DECOR.search(name or "")                # stdcall decoration encodes the byte count
    if m:
        return int(m.group(1)) // 4
    return None


def resolver():
    """A callable (dll, name) -> argc|None for tracer.trace_function(sig_resolver=...)."""
    return argc
