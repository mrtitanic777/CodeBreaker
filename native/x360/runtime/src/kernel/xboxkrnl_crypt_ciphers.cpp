// Kernel crypto exports for MD5, HMAC-MD5, DES / 3DES, AES-128 and the RSA public operation.
//
// MD5 runs over a guest XECRYPT_MD5_STATE (0x54 bytes: byte count, four state words, 64-byte buffer, big-endian
// words like the SHA state in xboxkrnl_crypt.cpp; HMAC-MD5 = inner then outer state) and is checked once against
// Windows' MD5. The block ciphers use Windows' implementations; their guest states hold:
//   XECRYPT_DES_STATE (0x80): the 8-byte key at +0 (the rest zero); XECRYPT_DES3_STATE (0x180): three of them.
//   XECRYPT_AES_STATE (0x160): the AES-128 encryption round keys (11 x 16 bytes, FIPS-197 key expansion; the first is
//   the key) and the equivalent inverse cipher's round keys (InvMixColumns applied, last round first) at +0xB0.
// The DES key layout is ours (no title is known to read a key schedule back); the AES one is the standard schedule.
// Blocks are big-endian byte strings in guest memory, as on the console. CBC feeds are updated in place (the last
// ciphertext block). Reference for signatures: Xenia / rexglue xboxkrnl_crypt (BSD).

#include "kernel/kernel.h"

#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstring>
#include <vector>

namespace
{
    // ---- MD5 (RFC 1321) --------------------------------------------------------------------------------------------

    constexpr uint32_t kMd5StateSize = 0x54;

