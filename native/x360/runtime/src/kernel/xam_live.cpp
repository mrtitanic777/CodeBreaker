// The ports' own Xbox LIVE (native/x360/system/live, protocol docs/x360/network_protocol.md): owner's plan
// (2026-10-15): a profile that signs in connects to the server (console_settings.ini live_server, default
// 127.0.0.1:36074) and is online there with the title it is in. Without a server everything stays as on a console
// without network.
//
// The server knows a profile by its online XUID (0009...): only Xbox LIVE-enabled profiles (copied from a console
// with an Xbox LIVE account) log on, as on a console.

#include "kernel/kernel.h"
#include "kernel/enumerator.h"
#include "kernel/overlapped.h"
#include "kernel/profile_store.h"
#include "kernel/users.h"
#include "settings.h"

#include "guide/host.h"
#include "live/client.h"
#include "xui/system_ui.h"

#include <windows.h>

#include <array>
#include <atomic>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <map>
#include <mutex>
#include <set>
#include <optional>
#include <tuple>
#include <cstdio>
#include <thread>
#include <random>
#include <format>

namespace
{
    // Notification IDs (xam.h XNID(version, area, index) = area << 25 | version << 16 | index).
    constexpr uint32_t kXnSysSigninChanged = 0x0000000A;
    constexpr uint32_t kXnLiveConnectionChanged = 0x02000001;
    constexpr uint32_t kXnFriendsPresenceChanged = 0x04000001;
    constexpr uint32_t kXnFriendsFriendAdded = 0x04000002;
    constexpr uint32_t kXnFriendsFriendRemoved = 0x04000003;
    constexpr uint32_t kXnPartyMembersChanged = 0x0E040002;
    constexpr uint32_t kLogonConnectionEstablished = 0x001510F0; // XONLINE_S_LOGON_CONNECTION_ESTABLISHED
    constexpr uint32_t kLogonConnectionLost = 0x80151004;        // XONLINE_E_LOGON_CONNECTION_LOST


    uint32_t LiveMask()
    {
        uint32_t mask = 0;
        for (uint32_t user = 0; user < 4; user++)
            if (live::SignedIn(user))
                mask |= 1u << user;
        return mask;
    }

