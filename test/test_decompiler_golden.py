#!/usr/bin/env python3
"""Golden regression tests for the native x86 decompiler. Run:  python test/test_decompiler_golden.py

Each case pins a lifter/decompiler bug that was found by behavioural verification and fixed, so it
cannot quietly come back. The machine code is embedded here and wrapped in a minimal PE, so the
tests need only the built decompiler -- not the retail corpus. They assert on what the output MEANS
(which values are returned, on how many paths), not on exact text, so cosmetic changes to variable
naming or layout do not break them.

One extra check runs the full behavioural verifier on the retail function when the corpus and
OpenWatcom are present, and is skipped otherwise."""
import os
import re
import struct
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from src.core import corpus, paths, toolchain   # noqa: E402

RESULTS = []
CODE_RVA = 0x1000


class Skip(Exception):
    pass


class KnownBug(Exception):
    """An open, understood defect. Reported as KNOWN (not a failure) until it is fixed."""


def make_pe(code, image_base=0x400000):
    """A minimal 32-bit PE holding `code` in one .text section at RVA 0x1000."""
    raw = code + b"\xCC" * (0x200 - len(code))
    dos = b"MZ" + b"\0" * 0x3A + struct.pack("<I", 0x40)
    coff = struct.pack("<HHIIIHH", 0x14C, 1, 0, 0, 0, 0xE0, 0x0102)
    opt = struct.pack("<HBBIIIIIIIIIHHHHHHIIIIHHIIIIII",
                      0x10B, 6, 0, 0x200, 0, 0, CODE_RVA, CODE_RVA, 0x2000, image_base,
                      0x1000, 0x200, 4, 0, 0, 0, 4, 0, 0, 0x2000, 0x200, 0, 3, 0,
                      0x100000, 0x1000, 0x100000, 0x1000, 0, 16) + b"\0" * (16 * 8)
    sect = b".text\0\0\0" + struct.pack("<IIIIIIHHI", len(code), CODE_RVA, 0x200, 0x200,
                                        0, 0, 0, 0, 0x60000020)
    hdr = dos + b"PE\0\0" + coff + opt + sect
    return hdr + b"\0" * (0x200 - len(hdr)) + raw


def decompile(code):
    dec = toolchain.decompiler("pe")
    if not dec:
        raise Skip("win95pedec not built")
    d = tempfile.mkdtemp()
    pe = os.path.join(d, "golden.dll")
    with open(pe, "wb") as f:
        f.write(make_pe(code))
    r = subprocess.run([dec, pe, "--at", "%x" % CODE_RVA, "%x" % (CODE_RVA + len(code))],
                       capture_output=True, text=True, timeout=60)
    m = re.search(r"(?ms)^\w[\w \*]*\([^;{]*\)\s*\{.*?^\}", r.stdout or "")
    assert m, "no function emitted:\n" + (r.stdout or r.stderr or "")[:400]
    return m.group(0)


def returns(c):
    return [x.strip() for x in re.findall(r"\breturn\s+([^;]+);", c)]


# kernel32!GetLastError: one `ret` shared by two paths that hold DIFFERENT values in EAX. The path
# that reaches it by `jmp` returns the per-thread error ([thread+0x70]); the fall-through path
# returns the global one. The lifter kept only the address-last value, so both paths returned the
# global error and the `mov eax,[eax+0x70]` was dropped as dead code.
GETLASTERROR = bytes.fromhex(
    "833d78c2fbbf00"   # cmp  dword [0xBFFBC278], 0
    "7410"             # je   else
    "a178c2fbbf"       # mov  eax, [0xBFFBC278]
    "8b00"             # mov  eax, [eax]
    "85c0"             # test eax, eax
    "7405"             # je   else
    "8b4070"           # mov  eax, [eax+0x70]
    "eb05"             # jmp  done
    "a198c2fbbf"       # else: mov eax, [0xBFFBC298]
    "c3")              # done: ret


def test_shared_ret_reached_by_jmp_keeps_both_values():
    c = decompile(GETLASTERROR)
    rets = returns(c)
    assert len(rets) == 2 and rets[0] != rets[1], "expected two distinct returns:\n" + c
    assert re.search(r"field_70|\+\s*0x70|\+\s*112", c), "the [eax+0x70] load was dropped:\n" + c
    assert "0xBFFBC298" in c, c


