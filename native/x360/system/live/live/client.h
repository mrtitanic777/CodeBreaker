#pragma once

// The client of the ports' own Xbox LIVE replacement (docs/x360/network_protocol.md): one TCP connection per running
// port, kept by a background thread that reconnects when the server comes back. Users are not signed in again after a
// reconnect by themselves (SignIn / Reconnect). The XAM side of a runtime reads the cached state (friends, messages,
// party) without blocking and is told about changes through the event handler; requests that need the server's
// answer block up to a timeout.
//
// Runtime-neutral: it logs through the runtime's "log.h" (Log), like native/x360/system/xui.

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace live
{
    // A player's Beacon (the Guide's "players wanted", XamShowBeaconsUI / server beacons): they want company in a
    // game. Shown on a friend's entry and in the beacons list.
    struct Beacon
    {
        uint64_t xuid = 0;        // filled in the beacons list; 0 on a Friend's own beacon
        std::string gamertag;
        uint32_t titleId = 0;
        std::string title, text;  // the game and the player's note
        int64_t at = 0;           // when it was set (Unix seconds)
    };

    struct Friend
    {
        uint64_t xuid = 0;
        std::string gamertag;
        bool online = false, party = false;
        uint32_t titleId = 0;
        std::string title, rich;
        std::string request; // "", "outgoing", "incoming"
        int64_t lastSeen = 0;
        std::string status; // online: "", "away" or "busy" (Preferences > Online Status)
        std::optional<Beacon> beacon; // their Beacon while online, else none
    };

    struct Message
    {
        uint32_t id = 0;
        uint64_t from = 0;
        std::string fromGamertag, text, kind; // kind: "text", "friend_request", "game_invite", "party_invite"
        int64_t sent = 0;
        bool read = false;
        uint32_t titleId = 0;
    };

    struct PartyMember
    {
        uint64_t xuid = 0;
        std::string gamertag;
        uint32_t titleId = 0;
        std::string title;
    };

    struct Party
    {
        std::string id;
        uint64_t leader = 0;
        std::vector<PartyMember> members;
        bool open = false;
        bool inviteOnly = false; // Party Status: Invite Only (else Open to Friends)
    };

    // A player met (the Guide's Players tab, `recent_players`): in the same party or the same game at the same time.
    struct RecentPlayer
    {
        uint64_t xuid = 0;
        std::string gamertag;
        uint32_t titleId = 0; // the game they were in when last met
        std::string title;
        int64_t met = 0;      // when (Unix seconds)
        bool online = false;  // signed in now
    };

    // An achievement of a played title as Compare Games shows it (no image: the other console uses its own).
    struct AchievementSummary
    {
        std::string name, description, unachieved;
        uint32_t gamerscore = 0;
        bool earned = false, shown = false; // shown: not a secret one
        uint64_t time = 0;                  // FILETIME (UTC) when earned
    };

    // A played title as Compare Games shows it: name, achievements earned of all, gamerscore earned, tile (PNG), and
    // its achievements (sent apart, title_achievements_update).
    struct TitleSummary
    {
        uint32_t titleId = 0;
        std::string name;
        uint32_t achievements = 0, earned = 0, gamerscore = 0;
        std::vector<uint8_t> tile;
        std::vector<AchievementSummary> list;
    };

    // The public part of a profile (its gamercard): sent at sign-in, read for other players.
    struct PublicProfile
    {
        std::string gamertag; // filled for other players (GetProfile)
        uint32_t gamerscore = 0;
        std::string motto;
        uint32_t zone = 0;
        float reputation = 0;              // XPROFILE_GAMERCARD_REP: 0-100 (the gamercard's stars x 20)
        std::string name, location, bio;   // the Personal Profile (UTF-8; empty: not set)
        std::vector<uint8_t> picture;      // 64x64 gamer picture PNG (empty: none)
        std::vector<uint8_t> pictureSmall; // its 32x32 version (the profile's tile_32.png; empty: none)
        int tenure = 0;                    // whole years on Xbox LIVE (the gamercard's "member since")
        int64_t created = 0;               // when the account was first seen (Unix seconds; 0 if unknown)
    };

    struct Config
    {
        std::string host = "127.0.0.1";
        uint16_t port = 36074;
        // More addresses tried in order when host:port does not answer (e.g. a server's public IP first, then
        // 127.0.0.1 for the PC that runs it: home routers often cannot reach their own public IP from inside).
        std::vector<std::pair<std::string, uint16_t>> fallbacks;
        std::string client;      // this title's name ("Xbox 360 Dashboard")
        uint32_t titleId = 0;    // this title's ID
        bool voiceDevices = true; // party chat through the PC's microphone and speakers (live/voice.h)
        bool sendLog = false;     // live debugging: the runtime's log lines go to the server too (SendLog)
    };

    // SignedOut: the server signed `user` out because the same account signed in on another console (as Xbox LIVE
    // does); it is not signed in again until the runtime calls SignIn.
    enum class EventKind { Connected, Disconnected, SignedIn, SignedOut, FriendChanged, FriendRemoved, MessageArrived,
                           PartyChanged };
    struct Event
    {
        EventKind kind;
        uint32_t user = 0;
        uint64_t xuid = 0; // FriendChanged / FriendRemoved: the friend
    };

    // Reads a settings value "host[:port][, host[:port]...]" into config.host / port / fallbacks (port default
    // 36074). Returns false if it names no server.
    bool ParseServers(const std::string& list, Config& config);

    // Starts the connection thread (once). Without a server the client keeps retrying every few seconds; until then
    // everything answers as offline.
    void Start(const Config& config);
    // Live debugging (config.sendLog): queues one log line for the server (`log`), which keeps each console's lines
    // in a file as they arrive. Lines written before Start are kept (up to a limit) and sent once connected; without
    // sendLog they are dropped at Start. Never logs itself (the runtime's Log calls it).
    void SendLog(const std::string& line);
    // Sends what is queued now, from the calling thread (before the process exits or after a fatal error).
    void FlushLog();
    void Stop();
    bool Connected();
    // Called on the client's thread for every change (keep it short; no requests from inside it).
    void SetEventHandler(std::function<void(const Event&)> handler);

    // A new Xbox LIVE account for a profile that has none (`register`): its online XUID, or the error text in `error`
    // ("gamertag taken", "not connected"...). `key` is this console's own random ID (the same console gets the same
    // XUID back for the gamertag).
    std::optional<uint64_t> Register(const std::string& gamertag, const std::string& key, std::string* error = nullptr);
    // Signs `user` in as the profile (blocks up to the timeout; not connected, it waits for one connection attempt).
    // Returns an error text, or nullopt on success. A user it could not sign in, or who lost the connection or was
    // signed out by another console, is kept offline (Held): never signed in again by itself, only by SignIn or Reconnect.
    std::optional<std::string> SignIn(uint32_t user, uint64_t xuid, const std::string& gamertag,
                                      const PublicProfile& profile = {});
    // Tries the server now if not connected (every address once, blocking up to their timeouts); true if connected.
    bool Connect();
    // Keeps `user` offline (a runtime that could not even ask the server, e.g. no Xbox LIVE account made yet).
    void Hold(uint32_t user);
    // Whether `user` is kept offline (see SignIn), and whether SignIn / Reconnect is signing them in now.
    bool Held(uint32_t user);
    bool SigningIn(uint32_t user);
    // "Connect to Xbox LIVE": signs the kept-offline `user` in again as last signed in (connecting first). For a user
    // the runtime never signed in (a profile without an account yet), a Connected event asks the runtime to.
    std::optional<std::string> Reconnect(uint32_t user);
    // Another player's public profile (blocks on the first request for that XUID; cached until that player's friend
    // entry changes, e.g. they sign in again).
    std::optional<PublicProfile> GetProfile(uint32_t user, uint64_t xuid);
    // The signed-in user's public profile changed (motto, zone, name, location, bio edited on the console): the
    // server keeps it and friends see it on their next look. Returns an error text, or nullopt on success.
    std::optional<std::string> UpdateProfile(uint32_t user, const PublicProfile& profile);
    void SignOut(uint32_t user);
    // The player quits the program (closes its window, Turn Off Console): every signed-in user signs out of our LIVE
    // and the call waits for the server's answers (at most the request timeout each), so a party seat is given up at
    // once and friends see the player go offline. Not for a transfer to another process (a game launched from the
    // dashboard, a port going back to it): there the connection just drops and the server keeps the seat a minute.
    void SignOutAllForExit();
    bool SignedIn(uint32_t user);
    void SetPresence(uint32_t user, uint32_t titleId, const std::string& title, const std::string& rich);

    // The server's LiveHive value for a key (sent at connection), nullopt if it has none.
    std::optional<std::string> LiveHive(const std::string& key);

    // Cached (refreshed at sign-in and by events).
    std::vector<Friend> Friends(uint32_t user);
    // A friend's friends (asks the server each time; empty when `xuid` is not the user's friend or offline).
    std::vector<Friend> FriendsOf(uint32_t user, uint64_t xuid);
    std::vector<Message> Messages(uint32_t user);
    // The players the user met, newest first (at most 50; asks the server unless its answer is a few seconds old;
    // empty when not signed in or not connected).
    std::vector<RecentPlayer> RecentPlayers(uint32_t user);
    std::optional<Party> CurrentParty(uint32_t user);

    // Blocking requests; nullopt on success, else the server's error text (or "not connected" / "timeout").
    std::optional<std::string> AddFriend(uint32_t user, const std::string& gamertag);
    std::optional<std::string> AcceptFriend(uint32_t user, uint64_t xuid);
    std::optional<std::string> DeclineFriend(uint32_t user, uint64_t xuid);
    std::optional<std::string> RemoveFriend(uint32_t user, uint64_t xuid);
    std::optional<std::string> SendLiveMessage(uint32_t user, const std::vector<uint64_t>& to, const std::string& text,
                                               const std::string& kind = "text", uint32_t titleId = 0);
    std::optional<std::string> MarkMessageRead(uint32_t user, uint32_t id);
    std::optional<std::string> DeleteMessage(uint32_t user, uint32_t id);
    std::optional<std::string> CreateParty(uint32_t user);
    std::optional<std::string> InviteToParty(uint32_t user, uint64_t xuid);
    std::optional<std::string> JoinParty(uint32_t user, const std::string& party);
    // A player review ("prefer" / "avoid") or a complaint ("complaint") about `xuid` (server `feedback`): nullopt once
    // sent, else the error.
    // Preferences > Online Status: "online", "away", "busy" or "offline" (Appear Offline). Set (empty `status`:
    // just read) and the status now; nullopt when not on our Xbox LIVE.
    std::optional<std::string> OnlineStatus(uint32_t user, const std::string& status);
    std::optional<std::string> SendFeedback(uint32_t user, uint64_t xuid, const std::string& kind,
                                            const std::string& category, const std::string& reason);
    // `user`'s played titles, published at each sign-in (now too if signed in; server `titles_update`), and another
    // player's (`titles_of`; empty when unknown or offline).
    void SetPlayedTitles(uint32_t user, std::vector<TitleSummary> titles);
    std::vector<TitleSummary> TitlesOf(uint32_t user, uint64_t xuid);
    std::vector<AchievementSummary> TitleAchievementsOf(uint32_t user, uint64_t xuid, uint32_t titleId);
    // The party a friend is in (the Guide's Join Party on a friend's profile; server `party_join` with `friend`).
    std::optional<std::string> JoinFriendsParty(uint32_t user, uint64_t friendXuid);
    // The leader removes a member (the Guide's Kick from Party; server `party_kick`).
    std::optional<std::string> KickFromParty(uint32_t user, uint64_t xuid);
    std::optional<std::string> LeaveParty(uint32_t user);
    // The leader's Party Status (Party Options): Invite Only, or Open to Friends.
    std::optional<std::string> SetPartyInviteOnly(uint32_t user, bool inviteOnly);

    // Beacons (the Guide's "players wanted", XamShowBeaconsUI): the signed-in user asks for company in a game; their
    // friends see it. Set (a game, not a system title, and a note), clear, or list the online friends' beacons.
    std::optional<std::string> SetBeacon(uint32_t user, uint32_t titleId, const std::string& title,
                                         const std::string& text);
    std::optional<std::string> ClearBeacon(uint32_t user);
    std::vector<Beacon> Beacons(uint32_t user);

    // The content subsystem (the server serves content packages the dashboard downloads, like the console did from
    // Xbox LIVE: tenure numbers, ...): one file's bytes, cached in memory after the first fetch. Empty on failure.
    std::vector<uint8_t> GetContent(const std::string& package, const std::string& file);

    // A web-service call the dashboard makes (XOnlineCallWebService: search, My Pins, ...), performed by our server
    // (server/web_services.py) instead of Microsoft's hosts. `user` is the signed-in caller or -1 for an anonymous
    // call. Nullopt when the server cannot be reached.
    struct WebReply
    {
        int status = 0;
        std::vector<std::pair<std::string, std::string>> headers;
        std::vector<uint8_t> body;
    };
    // XOnlineCallWebService (XLiveBase 0x5806A..0x5806F): the raw call goes to our server, which builds the HTTP
    // request XAM would send, answers it and returns the caller's response buffer (server/xws_engine.py). `resp` is
    // the buffer's leading bytes (the rest is zero); `length` is a stream body's length.
    struct WsCallRequest
    {
        int user = -1;
        std::string url, method, reqSchema, respSchema;
        std::vector<uint8_t> req;
        uint32_t flags = 0, respSize = 0;
    };
    struct WsResult
    {
        uint32_t status = 0, hr = 0;
        std::vector<uint8_t> resp;
        std::optional<uint32_t> length;
    };
    std::optional<WsResult> WsCall(const WsCallRequest& call);
    std::optional<uint32_t> WsBegin(const WsCallRequest& call); // the server's handle for the call
    std::optional<WsResult> WsResponse(uint32_t handle, const std::string& respSchema, uint32_t respSize,
                                       uint32_t flags);
    std::optional<std::vector<uint8_t>> WsRead(uint32_t handle, uint32_t offset, uint32_t size);
    void WsClose(uint32_t handle);

    // An Xbox LIVE service task (an .ashx service: XMessageSend, xstatread, vetstring2 ...), performed by our server
    // (server/xon_services.py): the marshalled request in, the bytes XAM would have unmarshalled into the caller's
    // user buffer (at userBase, at most bufSize) out, with the task's HRESULT. `schema` is the service schema in
    // memory (XamGetOnlineSchema), sent once per connection. Nullopt when the server cannot be reached.
    struct XonReply
    {
        uint32_t hr = 0;
        std::string path;
        std::vector<uint8_t> native;
    };
    std::optional<XonReply> XonTask(int user, const std::vector<uint8_t>& schema, uint32_t index,
                                    const std::vector<uint8_t>& wire, uint32_t userBase, uint32_t bufSize,
                                    uint32_t defaultCount);

    std::optional<WebReply> Web(int user, const std::string& method, const std::string& url,
                                const std::vector<std::pair<std::string, std::string>>& headers,
                                const std::vector<uint8_t>& body);
}
