#include "formats/xbox/crypto/smc.hpp"

#include <algorithm>
#include <format>
#include <numeric>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::smc {

namespace {

constexpr std::size_t kFlashHeaderSize = 0x80;
constexpr std::size_t kConfigOffsetAt = 0x74;
constexpr std::size_t kSizeAt = 0x78;
constexpr std::size_t kOffsetAt = 0x7C;
constexpr std::size_t kVersionSearchSize = 0x400;
constexpr std::array<std::uint8_t, 2> kFlashMagic = {0xFF, 0x4F};

// The config's field at `at`, when the data reaches it.
std::optional<std::uint8_t> config_byte(ByteView cfg, std::size_t at) {
    if (at >= cfg.size()) return std::nullopt;
    return cfg[at];
}

std::string field_text(const std::optional<std::uint8_t>& value) { return value ? std::to_string(*value) : "-"; }

// Both files named for an operation of the tool.
cli::Command transform_command(std::string name, std::string summary, bool decrypting) {
    return cli::Command{
        std::move(name),
        std::move(summary),
        {cli::Positional{"input", "the SMC image"}, cli::Positional{"output", "the file to write"}},
        [decrypting](const cli::Args& args) {
            const Bytes data = fs::read_file(fs::from_text(args.required("input")));
            const Bytes out = decrypting ? xecrypt::smc_decrypt(data) : xecrypt::smc_encrypt(data);
            fs::write_file(fs::from_text(args.required("output")), out);
            cb::out("{} {} bytes -> {}\n", decrypting ? "decrypted" : "encrypted", out.size(), args.required("output"));
            return 0;
        },
        {}, "", ""};
}

}  // namespace

Extracted extract(ByteView nand) {
    if (nand.size() < kFlashHeaderSize) throw FormatError("smc", nand.size(), "not a NAND image: shorter than the flash header");
    io::ByteReader r(nand, "smc");
    r.seek(kSizeAt);
    const std::uint32_t size = r.u32_be();
    const std::uint32_t offset = r.u32_be();
    if (size == 0 || size > kMaxSize || std::uint64_t{offset} + size > nand.size())
        throw FormatError("smc", kSizeAt, std::format("the SMC fields of the flash header do not fit (offset 0x{:X}, size 0x{:X})", offset, size));
    const ByteView firmware = nand.subspan(offset, size);
    return Extracted{Bytes(firmware.begin(), firmware.end()), offset};
}

std::string_view motherboard_name(std::uint8_t id) {
    switch (id) {
        case 0x00: return "Xenon";
        case 0x01: return "Zephyr";
        case 0x02: return "Falcon/Opus";
        case 0x03: return "Jasper";
        case 0x04: return "Trinity/Corona?";
        case 0x05: return "Corona";
        case 0x06: return "Winchester";
        default: return {};
    }
}

std::optional<Version> version(ByteView decrypted) {
    const ByteView tail = decrypted.size() > kVersionSearchSize ? decrypted.last(kVersionSearchSize) : decrypted;
    if (tail.size() < 5) return std::nullopt;
    for (std::size_t at = 0; at + 4 < tail.size(); ++at) {
        const std::uint8_t major = tail[at], board = tail[at + 1], build = tail[at + 2];
        if (major != 0 && major < 0x10 && !motherboard_name(board).empty() && build < 0x40) {
            Version v;
            std::copy_n(tail.begin() + static_cast<std::ptrdiff_t>(at), 4, v.bytes.begin());
            v.motherboard = motherboard_name(board);
            return v;
        }
    }
    return std::nullopt;
}

Config parse_config(ByteView nand_or_config) {
    ByteView cfg = nand_or_config;
    if (nand_or_config.size() >= kFlashHeaderSize && nand_or_config[0] == kFlashMagic[0] && nand_or_config[1] == kFlashMagic[1]) {
        io::ByteReader r(nand_or_config, "smc");
        r.seek(kConfigOffsetAt);
        const std::uint64_t offset = r.u32_be();
        cfg = offset >= nand_or_config.size() ? ByteView{} : nand_or_config.subspan(offset, std::min<std::uint64_t>(kConfigSize, nand_or_config.size() - offset));
    }
    Config config;
    if (cfg.size() >= 4) config.checksum = io::ByteReader(cfg, "smc config").u32_be();
    config.fan_override_cpu = config_byte(cfg, 0x08);
    config.fan_override_gpu = config_byte(cfg, 0x09);
    config.temp_cpu_target = config_byte(cfg, 0x10);
    config.temp_gpu_target = config_byte(cfg, 0x11);
    config.temp_ram_target = config_byte(cfg, 0x12);
    config.size = cfg.size();
    return config;
}

cli::Command command() {
    cli::Command info{
        "info",
        "size, checksum and motherboard of an SMC image",
        {cli::Positional{"smc", "the SMC image (scrambled)"}},
        [](const cli::Args& args) {
            const Bytes data = fs::read_file(fs::from_text(args.required("smc")));
            const Bytes decrypted = xecrypt::smc_decrypt(data);
            const std::uint32_t sum = std::accumulate(decrypted.begin(), decrypted.end(), std::uint32_t{0});
            cb::out("size        : {} bytes\n", data.size());
            cb::out("checksum(dec): 0x{:08X}\n", sum);
            if (const std::optional<Version> v = version(decrypted))
                cb::out("version     : {}.{}.{}.{}  (motherboard: {})\n", v->bytes[0], v->bytes[1], v->bytes[2], v->bytes[3], v->motherboard);
            else
                cb::out("version     : (not located)\n");
            return 0;
        },
        {}, "", ""};
    cli::Command extract_command{
        "extract",
        "pull the SMC firmware out of a NAND dump",
        {cli::Positional{"nand", "the NAND image"}, cli::Positional{"output", "the file to write"}},
        [](const cli::Args& args) {
            const Extracted smc = extract(fs::read_file(fs::from_text(args.required("nand"))));
            fs::write_file(fs::from_text(args.required("output")), smc.bytes);
            cb::out("SMC @0x{:X} len 0x{:X} -> {}\n", smc.offset, smc.bytes.size(), args.required("output"));
            return 0;
        },
        {}, "", ""};
    cli::Command config_command{
        "config",
        "the SMC config: fan overrides and temperature targets",
        {cli::Positional{"input", "a NAND image or an SMC config"}},
        [](const cli::Args& args) {
            const Config c = parse_config(fs::read_file(fs::from_text(args.required("input"))));
            cb::out("SMC config ({} bytes)  checksum=0x{:08X}\n", c.size, c.checksum);
            cb::out("  fan override  cpu={} gpu={}\n", field_text(c.fan_override_cpu), field_text(c.fan_override_gpu));
            cb::out("  temp targets  cpu={} gpu={} ram={}\n", field_text(c.temp_cpu_target), field_text(c.temp_gpu_target), field_text(c.temp_ram_target));
            return 0;
        },
        {}, "", ""};
    return cli::Command{"smc",
                        "the Xbox 360 SMC: decrypt, encrypt, info, extract, config",
                        {},
                        {},
                        {transform_command("decrypt", "de-obfuscate an SMC image", true), transform_command("encrypt", "re-obfuscate an SMC image", false),
                         std::move(info), std::move(extract_command), std::move(config_command)},
                        "",
                        ""};
}

}  // namespace cb::formats::xbox::crypto::smc
