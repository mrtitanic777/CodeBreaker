// cb/formats/xbox/crypto/se_patcher.hpp -- patch the SE bootloader (the hypervisor and kernel), compress it, and re-sign the SD
// that vouches for it.
//
//     SdSe pair = se_patcher::patch_rehash_sign_se(key, codec, sd, hypervisor_and_kernel, patch_table);
//     // pair.sd and pair.se are the images to write; the *_unpadded sizes are what XeBuild's checksums cover
//
// The SD holds the rot-sum hash of the SE at 0x24C, so a changed SE needs its SD signed again.
#pragma once

#include <cstddef>
#include <optional>

#include "cb/core/bytes.hpp"
#include "cb/formats/xbox/crypto/build_lib.hpp"
#include "cb/formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::se_patcher {

// An SD and its SE, each with its length before the padding to 16 (what the checksums are taken over).
struct SdSe {
    Bytes sd;
    std::size_t sd_unpadded = 0;
    Bytes se;
    std::size_t se_unpadded = 0;
};

// `patch_table` (when given) is written over the SE's body; the body is compressed (header, magic SE, build 17559, flags 0x8000),
// padded to 16, hashed, the hash written into a copy of the SD at 0x24C, the SD's size set and the SD signed with `key`.
// FormatError when the SD is shorter than the hash's place plus 20, or the patch table is malformed.
SdSe patch_rehash_sign_se(const xecrypt::RsaKey& key, build_lib::LzxCodec& codec, ByteView sd, ByteView se_body,
                          std::optional<ByteView> patch_table = std::nullopt);

}  // namespace cb::formats::xbox::crypto::se_patcher
