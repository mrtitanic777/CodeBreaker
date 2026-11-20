// Tests of cb/formats/xbox/crypto/se_patcher.hpp, over a stand-in LZX codec (the real library is not part of this program): the
// SE's frame (magic, build, flags, size, padding), the hash in the SD, the SD's size and signature, patch tables, and the errors.
//
// Behaviour pinned here: the key, the codec and the inputs are parameters; the SD and SE are returned with their lengths before padding
// in a struct.
#include "formats/xbox/crypto/se_patcher.hpp"

#include <algorithm>

#include "core/error.hpp"
#include "core/testing.hpp"

namespace se_patcher_test {

using namespace cb;
using namespace cb::formats::xbox::crypto;

namespace {

// "Compresses" by writing the data after a marker byte; the framing around it is what these tests are about.
class MarkerCodec : public build_lib::LzxCodec {
public:
    Bytes compress_continuous(ByteView data) override {
        Bytes out{0x5A};
        out.insert(out.end(), data.begin(), data.end());
        return out;
    }
    Bytes decompress_continuous(ByteView data) override { return Bytes(data.begin() + 1, data.end()); }
};

Bytes sd_image(std::size_t size) {
    Bytes data(size, 0);
    data[0] = 'S';
    data[1] = 'D';
    for (std::size_t i = 0x20; i < size; ++i) data[i] = static_cast<std::uint8_t>(i * 5 + 3);
    return data;
}

std::uint32_t be32(const Bytes& b, std::size_t at) {
    return (std::uint32_t{b[at]} << 24) | (std::uint32_t{b[at + 1]} << 16) | (std::uint32_t{b[at + 2]} << 8) | b[at + 3];
}

}  // namespace

CB_TEST("formats.xbox.crypto.se_patcher.frames_the_se_and_signs_the_sd", [] {
    const xecrypt::RsaKey key = xecrypt::RsaKey::generate(2048);
    MarkerCodec codec;
    const Bytes sd = sd_image(0x305 + 0x250);   // not a multiple of 16
    const Bytes body(0x123, 0xAB);
    const se_patcher::SdSe pair = se_patcher::patch_rehash_sign_se(key, codec, sd, body);

    // the SE: 0x20 bytes of header, then the compressed stream with its own 0x30-byte header
    const Bytes packed = build_lib::compress_se(codec, body);
    CB_CHECK(pair.se_unpadded == 0x20 + packed.size());
    CB_CHECK(pair.se.size() % 16 == 0 && pair.se.size() - pair.se_unpadded < 16);
    CB_CHECK(pair.se[0] == 'S' && pair.se[1] == 'E');
    CB_CHECK(pair.se[2] == 17559 / 256 && pair.se[3] == 17559 % 256);
    CB_CHECK(pair.se[4] == 0x80 && pair.se[5] == 0x00);   // flags
    CB_CHECK(be32(pair.se, 0xC) == pair.se_unpadded);
    CB_CHECK(std::equal(packed.begin() + 0x10, packed.end(), pair.se.begin() + 0x30));   // what follows is the stream

    // the SD: its length before padding at 0xC, the SE's hash at 0x24C, a signature that verifies
    CB_CHECK(pair.sd_unpadded == sd.size());
    CB_CHECK(pair.sd.size() % 16 == 0 && pair.sd.size() - pair.sd_unpadded < 16);
    CB_CHECK(be32(pair.sd, 0xC) == pair.sd_unpadded);
    Bytes hashed(pair.se.begin(), pair.se.begin() + 0x10);
    hashed.insert(hashed.end(), pair.se.begin() + 0x20, pair.se.end());
    const Bytes hash = xecrypt::rot_sum_sha(hashed);
    CB_CHECK(std::equal(hash.begin(), hash.begin() + 20, pair.sd.begin() + 0x24C));
    CB_CHECK(build_lib::verify_sd_4bl(key.public_key(), as_bytes(build_lib::kSdSalt), pair.sd));
    // the rest of the SD is the one given
    CB_CHECK(std::equal(sd.begin() + 0x260, sd.end(), pair.sd.begin() + 0x260));
});

CB_TEST("formats.xbox.crypto.se_patcher.a_patch_table_changes_the_se_and_so_its_hash", [] {
    const xecrypt::RsaKey key = xecrypt::RsaKey::generate(2048);
    MarkerCodec codec;
    const Bytes sd = sd_image(0x400);
    const Bytes body(0x100, 0x11);
    // one record: at 0x10, one word, 0xDEADBEEF; the table ends with 0xFFFFFFFF
    const Bytes table{0, 0, 0, 0x10, 0, 0, 0, 1, 0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF};
    const se_patcher::SdSe plain = se_patcher::patch_rehash_sign_se(key, codec, sd, body);
    const se_patcher::SdSe patched = se_patcher::patch_rehash_sign_se(key, codec, sd, body, ByteView(table));
    CB_CHECK(plain.se != patched.se);
    CB_CHECK(!std::equal(plain.sd.begin() + 0x24C, plain.sd.begin() + 0x260, patched.sd.begin() + 0x24C));
    // the patched word is in the stream the codec carries
    const std::uint8_t word[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    CB_CHECK(std::search(patched.se.begin(), patched.se.end(), word, word + 4) != patched.se.end());
    // the patching and framing are deterministic, and the signature (PKCS-style) too
    CB_CHECK(se_patcher::patch_rehash_sign_se(key, codec, sd, body, ByteView(table)).se == patched.se);
});

CB_TEST("formats.xbox.crypto.se_patcher.errors", [] {
    const xecrypt::RsaKey key = xecrypt::RsaKey::generate(2048);
    MarkerCodec codec;
    bool refused = false;
    try {
        (void)se_patcher::patch_rehash_sign_se(key, codec, sd_image(0x200), Bytes(0x20, 1));   // an SD too short for the hash's place
    } catch (const FormatError&) {
        refused = true;
    }
    CB_CHECK(refused);
    refused = false;
    try {
        const Bytes cut_table{0, 0, 0, 0x10, 0, 0, 0, 4, 1, 2};   // a record that stops in the middle
        (void)se_patcher::patch_rehash_sign_se(key, codec, sd_image(0x400), Bytes(0x20, 1), ByteView(cut_table));
    } catch (const TruncatedError&) {
        refused = true;
    }
    CB_CHECK(refused);
});

}  // namespace se_patcher_test
