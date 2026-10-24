# Installed recompiled games on the shared content drive

How a recompiled title must look on the runtime's content drive for the 17559 dashboard's **My Games** to list it.
Established by running the dashboard (Xbox 360 Dashboard NOTES, sessions 10-11); details of XAM's side are in
`native/x360/runtime/src/kernel/xam_content.cpp`.

## What the dashboard lists
My Games lists **installed content packages**, not titles played. The dash's content manager walks every owner and
title on the hard drive (XamContentAggregateCreateEnumerator) and its game library keeps these content types:
0x4000 installed disc, 0x5000 original Xbox, 0x7000 Games on Demand, 0x80000 demo, 0x90000, 0xC0000,
**0xD0000 Xbox LIVE Arcade** (plus two XNA special cases). The tile's achievements / gamerscore come from the signed-in
profile; box art comes from LIVE (offline: the controller placeholder, as on a console without network).

## Layout (Xenia's, used by both runtimes' content code)
```
content/0000000000000000/<TitleID>/<Type>/<FileName>/                 package files (common content, XUID 0)
content/0000000000000000/<TitleID>/Headers/<Type>/<FileName>.header   STFS header, 0xA000 bytes
```
- `.header`: the package's STFS header and metadata as a console stores them (magic `CON `/`LIVE`/`PIRS`, licenses at
  0x22C, content type 0x344, title ID 0x360, display names 0x411, title name 0x1691, transfer flags 0x1711, thumbnail
  size/data 0x1712/0x171A, title thumbnail 0x1716/0x571A), then the 42-byte file name at 0x971A.
- Licenses: one entry with ID 0xFFFFFFFFFFFFFFFF (any console) whose bits are the license mask (bit 0 = full game).
- An Arcade package must contain the game's **ArcadeInfo.xml** and the images it names (the dash reads the title name,
  icon and achievement images from it).

## Minecraft TU46 (title 584111F7, Arcade 0xD0000)
```
python -I tools/scripts/make_game_package.py content 584111F7 D0000 "Minecraft: Xbox 360 Edition" Minecraft
       "../Games/Minecraft/TU46/Code/Game/Base Game Dump/MinecraftIcon.png"
       --arcade-dir "../Games/Minecraft/TU46/Code/Game/Base Game Dump"
```
(run in `Xbox 360 Dashboard/`; it only reads the dump). Generated files are the user's data: never committed.

## games/ folder (the dashboard port, 2026-10-15)
Users do not run make_game_package.py: the dashboard's runtime scans `games/` beside its executable at start-up
(`native/x360/runtime/src/kernel/game_import.*`). Each `games/<any name>/` holding a `game.ini` is the game
as its port ships it (its release folder) and becomes the package above (the same bytes as make_game_package.py):
```
; game.ini: key = value, ';' starts a comment, paths relative to the game's folder
title_id     = 584111F7                          title ID, hex (required)
name         = Minecraft: Xbox 360 Edition       display name, the tile's text (required)
title_name   = Minecraft                         optional, default: name
content_type = D0000                             optional, hex, default D0000 (Arcade); 4000 installed disc, 7000 GoD
icon         = Base Game Dump/MinecraftIcon.png  package thumbnail, PNG, at most 0x3D00 bytes
arcade_dir   = Base Game Dump                    folder with ArcadeInfo.xml (copied with the images it names)
executable   = MinecraftRecomp.exe               the PC executable to start from My Games (launching, below)
license_bits = 1                                 optional, hex, default 1 (full game)
arcade_info  = C:/Dumps/Game/ArcadeInfo.xml      optional: the ArcadeInfo.xml when it is not in arcade_dir
arguments    = "C:/Games/Up/assets"             optional: the executable's command-line arguments
signals_ready = no                              optional, default yes: no = the game does not say when its first
                                                frame is up (transition::FrameShown), so the launch does not wait
```
`arcade_info`, `arguments` and `signals_ready` are read by the dashboard's runtime (DashRecomp's `game_import.cpp`);
CodeBreaker's runtime does not read them yet.
An Arcade item (D0000) needs its ArcadeInfo.xml: My Games leaves one without it out. The dashboard's
`tools/scripts/import_converted_games.py` writes these game.ini files for the games CodeBreaker converted
(`Converted/<Name>/`).
Beside each generated header the runtime keeps `Headers/<Type>/<FileName>.recomp` (the game's folder and executable).
A package with a marker whose game left `games/` (or whose game.ini / icon changed) is removed at the next start;
packages without a marker (made by hand) are never touched. Box art: `images/<TitleID>/largeboxart.jpg` beside the exe.

## Launching (the dashboard port, 2026-10-15)
A on a tile: the dash calls XamContentLaunchImageInternalEx(item, "default.xex", flags). On a console XAM mounts the
package, queues the title launch to its loader and the dash never returns: the loader ends it and starts the title.
The dashboard port does the same with a games/ package: it starts `executable` from game.ini in the game's folder
(working directory = that folder, no arguments) and the dashboard process exits. The game's environment carries:
- `XBOX360RECOMP_DASHBOARD` = the dashboard's executable (full path),
- `XBOX360RECOMP_CONTENT` = the dashboard's hard drive folder (its `content/`, Xenia's layout, as above).
A missing executable: the dash's own "Can't launch <title>." box (the console would show a fatal error instead).

## Open: returning to the dashboard (the game's side, proposal)
When `XBOX360RECOMP_DASHBOARD` is set, a title returns the way a console title does: XamLoaderLaunchTitle(NULL, 0)
(the Guide's "Xbox Home", the title's own exit) starts that executable and ends the title's process. When it is not
set (started on its own), the title keeps its current behaviour. Optional, later: use `XBOX360RECOMP_CONTENT` as the
title's hard drive so profiles and saves are the dashboard's (one drive, as on a console).
