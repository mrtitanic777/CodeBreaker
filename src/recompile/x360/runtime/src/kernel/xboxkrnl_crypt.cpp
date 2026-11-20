// Kernel crypto exports with console semantics: XeCryptRotSumSha, XeCryptBnQwBeSigVerify, XeKeysGetKey.
// Checked against the user's genuine NAND fonts: every .xtt verifies with
// the PIRS key and the salt "XBOX360XTT", a changed byte does not.

#include "kernel/kernel.h"
#include "kernel/console_keyvault.h"
#include "kernel/profile_store.h"
#include "settings.h"

#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <mutex>

namespace
{
    // XEKEY_CONSTANT_PIRS_KEY (0x39): the retail PIRS public key as XECRYPT_RSAPUB_2048 (big-endian guest layout:
    // cqw 0x20, public exponent 3, reserved, modulus as 0x20 big-endian qwords, least significant first). A public
    // key; it also verifies the PIRS system update packages (PKCS#1 over their headers).
    constexpr uint8_t kPirsPublicKey[0x110] = {
        0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xE6, 0x3B, 0x32, 0xB2, 0x8D, 0x9E, 0x9E, 0xE7, 0x9D, 0xFC, 0x5C, 0x72, 0x41, 0x94, 0x58, 0x47,
        0xDE, 0x0D, 0x18, 0x40, 0x72, 0xD6, 0xE3, 0x46, 0x8E, 0xBA, 0x8E, 0xBC, 0x1A, 0x90, 0xAC, 0x20,
        0xBA, 0x03, 0x85, 0xB5, 0x1A, 0x3E, 0x25, 0xF9, 0xA6, 0x58, 0xEB, 0xB6, 0xA3, 0xC4, 0xA3, 0xEE,
        0xB2, 0xB0, 0xAE, 0x97, 0x69, 0xEB, 0xFE, 0x71, 0xFC, 0x02, 0xAB, 0x77, 0xBA, 0xC8, 0xE6, 0x74,
        0xE6, 0x7C, 0x63, 0x0E, 0xAF, 0x4C, 0xF7, 0xE7, 0x11, 0x4A, 0x80, 0x24, 0x72, 0x05, 0x7A, 0x63,
        0xD0, 0xF8, 0x91, 0x02, 0xA6, 0xE7, 0x7D, 0x77, 0xC5, 0xA7, 0x9B, 0x08, 0x11, 0x2E, 0xA0, 0x64,
        0x45, 0x60, 0x46, 0xBC, 0x36, 0xE1, 0x17, 0x71, 0xBE, 0x66, 0x49, 0x2F, 0xAE, 0x20, 0xA4, 0x76,
        0x9C, 0x27, 0x51, 0xCF, 0x4B, 0x34, 0x7A, 0x35, 0xBC, 0xA4, 0xAA, 0x1C, 0x47, 0x4B, 0xF4, 0x97,
        0x22, 0x4E, 0x13, 0x24, 0xD3, 0xC1, 0x57, 0xDF, 0x4D, 0x84, 0xB9, 0x18, 0x97, 0x99, 0xAC, 0x00,
        0xB3, 0x3D, 0x03, 0x25, 0x60, 0xC8, 0x7A, 0x59, 0xFE, 0x48, 0xFF, 0x28, 0x3D, 0x10, 0xBB, 0x9E,
        0x09, 0x06, 0x2A, 0x61, 0x20, 0x2C, 0xF8, 0x72, 0xEB, 0x87, 0xE6, 0xD1, 0xFB, 0xB3, 0x66, 0xFC,
        0x4A, 0x02, 0xAE, 0xD4, 0xD8, 0x37, 0xCF, 0xA6, 0x32, 0x25, 0x79, 0x36, 0x0E, 0xF4, 0xED, 0x19,
        0xA2, 0x10, 0x27, 0x96, 0x2F, 0x9F, 0xA9, 0x3D, 0xA4, 0x37, 0x30, 0x11, 0x51, 0x83, 0xBD, 0xF7,
        0xC7, 0xE5, 0xCE, 0xAA, 0xEC, 0xDE, 0x48, 0xA0, 0x84, 0xF7, 0xB0, 0xF6, 0x4B, 0x8E, 0xF0, 0x89,
        0xBD, 0x47, 0x7C, 0x90, 0xDD, 0x88, 0x12, 0x17, 0x40, 0xD2, 0x4E, 0xA6, 0xC6, 0x11, 0x04, 0x1B,
        0x57, 0xA8, 0x68, 0xB4, 0x61, 0xF4, 0x1B, 0xC6, 0x8B, 0xE8, 0xD9, 0x20, 0xF2, 0x05, 0xE0, 0x70,
    };

    constexpr uint32_t kStatusBufferTooSmall = 0xC0000023;

    void Sha1(const std::initializer_list<std::pair<const void*, size_t>> parts, uint8_t digest[20])
    {
        BCRYPT_HASH_HANDLE hash = nullptr;
        if (!BCRYPT_SUCCESS(BCryptCreateHash(BCRYPT_SHA1_ALG_HANDLE, &hash, nullptr, 0, nullptr, 0, 0)))
            Fatal("kernel crypto: unable to create a SHA-1 hash");
        for (const auto& [data, size] : parts)
            if (size)
                BCryptHashData(hash, static_cast<PUCHAR>(const_cast<void*>(data)), ULONG(size), 0);
        BCryptFinishHash(hash, digest, 20, 0);
        BCryptDestroyHash(hash);
    }

    uint64_t LoadBe64(const uint8_t* p)
    {
        uint64_t v = 0;
        for (int i = 0; i < 8; i++)
            v = (v << 8) | p[i];
        return v;
    }

