#include "formats/xbox/crypto/harness.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/text.hpp"

namespace cb::formats::xbox::crypto::harness {

namespace {

namespace xe = cb::formats::xbox::crypto::xecrypt;

Bytes hex(std::string_view h) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < h.size(); i += 2) {
        out.push_back(static_cast<std::uint8_t>(std::stoi(std::string(h.substr(i, 2)), nullptr, 16)));
    }
    return out;
}

// ---- the answers, for the console's test data ------------------------------------------------------------------------------------
const std::string_view kRsaTestSalt = "XBOX_HAXXS";
const Bytes& rsa_test_hash() { static const Bytes v = hex("7A71F234F5EBF720C84A13A3C586478DCDE402B8"); return v; }
const Bytes& rc4_1() { static const Bytes v = hex("74285FBF1D46410B22435798B996F947"); return v; }
const Bytes& rc4_100() { static const Bytes v = hex("0B5D42406B73070ACB785F8CFE58C130"); return v; }
const Bytes& aes_ecb_1() { static const Bytes v = hex("59AF4143F1C4722C6EA0FA3FF80EAA9B"); return v; }
const Bytes& aes_ecb_100() { static const Bytes v = hex("A1AD9CE67C736D10668ABCA1B12CDB30"); return v; }
const Bytes& aes_cbc_1() { static const Bytes v = hex("E4F23DBF19C99213E08EBE0067DF23F1"); return v; }
const Bytes& aes_cbc_1_feed() { static const Bytes v = hex("44D49CC606E7146A7571D15537D46D55"); return v; }
const Bytes& aes_cbc_100() { static const Bytes v = hex("977DCE8A2DF1E38702B475BCB29B5337"); return v; }
const Bytes& des_1() { static const Bytes v = hex("EECA54C48ED8256F"); return v; }
const Bytes& des_100() { static const Bytes v = hex("09FA9615CBEC1128"); return v; }
const Bytes& des_cbc() {
    static const Bytes v = hex("58BB062A32178596C177C8FD9A60B0B38DE153ADBD0A9F334E0DAF483B1F1C21D4F907262643697F3BB4411DE046063C44E7A590A0EF5B3A3A7639476E15E1E9");
    return v;
}
const Bytes& des3_1() { static const Bytes v = hex("5EF685A69F187826"); return v; }
const Bytes& des3_100() { static const Bytes v = hex("DAD66BC1E20B2489"); return v; }
const Bytes& des3_cbc() {
    static const Bytes v = hex("5D9B0CB8665A3CACFCA4B4AA81ECA00FBBC6B12DDCE638A6D170FA7215B9BED3CD148AE566C3DADC16943DC3A2429FF054017FC87BCDE82A36CDEF5AE2643BC8");
    return v;
}
const Bytes& md5_digest() { static const Bytes v = hex("DE0137D93167B099A9C766D57C45E8DB"); return v; }
const Bytes& sha_digest() { static const Bytes v = hex("7A71F234F5EBF720C84A13A3C586478DCDE402B8"); return v; }
const Bytes& rotsum_sha_digest() { static const Bytes v = hex("C551302CE570CACA246D060E3478C52D8F5A8A45"); return v; }
const Bytes& hmac_1_digest() { static const Bytes v = hex("CEC5F44ADE23C6F21A42C43171D76407A3A30E1D"); return v; }
const Bytes& hmac_2_digest() { static const Bytes v = hex("7EC867712FB02FBE1792C8F019B01D5025698357"); return v; }
const Bytes& rotsum_digest() { static const Bytes v = hex("0000000000000001F83FB03F944C2298FFFFFFFFFFFFFFFEBFF676A5849C071F"); return v; }
const Bytes& invalid_cpu_key() { static const Bytes v = hex("12345678901234567890123456789010"); return v; }

constexpr int kRounds = 99;   // after the first round, a hundred in all

ByteView slice(ByteView data, std::size_t at, std::size_t n) { return data.subspan(at, n); }

// What one run builds up: the checks and the report text.
class Run {
public:
    explicit Run(Result& result) : result_(result) {}

    void section(std::string_view name) { section_ = std::string(name); result_.report += std::format("starting {} tests...\n", name); }
    // "    <label> - " then OK or FAILED on a line of its own
    void check(std::string label, Bytes actual, const Bytes& expected, std::string_view title = {}) {
        const bool ok = actual == expected;
        result_.report += std::format("    {} - \n{}\n", title.empty() ? label : std::string(title), ok ? "OK" : "FAILED");
        result_.checks.push_back({section_, std::move(label), std::move(actual), expected, false, ok});
        if (!ok) section_ok_ = false;
    }
    void finish(std::string_view name, std::string_view ok_text = "OK") {
        result_.report += std::format("{} {}\n\n", name, section_ok_ ? ok_text : "FAILED!");
        if (!section_ok_) result_.passed = false;
        section_ok_ = true;
    }
    void fail() { section_ok_ = false; }
    void raw(const std::string& line) { result_.report += line; }
    void record(std::string label, bool ok, bool skipped = false, Bytes actual = {}) {
        result_.checks.push_back({section_, std::move(label), std::move(actual), std::nullopt, skipped, ok});
    }

private:
    Result& result_;
    std::string section_;
    bool section_ok_ = true;
};

