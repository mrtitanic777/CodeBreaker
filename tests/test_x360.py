"""The Xbox 360 project lane: XenonRecomp configured by CodeBreaker, on the native/x360 runtime.

    python -m pytest tests/test_x360.py
    CB_SLOW_TESTS=1 python -m pytest tests/test_x360.py      # also Xenia's instruction tests (~15 min)

Uses Minecraft's image from the absorbed projects' folder when it is there (skipped otherwise).
"""
import os
import re
import struct
import subprocess
import sys
import tempfile

import pytest

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.recompile import config, discover, switches, xenon    # noqa: E402

def _converted():
    """Converted/ (never committed): next to tests/, or in the main checkout when this is a worktree."""
    d = ROOT
    while True:
        c = os.path.join(d, "Converted")
        if os.path.isdir(c):
            return c
        if os.path.dirname(d) == d:
            return os.path.join(ROOT, "Converted")
        d = os.path.dirname(d)


XEX = os.path.join(_converted(), "Minecraft", "original", "default.xex")   # (Converted/: never committed)


def _need_game():
    if not os.path.isfile(XEX):
        pytest.skip("no Minecraft XEX here")


def test_helpers_and_jump_buffers():
    """The save/restore runs (the eight XenonRecomp takes) and the CRT's setjmp/longjmp."""
    _need_game()
    found = config.helpers(discover._image(XEX))
    assert found == {
        "savegprlr_14_address": 0x82685810, "restgprlr_14_address": 0x82685860,
        "savefpr_14_address": 0x82686770, "restfpr_14_address": 0x826867BC,
        "savevmx_14_address": 0x82688D60, "restvmx_14_address": 0x82688FF8,
        "savevmx_64_address": 0x82688DF4, "restvmx_64_address": 0x8268908C,
        "setjmp_address": 0x8268DF40, "longjmp_address": 0x8268DC20}, found


class _Img:
    def __init__(self, lo, code, extents):
        self.code = (lo, lo + len(code))
        self.extents = extents


def _words(ws):
    return b"".join(struct.pack(">I", w) for w in ws)


def test_setjmp_through_a_register_other_than_r3():
    """setjmp that stores r1 at 0x90 of a buffer reached through r4 (not r3) is still found, so a
    longjmp below it is never emitted alone (the XAM / Guide.TFA system-module crash)."""
    lo = 0x82000000
    code = _words([0, 0xF8240090, 0x4E800020,          # std r1,0x90(r4); blr  -> setjmp at lo+4
                   0, 0x80A60134, 0x80E60090, 0x4E800020])  # lwz r5,0x134(r6); lwz r7,0x90(r6) -> longjmp at lo+16
    out = config.jump_buffers(_Img(lo, code, {lo + 16: (16, 0, 0)}), code)
    assert out == {"setjmp_address": lo + 4, "longjmp_address": lo + 16}, out


def test_longjmp_without_setjmp_emits_neither():
    """A longjmp with no setjmp found is the silent rip=0 crash, so neither address is emitted."""
    lo = 0x82000000
    code = _words([0, 0x80A60134, 0x80E60090, 0x4E800020])   # longjmp pattern, no `std r1,0x90(rA)`
    out = config.jump_buffers(_Img(lo, code, {lo + 4: (12, 0, 0)}), code)
    assert out == {}, out


def test_switch_tables():
    """switches.py finds the halfword table XenonAnalyse misses (no bounds check right before it,
    the index shifted into r0), and no table's labels leave their function."""
    _need_game()
    found, report = switches.find(XEX)
    tables = {t["bctr"]: t for t in found}
    assert tables[0x822EC698]["kind"] == "short" and len(tables[0x822EC698]["labels"]) == 19
    assert tables[0x82310674]["kind"] == "short" and len(tables[0x82310674]["labels"]) == 127
    assert len(found) == 213 and not report["outside"]


def test_extra_functions_respect_switches():
    """A switch's labels are never function starts, and nothing inside a save/restore run is one."""
    _need_game()
    img = discover._image(XEX)
    found, _ = switches.find(XEX)
    tables = {t["bctr"]: t["labels"] for t in found}
    extra = dict(config.extra_functions(XEX, img, tables))
    labels = {l for ls in tables.values() for l in ls}
    assert not labels & set(extra)
    assert 0x82688FD4 not in extra and 0x8268926C not in extra     # entries of the vector runs


def test_tail_jump_targets_are_functions():
    """A branch out of a function is a tail call, so its target is declared a function: the functions at
    0x821F36B8 and 0x821F3948 tail-call `b 0x821F0B90` / `b 0x822007F8`, which XenonRecomp otherwise had nothing to call
    for (and dropped)."""
    _need_game()
    img = discover._image(XEX)
    found, _ = switches.find(XEX)
    extra = dict(config.extra_functions(XEX, img, {t["bctr"]: t["labels"] for t in found}))
    assert 0x821F0B90 in extra and 0x822007F8 in extra


