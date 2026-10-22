// The Xbox Guide's screens and behaviour (guide/guide.h), moved here from the dashboard's kernel/xam_ui.cpp so every
// port shows the same Guide. The runtime supplies the system UI slot, profiles, sign-in, the title's achievements and
// the dashboard (guide/host.h); the drawing is the XUI library's (xui/system_ui.h) and Xbox LIVE is our live client
// (live/client.h, live/voice.h).

#include "guide/guide.h"
#include "guide/host.h"
#include "log.h"
#include "xui/system_ui.h"

#include "live/client.h"
#include "live/voice.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <algorithm>
#include <ctime>
#include <format>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <thread>
#include <vector>

namespace
{
    namespace host = guide::host;
    using guide::SystemUi;

    constexpr uint16_t kButtonA = 0x1000, kButtonB = 0x2000, kButtonBack = 0x0020;
    constexpr uint16_t kButtonUp = 0x0001, kButtonDown = 0x0002;
    constexpr uint16_t kButtonLeft = 0x0004, kButtonRight = 0x0008;

    std::string Utf8(const std::u16string& text)
    {
        std::wstring wide(text.begin(), text.end());
        const int size = WideCharToMultiByte(CP_UTF8, 0, wide.data(), int(wide.size()), nullptr, 0, nullptr, nullptr);
        std::string out(size_t(size), '\0');
        WideCharToMultiByte(CP_UTF8, 0, wide.data(), int(wide.size()), out.data(), size, nullptr, nullptr);
        return out;
    }

    // ---- The Xbox Guide (hud.xex 17559's GuideMainScene, opened by the Guide button) ----
    //
    // One system UI from the Guide button until the Guide closes (XN_SYS_UI 1 ... 0), whatever it shows meanwhile:
    // the Guide, a message box over it, or one of its panes (achievements, sign-in), from which B comes back to it.
    // Its controller is the one that opened it.


    struct GuideSession
    {
        enum class Mode { kMain, kBox, kAchievements, kSignIn, kCommunity, kCommunityBox, kKeyboard, kProfile,
                          kPartyOptions, kMessage, kCompose, kGames, kMessageType, kBio, kFriendsOf, kRecipients,
                          kCompareGames, kCompareAchievements, kAchievementDetails, kFeedback, kPreferences,
                          kProfileCreated, kAddXboxLive, kFeedbackText }
            mode = Mode::kMain;
        // The Community screen: its tab (the console's order: party, friends, messages, players), what each row and
        // each button of its message box does, and what the virtual keyboard's Done does.
        using Tab = guide::Tab;
        Tab tab = Tab::kFriends;
        // Party Options is Guide.Friends.xex's PartySettings.xur, else our rows (both: Chat Channel, Party Status,
        // Leave Party).
        bool partyScene = false;
        // The friends list's order (Y Change Sort): by online status, else by gamertag.
        bool sortByName = false;
        // The Achievements games list: the row focused, and whether the achievements came from it (B goes back).
        uint32_t gamesFocus = 0;
        bool achievementsFromGames = false;
        // The message composer (Create New, Reply, Send Message): the recipient and the text.
        // The composer's recipients (xuid, gamertag), in the order chosen.
        std::vector<std::pair<uint64_t, std::string>> composeTo;
        std::u16string composeText;
        std::vector<std::function<void()>> rowActions, boxActions;
        std::vector<std::function<void()>> rowX; // the Community rows' X (accept a request, delete a message)
        std::vector<std::function<void()>> rowY; // the Players tab's Y (Add Friend: the focused player)
        std::function<void(const std::u16string&)> keyboardDone;
        // A player's Profile: whose, its tab (0 Gamer Profile, 1 Personal Profile), and whether B goes back to the
        // Community (opened from its friends list) rather than closing (XamShowGamerCardUIForXUID).
        uint64_t profileXuid = 0;
        uint32_t profileTab = 0;
        // Compare Games: its titles (id, name, the other player's and my gamerscore in it, a tile), the title row
        // A was pressed on, and the achievements of that title shown (with the row focused, for B from details).
        struct CompareTitle
        {
            uint32_t titleId = 0;
            std::u16string name;
            uint32_t theirs = 0, mine = 0;
        };
        std::vector<CompareTitle> compareTitles;
        uint32_t compareFocus = 0, compareAchievementFocus = 0;
        bool compareDirect = false; // opened by XamShowGamesUI: B on its titles closes
        // Settings > Preferences: the scene shown (0 Preferences, 1 Online Status, 2 Notifications, 3 Voice,
        // 4 Vibration) and the online status read when it opened ("online", "away", "busy", "offline").
        uint32_t prefScene = 0;
        std::string onlineStatus = "online";
        bool feedbackDirect = false;
        bool fofDirect = false;
        // Opened after Create Profile (Save Profile): Edit Profile's B comes back to it.
        bool profileCreated = false; // the friends list opened by XamShowFofUI: B closes // opened by XamShowPlayerReviewUI / XamShowComplaintUI: done or B closes
        struct CompareAchievement
        {
            std::u16string name, description, unachieved;
            uint32_t gamerscore = 0;
            bool theirs = false, mine = false, shown = true;
            std::vector<uint8_t> image;
        };
        std::vector<CompareAchievement> compareAchievements;
        bool profileFromCommunity = false;
        // The Community row A (or Y Create New) was pressed on: B from what it opened (a Profile, a message, the
        // composer, the keyboard) comes back to it, as the console's Community scene keeps its list under a drill-down
        // (was the first row).
        uint32_t communityFocus = 0;
        // Which Profile screen kProfile shows: another player's, the user's own (Guide > Settings > Profile,
        // XamShowGamerCardUI) or its Edit Profile; and the row focused when coming back to it.
        enum class ProfileView { kPlayer, kMe, kEdit, kZone, kZoneInfo } profileView = ProfileView::kPlayer;
        uint32_t profileFocus = 0;
        // Where the keyboard's B and a box's answer go (empty: the Community's tab).
        std::function<void()> back;
        // Opened by a title's XamShow*UI on one of the Guide's screens: Back there closes the system UI.
        bool direct = false;
        uint32_t user = 0;
        // The message box over the Guide: its button count, focus, and what its first button (Yes / Turn Off
        // Console) does; any other answer returns to the Guide.
        size_t boxButtons = 0;
        uint32_t boxFocus = 0;
        std::function<void()> onConfirm;
        // Submit Player Review / File Complaint: B from their pages, the page again (at a row: from a text page),
        // whether X shows the Code of Conduct, and the row focused when a text page opened.
        std::function<void()> feedbackBack;
        std::function<void(uint32_t)> feedbackAgain;
        bool feedbackConduct = false;
        uint32_t feedbackFocus = 0;
        // Held while the session changes: by its controller's presses and by the threads that finish a sign-in or
        // Connect to Xbox LIVE (they update the Guide or show their answer when it is back).
        std::recursive_mutex mutex;
        bool connecting = false; // Connect to Xbox LIVE is trying the server
        bool signingIn = false;  // the sign-in pane waits for the profile's Xbox LIVE sign-in
        // The Party tab's members: their list row, XUID, whether it is the user, the voice icon shown; a thread keeps
        // the icons current while the tab shows (GuideVoiceTicker).
        struct PartyVoice
        {
            uint32_t row = 0;
            uint64_t xuid = 0;
            bool self = false;
            std::string icon;
        };
        std::vector<PartyVoice> partyVoice;
        bool voiceTicker = false;
        std::vector<size_t> eligible; // the sign-in pane's profiles (indices into host::Profiles())
        std::shared_ptr<SystemUi> ui;  // the Guide's system UI (closing it ends the Guide)
    };

    // The Guide open now (the Guide button closes it on its own controller).
    std::mutex g_mutex;
    std::weak_ptr<GuideSession> g_current;

    // What the Guide shows for `user` (hud.xex 0x913EA670's user state, the profile's gamertag and picture).
    xui::GuideInfo GuideInfoFor(uint32_t user)
    {
        xui::GuideInfo info;
        info.user = user;
        info.profiles = uint32_t(host::Profiles().size());
        info.trayState = host::DvdTrayState();
        const auto profile = host::SignedIn(user);
        if (!profile)
            return info;
        // Signed in to the ports' own Xbox LIVE (native/x360/system/live) 7; else a LIVE profile is "LIVE-enabled,
        // offline" (8), a local one 9.
        info.state = host::OnLive(user) ? 7 : profile->liveEnabled ? 8 : 9;
        if (info.state == 7)
        {
            for (const live::Friend& f : live::Friends(user))
                info.friendsOnline += f.online && f.request.empty();
            for (const live::Message& m : live::Messages(user))
                info.unreadMessages += !m.read;
            if (const auto party = live::CurrentParty(user))
                info.partyMembers = uint32_t(party->members.size());
        }
        const int size = MultiByteToWideChar(CP_UTF8, 0, profile->gamertag.data(), int(profile->gamertag.size()),
                                             nullptr, 0);
        std::wstring wide(size_t(size), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, profile->gamertag.data(), int(profile->gamertag.size()), wide.data(), size);
        info.gamertag.assign(wide.begin(), wide.end());
        info.picture = host::GamerPicture(profile->xuid);
        // The Achievements button's count: the profile's gamerscore (the dashboard GPD's gamercard credit), else what
        // it earned in this title.
        if (const auto cred = host::ReadGamercard(profile->xuid).gamerscore)
            info.gamerscore = *cred;
        else
            for (const xui::AchievementEntry& a : host::TitleAchievements(user))
                if (a.earned)
                    info.gamerscore += a.gamerscore;
        return info;
    }

    void GuideBack(const std::shared_ptr<GuideSession>& g)
    {
        if (g->direct)
        {
            g->ui->Close("Back", g->user);
            return;
        }
        g->mode = GuideSession::Mode::kMain;
        xui::ShowGuide(GuideInfoFor(g->user));
    }

    void GuideMessageBox(const std::shared_ptr<GuideSession>& g, const std::u16string& title, const std::u16string& text,
                         const std::vector<std::u16string>& buttons, uint32_t focus, xui::MessageIcon icon,
                         std::function<void()> onConfirm)
    {
        Log("System UI: the Guide asks \"{}\" / \"{}\"", Utf8(title), Utf8(text));
        g->mode = GuideSession::Mode::kBox;
        g->boxButtons = buttons.size();
        g->boxFocus = focus;
        g->onConfirm = std::move(onConfirm);
        xui::ShowGuideMessageBox(title, text, buttons, focus, icon);
    }

    // The Guide closes (hud: its vtable +0x2C close), as after a dashboard command.
    void GuideClose(const std::shared_ptr<GuideSession>& g, const char* why)
    {
        g->ui->Close(why, g->user);
    }

    // The dashboard command of a GuideDashCommandNavButton (hud.xex 0x913E63A8 reads it from the button): Xbox Home 0,
    // Music 6, Windows Media Center 8, Account Management 10, Family Settings 0x14, My Games 0x17, Video 0x27,
    // Pictures 0x2C, System Settings 0x2F.
    uint32_t GuideDashCommand(const std::string& id)
    {
        static const std::pair<const char*, uint32_t> kCommands[] = { { "btnDashboard", 0 }, { "btnY", 0 },
            { "btnMusicLibrary", 6 }, { "btnMCX", 8 }, { "btnAccountManagement", 10 }, { "btnFamilySettings", 0x14 },
            { "btnGamesLibrary", 0x17 }, { "btnVideoLibrary", 0x27 }, { "btnPictureLibrary", 0x2C },
            { "btnSystemSettings", 0x2F } };
        for (const auto& [name, command] : kCommands)
            if (id == name)
                return command;
        Fatal("System UI: Guide: no dashboard command for {}", id);
    }

    // A dashboard item (hud.xex 0x913E63A8 -> 0x913E62C8): the request (0x913E9378: 0x3FC bytes, +4 the command, +8
    // the user index; Account Management becomes command 0x3F with "Dash.MP.AccountMgmt.lex" at +0xC) goes to
    // XamLoaderRebootToDash, then the Guide closes. XamLoaderShouldConfirmReboot (XAM 0x8169D250) asks first while a
    // title runs (no executable privilege 0x12) or in dash context 6: title = the item, XamStrings 23 "This will end
    // your current session..." (10 in context 6), Yes / No (focus No), warning icon (0x913E6228 on Yes).
    void GuideDashboardItem(const std::shared_ptr<GuideSession>& g, const std::string& id, const std::u16string& item)
    {
        uint32_t command = GuideDashCommand(id);
        std::vector<uint8_t> request(0x3FC, 0);
        std::string data;
        if (command == 10)
            command = 0x3F, data = "Dash.MP.AccountMgmt.lex";
        auto put32 = [&request](size_t at, uint32_t v) {
            for (int b = 0; b < 4; b++)
                request[at + b] = uint8_t(v >> (24 - 8 * b));
        };
        put32(4, command);
        put32(8, g->user);
        std::copy(data.begin(), data.end(), request.begin() + 0xC);

        const bool dash = host::TitleIsDashboard();
        const bool confirm = !dash || host::DashContext() == 6;
        if (!confirm)
        {
            Log("System UI: Guide: dashboard command {:X} ({})", command, Utf8(item));
            host::RebootToDash(request);
            return GuideClose(g, "a dashboard command");
        }
        const bool context6 = dash;
        GuideMessageBox(g, item, context6 ? xui::CommonString(10, u"This will end your current session.")
                                          : xui::CommonString(23, u"This will end your current session."),
                        { xui::CommonString(298, u"Yes"), xui::CommonString(120, u"No") }, 1, xui::MessageIcon::kWarning,
                        [g, request, dash] {
                            if (!dash)
                                host::EndTitle("Xbox Home", &request);
                            host::RebootToDash(request);
                            GuideClose(g, "a dashboard command");
                        });
    }

    // TrayNavButton (hud.xex 0x913E5898 -> 0x913E5730): with no disc (XamLoaderGetMediaInfo type 0) the tray opens
    // when closed or closing (state below 2) and closes when open or opening (2, 3); the Guide stays open.
    void GuideTray()
    {
        const uint32_t state = host::DvdTrayState();
        if (state >= 4)
            return;
        host::OpenCloseDvdTray(state < 2);
        // The label shows the tray moving, then at rest (hud.xex refreshes it from the tray state).
        xui::SetGuideTrayState(host::DvdTrayState());
        std::thread([] {
            Sleep(1100);
            xui::SetGuideTrayState(host::DvdTrayState());
        }).detach();
    }

    // A Guide feature that needs Xbox LIVE, the dashboard or hardware this PC does not have: XAM's "Not Available"
    // message (XamStrings 30 / 4), then back to the Guide.
    void GuideNotAvailable(const std::shared_ptr<GuideSession>& g, const std::string& id)
    {
        Log("System UI: Guide item {} is not available here (Xbox LIVE / dashboard / hardware)", id);
        GuideMessageBox(g, xui::CommonString(30, u"Not Available"),
                        xui::CommonString(4, u"This feature is temporarily unavailable. Please try again later."),
                        { xui::CommonString(121, u"OK") }, 0, xui::MessageIcon::kNone, {});
    }

    // "Download Profile" (the sign-in pane's last row; the signed-out Guide's button): the console recovers a profile
    // from Xbox LIVE; here the runtime brings its new profiles onto the console (host::DownloadProfiles). Returns the
    // profile IDs added.
    std::vector<uint64_t> DownloadProfiles()
    {
        return host::DownloadProfiles();
    }

    // The sign-in pane's row for a profile ID among `eligible` (profile indices), else 0.
    uint32_t RowOf(const std::vector<size_t>& eligible, const std::vector<uint64_t>& ids)
    {
        const std::vector<guide::Profile> profiles = host::Profiles();
        for (size_t row = 0; row < eligible.size(); row++)
            for (uint64_t id : ids)
                if (eligible[row] < profiles.size() && profiles[eligible[row]].xuid == id)
                    return uint32_t(row);
        return 0;
    }

    void GuideCreateProfile(const std::shared_ptr<GuideSession>& g, const std::u16string& message,
                            const std::u16string& text = u"");

    // Sign In from the Guide (0x913EA870: XamShowSigninUIp, one pane): the profiles not signed in, then Create Profile
    // and Download Profile; A on a profile signs it in to the Guide's controller. `focus` = profile IDs to focus.
    void GuideSignIn(const std::shared_ptr<GuideSession>& g, const std::vector<uint64_t>& focus = {})
    {
        const std::vector<guide::Profile> profiles = host::Profiles();
        std::vector<xui::SignInEntry> entries;
        g->eligible.clear();
        for (size_t i = 0; i < profiles.size(); i++)
        {
            if (host::UserOfXuid(profiles[i].xuid))
                continue;
            g->eligible.push_back(i);
            entries.push_back({ std::u16string(profiles[i].gamertag.begin(), profiles[i].gamertag.end()),
                                profiles[i].liveEnabled ? u"Xbox Live" : u"", u"",
                                host::GamerPicture(profiles[i].xuid) });
        }
        Log("System UI: the Guide's sign-in pane ({} profiles)", entries.size());
        xui::GuideKeepPosition();
        g->mode = GuideSession::Mode::kSignIn;
        xui::ShowSignIn(entries, RowOf(g->eligible, focus));
    }

    void GuideAchievements(const std::shared_ptr<GuideSession>& g, uint32_t focus = 0);
    void GuideCommunity(const std::shared_ptr<GuideSession>& g, GuideSession::Tab tab, uint32_t focus = 0);
    void GuideCreateMessage(const std::shared_ptr<GuideSession>& g);
    void GuidePartyOptions(const std::shared_ptr<GuideSession>& g, uint32_t focus);
    void GuidePartyOption(const std::shared_ptr<GuideSession>& g, uint32_t index);
    void GuideToggleChatChannel(const std::shared_ptr<GuideSession>& g);
    void GuideCompose(const std::shared_ptr<GuideSession>& g, uint32_t focus);
    void GuideRecipients(const std::shared_ptr<GuideSession>& g, uint32_t focus);
    void GuideRecipientKeyboard(const std::shared_ptr<GuideSession>& g);
    xui::GamerCardInfo PlayerCard(uint32_t user, uint64_t xuid, const std::string& fallbackTag);
    void SetTenure(xui::GamerCardInfo& card, int years);
    void GuideProfile(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t tab);
    void GuideCompareGames(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t focus);
    void GuideCompareAchievements(const std::shared_ptr<GuideSession>& g, uint32_t focus);
    void GuideCompareAchievementDetails(const std::shared_ptr<GuideSession>& g);
    void GuidePlayerReview(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t focus);
    void GuideFeedbackText(const std::shared_ptr<GuideSession>& g, const std::u16string& heading,
                           const std::u16string& text);
    // File a Complaint's More Information and X Code of Conduct (the console's 47-page text is not reproduced).
    const char16_t* const kCodeOfConduct = u"File a complaint only if a player has violated the Xbox Live Code of "
                                           u"Conduct. The Code of Conduct and the Terms of Use are at www.xbox.com.";
    void GuideComplaint(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t focus);
    void GuideAvoid(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t focus);
    void GuideComplaintSteps(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t focus);
    void GuideComplaintReasons(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t category, uint32_t focus);