    std::u16string Utf16(const std::string& utf8)
    {
        const int n = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), int(utf8.size()), nullptr, 0);
        std::wstring w(size_t(n), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8.data(), int(utf8.size()), w.data(), n);
        return std::u16string(w.begin(), w.end());
    }

    // What the toasts compare against: friend requests already announced, the party's members last seen.
    std::mutex g_toastMutex;
    std::set<std::pair<uint32_t, uint64_t>> g_announcedRequests;
    std::map<uint32_t, std::map<uint64_t, std::string>> g_partyMembers;
    std::map<uint32_t, bool> g_inParty;

    // XAM's toasts for LIVE events (its notification table 0x815F82B0; kernel/xam_ui.cpp draws them).
    void Toast(const live::Event& e)
    {
        std::lock_guard lock(g_toastMutex);
        switch (e.kind)
        {
        case live::EventKind::FriendChanged:
            for (const live::Friend& f : live::Friends(e.user))
                if (f.xuid == e.xuid && f.request == "incoming" && g_announcedRequests.insert({ e.user, f.xuid }).second)
                    xui::QueueNotification(xui::kNotifyFriendRequest, e.user, Utf16(f.gamertag));
            break;
        case live::EventKind::MessageArrived:
        {
            const auto messages = live::Messages(e.user);
            if (!messages.empty())
                xui::QueueNotification(messages.front().kind == "party_invite" ? xui::kNotifyPartyInvite
                                                                              : xui::kNotifyMessage,
                                       e.user, Utf16(messages.front().fromGamertag));
            break;
        }
        case live::EventKind::PartyChanged:
        {
            const auto party = live::CurrentParty(e.user);
            std::map<uint64_t, std::string> now;
            if (party)
                for (const live::PartyMember& m : party->members)
                    now[m.xuid] = m.gamertag;
            auto& before = g_partyMembers[e.user];
            const bool wasIn = g_inParty[e.user];
            const auto self = kernel::users::SignedIn(e.user);
            const uint64_t me = self ? self->onlineXuid : 0;
            if (party && !wasIn && party->leader != me)
                xui::QueueNotification(xui::kNotifyPartyJoined, e.user, u"");
            else if (party && wasIn)
            {
                for (const auto& [xuid, tag] : now)
                    if (!before.contains(xuid) && xuid != me)
                        xui::QueueNotification(xui::kNotifyPartyMemberJoined, e.user, Utf16(tag));
                for (const auto& [xuid, tag] : before)
                    if (!now.contains(xuid) && xuid != me)
                        xui::QueueNotification(xui::kNotifyPartyMemberLeft, e.user, Utf16(tag));
            }
            before = std::move(now);
            g_inParty[e.user] = party.has_value();
            break;
        }
        case live::EventKind::SignedOut: // the account signed in on another console
            xui::QueueNotification(xui::kNotifyLiveDisconnected, e.user, u"");
            g_partyMembers.erase(e.user);
            g_inParty.erase(e.user);
            break;
        case live::EventKind::Disconnected:
            for (uint32_t user = 0; user < 4; user++)
                if (kernel::users::SignedIn(user) && kernel::users::SignedIn(user)->liveEnabled)
                {
                    xui::QueueNotification(xui::kNotifyLiveDisconnected, user, u"");
                    break;
                }
            g_partyMembers.clear();
            g_inParty.clear();
            break;
        default:
            break;
        }
    }

    // Users whose Xbox LIVE logon the title has seen (XamShowSigninUIp's logon, kernel::LiveLogonCommitted). Our server
    // signs a profile in as soon as it signs in locally (users::SignIn), but a console logs on to LIVE only when the
    // dash asks (signin.xex): XN_LIVE_CONNECTIONCHANGED before that (it arrived mid-sign-in over the Internet, where
    // the server's answer takes round trips) left the dash on "Signing in" with the hub never loaded (Colin's tests,
    // 2026-10-16; reproduced with 60 ms latency). Later sign-ins of a committed user (a reconnect) are announced.
    std::array<std::atomic<bool>, 4> g_liveLogonCommitted{};

    // Server events -> XAM's notifications, as XAM raises them for LIVE changes.
    void OnLiveEvent(const live::Event& e)
    {
        Toast(e);
        switch (e.kind)
        {
        case live::EventKind::SignedIn:
            if (e.user >= g_liveLogonCommitted.size() || !g_liveLogonCommitted[e.user])
            {
                Log("Live: user {} is on Xbox LIVE (announced at the title's logon)", e.user);
                break;
            }
            Log("Live: user {} is on Xbox LIVE", e.user);
            kernel::BroadcastNotification(kXnLiveConnectionChanged, kLogonConnectionEstablished);
            kernel::BroadcastNotification(kXnSysSigninChanged, kernel::users::SignedInMask());
            break;
        case live::EventKind::Disconnected:
        case live::EventKind::SignedOut: // the profile stays signed in locally, off Xbox LIVE
            kernel::BroadcastNotification(kXnLiveConnectionChanged, kLogonConnectionLost);
            kernel::BroadcastNotification(kXnSysSigninChanged, kernel::users::SignedInMask());
            break;
        case live::EventKind::FriendChanged:
        {
            const auto friends = live::Friends(e.user);
            const auto it = std::find_if(friends.begin(), friends.end(), [&](const live::Friend& f) {
                return f.xuid == e.xuid; });
            kernel::BroadcastNotification(it != friends.end() && !it->request.empty() ? kXnFriendsFriendAdded
                                                                                       : kXnFriendsPresenceChanged,
                                          e.user);
            break;
        }
        case live::EventKind::FriendRemoved:
            kernel::BroadcastNotification(kXnFriendsFriendRemoved, e.user);
            break;
        case live::EventKind::PartyChanged:
            kernel::BroadcastNotification(kXnPartyMembersChanged, 0);
            break;
        case live::EventKind::MessageArrived:
            // XAM's mailbox list changed (0x818E5718): 0x82000006 with the user index, for system listeners.
            kernel::BroadcastNotification(0x82000006, e.user);
            break;
        case live::EventKind::Connected:
            // Signed-in profiles without an Xbox LIVE account get one now (EnsureLiveAccount; requests cannot be made
            // from this thread), unless kept offline (live::Held: no automatic sign-in; the Guide's
            // Connect to Xbox LIVE raises this event again after connecting).
            std::thread([] {
                for (uint32_t user = 0; user < 4; user++)
                    if (const auto profile = kernel::users::SignedIn(user);
                        profile && !profile->liveEnabled && !live::Held(user))
                        kernel::users::LiveSignIn(user, *profile);
            }).detach();
            break;
        }
    }
}

void kernel::InitializeLive()
{
    const std::string& address = Settings().liveServer;
    if (address.empty())
    {
        Log("Live: no server configured (live_server is empty): offline");
        return;
    }
    live::Config config;
    if (!live::ParseServers(address, config)) // "host:port", or several tried in order ("a:36074, 127.0.0.1:36074")
    {
        Log("Live: live_server \"{}\" names no server: offline", address);
        return;
    }
    config.client = "Xbox 360 Dashboard";
    config.titleId = 0xFFFE07D1;
    // Party chat uses the PC's microphone and speakers, except in headless test runs (LIVE_VOICE_TONE /
    // LIVE_VOICE_DUMP stand in for them there).
    const char* headless = std::getenv("CB_HEADLESS");
    config.voiceDevices = !(headless && headless[0] == '1');
    // Live debugging (testers' copies): the log goes to the server too, from the first line (live_log).
    config.sendLog = Settings().liveLog;
    if (config.sendLog)
        Log("Live: live_log is on: this log also goes to the Xbox LIVE server as it is written");
    live::SetEventHandler(OnLiveEvent);
    live::Start(config);
}

bool kernel::UserOnLive(uint32_t user)
{
    return live::SignedIn(user);
}

