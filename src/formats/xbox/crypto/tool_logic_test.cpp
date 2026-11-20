// Tests of the logic of the small crypto tools (the signers, XSM3, the keyvault/fuse maker, the patch pipeline, the key tools, MemCrypto, the
// LZX codec's loading, the .def maker) against recorded answers on fixed inputs: testdata/tools_fn_goldens.txt.
// The recording's randomness (os.urandom, XeCryptRandom, randbytes) is replaced there by pattern(n, seed) for a request
// of n bytes and the native code gets the same bytes from PatternRandom, so the results are equal byte for byte; the key store is replaced by a key
// pair made for the tests (testdata/test_rsa_prv.bin and _pub.bin) and the 1BL key is pattern(16, 42).
//
// NOT run on real data (none is on this machine): the key store's own keys (SB_prv.bin, HVX_prv.bin, 1BL_key.bin), the Xbox 360 SDK's dumpbin, upstream's
// bin/ (xenon-as, the LZX DLLs, HV_17559_Cleaned.bin). Those paths are tested with the test keys, canned dumpbin text and by their refusals.
//
// Behaviour pinned here: the tools read the key store through the Environment's KeyStore (a missing key is a NotFoundError, a wrong checksum a
// FormatError); HXPC/SIGC signing takes the CPU key as an option.
#include <algorithm>
#include <array>
#include <format>
#include <map>
#include <string>
#include <vector>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "formats/command_test_kit.hpp"
#include "formats/xbox/crypto/bin2lang.hpp"
#include "formats/xbox/crypto/cpu_key.hpp"
#include "formats/xbox/crypto/cpukey_ecd.hpp"
#include "formats/xbox/crypto/custom_keys.hpp"
#include "formats/xbox/crypto/exp_signer.hpp"
#include "formats/xbox/crypto/kv_tool.hpp"
#include "formats/xbox/crypto/lzx_dll.hpp"
#include "formats/xbox/crypto/mem_crypto_tool.hpp"
#include "formats/xbox/crypto/patch_tools.hpp"
#include "formats/xbox/crypto/tools.hpp"
#include "formats/xbox/crypto/xecrypt.hpp"
#include "formats/xbox/crypto/xekeys_signer.hpp"
#include "formats/xbox/crypto/xkelib_exports.hpp"
#include "formats/xbox/crypto/xsm3.hpp"