    // Players muted from their Profile (the console's XAM mute list; kept for the run).
    std::set<uint64_t>& MutedPlayers()
    {
        static std::set<uint64_t> muted;
        return muted;
    }
    void GuideOwnProfile(const std::shared_ptr<GuideSession>& g, uint32_t focus = 0);
    void GuidePreferences(const std::shared_ptr<GuideSession>& g, uint32_t scene, uint32_t focus);
    void GuideFriendsOf(const std::shared_ptr<GuideSession>& g, uint64_t xuid, const std::string& tag);
    void GuideProfileCreated(const std::shared_ptr<GuideSession>& g, uint32_t focus);
    void GuideAddXboxLive(const std::shared_ptr<GuideSession>& g);
    void GuidePreferencePress(const std::shared_ptr<GuideSession>& g, uint32_t index, int step);
    void GuideEditProfile(const std::shared_ptr<GuideSession>& g, uint32_t focus);
    void GuideGamerZone(const std::shared_ptr<GuideSession>& g, uint32_t focus);
    void GuideSetGamerZone(const std::shared_ptr<GuideSession>& g, uint32_t zone);
    void GuideConnectToLive(const std::shared_ptr<GuideSession>& g);

    // Back from the keyboard or a box: where the screen that opened it said (`back`), else the Community's tab.
    void GuideReturn(const std::shared_ptr<GuideSession>& g)
    {
        if (g->back)
        {
            auto f = g->back;
            f();
        }
        else
            GuideCommunity(g, g->tab, g->communityFocus);
    }

    void GuideButtonPressed(const std::shared_ptr<GuideSession>& g, const xui::GuideAction& action)
    {
        Log("System UI: Guide: {} (\"{}\")", action.id, Utf8(action.text));
        // The Blades dashboards' Guide (hud.xex 1888 ... 6770: MainMenuSignedIn / Out.xur) names its items so; they do
        // what the later Guide's of the same name do.
        static const std::pair<const char*, const char*> kBlades[] = { { "btnOptions", "btnPersonalSettings" },
            { "btnSignInLive", "btnConnectToLive" }, { "baseMetaPane", "btnProfile" },
            { "btnNewProfile", "btnCreateNewGamertag" }, { "btnRecoverProfile", "btnRecoverGamertag" } };
        std::string alias = action.id;
        for (const auto& [blades, later] : kBlades)
            if (alias == blades)
                alias = later;
        const std::string& id = alias;
        // GuideDashCommandNavButton items (and Xbox Home) go to the dashboard.
        static const char* const kDashboard[] = { "btnDashboard", "btnGamesLibrary", "btnFamilySettings",
            "btnSystemSettings", "btnAccountManagement", "btnVideoLibrary", "btnMusicLibrary", "btnPictureLibrary",
            "btnMCX" };
        for (const char* d : kDashboard)
            if (id == d)
                return GuideDashboardItem(g, id, action.text);
        if (id == "btnAchievements")
            return GuideAchievements(g);
        if (id == "btnFriends" || id == "btnMessages" || id == "btnParty")
        {
            g->communityFocus = 0;
            return GuideCommunity(g, id == "btnFriends" ? GuideSession::Tab::kFriends :
                                     id == "btnMessages" ? GuideSession::Tab::kMessages : GuideSession::Tab::kParty);
        }
        if (id == "btnSignIn")
            return GuideSignIn(g);
        if (id == "btnPersonalSettings") // Settings > Preferences (hud.xex's Options.xur)
            return GuidePreferences(g, 0, 0);
        if (id == "btnProfile") // Settings > Profile (hud 0x913E6690: XamShowGamerCardUI for the Guide's user)
            return GuideOwnProfile(g);
        if (id == "btnRecoverGamertag") // Download Profile
            return GuideSignIn(g, DownloadProfiles());
        if (id == "btnShutdown")
        {
            // ShutdownNavButton (0x913E7280): hud string 52, Turn Off Console / Cancel (focus Cancel), alert icon.
            return GuideMessageBox(g, action.text, xui::HudString(52, u"Turning off the console will also turn off all "
                                                                      u"controllers."),
                                   { xui::CommonString(272, u"Turn Off Console"), xui::CommonString(11, u"Cancel") }, 1,
                                   xui::MessageIcon::kAlert, [] { host::EndTitle("Turn Off Console", nullptr); });
        }
        if (id == "btnDiscInTray")
            return GuideTray();
        if (id == "btnConnectToLive")
            return GuideConnectToLive(g);
        GuideNotAvailable(g, id);
    }

    // The Guide's state changed by something other than its controller (a sign-in or Connect to Xbox LIVE finishing):
    // the Guide shows it again if it is on its blades (the session's mutex held).
    void GuideRefresh(const std::shared_ptr<GuideSession>& g)
    {
        if (g->ui->Closed() || g->mode != GuideSession::Mode::kMain)
            return;
        xui::GuideKeepPosition();
        xui::ShowGuide(GuideInfoFor(g->user));
    }

    // Whether `user` is on our Xbox LIVE once its sign-in has decided: waits (up to 15 s) for the runtime's LIVE
    // sign-in to succeed or to keep the user offline (live::Held; no server).
    bool WaitForLive(uint32_t user)
    {
        for (int i = 0; i < 150; i++)
        {
            if (host::OnLive(user))
                return true;
            if (live::Held(user) || !host::SignedIn(user))
                return false;
            Sleep(100);
        }
        return host::OnLive(user);
    }

    // A profile chosen in the Guide's sign-in pane: signed in, then back to the Guide with the toast once its Xbox
    // LIVE sign-in has its answer (signin.xex's "Signing in" while the logon runs; the pane stays meanwhile): "signed
    // in" on Xbox LIVE or for a local profile, else "signed in offline". The toast used to come before the LIVE
    // sign-in had finished, so a LIVE profile that then signed in showed "offline". On Xbox LIVE it
    // is XAM's notification 8 "signed in to Xbox Live" (the user's capture), not 7.
    void GuideSignInProfile(const std::shared_ptr<GuideSession>& g, size_t index, const guide::Profile& profile)
    {
        const uint32_t user = g->user;
        const std::u16string tag(profile.gamertag.begin(), profile.gamertag.end());
        host::SignIn(user, index);
        if (!profile.liveEnabled)
        {
            xui::QueueNotification(xui::kNotifySignedIn, user, tag);
            return GuideBack(g);
        }
        g->signingIn = true;
        std::thread([g, user, tag] {
            const bool online = WaitForLive(user);
            Log("System UI: Guide: user {} signed in{}", user, online ? " (on Xbox LIVE)" : " offline");
            xui::QueueNotification(online ? xui::kNotifySignedInLive : xui::kNotifySignedInOffline, user, tag);
            std::lock_guard lock(g->mutex);
            g->signingIn = false;
            if (!g->ui->Closed() && g->mode == GuideSession::Mode::kSignIn)
                GuideBack(g);
        }).detach();
    }

    // Connect to Xbox Live (hud 0x913E6AA8; "Join Xbox Live" for a local profile): the user's profile signs in to our
    // Xbox LIVE again (live::Reconnect; nothing does it by itself after a failed sign-in or a lost connection). The
    // Guide stays usable meanwhile; on Xbox LIVE: the "signed in" toast and the Guide's LIVE blades, else signin.xex's
    // string 20 "Can't connect to Xbox Live ..." in a box over the Guide (was XAM's "Not Available").
    void GuideConnectToLive(const std::shared_ptr<GuideSession>& g)
    {
        const auto profile = host::SignedIn(g->user);
        if (!profile)
            return GuideSignIn(g);
        if (host::OnLive(g->user) || g->connecting)
            return;
        g->connecting = true;
        Log("System UI: Guide: Connect to Xbox LIVE for {}", profile->gamertag);
        const uint32_t user = g->user;
        const std::u16string tag(profile->gamertag.begin(), profile->gamertag.end());
        std::thread([g, user, tag] {
            const auto error = live::Reconnect(user);
            const bool online = !error && WaitForLive(user);
            if (!online)
                Log("System UI: Guide: Connect to Xbox LIVE failed: {}", error ? *error : std::string("not signed in"));
            if (online)
                xui::QueueNotification(xui::kNotifySignedInLive, user, tag);
            std::lock_guard lock(g->mutex);
            g->connecting = false;
            if (online)
                return GuideRefresh(g);
            if (!g->ui->Closed() && g->mode == GuideSession::Mode::kMain)
                GuideMessageBox(g, xui::CommonString(0x128, u"Xbox Live"),
                                xui::SigninString(20, u"Can't connect to Xbox Live. Try signing in again or exit the "
                                                      u"game."),
                                { xui::CommonString(121, u"OK") }, 0, xui::MessageIcon::kError, {});
        }).detach();
    }

    // X (0x913E86A0): signed out, the sign-in pane; signed in, "Sign Out" (XamStrings 159) asking XamStrings 158, Yes /
    // No (focus No, warning icon, 0x913E93E0); Yes signs the profile out and closes the Guide.
    void GuideSignInOut(const std::shared_ptr<GuideSession>& g)
    {
        const auto profile = host::SignedIn(g->user);
        if (!profile)
            return GuideSignIn(g);
        const uint32_t user = g->user;
        std::weak_ptr<SystemUi> weak = g->ui;
        GuideMessageBox(g, xui::CommonString(159, u"Sign Out"),
                        xui::CommonString(158, u"Are you sure you want to sign out?"),
                        { xui::CommonString(298, u"Yes"), xui::CommonString(120, u"No") }, 1, xui::MessageIcon::kWarning,
                        [weak, user, gamertag = profile->gamertag] {
                            host::SignOut(user);
                            xui::QueueNotification(xui::kNotifySignedOut, user,
                                                   std::u16string(gamertag.begin(), gamertag.end()));
                            if (auto ui = weak.lock())
                                ui->Close("Sign Out", user);
                        });
    }

    bool GuideInput(SystemUi& ui, const std::shared_ptr<GuideSession>& g, uint32_t user, uint16_t pressed)
    {
        auto done = [&] { return ui.Closed(); };
        if (!xui::PopupShowing())
            return done();
        const std::string source = std::format("the controller of user {}", user);
        using Mode = GuideSession::Mode;
        switch (g->mode)
        {
        case Mode::kMain:
        {
            const xui::GuideAction action = xui::GuidePress(pressed);
            if (action.kind == xui::GuideAction::kClose)
                return ui.Close(source.c_str(), user) || true;
            if (action.kind == xui::GuideAction::kLegendX)
                GuideSignInOut(g);
            else if (action.kind == xui::GuideAction::kLegendY)
                GuideDashboardItem(g, "btnY", action.text); // btnY, a GuideDashCommandNavButton "Xbox Home"
            else if (action.kind == xui::GuideAction::kButton)
                GuideButtonPressed(g, action);
            break;
        }
        case Mode::kBox:
            if (pressed & kButtonA)
            {
                xui::Press(g->boxFocus);
                const bool confirm = g->boxFocus == 0 && g->onConfirm;
                Log("System UI: the Guide's message box answered by {}: button {}", source, g->boxFocus);
                if (confirm)
                {
                    auto f = std::move(g->onConfirm);
                    f();
                    return done();
                }
                g->mode = Mode::kMain;
                xui::GuideMessageBoxClosed();
            }
            else if (pressed & (kButtonB | kButtonBack))
            {
                Log("System UI: the Guide's message box cancelled by {}", source);
                g->mode = Mode::kMain;
                xui::GuideMessageBoxClosed();
            }
            else
            {
                const uint32_t before = g->boxFocus;
                if ((pressed & kButtonUp) && g->boxFocus > 0)
                    g->boxFocus--;
                if ((pressed & kButtonDown) && g->boxFocus + 1 < g->boxButtons)
                    g->boxFocus++;
                if (g->boxFocus != before)
                    xui::SetFocus(g->boxFocus);
            }
            break;
        case Mode::kAchievements:
            if (xui::PanePress(pressed).kind == xui::PaneAction::kCancelled)
            {
                if (g->achievementsFromGames)
                    GuideAchievements(g, g->gamesFocus);
                else
                    GuideBack(g);
            }
            break;
        case Mode::kCompareGames:
        {
            // Compare Games over a player's Profile: A a title's achievements compared, B back to the Profile.
            const xui::PaneAction action = xui::PanePress(pressed);
            if (action.kind == xui::PaneAction::kCancelled)
            {
                if (g->compareDirect)
                    GuideBack(g);
                else
                    GuideProfile(g, g->profileXuid, g->profileTab);
            }
            else if (action.kind == xui::PaneAction::kChosen && action.index < g->compareTitles.size())
            {
                g->compareFocus = action.index;
                GuideCompareAchievements(g, 0);
            }
            break;
        }
        case Mode::kCompareAchievements:
        {
            // A title's achievements compared: A an achievement's details, B back to Compare Games' titles.
            const xui::PaneAction action = xui::PanePress(pressed);
            if (action.kind == xui::PaneAction::kCancelled)
                GuideCompareGames(g, g->profileXuid, g->compareFocus);
            else if (action.kind == xui::PaneAction::kChosen && action.index < g->compareAchievements.size())
            {
                g->compareAchievementFocus = action.index;
                GuideCompareAchievementDetails(g);
            }
            break;
        }
        case Mode::kProfileCreated:
        {
            // Save Profile: Join Xbox Live, Customize Profile (Edit Profile; B there comes back here), Done / B.
            const xui::PaneAction action = xui::PanePress(pressed);
            if (action.kind == xui::PaneAction::kCancelled || (action.kind == xui::PaneAction::kChosen && action.index == 2))
                GuideBack(g);
            else if (action.kind == xui::PaneAction::kChosen && action.index == 0)
                GuideAddXboxLive(g);
            else if (action.kind == xui::PaneAction::kChosen && action.index == 1)
            {
                g->profileCreated = true;
                GuideEditProfile(g, 0);
            }
            break;
        }
        case Mode::kAddXboxLive:
            break; // the connection attempt answers
        case Mode::kPreferences:
        {
            // Up / down move, A presses (opens a scene, toggles a check box, picks a radio button), left / right
            // move a slider; B goes back to Preferences, then to the Guide (closes when a title opened it).
            constexpr uint16_t kLeft = 0x0004, kRight = 0x0008;
            if (pressed & (kLeft | kRight))
            {
                GuidePreferencePress(g, xui::PaneFocus(), (pressed & kLeft) ? -1 : 1);
                break;
            }
            const xui::PaneAction action = xui::PanePress(pressed);
            if (action.kind == xui::PaneAction::kCancelled)
            {
                if (g->prefScene != 0)
                    GuidePreferences(g, 0, g->prefScene - 1);
                else
                    GuideBack(g);
            }
            else if (action.kind == xui::PaneAction::kChosen)
                GuidePreferencePress(g, action.index, 0);
            break;
        }
        case Mode::kFeedback:
        {
            // Submit Player Review / File Complaint's pages: A a row, B a step back, X the Code of Conduct.
            if ((pressed & 0x4000) && g->feedbackConduct)
            {
                GuideFeedbackText(g, u"Code of Conduct", kCodeOfConduct);
                break;
            }
            const xui::PaneAction action = xui::PanePress(pressed);
            if (action.kind == xui::PaneAction::kCancelled && g->feedbackBack)
            {
                auto f = g->feedbackBack;
                f();
            }
            else if (action.kind == xui::PaneAction::kChosen && action.index < g->rowActions.size())
            {
                auto f = g->rowActions[action.index];
                if (f)
                    f();
            }
            break;
        }
        case Mode::kFeedbackText:
        {
            if (xui::PanePress(pressed).kind == xui::PaneAction::kCancelled && g->feedbackAgain)
                g->feedbackAgain(g->feedbackFocus);
            break;
        }
        case Mode::kAchievementDetails:
        {
            if (xui::PanePress(pressed).kind == xui::PaneAction::kCancelled)
                GuideCompareAchievements(g, g->compareAchievementFocus);
            break;
        }
        case Mode::kGames:
        {
            const xui::PaneAction action = xui::PanePress(pressed);
            if (action.kind == xui::PaneAction::kCancelled)
                GuideBack(g);
            else if (action.kind == xui::PaneAction::kChosen && action.index < g->rowActions.size())
            {
                auto f = g->rowActions[action.index];
                f();
            }
            break;
        }
        case Mode::kCommunity:
        {
            g->communityFocus = xui::PaneFocus(); // the row A, X or Y acts on
            const xui::PaneAction action = xui::PanePress(pressed);
            if (action.kind == xui::PaneAction::kCancelled)
                GuideBack(g);
            else if (action.kind == xui::PaneAction::kChosen && action.index < g->rowActions.size())
            {
                g->communityFocus = action.index;
                auto f = g->rowActions[action.index];
                f();
            }
            else if ((pressed & 0x8000) && g->tab == GuideSession::Tab::kMessages && host::OnLive(g->user))
                GuideCreateMessage(g); // Y Create New
            // Friends: Y Change Sort, X Accept Friend Request on a request; Messages: X Delete.
            else if ((pressed & 0x8000) && g->tab == GuideSession::Tab::kFriends && host::OnLive(g->user))
            {
                g->sortByName = !g->sortByName;
                GuideCommunity(g, g->tab, xui::PaneFocus());
            }
            else if ((pressed & 0x4000) && (g->tab == GuideSession::Tab::kFriends ||
                                            g->tab == GuideSession::Tab::kMessages) &&
                     xui::PaneFocus() < g->rowX.size() && g->rowX[xui::PaneFocus()])
            {
                auto f = g->rowX[xui::PaneFocus()];
                f();
            }
            // Players: Y Add Friend on a player who is not a friend yet (X Send Feedback does nothing: no reviews).
            else if ((pressed & 0x8000) && g->tab == GuideSession::Tab::kPlayers && xui::PaneFocus() < g->rowY.size() &&
                     g->rowY[xui::PaneFocus()])
            {
                auto f = g->rowY[xui::PaneFocus()];
                f();
            }
            // In a party ("in a party.png"): X Leave Party, Y Chat Channel.
            else if ((pressed & 0xC000) && g->tab == GuideSession::Tab::kParty && live::CurrentParty(g->user))
            {
                if (pressed & 0x4000)
                {
                    live::LeaveParty(g->user);
                    GuideCommunity(g, GuideSession::Tab::kParty);
                }
                else
                    GuideToggleChatChannel(g);
            }
            // LB / RB and the d-pad or left stick left / right: the tab on the left / right, stopping at Party and
            // Players (Guide.Friends.xex's tab controller FUN_90135648: no wrap). The lists only move up and down.
            else if (pressed & (0x0300 | kButtonLeft | kButtonRight))
            {
                const int next = int(g->tab) + (pressed & (0x0100 | kButtonLeft) ? -1 : 1);
                if (next >= 0 && next < 4)
                {
                    g->communityFocus = 0;
                    GuideCommunity(g, GuideSession::Tab(next));
                }
            }
            break;
        }
        case Mode::kMessage:
        case Mode::kCompose:
        {
            // A message (B: back to Messages) and the composer (B Cancel: back where it was opened).
            const xui::PaneAction action = xui::PanePress(pressed);
            if (action.kind == xui::PaneAction::kCancelled)
            {
                if (g->back)
                {
                    auto back = g->back;
                    back();
                }
                else
                    GuideCommunity(g, g->tab, g->communityFocus);
            }
            else if (action.kind == xui::PaneAction::kChosen && action.index < g->rowActions.size() &&
                     g->rowActions[action.index])
            {
                auto f = g->rowActions[action.index];
                f();
            }
            break;
        }
        case Mode::kPartyOptions:
        {
            // Left / right change the focused setting, as A does; B goes back to the party (its Party Options row).
            if (pressed & (kButtonLeft | kButtonRight))
            {
                if (xui::PaneFocus() < 2) // the Chat Channel / Party Status spinners
                    GuidePartyOption(g, xui::PaneFocus());
                break;
            }
            const xui::PaneAction action = xui::PanePress(pressed);
            if (action.kind == xui::PaneAction::kCancelled)
                GuideCommunity(g, GuideSession::Tab::kParty, 2);
            else if (action.kind == xui::PaneAction::kChosen)
                GuidePartyOption(g, action.index);
            break;
        }
        case Mode::kMessageType:
        {
            // Create New's types (MessageType.xur): A a type, B back to Messages.
            const xui::PaneAction action = xui::PanePress(pressed);
            if (action.kind == xui::PaneAction::kCancelled)
                GuideCommunity(g, GuideSession::Tab::kMessages, g->communityFocus);
            else if (action.kind == xui::PaneAction::kChosen && action.index < g->rowActions.size() &&
                     g->rowActions[action.index])
            {
                auto f = g->rowActions[action.index];
                f();
            }
            break;
        }
        case Mode::kRecipients:
        {
            // Enter Gamertags: A on Enter Gamertag (or Y) types one, A on a friend checks / unchecks them, B back
            // to the composer.
            if (pressed & 0x8000)
            {
                GuideRecipientKeyboard(g);
                break;
            }
            const xui::PaneAction action = xui::PanePress(pressed);
            if (action.kind == xui::PaneAction::kCancelled)
                GuideCompose(g, 0);
            else if (action.kind == xui::PaneAction::kChosen && action.index < g->rowActions.size() &&
                     g->rowActions[action.index])
            {
                auto f = g->rowActions[action.index];
                f();
            }
            break;
        }
        case Mode::kFriendsOf:
        {
            // A friend's friends: A a player's Profile, B back to the friend's Personal Profile (or closed, opened
            // by XamShowFofUI).
            const xui::PaneAction action = xui::PanePress(pressed);
            if (action.kind == xui::PaneAction::kCancelled && g->fofDirect)
                GuideBack(g);
            else if (action.kind == xui::PaneAction::kCancelled)
                GuideProfile(g, g->profileXuid, 1);
            else if (action.kind == xui::PaneAction::kChosen && action.index < g->rowActions.size() &&
                     g->rowActions[action.index])
            {
                auto f = g->rowActions[action.index];
                f();
            }
            break;
        }
        case Mode::kBio:
        {
            // A player's bio: B back to their Personal Profile.
            if (xui::PanePress(pressed).kind == xui::PaneAction::kCancelled)
                GuideProfile(g, g->profileXuid, 1);
            break;
        }
        case Mode::kProfile:
        {
            using View = GuideSession::ProfileView;
            if ((pressed & 0x0300) && g->profileView == View::kPlayer) // LB / RB: the other tab
            {
                GuideProfile(g, g->profileXuid, g->profileTab ^ 1);
                break;
            }
            if (g->profileView == View::kZone && (pressed & 0x8000)) // Y More Info (yButton: GamerZoneInfo.xur)
            {
                g->profileFocus = xui::PaneFocus();
                g->profileView = View::kZoneInfo;
                xui::ShowGamerZoneInfo();
                break;
            }
            const xui::PaneAction action = xui::PanePress(pressed);
            if (g->profileView == View::kZoneInfo)
            {
                if (action.kind == xui::PaneAction::kCancelled)
                    GuideGamerZone(g, g->profileFocus);
                break;
            }
            if (g->profileView == View::kZone)
            {
                if (action.kind == xui::PaneAction::kCancelled)
                    GuideEditProfile(g, 2);
                else if (action.kind == xui::PaneAction::kChosen && action.index < 4)
                    GuideSetGamerZone(g, action.index + 1);
                break;
            }
            if (action.kind == xui::PaneAction::kCancelled)
            {
                if (g->profileView == View::kEdit && g->profileCreated)
                    GuideProfileCreated(g, 1); // Customize Profile from Save Profile
                else if (g->profileView == View::kEdit)
                    GuideOwnProfile(g, 1); // back on Edit Profile
                else if (g->profileView == View::kPlayer && g->profileFromCommunity)
                    GuideCommunity(g, g->tab, g->communityFocus);
                else
                    GuideBack(g);
            }
            else if (action.kind == xui::PaneAction::kChosen && action.index < g->rowActions.size() &&
                     g->rowActions[action.index])
            {
                auto f = g->rowActions[action.index];
                f();
            }
            break;
        }
        case Mode::kCommunityBox:
            if (pressed & kButtonA)
            {
                xui::Press(g->boxFocus);
                Log("System UI: Community: button {}", g->boxFocus);
                if (g->boxFocus < g->boxActions.size() && g->boxActions[g->boxFocus])
                {
                    auto f = g->boxActions[g->boxFocus];
                    f();
                }
                else
                    GuideReturn(g);
            }
            else if (pressed & (kButtonB | kButtonBack))
                GuideReturn(g);
            else
            {
                const uint32_t before = g->boxFocus;
                if ((pressed & kButtonUp) && g->boxFocus > 0)
                    g->boxFocus--;
                if ((pressed & kButtonDown) && g->boxFocus + 1 < g->boxButtons)
                    g->boxFocus++;
                if (g->boxFocus != before)
                    xui::SetFocus(g->boxFocus);
            }
            break;
        case Mode::kKeyboard:
        {
            const xui::KeyboardAction action = xui::KeyboardPress(pressed);
            if (action == xui::KeyboardAction::kDone && g->keyboardDone)
            {
                auto f = std::move(g->keyboardDone);
                f(xui::KeyboardText());
            }
            else if (action == xui::KeyboardAction::kDone || action == xui::KeyboardAction::kCancel)
                GuideReturn(g);
            break;
        }
        case Mode::kSignIn:
        {
            if (g->signingIn)
                break; // a profile is signing in to Xbox LIVE
            const xui::PaneAction action = xui::PanePress(pressed);
            if (action.kind == xui::PaneAction::kCancelled)
                GuideBack(g);
            else if (action.kind == xui::PaneAction::kChosen && action.index < g->eligible.size())
            {
                const size_t index = g->eligible[action.index];
                const std::vector<guide::Profile> profiles = host::Profiles();
                if (index >= profiles.size())
                    break;
                GuideSignInProfile(g, index, profiles[index]);
            }
            else if (action.kind == xui::PaneAction::kChosen && action.index == g->eligible.size() + 1)
                GuideSignIn(g, DownloadProfiles());
            else if (action.kind == xui::PaneAction::kChosen && host::createProfile)
                GuideCreateProfile(g, u"Enter a gamertag for the new profile.");
            else if (action.kind == xui::PaneAction::kChosen)
                Log("System UI: Guide sign-in: Create Profile is not available here");
            break;
        }
        }
        return done();
    }


