"""api16.py -- how many bytes of arguments a 16-bit Windows function takes.

    from codebreaker.emulate import api16
    api16.arg_bytes("USER", 171)        # -> 12, or None when it cannot be established

A 16-bit Windows function removes its own arguments, and says how many bytes in its own return
instruction (`retf 12`). So the answer for any imported function is in the module that exports
it: find the export by ordinal, read its first return. No header files and no tables to keep.

Used three ways: the decompiler is told the size, so a call to an import is recovered with the
right arguments; the emulator's stand-in for the function removes that many bytes, as the real
one would; and the verifier can then compare the arguments two builds pass.
"""
import json

_FILES = {"KERNEL": ("krnl386.exe", "kernel.exe"), "USER": ("user.exe",), "GDI": ("gdi.exe",)}
_cache = None
_modules = {}


def _cache_path():
    from codebreaker.base import settings
    return settings.state("api16.json")


def _load():
    global _cache
    if _cache is None:
        try:
            with open(_cache_path(), encoding="utf-8") as f:
                _cache = json.load(f)
        except (OSError, ValueError):
            _cache = {}
    return _cache


def _module(name):
    """(NEFile, {ordinal: entry}) of the module that exports under `name`, or None."""
    name = name.upper()
    if name not in _modules:
        from codebreaker.base import corpus
        from codebreaker.formats.ne import NEFile
        found = None
        for cand in _FILES.get(name, ()) + tuple(name.lower() + e for e in (".dll", ".exe", ".drv")):
            p = corpus.retail(cand)
            if p and corpus.lane(p) == "ne":
                try:
                    nef = NEFile(p)
                    found = (nef, {e["ordinal"]: e for e in nef.entries() if e.get("kind") in ("FIXED", "MOVEABLE")},
                             {s["idx"]: s for s in nef.segments()})
                    break
                except Exception:
                    pass
        _modules[name] = found
    return _modules[name]


def arg_bytes(module, ordinal):
    """Bytes of arguments the export removes on return; None when unknown."""
    key = "%s.%d" % (module.upper(), int(ordinal))
    cache = _load()
    if key not in cache:
        value = None
        mod = _module(module)
        if mod:
            from codebreaker.decompile import image16
            nef, entries, segs = mod
            e = entries.get(int(ordinal))
            s = segs.get(e["seg"]) if e else None
            if s and s["kind"] == "CODE" and s["file_off"]:
                code = nef.data[s["file_off"]:s["file_off"] + s["length"]]
                kind, pop = image16.returns(code, e["offset"])
                if kind == "far" and pop >= 0:
                    value = pop
        cache[key] = value
        try:
            with open(_cache_path(), "w", encoding="utf-8") as f:
                json.dump(cache, f, sort_keys=True)
        except OSError:
            pass
    return cache[key]


def of(api):
    """arg_bytes for a name as the emulator reports it: 'USER.@171' or 'USER_171'."""
    import re
    m = re.fullmatch(r"([A-Za-z0-9_]+?)[._]@?(\d+)", api or "")
    return arg_bytes(m.group(1), int(m.group(2))) if m else None
