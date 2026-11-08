// codebreaker/formats/xbox/crypto/fuses.cpp -- C++ port of codebreaker.formats.xbox.crypto.fuses (see fuses.hpp).

#include "codebreaker/formats/xbox/crypto/fuses.hpp"

namespace codebreaker::formats::xbox::crypto::fuses {

namespace {

// fuse_data[lo:hi].hex().upper()
py::str hex_line(const py::bytes& fuse_data, int64_t lo, int64_t hi) {
    return fuse_data.slice(lo, hi).hex().upper();
}

py::str fuse_text(const std::variant<py::str, int64_t>& v) {
    return py::to_str(v);
}

}  // namespace

// Set a fuse line (8 bytes) in fuse data.
py::bytearray set_fuseline(const py::bytes& fuse_data, int64_t line, const FuseValue& value_in) {
    py::bytearray data(fuse_data);
    py::bytes value;
    if (std::holds_alternative<int64_t>(value_in)) {
        value = py::pack(">Q", std::get<int64_t>(value_in));
    } else if (std::holds_alternative<py::str>(value_in)) {
        const py::str& text = std::get<py::str>(value_in);
        if (!(py::len(text) == 16)) {
            throw py::AssertionError("Invalid fuse line (need 16 hex chars)");
        }
        value = py::bytes::fromhex(text);
    } else {
        value = std::get<py::bytes>(value_in);
    }

    if (!(py::len(value) == 8)) {
        throw py::AssertionError("Invalid fuse line (need 8 bytes)");
    }
    // data[(line * 8):(line * 8) + 8] = value  (a slice assignment: the span is replaced)
    int64_t lo = py::checked_mul(line, 8);
    int64_t hi = py::checked_add(py::checked_mul(line, 8), 8);
    data.set_slice(lo, hi, value);
    return data;
}

// Get a fuse line (8 bytes) from fuse data.
py::bytes get_fuseline(const py::bytes& fuse_data, int64_t line) {
    return fuse_data.slice(py::checked_mul(line, 8), py::checked_add(py::checked_mul(line, 8), 8));
}

// Extract CPU key from fuse data (lines 3-4).
py::bytes get_cpu_key_from_fuses(const py::bytes& fuse_data) {
    return fuse_data.slice(0x18, 0x28);
}

// Parse fuse data into structured info.
FuseInfo parse_fuses(const py::bytes& fuse_data) {
    FuseInfo info;
    if (py::len(fuse_data) < 0x60) {  // 12 fuse lines x 8 bytes
        info = FuseInfo();
        info.set(py::str("error"), py::str("Fuse data too small"));
        return info;
    }

    info.set(py::str("fuseline_00"), hex_line(fuse_data, 0x00, 0x08));
    info.set(py::str("fuseline_01"), hex_line(fuse_data, 0x08, 0x10));
    info.set(py::str("fuseline_02"), hex_line(fuse_data, 0x10, 0x18));
    info.set(py::str("cpu_key"), hex_line(fuse_data, 0x18, 0x28));  // lines 3-4
    info.set(py::str("fuseline_05"), hex_line(fuse_data, 0x28, 0x30));
    info.set(py::str("fuseline_06"), hex_line(fuse_data, 0x30, 0x38));
    info.set(py::str("fuseline_07"), hex_line(fuse_data, 0x38, 0x40));
    info.set(py::str("fuseline_08"), hex_line(fuse_data, 0x40, 0x48));
    info.set(py::str("fuseline_09"), hex_line(fuse_data, 0x48, 0x50));
    info.set(py::str("fuseline_10"), hex_line(fuse_data, 0x50, 0x58));
    info.set(py::str("fuseline_11"), hex_line(fuse_data, 0x58, 0x60));

    // Lock Down Value (LDV) -- indicates how many updates have been applied
    int64_t ldv = 0;
    for (int64_t byte : fuse_data.slice(0x30, 0x40)) {  // fuselines 6-7
        for (int64_t bit = 0; bit < 8; ++bit) {
            if ((byte & (INT64_C(1) << bit)) != 0) {
                ldv += 1;
            }
        }
    }
    info.set(py::str("ldv"), ldv);

    return info;
}

const py::str DESCRIPTION("parse an Xbox 360 eFuse dump (fuselines, CPU key, retail/devkit)");

void main(const py::list<py::str>& argv) {
    const py::list<py::str>& a = argv;
    if (a.empty() || a[0] == py::str("-h") || a[0] == py::str("--help")) {
        py::print(py::str("usage: fuses <fuse-dump>"));
        return;
    }
    py::bytes data = py::open(a[0], py::str("rb")).read();
    FuseInfo info = parse_fuses(data);
    for (const auto& kv : info.items()) {
        py::print(py::percent(py::str("  %-20s %s"), std::get<0>(kv), fuse_text(std::get<1>(kv))));
    }
    try {
        py::bytes ck = get_cpu_key_from_fuses(data);
        if (!ck.empty()) {
            py::print(py::percent(py::str("  %-20s %s"), py::str("cpu_key"), ck.hex().upper()));
        }
    } catch (const py::Exception&) {
    }
}

}  // namespace codebreaker::formats::xbox::crypto::fuses
