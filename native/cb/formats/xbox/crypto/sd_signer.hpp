// cb/formats/xbox/crypto/sd_signer.hpp -- sign a retail CD or devkit SD bootloader (the 4BL) for the Xbox 360.
//
//     Bytes signed = sd_signer::sign_sd(key, sd_bytes);        // padded to 16, size set, signed, nonce zeroed
//     std::string line = sd_signer::checksum_line("SD.bin", signed);   // "SD.bin,1a2b3c4d" for XeBuild's checksum list
//
// The command `sd-signer <input> [-o <output>]` does it to a file with the key store's private SB key.
#pragma once

#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/formats/xbox/crypto/keystore.hpp"
#include "cb/formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::sd_signer {

// The bootloader, signed: its magic must be CD or SD (FormatError otherwise); it is padded with zeros to a multiple of 16, the
// size at 0xC set to the padded length, the signature written (salt XBOX_ROM_4) and the nonce at 0x10 zeroed. The nonce is
// zeroed after the signing, as the console's loader expects it; it is not part of what the signature covers.
Bytes sign_sd(const xecrypt::RsaKey& key, ByteView bootloader);

// "<name>,<crc32 in 8 lowercase hex digits>", the line XeBuild's checksum list wants.
std::string checksum_line(std::string_view name, ByteView data);

// `sd-signer`: the file is read, signed and written back (or to -o); the checksum line is printed. The key store is the one the
// command is made with.
cli::Command command(const keystore::KeyStore& store);

}  // namespace cb::formats::xbox::crypto::sd_signer