    // ---- The Guide's Community screen (owner's captures 7_47_01 ... 7_49_59) ----
    // The console's is a LIVE-delivered module (Guide.Friends.xex, not in the system update): drawn here in
    // gamerprofile.xex's full list pane (xui::ShowCommunity) from the live server's state (live/client.h).
    // Friends: "Add Friend", then friend requests, online friends, offline ones; Messages: newest first; Party: the
    // members or "Start a Party". LB / RB switch tabs. A row opens a message box of actions (Send Message, Invite to
    // Party, Accept / Decline, Reply, Delete, Join Party, Leave Party), text goes through the virtual keyboard.

    std::u16string Wide(const std::string& utf8)
    {
        const int n = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), int(utf8.size()), nullptr, 0);
        std::wstring w(size_t(n), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8.data(), int(utf8.size()), w.data(), n);
        return std::u16string(w.begin(), w.end());
    }

    // "9 hours ago" (the console's Last online / message times).
    std::u16string Ago(int64_t unix)
    {
        const int64_t seconds = std::max<int64_t>(0, int64_t(std::time(nullptr)) - unix);
        if (seconds < 60)
            return u"just now";
        if (seconds < 3600)
            return Wide(std::format("{} minute{} ago", seconds / 60, seconds / 60 == 1 ? "" : "s"));
        if (seconds < 86400)
            return Wide(std::format("{} hour{} ago", seconds / 3600, seconds / 3600 == 1 ? "" : "s"));
        if (seconds < 2 * 86400)
            return u"yesterday";
        return Wide(std::format("{} days ago", seconds / 86400));
    }

    std::u16string Presence(const live::Friend& f)
    {
        // "guide friends list.png": a request someone sent, and one sent to someone.
        if (f.request == "incoming")
            return u"Wants to be your friend";
        if (f.request == "outgoing")
            return u"Friend request pending";
        if (f.online && f.beacon)
        {
            // A beacon ("players wanted"): the game they want company in, then their note (or "Wants to play with
            // you", Friends strings #86).
            const std::u16string wants = xui::FriendsString(86, u"Wants to play with you");
            return f.beacon->title.empty() ? wants
                   : Wide(f.beacon->title) + u"\n" + (f.beacon->text.empty() ? wants : Wide(f.beacon->text));
        }
        if (f.online)
        {
            // The title, then its rich presence under it (owner's 7_49_36: "COD: Black Ops II" / "At the main
            // menu"); a presence string's own lines are joined.
            std::string rich = f.rich;
            for (size_t at; (at = rich.find('\n')) != std::string::npos;)
                rich.replace(at, 1, " - ");
            return rich.empty() ? Wide(f.title) : Wide(f.title) + u"\n" + Wide(rich);
        }
        return f.lastSeen ? u"Last online " + Ago(f.lastSeen) : u"Offline";
    }

    std::u16string Gamertag(const std::string& tag)
    {
        return Wide(tag);
    }

    // ---- The real Community scene's rows (Guide.Friends.xex 17559's list classes) ----

    // "%d" in one of the module's strings.
    std::u16string FormatCount(std::u16string format, size_t n)
    {
        if (const size_t at = format.find(u"%d"); at != std::u16string::npos)
            format.replace(at, 2, Wide(std::to_string(n)));
        return format;
    }

    // FriendsList (FUN_9011A3B8 text, FUN_9011AE10 images): 0 the gamertag, 6 the gamer picture, 3 the status icon,
    // 1 the right column (string 79 a request received, 229 one sent, "Last online ..." / 169 Offline, the title
    // or 170 Online), 8 the rich presence under the title, 9 under the gamertag the name and motto ("%s - %s",
    // owner's captures 7_47_01 / 7_49_36: "Hashim - artmaxgamer_89", ">:3").
    void FriendSlots(const live::Friend& f, const live::PublicProfile* profile, xui::CommunityRow& row)
    {
        row.slotText[0] = Gamertag(f.gamertag);
        if (profile && f.request.empty())
        {
            if (!profile->name.empty() && !profile->motto.empty())
                row.slotText[9] = Wide(profile->name + " - " + profile->motto);
            else
                row.slotText[9] = Wide(profile->name.empty() ? profile->motto : profile->name);
        }
        row.pictureSlot = 6;
        if (!f.request.empty())
            row.slotImage[3] = "friendlist_friendinvite.png";
        else if (!f.online)
            row.slotImage[3] = "friend_offline.png";
        else
            row.slotImage[3] = f.party ? "party_joinable.png" : "friend_online.png";
        if (f.request == "incoming")
            row.slotText[1] = xui::FriendsString(79, u"Wants to be your friend");
        else if (f.request == "outgoing")
            row.slotText[1] = xui::FriendsString(229, u"Friend request pending");
        else if (!f.online)
            row.slotText[1] = f.lastSeen ? u"Last online " + Ago(f.lastSeen) : xui::FriendsString(169, u"Offline");
        else if (f.beacon)
        {
            // A beacon ("players wanted", our LIVE's beacons): the game they want company in, then their note (or the
            // module's "Wants to play with you", strings.xus #86) under it.
            row.slotText[1] = f.beacon->title.empty() ? xui::FriendsString(86, u"Wants to play with you")
                                                      : Wide(f.beacon->title);
            row.slotText[8] = f.beacon->text.empty() ? xui::FriendsString(86, u"Wants to play with you")
                                                     : Wide(f.beacon->text);
        }
        else
        {
            row.slotText[1] = f.title.empty() ? xui::FriendsString(170, u"Online") : Wide(f.title);
            std::string rich = f.rich;
            for (size_t at; (at = rich.find('\n')) != std::string::npos;)
                rich.replace(at, 1, " - ");
            if (!rich.empty())
                row.slotText[8] = Wide(rich);
        }
    }

    // MessageList (FUN_9011E030 text, FUN_9011E338 images): 0 the sender, 1 the text (or the type's default:
    // 79 a friend request, 86 a game invite), 3 the type's icon; a read message is a checked item (XM_GET_ITEMCHECK,
    // flag 0x10: MessageListItem's NormalCheck / FocusCheck draw its icons at half opacity).
    void MessageSlots(const live::Message& m, xui::CommunityRow& row)
    {
        row.checked = m.read;
        row.slotText[0] = Gamertag(m.fromGamertag);
        if (m.kind == "friend_request")
        {
            row.slotText[1] = m.text.empty() ? xui::FriendsString(79, u"Wants to be your friend") : Wide(m.text);
            row.slotImage[3] = "friendinvite.png";
        }
        else if (m.kind == "game_invite")
        {
            row.slotText[1] = m.text.empty() ? xui::FriendsString(86, u"Wants to play with you") : Wide(m.text);
            row.slotImage[3] = "gameinvite.png";
        }
        else if (m.kind == "party_invite")
        {
            row.slotText[1] = row.line2;
            row.slotImage[3] = "ico_32x_PartyInvite.png";
        }
        else
        {
            row.slotText[1] = Wide(m.text.substr(0, 60));
            row.slotImage[3] = "sharedres://ico_32x_Mail.png";
        }
    }

    // PartyList (FUN_90127D48 text, FUN_90127F70 images): 11 the gamertag, 1 the title (170 Online without one),
    // 10 the gamer picture, 3 the presence icon (FUN_901156E0: player_online.png, the green controller, for a
    // member in a game, owner's "in a party.png"; the person on the dashboard), 2 the voice icon (FUN_90127F70 with
    // FUN_90137428: 0 PartyMute.png muted, 1 Voice.png talking, 3 Headset.png a headset, nothing without one as the
    // capture without a headset shows; 2 VoiceDisable.png, voice not allowed, does not happen on our LIVE), 8 / 12
    // the leader's Leader.png and Gold.png. Was empty for a member with a microphone who was not talking: our own row never showed one.
    // A party member's voice icon (slot 2) now: this PC's from its default recording device and its sound level,
    // another member's from the voice frames heard from them (their microphone is open, and how loud).
    std::string VoiceIcon(uint64_t xuid, bool self)
    {
        const auto has = [xuid](const std::vector<uint64_t>& xuids) {
            return std::find(xuids.begin(), xuids.end(), xuid) != xuids.end(); };
        if (self)
            return !live::voice::HasMicrophone() ? ""
                   : live::voice::Muted()       ? "PartyMute.png"
                   : live::voice::LocalTalking() ? "Voice.png"
                                                 : "Headset.png";
        return has(live::voice::Talking()) ? "Voice.png" : has(live::voice::Headsets()) ? "Headset.png" : "";
    }

    // The title in a party row (FUN_90127D48 slot 1): XamGetCachedTitleName of the member's title (this console's
    // running title for the user's own row), the name the console keeps from the title's SPA, e.g. "Minecraft" (the
    // owner's real capture; TU46's SPA string 0x8000), not the client name a port signs in with ("Minecraft: Xbox 360
    // Edition"). Ours: the running title's SPA name, else a title the user's profile has played, else the server's.
    std::u16string CachedTitleName(const live::PartyMember& m, bool self, const std::vector<host::PlayedTitle>& played)
    {
        if (self)
            if (std::u16string name = host::TitleName(); !name.empty())
                return name;
        for (const host::PlayedTitle& t : played)
            if (t.titleId == m.titleId && !t.name.empty())
                return t.name;
        return Wide(m.title);
    }

    void PartyMemberSlots(const live::PartyMember& m, bool leader, bool self, const std::u16string& title,
                          xui::CommunityRow& row)
    {
        row.slotText[11] = Gamertag(m.gamertag);
        row.slotText[1] = title.empty() ? xui::FriendsString(170, u"Online") : title;
        row.pictureSlot = 10;
        row.slotImage[3] = m.titleId != 0 && m.titleId != 0xFFFE07D1 ? "player_online.png" : "friend_online.png";
        if (const std::string voice = VoiceIcon(m.xuid, self); !voice.empty())
            row.slotImage[2] = voice;
        if (leader)
        {
            row.slotImage[8] = "Leader.png";
            row.slotImage[12] = "Gold.png";
        }
    }

    // "Last met 42 days ago" (FUN_901250A8: minutes, then hours from 60 minutes, days from 24 hours; strings
    // 131 / 132, 129 / 130, 127 "Last met yesterday" / 128; 169 Offline for a time it cannot place).
    std::u16string LastMet(int64_t met)
    {
        const int64_t seconds = int64_t(std::time(nullptr)) - met;
        if (met <= 0 || seconds < 0)
            return xui::FriendsString(169, u"Offline");
        int64_t n = seconds / 60;
        size_t index;
        const char16_t* fallback;
        if (n < 60)
        {
            n = std::max<int64_t>(n, 1);
            index = n == 1 ? 131 : 132;
            fallback = n == 1 ? u"Last met %d minute ago" : u"Last met %d minutes ago";
        }
        else if (n < 1440)
        {
            n /= 60;
            index = n == 1 ? 129 : 130;
            fallback = n == 1 ? u"Last met %d hour ago" : u"Last met %d hours ago";
        }
        else
        {
            n /= 1440;
            index = n == 1 ? 127 : 128;
            fallback = n == 1 ? u"Last met yesterday" : u"Last met %d days ago";
        }
        return FormatCount(xui::FriendsString(index, fallback), size_t(n));
    }

    // PlayersList (FUN_90125BE8 text, FUN_90124DF8 images; btn_RecentPlayers): text 0 the gamertag, 1 the title they
    // were met in, 2 "Last met ..."; images 0 the gamer picture, 1 the skin's "Reputation" visual at frame
    // (100 / 20 + rep - 1) * 20 / 100 of its 20 (XPROFILE_GAMERCARD_REP 0-100: 0 none ... 20 five stars), 2 the
    // status (player_online.png, player_offline.png; our LIVE has no away / busy / joinable sessions), 3 a review's
    // PreferredPlayer / NegativeFeedback (none: no player reviews on our LIVE).
    void PlayerSlots(const live::RecentPlayer& p, const live::PublicProfile* profile, xui::CommunityRow& row)
    {
        row.slotText[0] = Gamertag(p.gamertag);
        row.slotText[1] = Wide(p.title);
        row.slotText[2] = LastMet(p.met);
        row.pictureSlot = 0;
        const int rep = profile ? int(std::clamp(profile->reputation, 0.0f, 100.0f)) : 0;
        row.slotVisual[1] = { "Reputation", (100 / 20 + rep - 1) * 20 / 100 };
        row.slotImage[2] = p.online ? "player_online.png" : "player_offline.png";
    }

    void GuideKeyboard(const std::shared_ptr<GuideSession>& g, const std::u16string& title,
                       const std::u16string& description, size_t maxLength,
                       std::function<void(const std::u16string&)> done, const std::u16string& text = u"")
    {
        g->mode = GuideSession::Mode::kKeyboard;
        g->keyboardDone = std::move(done);
        xui::ShowKeyboard(title, description, text, maxLength);
    }

    // Create Profile from the Guide's sign-in pane: the gamertag on the keyboard, then the new profile signs in to the
    // Guide's controller (as choosing it in the pane does); a gamertag the runtime refuses asks again with the reason.
    void GuideCreateProfile(const std::shared_ptr<GuideSession>& g, const std::u16string& message,
                            const std::u16string& text)
    {
        GuideKeyboard(g, u"Create Profile", message, 15, [g](const std::u16string& gamertag) {
            std::u16string reason;
            const std::optional<size_t> index = host::createProfile(gamertag, reason);
            if (!index)
                return GuideCreateProfile(g, reason, gamertag);
            const std::vector<guide::Profile> profiles = host::Profiles();
            if (*index < profiles.size())
                GuideSignInProfile(g, *index, profiles[*index]);
        }, text);
    }

    // A message box of actions over the Community screen; Back (or B) returns to it.
    void GuideCommunityBox(const std::shared_ptr<GuideSession>& g, const std::u16string& title,
                           const std::u16string& text, std::vector<std::pair<std::u16string, std::function<void()>>> buttons)
    {
        g->mode = GuideSession::Mode::kCommunityBox;
        g->boxFocus = 0;
        g->boxActions.clear();
        std::vector<std::u16string> labels;
        for (auto& [label, action] : buttons)
        {
            labels.push_back(label);
            g->boxActions.push_back(std::move(action));
        }
        g->boxButtons = labels.size();
        Log("System UI: Community: \"{}\" / \"{}\"", Utf8(title), Utf8(text));
        xui::ShowMessageBox(title, text, labels, 0, xui::MessageIcon::kNone);
    }

    // The outcome of a request: a box with OK, then back to the tab.
    void GuideCommunityResult(const std::shared_ptr<GuideSession>& g, const std::u16string& title,
                              const std::optional<std::string>& error, const std::u16string& success)
    {
        GuideCommunityBox(g, title, error ? Wide("Sorry, that didn't work: " + *error + ".") : success,
                          { { u"OK", nullptr } });
    }

    void GuideCompose(const std::shared_ptr<GuideSession>& g, uint32_t focus);

    // Send Message / Reply: the composer to `to`, on Edit Text.
    void GuideSendMessage(const std::shared_ptr<GuideSession>& g, uint64_t to, const std::string& gamertag)
    {
        g->composeTo = { { to, gamertag } };
        g->composeText.clear();
        GuideCompose(g, 2);
    }

    // A player's Profile (owner's captures 7_47_09: a friend, 7_47_50: another player; xui::ShowGamerProfile): the
    // gamercard from our Xbox LIVE (their profile and, for a friend, presence) and the console's menu (rows per
    // FriendOptionsScene's modes, below). The Personal Profile's View Friends needs a friend (our LIVE's friends_of).
    void GuideProfile(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t tab)
    {
        const uint32_t user = g->user;
        g->mode = GuideSession::Mode::kProfile;
        g->profileView = GuideSession::ProfileView::kPlayer;
        g->back = nullptr;
        g->profileXuid = xuid;
        g->profileTab = tab;
        const std::vector<live::Friend> friends = live::Friends(user);
        const auto f = std::find_if(friends.begin(), friends.end(), [&](const live::Friend& x) { return x.xuid == xuid; });
        const bool isFriend = f != friends.end() && f->request.empty();
        const auto profile = live::GetProfile(user, xuid);
        const std::string tag = profile ? profile->gamertag : f != friends.end() ? f->gamertag : std::string();

        xui::GamerCardInfo card;
        card.gamertag = Gamertag(tag);
        if (profile)
        {
            card.gamerscore = profile->gamerscore;
            card.zone = profile->zone;
            card.reputation = profile->reputation / 20;
            card.motto = Wide(profile->motto);
            card.picture = profile->picture;
            // The Personal Profile tab (7_47_15: name, location, bio under the motto; blank when not set, 7_47_56).
            card.name = Wide(profile->name);
            card.location = Wide(profile->location);
            card.bio = Wide(profile->bio);
            SetTenure(card, profile->tenure);
        }
        if (isFriend)
        {
            card.online = f->online;
            if (f->online && f->beacon)
            {
                card.title = Wide(f->beacon->title);
                card.presence = f->beacon->text.empty() ? xui::FriendsString(86, u"Wants to play with you")
                                                        : Wide(f->beacon->text);
            }
            else
            {
                card.title = Wide(f->title);
                std::string rich = f->rich;
                for (size_t at; (at = rich.find('\n')) != std::string::npos;)
                    rich.replace(at, 1, " - ");
                card.presence = Wide(rich);
            }
            if (!f->online && f->lastSeen)
                card.offlineSeconds = std::max<int64_t>(0, int64_t(std::time(nullptr)) - f->lastSeen);
        }

        std::vector<xui::ProfileRow> rows;
        g->rowActions.clear();
        auto add = [&](const char16_t* text, std::function<void()> action) {
            rows.push_back({ text, bool(action) });
            g->rowActions.push_back(std::move(action));
        };
        if (tab == 0)
        {
            // Guide.Friends' FriendOptionsScene rows (table 0x90100E48, A-press FUN_901140C8): mode 0 a received request,
            // 1 a sent request, 2 a friend, 3 anyone else; FUN_90113218 / FUN_90113298 pick it. Rows our LIVE cannot
            // offer are left out as the console leaves them out (Send Gift without gifting, View Activity Feed without
            // its service URL, the title's custom actions) or greyed (Invite to Game outside a session, Private
            // Chat).
            const bool received = f != friends.end() && f->request == "incoming";
            const bool sent = f != friends.end() && f->request == "outgoing";
            const std::optional<live::Party> party = live::CurrentParty(user);
            const bool inMyParty = party && std::any_of(party->members.begin(), party->members.end(),
                                                        [&](const live::PartyMember& m) { return m.xuid == xuid; });
            const auto me = host::SignedIn(user);
            const bool leader = party && me && party->leader == me->onlineXuid;
            bool local = false;
            for (uint32_t u = 0; u < 4; u++)
                if (const auto other = host::SignedIn(u); other && other->onlineXuid == xuid)
                    local = true;
            if (f == friends.end())
                add(u"Send Friend Request", [g, user, tag] {
                    GuideCommunityResult(g, u"Send Friend Request", live::AddFriend(user, tag),
                                         u"A friend request was sent to " + Gamertag(tag) + u".");
                });
            if (received)
            {
                add(u"Accept Friend Request", [g, user, xuid, tag] {
                    GuideCommunityResult(g, u"Friend Request", live::AcceptFriend(user, xuid),
                                         Gamertag(tag) + u" is now your friend.");
                });
                add(u"Decline Friend Request", [g, user, xuid] {
                    live::DeclineFriend(user, xuid);
                    GuideCommunity(g, g->tab, g->communityFocus);
                });
            }
            add(u"Invite to Game", nullptr);
            // Invite to Party: not for a member of my party, a full party (8) or a profile signed in here.
            if (!inMyParty && !local && !(party && party->members.size() >= 8))
                add(u"Invite to Party", [g, user, xuid] {
                    if (!live::CurrentParty(user))
                        live::CreateParty(user);
                    if (const auto error = live::InviteToParty(user, xuid))
                        return GuideCommunityResult(g, u"Invite to Party", error, u"");
                    xui::QueueNotification(xui::kNotifyPartyInviteSent, user, u"");
                    GuideCommunity(g, GuideSession::Tab::kParty);
                });
            // Kick from Party: the leader, for another member of the party.
            if (inMyParty && leader && !local)
                add(u"Kick from Party", [g, user, xuid] {
                    if (const auto error = live::KickFromParty(user, xuid))
                        return GuideCommunityResult(g, u"Kick from Party", error, u"");
                    GuideCommunity(g, GuideSession::Tab::kParty);
                });
            // Join Party: a friend who is in a party I am not in.
            if (isFriend && f->party && !inMyParty)
                add(u"Join Party", [g, user, xuid] {
                    if (const auto error = live::JoinFriendsParty(user, xuid))
                        return GuideCommunityResult(g, u"Join Party", error, u"");
                    GuideCommunity(g, GuideSession::Tab::kParty);
                });
            add(u"Invite to Private Chat", nullptr);
            add(u"Send Message", [g, xuid, tag] { GuideSendMessage(g, xuid, tag); });
            add(u"Compare Games", [g, xuid] { GuideCompareGames(g, xuid, 0); });
            add(u"Submit Player Review", [g, xuid] { GuidePlayerReview(g, xuid, 0); });
            add(u"File Complaint", [g, xuid] { GuideComplaint(g, xuid, 0); });
            // Mute / Unmute (strings 163 / 247): always available; the console keeps the list in XAM (XMsg 0x5800C /
            // 0x5800D), here for the run.
            const bool muted = MutedPlayers().count(xuid) != 0;
            add(muted ? u"Unmute" : u"Mute", [g, xuid, tab, muted] {
                if (muted)
                    MutedPlayers().erase(xuid);
                else
                    MutedPlayers().insert(xuid);
                Log("System UI: Guide: player {:016X} {}", xuid, muted ? "unmuted" : "muted");
                // XAM's toast (owner's captures 3_25_29 / 3_25_36: "Hijack Assassin muted" / "unmuted").
                if (const auto profile = live::GetProfile(g->user, xuid))
                    xui::QueueNotification(muted ? xui::kNotifyUnmuted : xui::kNotifyMuted, g->user,
                                           Wide(profile->gamertag));
                GuideProfile(g, xuid, tab);
            });
            if (sent)
                add(u"Cancel Friend Request", [g, user, xuid] {
                    live::RemoveFriend(user, xuid);
                    GuideCommunity(g, g->tab, g->communityFocus);
                });
            if (isFriend)
                add(u"Remove Friend", [g, user, xuid, tag] {
                    GuideCommunityBox(g, u"Remove Friend", u"Remove " + Gamertag(tag) + u" from your friends list?",
                                      { { u"Remove", [g, user, xuid] {
                                            live::RemoveFriend(user, xuid);
                                            GuideCommunity(g, g->tab, g->communityFocus);
                                        } },
                                        { u"Cancel", nullptr } });
                });
        }
        else
        {
            // View Friends: Guide.Friends.xex's FofScene.xur ("%s's Friends", string 76; owner's 7_47_34) with the
            // friend's friends from our LIVE (friends_of), for a friend only.
            if (xui::CommunitySceneAvailable() && isFriend)
                add(u"View Friends", [g, xuid, tag] { GuideFriendsOf(g, xuid, tag); });
            else
                add(u"View Friends", nullptr);
            // View Bio: Guide.Friends.xex's Bio.xur ("%s's bio", string 21; owner's 7_47_21) when it is there.
            if (xui::CommunitySceneAvailable())
                add(u"View Bio", [g, tag, bio = profile ? profile->bio : std::string()] {
                    std::u16string heading = xui::FriendsString(21, u"%s's bio");
                    if (const size_t at = heading.find(u"%s"); at != std::u16string::npos)
                        heading.replace(at, 2, Gamertag(tag));
                    g->mode = GuideSession::Mode::kBio;
                    xui::GuideKeepPosition();
                    xui::ShowBio(heading, Wide(bio));
                });
            else
                add(u"View Bio", nullptr);
        }
        Log("System UI: Profile of {} ({:016X}{}), tab {}", tag, xuid, isFriend ? ", friend" : "", tab);
        xui::ShowGamerProfile({ u"Gamer Profile", u"Personal Profile" }, tab, rows, 0, card,
                              tab == 0 ? xui::ProfilePanel::kExtended : xui::ProfilePanel::kPersonal);
    }

    // The tenure number (years on Xbox LIVE) of a card: the console's own crown<N>.png from our server's tenure
    // content (imported from the system update's FFFE07DF00000001), N clamped to 19 as XAM does (0x81729310).
    void SetTenure(xui::GamerCardInfo& card, int years)
    {
        card.tenure = years;
        if (years > 0)
            card.tenureBadge = live::GetContent("tenure", "crown" + std::to_string(std::min(years, 19)) + ".png");
    }

    // The signed-in player's own gamercard: the profile's dashboard GPD (gamerscore, zone, rep, motto, Personal
    // Profile) and gamer picture.
    xui::GamerCardInfo OwnGamerCard(uint32_t user)
    {
        xui::GamerCardInfo card;
        const auto profile = host::SignedIn(user);
        if (!profile)
            return card;
        const guide::Gamercard g = host::ReadGamercard(profile->xuid);
        card.gamertag = Gamertag(profile->gamertag);
        card.picture = host::GamerPicture(profile->xuid);
        card.gamerscore = g.gamerscore.value_or(0);
        card.zone = g.zone;
        card.reputation = g.reputation;
        card.online = host::OnLive(user);
        card.motto = g.motto;
        card.name = g.name;
        card.location = g.location;
        card.bio = g.bio;
        if (card.online)
            if (const auto own = live::GetProfile(user, profile->onlineXuid)) // the server knows the online XUID
                SetTenure(card, own->tenure);
        return card;
    }

    // Empty texts of the own card read as gamerprofile.xex's "(No motto)" ... (GamerProfile_Custom.xus #82-85).
    xui::GamerCardInfo WithPlaceholders(xui::GamerCardInfo card)
    {
        if (card.motto.empty())
            card.motto = xui::GamerProfileString(84, u"(No motto)");
        if (card.name.empty())
            card.name = xui::GamerProfileString(85, u"(No name)");
        if (card.location.empty())
            card.location = xui::GamerProfileString(83, u"(No location)");
        if (card.bio.empty())
            card.bio = xui::GamerProfileString(82, u"(No bio)");
        return card;
    }

    // The player's own Profile (owner's capture 7_42_05; gamerprofile.xex's GamerCardScene with its own menus): the
    // card with the Personal Profile under it, and meOnlineButtonGroup on our Xbox LIVE, else meOfflineButtonGroup.
    // Edit Profile works; the others (games played, reputation, game defaults, sign-in preferences, account
    // security, exercise info, joining Xbox LIVE) are not here yet and greyed.
    void GuideOwnProfile(const std::shared_ptr<GuideSession>& g, uint32_t focus)
    {
        const uint32_t user = g->user;
        if (!host::SignedIn(user))
            return GuideBack(g);
        g->mode = GuideSession::Mode::kProfile;
        g->profileView = GuideSession::ProfileView::kMe;
        g->back = nullptr;
        g->rowActions.clear();
        std::vector<xui::ProfileRow> rows;
        auto add = [&](const char16_t* text, std::function<void()> action) {
            rows.push_back({ text, bool(action) });
            g->rowActions.push_back(std::move(action));
        };
        const bool online = host::OnLive(user);
        add(u"View Games", nullptr);
        add(u"Edit Profile", [g] { GuideEditProfile(g, 0); });
        if (online)
            add(u"View Rep", nullptr);
        add(u"Game Defaults", nullptr);
        add(u"Sign-in Preferences", nullptr);
        add(online ? u"Account Security" : u"Join Xbox Live", nullptr);
        add(u"Manage Exercise Info", nullptr);
        Log("System UI: own Profile of user {} ({})", user, online ? "on Xbox LIVE" : "offline");
        xui::ShowGamerProfile({}, 0, rows, std::min<uint32_t>(focus, uint32_t(rows.size()) - 1),
                              WithPlaceholders(OwnGamerCard(user)), xui::ProfilePanel::kPersonal);
    }

    // Edit Profile (7_42_23; 808_editProfile.xur): on our Xbox LIVE its onlineButtonGroup (Gamertag, Gamer Picture,
    // Gamer Zone, Motto, Avatar, Name, Location, Bio), else offlineButtonGroup (Gamer Picture, Profile Name, Avatar).
    // Motto, Name, Location and Bio open the keyboard on the current text (7_42_57) and are saved to the profile and
    // sent to our Xbox LIVE; changing the gamertag (a paid service), pictures, zones and avatars is not here yet.
    void GuideEditProfile(const std::shared_ptr<GuideSession>& g, uint32_t focus)
    {
        const uint32_t user = g->user;
        if (!host::SignedIn(user))
            return GuideBack(g);
        g->mode = GuideSession::Mode::kProfile;
        g->profileView = GuideSession::ProfileView::kEdit;
        g->back = nullptr;
        g->rowActions.clear();
        std::vector<xui::ProfileRow> rows;
        auto add = [&](const char16_t* text, std::function<void()> action) {
            rows.push_back({ text, bool(action) });
            g->rowActions.push_back(std::move(action));
        };
        const xui::GamerCardInfo card = OwnGamerCard(user);
        // A text setting: the keyboard (title, prompt and limit from gamerprofile.xex's strings), then saved; Back
        // or Done returns to this row.
        auto edit = [&](uint32_t row, uint32_t setting, size_t titleString, const char16_t* title,
                        size_t promptString, const char16_t* prompt, size_t maxLength, const std::u16string& current) {
            return [g, user, row, setting, titleString, title, promptString, prompt, maxLength, current] {
                g->back = [g, row] { GuideEditProfile(g, row); };
                GuideKeyboard(g, xui::GamerProfileString(titleString, title),
                              xui::GamerProfileString(promptString, prompt), maxLength,
                              [g, user, row, setting, current, titleString, title](const std::u16string& text) {
                                  if (text == current)
                                      return GuideEditProfile(g, row);
                                  const auto error = host::WriteGamercard(user, setting, text);
                                  if (!error)
                                      return GuideEditProfile(g, row);
                                  // Saved on the console; our Xbox LIVE did not take it (it gets it at the next
                                  // sign-in).
                                  GuideCommunityBox(g, xui::GamerProfileString(titleString, title),
                                                    Wide("Your profile was saved, but Xbox Live didn't get the "
                                                         "change: " + *error + "."),
                                                    { { u"OK", nullptr } });
                              },
                              current);
            };
        };
        if (host::OnLive(user))
        {
            add(u"Gamertag", nullptr);
            add(u"Gamer Picture", nullptr);
            add(u"Gamer Zone", [g, zone = card.zone] { GuideGamerZone(g, zone ? zone - 1 : 0); });
            add(u"Motto", edit(3, guide::kSettingMotto, 32, u"Motto", 31,
                               u"Your motto can contain up to 21 characters.", 21, card.motto));
            add(u"Avatar", nullptr);
            add(u"Name", edit(5, guide::kSettingName, 34, u"Name", 33,
                              u"Your name is visible to anyone who views your profile.", 129, card.name));
            add(u"Location", edit(6, guide::kSettingLocation, 30, u"Location", 29,
                                  u"Your location is visible to anyone who views your profile.", 40, card.location));
            add(u"Bio", edit(7, guide::kSettingBio, 28, u"Bio", 27,
                             u"Enter some information about yourself. Your bio is visible to anyone viewing your "
                             u"profile.", 499, card.bio));
        }
        else
        {
            add(u"Gamer Picture", nullptr);
            add(u"Profile Name", nullptr);
            add(u"Avatar", nullptr);
        }
        Log("System UI: Edit Profile of user {}", user);
        xui::ShowGamerProfile({}, 0, rows, std::min<uint32_t>(focus, uint32_t(rows.size()) - 1),
                              WithPlaceholders(card), xui::ProfilePanel::kPersonal, u"Edit Profile");
    }

    // Gamer Zone (7_42_50; SelectGamerzone.xur through xui::ShowGamerZone), from Edit Profile on our Xbox LIVE: the
    // focus starts on the profile's zone; A saves the focused one (host::WriteGamercard), Y shows More Info,
    // B goes back to Edit Profile's Gamer Zone row.
    void GuideGamerZone(const std::shared_ptr<GuideSession>& g, uint32_t focus)
    {
        const uint32_t user = g->user;
        if (!host::SignedIn(user))
            return GuideBack(g);
        g->mode = GuideSession::Mode::kProfile;
        g->profileView = GuideSession::ProfileView::kZone;
        g->back = nullptr;
        Log("System UI: Gamer Zone of user {}", user);
        xui::ShowGamerZone(WithPlaceholders(OwnGamerCard(user)), focus);
    }

    // A zone chosen (1-4, the list's order): saved as XPROFILE_GAMERCARD_ZONE and sent to our Xbox LIVE; back on Edit
    // Profile's Gamer Zone row.
    void GuideSetGamerZone(const std::shared_ptr<GuideSession>& g, uint32_t zone)
    {
        const uint32_t user = g->user;
        Log("System UI: gamer zone of user {} -> {}", user, zone);
        if (zone == OwnGamerCard(user).zone)
            return GuideEditProfile(g, 2);
        const auto error = host::WriteGamercard(user, guide::kSettingZone,
                                                         std::u16string(1, char16_t(u'0' + zone)));
        if (!error)
            return GuideEditProfile(g, 2);
        g->back = [g] { GuideEditProfile(g, 2); };
        GuideCommunityBox(g, u"Gamer Zone",
                          Wide("Your profile was saved, but Xbox Live didn't get the change: " + *error + "."),
                          { { u"OK", nullptr } });
    }

    void GuideFriendActions(const std::shared_ptr<GuideSession>& g, const live::Friend& f)
    {
        // A friend or a friend request opens the player's Profile (7_47_01 -> 7_47_09), a request with its Accept /
        // Decline (received) or Cancel Friend Request (sent) rows: FriendOptionsScene modes 0 / 1.
        if (f.request.empty() || f.request == "incoming" || f.request == "outgoing")
        {
            g->profileFromCommunity = true;
            return GuideProfile(g, f.xuid, 0);
        }
        const uint32_t user = g->user;
        const uint64_t xuid = f.xuid;
        const std::string tag = f.gamertag;
        std::vector<std::pair<std::u16string, std::function<void()>>> buttons;
        if (f.request == "incoming")
        {
            buttons.push_back({ u"Accept Friend Request", [g, user, xuid, tag] {
                GuideCommunityResult(g, u"Friend Request", live::AcceptFriend(user, xuid),
                                     Gamertag(tag) + u" is now your friend.");
            } });
            buttons.push_back({ u"Decline Friend Request", [g, user, xuid] {
                live::DeclineFriend(user, xuid);
                GuideCommunity(g, g->tab, g->communityFocus);
            } });
        }
        else if (f.request == "outgoing")
            buttons.push_back({ u"Cancel Friend Request", [g, user, xuid] {
                live::RemoveFriend(user, xuid);
                GuideCommunity(g, g->tab, g->communityFocus);
            } });
        else
        {
            buttons.push_back({ u"Send Message", [g, xuid, tag] { GuideSendMessage(g, xuid, tag); } });
            buttons.push_back({ u"Invite to Party", [g, user, xuid] {
                // The console starts a party if needed, invites and says so with its "Party invite sent" toast.
                if (!live::CurrentParty(user))
                    live::CreateParty(user);
                if (const auto error = live::InviteToParty(user, xuid))
                    return GuideCommunityResult(g, u"Invite to Party", error, u"");
                xui::QueueNotification(xui::kNotifyPartyInviteSent, user, u"");
                GuideCommunity(g, GuideSession::Tab::kParty);
            } });
            buttons.push_back({ u"Remove Friend", [g, user, xuid, tag] {
                GuideCommunityBox(g, u"Remove Friend", u"Remove " + Gamertag(tag) + u" from your friends list?",
                                  { { u"Remove", [g, user, xuid] {
                                        live::RemoveFriend(user, xuid);
                                        GuideCommunity(g, g->tab, g->communityFocus);
                                    } },
                                    { u"Cancel", nullptr } });
            } });
        }
        buttons.push_back({ u"Back", nullptr });
        GuideCommunityBox(g, Gamertag(tag), Presence(f), std::move(buttons));
    }

    // A friend's (or anyone's) gamercard from our Xbox LIVE: their profile and, for a friend, presence.
    xui::GamerCardInfo PlayerCard(uint32_t user, uint64_t xuid, const std::string& fallbackTag)
    {
        const std::vector<live::Friend> friends = live::Friends(user);
        const auto f = std::find_if(friends.begin(), friends.end(), [&](const live::Friend& x) { return x.xuid == xuid; });
        const auto profile = live::GetProfile(user, xuid);
        xui::GamerCardInfo card;
        card.gamertag = Gamertag(profile ? profile->gamertag : fallbackTag);
        if (profile)
        {
            card.gamerscore = profile->gamerscore;
            card.zone = profile->zone;
            card.reputation = profile->reputation / 20;
            card.motto = Wide(profile->motto);
            card.picture = profile->picture;
            SetTenure(card, profile->tenure);
        }
        if (f != friends.end() && f->request.empty())
        {
            card.online = f->online;
            if (!f->online && f->lastSeen)
                card.offlineSeconds = std::max<int64_t>(0, int64_t(std::time(nullptr)) - f->lastSeen); // "Offline for 3 hours"
        }
        return card;
    }

    // A message ("guide message.png"): when it came and when it expires over Reply / View Profile / Delete / Block
    // Communications (greyed: no blocking on our LIVE), the sender's gamercard and the text on the right. A party
    // invitation has Join Party in Reply's place. B: back to Messages.
    void GuideMessageActions(const std::shared_ptr<GuideSession>& g, const live::Message& m)
    {
        const uint32_t user = g->user;
        if (!m.read)
            live::MarkMessageRead(user, m.id);
        g->mode = GuideSession::Mode::kMessage;
        g->back = nullptr;
        g->rowActions.clear();
        std::vector<xui::ProfileRow> rows;
        auto add = [&](const std::u16string& text, std::function<void()> action) {
            rows.push_back({ text, bool(action) });
            g->rowActions.push_back(std::move(action));
        };
        const uint64_t from = m.from;
        const std::string tag = m.fromGamertag;
        std::u16string text = Wide(m.text);
        // The rows by type (Guide.Friends.xex's MessageOptionsScene table 0x901025E0: a party invite Join Party,
        // Decline Party, Reply, View Profile, Block; a friend request Accept / Decline Friend Request, Reply, View
        // Profile, Block; a message Reply, View Profile, Delete, Block), the title and icon by MessageTitle.
        auto withTag = [&](size_t index, const char16_t* fallback) {
            std::u16string t = xui::FriendsString(index, fallback);
            if (const size_t at = t.find(u"%s"); at != std::u16string::npos)
                t.replace(at, 2, Gamertag(tag));
            return t;
        };
        auto remove = [g, user, id = m.id] {
            live::DeleteMessage(user, id);
            GuideCommunity(g, GuideSession::Tab::kMessages, g->communityFocus);
        };
        auto reply = [g, from, tag] { GuideSendMessage(g, from, tag); };
        auto viewProfile = [g, from] {
            g->profileFromCommunity = true;
            GuideProfile(g, from, 0);
        };
        std::u16string title = xui::FriendsString(243, u"Message");
        std::string icon = "sharedres://ico_32x_Mail.png";
        if (m.kind == "party_invite")
        {
            const std::string party = m.text;
            text = withTag(180, u"%s wants you to join an Xbox Live Party.");
            title = xui::FriendsString(242, u"Party Invite");
            icon = "ico_32x_PartyInvite.png";
            add(xui::FriendsString(4, u"Join Party"), [g, user, party] {
                const auto error = live::JoinParty(user, party);
                if (error)
                    return GuideCommunityResult(g, u"Join Party", error, u"");
                GuideCommunity(g, GuideSession::Tab::kParty);
            });
            add(xui::FriendsString(53, u"Decline Party"), remove);
            add(xui::FriendsString(214, u"Reply"), reply);
            add(xui::FriendsString(255, u"View Profile"), viewProfile);
        }
        else if (m.kind == "friend_request")
        {
            text = withTag(78, u"%s wants to be your friend.");
            title = xui::FriendsString(239, u"Friend Request");
            icon = "friendinvite.png";
            add(xui::FriendsString(1, u"Accept Friend Request"), [g, user, from, tag] {
                GuideCommunityResult(g, u"Friend Request", live::AcceptFriend(user, from),
                                     Gamertag(tag) + u" is now your friend.");
            });
            add(xui::FriendsString(51, u"Decline Friend Request"), [g, user, from, remove] {
                live::DeclineFriend(user, from);
                remove();
            });
            add(xui::FriendsString(214, u"Reply"), reply);
            add(xui::FriendsString(255, u"View Profile"), viewProfile);
        }
        else
        {
            add(xui::FriendsString(214, u"Reply"), reply);
            add(xui::FriendsString(255, u"View Profile"), viewProfile);
            add(xui::FriendsString(55, u"Delete"), remove);
        }
        add(xui::FriendsString(22, u"Block Communications"), nullptr);
        // Received when, as the console writes it (7/01/2015, 20:21), and the 30 days our LIVE keeps a message.
        const std::time_t sent = std::time_t(m.sent);
        std::tm local{};
        localtime_s(&local, &sent);
        const int64_t left = std::max<int64_t>(0, (int64_t(m.sent) + 30 * 86400 - int64_t(std::time(nullptr))) / 86400);
        xui::ProfilePage page;
        page.info = Wide(std::format("Message Received\n{}/{:02}/{}\n{:02}:{:02}\nExpires in {} day{}",
                                     local.tm_mon + 1, local.tm_mday, local.tm_year + 1900, local.tm_hour,
                                     local.tm_min, left, left == 1 ? "" : "s"));
        page.textTitle = u"Message Text";
        page.text = text;
        page.icon = icon;
        Log("System UI: Guide: message {} from {}", m.id, tag);
        xui::ShowGamerProfile({}, 0, rows, 0, PlayerCard(user, from, tag), xui::ProfilePanel::kExtended, title, page);
    }

    // The status person before a friend's presence (owner's 7_47_01: green online, orange offline; XAM's
    // ico_32x_Friend.png is 235 grey, so the factors give those colours).
    constexpr const char* kStatusIcon = "sharedres://ico_32x_Friend.png";
    constexpr uint32_t kStatusOnline = 0xFF9BE825, kStatusOffline = 0xFFF34D1A;

    void GuideCreateMessage(const std::shared_ptr<GuideSession>& g)
    {
        // Messages > Create New: the console's type list first (Guide.Friends.xex's MessageType.xur, owner's
        // 7_50_28; descriptions strings 42 / 38 / 41 / 40 / 39): Message -> the composer, Party Invite -> the Friends
        // tab to pick who (in a party), Friend Request -> the gamertag. Private chat and game invites are greyed:
        // our LIVE has neither (the console greys Game Invite outside a game too). Without the scene: the composer.
        const uint32_t user = g->user;
        auto compose = [g] {
            g->composeTo.clear();
            g->composeText.clear();
            GuideCompose(g, 0);
        };
        const bool inParty = live::CurrentParty(user).has_value();
        const std::vector<xui::MessageTypeEntry> entries = {
            { true, xui::FriendsString(42, u"Send a message to one or more people.") },
            { false, xui::FriendsString(38, u"Invite someone to chat one-on-one.") },
            { inParty, xui::FriendsString(41, u"Invite someone to your party.") },
            { false, xui::FriendsString(40, u"Invite others to play a game.") },
            { true, xui::FriendsString(39, u"Ask another player to join your friends list.") },
        };
        g->rowActions = {
            compose,
            nullptr,
            [g] { GuideCommunity(g, GuideSession::Tab::kFriends); },
            nullptr,
            [g, user] {
                GuideKeyboard(g, xui::FriendsString(7, u"Add Friend"), xui::FriendsString(11, u"Enter a gamertag."), 15,
                              [g, user](const std::u16string& text) {
                                  const std::string tag = Utf8(text);
                                  if (tag.empty())
                                      return GuideCommunity(g, GuideSession::Tab::kMessages, g->communityFocus);
                                  GuideCommunityResult(g, u"Friend Request", live::AddFriend(user, tag),
                                                       u"A friend request was sent to " + text + u".");
                              });
            },
        };
        g->mode = GuideSession::Mode::kMessageType;
        g->back = nullptr;
        xui::GuideKeepPosition();
        if (!xui::ShowMessageCreate(entries, 0))
            compose();
    }

    // The composer ("guide send message.png", header "Message"): Change Gamertags, Add Voice and Add Picture (greyed:
    // our LIVE carries text only), Edit Text and, at the bottom, Send Message; "To:" and the text on the right. B
    // Cancel goes back to the Messages tab.
    std::u16string ComposeToText(const std::shared_ptr<GuideSession>& g)
    {
        std::u16string to;
        for (const auto& [xuid, tag] : g->composeTo)
            to += (to.empty() ? u"" : u", ") + Gamertag(tag); // RecipientsId: the names joined (FUN_9012AA88)
        return to;
    }

    // A gamertag typed in Enter Gamertags (or the composer's row without the scene): a friend joins the recipients.
    void GuideRecipientKeyboard(const std::shared_ptr<GuideSession>& g)
    {
        const uint32_t user = g->user;
        const bool scene = xui::CommunitySceneAvailable();
        GuideKeyboard(g, xui::FriendsString(12, u"Enter a Gamertag"), xui::FriendsString(11, u"Enter a gamertag."), 15,
                      [g, user, scene](const std::u16string& text) {
                          auto back = [g, scene] { scene ? GuideRecipients(g, 0) : GuideCompose(g, 0); };
                          const std::string tag = Utf8(text);
                          if (tag.empty())
                              return back();
                          for (const live::Friend& f : live::Friends(user))
                              if (f.request.empty() && _stricmp(f.gamertag.c_str(), tag.c_str()) == 0)
                              {
                                  const bool chosen = std::any_of(g->composeTo.begin(), g->composeTo.end(),
                                                                  [&](const auto& r) { return r.first == f.xuid; });
                                  if (!chosen)
                                      g->composeTo.push_back({ f.xuid, f.gamertag });
                                  return scene ? GuideRecipients(g, 0) : GuideCompose(g, 2);
                              }
                          g->back = back;
                          GuideCommunityBox(g, xui::FriendsString(10, u"Enter Gamertags"),
                                            text + u" isn't on your friends list.", { { u"OK", nullptr } });
                      });
        g->back = [g, scene] { scene ? GuideRecipients(g, 0) : GuideCompose(g, 0); };
    }

    // Enter Gamertags (Guide.Friends.xex's MessageRecipients.xur, MessageRecipientsScene): Enter Gamertag, then the
    // friends with check marks on the chosen ones (A toggles), the focused friend's gamercard; B back to the
    // composer with them.
    void GuideRecipients(const std::shared_ptr<GuideSession>& g, uint32_t focus)
    {
        const uint32_t user = g->user;
        g->mode = GuideSession::Mode::kRecipients;
        g->back = nullptr;
        g->rowActions.clear();
        std::vector<xui::RecipientEntry> entries;
        g->rowActions.push_back([g] { GuideRecipientKeyboard(g); });
        uint32_t index = 1;
        for (const live::Friend& f : live::Friends(user))
        {
            if (!f.request.empty())
                continue;
            xui::RecipientEntry e;
            e.gamertag = Gamertag(f.gamertag);
            e.checked = std::any_of(g->composeTo.begin(), g->composeTo.end(),
                                    [&](const auto& r) { return r.first == f.xuid; });
            e.card = PlayerCard(user, f.xuid, f.gamertag);
            entries.push_back(std::move(e));
            g->rowActions.push_back([g, xuid = f.xuid, tag = f.gamertag, index] {
                auto& to = g->composeTo;
                const auto it = std::find_if(to.begin(), to.end(), [&](const auto& r) { return r.first == xuid; });
                if (it != to.end())
                    to.erase(it);
                else if (to.size() < 100) // the module's limit
                    to.push_back({ xuid, tag });
                GuideRecipients(g, index);
            });
            index++;
        }
        xui::GuideKeepPosition();
        xui::ShowMessageRecipients(entries, focus);
    }

    // The composer ("guide send message.png"; Guide.Friends.xex's NewMessage.xur when it is there): the recipients
    // row (Enter Gamertags / Change Gamertags), Add Voice and Add Picture (greyed: our LIVE carries text only), Add /
    // Edit Text and, at the bottom, Send Message (greyed until there are recipients and text, as the module's
    // FUN_9012AD50); "To:" and the text on the right. B Cancel goes back to the Messages tab.
    void GuideCompose(const std::shared_ptr<GuideSession>& g, uint32_t focus)
    {
        const uint32_t user = g->user;
        g->mode = GuideSession::Mode::kCompose;
        g->back = nullptr;
        g->rowActions.clear();
        const bool scene = xui::CommunitySceneAvailable();
        std::vector<xui::ProfileRow> rows;
        auto add = [&](const std::u16string& text, std::function<void()> action, bool enabled = true) {
            rows.push_back({ text, bool(action) && enabled });
            g->rowActions.push_back(std::move(action));
        };
        const bool haveTo = !g->composeTo.empty(), haveText = !g->composeText.empty();
        add(haveTo ? xui::FriendsString(160, u"Change Gamertags") : xui::FriendsString(10, u"Enter Gamertags"),
            [g, scene] {
                if (scene)
                    return GuideRecipients(g, 0);
                GuideRecipientKeyboard(g);
            });
        add(xui::FriendsString(17, u"Add Voice"), nullptr);
        add(haveText ? xui::FriendsString(161, u"Edit Text") : xui::FriendsString(13, u"Add Text"), [g] {
            GuideKeyboard(g, xui::FriendsString(13, u"Add Text"), xui::FriendsString(14, u"Enter your message."), 250,
                          [g](const std::u16string& text) {
                              g->composeText = text;
                              GuideCompose(g, text.empty() ? 2 : 4);
                          }, g->composeText);
            g->back = [g] { GuideCompose(g, 2); };
        });
        add(xui::FriendsString(9, u"Add Picture"), nullptr);
        add(xui::FriendsString(226, u"Send Message"), [g, user] {
            g->back = [g] { GuideCompose(g, 4); };
            if (g->composeTo.empty())
                return GuideCommunityBox(g, u"Send Message", u"Choose who gets the message (Change Gamertags).",
                                         { { u"OK", nullptr } });
            if (g->composeText.empty())
                return GuideCommunityBox(g, u"Send Message", u"Type a message first (Edit Text).",
                                         { { u"OK", nullptr } });
            std::vector<uint64_t> to;
            for (const auto& r : g->composeTo)
                to.push_back(r.first);
            const auto error = live::SendLiveMessage(user, to, Utf8(g->composeText));
            const std::u16string names = ComposeToText(g);
            g->back = nullptr;
            if (!error)
            {
                g->composeText.clear();
                g->composeTo.clear();
                g->tab = GuideSession::Tab::kMessages;
            }
            GuideCommunityResult(g, u"Send Message", error, u"Your message was sent to " + names + u".");
        }, !scene || (haveTo && haveText));
        xui::ProfilePage page;
        page.card = false;
        page.to = ComposeToText(g);
        page.textTitle = u"Message Text:";
        page.text = g->composeText;
        page.rowIcons = scene ? std::vector<std::string>{ "ico_64x_friend.png", "ico_64x_VoiceAttach.png",
                                                          "ico_64x_addEditText.png", "ico_64x_PhotoAttach.png", "" }
                              : std::vector<std::string>{ "ico_64x_friend.png", "hud.hud/ico_32x_volume.png",
                                                          "builtin:Pencil", "ico_32x_camera.PNG", "builtin:SendMail" };
        page.bottomRow = 4;
        page.legendB = u"Cancel";
        page.icon = "sharedres://ico_32x_Mail.png";
        g->tab = GuideSession::Tab::kMessages;
        xui::ShowGamerProfile({}, 0, rows, focus, {}, xui::ProfilePanel::kExtended,
                              xui::FriendsString(243, u"Message"), page);
    }

    // While the Party tab shows its members: every 150 ms their voice icons as they are now (a default recording
    // device appearing, talking, muted), redrawn in place (xui::SetCommunityRowImage); the list used to keep the icons
    // it was built with. Ends when the Guide closes or leaves the Party tab.
    void GuideVoiceTicker(const std::shared_ptr<GuideSession>& g)
    {
        if (g->voiceTicker)
            return;
        g->voiceTicker = true;
        std::thread([g] {
            for (;;)
            {
                Sleep(150);
                std::lock_guard lock(g->mutex);
                if (g->ui->Closed() || g->mode != GuideSession::Mode::kCommunity ||
                    g->tab != GuideSession::Tab::kParty || g->partyVoice.empty())
                {
                    g->voiceTicker = false;
                    return;
                }
                for (GuideSession::PartyVoice& p : g->partyVoice)
                    if (std::string icon = VoiceIcon(p.xuid, p.self); icon != p.icon &&
                        xui::SetCommunityRowImage(uint32_t(GuideSession::Tab::kParty), p.row, 2, icon))
                        p.icon = std::move(icon);
            }
        }).detach();
    }

    void GuideCommunity(const std::shared_ptr<GuideSession>& g, GuideSession::Tab tab, uint32_t focus)
    {
        const uint32_t user = g->user;
        g->mode = GuideSession::Mode::kCommunity;
        g->back = nullptr;
        g->tab = tab;
        g->rowActions.clear();
        std::vector<xui::CommunityRow> rows;
        std::u16string message, footer, legendX, legendY;
        int counterFrom = -1, sceneButtons = 0;
        g->rowX.clear();
        g->rowY.clear();
        g->partyVoice.clear();
        std::vector<xui::CommunityTab> tabs;
        if (!host::OnLive(user))
            message = u"Sign in to Xbox Live to see your friends, messages and party.";
        else
        {
            // The tab strip's counts: party members, friends online, new messages, recent players online.
            auto friends = live::Friends(user);
            const std::vector<live::RecentPlayer> players = live::RecentPlayers(user);
            size_t playersOnline = 0;
            for (const auto& p : players)
                playersOnline += p.online;
            auto messages = live::Messages(user);
            std::stable_sort(messages.begin(), messages.end(),
                             [](const live::Message& a, const live::Message& b) { return a.sent > b.sent; }); // newest first
            const auto party = live::CurrentParty(user);
            size_t online = 0, unread = 0;
            for (const auto& f : friends)
                online += f.online && f.request.empty();
            for (const auto& m : messages)
                unread += !m.read;
            const size_t members = party ? party->members.size() : 0;
            const auto count = [](size_t n) { return Wide(std::to_string(n)); };
            const bool partyTab = tab == GuideSession::Tab::kParty, friendsTab = tab == GuideSession::Tab::kFriends,
                       messagesTab = tab == GuideSession::Tab::kMessages, playersTab = tab == GuideSession::Tab::kPlayers;
            tabs = { { "sharedres://ico_32x_StartParty.png",
                       partyTab ? Wide(std::format("Xbox Live Party ({})", members)) : count(members) },
                     { "sharedres://ico_32x_Friend.png",
                       friendsTab ? Wide(std::format("Friends ({} Online)", online)) : count(online) },
                     { "sharedres://ico_32x_Mail.png",
                       messagesTab ? Wide(std::format("Messages ({} New)", unread)) : count(unread) },
                     { "sharedres://ico_32x_controller.png",
                       playersTab ? Wide(std::format("Players ({} Online)", playersOnline)) : count(playersOnline) } };
            // The real scene's strip (Guide.Friends.xex strings 183 / 80 / 148 / 206 and "%d" on every tab).
            const size_t counts[4] = { members, online, unread, playersOnline };
            const size_t labels[4] = { 183, 80, 148, 206 };
            const char16_t* const labelFallbacks[4] = { u"Xbox Live Party (%d)", u"Friends (%d Online)",
                                                        u"Messages (%d New)", u"Players (%d Online)" };
            for (size_t i = 0; i < 4; i++)
            {
                tabs[i].label = FormatCount(xui::FriendsString(labels[i], labelFallbacks[i]), counts[i]);
                tabs[i].count = count(counts[i]);
            }

            if (friendsTab)
            {
                // Y Change Sort: requests and online friends first, or by gamertag only.
                const bool byName = g->sortByName;
                std::stable_sort(friends.begin(), friends.end(), [byName](const live::Friend& a, const live::Friend& b) {
                    auto rank = [](const live::Friend& f) {
                        return f.request == "outgoing" ? 0 : f.request == "incoming" ? 1 : f.online ? 2 : 3;
                    };
                    if (!byName && rank(a) != rank(b))
                        return rank(a) < rank(b);
                    return _stricmp(a.gamertag.c_str(), b.gamertag.c_str()) < 0;
                });
                footer = byName ? u"Sorted by gamertag" : u"Sorted by online status";
                legendY = u"Change Sort";
                counterFrom = 1;
                // "guide friends.png": Add Friend with the person and its circled plus.
                xui::CommunityRow add;
                add.line1 = u"Add Friend";
                add.icon = "builtin:AddFriend";
                // FriendsList item 0 (FUN_9011A3B8 / FUN_9011AE10): string 7 and the package's AddFriend.png.
                add.slotText[0] = xui::FriendsString(7, u"Add Friend");
                add.slotImage[2] = "AddFriend.png";
                rows.push_back(add);
                g->rowX.push_back(nullptr);
                g->rowActions.push_back([g, user] {
                    GuideKeyboard(g, u"Add Friend", u"Enter the gamertag of the person you want to add.", 15,
                                  [g, user](const std::u16string& text) {
                                      const std::string tag = Utf8(text);
                                      if (tag.empty())
                                          return GuideCommunity(g, g->tab, g->communityFocus);
                                      GuideCommunityResult(g, u"Add Friend", live::AddFriend(user, tag),
                                                           u"A friend request was sent to " + text + u".");
                                  });
                });
                for (const live::Friend& f : friends)
                {
                    xui::CommunityRow row;
                    row.line1 = Gamertag(f.gamertag);
                    row.right = Presence(f);
                    if (f.request.empty())
                    {
                        row.status = kStatusIcon;
                        row.statusColor = f.online ? kStatusOnline : kStatusOffline;
                    }
                    else
                    {
                        // An envelope: light for a request sent, dark for one to answer.
                        row.status = "sharedres://ico_32x_Mail.png";
                        row.statusColor = f.request == "incoming" ? 0xFF656D72 : 0xFFB9BFC3;
                    }
                    if (f.request == "incoming")
                    {
                        row.legendX = u"Accept Friend Request";
                        g->rowX.push_back([g, user, xuid = f.xuid, tag = f.gamertag] {
                            GuideCommunityResult(g, u"Friend Request", live::AcceptFriend(user, xuid),
                                                 Gamertag(tag) + u" is now your friend.");
                        });
                    }
                    else
                        g->rowX.push_back(nullptr);
                    const auto profile = live::GetProfile(user, f.xuid);
                    if (profile)
                    {
                        row.picture = profile->picture;
                        row.line2 = Wide(profile->motto);
                    }
                    FriendSlots(f, profile ? &*profile : nullptr, row);
                    rows.push_back(std::move(row));
                    g->rowActions.push_back([g, f] { GuideFriendActions(g, f); });
                }
            }
            else if (messagesTab)
            {
                // 7_50_04: "Create New" first, Y Create New.
                legendY = u"Create New";
                // "guide messages.png": Create New, then the sender, an envelope (dark: not read yet) and the text;
                // X Delete.
                legendX = u"Delete";
                xui::CommunityRow create;
                create.line1 = u"Create New";
                create.slotText[0] = xui::FriendsString(43, u"Create New"); // MessageList item 0 (FUN_9011E030)
                rows.push_back(create);
                g->rowActions.push_back([g] { GuideCreateMessage(g); });
                g->rowX.push_back(nullptr);
                for (const live::Message& m : messages)
                {
                    xui::CommunityRow row;
                    row.line1 = Gamertag(m.fromGamertag);
                    row.line2 = m.kind == "party_invite" ? u"Party invitation" : Wide(m.text.substr(0, 60));
                    row.status = m.kind == "party_invite" ? "sharedres://ico_32x_StartParty.png"
                                                          : "sharedres://ico_32x_Mail.png";
                    row.statusColor = m.read ? 0xFFA9AFB3 : 0xFF333A40;
                    MessageSlots(m, row);
                    rows.push_back(std::move(row));
                    g->rowActions.push_back([g, m] { GuideMessageActions(g, m); });
                    g->rowX.push_back([g, user, id = m.id] {
                        live::DeleteMessage(user, id);
                        GuideCommunity(g, g->tab, xui::PaneFocus());
                    });
                }
            }
            else if (partyTab)
            {
                if (!party)
                {
                    // "start a party.png": the one row, the explanation under it.
                    xui::CommunityRow start;
                    start.line1 = xui::FriendsString(234, u"Start a Party");
                    rows.push_back(start);
                    sceneButtons = 1;
                    g->rowActions.push_back([g, user] {
                        const auto error = live::CreateParty(user);
                        if (error)
                            return GuideCommunityResult(g, u"Start a Party", error, u"");
                        GuideCommunity(g, GuideSession::Tab::kParty);
                    });
                    message = xui::FriendsString(195, u"Start a new party. A party is a group of players that can "
                                                      u"play games and chat together.");
                }
                else
                {
                    // "in a party.png": Invite Players to Party, Invite Party to Game (greyed: no game session to
                    // invite to yet), Party Options with the chat channel, then the members (picture, gamertag, the
                    // controller when in a game, the title, the leader's mark); X Leave Party, Y Chat Channel.
                    legendX = u"Leave Party";
                    legendY = u"Chat Channel";
                    sceneButtons = 3; // btnInvite, btnInviteParty, btnSettings over the member list
                    xui::CommunityRow invite;
                    invite.line1 = xui::FriendsString(114, u"Invite Players to Party");
                    rows.push_back(invite);
                    g->rowActions.push_back([g] { GuideCommunity(g, GuideSession::Tab::kFriends); });
                    xui::CommunityRow game;
                    game.line1 = u"Invite Party to Game";
                    game.enabled = false;
                    rows.push_back(game);
                    g->rowActions.push_back([] {});
                    xui::CommunityRow options;
                    options.line1 = std::u16string(u"Party Options:  ") +
                                    (live::voice::GameChat() ? u"Game Chat" : u"Party Chat");
                    rows.push_back(options);
                    g->rowActions.push_back([g] { GuidePartyOptions(g, 0); });
                    const auto me = host::SignedIn(user);
                    const auto played = host::PlayedTitles(user);
                    for (const live::PartyMember& m : party->members)
                    {
                        const bool self = me && me->onlineXuid == m.xuid;
                        const std::u16string title = CachedTitleName(m, self, played);
                        g->partyVoice.push_back({ uint32_t(rows.size()), m.xuid, self, VoiceIcon(m.xuid, self) });
                        xui::CommunityRow row;
                        row.member = true;
                        row.leader = m.xuid == party->leader;
                        row.line1 = Gamertag(m.gamertag);
                        row.right = title;
                        if (m.titleId != 0 && m.titleId != 0xFFFE07D1) // in a game, not on the dashboard
                        {
                            row.status = "sharedres://ico_32x_controller.png";
                            row.statusColor = 0xFF92C542;
                        }
                        if (const auto profile = live::GetProfile(user, m.xuid))
                            row.picture = profile->picture;
                        PartyMemberSlots(m, row.leader, self, title, row);
                        rows.push_back(std::move(row));
                        const uint64_t xuid = m.xuid;
                        if (me && me->onlineXuid == xuid)
                            g->rowActions.push_back([] {});
                        else
                            g->rowActions.push_back([g, xuid] {
                                g->profileFromCommunity = true;
                                GuideProfile(g, xuid, 0);
                            });
                    }
                }
            }
            else if (playersTab)
            {
                // 7_50_12: the players met (our LIVE's recent_players: the same party, or the same game at the same
                // time), newest first; A their Profile, X Send Feedback (no player reviews on our LIVE: nothing),
                // Y Add Friend (a friend request to the focused player).
                legendX = xui::FriendsString(222, u"Send Feedback");
                legendY = xui::FriendsString(7, u"Add Friend");
                std::set<uint64_t> known;
                for (const live::Friend& f : friends)
                    known.insert(f.xuid);
                for (const live::RecentPlayer& p : players)
                {
                    xui::CommunityRow row;
                    row.line1 = Gamertag(p.gamertag);
                    row.right = Wide(p.title) + u"\n" + LastMet(p.met);
                    row.status = kStatusIcon;
                    row.statusColor = p.online ? kStatusOnline : kStatusOffline;
                    const auto profile = live::GetProfile(user, p.xuid);
                    if (profile)
                        row.picture = profile->picture;
                    PlayerSlots(p, profile ? &*profile : nullptr, row);
                    rows.push_back(std::move(row));
                    g->rowActions.push_back([g, xuid = p.xuid] {
                        g->profileFromCommunity = true;
                        GuideProfile(g, xuid, 0);
                    });
                    g->rowX.push_back(nullptr);
                    if (known.contains(p.xuid)) // a friend already, or a request either way
                        g->rowY.push_back(nullptr);
                    else
                        g->rowY.push_back([g, user, tag = p.gamertag] {
                            GuideCommunityResult(g, xui::FriendsString(7, u"Add Friend"), live::AddFriend(user, tag),
                                                 u"A friend request was sent to " + Gamertag(tag) + u".");
                        });
                }
            }
        }
        Log("System UI: Guide: Community tab {} ({} rows)", int(tab), rows.size());
        xui::GuideKeepPosition();
        // The Party and Messages tabs list one-line rows, half the Friends / Players rows' height (captures 7_49_59,
        // 7_50_04, partychat.mp4: 28 units against 46).
        const bool oneLine = tab == GuideSession::Tab::kParty || tab == GuideSession::Tab::kMessages;
        xui::CommunityOptions options;
        options.footer = footer;
        options.legendX = legendX;
        options.legendY = legendY;
        options.layout = oneLine ? xui::CommunityLayout::kOneLine : xui::CommunityLayout::kTwoLine;
        options.counterFrom = counterFrom;
        options.sceneButtons = sceneButtons;
        xui::ShowCommunity(u"Community", tabs, uint32_t(tab),
                           rows, std::min<uint32_t>(focus, rows.empty() ? 0 : uint32_t(rows.size() - 1)), message,
                           options);
        if (!g->partyVoice.empty())
            GuideVoiceTicker(g);
    }

    // Party Options ("party settings party chat.png", "... game chat.png", "... invite only.png"): Chat Channel
    // (Party Chat / Game Chat), Party Status (Open to Friends / Invite Only; the leader's to change) and Leave Party,
    // under the "Party Options" header without tabs.
    void GuidePartyOptions(const std::shared_ptr<GuideSession>& g, uint32_t focus)
    {
        const auto party = live::CurrentParty(g->user);
        if (!party)
            return GuideCommunity(g, GuideSession::Tab::kParty);
        g->mode = GuideSession::Mode::kPartyOptions;
        g->back = nullptr;
        {
            // Guide.Friends.xex's PartySettings.xur: strings 174 / 82 (channel), 267 / 266 (the leader's Party
            // Status spinner), 119 / 118 (another member's status).
            const auto me = host::SignedIn(g->user);
            const bool leader = me && me->onlineXuid == party->leader;
            const std::u16string channel = live::voice::GameChat() ? xui::FriendsString(82, u"Game Chat")
                                                                   : xui::FriendsString(174, u"Party Chat");
            const std::u16string status =
                party->inviteOnly ? xui::FriendsString(leader ? 266 : 118, u"Invite Only")
                                  : xui::FriendsString(leader ? 267 : 119, u"Open to Friends");
            xui::GuideKeepPosition();
            g->partyScene = xui::ShowPartySettings(channel, status, leader, std::min<uint32_t>(focus, 2));
            if (g->partyScene)
                return;
        }
        std::vector<xui::CommunityRow> rows(3);
        rows[0].line1 = u"Chat Channel";
        rows[0].value = live::voice::GameChat() ? u"Game Chat" : u"Party Chat";
        rows[1].line1 = u"Party Status";
        rows[1].value = party->inviteOnly ? u"Invite Only" : u"Open to Friends";
        const auto me = host::SignedIn(g->user);
        rows[1].enabled = me && me->onlineXuid == party->leader;
        rows[2].line1 = u"Leave Party";
        xui::GuideKeepPosition();
        xui::CommunityOptions options;
        options.layout = xui::CommunityLayout::kOneLine;
        xui::ShowCommunity(u"Party Options", {}, 0, rows, std::min<uint32_t>(focus, 2), u"", options);
    }

    // A on (or left / right over) a Party Options row.
    void GuidePartyOption(const std::shared_ptr<GuideSession>& g, uint32_t index)
    {
        const uint32_t user = g->user;
        if (index == 0)
        {
            live::voice::SetGameChat(!live::voice::GameChat());
            Log("System UI: Guide: party chat channel: {}", live::voice::GameChat() ? "Game Chat" : "Party Chat");
            return GuidePartyOptions(g, 0);
        }
        if (index == 1)
        {
            const auto party = live::CurrentParty(user);
            const auto me = host::SignedIn(user);
            if (!party || !me || me->onlineXuid != party->leader)
                return; // only the leader sets who may join
            if (const auto error = live::SetPartyInviteOnly(user, !party->inviteOnly))
                Log("System UI: Guide: Party Status: {}", *error);
            return GuidePartyOptions(g, 1);
        }
        if (index == 2)
        {
            live::LeaveParty(user);
            GuideCommunity(g, GuideSession::Tab::kParty);
        }
    }

    // Y on the party tab: Party Chat <-> Game Chat.
    void GuideToggleChatChannel(const std::shared_ptr<GuideSession>& g)
    {
        live::voice::SetGameChat(!live::voice::GameChat());
        GuideCommunity(g, GuideSession::Tab::kParty, xui::PaneFocus());
    }

    // Compare Games (XamShowGamesUI with another player; gamerprofile.xex's GameShowcaseYouScene): the titles either
    // of us has played, theirs from our Xbox LIVE (titles_of, published at each sign-in), mine from my profile; the
    // other player's order first (most recent first), then mine they have not played. Gamerscores per title, the totals
    // from the two gamercards.
    void GuideCompareGames(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t focus)
    {
        const uint32_t user = g->user;
        const auto me = host::SignedIn(user);
        if (!me)
            return;
        const std::vector<live::TitleSummary> theirs = live::TitlesOf(user, xuid);
        const std::vector<host::PlayedTitle> mine = host::PlayedTitles(user);
        const auto profile = live::GetProfile(user, xuid);
        std::vector<xui::CompareGameRow> rows;
        g->compareTitles.clear();
        std::set<uint32_t> listed;
        auto score = [](uint32_t gamerscore) { return Wide(std::to_string(gamerscore)); };
        for (const live::TitleSummary& t : theirs)
        {
            xui::CompareGameRow row;
            row.name = Wide(t.name);
            row.theirs = score(t.gamerscore);
            row.mine = u"--"; // not played (owner's capture 3_14_10)
            row.tile = t.tile;
            GuideSession::CompareTitle title{ t.titleId, row.name, t.gamerscore, 0 };
            for (const host::PlayedTitle& m : mine)
                if (m.titleId == t.titleId)
                {
                    row.mine = score(m.gamerscore);
                    title.mine = m.gamerscore;
                    if (row.tile.empty())
                        row.tile = m.tile;
                }
            listed.insert(t.titleId);
            rows.push_back(std::move(row));
            g->compareTitles.push_back(std::move(title));
        }
        for (const host::PlayedTitle& m : mine)
            if (!listed.count(m.titleId))
            {
                rows.push_back({ m.name, u"--", score(m.gamerscore), m.tile });
                g->compareTitles.push_back({ m.titleId, m.name, 0, m.gamerscore });
            }
        g->profileXuid = xuid;
        g->mode = GuideSession::Mode::kCompareGames;
        g->rowActions.clear();
        Log("System UI: Guide: Compare Games with {:016X}: {} of theirs, {} of mine, {} rows", xuid, theirs.size(),
            mine.size(), rows.size());
        xui::GuideKeepPosition();
        xui::ShowCompareGames(profile ? profile->picture : std::vector<uint8_t>(), profile ? profile->gamerscore : 0,
                              host::GamerPicture(me->xuid), host::ReadGamercard(me->xuid).gamerscore.value_or(0), rows,
                              focus);
    }

    // A Compare Games title's achievements (gamerprofile.xex's YouAchievementsScene): mine from my profile's GPD
    // (in its order, with the SPA's images), theirs from our Xbox LIVE (title_achievements_of), matched by name; the
    // ones only they have (a title I have not played) in their order after.
    void GuideCompareAchievements(const std::shared_ptr<GuideSession>& g, uint32_t focus)
    {
        if (g->compareFocus >= g->compareTitles.size())
            return;
        const uint32_t user = g->user;
        const auto me = host::SignedIn(user);
        if (!me)
            return;
        const GuideSession::CompareTitle& title = g->compareTitles[g->compareFocus];
        const std::vector<live::AchievementSummary> theirs = live::TitleAchievementsOf(user, g->profileXuid,
                                                                                        title.titleId);
        std::vector<xui::AchievementEntry> mine;
        const std::vector<host::PlayedTitle> played = host::PlayedTitles(user);
        if (std::any_of(played.begin(), played.end(),
                        [&](const host::PlayedTitle& t) { return t.titleId == title.titleId; }))
            mine = host::PlayedTitleAchievements(user, title.titleId);
        std::vector<GuideSession::CompareAchievement>& list = g->compareAchievements;
        list.clear();
        std::vector<bool> matched(theirs.size());
        for (const xui::AchievementEntry& m : mine)
        {
            GuideSession::CompareAchievement a{ m.label, m.description, m.unachieved, m.gamerscore, false, m.earned,
                                               m.showUnachieved, m.image };
            for (size_t i = 0; i < theirs.size(); i++)
                if (!matched[i] && Wide(theirs[i].name) == m.label)
                {
                    matched[i] = true;
                    a.theirs = theirs[i].earned;
                    break;
                }
            list.push_back(std::move(a));
        }
        for (size_t i = 0; i < theirs.size(); i++)
            if (!matched[i])
                list.push_back({ Wide(theirs[i].name), Wide(theirs[i].description), Wide(theirs[i].unachieved),
                                 theirs[i].gamerscore, theirs[i].earned, false, theirs[i].shown, {} });
        std::vector<xui::CompareAchievementRow> rows;
        for (const GuideSession::CompareAchievement& a : list)
            rows.push_back({ a.name, a.theirs, a.mine, a.shown, a.gamerscore, a.image });
        const auto profile = live::GetProfile(user, g->profileXuid);
        g->mode = GuideSession::Mode::kCompareAchievements;
        g->rowActions.clear();
        Log("System UI: Guide: Compare achievements of {:08X} with {:016X}: {} of theirs, {} of mine, {} rows",
            title.titleId, g->profileXuid, theirs.size(), mine.size(), rows.size());
        xui::GuideKeepPosition();
        xui::ShowCompareAchievements(title.name, profile ? profile->picture : std::vector<uint8_t>(), title.theirs,
                                     host::GamerPicture(me->xuid), title.mine, rows, focus);
    }

    // After Create Profile (XAM's createprofile.cp): Congratulations.xur, "Save Profile", with the new profile's
    // gamercard (owner-less defaults: no titles, achievements or gamerscore yet).
    void GuideProfileCreated(const std::shared_ptr<GuideSession>& g, uint32_t focus)
    {
        const auto me = host::SignedIn(g->user);
        if (!me)
            return GuideBack(g);
        g->mode = GuideSession::Mode::kProfileCreated;
        const guide::Gamercard card = host::ReadGamercard(me->xuid);
        const std::vector<host::PlayedTitle> titles = host::PlayedTitles(g->user);
        uint32_t achievements = 0;
        for (const host::PlayedTitle& t : titles)
            achievements += t.earned;
        xui::GuideKeepPosition();
        xui::ShowProfileCreated(Wide(me->gamertag), host::GamerPicture(me->xuid), card.gamerscore.value_or(0),
                                uint32_t(titles.size()), achievements, focus);
    }

    // Join Xbox Live: AddXboxLive.xur ("Checking network status") while the profile signs in to our Xbox LIVE
    // (registering its gamertag there); online, the screen closes with the Xbox LIVE toast, else "Can't Connect"
    // (createprofile strings 22 / 21) and back to Save Profile.
    void GuideAddXboxLive(const std::shared_ptr<GuideSession>& g)
    {
        const auto profile = host::SignedIn(g->user);
        if (!profile)
            return GuideBack(g);
        g->mode = GuideSession::Mode::kAddXboxLive;
        xui::GuideKeepPosition();
        xui::ShowAddXboxLive(xui::CreateProfileString(0, u"Checking network status"));
        const uint32_t user = g->user;
        const std::u16string tag(profile->gamertag.begin(), profile->gamertag.end());
        std::thread([g, user, tag] {
            bool online = host::OnLive(user);
            std::optional<std::string> error;
            if (!online)
            {
                error = live::Reconnect(user);
                online = !error && WaitForLive(user);
            }
            std::lock_guard lock(g->mutex);
            if (g->ui->Closed() || g->mode != GuideSession::Mode::kAddXboxLive)
                return;
            if (online)
            {
                Log("System UI: Guide: {} joined Xbox LIVE", Utf8(tag));
                xui::QueueNotification(xui::kNotifySignedInLive, user, tag);
                return GuideBack(g);
            }
            Log("System UI: Guide: Join Xbox LIVE failed: {}", error ? *error : std::string("not signed in"));
            g->back = [g] { GuideProfileCreated(g, 0); };
            GuideCommunityBox(g, xui::CreateProfileString(22, u"Can't Connect"),
                              xui::CreateProfileString(21, u"Can't connect to Xbox Live."), { { u"OK", nullptr } });
        }).detach();
    }

    // View Friends (a friend's Personal Profile) / XamShowFofUI: Guide.Friends.xex's FofScene.xur ("%s's Friends",
    // string 76; owner's 7_47_34) with the friend's friends from our LIVE (friends_of).
    void GuideFriendsOf(const std::shared_ptr<GuideSession>& g, uint64_t xuid, const std::string& tag)
    {
        const uint32_t user = g->user;
        g->profileXuid = xuid;
        std::u16string heading = xui::FriendsString(76, u"%s's Friends");
        if (const size_t at = heading.find(u"%s"); at != std::u16string::npos)
            heading.replace(at, 2, Gamertag(tag));
        std::vector<xui::CommunityRow> rows;
        g->rowActions.clear();
        for (const live::Friend& f : live::FriendsOf(user, xuid))
        {
            xui::CommunityRow row;
            const auto profile = live::GetProfile(user, f.xuid);
            if (profile)
                row.picture = profile->picture;
            FriendSlots(f, profile ? &*profile : nullptr, row);
            rows.push_back(std::move(row));
            const auto me = host::SignedIn(user);
            const uint64_t other = f.xuid;
            if (me && me->onlineXuid == other)
                g->rowActions.push_back(nullptr); // yourself in their list
            else
                g->rowActions.push_back([g, other] {
                    g->profileFromCommunity = false;
                    GuideProfile(g, other, 0);
                });
        }
        Log("System UI: Guide: {}'s friends ({})", tag, rows.size());
        g->mode = GuideSession::Mode::kFriendsOf;
        xui::GuideKeepPosition();
        xui::ShowFriendsOf(heading, rows, 0);
    }

    // Settings > Preferences: hud.xex's Options.xur (Online Status, Notifications, Voice, Vibration; Family Timer
    // and Word Registration are left out as the console leaves them out without a family timer / outside Japan) and
    // the scene each opens. Changes are saved as they are made.
    const char* const kPreferenceScenes[5] = { "Options.xur", "OptionsOnline.xur", "OptionsNotifications.xur",
                                               "OptionsVoice.xur", "OptionsController.xur" };
    const char* const kOnlineStatuses[4] = { "online", "away", "busy", "offline" };

    std::vector<xui::SceneControl> PreferenceControls(const std::shared_ptr<GuideSession>& g, uint32_t scene)
    {
        const host::Preferences p = host::ReadPreferences(g->user);
        switch (scene)
        {
        case 0:
            return { { "btnOnlineStatus" }, { "btnNotifications" }, { "btnVoice" }, { "btnController" } };
        case 1:
        {
            std::vector<xui::SceneControl> c;
            for (uint32_t i = 0; i < 4; i++)
                c.push_back({ std::string("radbtn") + (i == 0 ? "Online" : i == 1 ? "Away" : i == 2 ? "Busy" :
                              "AppearOffline"), g->onlineStatus == kOnlineStatuses[i] });
            return c;
        }
        case 2:
            return { { "chkShow", p.notifyShow }, { "chkSound", p.notifySound }, { "chkShowMovies", p.notifyVideos } };
        case 3:
            return { { "sliderVolume", false, int(p.voiceVolume) }, { "sliderDucking", false, int(p.gameVolume) },
                     { "radbtnPlayHeadset", p.voiceOutput == 0 }, { "radbtnPlayTV", p.voiceOutput == 1 },
                     { "radbtnPlayBoth", p.voiceOutput == 2 } };
        default:
            return { { "chkVibration", p.vibration } };
        }
    }

    void GuidePreferences(const std::shared_ptr<GuideSession>& g, uint32_t scene, uint32_t focus)
    {
        if (!host::SignedIn(g->user))
            return;
        if (scene == 1)
        {
            // Online Status is our Xbox LIVE's (status); offline the scene shows Online.
            const auto status = live::OnlineStatus(g->user, "");
            g->onlineStatus = status ? *status : "online";
        }
        g->mode = GuideSession::Mode::kPreferences;
        g->prefScene = scene;
        const std::vector<xui::SceneControl> controls = PreferenceControls(g, scene);
        if (scene == 1)
            for (uint32_t i = 0; i < controls.size(); i++)
                if (controls[i].checked)
                    focus = i;
        std::vector<std::string> hidden;
        if (scene == 0)
            hidden = { "btnPlayTimer", "btnWordRegister" };
        else if (scene == 2)
            hidden = { "labelSoundDisabled" };
        xui::GuideKeepPosition();
        xui::ShowControlScene("hud.hud", kPreferenceScenes[scene], controls, hidden, focus);
    }

    // A (step 0) or left / right (step -1 / 1) on a Preferences control.
    void GuidePreferencePress(const std::shared_ptr<GuideSession>& g, uint32_t index, int step)
    {
        const uint32_t scene = g->prefScene;
        if (scene == 0)
        {
            if (step == 0 && index < 4)
                GuidePreferences(g, index + 1, 0);
            return;
        }
        host::Preferences p = host::ReadPreferences(g->user);
        if (scene == 1)
        {
            if (step != 0 || index >= 4)
                return;
            if (const auto now = live::OnlineStatus(g->user, kOnlineStatuses[index]))
                g->onlineStatus = *now;
            Log("System UI: Guide: online status {}", g->onlineStatus);
        }
        else if (scene == 2 && step == 0)
        {
            bool* flags[3] = { &p.notifyShow, &p.notifySound, &p.notifyVideos };
            if (index < 3)
                *flags[index] = !*flags[index];
        }
        else if (scene == 3)
        {
            if (index < 2 && step != 0)
            {
                uint32_t& volume = index == 0 ? p.voiceVolume : p.gameVolume;
                volume = uint32_t(std::clamp(int(volume) + step, 0, 10));
            }
            else if (index >= 2 && index < 5 && step == 0)
                p.voiceOutput = index - 2;
            else
                return;
        }
        else if (scene == 4 && step == 0)
            p.vibration = !p.vibration;
        else
            return;
        if (scene != 1)
            host::WritePreferences(g->user, p);
        const std::vector<xui::SceneControl> controls = PreferenceControls(g, scene);
        for (uint32_t i = 0; i < controls.size(); i++)
            xui::UpdateSceneControl(i, controls[i]);
    }

    // Submit Player Review and File Complaint (XamShowPlayerReviewUI / XamShowComplaintUI: XAM message 0x21009 to the
    // HUD, mode 0 / 1), as the user's captures (3_16_48 - 3_24_39) show them: a page on the Profile frame with a
    // message at the top left, the rows at the bottom (with icons on the first page), the player's gamercard and the
    // focused row's description on the right; File a Complaint's pages have X Code of Conduct. Review: Prefer This
    // Player (sent at once, the "is now a preferred player" toast), Avoid This Player (a reason: the console's
    // further pages are not captured; the categories as kurtsh.com 2008-04-01), What Is Rep?. Complaint: More
    // Information / File Complaint / Don't File, then Step 1 (the general reason), Step 2 (the specific reason),
    // Step 3 (Confirm Complaint: Don't File / File Complaint). Each sends to our Xbox LIVE (feedback) and goes back to
    // the Profile (or closes, opened by a title).
    struct FeedbackReason
    {
        const char16_t *name, *description, *complaint; // complaint: Step 3's name for it
    };
    struct FeedbackCategory
    {
        const char16_t *name, *description;
        std::vector<FeedbackReason> reasons;
    };
    const std::vector<FeedbackCategory>& AvoidReasons()
    {
        static const std::vector<FeedbackCategory> reasons = {
            { u"Communication", u"", { { u"Trash-talking", u"", u"" }, { u"Bothersome Language", u"", u"" },
                                       { u"Disruptive Voice", u"", u"" } } },
            { u"Game Behavior", u"", { { u"Overly Aggressive", u"", u"" }, { u"Unsporting Conduct", u"", u"" },
                                       { u"Quit Early", u"", u"" } } },
            { u"Player Skill", u"", { { u"Lacking Skill", u"", u"" }, { u"Too Good", u"", u"" },
                                      { u"Unfamiliar With Game", u"", u"" } } } };
        return reasons;
    }
    // The captures' texts. Step 3's complaint names: "Offensive Gamertag" is captured (3_23_23); the others follow it.
    const std::vector<FeedbackCategory>& ComplaintReasons()
    {
        static const std::vector<FeedbackCategory> reasons = {
            { u"Profile", u"The player's gamertag, personal picture, or personal info contains inappropriate material.",
              { { u"Gamertag", u"The player's gamertag is offensive or inappropriate.", u"Offensive Gamertag" },
                { u"Personal Picture", u"The player's personal picture is offensive or inappropriate.",
                  u"Offensive Personal Picture" },
                { u"Personal Info", u"The player's motto, name, location, or bio is offensive or inappropriate.",
                  u"Offensive Personal Info" },
                { u"Player Content", u"In-game content created by the player was offensive. Player content may consist "
                                     u"of images, game maps, characters, vehicles, and so on.",
                  u"Offensive Player Content" } } },
            { u"Text and Voice Communications", u"The player used vulgar, hateful, harassing, or threatening language "
                                                u"in voice or text communications.",
              { { u"Text Message", u"The player sent a vulgar, hateful, harassing, or threatening text message.",
                  u"Offensive Text Message" },
                { u"Voice Message", u"The player sent a vulgar, hateful, harassing, or threatening voice message.",
                  u"Offensive Voice Message" },
                { u"Voice Chat", u"The player used vulgar, hateful, harassing, or threatening language in voice chat.",
                  u"Offensive Voice Chat" },
                { u"In-Game Voice", u"The player used vulgar, hateful, harassing, or threatening language during "
                                    u"in-game communications.",
                  u"Offensive In-Game Voice" } } },
            { u"Video Communications", u"The player was vulgar, hateful, harassing, or threatening in video "
                                       u"communications.",
              { { u"Picture Message", u"The player sent a vulgar, hateful, harassing, or threatening picture message.",
                  u"Offensive Picture Message" },
                { u"Video Chat", u"The player used vulgar, hateful, harassing, or threatening language or gestures in "
                                 u"Video Kinect or in-game video chat.",
                  u"Offensive Video Chat" } } },
            { u"Tampering", u"The player tampered with a game, a console, Xbox Live, or the feedback system.",
              { { u"Feedback Tampering", u"The player misused the feedback system or attempted to influence player "
                                         u"feedback.", u"Feedback Tampering" },
                { u"System Tampering", u"The player altered, attempted to alter, or promoted altering a game, an Xbox "
                                       u"console, or Xbox Live.", u"System Tampering" },
                { u"Cheating", u"The player used known or unknown in-game or other exploits to influence the results "
                               u"of a game.", u"Cheating" } } } };
        return reasons;
    }

    struct FeedbackRow
    {
        std::u16string text;
        std::string icon; // the first pages' row icons
        std::u16string description;
        std::function<void()> action;
    };

    // One feedback page; `back` is B, `conduct` adds X Code of Conduct.
    void GuideFeedbackPage(const std::shared_ptr<GuideSession>& g, uint64_t xuid, const std::u16string& header,
                           const std::u16string& message, const std::vector<FeedbackRow>& rows, uint32_t focus,
                           std::function<void()> back, bool conduct)
    {
        g->profileXuid = xuid;
        g->mode = GuideSession::Mode::kFeedback;
        g->feedbackBack = std::move(back);
        g->feedbackConduct = conduct;
        g->feedbackAgain = [g, xuid, header, message, rows, conduct, back = g->feedbackBack](uint32_t f) {
            GuideFeedbackPage(g, xuid, header, message, rows, f, back, conduct);
        };
        g->rowActions.clear();
        std::vector<xui::ProfileRow> list;
        xui::ProfilePage page;
        page.info = message;
        page.rowsAtBottom = true;
        bool icons = false;
        for (const FeedbackRow& r : rows)
        {
            list.push_back({ r.text, true });
            g->rowActions.push_back(r.action);
            page.rowIcons.push_back(r.icon);
            page.rowDescriptions.push_back(r.description);
            icons = icons || !r.icon.empty();
        }
        if (!icons)
            page.rowIcons.clear();
        if (conduct)
            page.legendX = u"Code of Conduct";
        xui::GuideKeepPosition();
        xui::ShowGamerProfile({}, 0, list, std::min<uint32_t>(focus, uint32_t(rows.size() - 1)),
                              PlayerCard(g->user, xuid, ""), xui::ProfilePanel::kPersonal, header, page);
    }

    // A page of text over the feedback pages (What Is Rep?, More Information, Code of Conduct): Guide.Friends'
    // Bio.xur; B back to the page it came from.
    void GuideFeedbackText(const std::shared_ptr<GuideSession>& g, const std::u16string& heading,
                           const std::u16string& text)
    {
        g->feedbackFocus = xui::PaneFocus();
        g->mode = GuideSession::Mode::kFeedbackText;
        xui::GuideKeepPosition();
        if (!xui::ShowBio(heading, text))
            g->feedbackAgain(g->feedbackFocus);
    }

    // After the review or complaint: back to the player's Profile, or closed when a title opened the screen.
    void GuideFeedbackDone(const std::shared_ptr<GuideSession>& g, uint64_t xuid)
    {
        if (g->direct && g->feedbackDirect)
            return GuideBack(g);
        GuideProfile(g, xuid, 0);
    }

    void GuideSendFeedback(const std::shared_ptr<GuideSession>& g, uint64_t xuid, const char* kind,
                           const std::u16string& category, const std::u16string& reason)
    {
        if (const auto error = live::SendFeedback(g->user, xuid, kind, Utf8(category), Utf8(reason)))
            Log("System UI: Guide: {} about {:016X} not sent: {}", kind, xuid, *error);
        else
            Log("System UI: Guide: {} about {:016X} sent ({} / {})", kind, xuid, Utf8(category), Utf8(reason));
        GuideFeedbackDone(g, xuid);
    }

    void GuideAvoidReasons(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t category)
    {
        const FeedbackCategory& c = AvoidReasons()[category];
        std::vector<FeedbackRow> rows;
        for (const FeedbackReason& r : c.reasons)
            rows.push_back({ r.name, "", r.description, [g, xuid, category, name = r.name] {
                GuideSendFeedback(g, xuid, "avoid", AvoidReasons()[category].name, name);
            } });
        GuideFeedbackPage(g, xuid, u"Submit Player Review", u"Select the specific reason for avoiding this player.",
                          rows, 0, [g, xuid, category] { GuideAvoid(g, xuid, category); }, false);
    }

    // Avoid This Player's general reasons.
    void GuideAvoid(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t focus)
    {
        std::vector<FeedbackRow> rows;
        for (uint32_t i = 0; i < AvoidReasons().size(); i++)
            rows.push_back({ AvoidReasons()[i].name, "", AvoidReasons()[i].description,
                             [g, xuid, i] { GuideAvoidReasons(g, xuid, i); } });
        GuideFeedbackPage(g, xuid, u"Submit Player Review", u"Select the general reason for avoiding this player.",
                          rows, focus, [g, xuid] { GuidePlayerReview(g, xuid, 1); }, false);
    }

    void GuidePlayerReview(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t focus)
    {
        std::vector<FeedbackRow> rows = {
            { u"Prefer This Player", "guide.friends.friends/PreferredPlayer.png",
              u"This player will become a preferred player, which will increase your chances of playing together in "
              u"the future.\n\nSelecting this option will immediately submit a preferred player review.",
              [g, xuid] {
                  const auto profile = live::GetProfile(g->user, xuid);
                  if (profile)
                      xui::QueueNotification(xui::kNotifyPreferredPlayer, g->user, Wide(profile->gamertag));
                  GuideSendFeedback(g, xuid, "prefer", u"", u"");
              } },
            { u"Avoid This Player", "guide.friends.friends/NegativeFeedback.png",
              u"This player will become an avoided player, which will decrease your chances of playing together."
              u"\n\nIf you select this option, you will be asked for more information.",
              [g, xuid] { GuideAvoid(g, xuid, 0); } },
            { u"What Is Rep?", "builtin:RepQuestion", u"View more information about Rep.",
              [g] {
                  GuideFeedbackText(g, u"What Is Rep?",
                                    u"Rep is a player's reputation: the stars on the gamercard. Other players' reviews "
                                    u"raise it (preferred) or lower it (avoided).");
              } } };
        GuideFeedbackPage(g, xuid, u"Submit Player Review", u"Please select the type of review you would like to submit.",
                          rows, focus, [g, xuid] { GuideFeedbackDone(g, xuid); }, false);
    }

    // Step 3: Confirm Complaint.
    void GuideComplaintConfirm(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t category, uint32_t reason)
    {
        const FeedbackCategory& c = ComplaintReasons()[category];
        const FeedbackReason& r = c.reasons[reason];
        std::vector<FeedbackRow> rows = {
            { u"Don't File", "", u"Cancel complaint.", [g, xuid] { GuideFeedbackDone(g, xuid); } },
            { u"File Complaint", "", u"Submit the complaint to Xbox Live.\n\nAll complaints are reviewed by Xbox Live. "
                                     u"However, Xbox Live policy does not permit disclosure of the status or results of "
                                     u"individual complaints.",
              [g, xuid, category, reason] {
                  const FeedbackCategory& c = ComplaintReasons()[category];
                  GuideSendFeedback(g, xuid, "complaint", c.name, c.reasons[reason].name);
              } } };
        GuideFeedbackPage(g, xuid, u"Confirm Complaint: Step 3 of 3",
                          std::u16string(u"Do you want to file this complaint?\nComplaint:\n") + r.complaint, rows, 0,
                          [g, xuid, category, reason] { GuideComplaintReasons(g, xuid, category, reason); }, true);
    }

    // Step 2: the specific reason.
    void GuideComplaintReasons(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t category, uint32_t focus)
    {
        const FeedbackCategory& c = ComplaintReasons()[category];
        std::vector<FeedbackRow> rows;
        for (uint32_t i = 0; i < c.reasons.size(); i++)
            rows.push_back({ c.reasons[i].name, "", c.reasons[i].description,
                             [g, xuid, category, i] { GuideComplaintConfirm(g, xuid, category, i); } });
        GuideFeedbackPage(g, xuid, u"File Complaint: Step 2 of 3", u"Select the specific reason for filing a complaint.",
                          rows, focus, [g, xuid, category] { GuideComplaintSteps(g, xuid, category); }, true);
    }

    // Step 1: the general reason.
    void GuideComplaintSteps(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t focus)
    {
        std::vector<FeedbackRow> rows;
        for (uint32_t i = 0; i < ComplaintReasons().size(); i++)
            rows.push_back({ ComplaintReasons()[i].name, "", ComplaintReasons()[i].description,
                             [g, xuid, i] { GuideComplaintReasons(g, xuid, i, 0); } });
        GuideFeedbackPage(g, xuid, u"File Complaint: Step 1 of 3", u"Select the general reason for filing a complaint.",
                          rows, focus, [g, xuid] { GuideComplaint(g, xuid, 1); }, true);
    }

    void GuideComplaint(const std::shared_ptr<GuideSession>& g, uint64_t xuid, uint32_t focus)
    {
        std::vector<FeedbackRow> rows = {
            { u"More Information", "builtin:GavelQuestion",
              u"View more information about filing a complaint. File a complaint only if the player has violated the "
              u"Xbox Live Code of Conduct.",
              [g] { GuideFeedbackText(g, u"More Information", kCodeOfConduct); } },
            { u"File Complaint", "builtin:Gavel",
              u"File a complaint against this player.\n\nIf the player did not violate the Code of Conduct, consider "
              u"a player review.",
              [g, xuid] { GuideComplaintSteps(g, xuid, 0); } },
            { u"Don't File", "builtin:NoEntry", u"Don't file a complaint against this player.",
              [g, xuid] { GuideFeedbackDone(g, xuid); } } };
        GuideFeedbackPage(g, xuid, u"File a Complaint",
                          u"All complaints are reviewed by Xbox Live. However, Xbox Live policy does not permit "
                          u"disclosure of the status or results of individual complaints.",
                          rows, focus, [g, xuid] { GuideFeedbackDone(g, xuid); }, true);
    }

    // An achievement's details (AchievementDetailsScene): its description once either player has it (or it is not
    // secret: what to do to unlock it), else XAM's secret text.
    void GuideCompareAchievementDetails(const std::shared_ptr<GuideSession>& g)
    {
        if (g->compareAchievementFocus >= g->compareAchievements.size() || g->compareFocus >= g->compareTitles.size())
            return;
        const GuideSession::CompareAchievement& a = g->compareAchievements[g->compareAchievementFocus];
        const bool earned = a.theirs || a.mine;
        const bool hidden = !earned && !a.shown;
        g->mode = GuideSession::Mode::kAchievementDetails;
        xui::GuideKeepPosition();
        xui::ShowAchievementDetails(g->compareTitles[g->compareFocus].name, hidden ? u"Secret Achievement" : a.name,
                                    hidden ? u"Continue playing to unlock this secret achievement." :
                                    earned ? a.description : a.unachieved,
                                    a.gamerscore, a.image, earned, a.shown);
    }

    void GuideAchievements(const std::shared_ptr<GuideSession>& g, uint32_t focus)
    {
        // AchievementsNavButton (0x913E5F48): a partial profile gets this title's achievements; a full one the
        // dashboard's games list first ("guide achievements.png": the titles played, the most recent first, with
        // their tile, "93 of 93 Achievements" and the gamerscore earned; A a title's achievements, B back).
        if (!host::SignedIn(g->user))
            return;
        const std::vector<host::PlayedTitle> titles = host::PlayedTitles(g->user);
        g->achievementsFromGames = false;
        if (titles.empty())
        {
            const std::vector<xui::AchievementEntry> entries = host::TitleAchievements(g->user);
            g->mode = GuideSession::Mode::kAchievements;
            xui::GuideKeepPosition();
            xui::ShowAchievements(host::TitleName(), entries);
            return;
        }
        g->mode = GuideSession::Mode::kGames;
        g->rowActions.clear();
        std::vector<xui::CommunityRow> rows;
        for (size_t i = 0; i < titles.size(); i++)
        {
            const host::PlayedTitle& t = titles[i];
            xui::CommunityRow row;
            row.line1 = t.name;
            row.line2 = Wide(std::format("{} of {} Achievements", t.earned, t.achievements));
            row.right = Wide(std::to_string(t.gamerscore)) + u" \uE00A"; // the gamerscore glyph
            row.picture = t.tile;
            rows.push_back(std::move(row));
            g->rowActions.push_back([g, i, id = t.titleId, name = t.name] {
                g->gamesFocus = uint32_t(i);
                g->achievementsFromGames = true;
                g->mode = GuideSession::Mode::kAchievements;
                xui::GuideKeepPosition();
                xui::ShowAchievements(name, host::PlayedTitleAchievements(g->user, id));
            });
        }
        xui::CommunityOptions options;
        options.rightAligned = true;
        xui::GuideKeepPosition();
        xui::ShowCommunity(u"Achievements", {}, 0, rows, std::min<uint32_t>(focus, uint32_t(rows.size() - 1)), u"",
                           options);
    }

    // Opens the Guide as the runtime's system UI for `user`: its main blades, or (a title's XamShow*UI) the screen
    // `start` shows, where Back closes it. `fromButton`: opened by the Guide button (only for the log).
    void OpenGuide(uint32_t user, std::function<void(const std::shared_ptr<GuideSession>&)> start, bool fromButton)
    {
        if (!xui::SystemUiAvailable())
        {
            Log("System UI: Guide button: the Xbox Guide needs the console's system UI assets (system_ui/)");
            return;
        }
        auto session = std::make_shared<GuideSession>();
        session->user = user;
        std::string busy;
        session->ui = host::OpenSystemUi(user, [session](SystemUi& ui, uint32_t u, uint16_t pressed) {
            std::lock_guard lock(session->mutex);
            return GuideInput(ui, session, u, pressed);
        }, busy);
        if (!session->ui)
        {
            if (fromButton)
                Log("System UI: Guide button on controller {} ignored while the {} is open", user + 1, busy);
            else
                Log("System UI: Guide button on controller {}: another system UI is open", user + 1);
            return;
        }
        {
            std::lock_guard lock(g_mutex);
            g_current = session;
        }
        if (start)
        {
            session->direct = true;
            start(session);
            return;
        }
        xui::ShowGuide(GuideInfoFor(user));
    }

    // A title's XamShow*UI for one of the Guide's Community screens: ERROR_SUCCESS once open (XN_SYS_UI 1 / 0 as for
    // the Guide); a user who is not on Xbox LIVE gets nothing (XAM 0x816C1718 needs sign-in state 2).
    uint32_t ShowCommunityUI(uint32_t user, std::function<void(const std::shared_ptr<GuideSession>&)> start)
    {
        if (user >= 4 || !host::SignedIn(user))
            return 0x525; // ERROR_NO_SUCH_USER
        if (!host::OnLive(user))
        {
            Log("System UI: user {} is not on Xbox LIVE: no Community UI", user);
            return 0;
        }
        OpenGuide(user, std::move(start), false);
        return 0;
    }
}