void rsa_section(Run& run, const Inputs& in, xe::RandomSource& rng) {
    run.section("RSA");
    const xe::RsaKey key = xe::RsaKey::parse(in.rsa_key);
    bool pass = true;
    run.raw("    testing RSA signature creation and verification with static key....\n");
    const Bytes signature = key.sign(rsa_test_hash(), as_bytes(kRsaTestSalt));
    run.raw("        RSA signature created OK\n        RSA signature encrypted OK\n");
    const bool verified = key.verify(signature, rsa_test_hash(), as_bytes(kRsaTestSalt));
    run.raw(verified ? "        RSA signature verified OK\n    RSA test OK\n" : "        RSA signature verify FAILED\n    RSA test FAILED\n");
    run.record("RSA signature", verified, false, signature);
    if (!verified) pass = false;

    run.raw("    testing PKCS1 signature creation and verification with static key....\n");
    const Bytes pkcs1 = key.sign_pkcs1(rsa_test_hash());
    run.raw("        PKCS1 signature created OK\n");
    const bool pkcs1_ok = key.verify_pkcs1(pkcs1, rsa_test_hash());
    run.raw(pkcs1_ok ? "        PKCS1 signature verified OK\n" : "        PKCS1 signature verify FAILED\n");
    run.record("PKCS1 signature", pkcs1_ok, false, pkcs1);
    if (!pkcs1_ok) pass = false;

    run.raw("    testing key generation...\n");
    bool generated = true;
    try {
        const xe::KeyPair pair = xe::generate_key_pair(2048, 0x10001, rng);
        (void)xe::RsaKey::parse(pair.private_key);   // its parameters hang together
    } catch (const FormatError&) {
        generated = false;
    }
    run.raw(generated ? "        Key generation OK\n" : "        Key generation FAILED\n");
    run.record("Key generation", generated);
    if (!generated) pass = false;
    if (!pass) run.fail();
    run.finish("RSA");
}

void rc4_section(Run& run, ByteView data) {
    run.section("RC4");
    xe::Rc4 cipher(slice(data, 0, 0x10));
    Bytes block = cipher.decrypt(slice(data, 0x10, 0x10));
    run.check("RC4 first round", block, rc4_1());
    for (int i = 0; i < kRounds; ++i) block = cipher.decrypt(block);
    run.check("RC4 hundred round", block, rc4_100());
    run.finish("RC4");
}

void aes_section(Run& run, ByteView data) {
    run.section("AES");
    {
        xe::BlockCipher cipher(xe::CipherAlgorithm::Aes, slice(data, 0, 0x10));
        Bytes block = cipher.decrypt(slice(data, 0x10, 0x10));
        run.check("AES-ECB first round", block, aes_ecb_1());
        for (int i = 0; i < kRounds; ++i) block = cipher.decrypt(block);
        run.check("AES-ECB hundred round", block, aes_ecb_100());
    }
    xe::BlockCipher first(xe::CipherAlgorithm::Aes, slice(data, 0, 0x10), xe::CipherMode::Cbc, slice(data, 0x20, 0x10));
    Bytes block = first.decrypt(slice(data, 0x10, 0x10));
    run.check("AES-CBC first round", block, aes_cbc_1());
    xe::BlockCipher hundred(xe::CipherAlgorithm::Aes, slice(data, 0, 0x10), xe::CipherMode::Cbc, aes_cbc_1_feed());
    for (int i = 0; i < kRounds; ++i) block = hundred.decrypt(block);
    run.check("AES-CBC hundred round", block, aes_cbc_100());
    run.finish("AES");
}

void des_section(Run& run, ByteView data) {
    run.section("DES");
    {
        xe::BlockCipher cipher(xe::CipherAlgorithm::Des, slice(data, 0, 8));
        Bytes block = cipher.decrypt(slice(data, 8, 8));
        run.check("DES-ECB first round", block, des_1());
        for (int i = 0; i < kRounds; ++i) block = cipher.decrypt(block);
        run.check("DES-ECB hundred round", block, des_100());
    }
    run.check("DES-CBC",
              xe::BlockCipher(xe::CipherAlgorithm::Des, slice(data, 0, 8), xe::CipherMode::Cbc, slice(data, 8, 8)).decrypt(slice(data, 16, 64)), des_cbc());
    {
        xe::BlockCipher cipher(xe::CipherAlgorithm::Des3, slice(data, 0, 0x18));
        Bytes block = cipher.decrypt(slice(data, 0x18, 8));
        run.check("DES3-ECB first round", block, des3_1());
        for (int i = 0; i < kRounds; ++i) block = cipher.decrypt(block);
        run.check("DES3-ECB hundred round", block, des3_100());
    }
    run.check("DES3-CBC",
              xe::BlockCipher(xe::CipherAlgorithm::Des3, slice(data, 0, 0x18), xe::CipherMode::Cbc, slice(data, 0x18, 8)).decrypt(slice(data, 0x20, 64)),
              des3_cbc());
    run.finish("DES");
}

