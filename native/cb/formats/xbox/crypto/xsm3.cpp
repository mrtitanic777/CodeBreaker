#include "cb/formats/xbox/crypto/xsm3.hpp"

#include <algorithm>
#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/xbox/crypto/keyvault.hpp"

namespace cb::formats::xbox::crypto::xsm3 {

namespace {

#include "cb/formats/xbox/crypto/xsm3_tables.inc"

constexpr std::size_t kIdentificationSize = 0x1D;
constexpr std::size_t kChallengeSize = 0x22;
constexpr std::size_t kVerifySize = 0x16;
constexpr std::size_t kHead = 5;
constexpr std::size_t kPlainTextUsed = 0x80;
constexpr std::uint8_t kResponseMagic0 = 0x49;
constexpr std::uint8_t kResponseMagic1 = 0x4C;

// The key as 3DES wants it: the key twice, the first 24 bytes of that.
Bytes triple_key(ByteView key) {
    Bytes doubled(key.begin(), key.end());
    doubled.insert(doubled.end(), key.begin(), key.end());
    doubled.resize(24);
    return doubled;
}

std::uint64_t load_be64(ByteView data) {
    std::uint64_t v = 0;
    for (const std::uint8_t b : data) v = (v << 8) | b;
    return v;   // a short block counts as the number it spells
}

Bytes store_be64(std::uint64_t v) {
    Bytes out(8);
    for (std::size_t i = 0; i < 8; ++i) out[i] = static_cast<std::uint8_t>(v >> (56 - 8 * i));
    return out;
}

Bytes slice(ByteView data, std::size_t at, std::size_t size) { return Bytes(data.begin() + static_cast<std::ptrdiff_t>(at), data.begin() + static_cast<std::ptrdiff_t>(at + size)); }

void require_size(ByteView packet, std::size_t size, std::string_view what) {
    if (packet.size() != size) throw UsageError(std::format("a {} packet is 0x{:X} bytes", what, size));
}

Bytes empty_response(std::uint8_t length) {
    Bytes response(0x30, 0);
    response[0] = kResponseMagic0;
    response[1] = kResponseMagic1;
    response[4] = length;
    return response;
}

}  // namespace

std::uint8_t packet_checksum(ByteView packet) {
    if (packet.size() <= 4) return 0;
    const std::size_t end = std::min<std::size_t>(packet.size(), std::size_t{packet[4]} + kHead);
    std::uint8_t sum = 0;
    for (std::size_t i = kHead; i < end; ++i) sum ^= packet[i];
    return sum;
}

bool packet_checksum_ok(ByteView packet) {
    if (packet.size() <= 4) return false;
    const std::size_t at = std::size_t{packet[4]} + kHead;
    return at < packet.size() && packet_checksum(packet) == packet[at];
}

Bytes crypt(ByteView key, ByteView data, CryptMode mode) {
    xecrypt::BlockCipher cipher(xecrypt::CipherAlgorithm::Des3, triple_key(key), xecrypt::CipherMode::Cbc, Bytes(8, 0));
    return mode == CryptMode::Decrypt ? cipher.decrypt(data) : cipher.encrypt(data);
}

Mac mac(ByteView key, std::optional<ByteView> salt, ByteView data) {
    xecrypt::BlockCipher des(xecrypt::CipherAlgorithm::Des, key.first(8), xecrypt::CipherMode::Ecb);
    Mac out;
    Bytes running;
    if (salt && !salt->empty()) {
        out.salt = store_be64(load_be64(salt->size() > 8 ? salt->last(8) : *salt) + 1);
        running = des.encrypt(out.salt);
    }
    for (std::size_t at = 0; at < data.size(); at += 8) {
        const std::size_t n = std::min<std::size_t>(8, data.size() - at);
        running = des.encrypt(store_be64(load_be64(running) ^ load_be64(data.subspan(at, n))));
    }
    running[0] ^= 0x80;
    xecrypt::BlockCipher triple(xecrypt::CipherAlgorithm::Des3, triple_key(key), xecrypt::CipherMode::Ecb);
    out.mac = triple.encrypt(running);
    return out;
}

Bytes acr(ByteView key, ByteView certificate, ByteView identification) {
    key = key.first(8);   // only the first eight bytes of the key are used
    Bytes block = slice(identification, 0, 4);
    append(block, certificate.first(4));
    const ByteView sbox(kSbox.data(), kSbox.size());
    const ByteView plain(kPlainText.data(), kPlainTextUsed);
    const Bytes iv = xecrypt::parve_ecb(key, sbox, identification.subspan(0x10, 8));
    const Bytes cd = xecrypt::parve_ecb(key, sbox, block);
    const Bytes ab = xecrypt::parve_cbc_mac(key, sbox, iv, plain);
    const Bytes sum = xecrypt::chain_and_sum_mac(cd, ab, plain);
    return store_be64(load_be64(sum) ^ load_be64(ab));
}

Session::Session(const StaticKeys& keys, ByteView identification, xecrypt::RandomSource& random) : keys_(keys), random_(&random) {
    require_size(identification, kIdentificationSize, "identification");
    // (the checksum is checked by the console; a bad one is let through)
    // payload bytes 5..: 15 bytes, two words, a byte, a word, a byte; they are laid out again as 15 bytes, a gap, the two words, the two bytes, the word
    identification_.assign(0x20, 0);
    std::copy_n(identification.begin() + 5, 15, identification_.begin());
    std::copy_n(identification.begin() + 20, 4, identification_.begin() + 16);        // the two words
    identification_[20] = identification[24];
    identification_[21] = identification[27];
    std::copy_n(identification.begin() + 25, 2, identification_.begin() + 22);        // the last word
}

Result<Bytes> Session::challenge_init(ByteView challenge) {
    require_size(challenge, kChallengeSize, "challenge");
    if (!packet_checksum_ok(challenge)) return FormatError("xsm3", 0, "checksum failed when validating the challenge init");
    const ByteView payload = challenge.subspan(kHead, 0x18);
    Bytes decrypted = crypt(keys_.key1, payload, CryptMode::Decrypt);
    random_console_ = slice(decrypted, 0, 0x10);
    console_id_ = slice(decrypted, 0x10, 8);
    const Mac incoming = mac(keys_.key2, std::nullopt, payload);
    if (!std::equal(incoming.mac.begin() + 4, incoming.mac.end(), challenge.begin() + static_cast<std::ptrdiff_t>(kHead + 0x18)))
        return FormatError("xsm3", kHead + 0x18, "MAC failed when validating the challenge init");

    Bytes swapped = slice(random_console_, 8, 8);
    append(swapped, ByteView(random_console_).first(8));
    random_console_enc_ = crypt(keys_.key1, random_console_, CryptMode::Encrypt);
    random_console_swap_enc_ = crypt(keys_.key2, swapped, CryptMode::Encrypt);
    random_controller_ = random_->bytes(0x10);

    Bytes response = empty_response(0x28);
    // the controller's random and the console's go where the decrypted challenge had its random and id
    Bytes buffer = random_controller_;
    append(buffer, random_console_);
    init_hash_ = xecrypt::sha(buffer);
    const Bytes sealed = crypt(random_console_enc_, buffer, CryptMode::Encrypt);
    std::copy(sealed.begin(), sealed.end(), response.begin() + kHead);
    const Mac response_mac = mac(random_console_swap_enc_, std::nullopt, ByteView(response).subspan(kHead, 0x20));
    const Bytes tag = acr(response_mac.mac, console_id_, identification_);
    std::copy(tag.begin(), tag.end(), response.begin() + kHead + 0x20);
    response[kHead + 0x28] = packet_checksum(response);

    // from now on the console's random is: the controller's last word, twice (its two words become the first eight bytes)
    std::copy_n(random_controller_.begin() + 0xC, 4, random_console_.begin());
    std::copy_n(random_console_.begin() + 0xC, 4, random_console_.begin() + 4);
    response.resize(kHead + response[4] + 1);
    return response;
}

Result<Bytes> Session::challenge_verify(ByteView challenge) {
    require_size(challenge, kVerifySize, "verify");
    if (!packet_checksum_ok(challenge)) return FormatError("xsm3", 0, "checksum failed when validating the challenge verify");
    if (random_controller_.empty() || random_console_.empty()) return UsageError("challenge_verify needs a challenge_init first");
    const ByteView payload = challenge.subspan(kHead, 8);
    const Bytes decrypted = crypt(random_controller_, payload, CryptMode::Decrypt);
    std::copy(decrypted.begin(), decrypted.end(), random_console_.begin() + 8);
    const Mac incoming = mac(init_hash_, ByteView(random_console_), payload);
    if (!std::equal(incoming.mac.begin(), incoming.mac.end(), challenge.begin() + static_cast<std::ptrdiff_t>(kHead + 8)))
        return FormatError("xsm3", kHead + 8, "MAC failed when validating the challenge verify");

    Bytes response = empty_response(0x10);
    const Bytes tag = acr(random_console_, console_id_, identification_);
    const Bytes sealed = crypt(random_console_enc_, ByteView(tag).first(8), CryptMode::Encrypt);
    std::copy(sealed.begin(), sealed.end(), response.begin() + kHead);
    const Mac response_mac = mac(random_console_swap_enc_, ByteView(random_console_), ByteView(response).subspan(kHead, 8));
    std::copy(response_mac.mac.begin(), response_mac.mac.end(), response.begin() + kHead + 8);
    response[kHead + 0x10] = packet_checksum(response);
    response.resize(kHead + response[4] + 1);
    return response;
}

std::span<const std::uint8_t> sample_identification() { return kMsControllerIdentification; }
std::span<const std::uint8_t> sample_challenge() { return kSetChallengePacket; }
std::span<const std::uint8_t> sample_verify() { return kGetResponseVerifyPacket; }

cli::Command command() {
    return cli::Command{
        "xsm3",
        "run the XSM3 controller exchange on the sample packets, with the static keys of a keyvault",
        {cli::Option{"--keyvault", "", "the keyvault whose devkit 2DES keys are the static keys (default KV/banned.bin)", cli::Arg::Value, "FILE", "KV/banned.bin"}},
        [](const cli::Args& args) {
            const keyvault::Keyvault kv = keyvault::Keyvault::parse(fs::read_file(fs::from_text(args.required("keyvault"))));
            Session session(StaticKeys{kv.global_dev_2des_key_1, kv.global_dev_2des_key_2}, sample_identification());
            const Result<Bytes> response = session.challenge_init(sample_challenge());
            if (!response.ok()) {
                cb::err("{}\n", response.error().what());
                return 1;
            }
            cb::out("0x{:X}\n", response.value().size());
            const Result<Bytes> verified = session.challenge_verify(sample_verify());
            if (!verified.ok()) cb::err("{}\n", verified.error().what());
            return 0;
        },
        {},
        "",
        ""};
}

}  // namespace cb::formats::xbox::crypto::xsm3
