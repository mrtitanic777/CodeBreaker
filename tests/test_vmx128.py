#!/usr/bin/env python3
"""VMX128, the Xbox 360's vector extension: the decode table (codebreaker/decompile/vmx128.py) and
the decompiler's language built from it (languages/ppc_64_xenon_be.sla, PowerPC:BE:64:Xenon).

    python tests/test_vmx128.py

Checks that every row decodes to its own mnemonic; that the SLEIGH in the tree is what the table
generates now; that the compiled language disassembles every VMX128 word exactly as the table
does (each row with random operand bits, and every vector word of a real game when one is here);
and that every other word reads as it did in the language without VMX128 (or, for an instruction
the Xenon does not have -- the Power ISA 2.07/3.0 additions -- not at all). The language checks
need native/decompiler/build/bin/pcodecheck; without it they are SKIPPED."""
import os
import random
import re
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.decompile import vmx128      # noqa: E402

LANG = os.path.join(ROOT, "native", "decompiler", "languages")
TOOL = os.path.join(ROOT, "native", "decompiler", "build", "bin", "pcodecheck" + (".exe" if os.name == "nt" else ""))
GAME = os.path.join(ROOT, "Projects", "MinecraftPC", "default.xex")
RESULTS = []


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def disassemble(sla, words):
    """{word: 'mnemonic operands'} through the compiled language."""
    if not os.path.isfile(TOOL):
        raise Skip("pcodecheck not built")
    p = subprocess.run([TOOL, os.path.join(LANG, sla), "ppc-be-64-asm"], capture_output=True, text=True,
                       input="".join("%08x\n" % w for w in words))
    out = {}
    for line in p.stdout.splitlines():
        w, _, text = line.partition(" ")
        out[int(w, 16)] = text.strip()
    return out


def same(table_text, sleigh_text):
    """The table and the language agree, allowing for how each writes numbers (SLEIGH: hex)."""
    m, _, ops = sleigh_text.partition(" ")
    ops = re.sub(r"-?0x[0-9a-f]+", lambda x: str(int(x.group(0), 16)), ops)
    tm, _, tops = table_text.partition(" ")
    return tm == m and tops.replace(" ", "") == ops.replace(" ", "")


def test_rows_decode_to_themselves():
    bad = [m for m, p, _mask, _f, _s in vmx128.ROWS if (vmx128.decode(p) or ("",))[0] != m]
    assert not bad, bad
    assert vmx128.text(0x102320C3) == "lvx128 v1,r3,r4"       # (llvm-mc: vucmprlb 1,3,4)


def test_sleigh_is_current():
    path = os.path.join(LANG, "src", "ppc", "vmx128.sinc")
    with tempfile.TemporaryDirectory() as d:
        for f in os.listdir(os.path.dirname(path)):
            if f.endswith(".sinc") and f != "vmx128.sinc":
                open(os.path.join(d, f), "w").write(open(os.path.join(os.path.dirname(path), f)).read())
        out = os.path.join(d, "vmx128.sinc")
        vmx128.main(["--sleigh", out])
        assert open(out).read() == open(path).read(), "vmx128.sinc is stale: regenerate it (--sleigh)"


def test_language_matches_table():
    rnd = random.Random(128)
    words = []
    for _m, pat, mask, _f, _s in vmx128.ROWS:
        words.append(pat)
        words += [pat | (rnd.getrandbits(32) & ~mask) for _ in range(60)]
    got = disassemble("ppc_64_xenon_be.sla", words)
    bad = ["%08X %s / %s" % (w, vmx128.text(w), got.get(w)) for w in words if not same(vmx128.text(w), got.get(w, ""))]
    assert not bad, "%d of %d differ, e.g. %s" % (len(bad), len(words), bad[:4])


def _game_vector_words():
    if not os.path.isfile(GAME):
        raise Skip("no game image here")
    from codebreaker.formats import powerpc_image, xex
    img = powerpc_image.read(xex.basefile(GAME))
    return sorted({w for va in range(img.code[0], img.code[1], 4)
                   for w in [img.word(va)] if w is not None and (w >> 26) in (4, 5, 6)})


def test_x31_forms():
    """The Xenon's opcode-31 unaligned vector loads and stores: decoded, and as the language reads them."""
    words = [(31 << 26) | (5 << 21) | (ra << 16) | (4 << 11) | (xo << 1) for _m, xo, _f, _s in vmx128.X31
             for ra in (0, 3)]
    got = disassemble("ppc_64_xenon_be.sla", words)
    bad = ["%08X %s / %s" % (w, vmx128.text(w), got.get(w)) for w in words if not same(vmx128.text(w), got.get(w, ""))]
    assert not bad, bad[:4]
    assert vmx128.text(0x7D804C0E) == "lvlx v12,0,r9"


def test_game_vmx128_words():
    words = [w for w in _game_vector_words() if vmx128.decode(w)]
    got = disassemble("ppc_64_xenon_be.sla", words)
    bad = ["%08X %s / %s" % (w, vmx128.text(w), got.get(w)) for w in words if not same(vmx128.text(w), got.get(w, ""))]
    assert not bad, "%d of %d differ, e.g. %s" % (len(bad), len(words), bad[:4])


def _left_out():
    """The mnemonics the Xbox 360 language leaves out (the Power ISA 2.07/3.0 additions, inside
    `@ifndef XENON` in the PowerPC .sinc files)."""
    out = set()
    src = os.path.join(LANG, "src", "ppc")
    for f in os.listdir(src):
        inside = False
        for line in open(os.path.join(src, f), encoding="utf-8"):
            if line.startswith("@ifndef XENON"):
                inside = True
            elif line.startswith("@endif # XENON"):
                inside = False
            elif inside and line.startswith(":"):
                out.add(line[1:].split()[0])
    return out


def test_other_words_unchanged():
    rnd = random.Random(4)
    words = [w for w in _game_vector_words() if not vmx128.decode(w)]
    words += [w for w in ((op << 26) | rnd.getrandbits(26) for op in (4, 5, 6) for _ in range(3000))
              if not vmx128.decode(w)]
    new, old = disassemble("ppc_64_xenon_be.sla", words), disassemble("ppc_64_isa_altivec_be.sla", words)
    gone = _left_out()
    bad = ["%08X %s / %s" % (w, old.get(w), new.get(w)) for w in words if old.get(w) != new.get(w) and
           not (new.get(w, "").startswith("err") and old.get(w, "").split()[0] in gone)]
    assert not bad, "%d of %d differ, e.g. %s" % (len(bad), len(words), bad[:4])


def main():
    for fn in (test_rows_decode_to_themselves, test_sleigh_is_current, test_language_matches_table,
               test_x31_forms, test_game_vmx128_words, test_other_words_unchanged):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "ok"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
