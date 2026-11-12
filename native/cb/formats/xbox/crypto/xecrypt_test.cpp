#include "cb/formats/xbox/crypto/xecrypt.hpp"

#include <bit>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::xbox::crypto::xecrypt {
namespace {

using J = nlohmann::json;
namespace stdfs = std::filesystem;

// golden.json is the ORIGINAL Python's (XeCrypt.py) answers; the keys next to it were generated with it. Nothing here signs
// anything real: the keys are test keys.
stdfs::path fixture(std::string_view name) { return testing::data_path(std::string("formats/testdata/xecrypt/") + std::string(name)); }
const J& golden() {
    static const J g = J::parse(fs::read_text(fixture("golden.json")));   // (test data, read once)
    return g;
}
Bytes hex(const J& j) { return cb::crypto::from_hex(j.get<std::string>()); }
Bytes hex(std::string_view s) { return cb::crypto::from_hex(s); }
Bytes hex(const char* s) { return cb::crypto::from_hex(s); }          // (a literal would be ambiguous with a JSON value)
Bytes hex(const std::string& s) { return cb::crypto::from_hex(s); }
std::string hexs(ByteView b) { return cb::crypto::to_hex(b); }
Nat nat(const J& j) { return Nat::from_be(hex(j.get<std::string>().size() % 2 ? "0" + j.get<std::string>() : j.get<std::string>())); }
Bytes key_file(std::string_view name) { return fs::read_file(fixture(name)); }
Bytes concat(std::initializer_list<ByteView> parts) {
    Bytes out;
    for (ByteView p : parts) append(out, p);
    return out;
}

// ---- big numbers, against Python's ints -----------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.xecrypt.nat_arithmetic_agrees_with_python_ints", [] {
    int checked = 0;
    for (const J& c : golden()["nat"]) {
        const Nat a = nat(c["a"]), b = nat(c["b"]), m = nat(c["m"]), e = nat(c["e"]);
        CB_CHECK_EQ((a * b).to_hex(), c["mul"].get<std::string>());
        CB_CHECK_EQ((a + b).to_hex(), c["add"].get<std::string>());
        if (!c["sub"].is_null()) CB_CHECK_EQ((a - b).to_hex(), c["sub"].get<std::string>());
        const Nat::DivMod qr = a.div_mod(b);
        CB_CHECK_EQ(qr.quotient.to_hex(), c["div"].get<std::string>());
        CB_CHECK_EQ(qr.remainder.to_hex(), c["mod"].get<std::string>());
        CB_CHECK_EQ(Nat::pow_mod(a, e, m).to_hex(), c["pow"].get<std::string>());
        CB_CHECK_EQ(Nat::gcd(a, b).to_hex(), c["gcd"].get<std::string>());
        const std::optional<Nat> inv = Nat::inv_mod(a, m);
        CB_CHECK_EQ(inv.has_value(), !c["inv"].is_null());
        if (inv && !c["inv"].is_null()) CB_CHECK_EQ(inv->to_hex(), c["inv"].get<std::string>());
        ++checked;
    }
    CB_CHECK_EQ(checked, 120);
});

CB_TEST("formats.xbox.crypto.xecrypt.nat_bytes_shifts_and_edges", [] {
    CB_CHECK(Nat().is_zero());
    CB_CHECK_EQ(Nat().to_hex(), "0");
    CB_CHECK_EQ(Nat(0x1234).to_be().size(), 2u);
    CB_CHECK(Nat().to_be().empty());
    CB_CHECK(Nat::from_be(hex("0000000000ff")) == Nat(255));
    CB_CHECK(Nat::from_le(hex("ff0000")) == Nat(255));
    CB_CHECK_EQ(hexs(Nat(0x0102).to_be(4)), "00000102");
    CB_CHECK_EQ(hexs(Nat(0x0102).to_le(4)), "02010000");
    CB_CHECK_THROWS_AS(UsageError, (void)Nat(0x10000).to_be(2));
    CB_CHECK_EQ(Nat(1).shifted_left(100).bit_length(), 101u);
    CB_CHECK(Nat(1).shifted_left(100).shifted_right(100) == Nat(1));
    CB_CHECK(Nat(5).shifted_right(3).is_zero());
    CB_CHECK_EQ(Nat(0xFFFFFFFFFFFFFFFFull).low64(), 0xFFFFFFFFFFFFFFFFull);
    CB_CHECK(Nat(10) < Nat(11) && Nat(1).shifted_left(64) > Nat(0xFFFFFFFFFFFFFFFFull));
    CB_CHECK_THROWS_AS(UsageError, (void)(Nat(3) - Nat(4)));
    CB_CHECK_THROWS_AS(UsageError, (void)Nat(3).div_mod(Nat(0)));
    CB_CHECK_THROWS_AS(UsageError, (void)Nat::pow_mod(Nat(2), Nat(3), Nat(0)));
    CB_CHECK(Nat::pow_mod(Nat(7), Nat(0), Nat(5)) == Nat(1));
    CB_CHECK(Nat::pow_mod(Nat(7), Nat(0), Nat(1)).is_zero());
    CB_CHECK(!Nat::inv_mod(Nat(4), Nat(8)).has_value());   // not coprime
    CB_CHECK(Nat::inv_mod(Nat(3), Nat(7)).value() == Nat(5));
    CB_CHECK(!Nat::inv_mod(Nat(3), Nat(1)).has_value());
});

