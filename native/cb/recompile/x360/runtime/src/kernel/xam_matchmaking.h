#pragma once

#include "live/client.h"

#include <cstdint>
#include <functional>
#include <optional>
#include <map>
#include <string>
#include <vector>

// A title's XSession* calls on our Xbox LIVE (Shared/docs/network_protocol.md "Sessions", "Matchmaking"): the XGI
// messages XAM answers (app 0xFB) mapped to the live client's session requests, and XSessionSearch(Ex) answered
// from the server's index of the title's matchmaking sessions. kernel/xam_msg.cpp dispatches here.
//
// Message numbers are 17559's XAM (analysis/xam_all_ann.c, FUN_8190aac0 and the handlers it calls):
//   0xB0010 XSessionCreate   { session object, flags, public slots, private slots, user index, XSESSION_INFO*, nonce* }
//   0xB0011 XSessionDelete   { session object }
//   0xB0012 XSessionJoin     { session object, count (1-4), XUID* (remote players) or 0, user index* (local) or 0,
//                              private slots* }  (exactly one of the two arrays)
//   0xB0013 XSessionLeave    the same
//   0xB0014 XSessionStart    { session object }   (XAM: state 2)
//   0xB0015 XSessionEnd      { session object }   (XAM: state 3, then 0)
//   0xB0018 XSessionModify   { session object, flags, public slots, private slots } (16 bytes; XAM 0x81908FA8: only the
//                            join and invite flags 0x100-0x800 may change, else XONLINE_E_SESSION_WRONG_STATE)
//   0xB0065 XSessionSearch / XSessionSearchEx: kSearch* offsets below
// A session object is the 4-byte body of the handle's object (XamSessionCreateHandle / XamSessionRefObjByHandle): the
// title passes the same address in every message of a session, which is how they are matched here.
namespace kernel::matchmaking
{
    constexpr uint32_t kMsgSessionCreate = 0x000B0010, kMsgSessionDelete = 0x000B0011, kMsgSessionJoin = 0x000B0012,
                       kMsgSessionLeave = 0x000B0013, kMsgSessionStart = 0x000B0014, kMsgSessionEnd = 0x000B0015,
                       kMsgSessionModify = 0x000B0018,
                       kMsgSessionSearch = 0x000B0065;

    // XSESSION_CREATE_* flags used here.
    constexpr uint32_t kCreateHost = 0x1, kCreatePresence = 0x2, kCreateStats = 0x4, kCreateMatchmaking = 0x8,
                       kCreateArbitration = 0x10, kCreateJoinInProgressDisabled = 0x400;

    // Whether `user`'s XSessionCreate with `flags` is a session on our Xbox LIVE (the user is on it, and the flags
    // use presence, stats, matchmaking or arbitration: not a system link session). Its XNKID is an online one.
    bool UsesLive(uint32_t user, uint32_t flags);

    // XSessionCreate happened (after kernel/xam_msg.cpp filled the XSESSION_INFO): `id` is the session's XNKID. A host
    // session that UsesLive is registered on the server with the user's matchmaking contexts and properties; a
    // client session (no host flag) is remembered to join the host's when its local users join.
    void SessionCreated(uint32_t sessionObject, uint32_t user, uint32_t flags, uint32_t publicSlots,
                        uint32_t privateSlots, uint64_t id);

    // The other session messages above (delete, join, leave, start, end): what the server needs to know, sent from a
    // worker thread so that the title never waits for the server. Returns true when the message is one of these.
    // Nothing is sent for a session that was not registered (a system link one).
    bool SessionMessage(uint32_t message, uint32_t buffer, uint32_t length);

    // The user that created a session of this title that is on our Xbox LIVE (UsesLive); nullopt for any other (a system
    // link one).
    std::optional<uint32_t> SessionUser(uint32_t sessionObject);
    // Runs `job` on the worker thread that sends the session requests, after the ones queued before it.
    void PostRequest(std::function<void()> job);

