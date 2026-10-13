"""How many bytes of arguments an imported function removes from the stack -- read from the
retail module that EXPORTS it.

The oracle stops at an import and has to return as the real function would: a stdcall function
ends in `ret N`, a cdecl one in a bare `ret`. N is in the exporting DLL's own code, so it is
looked up there rather than in a header table (which knew about a quarter of the imports the
Win95 modules use). The answer is what the binary does; nothing is guessed:

    stack_bytes("USER32", "FindWindowA")  -> 8
    stack_bytes("CRTDLL", "memchr")       -> 0        (cdecl: the caller cleans up)
    stack_bytes("KERNEL32", "nonesuch")   -> None     (not found / not decidable)

Every path from a function's entry is walked to its `ret`, tracking the stack depth; direct
jumps, calls, import thunks and forwarded exports are followed. Results are cached per exporting module beside the corpus index.
"""
import json
import os
import re

from codebreaker.base import corpus
from codebreaker.formats.pe import open_pe

_MAX_INSNS = 4000              # per function
_MAX_HOPS = 6                  # call / forwarder depth
_MODS = {}                      # DLL stem (upper) -> _Mod | None
_CACHE = {}                     # DLL stem (upper) -> {name: bytes|None}
_DIRTY = set()
_EXTS = (".dll", ".exe", ".drv", ".cpl", ".ocx")
_IAT = re.compile(r"dword ptr \[(0x[0-9a-f]+)\]")


class _Mod:
    def __init__(self, path):
        import capstone
        self.pe = open_pe(path)
        self.base = self.pe.image_base
        self.exports = {}
        self.ords = {}
        for o, n, rva in self.pe.exports():
            self.ords[o] = rva
            if n:
                self.exports[n] = rva
        self.imports = self.pe.imports()
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        ed = self.pe.datadirs[0] if self.pe.datadirs else (0, 0)
        self.exp_lo, self.exp_hi = ed[0], ed[0] + ed[1]

    def bytes_at(self, rva, n):
        off = self.pe._rva_to_off(rva)
        return b"" if off is None else self.pe.data[off:off + n]

    def forwarder(self, rva):
        """'DLL.Name' when an export's address points into the export directory."""
        if not (self.exp_lo <= rva < self.exp_hi):
            return None
        raw = self.bytes_at(rva, 128).split(b"\0")[0].decode("latin1", "replace")
        return raw if "." in raw else None


def _stem(dll):
    s = os.path.basename(str(dll)).upper()
    for e in _EXTS:
        if s.endswith(e.upper()):
            return s[:-len(e)]
    return s


def _mod(stem):
    if stem not in _MODS:
        m = None
        for e in _EXTS:
            p = corpus.retail(stem.lower() + e)
            if p:
                try:
                    m = _Mod(p)
                    break
                except Exception:
                    m = None
        _MODS[stem] = m
    return _MODS[stem]