CB_TEST("formats.xbox.crypto.xecrypt.primality_and_prime_generation", [] {
    SeededRandom rng(7);
    for (const J& c : golden()["primes"]) CB_CHECK_EQ(Nat(c[0].get<std::uint64_t>()).is_probable_prime(rng), c[1].get<bool>());
    CB_CHECK(Nat::from_be(hex("ffffffff00000001000000000000000000000000ffffffffffffffffffffffff")).is_probable_prime(rng));   // the P-256 prime
    CB_CHECK(!Nat::from_be(hex("ffffffff00000001000000000000000000000000fffffffffffffffffffffffd")).is_probable_prime(rng));
    for (std::size_t bits : {std::size_t{32}, std::size_t{64}, std::size_t{256}}) {
        const Nat p = Nat::random_prime(bits, rng);
        CB_CHECK_EQ(p.bit_length(), bits);
        CB_CHECK(p.bit(bits - 1) && p.bit(bits - 2));
        CB_CHECK(p.is_odd());
        CB_CHECK(p.is_probable_prime(rng, 40));
    }
    SeededRandom same_a(99), same_b(99);
    CB_CHECK(Nat::random_prime(128, same_a) == Nat::random_prime(128, same_b));   // a seeded run repeats
    CB_CHECK_THROWS_AS(UsageError, (void)Nat::random_prime(8, rng));
    CB_CHECK_THROWS_AS(UsageError, (void)Nat::random_below(Nat(0), rng));
});

// ---- hashes, MACs and ciphers ---------------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.xecrypt.hashes_and_macs", [] {
    CB_CHECK_EQ(hexs(md5(as_bytes("abc"))), "900150983cd24fb0d6963f7d28e17f72");   // RFC 1321
    CB_CHECK_EQ(hexs(sha(as_bytes("abc"))), "a9993e364706816aba3e25717850c26c9cd0d89d");   // FIPS 180
    CB_CHECK_EQ(hexs(hmac_sha(Bytes(20, 0x0b), as_bytes("Hi There"))), "b617318655057264e28bc0b6fb378c8ef146be00");   // RFC 2202 case 1
    CB_CHECK_EQ(hexs(hmac_md5(Bytes(16, 0x0b), as_bytes("Hi There"))), "9294727a3638bb1c13f48ef8158bfc9d");
    for (const J& c : golden()["hash"]) {
        CB_CHECK_EQ(hexs(md5(hex(c["data"]))), c["md5"].get<std::string>());
        CB_CHECK_EQ(hexs(sha(hex(c["data"]))), c["sha"].get<std::string>());
    }
    const J& p = golden()["hash_parts"];
    const Bytes a = hex(p["parts"][0]), b = hex(p["parts"][1]), c = hex(p["parts"][2]);
    CB_CHECK_EQ(hexs(md5({a, b, c})), p["md5"].get<std::string>());
    CB_CHECK_EQ(hexs(sha({a, b, c})), p["sha"].get<std::string>());
    for (const J& h : golden()["hmac"]) {
        const Bytes key = hex(h["key"]), p1 = hex(h["parts"][0]), p2 = hex(h["parts"][1]);
        CB_CHECK_EQ(hexs(hmac_md5(key, {p1, p2})), h["md5"].get<std::string>());
        CB_CHECK_EQ(hexs(hmac_sha(key, {p1, p2})), h["sha"].get<std::string>());
        CB_CHECK_EQ(hexs(hmac_sha(key, {p1, p2})), hexs(hmac_sha(key, concat({p1, p2}))));   // the parts are as if joined
    }
});

