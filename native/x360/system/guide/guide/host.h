#pragma once

// What the Guide library (native/x360/system/guide: the Xbox Guide's screens and behaviour) needs from the runtime that
// builds it. Besides the functions below, defined once in the runtime:
// - "log.h" on the include path with Log(fmt, ...) and Fatal(fmt, ...) (std::format style), as both runtimes have;
// - the XUI library (native/x360/system/xui: the drawing) and the live client (native/x360/system/live: friends, messages,
//   party, voice) in the same target; the Guide calls both directly.
// Interface and how to adopt it: docs/x360/shared_guide.md.

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "xui/system_ui.h"

namespace guide
{
    // A profile on the console (the sign-in pane lists them in this order).
    struct Profile
    {
        std::string gamertag; // UTF-8
        uint64_t xuid = 0;        // its offline XUID (the profile's ID on the console)
        bool liveEnabled = false; // its Account is an Xbox LIVE account
        uint64_t onlineXuid = 0;  // its Account's online XUID (0 if none)
    };

    // A profile's own gamercard and Personal Profile, as its dashboard GPD (title FFFE07D1) keeps them.
    struct Gamercard
    {
        std::optional<uint32_t> gamerscore; // XPROFILE_GAMERCARD_CRED (nullopt: the GPD has none)
        uint32_t zone = 0;                  // XPROFILE_GAMERCARD_ZONE: 0 none, 1 Recreation ... 4 Underground
        float reputation = 0;               // stars, 0-5 (XPROFILE_GAMERCARD_REP is out of 100)
        std::u16string motto, name, location, bio;
    };

    // The settings the Guide's Edit Profile changes (host::WriteGamercard), the ids the user's console profile uses.
    constexpr uint32_t kSettingZone = 0x10040004;     // XPROFILE_GAMERCARD_ZONE (the text is the zone's digit)
    constexpr uint32_t kSettingMotto = 0x402C0011;    // XPROFILE_GAMERCARD_MOTTO, 21 characters
    constexpr uint32_t kSettingName = 0x41040040;     // the Personal Profile's name, 129 characters
    constexpr uint32_t kSettingLocation = 0x40520041; // its location, 40 characters
    constexpr uint32_t kSettingBio = 0x43E80043;      // its bio, 499 characters

    // The runtime's open system UI, as host::OpenSystemUi gave it to the Guide.
    class SystemUi
    {
    public:
        virtual ~SystemUi() = default;
        // Closes it (as a cancel: the popup's closing animation, then XN_SYS_UI 0) unless it has closed already;
        // `why` and `user` are logged. True if this call closed it.
        virtual bool Close(const char* why, uint32_t user) = 0;
        // Whether it has closed (by Close, or by the runtime: XamAppUnloadStack, a title's XCancelOverlapped).
        virtual bool Closed() const = 0;
    };
}

namespace guide::host
{
    // ---- The system UI (the runtime's XamShow*UI machinery: one system UI at a time) ----

    // Opens the runtime's system UI for the Guide on `user`'s controller: XN_SYS_UI 1, the title's controllers read
    // as idle, and each frame the buttons newly pressed on that controller go to `onPress` with the UI they belong to
    // (stick directions as the d-pad, triggers as 0x0400 / 0x0800, as xui::PanePress expects); it returns true once
    // the UI has closed. XN_SYS_UI 0 follows when it has closed and its popup has gone (xui::ClosePopup). The Guide
    // draws itself through xui. `onPress` may be called before OpenSystemUi returns.
    // Returns nullptr when another system UI is open, with that one's kind ("message box", ...) in `busyKind`.
    std::shared_ptr<SystemUi> OpenSystemUi(uint32_t user,
                                           std::function<bool(SystemUi& ui, uint32_t user, uint16_t pressed)> onPress,
                                           std::string& busyKind);
    // Hands the controllers back after a SystemUi::Close made outside `onPress` (the Guide button closing the Guide).
    void EndSystemUiInput();

    // ---- Profiles and sign-in ----