    void StoreBe64(uint8_t* p, uint64_t v)
    {
        for (int i = 7; i >= 0; i--, v >>= 8)
            p[i] = uint8_t(v);
    }

    uint64_t Rotl64(uint64_t v, int n) { return (v << n) | (v >> (64 - n)); }

    // The rotating sums over big-endian qwords. The second borrow test compares the new difference with the
    // input (not with the old value): the console's rule, the only one the genuine fonts verify with.
    void RotSum(uint64_t s[4], const uint8_t* data, uint32_t size)
    {
        for (uint32_t i = 0; i + 8 <= size; i += 8)
        {
            const uint64_t v = LoadBe64(data + i);
            s[1] += v;
            s[3] -= v;
            if (s[1] < v)
                s[0]++;
            if (s[3] > v)
                s[2]--;
            s[1] = Rotl64(s[1], 29);
            s[3] = Rotl64(s[3], 31);
        }
    }

    // 2048-bit Montgomery arithmetic on little-endian 64-bit limbs.
    constexpr int kLimbs = 0x20;
    using Bn = std::array<uint64_t, kLimbs>;

    // r = a * b * 2^-2048 mod n (a, b < n); n0 = -n^-1 mod 2^64.
    Bn MontMul(const Bn& a, const Bn& b, const Bn& n, uint64_t n0)
    {
        uint64_t t[kLimbs + 2] = {};
        for (int i = 0; i < kLimbs; i++)
        {
            unsigned __int128 carry = 0;
            for (int j = 0; j < kLimbs; j++)
            {
                carry += (unsigned __int128)a[j] * b[i] + t[j];
                t[j] = uint64_t(carry);
                carry >>= 64;
            }
            carry += t[kLimbs];
            t[kLimbs] = uint64_t(carry);
            t[kLimbs + 1] = uint64_t(carry >> 64);

            const uint64_t m = t[0] * n0;
            carry = (unsigned __int128)m * n[0] + t[0];
            carry >>= 64;
            for (int j = 1; j < kLimbs; j++)
            {
                carry += (unsigned __int128)m * n[j] + t[j];
                t[j - 1] = uint64_t(carry);
                carry >>= 64;
            }
            carry += t[kLimbs];
            t[kLimbs - 1] = uint64_t(carry);
            t[kLimbs] = t[kLimbs + 1] + uint64_t(carry >> 64);
            t[kLimbs + 1] = 0;
        }
        Bn r;
        std::memcpy(r.data(), t, sizeof(r));
        bool subtract = t[kLimbs] != 0;
        if (!subtract)
        {
            subtract = true;
            for (int i = kLimbs - 1; i >= 0; i--)
                if (r[i] != n[i])
                {
                    subtract = r[i] > n[i];
                    break;
                }
        }
        if (subtract)
        {
            uint64_t borrow = 0;
            for (int i = 0; i < kLimbs; i++)
            {
                const unsigned __int128 d = (unsigned __int128)r[i] - n[i] - borrow;
                r[i] = uint64_t(d);
                borrow = uint64_t(d >> 64) & 1;
            }
        }
        return r;
    }

    bool Less(const Bn& a, const Bn& b)
    {
        for (int i = kLimbs - 1; i >= 0; i--)
            if (a[i] != b[i])
                return a[i] < b[i];
        return false;
    }

    // RC4 over the data in place.
    void Rc4(const uint8_t* key, size_t keySize, uint8_t* data, size_t size)
    {
        uint8_t s[256];
        for (int i = 0; i < 256; i++)
            s[i] = uint8_t(i);
        for (int i = 0, j = 0; i < 256; i++)
        {
            j = (j + s[i] + key[i % keySize]) & 0xFF;
            std::swap(s[i], s[j]);
        }
        for (size_t k = 0, i = 0, j = 0; k < size; k++)
        {
            i = (i + 1) & 0xFF;
            j = (j + s[i]) & 0xFF;
            std::swap(s[i], s[j]);
            data[k] ^= s[(s[i] + s[j]) & 0xFF];
        }
    }
}

// VOID XeCryptRotSumSha(PBYTE Input1, DWORD Size1, PBYTE Input2, DWORD Size2, PBYTE Digest, DWORD DigestSize):
// rotating sums over the whole qwords of both inputs, then SHA-1(sums, sums, Input1, Input2, ~sums, ~sums) with the
// sums as four big-endian qwords. DigestSize bytes (at most 20) are written.
GUEST_IMPORT(XeCryptRotSumSha)
{
    auto& mem = guest::g_memory;
    const uint8_t* in1 = ctx.r3.u32 ? mem.Translate(ctx.r3.u32) : nullptr;
    const uint8_t* in2 = ctx.r5.u32 ? mem.Translate(ctx.r5.u32) : nullptr;
    const uint32_t size1 = in1 ? ctx.r4.u32 : 0;
    const uint32_t size2 = in2 ? ctx.r6.u32 : 0;
    uint64_t s[4] = {};
    RotSum(s, in1, size1);
    RotSum(s, in2, size2);
    uint8_t sums[32], inverted[32];
    for (int i = 0; i < 4; i++)
    {
        StoreBe64(sums + i * 8, s[i]);
        StoreBe64(inverted + i * 8, ~s[i]);
    }
    uint8_t digest[20];
    Sha1({ { sums, 32 }, { sums, 32 }, { in1, size1 }, { in2, size2 }, { inverted, 32 }, { inverted, 32 } }, digest);
    if (ctx.r7.u32)
        std::memcpy(mem.Translate(ctx.r7.u32), digest, std::min<uint32_t>(ctx.r8.u32, sizeof(digest)));
}

