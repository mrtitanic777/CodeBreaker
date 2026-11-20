#include "formats/xbox/crypto/mem_crypto.hpp"

#include <algorithm>
#include <format>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/io.hpp"
#include "formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto {

namespace {

// The master public key the key blob is sealed to: modulus (1024 bits) and exponent 0x10001.
constexpr const char* kMasterModulusHex =
    "E1322F1DE92AD64B494455CB05173F6671A964A415536E2B680C40F54FDA808F19B82CD0D7E964B2224C56DE03E2462F946F4FFFAD4588"
    "CF78CEED1CE5FD0F80533AE97043EAD1D12E39880C3CAEEBFDA5ACA3A69445E542EF269D5459952D252945B0169BEF788FB1EAE548AC1A"
    "C3C878899708DE24D1ED04D0555079199527";
constexpr std::uint64_t kMasterExponent = 0x10001;
constexpr std::uint32_t kGf2Poly = 0x87;                 // the tweak's CRC-16 polynomial (initial value 0)
constexpr std::uint64_t kTweakAddressBits = 36;          // the engine's address width above the 16-byte block
constexpr std::size_t kSha1Size = 20;

// The SHA-1 of nothing: the key blob carries it after the nonce.
constexpr std::uint8_t kEmptySha1[kSha1Size] = {0xDA, 0x39, 0xA3, 0xEE, 0x5E, 0x6B, 0x4B, 0x0D, 0x32, 0x55,
                                                0xBF, 0xEF, 0x95, 0x60, 0x18, 0x90, 0xAF, 0xD8, 0x07, 0x09};

const std::array<std::uint16_t, 256>& tweak_table() {
    static const std::array<std::uint16_t, 256> table = gf2_table(0, kGf2Poly);
    return table;
}

// A 256-bit number as four little-endian words: room for the white key shifted by every address bit.
struct Wide {
    std::array<std::uint64_t, 4> w{};
    // this ^= (hi:lo) << shift, shift < 192
    void xor_shifted(std::uint64_t hi, std::uint64_t lo, unsigned shift) {
        const unsigned words = shift / 64, bits = shift % 64;
        const std::uint64_t src[2] = {lo, hi};
        for (unsigned i = 0; i < 2; ++i) {
            w[i + words] ^= src[i] << bits;
            if (bits != 0 && i + words + 1 < 4) w[i + words + 1] ^= src[i] >> (64 - bits);
        }
    }
};

std::uint64_t be64(ByteView v, std::size_t at) {
    std::uint64_t r = 0;
    for (std::size_t i = 0; i < 8; ++i) r = (r << 8) | v[at + i];
    return r;
}

void put_be64(std::uint8_t* out, std::uint64_t v) {
    for (int i = 7; i >= 0; --i) {
        out[i] = static_cast<std::uint8_t>(v);
        v >>= 8;
    }
}

std::array<std::uint8_t, 16> copy_key(ByteView key, const char* name) {
    if (key.size() != kMemKeySize) throw UsageError(std::format("the {} key is {} bytes, not 16", name, key.size()));
    std::array<std::uint8_t, 16> out{};
    std::copy(key.begin(), key.end(), out.begin());
    return out;
}

// The image's bytes [offset, offset+size); the hashes read fixed places, so an image that lacks them is not a hypervisor.
ByteView region(ByteView hv, std::size_t offset, std::size_t size) {
    if (hv.size() < kHypervisorSize) {
        throw FormatError("hypervisor", 0, std::format("the image is {:#x} bytes; the hashes read {:#x}", hv.size(), kHypervisorSize));
    }
    return io::checked_range(hv, offset, size);
}

Bytes xor_bytes(ByteView a, ByteView b) {
    CB_ASSERT(a.size() == b.size(), "xor of two unequal buffers");
    Bytes out(a.size());
    for (std::size_t i = 0; i < a.size(); ++i) out[i] = a[i] ^ b[i];
    return out;
}

std::uint16_t rotate_right16(std::uint16_t v, unsigned by) {   // by in 1..16: 16 gives the word back
    return static_cast<std::uint16_t>((v >> by) | (static_cast<std::uint32_t>(v) << (16 - by)));
}

}  // namespace