    // The profiles on this console.
    std::vector<Profile> Profiles();
    // The profile signed in to `user` (0-3), if any.
    std::optional<Profile> SignedIn(uint32_t user);
    // The user index a profile with this (offline) XUID is signed in to, if any.
    std::optional<uint32_t> UserOfXuid(uint64_t xuid);
    // Whether `user` is signed in to our Xbox LIVE (the live server accepted the profile).
    bool OnLive(uint32_t user);
    // Signs profile `index` (into Profiles()) in to `user` the way the sign-in UI does (a profile signed in elsewhere
    // moves; another profile on `user` is replaced; the live client signs it in) and broadcasts
    // XN_SYS_SIGNINCHANGED with the signed-in mask. The Guide raises the toast itself.
    void SignIn(uint32_t user, size_t index);
    // Signs `user` out (and off our Xbox LIVE) and broadcasts XN_SYS_SIGNINCHANGED.
    void SignOut(uint32_t user);
    // "Create Profile" (optional: a runtime that cannot create profiles leaves it empty and the row does nothing): makes
    // a new profile with this gamertag and returns its index into Profiles(); nullopt with the reason the console would
    // show (an invalid gamertag) in `reason`.
    inline std::function<std::optional<size_t>(const std::u16string& gamertag, std::u16string& reason)> createProfile;
    // "Download Profile": adds the profiles the runtime can bring onto the console (the dashboard: the `profiles`
    // folder) to Profiles(); returns their XUIDs.
    std::vector<uint64_t> DownloadProfiles();
    // A profile's gamer picture (64x64 PNG; empty: none).
    std::vector<uint8_t> GamerPicture(uint64_t xuid);
    // A profile's gamercard (zeros / empty texts when it has none).
    Gamercard ReadGamercard(uint64_t xuid);
    // Changes one of the kSetting* values of the profile signed in to `user`, tells the titles
    // (XN_SYS_PROFILESETTINGCHANGED) and sends the profile to our Xbox LIVE when the user is on it (live::UpdateProfile).
    // Returns the server's error text; nullopt when saved (and sent, or not on Xbox LIVE).
    std::optional<std::string> WriteGamercard(uint32_t user, uint32_t settingId, const std::u16string& text);

    // Guide > Settings > Preferences (hud.xex's Options scenes) of the profile signed in to `user`: vibration, the
    // voice settings and the notification switches. The console keeps vibration, voice volume and voice output in the
    // profile (XPROFILE_OPTION_CONTROLLER_VIBRATION 0x10040003, _VOICE_VOLUME 0x1004000E, _VOICE_THRU_SPEAKERS
    // 0x1004000D); the rest are the runtime's, per profile. Volumes are the sliders' 0-10.
    struct Preferences
    {
        bool vibration = true;
        uint32_t voiceVolume = 10, gameVolume = 10;
        uint32_t voiceOutput = 0; // 0 headset, 1 speakers, 2 both
        bool notifyShow = true, notifySound = true, notifyVideos = true;
    };
    Preferences ReadPreferences(uint32_t user);
    void WritePreferences(uint32_t user, const Preferences& preferences);

    // ---- The running title ----

    // The title's name (its SPA's) and `user`'s achievements in it, in the SPA's order (earned ones with their
    // unlock time from the user's profile).
    std::u16string TitleName();
    std::vector<xui::AchievementEntry> TitleAchievements(uint32_t user);
    // The titles `user`'s profile has played, the most recent first (the dashboard GPD's title records): name,
    // achievements earned of all, gamerscore earned, the title's tile (PNG from its GPD, empty: none). And a played
    // title's achievements from its GPD (the Guide's Achievements on the dashboard: the games list first).
    struct PlayedTitle
    {
        uint32_t titleId = 0;
        std::u16string name;
        uint32_t achievements = 0, earned = 0, gamerscore = 0;
        std::vector<uint8_t> tile;
    };
    std::vector<PlayedTitle> PlayedTitles(uint32_t user);
    std::vector<xui::AchievementEntry> PlayedTitleAchievements(uint32_t user, uint32_t titleId);

    // ---- The dashboard, the disc tray and the console ----

    // Whether the running title is the dashboard (FFFE07D1), and its context (XamSetDashContext).
    bool TitleIsDashboard();
    uint32_t DashContext();
    // The dashboard is the title: XamLoaderRebootToDash with this 0x3FC-byte request (+4 the command, +8 the user
    // index, +0xC data), the dashboard goes there; the Guide closes after.
    void RebootToDash(const std::vector<uint8_t>& request);
    // A title ends (the player confirmed leaving it): for the dashboard (`dashRequest`, as for RebootToDash: start
    // the dashboard with it when the runtime can, XBOX360RECOMP_DASHBOARD) or to turn the console off (nullptr).
    [[noreturn]] void EndTitle(const char* why, const std::vector<uint8_t>* dashRequest);
    // The DVD tray (XamLoaderGetDvdTrayState's values: 0 closed, 1 closing, 2 open, 3 opening; 4 or more: no tray
    // the Guide can move) and opening / closing it.
    uint32_t DvdTrayState();
    void OpenCloseDvdTray(bool open);
}
