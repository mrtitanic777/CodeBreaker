// The dashboard runtime's side of the shared Xbox Guide (native/x360/system/guide, guide/host.h): profiles and sign-in
// (kernel/users.h), the profile store, the SPA's achievements, the dashboard and the DVD tray. The system UI slot
// (OpenSystemUi / EndSystemUiInput) is in kernel/xam_ui.cpp with the other dialogs.

#include "kernel/stfs_mount.h"
#include "guide/host.h"

#include "kernel/kernel.h"
#include "kernel/profile_import.h"
#include "live/client.h"
#include "kernel/profile_store.h"
#include "kernel/spa.h"
#include "kernel/users.h"
#include "log.h"
#include "settings.h"

#include <windows.h>

#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>

namespace
{
    constexpr uint32_t kXnSysSigninChanged = 0x0000000A;
    constexpr uint32_t kDashboardTitleId = 0xFFFE07D1;
    constexpr uint32_t kSettingGamerscore = 0x10040006; // XPROFILE_GAMERCARD_CRED (the dashboard GPD)

    static_assert(guide::kSettingZone == kernel::users::kSettingZone && guide::kSettingMotto == kernel::users::kSettingMotto &&
                  guide::kSettingName == kernel::users::kSettingName &&
                  guide::kSettingLocation == kernel::users::kSettingLocation &&
                  guide::kSettingBio == kernel::users::kSettingBio);

    guide::Profile ToGuide(const kernel::users::Profile& p)
    {
        return { p.gamertag, p.xuid, p.liveEnabled, p.onlineXuid };
    }
}

std::vector<guide::Profile> guide::host::Profiles()
{
    std::vector<Profile> out;
    for (const kernel::users::Profile& p : kernel::users::Profiles())
        out.push_back(ToGuide(p));
    return out;
}

std::optional<guide::Profile> guide::host::SignedIn(uint32_t user)
{
    if (const auto p = kernel::users::SignedIn(user))
        return ToGuide(*p);
    return std::nullopt;
}

std::optional<uint32_t> guide::host::UserOfXuid(uint64_t xuid)
{
    return kernel::users::UserOfXuid(xuid);
}

bool guide::host::OnLive(uint32_t user)
{
    return kernel::UserOnLive(user);
}

void guide::host::SignIn(uint32_t user, size_t index)
{
    kernel::users::SignIn(user, index);
    kernel::BroadcastNotification(kXnSysSigninChanged, kernel::users::SignedInMask());
}

void guide::host::SignOut(uint32_t user)
{
    kernel::users::SignOut(user);
    kernel::BroadcastNotification(kXnSysSigninChanged, kernel::users::SignedInMask());
}

// "Download Profile" (the sign-in panes' last row; the signed-out Guide's button): the console recovers a profile from
// Xbox LIVE; here the profiles put in the `profiles` folder (kernel/profile_import.h) are copied to the hard drive and
// listed from then on. XamShowSigninUI's pane uses it too.
std::vector<uint64_t> guide::host::DownloadProfiles()
{
    std::vector<uint64_t> imported = kernel::profile::ImportNewProfiles();
    kernel::users::AddStoredProfiles();
    Log("System UI: Download Profile: {} new profile(s) in {}", imported.size(),
        kernel::profile::ImportFolder().string());
    return imported;
}

std::vector<uint8_t> guide::host::GamerPicture(uint64_t xuid)
{
    return kernel::profile::GamerPicture(xuid);
}

guide::Gamercard guide::host::ReadGamercard(uint64_t xuid)
{
    const kernel::users::Gamercard g = kernel::users::ReadGamercard(xuid);
    Gamercard card;
    // users::ReadGamercard reads the credit as 0 when the GPD has none; the Guide wants to know.
    if (kernel::profile::ReadSetting(xuid, kDashboardTitleId, kSettingGamerscore))
        card.gamerscore = g.gamerscore;
    card.zone = g.zone;
    card.reputation = g.reputation;
    card.motto = g.motto;
    card.name = g.name;
    card.location = g.location;
    card.bio = g.bio;
    return card;
}

namespace
{
    constexpr uint32_t kSettingVibration = 0x10040003, kSettingVoiceThruSpeakers = 0x1004000D,
                       kSettingVoiceVolume = 0x1004000E;

    std::optional<int32_t> ReadInt32Setting(uint64_t xuid, uint32_t id)
    {
        const auto s = kernel::profile::ReadSetting(xuid, kDashboardTitleId, id);
        if (!s)
            return std::nullopt;
        return int32_t(uint32_t(s->value[0]) << 24 | uint32_t(s->value[1]) << 16 | uint32_t(s->value[2]) << 8 |
                       s->value[3]);
    }

