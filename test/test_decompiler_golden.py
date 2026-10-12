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
    # a return on each path, each with its own value (two expressions, or one variable that is
    # assigned on each path)
    assert len(returns(c)) == 2, "expected a return on each path:\n" + c
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
    assert len(returns(c)) == 2, "expected a return on each path:\n" + c
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
    from src.formats.pe import open_pe
    from src.targets.win95 import bodyverify
    # No target is set here on purpose: this also pins two oracle fixes. verify_body must arm the
    # win95 target itself, and the oracle must map every code section (GetLastError is in
    # kernel32's second, .text; with only the first mapped it could not be traced at all).
    va = open_pe(k32).image_base + 0x12941
    out = os.path.join(tempfile.mkdtemp(), "getlasterror")
    r = bodyverify.verify_body(k32, va, outdir=out)
    assert r["verdict"] == "equivalent", r


# ---------------------------------------------------------------------------------------------
# Register flow. Each case is a few instructions of hand-written machine code that goes through
# the WHOLE real pipeline: wrapped in a PE, decompiled, recompiled with OpenWatcom, and run beside
# the original bytes on the oracle with the pipeline's input vectors. "equivalent" means the
# reconstruction returned what the bytes returned on every vector.
#
# They pin how a register or stack local is tracked across blocks: its value at the start of a
# block must come from the block's predecessors (merged where they differ), not from whichever
# block happens to precede it in the address space.
# ---------------------------------------------------------------------------------------------
def behaves(code, also=()):
    """Verdict of the real pipeline on the synthetic function at the start of `code`. `also`
    lists the offsets of further functions in `code` to verify in the same batch, so the first
    one links to their real reconstructions instead of stubs."""
    if not (toolchain.decompiler("pe") and toolchain.ow_tool("wcc386")):
        raise Skip("needs the decompiler and OpenWatcom")
    tdir = paths.target_dir("win95")
    if tdir not in sys.path:
        sys.path.insert(0, tdir)
    from src.targets.win95.verification import pipeline
    d = tempfile.mkdtemp()
    pe = os.path.join(d, "golden.dll")
    with open(pe, "wb") as f:
        f.write(make_pe(code))
    va = 0x400000 + CODE_RVA
    res = pipeline.verify("pe", pe, [va] + [va + o for o in also],
                          outdir=os.path.join(d, "out"), bank=False)
    r = res["%x" % va]
    return r["verdict"], r.get("reason", ""), d


def _assert_equivalent(code, also=()):
    verdict, reason, d = behaves(code, also)
    if verdict != "equivalent":
        try:
            c = decompile(code)
        except Exception as e:
            c = "(decompile failed: %s)" % e
        raise AssertionError("pipeline verdict %s %s\n%s" % (verdict, reason, c))


# Two paths set EAX differently and JOIN before using it. The second path's block precedes the
# join in the address space, so a straight-line register map gives the join that path's value
# on both paths.
JOIN_OF_TWO_VALUES = bytes.fromhex(
    "837c240400"       # cmp  dword [esp+4], 0
    "7507"             # jne  second
    "b801000000"       # mov  eax, 1
    "eb05"             # jmp  join
    "b802000000"       # second: mov eax, 2
    "83c010"           # join: add eax, 0x10
    "c3")              # ret


def test_join_of_two_register_values():
    _assert_equivalent(JOIN_OF_TWO_VALUES)


# A stack local assigned on both arms of an if/else and read after it.
LOCAL_SET_ON_BOTH_ARMS = bytes.fromhex(
    "558bec83ec04"     # push ebp; mov ebp, esp; sub esp, 4
    "837d0800"         # cmp  dword [ebp+8], 0
    "7409"             # je   else
    "c745fc07000000"   # mov  dword [ebp-4], 7
    "eb07"             # jmp  join
    "c745fc09000000"   # else: mov dword [ebp-4], 9
    "8b45fc"           # join: mov eax, [ebp-4]
    "8be55dc3")        # mov esp, ebp; pop ebp; ret


def test_local_set_on_both_arms():
    _assert_equivalent(LOCAL_SET_ON_BOTH_ARMS)


# A loop: EAX accumulates, ECX counts down. Both are carried round the back edge.
LOOP_CARRIED = bytes.fromhex(
    "8b4c2404"         # mov  ecx, [esp+4]
    "c1e914"           # shr  ecx, 20
    "83e107"           # and  ecx, 7
    "33c0"             # xor  eax, eax
    "85c9"             # test ecx, ecx
    "7405"             # jz   done
    "03c1"             # loop: add eax, ecx
    "49"               # dec  ecx
    "75fb"             # jnz  loop
    "c3")              # done: ret