namespace
{
    // This install's own random ID (live_console_key.txt in the data folder): our server gives the same online XUID
    // back to the console that registered a gamertag.
    std::string ConsoleKey()
    {
        static const std::string key = [] {
            const auto path = DataDirectory() / "live_console_key.txt";
            std::string k;
            if (FILE* f = _wfopen(path.c_str(), L"rb"))
            {
                char buffer[64] = {};
                k.assign(buffer, std::fread(buffer, 1, sizeof(buffer) - 1, f));
                std::fclose(f);
            }
            k.erase(k.find_last_not_of(" \r\n\t") + 1);
            if (k.size() != 32)
            {
                std::random_device random;
                k = std::format("{:08X}{:08X}{:08X}{:08X}", random(), random(), random(), random());
                if (FILE* f = _wfopen(path.c_str(), L"wb"))
                {
                    std::fwrite(k.data(), 1, k.size(), f);
                    std::fclose(f);
                }
            }
            return k;
        }();
        return key;
    }

    // A profile without an Xbox LIVE account (made on a PC, by Xenia / Nexia, or offline on a console) gets one from
    // our server: a new
    // online XUID, kept in the profile's Account (or live_accounts.txt for a profile without one), so the profile is
    // that account from now on. Returns the profile as it is now.
    kernel::users::Profile EnsureLiveAccount(const kernel::users::Profile& profile)
    {
        if (profile.liveEnabled && (profile.onlineXuid >> 48) == 0x0009)
            return profile;
        std::string error;
        const auto online = live::Register(profile.gamertag, ConsoleKey(), &error);
        if (!online)
        {
            Log("Live: {} has no Xbox LIVE account and none could be made ({}); it stays offline", profile.gamertag,
                error);
            return profile;
        }
        if (!profile.copied)
            kernel::profile::SaveLocalOnlineXuid(profile.xuid, *online);
        else if (!kernel::profile::MakeLiveAccount(profile.xuid, *online))
        {
            Log("Live: {}'s Account could not be written; it stays offline", profile.gamertag);
            return profile;
        }
        kernel::users::SetLiveAccount(profile.xuid, *online);
        Log("Live: {} ({:016X}) is now an Xbox LIVE account: online XUID {:016X}", profile.gamertag, profile.xuid,
            *online);
        kernel::users::Profile now = profile;
        now.liveEnabled = true;
        now.onlineXuid = *online;
        return now;
    }

    std::string Utf8(const std::u16string& text)
    {
        const std::wstring wide(text.begin(), text.end());
        const int size = WideCharToMultiByte(CP_UTF8, 0, wide.data(), int(wide.size()), nullptr, 0, nullptr, nullptr);
        std::string out(size_t(size), '\0');
        WideCharToMultiByte(CP_UTF8, 0, wide.data(), int(wide.size()), out.data(), size, nullptr, nullptr);
        return out;
    }

    live::PublicProfile PublicProfileOf(const kernel::users::Gamercard& g)
    {
        live::PublicProfile card;
        card.gamerscore = g.gamerscore;
        card.zone = g.zone;
        card.reputation = g.reputation * 20; // XPROFILE_GAMERCARD_REP's 0-100
        card.motto = Utf8(g.motto);
        card.name = Utf8(g.name);
        card.location = Utf8(g.location);
        card.bio = Utf8(g.bio);
        return card;
    }

    constexpr uint32_t kDashTitle = 0xFFFE07D1;
    constexpr uint32_t kSettingGamerscore = 0x10040006; // XPROFILE_GAMERCARD_CRED
    constexpr uint32_t kSettingReputation = 0x5004000B; // XPROFILE_GAMERCARD_REP (float)
    constexpr uint32_t kXnSysProfileSettingChanged = 0x0000000E;
}

kernel::users::Gamercard kernel::users::ReadGamercard(uint64_t xuid)
{
    Gamercard g;
    auto setting = [&](uint32_t id) { return kernel::profile::ReadSetting(xuid, kDashTitle, id); };
    auto int32 = [](const kernel::profile::Setting& s) {
        return uint32_t(s.value[0]) << 24 | uint32_t(s.value[1]) << 16 | uint32_t(s.value[2]) << 8 | s.value[3];
    };
    // String settings: UTF-16BE in the extended bytes, up to a terminating 0.
    auto text = [&](uint32_t id) {
        std::u16string out;
        if (const auto s = setting(id))
            for (size_t i = 0; i + 1 < s->extended.size(); i += 2)
            {
                const char16_t c = char16_t(s->extended[i] << 8 | s->extended[i + 1]);
                if (c == 0)
                    break;
                out += c;
            }
        return out;
    };
    if (const auto s = setting(kSettingGamerscore))
        g.gamerscore = int32(*s);
    if (const auto s = setting(kSettingZone))
        g.zone = int32(*s);
    if (const auto s = setting(kSettingReputation))
    {
        // A float out of 100 (the user's profile: 100.0 = the five stars of 7_42_05).
        const uint32_t bits = int32(*s);
        float rep;
        std::memcpy(&rep, &bits, 4);
        if (rep >= 0 && rep <= 100)
            g.reputation = rep / 20;
    }
    g.motto = text(kSettingMotto);
    g.name = text(kSettingName);
    g.location = text(kSettingLocation);
    g.bio = text(kSettingBio);
    return g;
}