namespace tool_logic_test {

using namespace cb;
namespace stdfs = std::filesystem;
namespace xcrypto = cb::formats::xbox::crypto;
namespace xecrypt = cb::formats::xbox::crypto::xecrypt;

Bytes pattern(std::size_t n, int seed) {
    Bytes out(n);
    for (std::size_t i = 0; i < n; ++i) out[i] = static_cast<std::uint8_t>((i * 131 + static_cast<std::size_t>(seed) * 17 + (i >> 8) * 7) & 0xFF);
    return out;
}

// pattern(n, seed) for each request of n bytes: the random source the goldens were recorded with.
class PatternRandom : public xecrypt::RandomSource {
public:
    explicit PatternRandom(int seed) : seed_(seed) {}
    void fill(ByteSpan out) override {
        const Bytes bytes = pattern(out.size(), seed_);
        std::copy(bytes.begin(), bytes.end(), out.begin());
    }

private:
    int seed_;
};

Bytes testdata_file(const std::string& name) { return fs::read_file(testing::data_path("formats/xbox/crypto/testdata/" + name)); }

// The lines of a section of the function-level goldens.
std::vector<std::string> section(const std::string& name) {
    const std::string log = fs::read_text(testing::data_path("formats/xbox/crypto/testdata/tools_fn_goldens.txt"));
    std::vector<std::string> lines;
    bool inside = false;
    for (const std::string_view line : text::lines(log)) {
        if (line.starts_with("== ")) inside = line.substr(3) == name;
        else if (inside) lines.emplace_back(line);
    }
    CB_CHECK(!lines.empty());
    return lines;
}

// "key value" of a section: the value (everything after the first space).
std::string value(const std::string& name, const std::string& key) {
    for (const std::string& line : section(name))
        if (line.starts_with(key + " ")) return line.substr(key.size() + 1);
    testing::record_failure(__FILE__, __LINE__, "no " + key + " in " + name);
    return {};
}

Bytes hex_value(const std::string& name, const std::string& key) { return cb::crypto::from_hex(value(name, key)); }

// Whether running the command with `args` throws an E (a failing handler's exception reaches the caller; the program's main turns it into an exit code).
template <class E>
bool fails_with(const cli::Command& command, std::vector<std::string> args) {
    try {
        (void)testing::run_command(command, std::move(args));
    } catch (const E&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

std::string hex(ByteView data) { return cb::crypto::to_hex(data); }
std::string sha1_hex(ByteView data) { return cb::crypto::to_hex(cb::crypto::sha1(data)); }

xecrypt::RsaKey test_key() { return xecrypt::RsaKey::parse(testdata_file("test_rsa_prv.bin")); }
Bytes bl1_key() { return pattern(16, 42); }

// ---- cpu_key ---------------------------------------------------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.tool_logic.cpu_key_splits_the_bits", [] {
    const std::string expected = text::join(section("cpu_key_report"), "\n") + "\n";
    CB_CHECK_EQ(xcrypto::cpu_key::report(pattern(16, 1)), expected);
    // a key the generator makes is valid, and the report's parts are 26 and 102 bits
    const std::string report = xcrypto::cpu_key::report(xecrypt::cpu_key_generate());
    const std::vector<std::string_view> lines = text::lines(report);
    CB_CHECK_EQ(lines.size(), std::size_t{3});
    CB_CHECK_EQ(lines[0].size(), std::string_view("ECC Bits: ").size() + 26);
    CB_CHECK_EQ(lines[1].size(), std::string_view("UID Bits: ").size() + 102);
    CB_CHECK(xecrypt::cpu_key_valid(cb::crypto::from_hex(lines[2])));
    CB_CHECK_THROWS_AS(UsageError, xcrypto::cpu_key::report(Bytes(15, 0)));
});

// ---- the signers -----------------------------------------------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.tool_logic.xekeys_signer_gives_the_recorded_payload", [] {
    const xecrypt::RsaKey key = test_key();
    for (const auto& [name, magic] : {std::pair{"devkit", xcrypto::xekeys_signer::PayloadMagic::Devkit}, std::pair{"retail", xcrypto::xekeys_signer::PayloadMagic::Retail}}) {
        PatternRandom random(41);
        const Bytes signed_image = xcrypto::xekeys_signer::sign(key, bl1_key(), pattern(100, 40), magic, random);
        CB_CHECK_EQ(std::format("{} sha1 {}", signed_image.size(), sha1_hex(signed_image)), value(std::string("xekeys_") + name, "size"));
    }
});

CB_TEST("formats.xbox.crypto.tool_logic.xekeys_signer_output_verifies_after_decrypting", [] {
    // a check that does not need the recording: the output decrypts to a payload whose signature the public key verifies
    const xecrypt::RsaKey key = test_key();
    PatternRandom random(70);
    const Bytes payload = pattern(300, 71);
    const Bytes image = xcrypto::xekeys_signer::sign(key, bl1_key(), payload, xcrypto::xekeys_signer::PayloadMagic::Retail, random);
    CB_CHECK_EQ(image.size(), std::size_t{0x120} + payload.size());
    CB_CHECK_EQ(image[0], 0x4D);
    CB_CHECK_EQ(image[1], 0x4D);
    CB_CHECK_EQ(image[2], 0xDE);
    CB_CHECK_EQ(image[3], 0xAD);
    const Bytes hvx_key(image.begin() + 0x10, image.begin() + 0x20);
    Bytes plain = image;
    const Bytes rc4_key = xecrypt::hmac_sha(bl1_key(), hvx_key);
    const Bytes body = xecrypt::rc4(ByteView(rc4_key).first(0x10), ByteView(image).subspan(0x20));
    std::copy(body.begin(), body.end(), plain.begin() + 0x20);
    CB_CHECK(std::equal(plain.begin() + 0x120, plain.end(), payload.begin()));
    Bytes covered(plain.begin(), plain.begin() + 0x10);
    covered.insert(covered.end(), plain.begin() + 0x120, plain.end());
    CB_CHECK(key.public_key().verify(ByteView(plain).subspan(0x20, 0x100), xecrypt::rot_sum_sha(covered), as_bytes("XBOX_EX_01")));
    CB_CHECK_THROWS_AS(UsageError, xcrypto::xekeys_signer::sign(key, Bytes(8, 0), payload, xcrypto::xekeys_signer::PayloadMagic::Devkit, random));
});