    struct Md5State
    {
        uint32_t count = 0;
        uint32_t h[4] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476 };
        uint8_t buffer[64] = {};
    };

    Md5State LoadMd5(uint32_t address)
    {
        auto& mem = guest::g_memory;
        Md5State s;
        s.count = mem.Read32(address);
        for (int i = 0; i < 4; i++)
            s.h[i] = mem.Read32(address + 4 + i * 4);
        std::memcpy(s.buffer, mem.Translate(address + 0x14), 64);
        return s;
    }

    void StoreMd5(uint32_t address, const Md5State& s)
    {
        auto& mem = guest::g_memory;
        mem.Write32(address, s.count);
        for (int i = 0; i < 4; i++)
            mem.Write32(address + 4 + i * 4, s.h[i]);
        std::memcpy(mem.Translate(address + 0x14), s.buffer, 64);
    }

    uint32_t Rol(uint32_t v, int n) { return (v << n) | (v >> (32 - n)); }

    void Md5Block(uint32_t h[4], const uint8_t block[64])
    {
        // K[i] = floor(|sin(i + 1)| * 2^32) (RFC 1321 3.4).
        static const std::array<uint32_t, 64> k = [] {
            std::array<uint32_t, 64> t{};
            for (int i = 0; i < 64; i++)
                t[i] = uint32_t(std::fabs(std::sin(double(i + 1))) * 4294967296.0);
            return t;
        }();
        static const int r[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                                   5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
                                   4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                                   6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };
        uint32_t m[16];
        for (int i = 0; i < 16; i++)
            m[i] = uint32_t(block[i * 4]) | uint32_t(block[i * 4 + 1]) << 8 | uint32_t(block[i * 4 + 2]) << 16 |
                   uint32_t(block[i * 4 + 3]) << 24;
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
        for (int i = 0; i < 64; i++)
        {
            uint32_t f;
            int g;
            if (i < 16)
            {
                f = (b & c) | (~b & d);
                g = i;
            }
            else if (i < 32)
            {
                f = (d & b) | (~d & c);
                g = (5 * i + 1) % 16;
            }
            else if (i < 48)
            {
                f = b ^ c ^ d;
                g = (3 * i + 5) % 16;
            }
            else
            {
                f = c ^ (b | ~d);
                g = (7 * i) % 16;
            }
            const uint32_t t = d;
            d = c;
            c = b;
            b = b + Rol(a + f + k[i] + m[g], r[i]);
            a = t;
        }
        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
    }

    void Md5Update(Md5State& s, const uint8_t* data, size_t size)
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
                Md5Block(s.h, s.buffer);
                used = 0;
            }
        }
    }

    void Md5Final(Md5State& s, uint8_t digest[16])
    {
        size_t used = s.count % 64;
        const uint64_t bits = uint64_t(s.count) * 8;
        s.buffer[used++] = 0x80;
        if (used > 56)
        {
            std::memset(s.buffer + used, 0, 64 - used);
            Md5Block(s.h, s.buffer);
            used = 0;
        }
        std::memset(s.buffer + used, 0, 56 - used);
        for (int i = 0; i < 8; i++)
            s.buffer[56 + i] = uint8_t(bits >> (8 * i)); // little-endian length
        Md5Block(s.h, s.buffer);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                digest[i * 4 + j] = uint8_t(s.h[i] >> (8 * j));
    }

    // Once: the incremental MD5 against Windows' on a 3-block message in uneven pieces.
    void CheckMd5()
    {
        static const bool ok = [] {
            uint8_t message[150];
            for (int i = 0; i < 150; i++)
                message[i] = uint8_t(i * 11 + 5);
            Md5State s;
            Md5Update(s, message, 3);
            Md5Update(s, message + 3, 64);
            Md5Update(s, message + 67, 83);
            uint8_t mine[16], reference[16];
            Md5Final(s, mine);
            BCRYPT_HASH_HANDLE hash = nullptr;
            if (!BCRYPT_SUCCESS(BCryptCreateHash(BCRYPT_MD5_ALG_HANDLE, &hash, nullptr, 0, nullptr, 0, 0)))
                return false;
            BCryptHashData(hash, message, sizeof(message), 0);
            BCryptFinishHash(hash, reference, 16, 0);
            BCryptDestroyHash(hash);
            return std::memcmp(mine, reference, 16) == 0;
        }();
        if (!ok)
            Fatal("kernel crypto: the incremental MD5 does not match Windows' MD5");
    }

    void Md5FromGuest(Md5State& s, uint32_t address, uint32_t size)
    {
        if (address && size)
            Md5Update(s, guest::g_memory.Translate(address), size);
    }

    void WriteOut(const uint8_t* digest, uint32_t digestSize, uint32_t out, uint32_t outSize)
    {
        if (out)
            std::memcpy(guest::g_memory.Translate(out), digest, std::min(outSize, digestSize));
    }

    void HmacMd5Init(Md5State& inner, Md5State& outer, uint32_t key, uint32_t keySize)
    {
        CheckMd5();
        uint8_t k[64] = {};
        if (keySize > 64)
        {
            Md5State hashed;
            Md5FromGuest(hashed, key, keySize);
            Md5Final(hashed, k);
        }
        else if (keySize)
            std::memcpy(k, guest::g_memory.Translate(key), keySize);
        uint8_t ipad[64], opad[64];
        for (int i = 0; i < 64; i++)
        {
            ipad[i] = k[i] ^ 0x36;
            opad[i] = k[i] ^ 0x5C;
        }
        inner = Md5State{};
        outer = Md5State{};
        Md5Update(inner, ipad, 64);
        Md5Update(outer, opad, 64);
    }

    // ---- Block ciphers through Windows ------------------------------------------------------------------------

    // One ECB operation on whole blocks.
    void Ecb(BCRYPT_ALG_HANDLE algorithm, const uint8_t* key, ULONG keySize, const uint8_t* in, uint8_t* out, ULONG size,
             bool encrypt)
    {
        BCRYPT_KEY_HANDLE handle = nullptr;
        NTSTATUS status = BCryptGenerateSymmetricKey(algorithm, &handle, nullptr, 0, const_cast<PUCHAR>(key), keySize, 0);
        if (!BCRYPT_SUCCESS(status))
            Fatal("kernel crypto: BCryptGenerateSymmetricKey failed ({:08X})", uint32_t(status));
        std::vector<uint8_t> buffer(in, in + size);
        ULONG done = 0;
        status = encrypt ? BCryptEncrypt(handle, buffer.data(), size, nullptr, nullptr, 0, out, size, &done, 0)
                         : BCryptDecrypt(handle, buffer.data(), size, nullptr, nullptr, 0, out, size, &done, 0);
        BCryptDestroyKey(handle);
        if (!BCRYPT_SUCCESS(status) || done != size)
            Fatal("kernel crypto: block cipher operation failed ({:08X})", uint32_t(status));
    }

    BCRYPT_ALG_HANDLE EcbAlgorithm(const wchar_t* name)
    {
        BCRYPT_ALG_HANDLE algorithm = nullptr;
        if (!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(&algorithm, name, nullptr, 0)) ||
            !BCRYPT_SUCCESS(BCryptSetProperty(algorithm, BCRYPT_CHAINING_MODE,
                                              reinterpret_cast<PUCHAR>(const_cast<wchar_t*>(BCRYPT_CHAIN_MODE_ECB)),
                                              sizeof(BCRYPT_CHAIN_MODE_ECB), 0)))
            Fatal("kernel crypto: cannot open {} in ECB mode", std::string(name, name + wcslen(name)));
        return algorithm;
    }

    BCRYPT_ALG_HANDLE Des() { static const BCRYPT_ALG_HANDLE a = EcbAlgorithm(BCRYPT_DES_ALGORITHM); return a; }
    BCRYPT_ALG_HANDLE Des3() { static const BCRYPT_ALG_HANDLE a = EcbAlgorithm(BCRYPT_3DES_ALGORITHM); return a; }
    BCRYPT_ALG_HANDLE Aes() { static const BCRYPT_ALG_HANDLE a = EcbAlgorithm(BCRYPT_AES_ALGORITHM); return a; }

    // CBC over whole blocks with `block` = 8 or 16: encrypt chains the previous ciphertext into each input.
    void Cbc(BCRYPT_ALG_HANDLE algorithm, const uint8_t* key, ULONG keySize, size_t block, uint32_t in, uint32_t size,
             uint32_t out, uint32_t feed, bool encrypt, const char* name)
    {
        auto& mem = guest::g_memory;
        if (size % block)
            Fatal("{}: {} bytes (not whole {}-byte blocks)", name, size, block);
        uint8_t chain[16], buffer[16], result[16];
        std::memcpy(chain, mem.Translate(feed), block);
        for (uint32_t offset = 0; offset < size; offset += uint32_t(block))
        {
            std::memcpy(buffer, mem.Translate(in + offset), block);
            if (encrypt)
            {
                for (size_t i = 0; i < block; i++)
                    buffer[i] ^= chain[i];
                Ecb(algorithm, key, keySize, buffer, result, ULONG(block), true);
                std::memcpy(chain, result, block);
            }
            else
            {
                Ecb(algorithm, key, keySize, buffer, result, ULONG(block), false);
                for (size_t i = 0; i < block; i++)
                    result[i] ^= chain[i];
                std::memcpy(chain, buffer, block);
            }
            std::memcpy(mem.Translate(out + offset), result, block);
        }
        std::memcpy(mem.Translate(feed), chain, block);
    }

    // ---- AES-128 key schedule (FIPS-197) -----------------------------------------------------------------------

    uint8_t Mul(uint8_t a, uint8_t b)
    {
        uint8_t r = 0;
        while (b)
        {
            if (b & 1)
                r ^= a;
            a = uint8_t((a << 1) ^ (a & 0x80 ? 0x1B : 0));
            b >>= 1;
        }
        return r;
    }

    const std::array<uint8_t, 256>& SBox()
    {
        static const std::array<uint8_t, 256> box = [] {
            std::array<uint8_t, 256> s{};
            for (int x = 0; x < 256; x++)
            {
                uint8_t inverse = 0;
                for (int y = 1; y < 256 && x; y++)
                    if (Mul(uint8_t(x), uint8_t(y)) == 1)
                    {
                        inverse = uint8_t(y);
                        break;
                    }
                uint8_t b = inverse, r = inverse;
                for (int i = 0; i < 4; i++)
                {
                    b = uint8_t((b << 1) | (b >> 7));
                    r ^= b;
                }
                s[x] = r ^ 0x63;
            }
            return s;
        }();
        return box;
    }

    // keys[0..10]: the encryption round keys; inverse[0..10]: the equivalent inverse cipher's (InvMixColumns of
    // round keys 9..1 between the last and the first).
    void AesSchedule(const uint8_t key[16], uint8_t keys[11][16], uint8_t inverse[11][16])
    {
        const auto& s = SBox();
        std::memcpy(keys[0], key, 16);
        uint8_t rcon = 1;
        for (int round = 1; round <= 10; round++)
        {
            const uint8_t* p = keys[round - 1];
            uint8_t t[4] = { s[p[13]], s[p[14]], s[p[15]], s[p[12]] };
            t[0] ^= rcon;
            rcon = Mul(rcon, 2);
            for (int i = 0; i < 4; i++)
                keys[round][i] = p[i] ^ t[i];
            for (int i = 4; i < 16; i++)
                keys[round][i] = p[i] ^ keys[round][i - 4];
        }
        std::memcpy(inverse[0], keys[10], 16);
        for (int round = 1; round < 10; round++)
        {
            const uint8_t* k = keys[10 - round];
            for (int c = 0; c < 4; c++)
            {
                const uint8_t* a = k + c * 4;
                uint8_t* o = inverse[round] + c * 4;
                o[0] = Mul(a[0], 14) ^ Mul(a[1], 11) ^ Mul(a[2], 13) ^ Mul(a[3], 9);
                o[1] = Mul(a[0], 9) ^ Mul(a[1], 14) ^ Mul(a[2], 11) ^ Mul(a[3], 13);
                o[2] = Mul(a[0], 13) ^ Mul(a[1], 9) ^ Mul(a[2], 14) ^ Mul(a[3], 11);
                o[3] = Mul(a[0], 11) ^ Mul(a[1], 13) ^ Mul(a[2], 9) ^ Mul(a[3], 14);
            }
        }
        std::memcpy(inverse[10], keys[0], 16);
    }
}

