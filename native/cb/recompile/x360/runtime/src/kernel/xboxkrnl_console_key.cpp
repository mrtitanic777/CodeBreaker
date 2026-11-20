// The console's identity and signature (XeKeysGetConsoleID, XeKeysConsolePrivateKeySign,
// XeKeysConsoleSignatureVerification) on this console's key (kernel/stfs_mount.h ConsoleKey: the user's keyvault or the
// installation's stand-in). XAM signs the CON packages it writes (profiles, saves) and checks the signature of every one
// it reads (xam.xex 0x8167C370: SHA-1 of header [0x22C, 0x344), then the console signature at header 0x004).

#include "kernel/kernel.h"
#include "kernel/stfs_mount.h"
#include "stfs/sign.h"

#include <cstring>
#include <format>

namespace
{
    const uint8_t* ConsoleId() { return kernel::stfs_mount::ConsoleKey().certificate.data() + 2; }
}

// NTSTATUS XeKeysGetConsoleID(PBYTE ConsoleId /* 5 bytes, optional */, PCHAR Text /* 13 chars, optional */): the
// console ID from its certificate; the text is the 12-digit form System Info shows (the upper 36 bits in decimal,
// then the low 4 bits; INFER: the console's own formatting was not traced).
GUEST_IMPORT(XeKeysGetConsoleID)
{
    auto& mem = guest::g_memory;
    const uint8_t* id = ConsoleId();
    if (ctx.r3.u32)
        std::memcpy(mem.Translate(ctx.r3.u32), id, 5);
    if (ctx.r4.u32)
    {
        uint64_t v = 0;
        for (int i = 0; i < 5; i++)
            v = v << 8 | id[i];
        const std::string text = std::format("{:011}{}", v >> 4, uint32_t(v & 0xF) % 10);
        std::memcpy(mem.Translate(ctx.r4.u32), text.c_str(), text.size() + 1);
    }
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// BOOL XeKeysConsolePrivateKeySign(const BYTE Hash[20], PXE_CONSOLE_SIGNATURE Signature): this console's certificate
// (0x1A8 bytes) and its signature of the hash (0x80 bytes, stored byte-reversed), as in a CON header at 0x004.
GUEST_IMPORT(XeKeysConsolePrivateKeySign)
{
    auto& mem = guest::g_memory;
    const stfs::ConsoleKey& key = kernel::stfs_mount::ConsoleKey();
    stfs::Sha1Digest digest;
    std::memcpy(digest.data(), mem.Translate(ctx.r3.u32), digest.size());
    std::array<uint8_t, stfs::kConSignatureSize> reversed;
    std::string error;
    if (!stfs::SignDigest(digest, key, reversed, &error))
        Fatal("XeKeysConsolePrivateKeySign: {}", error);
    uint8_t* out = mem.Translate(ctx.r4.u32);
    std::memcpy(out, key.certificate.data(), stfs::kCertificateSize);
    std::memcpy(out + stfs::kCertificateSize, reversed.data(), reversed.size());
    ctx.r3.u64 = 1;
}

// BOOL XeKeysConsoleSignatureVerification(const BYTE Hash[20], const XE_CONSOLE_SIGNATURE* Signature,
// PBOOL FromThisConsole): whether the signature is the certificate's key's signature of the hash. *FromThisConsole: the
// certificate is this console's (INFER: xam.xex treats a nonzero value as its own console's package, which installer
// content requires, 0x816826xx). The certificate's own Microsoft signature is not checked (no Microsoft key here).
GUEST_IMPORT(XeKeysConsoleSignatureVerification)
{
    auto& mem = guest::g_memory;
    stfs::Sha1Digest digest;
    std::memcpy(digest.data(), mem.Translate(ctx.r3.u32), digest.size());
    const uint8_t* signature = mem.Translate(ctx.r4.u32);
    const bool valid = stfs::VerifyDigest(digest, { signature, stfs::kCertificateSize },
                                          { signature + stfs::kCertificateSize, stfs::kConSignatureSize });
    if (ctx.r5.u32)
        mem.Write32(ctx.r5.u32, valid && std::memcmp(signature + 2, ConsoleId(), 5) == 0 ? 1 : 0);
    if (!valid)
        Log("XeKeysConsoleSignatureVerification from {:08X}: the signature does not verify with its certificate "
            "(console {:02X}{:02X}{:02X}{:02X}{:02X})", uint32_t(ctx.lr), signature[2], signature[3], signature[4],
            signature[5], signature[6]);
    ctx.r3.u64 = valid ? 1 : 0;
}
