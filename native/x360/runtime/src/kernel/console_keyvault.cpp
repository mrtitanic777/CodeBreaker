// The user's console keyvault (kernel/console_keyvault.h). From a flash dump: the flash without its ECC spare data
// (0x10 bytes after every 0x200-byte page in a raw small-block NAND dump), the keyvault where the flash header says
// (offset 0x6C, size 0x60: 0x4000 / 0x4000), [0, 0x10) its digest, RC4 key = HMAC-SHA1(CPU key, digest)[0..16] over
// [0x10, size), and the digest must equal HMAC-SHA1(CPU key, decrypted [0x10, size) + 07 12)[0..16]: a CPU key of
// another console stops the run (tools/scripts/nand_keyvault.py checks a dump the same way).

#include "kernel/console_keyvault.h"

#include "log.h"
#include "settings.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iterator>

namespace
{
    std::vector<uint8_t> ReadFileBytes(const std::filesystem::path& path)
    {
        std::ifstream in(path, std::ios::binary);
        return std::vector<uint8_t>((std::istreambuf_iterator<char>(in)), {});
    }

    void HmacSha1(const std::vector<uint8_t>& key, std::initializer_list<std::pair<const uint8_t*, size_t>> parts,
                  uint8_t out[20])
    {
        BCRYPT_HASH_HANDLE hash = nullptr;
        if (BCryptCreateHash(BCRYPT_HMAC_SHA1_ALG_HANDLE, &hash, nullptr, 0, const_cast<PUCHAR>(key.data()),
                             ULONG(key.size()), 0) != 0)
            Fatal("console keyvault: no HMAC-SHA1");
        for (const auto& [data, size] : parts)
            BCryptHashData(hash, const_cast<PUCHAR>(data), ULONG(size), 0);
        BCryptFinishHash(hash, out, 20, 0);
        BCryptDestroyHash(hash);
    }

    void Rc4(const uint8_t* key, size_t keySize, uint8_t* data, size_t size)
    {
        uint8_t s[256];
        for (int i = 0; i < 256; i++)
            s[i] = uint8_t(i);
        for (int i = 0, j = 0; i < 256; i++)
        {
            j = (j + s[i] + key[i % keySize]) & 255;
            std::swap(s[i], s[j]);
        }
        for (size_t n = 0, i = 0, j = 0; n < size; n++)
        {
            i = (i + 1) & 255;
            j = (j + s[i]) & 255;
            std::swap(s[i], s[j]);
            data[n] ^= s[(s[i] + s[j]) & 255];
        }
    }

    uint32_t Be32(const std::vector<uint8_t>& b, size_t at)
    {
        return uint32_t(b[at]) << 24 | uint32_t(b[at + 1]) << 16 | uint32_t(b[at + 2]) << 8 | b[at + 3];
    }

    // The CPU key: 32 hex digits in the setting itself or in the file it names (cpukey.txt).
    std::vector<uint8_t> CpuKey(const std::string& setting)
    {
        std::string text = setting;
        std::error_code ec;
        if (std::filesystem::is_regular_file(std::filesystem::path(setting), ec))
        {
            const std::vector<uint8_t> bytes = ReadFileBytes(setting);
            text.assign(bytes.begin(), bytes.end());
        }
        std::string hex;
        for (char c : text)
            if (std::isxdigit(static_cast<unsigned char>(c)))
                hex += c;
        if (hex.size() < 32)
            Fatal("console_cpu_key: not a CPU key (32 hex digits, or the path of cpukey.txt)");
        std::vector<uint8_t> key(16);
        for (size_t i = 0; i < 16; i++)
            key[i] = uint8_t(std::stoi(hex.substr(i * 2, 2), nullptr, 16));
        return key;
    }

    std::vector<uint8_t> FromFlash(const std::string& flashPath, const std::string& cpuKeySetting)
    {
        std::vector<uint8_t> raw = ReadFileBytes(flashPath);
        if (raw.size() < 0x10000)
            Fatal("console_flash {}: not a flash dump ({} bytes)", flashPath, raw.size());
        // A raw small-block dump: 0x200 data bytes then 0x10 spare bytes per page.
        const bool spare = raw.size() % 0x210 == 0 && (raw.size() % 0x200 != 0 || raw.size() == 0x1080000 ||
                                                       raw.size() == 0x4200000 || raw.size() == 0x8400000);
        std::vector<uint8_t> flash;
        if (spare)
        {
            flash.reserve(raw.size() / 0x210 * 0x200);
            for (size_t page = 0; page + 0x210 <= raw.size(); page += 0x210)
                flash.insert(flash.end(), raw.begin() + page, raw.begin() + page + 0x200);
        }
        else
            flash = std::move(raw);
        if (flash[0] != 0xFF || flash[1] != 0x4F)
            Fatal("console_flash {}: not an Xbox 360 flash image (no 0xFF4F header)", flashPath);
        const uint32_t size = Be32(flash, 0x60), offset = Be32(flash, 0x6C);
        if (size != 0x4000 || uint64_t(offset) + size > flash.size())
            Fatal("console_flash {}: keyvault at {:#x}, {:#x} bytes: not a keyvault this runtime reads", flashPath,
                  offset, size);
        std::vector<uint8_t> kv(flash.begin() + offset, flash.begin() + offset + size);
        SecureZeroMemory(flash.data(), flash.size());
        const std::vector<uint8_t> cpu = CpuKey(cpuKeySetting);
        uint8_t digest[20];
        HmacSha1(cpu, { { kv.data(), 0x10 } }, digest);
        Rc4(digest, 0x10, kv.data() + 0x10, kv.size() - 0x10);
        static const uint8_t kSuffix[2] = { 0x07, 0x12 };
        uint8_t check[20];
        HmacSha1(cpu, { { kv.data() + 0x10, kv.size() - 0x10 }, { kSuffix, 2 } }, check);
        if (!std::equal(check, check + 0x10, kv.begin()))
            Fatal("console_flash {}: the keyvault does not decrypt with console_cpu_key (the CPU key of another "
                  "console?)", flashPath);
        return kv;
    }

    struct Loaded
    {
        std::vector<uint8_t> kv;
        std::string source;
    };

    const Loaded& Load()
    {
        static const Loaded loaded = [] {
            Loaded l;
            const ConsoleSettings& s = Settings();
            if (!s.consoleKeyvault.empty())
            {
                l.kv = ReadFileBytes(s.consoleKeyvault);
                if (l.kv.size() != 0x4000)
                    Fatal("console_keyvault {}: {} bytes, not a decrypted keyvault (0x4000 bytes)", s.consoleKeyvault,
                          l.kv.size());
                l.source = "console_keyvault " + s.consoleKeyvault;
            }
            else if (!s.consoleFlash.empty())
            {
                if (s.consoleCpuKey.empty())
                    Fatal("console_flash {}: console_cpu_key is needed too (the flash's keyvault is encrypted with "
                          "the console's CPU key)", s.consoleFlash);
                l.kv = FromFlash(s.consoleFlash, s.consoleCpuKey);
                l.source = "console_flash " + s.consoleFlash;
            }
            if (!l.kv.empty())
            {
                if (l.kv[0x9C8] != 0x01 || l.kv[0x9C9] != 0xA8)
                    Fatal("{}: no console certificate in the keyvault", l.source);
                Log("Console keyvault: from {}", l.source);
            }
            return l;
        }();
        return loaded;
    }
}

namespace kernel
{
    const std::vector<uint8_t>& ConsoleKeyvault() { return Load().kv; }
    const std::string& ConsoleKeyvaultSource() { return Load().source; }
}
