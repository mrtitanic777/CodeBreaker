"""CPU key validation with ECD (Error Correction and Detection).

Ported from SK.Xbox360.CPUKey by Stelio Kontos.
Provides rigorous validation: length, non-zero, Hamming weight (0x35),
and ECD bit verification using the Xbox 360's LFSR-based algorithm
with the magic constant 0x360325.
"""


VALID_HAMMING_WEIGHT = 0x35
ECD_MAGIC = 0x360325


def compute_ecd(data: bytearray) -> bytearray:
    """Compute and set ECD bits in a 16-byte CPU key. Modifies in place.

    The ECD algorithm uses a customized LFSR variant:
      - Bits 0-105: data payload, processed with XOR feedback
      - Bits 106-126: ECD correction bits (21 bits)
      - Bit 127: overall parity bit
    """
    buf = bytearray(data)
    acc1 = 0
    acc2 = 0

    for i in range(128):
        b_tmp = buf[i >> 3]
        dw_tmp = (b_tmp >> (i & 7)) & 1

        if i < 0x6A:
            acc1 ^= dw_tmp
            if acc1 & 1:
                acc1 ^= ECD_MAGIC
            acc2 ^= dw_tmp
        elif i < 0x7F:
            if dw_tmp != (acc1 & 1):
                buf[i >> 3] = ((1 << (i & 7)) ^ (b_tmp & 0xFF)) & 0xFF
            acc2 ^= (acc1 & 1)
        else:
            if dw_tmp != acc2:
                buf[0xF] = ((0x80 ^ b_tmp) & 0xFF)

        acc1 >>= 1

    return buf


def verify_ecd(data: bytes) -> bool:
    """Verify the ECD bits in a 16-byte CPU key."""
    if len(data) != 16:
        return False
    recomputed = compute_ecd(bytearray(data))
    return recomputed == bytearray(data)


def compute_hamming_weight(data: bytes) -> int:
    """Compute the Hamming weight of a CPU key, excluding ECD bits.

    The last 22 bits (bits 106-127) are ECD and excluded from the count.
    Valid CPU keys have a Hamming weight of exactly 0x35 (53).
    """
    if len(data) != 16:
        return -1

    # Convert to two 64-bit integers (big-endian)
    hi = int.from_bytes(data[0:8], 'big')
    lo = int.from_bytes(data[8:16], 'big')

    # ECD mask: exclude bits 106-127 (the top 22 bits of the 128-bit key)
    # In big-endian layout, bits 106-127 are the top 22 bits of the high qword
    # Mask for low 64 bits: all bits count
    # Mask for high 64 bits: bottom 42 bits count (64 - 22 = 42)
    ecd_mask_hi = (1 << 42) - 1  # 0x3FFFFFFFFFF

    return bin(hi & ecd_mask_hi).count('1') + bin(lo).count('1')


def verify_hamming_weight(data: bytes) -> bool:
    """Verify the Hamming weight of a CPU key is exactly 0x35."""
    return compute_hamming_weight(data) == VALID_HAMMING_WEIGHT


def validate_cpukey(key_hex: str) -> dict:
    """Full validation of a CPU key hex string.

    Returns a dict with:
        valid: bool — overall validity
        length_ok: bool
        non_zero: bool
        hamming_ok: bool
        hamming_weight: int
        ecd_ok: bool
        errors: list of error strings
    """
    result = {
        "valid": False, "length_ok": False, "non_zero": False,
        "hamming_ok": False, "hamming_weight": 0,
        "ecd_ok": False, "errors": []
    }

    # Clean input
    key_hex = key_hex.strip().replace(" ", "").replace("-", "")

    # Length check
    if len(key_hex) != 32:
        result["errors"].append(f"Length {len(key_hex)} != 32 hex chars")
        return result
    result["length_ok"] = True

    # Hex parse
    try:
        key_bytes = bytes.fromhex(key_hex)
    except ValueError:
        result["errors"].append("Invalid hex characters")
        return result

    # Non-zero check
    if key_bytes == b"\x00" * 16:
        result["errors"].append("Key is all zeros")
        return result
    result["non_zero"] = True

    # Hamming weight
    hw = compute_hamming_weight(key_bytes)
    result["hamming_weight"] = hw
    if hw != VALID_HAMMING_WEIGHT:
        result["errors"].append(f"Hamming weight {hw} != {VALID_HAMMING_WEIGHT} (0x{VALID_HAMMING_WEIGHT:X})")
    else:
        result["hamming_ok"] = True

    # ECD verification
    if verify_ecd(key_bytes):
        result["ecd_ok"] = True
    else:
        result["errors"].append("ECD bits invalid")

    result["valid"] = result["hamming_ok"] and result["ecd_ok"]
    return result


def generate_cpukey() -> str:
    """Generate a random valid CPU key with correct Hamming weight and ECD."""
    import os
    while True:
        key = bytearray(os.urandom(16))
        if compute_hamming_weight(bytes(key)) == VALID_HAMMING_WEIGHT:
            key = compute_ecd(key)
            return key.hex().upper()


DESCRIPTION = "validate or generate an Xbox 360 CPU key (ECD + Hamming weight)"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: cpukey <hexkey>   |   cpukey gen"); return
    if a[0] in ("gen", "generate"):
        print(generate_cpukey()); return
    r = validate_cpukey(a[0])
    for k, v in r.items():
        print("  %-18s %s" % (k, v))


if __name__ == "__main__":
    main()
