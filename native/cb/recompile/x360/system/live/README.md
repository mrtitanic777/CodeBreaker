# Live client: the ports' own Xbox LIVE replacement (shared by both runtimes)

Presence, friends, messages and parties from our own server (the owner's, private; protocol in
`docs/x360/network_protocol.md`). Owner's plan (2026-10-15): a profile that signs in connects to the server
(127.0.0.1 for testing) and sends its status (the title it is in, party, online / offline). No Microsoft protocols,
no crossplay with real consoles.

- `live/client.h` — the API: `live::Start(config)` once (a background thread connects and reconnects),
  `SignIn` / `SignOut` / `SetPresence` per user index, cached `Friends` / `Messages` / `CurrentParty`, blocking
  requests (friend requests, messages, parties), an event handler for changes.
- `live/presence.h` — rich presence as LIVE showed it: the title's XLAST (its SPA's XSRC entry) turns the values
  the title sets (XUserSetContext(X_CONTEXT_PRESENCE, mode), other contexts, properties) into the text friends see
  ("Playing Alone\nRiding a pig"); the runtime sends it with `SetPresence`. Needs `runtime/xui` (inflate) in the
  same target. `RunTest(spaFile)` prints a SPA's modes formatted.
- `live.cmake` — `include()` it, add `${X360_LIVE_SOURCES}`, `${X360_LIVE_INCLUDE_DIRS}`, link `ws2_32`.
- `native/third_party/nlohmann/json.hpp` (shared, moved there 2026-11-11) — nlohmann/json 3.10.5 (MIT).

The XAM side (XFriends*, XPresence*, XMessage*, XParty*, the LIVE sign-in) stays in each runtime. Written by #2 for
the dashboard; #1 can build it the same way for Minecraft (one connection per running port; the server keeps the
user's presence on whichever port signed in last).
