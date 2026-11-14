// Tests of cb/formats/xbox/crypto/harness.hpp over synthetic test data (byte i = i*29 + (i>>7)*3 + 7, 0x40010 bytes) and the xecrypt test
// RSA key. The console's own test data is not in the repository, so the known answers do not match and every check of the symmetric
// ciphers and the digests reports FAILED; the report below is the expected output, line for line, and the values
// each check computed are the ones the cipher implementations give for the same inputs.
//
// Behaviour pinned here: the test data and the key come with the call rather than from `Data/` in the working folder, and the CPU key
// of the key store is an input; a test-data file shorter than the checks read is UsageError.
#include "cb/formats/xbox/crypto/harness.hpp"

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"

namespace harness_test {

using namespace cb;
using namespace cb::formats::xbox::crypto;

static Bytes test_data() {
    Bytes b(0x40010);
    for (std::size_t i = 0; i < b.size(); ++i) b[i] = static_cast<std::uint8_t>(i * 29 + (i >> 7) * 3 + 7);
    return b;
}

static harness::Inputs inputs() {
    harness::Inputs in;
    in.test_data = test_data();
    in.rsa_key = fs::read_file(testing::data_path("formats/testdata/xecrypt/rsa2048.prv"));
    return in;
}

static const harness::Check& find(const harness::Result& r, std::string_view label) {
    for (const harness::Check& c : r.checks) {
        if (c.label == label) return c;
    }
    throw InternalError("no check named " + std::string(label));
}

static const char kReport[] = R"REPORT(starting RSA tests...
    testing RSA signature creation and verification with static key....
        RSA signature created OK
        RSA signature encrypted OK
        RSA signature verified OK
    RSA test OK
    testing PKCS1 signature creation and verification with static key....
        PKCS1 signature created OK
        PKCS1 signature verified OK
    testing key generation...
        Key generation OK
RSA OK

starting RC4 tests...
    RC4 first round - 
FAILED
    RC4 hundred round - 
FAILED
RC4 FAILED!

starting AES tests...
    AES-ECB first round - 
FAILED
    AES-ECB hundred round - 
FAILED
    AES-CBC first round - 
FAILED
    AES-CBC hundred round - 
FAILED
AES FAILED!

starting DES tests...
    DES-ECB first round - 
FAILED
    DES-ECB hundred round - 
FAILED
    DES-CBC - 
FAILED
    DES3-ECB first round - 
FAILED
    DES3-ECB hundred round - 
FAILED
    DES3-CBC - 
FAILED
DES FAILED!

starting MD5 tests...
    MD5 first round - 
FAILED
MD5 FAILED!

starting SHA tests...
    SHA first round - 
FAILED
    SHA second round (ROTSUMSHA) - 
FAILED
    SHA third round (HMAC) - 
FAILED
    SHA fourth round (HMAC) - 
FAILED
SHA FAILED!

starting Miscellaneous tests...
    testing RotSum
FAILED
    testing valid CPU key
skipped (no cpu_key.bin in the key store)
    testing invalid CPU key
OK
    testing CPU key generation
OK
Misc. FAILED!

ERROR! One or more tests failed!
)REPORT";

CB_TEST("formats.xbox.crypto.harness.the_report_matches_the_recorded_output", [] {
    const harness::Result r = harness::run(inputs());
    CB_CHECK(!r.passed);
    CB_CHECK_EQ(r.text(), std::string(kReport));
});

