#include "cb/formats/xbox/crypto/tools.hpp"

#include <algorithm>
#include <span>

#include "cb/core/log.hpp"
#include "cb/formats/xbox/crypto/harness.hpp"
#include "cb/formats/xbox/crypto/nand_tool.hpp"
#include "cb/formats/xbox/crypto/sd_signer.hpp"
#include "cb/formats/xbox/crypto/shadowboot.hpp"

namespace cb::formats::xbox::crypto {

namespace {

// The Python package's tools, in the order it lists them (its modules that define main, by file name).
const std::vector<std::string_view> kToolNames = {
    "MemCrypto", "bin2lang",   "bootchain", "cpu_key",        "cpukey_ecd",    "create_key",     "exp_build_task", "exp_signer", "export_keys",
    "flashplan", "format_key", "fuses",     "harness",        "hypervisor",    "keyvault",       "kv_tool",        "lzx",        "nand",
    "nand_tool", "patch_build_task", "patch_checker", "patch_compile", "patch_dumper", "sd_signer", "se_patcher", "shadowboot", "smc",
    "xcp_dumper", "xdk_extract", "xebuild_config", "xekeys_signer", "xell_unpack", "xkelib_exports", "xsm3", "xval"};

constexpr std::string_view kNativeTools[] = {"harness", "sd_signer", "shadowboot", "nand_tool"};

constexpr std::string_view kDescription =
    "Console crypto: XeCrypt, keyvaults, the boot chain, NAND images, shadowboots. Most of these tools are Xbox-360-Crypto by GoobyCorp (BSD\n"
    "3-Clause; see LICENSE and ATTRIBUTION.md), kept as its authors wrote it. Each runs as `crypto <tool> [arguments]` and takes its own --help.\n"
    "What the tools read that is not part of the package comes from settings: `xbox360_keys` (the key store folder: 1BL_pub.bin,\n"
    "SB_prv.bin, ...; default Keys/ in the current folder) and `xbox360_crypto_bin` (upstream's bin/ folder: the LZX libraries, the Free60\n"
    "xenon-as and xenon-objcopy, hypervisor images). Neither is in the repository.\n"
    "\n"
    "Tools in the native command layer: harness, sd_signer, shadowboot, nand_tool. The others are listed but not yet in it.";

cli::Command renamed(cli::Command command, std::string name) {
    command.name = std::move(name);
    command.area.clear();
    return command;
}

cli::Command not_yet_native(std::string name) {
    cli::Command c;
    c.name = name;
    c.summary = "(not in the native command layer yet)";
    c.params = {cli::Positional{"arguments", "the tool's own arguments", false, true}};
    c.passthrough = true;
    c.handler = [name](const cli::Args&) {
        err("crypto {}: this tool is not in the native command layer yet\n", name);
        return kExitFailed;
    };
    return c;
}

}  // namespace

const std::vector<std::string_view>& tool_names() { return kToolNames; }

bool tool_is_native(std::string_view name) { return std::find(std::begin(kNativeTools), std::end(kNativeTools), name) != std::end(kNativeTools); }

cli::Command command(const Environment& environment) {
    std::vector<cli::Command> tools;
    for (const std::string_view name : kToolNames) {
        if (name == "harness") {
            tools.push_back(renamed(harness::command(environment.keys), "harness"));
        } else if (name == "sd_signer") {
            tools.push_back(renamed(sd_signer::command(environment.keys), "sd_signer"));
        } else if (name == "shadowboot") {
            tools.push_back(renamed(shadowboot::command(environment.keys, environment.codec), "shadowboot"));
        } else if (name == "nand_tool") {
            cli::Command c;
            c.name = "nand_tool";
            c.summary = "NAND images: info, verify, unecc, ecc, fix";
            c.description = std::string(nand_tool::usage());
            c.params = {cli::Positional{"arguments", "an operation and its files (see the usage above)", false, true}};
            c.passthrough = true;
            c.handler = [](const cli::Args& a) {
                const std::vector<std::string> args = a.values("arguments");
                return nand_tool::command(args);
            };
            tools.push_back(std::move(c));
        } else {
            tools.push_back(not_yet_native(std::string(name)));
        }
    }
    cli::Command c;
    c.name = "crypto";
    c.summary = "Xbox 360 console crypto: XeCrypt, keyvaults, the boot chain, NAND images, shadowboots";
    c.area = "Xbox 360";
    c.description = std::string(kDescription);
    c.subcommands = std::move(tools);
    return c;
}

}  // namespace cb::formats::xbox::crypto
