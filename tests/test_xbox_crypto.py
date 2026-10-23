#!/usr/bin/env python3
"""The Xbox 360 console crypto package (codebreaker/formats/xbox/crypto): XeCrypt's hashes and
ciphers give the published test vectors, a keyvault encrypted with a CPU key decrypts back with
it, the stream class reads and writes in both byte orders, and the `crypto` command lists and
runs its tools.

    python tests/test_xbox_crypto.py

No key material is needed: the vectors are the standards' (FIPS 180, RFC 2202, FIPS 197, RFC
6229), and the CPU key is generated here."""
import contextlib
import io
import os
import sys
import warnings

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
warnings.simplefilter("ignore")

from codebreaker.formats.xbox import crypto                       # noqa: E402
from codebreaker.formats.xbox.crypto import XeCrypt as xc         # noqa: E402
from codebreaker.formats.xbox.crypto.StreamIO import Endian, StreamIO  # noqa: E402

RESULTS = []


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def test_hashes():
    assert xc.XeCryptSha(b"abc").hex() == "a9993e364706816aba3e25717850c26c9cd0d89d"
    assert xc.XeCryptSha(b"a", b"bc") == xc.XeCryptSha(b"abc")          # the parts are one message
    assert xc.XeCryptMd5(b"abc").hex() == "900150983cd24fb0d6963f7d28e17f72"
    assert xc.XeCryptHmacSha(b"\x0b" * 20, b"Hi There").hex() == "b617318655057264e28bc0b6fb378c8ef146be00"


def test_ciphers():
    key = bytes(range(16))
    plain = bytes.fromhex("00112233445566778899aabbccddeeff")
    aes = xc.XeCryptAes.new(key)
    assert aes.encrypt(plain).hex() == "69c4e0d86a7b0430d8cdb78070b4c55a"
    assert xc.XeCryptAes.new(key).decrypt(bytes.fromhex("69c4e0d86a7b0430d8cdb78070b4c55a")) == plain
    assert xc.XeCryptRc4.new(bytes.fromhex("0102030405")).encrypt(bytes(16)).hex() == "b2396305f03dc027ccc3524a0a1118a8"


def test_cpu_key_and_keyvault():
    cpu_key = xc.XeCryptCpuKeyGen()
    assert len(cpu_key) == 16 and xc.XeCryptCpuKeyValid(cpu_key)
    assert not xc.XeCryptCpuKeyValid(bytes(16))
    vault = bytearray(os.urandom(0x4000))
    sealed = xc.XeCryptKeyVaultEncrypt(cpu_key, vault)
    opened = xc.XeCryptKeyVaultDecrypt(cpu_key, sealed)
    assert opened[0x18:] == bytes(vault[0x18:])                # the contents after the nonce and confounder
    assert sealed[0x18:] != bytes(vault[0x18:])


def test_stream_io():
    with StreamIO(endian=Endian.BIG) as s:
        s.write_uint32(0x12345678)
        s.write_bytes(b"\xAA\xBB")
        s.write_bytes_at(0, b"\x9A")
        assert s.getvalue() == bytes.fromhex("9A345678AABB")
        s.seek(0)
        assert s.read_uint32() == 0x9A345678 and s.read_ubytes(2) == b"\xAA\xBB"
        assert s.read_bytes_at(1, 2) == b"\x34\x56" and s.tell() == 6
    with StreamIO(bytes.fromhex("78563412"), Endian.LITTLE) as s:
        assert s.read_uint32() == 0x12345678


def test_command():
    names = crypto.tools()
    for tool in ("cpu_key", "shadowboot", "nand_tool", "xcp_dumper", "harness"):
        assert tool in names, tool
    out = io.StringIO()
    with contextlib.redirect_stdout(out):
        assert crypto.main(["cpu_key"]) == 0
    key = bytes.fromhex(out.getvalue().split()[-1])
    assert xc.XeCryptCpuKeyValid(key)


def main():
    for fn in (test_hashes, test_ciphers, test_cpu_key_and_keyvault, test_stream_io, test_command):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d failed" % (sum(1 for r in RESULTS if r[1] == "ok"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