CB_TEST("formats.xbox.crypto.xecrypt.rc4", [] {
    CB_CHECK_EQ(hexs(rc4(as_bytes("Key"), as_bytes("Plaintext"))), "bbf316e8d940af0ad3");
    for (const J& c : golden()["rc4"]) {
        const Bytes key = hex(c["key"]), data = hex(c["data"]);
        Rc4 one(key);
        const std::size_t split = c["split"].get<std::size_t>();
        Bytes got = one.encrypt(ByteView(data).first(split));
        append(got, one.encrypt(ByteView(data).subspan(split)));   // the keystream continues across calls
        CB_CHECK_EQ(hexs(got), c["out"].get<std::string>());
        CB_CHECK_EQ(hexs(Rc4(key).decrypt(data)), c["dec"].get<std::string>());
        CB_CHECK_EQ(hexs(rc4(key, data)), c["out"].get<std::string>());
        // the two directions are separate streams: decrypting what was encrypted needs a fresh decrypt side
        Rc4 pair(key);
        const Bytes enc = pair.encrypt(data);
        CB_CHECK(equal_bytes(pair.decrypt(enc), data));
    }
    CB_CHECK_THROWS_AS(UsageError, (void)Rc4(Bytes{}));
    CB_CHECK_THROWS_AS(UsageError, (void)Rc4(Bytes(257, 1)));
});

CB_TEST("formats.xbox.crypto.xecrypt.aes_and_des", [] {
    // FIPS-197 appendix C.1 and the classic DES example
    BlockCipher aes(CipherAlgorithm::Aes, hex("000102030405060708090a0b0c0d0e0f"));
    CB_CHECK_EQ(hexs(aes.encrypt(hex("00112233445566778899aabbccddeeff"))), "69c4e0d86a7b0430d8cdb78070b4c55a");
    CB_CHECK_EQ(hexs(aes.decrypt(hex("69c4e0d86a7b0430d8cdb78070b4c55a"))), "00112233445566778899aabbccddeeff");
    BlockCipher des(CipherAlgorithm::Des, hex("133457799bbcdff1"));
    CB_CHECK_EQ(hexs(des.encrypt(hex("0123456789abcdef"))), "85e813540f0ab405");
    for (const J& c : golden()["ciphers"]) {
        const std::string alg = c["alg"];
        const CipherAlgorithm a = alg == "XeCryptAes" ? CipherAlgorithm::Aes : alg == "XeCryptDes" ? CipherAlgorithm::Des : alg == "XeCryptDes2" ? CipherAlgorithm::Des2 : CipherAlgorithm::Des3;
        const CipherMode mode = c["mode"].get<int>() == 2 ? CipherMode::Cbc : CipherMode::Ecb;
        const Bytes key = hex(c["key"]), iv = hex(c["iv"]), data = hex(c["data"]);
        const std::size_t split = c["split"].get<std::size_t>();
        BlockCipher enc(a, key, mode, mode == CipherMode::Cbc ? ByteView(iv) : ByteView());
        Bytes got = enc.encrypt(ByteView(data).first(split));
        append(got, enc.encrypt(ByteView(data).subspan(split)));   // CBC chains across the calls
        CB_CHECK_EQ(hexs(got), c["enc"].get<std::string>());
        BlockCipher dec(a, key, mode, mode == CipherMode::Cbc ? ByteView(iv) : ByteView());
        CB_CHECK_EQ(hexs(dec.decrypt(got)), hexs(data));
    }
    CB_CHECK_THROWS_AS(UsageError, (void)BlockCipher(CipherAlgorithm::Aes, Bytes(24, 1)));
    CB_CHECK_THROWS_AS(UsageError, (void)BlockCipher(CipherAlgorithm::Des, Bytes(7, 1)));
    CB_CHECK_THROWS_AS(UsageError, (void)BlockCipher(CipherAlgorithm::Des3, Bytes(16, 1)));
    CB_CHECK_THROWS_AS(UsageError, (void)BlockCipher(CipherAlgorithm::Aes, Bytes(16, 1), CipherMode::Cbc));   // no IV
    CB_CHECK_THROWS_AS(UsageError, (void)BlockCipher(CipherAlgorithm::Aes, Bytes(16, 1), CipherMode::Cbc, Bytes(8, 0)));
    // a partial block is held back until the next call completes it
    BlockCipher stream(CipherAlgorithm::Aes, hex("000102030405060708090a0b0c0d0e0f"));
    CB_CHECK(stream.encrypt(hex("00112233445566778899aabbccddee")).empty());
    CB_CHECK_EQ(stream.pending_encrypt(), 15u);
    CB_CHECK_EQ(hexs(stream.encrypt(hex("ff"))), "69c4e0d86a7b0430d8cdb78070b4c55a");
    CB_CHECK_EQ(stream.pending_encrypt(), 0u);
    CB_CHECK_EQ(stream.pending_decrypt(), 0u);
    CB_CHECK_EQ(hexs(stream.decrypt(hex("69c4e0d86a7b0430d8cdb78070b4c55a00"))), "00112233445566778899aabbccddeeff");
    CB_CHECK_EQ(stream.pending_decrypt(), 1u);
    CB_CHECK_EQ(aes.block_size(), 16u);
    CB_CHECK_EQ(des.block_size(), 8u);
    CB_CHECK(aes.encrypt(Bytes{}).empty());
    for (const J& c : golden()["des_parity"]) CB_CHECK_EQ(hexs(des_parity(hex(c["in"]))), c["out"].get<std::string>());
    Bytes every(256);   // every byte comes out with odd parity and its top seven bits
    for (std::size_t i = 0; i < 256; ++i) every[i] = static_cast<std::uint8_t>(i);
    const Bytes fixed = des_parity(every);
    for (std::size_t i = 0; i < 256; ++i) {
        CB_CHECK_EQ(std::popcount(static_cast<unsigned>(fixed[i])) % 2, 1);
        CB_CHECK_EQ(fixed[i] & 0xFE, every[i] & 0xFE);
    }
});

