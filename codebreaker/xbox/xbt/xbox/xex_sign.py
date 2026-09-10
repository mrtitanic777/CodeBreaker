"""XEX2 devkit header signing / verification (PSS over RotSumSha, RSA e=3).

Clean-room port of XexTool-RE's xex_sign.cpp using Python's native big integers.
The devkit RSA key (N, d, e=3) is public/known and embedded in every copy of
XexTool; it re-signs devkit XEX headers. Retail headers get a zeroed signature
(no one has Microsoft's retail private key). Verified byte-identical against
XexTool-RE and against real devkit modules.
"""
import hashlib
import struct

_M64 = (1 << 64) - 1
_M32 = (1 << 32) - 1

kDevkitN = bytes.fromhex(
    "d7492f13b1c17fbf2c5b33e51cfa0002bb9d881671648107a6e9a7c278f5c0b8d4b7419b0a99600"
    "27f95271acc7bac2916601a8ac74b6893ed8c08ed2513ccbb9ec2a4cfb3ecc8059f18d04dada56e"
    "fcb0484cf99e6fa4764054226544102ad2d587667bf2289cb37207da63aaf33eed1ebcd83d91a78"
    "97c664e8bd022abdb90efa039ea1cf948667dbc4559d0e3f29744f48dc58451c81b4e3a8142c56e"
    "b949b0e0f51a8e8039c2b8d9a3f13b6e71db56910ff8e8bd904dccbed0c5a743aa6c4c5d8565988"
    "c4c6bb8705cc0be8f1b3d6d5d97fd7330763ae1767061e6be93273e0670624aa2ad9984e7ab2ee4"
    "dbae1e642f4e6c7399ace5c91c3577c8bfa06b")
kDevkitD = bytes.fromhex(
    "8f861f627680ffd4c83ccd4368a6aaac7d13b00ef64300afc49bc52c50a3d5d08dcf81120710eaa"
    "c550e1a11dda7c81b644011b1da3245b7f3b2b09e18b7ddd269d7188a77f33003bf65e033c918f4"
    "a8758588a6699fc2f9803816ee2d601c8c8e5a4452a1706877a15a9197c74cd49e147de57e611a5"
    "ba844345d356c72925f664e995622c73c3d9b7038113e9847449e2f660ccdc58a26974f6b9c4222"
    "e98e1a553eeb4416c0445da709ac06807d9e91293542c4ca53310d5e26331f791fe6ebb0d032492"
    "2713486ebd1220c81729c6b908be936a44d55169268ebb10f12de4fae198393352901bc167f442d"
    "33563a3c9eef40e6ddad8d0862f50f4d47cc5b")
kDevkitRinv2 = bytes.fromhex(
    "02947d392ad1feae9d2b7461e0350e8b2b9c702a142e01520baaf282674165f58a8010f10e49594"
    "a981b36676a37d40d27eee0c4bf9b782dd22c42a51e794eb9b5d8c39e3664dc84d927a9938cab62"
    "d6cad721111bc8f08f1dc9b5e19c86677f119077f35646789f1d9c6d81a0fe6a7106b4e169058d3"
    "417be5943001b20ccfca459d660fdbecab03878f92aec3f0f7277bd50f15ca2ca0119265641017"
    "228ecb89e32bd19d9eb5a4cfbbb03a6b49bf73c2f9f3fed444bbdcc2967f993a85e86635307c9ac"
    "88866975edbfcde5834b219d62445feb06ecc74adad09994ef281c7d29ade4f5b2b1e63ba0ef601"
    "7a0659fc5a3ba0c3303f476bb00795af10374b8")

_N = int.from_bytes(kDevkitN, "big")
_D = int.from_bytes(kDevkitD, "big")
_RINV2 = int.from_bytes(kDevkitRinv2, "big")
_R2 = pow(2, 4096, _N)                              # Montgomery R^2 (R = 2^2048)
_SALT = b"XBOX360XEX"