void guide::GuideButton(uint32_t user)
{
    std::shared_ptr<GuideSession> open;
    {
        std::lock_guard lock(g_mutex);
        open = g_current.lock();
    }
    if (!open || open->ui->Closed())
        return OpenGuide(user, {}, true);
    if (open->user == user)
    {
        if (open->ui->Close("the Guide button", user))
            host::EndSystemUiInput();
        return;
    }
    Log("System UI: Guide button on controller {} ignored while the {} is open", user + 1, "Xbox Guide");
}

uint32_t guide::ShowCommunity(uint32_t user, Tab tab)
{
    return ShowCommunityUI(user, [tab](const std::shared_ptr<GuideSession>& g) { GuideCommunity(g, tab); });
}

uint32_t guide::ShowFriendRequest(uint32_t user, uint64_t xuid)
{
    return ShowCommunityUI(user, [xuid](const std::shared_ptr<GuideSession>& g) {
        if (xuid == 0)
        {
            GuideCommunity(g, Tab::kFriends);
            if (!g->rowActions.empty())
                g->rowActions.front()(); // Add Friend
            return;
        }
        const auto profile = live::GetProfile(g->user, xuid);
        const std::string tag = profile ? profile->gamertag : std::string();
        GuideCommunityBox(g, u"Send Friend Request", u"Send a friend request to " + Wide(tag) + u"?",
                          { { u"Send Friend Request", [g, tag] {
                                GuideCommunityResult(g, u"Send Friend Request", live::AddFriend(g->user, tag),
                                                     u"A friend request was sent to " + Wide(tag) + u".");
                            } },
                            { u"Cancel", nullptr } });
    });
}

