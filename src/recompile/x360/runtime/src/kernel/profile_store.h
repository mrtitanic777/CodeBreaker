#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

// Each local profile's stored data, kept as the console keeps it: one GPD file (an XDBF database) per title, in
// Xenia's profile layout content/<XUID>/FFFE07D1/00010000/<XUID>/<TitleID>.gpd (XUID = the profile's offline XUID). The dashboard's GPD (FFFE07D1) holds
// system settings; each title's GPD holds its title-specific settings (and, on a console, its achievements).
//
// GPD settings are section-3 entries with id = setting id: a 0x18-byte header {u32 setting id, u32, u8 type,
// 7 bytes, 8-byte value (XUSER_DATA union; for strings and binary: u32 size, u32 0)} followed by the variable-size
// bytes. Entries of other sections are preserved when the file is rewritten.
//
// Achievements earned by the title are section-1 entries with id = achievement id (Xenia's X_XDBF_GPD_ACHIEVEMENT):
// {u32 struct size 0x1C, u32 id, u32 image id, u32 gamerscore, u32 flags (type | ACHIEVED 0x20000 | ...),
// u64 unlock time (FILETIME)} followed by the label, description and unachieved strings (UTF-16BE, terminated).
namespace kernel::profile
{
    struct Setting
    {
        uint8_t type = 0;
        uint8_t value[8] = {}; // big-endian XUSER_DATA value (size + 0 for strings and binary)
        std::vector<uint8_t> extended;
    };

    struct AchievementRecord
    {
        uint32_t id = 0, imageId = 0, gamerscore = 0, flags = 0;
        uint64_t unlockTime = 0;
        std::u16string label, description, unachieved;
    };

    // A profile copied from a console (tools/scripts/import_profile.py) has the console's Account file: the
    // XAMACCOUNTINFO (u32 reserved flags, u32 LIVE flags, 16 UTF-16BE gamertag characters, u64 online XUID, ...)
    // after an 8-byte confounder, RC4-encrypted with HMAC-SHA1(key, first 16 bytes) and checked by those 16 bytes
    // (= HMAC-SHA1(key, decrypted data)); key = the console's account key (retail or development kit).
    struct Account
    {
        std::u16string gamertag;
        uint32_t reservedFlags = 0;
        uint64_t onlineXuid = 0;
        std::vector<uint8_t> info; // the decrypted XAMACCOUNTINFO (0x17C bytes, big-endian)
        bool LiveEnabled() const { return (reservedFlags & 0x20000000) != 0; }
    };
    std::optional<Account> ReadAccount(uint64_t xuid);
    // The roamable account keys profiles are sealed with (retail first, then the development kit's), 16 bytes each:
    // XeKeysObfuscate / XeKeysUnObfuscate with Roaming = TRUE (kernel/xboxkrnl_crypt.cpp).
    const uint8_t* AccountKey(uint32_t index);
    constexpr uint32_t kAccountKeyCount = 2;
    // A new offline profile on the hard drive (XamShowCreateProfileUI): a random offline XUID (0xE + 60 bits), its
    // folder and an Account file encrypted with the retail account key as a console writes it (XAMACCOUNTINFO with
    // only the gamertag set). nullopt (logged) if it cannot be written.
    std::optional<uint64_t> CreateProfile(const std::u16string& gamertag);
    // Makes the profile's Account an Xbox LIVE account with this online XUID (reserved flag 0x20000000, the XUID at
    // 0x28), encrypted again with the key it had (our Xbox LIVE's `register`, kernel/xam_live.cpp). False if the
    // profile has no readable Account or the write fails.
    bool MakeLiveAccount(uint64_t xuid, uint64_t onlineXuid);
    // The online XUID our Xbox LIVE gave a profile without an Account file (live_accounts.txt in the data folder).
    std::optional<uint64_t> LocalOnlineXuid(uint64_t xuid);
    void SaveLocalOnlineXuid(uint64_t xuid, uint64_t onlineXuid);
    // The profiles on the hard drive that have an Account file (content/<XUID>/FFFE07D1/00010000/<XUID>/Account),
    // in XUID order.
    std::vector<uint64_t> StoredProfiles();
    // The profile's gamer picture (PNG: tile_64.png, or tile_32.png when `small`), empty if it has none.
    std::vector<uint8_t> GamerPicture(uint64_t xuid, bool small = false);
    // A file of the profile's package (content/<XUID>/FFFE07D1/00010000/<XUID>/<name>, e.g. pp_64.png), nullopt if
    // the package has no such file.
    std::optional<std::vector<uint8_t>> ReadPackageFile(uint64_t xuid, const std::string& name);

    // An entry's bytes as stored (any section; e.g. the dash GPD's section 4 title records and its sync list, id
    // 0x100000000: {be64 entry id, be64 sync id} per entry, oldest change first), nullopt if absent.
    std::optional<std::vector<uint8_t>> ReadEntry(uint64_t xuid, uint32_t titleId, uint16_t section, uint64_t id);
    std::optional<Setting> ReadSetting(uint64_t xuid, uint32_t titleId, uint32_t settingId);
    std::optional<AchievementRecord> ReadAchievement(uint64_t xuid, uint32_t titleId, uint32_t achievementId);
    // Every achievement of a title's GPD with its texts (label, description, unachieved), in ID order: what XAM shows
    // for a title that is not running (the Guide's games list).
    std::vector<AchievementRecord> ReadAchievements(uint64_t xuid, uint32_t titleId);
    void WriteAchievement(uint64_t xuid, uint32_t titleId, const AchievementRecord& record);
    void WriteSetting(uint64_t xuid, uint32_t titleId, uint32_t settingId, const Setting& setting);

    // A title the profile plays, in the dashboard GPD's titles played (section 4; XAM 17559 FUN_81920ac0, called for
    // each signed-in user when a title loads and when a user signs in to the running title, FUN_81911f28): the
    // record id = title ID {+0 title ID, +4 achievements possible, +8 earned, +0xC gamerscore possible, +0x10
    // earned, +0x17 / +0x19 / +0x1B avatar awards, +0x1C flags, +0x20 last played FILETIME, +0x28 the name (string
    // 0x8000, at most 0x40 characters), size (characters + 0x14) * 2}; the title moves to the end of the section's
    // sync list (0x100000000) with a new sync id (0x200000000's counter, FUN_819286b8) when it was not the last one
    // played or its record changed, and setting 0x10040012 (GamercardTitlesPlayed) is the list's length. Earned
    // counts are the title GPD's earned achievements (FUN_81921220's sync). The title's tile (image 0x8000 of the
    // title's SPA) goes to its own GPD's section 2, where the titles-played readers find it.
    struct TitlePlayed
    {
        uint32_t titleId = 0;
        std::u16string name;
        uint32_t achievements = 0, gamerscore = 0; // possible (the SPA's)
        std::vector<uint8_t> tile;                 // PNG; empty: none written
    };
    void RecordTitlePlayed(uint64_t xuid, const TitlePlayed& title);
    // The title's record's earned achievements and gamerscore again from its GPD (after an achievement is written).
    void RecountTitleEarned(uint64_t xuid, uint32_t titleId);
}
