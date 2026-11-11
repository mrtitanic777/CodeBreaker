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


def test_streamio_offset_labels_and_perform_function_at():
    """formats.xbox.crypto.StreamIO: `offset` is the position (xcp_dumper and build_lib assign and step it),
    labels name positions (shadowboot.create), perform_function_at changes bytes in place; the stand-in StreamIO
    lacked all of them, so xcp_dumper.main, apply_jump_sd_4bl and ShadowbootImage.create always failed."""
    from codebreaker.formats.xbox.crypto.StreamIO import Endian, StreamIO
    with StreamIO(bytes(range(16)), Endian.BIG) as s:
        s.offset = 4
        assert s.offset == s.tell() == 4 and s.read_uint8() == 4 and s.offset == 5
        s.offset -= 2
        assert s.tell() == 3
        s.set_label("a")
        s.seek(10)
        s.set_label("b")
        s.goto_label("a")
        assert s.tell() == 3 and s.get_label("b") == 10
        s.goto_label("b", 2)
        assert s.tell() == 12
        s.set_label("c", 99)
        assert s.get_label("c") == 99
        try:
            s.get_label("nope")
        except KeyError:
            pass
        else:
            raise AssertionError("a label never set")
        s.seek(1)
        out = s.perform_function_at(4, 3, lambda d: bytes(b ^ 0xFF for b in d))
        assert out == bytes([0xFB, 0xFA, 0xF9]) and s.tell() == 1
        assert s.getvalue()[4:7] == out and s.getvalue()[7:] == bytes(range(7, 16))


def test_apply_jump_sd_4bl_writes_the_branch():
    """formats.xbox.crypto.build_lib.apply_jump_sd_4bl: the 0x4C000024 word becomes a relative branch to `size`
    (it raised AttributeError on StreamIO.offset)."""
    import struct
    from codebreaker.formats.xbox.crypto import build_lib
    data = b"\x01\x02\x03\x04" * 3 + struct.pack(">I", 0x4C000024) + b"\xAA" * 4
    out = build_lib.apply_jump_sd_4bl(data, 0x100)
    assert bytes(out) == data[:12] + struct.pack(">I", 0x48000000 | (0x100 - 12)) + b"\xAA" * 4, bytes(out).hex()
    out = build_lib.apply_jump_sd_4bl(data, 0x8000000C)       # (bit 31 is left out of the distance)
    assert struct.unpack_from(">I", out, 12)[0] == 0x48000000


def test_xsm3_challenge_verify_answers_a_valid_packet():
    """formats.xbox.crypto.xsm3: xsm3_do_challenge_verify, past a valid MAC, calculates the ACR from the random
    console data (key), the console id and the identification data (it passed the 8 bytes of random data as the
    identification data, and the ACR raised IndexError)."""
    import random
    from codebreaker.formats.xbox.crypto import xsm3 as M
    k1, k2 = bytes(range(16)), bytes(range(16, 32))
    M.STATIC_KEY_1, M.STATIC_KEY_2 = k1, k2
    plain = bytes(range(40, 64))
    ct = M.UsbdSecXSM3AuthenticationCrypt(k1, plain, M.CryptMode.ENCRYPT)
    mac = M.UsbdSecXSM3AuthenticationMac(k2, None, ct)[1]
    pkt = bytes([9, 0x40, 0, 0, 0x1C]) + ct + mac[4:]
    pkt += bytes([M.xsm3_calculate_checksum(pkt)])
    random.seed(1234)
    st = M.XSM3State(M.xsm3_id_data_ms_controller)
    assert st.xsm3_do_challenge_init(pkt) is not None
    data8 = bytes(range(8))
    buf = M.UsbdSecXSM3AuthenticationCrypt(st.xsm3_random_controller_data, data8, M.CryptMode.DECRYPT)
    rcd = bytearray(st.xsm3_random_console_data)
    rcd[8:16] = buf
    vmac = M.UsbdSecXSM3AuthenticationMac(st.xsm3_challenge_init_hash, rcd, data8)[1]
    vpkt = bytes([9, 0x41, 0, 0, 0x10]) + data8 + vmac
    vpkt += bytes([M.xsm3_calculate_checksum(vpkt)])
    resp = st.xsm3_do_challenge_verify(vpkt)
    assert resp is not None and len(resp) == 5 + 0x10 + 1 and bytes(resp[:2]) == b"IL" and resp[4] == 0x10


def test_candidates_localize_twice_lays_out_the_merged_struct_array_the_same():
    """exact.draft.candidates: merging arrays of structs of one size into one array replaces its STRUCTARR entry by
    the list of its fields; the next variant of the same text read that list as a field's type and declared
    `[(0, 'int'), (4, 'int')] f0;`. The second run of localize on the same state comes out as the first."""
    from codebreaker.exact.draft import candidates as C, text as T

    class Prj:
        dgroup = 0x40
        image = bytes(0x100)

        def toolchain(self):
            class TC:
                name = "msc300"
                p = {}
            return TC()

        def symbols(self):
            return {}

    src = "int f(i)\nint i;\n{\n    int x;\n    x = g_1000[i].f0;\n    g_1004[i].f0 = x;\n    return x;\n}"
    T.ARRAYS.clear()
    T.STRUCTARR.clear()
    for g in ("g_1000", "g_1004"):
        T.ARRAYS[g] = "struct s_%s" % g
        T.STRUCTARR[g] = (8, "int")
    try:
        first = C.localize(Prj(), src, 0, {})
        again = C.localize(Prj(), src, 0, {})
    finally:
        T.ARRAYS.clear()
        T.STRUCTARR.clear()
    assert "struct s_g_1000 { int f0; char _2[2]; int f4; char _6[2]; };" in first[0], first[0]
    assert again == first, again[0]


def main():
    for fn in (test_pe_writer_nosplit_keeps_the_scaled_index, test_cpukey_generated_keys_validate,
               test_sms_writer_game_gear_rom, test_sd_signer_keeps_its_key_in_the_module,
               test_streamio_offset_labels_and_perform_function_at, test_apply_jump_sd_4bl_writes_the_branch,
               test_xsm3_challenge_verify_answers_a_valid_packet, test_candidates_localize_twice_lays_out_the_merged_struct_array_the_same):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d failed" % (sum(1 for r in RESULTS if r[1] == "ok"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
