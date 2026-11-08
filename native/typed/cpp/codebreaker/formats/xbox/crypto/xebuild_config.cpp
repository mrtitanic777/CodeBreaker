// codebreaker/formats/xbox/crypto/xebuild_config.cpp -- C++ port of codebreaker.formats.xbox.crypto.xebuild_config.

#include "codebreaker/formats/xbox/crypto/xebuild_config.hpp"

#include <array>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace codebreaker::formats::xbox::crypto::xebuild_config {

namespace {

std::ostream* out_stream_ = &std::cout;
std::ostream* err_stream_ = &std::cerr;

// binascii.crc32(data, value): the zlib CRC-32, continued from value.
int64_t crc32(const std::string& data, int64_t value) {
    static const std::array<uint32_t, 256> table = [] {
        std::array<uint32_t, 256> t{};
        for (uint32_t n = 0; n < 256; n++) {
            uint32_t c = n;
            for (int k = 0; k < 8; k++) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
            t[n] = c;
        }
        return t;
    }();
    uint32_t crc = static_cast<uint32_t>(value & 0xFFFFFFFF) ^ 0xFFFFFFFFu;
    for (unsigned char byte : data) crc = table[(crc ^ byte) & 0xFF] ^ (crc >> 8);
    return static_cast<int64_t>(crc ^ 0xFFFFFFFFu);
}

int64_t checksum_of(const std::string& data) {
    int64_t cksm = 0;
    // (BytesIO(data) read BUFF_SIZE at a time until it returns b"")
    for (size_t pos = 0; pos < data.size(); pos += static_cast<size_t>(BUFF_SIZE)) {
        cksm = crc32(data.substr(pos, static_cast<size_t>(BUFF_SIZE)), cksm);
    }
    return cksm;
}

const char* const kUsage = " [-h] [-d DIR]";
const char* const kDescription = "A script to generate build configs for XeBuild";

// argparse's reading of a command-line word as "-5" / "-.5": a number, not an option.
bool looks_negative_number(const std::string& s) {
    if (s.size() < 2 || s[0] != '-') return false;
    size_t i = 1;
    size_t digits = 0;
    while (i < s.size() && s[i] >= '0' && s[i] <= '9') { i++; digits++; }
    if (i == s.size()) return digits > 0;
    if (s[i] != '.') return false;
    i++;
    size_t frac = 0;
    while (i < s.size() && s[i] >= '0' && s[i] <= '9') { i++; frac++; }
    return i == s.size() && frac > 0;
}

// does argparse classify this word as an option (an 'O') rather than a value (an 'A')?
bool is_option_word(const std::string& s) {
    if (s.empty() || s[0] != '-') return false;
    if (s == "-") return false;
    if (s == "-h" || s == "--help" || s == "-d" || s == "--dir") return true;
    if (looks_negative_number(s)) return false;
    if (s.find(' ') != std::string::npos) {
        // (a word with a space is a value unless it names an option, e.g. "--dir=a b")
        if (s.rfind("--", 0) == 0) {
            std::string name = s.substr(0, s.find('='));
            return s.find('=') != std::string::npos && name.size() > 2 &&
                   (std::string("--help").rfind(name, 0) == 0 || std::string("--dir").rfind(name, 0) == 0);
        }
        return s.rfind("-d", 0) == 0 || s.rfind("-h", 0) == 0;
    }
    return true;
}

[[noreturn]] void fail(const std::string& prog, const std::string& message) {
    *err_stream_ << "usage: " << prog << kUsage << "\n" << prog << ": error: " << message << "\n";
    throw SystemExit(2);
}

[[noreturn]] void help(const std::string& prog) {
    *out_stream_ << "usage: " << prog << kUsage << "\n\n"
                 << kDescription << "\n\n"
                 << "options:\n"
                 << "  -h, --help     show this help message and exit\n"
                 << "  -d, --dir DIR  The directory to search for XeBuild files\n";
    throw SystemExit(0);
}

}  // namespace

int64_t checksum(const py::bytes& data) { return checksum_of(data.raw()); }

int64_t checksum(const py::bytearray& data) { return checksum_of(data.raw()); }

void set_streams(std::ostream& out, std::ostream& err) {
    out_stream_ = &out;
    err_stream_ = &err;
}

void main(const py::list<py::str>& argv) {
    // parser = ArgumentParser(description=...); parser.add_argument("-d", "--dir", type=str, help=...)
    std::string prog = argv.size() > 0 ? py::os::path::basename(argv[0]).utf8() : std::string("");
    std::vector<std::string> args;
    for (int64_t i = 1; i < argv.size(); i++) args.push_back(argv[i].utf8());

    std::optional<py::str> dir;   // args.dir
    std::vector<std::string> extras;
    for (size_t i = 0; i < args.size(); i++) {
        const std::string& a = args[i];
        if (a == "--") {   // everything after is a positional, and there are none declared
            for (size_t j = i; j < args.size(); j++) extras.push_back(args[j]);
            break;
        }
        std::optional<std::string> explicit_arg;
        bool is_help = false, is_dir = false;
        if (a.rfind("--", 0) == 0) {
            std::string name = a.substr(0, a.find('='));
            if (a.find('=') != std::string::npos) explicit_arg = a.substr(a.find('=') + 1);
            if (name.size() > 2 && std::string("--help").rfind(name, 0) == 0) is_help = true;
            else if (name.size() > 2 && std::string("--dir").rfind(name, 0) == 0) is_dir = true;
        } else if (a.size() >= 2 && a[0] == '-' && !looks_negative_number(a)) {
            if (a[1] == 'h') {
                is_help = true;
                if (a.size() > 2) explicit_arg = a.substr(2);
            } else if (a[1] == 'd') {
                is_dir = true;
                if (a.size() > 2) explicit_arg = a.substr(2);
            }
        }
        if (is_help && explicit_arg) {
            fail(prog, "argument -h/--help: ignored explicit argument '" + *explicit_arg + "'");
        }
        if (is_help) help(prog);
        if (is_dir) {
            if (explicit_arg) {
                dir = py::str(explicit_arg->c_str());
            } else if (i + 1 < args.size() && !is_option_word(args[i + 1])) {
                dir = py::str(args[i + 1].c_str());
                i++;
            } else {
                fail(prog, "argument -d/--dir: expected one argument");
            }
            continue;
        }
        extras.push_back(a);   // an unknown option, or a stray word: no positionals are declared
    }
    if (!extras.empty()) {
        std::string joined;
        for (size_t i = 0; i < extras.size(); i++) joined += (i ? " " : "") + extras[i];
        fail(prog, "unrecognized arguments: " + joined);
    }

    // root_p = Path("./"); with StringIO() as sio: for single in root_p.iterdir(): if single.is_file(): pass
    std::error_code ec;
    std::filesystem::directory_iterator it(std::filesystem::path("./"), ec);
    if (ec) throw py::FileNotFoundError("[WinError 3] The system cannot find the path specified: '.'");
    for (const std::filesystem::directory_entry& single : it) {
        std::error_code fec;
        if (single.is_regular_file(fec)) {
            // pass
        }
    }
}

}  // namespace codebreaker::formats::xbox::crypto::xebuild_config
