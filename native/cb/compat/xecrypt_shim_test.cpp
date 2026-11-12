// The shim at the old header path of formats.xbox.crypto.XeCrypt: the old py:: calls over cb/formats/xbox/crypto/xecrypt.
#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace xe = codebreaker::formats::xbox::crypto::XeCrypt;

namespace {
py::bytes hexb(const char* h) { return cb::compat::to_py(cb::ByteView(cb::crypto::from_hex(h))); }
std::string hexs(const py::bytes& b) { return cb::crypto::to_hex(cb::as_bytes(b.raw())); }
py::bytes key_file(const char* name) {
    return cb::compat::to_py(cb::ByteView(cb::fs::read_file(cb::testing::data_path(std::string("formats/testdata/xecrypt/") + name))));
}
}  // namespace

CB_TEST("compat.xecrypt.hashes_and_ciphers_with_the_old_calls", [] {
    CB_CHECK_EQ(hexs(xe::XeCryptSha(py::list<py::bytes>{py::bytes("a"), py::bytes("bc")})), "a9993e364706816aba3e25717850c26c9cd0d89d");
    CB_CHECK_EQ(hexs(xe::XeCryptMd5(py::list<py::bytes>{py::bytes("abc")})), "900150983cd24fb0d6963f7d28e17f72");
    CB_CHECK_EQ(hexs(xe::XeCryptHmacSha(py::bytes(std::string(20, '\x0b')), py::list<py::bytes>{py::bytes("Hi There")})), "b617318655057264e28bc0b6fb378c8ef146be00");
    xe::XeCryptRc4 rc4 = xe::XeCryptRc4::new_(py::bytes("Key"));
    xe::XeCryptRc4 copy = rc4;   // (the old class was a value; copies share the cipher)
    CB_CHECK_EQ(hexs(rc4.encrypt(py::bytes("Plaintext"))), "bbf316e8d940af0ad3");
    (void)copy;
    xe::XeCryptAes aes = xe::XeCryptAes::new_(hexb("000102030405060708090a0b0c0d0e0f"));
    CB_CHECK_EQ(hexs(aes.encrypt(hexb("00112233445566778899aabbccddeeff"))), "69c4e0d86a7b0430d8cdb78070b4c55a");
    xe::XeCryptDes3 des3 = xe::XeCryptDes3::new_(py::bytes(std::string(24, 'k')), xe::XeCryptDes3::MODE_CBC, py::bytes::zeros(8));
    CB_CHECK_EQ(des3.encrypt(py::bytes(std::string(16, 'x'))).size(), 16);
    CB_CHECK_THROWS_AS(py::ValueError, (void)xe::XeCryptAes(py::bytes(std::string(10, 'k'))));
    CB_CHECK_THROWS_AS(py::ValueError, (void)xe::XeCryptAes(py::bytes(std::string(16, 'k')), 7));
    CB_CHECK_EQ(xe::XeCryptRotSumSha(py::bytes(std::string(64, 'q'))).size(), 20);
    CB_CHECK(!xe::XeCryptRotSum(py::bytes("short")).has_value());   // None
    CB_CHECK_EQ(xe::XeCryptSmcDecrypt(xe::XeCryptSmcEncrypt(py::bytes("hello smc"))).raw(), "hello smc");
    CB_CHECK_EQ(xe::XeCryptRandom(0x10).size(), 16);
    CB_CHECK(xe::XeCryptCpuKeyValid(xe::XeCryptCpuKeyGen()));
    CB_CHECK_EQ(xe::bswap64(hexb("0102030405060708")).raw(), hexb("0807060504030201").raw());
    CB_CHECK_EQ(xe::XECRYPT_SD_SALT.raw(), "XBOX_ROM_4");
    CB_CHECK_EQ(xe::XECRYPT_KEYVAULT::fields().back().offset + xe::XECRYPT_KEYVAULT::fields().back().size, xe::XECRYPT_KEYVAULT::sizeof_);
});

