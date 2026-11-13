// Tests of cb/formats/xbox/crypto/mem_crypto.hpp against the values the Python module (MemCrypto.py) gives for the same inputs.
// The expected values below were made by running the Python on a synthetic 0x40000-byte image (byte i = i*7 + (i>>8)*13) with the
// keys 00..0f, 10..1f, 20..2f (state/work/typed-port/xbox-deferred/mem_golden.py in the work that wrote this), and the digest of the long ones.
//
// DIVERGES from the Python: the Python's tweak table is empty until its main() runs, so the module used as a library fails with an
// IndexError (fixed in the Python too, which now builds the table at import); a key blob of a nonce that is not 20 bytes resized
// the blob in the Python and is UsageError here (the Python now refuses it as well); an image shorter than the hashes read
// was silently clamped by the Python's slices and is FormatError here (and in the Python now).
#include "cb/formats/xbox/crypto/mem_crypto.hpp"

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace mem_crypto_test {

using namespace cb;
using namespace cb::formats::xbox::crypto;

static Bytes sequence(std::uint8_t first, std::size_t n = 16) {
    Bytes b(n);
    for (std::size_t i = 0; i < n; ++i) b[i] = static_cast<std::uint8_t>(first + i);
    return b;
}

static Bytes image() {
    Bytes hv(0x40000);
    for (std::size_t i = 0; i < hv.size(); ++i) hv[i] = static_cast<std::uint8_t>(i * 7 + (i >> 8) * 13);
    return hv;
}

static MemoryCrypto engine() { return MemoryCrypto(sequence(0), sequence(16), sequence(32)); }

static std::string hexed(ByteView v) { return cb::crypto::to_hex(v); }
static std::string sha1_hex(ByteView v) { return cb::crypto::to_hex(cb::crypto::sha1(v)); }

CB_TEST("formats.xbox.crypto.mem_crypto.the_tweak_table_and_fix_address", [] {
    const auto table = gf2_table(0, 0x87);
    CB_CHECK_EQ(table[1], 0x87);
    CB_CHECK_EQ(table[2], 0x10E);
    CB_CHECK_EQ(table[0x80], 0x4380);
    CB_CHECK_EQ(table[0xFF], 0x7D7D);
    CB_CHECK_EQ(MemoryCrypto::fix_address(0), 0u);
    CB_CHECK_EQ(MemoryCrypto::fix_address(0xFFFF), 0xFFFFu);
    CB_CHECK_EQ(MemoryCrypto::fix_address(0x10000), 0x200010000ull);
    CB_CHECK_EQ(MemoryCrypto::fix_address(0x40000), 0x800040000ull);
    CB_CHECK_EQ(MemoryCrypto::fix_address(0x40010), 0x40010u);
});

CB_TEST("formats.xbox.crypto.mem_crypto.tweaks_equal_the_pythons", [] {
    const MemoryCrypto m = engine();
    CB_CHECK_EQ(hexed(m.tweak(0)), "405060708090a0b0c0d0e0f00860f890");
    CB_CHECK_EQ(hexed(m.tweak(0x10)), "405162738495a6b7c8d9eafb046df69f");
    CB_CHECK_EQ(hexed(m.tweak(0x1230)), "4171251109396d50d0e0b48090d82461");
    CB_CHECK_EQ(hexed(m.tweak(0x7C00000400ull)), "1f083026ffe8d0c4a3b48c9a48d3d1e8");
    CB_CHECK_EQ(hexed(m.tweak(0x200000010ull)), "20d1c2b3659487f6a9584b3ae47d375f");
});

CB_TEST("formats.xbox.crypto.mem_crypto.blocks_and_the_whole_image", [] {
    const MemoryCrypto m = engine();
    const Bytes hv = image();
    const ByteView block = ByteView(hv).subspan(0, 16);
    CB_CHECK_EQ(hexed(m.encrypt_block(block, 0x10)), "b8736c86a41f2ce6822c261559826173");
    CB_CHECK_EQ(hexed(m.decrypt_block(block, 0x10)), "8ae35bbefed0c84db816ff3e06927618");

    const Bytes enc = m.encrypt(hv, 0);
    CB_CHECK_EQ(sha1_hex(enc), "ee59a23ddf65b0ed333ac79405a8aa77a70fd392");
    CB_CHECK_EQ(hexed(ByteView(enc).subspan(0, 16)), "7fa38e5d9e332ce16dabb97cdaca7da7");
    CB_CHECK(m.decrypt(enc, 0) == hv);   // the round trip
});

CB_TEST("formats.xbox.crypto.mem_crypto.sram_checksums", [] {
    const MemoryCrypto m = engine();
    const Bytes hv = image();
    CB_CHECK_EQ(MemoryCrypto::sram_checksum(ByteView(hv).subspan(0, 0x80)), 0xE8FD);
    const Bytes sums = m.calc_sram(hv);
    CB_CHECK_EQ(sums.size(), 4096u);
    CB_CHECK_EQ(sha1_hex(sums), "d2ee000d5fbfc376eb9b5ef7c248a321c5a2b37a");
    CB_CHECK_EQ(hexed(ByteView(sums).subspan(0, 8)), "097f441dced97f25");
    const Bytes enc = m.encrypt(hv, 0);
    CB_CHECK_EQ(hexed(m.checksum_chunk(hv, enc, 0x200, 0x40)),
                "c2d1dd9cb9cfeaec18f7277f19ab64a98c8a24337c2bff9f7c4ee9e8acc5e42a7526d8565405e3fc65db95af50c4012945c354f13"
                "4a027653baa1acdf08d7491");
});

CB_TEST("formats.xbox.crypto.mem_crypto.the_xosc_hashes_and_the_key_blob", [] {
    const MemoryCrypto m = engine();
    const Bytes hv = image();
    const Bytes salt = sequence(100);
    CB_CHECK_EQ(hexed(m.hash_1_digest(hv, salt)), "d19dbabf3983");
    CB_CHECK_EQ(hexed(m.hash_2_digest(hv, salt, 0xB00B)), "c5eae50b762657f6fe5bba834cd5c47077049a98");
    CB_CHECK_EQ(hexed(m.hash_2_digest(hv, salt, kHvexAddress)), "522700ef08ec015696e32c365ea3e11faefc9add");
    CB_CHECK_EQ(hexed(m.key_blob(sequence(1, 20))),
                "4232574cc599ca9380a016eb16c28dd9fecd64e5bd3a6bc9aa1c37e91bcaa5707b2d8921d64d44cd48a109e8145d59778214e7bc31e7e26d"
                "2d4f102ed7bfece5d48714fd9cb8f142ec8e4374813fe024cfbdce847f6deb2f4507da16498c3a2e00e7d6f29695b94ae777914ebb1dd70f"
                "1ae874b6b250e8f0e26bf829a13d97f7");
});

CB_TEST("formats.xbox.crypto.mem_crypto.the_table_of_ranges_and_the_key_mask", [] {
    const MemoryCrypto m = engine();
    Bytes hv = image();
    const std::uint32_t ranges[6][2] = {{0x100, 0x3F0}, {0x1001, 0x2000}, {0x8000, 0x8010},
                                        {0x20000, 0x20400}, {0x30005, 0x30300}, {0x3F000, 0x3FFFF}};
    for (std::size_t i = 0; i < 6; ++i) {
        for (std::size_t k = 0; k < 2; ++k) {
            for (int b = 0; b < 4; ++b) hv[kHv17559TableAddress + i * 8 + k * 4 + b] = static_cast<std::uint8_t>(ranges[i][k] >> (24 - 8 * b));
        }
    }
    CB_CHECK_EQ(hexed(m.calc_100f0(hv, kHv17559TableAddress)), "024cab53134a87a0fc1d590363a9ad3e");

    Bytes keys = sequence(0, 0x80);
    pack_sec_eng_keys(ByteView(keys).subspan(1, 0x14), keys, 0x15, 0x6B);
    CB_CHECK_EQ(sha1_hex(keys), "9cbd40e8f093b28a7cd98adc8516913d224a452a");
});

CB_TEST("formats.xbox.crypto.mem_crypto.bad_arguments", [] {
    CB_CHECK_THROWS_AS(UsageError, (void)MemoryCrypto(sequence(0, 15), sequence(16), sequence(32)));
    const MemoryCrypto m = engine();
    const Bytes odd(17);
    CB_CHECK_THROWS_AS(UsageError, (void)m.encrypt(odd, 0));
    CB_CHECK_THROWS_AS(UsageError, (void)m.encrypt_block(odd, 0));
    CB_CHECK_THROWS_AS(UsageError, (void)m.sram_checksums(Bytes(0x81), Bytes(0x81)));
    CB_CHECK_THROWS_AS(UsageError, (void)m.sram_checksums(Bytes(0x80), Bytes(0x100)));
    CB_CHECK_THROWS_AS(UsageError, (void)m.key_blob(sequence(1, 8)));
    const Bytes short_image(0x30000);
    CB_CHECK_THROWS_AS(FormatError, (void)m.hash_1_digest(short_image, sequence(0)));
    CB_CHECK_THROWS_AS(FormatError, (void)m.calc_sram(short_image));
    CB_CHECK_THROWS_AS(UsageError, (void)m.hash_2_digest(image(), sequence(0, 8), 1));
    Bytes buffer(8);
    CB_CHECK_THROWS_AS(UsageError, pack_sec_eng_keys(sequence(0), buffer, 4, 8));
    // a table that runs off the image
    CB_CHECK_THROWS_AS(FormatError, (void)m.calc_100f0(image(), 0x3FFF8));
});

}  // namespace mem_crypto_test