// ---- the console's MACs and checksums -------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.xecrypt.parve_chain_and_sum_and_rot_sum", [] {
    const J& p = golden()["parve"];
    const Bytes key = hex(p["key"]), sbox = hex(p["sbox"]);
    for (const J& c : p["ecb"]) CB_CHECK_EQ(hexs(parve_ecb(key, sbox, hex(c[0]))), c[1].get<std::string>());
    for (const J& c : p["cbc"]) CB_CHECK_EQ(hexs(parve_cbc_mac(key, sbox, hex(c[0]), hex(c[1]))), c[2].get<std::string>());
    for (const J& c : golden()["chainsum"]) CB_CHECK_EQ(hexs(chain_and_sum_mac(hex(c[0]), hex(c[1]), hex(c[2]))), c[3].get<std::string>());
    for (const J& c : golden()["rotsum"]) {
        CB_CHECK_EQ(hexs(rot_sum(hex(c[0]))), c[1].get<std::string>());
        CB_CHECK_EQ(hexs(rot_sum_sha(hex(c[0]))), c[2].get<std::string>());
    }
    CB_CHECK_EQ(hexs(rot_sum(Bytes(7, 0xFF))), hexs(Bytes(32, 0)));   // no whole qword: nothing summed
    CB_CHECK_THROWS_AS(UsageError, (void)parve_ecb(Bytes(7, 0), sbox, Bytes(8, 0)));
    CB_CHECK_THROWS_AS(UsageError, (void)parve_ecb(key, Bytes(255, 0), Bytes(8, 0)));
    CB_CHECK_THROWS_AS(UsageError, (void)parve_ecb(key, sbox, Bytes(7, 0)));
    CB_CHECK_THROWS_AS(UsageError, (void)parve_cbc_mac(key, sbox, Bytes(8, 0), Bytes(9, 0)));
    CB_CHECK_THROWS_AS(UsageError, (void)chain_and_sum_mac(Bytes(8, 0), Bytes(8, 0), Bytes(12, 0)));
});

CB_TEST("formats.xbox.crypto.xecrypt.smc_ecc_and_cpu_keys", [] {
    for (const J& c : golden()["smc"]) {
        CB_CHECK_EQ(hexs(smc_encrypt(hex(c[0]))), c[1].get<std::string>());
        CB_CHECK_EQ(hexs(smc_decrypt(hex(c[0]))), c[2].get<std::string>());
        CB_CHECK(equal_bytes(smc_decrypt(smc_encrypt(hex(c[0]))), hex(c[0])));
    }
    for (const J& c : golden()["hamming"]) CB_CHECK_EQ(hamming_weight(hex(c[0])), c[1].get<std::size_t>());
    for (const J& c : golden()["uid_ecc"]) CB_CHECK_EQ(hexs(uid_ecc_encode(hex(c[0]))), c[1].get<std::string>());
    CB_CHECK_THROWS_AS(UsageError, (void)uid_ecc_encode(Bytes(15, 0)));
    for (const J& c : golden()["cpu_key"]) CB_CHECK_EQ(cpu_key_valid(hex(c[0])), c[1].get<bool>());
    for (std::uint64_t seed = 1; seed <= 20; ++seed) {
        SeededRandom rng(seed);
        const Bytes k = cpu_key_generate(rng);
        CB_CHECK_EQ(k.size(), 16u);
        CB_CHECK(cpu_key_valid(k));
        SeededRandom again(seed);
        CB_CHECK(equal_bytes(k, cpu_key_generate(again)));
    }
    CB_CHECK(cpu_key_valid(cpu_key_generate()));   // the OS generator
});

CB_TEST("formats.xbox.crypto.xecrypt.page_ecc", [] {
    for (const J& c : golden()["page_ecc"]) {
        CB_CHECK_EQ(hexs(page_ecc_encode(hex(c[0]))), c[1].get<std::string>());
        Bytes page = hex(c[0]);
        page_ecc_encode_in_place(page);   // the same, in place
        CB_CHECK_EQ(hexs(page), c[1].get<std::string>());
    }
    CB_CHECK_THROWS_AS(UsageError, (void)page_ecc_encode(Bytes(0x20F, 0)));
});