// ---- MD5 ----------------------------------------------------------------------------------------------------------

// VOID XeCryptMd5Init(PXECRYPT_MD5_STATE State)
GUEST_IMPORT(XeCryptMd5Init)
{
    CheckMd5();
    StoreMd5(ctx.r3.u32, Md5State{});
}

// VOID XeCryptMd5Update(PXECRYPT_MD5_STATE State, const BYTE* Input, DWORD InputSize)
GUEST_IMPORT(XeCryptMd5Update)
{
    Md5State s = LoadMd5(ctx.r3.u32);
    Md5FromGuest(s, ctx.r4.u32, ctx.r5.u32);
    StoreMd5(ctx.r3.u32, s);
}

// VOID XeCryptMd5Final(PXECRYPT_MD5_STATE State, PBYTE Out, DWORD OutSize): the digest's first OutSize (<= 16) bytes.
GUEST_IMPORT(XeCryptMd5Final)
{
    Md5State s = LoadMd5(ctx.r3.u32);
    uint8_t digest[16];
    Md5Final(s, digest);
    StoreMd5(ctx.r3.u32, s);
    WriteOut(digest, 16, ctx.r4.u32, ctx.r5.u32);
}

// VOID XeCryptMd5(Input1, Size1, Input2, Size2, Input3, Size3, PBYTE Out, DWORD OutSize)
GUEST_IMPORT(XeCryptMd5)
{
    CheckMd5();
    Md5State s;
    Md5FromGuest(s, ctx.r3.u32, ctx.r4.u32);
    Md5FromGuest(s, ctx.r5.u32, ctx.r6.u32);
    Md5FromGuest(s, ctx.r7.u32, ctx.r8.u32);
    uint8_t digest[16];
    Md5Final(s, digest);
    WriteOut(digest, 16, ctx.r9.u32, ctx.r10.u32);
}

