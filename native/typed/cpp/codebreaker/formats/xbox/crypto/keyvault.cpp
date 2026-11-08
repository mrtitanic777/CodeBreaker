// codebreaker/formats/xbox/crypto/keyvault.cpp -- C++ port of codebreaker.formats.xbox.crypto.keyvault.

#include "codebreaker/formats/xbox/crypto/keyvault.hpp"

namespace codebreaker::formats::xbox::crypto::keyvault {

// ">Q" at an offset, as the full unsigned 64-bit value (py::unpack_from refuses values past int64).
static uint64_t u64_be_(const py::bytes& data, int64_t off) {
    if (data.size() - off < 8) {
        throw py::StructError("unpack_from requires a larger buffer");
    }
    uint64_t v = 0;
    for (int64_t k = 0; k < 8; ++k) {
        v = (v << 8) | static_cast<unsigned char>(data.raw()[static_cast<size_t>(off + k)]);
    }
    return v;
}

// Parse a decrypted keyvault into a structured dictionary.
ParseResult parse_keyvault(const py::bytes& data) {
    if (py::len(data) < 0x4000) {
        return KeyvaultError{py::str("Too small (") + py::to_str(py::len(data)) + " bytes, expected 0x4000)"};
    }
    Keyvault kv;

    // Basic fields
    kv.nonce = data.slice(0x00, 0x10);
    kv.obfuscation = data.slice(0x10, 0x18);
    kv.manufacturing_mode = data[0x18];
    kv.alternate_keyvault = data[0x19];
    kv.restricted_privilege_flags = data[0x1A];
    kv.odd_features = py::unpack_from1<int64_t>(">H", data, 0x1C);
    kv.odd_auth_type = py::unpack_from1<int64_t>(">H", data, 0x1E);
    kv.restricted_hv_ext_loader = py::unpack_from1<int64_t>(">H", data, 0x20);
    kv.policy_flash_size = py::unpack_from1<int64_t>(">I", data, 0x24);
    kv.policy_built_in_mu_size = py::unpack_from1<int64_t>(">I", data, 0x28);
    kv.restricted_privileges = u64_be_(data, 0x30);

    // Keys
    kv.console_obf_key = data.slice(0xBC, 0xCC);
    kv.key_obf_key = data.slice(0xCC, 0xDC);
    kv.roamable_obf_key = data.slice(0xDC, 0xEC);
    kv.odd_key = data.slice(0xEC, 0xFC);
    kv.primary_activation_key = data.slice(0xFC, 0x114);
    kv.secondary_activation_key = data.slice(0x114, 0x124);

    // 2DES keys
    kv.global_dev_2des_key_1 = data.slice(0x124, 0x134);
    kv.global_dev_2des_key_2 = data.slice(0x134, 0x144);
    kv.wireless_controller_2des_key_1 = data.slice(0x144, 0x154);
    kv.wireless_controller_2des_key_2 = data.slice(0x154, 0x164);

    // Console identity
    kv.console_serial = data.slice(0x9C, 0xA8);
    kv.motherboard_serial = data.slice(0xAC, 0xB4);
    kv.game_region = py::unpack_from1<int64_t>(">H", data, 0xB4);

    // Console certificate
    kv.console_certificate_size = py::unpack_from1<int64_t>(">H", data, 0x9C8);
    kv.console_id = data.slice(0x9CA, 0x9CF);
    kv.console_part_number = data.slice(0x9CF, 0x9DA);
    kv.console_privileges = py::unpack_from1<int64_t>(">H", data, 0x9E2);
    kv.console_type = py::unpack_from1<int64_t>(">I", data, 0x9E4);
    kv.manufacture_date = data.slice(0x9E8, 0x9F0);
    kv.console_public_key_exponent = py::unpack_from1<int64_t>(">I", data, 0x9F0);
    kv.console_public_key_modulus = data.slice(0x9F4, 0xA74);

    // Console type decode
    int64_t ct = kv.console_type;
    if (ct == 0x00000001) {
        kv.console_type_name = "Devkit";
    } else if (ct == 0x00000002) {
        kv.console_type_name = "Retail";
    } else if (ct == 0x40000001) {
        kv.console_type_name = "Test Kit (XDK recovery)";
    } else if (ct == 0x40000002) {
        kv.console_type_name = "Retail (XDK recovery)";
    } else {
        kv.console_type_name = py::str("Unknown (0x") + py::format(ct, "08X") + ")";
    }

    // Game region decode
    int64_t gr = kv.game_region;
    py::list<py::str> regions;
    if ((gr & 0x00FF) != 0) {
        regions.append(py::str("NTSC-U"));
    }
    if ((gr & 0xFF00) != 0) {
        regions.append(py::str("PAL"));
    }
    if ((gr & 0x00FE) != 0) {
        regions.append(py::str("NTSC-J"));
    }
    kv.game_region_name = !regions.empty() ? py::join(py::str("/"), regions) : py::str("0x") + py::format(gr, "04X");

    return kv;
}

// Return list of (label, value) tuples for display.
py::list<std::tuple<py::str, py::str>> display_keyvault(const ParseResult& result) {
    py::list<std::tuple<py::str, py::str>> lines;

    if (const KeyvaultError* err = std::get_if<KeyvaultError>(&result)) {
        return py::list<std::tuple<py::str, py::str>>{{py::str("Error"), err->error}};
    }
    const Keyvault& kv = std::get<Keyvault>(result);

    // (the serial is bytes here, so the Python's decode never fails and its "?" fallback never runs)
    py::str serial_str = kv.console_serial.decode("ascii", "replace");

    lines.append({py::str("Console Serial"), serial_str});
    lines.append({py::str("Motherboard Serial"), kv.motherboard_serial.hex().upper()});
    lines.append({py::str("Console ID"), kv.console_id.hex().upper()});
    lines.append({py::str("Console Type"), kv.console_type_name});
    lines.append({py::str("Game Region"), kv.game_region_name});
    lines.append({py::str("Manufacturing Mode"), py::to_str(kv.manufacturing_mode)});
    lines.append({py::str("ODD Features"), py::str("0x") + py::format(kv.odd_features, "04X")});
    lines.append({py::str("ODD Auth Type"), py::str("0x") + py::format(kv.odd_auth_type, "04X")});
    lines.append({py::str("Console Obf Key"), kv.console_obf_key.hex().upper()});
    lines.append({py::str("Key Obf Key"), kv.key_obf_key.hex().upper()});
    lines.append({py::str("Roamable Obf Key"), kv.roamable_obf_key.hex().upper()});
    lines.append({py::str("ODD Key"), kv.odd_key.hex().upper()});
    lines.append({py::str("RSA Pub Exponent"), py::str("0x") + py::format(kv.console_public_key_exponent, "X")});
    lines.append({py::str("RSA Pub Modulus"), kv.console_public_key_modulus.slice(0, 16).hex().upper() + "..."});
    lines.append({py::str("Manufacture Date"), kv.manufacture_date.hex().upper()});

    return lines;
}

const py::str DESCRIPTION = "parse an Xbox 360 keyvault (KV_dec.bin): console id, cert, fields";

// The Python's main() reads sys.argv[1:]; here they are the parameter.
void main(const py::list<py::str>& argv) {
    const py::list<py::str>& a = argv;
    if (a.empty() || a[0] == "-h" || a[0] == "--help") {
        py::print(py::str("usage: keyvault <KV_dec.bin>"));
        return;
    }
    ParseResult kv = parse_keyvault(py::open_rb(a[0]).read());
    for (const auto& line : display_keyvault(kv)) {
        py::print(py::percent("  %-22s %s", std::get<0>(line), std::get<1>(line)));
    }
}

}  // namespace codebreaker::formats::xbox::crypto::keyvault