def _rotsumsha(data):
    r = [0, 0, 0, 0]
    for i in range(len(data) // 8):
        v = int.from_bytes(data[i * 8:i * 8 + 8], "big")
        s = (r[1] + v) & _M64
        u5 = s & _M32
        u6 = (s >> 32) & _M32
        r[0] = (r[0] + (1 if s < v else 0)) & _M64
        r[1] = (((((u6 << 29) & _M32) | (u5 >> 3)) & _M32) << 32) | \
               (((u6 >> 3) | ((u5 << 29) & _M32)) & _M32)
        u1 = (r[3] - v) & _M64
        u4 = (u1 >> 32) & _M32
        u1l = u1 & _M32
        r[2] = (r[2] - (1 if v < u1 else 0)) & _M64
        r[3] = (((((u4 << 31) & _M32) | (u1l >> 1)) & _M32) << 32) | \
               (((u4 >> 1) | ((u1l << 31) & _M32)) & _M32)
    rb = b"".join(struct.pack(">Q", x) for x in r)
    nb = b"".join(struct.pack(">Q", (~x) & _M64) for x in r)
    return hashlib.sha1(rb + rb + data + nb + nb).digest()


def _rc4(key, n):
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + key[i % len(key)]) & 0xFF
        S[i], S[j] = S[j], S[i]
    out = bytearray()
    i = j = 0
    for _ in range(n):
        i = (i + 1) & 0xFF
        j = (j + S[i]) & 0xFF
        S[i], S[j] = S[j], S[i]
        out.append(S[(S[i] + S[j]) & 0xFF])
    return out


def _pss_encode(msg_hash, salt):
    out = bytearray(256)
    out[0xE0] = 1
    out[0xE1:0xE1 + 10] = salt
    H = hashlib.sha1(bytes(out[:8]) + msg_hash + salt).digest()
    out[0xEB:0xEB + 20] = H
    out[0xFF] = 0xBC
    mask = _rc4(H, 0xEB)
    for i in range(0xEB):
        out[i] ^= mask[i]
    out[0] &= 0x7F
    return bytes(out)


def devkit_sign(hdr, data_offset, do_sign, sec_off):
    """In-place: recompute HeaderHash, then sign (devkit) or zero (retail) the
    256-byte header signature. `hdr` is a bytearray."""
    sig_off = sec_off + 8
    img = sec_off + 0x108
    rgn_end = img + 0x74
    hh_off = img + 0x5C
    # HeaderHash = SHA1( hdr[rgn_end:data_offset] || hdr[0:sig_off] )
    hh = hashlib.sha1(bytes(hdr[rgn_end:data_offset]) + bytes(hdr[:sig_off])).digest()
    hdr[hh_off:hh_off + 20] = hh
    if not do_sign:
        hdr[sig_off:sig_off + 256] = b"\x00" * 256
        return
    mh = _rotsumsha(bytes(hdr[img:rgn_end]))
    pss = _pss_encode(mh, _SALT)
    base = (int.from_bytes(pss, "big") * _R2) % _N
    sig = pow(base, _D, _N).to_bytes(256, "big")
    # store qword-order-reversed
    out = bytearray(256)
    for i in range(0, 256, 8):
        out[256 - 8 - i:256 - i] = sig[i:i + 8]
    hdr[sig_off:sig_off + 256] = out


def verify_devkit_sig(sig256):
    """True if the 256-byte signature is a valid devkit PSS signature."""
    q = bytearray(256)
    for i in range(0, 256, 8):
        q[256 - 8 - i:256 - i] = sig256[i:i + 8]
    s = int.from_bytes(q, "big")
    if s >= _N:
        return False
    pss = (pow(s, 3, _N) * _RINV2) % _N
    pb = pss.to_bytes(256, "big")
    return pb[0xFF] == 0xBC and (pb[0] & 0x80) == 0
