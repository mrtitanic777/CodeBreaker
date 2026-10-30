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
jumps, calls, import thunks and forwarded exports are followed. What is found is kept in the
knowledge base (knowledge/: win32 "argument_bytes.code", win16 "argument_bytes"), once for every
later ask; the headers' table is its authored kind win32 "argument_bytes.header".
"""
import json
import os
import re
from codebreaker.base import corpus
from codebreaker.formats.pe import open_pe
from codebreaker import platforms


_MAX_INSNS = 4000              # per function
_MAX_HOPS = 6                  # call / forwarder depth
_MODS = {}                      # DLL stem (upper) -> _Mod | None
_CACHE = {}                     # DLL stem (upper) -> {name: bytes|None}
_EXTS = (".dll", ".exe", ".drv", ".cpl", ".ocx")
_IAT = re.compile(r"dword ptr \[(0x[0-9a-f]+)\]")


class _Mod:
    def __init__(self, path):
        self.pe = open_pe(path)
        self.base = self.pe.image_base
        self.exports = {}
        self.ords = {}
        for o, n, rva in self.pe.exports():
            self.ords[o] = rva
            if n:
                self.exports[n] = rva
        self.imports = self.pe.imports()
        self.md = platforms.decoder(32)
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


def _apiarity(stem):
    """The table an older CodeBreaker kept for `stem` beside the corpus (apiarity/<stem>.json),
    or {}: carried into the knowledge base once, then not read again."""
    try:
        with open(os.path.join(corpus.design_dir(), "apiarity", stem.lower() + ".json"), encoding="utf-8") as f:
            return json.load(f)
    except Exception:
        return {}


def _load(stem):
    """{name: bytes or None} found for the exports of `stem` (the knowledge base's, this process's view)."""
    if stem not in _CACHE:
        from codebreaker.knowledge import store
        kb = store.open()
        if kb.carried("apiarity " + stem) is None:
            old = _apiarity(stem)
            if old:
                kb.put_many("win32", "argument_bytes.code", {"%s.%s" % (stem, n): v for n, v in old.items()},
                            "%s's code (carried over from apiarity/%s.json)" % (stem.lower(), stem.lower()))
            kb.carried("apiarity " + stem, str(len(old)))
        prefix = stem + "."
        _CACHE[stem] = {k[len(prefix):]: v for k, v in kb.table("win32", "argument_bytes.code").items()
                        if k.startswith(prefix)}
    return _CACHE[stem]


def _save(stem, name):
    from codebreaker.knowledge import store
    store.open().put("win32", "argument_bytes.code", "%s.%s" % (stem, name), _CACHE[stem][name],
                     "%s's code" % stem.lower())


_TABLE = None


def header_bytes(name):
    """Argument bytes for `name` as the Win32 headers declare it, or None when the headers do
    not settle it (no prototype; variadic; caller-cleans; a wide by-value parameter). Read from
    win32_arguments.json beside this file (tools/make_api_table.py regenerates it), as the
    knowledge base has it."""
    global _TABLE
    from codebreaker.knowledge import reads
    if _TABLE is None:
        from codebreaker import knowledge
        _TABLE = knowledge._table("win32", "argument_bytes.header")
    reads.record("win32", "argument_bytes.header", name)       # (knowledge/reads.py)
    return _TABLE.get(name)


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
    from codebreaker.knowledge import reads
    stem = _stem(dll)
    c = _load(stem)
    if name not in c:
        c[name] = _resolve(stem, name, _MAX_HOPS)
        _save(stem, name)
    reads.record("win32", "argument_bytes.code", "%s.%s" % (stem, name))
    return c[name]


def argc(dll, name):
    """Argument count for the oracle's import stub (stack bytes / 4), or None if unknown."""
    b = stack_bytes(dll, name)
    return None if b is None or b % 4 else b // 4


