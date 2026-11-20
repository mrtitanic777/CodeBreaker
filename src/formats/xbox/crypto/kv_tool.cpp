#include "formats/xbox/crypto/kv_tool.hpp"

#include <algorithm>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "formats/xbox/crypto/fuses.hpp"
#include "formats/xbox/crypto/tool_support.hpp"

namespace cb::formats::xbox::crypto::kv_tool {

namespace {

constexpr std::size_t kDvdKeyAt = 0x100;
constexpr std::size_t kTypeBytesAt = 0xE;
constexpr std::size_t kLockdownAt = 0x38;

std::array<std::uint8_t, 2> type_bytes(ConsoleType type) {
    switch (type) {
        case ConsoleType::TestKit: return {0xF0, 0x0F};
        case ConsoleType::Devkit: return {0x0F, 0x0F};
        case ConsoleType::RetailPhat: return {0x0F, 0xF0};
        case ConsoleType::RetailSlim: return {0xF0, 0xF0};
    }
    return {0x0F, 0x0F};
}

std::optional<ConsoleType> type_from_name(std::string_view name) {
    if (name == "devkit") return ConsoleType::Devkit;
    if (name == "testkit") return ConsoleType::TestKit;
    if (name == "retail-phat") return ConsoleType::RetailPhat;
    if (name == "retail-slim") return ConsoleType::RetailSlim;
    return std::nullopt;
}

Bytes hex_option(const cli::Args& args, std::string_view name, std::size_t size) {
    const std::string value = args.required(name);
    try {
        Bytes bytes = cb::crypto::from_hex(value);
        if (bytes.size() == size) return bytes;
    } catch (const FormatError&) {
    }
    throw UsageError("--" + std::string(name) + " is " + std::to_string(size * 2) + " hex digits");
}

}  // namespace

Output make(ByteView vault, ByteView cpu_key, ByteView dvd_key, ConsoleType type, xecrypt::RandomSource& random) {
    if (vault.size() != xecrypt::kKeyvaultSize) throw UsageError("a keyvault is 0x4000 bytes");
    if (dvd_key.size() != 0x10) throw UsageError("a DVD key is 16 bytes");
    if (!xecrypt::cpu_key_valid(cpu_key)) throw UsageError("not a valid CPU key");

    Bytes sealed(vault.begin(), vault.end());
    std::copy(dvd_key.begin(), dvd_key.end(), sealed.begin() + static_cast<std::ptrdiff_t>(kDvdKeyAt));

    Output out;
    out.fuses.assign(fuses::kDumpSize, 0);
    const Bytes line0 = cb::crypto::from_hex("C0FFFFFFFFFFFFFF");
    fuses::set_line(out.fuses, 0, line0);
    out.fuses[kLockdownAt] = 0xF0;
    const Bytes console = cb::crypto::from_hex("0F0F0F0F0F0F");
    std::copy(console.begin(), console.end(), out.fuses.begin() + 8);
    const auto bytes = type_bytes(type);
    std::copy(bytes.begin(), bytes.end(), out.fuses.begin() + static_cast<std::ptrdiff_t>(kTypeBytesAt));
    fuses::set_line(out.fuses, 3, cpu_key.first(8));
    fuses::set_line(out.fuses, 4, cpu_key.first(8));
    fuses::set_line(out.fuses, 5, cpu_key.subspan(8, 8));
    fuses::set_line(out.fuses, 6, cpu_key.subspan(8, 8));

    out.keyvault = xecrypt::keyvault_encrypt(cpu_key, sealed, random);
    return out;
}

cli::Command command() {
    return cli::Command{
        "kv_tool",
        "make fuses.bin and kv_enc.bin from a keyvault: CPU key, DVD key and console type",
        {cli::Option{"--keyvault", "", "the keyvault to start from (default KV/banned.bin)", cli::Arg::Value, "FILE", "KV/banned.bin"},
         cli::Option{"--output-dir", "", "where to write fuses.bin and kv_enc.bin (default Output/Zero)", cli::Arg::Value, "DIR", "Output/Zero"},
         cli::Option{"--cpu-key", "", "the CPU key in hex (default: a new one)", cli::Arg::Value, "HEX", ""},
         cli::Option{"--dvd-key", "", "the DVD key in hex (default: zeros)", cli::Arg::Value, "HEX", "00000000000000000000000000000000"},
         cli::Option{"--console-type", "", "devkit (default), testkit, retail-phat or retail-slim", cli::Arg::Value, "TYPE", "devkit"}},
        [](const cli::Args& args) {
            const std::string type_name = args.required("console-type");
            const std::optional<ConsoleType> type = type_from_name(type_name);
            if (!type) throw UsageError("unknown console type '" + type_name + "' (devkit, testkit, retail-phat, retail-slim)");
            const std::string given = args.value_or("cpu-key", "");
            const Bytes cpu_key = given.empty() ? xecrypt::cpu_key_generate() : hex_option(args, "cpu-key", 0x10);
            const Bytes dvd_key = hex_option(args, "dvd-key", 0x10);
            const Output out = make(fs::read_file(fs::from_text(args.required("keyvault"))), cpu_key, dvd_key, *type);

            const std::filesystem::path dir = fs::from_text(args.required("output-dir"));
            fs::make_dirs(dir);
            fs::write_file(dir / "fuses.bin", out.fuses);
            fs::write_file(dir / "kv_enc.bin", out.keyvault);
            cb::out("CPU key: {}\nDVD key: {}\n\nFuses:\n", support::hex_upper(cpu_key), support::hex_upper(dvd_key));
            for (std::size_t i = 0; i < fuses::kLineCount; ++i) cb::out("{}\n", support::hex_upper(ByteView(out.fuses).subspan(i * fuses::kLineSize, fuses::kLineSize)));
            cb::out("\nKV written to \"{}\"!\nFuses written to \"{}\"!\n", fs::to_text(std::filesystem::absolute(dir / "kv_enc.bin")),
                    fs::to_text(std::filesystem::absolute(dir / "fuses.bin")));
            return 0;
        },
        {},
        "",
        ""};
}

}  // namespace cb::formats::xbox::crypto::kv_tool