uint32_t guide::ShowGamerCard(uint32_t user, uint64_t xuid)
{
    if (const auto profile = user < 4 ? host::SignedIn(user) : std::nullopt;
        profile && (xuid == 0 || xuid == profile->xuid || xuid == profile->onlineXuid))
        return ShowOwnProfile(user);
    return ShowCommunityUI(user, [xuid](const std::shared_ptr<GuideSession>& g) { GuideProfile(g, xuid, 0); });
}

uint32_t guide::ShowOwnProfile(uint32_t user)
{
    if (user >= 4 || !host::SignedIn(user))
        return 0x525; // ERROR_NO_SUCH_USER
    OpenGuide(user, [](const std::shared_ptr<GuideSession>& g) { GuideOwnProfile(g); }, false);
    return 0;
}

uint32_t guide::ShowGames(uint32_t user, uint64_t xuid)
{
    const auto profile = user < 4 ? host::SignedIn(user) : std::nullopt;
    if (!profile)
        return 0x525; // ERROR_NO_SUCH_USER
    if (xuid == 0 || xuid == profile->xuid || xuid == profile->onlineXuid)
    {
        OpenGuide(user, [](const std::shared_ptr<GuideSession>& g) { GuideAchievements(g); }, false);
        return 0;
    }
    return ShowCommunityUI(user, [xuid](const std::shared_ptr<GuideSession>& g) {
        g->compareDirect = true;
        GuideCompareGames(g, xuid, 0);
    });
}

