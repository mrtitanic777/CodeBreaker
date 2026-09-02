"""Xbox 360 System Link network tools.

Based on Emma's research notes on system link encryption.
Packet format, key derivation, 3DES-CBC encryption/decryption.
"""

import struct
from typing import Optional

from XeCrypt import (
    XeCryptSha, XeCryptHmacSha, XeCryptDes3, XeCryptDesParity, XeCryptRc4
)

# Constants
SYSLINK_PORT = 3074
BROADCAST_SRC_IP = "0.0.0.1"
BROADCAST_DST_IP = "255.255.255.255"

class SystemLinkKeys:
    """Holds the three derived keys for system link encryption."""
    def __init__(self, hmac_key: bytes, des3_key: bytes, cbc_feed: bytes):
        self.hmac_key = hmac_key    # 0x10 bytes — HMAC-SHA1 validation
        self.des3_key = des3_key    # 0x18 bytes — 3DES encryption
        self.cbc_feed = cbc_feed    # 0x08 bytes — CBC IV generation

    def __repr__(self):
        return (f"SystemLinkKeys(\n"
                f"  hmac_key={self.hmac_key.hex().upper()},\n"
                f"  des3_key={self.des3_key.hex().upper()},\n"
                f"  cbc_feed={self.cbc_feed.hex().upper()}\n)")

def derive_keys_xbox(lan_key: bytes, roamable_obf_key: bytes) -> SystemLinkKeys:
    """Derive system link encryption keys (Xbox 360 native path).
    
    Uses HMAC-SHA1 with roamable obfuscation key.
    """
    key_buffer = bytearray(0x3C)

    for i in range(3):
        config_buf = bytes([i]) + lan_key[:0x10]
        digest = XeCryptHmacSha(roamable_obf_key, config_buf)
        key_buffer[i * 0x14:(i + 1) * 0x14] = digest[:0x14]

    hmac_key = bytes(key_buffer[0x00:0x10])
    des3_key = XeCryptDesParity(bytes(key_buffer[0x10:0x28]))
    cbc_feed = bytes(key_buffer[0x28:0x30])

    return SystemLinkKeys(hmac_key, des3_key, cbc_feed)

def derive_keys_crossplatform(lan_key: bytes, xplat_syslink_key: bytes) -> SystemLinkKeys:
    """Derive system link encryption keys (cross-platform / GfWL path).
    
    Encrypts LAN key with AES, then SHA-1 hashes.
    """
    from XeCrypt import XeCryptAes

    # Encrypt the LAN key with cross-platform system link key
    enc_key = XeCryptAes.new(xplat_syslink_key).encrypt(lan_key[:0x10])

    key_buffer = bytearray(0x3C)

    for i in range(3):
        config_buf = bytes([i]) + enc_key
        digest = XeCryptSha(config_buf)
        key_buffer[i * 0x14:(i + 1) * 0x14] = digest[:0x14]

    hmac_key = bytes(key_buffer[0x00:0x10])
    des3_key = XeCryptDesParity(bytes(key_buffer[0x10:0x28]))
    cbc_feed = bytes(key_buffer[0x28:0x30])

    return SystemLinkKeys(hmac_key, des3_key, cbc_feed)

def calculate_iv(cbc_feed: bytes, seed: int) -> bytes:
    """Calculate the 3DES CBC initialization vector from seed and feed.
    
    Based on Emma's research: multiply feed halves by expanded seed, XOR results.
    """
    seed_ext = ((seed & 0xFFFF) << 16) | (seed & 0xFFFF)

    feed_lo = struct.unpack("<I", cbc_feed[0:4])[0]
    feed_hi = struct.unpack("<I", cbc_feed[4:8])[0]

    k0 = (feed_lo * seed_ext) & 0xFFFFFFFFFFFFFFFF
    k1 = (feed_hi * seed_ext) & 0xFFFFFFFFFFFFFFFF

    iv_lo = ((k1 >> 32) & 0xFFFFFFFF) ^ (k0 & 0xFFFFFFFF)
    iv_hi = (k1 & 0xFFFFFFFF) ^ ((k0 >> 32) & 0xFFFFFFFF)

    return struct.pack("<II", iv_lo, iv_hi)

def decrypt_packet(packet: bytes, keys: SystemLinkKeys) -> Optional[dict]:
    """Decrypt a system link broadcast packet.
    
    Returns dict with header_flags, decrypted_data, footer info, or None on failure.
    """
    if len(packet) < 0x1D:  # Minimum: 4 header + 0x19 footer
        return None

    result = {}

    # Header flags
    result["header_flags"] = struct.unpack("<I", packet[0:4])[0]

    # Footer is at the end — 0x19 bytes
    footer_start = len(packet) - 0x19
    footer = packet[footer_start:]

    result["title_id"] = struct.unpack("<I", footer[0:4])[0]
    result["title_version"] = struct.unpack("<I", footer[4:8])[0]
    result["xam_version"] = struct.unpack("<I", footer[8:12])[0]

    enc_len_div8 = footer[0x0C]
    enc_len = enc_len_div8 * 8
    seed = struct.unpack("<H", footer[0x0D:0x0F])[0]
    hmac_checksum = footer[0x0F:0x0F + 0x0A]

    result["encrypted_length"] = enc_len
    result["seed"] = seed

    # Decrypt the encrypted portion
    if enc_len > 0 and enc_len <= (footer_start - 4):
        iv = calculate_iv(keys.cbc_feed, seed)
        cipher = XeCryptDes3.new(keys.des3_key, XeCryptDes3.MODE_CBC, iv)
        encrypted_data = packet[4:4 + enc_len]
        result["decrypted_data"] = cipher.decrypt(encrypted_data)
    else:
        result["decrypted_data"] = b""

    # Unencrypted data between encrypted section and footer
    unenc_start = 4 + enc_len
    if unenc_start < footer_start:
        result["unencrypted_data"] = packet[unenc_start:footer_start]
    else:
        result["unencrypted_data"] = b""

    result["hmac"] = hmac_checksum
    return result

def encrypt_packet(data: bytes, keys: SystemLinkKeys, title_id: int,
                   title_version: int, xam_version: int, seed: int = None) -> bytes:
    """Create an encrypted system link broadcast packet."""
    import os

    if seed is None:
        seed = struct.unpack("<H", os.urandom(2))[0]

    # Pad data to 8-byte boundary for 3DES
    pad_len = (8 - (len(data) % 8)) % 8
    padded_data = data + (b"\x00" * pad_len)

    # Encrypt
    iv = calculate_iv(keys.cbc_feed, seed)
    cipher = XeCryptDes3.new(keys.des3_key, XeCryptDes3.MODE_CBC, iv)
    encrypted = cipher.encrypt(padded_data)

    # Build packet
    header_flags = 0x00000000  # TODO: proper flags
    enc_len_div8 = len(encrypted) // 8

    # Build footer
    footer = struct.pack("<III", title_id, title_version, xam_version)
    footer += struct.pack("B", enc_len_div8)
    footer += struct.pack("<H", seed)

    # Build packet without HMAC
    packet = struct.pack("<I", header_flags) + encrypted + footer

    # Calculate HMAC-SHA1 (over specific parts — simplified here)
    hmac_data = XeCryptHmacSha(keys.hmac_key, packet)[:0x0A]

    # Append HMAC
    packet += hmac_data

    return packet