    void WriteInt32Setting(uint64_t xuid, uint32_t id, int32_t value)
    {
        kernel::profile::Setting s;
        s.type = 1; // XUSER_DATA_TYPE_INT32
        for (int i = 0; i < 4; i++)
            s.value[i] = uint8_t(uint32_t(value) >> (24 - 8 * i));
        kernel::profile::WriteSetting(xuid, kDashboardTitleId, id, s);
    }

    // The runtime's own preferences of a profile: <data>/preferences/<XUID>.ini, "name=value" lines.
    std::filesystem::path PreferencesFile(uint64_t xuid)
    {
        return DataDirectory() / "preferences" / std::format("{:016X}.ini", xuid);
    }
}

guide::host::Preferences guide::host::ReadPreferences(uint32_t user)
{
    Preferences p;
    const uint64_t xuid = kernel::users::Xuid(user);
    if (!xuid)
        return p;
    p.vibration = ReadInt32Setting(xuid, kSettingVibration).value_or(3) != 0;
    p.voiceVolume = uint32_t(std::clamp(ReadInt32Setting(xuid, kSettingVoiceVolume).value_or(100), 0, 100) + 5) / 10;
    p.voiceOutput = uint32_t(std::clamp(ReadInt32Setting(xuid, kSettingVoiceThruSpeakers).value_or(0), 0, 2));
    std::ifstream file(PreferencesFile(xuid));
    std::string line;
    while (std::getline(file, line))
    {
        const size_t eq = line.find('=');
        if (eq == std::string::npos)
            continue;
        const std::string name = line.substr(0, eq);
        const int value = std::atoi(line.c_str() + eq + 1);
        if (name == "game_volume")
            p.gameVolume = uint32_t(std::clamp(value, 0, 10));
        else if (name == "notify_show")
            p.notifyShow = value != 0;
        else if (name == "notify_sound")
            p.notifySound = value != 0;
        else if (name == "notify_videos")
            p.notifyVideos = value != 0;
    }
    return p;
}

void guide::host::WritePreferences(uint32_t user, const Preferences& p)
{
    const uint64_t xuid = kernel::users::Xuid(user);
    if (!xuid)
        return;
    WriteInt32Setting(xuid, kSettingVibration, p.vibration ? 3 : 0);
    WriteInt32Setting(xuid, kSettingVoiceVolume, int32_t(p.voiceVolume * 10));
    WriteInt32Setting(xuid, kSettingVoiceThruSpeakers, int32_t(p.voiceOutput));
    std::error_code error;
    std::filesystem::create_directories(PreferencesFile(xuid).parent_path(), error);
    std::ofstream(PreferencesFile(xuid)) << std::format("game_volume={}\nnotify_show={}\nnotify_sound={}\nnotify_videos={}\n",
                                                        p.gameVolume, int(p.notifyShow), int(p.notifySound),
                                                        int(p.notifyVideos));
    kernel::BroadcastNotification(0x0000000E, 1u << user); // XN_SYS_PROFILESETTINGCHANGED
    Log("System UI: preferences of user {}: vibration {}, voice {} / game {}, output {}, notifications {}{}{}", user,
        p.vibration, p.voiceVolume, p.gameVolume, p.voiceOutput, p.notifyShow, p.notifySound, p.notifyVideos);
}

std::optional<std::string> guide::host::WriteGamercard(uint32_t user, uint32_t settingId, const std::u16string& text)
{
    return kernel::users::WriteGamercard(user, settingId, text);
}

std::u16string guide::host::TitleName()
{
    return kernel::spa::TitleName();
}

std::vector<xui::AchievementEntry> guide::host::TitleAchievements(uint32_t user)
{
    constexpr uint32_t kShowUnachieved = 0x8, kAchieved = 0x20000; // XACHIEVEMENT_DETAILS_*
    std::vector<xui::AchievementEntry> entries;
    const uint64_t xuid = kernel::users::Xuid(user);
    for (const kernel::spa::Achievement& a : kernel::spa::Achievements())
    {
        const auto record = kernel::profile::ReadAchievement(xuid, kernel::TitleId(), a.id);
        xui::AchievementEntry entry;
        entry.label = kernel::spa::String(a.labelId);
        entry.description = kernel::spa::String(a.descriptionId);
        entry.unachieved = kernel::spa::String(a.unachievedId);
        entry.gamerscore = a.gamerscore;
        entry.earned = record && (record->flags & kAchieved);
        entry.showUnachieved = (a.flags & kShowUnachieved) != 0;
        entry.unlockTime = entry.earned ? record->unlockTime : 0;
        const auto image = kernel::spa::Image(a.imageId);
        entry.image.assign(image.begin(), image.end());
        entries.push_back(std::move(entry));
    }
    return entries;
}

