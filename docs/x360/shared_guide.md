# The shared Xbox Guide (`runtime/guide`)

Owner's wish (2026-10-16): every recompiled port shows **the same Xbox Guide** — friends, messages, party chat,
profile — without changing each game. The drawing was already shared (`runtime/xui`); since 2026-10-16 the Guide's
**screens and behaviour** are shared too, in `runtime/guide`. A port links it and implements a small host interface;
the game itself is not touched (the Guide button, and the title's own XamShow*UI calls, open it).

```
title (recompiled)  --XamShow*UI-->  runtime's xam_ui.cpp  --guide::Show*-->  runtime/guide  --xui::Show*-->  runtime/xui
controller Guide button  --input's Guide handler-->  guide::GuideButton          |   live::*  -->  runtime/live (our Xbox LIVE)
                                                                                 v
                                                       guide::host::*  (the runtime: users, profiles, system UI slot, title, dashboard)
```

## What is in it

`guide/guide.cpp` (moved from the dashboard's `kernel/xam_ui.cpp`, behaviour unchanged): the Guide's main blades
(hud.xex 17559 GuideMainScene), message boxes over it (Sign Out, Turn Off Console, "Not Available", the dashboard
items' "This will end your current session"), Sign In pane with Download Profile, Achievements of the running title,
the Community (Friends / Messages / Party / Players tabs: Add Friend, friend requests, Send Message / Reply / Delete,
Create New, Invite to Party, Start / Leave Party, mute the party microphone), a player's Profile (Gamer Profile /
Personal Profile tabs, Send Friend Request, Remove Friend), the user's own Profile, Edit Profile (Motto, Name, Location,
Bio on the keyboard; saved and sent to our LIVE), Gamer Zone (+ More Info), the disc tray item.

Not in it (stays in each runtime, it reads guest memory or owns the dialog machinery): the plain XamShowMessageBoxUI /
XamShowKeyboardUI / XamShowSigninUI / XamShowSigninUIp / XamShowAchievementsUI dialogs, the native-window fallback
dialogs, XN_SYS_UI bookkeeping, XamAppUnloadStack / XCancelOverlapped of a system UI.

## The real Community scene (since 2026-10-16)

With `system_ui/guide.friends.friends.xzp` (Guide.Friends.xex 17559's XUI package, copied from a console's SystemAux:
`tools/scripts/extract_system_ui.py`'s third argument) the Community's four tabs are the console's own
`community.xur`: CommunityRootScene's tab strip and transitions (opening snaps to 2To1End / 1To2End / 2To3End /
3To4End; LB / RB play 1To2 ... 4To3, no wrap), PartyScene's buttons (Start a Party / Invite Players to Party, Invite
Party to Game, Party Options) over its member list, and the skin's rows (btn_Fullwidth_Icon_string_x3 / _x4,
btn_Fullwidth_2Icon_2string, btn_RecentPlayers) filled per DataAssociation as the module's FriendsList / PartyList /
MessageList / PlayersList answer XM_GET_SOURCE_TEXT / _IMAGE (`CommunityRow::slotText` / `slotImage` /
`pictureSlot`; guide.cpp FriendSlots, MessageSlots, PartyMemberSlots). Texts come from the module's strings.xus
(`xui::FriendsString`). Without the package, the screens are drawn as before (gamerprofile.xex's list pane).
Its drill-downs too: Party Options (PartySettings.xur, `xui::ShowPartySettings`), Create New's types
(MessageType.xur, `ShowMessageCreate`), View Bio (Bio.xur, `ShowBio`), View Friends (FofScene.xur, `ShowFriendsOf`,
LIVE request `friends_of`); each returns false without the package and the Guide draws its own. The composer
(NewMessage.xur), its recipients (MessageRecipients.xur, `ShowMessageRecipients`), a received message
(Messages_Options2.xur) and another player's Profile (ProfileRoot.xur) too, through `ShowGamerProfile`. Left: the
Players tab (no recent players on our LIVE yet).

## Entry points (`guide/guide.h`)

| Function | Called from | Result |
|---|---|---|
| `guide::GuideButton(user)` | the input layer's Guide-button handler | opens the Guide; on its own controller closes it |
| `guide::ShowCommunity(user, Tab)` | XamShowMessagesUI (kMessages), XamShowPartyUI (kParty), XamShowPartyInviteUI (kFriends), the dash's own tab requests | 0 / 0x525 |
| `guide::ShowFriendRequest(user, xuid)` | XamShowFriendRequestUI (xuid 0 = Add Friend) | 0 / 0x525 |
| `guide::ShowGamerCard(user, xuid)` | XamShowGamerCardUIForXUID (own XUIDs or 0 = own Profile) | 0 / 0x525 |
| `guide::ShowOwnProfile(user)` | XamShowGamerCardUI | 0 / 0x525 |
| `guide::ShowMessageCompose(user, recipients)` | XamShowMessageComposeUI | 0 / 0x525 |

0 = ERROR_SUCCESS once open (also when the user is not on Xbox LIVE and the screen needs it: nothing is shown, as
XAM 0x816C1718 does); 0x525 = ERROR_NO_SUCH_USER. Another system UI being open is logged, still 0 (as before).

## The host interface (`guide/host.h`, defined once by the runtime)

Also needed: `log.h` with `Log` / `Fatal` (as for xui and live), and the xui + live libraries in the same target.

| Function | What the runtime does | Dashboard (`kernel/guide_host.cpp`, `kernel/xam_ui.cpp`) |
|---|---|---|
| `OpenSystemUi(user, onPress, busyKind)` | opens its one system UI for the Guide (XN_SYS_UI 1, title input held, the controller's new presses to `onPress(ui, user, pressed)`, true = closed); returns a `guide::SystemUi` (`Close(why, user)`, `Closed()`), or nullptr + the open one's kind | a `Dialog` of kind "Xbox Guide" (hud, no buttons) through `Open`; `GuideUi` wraps it, `Close` = `Answer(cancel)` (popup closes, then XN_SYS_UI 0) |
| `EndSystemUiInput()` | ends the input routing after a Close made outside `onPress` | `input::EndSystemUi()` |
| `Profiles()`, `SignedIn(user)`, `UserOfXuid(xuid)` | the console's profiles (`guide::Profile`: gamertag UTF-8, xuid, liveEnabled, onlineXuid) | `kernel::users` |
| `OnLive(user)` | signed in to our Xbox LIVE | `kernel::UserOnLive` |
| `SignIn(user, index)`, `SignOut(user)` | sign in / out like the sign-in UI, then XN_SYS_SIGNINCHANGED (signed-in mask); the Guide raises the toast | `users::SignIn/SignOut` + `BroadcastNotification(0x0A)` |
| `DownloadProfiles()` | brings new profiles onto the console; their XUIDs | `profile::ImportNewProfiles` + `users::AddStoredProfiles` |
| `GamerPicture(xuid)` | 64x64 PNG | `profile::GamerPicture` |
| `ReadGamercard(xuid)` | `guide::Gamercard`: gamerscore (nullopt when the dash GPD has none), zone, rep stars, motto, name, location, bio | `users::ReadGamercard` (+ whether the GPD has the credit) |
| `WriteGamercard(user, setting, text)` | one of `guide::kSetting*`; XN_SYS_PROFILESETTINGCHANGED; send to our LIVE; error text or nullopt. Only called while the user is on Xbox LIVE | `users::WriteGamercard` |
| `TitleName()`, `TitleAchievements(user)` | the running title's SPA name and achievements (`xui::AchievementEntry`, earned + unlock time from the profile) | `kernel::spa` + `profile::ReadAchievement` |
| `TitleIsDashboard()`, `DashContext()`, `RebootToDash(request)` | the dashboard as the title: its context and XamLoaderRebootToDash (0x3FC bytes: +4 command, +8 user, +0xC data) | `kernel::TitleId`, `kernel::DashContext`, `kernel::RebootToDash` |
| `EndTitle(why, dashRequest)` `[[noreturn]]` | the player confirmed leaving the title: for the dashboard (`dashRequest`) or Turn Off Console (nullptr) | logs, flushes, ExitProcess(0) (the dash runtime only meets Turn Off Console) |
| `DvdTrayState()`, `OpenCloseDvdTray(open)` | XamLoaderGetDvdTrayState's values (0-3; 4 or more: no tray, the item does nothing) | `kernel::DvdTrayState` / `OpenCloseDvdTray` |

Threading: `onPress` runs on the runtime's input thread (the dashboard: the command processor's, after each frame)
and may run before `OpenSystemUi` returns; the Guide's screens are only changed from there or from the opening call.

## Ports using it

- The dashboard (`kernel/guide_host.cpp`, `kernel/xam_ui.cpp`).
- Minecraft TU46 (since 2026-10-16): `kernel/guide_host.cpp` (no games list:
  the Guide opens the title's achievements; Edit Profile is the dashboard's; no disc tray), the system UI slot in
  `kernel/xam_ui.cpp`, the shared xui instead of its own src/xui, Xbox LIVE through `kernel/dashboard_live.cpp`
  (the dashboard's sign-ins, else console_settings.ini's live_server).

## Adopting it in a port

1. Use the shared xui library (`runtime/xui/README.md`) and the live client (`runtime/live/README.md`, `live::Start`
   once, `live::SignIn` when a profile signs in) if not done yet.
2. `include(<Shared>/runtime/guide/guide.cmake)`, add `${X360_GUIDE_SOURCES}` and `${X360_GUIDE_INCLUDE_DIR}`.
3. Implement `guide/host.h` (the dashboard's `kernel/guide_host.cpp` is a template; the system UI slot is ~40 lines
   next to `Open` / `Answer` in xam_ui.cpp: see `GuideUi` and `guide::host::OpenSystemUi` there). What a port lacks can
   be simple: `OnLive` false until the live client is in (the Community then says "Sign in to Xbox Live ..."),
   `ReadGamercard` from the dash GPD if the profile store has it, `TitleIsDashboard` false.
4. Delete the runtime's own Guide code (GuideSession ... GuideButton), point the input layer's Guide handler at
   `guide::GuideButton`, and route XamShowMessagesUI / PartyUI / PartyInviteUI / FriendRequestUI / GamerCardUI(ForXUID)
   / MessageComposeUI to the `guide::Show*` functions.
5. `EndTitle` for a title (open items, `docs/content_drive_games.md` "Launching"): when `XBOX360RECOMP_DASHBOARD` is
   set, start that exe (the dashboard; `dashRequest` +4 is the command the player chose, e.g. 0x17 My Games) and exit;
   use `XBOX360RECOMP_CONTENT` as the hard drive so both share profiles and saves.

## Next step: the party carries over when the dashboard launches a game

In a party with a friend on the dashboard, launch Minecraft: still in the party with that friend. Today the dash's
process ends at launch, its LIVE connection closes, and `server/live_server.py`'s `sign_out` (line ~250) calls
`leave_party`, so the seat is gone; the game has no LIVE client to come back with. Planned (not built yet):
- **Server**: when a connection closes (not an explicit `signout` / `party_leave`), keep the member's party seat for a
  grace period (~60 s, shown as "joining"); when the same XUID signs in again within it, seat them again (the party
  event to everyone, voice resumes with the new `voice_hello`); after it, `leave_party` as today. An explicit
  `party_leave` or sign-out from the Guide still leaves at once.
- **Game (a port linking runtime/live + runtime/guide)**: at start-up, `live::Start` and `live::SignIn` the profiles
  the dashboard handed over (kernel/game_import.cpp HandProfilesToGame writes them to the game's drive and
  console_settings.ini; later `XBOX360RECOMP_CONTENT`), before the grace period ends. Nothing in `guide/host.h`
  changes: the Guide's Party tab reads `live::CurrentParty`, so the restored seat shows up there.
- **Back to the dashboard**: the same in reverse (the game's connection closes on EndTitle, the dash signs in at start).
- Protocol: note the grace seat in `docs/network_protocol.md` (party event: a member may be `"joining": true`).

## Changing it

Edit `runtime/guide`; keep it runtime-neutral (anything runtime-specific goes through `host.h`),
build and run both runtimes' Guide tests when you can, and note changes in `MESSAGES.md` (a new host function breaks
the other runtime's link until it defines it: say so there).
