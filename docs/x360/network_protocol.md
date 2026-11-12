# Network protocol: the ports' own "Xbox LIVE" (presence, friends, messages, parties)

Owner (2026-10-15): whenever a profile signs in, the port connects to a server (127.0.0.1 for testing) and sends its
status ("Xbox 360 Dashboard", "Minecraft: Xbox 360 Edition", in a party, online / offline); then friends lists,
messages and party chat. No crossplay with real consoles, so this is our own protocol: the XAM exports each runtime
implements natively (XFriends*, XPresence*, XMessage*, XParty*, sign-in to Xbox LIVE) are answered from it.

- Server: the owner's own (private, not in Shared since 2026-10-19). Clients only need this document.
- Client: `native/x360/system/live/` (C++ library both runtimes build; one connection per running port).

## Transport
TCP, default `127.0.0.1:36074` (a runtime reads its address from its settings). Each message is one JSON object on
one line (UTF-8, ended by `\n`). A request may carry `"id"` (a number); the server answers every request that has
one with `{"op":"reply","id":<id>,"ok":true,...}` or `{"op":"reply","id":<id>,"ok":false,"error":"<text>"}`.
Messages without `id` get no reply. The server also sends events (no `id`, below) at any time.

XUIDs are 16 upper-case hex digits; title IDs 8. Times are Unix seconds.