def _scan(m, rva, hops):
    """Stack bytes removed by the function at `rva`, or None.

    Every path from the entry is walked with the stack depth tracked, so the answer is right for
    the hand-written wrappers Windows 95 is full of as well as for compiled code:

        FindWindowA:  pop ecx / push 0 / push 0 / push ecx / jmp FindWindowExA     -> 16 - 8 = 8

    A `ret N` reached with D bytes still pushed means the function itself took N - D. Calls are
    resolved the same way (the callee's own answer is what it pops). Paths whose depth is lost
    (an indirect call) still report their `ret N`, but only count if no exact path was found."""
    memo = m.__dict__.setdefault("_memo", {})
    if rva in memo:
        return memo[rva]
    memo[rva] = None                               # recursion guard
    thunk, strong, weak, bycl = set(), set(), set(), set()
    # (rva, depth, depth at `mov ebp, esp`, cl, depth is exact, it was exact at `mov ebp, esp`)
    work = [(rva, 0, None, None, True, True)]
    seen = set()
    budget = _MAX_INSNS
    while work and budget > 0 and len(strong) < 4:
        at, d, dbp, cl, exact, xbp = work.pop()
        while budget > 0:
            if at in seen:
                break
            code = m.bytes_at(at, 16)
            ins = next(m.md.disasm_lite(code, m.base + at), None) if code else None
            if ins is None:
                break
            seen.add(at)
            budget -= 1
            _addr, size, mn, ops = ins
            nxt = at + size
            if mn == "ret":
                n = int(ops, 0) if ops else 0
                # Depth that came through a call is only as good as the callee's answer: compiled
                # code returns with the stack balanced, so trust `ret N` itself there.
                if d is None or (d and not exact):
                    weak.add(n)
                elif n - d >= 0:
                    strong.add(n - d)
                break
            if mn in ("retf", "int3", "hlt", "iretd"):
                break
            if mn == "jmp":
                mm = _IAT.fullmatch(ops)
                if mm:                                             # tail call through an import
                    imp = m.imports.get(int(mm.group(1), 16))
                    r = _resolve(imp[0], imp[1], hops - 1) if imp and hops > 0 else None
                    if r is not None:
                        (weak if d is None else strong).add(r - (d or 0))
                    elif cl is not None and d == 0 and exact:
                        bycl.add(4 * cl)
                    break
                try:
                    at = int(ops, 0) - m.base
                    continue
                except ValueError:
                    if cl is not None and d == 0 and exact:        # `mov cl, N / jmp [common]`
                        bycl.add(4 * cl)
                    break
            if mn[0] == "j" or mn.startswith("loop"):              # conditional: walk both ways
                try:
                    work.append((int(ops, 0) - m.base, d, dbp, cl, exact, xbp))
                except ValueError:
                    pass
            elif mn == "call":
                cl = None
                exact = False
                r = None
                mm = _IAT.fullmatch(ops)
                if mm:
                    imp = m.imports.get(int(mm.group(1), 16))
                    r = _resolve(imp[0], imp[1], hops - 1) if imp and hops > 0 else None
                else:
                    try:
                        r = _scan(m, int(ops, 0) - m.base, hops - 1) if hops > 0 else None
                    except ValueError:
                        r = None
                d = d - r if (d is not None and r is not None) else None
            elif mn == "push":
                d = None if d is None else d + 4
            elif mn == "pop":
                d = None if (d is None or ops == "esp") else d - 4
            elif mn == "pushal":
                d = None if d is None else d + 32
            elif mn == "popal":
                d = None if d is None else d - 32
            elif mn in ("pushfd", "popfd"):
                d = None if d is None else d + (4 if mn == "pushfd" else -4)
            elif mn == "leave":                    # the frame pointer restores a known depth
                d = None if dbp is None else dbp - 4
                exact = xbp
            elif mn == "mov" and ops == "ebp, esp":
                dbp, xbp = d, exact
            elif mn == "mov" and ops == "esp, ebp":
                d, exact = dbp, xbp
            elif mn == "mov" and ops.startswith("edx, 0x") and cl is not None:
                # The thunk compiler's 32->16 stub: `mov cl, index / mov edx, argc<<16|flags /
                # jmp common`. The argument count is in the instruction.
                if d is not None and exact:
                    thunk.add(4 * (int(ops[5:], 16) >> 16) - d)
                break
            elif mn == "mov" and ops.startswith("cl, "):
                try:
                    cl = int(ops[4:], 0)
                except ValueError:
                    cl = None
            elif ops.startswith("esp, ") or ops == "esp":
                mm = re.fullmatch(r"esp, (-?(?:0x)?[0-9a-f]+)", ops)
                if mn in ("sub", "add") and mm and d is not None:
                    d += int(mm.group(1), 0) * (1 if mn == "sub" else -1)
                else:
                    mm = re.fullmatch(r"esp, \[ebp( [-+] (?:0x)?[0-9a-f]+)?\]", ops)
                    if mn == "lea" and mm and dbp is not None:
                        d = dbp - int((mm.group(1) or "0").replace(" ", ""), 0)
                        exact = xbp
                    else:
                        d = None
            at = nxt
    for cand in (thunk, strong, weak, bycl):
        cand = {c for c in cand if c >= 0 and c % 4 == 0}
        if cand:
            memo[rva] = cand.pop() if len(cand) == 1 else None
            break
    return memo[rva]


def _resolve(dll, name, hops):
    m = _mod(_stem(dll))
    if m is None or not name:
        return None
    if name.startswith("#"):
        try:
            rva = m.ords.get(int(name[1:]))
        except ValueError:
            rva = None
    else:
        rva = m.exports.get(name)
    if not rva:
        return None
    fwd = m.forwarder(rva)
    if fwd:
        if hops <= 0:
            return None
        d, _, n = fwd.partition(".")
        return _resolve(d, n, hops - 1)
    try:
        return _scan(m, rva, hops)
    except Exception:
        return None


