r"""native_trace.py -- trace_function's hooks in C++ (native/cb/emulate/tracer: cbtrace.dll), through ctypes.

A run of one 32-bit function used to call back into Python on every instruction, every block
and every memory write: ping.exe's judge made 2,459 runs and 8.5 million callbacks, most of its
time. Here the hooks are C++: emulator.trace_function still lays the machine out, writes the
frame, starts each run and puts memory back; this module attaches the DLL's hooks to the same
Unicorn engine (uc._uch), in the order Python attached its own, and turns what they recorded
back into the same TraceRecord fields. During a run Python is asked one thing: an import's
argument count and what it gives back through a pointer, once per import entered.

    run = native_trace.Run(uc, ...)   # the hooks attached
    run.start(uc, begin, until)       # uc.emu_start, then what a Python hook would have raised
    run.settle(rec)                   # (a fast run stopped inside a straight run)
    run.collect(rec)                  # the record's fields
    run.detach(); run.restore()       # (a kept machine: hooks off, pages back)

The Python hooks this replaced are gone (docs/NATIVE.md, step 3): without the DLL a run raises
(require()). Build: toolsuild_cb.cmd (native/cb/CMakeLists.txt, target cbtrace; sources in native/cb/emulate/tracer).
"""
import ctypes
import os
import struct

from codebreaker.base import settings

_FAST, _LEAN, _READS, _QUIET, _STOP_WILD, _STUB = 0x1, 0x2, 0x4, 0x8, 0x10, 0x20
_UNIQUE, _BRANCHES, _PADS, _SAVED, _RESOLVER = 0x40, 0x80, 0x100, 0x200, 0x400
_CALL_WORDS, _MEM_WORDS, _LEAN_WORDS, _LAZY_WORDS, _BRANCH_WORDS = 21, 8, 4, 5, 11
_COMPARES = ("cmp", "test", "sub", "and", "or", "dec", "inc", "add", "xor")     # emulator._COMPARES
_ACCESS = ("read", "write", "fetch", "?")
_PAGE = 0x1000

RESOLVER = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_int32, ctypes.POINTER(ctypes.c_int64), ctypes.c_int32)
_P64 = ctypes.POINTER(ctypes.c_uint64)


class _Config(ctypes.Structure):
    _fields_ = [("uch", ctypes.c_void_p), ("flags", ctypes.c_uint32), ("sha_id", ctypes.c_int32),
                ("insn_cap", ctypes.c_int64), ("stub_ret", ctypes.c_int64), ("stub_cleanup", ctypes.c_int64),
                ("gov_n", ctypes.c_int64), ("gov_ceil", ctypes.c_int64),
                ("func_lo", ctypes.c_uint64), ("func_hi", ctypes.c_uint64),
                ("sec_lo", ctypes.c_uint64), ("sec_hi", ctypes.c_uint64),
                ("pads_lo", ctypes.c_uint64), ("pads_hi", ctypes.c_uint64),
                ("quiet_lo", ctypes.c_uint64), ("quiet_hi", ctypes.c_uint64),
                ("lean_low_end", ctypes.c_uint64), ("lean_high_begin", ctypes.c_uint64),
                ("statics", ctypes.POINTER(ctypes.c_int32)), ("nstatics", ctypes.c_int32),
                ("init_pages", _P64), ("ninit", ctypes.c_int64),
                ("mapped", _P64), ("nmapped", ctypes.c_int64),
                ("presaved", _P64), ("npresaved", ctypes.c_int64),
                ("sentinels", _P64), ("nsentinels", ctypes.c_int64),
                ("resolver", RESOLVER), ("shadow", ctypes.c_void_p)]


