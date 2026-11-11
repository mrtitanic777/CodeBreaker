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
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace live
{
    struct Friend
    {
        uint64_t xuid = 0;
        std::string gamertag;
        bool online = false, party = false;
        uint32_t titleId = 0;
        std::string title, rich;
        std::string request; // "", "outgoing", "incoming"
        int64_t lastSeen = 0;
        uint64_t sessionId = 0, sessionHost = 0; // the game session shown in presence (XNKID), its host; 0 = none
        bool joinable = false;                   // the user may join it through presence
        std::string status;                      // online: "", "away" or "busy" (Preferences > Online Status)
    };

    struct Message
    {
        uint32_t id = 0;
        uint64_t from = 0;
        std::string fromGamertag, text, kind; // kind: "text", "friend_request", "game_invite", "party_invite"
        int64_t sent = 0;
        bool read = false;
        uint32_t titleId = 0;
        uint64_t sessionId = 0; // a game invite's game session (XNKID), 0 for other kinds
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

    // A game the player has played (the dashboard GPD's title record; the gamercard's games, Compare Games).
    struct TitlePlayed
    {
        uint32_t titleId = 0;
        std::string name;
        uint32_t achievements = 0, achievementsTotal = 0; // earned / possible
        uint32_t gamerscore = 0, gamerscoreTotal = 0;     // earned / possible
        int64_t lastPlayed = 0;                           // Unix seconds
    };

    // One achievement of a title as a player has it (XACHIEVEMENT_DETAILS without the picture).
    struct AchievementInfo
    {
        uint32_t id = 0, imageId = 0, gamerscore = 0;
        uint32_t flags = 0;  // XACHIEVEMENT_DETAILS dwFlags: type | SHOWUNACHIEVED 0x8 | ACHIEVED 0x20000 ...
        int64_t earned = 0;  // when (Unix seconds; 0: not earned, or earned offline without a time)
        std::string label, description, unachieved; // UTF-8
    };

    // A beacon ("I want to play ..."; Guide.Beacons, the dash's BeaconsService): at most 3 per player.
    struct Beacon
    {
        uint32_t titleId = 0;
        std::string titleType; // "Game", "App" (up to 5 characters)
        std::string text;      // up to 44 characters (UTF-8)
        int64_t updated = 0;   // Unix seconds
    };

    struct FriendBeacons
    {
        uint64_t xuid = 0;
        std::string gamertag;
        std::vector<Beacon> beacons;
    };

    struct GamertagCheck
    {
        bool available = false;
        std::string reason;                   // why not ("gamertag taken", "bad gamertag characters" ...)
        std::vector<std::string> suggestions; // free gamertags like it, when it is not available
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
        // This console's LIVE key (32 hex digits; the runtime keeps it, e.g. in its Key Vault): sent in hello. Our
        // server binds an account to the key that first signs it in and refuses other PCs without one
        // (XboxLIVEServer src/ports/access_keys.h). Empty: none sent.
        std::string key;
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

    // Starts the connection thread (once). It connects only when asked (Connect, SignIn, Reconnect), never by itself:
    // not at the start, not after a failed attempt or a lost connection (owner: as a console, the player connects
    // again with Connect to Xbox Live). Live debugging (config.sendLog) keeps trying every few seconds. Until
    // connected everything answers as offline.
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
    // A free generated gamertag for a new account (`generate_gamertag`; the console's signup has Xbox LIVE make one and
    // never asks): it is not reserved, Register it. Nullopt with the error text in `error`.
    std::optional<std::string> GenerateGamertag(std::string* error = nullptr);
    // The Microsoft account the signup (Title.NewLiveSignup.xex) collected: Register sends it with the new account
    // (`register`'s `login`; the server keeps the password only as a hash) and forgets it once an account has it.
    // Register then fails with "email in use" for an email another account has.
    struct SignupLogin
    {
        std::string email, password, firstName, lastName, birthdate, country;
    };
    void SetSignupLogin(std::optional<SignupLogin> login);
    // Whether a new account may use `email` (`email_check`): nullopt when free, else why not ("email in use",
    // "bad email", "not connected"...).
    std::optional<std::string> CheckEmail(const std::string& email);
    // Signs `user` in as the profile (blocks up to the timeout; not connected, it waits for one connection attempt).
    // Returns an error text, or nullopt on success. A user it could not sign in, or who lost the connection or was
    // signed out by another console, is kept offline (Held): never signed in again by itself (owner, 2026-10-16: as
    // a console, offline until the player connects again), only by SignIn or Reconnect.
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
    // A game invite ("game_invite") names the inviter's game session (sessionId; the invitee joins it on accepting).
    std::optional<std::string> SendLiveMessage(uint32_t user, const std::vector<uint64_t>& to, const std::string& text,
                                               const std::string& kind = "text", uint32_t titleId = 0,
                                               uint64_t sessionId = 0);
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

    // Titles and achievements (protocol "Titles and achievements"): the signed-in user's games played and one title's
    // achievements (all of them, earned or not) go to the server, which shows them to other players.
    std::optional<std::string> UpdateTitles(uint32_t user, const std::vector<TitlePlayed>& titles);
    std::optional<std::string> UpdateAchievements(uint32_t user, uint32_t titleId,
                                                  const std::vector<AchievementInfo>& achievements);
    // Another player's games played (most recent first) / achievements in one title, as their console last sent
    // them; nullopt when the server cannot answer (not connected, unknown player).
    std::optional<std::vector<TitlePlayed>> Titles(uint32_t user, uint64_t xuid);
    std::optional<std::vector<AchievementInfo>> Achievements(uint32_t user, uint64_t xuid, uint32_t titleId);

    // Beacons (protocol "Beacons"): set / remove the user's beacon for a title ("too many beacons" past 3), a
    // player's beacons, and the user's friends that have beacons (the Social hub's suggested activities).
    std::optional<std::string> SetBeacon(uint32_t user, uint32_t titleId, const std::string& titleType,
                                         const std::string& text);
    std::optional<std::string> DeleteBeacon(uint32_t user, uint32_t titleId);
    std::optional<std::vector<Beacon>> Beacons(uint32_t user, uint64_t xuid);
    std::optional<std::vector<FriendBeacons>> FriendsWithBeacons(uint32_t user);

    // Change Gamertag (protocol "Gamertag change"): whether a gamertag is free, and the change itself. After a change
    // the server's gamertag wins at sign-in (SignIn's account carries it); the runtime renames its profile.
    std::optional<GamertagCheck> CheckGamertag(uint32_t user, const std::string& gamertag);
    std::optional<std::string> ChangeGamertag(uint32_t user, const std::string& gamertag);

    // Game sessions (protocol "Sessions"; the title's XSessionCreate / XSessionDelete): the host registers the
    // session's XNKID, flags and slots; deleting ends it (host) or leaves it (member). The server also ends a host's
    // sessions when the host signs out.
    std::optional<std::string> CreateGameSession(uint32_t user, uint64_t sessionId, uint32_t flags, uint32_t publicSlots,
                                                 uint32_t privateSlots);
    std::optional<std::string> DeleteGameSession(uint32_t user, uint64_t sessionId);
    // Players entering / leaving a session (XSessionJoinLocal / Remote, XSessionLeave*): their online XUIDs.
    std::optional<std::string> JoinGameSession(uint32_t user, uint64_t sessionId, const std::vector<uint64_t>& xuids);
    std::optional<std::string> LeaveGameSession(uint32_t user, uint64_t sessionId, const std::vector<uint64_t>& xuids);
    // The host's XSessionModify (flags, slots) and XSessionStart / XSessionEnd (playing).
    std::optional<std::string> ModifyGameSession(uint32_t user, uint64_t sessionId, uint32_t flags, uint32_t publicSlots,
                                                 uint32_t privateSlots);
    std::optional<std::string> SetGameSessionPlaying(uint32_t user, uint64_t sessionId, bool playing);
    // A session's details (XSessionSearchByIds, joining): nullopt when it does not exist (any more) or no answer.
    struct GameSessionInfo
    {
        uint64_t id = 0, host = 0;
        uint32_t titleId = 0, flags = 0, publicSlots = 0, privateSlots = 0;
        std::vector<uint64_t> members;
        bool playing = false;
        std::vector<uint8_t> qos; // the host's XNetQosListen data
        std::map<uint32_t, std::vector<uint8_t>> attributes; // matchmaking attributes (SetGameSessionAttributes)
    };
    std::optional<GameSessionInfo> GameSession(uint32_t user, uint64_t sessionId);
    // A matchmaking session's attributes (XSESSION_CREATE_USES_MATCHMAKING; the host's): the contexts and properties
    // of the title's matchmaking schema the host has set, by id (a context's value as 4 big-endian bytes, a
    // property's data as the title set it). Sent at XSessionCreate and XSessionModify, as XAM does.
    std::optional<std::string> SetGameSessionAttributes(uint32_t user, uint64_t sessionId,
                                                        const std::map<uint32_t, std::vector<uint8_t>>& attributes);
    // XSessionSearchEx: the caller's title's matchmaking sessions that have `players` open public slots, are joinable
    // now (not playing with XSESSION_CREATE_JOIN_IN_PROGRESS_DISABLED), are not the caller's own, and whose
    // attribute equals each filter's bytes (an attribute the session lacks fails its filter); at most `maxResults`,
    // newest first. nullopt when the server does not answer.
    std::optional<std::vector<GameSessionInfo>> SearchGameSessions(
        uint32_t user, const std::map<uint32_t, std::vector<uint8_t>>& filters, uint32_t players, uint32_t maxResults);
    // The host's QoS data for its session (XNetQosListen with XNET_QOS_LISTEN_SET_DATA; at most 1024 bytes).
    std::optional<std::string> SetGameSessionQos(uint32_t user, uint64_t sessionId, const std::vector<uint8_t>& data);

    // Leaderboards (protocol "Stats"). A column value: its column (attribute) id and XUSER_DATA type (1 int32, 2 int64,
    // 3 double, 5 float, 7 date/time) with the number in `integer` (int types) or `real` (double / float).
    struct StatsValue
    {
        uint16_t id = 0;
        uint8_t type = 0;
        int64_t integer = 0;
        double real = 0;
    };
    constexpr uint16_t kStatsRatingColumn = 0xFFFE;
    // A column written to a view, with the aggregation its SPA gives it ("last", "sum", "min", "max").
    struct StatsWrite
    {
        StatsValue value;
        std::string aggregation = "last";
    };
    struct StatsViewWrite
    {
        uint32_t view = 0;
        std::vector<StatsWrite> columns;
    };
    struct StatsRow
    {
        uint64_t xuid = 0;
        std::string gamertag;
        uint32_t rank = 0; // 0: not on this leaderboard
        int64_t rating = 0;
        std::vector<StatsValue> columns; // the requested columns the player has
    };
    struct StatsView
    {
        uint32_t view = 0;
        uint32_t total = 0; // rows on the leaderboard
        std::vector<StatsRow> rows;
    };
    struct StatsSpec
    {
        uint32_t view = 0;
        std::vector<uint16_t> columns;
    };
    // A session's cached XSessionWriteStats for one player (at XSessionFlushStats / XSessionEnd).
    std::optional<std::string> WriteStats(uint32_t user, uint32_t titleId, uint64_t xuid,
                                          const std::vector<StatsViewWrite>& views);
    // XUserReadStats: one row per XUID in each view; nullopt when the server cannot answer.
    std::optional<std::vector<StatsView>> ReadStats(uint32_t user, uint32_t titleId, const std::vector<uint64_t>& xuids,
                                                    const std::vector<StatsSpec>& specs);
    // XUserCreateStatsEnumeratorBy*: `by` "rank" (pivot = rank), "xuid" (pivot = XUID) or "rating" (pivot = rating).
    std::optional<std::vector<StatsView>> EnumerateStats(uint32_t user, uint32_t titleId, const std::string& by,
                                                         int64_t pivot, uint32_t rows,
                                                         const std::vector<StatsSpec>& specs);
    // XUserResetStatsView (view 0: all of the title's).
    std::optional<std::string> ResetStats(uint32_t user, uint32_t titleId, uint32_t view);
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
