#include "cb/formats/xbox/crypto/tools.hpp"

#include <algorithm>
#include <span>

#include "cb/core/log.hpp"
#include "cb/formats/xbox/crypto/bin2lang.hpp"
#include "cb/formats/xbox/crypto/bootchain.hpp"
#include "cb/formats/xbox/crypto/cpu_key.hpp"
#include "cb/formats/xbox/crypto/cpukey_ecd.hpp"
#include "cb/formats/xbox/crypto/custom_keys.hpp"
#include "cb/formats/xbox/crypto/exp_signer.hpp"
#include "cb/formats/xbox/crypto/flashplan.hpp"
#include "cb/formats/xbox/crypto/fuses.hpp"
#include "cb/formats/xbox/crypto/harness.hpp"
#include "cb/formats/xbox/crypto/hypervisor.hpp"
#include "cb/formats/xbox/crypto/keyvault.hpp"
#include "cb/formats/xbox/crypto/kv_tool.hpp"
#include "cb/formats/xbox/crypto/lzx_dll.hpp"
#include "cb/formats/xbox/crypto/mem_crypto_tool.hpp"
#include "cb/formats/xbox/crypto/nand_dump.hpp"
#include "cb/formats/xbox/crypto/nand_tool.hpp"
#include "cb/formats/xbox/crypto/patch_tools.hpp"
#include "cb/formats/xbox/crypto/sd_signer.hpp"
#include "cb/formats/xbox/crypto/se_patcher.hpp"
#include "cb/formats/xbox/crypto/shadowboot.hpp"
#include "cb/formats/xbox/crypto/smc.hpp"
#include "cb/formats/xbox/crypto/xcp_dumper.hpp"
#include "cb/formats/xbox/crypto/xdk_extract.hpp"
#include "cb/formats/xbox/crypto/xekeys_signer.hpp"
#include "cb/formats/xbox/crypto/xell_unpack.hpp"
#include "cb/formats/xbox/crypto/xkelib_exports.hpp"
#include "cb/formats/xbox/crypto/xsm3.hpp"
#include "cb/formats/xbox/crypto/xval.hpp"