CB_TEST("formats.xbox.crypto.tool_logic.exp_signer_gives_the_recorded_expansions", [] {
    namespace exp = xcrypto::exp_signer;
    const xecrypt::RsaKey key = test_key();
    const Bytes payload = pattern(70, 44);
    const auto check = [&](const std::string& name, exp::Options options) {
        PatternRandom random(43);
        options.bl1_key = bl1_key();
        const Bytes image = exp::sign(key, payload, options, random);
        CB_CHECK_EQ(std::to_string(image.size()) + " sha1 " + sha1_hex(image), value(name, "size"));
    };
    check("exp_encrypted", exp::Options{.encrypt = true});
    check("exp_sigm", exp::Options{.magic = exp::ExpansionMagic::Sigm});
    check("exp_unencrypted", exp::Options{.encrypt = false});
});

CB_TEST("formats.xbox.crypto.tool_logic.exp_signer_hxpc_and_sigc_take_a_cpu_key", [] {
    namespace exp = xcrypto::exp_signer;
    const xecrypt::RsaKey key = test_key();
    xecrypt::SeededRandom random(5);
    const xcrypto::cpukey_ecd::Key generated = xcrypto::cpukey_ecd::generate(random);
    const Bytes cpu_key(generated.begin(), generated.end());
    for (const exp::ExpansionMagic magic : {exp::ExpansionMagic::Hxpc, exp::ExpansionMagic::Sigc}) {
        exp::Options options;
        options.magic = magic;
        options.cpu_key = cpu_key;
        const Bytes image = exp::sign(key, pattern(32, 3), options, random);
        // the signature is over the first 0x30 bytes, with the CPU key's HMAC as the hash
        CB_CHECK(key.public_key().verify(ByteView(image).subspan(0x30, 0x100), xecrypt::hmac_sha(cpu_key, ByteView(image).first(0x30)), as_bytes(exp::kSalt)));
        options.cpu_key = Bytes(16, 0);
        CB_CHECK_THROWS_AS(UsageError, exp::sign(key, pattern(32, 3), options, random));
    }
    // HXPR with encryption and no 1BL key
    exp::Options options;
    CB_CHECK_THROWS_AS(UsageError, exp::sign(key, pattern(32, 3), options, random));
});

// ---- XSM3 ------------------------------------------------------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.tool_logic.xsm3_primitives_match_the_recorded_output", [] {
    namespace x = xcrypto::xsm3;
    const Bytes k = pattern(16, 50), data = pattern(32, 51);
    CB_CHECK_EQ(hex(x::crypt(k, data, x::CryptMode::Encrypt)), value("xsm3_primitives", "crypt_encrypt"));
    CB_CHECK_EQ(hex(x::crypt(k, data, x::CryptMode::Decrypt)), value("xsm3_primitives", "crypt_decrypt"));
    CB_CHECK_EQ(hex(x::mac(k, std::nullopt, data).mac), value("xsm3_primitives", "mac_no_salt"));
    const x::Mac salted = x::mac(k, ByteView(pattern(16, 52)), data);
    CB_CHECK_EQ(hex(salted.salt) + " " + hex(salted.mac), value("xsm3_primitives", "mac_salt"));
    CB_CHECK_EQ(hex(x::mac(pattern(20, 53), std::nullopt, data).mac), value("xsm3_primitives", "mac_20_byte_key"));
    CB_CHECK_EQ(hex(x::acr(k, pattern(8, 54), pattern(0x20, 55))), value("xsm3_primitives", "acr"));
});