// VOID XeCryptHmacMd5Init(PXECRYPT_HMACMD5_STATE State, const BYTE* Key, DWORD KeySize)
GUEST_IMPORT(XeCryptHmacMd5Init)
{
    Md5State inner, outer;
    HmacMd5Init(inner, outer, ctx.r4.u32, ctx.r5.u32);
    StoreMd5(ctx.r3.u32, inner);
    StoreMd5(ctx.r3.u32 + kMd5StateSize, outer);
}

// VOID XeCryptHmacMd5Update(PXECRYPT_HMACMD5_STATE State, const BYTE* Input, DWORD InputSize)
GUEST_IMPORT(XeCryptHmacMd5Update)
{
    Md5State inner = LoadMd5(ctx.r3.u32);
    Md5FromGuest(inner, ctx.r4.u32, ctx.r5.u32);
    StoreMd5(ctx.r3.u32, inner);
}

// VOID XeCryptHmacMd5Final(PXECRYPT_HMACMD5_STATE State, PBYTE Out, DWORD OutSize)
GUEST_IMPORT(XeCryptHmacMd5Final)
{
    Md5State inner = LoadMd5(ctx.r3.u32), outer = LoadMd5(ctx.r3.u32 + kMd5StateSize);
    uint8_t innerDigest[16], digest[16];
    Md5Final(inner, innerDigest);
    Md5Update(outer, innerDigest, 16);
    Md5Final(outer, digest);
    StoreMd5(ctx.r3.u32, inner);
    StoreMd5(ctx.r3.u32 + kMd5StateSize, outer);
    WriteOut(digest, 16, ctx.r4.u32, ctx.r5.u32);
}

