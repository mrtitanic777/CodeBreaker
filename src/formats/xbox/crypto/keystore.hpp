// formats/xbox/crypto/keystore.hpp -- the console keys the bootloader tools sign and verify with, read from the key store.
//
//     KeyStore store("Keys");                                       // the folder of the setting "xbox360_keys"
//     xecrypt::RsaKey sb = store.load_and_verify(KeyFile::SbPrv);   // the private key that signs SD/SE bootloaders
//
// The key store is a folder of key files, none of them in the repository. Each file is recognised by its CRC-32: a file that
// is not the key it is named for (a different console's, a damaged copy) is refused before it signs anything.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string_view>

#include "formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::keystore {

enum class KeyFile {
    Bl1Pub,      // the 1BL's public key
    XmacsPub,    // the XMACS public key
    SbPrv,       // the private key of the SB/SC/SD/SE bootloaders
    HvxPrv,      // the hypervisor's private key
    MasterPub,   // the master public key
};

std::string_view file_name(KeyFile which);   // "1BL_pub.bin" "XMACS_pub.bin" "SB_prv.bin" "HVX_prv.bin" "Master_pub.bin"
std::uint32_t checksum(KeyFile which);       // the CRC-32 of the genuine file

class KeyStore {
public:
    explicit KeyStore(std::filesystem::path folder = "Keys") : folder_(std::move(folder)) {}

    const std::filesystem::path& folder() const { return folder_; }

    // The key of the file, after its checksum has been checked. NotFoundError when the file is not in the folder; FormatError
    // when its CRC-32 is not the genuine one, or its bytes are not a key.
    xecrypt::RsaKey load_and_verify(KeyFile which) const;

    // Accept another checksum for a file (a store of keys other than the console's own: a test's, a modder's).
    void accept_checksum(KeyFile which, std::uint32_t crc) { accepted_[which] = crc; }

private:
    std::filesystem::path folder_;
    std::map<KeyFile, std::uint32_t> accepted_;
};

}  // namespace cb::formats::xbox::crypto::keystore
