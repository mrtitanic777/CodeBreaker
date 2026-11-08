// codebreaker/formats/xbox/crypto/xval.cpp -- C++ from codebreaker.formats.xbox.crypto.xval.

#include "codebreaker/formats/xbox/crypto/xval.hpp"

#include <array>

namespace codebreaker::formats::xbox::crypto::xval {

const int64_t FLAG_SSB_NONE = 0x0000;
const int64_t FLAG_SSB_AUTH_EX_FAILURE_D = 0x0001;             // DEPRECATED
const int64_t FLAG_SSB_AUTH_EX_NO_TABLE_D = 0x0002;            // DEPRECATED
const int64_t FLAG_SSB_AUTH_EX_RESERVED = 0x0004;
const int64_t FLAG_SSB_INVALID_DVD_GEOMETRY = 0x0008;
const int64_t FLAG_SSB_INVALID_DVD_DMI = 0x0010;
const int64_t FLAG_SSB_DVD_KEYVAULT_PAIR_MISMATCH_D = 0x0020;  // DEPRECATED
const int64_t FLAG_SSB_CRL_DATA_INVALID_D = 0x0040;            // DEPRECATED
const int64_t FLAG_SSB_CRL_CERTIFICATE_REVOKED = 0x0080;
const int64_t FLAG_SSB_UNAUTHORIZED_INSTALL = 0x0100;
const int64_t FLAG_SSB_KEYVAULT_POLICY_VIOLATION = 0x0200;
const int64_t FLAG_SSB_CONSOLE_BANNED_D = 0x0400;              // DEPRECATED
const int64_t FLAG_SSB_ODD_VIOLATION = 0x0800;
const int64_t FLAG_SSB_CIV_HASH_FAILURE = 0x1000;
const int64_t FLAG_SSB_AUTH_EX_NO_TABLE = 0x2000;
const int64_t FLAG_SSB_CRL_DATA_INVALID = 0x4000;
const int64_t FLAG_SSB_ODD_ENFORCE_THROUGHPUT_LOW = 0x8000;
const int64_t FLAG_SSB_ODD_ENFORCE_THROUGHPUT_HIGH = 0x10000;

namespace {

// ---- HMAC-SHA1 (Python's hmac.new(key, msg, sha1).digest()) --------------------------------------------------
py::bytes hmac_sha1(const py::bytes& key_in, const py::bytes& msg) {
    const int64_t block = 64;
    py::bytes key = key_in;
    if (py::len(key) > block) {
        key = py::hashlib::sha1(key).digest();
    }
    key = key + py::bytes::zeros(block - py::len(key));
    std::string inner_pad, outer_pad;
    for (int64_t i = 0; i < block; ++i) {
        inner_pad.push_back(static_cast<char>(key[i] ^ 0x36));
        outer_pad.push_back(static_cast<char>(key[i] ^ 0x5C));
    }
    py::bytes inner = py::hashlib::sha1(py::bytes(inner_pad) + msg).digest();
    return py::hashlib::sha1(py::bytes(outer_pad) + inner).digest();
}

// ---- DES, single block, ECB (pycryptodome's DES.new(key, DES.MODE_ECB).decrypt) -----------------------------------
// The tables are FIPS 46-3's; bit 1 is the most significant bit.
const uint8_t IP[64] = {58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22,
                        14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35,
                        27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};
const uint8_t FP[64] = {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22,
                        62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11,
                        51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25};
const uint8_t E[48] = {32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
                       16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};
const uint8_t P[32] = {16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
                       2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25};
const uint8_t PC1[56] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
                         63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};
const uint8_t PC2[48] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
                         41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};