CB_TEST("formats.xbox.crypto.tool_logic.xsm3_session_answers_the_challenge", [] {
    namespace x = xcrypto::xsm3;
    PatternRandom random(46);
    x::StaticKeys keys;
    const Bytes k1 = pattern(16, 44), k2 = pattern(16, 45);
    std::copy(k1.begin(), k1.end(), keys.key1.begin());
    std::copy(k2.begin(), k2.end(), keys.key2.begin());
    x::Session session(keys, x::sample_identification(), random);
    const Result<Bytes> init = session.challenge_init(hex_value("xsm3_exchange", "challenge"));
    CB_CHECK(init.ok());
    CB_CHECK_EQ(hex(init.value()), value("xsm3_exchange", "init_response"));
    const Result<Bytes> verified = session.challenge_verify(hex_value("xsm3_exchange", "verify"));
    CB_CHECK(verified.ok());
    CB_CHECK_EQ(hex(verified.value()), value("xsm3_exchange", "verify_response"));
});

CB_TEST("formats.xbox.crypto.tool_logic.xsm3_refuses_what_does_not_check", [] {
    namespace x = xcrypto::xsm3;
    PatternRandom random(46);
    x::StaticKeys keys;
    std::copy_n(pattern(16, 44).begin(), 16, keys.key1.begin());
    std::copy_n(pattern(16, 45).begin(), 16, keys.key2.begin());
    x::Session session(keys, x::sample_identification(), random);
    Bytes challenge = hex_value("xsm3_exchange", "challenge");
    // a wrong length is a usage error; a damaged checksum, a damaged MAC and a verify before an init are failed results
    CB_CHECK_THROWS_AS(UsageError, (void)session.challenge_init(ByteView(challenge).first(10)));
    Bytes bad_checksum = challenge;
    bad_checksum.back() ^= 1;
    CB_CHECK(!session.challenge_init(bad_checksum).ok());
    Bytes bad_mac = challenge;
    bad_mac[0x1E] ^= 1;
    bad_mac.back() = x::packet_checksum(bad_mac);
    CB_CHECK(!session.challenge_init(bad_mac).ok());
    CB_CHECK(!session.challenge_verify(hex_value("xsm3_exchange", "verify")).ok());
    CB_CHECK_THROWS_AS(UsageError, x::Session(keys, ByteView(challenge), random));
    CB_CHECK(x::packet_checksum_ok(challenge));
    CB_CHECK(!x::packet_checksum_ok(bad_checksum));
});

// ---- kv_tool -----------------------------------------------------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.tool_logic.kv_tool_makes_the_recorded_fuses_and_keyvault", [] {
    namespace kv = xcrypto::kv_tool;
    const Bytes cpu_key = hex_value("kv_tool", "cpu_key");
    const Bytes vault = pattern(0x4000, 61);
    PatternRandom random(60);
    const kv::Output out = kv::make(vault, cpu_key, Bytes(16, 0), kv::ConsoleType::Devkit, random);
    CB_CHECK_EQ(hex(out.fuses), value("kv_tool", "fuses"));
    CB_CHECK_EQ(sha1_hex(out.keyvault), value("kv_tool", "kv_enc_sha1"));
    // the recorded file opens with the native decryption to the vault (with the DVD key zeroed at 0x100)
    const Bytes recorded_sealed = testdata_file("kv_tool_enc.bin");
    CB_CHECK_EQ(sha1_hex(recorded_sealed), value("kv_tool", "kv_enc_sha1"));
    const Result<Bytes> opened = xecrypt::keyvault_decrypt(cpu_key, recorded_sealed);
    CB_CHECK(opened.ok());
    Bytes expected = vault;
    std::fill_n(expected.begin() + 0x100, 0x10, std::uint8_t{0});
    CB_CHECK(std::equal(opened.value().begin() + 0x18, opened.value().end(), expected.begin() + 0x18));
    // the other console types
    const auto types = [&](kv::ConsoleType type) { return kv::make(vault, cpu_key, Bytes(16, 0), type, random).fuses; };
    CB_CHECK_EQ(hex(ByteView(types(kv::ConsoleType::TestKit)).subspan(0xE, 2)), "f00f");
    CB_CHECK_EQ(hex(ByteView(types(kv::ConsoleType::RetailPhat)).subspan(0xE, 2)), "0ff0");
    CB_CHECK_EQ(hex(ByteView(types(kv::ConsoleType::RetailSlim)).subspan(0xE, 2)), "f0f0");
    CB_CHECK_THROWS_AS(UsageError, kv::make(vault, Bytes(16, 0), Bytes(16, 0), kv::ConsoleType::Devkit, random));      // not a valid CPU key
    CB_CHECK_THROWS_AS(UsageError, kv::make(Bytes(100, 0), cpu_key, Bytes(16, 0), kv::ConsoleType::Devkit, random));
    CB_CHECK_THROWS_AS(UsageError, kv::make(vault, cpu_key, Bytes(8, 0), kv::ConsoleType::Devkit, random));
});

