#include "formats/xbox/crypto/keystore.hpp"

#include <format>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"

namespace cb::formats::xbox::crypto::keystore {

std::string_view file_name(KeyFile which) {
    switch (which) {
        case KeyFile::Bl1Pub: return "1BL_pub.bin";
        case KeyFile::XmacsPub: return "XMACS_pub.bin";
        case KeyFile::SbPrv: return "SB_prv.bin";
        case KeyFile::HvxPrv: return "HVX_prv.bin";
        case KeyFile::MasterPub: return "Master_pub.bin";
    }
    return "";
}

std::uint32_t checksum(KeyFile which) {
    switch (which) {
        case KeyFile::Bl1Pub: return 0xD416B5E1;
        case KeyFile::XmacsPub: return 0xE4F01473;
        case KeyFile::SbPrv: return 0x490C9D35;
        case KeyFile::HvxPrv: return 0xDCC4B906;
        case KeyFile::MasterPub: return 0xE86E10FD;
    }
    return 0;
}

xecrypt::RsaKey KeyStore::load_and_verify(KeyFile which) const {
    const std::filesystem::path path = folder_ / std::string(file_name(which));
    const Bytes data = fs::read_file(path);
    const auto accepted = accepted_.find(which);
    const std::uint32_t expected = accepted != accepted_.end() ? accepted->second : checksum(which);
    const std::uint32_t crc = cb::crypto::crc32(data);
    if (crc != expected)
        throw FormatError("keystore", 0,
                          std::format("{}: invalid key checksum (CRC-32 {:08X}, expected {:08X})", fs::to_text(path), crc, expected));
    return xecrypt::RsaKey::parse(data);
}

}  // namespace cb::formats::xbox::crypto::keystore
