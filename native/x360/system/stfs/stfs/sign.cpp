// CON signatures (stfs/sign.h): PKCS#1 v1.5 RSA-SHA1 through Windows bcrypt, stored byte-reversed.

#include "stfs/sign.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <cstring>
#include <format>

namespace stfs
{
    namespace
    {
        constexpr uint32_t kCertExponent = 0x24; // in the certificate (header 0x028)
        constexpr uint32_t kCertModulus = 0x28;  // in the certificate (header 0x02C), BnQw, 0x80 bytes

        BCRYPT_ALG_HANDLE Rsa()
        {
            static BCRYPT_ALG_HANDLE algorithm = []
            {
                BCRYPT_ALG_HANDLE h = nullptr;
                BCryptOpenAlgorithmProvider(&h, BCRYPT_RSA_ALGORITHM, nullptr, 0);
                return h;
            }();
            return algorithm;
        }

        void Append(std::vector<uint8_t>& blob, const void* p, size_t n)
        {
            const auto* b = static_cast<const uint8_t*>(p);
            blob.insert(blob.end(), b, b + n);
        }

        std::vector<uint8_t> ExponentBytes(uint32_t e)
        {
            std::vector<uint8_t> out;
            for (int shift = 24; shift >= 0; shift -= 8)
                if (!out.empty() || (e >> shift) & 0xFF)
                    out.push_back(uint8_t(e >> shift));
            return out;
        }

        Sha1Digest SignedDigest(std::span<const uint8_t> header)
        {
            return Sha1(header.subspan(offsets::kLicenses, offsets::kSignedEnd - offsets::kLicenses));
        }

        // Checks a big-endian signature over digest with a public key (big-endian modulus).
        bool VerifyWith(const std::vector<uint8_t>& modulus, uint32_t exponent, const Sha1Digest& digest,
                        std::vector<uint8_t> signature)
        {
            const std::vector<uint8_t> e = ExponentBytes(exponent);
            BCRYPT_RSAKEY_BLOB head{};
            head.Magic = BCRYPT_RSAPUBLIC_MAGIC;
            head.BitLength = ULONG(modulus.size() * 8);
            head.cbPublicExp = ULONG(e.size());
            head.cbModulus = ULONG(modulus.size());
            std::vector<uint8_t> blob;
            Append(blob, &head, sizeof(head));
            Append(blob, e.data(), e.size());
            Append(blob, modulus.data(), modulus.size());
            BCRYPT_KEY_HANDLE key = nullptr;
            if (BCryptImportKeyPair(Rsa(), nullptr, BCRYPT_RSAPUBLIC_BLOB, &key, blob.data(), ULONG(blob.size()), 0) != 0)
                return false;
            BCRYPT_PKCS1_PADDING_INFO padding{BCRYPT_SHA1_ALGORITHM};
            const NTSTATUS status = BCryptVerifySignature(key, &padding, const_cast<PUCHAR>(digest.data()),
                                                          ULONG(digest.size()), signature.data(),
                                                          ULONG(signature.size()), BCRYPT_PAD_PKCS1);
            BCryptDestroyKey(key);
            return status == 0;
        }
    }

    std::vector<uint8_t> BnQwToBigEndian(std::span<const uint8_t> bnqw)
    {
        std::vector<uint8_t> out;
        out.reserve(bnqw.size());
        for (size_t q = bnqw.size() / 8; q-- > 0;)
            out.insert(out.end(), bnqw.begin() + q * 8, bnqw.begin() + q * 8 + 8);
        return out;
    }

    bool ConsoleKey::FromXeCrypt(std::span<const uint8_t> key, std::span<const uint8_t> certificate, ConsoleKey& out,
                                 std::string* error)
    {
        if (key.size() < 0x110 || certificate.size() < kCertificateSize || Be32(key.data()) != 0x10)
        {
            if (error)
                *error = "not an XECRYPT_RSAPRV_1024 key and a 0x1A8-byte certificate";
            return false;
        }
        std::memcpy(out.certificate.data(), certificate.data(), kCertificateSize);
        out.exponent = Be32(key.data() + 4);
        out.modulus = BnQwToBigEndian(key.subspan(0x10, 0x80));
        out.prime1 = BnQwToBigEndian(key.subspan(0x90, 0x40));
        out.prime2 = BnQwToBigEndian(key.subspan(0xD0, 0x40));
        return true;
    }

