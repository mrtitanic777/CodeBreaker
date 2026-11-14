// Tests of cb/formats/xbox/crypto/xcp_dumper.hpp on two small packages made for the tests (testdata/make_xcp_samples.py: a cabinet with the header
// reserve and the folder reserve an XCP has, encrypted piece by piece with a record each, and the same cabinet in the clear), against the recorded output
// (testdata/xcp_goldens.txt: `codebreaker.exe crypto xcp_dumper --ignore --no-backup [-k KEY]`, which extracts with Windows'
// expand.exe where the native tool reads the cabinet itself).
//
// NOT run on a real XCP (none is on this machine): the samples follow the format's documented offsets.
//
// Behaviour pinned here: the prompt before the file is consumed reads a line from standard input and an end of input stops the tool;
// names are renumbered for 1000 files at most; extraction is native (cab.hpp) rather than a program.
#include "cb/formats/xbox/crypto/xcp_dumper.hpp"

#include <string>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/command_test_kit.hpp"
#include "cb/formats/xbox/crypto/tools.hpp"

namespace xcp_dumper_test {

using namespace cb;
namespace stdfs = std::filesystem;
namespace xcrypto = cb::formats::xbox::crypto;

constexpr const char* kKeyHex = "00112233445566778899AABBCCDDEEFF";

Bytes sample(const std::string& name) { return fs::read_file(testing::data_path("formats/xbox/crypto/testdata/sample_" + name + ".xcp")); }

std::string golden(const std::string& name) {
    const std::string log = fs::read_text(testing::data_path("formats/xbox/crypto/testdata/xcp_goldens.txt"));
    std::string out;
    bool inside = false;
    for (const std::string_view line : text::lines(log)) {
        if (line.starts_with("== ")) inside = line.substr(3) == name;
        else if (inside) out += std::string(line) + "\n";
    }
    CB_CHECK(!out.empty());
    return out;
}

std::string forward(std::string s) { return text::replace_all(s, "\\", "/"); }

// What the tool prints with `args`, `exit=N`, and a line for each file left in the folder.
std::string run(const fs::ScopedTempDir& dir, std::vector<std::string> args) {
    const std::string w = forward(fs::to_text(dir.path()));
    args.insert(args.begin(), "xcp_dumper");
    std::string out = text::replace_all(forward(testing::run_command(xcrypto::command(xcrypto::Environment{}), args)), w, "<w>");
    std::vector<std::string> names;
    for (const auto& entry : stdfs::directory_iterator(dir.path())) names.push_back(fs::to_text(entry.path().filename()));
    std::sort(names.begin(), names.end());
    for (const std::string& name : names) {
        const Bytes data = fs::read_file(dir.path() / name);
        out += std::format("file {} {} {}\n", name, data.size(), crypto::to_hex(crypto::sha1(data)));
    }
    return out;
}

CB_TEST("formats.xbox.crypto.xcp_dumper.decrypts_and_converts_a_package", [] {
    const fs::ScopedTempDir dir{"cb-xcp"};
    fs::write_file(dir.path() / "sample.xcp", sample("encrypted"));
    const std::string w = forward(fs::to_text(dir.path()));
    CB_CHECK_EQ(run(dir, {"--ignore", "--no-backup", "-k", kKeyHex, w + "/sample.xcp"}), golden("encrypted"));
});

CB_TEST("formats.xbox.crypto.xcp_dumper.converts_a_package_that_is_not_encrypted", [] {
    const fs::ScopedTempDir dir{"cb-xcp"};
    fs::write_file(dir.path() / "sample.xcp", sample("plain"));
    const std::string w = forward(fs::to_text(dir.path()));
    CB_CHECK_EQ(run(dir, {"--ignore", "--no-backup", w + "/sample.xcp"}), golden("plain"));
});

CB_TEST("formats.xbox.crypto.xcp_dumper.the_input_is_consumed_and_a_backup_is_kept_unless_told_not_to", [] {
    const fs::ScopedTempDir dir{"cb-xcp"};
    const Bytes original = sample("encrypted");
    fs::write_file(dir.path() / "pkg.xcp", original);
    const std::string w = forward(fs::to_text(dir.path()));
    const std::string out = run(dir, {"--ignore", "-k", kKeyHex, w + "/pkg.xcp"});
    CB_CHECK(!stdfs::exists(dir.path() / "pkg.xcp"));                           // consumed
    CB_CHECK(equal_bytes(fs::read_file(dir.path() / "pkg.bak"), original));     // the backup is the file as it was
    CB_CHECK(stdfs::is_regular_file(dir.path() / "PKG"));
    const Bytes live = fs::read_file(dir.path() / "PKG");
    CB_CHECK_EQ(live.size(), std::size_t{25300});
    CB_CHECK_EQ(std::string(as_text(ByteView(live).first(4))), "LIVE");
    CB_CHECK(out.find("Backing up the input file...") != std::string::npos);
});

CB_TEST("formats.xbox.crypto.xcp_dumper.the_files_are_joined_in_order", [] {
    // the contents of the three files, one after the other, with the first four bytes the marker
    const fs::ScopedTempDir dir{"cb-xcp"};
    fs::write_file(dir.path() / "sample.xcp", sample("plain"));
    const std::string w = forward(fs::to_text(dir.path()));
    (void)run(dir, {"--ignore", "--no-backup", w + "/sample.xcp"});
    const Bytes live = fs::read_file(dir.path() / "SAMPLE");
    const auto pattern = [](std::size_t n, int seed) {
        Bytes out(n);
        for (std::size_t i = 0; i < n; ++i) out[i] = static_cast<std::uint8_t>((i * 131 + static_cast<std::size_t>(seed) * 17 + (i >> 8) * 7) & 0xFF);
        return out;
    };
    Bytes expected = pattern(5000, 70);
    append(expected, pattern(300, 71));
    append(expected, pattern(20000, 72));
    std::copy_n("LIVE", 4, expected.begin());
    CB_CHECK(live == expected);
});

CB_TEST("formats.xbox.crypto.xcp_dumper.refuses_a_missing_key_a_wrong_key_and_a_missing_file", [] {
    const fs::ScopedTempDir dir{"cb-xcp"};
    fs::write_file(dir.path() / "sample.xcp", sample("encrypted"));
    const std::string w = forward(fs::to_text(dir.path()));
    const cli::Command crypto = xcrypto::command(xcrypto::Environment{});
    const auto throws = [&](auto type_tag, std::vector<std::string> args) {
        using E = decltype(type_tag);
        args.insert(args.begin(), "xcp_dumper");
        try {
            (void)testing::run_command(crypto, args);
        } catch (const E&) {
            return true;
        } catch (...) {
            return false;
        }
        return false;
    };
    CB_CHECK(throws(UsageError{"x"}, {"--ignore", "--no-backup", w + "/sample.xcp"}));
    CB_CHECK(throws(FormatError("x", 0, "x"), {"--ignore", "--no-backup", "-k", "FFEEDDCCBBAA99887766554433221100", w + "/sample.xcp"}));
    CB_CHECK(throws(NotFoundError("x", 0, "x"), {"--ignore", "--no-backup", "-k", kKeyHex, w + "/missing.xcp"}));
    CB_CHECK(throws(UsageError{"x"}, {"--ignore", "--no-backup", "-k", "123", w + "/sample.xcp"}));
    // a wrong key leaves the package as it was
    CB_CHECK(fs::read_file(dir.path() / "sample.xcp") == sample("encrypted"));
});

}  // namespace xcp_dumper_test
