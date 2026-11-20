// formats/xbox/crypto/xval.hpp -- the "X:" value of the Xbox 360 dashboard, decrypted: whether the console has been flagged for
// security violations.
//
//     std::array<uint8_t, 8> plain = xval::decrypt("YYYYYYYYYYYY", "AABBCCDD-EEFF0011");     // serial, X: value
//     xval::Result result = xval::interpret(plain);
//
// By Redline99, with the flag values updated by cz. The value is eight bytes, single-DES encrypted with the key HMAC-SHA1(serial + NUL,
// "XBOX360SSB")[0..8]. Decrypted, it is two big-endian words: nothing set means the console is clean, all ones that the security data is
// invalid, and both words non-zero that the serial or the value is wrong; otherwise the low word holds the violation flags.
//
// Errors: UsageError for a serial that is not 12 characters or a value that is not 16 hex digits (dashes ignored).
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "core/cli.hpp"

namespace cb::formats::xbox::crypto::xval {

using Value = std::array<std::uint8_t, 8>;

enum class Verdict { Clean, Invalid, DecryptionError, Flags };

struct Result {
    Verdict verdict = Verdict::Clean;
    std::vector<std::string_view> findings;   // the violations a Flags verdict names, in flag order
};

Value decrypt(std::string_view console_serial, std::string_view console_xval);

Result interpret(const Value& value);

// What the verdict says, a line each: "Secdata is Clean", "Secdata is invalid", "Secdata decryption error" or the findings.
std::string describe(const Result& result);

cli::Command command();

}  // namespace cb::formats::xbox::crypto::xval
