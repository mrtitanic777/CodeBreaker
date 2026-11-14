// cb/formats/xbox/crypto/tools.hpp -- the `crypto` command: the console crypto tools under one name.
//
//     crypto <tool> [arguments]       (crypto: the list)
//
// Most of this is Xbox-360-Crypto by GoobyCorp (BSD 3-Clause; see the LICENSE and ATTRIBUTION.md of codebreaker/formats/xbox/crypto), kept
// as its authors wrote it: each tool runs as `crypto <tool>` and takes its own --help. What the tools read that is not part of the package
// comes from settings: the key store folder (1BL_pub.bin, SB_prv.bin, ...; Keys/ in the current folder by default), upstream's bin/ folder (the
// LZX libraries, the Free60 xenon-as and xenon-objcopy, hypervisor images) and the Xbox 360 SDK (dumpbin). None of them is in the repository.
//
// The tools, each a module of its own (the module's header says what it is for):
//
//     bin2lang        a binary as an array in source code      bootchain       the stages of a NAND dump's boot chain
//     cpu_key         make a CPU key                           cpukey_ecd      validate a CPU key
//     create_key      a custom RSA key pair                    export_keys     that pair as C++ arrays      format_key     its public key
//     exp_signer      sign an HvxExpansionInstall payload      xekeys_signer   sign an XeKeysExecute payload
//     exp_build_task  build and sign the expansions            patch_compile   assemble the RGLoader patches
//     patch_checker   overlapping patches                      patch_build_task compile and check      patch_dumper   a patch table as assembly
//     flashplan       RGH/JTAG image-type advisor              fuses           an eFuse dump           kv_tool        fuses and keyvault for a console
//     keyvault        a decrypted keyvault                     smc             the SMC firmware        hypervisor     a hypervisor image
//     nand            a NAND dump's type and hypervisor        nand_tool       page ECC, spares, raw/flash images
//     harness         the known-answer tests of the crypto     MemCrypto       memory-encryption hashes   lzx   the LZX library's self-test
//     sd_signer       sign a CD/SD bootloader                  shadowboot      extract and build shadowboots
//     xcp_dumper      decrypt an XCP package                   xdk_extract     the cabinets of an XDK installer
//     xell_unpack     the stages of an XeLL image              xkelib_exports  a .def of a library's exports
//     xsm3            the XSM3 controller exchange             xval            the dashboard's X: value
//     xebuild_config  (reserved: upstream's generator of XeBuild configurations writes nothing yet)
#pragma once

#include <filesystem>
#include <memory>
#include <string_view>
#include <vector>

#include "cb/core/cli.hpp"
#include "cb/formats/xbox/crypto/build_lib.hpp"
#include "cb/formats/xbox/crypto/keystore.hpp"

namespace cb::formats::xbox::crypto {

// The names of every tool, in the order they are listed.
const std::vector<std::string_view>& tool_names();
// Whether the tool is in the native command layer (every one is).
bool tool_is_native(std::string_view name);

struct Environment {
    keystore::KeyStore keys{std::filesystem::path("Keys")};   // the key store folder (Keys/ in the current folder by default)
    std::shared_ptr<build_lib::LzxCodec> codec;     // the LZX codec the shadowboot builder compresses with (none: the one over upstream's bin/ DLLs, when they are there)
    std::filesystem::path bin_dir{"bin"};           // upstream's bin/ folder: setting `xbox360_crypto_bin`
    std::filesystem::path sdk_bin_dir{"C:/Program Files (x86)/Microsoft Xbox 360 SDK/bin/win32"};   // the SDK's dumpbin: `xbox360_sdk`/bin/win32
};

// `crypto <tool> ...`. With no tool, or --help, it prints the description and the tools' names (exit 0); an unknown tool is a usage error.
cli::Command command(const Environment& environment);

}  // namespace cb::formats::xbox::crypto