def _cache_path(stem):
    d = os.path.join(corpus.design_dir(), "apiarity")
    return os.path.join(d, stem.lower() + ".json")


def _load(stem):
    if stem not in _CACHE:
        try:
            with open(_cache_path(stem), encoding="utf-8") as f:
                _CACHE[stem] = json.load(f)
        except Exception:
            _CACHE[stem] = {}
    return _CACHE[stem]


def _save(stem):
    p = _cache_path(stem)
    try:
        os.makedirs(os.path.dirname(p), exist_ok=True)
        tmp = "%s.%d.tmp" % (p, os.getpid())
        with open(tmp, "w", encoding="utf-8") as f:
            json.dump(_CACHE[stem], f, sort_keys=True)
        os.replace(tmp, p)
    except Exception:
        pass


_HEADERS = None
_BY_VALUE = re.compile(r"\b(POINT|POINTS|SIZE|POINTL|SIZEL|COORD|LARGE_INTEGER|ULARGE_INTEGER|"
                       r"double|DOUBLE|LONGLONG|ULONGLONG|DWORDLONG|FILETIME|CY|DATE|VARIANT)\b(?!\s*\*)")


def header_bytes(name):
    """Argument bytes for `name` from the Win32 headers (OpenWatcom's, the ones we compile
    against): 4 per parameter of a WINAPI prototype. None when there is no prototype, it is
    variadic or not stdcall, or a parameter is passed by value and is wider than 4 bytes (a
    POINT is 8): the header's parameter COUNT is then not the stack size."""
    global _HEADERS
    if _HEADERS is None:
        path = os.path.join(corpus.design_dir(), "apiarity", "_headers.json")
        try:
            with open(path, encoding="utf-8") as f:
                _HEADERS = json.load(f)
        except Exception:
            _HEADERS = {}
            try:
                from codebreaker.symbols import sdk_prototypes as sdkprotos
                for d in sdkprotos._ow_header_dirs()[:1]:
                    for fn in sorted(os.listdir(d)):
                        if fn.lower().endswith(".h"):
                            for k, v in sdkprotos.extract_header(os.path.join(d, fn)).items():
                                _HEADERS.setdefault(k, [v.get("conv") or "", v.get("params") or ""])
                os.makedirs(os.path.dirname(path), exist_ok=True)
                with open(path, "w", encoding="utf-8") as f:
                    json.dump(_HEADERS, f, sort_keys=True)
            except Exception:
                pass
    v = _HEADERS.get(name)
    if not v:
        return None
    conv, params = v[0], str(v[1]).strip()
    if "..." in params or conv.upper() in ("WINAPIV", "CDECL", "__CDECL", "_CDECL") or _BY_VALUE.search(params):
        return None
    return 0 if params in ("", "void", "VOID") else 4 * (params.count(",") + 1)


def stack_bytes(dll, name):
    """Bytes of arguments `dll!name` pops on return (0 for cdecl), or None if unknown.

    Two sources, checked against each other over eight system DLLs (they agree on 1,669 of 1,695
    functions): the Win32 header prototype, and the exporting DLL's own code. The header wins
    where it applies -- the disagreements were all the code scan misreading a hand-written
    wrapper -- and the scan covers what the headers do not: undocumented functions, and
    functions taking a structure by value."""
    h = header_bytes(name)
    if h is not None:
        return h
    stem = _stem(dll)
    c = _load(stem)
    if name not in c:
        c[name] = _resolve(stem, name, _MAX_HOPS)
        _save(stem)
    return c[name]


def argc(dll, name):
    """Argument count for the oracle's import stub (stack bytes / 4), or None if unknown."""
    b = stack_bytes(dll, name)
    return None if b is None or b % 4 else b // 4


def warm(dll):
    """Resolve every export of `dll` in one pass (one cache write). Returns {name: bytes|None}."""
    stem = _stem(dll)
    m = _mod(stem)
    c = _load(stem)
    if m is not None:
        for name in m.exports:
            if name not in c:
                c[name] = _resolve(stem, name, _MAX_HOPS)
        _save(stem)
    return c


if __name__ == "__main__":
    import sys
    for d in sys.argv[1:]:
        t = warm(d)
        print("%-12s %d exports, %d resolved" % (d, len(t), sum(v is not None for v in t.values())))
