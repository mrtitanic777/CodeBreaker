#include "cb/formats/xbox/crypto/se_patcher.hpp"

#include <algorithm>

#include "cb/core/error.hpp"

namespace cb::formats::xbox::crypto::se_patcher {

namespace {

constexpr std::string_view kReader = "se_patcher";
constexpr std::size_t kHashAt = 0x24C;     // where the SD holds the SE's hash
constexpr std::size_t kHashSize = 20;
constexpr std::size_t kSizeAt = 0xC;
constexpr std::uint16_t kSeBuild = 17559;
constexpr std::uint16_t kSeQfe = 0x8000;

void put_u16(Bytes& b, std::size_t at, std::uint16_t v) {
    b[at] = static_cast<std::uint8_t>(v >> 8);
    b[at + 1] = static_cast<std::uint8_t>(v);
}

void put_u32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

}  // namespace

SdSe patch_rehash_sign_se(const xecrypt::RsaKey& key, build_lib::LzxCodec& codec, ByteView sd, ByteView se_body,
                          std::optional<ByteView> patch_table) {
    if (sd.size() < kHashAt + kHashSize) throw FormatError(kReader, sd.size(), "an SD too short to hold the SE's hash");
    Bytes sd_data(sd.begin(), sd.end());

    // patch, compress and frame the SE: 0x20 bytes of header in front of what the compressor makes
    Bytes body = patch_table ? build_lib::apply_patches(se_body, *patch_table) : Bytes(se_body.begin(), se_body.end());
    Bytes se = Bytes(0x20, 0);
    const Bytes packed = build_lib::compress_se(codec, body);
    se.insert(se.end(), packed.begin(), packed.end());
    se[0] = 'S';
    se[1] = 'E';
    put_u16(se, 2, kSeBuild);
    put_u16(se, 4, kSeQfe);
    put_u16(se, 6, 0);
    put_u32(se, 8, 0);   // the entry point
    const std::size_t se_unpadded = se.size();
    put_u32(se, kSizeAt, static_cast<std::uint32_t>(se_unpadded));
    se.resize(se.size() + build_lib::calc_pad_size(se_unpadded), 0);

    // the hash covers the first 0x10 bytes and what follows the 0x20-byte header and nonce block
    Bytes hashed(se.begin(), se.begin() + 0x10);
    hashed.insert(hashed.end(), se.begin() + 0x20, se.end());
    const Bytes hash = xecrypt::rot_sum_sha(hashed);
    std::copy(hash.begin(), hash.begin() + static_cast<std::ptrdiff_t>(kHashSize), sd_data.begin() + static_cast<std::ptrdiff_t>(kHashAt));

    const std::size_t sd_unpadded = sd_data.size();
    put_u32(sd_data, kSizeAt, static_cast<std::uint32_t>(sd_unpadded));
    sd_data.resize(sd_data.size() + build_lib::calc_pad_size(sd_unpadded), 0);
    Bytes signed_sd = build_lib::sign_sd_4bl(key, as_bytes(build_lib::kSdSalt), sd_data);

    return SdSe{std::move(signed_sd), sd_unpadded, std::move(se), se_unpadded};
}

}  // namespace cb::formats::xbox::crypto::se_patcher