const uint8_t SHIFTS[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
const uint8_t S[8][64] = {
    {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
     4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
    {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
     0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
    {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
     13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
    {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
     10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
    {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
     4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
    {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
     9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
    {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
     1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
    {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
     7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};

// the n_out bits `table` names (1-based from the top of an n_in-bit value), as an n_out-bit value
uint64_t permute(uint64_t value, const uint8_t* table, int n_out, int n_in) {
    uint64_t out = 0;
    for (int i = 0; i < n_out; ++i) {
        out = (out << 1) | ((value >> (n_in - table[i])) & 1);
    }
    return out;
}

uint64_t be64(const py::bytes& b) {
    uint64_t v = 0;
    for (int64_t i = 0; i < 8; ++i) v = (v << 8) | static_cast<uint64_t>(b[i]);
    return v;
}

// DES decryption of one 8-byte block under an 8-byte key (the parity bits are ignored).
py::bytes des_decrypt_block(const py::bytes& key, const py::bytes& block) {
    uint64_t k = permute(be64(key), PC1, 56, 64);
    uint32_t c = static_cast<uint32_t>(k >> 28) & 0xFFFFFFF, d = static_cast<uint32_t>(k) & 0xFFFFFFF;
    std::array<uint64_t, 16> subkeys{};
    for (int r = 0; r < 16; ++r) {
        for (int s = 0; s < SHIFTS[r]; ++s) {
            c = ((c << 1) | (c >> 27)) & 0xFFFFFFF;
            d = ((d << 1) | (d >> 27)) & 0xFFFFFFF;
        }
        subkeys[r] = permute((static_cast<uint64_t>(c) << 28) | d, PC2, 48, 56);
    }
    uint64_t state = permute(be64(block), IP, 64, 64);
    uint32_t left = static_cast<uint32_t>(state >> 32), right = static_cast<uint32_t>(state);
    for (int r = 15; r >= 0; --r) {   // decryption runs the subkeys backwards
        uint64_t x = permute(right, E, 48, 32) ^ subkeys[r];
        uint64_t sboxed = 0;
        for (int i = 0; i < 8; ++i) {
            unsigned six = static_cast<unsigned>((x >> (42 - 6 * i)) & 0x3F);
            unsigned row = ((six >> 4) & 2) | (six & 1), col = (six >> 1) & 0xF;
            sboxed = (sboxed << 4) | S[i][row * 16 + col];
        }
        uint32_t f = static_cast<uint32_t>(permute(sboxed, P, 32, 32));
        uint32_t next = left ^ f;
        left = right;
        right = next;
    }
    uint64_t out = permute((static_cast<uint64_t>(right) << 32) | left, FP, 64, 64);
    std::string s;
    for (int i = 7; i >= 0; --i) s.push_back(static_cast<char>((out >> (8 * i)) & 0xFF));
    return py::bytes(s);
}

// bytes.fromhex(text) as Python reads it: ASCII blanks between byte pairs only (the runtime's reader
// would also skip Unicode blanks and the 0x1C-0x1F separators)
py::bytes from_hex(const py::str& text) {
    for (char32_t c : text.u32()) {
        bool hex = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
        bool blank = c == ' ' || (c >= 9 && c <= 13);
        if (!hex && !blank) {
            throw py::ValueError("non-hexadecimal number found in fromhex() arg");
        }
    }
    return py::bytes::fromhex(text);
}

}  // namespace

py::bytes decrypt_xval(const py::str& console_serial, const py::str& console_xval) {
    py::bytes serial = console_serial.encode("ascii");
    py::bytes xval = from_hex(console_xval.replace("-", ""));
    if (!(py::len(serial) == 0xC)) {
        throw py::AssertionError("Invalid console serial length");
    }
    py::bytes des_key = hmac_sha1(serial + py::bytes("\0", 1), py::bytes("XBOX360SSB")).slice_to(8);
    if (!(py::len(xval) == 8)) {
        throw py::AssertionError("Invalid decrypted XVal size");
    }
    return des_decrypt_block(des_key, xval);
}

void display_results(const py::bytes& xval) {
    // extract our integers formt the buffer
    if (py::len(xval) != 8) {
        throw py::StructError("unpack requires a buffer of 8 bytes");
    }
    auto [xval_h, xval_l] = py::unpack_from<int64_t, int64_t>(">LL", xval);
    // nothing is flagged in secdata.bin, all is good from this standpoint
    if (xval_h == 0 && xval_l == 0) {
        py::print(py::str("Secdata is Clean"));
    // secdata was prob tampered with
    } else if (xval_h == 0xFFFFFFFF && xval_l == 0xFFFFFFFF) {
        py::print(py::str("Secdata is invalid"));
    // most likely the serial or xval is incorrect
    } else if (xval_h != 0 && xval_l != 0) {
        py::print(py::str("Secdata decryption error"));
    // the high dword = 0 and low dword not 0
    } else {
        if (xval_l & FLAG_SSB_AUTH_EX_FAILURE_D) {  // AP25 related
            py::print(py::str("AuthEx Challenge Failure (Deprecated)"));
        }
        if (xval_l & FLAG_SSB_AUTH_EX_NO_TABLE_D) {  // AP25 related
            py::print(py::str("AuthEx Table missing (Deprecated)"));
        }
        if (xval_l & FLAG_SSB_AUTH_EX_RESERVED) {  // AP25 related
            py::print(py::str("AuthEx Reserved Flag"));
        }
        if (xval_l & FLAG_SSB_INVALID_DVD_GEOMETRY) {
            py::print(py::str("Invalid DVD Geometry"));
        }
        if (xval_l & FLAG_SSB_INVALID_DVD_DMI) {
            py::print(py::str("Invalid DVD DMI"));
        }
        if (xval_l & FLAG_SSB_DVD_KEYVAULT_PAIR_MISMATCH_D) {
            py::print(py::str("DVD Keyvault Pair Mismatch (Deprecated)"));
        }
        if (xval_l & FLAG_SSB_CRL_DATA_INVALID_D) {
            py::print(py::str("Invalid CRL Data (Deprecated)"));
        }
        if (xval_l & FLAG_SSB_CRL_CERTIFICATE_REVOKED) {
            py::print(py::str("CRL Certificate Revoked"));
        }
        if (xval_l & FLAG_SSB_UNAUTHORIZED_INSTALL) {
            py::print(py::str("Unauthorized Install"));
        }
        if (xval_l & FLAG_SSB_KEYVAULT_POLICY_VIOLATION) {
            py::print(py::str("Keyvault Policy Violation"));
        }
        if (xval_l & FLAG_SSB_CONSOLE_BANNED_D) {
            py::print(py::str("Console Banned (Deprecated)"));
        }
        if (xval_l & FLAG_SSB_ODD_VIOLATION) {
            py::print(py::str("ODD Violation"));
        }
        if (xval_l & FLAG_SSB_CIV_HASH_FAILURE) {  // AP25 related
            py::print(py::str("Content Intergrity Verification Hash Failure"));
        }
        if (xval_l & FLAG_SSB_AUTH_EX_NO_TABLE) {  // AP25 related
            py::print(py::str("AuthEx Table missing"));
        }
        if (xval_l & FLAG_SSB_CRL_DATA_INVALID) {
            py::print(py::str("Invalid CRL Data"));
        }
        if (xval_l & FLAG_SSB_ODD_ENFORCE_THROUGHPUT_LOW) {
            py::print(py::str("Enforce ODD Throughput Low"));
        }
        if (xval_l & FLAG_SSB_ODD_ENFORCE_THROUGHPUT_HIGH) {
            py::print(py::str("Enforce ODD Throughput High"));
        }
        if (xval_l & 0xFFFE0000) {  // mask for bits we dont have a description for,
            py::print(py::str("Unknown Violation(s)"));
        }
    }
}

// ---- the command line (argparse, for this one parser) -------------------------------------------------------------
namespace {

const int64_t TEXT_WIDTH = 78;   // an 80-column terminal, less 2
const char* const DESCRIPTION = "A script to decrypt the \"X:\" Value from the Xbox 360 dashboard";

void write_out(FILE* f, const py::str& text) {
    std::string s = text.utf8();
    std::fwrite(s.data(), 1, s.size(), f);
}

// The "usage: prog [-h] serial xval" block (wrapped as argparse wraps it), with its final newline.
py::str format_usage(const py::str& prog) {
    py::str prefix = "usage: ";
    py::list<py::str> opt_parts = {py::str("[-h]")};
    py::list<py::str> pos_parts = {py::str("serial"), py::str("xval")};
    py::str action_usage = "[-h] serial xval";
    py::str usage = prog + " " + action_usage;
    if (py::len(prefix) + py::len(usage) > TEXT_WIDTH) {
        auto get_lines = [&](const py::list<py::str>& parts, const py::str& indent, const std::optional<py::str>& line_prefix) {
            py::list<py::str> lines;
            py::list<py::str> line;
            int64_t indent_length = py::len(indent);
            int64_t line_len = line_prefix ? py::len(*line_prefix) - 1 : indent_length - 1;
            for (const py::str& part : parts) {
                if (line_len + 1 + py::len(part) > TEXT_WIDTH && !line.empty()) {
                    lines.append(indent + py::join(" ", line));
                    line = py::list<py::str>();
                    line_len = indent_length - 1;
                }
                line.append(part);
                line_len += py::len(part) + 1;
            }
            if (!line.empty()) {
                lines.append(indent + py::join(" ", line));
            }
            if (line_prefix) {
                lines[0] = lines[0].slice_from(indent_length);
            }
            return lines;
        };
        py::list<py::str> lines;
        if (static_cast<double>(py::len(prefix) + py::len(prog)) <= 0.75 * static_cast<double>(TEXT_WIDTH)) {
            py::str indent = py::repeat(py::str(" "), py::len(prefix) + py::len(prog) + 1);
            py::list<py::str> first = {prog};
            first.extend(opt_parts);
            lines = get_lines(first, indent, prefix);
            lines.extend(get_lines(pos_parts, indent, std::nullopt));
        } else {
            py::str indent = py::repeat(py::str(" "), py::len(prefix));
            py::list<py::str> parts = opt_parts;
            parts.extend(pos_parts);
            lines = get_lines(parts, indent, std::nullopt);
            if (py::len(lines) > 1) {
                lines = py::list<py::str>();
                lines.extend(get_lines(opt_parts, indent, std::nullopt));
                lines.extend(get_lines(pos_parts, indent, std::nullopt));
            }
            py::list<py::str> all = {prog};
            all.extend(lines);
            lines = all;
        }
        usage = py::join("\n", lines);
    }
    return prefix + usage + "\n";
}

[[noreturn]] void print_help_and_exit(const py::str& prog) {
    write_out(stdout, format_usage(prog) + "\n" + DESCRIPTION + "\n\n" +
                          "positional arguments:\n"
                          "  serial      The console's serial number\n"
                          "  xval        The console's X: value\n"
                          "\n"
                          "options:\n"
                          "  -h, --help  show this help message and exit\n");
    throw SystemExit(0);
}

[[noreturn]] void error_and_exit(const py::str& prog, const py::str& message) {
    write_out(stderr, format_usage(prog));
    write_out(stderr, prog + ": error: " + message + "\n");
    throw SystemExit(2);
}

// What _parse_optional makes of one argument: nothing (a positional), or an option: the help action
// (known) with the option string it was written as, and the "=" and explicit argument after it; or an
// option this parser has none of.
struct Option {
    bool known = false;
    py::str option_string;
    std::optional<py::str> sep;
    std::optional<py::str> explicit_arg;
};

bool is_option_string(const py::str& s) { return s == "-h" || s == "--help"; }

// (a list: an argument that matches several options is ambiguous when its turn comes)
std::optional<py::list<Option>> parse_optional(const py::str& arg) {
    if (arg.empty()) {
        return std::nullopt;
    }
    if (arg.at_(0) != '-') {
        return std::nullopt;
    }
    if (is_option_string(arg)) {
        return py::list<Option>{Option{true, arg, std::nullopt, std::nullopt}};
    }
    // if it's just a single character, it was meant to be positional
    if (py::len(arg) == 1) {
        return std::nullopt;
    }
    // if the option string before the "=" is present in the parser, return the action
    auto [head, sep, tail] = arg.partition("=");
    if (!sep.empty() && is_option_string(head)) {
        return py::list<Option>{Option{true, head, sep, tail}};
    }
    // all the prefixes of the option string against the parser's options (_get_option_tuples)
    py::list<Option> found;
    std::optional<py::str> opt_sep = sep.empty() ? std::nullopt : std::optional<py::str>(sep);
    std::optional<py::str> opt_tail = sep.empty() ? std::nullopt : std::optional<py::str>(tail);
    if (arg.at_(1) == '-') {
        for (const char* opt : {"-h", "--help"}) {
            if (py::str(opt).startswith(head)) {
                found.append(Option{true, py::str(opt), opt_sep, opt_tail});
            }
        }
    } else {
        py::str short_option_prefix = arg.slice_to(2);
        py::str short_explicit_arg = arg.slice_from(2);
        for (const char* opt : {"-h", "--help"}) {
            if (py::str(opt) == short_option_prefix) {
                found.append(Option{true, py::str(opt), py::str(""), short_explicit_arg});
            } else if (py::str(opt).startswith(head)) {
                found.append(Option{true, py::str(opt), opt_sep, opt_tail});
            }
        }
    }
    if (!found.empty()) {
        return found;
    }
    // looks like a negative number: meant to be positional
    static const py::re::Pattern negative_number = py::re::compile(R"(-\.?\d)");
    if (negative_number.match(arg)) {
        return std::nullopt;
    }
    // contains a space: meant to be positional
    if (arg.contains(" ")) {
        return std::nullopt;
    }
    return py::list<Option>{Option{false, arg, std::nullopt, std::nullopt}};
}

// consume_optional for the help option: -h, --help, their abbreviations and single-dash clusters.
// Always ends the program (help or an error); an unknown tail of a cluster becomes an extra.
[[noreturn]] void take_option(const py::str& prog, Option opt, py::list<py::str>& extras) {
    while (true) {
        if (opt.explicit_arg) {
            const py::str& explicit_arg = *opt.explicit_arg;
            // a single-dash flag that takes no argument: more single-dash flags may follow it
            if (opt.option_string.at_(1) != '-' && !explicit_arg.empty()) {
                if ((opt.sep && !opt.sep->empty()) || explicit_arg.at_(0) == '-') {
                    error_and_exit(prog, "argument -h/--help: ignored explicit argument " + py::repr(explicit_arg));
                }
                py::str next = py::str("-") + explicit_arg.slice_to(1);
                if (is_option_string(next)) {
                    opt.option_string = next;
                    py::str rest = explicit_arg.slice_from(1);
                    if (rest.empty()) {
                        opt.sep = std::nullopt;
                        opt.explicit_arg = std::nullopt;
                    } else if (rest.at_(0) == '=') {
                        opt.sep = py::str("=");
                        opt.explicit_arg = rest.slice_from(1);
                    } else {
                        opt.sep = py::str("");
                        opt.explicit_arg = rest;
                    }
                    continue;
                }
                extras.append(py::str("-") + explicit_arg);
                break;
            }
            error_and_exit(prog, "argument -h/--help: ignored explicit argument " + py::repr(explicit_arg));
        }
        break;
    }
    print_help_and_exit(prog);
}

}  // namespace

void main(const py::list<py::str>& argv) {
    py::str prog = py::os::path::basename(argv[0]);
    py::list<py::str> arg_strings = argv.slice_from(1);

    // the pattern of the command line: O an option, A an argument, - the first "--"
    py::str pattern;
    py::dict<int64_t, py::list<Option>> option_string_indices;
    bool after_dashes = false;
    for (int64_t i = 0; i < py::len(arg_strings); ++i) {
        const py::str& a = arg_strings[i];
        if (after_dashes) {
            pattern += "A";
        } else if (a == "--") {
            pattern += "-";
            after_dashes = true;
        } else {
            std::optional<py::list<Option>> opt = parse_optional(a);
            if (!opt) {
                pattern += "A";
            } else {
                option_string_indices.set(i, *opt);
                pattern += "O";
            }
        }
    }

    py::list<py::str> extras;
    py::list<py::str> names = {py::str("serial"), py::str("xval")};
    py::list<py::str> values;   // those taken by the positionals so far
    int64_t max_option_string_index = -1;
    for (int64_t k : option_string_indices) {
        max_option_string_index = py::max(max_option_string_index, k);
    }

    // match as many positionals as possible (_match_arguments_partial), take their arguments
    auto consume_positionals = [&](int64_t start_index) {
        int64_t left = py::len(names) - py::len(values);
        py::str selected = pattern.slice_from(start_index);
        py::list<int64_t> counts;
        for (int64_t i = left; i > 0; --i) {
            py::str regex;
            for (int64_t j = 0; j < i; ++j) regex += "(-*A-*)";
            std::optional<py::re::Match> m = py::re::match(regex, selected);
            if (m) {
                for (int64_t g = 1; g <= i; ++g) counts.append(py::len(m->group(g)));
                break;
            }
        }
        for (int64_t count : counts) {
            py::list<py::str> args = arg_strings.slice(start_index, start_index + count);
            // strip out the first '--'
            if (pattern.find("-", start_index, start_index + count) >= 0) {
                args.remove(py::str("--"));
            }
            start_index += count;
            values.append(args[0]);
        }
        return start_index;
    };

    int64_t start_index = 0;
    while (start_index <= max_option_string_index) {
        // consume any positionals preceding the next option
        int64_t next_option_string_index = start_index;
        while (next_option_string_index <= max_option_string_index) {
            if (option_string_indices.contains(next_option_string_index)) {
                break;
            }
            next_option_string_index += 1;
        }
        if (start_index != next_option_string_index) {
            int64_t positionals_end_index = consume_positionals(start_index);
            // only try to parse the next optional if we didn't consume the option string during the positionals parsing
            if (positionals_end_index > start_index) {
                start_index = positionals_end_index;
                continue;
            } else {
                start_index = positionals_end_index;
            }
        }
        // if we consumed all the positionals we could and we're not at the index of an option string, there were extra arguments
        if (!option_string_indices.contains(start_index)) {
            extras.extend(arg_strings.slice(start_index, next_option_string_index));
            start_index = next_option_string_index;
        }
        // consume the next optional and any arguments for it
        py::list<Option> option_tuples = option_string_indices.at(start_index);
        // if multiple actions match, the option string was ambiguous
        if (py::len(option_tuples) > 1) {
            py::list<py::str> matches;
            for (const Option& o : option_tuples) matches.append(o.option_string);
            error_and_exit(prog, "ambiguous option: " + arg_strings[start_index] + " could match " + py::join(", ", matches));
        }
        Option opt = option_tuples[0];
        if (!opt.known) {
            extras.append(arg_strings[start_index]);
            start_index += 1;
            continue;
        }
        take_option(prog, opt, extras);
    }
    // consume any positionals following the last optional
    int64_t stop_index = consume_positionals(start_index);
    extras.extend(arg_strings.slice_from(stop_index));

    if (py::len(values) < py::len(names)) {
        py::list<py::str> missing = names.slice_from(py::len(values));
        error_and_exit(prog, "the following arguments are required: " + py::join(", ", missing));
    }
    if (!extras.empty()) {
        error_and_exit(prog, "unrecognized arguments: " + py::join(" ", extras));
    }
    display_results(decrypt_xval(values[0], values[1]));
}

}  // namespace codebreaker::formats::xbox::crypto::xval
