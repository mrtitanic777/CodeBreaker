#!/usr/bin/env python3
"""Tests for the native run recorder (native/tracer, emulate/native_trace.py): a function traced
with the Python hooks and with the C++ ones gives the same record -- every key, and the
instruction texts in the same order. Run:  python tests/test_native_trace.py   (exit 0 = passed)

The first checks use a small module made here (hand-assembled code, an import slot), so they
need no corpus; the last traces lz32.dll's functions when the corpus is there. Everything is
SKIPPED when cbtrace.dll is not built."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.emulate import emulator as E, native_trace   # noqa: E402

RESULTS = []


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "PASS", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


VA0 = 0x10001000
IAT = 0x10002000
# f (0x00): stdcall f(a, p): if *p != a { *p = a; Sleep(a) }; read an unmapped page; a loop in g
CODE_F = bytes.fromhex(
    "55" "8BEC" "53"                 # push ebp; mov ebp, esp; push ebx
    "8B4508" "8B4D0C"                # mov eax, [ebp+8]; mov ecx, [ebp+0Ch]
    "8B19" "3BD8" "7409"             # mov ebx, [ecx]; cmp ebx, eax; je +9
    "8901" "50" "FF1500200010"       # mov [ecx], eax; push eax; call [IAT] (Sleep)
    "8B1500004000"                   # mov edx, [0x400000]: an unmapped page
    "49" "7502" "33C0"               # dec ecx; jne +2; xor eax, eax
    "E807000000"                     # call g
    "5B" "5D" "C20800" "9090")       # pop ebx; pop ebp; ret 8
CODE_G = bytes.fromhex("B903000000" "01C8" "E2FC" "C3" + "90" * 6)     # g (0x30): a loop
CODE_H = bytes.fromhex("33C0" "FFD0" "CD2001000200" "C3" + "90" * 5)   # h (0x40): call 0; int 20h
MODULE = "native-trace-test.dll"


def _install():
    code = (CODE_F + CODE_G + CODE_H).ljust(0x100, b"\x90")
    funcs = {0: {"end": 0x30}, 0x30: {"end": 0x40}, 0x40: {"end": 0x50}}
    E._LOADED[MODULE] = (MODULE, VA0, code, ".text", funcs, "native-trace-test", {IAT: ("KERNEL32.dll", "Sleep")})


def _uninstall():
    E._LOADED.pop(MODULE, None)
    for k in [k for k in E._RESOLVED if k[0] == MODULE]:
        del E._RESOLVED[k]
    for k in [k for k in E._MACHINES if k[0] == MODULE]:
        del E._MACHINES[k]


def _need():
    os.environ["CB_NATIVE_TRACE"] = "1"
    if not native_trace.enabled():
        raise Skip("cbtrace.dll not usable: %s" % native_trace.why_not())


def _both(module, func, **kw):
    """(Python's record, the native one) of one run."""
    out = []
    for mode in ("0", "1"):
        os.environ["CB_NATIVE_TRACE"] = mode
        try:
            out.append(E.trace_function(module, func=func, **kw))
        except Exception as e:                       # (both must raise alike too)
            out.append(("raised", type(e).__name__, str(e)))
    os.environ.pop("CB_NATIVE_TRACE", None)
    return out


def _same(a, b, what):
    if a != b:
        keys = sorted(k for k in set(a) | set(b) if a.get(k, "-") != b.get(k, "-")) \
            if isinstance(a, dict) and isinstance(b, dict) else ["(raised)"]
        raise AssertionError("%s: the records differ in %s" % (what, ", ".join(keys)))
    if isinstance(a, dict):
        assert list(a["mnemonics"].items()) == list(b["mnemonics"].items()), "%s: text order differs" % what


def _resolver(dll, name):
    return 1 if name == "Sleep" else None


JUDGE = dict(stub_externals=True, reads=False, stop_wild=True, quiet_stack=True, reuse=True, fast=True,
             lean=True, pads=(0xC0DE0000, 0xC0DE1000), sig_resolver=_resolver)
CONFIGS = {
    "plain": dict(),
    "stub": dict(stub_externals=True, sig_resolver=_resolver),
    "stub-unique-reads": dict(stub_externals=True, stub_ret_unique=True, reads=True, sig_resolver=_resolver),
    "judge": JUDGE,
    "judge-branches": dict(JUDGE, branches=True),
    "judge-capped": dict(JUDGE, insn_cap=12),
    "one-at-a-time": dict(JUDGE, fast=False, lean=False, branches=True),
    "no-resolver": dict(stub_externals=True, reuse=True, fast=True),
}
P = E._SCRATCH_BASE + 0x100


def test_same_record_synthetic():
    _need()
    _install()
    try:
        n = 0
        for name, cfg in CONFIGS.items():
            for args, mem in (([5, P], [(P, (7).to_bytes(4, "little"))]),    # *p != a: the write and the import
                              ([7, P], [(P, (7).to_bytes(4, "little"))])):   # *p == a: the branch taken
                a, b = _both(MODULE, VA0, args=args, mem_init=mem, **cfg)
                _same(a, b, "f %s %s" % (name, args))
                assert isinstance(a, dict), (name, a)
                if cfg.get("sig_resolver"):           # (else Sleep's argument stays on the stack)
                    assert a["exit"] == ("truncated" if "insn_cap" in cfg else "ret"), (name, a["exit"])
                n += 1
            for off in (0x30, 0x40):
                a, b = _both(MODULE, VA0 + off, args=[0, 0], **cfg)
                _same(a, b, "0x%x %s" % (off, name))
                n += 1
        assert n == 4 * len(CONFIGS)
    finally:
        _uninstall()


def test_record_has_what_was_run():
    """The native record of f holds what f did: the import entered with its argument, the write
    through the pointer, the page mapped on demand, the branch the compare decided."""
    _need()
    _install()
    try:
        os.environ["CB_NATIVE_TRACE"] = "1"
        r = E.trace_function(MODULE, func=VA0, args=[5, P], mem_init=[(P, (7).to_bytes(4, "little"))],
                             stub_externals=True, sig_resolver=_resolver, branches=True)
        assert r["api_calls"] == [("KERNEL32.dll!Sleep", [5])], r.get("api_calls")
        assert any(w["addr"] == P and w["value"] == 5 and w["old"] == 7 for w in r["mem_writes"])
        assert any(m["page"] == 0x400000 and m["access"] == "read" for m in r["lazy_maps"])
        assert r["branches"][0][:2] == (VA0 + 0x0E, False), r["branches"]
        # (7 loaded from *p, 5 from the argument's place on the stack)
        assert r["branches"][0][2] == ("cmp", 4, 7, 5, P, E._STACK_TOP - 8), r["branches"][0]
        assert r["mnemonics"]["0x%x" % VA0] == "push ebp" and r["esp_delta"] == 12
        h = E.trace_function(MODULE, func=VA0 + 0x40, args=[0, 0])
        assert h["bogus_stubs"] == 1 and h["vxd_services"] == [VA0 + 0x46], h
    finally:
        os.environ.pop("CB_NATIVE_TRACE", None)
        _uninstall()


def test_python_raises_through():
    """A resolver that raises stops the run and the exception comes out of trace_function, as
    it does from the Python hook."""
    _need()
    _install()
    try:
        def bad(dll, name):
            raise KeyError(name)
        a, b = _both(MODULE, VA0, args=[5, P], mem_init=[(P, (7).to_bytes(4, "little"))],
                     stub_externals=True, sig_resolver=bad)
        assert a == b == ("raised", "KeyError", "'Sleep'"), (a, b)
    finally:
        _uninstall()


# ---- the flat driver recorder (emulate/driver.py run, emulate/native_driver.py) ----------------
DRV_BASE = 0x80010000
# a flat ring-0 routine: set EAX, store it through a far address (an unmapped page), poll a UART
# port and write it, make an INT 20h service call (VMM 0001:0001), call out through a pointer
# (EAX, nothing there), then return.
DRV_CODE = bytes.fromhex(
    "B844332211"          # mov eax, 0x11223344
    "A300000090"          # mov [0x90000000], eax   (an unmapped write -> a page mapped on demand)
    "66BAF803"            # mov dx, 0x3F8
    "EC"                  # in al, dx               (a UART port read)
    "EE"                  # out dx, al              (a UART port write)
    "CD20" "01000100"     # int 20h ; dd 0001:0001  (a VMM service call)
    "FFD0"                # call eax                (a call out through a pointer)
    "C3")                 # ret


class _DrvImage:
    def __init__(self, base, code):
        self.regions = [(base, code)]
        self.spans = [(base, base + len(code), 0)]

    def holds(self, a):
        return any(lo <= a < hi for lo, hi, _i in self.spans)


def _need_drv():
    os.environ["CB_NATIVE_TRACE_DRV"] = "1"
    from codebreaker.emulate import native_driver
    if not native_driver.enabled():
        raise Skip("cbtrace.dll not usable: %s" % native_driver.why_not())


def _drv_both(image, entry, **kw):
    from codebreaker.emulate import driver
    out = []
    for mode in ("0", "1"):
        os.environ["CB_NATIVE_TRACE_DRV"] = mode
        try:
            out.append(driver.run(image, entry, **kw))
        except Exception as e:
            out.append(("raised", type(e).__name__, str(e)))
    os.environ.pop("CB_NATIVE_TRACE_DRV", None)
    return out


def test_driver_same_record():
    """A flat driver run traced with the Python hooks and the C++ ones gives the same rec --
    every field: the cover, the service and outside calls, the ports, the writes, the page
    mapped on demand, the exit and the result registers. No corpus needed."""
    _need_drv()
    image = _DrvImage(DRV_BASE, DRV_CODE)
    for extra in ({}, {"service_eax": 0x1234, "service_carry": 1}, {"cap": 4}, {"stack": (7, 8, 9)}):
        a, b = _drv_both(image, DRV_BASE, **extra)
        assert isinstance(a, dict) and isinstance(b, dict), (extra, a, b)
        if a != b:
            keys = sorted(k for k in set(a) | set(b) if a.get(k, "-") != b.get(k, "-"))
            raise AssertionError("driver %s: records differ in %s\n  py=%r\n  na=%r"
                                 % (extra, keys, {k: a.get(k) for k in keys}, {k: b.get(k) for k in keys}))
    # and what the run did is actually present
    r = _drv_both(image, DRV_BASE)[1]
    assert r["exit"] == "ret", r["exit"]
    assert any(d is not None and s == 1 for d, s, _re, _wo in r["calls"]), r["calls"]      # the service
    assert any(d is None for d, _s, _re, _wo in r["calls"]), r["calls"]                     # the call out
    assert r["ports"] and r["outside"] == 1, (r["ports"], r["outside"])
    assert any(w["addr"] == 0x90000000 and w["value"] == 0x11223344 for w in r["mem_writes"]), r["mem_writes"]
    assert any(m["page"] == 0x90000000 for m in r["lazy_maps"]), r["lazy_maps"]


# ---- the 16-bit recorder (emulate/windows16.py trace_function16, emulate/native_win16.py) -------
def _win16_both(path, seg, off, **kw):
    from codebreaker.emulate import windows16 as ne
    out = []
    for mode in ("0", "1"):
        os.environ["CB_NATIVE_TRACE16"] = mode
        try:
            out.append(ne.probe(path, seg=seg, offset=off, **kw))
        except Exception as e:
            out.append(("raised", type(e).__name__, str(e)))
    os.environ.pop("CB_NATIVE_TRACE16", None)
    return out


def test_win16_same_record():
    """A 16-bit NE function traced with the Python hooks and the C++ ones gives the same record --
    the path, the instruction count, the imports hit, the writes, the pages mapped on demand, the
    ports, the exit and the result. Uses lzexpand.dll's functions; skipped without the corpus."""
    _need()
    from codebreaker.emulate import native_win16
    os.environ["CB_NATIVE_TRACE16"] = "1"
    if not native_win16.enabled():
        raise Skip("cbtrace.dll not usable: %s" % native_win16.why_not())
    from codebreaker.base import corpus
    from codebreaker.formats.ne import NEFile
    try:
        path = corpus.retail("lzexpand.dll")
    except Exception:
        path = None
    if not path or not os.path.isfile(path):
        raise Skip("lzexpand.dll is not in the corpus")
    ents = NEFile(path).entries()
    funcs = [e for e in ents if e and e.get("kind") != "UNUSED" and e.get("seg")][:12]
    n = 0
    for e in funcs:
        for extra in ({}, {"ports": True}, {"insn_cap": 64}):
            a, b = _win16_both(path, e["seg"], e["offset"], args=[0, 0, 0, 0], **extra)
            if a != b:
                keys = sorted(k for k in set(a) | set(b) if a.get(k, "-") != b.get(k, "-")) \
                    if isinstance(a, dict) and isinstance(b, dict) else ["(raised)"]
                raise AssertionError("lz16 %d:%04x %s: records differ in %s"
                                     % (e["seg"], e["offset"], extra, ", ".join(map(str, keys))))
            n += 1
    assert n == 3 * len(funcs) and funcs


def test_same_record_lz32():
    _need()
    from codebreaker.base import corpus
    try:
        corpus.resolve("lz32.dll")
    except Exception:
        raise Skip("lz32.dll is not in the corpus")
    from codebreaker.verify.lanes import win32
    _p, va0, _c, _s, funcs, _sha, _imp = E._load("lz32.dll")
    offs = sorted(o for o, f in funcs.items() if f["end"] - o >= 8)[:12]
    cfg = dict(JUDGE, sig_resolver=win32._argc)
    for off in offs:
        for extra in ({}, {"branches": True}, {"fast": False, "lean": False}):
            a, b = _both("lz32.dll", va0 + off, args=[P, 0, 3, 0xFFFFFFFF], **dict(cfg, **extra))
            _same(a, b, "lz32 0x%x %s" % (off, extra))


def main():
    for fn in (test_same_record_synthetic, test_record_has_what_was_run, test_python_raises_through,
               test_driver_same_record, test_win16_same_record, test_same_record_lz32):
        check(fn)
    for name, status, why in RESULTS:
        print("%-32s %s %s" % (name, status, why))
    failed = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (sum(1 for r in RESULTS if r[1] == "PASS"),
                                                sum(1 for r in RESULTS if r[1] == "SKIP"), failed))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