// BOOL XeCryptBnQwBeSigVerify(PXECRYPT_SIG Sig, const BYTE* Hash, const BYTE* Salt, PXECRYPT_RSA Key):
// Sig (0x100 bytes, big-endian qwords least significant first) is raised to the public exponent with Montgomery
// multiplications, which leaves sig^3 * R^-2 mod N (R = 2^2048), read as a big-endian XECRYPT_SIG: zero pad (0xE0),
// 0x01, Salt[10], H = SHA-1(8 zero bytes, Hash[20], Salt[10]), 0xBC, with the first 0xEB bytes RC4-encrypted under
// H and the top bit of byte 0 cleared. TRUE if it matches.
GUEST_IMPORT(XeCryptBnQwBeSigVerify)
{
    auto& mem = guest::g_memory;
    const uint32_t sigAddress = ctx.r3.u32, hashAddress = ctx.r4.u32, saltAddress = ctx.r5.u32, keyAddress = ctx.r6.u32;
    if (!sigAddress || !hashAddress || !saltAddress || !keyAddress)
        Fatal("XeCryptBnQwBeSigVerify({:08X}, {:08X}, {:08X}, {:08X}): null argument", sigAddress, hashAddress,
            saltAddress, keyAddress);
    const uint8_t* key = mem.Translate(keyAddress);
    const uint32_t cqw = mem.Read32(keyAddress), exponent = mem.Read32(keyAddress + 4);
    if (cqw != kLimbs || exponent != 3)
        Fatal("XeCryptBnQwBeSigVerify: key with {} qwords and exponent {} (only 2048-bit e=3 keys are known)", cqw,
            exponent);

    Bn n, sig;
    for (int i = 0; i < kLimbs; i++)
    {
        n[i] = LoadBe64(key + 0x10 + i * 8);
        sig[i] = LoadBe64(mem.Translate(sigAddress) + i * 8);
    }
    BOOL valid = FALSE;
    if (Less(sig, n))
    {
        uint64_t inverse = 1; // n^-1 mod 2^64 by Newton's iteration (n is odd)
        for (int i = 0; i < 6; i++)
            inverse *= 2 - n[0] * inverse;
        const Bn m = MontMul(MontMul(sig, sig, n, 0 - inverse), sig, n, 0 - inverse);

        uint8_t block[0x100];
        for (int i = 0; i < kLimbs; i++)
            StoreBe64(block + 0x100 - (i + 1) * 8, m[i]);

        const uint8_t* salt = mem.Translate(saltAddress);
        uint8_t expected[0x100] = {};
        expected[0xE0] = 1;
        std::memcpy(expected + 0xE1, salt, 10);
        const uint8_t zeros[8] = {};
        Sha1({ { zeros, 8 }, { mem.Translate(hashAddress), 20 }, { salt, 10 } }, expected + 0xEB);
        expected[0xFF] = 0xBC;
        Rc4(expected + 0xEB, 20, expected, 0xEB);
        expected[0] &= 0x7F;
        valid = std::memcmp(block, expected, sizeof(block)) == 0;
    }
    Log("XeCryptBnQwBeSigVerify(salt \"{}\") from {:08X}: {}",
        std::string(reinterpret_cast<const char*>(mem.Translate(saltAddress)), 10), uint32_t(ctx.lr),
        valid ? "valid" : "INVALID");
    ctx.r3.u64 = valid;
}

// The key vault's non-secret identity keys (decrypted key vault layout): the console serial number (0x14, 12 ASCII
// digits at 0xB0), the motherboard serial (0x15, 8 bytes at 0xBC), the game region (0x16, be16 at 0xC8). From the
// owner's own key vault when console_settings.ini names one (read in place, never logged), else stand-ins: a serial
// stable per PC (from the data directory's path), a zero motherboard serial, the configured game region.
namespace
{
    struct VaultKey
    {
        uint16_t id;
        uint32_t offset, size;
    };
    constexpr VaultKey kVaultKeys[] = { { 0x14, 0xB0, 12 }, { 0x15, 0xBC, 8 }, { 0x16, 0xC8, 2 } };

    const VaultKey* FindVaultKey(uint16_t id)
    {
        for (const VaultKey& k : kVaultKeys)
            if (k.id == id)
                return &k;
        return nullptr;
    }

    const std::vector<uint8_t>& KeyVault() { return kernel::ConsoleKeyvault(); }

    std::vector<uint8_t> VaultKeyValue(const VaultKey& k)
    {
        const auto& kv = KeyVault();
        if (!kv.empty())
            return { kv.begin() + k.offset, kv.begin() + k.offset + k.size };
        std::vector<uint8_t> v(k.size, 0);
        if (k.id == 0x14)
        {
            uint64_t h = 0xCBF29CE484222325ull;
            for (wchar_t c : DataDirectory().wstring())
                h = (h ^ uint64_t(towlower(c))) * 0x100000001B3ull;
            for (uint32_t i = 0; i < k.size; i++, h /= 10)
                v[i] = uint8_t('0' + h % 10);
        }
        else if (k.id == 0x16)
        {
            v[0] = uint8_t(Settings().gameRegion >> 8);
            v[1] = uint8_t(Settings().gameRegion);
        }
        return v;
    }
}

// ULONG XeKeysGetKeyProperties(WORD KeyId): the key's size in the low 16 bits (xam.xex 0x816BA9B8 reads only those).
GUEST_IMPORT(XeKeysGetKeyProperties)
{
    const uint16_t id = uint16_t(ctx.r3.u32);
    if (const VaultKey* k = FindVaultKey(id))
        ctx.r3.u64 = k->size;
    else if (id == 0x39)
        ctx.r3.u64 = sizeof(kPirsPublicKey);
    else
        Fatal("XeKeysGetKeyProperties(0x{:X}) from {:08X}: key not known", id, uint32_t(ctx.lr));
}