CB_TEST("formats.xbox.crypto.xecrypt.byte_order_helpers", [] {
    CB_CHECK_EQ(hexs(reversed(hex("010203"))), "030201");
    CB_CHECK_EQ(hexs(swap_words(hex("01020304"))), "02010403");
    CB_CHECK_EQ(hexs(swap_dwords(hex("0102030405060708"))), "0403020108070605");
    CB_CHECK_EQ(hexs(swap_qwords(hex("0102030405060708090a0b0c0d0e0f10"))), "0807060504030201100f0e0d0c0b0a09");
    CB_CHECK_THROWS_AS(UsageError, (void)swap_qwords(Bytes(9, 0)));
    // a BnQw: qwords big-endian, least significant first
    CB_CHECK(nat_from_qwords(hex("00000000000000010000000000000002")) == Nat(1) + Nat(2).shifted_left(64));
    CB_CHECK_EQ(hexs(nat_to_qwords(Nat(1) + Nat(2).shifted_left(64), 16)), "00000000000000010000000000000002");
    CB_CHECK_THROWS_AS(UsageError, (void)nat_to_qwords(Nat(1), 12));
    CB_CHECK_THROWS_AS(UsageError, (void)nat_to_qwords(Nat(1).shifted_left(64), 8));
});

// ---- RSA ------------------------------------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.xecrypt.keys_parse_and_agree_with_the_python", [] {
    for (const auto& [bits_text, want] : golden()["keys"].items()) {
        const unsigned bits = static_cast<unsigned>(std::stoul(bits_text));
        const Bytes prv_bytes = key_file("rsa" + bits_text + ".prv"), pub_bytes = key_file("rsa" + bits_text + ".pub");
        const RsaKey prv = RsaKey::parse(prv_bytes), pub = RsaKey::parse(pub_bytes);
        CB_CHECK(prv.is_private() && !pub.is_private());
        CB_CHECK_EQ(prv.bits(), bits);
        CB_CHECK_EQ(prv.qword_count(), want["cqw"].get<std::size_t>());
        CB_CHECK_EQ(prv.exponent(), want["e"].get<std::uint32_t>());
        CB_CHECK_EQ(prv.n().to_hex(), want["n"].get<std::string>());
        CB_CHECK_EQ(prv.d().to_hex(), want["d"].get<std::string>());
        CB_CHECK_EQ(prv.montgomery_constant(), want["mod_inv"].get<std::uint64_t>());
        CB_CHECK(pub.n() == prv.n());
        CB_CHECK(equal_bytes(prv.public_key().bytes(), pub_bytes));
        CB_CHECK(equal_bytes(pub.public_key().bytes(), pub_bytes));   // a public key is its own public half
        CB_CHECK(want["pub_matches_prv_head"].get<bool>());
        if (!want["r"].is_null()) CB_CHECK_EQ(prv.r().to_hex(), want["r"].get<std::string>());
        // the raw operations
        const Bytes m = hex(want["prv_crypt"][0]);
        CB_CHECK_EQ(hexs(prv.prv_crypt(m)), want["prv_crypt"][1].get<std::string>());
        CB_CHECK(equal_bytes(pub.pub_crypt(prv.prv_crypt(m)), m));   // public undoes private
        if (!want["pub_crypt"].is_null()) CB_CHECK_EQ(hexs(pub.pub_crypt(m)), want["pub_crypt"][1].get<std::string>());   // (the Python could not open a public 1536 key)
        // PKCS#1
        const Bytes h = sha(as_bytes("keys"));
        CB_CHECK_EQ(hexs(prv.sign_pkcs1(h)), want["pkcs1"][0].get<std::string>());
        CB_CHECK(pub.verify_pkcs1(hex(want["pkcs1"][0]), h));
        Bytes wrong = h;
        std::reverse(wrong.begin(), wrong.end());
        CB_CHECK(!pub.verify_pkcs1(hex(want["pkcs1"][0]), wrong));
        CB_CHECK(!pub.verify_pkcs1(Bytes(8, 0), h));   // the wrong size
        CB_CHECK_THROWS_AS(UsageError, (void)pub.prv_crypt(m));
        CB_CHECK_THROWS_AS(UsageError, (void)pub.sign_pkcs1(h));
        CB_CHECK_THROWS_AS(UsageError, (void)pub.d());
    }
});

