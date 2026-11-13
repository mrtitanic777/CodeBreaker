// cb/formats/xbox/crypto/tools.hpp -- the `crypto` command: the console crypto tools under one name.
//
//     crypto <tool> [arguments]       (crypto: the list)
//
// Most of this is Xbox-360-Crypto by GoobyCorp (BSD 3-Clause; see the LICENSE and ATTRIBUTION.md of codebreaker/formats/xbox/crypto), kept
// as its authors wrote it: each tool runs as `crypto <tool>` and takes its own --help. What the tools read that is not part of the package
// comes from settings: the key store folder (1BL_pub.bin, SB_prv.bin, ...; Keys/ in the current folder by default) and upstream's bin/
// folder (the LZX libraries, the Free60 xenon-as and xenon-objcopy, hypervisor images). Neither is in the repository.
//
// The tools in the native command layer:
//
//     harness      the known-answer tests of the crypto (RSA, RC4, AES, DES, MD5, SHA-1, HMAC, CPU keys)
//     sd_signer    sign a retail CD or devkit SD bootloader and print its XeBuild checksum
//     shadowboot   extract and build shadowboots
//     nand_tool    NAND images: info, verify, unecc, ecc, fix
//
// The others (bin2lang, bootchain, cpu_key, ..., xval) are listed but not yet in this layer: running one says so and exits 1.
#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "cb/core/cli.hpp"
#include "cb/formats/xbox/crypto/build_lib.hpp"
#include "cb/formats/xbox/crypto/keystore.hpp"

namespace cb::formats::xbox::crypto {

// The names of every tool of the Python package, in the order it lists them.
const std::vector<std::string_view>& tool_names();
// Whether the tool is in the native command layer.
bool tool_is_native(std::string_view name);

struct Environment {
    keystore::KeyStore keys{std::filesystem::path("Keys")};   // the key store folder (Keys/ in the current folder by default)
    std::shared_ptr<build_lib::LzxCodec> codec;     // the LZX codec the shadowboot builder compresses with (none: it cannot compress)
};

// `crypto <tool> ...`. With no tool, or --help, it prints the description and the tools' names (exit 0); an unknown tool is a usage error.
cli::Command command(const Environment& environment);

}  // namespace cb::formats::xbox::crypto