def test_loop_carried_registers():
    _assert_equivalent(LOOP_CARRIED)


# A copy of a loop-carried register must keep the value it captured: EAX takes EDX's value,
# then EDX changes before the loop goes round. Returning EAX must give the value from before.
COPY_OF_LOOP_VALUE = bytes.fromhex(
    "8b4c2404"         # mov  ecx, [esp+4]
    "c1e914"           # shr  ecx, 20
    "83e107"           # and  ecx, 7
    "33c0"             # xor  eax, eax
    "ba05000000"       # mov  edx, 5
    "85c9"             # loop: test ecx, ecx
    "7408"             # jz   done
    "8bc2"             # mov  eax, edx
    "83c203"           # add  edx, 3
    "49"               # dec  ecx
    "ebf4"             # jmp  loop
    "c3")              # done: ret


def test_copy_of_loop_carried_value():
    _assert_equivalent(COPY_OF_LOOP_VALUE)


# ---------------------------------------------------------------------------------------------
# Stack and operand modelling.
# ---------------------------------------------------------------------------------------------
# 32-bit calls push their LAST argument first. f(1, 2, 3) is `push 3; push 2; push 1; call f`.
# The callee folds its three arguments into one number, so the caller only verifies if the
# reconstruction passes them in the right order.
CALL_ARG_ORDER = bytes.fromhex(
    "6a03"             # push 3
    "6a02"             # push 2
    "6a01"             # push 1
    "e805000000"       # call callee
    "83c40c"           # add  esp, 12
    "c3"               # ret
    "90"               # (pad)
    "8b442404"         # callee: mov eax, [esp+4]
    "c1e004"           # shl  eax, 4
    "03442408"         # add  eax, [esp+8]
    "c1e004"           # shl  eax, 4
    "0344240c"         # add  eax, [esp+12]
    "c3")              # ret


def test_call_arguments_are_in_order():
    _assert_equivalent(CALL_ARG_ORDER, also=(16,))
    c = decompile(CALL_ARG_ORDER)
    assert re.search(r"\(\s*1\s*,\s*2\s*,\s*3\s*\)", c), "arguments must read (1, 2, 3):\n" + c


# An address with an index register: a + i*4 + 8. The index used to be dropped.
INDEXED_ADDRESS = bytes.fromhex(
    "8b442404"         # mov  eax, [esp+4]
    "8b4c2408"         # mov  ecx, [esp+8]
    "8d448808"         # lea  eax, [eax+ecx*4+8]
    "c3")              # ret


def test_indexed_address():
    _assert_equivalent(INDEXED_ADDRESS)


# EBX saved and restored round its use, and an argument read through ESP after the push moved it.
SAVED_EBX_AND_ESP_ARGUMENT = bytes.fromhex(
    "53"               # push ebx
    "8b5c2408"         # mov  ebx, [esp+8]        ; the first argument, one push deeper
    "8d041b"           # lea  eax, [ebx+ebx]
    "5b"               # pop  ebx
    "c3")              # ret


def test_saved_register_and_esp_relative_argument():
    _assert_equivalent(SAVED_EBX_AND_ESP_ARGUMENT)
    c = decompile(SAVED_EBX_AND_ESP_ARGUMENT)
    head = c.split("{")[0]
    assert "bx" not in head, "saving EBX must not make it a parameter:\n" + c
    assert "sp" not in c.replace("sub_", ""), "the argument must not be read through a raw sp:\n" + c


# A value pushed and popped back into another register.
PUSH_THEN_POP = bytes.fromhex(
    "ff742404"         # push dword [esp+4]
    "58"               # pop  eax
    "83c005"           # add  eax, 5
    "c3")              # ret


def test_push_then_pop_moves_the_value():
    _assert_equivalent(PUSH_THEN_POP)


# AH is the second byte of EAX, not a register of its own (and certainly not SP).
HIGH_BYTE_REGISTER = bytes.fromhex(
    "8b442404"         # mov  eax, [esp+4]
    "b412"             # mov  ah, 0x12
    "c3")              # ret


def test_high_byte_register():
    _assert_equivalent(HIGH_BYTE_REGISTER)


def main():
    for fn in (test_shared_ret_reached_by_jmp_keeps_both_values,
               test_shared_ret_reached_by_jcc_keeps_both_values,
               test_ret_block_with_its_own_value_is_not_split,
               test_join_of_two_register_values,
               test_local_set_on_both_arms,
               test_loop_carried_registers,
               test_copy_of_loop_carried_value,
               test_call_arguments_are_in_order,
               test_indexed_address,
               test_saved_register_and_esp_relative_argument,
               test_push_then_pop_moves_the_value,
               test_high_byte_register,
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
