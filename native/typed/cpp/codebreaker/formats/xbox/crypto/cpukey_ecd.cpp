// codebreaker/formats/xbox/crypto/cpukey_ecd.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.cpukey_ecd.

#include "codebreaker/formats/xbox/crypto/cpukey_ecd.hpp"

#include <random>

namespace codebreaker::formats::xbox::crypto::cpukey_ecd {

const int64_t VALID_HAMMING_WEIGHT = 0x35;
const int64_t ECD_MAGIC = 0x360325;

py::bytearray compute_ecd(const py::bytearray& data) {
    py::bytearray buf(data.view());
    int64_t acc1 = 0;
    int64_t acc2 = 0;

    for (int64_t i = 0; i < 128; ++i) {
        int64_t b_tmp = buf[i >> 3];
        int64_t dw_tmp = (b_tmp >> (i & 7)) & 1;

        if (i < 0x6A) {
            acc1 ^= dw_tmp;
            if (acc1 & 1) {
                acc1 ^= ECD_MAGIC;
            }
            acc2 ^= dw_tmp;
        } else if (i < 0x7F) {
            if (dw_tmp != (acc1 & 1)) {
                buf[i >> 3] = ((int64_t(1) << (i & 7)) ^ (b_tmp & 0xFF)) & 0xFF;
            }
            acc2 ^= (acc1 & 1);
        } else {
            if (dw_tmp != acc2) {
                buf[0xF] = ((0x80 ^ b_tmp) & 0xFF);
            }
        }

        acc1 >>= 1;
    }

    return buf;
}

bool verify_ecd(const py::bytes& data) {
    if (py::len(data) != 16) {
        return false;
    }
    py::bytearray recomputed = compute_ecd(py::bytearray(data));
    return recomputed == data;
}

namespace {

int64_t popcount64(uint64_t v) {
    int64_t n = 0;
    for (; v; v &= v - 1) ++n;
    return n;
}

}  // namespace

int64_t compute_hamming_weight(const py::bytes& data) {
    if (py::len(data) != 16) {
        return -1;
    }

    // The ECD occupies bits 106-127 in the order compute_ecd walks them (bit i is bit i & 7 of byte i >> 3):
    // the top six bits of byte 13 and all of bytes 14 and 15. Those are the bits left out of the count, so the
    // weight does not change when compute_ecd sets them (XeCryptCpuKeyValid masks the same bits).
    int64_t n = 0;
    for (int64_t k = 0; k < 13; ++k) n += popcount64(static_cast<uint64_t>(data[k]));
    return n + popcount64(static_cast<uint64_t>(data[13]) & 0x03);
}

bool verify_hamming_weight(const py::bytes& data) {
    return compute_hamming_weight(data) == VALID_HAMMING_WEIGHT;
}

CpuKeyCheck validate_cpukey(const py::str& key_hex_in) {
    CpuKeyCheck result;

    // Clean input
    py::str key_hex = key_hex_in.strip().replace(py::str(" "), py::str("")).replace(py::str("-"), py::str(""));

    // Length check
    if (py::len(key_hex) != 32) {
        result.errors.append(py::str("Length ") + py::to_str(py::len(key_hex)) + py::str(" != 32 hex chars"));
        return result;
    }
    result.length_ok = true;

    // Hex parse
    py::bytes key_bytes;
    try {
        key_bytes = py::bytes::fromhex(key_hex);
    } catch (const py::ValueError&) {
        result.errors.append(py::str("Invalid hex characters"));
        return result;
    }

    // Non-zero check
    if (key_bytes == py::bytes::zeros(16)) {
        result.errors.append(py::str("Key is all zeros"));
        return result;
    }
    result.non_zero = true;

    // Hamming weight
    int64_t hw = compute_hamming_weight(key_bytes);
    result.hamming_weight = hw;
    if (hw != VALID_HAMMING_WEIGHT) {
        result.errors.append(py::str("Hamming weight ") + py::to_str(hw) + py::str(" != ") +
                             py::to_str(VALID_HAMMING_WEIGHT) + py::str(" (0x") +
                             py::format(VALID_HAMMING_WEIGHT, "X") + py::str(")"));
    } else {
        result.hamming_ok = true;
    }

    // ECD verification
    if (verify_ecd(key_bytes)) {
        result.ecd_ok = true;
    } else {
        result.errors.append(py::str("ECD bits invalid"));
    }

    result.valid = result.hamming_ok && result.ecd_ok;
    return result;
}

py::str generate_cpukey() {
    std::random_device rd;
    while (true) {
        py::bytearray key = py::bytearray::zeros(16);
        for (int64_t k = 0; k < 16; ++k) key[k] = static_cast<int64_t>(rd() & 0xFF);
        if (compute_hamming_weight(key.view()) == VALID_HAMMING_WEIGHT) {
            key = compute_ecd(key);
            return key.hex().upper();
        }
    }
}

const py::str DESCRIPTION("validate or generate an Xbox 360 CPU key (ECD + Hamming weight)");

void main(const py::list<py::str>& a) {
    if (a.empty() || a[0] == py::str("-h") || a[0] == py::str("--help")) {
        py::print(py::str("usage: cpukey <hexkey>   |   cpukey gen"));
        return;
    }
    if (a[0] == py::str("gen") || a[0] == py::str("generate")) {
        py::print(generate_cpukey());
        return;
    }
    CpuKeyCheck r = validate_cpukey(a[0]);
    for_each_field(r, [](const char* k, const auto& v) {
        py::print(py::percent(py::str("  %-18s %s"), py::str(k), py::to_str(v)));
    });
}

}  // namespace codebreaker::formats::xbox::crypto::cpukey_ecd