class _Result(ctypes.Structure):
    _fields_ = [(n, ctypes.c_int64) for n in (
                    "insn_count", "truncated", "governed", "dirty", "ran_into_zeros", "switched", "memops",
                    "writes_seen", "stuck", "bogus_stubs", "stubbed", "lean_seen", "api_calls_set", "gov_n",
                    "hook_error", "uc_err")] + [
                ("path", ctypes.c_void_p), ("npath", ctypes.c_int64),
                ("mn_addr", ctypes.c_void_p), ("nmn", ctypes.c_int64),
                ("mn_text", ctypes.c_void_p), ("mn_text_len", ctypes.c_int64),
                ("calls", ctypes.c_void_p), ("ncalls", ctypes.c_int64),
                ("reads", ctypes.c_void_p), ("nreads", ctypes.c_int64),
                ("writes", ctypes.c_void_p), ("nwrites", ctypes.c_int64),
                ("pages0", ctypes.c_void_p), ("pages0_data", ctypes.c_void_p), ("npages0", ctypes.c_int64),
                ("lazy", ctypes.c_void_p), ("nlazy", ctypes.c_int64),
                ("deps", ctypes.c_void_p), ("ndeps", ctypes.c_int64),
                ("api_calls", ctypes.c_void_p), ("napi_words", ctypes.c_int64),
                ("vxd", ctypes.c_void_p), ("nvxd", ctypes.c_int64),
                ("branches", ctypes.c_void_p), ("nbranches", ctypes.c_int64)]


_LIB = []                 # [the DLL, or None when it cannot be used here]
_WHY = []                 # why not, once
_STATICS = {}             # id(an emulator._INSN table) -> (its native id, the table: kept alive)
_SHAS = {}


def dll_path():
    """Where cbtrace.dll is: the `tracer_bin` setting (default state/cxx/cb/bin)."""
    return os.path.join(settings.get("tracer_bin"), "cbtrace.dll")


def _load():
    if _LIB:
        return _LIB[0]
    lib = None
    try:
        path = dll_path()
        if not os.path.isfile(path):
            raise OSError("not built: %s (native/cb/CMakeLists.txt, target cbtrace)" % path)
        from unicorn.unicorn_py3.unicorn import uclib                      # the DLLs Python loaded
        import capstone
        uc_dll, cs_dll = uclib._name, capstone._cs._name
        lib = settings.load_native(path)
        lib.cbt_version.restype = ctypes.c_int
        if lib.cbt_version() != 1:
            raise OSError("cbtrace.dll is another version: rebuild cbtrace (toolsuild_cb.cmd)")
        lib.cbt_init.argtypes = (ctypes.c_wchar_p, ctypes.c_wchar_p, ctypes.c_char_p, ctypes.c_int32)
        err = ctypes.create_string_buffer(256)
        if lib.cbt_init(os.path.abspath(uc_dll), os.path.abspath(cs_dll), err, 256):
            raise OSError(err.value.decode("ascii", "replace"))
        lib.cbt_register.argtypes = (ctypes.c_char_p, ctypes.c_int32)
        from unicorn import x86_const
        for name in dir(x86_const):
            if name.startswith("UC_X86_REG_"):
                lib.cbt_register(name[len("UC_X86_REG_"):].encode("ascii"), getattr(x86_const, name))
        lib.cbt_static.argtypes = (ctypes.c_uint64, ctypes.c_uint64, ctypes.c_char_p)
        lib.cbt_static.restype = ctypes.c_int32
        lib.cbt_intern.argtypes = (ctypes.c_char_p,)
        lib.cbt_intern.restype = ctypes.c_int32
        lib.cbt_begin.argtypes = (ctypes.POINTER(_Config),)
        lib.cbt_begin.restype = ctypes.c_void_p
        for name in ("cbt_attach", "cbt_attach_code", "cbt_switched", "cbt_detach", "cbt_restore"):
            getattr(lib, name).argtypes = (ctypes.c_void_p,)
            getattr(lib, name).restype = ctypes.c_int
        lib.cbt_settle.argtypes = lib.cbt_free.argtypes = (ctypes.c_void_p,)
        lib.cbt_settle.restype = lib.cbt_free.restype = None
        lib.cbt_state.argtypes = (ctypes.c_void_p, ctypes.POINTER(ctypes.c_int64))
        lib.cbt_state.restype = None
        lib.cbt_result_get.argtypes = (ctypes.c_void_p,)
        lib.cbt_result_get.restype = ctypes.POINTER(_Result)
    except Exception as e:                                   # (no DLL: the Python hooks run)
        _WHY.append("%s: %s" % (type(e).__name__, e))
        lib = None
    _LIB.append(lib)
    return lib


