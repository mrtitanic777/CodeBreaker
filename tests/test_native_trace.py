#!/usr/bin/env python3
"""Tests for the native run recorders (native/tracer: cbtrace.dll -- the 32-bit tracer, the flat
driver recorder, the 16-bit recorder) and the path explorer's shadow in the same DLL. Run:
python tests/test_native_trace.py   (exit 0 = passed)

The Python hooks these replaced are gone (docs/NATIVE.md, step 3). Before they went, every case
below was traced both ways and the records agreed in every key (and the instruction texts in the
same order); the agreed record of each case is kept as a digest in tests/data/native_trace_golden.txt
(written by state/work/shadow-native/golden.py at the time). A case now traces with the C++ and
wants the same digest. Shadowed cases also hold the shadow's path, trail, inputs and what it lost.

The first cases use a small module made here (hand-assembled code, an import slot) and need no
corpus; the lzexpand.dll and lz32.dll cases are skipped without it. Everything is SKIPPED when
cbtrace.dll is not built."""
import hashlib
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.emulate import emulator as E, native_trace   # noqa: E402

RESULTS = []
GOLDEN = os.path.join(ROOT, "tests", "data", "native_trace_golden.txt")


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
    if not native_trace.enabled():
        raise Skip("cbtrace.dll not usable: %s" % native_trace.why_not())


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

# ---- the flat driver recorder (emulate/driver.py run) -------------------------------------------
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


# ---- the cases ----------------------------------------------------------------------------------
_SWITCHES = ("CB_NATIVE_TRACE", "CB_NATIVE_TRACE16", "CB_NATIVE_TRACE_DRV", "CB_NATIVE_SHADOW")


def _modes(both):
    """("1",) -- the C++; both: ("0", "1"), the Python hooks and shadow first (golden.py, while they were there)."""
    return ("0", "1") if both else (None,)


def _call(mode, fn):
    saved = {k: os.environ.get(k) for k in _SWITCHES}
    if mode is not None:
        for k in _SWITCHES:
            os.environ[k] = mode
    try:
        return fn()
    except Exception as e:                           # (both must raise alike too)
        return ("raised", type(e).__name__, str(e))
    finally:
        for k, v in saved.items():
            if v is None:
                os.environ.pop(k, None)
            else:
                os.environ[k] = v


def _with_shadow(make, run):
    """A run beside its shadow: (the record, the shadow's path, trail, inputs, what it lost)."""
    sh = make()
    rec = run(sh)
    return (rec, sh.path, sh.trail, sh.vars, sorted(sh.lost))


def _lifter(mode):
    from codebreaker.emulate import pcode
    return pcode.lifter(mode)