std::optional<std::string> kernel::users::WriteGamercard(uint32_t user, uint32_t settingId, const std::u16string& text)
{
    const auto profile = SignedIn(user);
    if (!profile)
        return std::string("not signed in");
    kernel::profile::Setting s;
    if (settingId == kSettingZone)
    {
        const uint32_t zone = text.empty() ? 0 : uint32_t(text[0] - u'0');
        if (zone > 4)
            Fatal("WriteGamercard: bad gamer zone {}", zone);
        s.type = 1; // XUSER_DATA_TYPE_INT32
        s.value[3] = uint8_t(zone);
    }
    else
    {
        static const std::map<uint32_t, size_t> kMaxLength = { { kSettingMotto, 21 }, { kSettingName, 129 },
                                                               { kSettingLocation, 40 }, { kSettingBio, 499 } };
        const auto max = kMaxLength.find(settingId);
        if (max == kMaxLength.end())
            Fatal("WriteGamercard: setting {:08X} is not a gamercard text", settingId);
        // XUSER_DATA_TYPE_UNICODE: the byte size (with the terminating 0) in the value, the UTF-16BE text after it.
        const std::u16string value = text.substr(0, max->second);
        s.type = 4;
        for (char16_t c : value)
        {
            s.extended.push_back(uint8_t(c >> 8));
            s.extended.push_back(uint8_t(c));
        }
        s.extended.push_back(0);
        s.extended.push_back(0);
        const uint32_t size = uint32_t(s.extended.size());
        s.value[0] = uint8_t(size >> 24);
        s.value[1] = uint8_t(size >> 16);
        s.value[2] = uint8_t(size >> 8);
        s.value[3] = uint8_t(size);
    }
    kernel::profile::WriteSetting(profile->xuid, kDashTitle, settingId, s);
    Log("Live: {} changed profile setting {:08X}", profile->gamertag, settingId);
    kernel::BroadcastNotification(kXnSysProfileSettingChanged, 1u << user);
    if (!live::SignedIn(user))
        return std::nullopt;
    return live::UpdateProfile(user, PublicProfileOf(ReadGamercard(profile->xuid)));
}

void kernel::users::LiveSignIn(uint32_t user, const Profile& signedIn)
{
    if (Settings().liveServer.empty() || user >= 4)
        return;
    // One at a time per user: the background sign-in users::SignIn starts and XamShowSigninUIp's (which then finds
    // the user on LIVE). A profile signed out meanwhile is not signed in to LIVE afterwards.
    static std::array<std::mutex, 4> mutexes;
    std::lock_guard lock(mutexes[user]);
    if (const auto now = SignedIn(user); !now || now->xuid != signedIn.xuid)
        return;
    // Decided already by the sign-in's own attempt (XamShowSigninUIp comes after it): on Xbox LIVE, or kept offline
    // until Connect to Xbox LIVE.
    if (live::SignedIn(user) || live::Held(user))
        return;
    if (!(signedIn.liveEnabled && (signedIn.onlineXuid >> 48) == 0x0009) && !live::Connect())
    {
        // Kept offline like a profile whose sign-in found no server (Connect to Xbox LIVE in the Guide tries again).
        Log("Live: {} has no Xbox LIVE account yet and the server does not answer: offline", signedIn.gamertag);
        live::Hold(user);
        return;
    }
    const Profile profile = EnsureLiveAccount(signedIn);
    // XAM logs on only Xbox LIVE-enabled accounts (0x8199C838: reserved flag 0x20000000) with an online XUID.
    if (!profile.liveEnabled || (profile.onlineXuid >> 48) != 0x0009)
        return;
    // The gamercard others see (gamerscore, zone, motto, the Personal Profile) and the 64x64 gamer picture.
    live::PublicProfile card = PublicProfileOf(kernel::users::ReadGamercard(profile.xuid));
    card.picture = kernel::profile::GamerPicture(profile.xuid);
    card.pictureSmall = kernel::profile::GamerPicture(profile.xuid, true);
    // The titles it has played (Compare Games on other consoles), published with the sign-in.
    std::vector<live::TitleSummary> titles;
    auto utf8 = [](const std::u16string& text) {
        const int n = WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<const wchar_t*>(text.data()), int(text.size()),
                                          nullptr, 0, nullptr, nullptr);
        std::string out(size_t(n), '\0');
        WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<const wchar_t*>(text.data()), int(text.size()), out.data(), n,
                            nullptr, nullptr);
        return out;
    };
    for (const guide::host::PlayedTitle& t : guide::host::PlayedTitles(user))
    {
        live::TitleSummary summary{ t.titleId, utf8(t.name), t.achievements, t.earned, t.gamerscore, t.tile, {} };
        for (const xui::AchievementEntry& a : guide::host::PlayedTitleAchievements(user, t.titleId))
            summary.list.push_back({ utf8(a.label), utf8(a.description), utf8(a.unachieved), a.gamerscore, a.earned,
                                     a.showUnachieved, a.unlockTime });
        titles.push_back(std::move(summary));
    }
    live::SetPlayedTitles(user, std::move(titles));
    if (const auto error = live::SignIn(user, profile.onlineXuid, profile.gamertag, card))
        Log("Live: {} could not sign in ({}): offline until Connect to Xbox LIVE", profile.gamertag, *error);
    kernel::RefreshPresence(user); // (signing in starts with the title alone)
}

