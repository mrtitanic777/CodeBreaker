# Guide library: the Xbox Guide's screens and behaviour (shared by every port)

The Xbox Guide as both runtimes show it: main blades, Sign In / Sign Out, Achievements, the Community (friends,
messages, party with voice over `runtime/live`), Profiles, Edit Profile, Gamer Zone, the dashboard items. Moved here
from the dashboard's `kernel/xam_ui.cpp` by #2 (2026-10-16) so every port links the same Guide. Drawn by `runtime/xui`.

- `guide/guide.h` — the entry points (`guide::GuideButton`, `guide::Show*` for the XamShow*UI calls).
- `guide/host.h` — what a runtime provides: `log.h` (Log / Fatal), xui and live in the same target, and the
  `guide::host` functions (system UI slot, profiles and sign-in, gamercard, the title's achievements, dashboard, tray).
- `guide.cmake` — `include()` it, then `target_sources(<runtime> PRIVATE ${X360_GUIDE_SOURCES})` and
  `target_include_directories(<runtime> PRIVATE ${X360_GUIDE_INCLUDE_DIR})`.

Interface, threading and how to adopt it: `../../docs/shared_guide.md`. The dashboard's host is
`Xbox 360 Dashboard/DashRecomp/src/kernel/guide_host.cpp` (+ `OpenSystemUi` in its `xam_ui.cpp`).

Changes: both Claudes may edit; keep it runtime-neutral, note changes in `MESSAGES.md`.
