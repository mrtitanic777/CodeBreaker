# Network protocol: the ports' own "Xbox LIVE" (presence, friends, messages, parties)

Owner (2026-10-15): whenever a profile signs in, the port connects to a server (127.0.0.1 for testing) and sends its
status ("Xbox 360 Dashboard", "Minecraft: Xbox 360 Edition", in a party, online / offline); then friends lists,
messages and party chat. No crossplay with real consoles, so this is our own protocol: the XAM exports each runtime
implements natively (XFriends*, XPresence*, XMessage*, XParty*, sign-in to Xbox LIVE) are answered from it.

- Server: `native/x360/server/live_server.py` (test server, Python 3, no dependencies; data in a JSON file).
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
| `register` | `gamertag`, `key` (the console's own random ID, kept per install) | `account`: Account with a new online XUID (0009 + 48 random bits). For a profile without an Xbox LIVE account (made on a PC or offline); the client keeps the XUID in the profile. The same gamertag again: the same XUID if `key` matches the console that registered it, else `"gamertag taken"` |
| `signin` | `user` 0-3, `xuid` (the profile's online XUID, 0009...: only Xbox LIVE-enabled profiles sign in), `gamertag` | `account`: Account |
| `signout` | `user` | |
| `profile` | `user`, `xuid` | `account`, `profile`: Profile (as that player last signed in or updated it) |
| `profile_update` | `user`, `profile`: Profile without the pictures (`gamerscore`, `motto`, `zone`, `name`, `location`, `bio`) | (kept; the user's friends get a `friend` event, so their consoles read the profile again) |
| `presence` | `user`, `title_id`, `title` (the title's name), `rich` (rich-presence text, may be ""; formatted by the client from the title's own XUserSetContext / XUserSetPropertyEx values and its XLAST: `runtime/live/live/presence.h`) | |

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

Account: `{"xuid","gamertag","created":<time>,"tenure":<years>}` (`created`: when the account was first seen, 0 if unknown; `tenure`: whole years since, the gamercard's "member since" / XPROFILE_GAMERCARD_TENURE).

## Friends
Friend: `{"xuid","gamertag","online":bool,"title_id","title","rich","party":bool,"request":"" | "outgoing" |
"incoming","last_seen":<time>,"status","beacon": Beacon | null}` (`request` set for pending friend requests, the
console lists them with the friends; `status` the friend's online status when shown; `beacon` their Beacon while online,
else null).

| request | fields | reply |
|---|---|---|
| `friends` | `user` | `friends`: [Friend] (accepted friends and pending requests) |
| `friends_of` | `user`, `xuid` (one of the user's friends) | `friends`: [Friend] (that friend's accepted friends; the Guide's View Friends) |
| `friend_add` | `user`, `gamertag` | `friend`: Friend. A request; if that gamertag already asked us, both become friends (the list holds 100 friends, the console's limit) |
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

## Beacons
Beacon: `{"title_id","title","text","at":<time>}` (also `"xuid"` / `"gamertag"` in the `beacons` list). The Guide's
Beacons (`XamShowBeaconsUI`, "players wanted"): a signed-in user says they want company in a game; their friends see
it. A beacon needs a game (`title_id` not a system title FFFE...); the note is cut to 168 characters. It clears when
the player clears it or signs out (a dropped console keeps it for the carry-over minute).

| request | fields | reply |
|---|---|---|
| `beacon_set` | `user`, `title_id`, `title`, `text` | `beacon`: Beacon |
| `beacon_clear` | `user` | |
| `beacons` | `user` | `beacons`: [Beacon with `xuid` / `gamertag`] (online friends' beacons, newest first) |

A friend setting or clearing a beacon reaches friends through the `friend` event (its `beacon` field).

## Content
Content the dashboard downloads from the server, as the console downloaded system content from Xbox LIVE. The server's
`--content` folder (default `content/` beside it) holds `<package>/<file>`; no sign-in is needed. Files over 4 MB are
not served. The client caches each file in memory after the first fetch.

| request | fields | reply |
|---|---|---|
| `content_manifest` | optional `package` | `content`: {package: {file: {`size`, `hash` (SHA-1 hex)}}} |
| `content_get` | `package`, `file` | `package`, `file`, `hash`, `data` (base64) |

Package `tenure`: the gamercard's tenure number, `crown1.png` .. `crown19.png` (88x40; the card shows
`crown<min(years,19)>.png`), and `milestone1.png` .. `milestone3.png`. These are the console's own images, copied by
`server/import_tenure.py` from the user's system update (package `FFFE07DF00000001`); they are never committed.

## Web services
The HTTP web services the dashboard calls (XOnlineCallWebService, XLiveBase message 0x5806A): the client performs
the call by sending it here instead of to Microsoft's hosts. The service roots come from LiveHive in the `hello`
reply (`server/web_services.py` LIVEHIVE: `SearchDataURL`, `SearchAutosuggestURL`, `EDSUriRoot`,
`ListServiceUriRoot` ... on host `live.local`); any URL is accepted, routed by its path.

| request | fields | reply |
|---|---|---|
| `web` | optional `user` (the caller, for its own lists), `method`, `url`, `headers` {name: value}, `body` (base64) | `status` (HTTP), `headers`, `body` (base64) |

Answered so far: `.../searchsettings.xml` (the bing hub's settings), `/eds/media/<locale>/crossMediaGroupSearch`
`details` `browse` `related` (EDS XML: `Response/Items/Item` from every player's played titles), `/eds/image/<title>.png`
(their tiles), `/search/qsonhs.aspx` (suggestions JSON `AS.Results[].Suggests[].Txt`: the catalog, then the PC's
search engine with `--search-provider google`), `/lists/users/xuid(<n>)/lists/<type>/<name>` GET / POST / PUT /
DELETE (My Pins, the caller's own only; kept in the data file). Anything else: 404.

## Messages
Message: `{"id","from":Account,"sent":<time>,"text","read":bool,"kind":"text" | "friend_request" | "game_invite"
| "party_invite","title_id"}`.

| request | fields | reply |
|---|---|---|
| `message_send` | `user`, `to`: [xuid] (1-16), `text` (at most 256 characters), optional `kind`, `title_id` | |
| `messages` | `user` | `messages`: [Message] (newest first) |
| `message_read` | `user`, `id` | |
| `message_delete` | `user`, `id` | |

Event `message`: `{"op":"message","user","message":Message}` when one arrives. An inbox keeps its newest 200 messages.

## Parties
Party: `{"id","leader":xuid,"members":[Friend-like {"xuid","gamertag","title_id","title"}],"open":bool,
"invite_only":bool}` (`open`: anyone may join; else friends of members and the invited; `invite_only`: only the
invited, the Party Options' "Invite Only" against "Open to Friends").

| request | fields | reply |
|---|---|---|
| `party_create` | `user` | `party` |
| `party_invite` | `user`, `xuid` | (sends a `party_invite` message) |
| `party_join` | `user`, `party` (id) or `friend` (a friend's XUID: the party they are in) | `party` |
| `status` | `user`, `status` (optional: `online`, `away`, `busy`, `offline` = Appear Offline) | `status` (now) |
| `feedback` | `user`, `xuid`, `kind` (`prefer`, `avoid`, `complaint`), `category`, `reason` (the Guide's English labels) | |
| `titles_update` | `user`, `titles` ([{`id` (hex title ID), `name`, `achievements`, `earned`, `gamerscore`, `tile` (base64 PNG)}], at most 64) | |
| `titles_of` | `user`, `xuid` | `titles` (as sent; Compare Games) |
| `title_achievements_update` | `user`, `title_id` (a title sent in `titles_update`; not `id`, the request's), `achievements` ([{`name`, `description`, `unachieved`, `gamerscore`, `earned`, `shown` (not secret), `time` (FILETIME)}], at most 256) | |
| `title_achievements_of` | `user`, `xuid`, `title_id` | `achievements` (as sent; empty when unknown) |
| `party_kick` | `user`, `xuid` (the leader only; a member) | `party` |
| `party_settings` | `user`, `invite_only` (the leader only) | `party` |
| `party_leave` | `user` | |
| `party` | `user` | `party` or null |

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

## Voice (party chat)
UDP on the same port as TCP. The `signin` reply carries `voice_token` (16 hex digits): the client's packets for that
user start with it. Packets (big-endian):

- client -> server: `"XV01"`, token (8 bytes), sequence (u32), payload. An empty payload is a keep-alive that tells
  the server where to send (sent every second while the user is in a party, and before the first frame).
- server -> client: `"XV01"`, the speaker's XUID (8 bytes), sequence (u32), payload.

The server relays each frame to the other members of the speaker's party (members it has heard from). Payload: one
20 ms frame of 16 kHz mono audio, IMA ADPCM: predictor (s16), step index (u8), 0, then 320 samples as 160 bytes
(two 4-bit codes per byte, the first in the low nibble). Clients mix what they receive; nobody hears themselves.
