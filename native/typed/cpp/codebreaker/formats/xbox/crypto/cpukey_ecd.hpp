// codebreaker/formats/xbox/crypto/cpukey_ecd.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.cpukey_ecd.
//
// CPU key validation with ECD (Error Correction and Detection).
//
// Ported from SK.Xbox360.CPUKey by Stelio Kontos.
// Provides rigorous validation: length, non-zero, Hamming weight (0x35),
// and ECD bit verification using the Xbox 360's LFSR-based algorithm
// with the magic constant 0x360325.
//
// Typing notes:
//   validate_cpukey returns a dict of fixed keys: here the record CpuKeyCheck (same keys, same order).
//   main() reads sys.argv[1:] in the Python; here the argument list is passed in.
//   generate_cpukey draws from os.urandom in the Python; here from std::random_device.
#pragma once

#include <cstdint>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::cpukey_ecd {

extern const int64_t VALID_HAMMING_WEIGHT;
extern const int64_t ECD_MAGIC;

// The result of validate_cpukey (the Python's dict with these keys, in this order).
struct CpuKeyCheck {
    bool valid = false;            // overall validity
    bool length_ok = false;
    bool non_zero = false;
    bool hamming_ok = false;
    int64_t hamming_weight = 0;
    bool ecd_ok = false;
    py::list<py::str> errors;      // error strings
};
template <class F> void for_each_field(const CpuKeyCheck& r, F&& f) {
    f("valid", r.valid);
    f("length_ok", r.length_ok);
    f("non_zero", r.non_zero);
    f("hamming_ok", r.hamming_ok);
    f("hamming_weight", r.hamming_weight);
    f("ecd_ok", r.ecd_ok);
    f("errors", r.errors);
}
template <class F> void for_each_field_mut(CpuKeyCheck& r, F&& f) {
    f("valid", r.valid, true);
    f("length_ok", r.length_ok, true);
    f("non_zero", r.non_zero, true);
    f("hamming_ok", r.hamming_ok, true);
    f("hamming_weight", r.hamming_weight, true);
    f("ecd_ok", r.ecd_ok, true);
    f("errors", r.errors, true);
}

// Compute the ECD bits of a 16-byte CPU key and return the key with them set (a copy; the
// argument is not modified, as in the Python despite its docstring).
//
// The ECD algorithm uses a customized LFSR variant:
//   - Bits 0-105: data payload, processed with XOR feedback
//   - Bits 106-126: ECD correction bits (21 bits)
//   - Bit 127: overall parity bit
// Raises IndexError for fewer than 16 bytes.
py::bytearray compute_ecd(const py::bytearray& data);

// Verify the ECD bits in a 16-byte CPU key.
bool verify_ecd(const py::bytes& data);

// Compute the Hamming weight of a CPU key, excluding ECD bits (-1 unless 16 bytes).
//
// The last 22 bits (bits 106-127) are ECD and excluded from the count.
// Valid CPU keys have a Hamming weight of exactly 0x35 (53).
int64_t compute_hamming_weight(const py::bytes& data);

// Verify the Hamming weight of a CPU key is exactly 0x35.
bool verify_hamming_weight(const py::bytes& data);

// Full validation of a CPU key hex string.
CpuKeyCheck validate_cpukey(const py::str& key_hex);

// Generate a random valid CPU key with correct Hamming weight and ECD.
py::str generate_cpukey();

extern const py::str DESCRIPTION;

// `a` is sys.argv[1:].
void main(const py::list<py::str>& a);

}  // namespace codebreaker::formats::xbox::crypto::cpukey_ecd
