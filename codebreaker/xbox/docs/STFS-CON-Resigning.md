# STFS CON resigning — keyvault, console private key, and the signature crack

This documents **exactly** how an Xbox 360 STFS `CON ` package is signed by a
console, where the signing material lives inside a decrypted keyvault
(`KV_dec.bin`), and how the scheme was reverse-engineered and verified
byte-exact against genuine hardware-signed packages. It is the reference for
`xbt/xbox/stfs.py`'s `resign` command.

Everything here was derived empirically from real data you own (a KV extracted
with J-Runner + real console-signed saves), not copied from any GPL tool.

---

## 0. TL;DR

An STFS `CON ` package is signed with the console's **1024-bit RSA private key**
using **textbook PKCS#1 v1.5 RSA-SHA1** — *not* the fancy XeCrypt "BeSig" format
the bootloaders use. Concretely:

```
digest      = SHA1( header[0x22C : 0x344] )              # 0x118 bytes
DigestInfo  = 3021300906052B0E03021A05000414 || digest   # standard SHA1 DER prefix
block(0x80) = 00 01  FF..FF(pad)  00  DigestInfo          # PKCS#1 v1.5, 128 bytes
sig(0x80)   = ( block ^ d  mod  n )                        # RSA private-key op
stored      = sig[::-1]                                    # BYTE-REVERSED
             -> written at header[0x1AC : 0x22C]
```

The console **certificate** (0x1A8 bytes) is copied verbatim from the keyvault
into `header[0x04 : 0x1AC]`. Both the cert and the signature live *before* the
0x22C signed region, so re-signing never disturbs its own signed input.

Two gotchas that cost the most time:
1. The signature is stored **byte-reversed** in the file.
2. RSA big-numbers (the modulus in the cert *and* the key components in the KV)
   are stored in XeCrypt **"BnQw" form** — an array of big-endian 64-bit qwords —
   not plain big-endian. Decode with `_bn()` (below), never `int.from_bytes(.,'big')`.

---

## 1. The decrypted keyvault (`KV_dec.bin`)

A decrypted keyvault is **0x4000 bytes**. Its layout is the `XECRYPT_KEYVAULT`
C-struct (`_pack_ = 1`), which is already defined in
`xbt/crypto/XeCrypt.py`. The two regions that matter for resigning:

| Field                        | KV offset | Size   | Notes |
|------------------------------|-----------|--------|-------|
| `console_private_key`        | **0x298** | 0x1D0  | `XECRYPT_RSAPRV_1024` — the signing key |
| console **certificate** blob | **0x9C8** | **0x1A8** | copied verbatim into the STFS header |

The certificate sub-fields (inside the 0x1A8 blob at 0x9C8), which mirror the
on-disk STFS header cert exactly:

