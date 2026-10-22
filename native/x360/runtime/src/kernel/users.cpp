// Local profiles and sign-in state (kernel/users.h).

#include "kernel/users.h"

#include "kernel/kernel.h"
#include "kernel/profile_import.h"
#include "kernel/profile_store.h"
#include "live/client.h"
#include "settings.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <format>
#include <mutex>
#include <thread>

namespace kernel::users
{
    namespace
    {
        constexpr size_t kNoProfile = ~size_t(0);

        std::mutex g_mutex;
        std::array<size_t, kMaxUsers> g_signedIn = { kNoProfile, kNoProfile, kNoProfile, kNoProfile };
    }

    namespace
    {
        // Built once; profiles copied later (the Guide's Download Profile) are appended without moving the others,
        // so references into it and the signed-in indices stay valid (room is reserved).
        constexpr size_t kMaxAddedProfiles = 64;

        Profile CopiedProfile(uint64_t xuid, const profile::Account& account)
        {
            std::string name;
            for (char16_t c : account.gamertag)
                name += c < 0x80 ? char(c) : '?'; // gamertags are ASCII
            return { name, xuid, true, account.LiveEnabled(), account.onlineXuid };
        }

        std::vector<Profile>& List()
        {
            static std::vector<Profile> profiles = [] {
                std::vector<Profile> copied;
                for (const uint64_t xuid : profile::StoredProfiles())
                {
                    if (const auto account = profile::ReadAccount(xuid))
                        copied.push_back(CopiedProfile(xuid, *account));
                }
                // Gamertags compare without case, as on Xbox LIVE.
                auto same = [](const std::string& a, const std::string& b) {
                    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin(), [](char x, char y) {
                        return std::tolower(uint8_t(x)) == std::tolower(uint8_t(y));
                    });
                };
                std::vector<Profile> list;
                std::vector<bool> used(copied.size());
                for (const std::string& name : Settings().profiles)
                {
                    if (name.empty())
                        continue;
                    size_t i = 0;
                    while (i < copied.size() && (used[i] || !same(copied[i].gamertag, name)))
                        i++;
                    if (i < copied.size())
                    {
                        used[i] = true;
                        list.push_back(copied[i]);
                    }
                    else if (profile::IsDeletedGamertag(name))
                    {
                        Log("Users: {} (console_settings.ini) was deleted on this console; not listed", name);
                    }
                    else
                    {
                        Profile local{ name, ConsoleSettings::OfflineXuid(name) };
                        // An Xbox LIVE account our server made for it earlier (kernel/xam_live.cpp)
                        if (const auto online = profile::LocalOnlineXuid(local.xuid))
                        {
                            local.liveEnabled = true;
                            local.onlineXuid = *online;
                        }
                        list.push_back(local);
                    }
                }
                for (size_t i = 0; i < copied.size(); i++)
                    if (!used[i])
                        list.push_back(copied[i]);
                for (const Profile& p : list)
                    Log("Users: profile {} ({:016X}{})", p.gamertag, p.xuid,
                        p.copied ? p.liveEnabled ? ", copied from a console, Xbox LIVE account" : ", copied from a console" : "");
                list.reserve(list.size() + kMaxAddedProfiles);
                return list;
            }();
            return profiles;
        }
    }

    const std::vector<Profile>& Profiles()
    {
        return List();
    }

    size_t AddStoredProfiles()
    {
        std::lock_guard lock(g_mutex);
        std::vector<Profile>& list = List();
        size_t added = 0;
        for (const uint64_t xuid : profile::StoredProfiles())
        {
            if (std::any_of(list.begin(), list.end(), [&](const Profile& p) { return p.xuid == xuid; }))
                continue;
            const auto account = profile::ReadAccount(xuid);
            if (!account)
            {
                Log("Users: {:016X}: its Account file does not decode; not listed", xuid);
                continue;
            }
            if (list.size() == list.capacity())
            {
                Log("Users: too many profiles copied during this run; restart to list {:016X}", xuid);
                break;
            }
            list.push_back(CopiedProfile(xuid, *account));
            Log("Users: profile {} ({:016X}, copied from a console{}) added", list.back().gamertag, xuid,
                account->LiveEnabled() ? ", Xbox LIVE account" : "");
            added++;
        }
        return added;
    }

    void SaveSignIns()
    {
        std::string text;
        {
            std::lock_guard lock(g_mutex);
            // " offline": a LIVE profile not on our Xbox LIVE now stays offline when it is signed in again.
            for (uint32_t user = 0; user < kMaxUsers; user++)
                if (g_signedIn[user] != kNoProfile)
                {
                    const Profile& p = Profiles()[g_signedIn[user]];
                    text += std::format("{} {:016X}{}\n", user, p.xuid,
                                        p.liveEnabled && !live::SignedIn(user) ? " offline" : "");
                }
        }
        if (FILE* f = _wfopen((DataDirectory() / "signed_in.txt").c_str(), L"wb"))
        {
            std::fwrite(text.data(), 1, text.size(), f);
            std::fclose(f);
        }
    }

    size_t RestoreSignIns()
    {
        const auto path = DataDirectory() / "signed_in.txt";
        FILE* f = _wfopen(path.c_str(), L"rb");
        if (!f)
            return 0;
        struct Entry { uint32_t user; uint64_t xuid; bool offline; };
        std::vector<Entry> entries;
        char line[128];
        while (std::fgets(line, sizeof(line), f))
        {
            unsigned user = 0;
            unsigned long long xuid = 0;
            if (std::sscanf(line, "%u %llx", &user, &xuid) == 2)
                entries.push_back({ user, xuid, std::strstr(line, "offline") != nullptr });
        }
        std::fclose(f);
        std::error_code ec;
        std::filesystem::remove(path, ec);
        size_t restored = 0;
        for (const auto& [u, x, offline] : entries)
        {
            const auto& list = Profiles();
            const auto it = std::find_if(list.begin(), list.end(), [&](const Profile& p) { return p.xuid == x; });
            if (u >= kMaxUsers || it == list.end())
                continue;
            if (offline)
                live::Hold(u); // LiveSignIn leaves it offline (Connect to Xbox LIVE signs it in)
            SignIn(u, size_t(it - list.begin()));
            restored++;
        }
        return restored;
    }

    void SetLiveAccount(uint64_t xuid, uint64_t onlineXuid)
    {
        std::lock_guard lock(g_mutex);
        for (Profile& p : List())
            if (p.xuid == xuid)
            {
                p.liveEnabled = true;
                p.onlineXuid = onlineXuid;
            }
    }

    void RemoveProfile(uint64_t xuid)
    {
        std::lock_guard lock(g_mutex);
        std::vector<Profile>& list = List();
        const auto it = std::find_if(list.begin(), list.end(), [&](const Profile& p) { return p.xuid == xuid; });
        if (it == list.end())
            return;
        const size_t index = size_t(it - list.begin());
        for (size_t& signedIn : g_signedIn)
        {
            if (signedIn == index)
                Fatal("Users: {} ({:016X}) was deleted while signed in (behaviour not established)", it->gamertag,
                      xuid);
            if (signedIn != kNoProfile && signedIn > index)
                signedIn--;
        }
        Log("Users: profile {} ({:016X}) removed", it->gamertag, xuid);
        list.erase(it);
    }

    void Initialize()
    {
        std::lock_guard lock(g_mutex);
        if (Profiles().empty())
        {
            Log("Users: no profiles on the console; nobody signed in");
            return;
        }
        g_signedIn[0] = 0;
        Log("Users: {} signed in to user 0 ({} profiles on the console)", Profiles()[0].gamertag, Profiles().size());
    }

    std::optional<Profile> SignedIn(uint32_t user)
    {
        if (user >= kMaxUsers)
            return std::nullopt;
        std::lock_guard lock(g_mutex);
        if (g_signedIn[user] == kNoProfile)
            return std::nullopt;
        return Profiles()[g_signedIn[user]];
    }

    uint64_t Xuid(uint32_t user)
    {
        const auto profile = SignedIn(user);
        return profile ? profile->xuid : 0;
    }

    std::optional<uint32_t> UserOfXuid(uint64_t xuid)
    {
        std::lock_guard lock(g_mutex);
        for (uint32_t user = 0; user < kMaxUsers; user++)
            if (g_signedIn[user] != kNoProfile && Profiles()[g_signedIn[user]].xuid == xuid)
                return user;
        return std::nullopt;
    }

    uint32_t SignedInMask()
    {
        std::lock_guard lock(g_mutex);
        uint32_t mask = 0;
        for (uint32_t user = 0; user < kMaxUsers; user++)
            if (g_signedIn[user] != kNoProfile)
                mask |= 1u << user;
        return mask;
    }

    void SignIn(uint32_t user, size_t profile)
    {
        if (user >= kMaxUsers || profile >= Profiles().size())
            Fatal("Users: cannot sign profile {} in to user {}", profile, user);
        std::vector<uint32_t> signedOut;
        {
            std::lock_guard lock(g_mutex);
            for (uint32_t other = 0; other < kMaxUsers; other++)
            {
                if (other != user && g_signedIn[other] == profile)
                {
                    Log("Users: {} signed out of user {}", Profiles()[profile].gamertag, other);
                    g_signedIn[other] = kNoProfile;
                    signedOut.push_back(other);
                }
            }
            if (g_signedIn[user] != kNoProfile && g_signedIn[user] != profile)
            {
                Log("Users: {} signed out of user {}", Profiles()[g_signedIn[user]].gamertag, user);
                signedOut.push_back(user); // off our Xbox LIVE too (its LIVE state is the old profile's)
            }
            g_signedIn[user] = profile;
            Log("Users: {} signed in to user {}", Profiles()[profile].gamertag, user);
        }
        for (const uint32_t other : signedOut)
            LiveSignOut(other);
        // Our Xbox LIVE in the background: a console's local sign-in (XamUserLogonEx) is immediate and its LIVE logon
        // comes later (XamShowSigninUIp, which waits for this one: LiveSignIn is serialized per user). Signing in here
        // synchronously held XamUserLogonEx for the server's round trips; over the Internet the dash's main thread
        // reached its "Signing in" context before the logon returned and never loaded the hub (2026-10-16).
        std::thread([user, p = Profiles()[profile]] { LiveSignIn(user, p); }).detach();
    }

    void SignOut(uint32_t user)
    {
        {
            std::lock_guard lock(g_mutex);
            if (user >= kMaxUsers || g_signedIn[user] == kNoProfile)
                return;
            Log("Users: {} signed out of user {}", Profiles()[g_signedIn[user]].gamertag, user);
            g_signedIn[user] = kNoProfile;
        }
        LiveSignOut(user);
    }
}

bool kernel::UserSignedIn(uint32_t user)
{
    return users::SignedIn(user).has_value();
}