def cases(both=False):
    """(test, key, record[, record]) of every case: one record each, or both=True two (Python, C++)."""
    from codebreaker.emulate import driver, shadow
    _install()
    try:
        for name, cfg in CONFIGS.items():
            for args, mem in (([5, P], [(P, (7).to_bytes(4, "little"))]),
                              ([7, P], [(P, (7).to_bytes(4, "little"))])):
                yield ("synthetic", "f %s %s" % (name, args)) + tuple(
                    _call(m, lambda: E.trace_function(MODULE, func=VA0, args=args, mem_init=mem, **cfg))
                    for m in _modes(both))
            for off in (0x30, 0x40):
                yield ("synthetic", "0x%x %s" % (off, name)) + tuple(
                    _call(m, lambda: E.trace_function(MODULE, func=VA0 + off, args=[0, 0], **cfg)) for m in _modes(both))

        def bad(dll, name):
            raise KeyError(name)
        yield ("raises", "f raises") + tuple(
            _call(m, lambda: E.trace_function(MODULE, func=VA0, args=[5, P], mem_init=[(P, (7).to_bytes(4, "little"))],
                                              stub_externals=True, sig_resolver=bad)) for m in _modes(both))
        lifter = _lifter("x86-32-flat")
        if lifter is not None:
            for args, cfg in (([5, P], dict(stub_externals=True, sig_resolver=_resolver)),
                              ([7, P], dict(stub_externals=True, sig_resolver=_resolver, stub_ret_unique=True)),
                              ([5, P], dict(stub_externals=True, sig_resolver=_resolver, branches=True, insn_cap=9))):
                def one(args=args, cfg=cfg):
                    def make():
                        sh = shadow.Shadow32(lifter, [(P, P + 8)], span=(VA0, VA0 + 0x30))
                        esp = E._STACK_TOP - 4 * len(args) - 4
                        sh.enter([(esp + 4 + 4 * k, a, k) for k, a in enumerate(args)], bits=32)
                        return sh
                    return _with_shadow(make, lambda sh: E.trace_function(
                        MODULE, func=VA0, args=args, mem_init=[(P, (7).to_bytes(4, "little"))], shadow=sh, **cfg))
                yield ("shadow32", "f shadow %s %s" % (args, sorted(cfg))) + tuple(_call(m, one) for m in _modes(both))
    finally:
        _uninstall()

    image = _DrvImage(DRV_BASE, DRV_CODE)
    for extra in ({}, {"service_eax": 0x1234, "service_carry": 1}, {"cap": 4}, {"stack": (7, 8, 9)}):
        yield ("driver", "driver %s" % sorted(extra.items())) + tuple(
            _call(m, lambda: driver.run(image, DRV_BASE, **extra)) for m in _modes(both))
    if lifter is not None:
        def drv_shadow():
            def make():
                sh = shadow.ShadowLE(lifter, [(E._SCRATCH_BASE, E._SCRATCH_BASE + E._SCRATCH_SIZE)])
                sh.enter([("EAX", 3, "eax"), ("EDX", 0x3F8, "edx")], bits=32)
                sh.enter([("CF", 0, "cf"), ("ZF", 0, "zf")], bits=8)
                return sh
            return _with_shadow(make, lambda sh: driver.run(image, DRV_BASE, {"eax": 3, "edx": 0x3F8},
                                                            service_eax=5, service_carry=1, shadow=sh))
        yield ("driver", "driver shadow") + tuple(_call(m, drv_shadow) for m in _modes(both))

    from codebreaker.base import corpus
    try:
        lz16 = corpus.retail("lzexpand.dll")
    except Exception:
        lz16 = None
    if lz16 and os.path.isfile(lz16):
        from codebreaker.emulate import windows16 as ne
        from codebreaker.formats.ne import NEFile
        ents = NEFile(lz16).entries()
        funcs = [e for e in ents if e and e.get("kind") != "UNUSED" and e.get("seg")][:12]
        for e in funcs:
            for extra in ({}, {"ports": True}, {"insn_cap": 64}):
                yield ("win16", "lz16 %d:%04x %s" % (e["seg"], e["offset"], sorted(extra))) + tuple(
                    _call(m, lambda: ne.probe(lz16, seg=e["seg"], offset=e["offset"], args=[0, 0, 0, 0], **extra))
                    for m in _modes(both))
        first = funcs[0]
        for e in funcs[1:5]:                         # (a prelude: another function run first, its trace dropped)
            yield ("win16", "lz16 %d:%04x prelude" % (e["seg"], e["offset"])) + tuple(
                _call(m, lambda: ne.probe(lz16, seg=e["seg"], offset=e["offset"], args=[0, 0, 0, 0],
                                          prelude=[{"seg": first["seg"], "offset": first["offset"], "args": [1, 2]}]))
                for m in _modes(both))
        lifter16 = _lifter("x86-16-real")
        if lifter16 is not None:
            auto = NEFile(lz16).h.get("autodata_seg")
            ds = ne.segment_base(auto) if auto else 0
            for e in funcs:
                def w16(e=e):
                    base = ne.segment_base(e["seg"])
                    return _with_shadow(
                        lambda: shadow.Shadow16(lifter16, ne.selector_base, [(ds, ds + 0x10000),
                                                (ne.SCRATCH_BASE, ne.SCRATCH_BASE + 0x10000)],
                                                span=(base + e["offset"], base + e["offset"] + 0x400)),
                        lambda sh: ne.probe(lz16, seg=e["seg"], offset=e["offset"], args=[0, 0, 0, 0],
                                            shadow=sh, ports=True))
                yield ("win16", "lz16 %d:%04x shadow" % (e["seg"], e["offset"])) + tuple(_call(m, w16) for m in _modes(both))

    try:
        corpus.resolve("lz32.dll")
        have32 = True
    except Exception:
        have32 = False
    if have32:
        from codebreaker.verify.lanes import win32
        _p, va0, _c, _s, funcs, _sha, _imp = E._load("lz32.dll")
        offs = sorted(o for o, f in funcs.items() if f["end"] - o >= 8)[:12]
        cfg = dict(JUDGE, sig_resolver=win32._argc)
        for off in offs:
            for extra in ({}, {"branches": True}, {"fast": False, "lean": False}):
                yield ("lz32", "lz32 0x%x %s" % (off, sorted(extra))) + tuple(
                    _call(m, lambda: E.trace_function("lz32.dll", func=va0 + off, args=[P, 0, 3, 0xFFFFFFFF],
                                                      **dict(cfg, **extra))) for m in _modes(both))


