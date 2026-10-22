#include "kernel/profile_store.h"
#include "kernel/profile_location.h"

#include "log.h"
#include "settings.h"

#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <map>
#include <mutex>

namespace fs = std::filesystem;

namespace kernel::profile
{
    namespace
    {
        constexpr uint32_t kMagicXdbf = 0x58444246; // "XDBF"
        constexpr uint32_t kVersion = 0x00010000;
        constexpr uint32_t kTableCapacity = 512;    // entry and free tables grow in steps of 512
        constexpr uint16_t kSectionAchievement = 1;
        constexpr uint16_t kSectionSetting = 3;
        constexpr size_t kAchievementHeaderSize = 0x1C;
        constexpr size_t kSettingHeaderSize = 0x18;

        // A GPD in memory: entries by (section, id).
        using Entries = std::map<std::pair<uint16_t, uint64_t>, std::vector<uint8_t>>;

        std::mutex g_mutex;
        std::map<std::pair<uint64_t, uint32_t>, Entries> g_gpds; // (XUID, title ID) -> entries (loaded on first use)

        uint32_t Be32(const uint8_t* p) { return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3]; }
        uint16_t Be16(const uint8_t* p) { return uint16_t(p[0] << 8 | p[1]); }

        void Put32(std::vector<uint8_t>& v, uint32_t x)
        {
            for (int i = 0; i < 4; i++)
                v.push_back(uint8_t(x >> (24 - 8 * i)));
        }

        // The profile's package folder on whichever device holds it (kernel/profile_location.h).
        fs::path ProfileDirectory(uint64_t xuid)
        {
            return kernel::profile_location::Folder(xuid);
        }

        fs::path GpdPath(uint64_t xuid, uint32_t titleId)
        {
            return ProfileDirectory(xuid) / std::format("{:08X}.gpd", titleId);
        }

