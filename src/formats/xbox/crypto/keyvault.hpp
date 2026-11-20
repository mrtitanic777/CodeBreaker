// formats/xbox/crypto/keyvault.hpp -- read a decrypted Xbox 360 keyvault (KV_dec.bin, 0x4000 bytes) and say what it holds.
//
//     keyvault::Keyvault kv = keyvault::Keyvault::parse(bytes);
//     kv.console_type;            // ConsoleType::Retail
//     keyvault::describe(kv);     // the report `crypto keyvault <file>` prints
//
// Offsets are the keyvault's documented layout; the numbers are big-endian. The serial is ASCII, the identifiers and keys raw bytes.
// The console type is the certificate's: 1 devkit, 2 retail, 0x40000001 a test kit and 0x40000002 a retail console, both from the XDK
// recovery; anything else is reported as unknown with its value.
//
// Errors: FormatError for fewer than 0x4000 bytes.
#pragma once

#include <array>
#include <cstdint>
#include <string>

#include "core/bytes.hpp"
#include "core/cli.hpp"

namespace cb::formats::xbox::crypto::keyvault {

inline constexpr std::size_t kSize = 0x4000;

enum class ConsoleType : std::uint32_t {
    Devkit = 0x00000001,
    Retail = 0x00000002,
    TestKitRecovery = 0x40000001,
    RetailRecovery = 0x40000002,
};

struct Keyvault {
    std::array<std::uint8_t, 0x10> nonce{};
    std::array<std::uint8_t, 8> obfuscation{};
    std::uint8_t manufacturing_mode = 0;
    std::uint8_t alternate_keyvault = 0;
    std::uint8_t restricted_privilege_flags = 0;
    std::uint16_t odd_features = 0;
    std::uint16_t odd_auth_type = 0;
    std::uint16_t restricted_hv_ext_loader = 0;
    std::uint32_t policy_flash_size = 0;
    std::uint32_t policy_built_in_mu_size = 0;
    std::uint64_t restricted_privileges = 0;

    std::array<std::uint8_t, 0x10> console_obf_key{};
    std::array<std::uint8_t, 0x10> key_obf_key{};
    std::array<std::uint8_t, 0x10> roamable_obf_key{};
    std::array<std::uint8_t, 0x10> odd_key{};
    std::array<std::uint8_t, 0x18> primary_activation_key{};
    std::array<std::uint8_t, 0x10> secondary_activation_key{};
    std::array<std::uint8_t, 0x10> global_dev_2des_key_1{};
    std::array<std::uint8_t, 0x10> global_dev_2des_key_2{};
    std::array<std::uint8_t, 0x10> wireless_controller_2des_key_1{};
    std::array<std::uint8_t, 0x10> wireless_controller_2des_key_2{};

    std::array<std::uint8_t, 12> console_serial{};
    std::array<std::uint8_t, 8> motherboard_serial{};
    std::uint16_t game_region = 0;

    std::uint16_t console_certificate_size = 0;
    std::array<std::uint8_t, 5> console_id{};
    std::array<std::uint8_t, 11> console_part_number{};
    std::uint16_t console_privileges = 0;
    std::uint32_t console_type = 0;      // a ConsoleType's value, or another
    std::array<std::uint8_t, 8> manufacture_date{};
    std::uint32_t console_public_key_exponent = 0;
    Bytes console_public_key_modulus;    // 0x80 bytes

    static Keyvault parse(ByteView data);
};

// "Devkit", "Retail", "Test Kit (XDK recovery)", "Retail (XDK recovery)", or "Unknown (0x........)".
std::string console_type_name(std::uint32_t console_type);
// The regions the game-region word names, joined with "/" ("NTSC-U/PAL"); its value in hex when it names none.
std::string game_region_name(std::uint16_t game_region);

// The report: one "  <label>  <value>" row each for the serial, ids, type, region, keys, the public key and the manufacture date.
std::string describe(const Keyvault& keyvault);

cli::Command command();

}  // namespace cb::formats::xbox::crypto::keyvault