// The dash GPD's section 4: its sync list (id 0x100000000: {be64 entry id, be64 sync id}, oldest change first) and
// a title record per title (id = title ID: title ID +0, achievements +4, earned +8, gamerscore total +0xC, earned
// +0x10, last played FILETIME +0x20, UTF-16BE name +0x28; XamUserCreateTitlesPlayedEnumerator). A title's tile is
// image 0x8000 (section 2) of its own GPD.
std::vector<guide::host::PlayedTitle> guide::host::PlayedTitles(uint32_t user)
{
    constexpr uint16_t kSectionImage = 2, kSectionTitle = 4;
    constexpr uint64_t kSyncListId = 0x100000000, kTitleTileId = 0x8000;
    std::vector<PlayedTitle> titles;
    const uint64_t xuid = kernel::users::Xuid(user);
    if (!xuid)
        return titles;
    const auto list = kernel::profile::ReadEntry(xuid, kDashboardTitleId, kSectionTitle, kSyncListId);
    const size_t count = list ? list->size() / 16 : 0;
    auto be32 = [](const std::vector<uint8_t>& d, size_t at) {
        return uint32_t(d[at]) << 24 | uint32_t(d[at + 1]) << 16 | uint32_t(d[at + 2]) << 8 | d[at + 3];
    };
    for (size_t i = count; i-- > 0;)
    {
        uint64_t id = 0;
        for (int b = 0; b < 8; b++)
            id = id << 8 | (*list)[i * 16 + b];
        const auto record = kernel::profile::ReadEntry(xuid, kDashboardTitleId, kSectionTitle, id);
        if (!record || record->size() < 0x2A)
            continue;
        PlayedTitle t;
        t.titleId = be32(*record, 0);
        t.achievements = be32(*record, 4);
        t.earned = be32(*record, 8);
        t.gamerscore = be32(*record, 0x10);
        for (size_t at = 0x28; at + 1 < record->size(); at += 2)
        {
            const char16_t c = char16_t((*record)[at] << 8 | (*record)[at + 1]);
            if (c == 0)
                break;
            t.name.push_back(c);
        }
        if (const auto tile = kernel::profile::ReadEntry(xuid, t.titleId, kSectionImage, kTitleTileId))
            t.tile = *tile;
        titles.push_back(std::move(t));
    }
    return titles;
}

std::vector<xui::AchievementEntry> guide::host::PlayedTitleAchievements(uint32_t user, uint32_t titleId)
{
    constexpr uint32_t kShowUnachieved = 0x8, kAchieved = 0x20000; // XACHIEVEMENT_DETAILS_*
    constexpr uint16_t kSectionImage = 2;
    std::vector<xui::AchievementEntry> entries;
    const uint64_t xuid = kernel::users::Xuid(user);
    for (const kernel::profile::AchievementRecord& a : kernel::profile::ReadAchievements(xuid, titleId))
    {
        xui::AchievementEntry entry;
        entry.label = a.label;
        entry.description = a.description;
        entry.unachieved = a.unachieved;
        entry.gamerscore = a.gamerscore;
        entry.earned = (a.flags & kAchieved) != 0;
        entry.showUnachieved = (a.flags & kShowUnachieved) != 0;
        entry.unlockTime = entry.earned ? a.unlockTime : 0;
        if (const auto image = kernel::profile::ReadEntry(xuid, titleId, kSectionImage, a.imageId))
            entry.image = *image;
        entries.push_back(std::move(entry));
    }
    return entries;
}

bool guide::host::TitleIsDashboard()
{
    return kernel::TitleId() == kDashboardTitleId;
}

uint32_t guide::host::DashContext()
{
    return kernel::DashContext();
}

void guide::host::RebootToDash(const std::vector<uint8_t>& request)
{
    kernel::RebootToDash(request);
}

// Leaving a title for the dashboard or turning the console off (the console's warning about unsaved progress was
// confirmed): the title ends there, so the process ends. This runtime only runs the dashboard (TitleIsDashboard), so
// here it is Turn Off Console; leaving for the dashboard is for the ports' runtimes.
void guide::host::EndTitle(const char* why, const std::vector<uint8_t>*)
{
    Log("System UI: {}: the game ends (no dashboard on the PC)", why);
    live::SignOutAllForExit(); // Turn Off Console: the player quits, party seats are given up at once
    kernel::stfs_mount::WaitForAllWrites(); // content packages still being written back
    logging::Flush();
    ExitProcess(0);
}

uint32_t guide::host::DvdTrayState()
{
    return kernel::DvdTrayState();
}

void guide::host::OpenCloseDvdTray(bool open)
{
    kernel::OpenCloseDvdTray(open);
}