# The conditional form: `jnz` straight to the shared `ret` while EAX holds the first value; the
# fall-through path loads a second value. Must come out as `if (...) return first;` + `return second`.
COND_RET = bytes.fromhex(
    "a100000010"       # mov  eax, [0x10000000]
    "85c0"             # test eax, eax
    "7505"             # jnz  done
    "a100000020"       # mov  eax, [0x20000000]
    "c3")              # done: ret


def test_shared_ret_reached_by_jcc_keeps_both_values():
    c = decompile(COND_RET)
    rets = returns(c)
    assert len(rets) == 2 and rets[0] != rets[1], "expected two distinct returns:\n" + c
    assert re.search(r"if\s*\(.*\)\s*return\b", c), "expected a conditional return:\n" + c
    assert "0x20000000" in c and "0x10000000" in c, c


# Guard against over-applying the fix: a block that jumps to a `ret` block which sets its OWN
# return value must still go there (the target's value wins), not return early with its own EAX.
RET_SETS_OWN_VALUE = bytes.fromhex(
    "a100000010"       # mov  eax, [0x10000000]
    "85c0"             # test eax, eax
    "7502"             # jnz  done
    "33c0"             # xor  eax, eax        (fall-through only)
    "b807000000"       # done: mov eax, 7
    "c3")              # ret


def test_ret_block_with_its_own_value_is_not_split():
    c = decompile(RET_SETS_OWN_VALUE)
    rets = returns(c)
    assert len(rets) == 1, "the function has one return value (7):\n" + c
    assert re.search(r"\b(7|0x7)\b", rets[0]), "must return the constant 7:\n" + c


def test_getlasterror_behaves_as_retail():
    """End to end on the real binary: decompile, recompile, run beside retail on the oracle."""
    k32 = corpus.retail("kernel32.dll")
    if not k32 or not toolchain.ow_tool("wcc386") or not toolchain.decompiler("pe"):
        raise Skip("needs the retail corpus, OpenWatcom and the decompiler")
    tdir = paths.target_dir("win95")
    if tdir not in sys.path:
        sys.path.insert(0, tdir)
    from src.core import cores
    from src.targets.win95 import autobuild95, bodyverify
    # The oracle resolves its engines through the active target. Nothing in verify_body sets it,
    # and without it every trace fails with an error that is swallowed into "inconclusive".
    cores.set_target(tdir)
    va =autobuild95._imagebase(autobuild95.PE(k32)) + 0x12941
    out = os.path.join(tempfile.mkdtemp(), "getlasterror")
    r = bodyverify.verify_body(k32, va, outdir=out)
    if r["verdict"] == "inconclusive":
        # The oracle (tracer._load) maps only a module's FIRST code section. kernel32 has three
        # (_FREQASM, .text, _INIT) and GetLastError is in .text, so the retail side cannot be
        # traced at all. 24 PE modules have more than one code section.
        raise KnownBug("oracle maps only the first code section; GetLastError is in kernel32's .text")
    assert r["verdict"] == "equivalent", r


def main():
    for fn in (test_shared_ret_reached_by_jmp_keeps_both_values,
               test_shared_ret_reached_by_jcc_keeps_both_values,
               test_ret_block_with_its_own_value_is_not_split,
               test_getlasterror_behaves_as_retail):
        try:
            fn()
            RESULTS.append((fn.__name__, "PASS", ""))
        except Skip as s:
            RESULTS.append((fn.__name__, "SKIP", str(s)))
        except KnownBug as k:
            RESULTS.append((fn.__name__, "KNOWN", str(k)))
        except Exception as e:
            RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, str(e)[:700])))
    for name, status, detail in RESULTS:
        print("  %-5s %s%s" % (status, name, ("\n       " + detail.replace("\n", "\n       ")) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d known bugs, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "PASS"), sum(1 for r in RESULTS if r[1] == "SKIP"),
        sum(1 for r in RESULTS if r[1] == "KNOWN"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