void md5_section(Run& run, ByteView data) {
    run.section("MD5");
    run.check("MD5 first round", xe::md5(data), md5_digest());
    run.finish("MD5");
}

void sha_section(Run& run, ByteView data) {
    run.section("SHA");
    run.check("SHA first round", xe::sha(data), sha_digest());
    run.check("SHA second round (ROTSUMSHA)", xe::rot_sum_sha(data), rotsum_sha_digest());
    run.check("SHA third round (HMAC)", xe::hmac_sha(slice(data, 0, 0x10), data.subspan(0x10)), hmac_1_digest());
    const ByteView parts[] = {slice(data, 0, 0x20000), slice(data, 0x20000, 0x20000), slice(data, 0x40000, 6)};
    run.check("SHA fourth round (HMAC)", xe::hmac_sha(slice(data, 0, 0x40), parts), hmac_2_digest());
    run.finish("SHA");
}

void misc_section(Run& run, const Inputs& in, ByteView data, xe::RandomSource& rng) {
    run.section("Miscellaneous");
    run.raw("    testing RotSum\n");
    {
        const Bytes actual = xe::rot_sum(slice(data, 0, 0x20));
        const bool ok = actual == rotsum_digest();
        run.raw(ok ? "OK\n" : "FAILED\n");
        run.record("RotSum", ok, false, actual);
        if (!ok) run.fail();
    }
    run.raw("    testing valid CPU key\n");
    if (!in.valid_cpu_key) {
        run.raw("skipped (no cpu_key.bin in the key store)\n");
        run.record("valid CPU key", true, true);
    } else {
        const bool ok = xe::cpu_key_valid(*in.valid_cpu_key);
        run.raw(ok ? "OK\n" : "FAILED\n");
        run.record("valid CPU key", ok);
        if (!ok) run.fail();
    }
    run.raw("    testing invalid CPU key\n");
    {
        const bool ok = !xe::cpu_key_valid(invalid_cpu_key());
        run.raw(ok ? "OK\n" : "FAILED\n");
        run.record("invalid CPU key", ok);
        if (!ok) run.fail();
    }
    run.raw("    testing CPU key generation\n");
    {
        bool ok = true;
        for (int i = 0; i < 20; ++i) ok = xe::cpu_key_valid(xe::cpu_key_generate(rng)) && ok;   // (all twenty are made, as the tool does)
        run.raw(ok ? "OK\n" : "FAILED\n");
        run.record("CPU key generation", ok);
        if (!ok) run.fail();
    }
    run.finish("Misc.");
}

}  // namespace

Result run(const Inputs& inputs) {
    if (inputs.test_data.size() < kMinimumTestData) {
        throw UsageError(std::format("the test data is {:#x} bytes; the checks read {:#x}", inputs.test_data.size(), kMinimumTestData));
    }
    xe::RandomSource& rng = inputs.random != nullptr ? *inputs.random : xe::os_random();
    Result result;
    Run r(result);
    const ByteView data = inputs.test_data;
    rsa_section(r, inputs, rng);
    rc4_section(r, data);
    aes_section(r, data);
    des_section(r, data);
    md5_section(r, data);
    sha_section(r, data);
    misc_section(r, inputs, data, rng);
    result.report += result.passed ? "SUCCESS! All tests passed!\n" : "ERROR! One or more tests failed!\n";
    return result;
}

cli::Command command(const keystore::KeyStore& store) {
    return cli::Command{
        "crypto-harness",
        "run the known-answer tests of the Xbox 360 crypto (RSA, RC4, AES, DES, MD5, SHA-1, HMAC, CPU keys)",
        {cli::Option{"--data", "-d", "the folder with test_data.bin and rsa_test_key.bin", cli::Arg::Value, "DIR", "Data"}},
        [store](const cli::Args& args) {
            const std::filesystem::path data = fs::from_text(args.value_or("data", "Data"));
            Inputs in;
            in.test_data = fs::read_file(data / "test_data.bin");
            in.rsa_key = fs::read_file(data / "rsa_test_key.bin");
            in.valid_cpu_key = xe::load_store_key(store.folder(), "cpu_key.bin");   // a console's own, never committed
            const Result result = run(in);
            cb::out_text(result.text());
            return result.passed ? 0 : 1;
        },
        {},
        "Xbox 360",
        ""};
}

}  // namespace cb::formats::xbox::crypto::harness
