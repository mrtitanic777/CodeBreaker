#pragma once

// The console's system UI drawn from XAM 17559's own XUI assets (assets.h): the Guide's popups that a title opens
// with XamShow*UI and the notification toasts, composited over the title's frame like XAM's HUD.
//
// kernel/xam_ui.cpp keeps the logic (answers, notifications, input) and tells this module what to show; the renderer
// asks for the overlay at every present. The HUD is laid out in XAM's 852x480 design space, scaled by the output
// height and centred (1280x720: x1.5, 1 pixel left margin), and drawn into an overlay the size of the title's front
// buffer that is composited over it before the display gamma, as XAM draws into the front buffer. A popup darkens
// the title's frame (a black layer, alpha 0xDC, measured on the owner's console captures) and plays hudbkgnd.xur's
// state animations (ClosedToError / ErrorToClosed for message boxes; for the Guide's panes Half or Full by the pane
// scene's OpenType). A pane (the Guide, sign-in, keyboard, achievements) is a HUDScene hosted in hudbkgnd's
// AppHostElementId, with hudbkgnd's legend packed from its LegendA..Y, the clock and the user's gamer picture
// (local profiles made here have none). Animations advance with the emulated vblank (60 per second), on every title swap even when nothing is
// presented. Toasts (notify.xur) are drawn 1:1 into the same 852x480 HUD space (XAM renders its HUD into an 852x480
// target that is scaled onto the frame).
//
// Without the assets (system_ui/ missing) Available() is false and xam_ui.cpp falls back to native dialogs.

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace xui
{
    bool SystemUiAvailable(); // loads the assets on first use
    // Loads the assets, the HUD's scenes and every image of XAM's packages on a background thread at start-up, so the
    // first popup or Guide finds everything parsed and decoded.
    void PreloadSystemUi();

    enum class MessageIcon { kNone = 0, kError = 1, kWarning = 2, kAlert = 3, kInformation = 4 }; // XMB_*ICON; XAM's 4

    // A row of the sign-in pane (signin.xex's user list): gamertag, "Xbox Live" for a LIVE profile, the storage
    // device ("HDD"), the gamer picture (PNG, empty if the profile has none).
    struct SignInEntry
    {
        std::u16string gamertag;
        std::u16string status;
        std::u16string storage;
        std::vector<uint8_t> picture;
        // signin.xex's "Guest" row (XSSUI_FLAGS_ENABLE_GUEST): its string #16 as the name, no storage line.
        bool guest = false;
        // A profile listed but not choosable (greyed: already signed in, XSSUI_FLAGS_ADDUSER; signin.xex 0x90118BF8).
        bool enabled = true;
    };

    // An achievement of the Guide's achievement list (gamerprofile.xex's 802_Achievements.xur).
    struct AchievementEntry
    {
        std::u16string label, description, unachieved;
        uint32_t gamerscore = 0;
        bool earned = false;
        bool showUnachieved = false; // XACHIEVEMENT_DETAILS_SHOWUNACHIEVED: not a secret one
        uint64_t unlockTime = 0;     // FILETIME (UTC) when earned
        std::vector<uint8_t> image;  // the SPA's PNG tile
    };

    // A row of the Guide's Community lists (friends, messages, party): two lines with an icon, and a right column.
    // `icon` names one of XAM's images instead of a picture ("ico_32x_Friend.png" in xam.shrdres, ...).
    // `right` starts beside `line1` (a second line after '\n'); `status` names an image drawn before it, tinted by
    // `statusColor` (the console's online / offline person: "sharedres://ico_32x_Friend.png" green / orange).
    struct CommunityRow
    {
        std::u16string line1, line2, right;
        std::vector<uint8_t> picture; // PNG (a gamer picture), empty for none
        std::string icon, iconPackage;
        std::string status;
        uint32_t statusColor = 0xFFFFFFFF;
        // One-line rows only (CommunityLayout::kOneLine): `enabled` false greys the row (the skin's Disable states).
        // A `member` row is a party member (owner's capture "in a party.png"): its `picture` before line1, the
        // tinted `status` image in the middle (the controller), `right` the title column, `leader` the party
        // leader's mark at the right end. A row with a `value` is a setting (Party Options, "party settings *.png"):
        // line1 keeps its colours on the focused row, the value is centred in the right part between < > there.
        bool enabled = true;
        bool member = false, leader = false;
        std::u16string value;
        // The legend's X while this row has the focus (empty: the screen's; Friends: "Accept Friend Request" on a
        // request, capture "guide friends list.png").
        std::u16string legendX;
        // The real Community scene (Guide.Friends.xex's community.xur, CommunitySceneAvailable): what the row's
        // presenters show, by DataAssociation, as the module's list classes answer XM_GET_SOURCE_TEXT / _IMAGE
        // (NOTES session 20): texts, images by locator (relative to the module's package: "friend_online.png"), and
        // the slot `picture` (the gamer picture) goes to (-1: none). A row of a tab's buttons (sceneButtons) uses
        // slot 0's text only.
        std::map<int, std::u16string> slotText;
        std::map<int, std::string> slotImage;
        int pictureSlot = -1;
        // An image slot showing a skin visual at one frame of its timelines (PlayersList's slot 1: the "Reputation"
        // stars, frame 0 none ... 20 five).
        struct SlotVisual
        {
            std::string visual;
            int frame = 0;
        };
        std::map<int, SlotVisual> slotVisual;
        // The item's check state (MessageList's XM_GET_ITEMCHECK: the message was read).
        bool checked = false;
    };
    // A tab of the Community screen's tab strip: its icon ("sharedres://ico_32x_Mail.png") and its text (the focused
    // tab's "Friends (1 Online)", the others' count "0").
    struct CommunityTab
    {
        std::string icon;
        std::u16string text;
        // The real scene's tab strip: the long label ("Friends (2 Online)") and the number, both set on every tab
        // (the scene's timelines show one or the other).
        std::u16string label, count;
    };

    // Input for an open list pane (sign-in, achievements, community): moves the focus; A answers with the focused row, B / BACK
    // cancels.
    struct PaneAction
    {
        enum Kind { kNone, kChosen, kCancelled } kind = kNone;
        uint32_t index = 0;
    };
    PaneAction PanePress(uint16_t pressed);
    // The open pane's focused row (a list's item, else a button).
    uint32_t PaneFocus();

    // Popups (one at a time). `user` = the controller's user index the popup follows (0xFF any).
    // A pass code box (XMB_PASSCODEMODE / XMB_VERIFYPASSCODEMODE: the skin's PasscodeMessageBox): the title, the
    // text, four dots that fill as buttons are pressed and the controller's pass code buttons under them; B Back.
    // SetPasscodeEntered fills `count` dots and, `text` not empty, replaces the text ("Pass code incorrect.").
    void ShowPasscodeBox(const std::u16string& title, const std::u16string& text, MessageIcon icon);
    // A scene of controls (hud.xex's Options scenes): `controls` by id in focus order (up / down, A: PanePress's
    // kChosen with the index), each a button, check box / radio button (`checked`) or slider (`value`, its RangeMax
    // steps); `hidden` lists elements to hide. UpdateSceneControl redraws one after a change.
    struct SceneControl
    {
        std::string id;
        bool checked = false;
        int value = 0;
        bool enabled = true;
    };
    void ShowControlScene(const std::string& package, const std::string& scene, const std::vector<SceneControl>& controls,
                          const std::vector<std::string>& hidden, uint32_t focus);
    void UpdateSceneControl(uint32_t index, const SceneControl& control);
    // Guide.Voicemail.xex's Voice Mail (VoiceMail.xur, its VoiceMailScene): the Record, Stop and Play buttons
    // (scnButtons) and Done (btnAccept), focus moving left / right over the enabled ones (PanePress: kChosen with
    // 0 Record, 1 Stop, 2 Play, 3 Done; B Cancel), the progress bar (0-100), txtInfo and txtWarning (empty: hidden).
    // ShowVoiceMail opens it; UpdateVoiceMail changes what shows while it is open. VoiceMailString: its Strings.xus.
    struct VoiceMailView
    {
        bool record = false, stop = false, play = false;
        uint32_t focus = 0;
        int progress = 0;
        std::u16string info, warning;
    };
    void ShowVoiceMail(const VoiceMailView& view);
    void UpdateVoiceMail(const VoiceMailView& view);
    std::u16string VoiceMailString(size_t index, const char16_t* fallback);

    // Guide.Beacons (its RootScene / TabBaseScene / TitlePickerScene / GameDetails / SetBeacon scenes, drawn with
    // its own Skin.xur). A row of its lists: the control's text (the game or the gamertag), the data slots 1-3
    // (btn_Beacons_JumpInList: 1 txtSubText, 2 txtHeatCount2, 3 txtHeatCount1; btn_Beacons_FriendList: 1
    // txtBeaconText, 2 txtLastPlayed; btn_Beacons_TitlePicker: 1 txtPlatformType), the row's picture (a game's tile or
    // a gamer picture, PNG) or `image` (one of the module's images, "ico_64x_add.png"), the image slots 1 / 2
    // (FriendList's imgBeaconIcon / imgPresenceIcon), and whether it can be chosen.
    struct BeaconRow
    {
        std::u16string text, slot[4];
        std::vector<uint8_t> picture;
        std::string image, slotImage[3];
        bool enabled = true;
    };
    // A tab of Beacons & Activity (TabBaseScene.xur): labelDescription, txtListCount, labelStatus (the empty / error
    // text, shown instead of the list) and its wait cursor while `waiting`.
    struct BeaconTab
    {
        std::u16string description, count, status;
        bool waiting = false;
        std::vector<BeaconRow> rows;
    };
    // RootScene.xur: Tab1 My Beacons, Tab2 My Activity, Tab3 Friends' Activity (XuiTabScene, LB / RB and left /
    // right; the list up / down, A the focused row, B back). UpdateBeaconTab redraws one tab's data (a fetch done).
    void ShowBeacons(const std::vector<BeaconTab>& tabs, int tab, uint32_t focus);
    void UpdateBeaconTab(int tab, const BeaconTab& data);
    struct BeaconsAction
    {
        enum Kind { kNone, kChosen, kCancelled, kTab } kind = kNone;
        int tab = 0;
        uint32_t index = 0;
    };
    BeaconsAction BeaconsPress(uint16_t pressed);
    // TitlePickerScene.xur ("Choose an activity.": the games to set a beacon for) or GameDetails.xur (a game's
    // friends: its tile, name and platform over the list): a list pane (PanePress: kChosen with the row, kCancelled).
    struct BeaconListInfo
    {
        bool details = false; // GameDetails.xur, else TitlePickerScene.xur
        std::u16string title, platform, description, count, status, sort;
        std::vector<uint8_t> tile;
        bool waiting = false;
        std::vector<BeaconRow> rows;
        uint32_t focus = 0;
    };
    void ShowBeaconList(const BeaconListInfo& info);
    // SetBeacon.xur: btnComment (Add / Edit Comment), chkSocialNetwork (greyed: no social networks here), the
    // actions list (lstSetActions: Set Beacon; lstEditActions: Update Beacon, Remove Beacon) and the preview (the
    // game's tile and name, slots 1 platform, 2 / 3 friends recently / online, 4 "Comment", 5 the comment). PanePress:
    // kChosen with 0 the comment button, 2 + n the n-th action; B back.
    struct SetBeaconInfo
    {
        bool edit = false;
        std::u16string comment, share, description, title, platform, recent, online, commentHeader, commentText;
        std::vector<uint8_t> tile;
        std::vector<std::u16string> actions;
        uint32_t focus = 2;
    };
    void ShowSetBeacon(const SetBeaconInfo& info);
    std::u16string BeaconsString(size_t index, const char16_t* fallback);

    // Guide.ChatAndMessenger's Chat (QuickChatRootScene, rootscene.xur: its ChatSessionList "listItems", visual
    // List_twoline-2iconChat / btn_2Line-2iconChat): a row per channel or chat, `line1` (txt_line3, slot 3), `line2`
    // (txt_line2, slot 4), the main icon (one of the module's images), the small icon (slot 1: the active channel's
    // speaker) and the other player's gamer picture (slot 2). A list pane (PanePress: kChosen with the row,
    // kCancelled). UpdateChatList redraws the rows while it shows.
    struct ChatRow
    {
        std::u16string line1, line2;
        std::string icon, smallIcon;
        std::vector<uint8_t> picture;
    };
    void ShowChatList(const std::vector<ChatRow>& rows, uint32_t focus);
    void UpdateChatList(const std::vector<ChatRow>& rows);
    struct GamerCardInfo;
    // QuickChatOptionsScene (VoicechatOptions.xur): Join Chat, End Chat, Page Gamer, View Profile (rows 0-3 as a
    // drill-down: PanePress, greyed ones do nothing) beside the player's gamercard.
    void ShowChatOptions(const GamerCardInfo& card, const bool (&enabled)[4], uint32_t focus);
    std::u16string ChatString(size_t index, const char16_t* fallback);
    void SetPasscodeEntered(uint32_t count, const std::u16string& text);
    void ShowMessageBox(const std::u16string& title, const std::u16string& text,
                        const std::vector<std::u16string>& buttons, uint32_t focus, MessageIcon icon);
    // The one-pane sign-in (signin1.xur + UserListSinglePane.xur): the profiles, then signin.xex's "Create Profile"
    // and "Download Profile" rows (indices entries.size() and entries.size() + 1).
    void ShowSignIn(const std::vector<SignInEntry>& entries, uint32_t focus);
    // The same pane while a profile signs in to Xbox LIVE (signin.xex's status.xur in it): the profile's row and
    // "Signing in" with its wait cursor. No input (a wait).
    void ShowSignInStatus(const SignInEntry& entry);
    // hud.xex's "Join Xbox Live" scene (InfoUpsellLive.xur, the Guide's Join Xbox Live for an offline profile): its
    // one big Join Xbox Live button (row 0: PanePress), B back. Its button calls XamShowLiveSignupUI.
    void ShowJoinLive();
    // hud.xex's StatusScene (Status.xur): `text` over its wait cursor (txtMessage, the skin's txt_Starting), no input.
    // The Guide's Sign Out shows it with XamStrings 156 "Signing out" while the profile signs out (hud 0x913F8448).
    void ShowHudStatus(const std::u16string& text);
    // The Guide's virtual keyboard (vk.xex's scenes and key layouts). Controller buttons go to KeyboardPress, which
    // says when the gamer pressed Done (START or the Done key) or cancelled (B / BACK); KeyboardText is the text.
    // `afterUnloading`: the keyboard comes from another module after the open full scene (createprofile.xex calls
    // XamShowKeyboardUI after XamShowDeviceSelectorUI): the HUD's big panel stays, the open scene goes at once, the
    // panel stays empty while vk.xex loads (107 frames on the owner's console), then the keyboard shows in it. A popup
    // in another HUD state closes first and the keyboard opens with its own animation.
    // `gamertag`: only a gamertag's characters (letters, digits, space; VKBD_LATIN_GAMERTAG): the other keys, Symbols and
    // Accents are greyed and do nothing, the PC keyboard types only those characters.
    void ShowKeyboard(const std::u16string& title, const std::u16string& description, const std::u16string& text,
                      size_t maxLength, bool afterUnloading = false, bool gamertag = false);
    enum class KeyboardAction { kNone, kDone, kCancel };
    KeyboardAction KeyboardPress(uint16_t pressed); // XINPUT_GAMEPAD buttons newly pressed (input.h's UI bits)
    std::u16string KeyboardText();
    // A PC keyboard typing into the open virtual keyboard: a character at the cursor, '\b' deletes the one before
    // it (the Backspace key). KeyboardOpen says whether typing goes there (the input layer then keeps those keys
    // from acting as controller buttons; Enter is the keyboard's Done, START).
    bool KeyboardOpen();
    void KeyboardTypeChar(char16_t c);
    void ShowAchievements(const std::u16string& titleName, const std::vector<AchievementEntry>& achievements);
    // XShowDeviceSelectorUI's "Select a Device" (deviceselector.xex's DeviceSelector.xur, its List_3Line_Icon_prgbr
    // rows): per device its name, its kind's icon, a status line (`status`, else DeviceSelectorFree of `freeBytes`:
    // "<size> free") and the used share of its space as a bar ((total - free) / total; no bar without `totalBytes`).
    // Disabled rows (too little space) are greyed.
    struct DeviceEntry
    {
        std::u16string name;
        uint32_t kind = 1;        // XDEVICE_DATA type: 1 hard drive, 2 memory unit, 8 transfer cable, 0x10 USB
        uint64_t freeBytes = 0;
        uint64_t totalBytes = 0;
        std::u16string status;    // e.g. "Offline" (empty: the free space)
        bool enabled = true;
    };
    void ShowDeviceSelector(const std::vector<DeviceEntry>& devices, uint32_t focus);
    // deviceselector.xex's text for `bytes` free ("2.2 GB free": strings 5 and 31, units 25-28) / one of its strings.
    std::u16string DeviceSelectorFree(uint64_t bytes);
    std::u16string DeviceSelectorString(size_t index, const char16_t* fallback);
    // Create Profile's scenes (createprofile.xex): its wait scene (AddXboxLive.xur: the wait cursor over `status`,
    // "Creating profile", "Signing in", "Updating profile", "Checking network status"; its back button hidden; called
    // again while it shows, only the text changes) and Save Profile (Congratulations.xur: the new profile's gamercard,
    // the buttons Join Xbox Live / Customize Profile / Done (rows 0-2: SetFocus / PanePress as a drill-down), the
    // focused button's text beside them). CreateProfileString: its CreateProfile_Custom.xus.
    void ShowProfileWait(const std::u16string& status);
    void ShowSaveProfile(const std::u16string& gamertag, const std::vector<uint8_t>& picture, uint32_t focus);

    // The Guide's Games & Apps > Recent: hud.xex's QuickLaunch.xur (QuickLaunchMainScene; XuiTabScene Tab1 Games &
    // Apps = QLRecentlyPlayedList, Tab2 Downloads = QLRecentlyDownloadedList, Tab3 All = QLAllGamesList). A row:
    // `name`, Tab1's second line and right column (`line2` "x of y Achievements", `right` the gamerscore), Tab2's
    // date (`right`), the tile / thumbnail (PNG). `trial` rows leave the All tab while Y "Hide Trial Games" is on.
    struct QuickLaunchRow
    {
        std::u16string name, line2, right;
        std::vector<uint8_t> picture;
        bool trial = false;
    };
    struct QuickLaunchInfo
    {
        bool signedIn = false; // Tab1 says "Sign in to see your recent items." without a profile
        std::vector<QuickLaunchRow> played, downloads, all;
    };
    // Opens it on Tab1 (always, 0x913F7DF0).
    void ShowQuickLaunch(const QuickLaunchInfo& info);
    // Input while it shows: RB / RT and LB / LT change tabs (XuiTabScene, no wrap), up / down the list, A launches the
    // focused row (kLaunch: tab 0-2 and the row's index in that tab's rows as given), Y on All toggles Hide / Show
    // Trial Games, B / BACK closes (kCancelled).
    struct QuickLaunchAction
    {
        enum Kind { kNone, kLaunch, kCancelled } kind = kNone;
        uint32_t tab = 0, index = 0;
    };
    QuickLaunchAction QuickLaunchPress(uint16_t pressed);
    std::u16string CreateProfileString(size_t index, const char16_t* fallback);
    // Compare Games (gamerprofile.xex's 825_GameShowcaseCompare.xur, the Guide's Profile row / XamShowGamesUI with
    // another player): the header's two gamer pictures and total gamerscores (the other player's first, "you" in
    // the scene's names), then a row per title either has played: its tile, name, the other player's gamerscore in it
    // and yours (empty: not played). A / B through PanePress.
    struct CompareGameRow
    {
        std::u16string name, theirs, mine;
        std::vector<uint8_t> tile;
    };
    void ShowCompareGames(const std::vector<uint8_t>& theirPicture, uint32_t theirGamerscore,
                          const std::vector<uint8_t>& myPicture, uint32_t myGamerscore,
                          const std::vector<CompareGameRow>& rows, uint32_t focus);
    // A title's achievements compared (826-7_GameAchievStatsYou.xur, A on a Compare Games row): the title, both
    // pictures and the gamerscore each earned in it, then a row per achievement: its name and, for each player, the
    // achievement's image (its tile when earned, else XAM's unearned / secret image) and the gamerscore earned.
    struct CompareAchievementRow
    {
        std::u16string name;
        bool theirs = false, mine = false; // earned
        bool shown = true;                 // not a secret one (its name hidden while neither has it)
        uint32_t gamerscore = 0;
        std::vector<uint8_t> image;        // the SPA's tile when known (from my profile)
    };
    void ShowCompareAchievements(const std::u16string& titleName, const std::vector<uint8_t>& theirPicture,
                                 uint32_t theirGamerscore, const std::vector<uint8_t>& myPicture, uint32_t myGamerscore,
                                 const std::vector<CompareAchievementRow>& rows, uint32_t focus);
    // An achievement's details (828_AchievDetails.xur): the title, its name, image, gamerscore and description. B
    // through PanePress.
    void ShowAchievementDetails(const std::u16string& titleName, const std::u16string& name,
                                const std::u16string& description, uint32_t gamerscore,
                                const std::vector<uint8_t>& image, bool earned, bool shown);
    // The Guide's Community screen (the console's is a LIVE-delivered module, Guide.Friends.xex; drawn here in
    // gamerprofile.xex's full games-list pane, 820_GameShowcaseMe.xur, whose rows have the same parts): `title` in the
    // header ("Community"), the tab strip (`tabs`, `activeTab` drawn wide and light, as hud's QuickLaunch.xur draws
    // its tabs with the skin's tab_activeGS1 / tab_inactiveGS2; none: the list starts where they would be, as Party
    // Options does), the rows, `message` centred in the list area (empty: none), `footer` at its bottom left ("Sorted
    // by online status") and the legend's X / Y texts (empty: none). Layout measured on the owner's captures 7_47_01
    // / 7_50_12, partychat.mp4 and "party chat/*.png". A / B through PanePress.
    // `layout`: kTwoLine rows (46 high: picture or icon, line1 over line2, the right column and status person; the
    // Friends and Players tabs), or kOneLine rows (28 high, XAM's skin btn_oneline_3strings: line1, line2 and right in
    // three columns, no picture or status; the Party and Messages tabs, as captures 7_49_59 / 7_50_04 and
    // partychat.mp4 show their "Learn More" / "Create New" / "Join Party" rows).
    // An `icon` may also be "builtin:AddFriend" / "builtin:Pencil" / "builtin:SendMail" (IconImage). One-line rows
    // with a `status` image and no `member`: a message (owner's "guide messages.png"): the sender, the envelope, the
    // text.
    enum class CommunityLayout { kTwoLine, kOneLine };
    struct CommunityOptions
    {
        std::u16string footer, legendX, legendY;
        CommunityLayout layout = CommunityLayout::kTwoLine;
        // "n of N" at the footer's right while the list is longer than its page: the rows from `counterFrom` on are
        // counted (Friends: all but Add Friend, capture "guide friends list.png"); -1: none.
        int counterFrom = -1;
        // Two-line rows: the right column right-aligned at the row's end and centred on it (the Guide's games list,
        // "guide achievements.png": the gamerscore), else left-aligned from x 284 level with line 1.
        bool rightAligned = false;
        // The real scene's Party tab: the first `sceneButtons` rows are PartyScene's buttons (btnInvite,
        // btnInviteParty, btnSettings in order; one: btnInvite alone, "Start a Party"), the rest the member list;
        // `message` is its text_PartyDescription.
        int sceneButtons = 0;
    };
    // The Community's drill-downs as Guide.Friends.xex draws them (its package in system_ui; false: not there, nothing
    // shown, the caller draws its own). Controller input through PanePress / PaneFocus.
    // Party Options (PartySettings.xur): rows 0 Chat Channel (spinner), 1 Party Status (the leader's spinner, else a
    // read-only button with the status), 2 Leave Party (its Quickplay is hidden, as on the 17559 console).
    bool ShowPartySettings(const std::u16string& chatChannel, const std::u16string& partyStatus, bool leader,
                           uint32_t focus);
    // Messages > Create New (MessageType.xur): the scene's five types (Message, Private Chat Invite, Party Invite,
    // Game Invite, Friend Request) with their description in the right panel; greyed ones take the focus only.
    struct MessageTypeEntry
    {
        bool enabled = true;
        std::u16string description;
    };
    bool ShowMessageCreate(const std::vector<MessageTypeEntry>& entries, uint32_t focus);
    // A friend's friends (FofScene.xur: "%s's Friends", the rows as the Friends tab's (slots), label_Total "n of N",
    // "No friends available." when empty; owner's 7_47_34). A / B through PanePress.
    bool ShowFriendsOf(const std::u16string& heading, const std::vector<CommunityRow>& rows, uint32_t focus);
    // A player's bio (Bio.xur: "%s's bio" over the text; owner's 7_47_21). B through PanePress.
    bool ShowBio(const std::u16string& heading, const std::u16string& bio);
    // Whether the Guide's real Community scene is there (system_ui/guide.friends.friends.xzp, from the console's
    // SystemAux: extract_system_ui.py's third argument). Without it ShowCommunity draws its own version.
    bool CommunitySceneAvailable();
    // While the real Community scene shows tab `tab`: list row `row` (its index among the rows ShowCommunity was given,
    // not one of the Party tab's buttons) shows `path` in image slot `slot` (empty: none), redrawn in place (the party
    // members' voice icons as they talk). False when that scene or tab is not showing.
    bool SetCommunityRowImage(uint32_t tab, uint32_t row, int slot, const std::string& path);
    // Guide.Friends.xex's strings.xus entry, or `fallback`.
    std::u16string FriendsString(size_t index, const char16_t* fallback);
    void ShowCommunity(const std::u16string& title, const std::vector<CommunityTab>& tabs, uint32_t activeTab,
                       const std::vector<CommunityRow>& rows, uint32_t focus, const std::u16string& message,
                       const CommunityOptions& options);
    // A player's gamercard as XAM's gamercard.xur (xam.gamercrd) shows it: the base card (picture, gamertag, rep,
    // gamerscore, zone) and the extended panel under it (motto, title, rich presence, online status).
    struct GamerCardInfo
    {
        std::u16string gamertag;
        std::vector<uint8_t> picture; // the 64x64 gamer picture (PNG), empty: none
        uint32_t gamerscore = 0;
        uint32_t zone = 0;            // XPROFILE_GAMERCARD_ZONE: 0 none, 1 Recreation, 2 Pro, 3 Family, 4 Underground
        float reputation = 0;         // stars, 0-5
        bool online = false;
        int64_t offlineSeconds = -1;  // offline: how long ago they were last online (-1: unknown)
        std::u16string motto, title, presence;
        std::u16string name, location, bio; // the Personal Profile (onlineTextGroup under the base card)
        int tenure = 0;                     // years on Xbox LIVE (the orange header number); 0 = none shown
        std::vector<uint8_t> tenureBadge;   // the tenure number image (server content tenure/crown<N>.png), empty if none
    };
    struct ProfileRow
    {
        std::u16string text;
        bool enabled = true;          // greyed rows take the focus but do nothing (the console's "Invite to Game")
    };
    // What the right panel shows under the base card: the extended panel (motto, title, rich presence, status) or the
    // Personal Profile (onlineTextGroup: motto, name, location, bio).
    enum class ProfilePanel { kExtended, kPersonal };
    // A Profile screen: gamerprofile.xex's GamerCardScene.xur (`header`, empty: the scene's "Profile"; the right
    // panel with its XuiGamerCard) with the menu rows on the left, under an optional tab strip (`tabs`, e.g. Gamer
    // Profile / Personal Profile). Used for another player's Profile (the Guide's friend profile, owner's captures
    // 7_47_09 / 7_47_50; the console's is Guide.Friends.xex, not in the system update) and for the signed-in
    // player's own Profile and Edit Profile (gamerprofile.xex's, 7_42_05 / 7_42_23: no tabs, the Personal Profile
    // panel). A / B / up / down through PanePress.
    // A page on the Profile screen's frame (owner's "guide message.png", "guide send message.png"): `info` lines over
    // the rows (a message: "Message Received", date, time, expiry); the right panel's gamercard (`card` false: the
    // composer, text only), "To:" with `to`, and an envelope with `textTitle` over `text` (the message); `rowIcons`:
    // 32-unit icons before the rows (IconImage names), which are then 46 high; `bottomRow`: that row at the bottom
    // of the menu (Send Message).
    struct ProfilePage
    {
        std::u16string info;
        bool card = true;
        std::u16string to, textTitle, text;
        std::vector<std::string> rowIcons;
        int bottomRow = -1;
        std::u16string legendB; // the legend's B (empty: the scene's "Back"; the composer: "Cancel")
        // Guide.Friends' composer / message scenes: the message type's icon by MessageTitle (empty: the envelope).
        std::string icon;
        // The feedback pages (Submit Player Review, File a Complaint; owner's captures 3_16_48 - 3_24_39): the rows
        // stacked at the bottom of the menu, the right panel's text under the card following the focused row
        // (`rowDescriptions`), and the legend's X (Code of Conduct).
        bool rowsAtBottom = false;
        std::vector<std::u16string> rowDescriptions;
        std::u16string legendX;
    };
    void ShowGamerProfile(const std::vector<std::u16string>& tabs, uint32_t activeTab,
                          const std::vector<ProfileRow>& rows, uint32_t focus, const GamerCardInfo& card,
                          ProfilePanel panel, const std::u16string& header = u"", const ProfilePage& page = {});
    // Gamer Zone (gamerprofile.xex's SelectGamerzone.xur, owner's capture 7_42_50): the card (its zone = the saved
    // one), the four zones in the console's order (Recreation, Pro, Family, Underground = XPROFILE_GAMERCARD_ZONE 1-4)
    // with xam.shrdres' ico_32x_zone_* icons in zoneList, the focused zone's description (LiveProfile.xus) in the right
    // panel. PanePress answers the chosen row (0-3 = zones 1-4) or B; Y (More Info) is the caller's, which shows
    // ShowGamerZoneInfo (GamerZoneInfo.xur: the matchmaking text; B through PanePress).
    void ShowGamerZone(const GamerCardInfo& card, uint32_t focus);
    void ShowGamerZoneInfo();
    // The composer's recipients (MessageRecipients.xur, "Enter Gamertags"): row 0 Enter Gamertag, then the friends
    // with a check mark on the chosen ones; the focused friend's gamercard on the right. Y is the legend's Enter
    // Gamertag. A / B / Y through PanePress / the caller.
    struct RecipientEntry
    {
        std::u16string gamertag;
        bool checked = false;
        GamerCardInfo card;
    };
    bool ShowMessageRecipients(const std::vector<RecipientEntry>& entries, uint32_t focus);
    // gamerprofile.xex's strings (GamerProfile_Custom.xus: Motto, "(No motto)", the edit prompts ...).
    std::u16string GamerProfileString(size_t index, const char16_t* fallback);
    void SetFocus(uint32_t index);   // focused button / list entry
    void Press(uint32_t index);      // the pressed button's Press state
    // Closing animation, then nothing; `closed` runs (outside the system UI's lock, on the thread advancing the
    // overlay) once the popup has gone, at once if nothing is animating.
    void ClosePopup(std::function<void()> closed = {});
    bool PopupShowing();             // false while a popup waits for the previous one's closing animation
    // The gamer picture in the HUD's header (hudbkgnd's GamerPic) of the panes shown from now on (PNG; empty: none).
    void SetGamerPicture(const std::vector<uint8_t>& png);

    // The Xbox Guide (hud.xex's GuideMain.xur): four blades (Games & Apps, the profile's own, Media, Settings) of
    // buttons, as hud.xex fills them for the user's state. Opening it, or coming back to it from one of its panes
    // (achievements, sign-in), shows the blade it was left on.
    struct GuideInfo
    {
        uint32_t user = 0;            // the controller that opened it (its quadrant of the ring of light)
        // hud.xex's user state (0x913EA670): 6 signed out, 7 signed in to Xbox LIVE, 8 a LIVE profile offline,
        // 9 a local profile.
        uint32_t state = 6;
        std::u16string gamertag;
        std::vector<uint8_t> picture; // gamer picture PNG (empty: none)
        uint32_t profiles = 0;        // profiles on the console ("Sign In (%d Profiles)")
        uint32_t gamerscore = 0;      // the Achievements button's count
        // The DVD tray (XamLoaderGetDvdTrayState: 0 closed, 1 closing, 2 open, 3 opening, 4 reading): the tray
        // button's label.
        uint32_t trayState = 0;
        // On Xbox LIVE (state 7): the Home blade's counts (HomeTabSignedInLive.xur's btn_Count rows: friends online,
        // party members, unread messages; owner's capture "guide/guide start page.png").
        uint32_t friendsOnline = 0, partyMembers = 0, unreadMessages = 0;
    };
    void ShowGuide(const GuideInfo& info);
    // The tray moved while the Guide is open: its button's label follows (hud.xex refreshes it from the tray state).
    void SetGuideTrayState(uint32_t trayState);
    // The Guide is leaving for a scene that returns to it (sign-in, achievements): its next opening comes back to the
    // blade and button it had. Any other opening starts on the default blade (hud.xex 0x913E8130 / 0x913E79F8).
    void GuideKeepPosition();
    // A new Guide opening (the Guide button): forget a position kept by a scene the Guide was closed from (owner,
    // 2026-10-22: the Guide button always reopens on Xbox Home; only B back from a scene returns to its button).
    void GuideForgetPosition();
    // Controller input for the Guide: the d-pad / left stick move through a blade's buttons, left / right (and LT /
    // RT) switch blades, A presses the focused button (`id` = its Id, `text` its text), X and Y are the legend's
    // buttons, B / BACK close it.
    struct GuideAction
    {
        enum Kind { kNone, kClose, kButton, kLegendX, kLegendY } kind = kNone;
        std::string id;
        std::u16string text;
    };
    GuideAction GuidePress(uint16_t pressed);
    // A message box the Guide asks (Xbox Home, Turn Off Console, Sign Out): drawn over the Guide, whose blades fade
    // (hudbkgnd Half -> Error); SetFocus / Press drive its buttons. GuideMessageBoxClosed returns to the Guide
    // (Error -> Half); ClosePopup closes both.
    void ShowGuideMessageBox(const std::u16string& title, const std::u16string& text,
                             const std::vector<std::u16string>& buttons, uint32_t focus, MessageIcon icon);
    void GuideMessageBoxClosed();
    // XamStrings.xus (XAM's common strings) / hud.xex's Strings.xus entry, or `fallback`.
    std::u16string CommonString(size_t index, const char16_t* fallback);
    std::u16string HudString(size_t index, const char16_t* fallback);
    // signin.xex's strings.xus entry, or `fallback`.
    std::u16string SigninString(size_t index, const char16_t* fallback);

    // Notification toasts, as XAM queues them (XNotifyQueueUI): `type` is XAM's notification id (the ones below; it
    // picks the icon and the XamStrings text), `user` the user index it concerns (0xFF: none; lights that quadrant of
    // the toast's ring), `param` the text's argument: a gamertag, or "<gamerscore>|<name>" for one achievement and
    // "<count>|<gamerscore>" for several. Toasts are shown one after another, each for 5 seconds after it opens.
    // 8 "%s signed in to Xbox Live" (XamStrings 117) is an Xbox LIVE profile's sign-in once its LIVE logon succeeded;
    // 7 "%s signed in" a local profile's, 9 "%s signed in offline" a LIVE profile's without Xbox LIVE.
    constexpr uint32_t kNotifySignedOut = 0x06, kNotifySignedIn = 0x07, kNotifySignedInLive = 0x08,
                       kNotifySignedInOffline = 0x09,
                       kNotifyAchievement = 0x1B, kNotifyAchievements = 0x27;
    // Xbox LIVE's (param = the other gamer's gamertag where the text has one): "%s wants to be your friend",
    // "%s sent you a message", "Disconnected from Xbox Live", "%s wants you to join an Xbox Live Party", "Party invite
    // sent", "%s has joined / left your Xbox Live Party", "Joined Xbox Live Party".
    // 1 "%s has invited you to play" (XNOTIFYUI_TYPE_GAMEINVITE, XamStrings 63, the mail icon).
    constexpr uint32_t kNotifyGameInvite = 0x01;
    // 0x16 "Game invite sent" (XamStrings 64, the mail icon): Guide.Friends after an Invite to Game was sent.
    constexpr uint32_t kNotifyGameInviteSent = 0x16;
    constexpr uint32_t kNotifyFriendRequest = 0x02, kNotifyMessage = 0x05, kNotifyLiveDisconnected = 0x0B,
                       kNotifyPartyInvite = 0x31, kNotifyPartyInviteSent = 0x32, kNotifyPartyMemberJoined = 0x39,
                       kNotifyPartyMemberLeft = 0x3A, kNotifyPartyJoined = 0x3D;
    // "%s muted" / "%s unmuted" (XamStrings 103 / 104, chat.png); and Guide.Friends' own "%s is now a preferred
    // player." after Prefer This Player (owner's capture 3_17_08: PreferredPlayer.png; not one of XAM's types, the
    // module's text is not in the system update: the capture's English).
    constexpr uint32_t kNotifyMuted = 0x3F, kNotifyUnmuted = 0x40, kNotifyPreferredPlayer = 0x10001;
    // Guide.Beacons' own toast (SetBeacon.lua's ShowToast: sharedres://ico_32x_Beacons.png): `param` is the whole
    // text ("Beacon set for <the game>").
    constexpr uint32_t kNotifyBeacons = 0x10002;
    void QueueNotification(uint32_t type, uint32_t user, const std::u16string& param);
    void SetNotificationPosition(uint32_t position); // XNotifyPositionUI's XNOTIFYUI_POS_* bits

    // A full-screen scene of the runtime's own (a launcher), from XUI XML (xur_write.h
    // CompileScene): drawn opaque under the HUD, its root's Width x Height scaled to cover the output (an older
    // dashboard's 1120x770 scenes: cropped top and bottom, as it shows them) and centred; its
    // timelines run with the overlay's clock. Its relative paths are xam.xex's shrdres package's. Returns false with
    // a reason if the assets are missing or the scene does not compile.
    class Element;
    bool ShowFullScreenScene(std::string_view xml, std::string& error);
    // A scene of a package instead (one of the system UI's, or one added with AddSystemUiPackage: an older
    // dashboard's, which the launcher's era themes draw from); its relative paths are its package's.
    bool ShowFullScreenScene(const std::string& package, const std::string& entry, std::string& error);
    // `nxe`: an NXE dashboard's package (6683 on), whose version 5 scenes write version 8's XuiControl (Scene::Load).
    bool AddSystemUiPackage(std::string name, std::vector<uint8_t> data, std::string& error, bool nxe = false);
    // The system UI of an older system update (the Guide, its skin, XAM's scenes: <module>.<resource>.xzp files in
    // `dir`) in place of the base folder's packages of those names, for an older dashboard (Assets::ReplacePackages).
    // Before the HUD is built: what it already shows keeps the packages it was built from.
    bool UseSystemUiPackages(const std::filesystem::path& dir, std::string& error);
    // Skins whose visuals controls use before the HUD skin's (package, scene; Assets::SetThemeSkins), for the next
    // ShowFullScreenScene: an older dashboard's while the launcher shows its era. Empty: the HUD skin alone.
    bool SetFullScreenSkins(const std::vector<std::pair<std::string, std::string>>& skins, std::string& error);
    // Locator schemes resolved to other packages first (Assets::SetThemeSchemes): the era's "common://" and
    // "sharedres://". Empty: XAM 17559's alone.
    void SetFullScreenSchemes(std::vector<std::pair<std::string, std::string>> schemes);
    // Adds a layer over the full-screen scene: our own XUI XML whose root becomes the scene root's last child (the
    // launcher's controls over an era's background scene). It goes with the scene.
    bool AddFullScreenLayer(std::string_view xml, std::string& error);
    // Runs `edit` on the scene's root under the overlay's lock (texts, visibility, named frames) and redraws it.
    void EditFullScreenScene(const std::function<void(Element& root)>& edit);
    void CloseFullScreenScene();

    // Every title swap, presented or not (headless runs): advances the HUD's animations, toasts and clock to the
    // emulated vblank count (RenderOverlay does too).
    void AdvanceOverlay(uint64_t vblank);
    // Renderer: the overlay for the frame being presented, premultiplied RGBA8 rows of `width`; returns false when
    // nothing is visible. `vblank` is the emulated vblank count (the HUD's clock). `changed` tells whether it differs
    // from the previous call's.
    bool RenderOverlay(uint32_t width, uint32_t height, uint64_t vblank, std::vector<uint8_t>& rgba, bool& changed);
    // The overlay drawn on another surface (a GPU one) over what is there, within `clip`, at the surface's size;
    // false when nothing is visible. AdvanceOverlay runs its animations. `changed`: it differs from the last draw.
    class Surface;
    struct ClipRect;
    bool DrawOverlay(Surface& surface, const ClipRect& clip, bool& changed);
    bool OverlayVisible(); // the HUD, a popup, a toast or the full-screen scene is showing
}
