"""Original Xbox EEPROM (256 bytes) parser — console identity + config.

The security block (HDD unlock key + XBE game region) is RC4-encrypted; the RC4
key is an Xbox-specific keyed hash of the stored 20-byte HMAC. That keyed hash
is NOT standard HMAC: it is two chained SHA-1 passes whose initial states (IVs)
are replaced by version-specific constants — `XcSHA1(IV2, XcSHA1(IV1, data))`.
The correct kernel version is the one whose IVs re-hash the decrypted block back
to the stored value.

IVs and flow are clean-room from the documented algorithm (as implemented by
didix16/xboxhdd-keygen and the Xbox kernel). Plaintext fields (serial, MAC,
video standard) need no key.

Layout (little-endian):
  0x00 20 keyed-hash    0x14 8 confounder  0x1C 16 HDDKey  0x2C 4 XBERegion (enc)
  0x30 4 factory-checksum  0x34 12 serial  0x40 6 MAC  0x48 16 online key  0x58 4 video
"""

import struct
import sys

# (IV1, IV2) — 5 words each — per kernel version.
EEPROM_KEYS = {
    "1.0": ((0x72127625, 0x336472B9, 0xBE609BEA, 0xF55E226B, 0x99958DAC),
            (0x76441D41, 0x4DE82659, 0x2E8EF85E, 0xB256FACA, 0xC4FE2DE8)),
    "1.1-1.5": ((0x39B06E79, 0xC9BD25E8, 0xDBC6B498, 0x40B4389D, 0x86BBD7ED),
                (0x9B49BED3, 0x84B430FC, 0x6B8749CD, 0xEBFE5FE5, 0xD96E7393)),
    "1.6": ((0x8058763A, 0xF97D4E0E, 0x865A9762, 0x8A3D920D, 0x08995B2C),
            (0x01075307, 0xA2F1E037, 0x1186EEEA, 0x88DA9992, 0x168A5609)),
    "debug": ((0x85F9E51A, 0xE04613D2, 0x6D86A50C, 0x77C32E3C, 0x4BD717A4),
              (0x5D7A9C6B, 0xE1922BEB, 0xB82CCDBC, 0x3137AB34, 0x486B52B3)),
}

VIDEO_STANDARD = {0x00014000: "NTSC-M", 0x00024000: "NTSC-J", 0x00038000: "PAL-I"}
XBE_REGION = {0x1: "North America", 0x2: "Japan", 0x4: "Europe/Australia"}


def _rol(v, n):
    return ((v << n) | (v >> (32 - n))) & 0xFFFFFFFF


def _sha1_iv(iv, data):
    """Standard SHA-1 compression but starting from a custom 5-word IV."""
    h = list(iv)
    msg = bytearray(data)
    ml = len(data) * 8
    msg.append(0x80)
    while len(msg) % 64 != 56:
        msg.append(0)
    msg += struct.pack(">Q", ml)
    for off in range(0, len(msg), 64):
        w = list(struct.unpack(">16I", msg[off:off + 64]))
        for i in range(16, 80):
            w.append(_rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1))
        a, b, c, d, e = h
        for i in range(80):
            if i < 20:
                f = (b & c) | (~b & 0xFFFFFFFF & d); k = 0x5A827999
            elif i < 40:
                f = b ^ c ^ d; k = 0x6ED9EBA1
            elif i < 60:
                f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC
            else:
                f = b ^ c ^ d; k = 0xCA62C1D6
            t = (_rol(a, 5) + f + e + k + w[i]) & 0xFFFFFFFF
            e, d, c, b, a = d, c, _rol(b, 30), a, t
        h = [(h[0] + a) & 0xFFFFFFFF, (h[1] + b) & 0xFFFFFFFF, (h[2] + c) & 0xFFFFFFFF,
             (h[3] + d) & 0xFFFFFFFF, (h[4] + e) & 0xFFFFFFFF]
    return struct.pack(">5I", *h)


def xc_hmac(version, *buffers):
    """XcSHA1(IV2, XcSHA1(IV1, data)) — the Xbox keyed hash."""
    iv1, iv2 = EEPROM_KEYS[version]
    step1 = _sha1_iv(iv1, b"".join(buffers))
    return _sha1_iv(iv2, step1)


def _rc4(key, data):
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + key[i % len(key)]) & 0xFF
        S[i], S[j] = S[j], S[i]
    out = bytearray()
    i = j = 0
    for byte in data:
        i = (i + 1) & 0xFF
        j = (j + S[i]) & 0xFF
        S[i], S[j] = S[j], S[i]
        out.append(byte ^ S[(S[i] + S[j]) & 0xFF])
    return bytes(out)


def decrypt_security(eeprom):
    """Return (version, decrypted_block[28]) or (None, None)."""
    stored = eeprom[0x00:0x14]
    enc = eeprom[0x14:0x30]
    for ver in EEPROM_KEYS:
        rc4key = xc_hmac(ver, stored)
        dec = _rc4(rc4key, enc)
        confirm = xc_hmac(ver, dec[0:8], dec[8:28])
        if confirm[:0x14] == stored:
            return ver, dec
    return None, None


def encrypt_security(block, version):
    """Inverse (build/round-trip): block = confounder(8)+HDDKey(16)+region(4).
    Returns a 48-byte security region (hash20 + encrypted28)."""
    h = xc_hmac(version, block[0:8], block[8:28])
    rc4key = xc_hmac(version, h)
    return h + _rc4(rc4key, block)


def parse(eeprom):
    d = {"serial": eeprom[0x34:0x40].decode("ascii", "replace").rstrip("\x00"),
         "mac": ":".join("%02X" % b for b in eeprom[0x40:0x46])}
    vs = struct.unpack_from("<I", eeprom, 0x58)[0]
    d["video_standard"] = VIDEO_STANDARD.get(vs, "0x%08X" % vs)
    ver, dec = decrypt_security(eeprom)
    d["version"] = ver
    if dec:
        d["hdd_key"] = dec[8:24].hex().upper()
        region = struct.unpack_from("<I", dec, 24)[0]
        d["xbe_region"] = XBE_REGION.get(region, "0x%X" % region)
    else:
        d["hdd_key"] = d["xbe_region"] = None
    return d


DESCRIPTION = "parse an Original Xbox EEPROM: serial/MAC/video + decrypt HDD key & region"


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: eeprom <eeprom.bin>"); return
    eeprom = open(a[0], "rb").read()
    if len(eeprom) < 0x100:
        print("EEPROM too small (%d bytes; expected 256)" % len(eeprom)); return
    d = parse(eeprom)
    print("serial        : %s" % d["serial"])
    print("MAC address   : %s" % d["mac"])
    print("video standard: %s" % d["video_standard"])
    print("kernel version: %s" % (d["version"] or "(security block did not decrypt)"))
    print("HDD key       : %s" % (d["hdd_key"] or "-"))
    print("XBE region    : %s" % (d["xbe_region"] or "-"))


if __name__ == "__main__":
    main()
