#pragma once

#include "live/client.h"

#include <cstdint>
#include <optional>

// Game invites on our Xbox LIVE (Shared/docs/network_protocol.md "Invites and joining"): what the title sees of an invite
// the player accepted in the Guide (XN_LIVE_INVITE_ACCEPTED, XInviteGetAcceptedInfo), and the game session this console
// hosts that friends may be invited to (the Guide's Invite to Game).
namespace kernel::invites
{
    // XLiveBase message of XInviteGetAcceptedInfo (XAM 0x818E39A0, case 0x20 of the XLiveBase switch 0x818CCE08).
    constexpr uint32_t kMsgInviteGetAcceptedInfo = 0x00058023;

    // `user` accepted an invite or joined a friend's game (the Guide's Accept Game Invite / Join Game): the title is told
    // with XN_LIVE_INVITE_ACCEPTED (0x02000002, the user index) and reads it with XInviteGetAcceptedInfo.
    void Accepted(uint32_t user, const live::InviteInfo& info);

    // XInviteGetAcceptedInfo: `arguments` is the message's argument list (16-byte slots { u32 size, pad, u64 pointer },
    // count at +0x200): slot 0 &UserIndex, slot 1 the XINVITE_INFO* to fill (0x54 bytes: xuidInvitee, xuidInviter,
    // dwTitleID, XSESSION_INFO hostInfo, fFromGameInvite). Once read, further calls answer
    // XONLINE_E_MESSAGE_PROPERTY_NOT_FOUND (0x80155A03) until another invite is accepted.
    uint32_t GetAcceptedInfo(uint32_t arguments);

    // The sessions the title hosts on our Xbox LIVE (kernel/xam_matchmaking.cpp tells at XSessionCreate / XSessionDelete).
    void SessionHosted(uint32_t user, uint32_t flags, uint64_t sessionId);
    void SessionEnded(uint64_t sessionId);
    // The session `user` hosts that others can be invited to (XSESSION_CREATE_USES_PRESENCE set, invites not disabled).
    std::optional<uint64_t> HostedPresenceSession(uint32_t user);
}