namespace cb::formats::xbox::crypto {

namespace {

// The Python package's tools, in the order it lists them (its modules that define main, by file name).
const std::vector<std::string_view> kToolNames = {
    "MemCrypto", "bin2lang",   "bootchain", "cpu_key",        "cpukey_ecd",    "create_key",     "exp_build_task", "exp_signer", "export_keys",
    "flashplan", "format_key", "fuses",     "harness",        "hypervisor",    "keyvault",       "kv_tool",        "lzx",        "nand",
    "nand_tool", "patch_build_task", "patch_checker", "patch_compile", "patch_dumper", "sd_signer", "se_patcher", "shadowboot", "smc",
    "xcp_dumper", "xdk_extract", "xebuild_config", "xekeys_signer", "xell_unpack", "xkelib_exports", "xsm3", "xval"};

constexpr std::string_view kDescription =
    "Console crypto: XeCrypt, keyvaults, the boot chain, NAND images, shadowboots. Most of these tools are Xbox-360-Crypto by GoobyCorp (BSD\n"
    "3-Clause; see LICENSE and ATTRIBUTION.md), kept as its authors wrote it. Each runs as `crypto <tool> [arguments]` and takes its own --help.\n"
    "What the tools read that is not part of the package comes from settings: `xbox360_keys` (the key store folder: 1BL_pub.bin,\n"
    "SB_prv.bin, ...; default Keys/ in the current folder), `xbox360_crypto_bin` (upstream's bin/ folder: the LZX libraries, the Free60\n"
    "xenon-as and xenon-objcopy, hypervisor images) and `xbox360_sdk` (dumpbin). None of them is in the repository.";

cli::Command renamed(cli::Command command, std::string name) {
    command.name = std::move(name);
    command.area.clear();
    return command;
}

// The tool upstream lists whose body does nothing: it reads its --dir option and goes through the folder's files without using them.
cli::Command xebuild_config_command() {
    return cli::Command{"xebuild_config",
                        "reserved: upstream's generator of XeBuild configurations (it writes nothing yet)",
                        {cli::Option{"--dir", "-d", "the folder to search for XeBuild files", cli::Arg::Value, "DIR", ""}},
                        [](const cli::Args&) { return 0; },
                        {},
                        "",
                        ""};
}

cli::Command nand_tool_command() {
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
    return c;
}

}  // namespace

const std::vector<std::string_view>& tool_names() { return kToolNames; }

bool tool_is_native(std::string_view name) { return std::find(kToolNames.begin(), kToolNames.end(), name) != kToolNames.end(); }

cli::Command command(const Environment& environment) {
    const keystore::KeyStore& keys = environment.keys;
    const build_lib::PatchTools patch_tools_config{environment.bin_dir};
    const std::shared_ptr<build_lib::LzxCodec> codec = environment.codec ? environment.codec : lzx_dll::lazy(environment.bin_dir);

    std::vector<cli::Command> tools;
    for (const std::string_view name : kToolNames) {
        if (name == "MemCrypto") tools.push_back(mem_crypto_tool::command(environment.bin_dir));
        else if (name == "bin2lang") tools.push_back(bin2lang::command());
        else if (name == "bootchain") tools.push_back(bootchain::command());
        else if (name == "cpu_key") tools.push_back(cpu_key::command());
        else if (name == "cpukey_ecd") tools.push_back(cpukey_ecd::command());
        else if (name == "create_key") tools.push_back(custom_keys::create_command(keys));
        else if (name == "exp_build_task") tools.push_back(patch_tools::exp_build_command(keys, patch_tools_config));
        else if (name == "exp_signer") tools.push_back(exp_signer::command(keys));
        else if (name == "export_keys") tools.push_back(custom_keys::export_command(keys));
        else if (name == "flashplan") tools.push_back(flashplan::command());
        else if (name == "format_key") tools.push_back(custom_keys::format_command(keys));
        else if (name == "fuses") tools.push_back(fuses::command());
        else if (name == "harness") tools.push_back(renamed(harness::command(keys), "harness"));
        else if (name == "hypervisor") tools.push_back(hypervisor::command());
        else if (name == "keyvault") tools.push_back(keyvault::command());
        else if (name == "kv_tool") tools.push_back(kv_tool::command());
        else if (name == "lzx") tools.push_back(lzx_dll::command(environment.bin_dir));
        else if (name == "nand") tools.push_back(nand_dump::command());
        else if (name == "nand_tool") tools.push_back(nand_tool_command());
        else if (name == "patch_build_task") tools.push_back(patch_tools::build_task_command(patch_tools_config));
        else if (name == "patch_checker") tools.push_back(patch_tools::checker_command());
        else if (name == "patch_compile") tools.push_back(patch_tools::compile_command(patch_tools_config));
        else if (name == "patch_dumper") tools.push_back(patch_tools::dumper_command());
        else if (name == "sd_signer") tools.push_back(renamed(sd_signer::command(keys), "sd_signer"));
        else if (name == "se_patcher") tools.push_back(se_patcher::command(keys, codec));
        else if (name == "shadowboot") tools.push_back(renamed(shadowboot::command(keys, codec), "shadowboot"));
        else if (name == "smc") tools.push_back(smc::command());
        else if (name == "xcp_dumper") tools.push_back(xcp_dumper::command());
        else if (name == "xdk_extract") tools.push_back(xdk_extract::command());
        else if (name == "xebuild_config") tools.push_back(xebuild_config_command());
        else if (name == "xekeys_signer") tools.push_back(xekeys_signer::command(keys));
        else if (name == "xell_unpack") tools.push_back(xell_unpack::command());
        else if (name == "xkelib_exports") tools.push_back(xkelib_exports::command(environment.sdk_bin_dir));
        else if (name == "xsm3") tools.push_back(xsm3::command());
        else if (name == "xval") tools.push_back(xval::command());
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