std::array<std::uint16_t, 256> gf2_table(std::uint32_t iv, std::uint32_t poly) {
    std::array<std::uint16_t, 256> table{};
    for (std::uint32_t i = 0; i < 256; ++i) {
        std::uint32_t crc = iv, c = i << 8;
        for (int j = 0; j < 8; ++j) {
            crc = ((crc ^ c) & 0x8000) ? ((crc << 1) ^ poly) : (crc << 1);
            c <<= 1;
        }
        table[i] = static_cast<std::uint16_t>(crc);
    }
    return table;
}

void pack_sec_eng_keys(ByteView key, ByteSpan buffer, std::size_t offset, std::size_t length) {
    if (offset > buffer.size() || length > buffer.size() - offset) throw UsageError("pack_sec_eng_keys: the range is outside the buffer");
    const Bytes key_copy(key.begin(), key.end());   // (the key may be a part of the buffer)
    for (std::uint32_t cycle = 0; length > 0; ++cycle) {
        cb::crypto::Hasher h(cb::crypto::Hasher::Algorithm::Sha1);
        h.update(key_copy);
        const std::uint8_t counter[4] = {static_cast<std::uint8_t>(cycle >> 24), static_cast<std::uint8_t>(cycle >> 16),
                                         static_cast<std::uint8_t>(cycle >> 8), static_cast<std::uint8_t>(cycle)};
        h.update(counter);
        const Bytes digest = h.finish();
        const std::size_t piece = std::min(length, kSha1Size);
        for (std::size_t i = 0; i < piece; ++i) buffer[offset + i] ^= digest[i];
        offset += piece;
        length -= piece;
    }
}

MemoryCrypto::MemoryCrypto(ByteView white_key, ByteView aes_key, ByteView hash_key)
    : white_(copy_key(white_key, "white")), aes_(copy_key(aes_key, "AES")), hash_(copy_key(hash_key, "hash")) {}

std::uint64_t MemoryCrypto::fix_address(std::uint64_t address) {
    if (address > kHypervisorSize) return address;
    return address | (0x200000000ull * (address / 0x10000));
}

std::array<std::uint8_t, 16> MemoryCrypto::tweak(std::uint64_t address) const {
    const std::uint64_t key_hi = be64(white_, 0), key_lo = be64(white_, 8);
    Wide value;
    value.xor_shifted(key_hi, key_lo, kTweakAddressBits);   // room for 36 bits below the key
    address >>= 4;   // (the block number)
    for (unsigned i = 0; address >> i != 0; ++i) {
        if ((address >> i) & 1) value.xor_shifted(key_hi, key_lo, i);
    }
    // The 36 bits above the key's 128 are folded into it, a byte at a time, through the CRC-16 table.
    std::uint64_t over = value.w[2] & ((1ull << kTweakAddressBits) - 1);
    std::uint64_t lo = value.w[0];
    const std::uint64_t hi = value.w[1];
    for (unsigned i = 0; over != 0; ++i, over >>= 8) lo ^= static_cast<std::uint64_t>(tweak_table()[over & 0xFF]) << (i * 8);
    std::array<std::uint8_t, 16> out{};
    put_be64(out.data(), hi);
    put_be64(out.data() + 8, lo);
    return out;
}

namespace {

// The engine's transform for a run of blocks: tweak, AES in `decrypt_direction`, tweak. Block i is at fix(address + 16 i), or at
// `address + 16 i` as it is when `fix` is off.
Bytes transform(const MemoryCrypto& mem, ByteView key, ByteView data, std::uint64_t address, bool decrypt_direction, bool fix) {
    if (data.size() % 16 != 0) throw UsageError("the size must be a whole number of AES blocks (16 bytes)");
    std::vector<std::array<std::uint8_t, 16>> tweaks;
    tweaks.reserve(data.size() / 16);
    Bytes work(data.size());
    for (std::size_t i = 0; i < data.size(); i += 16) {
        const std::uint64_t at = address + i;
        tweaks.push_back(mem.tweak(fix ? MemoryCrypto::fix_address(at) : at));
        for (std::size_t j = 0; j < 16; ++j) work[i + j] = data[i + j] ^ tweaks.back()[j];
    }
    xecrypt::BlockCipher cipher(xecrypt::CipherAlgorithm::Aes, key, xecrypt::CipherMode::Ecb);
    work = decrypt_direction ? cipher.decrypt(work) : cipher.encrypt(work);
    for (std::size_t i = 0; i < work.size(); ++i) work[i] ^= tweaks[i / 16][i % 16];
    return work;
}

}  // namespace