// NTSTATUS XeKeysGetKey(WORD KeyId, PVOID KeyBuffer, PDWORD KeyLength): the identity keys above and the public PIRS
// key (0x39); other keys are console secrets and stop.
GUEST_IMPORT(XeKeysGetKey)
{
    auto& mem = guest::g_memory;
    const uint16_t id = uint16_t(ctx.r3.u32);
    if (const VaultKey* k = FindVaultKey(id))
    {
        if (!ctx.r5.u32)
            Fatal("XeKeysGetKey(0x{:X}): no length pointer", id);
        const uint32_t length = mem.Read32(ctx.r5.u32);
        mem.Write32(ctx.r5.u32, k->size);
        if (length < k->size || !ctx.r4.u32)
        {
            ctx.r3.u64 = kStatusBufferTooSmall;
            return;
        }
        const std::vector<uint8_t> value = VaultKeyValue(*k);
        std::memcpy(mem.Translate(ctx.r4.u32), value.data(), value.size());
        ctx.r3.u64 = kernel::kStatusSuccess;
        return;
    }
    // 0x41: the Xbox LIVE machine account service's RSA-2048 public key (xam.xex encrypts a secret with it before
    // contacting XEMACS, 0x81946388, and checks the status). Not carried: the machine account step fails and the
    // console stays off Microsoft's Xbox LIVE.
    if (id == 0x41)
    {
        static bool logged = false;
        if (!logged)
        {
            logged = true;
            Log("XeKeysGetKey(0x41) from {:08X}: the Xbox LIVE machine account key is not available (no Microsoft "
                "Xbox LIVE sign-up)", uint32_t(ctx.lr));
        }
        ctx.r3.u64 = 0xC0000225; // STATUS_NOT_FOUND
        return;
    }
    if (id != 0x39)
        Fatal("XeKeysGetKey(0x{:X}) from {:08X}: key not available", id, uint32_t(ctx.lr));
    if (!ctx.r5.u32)
        Fatal("XeKeysGetKey(0x39): no length pointer");
    const uint32_t length = mem.Read32(ctx.r5.u32);
    mem.Write32(ctx.r5.u32, sizeof(kPirsPublicKey));
    if (length < sizeof(kPirsPublicKey) || !ctx.r4.u32)
    {
        ctx.r3.u64 = kStatusBufferTooSmall;
        return;
    }
    std::memcpy(mem.Translate(ctx.r4.u32), kPirsPublicKey, sizeof(kPirsPublicKey));
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// ---- SHA-1 and HMAC-SHA1 ------------------------------------------------------------------------------------------
// XECRYPT_SHA_STATE (0x58 bytes, guest memory, big-endian): be32 count (bytes hashed), be32 state[5], buffer[64] (the
// partial block). Callers own and may copy it, so it is kept in that layout here. XECRYPT_HMACSHA_STATE = two of them:
// the inner hash (key ^ 0x36 bytes, then the data) and the outer one (key ^ 0x5C bytes). HMAC as standard (the key
// padded to 64 bytes): the profile Account files' HMAC (profile_store.h) confirms the console's HMAC-SHA1 is standard.
namespace
{
    constexpr uint32_t kShaStateSize = 0x58;

    struct ShaState
    {
        uint32_t count = 0;
        uint32_t h[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };
        uint8_t buffer[64] = {};
    };

    ShaState LoadSha(uint32_t address)
    {
        auto& mem = guest::g_memory;
        ShaState s;
        s.count = mem.Read32(address);
        for (int i = 0; i < 5; i++)
            s.h[i] = mem.Read32(address + 4 + i * 4);
        std::memcpy(s.buffer, mem.Translate(address + 0x18), 64);
        return s;
    }

    void StoreSha(uint32_t address, const ShaState& s)
    {
        auto& mem = guest::g_memory;
        mem.Write32(address, s.count);
        for (int i = 0; i < 5; i++)
            mem.Write32(address + 4 + i * 4, s.h[i]);
        std::memcpy(mem.Translate(address + 0x18), s.buffer, 64);
    }

    uint32_t Rol(uint32_t x, int n) { return (x << n) | (x >> (32 - n)); }

    void ShaBlock(uint32_t h[5], const uint8_t* block)
    {
        uint32_t w[80];
        for (int i = 0; i < 16; i++)
            w[i] = uint32_t(block[i * 4]) << 24 | uint32_t(block[i * 4 + 1]) << 16 | uint32_t(block[i * 4 + 2]) << 8 |
                block[i * 4 + 3];
        for (int i = 16; i < 80; i++)
            w[i] = Rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
        for (int i = 0; i < 80; i++)
        {
            const uint32_t f = i < 20 ? (b & c) | (~b & d) : i < 40 ? b ^ c ^ d : i < 60 ? (b & c) | (b & d) | (c & d)
                : b ^ c ^ d;
            const uint32_t k = i < 20 ? 0x5A827999 : i < 40 ? 0x6ED9EBA1 : i < 60 ? 0x8F1BBCDC : 0xCA62C1D6;
            const uint32_t t = Rol(a, 5) + f + e + k + w[i];
            e = d;
            d = c;
            c = Rol(b, 30);
            b = a;
            a = t;
        }
        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
    }

    void ShaUpdate(ShaState& s, const uint8_t* data, size_t size)
    {
        size_t used = s.count % 64;
        s.count += uint32_t(size);
        while (size)
        {
            const size_t take = std::min(size, 64 - used);
            std::memcpy(s.buffer + used, data, take);
            used += take;
            data += take;
            size -= take;
            if (used == 64)
            {
                ShaBlock(s.h, s.buffer);
                used = 0;
            }
        }
    }

    // Pads in place (the buffer ends up holding the last block compressed) and leaves the digest in h.
    void ShaFinal(ShaState& s, uint8_t digest[20])
    {
        size_t used = s.count % 64;
        const uint64_t bits = uint64_t(s.count) * 8;
        s.buffer[used++] = 0x80;
        if (used > 56)
        {
            std::memset(s.buffer + used, 0, 64 - used);
            ShaBlock(s.h, s.buffer);
            used = 0;
        }
        std::memset(s.buffer + used, 0, 56 - used);
        for (int i = 0; i < 8; i++)
            s.buffer[56 + i] = uint8_t(bits >> (56 - 8 * i));
        ShaBlock(s.h, s.buffer);
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 4; j++)
                digest[i * 4 + j] = uint8_t(s.h[i] >> (24 - 8 * j));
    }

    void UpdateFromGuest(ShaState& s, uint32_t address, uint32_t size)
    {
        if (address && size)
            ShaUpdate(s, guest::g_memory.Translate(address), size);
    }

    void WriteDigest(const uint8_t digest[20], uint32_t out, uint32_t outSize)
    {
        if (out)
            std::memcpy(guest::g_memory.Translate(out), digest, std::min<uint32_t>(outSize, 20));
    }

    // Once: the incremental SHA-1 above against Windows' on a 3-block message in uneven pieces.
    void CheckSha()
    {
        static const bool ok = [] {
            uint8_t message[150];
            for (int i = 0; i < 150; i++)
                message[i] = uint8_t(i * 7 + 3);
            ShaState s;
            ShaUpdate(s, message, 1);
            ShaUpdate(s, message + 1, 70);
            ShaUpdate(s, message + 71, 79);
            uint8_t mine[20], reference[20];
            ShaFinal(s, mine);
            Sha1({ { message, sizeof(message) } }, reference);
            return std::memcmp(mine, reference, 20) == 0;
        }();
        if (!ok)
            Fatal("kernel crypto: the incremental SHA-1 does not match Windows' SHA-1");
    }

    // The inner and outer states of HMAC-SHA1 with the guest key.
    void HmacInit(ShaState& inner, ShaState& outer, uint32_t key, uint32_t keySize)
    {
        CheckSha();
        if (keySize > 64)
            Fatal("XeCryptHmacSha: a {}-byte key (over 64 bytes) is not implemented", keySize);
        uint8_t ipad[64], opad[64];
        std::memset(ipad, 0x36, 64);
        std::memset(opad, 0x5C, 64);
        const uint8_t* k = keySize ? guest::g_memory.Translate(key) : nullptr;
        for (uint32_t i = 0; i < keySize; i++)
        {
            ipad[i] ^= k[i];
            opad[i] ^= k[i];
        }
        inner = ShaState{};
        outer = ShaState{};
        ShaUpdate(inner, ipad, 64);
        ShaUpdate(outer, opad, 64);
    }

    void HmacFinal(ShaState& inner, ShaState& outer, uint8_t digest[20])
    {
        uint8_t innerDigest[20];
        ShaFinal(inner, innerDigest);
        ShaUpdate(outer, innerDigest, 20);
        ShaFinal(outer, digest);
    }

    // XeCryptHmacSha / XeCryptHmacShaVerify: key, key size and three (pointer, size) inputs in r3-r10.
    void HmacOfArguments(const PPCContext& ctx, uint8_t digest[20])
    {
        ShaState inner, outer;
        HmacInit(inner, outer, ctx.r3.u32, ctx.r4.u32);
        UpdateFromGuest(inner, ctx.r5.u32, ctx.r6.u32);
        UpdateFromGuest(inner, ctx.r7.u32, ctx.r8.u32);
        UpdateFromGuest(inner, ctx.r9.u32, ctx.r10.u32);
        HmacFinal(inner, outer, digest);
    }
}