namespace
{
    // The user whose session asks the server for other players' profiles: any signed in to our Xbox LIVE.
    std::optional<uint32_t> LiveUser()
    {
        for (uint32_t user = 0; user < 4; user++)
            if (live::SignedIn(user))
                return user;
        return std::nullopt;
    }
}

std::optional<std::vector<uint8_t>> kernel::LiveGamerPicture(uint64_t onlineXuid, bool size32)
{
    const auto user = LiveUser();
    if (!user)
        return std::nullopt;
    const auto profile = live::GetProfile(*user, onlineXuid);
    if (!profile)
        return std::nullopt;
    const std::vector<uint8_t>& png = size32 ? profile->pictureSmall : profile->picture;
    return png.empty() ? std::nullopt : std::optional(png);
}

void kernel::PrefetchGamerPictures(std::vector<uint64_t> onlineXuids)
{
    if (onlineXuids.empty() || !LiveUser())
        return;
    std::thread([xuids = std::move(onlineXuids)] {
        if (const auto user = LiveUser())
            for (const uint64_t xuid : xuids)
                live::GetProfile(*user, xuid);
    }).detach();
}

void kernel::users::LiveSignOut(uint32_t user)
{
    if (user < g_liveLogonCommitted.size())
        g_liveLogonCommitted[user] = false;
    live::SignOut(user);
}

void kernel::LiveLogonCommitted(uint32_t user)
{
    if (user < g_liveLogonCommitted.size())
        g_liveLogonCommitted[user] = true;
}

namespace
{
    constexpr uint32_t kFriendRecordSize = 0xC4; // XONLINE_FRIEND as XAM copies it (0x818DE0F0)

    void PutBe32(std::vector<uint8_t>& v, size_t at, uint32_t x)
    {
        for (int i = 0; i < 4; i++)
            v[at + i] = uint8_t(x >> (24 - 8 * i));
    }

    void PutBe64(std::vector<uint8_t>& v, size_t at, uint64_t x)
    {
        PutBe32(v, at, uint32_t(x >> 32));
        PutBe32(v, at + 4, uint32_t(x));
    }

    // Unix time -> FILETIME. (Xbox LIVE's own times count seconds from 2003-01-01: XAM converts them with
    // (t + 0x2F422C080) * 10^7; our server sends Unix times.)
    uint64_t FileTimeFromUnix(int64_t unix)
    {
        return unix > 0 ? uint64_t(unix + 11644473600ll) * 10000000ull : 0;
    }

    // XONLINE_FRIEND (0xC4): xuid 0x00, gamertag 0x08, state 0x18, sessionID 0x1C, titleID 0x24, ftUserTime 0x28,
    // xnkidInvite 0x30, gameinviteTime 0x38, cchRichPresence 0x40, wszRichPresence[64] 0x44. State: ONLINE 1,
    // PLAYING 2 (in a title other than the dashboard), console type Xbox 360 0x1000; SENTREQUEST 0x40000000,
    // RECEIVEDREQUEST 0x80000000. XAM zeroes the session, title and rich presence of offline friends and keeps
    // ftUserTime (their last time online) and only the top six state bits.
    std::vector<uint8_t> FriendRecord(const live::Friend& f)
    {
        std::vector<uint8_t> r(kFriendRecordSize, 0);
        PutBe64(r, 0x00, f.xuid);
        for (size_t i = 0; i < f.gamertag.size() && i < 15; i++)
            r[0x08 + i] = uint8_t(f.gamertag[i]);
        uint32_t state = f.request == "incoming" ? 0x80000000u : f.request == "outgoing" ? 0x40000000u : 0;
        if (f.online)
        {
            state |= 0x1 | 0x1000;
            if (f.titleId != 0xFFFE07D1)
                state |= 0x2;
            PutBe32(r, 0x24, f.titleId);
            const std::string& text = f.rich.empty() ? f.title : f.rich;
            const int wide = MultiByteToWideChar(CP_UTF8, 0, text.data(), int(text.size()), nullptr, 0);
            std::wstring w(size_t(wide), L'\0');
            MultiByteToWideChar(CP_UTF8, 0, text.data(), int(text.size()), w.data(), wide);
            const size_t n = std::min<size_t>(w.size(), 63);
            for (size_t i = 0; i < n; i++)
            {
                r[0x44 + 2 * i] = uint8_t(w[i] >> 8);
                r[0x44 + 2 * i + 1] = uint8_t(w[i]);
            }
            PutBe32(r, 0x40, n ? uint32_t(n + 1) : 0);
            PutBe64(r, 0x28, FileTimeFromUnix(std::time(nullptr)));
        }
        else
            PutBe64(r, 0x28, FileTimeFromUnix(f.lastSeen));
        PutBe32(r, 0x18, state);
        return r;
    }