def _canon(v):
    """A text of `v` the same in every process (a set's order is not)."""
    if isinstance(v, (set, frozenset)):
        return "{%s}" % ", ".join(sorted(_canon(x) for x in v))
    if isinstance(v, dict):
        return "{%s}" % ", ".join(sorted("%s: %s" % (_canon(k), _canon(x)) for k, x in v.items()))
    if isinstance(v, (list, tuple)):
        return "%s%s%s" % ("[" if isinstance(v, list) else "(", ", ".join(_canon(x) for x in v),
                           "]" if isinstance(v, list) else ")")
    return repr(v)


def digest(rec):
    text = _canon(rec)
    if isinstance(rec, dict):                        # (and the instruction texts in their order)
        text += repr(list(rec.get("mnemonics", {}).items()))
    return hashlib.sha1(text.encode()).hexdigest()


_ALL = []


def _golden():
    want = {}
    for line in open(GOLDEN):
        key, _, d = line.rstrip("\n").rpartition(" ")
        if key:
            want[key] = d
    return want


def _run_cases():
    if not _ALL:
        _ALL.extend(c[:3] for c in cases())
    return _ALL


def _held(test):
    _need()
    want = _golden()
    got = [(k, r) for t, k, r in _run_cases() if t == test]
    if not got:
        raise Skip("no %s cases here (the corpus or the lifter is not there)" % test)
    bad = [k for k, r in got if want.get(k) != digest(r)]
    assert not bad, "%d of %d records differ from the agreed ones: %s" % (len(bad), len(got), bad[:5])


def test_same_record_synthetic():
    _held("synthetic")


def test_python_raises_through():
    """A resolver that raises stops the run and the exception comes out of trace_function."""
    _held("raises")
    r = [r for t, _k, r in _run_cases() if t == "raises"][0]
    assert r == ("raised", "KeyError", "'Sleep'"), r


def test_shadow32_same():
    _held("shadow32")
    r = [r for t, _k, r in _run_cases() if t == "shadow32"][0]
    assert r[1] and r[3], "the shadowed run made no conditions: %r" % (r[1:],)


def test_driver_same_record():
    _held("driver")


def test_win16_same_record():
    _held("win16")


def test_same_record_lz32():
    _held("lz32")


def test_record_has_what_was_run():
    """The native record of f holds what f did: the import entered with its argument, the write
    through the pointer, the page mapped on demand, the branch the compare decided."""
    _need()
    _install()
    try:
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
        _uninstall()
    from codebreaker.emulate import driver
    r = driver.run(_DrvImage(DRV_BASE, DRV_CODE), DRV_BASE)
    assert r["exit"] == "ret", r["exit"]
    assert any(d is not None and s == 1 for d, s, _re, _wo in r["calls"]), r["calls"]      # the service
    assert any(d is None for d, _s, _re, _wo in r["calls"]), r["calls"]                     # the call out
    assert r["ports"] and r["outside"] == 1, (r["ports"], r["outside"])
    assert any(w["addr"] == 0x90000000 and w["value"] == 0x11223344 for w in r["mem_writes"]), r["mem_writes"]
    assert any(m["page"] == 0x90000000 for m in r["lazy_maps"]), r["lazy_maps"]


def main():
    for fn in (test_same_record_synthetic, test_record_has_what_was_run, test_python_raises_through,
               test_shadow32_same, test_driver_same_record, test_win16_same_record, test_same_record_lz32):
        check(fn)
    for name, status, why in RESULTS:
        print("%-32s %s %s" % (name, status, why))
    failed = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (sum(1 for r in RESULTS if r[1] == "PASS"),
                                                sum(1 for r in RESULTS if r[1] == "SKIP"), failed))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
