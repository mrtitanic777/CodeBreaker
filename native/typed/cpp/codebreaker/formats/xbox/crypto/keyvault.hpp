// codebreaker/formats/xbox/crypto/keyvault.hpp -- C++ port of codebreaker.formats.xbox.crypto.keyvault:
// Xbox 360 Keyvault tools -- decrypt, parse, validate, display.
#pragma once

#include <variant>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::keyvault {

// The dict parse_keyvault builds (every key is set when the data is long enough), in the order the
// Python sets them.
struct Keyvault {
    py::bytes nonce;
    py::bytes obfuscation;
    int64_t manufacturing_mode = 0;
    int64_t alternate_keyvault = 0;
    int64_t restricted_privilege_flags = 0;
    int64_t odd_features = 0;
    int64_t odd_auth_type = 0;
    int64_t restricted_hv_ext_loader = 0;
    int64_t policy_flash_size = 0;
    int64_t policy_built_in_mu_size = 0;
    uint64_t restricted_privileges = 0;   // ">Q": the full 64 bits (Python's int is unbounded)
    py::bytes console_obf_key;
    py::bytes key_obf_key;
    py::bytes roamable_obf_key;
    py::bytes odd_key;
    py::bytes primary_activation_key;
    py::bytes secondary_activation_key;
    py::bytes global_dev_2des_key_1;
    py::bytes global_dev_2des_key_2;
    py::bytes wireless_controller_2des_key_1;
    py::bytes wireless_controller_2des_key_2;
    py::bytes console_serial;
    py::bytes motherboard_serial;
    int64_t game_region = 0;
    int64_t console_certificate_size = 0;
    py::bytes console_id;
    py::bytes console_part_number;
    int64_t console_privileges = 0;
    int64_t console_type = 0;
    py::bytes manufacture_date;
    int64_t console_public_key_exponent = 0;
    py::bytes console_public_key_modulus;
    py::str console_type_name;
    py::str game_region_name;
};

// The dict {"error": ...} parse_keyvault returns for data that is too small.
struct KeyvaultError {
    py::str error;
};

// parse_keyvault returns one of two dict shapes: the error dict or the full one (a std::variant of the two).
using ParseResult = std::variant<KeyvaultError, Keyvault>;

template <class F> void for_each_field(const Keyvault& r, F&& f) {
    f("nonce", r.nonce);
    f("obfuscation", r.obfuscation);
    f("manufacturing_mode", r.manufacturing_mode);
    f("alternate_keyvault", r.alternate_keyvault);
    f("restricted_privilege_flags", r.restricted_privilege_flags);
    f("odd_features", r.odd_features);
    f("odd_auth_type", r.odd_auth_type);
    f("restricted_hv_ext_loader", r.restricted_hv_ext_loader);
    f("policy_flash_size", r.policy_flash_size);
    f("policy_built_in_mu_size", r.policy_built_in_mu_size);
    f("restricted_privileges", r.restricted_privileges);
    f("console_obf_key", r.console_obf_key);
    f("key_obf_key", r.key_obf_key);
    f("roamable_obf_key", r.roamable_obf_key);
    f("odd_key", r.odd_key);
    f("primary_activation_key", r.primary_activation_key);
    f("secondary_activation_key", r.secondary_activation_key);
    f("global_dev_2des_key_1", r.global_dev_2des_key_1);
    f("global_dev_2des_key_2", r.global_dev_2des_key_2);
    f("wireless_controller_2des_key_1", r.wireless_controller_2des_key_1);
    f("wireless_controller_2des_key_2", r.wireless_controller_2des_key_2);
    f("console_serial", r.console_serial);
    f("motherboard_serial", r.motherboard_serial);
    f("game_region", r.game_region);
    f("console_certificate_size", r.console_certificate_size);
    f("console_id", r.console_id);
    f("console_part_number", r.console_part_number);
    f("console_privileges", r.console_privileges);
    f("console_type", r.console_type);
    f("manufacture_date", r.manufacture_date);
    f("console_public_key_exponent", r.console_public_key_exponent);
    f("console_public_key_modulus", r.console_public_key_modulus);
    f("console_type_name", r.console_type_name);
    f("game_region_name", r.game_region_name);
}
template <class F> void for_each_field(const KeyvaultError& r, F&& f) { f("error", r.error); }

extern const py::str DESCRIPTION;

// Parse a decrypted keyvault into a structured dictionary.
ParseResult parse_keyvault(const py::bytes& data);

// Return list of (label, value) tuples for display.
py::list<std::tuple<py::str, py::str>> display_keyvault(const ParseResult& kv);

// main() of the Python reads sys.argv[1:]: here they are the parameter.
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::crypto::keyvault
