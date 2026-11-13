#include "cb/formats/xbox/crypto/keyvault.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/xbox/crypto/tool_support.hpp"

namespace cb::formats::xbox::crypto::keyvault {

namespace {

template <std::size_t N>
void fill_array(std::array<std::uint8_t, N>& out, ByteView data, std::size_t at) {
    const ByteView part = io::checked_range(data, at, N);
    std::copy(part.begin(), part.end(), out.begin());
}

}  // namespace

Keyvault Keyvault::parse(ByteView data) {
    if (data.size() < kSize) throw FormatError("keyvault", data.size(), std::format("too small ({} bytes, expected 0x{:X})", data.size(), kSize));
    Keyvault kv;
    io::ByteReader r(data, "keyvault");
    fill_array(kv.nonce, data, 0x00);
    fill_array(kv.obfuscation, data, 0x10);
    kv.manufacturing_mode = data[0x18];
    kv.alternate_keyvault = data[0x19];
    kv.restricted_privilege_flags = data[0x1A];
    r.seek(0x1C);
    kv.odd_features = r.u16_be();
    kv.odd_auth_type = r.u16_be();
    kv.restricted_hv_ext_loader = r.u16_be();
    r.seek(0x24);
    kv.policy_flash_size = r.u32_be();
    kv.policy_built_in_mu_size = r.u32_be();
    r.seek(0x30);
    kv.restricted_privileges = r.u64_be();

    fill_array(kv.console_obf_key, data, 0xBC);
    fill_array(kv.key_obf_key, data, 0xCC);
    fill_array(kv.roamable_obf_key, data, 0xDC);
    fill_array(kv.odd_key, data, 0xEC);
    fill_array(kv.primary_activation_key, data, 0xFC);
    fill_array(kv.secondary_activation_key, data, 0x114);
    fill_array(kv.global_dev_2des_key_1, data, 0x124);
    fill_array(kv.global_dev_2des_key_2, data, 0x134);
    fill_array(kv.wireless_controller_2des_key_1, data, 0x144);
    fill_array(kv.wireless_controller_2des_key_2, data, 0x154);

    fill_array(kv.console_serial, data, 0x9C);
    fill_array(kv.motherboard_serial, data, 0xAC);
    r.seek(0xB4);
    kv.game_region = r.u16_be();

    r.seek(0x9C8);
    kv.console_certificate_size = r.u16_be();
    fill_array(kv.console_id, data, 0x9CA);
    fill_array(kv.console_part_number, data, 0x9CF);
    r.seek(0x9E2);
    kv.console_privileges = r.u16_be();
    kv.console_type = r.u32_be();
    fill_array(kv.manufacture_date, data, 0x9E8);
    r.seek(0x9F0);
    kv.console_public_key_exponent = r.u32_be();
    const ByteView modulus = io::checked_range(data, 0x9F4, 0x80);
    kv.console_public_key_modulus.assign(modulus.begin(), modulus.end());
    return kv;
}

std::string console_type_name(std::uint32_t console_type) {
    switch (static_cast<ConsoleType>(console_type)) {
        case ConsoleType::Devkit: return "Devkit";
        case ConsoleType::Retail: return "Retail";
        case ConsoleType::TestKitRecovery: return "Test Kit (XDK recovery)";
        case ConsoleType::RetailRecovery: return "Retail (XDK recovery)";
    }
    return std::format("Unknown (0x{:08X})", console_type);
}

std::string game_region_name(std::uint16_t game_region) {
    std::string name;
    const auto add = [&name](std::string_view region) { name += (name.empty() ? "" : "/") + std::string(region); };
    if (game_region & 0x00FF) add("NTSC-U");
    if (game_region & 0xFF00) add("PAL");
    if (game_region & 0x00FE) add("NTSC-J");
    return name.empty() ? std::format("0x{:04X}", game_region) : name;
}

std::string describe(const Keyvault& kv) {
    // the serial is printed as text; a byte that is not ASCII shows as U+FFFD
    std::string serial;
    for (const std::uint8_t b : kv.console_serial) serial += b < 0x80 ? std::string(1, static_cast<char>(b)) : std::string("\xEF\xBF\xBD");
    std::string out;
    const auto row = [&out](std::string_view label, const std::string& value) { out += std::format("  {:<22} {}\n", label, value); };
    row("Console Serial", serial);
    row("Motherboard Serial", support::hex_upper(kv.motherboard_serial));
    row("Console ID", support::hex_upper(kv.console_id));
    row("Console Type", console_type_name(kv.console_type));
    row("Game Region", game_region_name(kv.game_region));
    row("Manufacturing Mode", std::to_string(kv.manufacturing_mode));
    row("ODD Features", std::format("0x{:04X}", kv.odd_features));
    row("ODD Auth Type", std::format("0x{:04X}", kv.odd_auth_type));
    row("Console Obf Key", support::hex_upper(kv.console_obf_key));
    row("Key Obf Key", support::hex_upper(kv.key_obf_key));
    row("Roamable Obf Key", support::hex_upper(kv.roamable_obf_key));
    row("ODD Key", support::hex_upper(kv.odd_key));
    row("RSA Pub Exponent", std::format("0x{:X}", kv.console_public_key_exponent));
    row("RSA Pub Modulus", support::hex_upper(ByteView(kv.console_public_key_modulus).first(16)) + "...");
    row("Manufacture Date", support::hex_upper(kv.manufacture_date));
    return out;
}

cli::Command command() {
    return cli::Command{"keyvault",
                        "parse an Xbox 360 keyvault (KV_dec.bin): console id, certificate, keys",
                        {cli::Positional{"keyvault", "the decrypted keyvault (0x4000 bytes)"}},
                        [](const cli::Args& args) {
                            cb::out_text(describe(Keyvault::parse(fs::read_file(fs::from_text(args.required("keyvault"))))));
                            return 0;
                        },
                        {},
                        "",
                        ""};
}

}  // namespace cb::formats::xbox::crypto::keyvault