def test_basic_xex():
    """basic(): the header kept, one data block holding the whole image."""
    _need_game()
    from codebreaker.formats import xex
    with tempfile.TemporaryDirectory() as d:
        out = xex.basic(XEX, os.path.join(d, "basic.xex"))
        data = open(out, "rb").read()
        original = open(XEX, "rb").read()
        pe_offset, = struct.unpack(">I", data[8:12])
        assert data[:4] == b"XEX2" and data[pe_offset:pe_offset + 2] == b"MZ"
        count, = struct.unpack(">I", data[0x14:0x18])
        info = next(v for k, v in (struct.unpack(">II", data[0x18 + i * 8:0x20 + i * 8]) for i in range(count))
                    if k == 0x3FF)
        _size, encryption, compression, data_size, zeros = struct.unpack(">IHHII", data[info:info + 16])
        assert (encryption, compression, zeros) == (0, 1, 0) and data_size == len(data) - pe_offset
        assert data[0x18:info] == original[0x18:info]


def build_read_image_test(build_dir):
    """native/x360/runtime/test built with clang-cl into build_dir -> the program (xex_image_test.exe)."""
    env, cc = xenon.msvc_env(), xenon.clang_cl()
    for cmd in (["cmake", "-S", os.path.join(ROOT, "native", "x360", "runtime", "test"), "-B", build_dir, "-G", "Ninja",
                 "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_CXX_COMPILER=" + cc], ["cmake", "--build", build_dir, "-j", "4"]):
        r = subprocess.run(cmd, capture_output=True, text=True, env=env)
        assert r.returncode == 0, (r.stdout + r.stderr)[-3000:]
    return os.path.join(build_dir, "xex_image_test.exe")


def read_image_problems(program, path, work):
    """How the runtime's xex::ReadImage of `path` (the program built above) differs from the xex engine's basefile of
    it, at the base address its header gives; [] when it does not. -> (problems, resources it read)"""
    from codebreaker.formats import xex
    out = os.path.join(work, "image.bin")
    r = subprocess.run([program, path, out], capture_output=True, text=True)
    if r.returncode:
        return ["ReadImage failed: " + (r.stderr or r.stdout).strip()[-300:]], []
    lines = r.stdout.splitlines()
    base, size = lines[0].split()[1], int(lines[0].split()[3])
    resources = [l.split()[1:] for l in lines[1:]]
    problems = []
    with open(xex.basefile(path), "rb") as f:
        want = f.read()
    with open(out, "rb") as f:
        got = f.read()
    if got != want or size != len(want):
        problems.append("image differs from the basefile (%d bytes, basefile %d)" % (len(got), len(want)))
    header = xex.header(path)
    if header.base is not None and int(base, 16) != header.base:      # (none for a module of resources alone)
        problems.append("base %s, header %08X" % (base, header.base))
    problems += ["resource %s outside the image" % r[0] for r in resources if r[3] != "inside"]
    # the resources as the xex engine lists them, under "Resources": "0) 90F90000 - 90FA284B : skin" (start, end)
    listing = subprocess.run([xex.program(), "list", path], capture_output=True, text=True).stdout
    section = re.search(r"(?m)^Resources\n((?:[ \t]+.*\n?)*)", listing)
    listed = sorted((m.group(3), int(m.group(1), 16), int(m.group(2), 16) - int(m.group(1), 16))
                    for m in re.finditer(r"(?m)^\s+\d+\)\s+([0-9A-F]{8}) - ([0-9A-F]{8}) : (\S+)$",
                                         section.group(1) if section else ""))
    read = sorted((r[0], int(r[1], 16), int(r[2], 16)) for r in resources)
    if read != listed:
        problems.append("resources %s, listed %s" % (read[:4], listed[:4]))
    return problems, resources


@pytest.mark.skipif(not os.environ.get("CB_SLOW_TESTS"), reason="slow: set CB_SLOW_TESTS=1")
def test_runtime_read_image():
    """The runtime's xex::ReadImage (src/xex.cpp, through the shared XEX reader; the path XAM's data reads take) gives
    the xex engine's basefile of a real XEX, at its base. Also XAM's own modules (xam.xex, huduiskin.xex) when
    CB_X360_CONVERTED names the folder holding them (the dashboard's SystemUpdate/converted)."""
    _need_game()
    try:
        xenon.clang_cl()
    except RuntimeError as e:
        pytest.skip(str(e))
    modules = [XEX]
    folder = os.environ.get("CB_X360_CONVERTED")
    if folder:
        modules += [os.path.join(folder, n) for n in ("xam.xex", "huduiskin.xex")]
    with tempfile.TemporaryDirectory() as d:
        program = build_read_image_test(os.path.join(d, "build"))
        for path in modules:
            problems, _resources = read_image_problems(program, path, d)
            assert not problems, (path, problems)


@pytest.mark.skipif(not os.environ.get("CB_SLOW_TESTS"), reason="slow: set CB_SLOW_TESTS=1")
def test_instruction_tests():
    """Xenia's PowerPC instruction tests through XenonRecomp: no mismatch."""
    try:
        result = xenon.instruction_tests()
    except RuntimeError as e:
        pytest.skip(str(e))
    assert result["tested"] >= 150 and not result["mismatches"], result["mismatches"][:10]