CB_TEST("formats.xbox.crypto.tool_logic.kv_tool_command_writes_the_two_files_and_prints_the_lines", [] {
    const fs::ScopedTempDir dir{"cb-crypto-kv"};
    const std::string w = text::replace_all(fs::to_text(dir.path()), "\\", "/");
    fs::write_file(dir.path() / "banned.bin", pattern(0x4000, 61));
    const std::string cpu = text::upper(value("kv_tool", "cpu_key"));
    const std::string out = testing::run_command(xcrypto::kv_tool::command(), {"--keyvault", w + "/banned.bin", "--output-dir", w + "/o", "--cpu-key", cpu});
    CB_CHECK(out.starts_with("CPU key: " + cpu + "\nDVD key: 00000000000000000000000000000000\n\nFuses:\n"));
    CB_CHECK(out.ends_with("exit=0\n"));
    CB_CHECK_EQ(hex(fs::read_file(dir.path() / "o" / "fuses.bin")), value("kv_tool", "fuses"));
    CB_CHECK_EQ(fs::file_size(dir.path() / "o" / "kv_enc.bin"), std::uint64_t{0x4000});
    CB_CHECK((fails_with<UsageError>(xcrypto::kv_tool::command(), {"--keyvault", w + "/banned.bin", "--output-dir", w + "/o", "--console-type", "toaster"})));
    CB_CHECK((fails_with<UsageError>(xcrypto::kv_tool::command(), {"--keyvault", w + "/banned.bin", "--output-dir", w + "/o", "--cpu-key", "12"})));
});

// ---- the patch pipeline -------------------------------------------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.tool_logic.patch_dumper_writes_the_assembly", [] {
    namespace pt = xcrypto::patch_tools;
    Bytes table;
    const auto record = [&table](std::uint32_t address, const std::vector<std::uint32_t>& words) {
        for (const std::uint32_t v : {address, static_cast<std::uint32_t>(words.size())})
            for (int i = 3; i >= 0; --i) table.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
        for (const std::uint32_t v : words)
            for (int i = 3; i >= 0; --i) table.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
    };
    record(0x80001000, {0x38600001, 0x4E800020, 0xFFFFFFFF, 0x7C0802A6, 0x60000000, 0x48000010, 0x2C030000, 0x80010008});
    record(0x80002000, {0x4E800020});
    for (int i = 0; i < 4; ++i) table.push_back(0xFF);
    const std::string got = pt::disassemble(table);
    // the header differs, the rest is its text
    const std::string marker = "MAKEPATCH";
    const std::string body = got.substr(got.find(marker));
    std::string expected = text::join(section("patch_dumper"), "\n");
    expected = expected.substr(expected.find(marker));
    CB_CHECK_EQ(std::string(text::trim(body)), std::string(text::trim(expected)));
    // a word Capstone cannot decode is kept as a .long
    const Bytes odd = {0, 0, 0x10, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0x4E, 0x80, 0, 0x20, 0xFF, 0xFF, 0xFF, 0xFF};
    CB_CHECK(pt::disassemble(odd).find("0:\n\t.long 0x00000000\n\tblr\n9:") != std::string::npos);
    CB_CHECK(got.starts_with("# "));
    // a table cut short is an error
    CB_CHECK_THROWS_AS(TruncatedError, pt::disassemble(ByteView(table).first(10)));
});