_FILES = {"KERNEL": ("krnl386.exe", "kernel.exe"), "USER": ("user.exe",), "GDI": ("gdi.exe",),
          # (a driver exports under its interface's name, not its file's: Setup's display
          # driver is VGA.DRV, module DISPLAY -- found by its file, USER's ShowCursor calling
          # DISPLAY.102 (SetCursor) was given no argument size, and its C "faulted" in the gate)
          "DISPLAY": ("vga.drv", "display.drv"), "MOUSE": ("msmous31.drv", "lmouse31.drv", "mouse.drv")}
_cache = None
_scoped = ""
_modules = {}


_product = None


def _scope():
    """One table per product: USER.171 is not the same function in every Windows -- and per way
    of reading it: named by the code that reads an export (arg_bytes, describe.returns and
    .forwards), so a table made by an older reading is not reused. (The one table kept before
    went on giving the sizes it was first given: USER.471, lstrcmpi, at 12 bytes long after the
    reading said 10.) Kept in the knowledge base as win16 "argument_bytes", keyed
    "<product>:<reading>|MODULE.ordinal"."""
    from codebreaker.base import corpus
    return "%s:%s" % (corpus.current(), _reading())


def _reading():
    # (and by the folder of binaries read: a product's folder can be pointed elsewhere --
    # CB_CORPUS_ROOT at Setup's MINI.CAB, whose USER and KRNL386 are other files than Windows'
    # own -- and the sizes read from one must not be handed to the other under the same name)
    import hashlib
    import inspect
    from codebreaker.base import corpus
    from codebreaker.decompile import describe
    src = "".join(inspect.getsource(f) for f in (arg_bytes, describe.returns, describe.forwards))
    src += os.path.normcase(os.path.abspath(corpus.root() or ""))
    return hashlib.sha1(src.encode("utf-8")).hexdigest()[:10]


def _load16():
    global _cache, _product, _scoped
    from codebreaker.base import corpus
    if _product != corpus.current():                         # another product was selected
        _product = corpus.current()
        _cache = None
        _modules.clear()
    if _cache is None:
        from codebreaker.knowledge import store
        _scoped = _scope() + "|"
        _cache = {k[len(_scoped):]: v for k, v in store.open().table("win16", "argument_bytes").items()
                  if k.startswith(_scoped)}
    return _cache


def sizes16():
    """The table of sizes found so far for this product and reading, as bytes (sorted JSON): what a
    cache of the checker's verdicts is keyed by, since the checker answers calls by it."""
    return json.dumps(_load16(), sort_keys=True).encode("utf-8")


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
    cache = _load16()
    if key not in cache:
        value = None
        mod = _module(module)
        if mod:
            from codebreaker.decompile import describe
            nef, entries, segs = mod
            e = entries.get(int(ordinal))
            s = segs.get(e["seg"]) if e else None
            if s and s["kind"] == "CODE" and s["file_off"]:
                code = nef.data[s["file_off"]:s["file_off"] + s["length"]]
                # (a thunk that puts a word of its own under the return address and goes on:
                # what its callers push is what the body removes less what it added -- Win95
                # USER.471, lstrcmpi, read as its body's `retf 0Ah` gave the C two arguments too many)
                own = type("Exporter", (), {})()
                own.segment_bytes = {k: bytes(nef.data[g["file_off"]:g["file_off"] + g["length"]])
                                     for k, g in segs.items() if g["kind"] == "CODE" and g["file_off"]}
                fwd = describe.forwards(own, e["seg"], e["offset"], "far")
                kind, pop = describe.returns(code, e["offset"])
                if fwd is not None:
                    value = fwd
                elif kind == "far" and pop >= 0:
                    value = pop
        cache[key] = value
        from codebreaker.knowledge import store
        store.open().put("win16", "argument_bytes", _scoped + key, value,
                         "%s's code" % (os.path.basename(mod[0].path).lower() if mod else module.upper()))
    from codebreaker.knowledge import reads
    reads.record("win16", "argument_bytes", _scoped + key)
    return cache[key]


def of(api):
    """arg_bytes for a name as the emulator reports it: 'USER.@171' or 'USER_171'."""
    import re
    m = re.fullmatch(r"([A-Za-z0-9_]+?)[._]@?(\d+)", api or "")
    return arg_bytes(m.group(1), int(m.group(2))) if m else None