CB_TEST("formats.xbox.crypto.xecrypt.the_bootloader_signature_scheme_as_the_python_made_it", [] {
    const RsaKey prv = RsaKey::parse(key_file("rsa2048.prv")), pub = prv.public_key();
    for (const J& c : golden()["sig"]) {
        const Bytes hash = hex(c["hash"]), salt = hex(c["salt"]);
        CB_CHECK_EQ(hexs(sig_format(0x20, hash, salt)), c["format"].get<std::string>());
        CB_CHECK_EQ(hexs(sig_create(hash, salt, prv)), c["create"].get<std::string>());
        const Bytes signature = prv.sign(hash, salt);
        CB_CHECK_EQ(hexs(signature), c["sign"].get<std::string>());
        CB_CHECK_EQ(pub.verify(signature, hash, salt), c["verify"].get<bool>());
        CB_CHECK(pub.verify(hex(c["sign"]), hash, salt));
        if (!c["verify_wrong_salt"].is_null()) CB_CHECK_EQ(pub.verify(signature, hash, concat({salt, as_bytes("x")})), c["verify_wrong_salt"].get<bool>());
        Bytes wrong = hash;
        std::reverse(wrong.begin(), wrong.end());
        CB_CHECK_EQ(pub.verify(signature, wrong, salt), c["verify_wrong_hash"].get<bool>());
    }
    // the free functions and the key's are the same thing
    const Bytes hash = hex(golden()["sig"][1]["hash"]);
    CB_CHECK(sig_verify(prv.sign(hash, as_bytes("SALT")), hash, as_bytes("SALT"), pub));
    Bytes damaged = prv.sign(hash, as_bytes("SALT"));
    damaged[17] ^= 1;
    CB_CHECK(!sig_verify(damaged, hash, as_bytes("SALT"), pub));
    CB_CHECK(!sig_verify(Bytes(0x80, 0), hash, as_bytes("SALT"), pub));   // the wrong size
    // misuse
    CB_CHECK_THROWS_AS(UsageError, (void)sig_format(0x10, hash, as_bytes("S")));
    CB_CHECK_THROWS_AS(UsageError, (void)sig_format(0x20, hash, as_bytes("0123456789a")));   // salt over 10 bytes
    CB_CHECK_THROWS_AS(UsageError, (void)sig_create(hash, as_bytes("0123456789a"), prv));
    CB_CHECK_THROWS_AS(UsageError, (void)sig_create(hash, {}, RsaKey::parse(key_file("rsa1024.prv"))));   // only 2048-bit keys
    CB_CHECK_THROWS_AS(UsageError, (void)sig_verify(Bytes(0x100, 0), hash, {}, RsaKey::parse(key_file("rsa4096.pub"))));
});

CB_TEST("formats.xbox.crypto.xecrypt.pkcs1_blocks", [] {
    const Bytes hash = sha(as_bytes("keys"));
    for (const J& c : golden()["pkcs1_format"]) {
        const Bytes block = pkcs1_format(hash, c[0].get<int>(), c[1].get<std::size_t>());
        CB_CHECK_EQ(hexs(block), c[2].get<std::string>());
        CB_CHECK(pkcs1_verify(ByteView(block).first(c[1].get<std::size_t>()), hash, c[1].get<std::size_t>()));
    }
    CB_CHECK_THROWS_AS(UsageError, (void)pkcs1_format(hash, 0, 0x26));
    CB_CHECK_THROWS_AS(UsageError, (void)pkcs1_format(hash, 0, 0x201));
    CB_CHECK_THROWS_AS(UsageError, (void)pkcs1_format(Bytes(19, 0), 0, 0x80));
    CB_CHECK(!pkcs1_verify(Bytes(0x26, 0), hash, 0x26));
    Bytes block = pkcs1_format(hash, 0, 0x80);
    block.resize(0x80);
    block[3] ^= 0x40;
    CB_CHECK(!pkcs1_verify(block, hash, 0x80));
});

CB_TEST("formats.xbox.crypto.xecrypt.damaged_and_odd_keys_are_refused", [] {
    const Bytes good = key_file("rsa1024.prv");
    CB_CHECK_THROWS_AS(TruncatedError, (void)RsaKey::parse(Bytes(8, 0)));                      // not even a header
    CB_CHECK_THROWS_AS(FormatError, (void)RsaKey::parse(Bytes(0x200, 0)));                     // no modulus size
    CB_CHECK_THROWS_AS(FormatError, (void)RsaKey::parse(ByteView(good).first(0x1D0 - 1)));     // cut short
    Bytes odd_size = good;
    odd_size[3] = 0x11;   // 17 qwords
    CB_CHECK_THROWS_AS(FormatError, (void)RsaKey::parse(odd_size));
    Bytes flipped = good;
    flipped[16 + 5] ^= 1;   // the modulus no longer is p * q
    CB_CHECK_THROWS_AS(FormatError, (void)RsaKey::parse(flipped));
    Bytes even_e = good;
    even_e[7] = 4;
    CB_CHECK_THROWS_AS(FormatError, (void)RsaKey::parse(even_e));
    Bytes small_e = good;
    small_e[4] = small_e[5] = small_e[6] = 0;
    small_e[7] = 1;
    CB_CHECK_THROWS_AS(FormatError, (void)RsaKey::parse(small_e));
    Bytes bad_dp = good;
    bad_dp[16 + 128 + 64 + 64 + 3] ^= 2;   // dp
    CB_CHECK_THROWS_AS(FormatError, (void)RsaKey::parse(bad_dp));
    Bytes bad_u = good;
    bad_u[0x1D0 - 3] ^= 4;   // the coefficient
    CB_CHECK_THROWS_AS(FormatError, (void)RsaKey::parse(bad_u));
    // trailing bytes after a key are tolerated (the old reader did), the key is the leading part
    Bytes longer = good;
    longer.resize(good.size() + 16, 0);
    CB_CHECK_NOTHROW((void)RsaKey::parse(longer));
});

