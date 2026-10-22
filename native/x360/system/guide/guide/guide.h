#pragma once

// The Xbox Guide (hud.xex 17559's GuideMainScene and the screens it opens): its main blades, Sign In / Sign Out,
// Achievements, the Community (friends, messages, party with voice, over our Xbox LIVE: native/x360/system/live), a
// player's Profile, the user's own Profile and Edit Profile, Gamer Zone, the dashboard items and Turn Off Console.
// Drawn by the XUI library (native/x360/system/xui); everything runtime-specific goes through guide/host.h.
// Interface and how to adopt it: docs/x360/shared_guide.md.
//
// The entry points open the Guide as the runtime's one system UI (host::OpenSystemUi); a title's XamShow*UI gets
// their result: 0 (ERROR_SUCCESS) once open, or when the user is not on Xbox LIVE and the screen needs it (nothing
// shown, as XAM 0x816C1718 does), 0x525 (ERROR_NO_SUCH_USER) for a user index that is not signed in.

#include <cstdint>
#include <string>
#include <vector>

namespace guide
{
    // The Community's tabs in the console's order.
    enum class Tab { kParty, kFriends, kMessages, kPlayers };

    // The Guide button on `user`'s controller (the runtime's Guide-button handler): opens the Guide for it; on the
    // Guide's own controller it closes the Guide again; ignored while another system UI is open.
    void GuideButton(uint32_t user);

    // XamShowMessagesUI / XamShowPartyUI / XamShowPartyInviteUI (friends) / the dash's own tab requests: the
    // Community on `tab`; Back closes it. Needs Xbox LIVE.
    uint32_t ShowCommunity(uint32_t user, Tab tab);
    // XamShowFriendRequestUI: xuid 0 = Add Friend (gamertag on the keyboard), else a friend request to that player
    // after the console's question. Needs Xbox LIVE.
    uint32_t ShowFriendRequest(uint32_t user, uint64_t xuid);
    // XamShowGamerCardUIForXUID: that player's Profile (Back closes it). The user's own XUIDs (offline or online) or 0
    // show their own Profile (as XAM 0x816C4290 hands those to XamShowGamerCardUI); other players need Xbox LIVE.
    uint32_t ShowGamerCard(uint32_t user, uint64_t xuid);
    // XamShowGamerCardUI: the user's own Profile, on Xbox LIVE or not.
    uint32_t ShowOwnProfile(uint32_t user);
    // XamShowGamesUI: the user's played games (the Guide's Achievements games list) for their own XUIDs or 0, else
    // Compare Games with that player (needs Xbox LIVE). Back closes it.
    uint32_t ShowGames(uint32_t user, uint64_t xuid);
    // XamShowPlayerReviewUI / XamShowComplaintUI: Submit Player Review or File Complaint about that player (needs Xbox
    // LIVE); done or Back closes it.
    uint32_t ShowFeedback(uint32_t user, uint64_t xuid, bool complaint);
    // XamShowOptionsUI (XAM 0x816C2BD8): Settings > Preferences for the user's profile. Back closes it.
    uint32_t ShowPreferences(uint32_t user);
    // After Create Profile: Save Profile (createprofile.cp's Congratulations) for the new profile signed in to `user`.
    // 0 when shown, 5 while another system UI is still open.
    uint32_t ShowProfileCreated(uint32_t user);
    // XamShowFofUI: that friend's friends (Back closes); XamShowJoinSessionInProgressUI: joining that player's game
    // session (our Xbox LIVE has none to join: the console's "Couldn't join" box). Both need Xbox LIVE.
    uint32_t ShowFriendsOf(uint32_t user, uint64_t xuid, const std::string& gamertag);
    uint32_t ShowJoinSessionInProgress(uint32_t user, uint64_t xuid);
    // XamShowMessageComposeUI: a text message to the recipients on the keyboard. Needs Xbox LIVE.
    uint32_t ShowMessageCompose(uint32_t user, const std::vector<uint64_t>& recipients);
}