    bool SignDigest(const Sha1Digest& digest, const ConsoleKey& key, std::array<uint8_t, kConSignatureSize>& reversed,
                    std::string* error)
    {
        auto fail = [&](std::string message)
        {
            if (error)
                *error = std::move(message);
            return false;
        };
        if (key.modulus.size() != kConSignatureSize || key.prime1.empty() || key.prime2.empty())
            return fail("the key is not a 1024-bit RSA private key");

        const std::vector<uint8_t> e = ExponentBytes(key.exponent);
        BCRYPT_RSAKEY_BLOB head{};
        head.Magic = BCRYPT_RSAPRIVATE_MAGIC;
        head.BitLength = ULONG(key.modulus.size() * 8);
        head.cbPublicExp = ULONG(e.size());
        head.cbModulus = ULONG(key.modulus.size());
        head.cbPrime1 = ULONG(key.prime1.size());
        head.cbPrime2 = ULONG(key.prime2.size());
        std::vector<uint8_t> blob;
        Append(blob, &head, sizeof(head));
        Append(blob, e.data(), e.size());
        Append(blob, key.modulus.data(), key.modulus.size());
        Append(blob, key.prime1.data(), key.prime1.size());
        Append(blob, key.prime2.data(), key.prime2.size());
        BCRYPT_KEY_HANDLE handle = nullptr;
        NTSTATUS status = BCryptImportKeyPair(Rsa(), nullptr, BCRYPT_RSAPRIVATE_BLOB, &handle, blob.data(),
                                              ULONG(blob.size()), 0);
        SecureZeroMemory(blob.data(), blob.size());
        if (status != 0)
            return fail(std::format("bcrypt refused the key ({:#x})", uint32_t(status)));
        BCRYPT_PKCS1_PADDING_INFO padding{BCRYPT_SHA1_ALGORITHM};
        std::vector<uint8_t> signature(kConSignatureSize);
        ULONG written = 0;
        status = BCryptSignHash(handle, &padding, const_cast<PUCHAR>(digest.data()), ULONG(digest.size()),
                                signature.data(), ULONG(signature.size()), &written, BCRYPT_PAD_PKCS1);
        BCryptDestroyKey(handle);
        if (status != 0 || written != kConSignatureSize)
            return fail(std::format("signing failed ({:#x})", uint32_t(status)));
        std::reverse_copy(signature.begin(), signature.end(), reversed.begin());
        return true;
    }

    bool VerifyDigest(const Sha1Digest& digest, std::span<const uint8_t> certificate, std::span<const uint8_t> reversed)
    {
        if (certificate.size() < kCertificateSize || reversed.size() < kConSignatureSize)
            return false;
        const uint32_t exponent = Be32(certificate.data() + kCertExponent);
        const std::vector<uint8_t> modulus = BnQwToBigEndian(certificate.subspan(kCertModulus, kConSignatureSize));
        std::vector<uint8_t> signature(reversed.begin(), reversed.begin() + kConSignatureSize);
        std::reverse(signature.begin(), signature.end());
        return VerifyWith(modulus, exponent, digest, std::move(signature));
    }

