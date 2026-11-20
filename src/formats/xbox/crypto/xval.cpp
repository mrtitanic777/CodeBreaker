#include "formats/xbox/crypto/xval.hpp"

#include <algorithm>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/log.hpp"
#include "formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::xval {

namespace {

struct Flag {
    std::uint32_t bit;
    std::string_view meaning;
};

// The violation flags of the low word. The "(Deprecated)" ones are no longer set by the console software.
constexpr Flag kFlags[] = {
    {0x00001, "AuthEx Challenge Failure (Deprecated)"},
    {0x00002, "AuthEx Table missing (Deprecated)"},
    {0x00004, "AuthEx Reserved Flag"},
    {0x00008, "Invalid DVD Geometry"},
    {0x00010, "Invalid DVD DMI"},
    {0x00020, "DVD Keyvault Pair Mismatch (Deprecated)"},
    {0x00040, "Invalid CRL Data (Deprecated)"},
    {0x00080, "CRL Certificate Revoked"},
    {0x00100, "Unauthorized Install"},
    {0x00200, "Keyvault Policy Violation"},
    {0x00400, "Console Banned (Deprecated)"},
    {0x00800, "ODD Violation"},
    {0x01000, "Content Integrity Verification Hash Failure"},
    {0x02000, "AuthEx Table missing"},
    {0x04000, "Invalid CRL Data"},
    {0x08000, "Enforce ODD Throughput Low"},
    {0x10000, "Enforce ODD Throughput High"},
};
constexpr std::uint32_t kUnknownFlags = 0xFFFE0000;   // bits no flag is known for

std::uint32_t be32(const Value& v, std::size_t at) {
    return (std::uint32_t{v[at]} << 24) | (std::uint32_t{v[at + 1]} << 16) | (std::uint32_t{v[at + 2]} << 8) | v[at + 3];
}

}  // namespace

Value decrypt(std::string_view console_serial, std::string_view console_xval) {
    if (console_serial.size() != 0xC) throw UsageError("the console serial is 12 characters");
    std::string digits;
    for (const char c : console_xval)
        if (c != '-') digits += c;
    Bytes encrypted;
    try {
        encrypted = cb::crypto::from_hex(digits);
    } catch (const FormatError&) {
        throw UsageError("the X: value is hex digits");
    }
    if (encrypted.size() != 8) throw UsageError("the X: value is 8 bytes (16 hex digits)");
    Bytes hmac_key = to_bytes(console_serial);
    hmac_key.push_back(0);
    const Bytes mac = xecrypt::hmac_sha(hmac_key, as_bytes("XBOX360SSB"));
    xecrypt::BlockCipher des(xecrypt::CipherAlgorithm::Des, ByteView(mac).first(8), xecrypt::CipherMode::Ecb);
    const Bytes plain = des.decrypt(encrypted);
    Value out{};
    std::copy_n(plain.begin(), out.size(), out.begin());
    return out;
}

Result interpret(const Value& value) {
    const std::uint32_t high = be32(value, 0), low = be32(value, 4);
    Result result;
    if (high == 0 && low == 0) {
        result.verdict = Verdict::Clean;
    } else if (high == 0xFFFFFFFF && low == 0xFFFFFFFF) {
        result.verdict = Verdict::Invalid;
    } else if (high != 0 && low != 0) {
        result.verdict = Verdict::DecryptionError;
    } else {
        result.verdict = Verdict::Flags;
        for (const Flag& flag : kFlags)
            if (low & flag.bit) result.findings.push_back(flag.meaning);
        if (low & kUnknownFlags) result.findings.push_back("Unknown Violation(s)");
    }
    return result;
}

std::string describe(const Result& result) {
    switch (result.verdict) {
        case Verdict::Clean: return "Secdata is Clean\n";
        case Verdict::Invalid: return "Secdata is invalid\n";
        case Verdict::DecryptionError: return "Secdata decryption error\n";
        case Verdict::Flags: break;
    }
    std::string out;
    for (const std::string_view finding : result.findings) out += std::string(finding) + "\n";
    return out;
}

cli::Command command() {
    return cli::Command{"xval",
                        "decrypt the \"X:\" value of the Xbox 360 dashboard: has the console been flagged",
                        {cli::Positional{"serial", "the console's serial number"}, cli::Positional{"xval", "the console's X: value"}},
                        [](const cli::Args& args) {
                            cb::out_text(describe(interpret(decrypt(args.required("serial"), args.required("xval")))));
                            return 0;
                        },
                        {},
                        "",
                        ""};
}

}  // namespace cb::formats::xbox::crypto::xval