// VOID XeCryptShaInit(PXECRYPT_SHA_STATE State)
GUEST_IMPORT(XeCryptShaInit)
{
    CheckSha();
    StoreSha(ctx.r3.u32, ShaState{});
}

// VOID XeCryptShaUpdate(PXECRYPT_SHA_STATE State, const BYTE* Input, DWORD InputSize)
GUEST_IMPORT(XeCryptShaUpdate)
{
    ShaState s = LoadSha(ctx.r3.u32);
    UpdateFromGuest(s, ctx.r4.u32, ctx.r5.u32);
    StoreSha(ctx.r3.u32, s);
}

// VOID XeCryptShaFinal(PXECRYPT_SHA_STATE State, PBYTE Out, DWORD OutSize): the digest's first OutSize (<= 20) bytes;
// the state keeps the digest and the last padded block.
GUEST_IMPORT(XeCryptShaFinal)
{
    ShaState s = LoadSha(ctx.r3.u32);
    uint8_t digest[20];
    ShaFinal(s, digest);
    StoreSha(ctx.r3.u32, s);
    WriteDigest(digest, ctx.r4.u32, ctx.r5.u32);
}

// VOID XeCryptHmacShaInit(PXECRYPT_HMACSHA_STATE State, const BYTE* Key, DWORD KeySize)
GUEST_IMPORT(XeCryptHmacShaInit)
{
    ShaState inner, outer;
    HmacInit(inner, outer, ctx.r4.u32, ctx.r5.u32);
    StoreSha(ctx.r3.u32, inner);
    StoreSha(ctx.r3.u32 + kShaStateSize, outer);
}

// VOID XeCryptHmacShaUpdate(PXECRYPT_HMACSHA_STATE State, const BYTE* Input, DWORD InputSize)
GUEST_IMPORT(XeCryptHmacShaUpdate)
{
    ShaState inner = LoadSha(ctx.r3.u32);
    UpdateFromGuest(inner, ctx.r4.u32, ctx.r5.u32);
    StoreSha(ctx.r3.u32, inner);
}

