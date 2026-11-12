#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

// The console's local profiles and which one is signed in to each user index (controller slot 0-3).
//
// The profiles are the gamertags of console_settings.ini (settings.h) plus the profiles copied from a console onto
// the hard drive (content/<XUID>/FFFE07D1/00010000/<XUID>/ with the console's Account file, see
// tools/scripts/import_profile.py). A configured gamertag that names a copied profile is that profile (its XUID,
// GPDs and gamer picture); otherwise it is a local profile with an offline XUID derived from the gamertag. Each has
// its own stored data (GPDs: kernel/profile_store.h, content: content/<XUID>/). All are signed in locally (there is
// no Xbox LIVE connection), with the profile's XUID (offline, E... ). At start-up the first configured profile is
// signed in to user index 0, as a console's auto sign-in profile would be; the others are signed in by the Guide's
// sign-in UI (XamShowSigninUI, kernel/xam_ui.cpp).
namespace kernel::users
{
    constexpr uint32_t kMaxUsers = 4;

    struct Profile
    {
        std::string gamertag; // UTF-8
        uint64_t xuid = 0;
        bool copied = false;      // from a console (has an Account file)
        bool liveEnabled = false; // its Account is an Xbox LIVE account
        uint64_t onlineXuid = 0;  // its Account's online XUID (0 if none)
    };

    // The profiles on this console: the configured ones in settings order, then the other copied profiles.
    const std::vector<Profile>& Profiles();
    // Appends the profiles copied to the hard drive since (kernel/profile_import.h); returns how many.
    size_t AddStoredProfiles();
    // The profile became an Xbox LIVE account (our server's `register`, kernel/xam_live.cpp).
    void SetLiveAccount(uint64_t xuid, uint64_t onlineXuid);
    // Takes a deleted profile (XamProfileDelete) off the list; the others keep their sign-ins (indices after it move
    // down by one).
    void RemoveProfile(uint64_t xuid);
    // Sign-ins survive a title launch on a console (XAM keeps them): the next process of the dashboard takes them over.
    // SaveSignIns writes the signed-in profiles (by offline XUID; " offline" when a LIVE profile is not on our Xbox
    // LIVE) to signed_in.txt in the data folder; RestoreSignIns signs them in again, the offline ones kept off our
    // Xbox LIVE (live::Hold), and deletes the file. Returns how many were restored.
    void SaveSignIns();
    size_t RestoreSignIns();
    // Started by Native Dashboard: NATIVE_DASH_SIGNED_IN ("<user>:<offline XUID>[ offline],...") are the dashboard's
    // sign-ins, kept through the launch as XAM keeps them (Initialize signs them in instead of the first profile;
    // nobody when it is empty), and every change of them is written to NATIVE_DASH_SIGNIN_FILE (SaveSignIns' format)
    // for the dashboard to take back when this title ends.
    bool HostedSignIns();
    // Signs the first profile in to user index 0 (call once, before the title runs; nobody if there are none).
    void Initialize();

    // The profile signed in to `user`, if any (user >= 4: none).
    std::optional<Profile> SignedIn(uint32_t user);
    // The offline XUID of the profile signed in to `user`, 0 if none.
    uint64_t Xuid(uint32_t user);
    // The user index a profile with this XUID is signed in to, if any.
    std::optional<uint32_t> UserOfXuid(uint64_t xuid);
    // Bit n set when a profile is signed in to user index n.
    uint32_t SignedInMask();

    // Signs `profile` (an index into Profiles()) in to `user`, as the sign-in UI does when that user's controller picks
    // it: a profile signed in to another index is signed out there first, and a different profile signed in to
    // `user` is replaced. Does not notify; the caller broadcasts XN_SYS_SIGNINCHANGED once the sign-in is complete.
    void SignIn(uint32_t user, size_t profile);
    // Signs out the profile signed in to `user` (the Guide's Sign Out). Does not notify, like SignIn.
    void SignOut(uint32_t user);

    // The live service (kernel/xam_live.cpp): SignIn / SignOut tell it, so the profile is online on the ports' own
    // Xbox LIVE server while signed in (docs/x360/network_protocol.md).
    void LiveSignIn(uint32_t user, const Profile& profile);
    void LiveSignOut(uint32_t user);

    // The profile's own gamercard and Personal Profile as its dashboard GPD keeps them (kernel/xam_live.cpp; the ids
    // as the user's console profile stores them): what the Profile screens show and our Xbox LIVE gets at sign-in.
    constexpr uint32_t kSettingZone = 0x10040004;     // XPROFILE_GAMERCARD_ZONE (0 none, 1 Recreation ... 4)
    constexpr uint32_t kSettingMotto = 0x402C0011;    // XPROFILE_GAMERCARD_MOTTO, 21 characters
    constexpr uint32_t kSettingName = 0x41040040;     // the Personal Profile's name, 129 characters
    constexpr uint32_t kSettingLocation = 0x40520041; // its location, 40 characters
    constexpr uint32_t kSettingBio = 0x43E80043;      // its bio, 499 characters
    struct Gamercard
    {
        uint32_t gamerscore = 0, zone = 0;
        float reputation = 0; // stars, 0-5 (XPROFILE_GAMERCARD_REP is out of 100)
        std::u16string motto, name, location, bio;
    };
    Gamercard ReadGamercard(uint64_t xuid);
    // Changes one of the texts above (or the zone, `text` = its digit) in the profile signed in to `user`, tells the
    // titles (XN_SYS_PROFILESETTINGCHANGED) and sends the profile to our Xbox LIVE when the user is signed in there.
    // Returns the server's error text, nullopt when saved (and sent, or not on Xbox LIVE).
    std::optional<std::string> WriteGamercard(uint32_t user, uint32_t settingId, const std::u16string& text);
}