def enabled():
    """The 32-bit recorder for this process: the DLL loads here (the Python hooks it replaced are gone;
    require() raises with why_not() when it does not)."""
    return _load() is not None


def why_not():
    return _WHY[0] if _WHY else None


def _static_id(lib, lo, hi, image, table):
    got = _STATICS.get(id(table))
    if got is None or got[1] is not table:
        got = _STATICS[id(table)] = (lib.cbt_static(lo, hi, bytes(image)), table)
    return got[0]


def _sha_id(lib, sha):
    got = _SHAS.get(sha)
    if got is None:
        got = _SHAS[sha] = lib.cbt_intern(str(sha).encode("utf-8", "replace"))
    return got


def _u64s(values):
    values = list(values)
    return (ctypes.c_uint64 * max(len(values), 1))(*values), len(values)


def _words(ptr, n, fmt):
    if not n:
        return []
    size = struct.calcsize(fmt)
    return list(struct.iter_unpack(fmt, ctypes.string_at(ptr, n * size)))


class Run:
    """One run's native hooks on `uc` (see the module docstring). The arguments are
    trace_function's own, already worked out: `statics` its [(lo, hi, image, table)],
    `sentinels` {address: (dll, name)} (empty when not stubbing), `saved` the reuse dict (or
    None), `gov` emulator._GOV."""

    def __init__(self, uc, *, statics, init_pages, mapped, sentinels, saved, sha, gov, fast, lean, reads,
                 quiet, stop_wild, stub, unique, branches, pads, insn_cap, stub_ret, stub_cleanup,
                 func, sec, lean_ranges, sig_resolver, outs, cache=None, sentinel_from=(), shadow=None):
        lib = self.lib = _load()
        self.uc, self.gov, self.exc, self.handle = uc, gov, None, None
        self.outs, self.sig_resolver = outs, sig_resolver
        flags = ((_FAST if fast else 0) | (_LEAN if lean else 0) | (_READS if reads else 0)
                 | (_QUIET if quiet else 0) | (_STOP_WILD if stop_wild else 0) | (_STUB if stub else 0)
                 | (_UNIQUE if unique else 0) | (_BRANCHES if branches else 0) | (_PADS if pads else 0)
                 | (_SAVED if saved is not None else 0) | _RESOLVER)
        # the import landing places: kept with the machine (`cache`), made again only when what
        # they were made from (`sentinel_from`: the machine's map and stub_at, by identity) changes
        cache = cache if sentinel_from else None
        held = cache.get("_native_sentinels") if cache is not None else None
        if held is not None and len(held[0]) == len(sentinel_from) \
                and all(a is b for a, b in zip(held[0], sentinel_from)) and held[1] == len(sentinels):
            names, sent = held[2], held[3]
        else:
            names = list(sentinels.values())
            sent = _u64s(sentinels.keys())
            if cache is not None:
                cache["_native_sentinels"] = (tuple(sentinel_from), len(sentinels), names, sent)
        self.names = names
        ids = [_static_id(lib, lo, hi, image, table) for lo, hi, image, table in statics]
        self._keep = [(ctypes.c_int32 * max(len(ids), 1))(*ids), _u64s(init_pages), _u64s(mapped),
                      _u64s(saved.keys() if saved is not None else ()), sent]
        self._resolver = RESOLVER(self._resolve)
        c = self.config = _Config()
        c.uch = uc._uch.value if hasattr(uc._uch, "value") else uc._uch
        c.flags, c.sha_id = flags, _sha_id(lib, sha)
        c.insn_cap, c.stub_ret, c.stub_cleanup = int(insn_cap), int(stub_ret), int(stub_cleanup)
        c.gov_n, c.gov_ceil = int(gov["n"]), int(gov["ceil"] or 0)
        c.func_lo, c.func_hi = func
        c.sec_lo, c.sec_hi = sec
        if pads:
            c.pads_lo, c.pads_hi = int(pads[0]), int(pads[1])
        c.quiet_lo, c.quiet_hi = quiet or (0, 0)
        c.lean_low_end, c.lean_high_begin = lean_ranges
        c.statics, c.nstatics = self._keep[0], len(ids)
        c.init_pages, c.ninit = self._keep[1]
        c.mapped, c.nmapped = self._keep[2]
        c.presaved, c.npresaved = self._keep[3]
        c.sentinels, c.nsentinels = sent
        c.resolver = self._resolver
        c.shadow = shadow
        self.handle = lib.cbt_begin(ctypes.byref(c))
        if not self.handle:
            raise RuntimeError("cbtrace: a run could not be set up")
        e = lib.cbt_attach(self.handle)
        if e:
            import unicorn
            raise unicorn.UcError(e)
        self._state = (ctypes.c_int64 * 5)()

    def _resolve(self, index, out, cap):
        """An import entered: its argument count (sig_resolver) and what it gives back (_outs)."""
        try:
            dll, name = self.names[index]
            if dll == "CALLBACK":
                argc = 0
            else:
                argc = self.sig_resolver(dll, name) if self.sig_resolver else None
            out[0] = -1 if argc is None else int(argc)
            out[1] = 0
            if argc and dll != "CALLBACK":
                given = self.outs(name)[:cap]
                out[1] = len(given)
                for k, (i, v) in enumerate(given):
                    out[2 + 2 * k] = int(i)
                    out[3 + 2 * k] = -1 if v is None else int(v)
            return 0
        except BaseException as e:                       # raised where Python's hook would have
            if self.exc is None:
                self.exc = e
            return 1

    def state(self):
        self.lib.cbt_state(self.handle, self._state)
        s = self._state
        self.gov["n"] = s[3]
        return s

    def start(self, uc, begin, until, count=0):
        """uc.emu_start, then raise what a Python hook would have raised in it."""
        self.exc = None
        try:
            uc.emu_start(begin, until, count=count)
        finally:
            s = self.state()
        if self.exc is not None:
            e, self.exc = self.exc, None
            raise e
        if s[0] == 2:                                    # a read a hook could not make
            import unicorn
            raise unicorn.UcError(int(s[1]))

    def switched(self):
        return bool(self.lib.cbt_switched(self.handle))

    def attach_code(self):
        e = self.lib.cbt_attach_code(self.handle)
        if e:
            import unicorn
            raise unicorn.UcError(e)

    def settle(self, rec):
        self.lib.cbt_settle(self.handle)
        rec["truncated"] = bool(self.state()[2])

    def collect(self, rec):
        """The record's fields, as trace_function's Python hooks leave them."""
        r = self.lib.cbt_result_get(self.handle).contents
        self.gov["n"] = r.gov_n
        rec["insn_count"] = r.insn_count
        rec["truncated"] = bool(r.truncated)
        if r.governed:
            rec["governed"] = True
        rec["path"] = list((ctypes.c_uint64 * r.npath).from_address(r.path)) if r.npath else []
        if r.nmn:
            addrs = (ctypes.c_uint64 * r.nmn).from_address(r.mn_addr)
            texts = ctypes.string_at(r.mn_text, r.mn_text_len).decode("ascii", "replace").split("\0")
            rec["mnemonics"] = dict(zip(addrs, texts))
        else:
            rec["mnemonics"] = {}
        calls = []
        for w in _words(r.calls, r.ncalls, "<%dq" % _CALL_WORDS):
            tgt = w[2] if w[1] else None
            cr = {"pc": w[0], "target": tgt, "indirect": tgt is None, "into_func": bool(w[3])}
            if w[4] == 1:
                cr["resolved_target"] = w[5]
                cr["external"] = bool(w[6])
            elif w[4] == 2:
                cr.update(resolved_target=w[5], external=True, api=self._api(w[7]), stubbed=True,
                          ret_sentinel=w[9] if w[8] else None)
            if w[10]:
                cr["args_stack"] = list(w[11:17])
                cr["ecx"] = w[17]
                cr["edx"] = w[18]
            if w[19]:
                cr["argc"] = w[20]
            calls.append(cr)
        rec["calls"] = calls
        rec["mem_reads"] = [{"pc": w[0], "addr": w[1], "size": w[2], "value": None if w[4] else w[3], "n": w[5]}
                            for w in _words(r.reads, r.nreads, "<%dQ" % _MEM_WORDS)]
        if self.config.flags & _LEAN:
            rec["mem_writes"] = _words(r.writes, r.nwrites, "<%dQ" % _LEAN_WORDS)
            data = ctypes.string_at(r.pages0_data, r.npages0 * _PAGE) if r.npages0 else b""
            bases = (ctypes.c_uint64 * r.npages0).from_address(r.pages0) if r.npages0 else ()
            rec["pages0"] = {b: data[i * _PAGE:(i + 1) * _PAGE] for i, b in enumerate(bases)}
        else:
            rec["mem_writes"] = [{"pc": w[0], "addr": w[1], "size": w[2], "value": w[3], "n": w[5], "old": w[7]}
                                 for w in _words(r.writes, r.nwrites, "<%dQ" % _MEM_WORDS)]
        rec["lazy_maps"] = [{"pc": w[0], "addr": w[1], "page": w[2], "access": _ACCESS[w[3]], "size": w[4]}
                            for w in _words(r.lazy, r.nlazy, "<%dQ" % _LAZY_WORDS)]
        if r.memops >= 0:
            rec["memops"] = r.memops
        if r.writes_seen >= 0:
            rec["writes_seen"] = r.writes_seen
        if r.dirty:
            rec["_dirty"] = True
        if r.stuck >= 0:
            rec["_stuck"] = r.stuck
        if r.bogus_stubs >= 0:
            rec["bogus_stubs"] = r.bogus_stubs
        if r.ran_into_zeros:
            rec["ran_into_zeros"] = True
        if r.ndeps:
            rec["api_deps"] = [self._api(d) for d in (ctypes.c_int64 * r.ndeps).from_address(r.deps)]
        if r.api_calls_set:
            seq, words = [], list((ctypes.c_int64 * r.napi_words).from_address(r.api_calls)) if r.napi_words else []
            i = 0
            while i < len(words):
                code, argc = words[i], words[i + 1]
                if argc < 0:
                    seq.append((self._api(code), None))
                    i += 2
                else:
                    seq.append((self._api(code), words[i + 2:i + 2 + argc]))
                    i += 2 + argc
            rec["api_calls"] = seq
        if r.stubbed >= 0:
            rec["stubbed_externals"] = r.stubbed
        if r.nvxd:
            rec["vxd_services"] = list((ctypes.c_uint64 * r.nvxd).from_address(r.vxd))
        if self.config.flags & _BRANCHES:
            log = []
            for w in _words(r.branches, r.nbranches, "<%dQ" % _BRANCH_WORDS):
                cmp = (_COMPARES[w[3]], w[4], w[5], w[6], w[8] if w[7] else None, w[10] if w[9] else None) \
                    if w[2] else None
                log.append((w[0], bool(w[1]), cmp))
            rec["branches"] = log
        self.lean_seen = r.lean_seen

    def _api(self, code):
        if code < 0:
            return "CALLBACK!%d" % (-code - 1)
        return "%s!%s" % self.names[code]

    def detach(self):
        if self.lib.cbt_detach(self.handle):
            raise RuntimeError("cbtrace: a hook could not be taken off")

    def restore(self):
        e = self.lib.cbt_restore(self.handle)
        if e:
            import unicorn
            raise unicorn.UcError(e)

    def __del__(self):
        h, self.handle = getattr(self, "handle", None), None
        if h:
            self.lib.cbt_free(h)


def require():
    """The recorder is C++ only (the Python hooks it replaced are gone): raise when the DLL cannot
    be used here, saying why."""
    if not enabled():
        raise RuntimeError("the run recorder is native (native/cb/emulate/tracer, cbtrace.dll): %s" % why_not())