    // XAM's friends order (0x818DD0B0): received requests, sent requests, online, online in this title, then by
    // gamertag (invites, not modelled here, come first on a console).
    bool FriendBefore(const live::Friend& a, const live::Friend& b)
    {
        auto key = [](const live::Friend& f) {
            return std::make_tuple(f.request == "incoming" ? 0 : 1, f.request == "outgoing" ? 0 : 1, f.online ? 0 : 1,
                                   f.online && f.titleId == 0xFFFE07D1 ? 0 : 1);
        };
        if (key(a) != key(b))
            return key(a) < key(b);
        return _stricmp(a.gamertag.c_str(), b.gamertag.c_str()) < 0;
    }
}

// XFriendsCreateEnumerator (XLiveBase 0x58020, XAM 0x818DDF40): arguments = 16-byte slots {u32 size, pad, u64 pointer}:
// &UserIndex, &StartingIndex, &FriendsToReturn, pcbBuffer, phEnum. The user needs an online XUID (XamUserGetXUID
// flags 2), FriendsToReturn and StartingIndex below 100; else E_INVALIDARG and no handle. *pcbBuffer =
// FriendsToReturn * 0xC4; the handle enumerates (0x58021) the friends list from StartingIndex, FriendsToReturn per
// call, ERROR_NO_MORE_FILES at the end. Here the list is the live client's (the server's) at creation.
uint32_t kernel::FriendsCreateEnumerator(uint32_t arguments)
{
    auto& mem = guest::g_memory;
    auto slot = [&](uint32_t i) { return mem.Read32(arguments + i * 16 + 12); }; // {u32 size, pad, u64 pointer}
    const uint32_t user = mem.Read32(slot(0)), start = mem.Read32(slot(1)), count = mem.Read32(slot(2));
    const uint32_t sizePtr = slot(3), handlePtr = slot(4);
    if (handlePtr)
        mem.Write32(handlePtr, 0);
    if (user >= 4 || !UserOnLive(user) || count >= 0x65 || start >= 100)
    {
        Log("XFriendsCreateEnumerator(user {}, start {}, count {}): E_INVALIDARG{}", user, start, count,
            user < 4 && !UserOnLive(user) ? " (not on Xbox LIVE)" : "");
        return 0x80070057;
    }
    auto friends = live::Friends(user);
    std::sort(friends.begin(), friends.end(), FriendBefore);
    std::vector<std::vector<uint8_t>> items;
    for (size_t i = start; i < friends.size(); i++)
        items.push_back(FriendRecord(friends[i]));
    EnumeratorInfo info;
    info.appId = 0xFC;
    info.message = 0x58021;
    info.closeMessage = 0x58022;
    info.userIndex = user;
    info.itemsPerEnumerate = count;
    if (sizePtr)
        mem.Write32(sizePtr, count * kFriendRecordSize);
    const uint32_t handle = CreateHostEnumerator(info, kFriendRecordSize, std::move(items));
    if (handlePtr)
        mem.Write32(handlePtr, handle);
    Log("XFriendsCreateEnumerator(user {}, start {}, count {}): {} friends", user, start, count,
        friends.size() > start ? friends.size() - start : 0);
    return 0;
}

// LiveHive: Xbox LIVE's downloaded configuration (service URLs and settings by key): the live server's values
// (sent when the console connects); keys it lacks are "not set", except EPIX's manifest location, which defaults
// to the runtime's own host (the hub's online home: kernel/xam_xhttp.cpp, epix_home.cpp; the dash fetches
// EpixXManUriRoot + EpixXManManifestUriPath + "dashhome.xml" with $locale replaced).
std::optional<std::string> kernel::LiveHiveValue(const std::string& key)
{
    if (auto value = live::LiveHive(key))
        return value;
    if (key == "EpixXManUriRoot")
        return "http://epix.localhost/";
    if (key == "EpixXManManifestUriPath")
        return "$locale/";
    return std::nullopt;
}

bool kernel::ConsoleOnLive()
{
    for (uint32_t user = 0; user < 4; user++)
        if (UserOnLive(user))
            return true;
    return false;
}

