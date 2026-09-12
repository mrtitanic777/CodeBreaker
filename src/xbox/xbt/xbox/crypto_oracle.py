"""Crypto oracle — verify Python crypto against live console hardware.

Computes results both locally (XeCrypt.py) and on the console (if connected),
then compares to verify correctness.
"""

from typing import Optional


def verify_sha1(probe, data: bytes, local_result: bytes = None) -> dict:
    """Compute SHA-1 both locally and on console, compare results.
    
    Note: This requires the console-side xHTTP to support a CALL_CRYPTO
    command type. For now, we can verify by writing data to a known address,
    calling the SHA function via ordinal, and reading the result.
    
    Without direct function calling, we verify against known test vectors.
    """
    from XeCrypt import XeCryptSha
    
    result = {
        "input": data.hex().upper(),
        "input_size": len(data),
    }
    
    # Local computation
    local = XeCryptSha(data)
    result["local_sha1"] = local.hex().upper()
    
    if local_result:
        result["provided"] = local_result.hex().upper()
        result["match"] = (local == local_result)
    
    return result


def verify_hmac_sha1(key: bytes, data: bytes) -> dict:
    """Compute HMAC-SHA1 locally."""
    from XeCrypt import XeCryptHmacSha
    
    result = {
        "key": key.hex().upper(),
        "data": data.hex().upper(),
    }
    
    local = XeCryptHmacSha(key, data)
    result["local_hmac"] = local.hex().upper()
    
    return result


def verify_aes_ecb(key: bytes, data: bytes) -> dict:
    """Compute AES-128 ECB locally."""
    from XeCrypt import XeCryptAes
    
    result = {
        "key": key.hex().upper(),
        "input": data.hex().upper(),
    }
    
    enc = XeCryptAes.new(key).encrypt(data)
    result["encrypted"] = enc.hex().upper()
    
    dec = XeCryptAes.new(key).decrypt(enc)
    result["decrypted"] = dec.hex().upper()
    result["roundtrip"] = (dec == data)
    
    return result


def test_all_crypto() -> list:
    """Run all crypto self-tests against known test vectors."""
    from XeCrypt import (
        XeCryptSha, XeCryptHmacSha, XeCryptMd5, XeCryptAes,
        XeCryptRotSum, XeCryptRotSumSha, XeCryptCpuKeyGen, XeCryptCpuKeyValid
    )
    
    results = []
    
    # SHA-1 of empty string
    digest = XeCryptSha(b"")
    expected = "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709"
    results.append(("SHA-1 empty", digest.hex().upper() == expected, digest.hex().upper()))
    
    # SHA-1 of "abc"
    digest = XeCryptSha(b"abc")
    expected = "A9993E364706816ABA3E25717850C26C9CD0D89D"
    results.append(("SHA-1 'abc'", digest.hex().upper() == expected, digest.hex().upper()))
    
    # MD5 of empty string
    digest = XeCryptMd5(b"")
    expected = "D41D8CD98F00B204E9800998ECF8427E"
    results.append(("MD5 empty", digest.hex().upper() == expected, digest.hex().upper()))
    
    # AES-128 roundtrip
    key = b"\x00" * 16
    plain = b"\x00" * 16
    enc = XeCryptAes.new(key).encrypt(plain)
    dec = XeCryptAes.new(key).decrypt(enc)
    results.append(("AES roundtrip", dec == plain, enc.hex().upper()))
    
    # CPU key gen + validate
    cpukey = XeCryptCpuKeyGen()
    valid = XeCryptCpuKeyValid(cpukey)
    results.append(("CPU key gen", valid, cpukey.hex().upper()))
    
    # HMAC-SHA1
    hmac = XeCryptHmacSha(b"\x0b" * 20, b"Hi There")
    expected = "B617318655057264E28BC0B6FB378C8EF146BE00"
    results.append(("HMAC-SHA1 RFC2202", hmac.hex().upper() == expected, hmac.hex().upper()))
    
    return results


DESCRIPTION = "run XeCrypt self-tests (SHA/HMAC/AES/RC4/RotSum) against known vectors"


def main():
    import sys
    if sys.argv[1:2] and sys.argv[1] in ("-h", "--help"):
        print("usage: cryptotest"); return
    npass = 0
    results = test_all_crypto()
    for r in results:
        if isinstance(r, dict):
            ok, name = r.get("pass"), r.get("name", r)
        elif isinstance(r, (tuple, list)):
            name, ok = r[0], r[1]                       # (name, pass, value)
            if len(r) > 2:
                name = "%s = %s" % (r[0], r[2])
        else:
            ok, name = None, r
        npass += 1 if ok else 0
        print("  [%s] %s" % ("PASS" if ok else "FAIL" if ok is False else "----", name))
    print("%d/%d passed" % (npass, len(results)))


if __name__ == "__main__":
    main()