// VOID XeCryptHmacShaFinal(PXECRYPT_HMACSHA_STATE State, PBYTE Out, DWORD OutSize)
GUEST_IMPORT(XeCryptHmacShaFinal)
{
    ShaState inner = LoadSha(ctx.r3.u32), outer = LoadSha(ctx.r3.u32 + kShaStateSize);
    uint8_t digest[20];
    HmacFinal(inner, outer, digest);
    StoreSha(ctx.r3.u32, inner);
    StoreSha(ctx.r3.u32 + kShaStateSize, outer);
    WriteDigest(digest, ctx.r4.u32, ctx.r5.u32);
}

// VOID XeCryptHmacSha(const BYTE* Key, DWORD KeySize, Input1, Size1, Input2, Size2, Input3, Size3, PBYTE Out,
//     DWORD OutSize)
GUEST_IMPORT(XeCryptHmacSha)
{
    uint8_t digest[20];
    HmacOfArguments(ctx, digest);
    WriteDigest(digest, kernel::StackArg32(ctx, 8), kernel::StackArg32(ctx, 9));
}

// BOOL XeCryptHmacShaVerify(const BYTE* Key, DWORD KeySize, Input1, Size1, Input2, Size2, Input3, Size3,
//     const BYTE* Digest, DWORD DigestSize): whether the HMAC's first DigestSize (<= 20) bytes match.
GUEST_IMPORT(XeCryptHmacShaVerify)
{
    uint8_t digest[20];
    HmacOfArguments(ctx, digest);
    const uint32_t expected = kernel::StackArg32(ctx, 8), size = std::min<uint32_t>(kernel::StackArg32(ctx, 9), 20);
    ctx.r3.u64 = expected && std::memcmp(guest::g_memory.Translate(expected), digest, size) == 0 ? 1 : 0;
}

// VOID XeCryptRc4(const BYTE* Key, DWORD KeySize, PBYTE Data, DWORD Size): RC4 over the data in place.
GUEST_IMPORT(XeCryptRc4)
{
    auto& mem = guest::g_memory;
    if (ctx.r4.u32 == 0 || ctx.r6.u32 == 0)
        return;
    std::vector<uint8_t> key(mem.Translate(ctx.r3.u32), mem.Translate(ctx.r3.u32) + ctx.r4.u32);
    Rc4(key.data(), key.size(), mem.Translate(ctx.r5.u32), ctx.r6.u32);
}

// NTSTATUS XeKeysHmacSha(WORD KeyId, Input1, Size1, Input2, Size2, Input3, Size3, PBYTE Out, DWORD OutSize): HMAC-SHA1
// keyed with one of the console's key vault keys. xam.xex signs its saved network settings with key 0x17 (0x81763838).
// The console's real key is a per-console secret the ports never read; what these HMACs protect is data the runtime
// itself writes and later checks, so a fixed stand-in key per key id gives the same results to every caller.
namespace
{
    // Guest copies of the stand-in keys (16 bytes per key id), for the guest-memory HMAC helpers.
    uint32_t StandInKey(uint32_t id)
    {
        auto& mem = guest::g_memory;
        static uint32_t keys = 0;
        if (keys == 0)
        {
            keys = mem.SystemAlloc(0x100 * 16);
            for (uint32_t k = 0; k < 0x100; k++)
                for (uint32_t i = 0; i < 16; i++)
                    mem.Write8(keys + k * 16 + i, uint8_t(0xA5 ^ k ^ (i * 0x3B)));
        }
        return keys + (id & 0xFF) * 16;
    }

    // HMAC-SHA1 with a stand-in key over one range of guest bytes.
    void StandInHmac(uint32_t id, uint32_t data, uint32_t size, uint8_t digest[20])
    {
        ShaState inner, outer;
        HmacInit(inner, outer, StandInKey(id), 16);
        UpdateFromGuest(inner, data, size);
        HmacFinal(inner, outer, digest);
    }
}