// ---- DES / 3DES ---------------------------------------------------------------------------------------------------

// VOID XeCryptDesParity(const BYTE* Input, DWORD InputSize, PBYTE Output): each byte with bit 0 set for odd parity.
GUEST_IMPORT(XeCryptDesParity)
{
    auto& mem = guest::g_memory;
    for (uint32_t i = 0; i < ctx.r4.u32; i++)
    {
        uint8_t b = mem.Read8(ctx.r3.u32 + i) & 0xFE;
        b |= (std::popcount(unsigned(b)) & 1) ? 0 : 1;
        mem.Write8(ctx.r5.u32 + i, b);
    }
}

// VOID XeCryptDesKey(PXECRYPT_DES_STATE State, const BYTE* Key): 8 bytes.
GUEST_IMPORT(XeCryptDesKey)
{
    auto& mem = guest::g_memory;
    std::memset(mem.Translate(ctx.r3.u32), 0, 0x80);
    std::memcpy(mem.Translate(ctx.r3.u32), mem.Translate(ctx.r4.u32), 8);
}

// VOID XeCryptDesEcb(PXECRYPT_DES_STATE State, const BYTE* Input, PBYTE Output, BOOL Encrypt): one 8-byte block.
GUEST_IMPORT(XeCryptDesEcb)
{
    auto& mem = guest::g_memory;
    uint8_t block[8];
    Ecb(Des(), mem.Translate(ctx.r3.u32), 8, mem.Translate(ctx.r4.u32), block, 8, ctx.r6.u32 != 0);
    std::memcpy(mem.Translate(ctx.r5.u32), block, 8);
}

// VOID XeCryptDes3Key(PXECRYPT_DES3_STATE State, const BYTE* Key): 24 bytes (three DES keys).
GUEST_IMPORT(XeCryptDes3Key)
{
    auto& mem = guest::g_memory;
    std::memset(mem.Translate(ctx.r3.u32), 0, 0x180);
    for (uint32_t i = 0; i < 3; i++)
        std::memcpy(mem.Translate(ctx.r3.u32 + i * 0x80), mem.Translate(ctx.r4.u32 + i * 8), 8);
}

// VOID XeCryptDes3Cbc(PXECRYPT_DES3_STATE State, const BYTE* Input, DWORD InputSize, PBYTE Output, PBYTE Feed,
//                     BOOL Encrypt)
GUEST_IMPORT(XeCryptDes3Cbc)
{
    auto& mem = guest::g_memory;
    uint8_t key[24];
    for (uint32_t i = 0; i < 3; i++)
        std::memcpy(key + i * 8, mem.Translate(ctx.r3.u32 + i * 0x80), 8);
    Cbc(Des3(), key, 24, 8, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32 != 0, "XeCryptDes3Cbc");
}

// ---- AES-128 ------------------------------------------------------------------------------------------------------

// VOID XeCryptAesKey(PXECRYPT_AES_STATE State, const BYTE* Key): 16 bytes.
GUEST_IMPORT(XeCryptAesKey)
{
    auto& mem = guest::g_memory;
    uint8_t keys[11][16], inverse[11][16];
    AesSchedule(mem.Translate(ctx.r4.u32), keys, inverse);
    std::memcpy(mem.Translate(ctx.r3.u32), keys, sizeof(keys));
    std::memcpy(mem.Translate(ctx.r3.u32 + 0xB0), inverse, sizeof(inverse));
}

// VOID XeCryptAesCbc(PXECRYPT_AES_STATE State, const BYTE* Input, DWORD InputSize, PBYTE Output, PBYTE Feed,
//                    BOOL Encrypt): the key is the state's first round key.
GUEST_IMPORT(XeCryptAesCbc)
{
    auto& mem = guest::g_memory;
    uint8_t key[16];
    std::memcpy(key, mem.Translate(ctx.r3.u32), 16);
    Cbc(Aes(), key, 16, 16, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32 != 0, "XeCryptAesCbc");
}

// ---- Big numbers --------------------------------------------------------------------------------------------------

