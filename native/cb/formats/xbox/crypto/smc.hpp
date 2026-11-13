// cb/formats/xbox/crypto/smc.hpp -- the Xbox 360's SMC (system management controller): the small always-on chip that runs the fans, power,
// the front LEDs, temperature and the eject button. Its firmware lies in the NAND, scrambled with the "BuNy" rolling cipher
// (xecrypt::smc_decrypt/smc_encrypt); the RGH and JTAG hacks patch the decrypted firmware.
//
//     Bytes firmware = smc::extract(nand).bytes;               // the scrambled SMC out of a NAND image
//     std::optional<smc::Version> v = smc::version(smc::decrypt(firmware));
//     smc::Config config = smc::parse_config(nand);
//
// The flash header (big-endian) holds the SMC config's offset at 0x74, the SMC's size at 0x78 and its offset at 0x7C.
//
// Commands of `crypto smc`: decrypt <in> <out>, encrypt <in> <out>, info <smc>, extract <nand> <out>, config <nand|config>.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"

namespace cb::formats::xbox::crypto::smc {

inline constexpr std::size_t kMaxSize = 0x4000;       // the biggest SMC the flash header may name
inline constexpr std::size_t kConfigSize = 0x2A0;

struct Extracted {
    Bytes bytes;
    std::uint32_t offset = 0;
};

// The SMC firmware (still scrambled) a NAND image's flash header points at. FormatError for an image shorter than its header, or a size
// or place that does not fit (a size of 0 or over 0x4000, or past the end of the image).
Extracted extract(ByteView nand);

struct Version {
    std::array<std::uint8_t, 4> bytes{};   // major, motherboard, build, revision
    std::string_view motherboard;           // the board the second byte names
};

// "Xenon", "Zephyr", "Falcon/Opus", "Jasper", "Trinity/Corona?", "Corona", "Winchester" for the motherboard byte; empty when it is none.
std::string_view motherboard_name(std::uint8_t id);

// The four-byte version record in the last 0x400 bytes of a decrypted image: a first byte of 1..15, a known motherboard byte and a build
// byte under 0x40. nullopt when none looks like one.
std::optional<Version> version(ByteView decrypted);

struct Config {
    std::uint32_t checksum = 0;
    std::optional<std::uint8_t> fan_override_cpu;   // 0: automatic
    std::optional<std::uint8_t> fan_override_gpu;
    std::optional<std::uint8_t> temp_cpu_target;    // raw SMC units
    std::optional<std::uint8_t> temp_gpu_target;
    std::optional<std::uint8_t> temp_ram_target;
    std::size_t size = 0;                           // the bytes the config was read from
};

// The config out of a NAND image (its flash header names where) or out of a config on its own. Fields the data is too short for are absent.
Config parse_config(ByteView nand_or_config);

cli::Command command();

}  // namespace cb::formats::xbox::crypto::smc