GUEST_IMPORT(XeKeysHmacSha)
{
    const uint32_t id = ctx.r3.u32 & 0xFF;
    ShaState inner, outer;
    HmacInit(inner, outer, StandInKey(id), 16);
    UpdateFromGuest(inner, ctx.r4.u32, ctx.r5.u32);
    UpdateFromGuest(inner, ctx.r6.u32, ctx.r7.u32);
    UpdateFromGuest(inner, ctx.r8.u32, ctx.r9.u32);
    uint8_t digest[20];
    HmacFinal(inner, outer, digest);
    WriteDigest(digest, ctx.r10.u32, kernel::StackArg32(ctx, 8));
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS XeKeysObfuscate(BOOL Roaming, const BYTE* Input, DWORD InputSize, PBYTE Output, PDWORD OutputSize) and
// BOOL XeKeysUnObfuscate(the same arguments; TRUE when it unseals): data sealed with the console's obfuscation key (0x17) or the roamable one
// (0x19). Sealed = a 16-byte checksum (HMAC of the confounder and data), an 8-byte confounder, the data; confounder
// and data RC4-encrypted with the HMAC of the checksum (xam.xex seals 0x14 bytes into 0x2C, 0x81899BF8). The roamable
// key is every retail console's (profiles' Account files move between consoles with it: kernel/profile_store.h
// AccountKey; unsealing also tries the development kit's); the console's own key is the stand-in of XeKeysHmacSha.
namespace
{
    constexpr uint32_t kObfuscateHeader = 0x18;

    // Guest copies of the roamable keys (retail, development kit) for the guest-memory HMAC helpers.
    uint32_t RoamingKey(uint32_t index)
    {
        auto& mem = guest::g_memory;
        static uint32_t keys = 0;
        if (keys == 0)
        {
            keys = mem.SystemAlloc(16 * kernel::profile::kAccountKeyCount);
            for (uint32_t k = 0; k < kernel::profile::kAccountKeyCount; k++)
                std::memcpy(mem.Translate(keys + k * 16), kernel::profile::AccountKey(k), 16);
        }
        return keys + index * 16;
    }

    // The sealing key: the console's stand-in key 0x17, or a roamable key.
    struct SealingKey
    {
        uint32_t address;
    };

    void KeyHmac(SealingKey key, uint32_t data, uint32_t size, uint8_t digest[20])
    {
        ShaState inner, outer;
        HmacInit(inner, outer, key.address, 16);
        UpdateFromGuest(inner, data, size);
        HmacFinal(inner, outer, digest);
    }

    void SealKey(SealingKey key, const uint8_t checksum[16], uint8_t rc4Key[16])
    {
        auto& mem = guest::g_memory;
        const uint32_t scratch = mem.SystemAlloc(16);
        std::memcpy(mem.Translate(scratch), checksum, 16);
        uint8_t digest[20];
        KeyHmac(key, scratch, 16, digest);
        mem.SystemFree(scratch);
        std::memcpy(rc4Key, digest, 16);
    }
}

GUEST_IMPORT(XeKeysObfuscate)
{
    auto& mem = guest::g_memory;
    const SealingKey key{ ctx.r3.u32 ? RoamingKey(0) : StandInKey(0x17) };
    const uint32_t input = ctx.r4.u32, size = ctx.r5.u32, out = ctx.r6.u32, outSize = ctx.r7.u32;
    if (!out || !outSize || mem.Read32(outSize) < size + kObfuscateHeader)
    {
        if (outSize)
            mem.Write32(outSize, size + kObfuscateHeader);
        ctx.r3.u64 = kStatusBufferTooSmall;
        return;
    }
    // The confounder (random) and the data at out + 16, the checksum over both at out.
    uint8_t confounder[8];
    BCryptGenRandom(nullptr, confounder, 8, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    std::memmove(mem.Translate(out + kObfuscateHeader), mem.Translate(input), size);
    std::memcpy(mem.Translate(out + 16), confounder, 8);
    uint8_t digest[20], rc4Key[16];
    KeyHmac(key, out + 16, 8 + size, digest);
    std::memcpy(mem.Translate(out), digest, 16);
    SealKey(key, digest, rc4Key);
    Rc4(rc4Key, 16, mem.Translate(out + 16), 8 + size);
    mem.Write32(outSize, size + kObfuscateHeader);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

GUEST_IMPORT(XeKeysUnObfuscate)
{
    auto& mem = guest::g_memory;
    const uint32_t input = ctx.r4.u32, size = ctx.r5.u32, out = ctx.r6.u32, outSize = ctx.r7.u32;
    if (size < kObfuscateHeader || !out || !outSize || mem.Read32(outSize) < size - kObfuscateHeader)
    {
        if (outSize && size >= kObfuscateHeader)
            mem.Write32(outSize, size - kObfuscateHeader);
        ctx.r3.u64 = 0;
        return;
    }
    uint8_t checksum[16];
    std::memcpy(checksum, mem.Translate(input), 16);
    std::vector<SealingKey> keys;
    if (ctx.r3.u32)
        for (uint32_t k = 0; k < kernel::profile::kAccountKeyCount; k++)
            keys.push_back({ RoamingKey(k) });
    else
        keys.push_back({ StandInKey(0x17) });
    const uint32_t scratch = mem.SystemAlloc(size - 16);
    bool valid = false;
    for (const SealingKey key : keys)
    {
        uint8_t rc4Key[16];
        SealKey(key, checksum, rc4Key);
        std::memcpy(mem.Translate(scratch), mem.Translate(input + 16), size - 16);
        Rc4(rc4Key, 16, mem.Translate(scratch), size - 16);
        uint8_t digest[20];
        KeyHmac(key, scratch, size - 16, digest);
        if (std::memcmp(digest, checksum, 16) == 0)
        {
            valid = true;
            break;
        }
    }
    if (valid)
        std::memcpy(mem.Translate(out), mem.Translate(scratch + 8), size - kObfuscateHeader);
    mem.SystemFree(scratch);
    mem.Write32(outSize, size - kObfuscateHeader);
    if (!valid)
        Log("XeKeysUnObfuscate({}, {:#x} bytes) from {:08X}: does not unseal with {}", ctx.r3.u32 ? "roaming" : "console",
            size, uint32_t(ctx.lr), ctx.r3.u32 ? "the retail or development kit account key" : "this console's key");
    ctx.r3.u64 = valid ? 1 : 0; // BOOL (xam.xex 0x81686C80, 0x8179xxxx test it for nonzero)
}

// ---- The key vault's extension (XeKeysEx*): keys XAM generates on the console itself (0x100-0x102: its LIVE
// machine account, 0x81898438) and saves into the key vault. Kept in the data directory (keyvault_ex.bin: id, size,
// bytes per key), which XeKeysExSaveKeyVault writes. ----
namespace
{
    std::mutex g_exMutex;
    std::map<uint32_t, std::vector<uint8_t>> g_exKeys;
    bool g_exLoaded = false;

    std::filesystem::path ExPath() { return DataDirectory() / "keyvault_ex.bin"; }

    void LoadEx()
    {
        if (g_exLoaded)
            return;
        g_exLoaded = true;
        std::ifstream in(ExPath(), std::ios::binary);
        uint32_t header[2];
        while (in.read(reinterpret_cast<char*>(header), sizeof(header)) && header[1] <= 0x1000)
        {
            std::vector<uint8_t> value(header[1]);
            if (!in.read(reinterpret_cast<char*>(value.data()), std::streamsize(value.size())))
                break;
            g_exKeys[header[0]] = std::move(value);
        }
    }
}

// NTSTATUS XeKeysExGetKey(DWORD KeyId, PVOID Buffer, PDWORD Length)
GUEST_IMPORT(XeKeysExGetKey)
{
    auto& mem = guest::g_memory;
    std::lock_guard lock(g_exMutex);
    LoadEx();
    auto it = g_exKeys.find(ctx.r3.u32);
    if (it == g_exKeys.end())
    {
        ctx.r3.u64 = 0xC0000034; // not generated yet: XAM makes it
        return;
    }
    const uint32_t length = ctx.r5.u32 ? mem.Read32(ctx.r5.u32) : 0;
    if (ctx.r5.u32)
        mem.Write32(ctx.r5.u32, uint32_t(it->second.size()));
    if (length < it->second.size() || !ctx.r4.u32)
    {
        ctx.r3.u64 = kStatusBufferTooSmall;
        return;
    }
    std::memcpy(mem.Translate(ctx.r4.u32), it->second.data(), it->second.size());
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS XeKeysExSetKey(DWORD KeyId, const VOID* Buffer, DWORD Length)
GUEST_IMPORT(XeKeysExSetKey)
{
    auto& mem = guest::g_memory;
    std::lock_guard lock(g_exMutex);
    LoadEx();
    std::vector<uint8_t> value(ctx.r5.u32);
    if (ctx.r5.u32 && ctx.r4.u32)
        std::memcpy(value.data(), mem.Translate(ctx.r4.u32), value.size());
    g_exKeys[ctx.r3.u32] = std::move(value);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS XeKeysExSaveKeyVault(...): writes the extension keys out.
GUEST_IMPORT(XeKeysExSaveKeyVault)
{
    std::lock_guard lock(g_exMutex);
    std::ofstream out(ExPath(), std::ios::binary | std::ios::trunc);
    for (const auto& [id, value] : g_exKeys)
    {
        const uint32_t header[2] = { id, uint32_t(value.size()) };
        out.write(reinterpret_cast<const char*>(header), sizeof(header));
        out.write(reinterpret_cast<const char*>(value.data()), std::streamsize(value.size()));
    }
    ctx.r3.u64 = out ? kernel::kStatusSuccess : kernel::kStatusUnsuccessful;
}

// VOID XeCryptHmacMd5(const BYTE* Key, DWORD KeySize, Input1, Size1, Input2, Size2, Input3, Size3, PBYTE Out,
//     DWORD OutSize): HMAC-MD5 over the concatenated inputs (xam.xex's Kerberos RC4-HMAC, 0x81947288).
GUEST_IMPORT(XeCryptHmacMd5)
{
    auto& mem = guest::g_memory;
    BCRYPT_HASH_HANDLE hash = nullptr;
    if (BCryptCreateHash(BCRYPT_HMAC_MD5_ALG_HANDLE, &hash, nullptr, 0,
                         ctx.r4.u32 ? mem.Translate(ctx.r3.u32) : nullptr, ctx.r4.u32, 0) != 0)
        Fatal("XeCryptHmacMd5: no HMAC-MD5");
    const uint32_t inputs[3][2] = { { ctx.r5.u32, ctx.r6.u32 }, { ctx.r7.u32, ctx.r8.u32 }, { ctx.r9.u32, ctx.r10.u32 } };
    for (const auto& [address, size] : inputs)
        if (address && size)
            BCryptHashData(hash, mem.Translate(address), size, 0);
    uint8_t digest[16];
    BCryptFinishHash(hash, digest, 16, 0);
    BCryptDestroyHash(hash);
    const uint32_t out = kernel::StackArg32(ctx, 8), outSize = std::min<uint32_t>(kernel::StackArg32(ctx, 9), 16);
    if (out)
        std::memcpy(mem.Translate(out), digest, outSize);
}

// VOID XeCryptRc4Key(PXECRYPT_RC4_STATE State, const BYTE* Key, DWORD KeySize) and
// VOID XeCryptRc4Ecb(PXECRYPT_RC4_STATE State, PBYTE Data, DWORD Size): RC4 with its state in guest memory
// (XECRYPT_RC4_STATE: S[256], i, j), the stream continuing across calls.
GUEST_IMPORT(XeCryptRc4Key)
{
    auto& mem = guest::g_memory;
    uint8_t* s = mem.Translate(ctx.r3.u32);
    const uint8_t* key = mem.Translate(ctx.r4.u32);
    const uint32_t size = ctx.r5.u32;
    for (int i = 0; i < 256; i++)
        s[i] = uint8_t(i);
    for (int i = 0, j = 0; i < 256 && size; i++)
    {
        j = (j + s[i] + key[i % size]) & 255;
        std::swap(s[i], s[j]);
    }
    s[256] = 0;
    s[257] = 0;
}

GUEST_IMPORT(XeCryptRc4Ecb)
{
    auto& mem = guest::g_memory;
    uint8_t* s = mem.Translate(ctx.r3.u32);
    uint8_t* data = mem.Translate(ctx.r4.u32);
    uint8_t i = s[256], j = s[257];
    for (uint32_t n = 0; n < ctx.r5.u32; n++)
    {
        i = uint8_t(i + 1);
        j = uint8_t(j + s[i]);
        std::swap(s[i], s[j]);
        data[n] ^= s[uint8_t(s[i] + s[j])];
    }
    s[256] = i;
    s[257] = j;
}