namespace
{
    // XAM 0x8169AD08: the cached value (0x8169AA48): found -> copied, 0; missing key -> 1 (S_FALSE, buffer
    // untouched); too small -> 0x8007007A. With an overlapped a non-negative result completes it at once
    // (Internal 0, dwExtendedError = the result) and E_PENDING is returned.
    uint32_t QueryLiveHive(const std::string& key, uint32_t buffer, uint32_t size, uint32_t overlapped, bool wide)
    {
        auto& mem = guest::g_memory;
        // Offline the hive was never downloaded: XAM finds none in memory, asks XLiveBase for the logon state
        // (0x58003: disconnected) and returns XONLINE_E_LOGON_NOT_LOGGED_ON without touching the buffer.
        if (!kernel::ConsoleOnLive())
        {
            Log("XamQueryLiveHive(\"{}\"): not logged on to Xbox LIVE", key);
            return 0x80151802;
        }
        uint32_t result = 1;
        if (const auto value = kernel::LiveHiveValue(key))
        {
            if (size < value->size() + 1)
                result = 0x8007007A;
            else
            {
                for (size_t i = 0; i <= value->size(); i++)
                {
                    const uint8_t c = i < value->size() ? uint8_t((*value)[i]) : 0;
                    if (wide)
                        mem.Write16(buffer + uint32_t(i) * 2, c);
                    else
                        mem.Write8(buffer + uint32_t(i), c);
                }
                result = 0;
            }
        }
        Log("XamQueryLiveHive(\"{}\"): {}", key, result == 1 ? "not set" : result == 0 ? "found" : "buffer too small");
        if (int32_t(result) >= 0 && overlapped)
        {
            mem.Write32(overlapped + 0x18, result);
            mem.Write32(overlapped + 0x00, 0);
            return 0x8000000A;
        }
        return result;
    }
}

// HRESULT XamQueryLiveHiveA(LPCSTR Key, LPSTR Buffer, DWORD Size, PXOVERLAPPED Overlapped)
GUEST_IMPORT(XamQueryLiveHiveA)
{
    const std::string key = ctx.r3.u32 ? reinterpret_cast<const char*>(guest::g_memory.Translate(ctx.r3.u32)) : "";
    ctx.r3.u64 = QueryLiveHive(key, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, false);
}

// HRESULT XamQueryLiveHiveW(LPCWSTR Key, LPWSTR Buffer, DWORD Size, PXOVERLAPPED Overlapped) (XAM 0x8169ADA8): the
// key converted to multibyte, the value written as UTF-16.
GUEST_IMPORT(XamQueryLiveHiveW)
{
    std::string key;
    for (const char16_t c : kernel::ReadGuestWideString(ctx.r3.u32, 0xFF))
        key += c < 0x80 ? char(c) : char(63);
    ctx.r3.u64 = QueryLiveHive(key, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, true);
}

// HRESULT XamGetLiveHiveValueW(LPCWSTR Key, LPWSTR Buffer, DWORD Characters, DWORD, PXOVERLAPPED Overlapped)
// (ordinal 1801; the 9199 dashboard asks it for GameImageAssetUriRoot). As 9199's XAM (0x818F7470, rebuilt from the
// owner's system update): a null key, buffer or size is E_INVALIDARG; it asks XLiveBase for the logon state
// (0x58003) and, not logged on, returns XONLINE_E_LOGON_NOT_LOGGED_ON; else it clears the buffer and looks the key
// up (XLiveBase 0x27001 to app 0xFE), with an overlapped E_PENDING. The lookup is XamQueryLiveHive's.
GUEST_IMPORT(XamGetLiveHiveValueW)
{
    auto& mem = guest::g_memory;
    const uint32_t keyAddress = ctx.r3.u32, buffer = ctx.r4.u32, characters = ctx.r5.u32, overlapped = ctx.r7.u32;
    if (!keyAddress || !buffer || !characters)
    {
        ctx.r3.u64 = 0x80070057;
        return;
    }
    std::string key;
    for (const char16_t c : kernel::ReadGuestWideString(keyAddress, 0xFF))
        key += c < 0x80 ? char(c) : char(63);
    if (kernel::ConsoleOnLive())
        for (uint32_t i = 0; i < characters; i++)
            mem.Write16(buffer + i * 2, 0);
    ctx.r3.u64 = QueryLiveHive(key, buffer, characters, overlapped, true);
}

// HRESULT XamGetLiveHiveValueA(LPCSTR Key, LPSTR Buffer, DWORD Size, DWORD, PXOVERLAPPED Overlapped) (ordinal 1800):
// 9199's XAM enters the same worker (0x818F7558) with its multibyte flag set: as XamGetLiveHiveValueW, in bytes.
GUEST_IMPORT(XamGetLiveHiveValueA)
{
    auto& mem = guest::g_memory;
    const uint32_t keyAddress = ctx.r3.u32, buffer = ctx.r4.u32, size = ctx.r5.u32, overlapped = ctx.r7.u32;
    if (!keyAddress || !buffer || !size)
    {
        ctx.r3.u64 = 0x80070057;
        return;
    }
    const std::string key = reinterpret_cast<const char*>(mem.Translate(keyAddress));
    if (kernel::ConsoleOnLive())
        std::memset(mem.Translate(buffer), 0, size);
    ctx.r3.u64 = QueryLiveHive(key, buffer, size, overlapped, false);
}