CB_TEST("formats.xbox.crypto.xecrypt.generated_keys_are_valid_and_seeded_runs_repeat", [] {
    SeededRandom rng(2026);
    const KeyPair pair = generate_key_pair(1024, 0x10001, rng);
    CB_CHECK_EQ(pair.private_key.size(), kRsaPrv1024Size);
    CB_CHECK_EQ(pair.public_key.size(), kRsaPub1024Size);
    CB_CHECK(equal_bytes(pair.public_key, ByteView(pair.private_key).first(kRsaPub1024Size)));
    const RsaKey prv = RsaKey::parse(pair.private_key);   // (parse proves n = p*q, dp, dq, the coefficient)
    CB_CHECK_EQ(prv.exponent(), 0x10001u);
    CB_CHECK_EQ(prv.n().bit_length(), 1024u);
    Bytes m(128, 0);
    for (std::size_t i = 0; i < 120; ++i) m[i] = static_cast<std::uint8_t>(i * 3 + 1);
    CB_CHECK(equal_bytes(prv.public_key().pub_crypt(prv.prv_crypt(m)), m));
    CB_CHECK(equal_bytes(prv.prv_crypt(prv.public_key().pub_crypt(m)), m));
    SeededRandom again(2026);
    CB_CHECK(equal_bytes(generate_key_pair(1024, 0x10001, again).private_key, pair.private_key));
    // exponent 3
    SeededRandom rng3(5);
    const RsaKey e3 = RsaKey::generate(1024, 3, rng3);
    CB_CHECK_EQ(e3.exponent(), 3u);
    CB_CHECK(equal_bytes(e3.public_key().pub_crypt(e3.prv_crypt(m)), m));
    CB_CHECK_THROWS_AS(UsageError, (void)RsaKey::generate(512));
    CB_CHECK_THROWS_AS(UsageError, (void)RsaKey::generate(1024, 5));
    // a 2048-bit key signs and verifies with the bootloader scheme
    SeededRandom rng2(11);
    const RsaKey k2048 = RsaKey::generate(2048, 0x10001, rng2);
    const Bytes hash = sha(as_bytes("boot"));
    CB_CHECK(k2048.public_key().verify(k2048.sign(hash, as_bytes(kSaltSd)), hash, as_bytes(kSaltSd)));
    CB_CHECK(!k2048.public_key().verify(k2048.sign(hash, as_bytes(kSaltSd)), hash, as_bytes(kSaltSc)));
    // the OS-random path
    for (unsigned bits : {1024u, 1536u, 2048u}) {   // (parse() inside generate() proves the key hangs together)
        const RsaKey k = RsaKey::generate(bits);
        CB_CHECK_EQ(k.bits(), bits);
        CB_CHECK_EQ(k.exponent(), 0x10001u);
        CB_CHECK(k.is_private());
    }
});

CB_TEST("formats.xbox.crypto.xecrypt.montgomery_inverse", [] {
    const Nat n = RsaKey::parse(key_file("rsa2048.prv")).n();
    const Nat inv = montgomery_inverse(n);
    CB_CHECK(((inv * Nat(1).shifted_left(64)) % n) == Nat(1));
    CB_CHECK_THROWS_AS(UsageError, (void)montgomery_inverse(Nat(10)));
    CB_CHECK_THROWS_AS(UsageError, (void)montgomery_inverse(Nat(0)));
});

