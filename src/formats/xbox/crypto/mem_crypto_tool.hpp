// formats/xbox/crypto/mem_crypto_tool.hpp -- the `crypto MemCrypto` tool: the memory-encryption crypto (mem_crypto.hpp) on the files of upstream's
// bin/ folder.
//
//     crypto MemCrypto calculate          the challenge-response hashes and key blob of the cleaned 17559 hypervisor, with the all-0x55 test keys
//     crypto MemCrypto test               decrypt HV.enc.bin with the keys of keys.bin (white, AES, hash: 16 bytes each) into HV.dec.bin
//
// `calculate` reads HV_17559_Cleaned.bin: hash 1 (salt 892BB9F9...), hash 2 (the same salt and the HVEX address 0xB00B) and the key blob
// (nonce "testtest", zero padded to 20), each printed in upper-case hex. `test` reads keys.bin and HV.enc.bin and writes HV.dec.bin in the
// current folder. Both read from the folder of the setting `xbox360_crypto_bin`.
#pragma once

#include <filesystem>
#include <string>

#include "core/bytes.hpp"
#include "core/cli.hpp"

namespace cb::formats::xbox::crypto::mem_crypto_tool {

// The text `calculate` prints for a cleaned hypervisor image.
std::string calculate_report(ByteView hypervisor);

cli::Command command(const std::filesystem::path& bin_dir);

}  // namespace cb::formats::xbox::crypto::mem_crypto_tool
