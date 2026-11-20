# XUI library: the Xbox 360 system UI (shared by both runtimes)

The console's system UI drawn from XAM 17559's own XUI assets: XUI packages (XZP) and scenes (XUR), fonts, images,
the **Xbox Guide** (hud.xex's GuideMain with its four blades), message boxes, sign-in, the virtual keyboard and the
achievement list. **Written by #1 for the Minecraft TU46 runtime** (its `MinecraftRecomp/src/xui/` at b7f847b); moved
here by #2 (2026-10-15) so the dashboard and Minecraft build the same code. Owner's wish: one Guide for every port.

- `xui/` — the sources (`#include "xui/<name>.h"`). Additions since b7f847b: `Package::Load(data)` (a package
  from memory), `SigninString` (signin.xex's strings) — both used by the dashboard.
- `xui/host.h` — what a runtime provides: `log.h` (Log / Fatal) on its include path and the `xui::host` functions;
  optionally a UI sound player (`xui::host::SetSoundPlayer`: XuiSoundXAudio files as a timeline sets them; none = silent).
- `xui.cmake` — `include()` it, then `target_sources(<runtime> PRIVATE ${X360_XUI_SOURCES})` and
  `target_include_directories(<runtime> PRIVATE ${X360_XUI_INCLUDE_DIR})`.
- Tests: `CB_XUI_TEST=1` runs `xui::RunXuiTests()` (renders to `analysis/xui/`), from the runtime's main.

The Guide's screens and actions are the Guide library (`../guide`, `docs/shared_guide.md`); the XAM side that
opens plain dialogs (message box, keyboard, sign-in: `kernel/xam_ui.cpp`) stays in each runtime: it depends on the
runtime's kernel. The assets themselves are never here (Microsoft files): each runtime extracts them to
its own `system_ui/` with `tools/scripts/extract_system_ui.py`.

Changes: both Claudes may edit; keep it runtime-neutral (anything a runtime must supply goes through `host.h`), run
both projects' XUI tests when you can, and note changes in `MESSAGES.md`.