| Field                          | KV offset | Size  |
|--------------------------------|-----------|-------|
| `console_certificate_size`     | 0x9C8     | 2     |
| `console_id`                   | 0x9CA     | 5     |
| `console_part_number`          | 0x9CF     | 0x14  |
| ... type / privileges / date   |           |       |
| `console_public_key_exponent`  |           | 4     |
| `console_public_key_modulus`   | 0x9F0     | 0x80  |
| `console_certificate_signature`|           | 0x100 (Microsoft's sig over the cert) |

Getting these offsets from the ctypes struct (no guessing):

```python
from XeCrypt import XECRYPT_KEYVAULT
XECRYPT_KEYVAULT.console_private_key.offset   # -> 0x298
XECRYPT_KEYVAULT.console_certificate_size.offset  # -> 0x9C8
XECRYPT_KEYVAULT.xeika_certificate_size.offset    # -> 0xB70  (== 0x9C8 + 0x1A8)
```

> **Which KV?** J-Runner writes a KV per extraction under `output/Extracts-*/`.
> You may have several; only the one from the console you want the package to run
> on is useful. Match `console_id` (KV 0x9CA) to the target console.

---

## 2. The console private key — `XECRYPT_RSAPRV_1024`

At KV **0x298**, 0x1D0 bytes, big-endian struct:

| Sub-field    | Rel. off | Size | Meaning |
|--------------|----------|------|---------|
| `cqw`        | +0x00    | 4    | qword count = **0x10** (16 qwords = 1024-bit) |
| `e`          | +0x04    | 4    | public exponent = **0x10001** |
| `qwReserved` | +0x08    | 8    | Montgomery `mod_inv` (unused by us) |
| `n`          | +0x10    | 0x80 | modulus (BnQw) |
| `p`          | +0x90    | 0x40 | prime P (BnQw) |
| `q`          | +0xD0    | 0x40 | prime Q (BnQw) |
| `dp`         | +0x110   | 0x40 | D mod (P-1) |
| `dq`         | +0x150   | 0x40 | D mod (Q-1) |
| `cr`         | +0x190   | 0x40 | CRT coefficient |

To sign we only need `e`, `n`, `p`, `q` — the private exponent is recovered as
`d = e⁻¹ mod ((p-1)(q-1))`.

### 2.1 BnQw decoding — the critical detail

XeCrypt stores each big-number as an **array of big-endian 64-bit qwords**. To
turn the raw bytes into a Python integer you must reverse the bytes *within each
8-byte group*, then read the whole buffer little-endian:

```python
def _bn(b):
    swapped = b"".join(b[i:i+8][::-1] for i in range(0, len(b), 8))
    return int.from_bytes(swapped, "little")
```

`int.from_bytes(b, "big")` gives the **wrong** number and was the single biggest
source of "it verifies for me but the block looks random" confusion. This
applies to the KV key components **and** to the modulus stored in the STFS cert.

### 2.2 Signing, in full

```python
e = struct.unpack_from(">I", kv, 0x298+4)[0]
n = _bn(kv[0x298+0x10 : 0x298+0x10+0x80])
p = _bn(kv[0x298+0x90 : 0x298+0x90+0x40])
q = _bn(kv[0x298+0xD0 : 0x298+0xD0+0x40])
d = pow(e, -1, (p-1)*(q-1))

DER   = bytes.fromhex("3021300906052b0e03021a05000414")   # SHA1 DigestInfo
di    = DER + hashlib.sha1(region).digest()               # region = header[0x22C:0x344]
block = b"\x00\x01" + b"\xff"*(0x80-len(di)-3) + b"\x00" + di
sig   = pow(int.from_bytes(block, "big"), d, n).to_bytes(0x80, "big")[::-1]
# sig -> header[0x1AC:0x22C]
```

No external crypto library is needed — this is pure `hashlib` + `struct` + big-int
`pow`. `xbt/crypto/XeCrypt.py`'s `XeCryptBnQwBeSigCreate` is **not** usable here:
it hard-asserts `cqw == 0x20` (2048-bit) because it's built for the bootloader
BeSig format, which STFS does not use.

---

## 3. The STFS `CON ` header layout

All offsets are from the start of the package (the header). The certificate +
signature occupy `0x04 .. 0x22C`; the STFS metadata/volume descriptor begins at
`0x22C`.

| Offset | Size  | Field |
|--------|-------|-------|
| 0x000  | 4     | magic `"CON "` (0x434F4E20) |
| 0x004  | 2     | public-key certificate size (0x1A8) |
| 0x006  | 5     | owner console ID |
| 0x00B  | 0x14  | owner console part number (e.g. `X858518-001`) |
| 0x01F  | 1     | console type |
| 0x020  | 8     | date string, `MM-DD-YY` |
| 0x028  | 4     | public exponent (0x10001) |
| 0x02C  | 0x80  | public modulus (**BnQw** — decode with `_bn`) |
| 0x0AC  | 0x100 | certificate signature (Microsoft's sig over the cert) |
| **0x1AC** | **0x80** | **console signature** (byte-reversed PKCS#1 v1.5, the thing we forge) |
| 0x22C  | 0x100 | license descriptors (0x10 entries × 0x10) — *start of signed region* |
| 0x32C  | 0x14  | header hash = `SHA1(header[0x344 : first_hash])` |
| 0x340  | 4     | header size |
| 0x344  | ...   | volume descriptor / content metadata / hash tables |

- `0x004 .. 0x1AC` (0x1A8 bytes) = the console **certificate** → copied verbatim
  from KV `0x9C8`.
- **Signed region** = `header[0x22C : 0x344]` = 0x118 bytes = license descriptors
  + header hash + header size.
- `first_hash = (header_size + 0xFFF) & 0xFFFFF000` (the header rounded up to the
  next 0x1000 boundary; where the first hash table lives).

Because the cert (0x04) and signature (0x1AC) sit **before** 0x22C, replacing
them never changes the bytes being signed. So resigning is self-contained: fix
the header hash, copy the cert, sign the region — in any order.

### 3.1 The header hash (0x32C)

```python
first_hash = (struct.unpack_from(">I", d, 0x340)[0] + 0xFFF) & 0xFFFFF000
d[0x32C:0x340] = hashlib.sha1(d[0x344:first_hash]).digest()
```

This definition is **identical** for a normal STFS package and for a GoD/SVOD CON
header (a GoD header is 0xB000 bytes; header size 0xAD0E rounds up to first_hash
0xB000, so the hash covers `0x344 .. 0xB000` = 0xACBC bytes — exactly what
`god.py` writes). That's why `stfs resign` works on GoD headers too.

---

## 4. How the scheme was cracked

The hard constraint: a *wrong* signing scheme (wrong region, wrong padding, wrong
salt) would still pass a naive sign→verify self-check — because you'd control both
sides with the same wrong assumption — yet fail on real hardware. A **false
positive** is worse than no implementation. So ground truth from real signed data
was required, not self-consistency.

**Step 1 — refuse to guess the salt.** The XeCrypt `BeSigCreate/Verify` primitives
use a 10-byte salt and an RC4-masked block, and only support 2048-bit keys.
Console keys are 1024-bit, so that path clearly didn't apply, and inventing a salt
was exactly the false-positive trap above.

**Step 2 — get authoritative offsets, not folklore.** The full `XECRYPT_KEYVAULT`
ctypes struct was already in the codebase (BSD-3 Xbox-360-Crypto). Reading field
offsets off the struct pinned the console cert (0x9C8) and private key (0x298)
with zero guessing.

**Step 3 — find real console-signed packages.** A bounded scan of the Xbox 360
backups turned up genuine `CON ` packages — Minecraft **saves** signed by a real
console (title 584111F7, console folder `E00013258D7953EE`).

**Step 4 — the golden coincidence.** Comparing each KV's `console_id`/modulus to
the saves' cert showed **two of the three J-Runner KVs are from the very console
(`83749636C8`) that signed those saves.** That means the KV holds the *matching
private key* for a signature we already possess → a golden test vector: sign the
save's own hash and demand the output equal the real signature **byte-for-byte**.

**Step 5 — locate the cert fields.** First attempt read the exponent as garbage
(`0x1219188`) — the fields were misaligned. Re-pinning them *backwards* from the
metadata start (0x22C: sig 0x1AC, cert-sig 0xAC, modulus 0x2C, exponent 0x28)
made the exponent read a clean `0x10001` and the part number a real `X858518-001`.
Alignment confirmed.

**Step 6 — recover the padded block.** RSA-encrypting the signature with the
cert's embedded public key produced a **random-looking** block in every naive
interpretation — no `0xBC` trailer (ruling out XeCrypt BeSig), no visible hash.

**Step 7 — brute-force the byte order.** Trying {plain, bswap64, full-reverse} ×
{modulus, signature}: the winner was **signature byte-reversed, big-endian**,
which yielded:

```
00 01 FF FF FF ... FF 00 <SHA1 DER> <20-byte hash>
```

— unmistakable **PKCS#1 v1.5**, and the trailing 20 bytes equaled
`SHA1(header[0x22C:0x344])`. Scheme identified: plain RSA-SHA1 PKCS#1 v1.5, no
salt, no masking, signature stored byte-reversed.

**Step 8 — golden verification.** Extract the console private key from the KV,
`d = e⁻¹ mod (p-1)(q-1)`, sign the PKCS#1 block, reverse the bytes → **exact match
to the real signature on both saves.** Then the full pipeline: resign a complete
save → output **byte-identical** to the original console-signed file; and from a
fully clobbered cert/sig state → still reconstructs the byte-identical original.

The one remaining wrinkle (Step 2's BnQw detail): the self-verify initially failed
because the cert modulus, like the key, is BnQw-encoded — `_bn(modulus)`, not
`int.from_bytes(...,'big')`. Fixed, and self-verify passes.

---

## 5. What `stfs resign` does

`xbt/xbox/stfs.py :: resign(package, kv, out=None, bind_console=False)`:

1. Accept `CON `, `LIVE`, or `PIRS`. If not already CON, **convert to CON**:
   flip the magic to `CON ` and reset the license descriptor at 0x22C to a single
   Unrestricted entry (`license[0] = FF·8`, the rest zero) — the state console
   content uses. (See §5.1 on why this is the *only* way to load a modified
   LIVE/PIRS package.)
2. Copy `kv[0x9C8 : 0xB70]` (0x1A8) → `header[0x04 : 0x1AC]` (the console
   certificate; this also writes the 0x1A8 cert size at 0x04).
3. `bind_console` (optional): stamp the KV's console id into the content metadata
   at 0x36C. Off by default so a same-console CON resign stays byte-identical to
   the console's own output.
4. Recompute the header hash at 0x32C over `header[0x344 : first_hash]` (capped at
   end-of-file, as Velocity does).
5. PKCS#1 v1.5 RSA-SHA1 sign `header[0x22C:0x344]` with the console private key;
   store byte-reversed at `header[0x1AC:0x22C]`.
6. Self-check: verify the new signature against the cert's own (BnQw) modulus;
   raise if it fails. Write in place unless an `out` path is given.

```bash
# resign a CON save/profile:
python xbt/cli/xboxtranspiler.py stfs resign MySave.bin KV_dec.bin
# convert a LIVE/PIRS package (DLC, title update, GoD header) to console CON:
python xbt/cli/xboxtranspiler.py stfs resign SomeDLC KV_dec.bin out.con --bind
```

### 5.1 Why LIVE/PIRS "signing" == LIVE→CON conversion

A `LIVE`/`PIRS` signature is 2048-bit RSA made with **Microsoft's private key**,
which no one outside Microsoft has — so a modified LIVE/PIRS package can **never**
carry a valid LIVE/PIRS signature again. That is a hard cryptographic fact, not a
tooling gap. What every tool (Velocity, Le Fluffie, Horizon) actually does is
re-sign the package as **CON** with *your* console key. The metadata from 0x22C
on is byte-for-byte identical across CON/LIVE/PIRS, so the conversion only
rewrites the `0x04..0x22C` signature block, the 4-byte magic, and the license
descriptor. This is fully implemented here and matches Velocity's `ResignHeader`
exactly (verified field-for-field: cert @KV 0x9C8, key n/p/q @KV 0x2A8/0x328/0x368,
sign region 0x22C+0x118, `EMSA3(SHA-1)` = PKCS#1 v1.5, 8-byte-swapped signature).

### 5.2 Scope / caveats

- The **CON signature** is verified **byte-exact** against real hardware
  signatures (§4). The **LIVE→CON conversion** is verified *structurally* — the
  output reads back as a valid CON package with files intact and a signature that
  verifies against the console cert — but end-to-end acceptance by a **stock
  retail** console can't be tested here (no console in the loop). On RGH/JTAG
  (where the KV came from) signature checks are patched out anyway.
- `resign` does **not** rebuild hash tables — it assumes the package is already
  hash-consistent (as produced by `stfs pack`, or an untouched original). If you
  modified file *contents*, run `stfs pack` first (it rehashes), then `resign`.
- GoD downloads from `iso2god` are **LIVE** and load on RGH as-is; `resign` will
  convert one's CON header to a console-bound CON with the same machinery.
- **PEC** (profile-embedded content) uses different offsets (sign region 0x23C,
  hash 0x228, console id 0x275) and is not yet handled — standard CON/LIVE/PIRS is.

---

## 6. Files

- `xbt/xbox/stfs.py` — reader + `pack` + **`resign`** (this document's subject).
- `xbt/crypto/XeCrypt.py` — source of the `XECRYPT_KEYVAULT` / `XECRYPT_RSAPRV_1024`
  struct definitions (BSD-3, GoobyCorp). Used here only for offsets; the signing
  is reimplemented dependency-free in `stfs.py`.