// XMessageEnumerate (XLiveBase 0x58024, XAM 0x818E4DA0; dash 0x9224B730): not an enumerator: argument slots &Xuid,
// pcItems, Buffer. The user with that online XUID (logged on to LIVE) gets up to *pcItems (below 0x7E) message
// summaries, newest first, 0x60 bytes each: sender XUID +0x00, sent FILETIME +0x10, message ID +0x18, flags +0x1C
// (0x10 read), lifetime in minutes +0x24 (30 days), attribute marker +0x26, type +0x28 (8: the dash's text message;
// the other types are not decoded), sender gamertag +0x29 (15 bytes); the buffer is zeroed to *pcItems records and
// *pcItems becomes the count. The dash counts the unread ones (Lua GetUnreadMessageCount 0x927DCB68). The list
// itself, reading and deleting are the Guide's (XamShowMessagesUI).
uint32_t kernel::MessageEnumerate(uint32_t arguments)
{
    auto& mem = guest::g_memory;
    auto slot = [&](uint32_t i) { return mem.Read32(arguments + i * 16 + 12); };
    const uint32_t xuidPtr = slot(0), countPtr = slot(1), buffer = slot(2);
    if (buffer == 0 || countPtr == 0 || mem.Read32(countPtr) >= 0x7E)
        return 0x80070057;
    const uint64_t xuid = xuidPtr ? mem.Read64(xuidPtr) : 0;
    std::optional<uint32_t> user;
    for (uint32_t u = 0; u < 4; u++)
    {
        const auto p = users::SignedIn(u);
        if (p && UserOnLive(u) && p->onlineXuid == xuid)
            user = u;
    }
    const uint32_t capacity = mem.Read32(countPtr);
    if (!user)
    {
        mem.Write32(countPtr, 0);
        return 0x80070057;
    }
    std::memset(mem.Translate(buffer), 0, size_t(capacity) * 0x60);
    const auto messages = live::Messages(*user);
    uint32_t n = 0;
    for (const live::Message& m : messages)
    {
        if (n == capacity)
            break;
        std::vector<uint8_t> r(0x60, 0);
        PutBe64(r, 0x00, m.from);
        PutBe64(r, 0x10, FileTimeFromUnix(m.sent));
        PutBe32(r, 0x18, m.id);
        PutBe32(r, 0x1C, m.read ? 0x10 : 0);
        r[0x24] = 0xA8;
        r[0x25] = 0xC0;
        r[0x27] = m.text.empty() ? 0 : 1;
        r[0x28] = 8;
        for (size_t i = 0; i < m.fromGamertag.size() && i < 15; i++)
            r[0x29 + i] = uint8_t(m.fromGamertag[i]);
        std::memcpy(mem.Translate(buffer + n * 0x60), r.data(), r.size());
        n++;
    }
    mem.Write32(countPtr, n);
    return 0;
}

namespace
{
    bool AnyoneOnLive()
    {
        for (uint32_t user = 0; user < 4; user++)
            if (kernel::UserOnLive(user))
                return true;
        return false;
    }
}

// DWORD XNetLogonGetServiceNetworkID() (XAM 0x81739708): the logon reply's service network (+0x1100), 0 before any
// logon. The dash and XAM only test for "PROD" (0x50524F44, else the test environment: dash 0x928062C0).
GUEST_IMPORT(XNetLogonGetServiceNetworkID)
{
    ctx.r3.u64 = AnyoneOnLive() ? 0x50524F44 : 0;
}

// XOnlineGetServiceInfo (XLiveBase 0x58007, XAM 0x818CB028 -> XNetLogonGetServiceInfo 0x81735850): the logon reply's
// entry {id, IP, port} for a LIVE service. Not logged on: 0x80151802; the ports' own LIVE lists no services:
// 0x80151100 (service not found; the dash maps it to ERROR 0x4DB).
uint32_t kernel::OnlineGetServiceInfo(uint32_t serviceId, uint32_t info)
{
    (void)info;
    const uint32_t result = AnyoneOnLive() ? 0x80151100 : 0x80151802;
    Log("XOnlineGetServiceInfo({:08X}): {:08X}", serviceId, result);
    return result;
}

// DWORD XamGetErrorStringFromWebService(LPWSTR Buffer, DWORD cch, HRESULT hr, PXOVERLAPPED Overlapped) (XAM 0x816FCBE8
// -> 0x2E004 -> 0x816FCCA8): an HTTP GET of {LiveHive:LiveServiceUrl.settings}/settings/strings/<hr>.<locale> from
// LIVE. No web services here: the request fails (0x80072EFD); the dash then shows its own text with the HRESULT.
GUEST_IMPORT(XamGetErrorStringFromWebService)
{
    const uint32_t buffer = ctx.r3.u32, cch = ctx.r4.u32, hr = ctx.r5.u32, overlapped = ctx.r6.u32;
    if (buffer == 0 || cch == 0 || hr == 0)
    {
        ctx.r3.u64 = 0x80070057;
        return;
    }
    Log("XamGetErrorStringFromWebService({:08X}): no web services on this Xbox LIVE", hr);
    constexpr uint32_t kCannotConnect = 0x80072EFD;
    if (overlapped)
    {
        kernel::CompleteOverlapped(overlapped, 0x65B, kCannotConnect, 0);
        ctx.r3.u64 = 0x800703E5;
        return;
    }
    ctx.r3.u64 = kCannotConnect;
}
