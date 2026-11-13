// cb/formats/xbox/crypto/harness.hpp -- the known-answer tests of the Xbox 360 crypto tools: RSA signatures, RC4, AES, DES and 3DES, MD5,
// SHA-1, HMAC, rot-sum, CPU keys, run over the console's test vectors.
//
//     Inputs in{.test_data = fs::read_file("Data/test_data.bin"), .rsa_key = fs::read_file("Data/rsa_test_key.bin")};
//     Result result = run(in);
//     cb::out_text(result.text());           the report the tool prints
//     return result.passed ? 0 : 1;
//
// The answers are fixed values for one particular test-data file (the `Data/test_data.bin` of the tools this comes from, which is not in
// the repository) and one RSA test key; with other data every check reports FAILED, which is also how the checks themselves are told
// to work (each carries the value it computed and the one it expects, so a test can compare the computing against a reference).
//
//   test data layout   RC4: key 0x10, data 0x10; AES: key 0x10, data 0x10, feed 0x10; DES: key 8, data 8 (CBC: key 8, feed 8, data 64);
//                      3DES: key 0x18, data 8 (CBC: key 0x18, feed 8, data 64); MD5, SHA-1 over all of it; rot-sum over the first 0x20
//                      bytes; HMAC over the first 0x10 bytes as key and the rest, then over the first 0x40 as key and three parts
//                      (0x20000, 0x20000, 6 bytes) from the start.
//
// Errors: UsageError for test data shorter than the layout needs.
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/formats/xbox/crypto/keystore.hpp"
#include "cb/formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::harness {

inline constexpr std::size_t kMinimumTestData = 0x40006;

struct Inputs {
    Bytes test_data;
    Bytes rsa_key;                              // the private test key (console format)
    std::optional<Bytes> valid_cpu_key;         // a console's CPU key, from the key store; without it that check is skipped
    xecrypt::RandomSource* random = nullptr;    // for key generation and CPU-key generation; the operating system's when null
};

// One check: what it computed, what it should be (none for a check made of a yes or no), and the verdict.
struct Check {
    std::string section;                        // "RSA" "RC4" "AES" "DES" "MD5" "SHA" "Misc."
    std::string label;                          // "RC4 first round", "AES-CBC hundred round" ...
    Bytes actual;
    std::optional<Bytes> expected;
    bool skipped = false;
    bool passed = false;
};

struct Result {
    std::vector<Check> checks;
    bool passed = true;                         // every section passed
    // The report: a block per section ("starting RC4 tests...", each check with OK or FAILED, "RC4 OK"), a blank line after each, and
    // the verdict ("SUCCESS! All tests passed!" / "ERROR! One or more tests failed!").
    std::string report;
    std::string text() const { return report; }
};

Result run(const Inputs& inputs);

// `crypto-harness [--data DIR]`: reads DIR/test_data.bin and DIR/rsa_test_key.bin (DIR is "Data" by default) and the CPU key from the key
// store when it has one, prints the report, and exits 0 when all passed and 1 when any failed.
cli::Command command(const keystore::KeyStore& store);

}  // namespace cb::formats::xbox::crypto::harness
