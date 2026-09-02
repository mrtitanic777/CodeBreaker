#!/usr/bin/env python3
"""Account — Xbox 360 profile Account-file decryptor.

The Account file in a gamer profile is a 0x194-byte blob: a 16-byte HMAC-SHA1
tag followed by an RC4-encrypted XEACCOUNTINFO record. The RC4 key is derived
by HMAC-SHA1'ing the tag under a static per-environment key (retail / devkit);
the same HMAC over the decrypted payload authenticates it. Clean-room Python
implementation from the public format — the account half of the GPL X360
library, as original code. The static keys are public well-known constants.

Commands:
    account info <Account>     decrypt and show gamertag / XUID / flags
"""
DESCRIPTION = "decrypt an Xbox 360 profile Account file (gamertag, XUID)"

import hashlib
import hmac
import os
import struct
import sys

# public, well-known static account keys
RETAIL_KEY = bytes.fromhex("E1BC159C73B1EAE9AB3170F3AD47EBF3")
DEVKIT_KEY = bytes.fromhex("DA B6 9A D9 8E 0F A5 EC A0 96 79 98 87 A0 D9 CB".replace(" ", ""))


def _rc4(key, data):
    s = list(range(256))
    j = 0
    for i in range(256):
        j = (j + s[i] + key[i % len(key)]) & 0xFF
        s[i], s[j] = s[j], s[i]
    out = bytearray()
    i = j = 0
    for c in data:
        i = (i + 1) & 0xFF
        j = (j + s[i]) & 0xFF
        s[i], s[j] = s[j], s[i]
        out.append(c ^ s[(s[i] + s[j]) & 0xFF])
    return bytes(out)


class Account:
    def __init__(self, data):
        if len(data) < 0x194:
            raise ValueError("Account file too small (%d bytes)" % len(data))
        self.raw = data
        tag = data[:0x10]
        enc = data[0x10:0x184 + 0x10]
        self.environment = None
        for name, key in (("retail", RETAIL_KEY), ("devkit", DEVKIT_KEY)):
            rc4_key = hmac.new(key, tag, hashlib.sha1).digest()[:0x10]
            dec = _rc4(rc4_key, enc)
            mac = hmac.new(key, dec, hashlib.sha1).digest()[:0x10]
            if mac == tag:
                self.environment = name
                self.dec = dec
                break
        else:
            # MAC mismatch under both keys: still decrypt with retail so the
            # fields are visible, but flag it as unverified
            rc4_key = hmac.new(RETAIL_KEY, tag, hashlib.sha1).digest()[:0x10]
            self.dec = _rc4(rc4_key, enc)
            self.environment = "unverified"

        d = self.dec
        # gamertag is a UTF-16BE WCHAR[16] at 0x10; the online XUID (LIVE
        # accounts carry the 0x0009... prefix) sits at 0x30.
        self.gamertag = d[0x10:0x30].decode("utf-16-be", "replace").split("\x00", 1)[0]
        self.xuid = d[0x30:0x38].hex().upper()
        self.is_live = self.xuid[:4] == "0009"


# ------------------------------------------------------------------------ CLI
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: account info <Account-file>"); return
    op = a[0]
    if len(a) < 2:
        print("usage: account info <Account-file>"); sys.exit(2)
    acc = Account(open(a[1], "rb").read())
    print("file        : %s" % os.path.basename(a[1]))
    print("environment : %s" % acc.environment)
    print("gamertag    : %r" % acc.gamertag)
    print("XUID        : %s" % acc.xuid)
    print("type        : %s" % ("LIVE" if acc.is_live else "offline"))


if __name__ == "__main__":
    main()