Bytes MemoryCrypto::encrypt_block(ByteView block, std::uint64_t address) const {
    if (block.size() != 16) throw UsageError("a block is 16 bytes");
    return transform(*this, aes_, block, address, true, false);
}
Bytes MemoryCrypto::decrypt_block(ByteView block, std::uint64_t address) const {
    if (block.size() != 16) throw UsageError("a block is 16 bytes");
    return transform(*this, aes_, block, address, false, false);
}

// ("encrypt" is the engine's word: it runs the AES decryption, as the console's does)
Bytes MemoryCrypto::encrypt(ByteView data, std::uint64_t address) const { return transform(*this, aes_, data, address, true, true); }
Bytes MemoryCrypto::decrypt(ByteView data, std::uint64_t address) const { return transform(*this, aes_, data, address, false, true); }

Bytes MemoryCrypto::encrypt_and_checksums(ByteView data, std::uint64_t address) const {
    return sram_checksums(data, encrypt(data, address));
}

std::uint16_t MemoryCrypto::sram_checksum(ByteView page) {
    std::uint16_t sum = 0;
    unsigned rotation = 1;
    for (std::size_t i = 0; i + 1 < page.size(); i += 2) {
        const auto word = static_cast<std::uint16_t>((page[i] << 8) | page[i + 1]);
        sum ^= rotate_right16(word, rotation);
        rotation = static_cast<unsigned>((i / 2 + 1) / 4 + 1);
    }
    return sum;
}

Bytes MemoryCrypto::sram_checksums(ByteView dec, ByteView enc) const {
    if (dec.size() != enc.size()) throw UsageError("the decrypted and encrypted memory differ in size");
    if (dec.size() % kSramChecksumPage != 0) throw UsageError("the checksums need a size that is a multiple of 0x80");
    Bytes masked(dec.size());
    for (std::size_t i = 0; i < dec.size(); ++i) masked[i] = static_cast<std::uint8_t>((enc[i] & hash_[i % 16]) ^ dec[i]);
    Bytes out;
    out.reserve(dec.size() / kSramChecksumPage * 2);
    for (std::size_t page = 0; page < masked.size(); page += kSramChecksumPage) {
        const std::uint16_t sum = sram_checksum(ByteView(masked).subspan(page, kSramChecksumPage));
        out.push_back(static_cast<std::uint8_t>(sum >> 8));
        out.push_back(static_cast<std::uint8_t>(sum));
    }
    return out;
}

Bytes MemoryCrypto::checksum_chunk(ByteView hv_dec, ByteView hv_enc, std::size_t sram_offset, std::size_t sram_size) const {
    const std::size_t at = sram_to_hv(sram_offset), size = sram_to_hv(sram_size);
    return sram_checksums(io::checked_range(hv_dec, at, size), io::checked_range(hv_enc, at, size));
}

Bytes MemoryCrypto::calc_sram(ByteView hv_dec) const {
    const ByteView image = region(hv_dec, 0, kHypervisorSize);
    return sram_checksums(image, encrypt(image, 0));
}

Bytes MemoryCrypto::hash_1_digest(ByteView hv_dec, ByteView salt) const {
    salt = salt.subspan(0, std::min<std::size_t>(salt.size(), 16));
    struct Piece {
        std::size_t at, size;
    };
    static constexpr Piece kPieces[] = {{0x34, 0x40},    {0x78, 0xFF88},  {0x100C0, 0x40}, {0x10350, 0x5F70},
                                        {0x16EA0, 0x9160}, {0x20000, 0xFFFF}, {0x30000, 0xFFFF}};
    cb::crypto::Hasher h(cb::crypto::Hasher::Algorithm::Sha1);
    h.update(salt);
    for (const Piece& p : kPieces) h.update(region(hv_dec, p.at, p.size));
    const Bytes digest = h.finish();
    return Bytes(digest.begin() + 0xE, digest.begin() + 0xE + 6);
}

