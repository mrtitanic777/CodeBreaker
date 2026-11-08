// codebreaker/formats/xbox/crypto/kv_tool.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.kv_tool.

#include "codebreaker/formats/xbox/crypto/kv_tool.hpp"

#include "codebreaker/formats/xbox/crypto/MemCrypto.hpp"
#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"

namespace codebreaker::formats::xbox::crypto::kv_tool {

namespace {

// bytes.fromhex(text): only ASCII whitespace (space, \t \n \v \f \r) is skipped between the byte pairs. (The
// runtime's py::bytes::fromhex also skips the other whitespace of str.isspace -- U+001C..U+001F, U+0085,
// U+00A0, U+2003 ... -- which bytes.fromhex rejects, so anything else than a hex digit or ASCII whitespace is
// refused here, with the ValueError bytes.fromhex raises.)
py::bytes bytes_fromhex(const py::str& text) {
    for (char32_t c : text.u32()) {
        bool hex_digit = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
        bool ascii_space = c == ' ' || (c >= 9 && c <= 13);
        if (!hex_digit && !ascii_space) throw py::ValueError("non-hexadecimal number found in fromhex() arg");
    }
    return py::bytes::fromhex(text);
}

// The 8 bytes a fuse line is set to: an int packed as ">Q", a hex string of 16 characters, or bytes.
py::bytes fuse_line_bytes(const FuseValue& value) {
    py::bytes bytes_value;
    if (const int64_t* number = std::get_if<int64_t>(&value)) {
        bytes_value = py::pack(">Q", *number);
    } else if (const py::str* text = std::get_if<py::str>(&value)) {
        if (!(py::len(*text) == 16)) throw py::AssertionError("Invalid fuse line");
        bytes_value = bytes_fromhex(*text);
    } else if (const py::bytes* raw = std::get_if<py::bytes>(&value)) {
        bytes_value = *raw;
    } else {
        bytes_value = py::bytes(std::get<py::bytearray>(value));
    }

    if (!(py::len(bytes_value) == 8)) throw py::AssertionError("Invalid fuse line");
    return bytes_value;
}

}  // namespace

py::bytearray set_fuseline(const py::bytearray& fuse_data, int64_t line, const FuseValue& value) {
    py::bytes bytes_value = fuse_line_bytes(value);
    fuse_data.set_slice(line * 8, (line * 8) + 8, bytes_value);

    return fuse_data;
}

py::bytearray set_fuseline(const py::bytes& fuse_data, int64_t line, const FuseValue& value) {
    // if type(fuse_data) is bytes: fuse_data = bytearray(fuse_data)
    return set_fuseline(py::bytearray(fuse_data), line, value);
}

int64_t main() {
    py::bytes cpu_key = XeCrypt::XeCryptCpuKeyGen();
    py::bytes dvd_key = py::bytes::zeros(16);
    ConsoleType console_type = ConsoleType::DEVKIT;

    py::print(py::str("CPU key: ") + cpu_key.hex().upper());
    py::print(py::str("DVD key: ") + dvd_key.hex().upper());

    // create fuse buffer 12 lines by 8 bytes
    py::bytearray fuse_data = py::bytearray::zeros(12 * 8);
    // ("8s", fuse_data, 0, bytes.fromhex("C0FFFFFFFFFFFFFF"))  # line #1
    fuse_data = set_fuseline(fuse_data, 0, py::str("C0FFFFFFFFFFFFFF"));   // line #1
    fuse_data.set_slice(0x38, 0x38 + 1, py::bytes("\xF0"));

    // read the KV
    py::str kv_path("KV/banned.bin");
    py::bytearray kv_data(MemCrypto::read_file(kv_path));

    // update the DVD key
    kv_data.set_slice(0x100, 0x100 + 0x10, dvd_key);

    // encrypt the KV with the specified CPU key
    py::bytes kv_enc = XeCrypt::XeCryptKeyVaultEncrypt(cpu_key, kv_data);

    // update console type
    fuse_data.set_slice(8, 8 + 6, py::bytes::fromhex(py::str("0F0F0F0F0F0F")));
    if (console_type == ConsoleType::TEST_KIT) {
        fuse_data.set_slice(0xE, 0xE + 2, py::bytes::fromhex(py::str("F00F")));
    } else if (console_type == ConsoleType::DEVKIT) {
        fuse_data.set_slice(0xE, 0xE + 2, py::bytes::fromhex(py::str("0F0F")));
    } else if (console_type == ConsoleType::RETAIL_PHAT) {
        fuse_data.set_slice(0xE, 0xE + 2, py::bytes::fromhex(py::str("0FF0")));
    } else if (console_type == ConsoleType::RETAIL_SLIM) {
        fuse_data.set_slice(0xE, 0xE + 2, py::bytes::fromhex(py::str("F0F0")));
    }

    // update CPU key in fuses
    fuse_data = set_fuseline(fuse_data, 3, cpu_key.slice(0, 8));
    fuse_data = set_fuseline(fuse_data, 4, cpu_key.slice(0, 8));
    fuse_data = set_fuseline(fuse_data, 5, cpu_key.slice(8, 16));
    fuse_data = set_fuseline(fuse_data, 6, cpu_key.slice(8, 16));

    // setup fuse path
    py::str fuse_path("Output/Zero/fuses.bin");
    // write fuses
    MemCrypto::write_file(fuse_path, fuse_data.view());
    // setup KV path
    kv_path = py::str("Output/Zero/kv_enc.bin");
    // write the KV
    MemCrypto::write_file(kv_path, kv_enc);

    // print fuse lines
    py::print();
    py::print(py::str("Fuses:"));
    for (int64_t i = 0; i < 12; ++i) {
        py::print(fuse_data.slice(i * 8, (i * 8) + 8).hex().upper());
    }

    // print output paths: Path(...).absolute(), the current folder joined with the path
    py::print();
    py::print(py::str("KV written to \"") + py::os::path::join(py::os::getcwd(), py::str("Output\\Zero\\kv_enc.bin")) + py::str("\"!"));
    py::print(py::str("Fuses written to \"") + py::os::path::join(py::os::getcwd(), py::str("Output\\Zero\\fuses.bin")) + py::str("\"!"));

    return 0;
}

}  // namespace codebreaker::formats::xbox::crypto::kv_tool
