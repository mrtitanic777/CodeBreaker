"""Hypervisor analysis and manipulation tools."""

from pathlib import Path
from struct import unpack_from, pack

def clean_hypervisor(dumped: bytes, base: bytes) -> bytes:
    """Remove RGH patches from a dumped hypervisor using the base/offline version.
    
    Based on Hypervisor Manager's CleanHypervisor function.
    """
    cleaned = bytearray(dumped[:0x40000])

    # Copy clean sections from base HV
    ranges = [
        (0x08, 0x88, 0x0C),      # Boot info
        (0x34, 0x34, 0x0C),      # Version info
        (0x70, 0x70, 0x04),      # Flags
        (0x78, 0x78, 0xFF88),    # Main code segment 1
        (0x10008, 0x10008, 0x18), # Security data
        (0x10130, 0x10130, 0x6190), # Code segment 2
        (0x11018, 0x11018, 0x52BB), # Code segment 3
        (0x16F00, 0x16F00, 0x29100), # Code + data segment 4
    ]

    for (src_off, dst_off, size) in ranges:
        if src_off + size <= len(base) and dst_off + size <= len(cleaned):
            cleaned[dst_off:dst_off + size] = base[src_off:src_off + size]

    return bytes(cleaned)

def extract_hv_info(data: bytes) -> dict:
    """Extract key information from a hypervisor image."""
    info = {}
    if len(data) < 0x40000:
        return info

    info["size"] = len(data)

    # Version info at offset 0x02
    if len(data) >= 0x10:
        info["build"] = unpack_from(">H", data, 0x02)[0]
        info["qfe"] = unpack_from(">H", data, 0x04)[0]

    # Encryption keys (if present — these are at 0x10100 in decrypted HV)
    if len(data) >= 0x10130:
        info["white_key"] = data[0x10100:0x10110]
        info["aes_key"] = data[0x10110:0x10120]
        info["hash_key"] = data[0x10120:0x10130]
        info["keys_are_55"] = all(b == 0x55 for b in info["white_key"] + info["aes_key"] + info["hash_key"])

    # Security hash at 0x100F0
    if len(data) >= 0x10100:
        info["security_hash_100f0"] = data[0x100F0:0x10100]

    # Hash table address at 0x10878
    if len(data) >= 0x108A8:
        info["hash_table_entries"] = []
        for i in range(6):
            addr = unpack_from(">I", data, 0x10878 + i * 8)[0]
            size = unpack_from(">I", data, 0x10878 + i * 8 + 4)[0]
            info["hash_table_entries"].append((addr, size))

    return info

def calc_100f0_hash(hv_data: bytes, ecc_data: bytes) -> bytes:
    """Calculate the security hash at 0x100F0.
    
    Based on Hypervisor Manager's Calculate100F0 function.
    """
    import hashlib
    h = hashlib.sha1()

    for i in range(6):
        addr_raw = unpack_from(">I", hv_data, 0x10878 + i * 8)[0]
        size_raw = unpack_from(">I", hv_data, 0x10878 + i * 8 + 4)[0]

        addr = (addr_raw + 0x7F) & 0xFFFFFFFFFFFFFF80
        size = size_raw & 0xFFFFFF80

        if addr < size:
            ecc_offset = addr >> 6
            ecc_size = (size - addr) >> 6
            if ecc_offset + ecc_size <= len(ecc_data):
                h.update(ecc_data[ecc_offset:ecc_offset + ecc_size])

    return h.digest()[:0x10]

def decrypt_challenge_payload(enc_data: bytes, fbl_key: bytes) -> bytes:
    """Decrypt a code challenge payload.
    
    Based on Hypervisor Manager's DecryptChallengePayload function.
    """
    from .XeCrypt import XeCryptHmacSha, XeCryptRc4

    payload_hash = enc_data[0x10:0x20]
    hmac_result = XeCryptHmacSha(fbl_key, payload_hash)[:0x10]
    data_section = enc_data[0x20:]

    decrypted = bytearray(enc_data)
    decrypted[0x20:] = XeCryptRc4.new(hmac_result).decrypt(data_section)
    return bytes(decrypted)