CB_TEST("compat.xecrypt.the_rsa_key_class_and_the_bootloader_signatures", [] {
    xe::XeCryptRsaKey prv(key_file("rsa2048.prv"));
    CB_CHECK(prv.is_private_key());
    CB_CHECK(prv.verify_parameters());
    CB_CHECK_EQ(prv.n_size_in_bits(), 2048);
    CB_CHECK_EQ(prv.cqw(), 0x20);
    const py::bytes hash = py::bytes(std::string(20, '\x5a'));
    const py::bytes sig = prv.sig_create(hash, xe::XECRYPT_SD_SALT);
    CB_CHECK_EQ(sig.size(), 256);
    CB_CHECK(prv.sig_verify(sig, hash, xe::XECRYPT_SD_SALT));
    CB_CHECK(!prv.sig_verify(sig, hash, xe::XECRYPT_SC_SALT));
    CB_CHECK(xe::XeCryptBnQwBeSigVerify(sig, hash, xe::XECRYPT_SD_SALT, prv.public_key().to_bytes()));
    const std::optional<py::bytes> pkcs1 = prv.sig_create_pkcs1(hash);
    CB_CHECK(pkcs1.has_value());
    CB_CHECK(prv.sig_verify_pkcs1(*pkcs1, hash));
    CB_CHECK(xe::XeKeysPkcs1Verify(*pkcs1, hash, prv.public_key().to_bytes()));
    CB_CHECK_EQ(hexs(*xe::XeKeysPkcs1Create(hash, prv.to_bytes())), hexs(*pkcs1));
    // the two-step form export_keys uses: format+convert, then the private operation
    const py::bytes raw = xe::XeCryptBnQwBeSigCreate(hash, py::bytes("SALTTEST"), prv.to_bytes());
    CB_CHECK_EQ(xe::XeCryptBnQwNeRsaPrvCrypt(raw, prv.to_bytes()).size(), 256);
    auto [pub_key, prv_key] = xe::XeCryptBnQwNeRsaKeyGen(1024);
    CB_CHECK_EQ(pub_key.size(), 0x90);
    CB_CHECK(xe::XeCryptRsaKey(prv_key).verify_parameters());
    CB_CHECK_THROWS_AS(py::ValueError, (void)xe::XeCryptRsaKey(py::bytes("not a key at all")));
    CB_CHECK_THROWS_AS(py::ValueError, (void)xe::XeCryptBnQwNeRsaKeyGen(512));
    std::optional<xe::XeCryptRsaKey> held;   // the callers keep keys in optionals
    held.emplace(key_file("rsa1024.prv"));
    held = xe::XeCryptRsaKey(key_file("rsa2048.prv"));
    held.reset();
});

CB_TEST("compat.xecrypt.keyvaults_and_pages_in_place_as_the_old_calls_did", [] {
    const py::bytes cpu = xe::XeCryptCpuKeyGen();
    py::bytearray vault = py::bytearray::zeros(0x4000);
    const py::bytes sealed = xe::XeCryptKeyVaultEncrypt(cpu, vault);
    CB_CHECK_EQ(sealed.size(), 0x4000);
    CB_CHECK(hexs(vault.view()).substr(0, 32) != std::string(32, '0'));   // the digest was written into the bytearray
    CB_CHECK_EQ(hexs(xe::XeCryptKeyVaultDecrypt(cpu, sealed)), hexs(vault.view()));
    CB_CHECK_THROWS_AS(py::ValueError, (void)xe::XeCryptKeyVaultDecrypt(xe::XeCryptCpuKeyGen(), sealed));
    py::bytearray page = py::bytearray::zeros(0x210);
    const py::bytes ecc = xe::XeCryptPageEccEncode(page);
    CB_CHECK_EQ(hexs(ecc), hexs(page.view()));
    CB_CHECK_EQ(hexs(xe::XeCryptPageEccEncode(py::bytes(std::string(0x210, '\0')))), hexs(ecc));
    py::bytearray swap = py::bytearray(hexb("0102030405060708090a0b0c0d0e0f10"));
    xe::XeCryptBnQwBeBufSwap(swap);
    CB_CHECK_EQ(hexs(swap.view()), "090a0b0c0d0e0f100102030405060708");
    CB_CHECK_EQ(hexs(xe::XeCryptBnQwBeBufSwap(hexb("0102030405060708090a0b0c0d0e0f10"))), "090a0b0c0d0e0f100102030405060708");
});