uint32_t guide::ShowProfileCreated(uint32_t user)
{
    if (user >= 4 || !host::SignedIn(user))
        return 0x525; // ERROR_NO_SUCH_USER
    std::shared_ptr<GuideSession> opened;
    OpenGuide(user, [&opened](const std::shared_ptr<GuideSession>& g) {
        opened = g;
        GuideProfileCreated(g, 0);
    }, false);
    return opened ? 0 : 5;
}

uint32_t guide::ShowFriendsOf(uint32_t user, uint64_t xuid, const std::string& gamertag)
{
    return ShowCommunityUI(user, [xuid, gamertag](const std::shared_ptr<GuideSession>& g) {
        g->fofDirect = true;
        std::string tag = gamertag;
        if (tag.empty())
            if (const auto profile = live::GetProfile(g->user, xuid))
                tag = profile->gamertag;
        GuideFriendsOf(g, xuid, tag);
    });
}

uint32_t guide::ShowJoinSessionInProgress(uint32_t user, uint64_t xuid)
{
    return ShowCommunityUI(user, [xuid](const std::shared_ptr<GuideSession>& g) {
        // Our Xbox LIVE has no joinable game sessions: the console's answer when the session cannot be joined
        // (Guide.Friends strings 115 "Join Session in Progress", 71 "Couldn't join the session in progress.").
        Log("System UI: Guide: join session in progress of {:016X}: no joinable session", xuid);
        g->back = [g] { GuideBack(g); };
        GuideCommunityBox(g, xui::FriendsString(115, u"Join Session in Progress"),
                          xui::FriendsString(71, u"Couldn't join the session in progress."), { { u"OK", nullptr } });
    });
}