    // XSessionSearch / XSessionSearchEx (0xB0065): the request at `buffer`, 0x30 bytes at least:
    //   +0 procedure index (the title's XLAST query; -1 / -3: none), +4 user index, +8 results wanted,
    //   +0x18 u16 search properties, +0x1A u16 search contexts, +0x1C XUSER_PROPERTY*, +0x20 XUSER_CONTEXT*,
    //   +0x24 size of the results buffer, +0x28 XSESSION_SEARCHRESULT_HEADER*, +0x2C u16 users who will join (low half).
    // Returns the result HRESULT: S_OK with the results written, or XONLINE_E_LOGON_NOT_LOGGED_ON when the user is not
    // on our Xbox LIVE (as a console off Xbox LIVE would answer) or the server does not answer.
    uint32_t SessionSearch(uint32_t buffer, uint32_t length);

    // XSessionSearchByIds (0xB0060; XAM 0x81907288): the request at `buffer`, 20 bytes:
    //   +0 user index, +4 count of ids (1-100), +8 XNKID*, +0xC size of the results buffer,
    //   +0x10 XSESSION_SEARCHRESULT_HEADER*.
    // XAM makes it a search with procedure -3 and the ids as blob property 0x60008012 (FUN_819183c0, the 0xB0065
    // search). Answered from the server: each id's session (protocol `session`) that still exists, in the order asked.
    // Minecraft finds its friends' games with it (their presence session ids). Returns as SessionSearch.
    constexpr uint32_t kMsgSessionSearchByIds = 0x000B0060, kSearchMaxIds = 100; // XSESSION_SEARCH_MAX_IDS
    uint32_t SessionSearchByIds(uint32_t buffer, uint32_t length);

    // The user that hosts this console's session `id` on our Xbox LIVE (a host session that UsesLive); nullopt for any
    // other. kernel/xam_qos.cpp sends the host's XNetQosListen data for it.
    std::optional<uint32_t> HostUser(uint64_t id);

    // ---- the pieces SessionSearch is made of (the test harness drives them with a plain buffer) ----

    // A search's attributes as the server wants them (ids of contexts have the type nibble 0, of properties 1-7):
    // contexts equal (4 big-endian bytes). Properties are not sent: the title's XLAST query says how each is compared,
    // and this runtime reads no query from the XLAST (only its presence modes, live/presence.h), so an operator is
    // not known; sending them as "equal" would hide sessions the query would have found. When query operators are
    // read, SessionQuery().ops is where they go (live/client.h SessionFilterOp).
    struct SearchAttribute
    {
        uint32_t id = 0;
        std::vector<uint8_t> value; // a context's 4 bytes, a property's data, as the guest holds them (big-endian)
    };
    struct SearchRequest
    {
        uint32_t user = 0, procedure = 0xFFFFFFFF, results = 0, users = 1;
        std::vector<SearchAttribute> attributes;
    };
    struct SearchQueryOptions
    {
        std::map<uint32_t, std::vector<uint8_t>> filters;
        live::SessionQuery query;
        uint32_t players = 1, maxResults = 0;
    };
    // The filters, players and ranking for `request`: the fullest joinable session first (what a quick match wants;
    // the server breaks ties newest first).
    SearchQueryOptions BuildSearch(const SearchRequest& request);

    // XSESSION_SEARCHRESULT_HEADER and its results for `sessions`, as the guest holds them (big-endian), laid out for
    // a buffer at guest address `base` of `capacity` bytes (a result that does not fit is left out). Returns the
    // bytes (header first), their count is the size used.
    std::vector<uint8_t> SerializeResults(const std::vector<live::GameSessionInfo>& sessions, uint32_t base,
                                          uint32_t capacity);

    // ---- the queue hook (console_settings.ini live_matchmaking_queue, default 0: off) ----
    // A search that finds nothing for a user who is on LIVE queues a ticket (live::QueueMatch) with the search's
    // filters, so that two players who search at the same moment are matched; the title's own flow (create a session
    // and wait) is never touched. When the server answers with a session to join, the user's next search returns it
    // first; a host answer, a timeout and the ticket's end are only logged (the title hosts anyway). Called by
    // SessionSearch; the live event handler (kernel/xam_live.cpp) forwards MatchFound / MatchTimeout.
    void OnLiveEvent(const live::Event& event);
}