CB_TEST("formats.xbox.crypto.harness.each_check_computes_what_xecrypt_does", [] {
    const harness::Result r = harness::run(inputs());
    const struct {
        const char* label;
        const char* hex;
    } values[] = {
        {"RC4 first round", "7424f14a5c242f724858a35ea67c87a2"},
        {"RC4 hundred round", "6b8e7602703235904782a2d803b38ea6"},
        {"AES-ECB first round", "d46cc93048b84f95079c99c86f2070a7"},
        {"AES-ECB hundred round", "430cbb52815d8c2d805e4de6a46fcdaf"},
        {"AES-CBC first round", "73a828ce53801ae78830502e6c004dfd"},
        {"AES-CBC hundred round", "b46e348eece506d50cd2ddc399f9ce99"},
        {"DES-ECB first round", "56daf5ed08b71c1f"},
        {"DES-ECB hundred round", "72a91581aed0da44"},
        {"DES-CBC", "217ca31a4f1c121fa6d67be4ac45b79c64f5c874b5b1a941dec8ef2874f30ace739bf7d15da8ef934d1d8e536fcd38580785d619b657a71636d8d80d979f3765"},
        {"DES3-ECB first round", "5832c6d1b2ef5dd9"},
        {"DES3-ECB hundred round", "cba50b5305bd986a"},
        {"DES3-CBC", "280899095cca951acd5be6265017d139343f1fe6586e2334543f059b266256164bbe8655f67195665fb7594d4cb49e0c62c1727f8937117976a3c002187d0563"},
        {"MD5 first round", "c17d6dca065e736d41575487c7f96a9b"},
        {"SHA first round", "d8adcb180a6648dafcda499cc55e19c2cc05ee26"},
        {"SHA second round (ROTSUMSHA)", "e5170b386ac6d6c1667cfaadb1f73a7e51300ca4"},
        {"SHA third round (HMAC)", "cea96246bb4eee1b80ba339b8a52b311a9bfd6c7"},
        {"SHA fourth round (HMAC)", "fed6998b6dd1d3d0fbc1fb0decbcfb6b9ea81a24"},
        {"RotSum", "0000000000000002521c1088e40880d9fffffffffffffffe93786d22679c5186"}};
    for (const auto& v : values) {
        const harness::Check& c = find(r, v.label);
        CB_CHECK_EQ(crypto::to_hex(c.actual), v.hex);
        CB_CHECK(!c.skipped);
    }
    // the answers expected are the console's: not these
    CB_CHECK(!find(r, "RC4 first round").passed);
    CB_CHECK(find(r, "RSA signature").passed);
    CB_CHECK(find(r, "PKCS1 signature").passed);
    CB_CHECK(find(r, "Key generation").passed);
    CB_CHECK(find(r, "valid CPU key").skipped);
    CB_CHECK(find(r, "invalid CPU key").passed);
    CB_CHECK(find(r, "CPU key generation").passed);
});

CB_TEST("formats.xbox.crypto.harness.a_cpu_key_from_the_store_is_checked_and_a_bad_one_fails", [] {
    harness::Inputs in = inputs();
    in.valid_cpu_key = fs::read_file(testing::data_path("formats/testdata/shadowboot/cpu_key.bin"));
    const harness::Result good = harness::run(in);
    CB_CHECK(!find(good, "valid CPU key").skipped);
    CB_CHECK(find(good, "valid CPU key").passed);
    CB_CHECK(good.text().find("    testing valid CPU key\nOK\n") != std::string::npos);
    in.valid_cpu_key = Bytes(16, 0x12);
    const harness::Result bad = harness::run(in);
    CB_CHECK(!find(bad, "valid CPU key").passed);
    CB_CHECK(bad.text().find("    testing valid CPU key\nFAILED\n") != std::string::npos);
});

CB_TEST("formats.xbox.crypto.harness.bad_inputs_are_errors", [] {
    harness::Inputs in = inputs();
    in.test_data.resize(0x1000);
    CB_CHECK_THROWS_AS(UsageError, (void)harness::run(in));
    in = inputs();
    in.rsa_key = Bytes(10, 1);
    CB_CHECK_THROWS_AS(FormatError, (void)harness::run(in));
});

CB_TEST("formats.xbox.crypto.harness.the_command_prints_the_report_and_exits_1_when_any_check_fails", [] {
    fs::ScopedTempDir dir("cb-harness");
    fs::write_file(dir.path() / "test_data.bin", test_data());
    fs::write_file(dir.path() / "rsa_test_key.bin", inputs().rsa_key);
    const cli::Cli program("codebreaker", {harness::command(keystore::KeyStore(dir.path() / "Keys"))});
    testing::CapturedStd out;
    CB_CHECK_EQ(program.run({"crypto-harness", "--data", fs::to_text(dir.path())}), 1);
    CB_CHECK_EQ(out.out(), std::string(kReport));
});

}  // namespace harness_test
