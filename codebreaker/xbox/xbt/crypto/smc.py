"""Xbox 360 SMC (System Management Controller) — decrypt / encrypt / extract /
config parse.

The SMC is the little always-on microcontroller that runs fans, power, front
LEDs, temperature and the eject button. Its firmware lives in NAND, obfuscated
with the well-known "BuNy" rolling cipher; RGH/JTAG hacks patch the decrypted
SMC. This completes the NAND console layer (nand / bootchain / fuses / cpukey /
keyvault / hypervisor) with the one blob those didn't touch.

Flash-header fields (big-endian, documented layout):
  0x74 u32 SMC config offset      0x78 u32 SMC boot size      0x7C u32 SMC boot offset

Commands:
  smc decrypt <in> <out>          de-obfuscate an SMC image
  smc encrypt <in> <out>          re-obfuscate (inverse; round-trips)
  smc info    <smc>               size, checksum, motherboard guess
  smc extract <nand> <out>        pull the SMC firmware out of a NAND dump
  smc config  <nand|cfg>          parse the SMC config (fan override, temps)
"""

import struct
import sys

_KEY = (0x42, 0x75, 0x4E, 0x79)                          # "BuNy"

# motherboard byte (SMC version[1]) -> console revision
_MOBO = {
    0x00: "Xenon", 0x01: "Zephyr", 0x02: "Falcon/Opus", 0x03: "Jasper",
    0x04: "Trinity/Corona?", 0x05: "Corona", 0x06: "Winchester",
}


def smc_decrypt(data: bytes) -> bytes:
    key = list(_KEY)
    out = bytearray(len(data))
    for i in range(len(data)):
        j = data[i]
        mod = (j * 0xFB) & 0xFFFFFFFF
        out[i] = j ^ (key[i & 3] & 0xFF)
        key[(i + 1) & 3] = (key[(i + 1) & 3] + mod) & 0xFFFFFFFF
        key[(i + 2) & 3] = (key[(i + 2) & 3] + (mod >> 8)) & 0xFFFFFFFF
    return bytes(out)


def smc_encrypt(data: bytes) -> bytes:
    key = list(_KEY)
    out = bytearray(len(data))
    for i in range(len(data)):
        j = data[i] ^ (key[i & 3] & 0xFF)
        mod = (j * 0xFB) & 0xFFFFFFFF
        out[i] = j
        key[(i + 1) & 3] = (key[(i + 1) & 3] + mod) & 0xFFFFFFFF
        key[(i + 2) & 3] = (key[(i + 2) & 3] + (mod >> 8)) & 0xFFFFFFFF
    return bytes(out)


def smc_version(dec: bytes):
    """The 4-byte SMC version lives near the tail of the decrypted image
    (…marked by the 0x10-terminated record). Returns (major, mobo, build, rev)
    or None if not located."""
    # the version record is 4 bytes whose first byte is a small length/tag;
    # scan the last 0x400 bytes for the canonical 4-byte version block.
    tail = dec[-0x400:] if len(dec) > 0x400 else dec
    for o in range(len(tail) - 4):
        b = tail[o:o + 4]
        # heuristic: version bytes are all < 0x40 and mobo byte is known
        if b[0] and b[0] < 0x10 and b[1] in _MOBO and b[2] < 0x40:
            return tuple(b)
    return None


def extract_from_nand(nand: bytes):
    """Return (smc_bytes, offset, length) from a NAND dump via the flash header."""
    if len(nand) < 0x80:
        raise ValueError("not a NAND image")
    length = struct.unpack_from(">I", nand, 0x78)[0]
    offset = struct.unpack_from(">I", nand, 0x7C)[0]
    if not (0 < length <= 0x4000) or offset + length > len(nand):
        raise ValueError("SMC boot fields invalid (off=0x%X len=0x%X)" % (offset, length))
    return nand[offset:offset + length], offset, length


def parse_config(nand_or_cfg: bytes):
    """SMC config = a checksummed record of fan/temperature/power overrides. If
    given a whole NAND, pull the config via the header (0x74). Returns a dict."""
    cfg = nand_or_cfg
    if len(nand_or_cfg) >= 0x80 and nand_or_cfg[0:2] == b"\xff\x4f":
        off = struct.unpack_from(">I", nand_or_cfg, 0x74)[0]
        cfg = nand_or_cfg[off:off + 0x2A0]
    d = {
        "checksum": struct.unpack_from(">I", cfg, 0x00)[0] if len(cfg) >= 4 else None,
        # fan-speed override: 0 = auto; else a percent-ish 0..0x7F with 0x80 set
        "fan_override_cpu": cfg[0x08] if len(cfg) > 0x08 else None,
        "fan_override_gpu": cfg[0x09] if len(cfg) > 0x09 else None,
        # temperature targets (raw SMC units) at the documented config offsets
        "temp_cpu_target": cfg[0x10] if len(cfg) > 0x10 else None,
        "temp_gpu_target": cfg[0x11] if len(cfg) > 0x11 else None,
        "temp_ram_target": cfg[0x12] if len(cfg) > 0x12 else None,
        "raw_len": len(cfg),
    }
    return d


DESCRIPTION = "Xbox 360 SMC: decrypt/encrypt/info/extract/config (fans, temps, power)"


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print(__doc__.split("Commands:")[1].rstrip())
        return
    op = a[0]
    if op in ("decrypt", "encrypt"):
        data = open(a[1], "rb").read()
        out = smc_decrypt(data) if op == "decrypt" else smc_encrypt(data)
        open(a[2], "wb").write(out)
        print("%sed %d bytes -> %s" % (op, len(out), a[2])); return
    if op == "extract":
        nand = open(a[1], "rb").read()
        smc, off, ln = extract_from_nand(nand)
        open(a[2], "wb").write(smc)
        print("SMC @0x%X len 0x%X -> %s" % (off, ln, a[2])); return
    if op == "info":
        data = open(a[1], "rb").read()
        dec = smc_decrypt(data)
        ver = smc_version(dec)
        print("size        : %d bytes" % len(data))
        print("checksum(dec): 0x%08X" % (sum(dec) & 0xFFFFFFFF))
        if ver:
            print("version     : %d.%d.%d.%d  (motherboard: %s)" % (
                ver[0], ver[1], ver[2], ver[3], _MOBO.get(ver[1], "?")))
        else:
            print("version     : (not located)")
        return
    if op == "config":
        d = parse_config(open(a[1], "rb").read())
        print("SMC config (%d bytes)  checksum=0x%08X" % (d["raw_len"], d["checksum"] or 0))
        print("  fan override  cpu=%s gpu=%s" % (d["fan_override_cpu"], d["fan_override_gpu"]))
        print("  temp targets  cpu=%s gpu=%s ram=%s" % (
            d["temp_cpu_target"], d["temp_gpu_target"], d["temp_ram_target"]))
        return
    print("unknown op: %s" % op)


if __name__ == "__main__":
    main()
