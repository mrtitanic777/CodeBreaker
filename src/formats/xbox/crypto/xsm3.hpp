// formats/xbox/crypto/xsm3.hpp -- XSM3, the Xbox 360's "security method 3": the challenge-response a console and its USB controller (or
// accessory) go through to prove the controller is genuine.
//
//     xsm3::Session session(keys, controller_identification_packet);       // the controller's 0x1D-byte identification
//     Result<Bytes> response = session.challenge_init(challenge_packet);   // the console's 0x22-byte challenge -> the 0x2E-byte reply
//     Result<Bytes> verify = session.challenge_verify(verify_packet);      // the console's 0x16-byte verify -> the 0x16-byte reply
//
// After oct0xor's write-up and libxsm3 (InvoxiPlayGames). This is the controller's side: the session answers the console. The two static keys
// are 16-byte 2DES keys of the console keyvault (its global devkit keys at 0x124 and 0x134). A packet is: two magic bytes, two more, the payload
// length, the payload and a checksum byte (the XOR of the payload). Crypt is 3DES in CBC mode and a MAC is single-DES CBC-MAC over the data
// finished with 3DES, both with the 16-byte key doubled to 24 bytes; the console's certificate id and the identification data go into the
// ACR, which the Parve MAC family computes (xecrypt.hpp).
//
// Errors: a packet of the wrong length is a UsageError; a failed checksum or MAC is the FormatError in the Result.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <span>

#include "core/bytes.hpp"
#include "core/cli.hpp"
#include "core/error.hpp"
#include "formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::xsm3 {

struct StaticKeys {
    std::array<std::uint8_t, 16> key1{};
    std::array<std::uint8_t, 16> key2{};
};

// The XOR of a packet's payload (the bytes after its five-byte head, as many as its fifth byte says).
std::uint8_t packet_checksum(ByteView packet);
bool packet_checksum_ok(ByteView packet);

enum class CryptMode { Decrypt, Encrypt };
// 3DES-CBC (zero IV) under the key doubled to 24 bytes (a longer or shorter key is doubled and cut to 24).
Bytes crypt(ByteView key, ByteView data, CryptMode mode);
// The MAC: the data's single-DES CBC-MAC (started from the DES encryption of salt+1 when there is a salt), its first bit flipped, 3DES-encrypted.
// `salt` (any length: its last eight bytes, plus one, are used) is nullopt for none; answers the salt used (empty without one) and the MAC.
struct Mac {
    Bytes salt;
    Bytes mac;
};
Mac mac(ByteView key, std::optional<ByteView> salt, ByteView data);
// The accessory's ACR value for a key, the console's certificate id and the identification data.
Bytes acr(ByteView key, ByteView certificate, ByteView identification);

class Session {
public:
    // `identification` is the controller's 0x1D-byte identification packet (UsageError for another length; a bad checksum is allowed, as the
    // console tolerates it).
    Session(const StaticKeys& keys, ByteView identification, xecrypt::RandomSource& random = xecrypt::os_random());

    Result<Bytes> challenge_init(ByteView challenge);      // 0x22 bytes in; the 0x2E-byte response out
    Result<Bytes> challenge_verify(ByteView challenge);    // 0x16 bytes in; the 0x16-byte response out

private:
    StaticKeys keys_;
    xecrypt::RandomSource* random_;
    Bytes identification_;          // 0x20
    Bytes random_console_;          // 16
    Bytes console_id_;              // 8
    Bytes init_hash_;               // SHA-1 of the decrypted challenge
    Bytes random_controller_;       // 16
    Bytes random_console_enc_;
    Bytes random_console_swap_enc_;
};

// The sample packets of the tool: a Microsoft controller's identification, a console challenge and a verify.
std::span<const std::uint8_t> sample_identification();
std::span<const std::uint8_t> sample_challenge();
std::span<const std::uint8_t> sample_verify();

// `crypto xsm3 [--keyvault KV/banned.bin]`: runs the sample exchange with the keys from a keyvault and prints the size of the first response.
cli::Command command();

}  // namespace cb::formats::xbox::crypto::xsm3