## Connection and sign-in
| request | fields | reply |
|---|---|---|
| `hello` | `version` 1, `client` (the title's name, e.g. "Xbox 360 Dashboard"), `title_id`, `console` (a random ID kept per install) | `server`, `version` |
| `generate_gamertag` | (none) | `gamertag`: a free generated one (adjective + animal + number), not reserved. The console's signup (Title.NewLiveSignup.xex) gets the new account's gamertag this way (GenerateGamertag.ashx) and never asks for one; the Guide's Join Xbox Live then `register`s it |
| `register` | `gamertag`, `key` (the console's own random ID, kept per install) | `account`: Account with a new online XUID (0009 + 48 random bits). For a profile without an Xbox LIVE account (made on a PC or offline); the client keeps the XUID in the profile. The same gamertag again: the same XUID if `key` matches the console that registered it, else `"gamertag taken"`. Optional `login` (the signup's Microsoft account, Title.NewLiveSignup.xex): `email`, `password` (8-16 characters), `first_name`, `last_name`, `birthdate`, `country`; the server keeps it with the account, the password only as a salted PBKDF2-SHA256 hash; an email another account has: `"email in use"` (nothing is made); `"bad email"` / `"bad password"` |
| `email_check` | `email` | `free`: whether a new account may use it (the signup's email screen); `"bad email"` for one that is not an address |
| `signin` | `user` 0-3, `xuid` (the profile's online XUID, 0009...: only Xbox LIVE-enabled profiles sign in), `gamertag` | `account`: Account |
| `signout` | `user` | |
| `profile` | `user`, `xuid` | `account`, `profile`: Profile (as that player last signed in or updated it; once others have reviewed the player, `reputation` comes from their reviews: "Player reviews"), `review`: the user's own review of that player (FB_REVIEW_*) or -1 |
| `profile_update` | `user`, `profile`: Profile without the pictures (`gamerscore`, `motto`, `zone`, `name`, `location`, `bio`) | (kept; the user's friends get a `friend` event, so their consoles read the profile again) |
| `presence` | `user`, `title_id`, `title` (the title's name), `rich` (rich-presence text, may be ""; formatted by the client from the title's own XUserSetContext / XUserSetPropertyEx values and its XLAST: `native/x360/system/live/live/presence.h`) | |

The `hello` reply also carries `livehive`: `{key: string}`, Xbox LIVE's configuration values (XamQueryLiveHive);
keys it lacks are "not set" for the console. The server keeps them in its data file (`"livehive"`).

A connection may sign in up to four users (one port, four controllers). A gamertag belongs to the first XUID that
signs in with it (case-insensitive); another XUID with the same gamertag is refused (`"gamertag taken"`). Closing
the connection signs its users out. Friends of a user see them online while signed in.

The same XUID signing in on another connection takes the account over, as on Xbox LIVE: the old connection gets the
event `{"op":"signed_out","user","reason":"signed in on another console"}` and its client does not sign that user
in again by itself (live/client.h EventKind::SignedOut).

Live debugging: a client may send `{"op":"log","lines":[...],"dropped":n}` (no `id`, no reply) with its runtime's log
lines as they are written (live::SendLog, the dashboard's console_settings.ini `live_log = 1`); the server keeps each
connection's lines in `live_logs/<date>-<time>-<address>.log` beside its data file (`--logs` to move it).

`signin` may carry `profile`: Profile, the public part of the profile that others see on its gamercard:
`{"gamerscore", "motto", "zone", "name", "location", "bio", "reputation", "picture", "picture_small"}` (picture = the
64x64 gamer picture PNG, base64; picture_small = its 32x32 version, the profile's own tile_32.png; name / location / bio
= the Personal Profile, UTF-8, empty when not set; reputation = the profile's XPROFILE_GAMERCARD_REP, 0-100, the
stars x 20; the server cuts texts to the console's limits: motto 21 characters, name 129, location 40, bio 499). Consoles read other players' pictures with `profile` (their tile cache:
XamPrepareGamerTiles / XamReadTile).

Account: `{"xuid","gamertag"}` (the server adds `created`, when it first saw the account, and `tenure`, its years:
the gamercard's "member since").

| request | fields | reply |
|---|---|---|
| `status` | `user`, optional `status`: "online" / "away" / "busy" / "offline" (Preferences > Online Status; "offline" is Appear Offline) | `status`: the account's (kept; friends get a `friend` event, and see `status` in the Friend entry) |

## Friends
Friend: `{"xuid","gamertag","online":bool,"title_id","title","rich","party":bool,"request":"" | "outgoing" |
"incoming","last_seen":<time>,"session_id","session_host","joinable":bool}` (`request` set for pending friend
requests; the console lists them with the friends). `session_id` (XNKID, 16 hex digits, or "") is the game session the
friend shows in presence (a member of a session created with XSESSION_CREATE_PRESENCE), `session_host` its host's
XUID; `joinable` when the viewer may join it through presence (JOIN_VIA_PRESENCE_DISABLED 0x200 off,
JOIN_IN_PROGRESS_DISABLED 0x400 off while playing, JOIN_VIA_PRESENCE_FRIENDS_ONLY 0x800 = the host's friends only, a
free slot). Friends get a `friend` event when it changes.

| request | fields | reply |
|---|---|---|
| `friends` | `user` | `friends`: [Friend] (accepted friends and pending requests) |
| `friends_of` | `user`, `xuid` (one of the user's friends) | `friends`: [Friend] (that friend's accepted friends; the Guide's View Friends) |
| `friend_add` | `user`, `gamertag` | `friend`: Friend. A request; if that gamertag already asked us, both become friends |
| `friend_accept` | `user`, `xuid` | `friend` |
| `friend_decline` | `user`, `xuid` | (removes the incoming request) |
| `friend_remove` | `user`, `xuid` | (removes a friend or an outgoing request) |

Event `friend`: `{"op":"friend","user","friend":Friend}` whenever a friend's entry changes (online, title, rich
presence, party, a request arriving or being answered); `{"op":"friend_removed","user","xuid"}`.

## Recent players
The Guide's Players tab (Guide.Friends.xex's PlayersList): the server remembers, per account, the players it met,
newest first, at most 50 (the module's limit), kept in the data file. Two signed-in players meet when they are in the
same party, or signed in to the same game at the same time (not the dashboard or another system title, FFFE....;
never two users of the same console). The meeting is refreshed while they stay together: at a join, a game change, a
party leave or a sign-out (the "Last met ..." time is the last time they were together).

RecentPlayer: `{"xuid","gamertag","title_id","title","met":<time>,"online":bool}` (`title_id` / `title`: the game the
other player was in when they last met; `online`: signed in now, the row's status icon and the tab's "Players (%d
Online)").

| request | fields | reply |
|---|---|---|
| `recent_players` | `user` | `players`: [RecentPlayer] (newest first) |

## Messages
Message: `{"id","from":Account,"sent":<time>,"text","read":bool,"kind":"text" | "friend_request" | "game_invite"
| "party_invite","title_id"}`, plus `"session_id"` for a game invite (the inviter's game session, XNKID: accepting
the invite joins it; the console's XN_LIVE_INVITE_ACCEPTED / XInviteGetAcceptedInfo) and `"attachment"` when it carries
one: `{"type":"voice" | "photo","format","size",
"duration_ms" (voice),"key"}` (the data is read with `message_attachment`).

| request | fields | reply |
|---|---|---|
| `message_send` | `user`, `to`: [xuid], `text`, optional `kind`, `title_id`, `session_id` (game invites), `attachment` (below) | |
| `messages` | `user` | `messages`: [Message] (newest first) |
| `message_read` | `user`, `message` (the message's id) | |
| `message_delete` | `user`, `message` | (an attachment's data goes once no message refers to it) |
| `message_attachment` | `user`, `message` | `type`, `format`, `data` (base64) |

The message's id goes in `message`: a request's own `id` is its reply number (until 2026-10-19 the clients put the
message's id in `id`, where the request number replaced it; the server still reads `id` when `message` is missing).

Attachments (the Guide's Voice Mail and photo messages, Guide.Voicemail.xex), one per message, sent as
`{"type","format","data":base64}`:
- `voice`, format `adpcm16k`: the party-voice frames ("Voice" below: 16 kHz mono, 20 ms, 164 bytes each)
  concatenated; at most 750 frames (15 s: "Record and play back your voice message, up to 15 seconds long").
- `photo`, format `png` or `jpeg`: at most 256 KiB.
Refused: `"attachment too large"`, `"bad voice data"` (not whole frames), `"bad photo data"`, `"bad attachment ..."`.

Event `message`: `{"op":"message","user","message":Message}` when one arrives.

## Parties
Party: `{"id","leader":xuid,"members":[Friend-like {"xuid","gamertag","title_id","title"}],"open":bool,
"invite_only":bool}` (`open`: anyone may join; else friends of members and the invited; `invite_only`: only the
invited, the Party Options' "Invite Only" against "Open to Friends").

| request | fields | reply |
|---|---|---|
| `party_create` | `user` | `party` |
| `party_invite` | `user`, `xuid` | (sends a `party_invite` message) |
| `party_join` | `user`, `party` (id) | `party` |
| `party_settings` | `user`, `invite_only` (the leader only) | `party` |
| `party_leave` | `user` | |
| `party` | `user` | `party` or null |
| `party_of` | `user`, `xuid` (one of the user's friends) | `party`: that friend's Party or null (the Social hub's party details; a Friend's `party` flag says whether there is one) |
| `party_kick` | `user` (the leader), `xuid` (a member) | `party` (Guide.Friends' Kick from Party, XamPartyKickUser: the member leaves) |

Event `party`: `{"op":"party","user","party":Party | null}` when the user's party changes.

**Party carry-over**: a member whose connection drops (a port ending to start a game or the dashboard) or whose
account signs in from another process keeps their seat for 60 s (listed without a title meanwhile); signing in again
with the same XUID puts them back in the party (a `party` event), in the same place: a leader is still the leader.
Meanwhile friends and party members keep seeing them online in their last title (no offline flicker: it looks as if
they never left); after the minute without a sign-in they go offline (and leave the party). Only `signout` /
`party_leave` leave at once. The
dashboard hands its LIVE sign-ins to the game it starts in `XBOX360RECOMP_LIVE` (`<servers>|<user>:<online XUID
hex>:<gamertag>|...`); a sign-in with an empty profile keeps the stored gamercard. Party chat audio: the
"Voice" section.

## Titles and achievements
What a gamercard shows of another player (the Social hub's Compare Games / completed games, the Guide's gamercard).
Each console sends its signed-in user's data from the profile's GPDs, as Xbox LIVE kept it in sync.

Title: `{"title_id","name","achievements","achievements_total","gamerscore","gamerscore_total","last_played":<time>}`
(earned / possible; the dashboard GPD's title records). Achievement: `{"id","label","description","unachieved",
"image_id","gamerscore","flags","earned":<time or 0>}` (XACHIEVEMENT_DETAILS; flags: type | SHOWUNACHIEVED 0x8 |
ACHIEVED_ONLINE 0x10000 | ACHIEVED 0x20000; texts cut to 31 / 99 / 99 characters).

| request | fields | reply |
|---|---|---|
| `titles_update` | `user`, `titles`: [Title] (at most 1000) | (replaces the user's list) |
| `achievements_update` | `user`, `title_id`, `achievements`: [Achievement] (all of the title's, earned or not; at most 500) | (replaces that title's) |
| `titles` | `user`, `xuid` (any player) | `titles`: [Title], most recently played first ([] if never sent) |
| `achievements` | `user`, `xuid`, `title_id` | `achievements`: [Achievement] ([] if never sent) |

The ports send both after each sign-in and again when the user earns an achievement (live/client.h UpdateTitles /
UpdateAchievements; reading: Titles / Achievements).

The dashboard's own sign-in list (live/client.cpp SendTitles) is a second form of the same data, which the server
keeps in one record per title and gives back in either form:

| request | fields | reply |
|---|---|---|
| `titles_update` | `user`, `titles`: [{`id`,`name`,`achievements` (the title's count),`earned`,`gamerscore`,`tile` (base64 PNG, at most 24 KiB)}] | (the server keeps 64 titles per player) |
| `title_achievements_update` | `user`, `title_id`, `achievements`: [{`name`,`description`,`unachieved`,`gamerscore`,`earned`:bool,`shown`:bool,`time`}] | (after `titles_update`; at most 256) |
| `titles_of` | `user`, `xuid` | `titles`: [that form] (the Guide's Compare Games) |
| `title_achievements_of` | `user`, `xuid`, `title_id` | `achievements`: [that form] |

## Sessions (game sessions, online multiplayer)
A title's XSession* calls (XGI messages to XAM) go to the server so friends can find and join games. The XNKID is made
by the host console (16 hex digits; 0x8 in the top nibble for online peer sessions). Session: `{"id","title_id","host",
"flags" (XSESSION_CREATE_*),"public_slots","private_slots","members":[Account],"state":"lobby" | "playing",
"created","attributes"}`. In memory only: a session ends when its host signs out (or its connection drops).
`attributes` (matchmaking, XSESSION_CREATE_USES_MATCHMAKING): `{"<id, 8 hex digits>": base64}`, the host's contexts
(4 big-endian bytes) and properties (the title's bytes) of the title's matchmaking schema (SPA XMAT).

| request | fields | reply |
|---|---|---|
| `session_create` | `user`, `session_id`, `flags`, `public_slots`, `private_slots` | `session` (the host is its first member; its title is the host's) |
| `session_delete` | `user`, `session_id` | (the host ends it; a member leaves) |
| `session_join` | `user`, `session_id`, `xuids` | `session` (the host adds anyone; others only themselves) |
| `session_leave` | `user`, `session_id`, `xuids` | |
| `session_modify` | `user`, `session_id`, `flags`, `public_slots`, `private_slots` (host) | `session` |
| `session_state` | `user`, `session_id`, `state`: "playing" / "lobby" (host: XSessionStart / XSessionEnd) | |
| `session_qos` | `user`, `session_id`, `qos` (base64, at most 1024 bytes; the host's XNetQosListen data) | |
| `session` | `user`, `session_id` | `session` (with `qos`: base64) or null |
| `session_attributes` | `user`, `session_id`, `attributes` (host; at XSessionCreate / XSessionModify) | |
| `session_search` | `user`, `filters` (as `attributes`), `players`, `max_results` (XSessionSearchEx) | `sessions`: the caller's title's matchmaking sessions (flag 0x8) with `players` open public slots, joinable now (not playing with JOIN_IN_PROGRESS_DISABLED 0x400), not the caller's, whose attributes equal every filter; newest first |

Client: live/client.h CreateGameSession, DeleteGameSession, JoinGameSession, LeaveGameSession, ModifyGameSession,
SetGameSessionPlaying, SetGameSessionAttributes, SearchGameSessions. Game traffic goes through the server ("Game data" below); friends' joinable sessions are in
their Friend entries ("Friends").

## Stats (leaderboards)
A title's leaderboards (XSessionWriteStats, XUserReadStats, XUserCreateStatsEnumeratorBy*). A leaderboard is a stats
view of the title's SPA (`view`, its XLAST id); a row is one player's columns there, ranked by the rating column
(id 65534 = 0xFFFE): highest first, ties by who got there first; ranks run 1..total. Column value: `{"id" (the column's
attribute id),"type" (XUSER_DATA_TYPE: 1 int32, 2 int64, 3 double, 5 float, 7 date/time FILETIME),"value":<number>}`.

As on a console, the client caches XSessionWriteStats per session and player (the last write of each property wins)
and sends it with `stats_write` at XSessionFlushStats / XSessionEnd, mapped to columns by the SPA's view (property ->
column, with the column's aggregation). The server applies the aggregation, as Xbox LIVE did: `last` (default),
`sum`, `min`, `max`.

| request | fields | reply |
|---|---|---|
| `stats_write` | `user`, `title_id`, `xuid` (the player whose row it is: a host writes for remote players too), `views`: [{`view`, `columns`: [{`id`,`type`,`value`,`aggregation`}]}] (at most 10 views, 65 columns each) | |
| `stats_read` | `user`, `title_id`, `xuids` (1-101), `views`: [{`view`, `columns`: [ids]}] (1-10) | `views`: [{`view`,`total` (rows on the board),`rows`: [Row], one per requested XUID in order}] |
| `stats_enumerate` | `user`, `title_id`, `by`: "rank" (+ `rank`, 1 = the top) / "xuid" (+ `xuid`: rows around that player; none when they are not on the board) / "rating" (+ `rating`: from the first row at or below it), `rows` (1-100), `views` as `stats_read` | `views`: [{`view`,`total`,`rows`: [Row] in rank order}] |
| `stats_reset` | `user`, `title_id`, `view` (0 = all of the title's) | (XUserResetStatsView: the user's rows leave the board) |

Row: `{"xuid","gamertag","rank" (0 = not on this board),"rating","columns": [column values]}`: only the requested
columns the player has; the client fills the others with the column's type and zero. Client: live/client.h
WriteStats, ReadStats, EnumerateStats, ResetStats.

## Beacons
"I want to play ..." (Guide.Beacons; read by the dash's BeaconsService in dash.social.lex, whose own Lua builds the
Social hub's suggested activities from the friends list and these). Beacon: `{"title_id","title_type" (up to 5
characters: "Game", "App"),"text" (up to 44),"updated":<time>}`; at most 3 per player, one per title.

| request | fields | reply |
|---|---|---|
| `beacon_set` | `user`, `title_id`, `title_type`, `text` | `beacons`: the user's [Beacon], newest first. Replaces that title's; a fourth title: `"too many beacons"`. Friends get a `friend` event |
| `beacon_delete` | `user`, `title_id` | `beacons` |
| `beacons` | `user`, `xuid` (any player) | `beacons` |
| `friends_beacons` | `user` | `friends`: [{`xuid`,`gamertag`,`beacons`}] (friends that have any, at most 100) |
| `beacon_clear` | `user` | (all of the user's beacons go) |
| `beacons` (no `xuid`) | `user` | `beacons`: the online friends' beacons, newest first, each with that friend's `xuid` and `gamertag` (the dashboard's Beacons list) |

A Friend entry carries the friend's `beacons` and the newest one as `beacon` (null without). The server cuts a
beacon's text at 168 characters. A deliberate sign-out takes a player's beacons down; a dropped connection keeps
them for the seat's minute.

## Gamertag change
dash.lf.gamertagchange (Keyboard, Suggestions, Confirmation scenes). A gamertag: 1-15 characters, letters, digits
and single spaces, starting with a letter, no trailing space; unique (case-insensitive).

| request | fields | reply |
|---|---|---|
| `gamertag_check` | `user`, `gamertag` | `available`, `reason` (why not), `suggestions`: 3 free gamertags like it when not available |
| `gamertag_change` | `user`, `gamertag` | `account` with the new gamertag. Friends and the party are told; stored messages from the user show it |

After a change the server's gamertag wins: a console signing in with the old one is signed in with the new one (the
`signin` reply's `account.gamertag`); its runtime should rename the profile to match.

## Player reviews and complaints
The Guide's Player Review (Guide.PlayerFeedback, XShowPlayerReviewUI) and File a Complaint. Types as xonline.h:
review `FB_REVIEW_PREFER` 0, Avoid 1-9 (`FB_REVIEW_AVOID_TRASH_TALK` .. `_UNFAMILIAR`); complaint
`FB_COMPLAINT_OFFENSIVE_GAMERTAG` 0 .. `FB_COMPLAINT_OFFENSIVE_URL` 15.

| request | fields | reply |
|---|---|---|
| `player_review` | `user`, `xuid` (not the user), `review` 0-9 | `reputation`: that player's new value. One review per reviewer: a new one replaces the old. The player's friends get a `friend` event |
| `complaint` | `user`, `xuid`, `type` 0-15, optional `text` (cut to 499) | (kept for the server's owner; no moderation) |
| `feedback` | `user`, `xuid`, `kind`: "prefer" / "avoid" / "complaint", `category`, `reason` | (the dashboard's older form of both; kept with the complaints) |

Reputation (0-100, the gamercard's stars x 20) once a player has reviews: the average of the reviews (Prefer 100,
Avoid 0) with 5 more counted at the player's own value (their profile's `reputation`, else 60 = 3 stars). Xbox LIVE's
formula was never published; this one moves slowly and stays within 0-100.

## Content (packages the dashboard downloads)
The server's `content/<package>/<file>` folder (`--content`): data the dashboard downloads as a console did from
Xbox LIVE (the tenure numbers, and whatever else is added), cached by hash on the client.

| request | fields | reply |
|---|---|---|
| `content_manifest` | optional `package` | `content`: every package's files with their sizes and SHA-1s (the client fetches only what changed) |
| `content_get` | `package`, `file` | `package`, `file`, `hash`, `data` (base64) |

## Web services and Xbox LIVE services
A title's or the dashboard's HTTP calls to Xbox LIVE services, answered by the server's own implementations
(server/web_services.py, xws_engine.py, xon_services.py):

| request | fields | reply |
|---|---|---|
| `web` | optional `user`, `method`, `url`, `headers`, `body` (base64) | `status`, `headers`, `body` (base64): the dashboard's XOnlineCallWebService (My Pins, the bing hub's search, marketplace lists); without `user` a call is anonymous |
| `ws_call` | `user`, `url`, `method`, `flags`, `req_schema`, `req`, `resp_schema`, `resp_size` | `status`, `hr`, `resp`, `length`: a whole XOnlineCallWebService (XAM 0x5806A) |
| `ws_begin` | as `ws_call` | `handle`: BeginRequest (0x5806B); the call runs now and its response waits for: |
| `ws_response` | `handle`, `resp_schema`, `resp_size`, `flags` | `status`, `hr`, `resp`, `length` (BeginResponse, 0x5806D) |
| `ws_read` | `handle`, `offset`, `size` | `data`: the next bytes of a streamed body (ContinueResponse, 0x5806E) |
| `ws_close` | `handle` | (XOnlineCloseWebServiceHandle, 0x5806F) |
| `xon_schema` | the client's Xbox LIVE service schema (once per connection, before its first `xon`) | (kept in memory for the connection only: it is Microsoft's data) |
| `xon` | `index`, `wire`, `user_base`, `buf_size`, `default_count` | `hr`, `path`, `native` (base64), `used`: one .ashx service task |

## Voice (party chat)
UDP on the same port as TCP. The `signin` reply carries `voice_token` (16 hex digits): the client's packets for that
user start with it. Packets (big-endian):

- client -> server: `"XV01"`, token (8 bytes), sequence (u32), payload. An empty payload is a keep-alive that tells
  the server where to send (sent every second while the user is in a party, and before the first frame).
- server -> client: `"XV01"`, the speaker's XUID (8 bytes), sequence (u32), payload.

The server relays each frame to the other members of the speaker's party (members it has heard from). Payload: one
20 ms frame of 16 kHz mono audio, IMA ADPCM: predictor (s16), step index (u8), 0, then 320 samples as 160 bytes
(two 4-bit codes per byte, the first in the low nibble). Clients mix what they receive; nobody hears themselves.

## Game data (online multiplayer traffic)
UDP on the same port, relayed by the server so players need no port forwarding and two consoles on one PC work (the
Nexia360 hub's idea). A console's address in its XNADDR (`ina` / `inaOnline`) is a handle `10.x.y.z` made of its online
XUID's low 24 bits; a title's datagram to such an address goes to the server, which finds the signed-in player with
those bits (one playing the sender's title first). Packets (big-endian), the token being the signin reply's
`voice_token`:

- client -> server: `"XG01"`, token (8 bytes), destination handle (u32), destination port (u16), source port (u16),
  payload. Destination 0 with no payload is a keep-alive that tells the server where to send (every 2 s while
  signed in).
- server -> client: `"XG01"`, destination XUID (8 bytes), source XUID (8), source port (u16), destination port (u16),
  payload.

Client: live/game.h (Start with a receive handler, Send).

## Private chats and voice channels

Private voice chat for the ports front end, next to the party (wire: JSON lines on TCP 36074, voice frames `XV01` on
UDP, unchanged). Server code (XboxLIVEServer): `src/ports/live_chats.cpp`, the `Session::channel` field and `Server::datagram` in
`live_server.cpp`, the message bucket in `moderation.cpp`.

**Ops** (every request carries `user`, as always; errors are the usual `{ok:false, error}`):

- `chat_invite {user, xuid}` -- the caller's private chat with that player: the chat in which the caller is a member and
  `xuid` is a member or already invited is reused, otherwise a new one is made (the caller a member, `xuid` invited).
  Delivers a message `kind:"chat_invite"`, `text` = the chat id, `from` = the caller, as `party_invite` does (one open
  invitation per inviter and chat: a repeat replaces it). Reply `{chat: view}`. Asking again for a player who already
  joined returns the view and sends nothing. OURS, optional: `chat: "<id>"` invites `xuid` into that chat of the caller's
  (how a chat grows past two; without it every player gets a chat of their own).
- `chat_join {user, chat}` -- an invited player joins; the invitation message is used up. Reply `{chat: view}` (also when
  already a member).
- `chat_leave {user, chat}` -- End Chat. A member leaves; an invitee leaving is declining. Reply `{}`. The leaver gets no
  push. When fewer than two members are left the chat ends: everyone left (members and invitees) is pushed
  `{"op":"chat","chat":{"id":..,"ended":true}}`. A chat with one member and nobody left to answer an invitation ends too.
- `chats {user}` -> `{chats:[view, ...]}`: the chats the caller is a member of or invited to.
- `view` = `{id, members:[{xuid,gamertag}], invited:[{xuid,gamertag}], created}` (`created`: Unix seconds; ids look like `C1`).
- `voice_channel {user, channel}` -> `{channel}`. `channel` is `"party"` (the default, what a session has until it sends this),
  `"chat:<id>"` (the caller must be a member) or `"game"` (no server voice). Anything else: `bad channel`.

**Pushes**: `{"op":"chat","chat":view}` to the members and the invitees on every change (invite, join, leave, decline, sign-out
of a member or invitee); `{"op":"chat","chat":{"id":..,"ended":true}}` when it ends. Signing out or dropping the connection is a
`chat_leave` from every chat the player is in (a chat has no held seat, unlike a party's minute) and drops the invitations
to them (the invitation messages are removed from their inbox and the others' views lose them).

**Voice routing**: a frame goes to the members of the sender's active channel whose own active channel is the same one
(the party's members for `"party"`, the chat's for `"chat:<id>"`), except the sender and listeners who muted the sender.
`"game"` sends and receives nothing. A player may be in a party and in chats at once; one channel is active. When the active
chat ends or the player leaves it, the channel falls back to `"party"`. With no `voice_channel` ever sent every frame
is routed exactly as before (the party).

**Errors**: `no such chat`, `not invited`, `not a member of that chat`, `chat full`, `you cannot invite yourself`, `no such player`
(unknown account), `that player is not online`, `bad xuid` / `bad chat` / `bad channel` (not a string, too long, not a XUID),
`too many open chats` (a player is a member of at most 8), `that player has too many chat invitations` (16 unanswered),
and the message bucket's `too many messages, try again in a minute` (`chat_invite` shares it with `message_send`,
`party_invite`...: 30 a minute, burst 20). A chat holds 8 members and invitees together (`CHAT_MAX`).

**Mutes**: the invitee's mute of the inviter drops the invitation silently, like `party_invite`: the inviter's reply
is a normal view (the invitee listed as invited), but the invitee gets no message, no push, does not see the chat in `chats`
and cannot join (`not invited`); unmuting makes a later invite work (the same one, asked again). A mute also keeps the
muted player's voice from the muter, in chats as in the party.

**Federation**: players of linked servers cannot be invited to private chats: `players on linked servers cannot be invited to
private chats` (a chat is local state; the link carries no chat traffic yet).

Tests: `tests/ports/test_chat.py` (every op and push, all the errors, mutes, disconnect cleanup, voice routing among chat, party,
`game` and the default).

Client: `native/x360/system/live/live/client.h` (Chats, InviteToChat, JoinChat, LeaveChat, SetVoiceChannel); the Guide's Chat: `native/x360/system/guide/guide/guide.cpp` (GuideChat).
