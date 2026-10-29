#!/usr/bin/env python3
"""Tests for the one run record (emulate/run.py) every emulator's run slot returns, and for the
lanes' generic comparing of it (verify/lane_common.py fingerprint, left, changed, calls).
Run:  python tests/test_run_record.py     (exit code 0 = all passed)

The record's rules are checked on made-up writes; every platform's runner on a run of its own
(real modules where the corpus has them, a few bytes of code for the machines); and the memory
digests the 32-bit and 16-bit judges now make from the record against the ones their own code
made from each emulator's record before (kept here as the reference)."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)
os.environ["CB_NO_FP_CACHE"] = "1"

RESULTS = []


class Skip(Exception):
    pass


FIELDS = ("exit", "ended", "results", "writes", "capped", "calls", "io", "stack", "preserved", "path", "steps",
          "truncated", "endian", "raw")


def _is_run(r):
    from codebreaker.emulate.run import Run
    return isinstance(r, Run) and all(hasattr(r, f) for f in FIELDS)


# --- the record's rules -------------------------------------------------------------------------
def test_what_a_run_left_changed():
    """A byte compared is what the last write left, against what the first write found there; a
    byte put back as it was is no effect; a harness write only says what was there; a place left
    out is not compared; a 4-byte value can stand for something else in each of its bytes."""
    from codebreaker.emulate.run import Run, Write
    from codebreaker.verify import lane_common
    r = Run("ret", writes=[Write(0x100, 2, 0x1234, 0x0000, False), Write(0x101, 1, 0x00, 0x12, False),
                           Write(0x200, 1, 0x55, 0x55, False), Write(0x300, 4, 0x300004, 0, False),
                           Write(0x400, 1, 0x7, 0x1, True), Write(0x400, 1, 0x9, 0x7, False),
                           Write(0x500, 1, 0x1, 0x0, False)])
    got = lane_common.changed(r, place=lambda a: None if a == 0x500 else a,
                              word=lambda v: "S" if 0x300000 <= v < 0x340000 else None)
    assert got == {0x100: 0x34, 0x300: "S", 0x301: "S", 0x302: "S", 0x303: "S", 0x400: 0x9}, got
    # (the first kept write's view: 0x400's first kept write found 7 there)
    got = lane_common.changed(r, everything=False)
    assert got[0x400] == 0x9 and 0x200 not in got
    assert lane_common.changed(Run("ret", writes=[Write(0x10, 1, 1, 1, False)])) == {}
    # places named by a key, the bytes after it following; and big-endian values
    tagged = lane_common.changed(r, place=lambda a: ("D", a - 0x100) if a < 0x200 else None)
    assert tagged == {("D", 0): 0x34}, tagged
    big = Run("ret", writes=[Write(0x10, 2, 0xA1B2, 0, False)], endian="big")
    assert lane_common.changed(big) == {0x10: 0xA1, 0x11: 0xB2}
    assert lane_common.left(Run("ret", capped=True)) is None and lane_common.left(Run("ret")) == ""


def test_a_fingerprint_from_a_record():
    """The stack's move and the saved registers count only for a run that returned; a lane's
    own fields are carried along."""
    from codebreaker.emulate.run import Call, Run
    from codebreaker.verify import lane_common
    done = Run("ret", stack=8, preserved={"EBX": (1, 1), "ESI": (2, 2)}, calls=[Call("K.F", [1, 2], None)])
    fp = lane_common.fingerprint(done, lane_common.calls(done.calls, str), ret=3, saved=done.kept(), extra=1)
    assert fp == {"ret": 3, "exit": "ret", "truncated": False, "deps": ("K.F(1,2)",), "writes": "", "pop": 8,
                  "saved": True, "cov": (), "reach": (), "extra": 1}, fp
    fault = Run("exception: x", stack=8, preserved={"EBX": (1, 2)})
    fp = lane_common.fingerprint(fault, (), saved=fault.kept())
    assert fp["exit"] == "fault" and fp["pop"] is None and fp["saved"] is None
    assert not fault.kept() and Run("ret").kept() is False and done.kept(("EBX",))


# --- every runner -------------------------------------------------------------------------------
def test_every_runner_returns_the_record():
    """Each platform's emulator.run gives a Run: the 16-bit and 32-bit tracers on LZEXPAND and LZ32,
    the PowerPC interpreter and the four machines on a few bytes of their code."""
    from codebreaker import platforms
    from codebreaker.base import corpus
    checked = []
    lz = corpus.retail("lzexpand.dll")
    if lz:
        e = [e for e in platforms.of("ne").loader.open(lz).entries() if e.get("seg")][1]
        r = platforms.of("ne").emulator.run(lz, seg=e["seg"], offset=e["offset"], args=[0] * 8)
        assert _is_run(r) and r.ended in ("ret", "fault", "truncated") and "AX" in r.results and r.steps
        assert set(r.preserved) == {"SI", "DI", "DS"}, r.preserved
        checked.append("ne")
    lz32 = corpus.retail("lz32.dll")
    if lz32:
        from codebreaker.formats.pe import open_pe
        pe = open_pe(lz32)
        va = pe.image_base + pe.exports()[0][2]
        r = platforms.of("pe").emulator.run(lz32, func=va, args=[0] * 4, stub_externals=True)
        assert _is_run(r) and "EAX" in r.results and r.path and set(r.preserved or {}) <= {"EBX", "ESI", "EDI", "EBP"}
        checked.append("pe")
    code = bytes.fromhex("38600005" "4E800020")                  # li r3, 5 / blr
    r = platforms.of("powerpc").emulator.run([(0x82000000, code)], 0x82000000, [0])
    assert _is_run(r) and r.ended == "ret" and r.results["r3"] == 5 and r.kept() and r.endian == "big"
    from codebreaker.emulate import m6502, m68k, sm83, z80
    mem = bytearray(0x10000)
    mem[0x8000:0x8003] = bytes.fromhex("A90560")                 # LDA #5 / RTS
    r = platforms.of("nes").emulator.run(m6502.Bus(mem), 0x8000)
    assert _is_run(r) and r.exit == "return" and r.results["A"] == 5
    for kind, mod in (("sms", z80), ("gb", sm83)):
        mem = bytearray(0x10000)
        mem[0x100:0x103] = bytes.fromhex("3E05C9")               # LD A, 5 / RET
        r = platforms.of(kind).emulator.run(mod.Bus(mem), 0x100)
        assert _is_run(r) and r.exit == "return" and r.results["A"] == 5, (kind, r)
    mem = bytearray(0x1000000)
    mem[0x1000:0x1004] = bytes.fromhex("70054E75")               # MOVEQ #5, D0 / RTS
    r = platforms.of("genesis").emulator.run(m68k.Bus(mem), 0x1000)
    assert _is_run(r) and r.exit == "return" and r.results["D0"] == 5
    checked += ["powerpc", "nes", "sms", "gb", "genesis"]
    vxd = corpus.retail("splitter.vxd")
    if vxd:
        from codebreaker.verify.lanes import driver
        image = driver.load(vxd)
        label = sorted(image.entries)[0]
        r = platforms.of("le").emulator.run(image, image.entries[label], {"eax": 0})
        assert _is_run(r) and "eax" in r.results and r.steps is not None
        checked.append("le")
    assert len(checked) >= 5, checked


# --- the judges' digests, as their own code made them ------------------------------------------
def _old_effects32(r, own):
    """verify/lanes/win32.py _effects and _effects_lean before the run record (the reference)."""
    from codebreaker.emulate import emulator as tracer
    from codebreaker.verify import lane_common
    lo, hi = tracer._STACK_BASE, tracer._STACK_BASE + tracer._STACK_SIZE
    ws = r.get("mem_writes") or []
    if r.get("writes_seen", len(ws)) >= tracer._MAX_MEM:
        return None
    mn = r.get("mnemonics") or {}
    if r.get("pages0") is not None:
        latest = {}
        for i, (a, n, v, pc) in enumerate(ws):
            if pc and mn.get("0x%x" % pc) == "add byte ptr [eax], al":
                continue
            if own and own[0] <= a < own[1]:
                continue
            latest[(a, n)] = (i, v)
        state = {}
        for (a, n), (_i, v) in sorted(latest.items(), key=lambda kv: kv[1][0]):
            if n == 4 and (lo <= v < hi or (own and own[0] <= v < own[1])):
                for i in range(4):
                    state[a + i] = "S"
            else:
                for i in range(min(n, 8)):
                    state[a + i] = (v >> (8 * i)) & 0xFF
        pages = r["pages0"]
        return lane_common.writes({k: x for k, x in state.items() if x != pages[k & ~0xFFF][k & 0xFFF]})
    state, before = {}, {}
    for w in ws:
        a, n, v = w["addr"], w["size"], w["value"] or 0
        for i in range(min(n, 8)):
            before.setdefault(a + i, ((w.get("old") or 0) >> (8 * i)) & 0xFF if "old" in w else None)
        if mn.get("0x%x" % w["pc"]) == "add byte ptr [eax], al":
            continue
        if lo <= a < hi or (own and own[0] <= a < own[1]):
            continue
        if n == 4 and (lo <= v < hi or (own and own[0] <= v < own[1])):
            for i in range(4):
                state[a + i] = "S"
        else:
            for i in range(min(n, 8)):
                state[a + i] = (v >> (8 * i)) & 0xFF
    return lane_common.writes({k: x for k, x in state.items() if x != before.get(k)})


def test_the_32_bit_judge_s_memory_digest_is_as_it_was():
    """LZ32's exports on the judge's inputs, traced lean and plain: the digest made from the run
    record is the one the judge's own code made from the tracer's record."""
    from codebreaker.base import corpus
    from codebreaker.emulate import emulator
    from codebreaker.verify.lanes import win32
    path = corpus.retail("lz32.dll")
    if not path:
        raise Skip("needs lz32.dll in the corpus")
    from codebreaker.formats.pe import open_pe
    pe = open_pe(path)
    n = 0
    for _o, _name, rva in pe.exports():
        for v in list(win32._vectors(4, "pe")) + list(win32._confirm_vectors(4, "pe")):
            for lean in (True, False):
                raw = emulator.trace_function(path, func=pe.image_base + rva, args=v["args"], mem_init=v.get("mem_init"),
                                              stub_externals=True, stub_ret=int(v.get("stub_ret", 0)), reads=False,
                                              quiet_stack=True, lean=lean, fast=lean, reuse=lean, pads=win32._PADS,
                                              sig_resolver=win32._argc)
                for own in (None, (pe.image_base, pe.image_base + 0x1000)):
                    assert win32._effects(emulator.record(raw), own) == _old_effects32(raw, own), (rva, v["args"], lean)
                n += 1
    assert n > 20


def _old_writes16(r, ds, own, heap, stride):
    from codebreaker.verify import lane_common
    state, before = {}, {}
    for w in r.get("mem_writes") or []:
        a, n, val = w["addr"], w["size"], w["value"] or 0
        if ds is not None and ds <= a < ds + 0x10000:
            if any(lo <= a - ds < hi for lo, hi in own):
                continue
            where = ("D", a - ds)
        elif heap <= a < heap + 64 * stride:
            where = ("H", a - heap)
        else:
            continue
        for i in range(min(n, 4)):
            k = (where[0], where[1] + i)
            before.setdefault(k, ((w.get("old") or 0) >> (8 * i)) & 0xFF)
            state[k] = (val >> (8 * i)) & 0xFF
    return lane_common.writes({k: x for k, x in state.items() if x != before.get(k)})


def test_the_16_bit_judge_s_memory_digest_is_as_it_was():
    """LZEXPAND's exports on the judge's inputs: the fingerprint's writes made from the run record
    are the ones the judge's own code made from probe's record (data segment and heap; the
    places a build keeps for itself left out)."""
    from codebreaker.base import corpus
    from codebreaker.emulate import windows16 as emu
    from codebreaker.verify.lanes import win16
    path = corpus.retail("lzexpand.dll")
    if not path:
        raise Skip("needs lzexpand.dll in the corpus")
    entries = [e for e in win16.P.loader.open(path).entries() if e.get("seg")][:10]
    n = 0
    for e in entries:
        for v in win16.vectors():
            raw = emu.probe(path, seg=e["seg"], offset=e["offset"], args=v["args"], stub_ret=v.get("stub_ret"),
                            mem_init=v.get("mem"), ports=True)
            ds = (raw.get("layout") or {}).get("ds_base")
            for own in ((), ((0x10, 0x40),)):
                fp = win16.fingerprint(path, e["seg"], e["offset"], False, False, v, own=own, ran=emu.record(raw))
                assert fp["writes"] == _old_writes16(raw, ds, own, emu.HEAP_BASE, emu.HEAP_STRIDE), (e, v)
            n += 1
    assert n > 20


def main():
    tests = [(n, f) for n, f in sorted(globals().items()) if n.startswith("test_") and callable(f)]
    failed = 0
    for name, fn in tests:
        try:
            fn()
            RESULTS.append(("PASS", name))
        except Skip as e:
            RESULTS.append(("SKIP", "%s (%s)" % (name, e)))
        except Exception as e:
            import traceback
            traceback.print_exc()
            RESULTS.append(("FAIL", "%s: %s" % (name, e)))
            failed += 1
    for status, what in RESULTS:
        print("%s  %s" % (status, what))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
