"""Xbox 360 Keyvault tools — decrypt, parse, validate, display."""

from struct import unpack_from
from typing import Optional

def parse_keyvault(data: bytes) -> dict:
    """Parse a decrypted keyvault into a structured dictionary."""
    kv = {}

    if len(data) < 0x4000:
        return {"error": f"Too small ({len(data)} bytes, expected 0x4000)"}

    # Basic fields
    kv["nonce"] = data[0x00:0x10]
    kv["obfuscation"] = data[0x10:0x18]
    kv["manufacturing_mode"] = data[0x18]
    kv["alternate_keyvault"] = data[0x19]
    kv["restricted_privilege_flags"] = data[0x1A]
    kv["odd_features"] = unpack_from(">H", data, 0x1C)[0]
    kv["odd_auth_type"] = unpack_from(">H", data, 0x1E)[0]
    kv["restricted_hv_ext_loader"] = unpack_from(">H", data, 0x20)[0]
    kv["policy_flash_size"] = unpack_from(">I", data, 0x24)[0]
    kv["policy_built_in_mu_size"] = unpack_from(">I", data, 0x28)[0]
    kv["restricted_privileges"] = unpack_from(">Q", data, 0x30)[0]

    # Keys
    kv["console_obf_key"] = data[0xBC:0xCC]
    kv["key_obf_key"] = data[0xCC:0xDC]
    kv["roamable_obf_key"] = data[0xDC:0xEC]
    kv["odd_key"] = data[0xEC:0xFC]
    kv["primary_activation_key"] = data[0xFC:0x114]
    kv["secondary_activation_key"] = data[0x114:0x124]

    # 2DES keys
    kv["global_dev_2des_key_1"] = data[0x124:0x134]
    kv["global_dev_2des_key_2"] = data[0x134:0x144]
    kv["wireless_controller_2des_key_1"] = data[0x144:0x154]
    kv["wireless_controller_2des_key_2"] = data[0x154:0x164]

    # Console identity
    kv["console_serial"] = data[0x9C:0xA8]
    kv["motherboard_serial"] = data[0xAC:0xB4]
    kv["game_region"] = unpack_from(">H", data, 0xB4)[0]

    # Console certificate
    kv["console_certificate_size"] = unpack_from(">H", data, 0x9C8)[0]
    kv["console_id"] = data[0x9CA:0x9CF]
    kv["console_part_number"] = data[0x9CF:0x9DA]
    kv["console_privileges"] = unpack_from(">H", data, 0x9E2)[0]
    kv["console_type"] = unpack_from(">I", data, 0x9E4)[0]
    kv["manufacture_date"] = data[0x9E8:0x9F0]
    kv["console_public_key_exponent"] = unpack_from(">I", data, 0x9F0)[0]
    kv["console_public_key_modulus"] = data[0x9F4:0xA74]

    # Console type decode
    ct = kv["console_type"]
    if ct == 0x00000001:
        kv["console_type_name"] = "Devkit"
    elif ct == 0x00000002:
        kv["console_type_name"] = "Retail"
    elif ct == 0x40000001:
        kv["console_type_name"] = "Test Kit (XDK recovery)"
    elif ct == 0x40000002:
        kv["console_type_name"] = "Retail (XDK recovery)"
    else:
        kv["console_type_name"] = f"Unknown (0x{ct:08X})"

    # Game region decode
    gr = kv["game_region"]
    regions = []
    if gr & 0x00FF: regions.append("NTSC-U")
    if gr & 0xFF00: regions.append("PAL")
    if gr & 0x00FE: regions.append("NTSC-J")
    kv["game_region_name"] = "/".join(regions) if regions else f"0x{gr:04X}"

    return kv

def display_keyvault(kv: dict) -> list:
    """Return list of (label, value) tuples for display."""
    lines = []

    if "error" in kv:
        return [("Error", kv["error"])]

    try:
        serial = kv["console_serial"]
        serial_str = serial.decode('ascii', errors='replace') if isinstance(serial, bytes) else str(serial)
    except:
        serial_str = "?"

    lines.append(("Console Serial", serial_str))
    lines.append(("Motherboard Serial", kv.get("motherboard_serial", b"").hex().upper()))
    lines.append(("Console ID", kv.get("console_id", b"").hex().upper()))
    lines.append(("Console Type", kv.get("console_type_name", "?")))
    lines.append(("Game Region", kv.get("game_region_name", "?")))
    lines.append(("Manufacturing Mode", str(kv.get("manufacturing_mode", "?"))))
    lines.append(("ODD Features", f"0x{kv.get('odd_features', 0):04X}"))
    lines.append(("ODD Auth Type", f"0x{kv.get('odd_auth_type', 0):04X}"))
    lines.append(("Console Obf Key", kv.get("console_obf_key", b"").hex().upper()))
    lines.append(("Key Obf Key", kv.get("key_obf_key", b"").hex().upper()))
    lines.append(("Roamable Obf Key", kv.get("roamable_obf_key", b"").hex().upper()))
    lines.append(("ODD Key", kv.get("odd_key", b"").hex().upper()))
    lines.append(("RSA Pub Exponent", f"0x{kv.get('console_public_key_exponent', 0):X}"))
    lines.append(("RSA Pub Modulus", kv.get("console_public_key_modulus", b"")[:16].hex().upper() + "..."))
    lines.append(("Manufacture Date", kv.get("manufacture_date", b"").hex().upper()))

    return lines


DESCRIPTION = "parse an Xbox 360 keyvault (KV_dec.bin): console id, cert, fields"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: keyvault <KV_dec.bin>"); return
    kv = parse_keyvault(open(a[0], "rb").read())
    for line in display_keyvault(kv):
        if isinstance(line, (tuple, list)):
            print("  %-22s %s" % (str(line[0]), line[1] if len(line) > 1 else ""))
        else:
            print(line)


if __name__ == "__main__":
    main()