CB_TEST("formats.xbox.crypto.tool_logic.patch_conflicts_count_touching_patches", [] {
    namespace pt = xcrypto::patch_tools;
    Bytes table;
    const auto record = [&table](std::uint32_t address, std::size_t words) {
        for (const std::uint32_t v : {address, static_cast<std::uint32_t>(words)})
            for (int i = 3; i >= 0; --i) table.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
        table.resize(table.size() + words * 4, 0);
    };
    record(0x100, 4);    // 0x100..0x110
    record(0x111, 1);    // 0x111..0x115: one byte after the first ends: counts the byte after
    record(0x200, 1);
    for (int i = 0; i < 4; ++i) table.push_back(0xFF);
    const std::vector<pt::Conflict> conflicts = pt::find_conflicts(table);
    CB_CHECK_EQ(conflicts.size(), std::size_t{2});
    CB_CHECK_EQ(conflicts[0].a_start, std::uint32_t{0x100});
    CB_CHECK_EQ(conflicts[0].b_start, std::uint32_t{0x111});
    CB_CHECK(pt::conflict_report({}).empty());
});

CB_TEST("formats.xbox.crypto.tool_logic.the_patch_builds_need_the_free60_tools", [] {
    const fs::ScopedTempDir dir{"cb-crypto-patch"};
    xcrypto::build_lib::PatchTools tools;
    tools.bin_dir = dir.path() / "no-bin";
    fs::make_dirs(dir.path() / "Patches" / "RGL" / "17559-dev");
    fs::write_text(dir.path() / "Patches" / "RGL" / "17559-dev" / "RGLoader-dev.S", ".text\n");
    CB_CHECK_THROWS_AS(ToolError, xcrypto::patch_tools::compile_patches(tools, dir.path()));
});

// ---- the key tools -------------------------------------------------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.tool_logic.create_key_makes_a_pair_in_the_key_store_and_export_keys_prints_it_signed", [] {
    const fs::ScopedTempDir dir{"cb-crypto-keys"};
    xecrypt::SeededRandom random(11);
    xcrypto::custom_keys::create(dir.path() / "Keys", 2048, random);
    const Bytes pub = fs::read_file(dir.path() / "Keys" / "custom_pub.bin"), prv = fs::read_file(dir.path() / "Keys" / "custom_prv.bin");
    CB_CHECK_EQ(pub.size(), xecrypt::kRsaPub2048Size);
    CB_CHECK_EQ(prv.size(), xecrypt::kRsaPrv2048Size);
    CB_CHECK(!xecrypt::RsaKey::parse(pub).is_private());
    CB_CHECK(xecrypt::RsaKey::parse(prv).is_private());
    CB_CHECK(equal_bytes(ByteView(prv).first(pub.size()), pub));        // the public key is the first part of the private one

    // export_keys: four arrays, the signature one that the public key verifies over the data array's rot-sum-SHA
    PatternRandom pattern_random(80);
    const std::string source = xcrypto::custom_keys::export_source(dir.path() / "Keys", pattern_random);
    const auto array_of = [&source](const std::string& name) {
        const std::size_t at = source.find("BYTE " + name + "[] = {");
        CB_CHECK(at != std::string::npos);
        const std::size_t end = source.find("};", at);
        Bytes bytes;
        const std::string block = source.substr(at, end - at);   // (named: a temporary in a range-for's range expression would be gone before the loop)
        for (const std::string_view row : text::lines(block))
            for (const std::string_view part : text::split(row, ',')) {
                const std::string_view t = text::trim(part);
                if (t.starts_with("0x")) bytes.push_back(*text::parse_int<std::uint8_t>(t.substr(2), 16));
            }
        return bytes;
    };
    CB_CHECK(source.find("TestPrvKey") < source.find("TestPubKey") && source.find("TestPubKey") < source.find("TestData") && source.find("TestData") < source.find("TestSignature"));
    CB_CHECK(equal_bytes(array_of("TestPrvKey"), prv));
    CB_CHECK(equal_bytes(array_of("TestPubKey"), pub));
    CB_CHECK(equal_bytes(array_of("TestData"), pattern(0x100, 80)));
    const Bytes signature = array_of("TestSignature");
    CB_CHECK_EQ(signature.size(), std::size_t{0x100});
    CB_CHECK(xecrypt::RsaKey::parse(pub).verify(signature, xecrypt::rot_sum_sha(array_of("TestData")), as_bytes("SALTTEST")));

    // the commands: in the store's folder
    xcrypto::Environment environment;
    environment.keys = xcrypto::keystore::KeyStore(dir.path() / "More");
    const std::string made = testing::run_command(xcrypto::command(environment), {"create_key"});
    CB_CHECK_EQ(made, "Done!\nexit=0\n");
    CB_CHECK(stdfs::is_regular_file(dir.path() / "More" / "custom_pub.bin"));
    const std::string exported = testing::run_command(xcrypto::command(environment), {"export_keys"});
    CB_CHECK(exported.find("TestSignature") != std::string::npos && exported.ends_with("exit=0\n"));
    // no pair: a not-found error
    environment.keys = xcrypto::keystore::KeyStore(dir.path() / "Nothing");
    CB_CHECK((fails_with<NotFoundError>(xcrypto::command(environment), {"export_keys"})));
});