    bool GenerateConsoleKey(std::span<const uint8_t, 5> consoleId, ConsoleKey& out, std::string* error)
    {
        auto fail = [&](std::string message)
        {
            if (error)
                *error = std::move(message);
            return false;
        };
        BCRYPT_KEY_HANDLE handle = nullptr;
        if (BCryptGenerateKeyPair(Rsa(), &handle, 1024, 0) != 0 || BCryptFinalizeKeyPair(handle, 0) != 0)
            return fail("bcrypt could not generate a key pair");
        ULONG size = 0;
        BCryptExportKey(handle, nullptr, BCRYPT_RSAPRIVATE_BLOB, nullptr, 0, &size, 0);
        std::vector<uint8_t> blob(size);
        const NTSTATUS status = BCryptExportKey(handle, nullptr, BCRYPT_RSAPRIVATE_BLOB, blob.data(), size, &size, 0);
        BCryptDestroyKey(handle);
        if (status != 0)
            return fail(std::format("bcrypt could not export the key ({:#x})", uint32_t(status)));
        BCRYPT_RSAKEY_BLOB head;
        std::memcpy(&head, blob.data(), sizeof(head));
        const uint8_t* p = blob.data() + sizeof(head);
        out.exponent = 0;
        for (ULONG i = 0; i < head.cbPublicExp; i++)
            out.exponent = out.exponent << 8 | p[i];
        p += head.cbPublicExp;
        out.modulus.assign(p, p + head.cbModulus);
        p += head.cbModulus;
        out.prime1.assign(p, p + head.cbPrime1);
        p += head.cbPrime1;
        out.prime2.assign(p, p + head.cbPrime2);
        SecureZeroMemory(blob.data(), blob.size());
        if (out.modulus.size() != kConSignatureSize)
            return fail("unexpected key size");
        // XE_CONSOLE_CERTIFICATE: size, console ID (2), part number (7), reserved, privileges, console type (0x1C),
        // manufacturing date (0x20), exponent (0x24), modulus (0x28, BnQw), Microsoft's signature (0xA8, left zero).
        auto& c = out.certificate;
        c.fill(0);
        c[0] = uint8_t(kCertificateSize >> 8);
        c[1] = uint8_t(kCertificateSize);
        std::copy(consoleId.begin(), consoleId.end(), c.begin() + 2);
        static const char kPart[] = "X800000-000";
        std::memcpy(c.data() + 7, kPart, 11);
        PutBe32(c.data() + 0x1C, 2); // retail
        PutBe32(c.data() + kCertExponent, out.exponent);
        for (size_t q = 0; q < kConSignatureSize / 8; q++) // big-endian modulus -> BnQw (least significant qword first)
            std::memcpy(c.data() + kCertModulus + q * 8, out.modulus.data() + kConSignatureSize - (q + 1) * 8, 8);
        return true;
    }

    bool SignCon(std::vector<uint8_t>& header, const ConsoleKey& key, std::string* error)
    {
        auto fail = [&](std::string message)
        {
            if (error)
                *error = std::move(message);
            return false;
        };
        if (header.size() < offsets::kSignedEnd)
            return fail("header too short");
        if (Be32(header.data()) != uint32_t(Magic::Con))
            return fail("only CON packages are signed with a console key");
        std::memcpy(header.data() + offsets::kCertificate, key.certificate.data(), kCertificateSize);
        std::array<uint8_t, kConSignatureSize> reversed;
        if (!SignDigest(SignedDigest(header), key, reversed, error))
            return false;
        std::copy(reversed.begin(), reversed.end(), header.begin() + offsets::kConSignature);

        if (VerifySignature(header) != SignatureStatus::Valid)
            return fail("the signature does not verify with the certificate's key (key and certificate differ?)");
        return true;
    }

    SignatureStatus VerifySignature(std::span<const uint8_t> header)
    {
        if (header.size() < offsets::kSignedEnd || Be32(header.data()) != uint32_t(Magic::Con))
            return SignatureStatus::NotChecked;
        const uint8_t* stored = header.data() + offsets::kConSignature;
        if (std::all_of(stored, stored + kConSignatureSize, [](uint8_t b) { return b == 0; }))
            return SignatureStatus::Unsigned;
        const uint8_t* cert = header.data() + offsets::kCertificate;
        const uint32_t exponent = Be32(cert + kCertExponent);
        const std::vector<uint8_t> modulus = BnQwToBigEndian({cert + kCertModulus, kConSignatureSize});
        std::vector<uint8_t> signature(stored, stored + kConSignatureSize);
        std::reverse(signature.begin(), signature.end());
        return VerifyWith(modulus, exponent, SignedDigest(header), std::move(signature)) ? SignatureStatus::Valid
                                                                                         : SignatureStatus::Invalid;
    }
}
