// cb/formats/xbox/crypto/mem_crypto.hpp -- the Xbox 360 hypervisor's memory encryption, its SRAM checksums, and what the console's
// challenge-response (XOSC) is made of.
//
//     MemoryCrypto mem(white_key, aes_key, hash_key);          the three 16-byte keys of a console's memory encryption engine
//     Bytes enc = mem.encrypt(hv_dec, 0);                      a decrypted image at hypervisor address 0, as it sits in RAM
//     Bytes dec = mem.decrypt(hv_enc, 0);
//     Bytes sums = mem.sram_checksums(hv_dec, enc);            the SRAM checksum bytes of the image, 2 per 0x80-byte page
//     Bytes h1 = mem.hash_1_digest(hv_dec, salt);              the XOSC hashes of a 0x40000-byte hypervisor image
//     Bytes blob = mem.key_blob(nonce);                        the key blob the keys travel in, sealed to the master public key
//
// Memory is "encrypted" the way the console's engine does it: every 16-byte block at address A is xored with a tweak made from the
// white key and A, run through AES-128 in the DECRYPT direction (the engine's encryption is AES decryption, so the names are the
// engine's, not the cipher's), and xored again. The tweak is the white key multiplied by the block number in GF(2), the bits above
// the 128th folded back in with a CRC-16 table. An address inside the 0x40000-byte hypervisor image (0 through 0x40000) is
// extended, the way the console maps it, with `fix_address`.
//
// Credit: the scheme was worked out by the Xbox 360 scene (tydye81, teir1plus2, no-op, juv; the Hypervisor Manager and SciresM's
// AES gist are what the tools were written from).
//
// Errors: a key or a salt of the wrong size, a size that is not a whole number of blocks or pages, an address range outside the
// image: UsageError. An image too short for the hashes: FormatError.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "cb/core/bytes.hpp"

namespace cb::formats::xbox::crypto {

inline constexpr std::size_t kMemKeySize = 16;            // the white, AES and hash keys
inline constexpr std::size_t kSramChecksumPage = 0x80;    // bytes of memory per SRAM checksum
inline constexpr std::size_t kHypervisorSize = 0x40000;   // the hypervisor image the hashes read
inline constexpr std::size_t kKeyBlobSize = 0x80;
inline constexpr std::size_t kKeyBlobNonceSize = 20;
inline constexpr std::uint32_t kHvexAddress = 0x01B5;
inline constexpr std::uint32_t kHv17559TableAddress = 0x10878;

class MemoryCrypto {
public:
    // Each key is 16 bytes (UsageError otherwise).
    MemoryCrypto(ByteView white_key, ByteView aes_key, ByteView hash_key);

    // The address the engine uses for a block at `address`: one inside the hypervisor image (0 .. 0x40000) is placed in its 4 GB
    // window (bit 33 up, one 64 KB page per step); any other is left as it is.
    static std::uint64_t fix_address(std::uint64_t address);

    // The 16-byte tweak of a block at `address` (the address is used as it is; `fix_address` is the caller's step).
    std::array<std::uint8_t, 16> tweak(std::uint64_t address) const;

    // One block: tweak, AES (decrypt for "encrypt", encrypt for "decrypt"), tweak.
    Bytes encrypt_block(ByteView block, std::uint64_t address) const;
    Bytes decrypt_block(ByteView block, std::uint64_t address) const;

    // `data` (a whole number of 16-byte blocks) as it sits at hypervisor address `address`: block i is at fix_address(address+i*16).
    Bytes encrypt(ByteView data, std::uint64_t address) const;
    Bytes decrypt(ByteView data, std::uint64_t address) const;
    // The SRAM checksums of `data` once encrypted at `address` (a multiple of 0x80 bytes).
    Bytes encrypt_and_checksums(ByteView data, std::uint64_t address) const;

    // The 16-bit checksum of one 0x80-byte page: the big-endian words xored, each rotated right by an amount that grows by one every
    // four words (1 for the first four). A rotation of 16 is the word as it is.
    static std::uint16_t sram_checksum(ByteView page);
    // The checksums, 2 bytes big-endian per page, of memory `dec` and its encrypted form `enc` (same size, a multiple of 0x80):
    // each page's words are xored with the encrypted ones masked by the hash key first.
    Bytes sram_checksums(ByteView dec, ByteView enc) const;
    // The checksums of the part of a hypervisor image that SRAM offset and size (2 bytes of SRAM per 0x80 of memory) name.
    Bytes checksum_chunk(ByteView hv_dec, ByteView hv_enc, std::size_t sram_offset, std::size_t sram_size) const;
    static std::size_t sram_to_hv(std::size_t sram) { return sram / 2 * kSramChecksumPage; }

    // The checksums of the whole 0x40000-byte image `hv_dec` (FormatError if it is not that size or larger).
    Bytes calc_sram(ByteView hv_dec) const;

    // The first XOSC hash: 6 bytes (bytes 14..19 of a SHA-1 over the salt and seven regions of the image). `salt` is at most 16
    // bytes of its first 16 (a longer one is cut). FormatError for an image under 0x40000 bytes.
    Bytes hash_1_digest(ByteView hv_dec, ByteView salt) const;
    // The second: a SHA-1 (20 bytes) over the encrypted pieces of the image and their checksums, and the salt repeated eight times
    // as memory at `hvex_addr` << 16 in the 0x7C00000000 window.
    Bytes hash_2_digest(ByteView hv_dec, ByteView salt, std::uint32_t hvex_addr) const;

    // The 0x80-byte key blob for a 20-byte nonce (UsageError for another size): nonce, the SHA-1 of nothing, a flag, the three keys,
    // masked twice with SHA-1 streams keyed by themselves, then raised to the master public key's power (the console's RSA).
    Bytes key_blob(ByteView nonce) const;

    // The 16 bytes that are the SHA-1 prefix of the checksums of the six RAM ranges the table at `table_addr` in the image lists
    // (pairs of big-endian start and stop addresses, aligned inward to 0x80). FormatError when the table or a range is outside the
    // image.
    Bytes calc_100f0(ByteView hv_dec, std::uint32_t table_addr) const;

private:
    std::array<std::uint8_t, 16> white_{}, aes_{}, hash_{};
};

// The SHA-1 stream masking the secure-engine keys: `buffer`[offset, offset+length) is xored in pieces of 20 bytes with
// SHA-1(key || big-endian counter), the counter starting at 0. `key` is read first, so it may be a part of `buffer` that has not
// been changed yet.
void pack_sec_eng_keys(ByteView key, ByteSpan buffer, std::size_t offset, std::size_t length);

// The table of the CRC-16 (initial value `iv`, polynomial `poly`, high bit first) the tweak folds with.
std::array<std::uint16_t, 256> gf2_table(std::uint32_t iv, std::uint32_t poly);

}  // namespace cb::formats::xbox::crypto