// ---- keyvaults ------------------------------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.xecrypt.keyvaults_as_the_python_sealed_them", [] {
    const J& g = golden()["keyvault"];
    const Bytes cpu = hex(g["cpu_key"]), enc = hex(g["enc"]), plain = hex(g["plain"]);
    const Result<Bytes> opened = keyvault_decrypt(cpu, enc);
    CB_CHECK(opened.ok());
    CB_CHECK(equal_bytes(opened.value(), plain));
    // sealing again: a new obfuscation key and digest, and the result opens
    SeededRandom rng(4);
    Bytes vault = plain;
    const Bytes sealed = keyvault_encrypt(cpu, vault, rng);
    CB_CHECK_EQ(sealed.size(), kKeyvaultSize);
    CB_CHECK(!equal_bytes(sealed, enc));   // (a different obfuscation key)
    CB_CHECK(equal_bytes(ByteView(vault).subspan(0x18), ByteView(plain).subspan(0x18)));   // the rest of the vault is untouched
    CB_CHECK(keyvault_decrypt(cpu, sealed).ok());
    CB_CHECK(equal_bytes(keyvault_decrypt(cpu, sealed).value(), vault));
    SeededRandom same(4);
    Bytes vault2 = plain;
    CB_CHECK(equal_bytes(keyvault_encrypt(cpu, vault2, same), sealed));   // seeded runs repeat
    // wrong key, damage, misuse
    SeededRandom other(9);
    const Bytes wrong_key = cpu_key_generate(other);
    const Result<Bytes> refused = keyvault_decrypt(wrong_key, enc);
    CB_CHECK(!refused.ok());
    CB_CHECK(dynamic_cast<const FormatError*>(&refused.error()) != nullptr);
    Bytes damaged = enc;
    damaged[0x2000] ^= 1;
    CB_CHECK(!keyvault_decrypt(cpu, damaged).ok());
    CB_CHECK_THROWS_AS(UsageError, (void)keyvault_decrypt(Bytes(16, 1), enc));      // not a CPU key
    CB_CHECK_THROWS_AS(UsageError, (void)keyvault_decrypt(cpu, Bytes(8, 0)));
    Bytes tiny(0x10, 0);
    CB_CHECK_THROWS_AS(UsageError, (void)keyvault_encrypt(cpu, tiny, rng));
    Bytes any(0x40, 0);
    CB_CHECK_THROWS_AS(UsageError, (void)keyvault_encrypt(Bytes(16, 1), any, rng));
});

CB_TEST("formats.xbox.crypto.xecrypt.keyvault_certificate_verification", [] {
    const J& g = golden()["keyvault"];
    const Bytes cpu = hex(g["cpu_key"]);
    const RsaKey pub = RsaKey::parse(key_file("rsa2048.pub"));
    CB_CHECK_EQ(keyvault_verify(cpu, hex(g["signed"]), pub), g["verify"].get<bool>());
    CB_CHECK(keyvault_verify(cpu, hex(g["signed"]), pub));
    CB_CHECK_EQ(keyvault_verify(cpu, hex(g["bad"]), pub), g["bad_verify"].get<bool>());
    CB_CHECK(!keyvault_verify(cpu, hex(g["bad"]), pub));
    CB_CHECK(!keyvault_verify(cpu, hex(g["signed"]), RsaKey::parse(key_file("rsa1024.pub"))));   // another key does not verify it
    CB_CHECK_THROWS_AS(UsageError, (void)keyvault_verify(cpu, Bytes(0x3000, 0), pub));
    CB_CHECK_THROWS_AS(UsageError, (void)keyvault_verify(Bytes(16, 0), hex(g["signed"]), pub));
});

// ---- the key store, randomness --------------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.xecrypt.the_key_store_and_random_sources", [] {
    fs::ScopedTempDir dir("xecrypt");
    fs::write_file(dir.path() / "1BL_key.bin", hex("00112233"));
    CB_CHECK_EQ(hexs(*load_store_key(dir.path(), "1BL_key.bin")), "00112233");
    CB_CHECK(!load_store_key(dir.path(), "missing.bin").has_value());
    CB_CHECK(!load_store_key(dir.path() / "no-such-folder", "1BL_key.bin").has_value());
    SeededRandom a(1), b(1), c(2);
    const Bytes x = a.bytes(37);
    CB_CHECK_EQ(x.size(), 37u);
    CB_CHECK(equal_bytes(x, b.bytes(37)));
    CB_CHECK(!equal_bytes(x, c.bytes(37)));
    CB_CHECK(!equal_bytes(os_random().bytes(32), os_random().bytes(32)));
    CB_CHECK(os_random().bytes(0).empty());
    CB_CHECK_EQ(kSaltSd, "XBOX_ROM_4");
    CB_CHECK_EQ(kSalt1bl.size(), 10u);
    // the keyvault layout adds up to the vault
    const std::span<const KeyvaultField> layout = keyvault_layout();
    CB_CHECK_EQ(layout.front().name, "nonce");
    CB_CHECK_EQ(layout.back().name, "cardea_certificate");
    CB_CHECK_EQ(layout.back().offset + layout.back().size, kKeyvaultSize);
    std::size_t expected = 0;
    for (const KeyvaultField& f : layout) {
        CB_CHECK_EQ(f.offset, expected);
        expected += f.size;
    }
});

}  // namespace
}  // namespace cb::formats::xbox::crypto::xecrypt
