#pragma once

// CON package signatures (Shared/docs/stfs.md, "Signatures"). A console signs [0x22C, 0x344) of the header (licenses,
// header hash, header size) with its 1024-bit RSA key: PKCS#1 v1.5 with SHA-1, the 0x80-byte result stored
// byte-reversed at 0x1AC; its certificate (0x1A8 bytes, from its keyvault) is at 0x004 and holds the public key.
// LIVE / PIRS packages are signed by Microsoft (2048-bit); the library never re-signs them.
//
// The library carries no key material and never looks for any: a key is only ever the caller's (the user points the
// runtime at their own console's). RSA through Windows bcrypt.

#include "stfs/format.h"
#include "stfs/package.h"

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace stfs
{
    // XeCrypt "BnQw" big number (big-endian qwords, least significant qword first) -> plain big-endian bytes.
    std::vector<uint8_t> BnQwToBigEndian(std::span<const uint8_t> bnqw);

    struct ConsoleKey
    {
        std::array<uint8_t, kCertificateSize> certificate{}; // copied to header 0x004
        uint32_t exponent = 0x10001;
        std::vector<uint8_t> modulus, prime1, prime2;        // big-endian, 0x80 / 0x40 / 0x40 bytes

        // From an XECRYPT_RSAPRV_1024 (0x1D0 bytes: cqw, e, reserved, n, p, q, dp, dq, cr; BnQw numbers) and the
        // console certificate, both as the caller read them from their console's decrypted keyvault.
        static bool FromXeCrypt(std::span<const uint8_t> rsaPrivate1024, std::span<const uint8_t> certificate,
                                ConsoleKey& out, std::string* error = nullptr);
    };

    // Writes key.certificate to 0x004 and the signature of [0x22C, 0x344) to 0x1AC, then checks it against the
    // certificate's public key. The header hash (0x32C) must already be final. header.size() >= 0x344.
    bool SignCon(std::vector<uint8_t>& header, const ConsoleKey& key, std::string* error = nullptr);

    // A CON header's signature checked with the public key in its own certificate. NotChecked for LIVE / PIRS.
    SignatureStatus VerifySignature(std::span<const uint8_t> header);

    // The console signature primitives on a digest computed by the caller (a console's XeKeysConsolePrivateKeySign /
    // XeKeysConsoleSignatureVerification): the 0x80-byte signature byte-reversed, as stored in a header.
    bool SignDigest(const Sha1Digest& digest, const ConsoleKey& key, std::array<uint8_t, kConSignatureSize>& reversed,
                    std::string* error = nullptr);
    bool VerifyDigest(const Sha1Digest& digest, std::span<const uint8_t> certificate, std::span<const uint8_t> reversed);

    // A new 1024-bit key pair of the library's own making (no console's) with a certificate holding its public key and
    // console ID `consoleId` (retail type, no manufacturing date, no Microsoft signature): a stand-in console for a
    // runtime run without the user's keyvault.
    bool GenerateConsoleKey(std::span<const uint8_t, 5> consoleId, ConsoleKey& out, std::string* error = nullptr);
}