// ---- MemCrypto ------------------------------------------------------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.tool_logic.mem_crypto_tool_gives_the_recorded_digests", [] {
    const std::string report = xcrypto::mem_crypto_tool::calculate_report(pattern(0x40000, 62));
    const std::string w = "W: " + std::string(32, '5') + "\nA: " + std::string(32, '5') + "\nH: " + std::string(32, '5') + "\n\n";
    CB_CHECK_EQ(report, w + w + "Hash 1: " + value("memcrypto_calculate", "hash1") + "\n\nHash 2: " + value("memcrypto_calculate", "hash2") + "\n\nKey blob: " +
                            value("memcrypto_calculate", "blob") + "\n\n");

    // `test`: decrypt HV.enc.bin with the keys of keys.bin into HV.dec.bin
    const fs::ScopedTempDir dir{"cb-crypto-mem"};
    fs::write_file(dir.path() / "keys.bin", pattern(48, 64));
    fs::write_file(dir.path() / "HV.enc.bin", pattern(0x400, 65));
    const fs::ScopedTempDir work{"cb-crypto-mem-out"};
    const stdfs::path before = stdfs::current_path();
    stdfs::current_path(work.path());
    const std::string out = testing::run_command(xcrypto::mem_crypto_tool::command(dir.path()), {"test"});
    stdfs::current_path(before);
    CB_CHECK_EQ(out, "Done!\nexit=0\n");
    CB_CHECK_EQ(sha1_hex(fs::read_file(work.path() / "HV.dec.bin")), value("memcrypto_decrypt", "sha1"));
    CB_CHECK((fails_with<NotFoundError>(xcrypto::mem_crypto_tool::command(dir.path() / "missing"), {"calculate"})));
});

// ---- the LZX codec and the .def maker ---------------------------------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.tool_logic.the_lzx_codec_is_absent_without_the_dlls", [] {
    const fs::ScopedTempDir dir{"cb-crypto-lzx"};
    CB_CHECK(xcrypto::lzx_dll::load(dir.path()) == nullptr);
    const std::shared_ptr<xcrypto::build_lib::LzxCodec> lazy = xcrypto::lzx_dll::lazy(dir.path());
    CB_CHECK(lazy != nullptr);
    CB_CHECK_THROWS_AS(UnsupportedError, lazy->compress_continuous(pattern(10, 1)));
    CB_CHECK((fails_with<NotFoundError>(xcrypto::lzx_dll::command(dir.path()), {})));
});

CB_TEST("formats.xbox.crypto.tool_logic.xkelib_exports_reads_dumpbins_text", [] {
    namespace xk = xcrypto::xkelib_exports;
    const Bytes headers_bytes = hex_value("xkelib_exports", "headers");
    const std::string headers(as_text(headers_bytes));
    const Bytes exports_bytes = hex_value("xkelib_exports", "exports");
    const std::string exports(as_text(exports_bytes));
    CB_CHECK_EQ(xk::library_name(headers), "xboxkrnl.exe");
    xk::Library library{xk::library_name(headers), xk::parse_exports(exports)};
    const Bytes expected = hex_value("xkelib_exports", "def");
    CB_CHECK_EQ(xk::def_text(library), std::string(as_text(expected)));
    CB_CHECK(xk::library_name("nothing here").empty());
    CB_CHECK(xk::parse_exports("").empty());
    // without the SDK the command says so
    CB_CHECK((fails_with<NotFoundError>(xk::command(stdfs::path("no-such-sdk")), {"a.lib", "a.def"})));
});

}  // namespace tool_logic_test