uint32_t guide::ShowPreferences(uint32_t user)
{
    if (user >= 4 || !host::SignedIn(user))
        return 0x525; // ERROR_NO_SUCH_USER
    OpenGuide(user, [](const std::shared_ptr<GuideSession>& g) { GuidePreferences(g, 0, 0); }, false);
    return 0;
}

uint32_t guide::ShowFeedback(uint32_t user, uint64_t xuid, bool complaint)
{
    return ShowCommunityUI(user, [xuid, complaint](const std::shared_ptr<GuideSession>& g) {
        g->feedbackDirect = true;
        if (complaint)
            GuideComplaint(g, xuid, 0);
        else
            GuidePlayerReview(g, xuid, 0);
    });
}

uint32_t guide::ShowMessageCompose(uint32_t user, const std::vector<uint64_t>& recipients)
{
    return ShowCommunityUI(user, [recipients](const std::shared_ptr<GuideSession>& g) {
        if (recipients.size() == 1)
        {
            const auto profile = live::GetProfile(g->user, recipients.front());
            GuideSendMessage(g, recipients.front(), profile ? profile->gamertag : std::string());
            return;
        }
        const uint32_t user = g->user;
        GuideKeyboard(g, u"Send Message", u"Type your message.", 250, [g, user, recipients](const std::u16string& text) {
            GuideCommunityResult(g, u"Send Message", live::SendLiveMessage(user, recipients, Utf8(text)),
                                 u"Your message was sent.");
        });
    });
}
