#!/usr/bin/env python3
"""Makes testdata/tools_fn_goldens.txt (and kv_tool_enc.bin): what the Python's functions give on fixed inputs, for the tests of the native tools whose
work is random or needs keys no machine here has (tool_logic_test.cpp). The Python is called in-process, with what it asks the system for replaced:

    os.urandom / XeCryptRandom / randbytes      pattern(n, seed) for a request of n bytes (the tests give the native code the same bytes)
    the key store (load_and_verify_hvx_prv)      a key pair made for the tests (test_rsa_prv.bin), and the 1BL key = pattern(16, 42)

    python make_tools_fn_goldens.py <work folder>
"""
import contextlib
import hashlib
import io
import os
import struct
import sys
import tempfile

import make_tools_goldens as g
from make_tools_goldens import HERE, make_patch_table, pattern, td, valid_cpu_key


class PatternRandom:
    """pattern(n, seed) for each request of n bytes."""

    def __init__(self, seed):
        self.seed = seed

    def __call__(self, n):
        return pattern(n, self.seed)


def fn_goldens(work):
    from codebreaker.formats.xbox.crypto import XeCrypt as xc
    from codebreaker.formats.xbox.crypto import exp_signer, kv_tool, xekeys_signer, xsm3
    out = []
    prv = td("test_rsa_prv.bin")
    bl1 = pattern(16, 42)

    def line(*parts):
        out.append(" ".join(str(p) for p in parts))

    # cpu_key: the report its main prints, for a fixed key
    key = pattern(16, 1)
    bits = "".join([f"{x:0>8b}" for x in key])
    out.append("== cpu_key_report")
    line("ECC Bits: " + bits[-26:])
    line("UID Bits: " + bits[:-26])
    line(key.hex().upper())

    # xekeys_signer: sign_xekeys with the test key as the HVX key, the 1BL key set and the random key fixed
    xekeys_signer.XECRYPT_1BL_KEY = bl1
    xekeys_signer.load_and_verify_hvx_prv = lambda: xc.XeCryptRsaKey(prv)
    xekeys_signer.urandom = PatternRandom(41)
    for name, magic in (("devkit", xekeys_signer.PayloadMagic.DEVKIT), ("retail", xekeys_signer.PayloadMagic.RETAIL)):
        with tempfile.TemporaryDirectory(dir=work) as tmp:
            open(os.path.join(tmp, "in.bin"), "wb").write(pattern(100, 40))
            xekeys_signer.sign_xekeys(os.path.join(tmp, "in.bin"), os.path.join(tmp, "out.bin"), magic)
            data = open(os.path.join(tmp, "out.bin"), "rb").read()
        out.append("== xekeys_" + name)
        line("size", len(data), "sha1", hashlib.sha1(data).hexdigest())

    # exp_signer: encrypted HXPR (the IV fixed), SIGM, and HXPR unencrypted
    exp_signer.XECRYPT_1BL_KEY = bl1
    exp_signer.urandom = PatternRandom(43)
    for name, kwargs in (("encrypted", dict(encrypt=True)), ("sigm", dict(exp_magic=exp_signer.ExpansionMagic.SIGM)), ("unencrypted", dict(encrypt=False))):
        with tempfile.TemporaryDirectory(dir=work) as tmp:
            open(os.path.join(tmp, "in.bin"), "wb").write(pattern(70, 44))
            open(os.path.join(tmp, "prv.bin"), "wb").write(prv)
            exp_signer.sign_exp(os.path.join(tmp, "in.bin"), os.path.join(tmp, "out.bin"), os.path.join(tmp, "prv.bin"), **kwargs)
            data = open(os.path.join(tmp, "out.bin"), "rb").read()
        out.append("== exp_" + name)
        line("size", len(data), "sha1", hashlib.sha1(data).hexdigest())

    # xsm3: a challenge a console would make with the test keys, answered by the controller
    xsm3.STATIC_KEY_1, xsm3.STATIC_KEY_2 = pattern(16, 44), pattern(16, 45)
    xsm3.randbytes = PatternRandom(46)
    ident = bytes(xsm3.xsm3_id_data_ms_controller)
    payload = bytes(xsm3.UsbdSecXSM3AuthenticationCrypt(xsm3.STATIC_KEY_1, pattern(16, 47) + pattern(8, 48), xsm3.CryptMode.ENCRYPT))
    mac = xsm3.UsbdSecXSM3AuthenticationMac(xsm3.STATIC_KEY_2, None, payload)[1]
    packet = bytearray(bytes([0x09, 0x40, 0, 0, 0x1C]) + payload + bytes(mac)[4:])
    packet.append(xsm3.xsm3_calculate_checksum(packet))
    state = xsm3.XSM3State(ident)
    init_response = bytes(state.xsm3_do_challenge_init(bytes(packet)))
    payload8 = bytes(xsm3.UsbdSecXSM3AuthenticationCrypt(state.xsm3_random_controller_data, pattern(8, 49), xsm3.CryptMode.ENCRYPT))
    console_random = bytearray(state.xsm3_random_console_data)
    console_random[8:16] = pattern(8, 49)
    mac8 = bytes(xsm3.UsbdSecXSM3AuthenticationMac(state.xsm3_challenge_init_hash, bytes(console_random), payload8)[1])
    verify = bytearray(bytes([0x09, 0x41, 0, 0, 0x10]) + payload8 + mac8)
    verify.append(xsm3.xsm3_calculate_checksum(verify))
    verify_response = bytes(state.xsm3_do_challenge_verify(bytes(verify)))
    out.append("== xsm3_exchange")
    line("challenge", bytes(packet).hex())
    line("init_response", init_response.hex())
    line("verify", bytes(verify).hex())
    line("verify_response", verify_response.hex())

    out.append("== xsm3_primitives")
    k = pattern(16, 50)
    data = pattern(32, 51)
    line("crypt_encrypt", bytes(xsm3.UsbdSecXSM3AuthenticationCrypt(k, data, xsm3.CryptMode.ENCRYPT)).hex())
    line("crypt_decrypt", bytes(xsm3.UsbdSecXSM3AuthenticationCrypt(k, data, xsm3.CryptMode.DECRYPT)).hex())
    line("mac_no_salt", bytes(xsm3.UsbdSecXSM3AuthenticationMac(k, None, data)[1]).hex())
    salt, tag = xsm3.UsbdSecXSM3AuthenticationMac(k, pattern(16, 52), data)
    line("mac_salt", bytes(salt).hex(), bytes(tag).hex())
    line("mac_20_byte_key", bytes(xsm3.UsbdSecXSM3AuthenticationMac(pattern(20, 53), None, data)[1]).hex())
    line("acr", bytes(xsm3.UsbdSecXSMAuthenticationAcr(k, pattern(8, 54), pattern(0x20, 55))).hex())

    # kv_tool: its main with a fixed CPU key and random, in a folder of its own
    key = valid_cpu_key(9)
    xc.XeCryptRandom = PatternRandom(60)
    kv_tool.XeCryptCpuKeyGen = lambda: key
    with tempfile.TemporaryDirectory(dir=work) as tmp:
        before = os.getcwd()
        os.chdir(tmp)
        try:
            os.makedirs("KV")
            os.makedirs("Output/Zero")
            open("KV/banned.bin", "wb").write(pattern(0x4000, 61))
            with contextlib.redirect_stdout(io.StringIO()):
                kv_tool.main()
            fuses = open("Output/Zero/fuses.bin", "rb").read()
            sealed = open("Output/Zero/kv_enc.bin", "rb").read()
        finally:
            os.chdir(before)
    out.append("== kv_tool")
    line("cpu_key", key.hex())
    line("fuses", fuses.hex())
    line("kv_enc_sha1", hashlib.sha1(sealed).hexdigest())
    open(os.path.join(HERE, "kv_tool_enc.bin"), "wb").write(sealed)

    # patch_dumper: the loop of its main, on a table of our own
    from codebreaker import platforms
    md = platforms.decoder(32, arch="ppc")
    words = [0x38600001, 0x4E800020, 0xFFFFFFFF, 0x7C0802A6, 0x60000000, 0x48000010, 0x2C030000, 0x80010008]
    table = make_patch_table([(0x80001000, b"".join(struct.pack(">I", w) for w in words)), (0x80002000, struct.pack(">I", 0x4E800020))])
    lines = []
    pos = 0
    while pos < len(table):
        while True:
            (addr,) = struct.unpack(">I", table[pos:pos + 4])
            pos += 4
            if addr == 0xFFFFFFFF:
                lines.append("# done with patch section\n")
                break
            (size,) = struct.unpack(">I", table[pos:pos + 4])
            pos += 4
            lines.append(f"MAKEPATCH 0x{addr:08X}")
            lines.append("0:")
            for i in range(size):
                code = table[pos:pos + 4]
                pos += 4
                disasm = [x for x in md.disasm(code, addr + (i * 4))]
                if len(disasm) > 0:
                    for x in disasm:
                        lines.append(f"\t{x.mnemonic} {x.op_str}".strip(" "))
                else:
                    lines.append(f"\t.long 0x{code.hex()}")
            lines.append("9:\n")
    out.append("== patch_dumper")
    out.append("\n".join(lines))

    # MemCrypto: the three digests its `calculate` prints and the decryption its `test` makes, on images of our own
    from codebreaker.formats.xbox.crypto import MemCrypto
    key55 = b"\x55" * 0x10
    hv = pattern(0x40000, 62)
    salt = bytes.fromhex("892BB9F952C7759392A12A3184E0358E")
    with MemCrypto.MemoryCrypto(key55, key55, key55) as mem:
        out.append("== memcrypto_calculate")
        line("hash1", mem.calc_hash_1_digest(hv, salt).hex().upper())
        line("hash2", mem.calc_hash_2_digest(hv, salt, 0xB00B).hex().upper())
        line("blob", mem.calc_key_blob(b"testtest".ljust(20, bytes(1))).hex().upper())
    keys = pattern(48, 64)
    enc = pattern(0x400, 65)
    with MemCrypto.MemoryCrypto(keys[:0x10], keys[0x10:0x20], keys[-0x10:]) as mem:
        dec = mem.decrypt(enc, 0, len(enc), offset_is_address=True)
    out.append("== memcrypto_decrypt")
    line("sha1", hashlib.sha1(dec).hexdigest())

    # xkelib_exports: create_def's parsing of dumpbin's text (no SDK here: the text is what dumpbin prints, written by hand)
    import re
    from codebreaker.formats.xbox.crypto import xkelib_exports as xk
    headers = "Microsoft (R) COFF/PE Dumper Version 11.00\r\n\r\n  File Type: DLL\r\n\r\n    DLL name : xboxkrnl.exe\r\n\r\n"
    exports = ("Dump of file xboxkrnl.exe\r\n\r\n  Section contains the following exports for xboxkrnl.exe\r\n\r\n"
               "    ordinal hint RVA      name\r\n\r\n"
               "          1    0 00001000 ExAllocatePool\r\n          3    1 00001020 .local_label\r\n          2    2 00001010 ExFreePool\r\n"
               "         10    3 00001030 KeBugCheck\r\n\r\n  Summary\r\n\r\n        1000 .text\r\n")
    clean = []
    for (ordinal, func) in xk.EXPORT_RE.findall(exports, re.MULTILINE):
        if not func.startswith("."):
            clean.append((int(ordinal), func))
    clean.sort(key=lambda i: i[0])
    text = f"LIBRARY {xk.fetch_version(headers)}\r\nEXPORTS\r\n" + "".join(f"\t{func} @{ordinal}\r\n" for (ordinal, func) in clean)
    out.append("== xkelib_exports")
    line("headers", headers.encode().hex())
    line("exports", exports.encode().hex())
    line("def", text.encode().hex())
    return "\n".join(out) + "\n"


if __name__ == "__main__":
    work = sys.argv[1]
    os.makedirs(work, exist_ok=True)
    open(os.path.join(HERE, "tools_fn_goldens.txt"), "w", newline="\n").write(fn_goldens(work))
