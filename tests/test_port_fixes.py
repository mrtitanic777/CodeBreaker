#!/usr/bin/env python3
"""Regression tests for bugs the original Python carried (found while it was ported to typed C++, and fixed
in the Python and in the port together).

    python tests/test_port_fixes.py
"""
import os
import sys
import types
import warnings

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
warnings.simplefilter("ignore")

RESULTS = []


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def test_pe_writer_nosplit_keeps_the_scaled_index():
    """source.pe_writer: a scaled index in a memory operand is written `[nosplit ecx*4 ...]`; the replacement
    string once held control characters where the group references belong, so NASM refused the instruction."""
    for name in ("pe_writer", "le_writer"):       # (the source text: no control characters; the groups are written \1 and \2)
        text = open(os.path.join(ROOT, "codebreaker", "source", name + ".py"), encoding="utf-8").read()
        assert not [c for c in text if ord(c) < 0x20 and c not in "\r\n\t"], name
        assert 'r"[\\1nosplit \\2"' in text, name
    try:
        from codebreaker.source import pe_writer
    except ImportError:                          # (the image comparer it imports now lives only in the typed port)
        return
    w = pe_writer.Writer.__new__(pe_writer.Writer)
    w.sites = {}
    i = types.SimpleNamespace(address=0x401000, prefix=[], mnemonic="mov", op_str="eax, dword ptr [ecx*4 + 0x10]",
                              bytes=b"\x8b\x04\x8d\x10\x00\x00\x00", imm_size=0, imm_offset=0, disp_size=0, disp_offset=0)
    got = w._nasm(i, None, [])
    assert got == "mov eax, dword [nosplit ecx*4 + 0x10]", repr(got)
    i.op_str = "eax, dword ptr fs:[edx*8]"
    got = w._nasm(i, None, [])
    assert got == "mov eax, dword [fs:nosplit edx*8]", repr(got)
    assert all(ord(c) >= 0x20 for c in got)


def test_cpukey_generated_keys_validate():
    """formats.xbox.crypto.cpukey_ecd: the Hamming weight leaves out the bits the ECD fills (106-127 in the
    order compute_ecd walks them), so a generated key is valid; it once counted other bits and about half the
    generated keys failed validate_cpukey. XeCrypt's CpuKeyValid is the reference."""
    from codebreaker.formats.xbox.crypto import cpukey_ecd, XeCrypt as xc
    for _ in range(100):
        k = cpukey_ecd.generate_cpukey()
        r = cpukey_ecd.validate_cpukey(k)
        assert r["valid"] and r["hamming_ok"] and r["ecd_ok"], (k, r)
        assert xc.XeCryptCpuKeyValid(bytes.fromhex(k)), k
    for _ in range(20):
        k = xc.XeCryptCpuKeyGen()
        assert cpukey_ecd.validate_cpukey(k.hex())["valid"], k.hex()
        assert cpukey_ecd.verify_hamming_weight(k)
    # the ECD fill never moves the weight
    base = bytearray(16)
    for q in range(0, 106, 2):
        base[q >> 3] |= 1 << (q & 7)
    assert cpukey_ecd.compute_hamming_weight(bytes(base)) == 53
    assert cpukey_ecd.compute_hamming_weight(bytes(cpukey_ecd.compute_ecd(base))) == 53
    assert cpukey_ecd.validate_cpukey(cpukey_ecd.compute_ecd(base).hex())["valid"]


def test_sms_writer_game_gear_rom():
    """source.sms_writer: a Game Gear cartridge is written (the Game Gear's ports were merged into the console's
    with dict(PORTS, **GG_PORTS), whose integer keys raise TypeError), and names its own ports."""
    import tempfile
    from codebreaker.build import z80_asm
    from codebreaker.source import sms_writer
    _at, image = z80_asm.build_flat("        DI\n        OUT ($06),A\n        IN A,($00)\n        OUT ($BF),A\n        JR $\n")
    work = tempfile.mkdtemp(prefix="cb_gg_")
    path = os.path.join(work, "t.gg")
    with open(path, "wb") as f:
        f.write(bytes(image) + bytes(0x4000 - len(image)))
    r = sms_writer.reconstruct(path, work)
    text = open(r["source"]).read()
    assert "Game Gear" in text and "GG_STEREO" in text and "GG_START" in text, text[:600]


def test_sd_signer_keeps_its_key_in_the_module():
    """formats.xbox.crypto.sd_signer: main() loads the signing key into the module's SB_PRV_KEY (it assigned a
    local of that name, and the module's stayed None). The key loader and the signer are stood in for here."""
    import contextlib
    import io
    import tempfile
    from codebreaker.formats.xbox.crypto import sd_signer
    key = object()
    seen = []
    real = sd_signer.load_and_verify_sb_prv, sd_signer.sign_sd_4bl
    sd_signer.load_and_verify_sb_prv = lambda: key
    sd_signer.sign_sd_4bl = lambda k, salt, data: seen.append(k) or data
    work = tempfile.mkdtemp(prefix="cb_sd_")
    path = os.path.join(work, "sd.bin")
    with open(path, "wb") as f:
        f.write(b"SD" + bytes(0x1E))
    argv = sys.argv
    sys.argv = ["sd_signer.py", path]
    try:
        assert sd_signer.SB_PRV_KEY is None
        with contextlib.redirect_stdout(io.StringIO()):
            sd_signer.main()
        assert sd_signer.SB_PRV_KEY is key and seen == [key]
    finally:
        sys.argv = argv
        sd_signer.load_and_verify_sb_prv, sd_signer.sign_sd_4bl = real
        sd_signer.SB_PRV_KEY = None


def main():
    for fn in (test_pe_writer_nosplit_keeps_the_scaled_index, test_cpukey_generated_keys_validate,
               test_sms_writer_game_gear_rom, test_sd_signer_keeps_its_key_in_the_module):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d failed" % (sum(1 for r in RESULTS if r[1] == "ok"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
