// codebreaker/formats/xbox/crypto/harness.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.harness.

#include "codebreaker/formats/xbox/crypto/harness.hpp"

#include <string>
#include <vector>

#include "codebreaker/formats/xbox/crypto/build_lib.hpp"

namespace codebreaker::formats::xbox::crypto::harness {

using namespace XeCrypt;

namespace {

py::bytes hex_(const char* h) { return py::bytes::fromhex(py::str(h)); }

void say(const std::string& s) { py::print(py::str(s)); }

// struct.unpack_from("<{n0}s {n1}s ...", TEST_DATA, 0): the pieces by size (struct.error when TEST_DATA is too short)
std::vector<py::bytes> unpack_s(const std::vector<int64_t>& sizes) {
    int64_t total = 0;
    for (int64_t n : sizes) total += n;
    if (TEST_DATA.size() < total) {
        throw py::StructError("unpack_from requires a buffer of at least " + std::to_string(total) + " bytes for unpacking " +
                              std::to_string(total) + " bytes at offset 0 (actual buffer size is " + std::to_string(TEST_DATA.size()) + ")");
    }
    std::vector<py::bytes> out;
    int64_t at = 0;
    for (int64_t n : sizes) {
        out.push_back(TEST_DATA.slice(at, at + n));
        at += n;
    }
    return out;
}

const char* ok_or(bool pass, const char* ok, const char* bad) { return pass ? ok : bad; }

}  // namespace

// test data
py::bytes TEST_DATA;

// RSA
py::bytes rsa_test_hash = hex_("7A71F234F5EBF720C84A13A3C586478DCDE402B8");
py::bytes rsa_test_salt("XBOX_HAXXS");

// RC4 ECB
py::bytes rc4_1_round = hex_("74285FBF1D46410B22435798B996F947");
py::bytes rc4_100_round = hex_("0B5D42406B73070ACB785F8CFE58C130");

// AES ECB
py::bytes aes_ecb_1_round = hex_("59AF4143F1C4722C6EA0FA3FF80EAA9B");
py::bytes aes_ecb_100_round = hex_("A1AD9CE67C736D10668ABCA1B12CDB30");

// AES CBC
py::bytes aes_cbc_1_round = hex_("E4F23DBF19C99213E08EBE0067DF23F1");
py::bytes aes_cbc_1_round_feed = hex_("44D49CC606E7146A7571D15537D46D55");
py::bytes aes_cbc_100_round = hex_("977DCE8A2DF1E38702B475BCB29B5337");
py::bytes aes_cbc_100_round_feed = hex_("6C88B3349984F7A0D959FCBA864AFAEC");

// DES CBC
py::bytes des_1_round = hex_("EECA54C48ED8256F");
py::bytes des_100_round = hex_("09FA9615CBEC1128");
py::bytes des_cbc_round = hex_(
    "58BB062A32178596C177C8FD9A60B0B38DE153ADBD0A9F334E0DAF483B1F1C21D4F907262643697F3BB4411DE046063C44E7A590A0EF5B3A3A7639476E15E1E9");

// DES3 CBC
py::bytes des3_1_round = hex_("5EF685A69F187826");
py::bytes des3_100_round = hex_("DAD66BC1E20B2489");
py::bytes des3_cbc_round = hex_(
    "5D9B0CB8665A3CACFCA4B4AA81ECA00FBBC6B12DDCE638A6D170FA7215B9BED3CD148AE566C3DADC16943DC3A2429FF054017FC87BCDE82A36CDEF5AE2643BC8");

// MD5
py::bytes md5_digest = hex_("DE0137D93167B099A9C766D57C45E8DB");

// SHA-1
py::bytes sha_digest = hex_("7A71F234F5EBF720C84A13A3C586478DCDE402B8");
py::bytes rotsumsha_digest = hex_("C551302CE570CACA246D060E3478C52D8F5A8A45");
py::bytes hmac_1_digest = hex_("CEC5F44ADE23C6F21A42C43171D76407A3A30E1D");
py::bytes hmac_2_digest = hex_("7EC867712FB02FBE1792C8F019B01D5025698357");

// RotSum
py::bytes rotsum_digest = hex_("0000000000000001F83FB03F944C2298FFFFFFFFFFFFFFFEBFF676A5849C071F");

// CPU keys
const std::optional<py::bytes>& valid_cpu_key() {
    static const std::optional<py::bytes> key = _store_key(py::str("cpu_key.bin"));   // a console's CPU key, from the key store (not committed)
    return key;
}
py::bytes invalid_cpu_key = hex_("12345678901234567890123456789010");

bool sig_create_verify_test(XeCryptRsaKey& prv_key) {
    py::bytes sig = prv_key.sig_create(rsa_test_hash, rsa_test_salt);
    say("        RSA signature created OK");
    say("        RSA signature encrypted OK");
    if (prv_key.sig_verify(sig, rsa_test_hash, rsa_test_salt)) {
        say("        RSA signature verified OK");
        return true;
    } else {
        say("        RSA signature verify FAILED");
    }
    return false;
}

bool do_rsa_test() {
    bool rsa_pass = true;
    XeCryptRsaKey prv_key(std::get<py::bytes>(build_lib::read_file(py::str("Data/rsa_test_key.bin"))));

    say("starting RSA tests...");
    say("    testing RSA signature creation and verification with static key....");
    if (sig_create_verify_test(prv_key)) {
        say("    RSA test OK");
    } else {
        say("    RSA test FAILED");
        rsa_pass = false;
    }

    say("    testing PKCS1 signature creation and verification with static key....");

    std::optional<py::bytes> sig = prv_key.sig_create_pkcs1(rsa_test_hash);
    say("        PKCS1 signature created OK");
    // (a key too big for PKCS1 gives None, which sig_verify_pkcs1 cannot take)
    if (!sig) throw py::TypeError("a bytes-like object is required, not 'NoneType'");
    if (prv_key.sig_verify_pkcs1(*sig, rsa_test_hash)) {
        say("        PKCS1 signature verified OK");
    } else {
        say("        PKCS1 signature verify FAILED");
        rsa_pass = false;
    }

    say("    testing key generation...");
    auto [pub_key, prv_key_data] = XeCryptBnQwNeRsaKeyGen();
    XeCryptRsaKey key(prv_key_data);
    if (key.verify_parameters()) {
        say("        Key generation OK");
    } else {
        say("        Key generation FAILED");
        rsa_pass = false;
    }

    say(std::string("RSA ") + ok_or(rsa_pass, "OK", "FAILED!"));

    return rsa_pass;
}

bool do_rc4_test() {
    bool rc4_pass = true;

    std::vector<py::bytes> parts = unpack_s({0x10, 0x10});
    py::bytes rc4_key = parts[0];
    py::bytes rc4_data = parts[1];

    say("starting RC4 tests...");
    XeCryptRc4 cipher = XeCryptRc4::new_(rc4_key);
    rc4_data = cipher.decrypt(rc4_data);
    say("    RC4 first round - ");
    if (rc4_data == rc4_1_round) {
        say("OK");
    } else {
        say("FAILED");
        rc4_pass = false;
    }

    for (int64_t cnt = 0; cnt < 99; ++cnt) rc4_data = cipher.decrypt(rc4_data);
    say("    RC4 hundred round - ");
    if (rc4_data == rc4_100_round) {
        say("OK");
    } else {
        say("FAILED");
        rc4_pass = false;
    }

    say(std::string("RC4 ") + ok_or(rc4_pass, "OK", "FAILED!"));

    return rc4_pass;
}

bool do_aes_test() {
    bool aes_pass = true;

    std::vector<py::bytes> parts = unpack_s({XECRYPT_AES_KEY_SIZE, XECRYPT_AES_BLOCK_SIZE});
    py::bytes aes_key = parts[0];
    py::bytes aes_data = parts[1];

    say("starting AES tests...");
    XeCryptAes cipher = XeCryptAes::new_(aes_key);
    aes_data = cipher.decrypt(aes_data);
    say("    AES-ECB first round - ");
    if (aes_data == aes_ecb_1_round) {
        say("OK");
    } else {
        say("FAILED");
        aes_pass = false;
    }

    for (int64_t cnt = 0; cnt < 99; ++cnt) aes_data = cipher.decrypt(aes_data);
    say("    AES-ECB hundred round - ");
    if (aes_data == aes_ecb_100_round) {
        say("OK");
    } else {
        say("FAILED");
        aes_pass = false;
    }

    parts = unpack_s({XECRYPT_AES_KEY_SIZE, XECRYPT_AES_BLOCK_SIZE, XECRYPT_AES_FEED_SIZE});
    aes_key = parts[0];
    aes_data = parts[1];
    py::bytes aes_feed = parts[2];

    XeCryptAes cbc = XeCryptAes::new_(aes_key, XeCryptAes::MODE_CBC, aes_feed);
    aes_data = cbc.decrypt(aes_data);
    say("    AES-CBC first round - ");
    if (aes_data == aes_cbc_1_round) {
        say("OK");
    } else {
        say("FAILED");
        aes_pass = false;
    }

    cbc = XeCryptAes::new_(aes_key, XeCryptAes::MODE_CBC, aes_cbc_1_round_feed);
    for (int64_t cnt = 0; cnt < 99; ++cnt) aes_data = cbc.decrypt(aes_data);
    say("    AES-CBC hundred round - ");
    if (aes_data == aes_cbc_100_round) {
        say("OK");
    } else {
        say("FAILED");
        aes_pass = false;
    }

    say(std::string("AES ") + ok_or(aes_pass, "OK", "FAILED!"));

    return aes_pass;
}

bool do_des_test() {
    bool des_pass = true;

    std::vector<py::bytes> parts = unpack_s({XECRYPT_DES_KEY_SIZE, XECRYPT_DES_BLOCK_SIZE});
    py::bytes des_key = parts[0];
    py::bytes des_data = parts[1];
    say("starting DES tests...");
    des_data = XeCryptDes::new_(des_key).decrypt(des_data);
    say("    DES-ECB first round - ");
    if (des_data == des_1_round) {
        say("OK");
    } else {
        say("FAILED");
        des_pass = false;
    }

    XeCryptDes cipher = XeCryptDes::new_(des_key);
    for (int64_t cnt = 0; cnt < 99; ++cnt) des_data = cipher.decrypt(des_data);
    say("    DES-ECB hundred round - ");
    if (des_data == des_100_round) {
        say("OK");
    } else {
        say("FAILED");
        des_pass = false;
    }

    parts = unpack_s({XECRYPT_DES_KEY_SIZE, XECRYPT_DES_BLOCK_SIZE, XECRYPT_DES_BLOCK_SIZE * 8});
    des_key = parts[0];
    py::bytes des_feed = parts[1];
    des_data = parts[2];
    des_data = XeCryptDes::new_(des_key, XeCryptDes::MODE_CBC, des_feed).decrypt(des_data);
    say("    DES-CBC - ");
    if (des_data == des_cbc_round) {
        say("OK");
    } else {
        say("FAILED");
        des_pass = false;
    }

    parts = unpack_s({XECRYPT_DES3_KEY_SIZE, XECRYPT_DES3_BLOCK_SIZE});
    des_key = parts[0];
    des_data = parts[1];
    des_data = XeCryptDes3::new_(des_key).decrypt(des_data);
    say("    DES3-ECB first round - ");
    if (des_data == des3_1_round) {
        say("OK");
    } else {
        say("FAILED");
        des_pass = false;
    }

    XeCryptDes3 cipher3 = XeCryptDes3::new_(des_key);
    for (int64_t cnt = 0; cnt < 99; ++cnt) des_data = cipher3.decrypt(des_data);
    say("    DES3-ECB hundred round - ");
    if (des_data == des3_100_round) {
        say("OK");
    } else {
        say("FAILED");
        des_pass = false;
    }

    parts = unpack_s({XECRYPT_DES3_KEY_SIZE, XECRYPT_DES3_BLOCK_SIZE, XECRYPT_DES3_BLOCK_SIZE * 8});
    des_key = parts[0];
    des_feed = parts[1];
    des_data = parts[2];
    des_data = XeCryptDes3::new_(des_key, XeCryptDes3::MODE_CBC, des_feed).decrypt(des_data);
    say("    DES3-CBC - ");
    if (des_data == des3_cbc_round) {
        say("OK");
    } else {
        say("FAILED");
        des_pass = false;
    }

    say(std::string("DES ") + ok_or(des_pass, "OK", "FAILED!"));

    return des_pass;
}

bool do_md5_test() {
    bool md5_pass = true;

    say("starting MD5 tests...");

    py::bytes md5_val = XeCryptMd5(py::list<py::bytes>{TEST_DATA});
    say("    MD5 first round - ");
    if (md5_val == md5_digest) {
        say("OK");
    } else {
        say("FAILED");
        md5_pass = false;
    }

    say(std::string("MD5 ") + ok_or(md5_pass, "OK", "FAILED!"));

    return md5_pass;
}

bool do_sha_test() {
    bool sha_pass = true;

    say("starting SHA tests...");

    py::bytes sha_val = XeCryptSha(py::list<py::bytes>{TEST_DATA});
    say("    SHA first round - ");
    if (sha_val == sha_digest) {
        say("OK");
    } else {
        say("FAILED");
        sha_pass = false;
    }

    sha_val = XeCryptRotSumSha(TEST_DATA);
    say("    SHA second round (ROTSUMSHA) - ");
    if (sha_val == rotsumsha_digest) {
        say("OK");
    } else {
        say("FAILED");
        sha_pass = false;
    }

    sha_val = XeCryptHmacSha(TEST_DATA.slice(0, 0x10), py::list<py::bytes>{TEST_DATA.slice(0x10, TEST_DATA.size())});
    say("    SHA third round (HMAC) - ");
    if (sha_val == hmac_1_digest) {
        say("OK");
    } else {
        say("FAILED");
        sha_pass = false;
    }

    std::vector<py::bytes> parts = unpack_s({0x20000, 0x20000, 6});
    sha_val = XeCryptHmacSha(TEST_DATA.slice(0, 0x40), py::list<py::bytes>{parts[0], parts[1], parts[2]});
    say("    SHA fourth round (HMAC) - ");
    if (sha_val == hmac_2_digest) {
        say("OK");
    } else {
        say("FAILED");
        sha_pass = false;
    }

    say(std::string("SHA ") + ok_or(sha_pass, "OK", "FAILED!"));

    return sha_pass;
}

bool do_misc_test() {
    bool misc_pass = true;

    say("starting Miscellaneous tests...");

    say("    testing RotSum");

    std::optional<py::bytes> rotsum = XeCryptRotSum(TEST_DATA.slice(0, 0x20));
    if (rotsum && *rotsum == rotsum_digest) {
        say("OK");
    } else {
        say("FAILED");
        misc_pass = false;
    }

    say("    testing valid CPU key");
    if (!valid_cpu_key()) {
        say("skipped (no cpu_key.bin in the key store)");
    } else if (XeCryptCpuKeyValid(*valid_cpu_key())) {
        say("OK");
    } else {
        say("FAILED");
        misc_pass = false;
    }

    say("    testing invalid CPU key");
    if (!XeCryptCpuKeyValid(invalid_cpu_key)) {
        say("OK");
    } else {
        say("FAILED");
        misc_pass = false;
    }

    say("    testing CPU key generation");
    {
        // all([XeCryptCpuKeyValid(XeCryptCpuKeyGen()) for i in range(20)]): the list is made whole before all() looks at it
        bool all_valid = true;
        for (int64_t i = 0; i < 20; ++i) {
            if (!XeCryptCpuKeyValid(XeCryptCpuKeyGen())) all_valid = false;
        }
        if (all_valid) {
            say("OK");
        } else {
            say("FAILED");
            misc_pass = false;
        }
    }

    say(std::string("Misc. ") + ok_or(misc_pass, "OK", "FAILED!"));

    return misc_pass;
}

int64_t main() {
    bool test_pass = true;

    // test data
    TEST_DATA = std::get<py::bytes>(build_lib::read_file(py::str("Data/test_data.bin")));

    // RSA tests
    if (!do_rsa_test()) test_pass = false;
    say("");

    // RC4 tests
    if (!do_rc4_test()) test_pass = false;
    say("");

    // AES tests
    if (!do_aes_test()) test_pass = false;
    say("");

    // DES tests
    if (!do_des_test()) test_pass = false;
    say("");

    // MD5 tests
    if (!do_md5_test()) test_pass = false;
    say("");

    // SHA-1, RotSumSha, and HMAC-SHA-1 tests
    if (!do_sha_test()) test_pass = false;
    say("");

    // misc. tests
    if (!do_misc_test()) test_pass = false;
    say("");

    // results
    if (test_pass) {
        say("SUCCESS! All tests passed!");
        return 0;
    } else {
        say("ERROR! One or more tests failed!");
        return 1;
    }
}

}  // namespace codebreaker::formats::xbox::crypto::harness