Bytes MemoryCrypto::hash_2_digest(ByteView hv_dec, ByteView salt, std::uint32_t hvex_addr) const {
    salt = salt.subspan(0, std::min<std::size_t>(salt.size(), 16));
    if (salt.size() != 16) throw UsageError("the salt of the second hash is 16 bytes");
    Bytes salt_memory;
    for (int i = 0; i < 8; ++i) append(salt_memory, salt);
    const std::uint64_t salt_address = (static_cast<std::uint64_t>(hvex_addr) << 16) | 0x7C00000400ull;

    cb::crypto::Hasher h(cb::crypto::Hasher::Algorithm::Sha1);
    h.update(encrypt_and_checksums(salt_memory, salt_address));
    h.update(region(hv_dec, 0x34, 0xC));
    h.update(encrypt(region(hv_dec, 0x40, 0x30), 0x40));
    h.update(region(hv_dec, 0x70, 4));
    h.update(region(hv_dec, 0x78, 8));
    h.update(encrypt_and_checksums(region(hv_dec, 0x80, 0xFF80), 0x80));
    h.update(encrypt(region(hv_dec, 0x100C0, 0x40), 0x100C0));
    h.update(encrypt(region(hv_dec, 0x10350, 0x30), 0x10350));
    h.update(encrypt_and_checksums(region(hv_dec, 0x10380, 0x5F00), 0x10380));
    h.update(encrypt(region(hv_dec, 0x16280, 0x40), 0x16280));
    h.update(encrypt(region(hv_dec, 0x16EA0, 0x60), 0x16EA0));
    h.update(encrypt_and_checksums(region(hv_dec, 0x16F00, 0x9100), 0x16F00));
    h.update(encrypt_and_checksums(region(hv_dec, 0x20000, 0x10000), 0x20000));
    h.update(encrypt_and_checksums(region(hv_dec, 0x30000, 0x10000), 0x30000));
    return h.finish();
}

Bytes MemoryCrypto::key_blob(ByteView nonce) const {
    if (nonce.size() != kKeyBlobNonceSize) throw UsageError(std::format("the key blob's nonce is 20 bytes, not {}", nonce.size()));
    Bytes blob(kKeyBlobSize, 0);
    std::copy(nonce.begin(), nonce.end(), blob.begin() + 1);
    std::copy(std::begin(kEmptySha1), std::end(kEmptySha1), blob.begin() + 0x15);
    blob[0x4F] = 1;
    std::copy(white_.begin(), white_.end(), blob.begin() + 0x50);
    std::copy(aes_.begin(), aes_.end(), blob.begin() + 0x60);
    std::copy(hash_.begin(), hash_.end(), blob.begin() + 0x70);
    pack_sec_eng_keys(ByteView(blob).subspan(1, 0x14), blob, 0x15, 0x6B);
    pack_sec_eng_keys(ByteView(blob).subspan(0x15, 0x6B), blob, 1, 0x14);

    static const xecrypt::Nat modulus = xecrypt::Nat::from_be(cb::crypto::from_hex(kMasterModulusHex));
    // the blob is read little-endian, the answer written big-endian, as the console's key blob is
    const xecrypt::Nat sealed = xecrypt::Nat::pow_mod(xecrypt::Nat::from_le(blob), kMasterExponent, modulus);
    return sealed.to_be(kKeyBlobSize);
}

Bytes MemoryCrypto::calc_100f0(ByteView hv_dec, std::uint32_t table_addr) const {
    const ByteView image = region(hv_dec, 0, kHypervisorSize);
    const ByteView table = io::checked_range(image, table_addr, 6 * 8);
    const Bytes enc = encrypt(image, 0);
    cb::crypto::Hasher h(cb::crypto::Hasher::Algorithm::Sha1);
    for (std::size_t i = 0; i < 6; ++i) {
        const std::uint64_t start = be64(table, i * 8) >> 32, stop = be64(table, i * 8) & 0xFFFFFFFFull;
        const std::uint64_t a_start = (start + 0x7F) & ~std::uint64_t{0x7F}, a_stop = stop & 0xFFFFFF80ull;   // aligned inward
        if (a_start >= a_stop) continue;
        const std::size_t sram_offset = static_cast<std::size_t>(a_start / kSramChecksumPage * 2);
        const std::size_t sram_size = static_cast<std::size_t>((a_stop - a_start) / kSramChecksumPage * 2);
        h.update(checksum_chunk(image, enc, sram_offset, sram_size));
    }
    const Bytes digest = h.finish();
    return Bytes(digest.begin(), digest.begin() + 0x10);
}

}  // namespace cb::formats::xbox::crypto