// VOID XeCryptBnQw_SwapDwQwLeBe(const QWORD* Input, QWORD* Output, DWORD Count): each qword byte-swapped.
GUEST_IMPORT(XeCryptBnQw_SwapDwQwLeBe)
{
    auto& mem = guest::g_memory;
    for (uint32_t i = 0; i < ctx.r5.u32; i++)
        mem.Write64(ctx.r4.u32 + i * 8, __builtin_bswap64(mem.Read64(ctx.r3.u32 + i * 8)));
}

// BOOL XeCryptBnQwNeRsaPubCrypt(const QWORD* Input, QWORD* Output, PXECRYPT_RSA Key): Output = Input ^ e mod N.
// Numbers are arrays of qwords, least significant first; the key is XECRYPT_RSA (qword count, public exponent, pad,
// modulus). FALSE when the input is not below the modulus.
GUEST_IMPORT(XeCryptBnQwNeRsaPubCrypt)
{
    auto& mem = guest::g_memory;
    const uint32_t input = ctx.r3.u32, output = ctx.r4.u32, key = ctx.r5.u32;
    const uint32_t cqw = mem.Read32(key), exponent = mem.Read32(key + 4);
    if (cqw == 0 || cqw > 64)
        Fatal("XeCryptBnQwNeRsaPubCrypt: a key of {} qwords", cqw);
    const uint32_t bytes = cqw * 8;
    // Big-endian byte strings for Windows: the most significant qword first.
    std::vector<uint8_t> modulus(bytes), value(bytes);
    for (uint32_t i = 0; i < cqw; i++)
        for (uint32_t b = 0; b < 8; b++)
        {
            modulus[(cqw - 1 - i) * 8 + b] = mem.Read8(key + 0x10 + i * 8 + b);
            value[(cqw - 1 - i) * 8 + b] = mem.Read8(input + i * 8 + b);
        }
    if (!std::lexicographical_compare(value.begin(), value.end(), modulus.begin(), modulus.end()))
    {
        ctx.r3.u64 = 0;
        return;
    }
    uint8_t e[4] = { uint8_t(exponent >> 24), uint8_t(exponent >> 16), uint8_t(exponent >> 8), uint8_t(exponent) };
    const uint8_t* eStart = e;
    while (eStart < e + 3 && *eStart == 0)
        eStart++;
    const ULONG eSize = ULONG(e + 4 - eStart);
    std::vector<uint8_t> blob(sizeof(BCRYPT_RSAKEY_BLOB) + eSize + bytes);
    auto* header = reinterpret_cast<BCRYPT_RSAKEY_BLOB*>(blob.data());
    header->Magic = BCRYPT_RSAPUBLIC_MAGIC;
    header->BitLength = bytes * 8;
    header->cbPublicExp = eSize;
    header->cbModulus = bytes;
    std::memcpy(blob.data() + sizeof(BCRYPT_RSAKEY_BLOB), eStart, eSize);
    std::memcpy(blob.data() + sizeof(BCRYPT_RSAKEY_BLOB) + eSize, modulus.data(), bytes);
    BCRYPT_KEY_HANDLE handle = nullptr;
    NTSTATUS status = BCryptImportKeyPair(BCRYPT_RSA_ALG_HANDLE, nullptr, BCRYPT_RSAPUBLIC_BLOB, &handle, blob.data(),
                                          ULONG(blob.size()), 0);
    if (!BCRYPT_SUCCESS(status))
        Fatal("XeCryptBnQwNeRsaPubCrypt: Windows refused the {}-bit key ({:08X})", bytes * 8, uint32_t(status));
    std::vector<uint8_t> result(bytes);
    ULONG done = 0;
    status = BCryptEncrypt(handle, value.data(), bytes, nullptr, nullptr, 0, result.data(), bytes, &done, BCRYPT_PAD_NONE);
    BCryptDestroyKey(handle);
    if (!BCRYPT_SUCCESS(status) || done != bytes)
        Fatal("XeCryptBnQwNeRsaPubCrypt: the RSA operation failed ({:08X})", uint32_t(status));
    for (uint32_t i = 0; i < cqw; i++)
        for (uint32_t b = 0; b < 8; b++)
            mem.Write8(output + i * 8 + b, result[(cqw - 1 - i) * 8 + b]);
    ctx.r3.u64 = 1;
}