        Entries Load(uint64_t xuid, uint32_t titleId)
        {
            Entries entries;
            std::ifstream in(GpdPath(xuid, titleId), std::ios::binary);
            if (!in)
                return entries;
            const std::vector<uint8_t> file((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            if (file.size() < 24 || Be32(file.data()) != kMagicXdbf)
                Fatal("{} is not an XDBF file", GpdPath(xuid, titleId).string());
            const uint32_t entryCount = Be32(&file[8]), used = Be32(&file[12]), freeCount = Be32(&file[16]);
            const uint64_t dataStart = 24 + uint64_t(entryCount) * 18 + uint64_t(freeCount) * 8;
            for (uint32_t i = 0; i < used; i++)
            {
                const uint8_t* e = &file[24 + i * 18];
                const uint16_t section = Be16(e);
                const uint64_t id = uint64_t(Be32(e + 2)) << 32 | Be32(e + 6);
                const uint32_t offset = Be32(e + 10), size = Be32(e + 14);
                if (dataStart + offset + size > file.size())
                    Fatal("{}: entry ({}, {:X}) lies outside the file", GpdPath(xuid, titleId).string(), section, id);
                entries[{ section, id }].assign(file.begin() + dataStart + offset, file.begin() + dataStart + offset + size);
            }
            return entries;
        }

        Entries& Get(uint64_t xuid, uint32_t titleId)
        {
            if (xuid == 0)
                Fatal("Profile store: no profile (XUID 0) for title {:08X}", titleId);
            auto it = g_gpds.find({ xuid, titleId });
            if (it == g_gpds.end())
                it = g_gpds.emplace(std::pair{ xuid, titleId }, Load(xuid, titleId)).first;
            return it->second;
        }

        // Rewrites the GPD: entries sorted by section then id, data packed in that order, one free-space entry after
        // the data (the remainder of the 32-bit space), tables sized in steps of 512 as Xenia writes them.
        void Save(uint64_t xuid, uint32_t titleId, const Entries& entries)
        {
            const uint32_t capacity = std::max<uint32_t>(kTableCapacity,
                uint32_t((entries.size() + kTableCapacity - 1) / kTableCapacity * kTableCapacity));
            std::vector<uint8_t> out;
            Put32(out, kMagicXdbf);
            Put32(out, kVersion);
            Put32(out, capacity);
            Put32(out, uint32_t(entries.size()));
            Put32(out, capacity);
            Put32(out, 1);
            uint32_t offset = 0;
            for (const auto& [key, data] : entries)
            {
                out.push_back(uint8_t(key.first >> 8));
                out.push_back(uint8_t(key.first));
                Put32(out, uint32_t(key.second >> 32));
                Put32(out, uint32_t(key.second));
                Put32(out, offset);
                Put32(out, uint32_t(data.size()));
                offset += uint32_t(data.size());
            }
            out.resize(24 + size_t(capacity) * 18, 0);
            Put32(out, offset);
            Put32(out, 0xFFFFFFFF - offset);
            out.resize(24 + size_t(capacity) * 18 + size_t(capacity) * 8, 0);
            for (const auto& [key, data] : entries)
                out.insert(out.end(), data.begin(), data.end());

            const fs::path path = GpdPath(xuid, titleId);
            fs::create_directories(path.parent_path());
            const fs::path temporary = path.string() + ".tmp";
            {
                std::ofstream file(temporary, std::ios::binary | std::ios::trunc);
                file.write(reinterpret_cast<const char*>(out.data()), std::streamsize(out.size()));
                if (!file)
                    Fatal("Cannot write {}", temporary.string());
            }
            fs::rename(temporary, path);
            kernel::profile_location::Changed(xuid); // an STFS profile package gets the change written back
        }
    }

    std::optional<std::vector<uint8_t>> ReadEntry(uint64_t xuid, uint32_t titleId, uint16_t section, uint64_t id)
    {
        std::lock_guard lock(g_mutex);
        const Entries& entries = Get(xuid, titleId);
        const auto it = entries.find({ section, id });
        if (it == entries.end())
            return std::nullopt;
        return it->second;
    }

    std::optional<Setting> ReadSetting(uint64_t xuid, uint32_t titleId, uint32_t settingId)
    {
        std::lock_guard lock(g_mutex);
        const Entries& entries = Get(xuid, titleId);
        const auto it = entries.find({ kSectionSetting, settingId });
        if (it == entries.end())
            return std::nullopt;
        const std::vector<uint8_t>& d = it->second;
        if (d.size() < kSettingHeaderSize)
            Fatal("{}: setting {:08X} is truncated", GpdPath(xuid, titleId).string(), settingId);
        Setting s;
        s.type = d[8];
        std::memcpy(s.value, &d[0x10], 8);
        s.extended.assign(d.begin() + kSettingHeaderSize, d.end());
        return s;
    }

    void WriteSetting(uint64_t xuid, uint32_t titleId, uint32_t settingId, const Setting& setting)
    {
        std::lock_guard lock(g_mutex);
        Entries& entries = Get(xuid, titleId);
        std::vector<uint8_t> d(kSettingHeaderSize, 0);
        for (int i = 0; i < 4; i++)
            d[i] = uint8_t(settingId >> (24 - 8 * i));
        d[8] = setting.type;
        std::memcpy(&d[0x10], setting.value, 8);
        d.insert(d.end(), setting.extended.begin(), setting.extended.end());
        entries[{ kSectionSetting, settingId }] = std::move(d);
        Save(xuid, titleId, entries);
    }

    std::optional<AchievementRecord> ReadAchievement(uint64_t xuid, uint32_t titleId, uint32_t achievementId)
    {
        std::lock_guard lock(g_mutex);
        const Entries& entries = Get(xuid, titleId);
        const auto it = entries.find({ kSectionAchievement, achievementId });
        if (it == entries.end())
            return std::nullopt;
        const std::vector<uint8_t>& d = it->second;
        if (d.size() < kAchievementHeaderSize)
            Fatal("{}: achievement {} is truncated", GpdPath(xuid, titleId).string(), achievementId);
        AchievementRecord r;
        r.id = Be32(&d[4]);
        r.imageId = Be32(&d[8]);
        r.gamerscore = Be32(&d[12]);
        r.flags = Be32(&d[16]);
        r.unlockTime = uint64_t(Be32(&d[20])) << 32 | Be32(&d[24]);
        return r;
    }

    std::vector<AchievementRecord> ReadAchievements(uint64_t xuid, uint32_t titleId)
    {
        std::lock_guard lock(g_mutex);
        const Entries& entries = Get(xuid, titleId);
        std::vector<AchievementRecord> out;
        for (const auto& [key, d] : entries)
        {
            // Section 1's sync list and sync data (IDs 0x100000000 / 0x200000000) are not achievements.
            if (key.first != kSectionAchievement || key.second >= 0x100000000ull || d.size() < kAchievementHeaderSize)
                continue;
            AchievementRecord r;
            r.id = Be32(&d[4]);
            r.imageId = Be32(&d[8]);
            r.gamerscore = Be32(&d[12]);
            r.flags = Be32(&d[16]);
            r.unlockTime = uint64_t(Be32(&d[20])) << 32 | Be32(&d[24]);
            size_t at = kAchievementHeaderSize;
            for (std::u16string* text : { &r.label, &r.description, &r.unachieved })
                for (; at + 1 < d.size(); at += 2)
                {
                    const char16_t c = char16_t(d[at] << 8 | d[at + 1]);
                    if (c == 0)
                    {
                        at += 2;
                        break;
                    }
                    text->push_back(c);
                }
            out.push_back(std::move(r));
        }
        return out;
    }

    void WriteAchievement(uint64_t xuid, uint32_t titleId, const AchievementRecord& record)
    {
        std::lock_guard lock(g_mutex);
        Entries& entries = Get(xuid, titleId);
        std::vector<uint8_t> d;
        Put32(d, uint32_t(kAchievementHeaderSize));
        Put32(d, record.id);
        Put32(d, record.imageId);
        Put32(d, record.gamerscore);
        Put32(d, record.flags);
        Put32(d, uint32_t(record.unlockTime >> 32));
        Put32(d, uint32_t(record.unlockTime));
        for (const std::u16string* text : { &record.label, &record.description, &record.unachieved })
        {
            for (char16_t c : *text)
            {
                d.push_back(uint8_t(c >> 8));
                d.push_back(uint8_t(c));
            }
            d.push_back(0);
            d.push_back(0);
        }
        entries[{ kSectionAchievement, record.id }] = std::move(d);
        Save(xuid, titleId, entries);
    }

    namespace
    {
        std::vector<uint8_t> ReadWholeFile(const fs::path& path)
        {
            std::ifstream in(path, std::ios::binary);
            if (!in)
                return {};
            return std::vector<uint8_t>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        }

        bool HmacSha1(const uint8_t* key, size_t keySize, const uint8_t* data, size_t size, uint8_t out[20])
        {
            BCRYPT_ALG_HANDLE algorithm = nullptr;
            if (BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_SHA1_ALGORITHM, nullptr, BCRYPT_ALG_HANDLE_HMAC_FLAG) != 0)
                return false;
            BCRYPT_HASH_HANDLE hash = nullptr;
            bool ok = BCryptCreateHash(algorithm, &hash, nullptr, 0, const_cast<uint8_t*>(key), ULONG(keySize), 0) == 0 &&
                BCryptHashData(hash, const_cast<uint8_t*>(data), ULONG(size), 0) == 0 &&
                BCryptFinishHash(hash, out, 20, 0) == 0;
            if (hash)
                BCryptDestroyHash(hash);
            BCryptCloseAlgorithmProvider(algorithm, 0);
            return ok;
        }

        void Rc4(const uint8_t* key, size_t keySize, uint8_t* data, size_t size)
        {
            uint8_t s[256];
            for (int i = 0; i < 256; i++)
                s[i] = uint8_t(i);
            for (int i = 0, j = 0; i < 256; i++)
            {
                j = (j + s[i] + key[size_t(i) % keySize]) & 255;
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
    }

    namespace
    {
        // The console's account keys (retail, development kit), as documented for the Account file and used by Xenia.
        constexpr uint8_t kAccountKeys[2][16] = {
            { 0xE1, 0xBC, 0x15, 0x9C, 0x73, 0xB1, 0xEA, 0xE9, 0xAB, 0x31, 0x70, 0xF3, 0xAD, 0x47, 0xEB, 0xF3 },
            { 0xDA, 0xB6, 0x9A, 0xD9, 0x8E, 0x28, 0x76, 0x4F, 0x97, 0x7E, 0xE2, 0x48, 0x7E, 0x4F, 0x3F, 0x68 },
        };
        constexpr size_t kAccountSize = 0x194; // 16-byte hash + 8-byte confounder + 380-byte XAMACCOUNTINFO

        // The decrypted Account file (confounder + XAMACCOUNTINFO) and the key it decrypted with.
        std::optional<std::pair<std::vector<uint8_t>, const uint8_t*>> DecryptAccount(uint64_t xuid)
        {
            const std::vector<uint8_t> file = ReadWholeFile(ProfileDirectory(xuid) / "Account");
            if (file.size() != kAccountSize)
                return std::nullopt;
            for (const auto& key : kAccountKeys)
            {
                uint8_t rc4Key[20];
                if (!HmacSha1(key, 16, file.data(), 16, rc4Key))
                    return std::nullopt;
                std::vector<uint8_t> plain(file.begin() + 16, file.end());
                Rc4(rc4Key, 16, plain.data(), plain.size());
                uint8_t check[20];
                if (HmacSha1(key, 16, plain.data(), plain.size(), check) && std::memcmp(check, file.data(), 16) == 0)
                    return std::make_pair(std::move(plain), key);
            }
            Log("Profiles: {:016X}'s Account file does not decrypt with the console's keys", xuid);
            return std::nullopt;
        }
    }

    std::optional<Account> ReadAccount(uint64_t xuid)
    {
        if (const auto decrypted = DecryptAccount(xuid))
        {
            const std::vector<uint8_t>& plain = decrypted->first;
            const uint8_t* info = plain.data() + 8;
            Account account;
            account.reservedFlags = Be32(info);
            for (size_t i = 0; i < 16; i++)
            {
                const char16_t c = char16_t(info[8 + 2 * i] << 8 | info[9 + 2 * i]);
                if (c == 0)
                    break;
                account.gamertag.push_back(c);
            }
            account.onlineXuid = uint64_t(Be32(info + 0x28)) << 32 | Be32(info + 0x2C);
            account.info.assign(plain.begin() + 8, plain.end());
            return account;
        }
        return std::nullopt;
    }

    std::optional<uint64_t> CreateProfile(const std::u16string& gamertag)
    {
        uint64_t xuid = 0;
        for (int attempt = 0; attempt < 16 && (xuid == 0 || fs::exists(ProfileDirectory(xuid))); attempt++)
        {
            uint64_t random = 0;
            BCryptGenRandom(nullptr, reinterpret_cast<PUCHAR>(&random), sizeof(random), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
            xuid = 0xE000000000000000ull | (random & 0x0FFFFFFFFFFFFFFFull);
        }
        std::error_code ec;
        const fs::path folder = ProfileDirectory(xuid);
        fs::create_directories(folder, ec);
        // Confounder (8 random bytes) + XAMACCOUNTINFO: reserved flags 0, LIVE flags 0, the gamertag (16 UTF-16BE
        // characters), the rest zero (no online XUID, passcode, domain ...).
        std::vector<uint8_t> plain(kAccountSize - 16, 0);
        BCryptGenRandom(nullptr, plain.data(), 8, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
        for (size_t i = 0; i < gamertag.size() && i < 15; i++)
        {
            plain[8 + 8 + i * 2] = uint8_t(gamertag[i] >> 8);
            plain[8 + 8 + i * 2 + 1] = uint8_t(gamertag[i]);
        }
        const uint8_t* key = kAccountKeys[0];
        std::vector<uint8_t> file(16);
        uint8_t hash[20], rc4Key[20];
        if (!HmacSha1(key, 16, plain.data(), plain.size(), hash) || !HmacSha1(key, 16, hash, 16, rc4Key))
            return std::nullopt;
        std::memcpy(file.data(), hash, 16);
        Rc4(rc4Key, 16, plain.data(), plain.size());
        file.insert(file.end(), plain.begin(), plain.end());
        {
            std::ofstream out(folder / "Account", std::ios::binary | std::ios::trunc);
            out.write(reinterpret_cast<const char*>(file.data()), std::streamsize(file.size()));
            if (!out)
            {
                Log("Profiles: cannot write the Account file of the new profile in {}", folder.string());
                return std::nullopt;
            }
        }
        // Its location was looked up (and cached as absent) while choosing the XUID.
        kernel::profile_location::Forget(xuid);
        const auto check = ReadAccount(xuid);
        if (!check || check->gamertag != gamertag)
        {
            Log("Profiles: the new profile's Account file does not read back");
            return std::nullopt;
        }
        Log("Profiles: created {:016X} in {}", xuid, folder.string());
        return xuid;
    }

    bool MakeLiveAccount(uint64_t xuid, uint64_t onlineXuid)
    {
        auto decrypted = DecryptAccount(xuid);
        if (!decrypted)
            return false;
        auto& [plain, key] = *decrypted;
        uint8_t* info = plain.data() + 8;
        const uint32_t reserved = Be32(info) | 0x20000000; // Xbox LIVE-enabled
        for (int i = 0; i < 4; i++)
            info[i] = uint8_t(reserved >> (24 - 8 * i));
        for (int i = 0; i < 8; i++)
            info[0x28 + i] = uint8_t(onlineXuid >> (56 - 8 * i));
        // Encrypted as the console does: hash = HMAC-SHA1(key, data)[0..16], RC4 key = HMAC-SHA1(key, hash)[0..16].
        std::vector<uint8_t> file(16);
        uint8_t hash[20], rc4Key[20];
        if (!HmacSha1(key, 16, plain.data(), plain.size(), hash) || !HmacSha1(key, 16, hash, 16, rc4Key))
            return false;
        std::memcpy(file.data(), hash, 16);
        Rc4(rc4Key, 16, plain.data(), plain.size());
        file.insert(file.end(), plain.begin(), plain.end());
        const fs::path path = ProfileDirectory(xuid) / "Account";
        std::ofstream out(path, std::ios::binary | std::ios::trunc);
        out.write(reinterpret_cast<const char*>(file.data()), std::streamsize(file.size()));
        if (!out)
            return false;
        out.close();
        kernel::profile_location::Changed(xuid);
        const auto check = ReadAccount(xuid);
        return check && check->LiveEnabled() && check->onlineXuid == onlineXuid;
    }

    namespace
    {
        // Online XUIDs of profiles that have no Account file (console_settings.ini gamertags): live_accounts.txt in the
        // data folder, one "<offline XUID> <online XUID>" per line.
        fs::path LocalAccountsFile()
        {
            return DataDirectory() / "live_accounts.txt";
        }
    }

    std::optional<uint64_t> LocalOnlineXuid(uint64_t xuid)
    {
        std::ifstream in(LocalAccountsFile());
        std::string offline, online;
        while (in >> offline >> online)
            if (std::strtoull(offline.c_str(), nullptr, 16) == xuid)
                return std::strtoull(online.c_str(), nullptr, 16);
        return std::nullopt;
    }

    void SaveLocalOnlineXuid(uint64_t xuid, uint64_t onlineXuid)
    {
        std::ofstream(LocalAccountsFile(), std::ios::app) << std::format("{:016X} {:016X}\n", xuid, onlineXuid);
    }

    std::vector<uint64_t> StoredProfiles()
    {
        // Profiles on every device (kernel/profile_location.h) that have an Account file.
        std::vector<uint64_t> out;
        std::error_code error;
        for (const uint64_t xuid : kernel::profile_location::All())
            if (fs::exists(ProfileDirectory(xuid) / "Account", error))
                out.push_back(xuid);
        std::sort(out.begin(), out.end());
        return out;
    }

    std::vector<uint8_t> GamerPicture(uint64_t xuid, bool small)
    {
        return ReadWholeFile(ProfileDirectory(xuid) / (small ? "tile_32.png" : "tile_64.png"));
    }

    std::optional<std::vector<uint8_t>> ReadPackageFile(uint64_t xuid, const std::string& name)
    {
        const fs::path path = ProfileDirectory(xuid) / name;
        std::error_code error;
        if (!fs::is_regular_file(path, error))
            return std::nullopt;
        return ReadWholeFile(path);
    }
}

namespace kernel::profile
{
    const uint8_t* AccountKey(uint32_t index) { return kAccountKeys[index]; }
}