def extract_challenge_ranges(dec_challenge: bytes) -> dict:
    """Extract hash address ranges from a decrypted code challenge.
    
    Based on Hypervisor Manager's OutputDigestRanges function.
    """
    result = {"hash1_ranges": [], "hash2_ranges": []}

    if len(dec_challenge) <= 0x3B0:
        return result

    # Hash 1 ranges (7 entries at 0x3B0)
    for i in range(7):
        raw = unpack_from(">q", dec_challenge, 0x3B0 + i * 8)[0]
        addr = (raw >> 16) & 0xFFFFF
        size = raw & 0xFFFF
        result["hash1_ranges"].append({"address": addr, "size": size})

    # Hash 2 ranges (13 entries at 0x348)
    for i in range(13):
        raw = unpack_from(">q", dec_challenge, 0x348 + i * 8)[0]
        entry = {"raw": raw}
        if raw & 0x8000:
            entry["type"] = "ECC"
            entry["address"] = (raw >> 22) & 0xFFF
            entry["size"] = (raw & 0x3FF) << 1
        elif (raw & 0x7F) >= 16:
            entry["type"] = "HV_ENC"
            entry["address"] = (raw >> 16) & 0x3FFFF
            entry["size"] = raw & 0x7F
        else:
            entry["type"] = "HV_DEC"
            entry["address"] = raw >> 16
            entry["size"] = raw & 0x7F
        result["hash2_ranges"].append(entry)

    return result

def generate_hash1_results(clean_hv: bytes, salts: list) -> list:
    """Generate all Hash 1 results for all salts.
    
    Based on Hypervisor Manager's GenerateHash1Results function.
    Returns list of (salt, hash_result) tuples.
    """
    import hashlib
    results = []

    for salt in salts:
        h = hashlib.sha1()
        h.update(salt[:0x10])
        h.update(clean_hv[0x34:0x34 + 0x40])
        h.update(clean_hv[0x78:0x78 + 0xFF88])
        h.update(clean_hv[0x100C0:0x100C0 + 0x40])
        h.update(clean_hv[0x10350:0x10350 + 0x5F70])
        h.update(clean_hv[0x16EA0:0x16EA0 + 0x9160])
        h.update(clean_hv[0x20000:0x20000 + 0xFFFF])
        h.update(clean_hv[0x30000:0x30000 + 0xFFFF])

        digest = h.digest()
        hash_result = digest[0x0E:0x0E + 0x06]
        results.append((salt, hash_result))

    return results

def load_salt_array(salt_file: str) -> list:
    """Load the 256 salt array from a binary file."""
    data = Path(salt_file).read_bytes()
    salts = []
    for i in range(256):
        salts.append(data[i * 0x10:(i + 1) * 0x10])
    return salts


DESCRIPTION = "analyze an Xbox 360 hypervisor image (info; clean patches vs a base)"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: hypervisor info <hv.bin>  |  hypervisor clean <dumped> <base> <out>"); return
    op = a[0]
    if op == "info" and len(a) > 1:
        for k, v in extract_hv_info(open(a[1], "rb").read()).items():
            print("  %-20s %s" % (k, v.hex().upper() if isinstance(v, (bytes, bytearray)) else v))
    elif op == "clean" and len(a) > 3:
        out = clean_hypervisor(open(a[1], "rb").read(), open(a[2], "rb").read())
        open(a[3], "wb").write(out)
        print("cleaned HV (%d bytes) -> %s" % (len(out), a[3]))
    else:
        print("usage: hypervisor info <hv.bin>  |  hypervisor clean <dumped> <base> <out>"); sys.exit(2)


if __name__ == "__main__":
    main()
