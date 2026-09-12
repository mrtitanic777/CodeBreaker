"""Brute-force key scanner — scan console memory regions for the Kerberos Online Key.

Reads the saved PA-TIMESTAMP from the LIVE server debug dir and tries every
16-byte aligned block in a memory region as the RC4-HMAC-MD5 key.
"""

import os
import struct
import hmac
import hashlib
from typing import Optional, List, Tuple


def rc4(key: bytes, data: bytes) -> bytes:
    """RC4 encrypt/decrypt."""
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + key[i % len(key)]) % 256
        S[i], S[j] = S[j], S[i]
    i = j = 0
    result = []
    for byte in data:
        i = (i + 1) % 256
        j = (j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        result.append(byte ^ S[(S[i] + S[j]) % 256])
    return bytes(result)


def try_key_against_pa_timestamp(key: bytes, encrypted_ts: bytes, 
                                  key_usage: int = 1) -> Tuple[bool, bytes]:
    """Try a 16-byte key against an RC4-HMAC-MD5 encrypted PA-TIMESTAMP.
    
    MS RC4-HMAC-MD5 decryption:
    1. K1 = HMAC-MD5(key, usage_le32)
    2. K3 = HMAC-MD5(K1, checksum)
    3. plaintext = RC4(K3, ciphertext)
    4. Verify: HMAC-MD5(K1, plaintext) == checksum
    
    Returns (success, plaintext_or_empty)
    """
    if len(encrypted_ts) < 24:
        return False, b''
    
    checksum = encrypted_ts[:16]
    ciphertext = encrypted_ts[16:]
    
    # K1 = HMAC-MD5(key, key_usage as LE uint32)
    k1 = hmac.new(key, struct.pack('<I', key_usage), hashlib.md5).digest()
    
    # K3 = HMAC-MD5(K1, checksum)
    k3 = hmac.new(k1, checksum, hashlib.md5).digest()
    
    # Decrypt
    plaintext = rc4(k3, ciphertext)
    
    # Verify checksum
    expected = hmac.new(k1, plaintext, hashlib.md5).digest()
    
    if checksum == expected:
        return True, plaintext
    
    return False, b''


def scan_memory_for_kerberos_key(probe, pa_timestamp: bytes,
                                  regions: list = None,
                                  key_usage: int = 1) -> List[dict]:
    """Scan memory regions for the 16-byte key that decrypts the PA-TIMESTAMP.
    
    Args:
        probe: NexGen console probe
        pa_timestamp: The encrypted PA-ENC-TIMESTAMP bytes
        regions: List of (address, size) tuples to scan. Defaults to common XAM regions.
        key_usage: Kerberos key usage number (1 for PA-ENC-TIMESTAMP)
    
    Returns list of matches: [{address, key_hex, plaintext}]
    """
    if regions is None:
        regions = [
            (0x8C000000, 0x2000),   # XAM heap header + crypto workspace
            (0x8C010000, 0x1000),   # XAM allocation pool
            (0x81B4B300, 0x400),    # XAM security context area
            (0x81B4D300, 0x600),    # XAM data region
            (0x81AA0900, 0x200),    # XAM security module
        ]
    
    matches = []
    
    for region_addr, region_size in regions:
        data = probe.read_memory(region_addr, region_size)
        if not data:
            continue
        
        # Try every 4-byte aligned offset
        for offset in range(0, len(data) - 16, 4):
            candidate = data[offset:offset + 16]
            
            # Skip all-zero or low-entropy blocks
            if candidate.count(0) > 8:
                continue
            if len(set(candidate)) < 6:
                continue
            
            success, plaintext = try_key_against_pa_timestamp(
                candidate, pa_timestamp, key_usage)
            
            if success:
                matches.append({
                    "address": region_addr + offset,
                    "key_hex": candidate.hex().upper(),
                    "plaintext": plaintext,
                    "confounder": plaintext[:8].hex().upper() if len(plaintext) >= 8 else "",
                    "timestamp_data": plaintext[8:].decode('ascii', errors='replace') if len(plaintext) > 8 else "",
                })
    
    return matches


def scan_wide_for_kerberos_key(probe, pa_timestamp: bytes) -> List[dict]:
    """Wide scan across all known XAM memory regions.
    
    This is slower but covers more ground.
    """
    regions = [
        # XAM heap and crypto workspace
        (0x8C000000, 0x4000),
        (0x8C010000, 0x2000),
        # XAM code/data sections (where keys might be computed)
        (0x81670000, 0x1000),   # XAM base
        (0x81A80000, 0x2000),   # XAM high region
        (0x81AA0000, 0x1000),   # Security module area
        (0x81B40000, 0x2000),   # XAM runtime data
        (0x81B4B000, 0x1000),   # Security context region
        (0x81B4D000, 0x1000),   # Extended data
        (0x81B4F000, 0x1000),   # Key material region
        # Kernel security areas
        (0x801B0E00, 0x400),    # PROTDATA security section
        (0x801A6100, 0x200),    # Security state
        # ETX buffer (session tokens)
        (0x3A060000, 0x2000),   # ETX data sections
    ]
    
    return scan_memory_for_kerberos_key(probe, pa_timestamp, regions)
