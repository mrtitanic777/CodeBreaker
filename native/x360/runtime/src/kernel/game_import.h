#pragma once

// Recompiled games put in the `games` folder beside the executable (PC port): each one is presented
// to XAM as an installed content package on the hard drive (content/), so the dashboard's My Games lists it the way a
// console lists an installed Arcade title or Games on Demand. Layout and descriptor: docs/x360/
// content_drive_games.md ("games/ folder").
//
//   games/<any name>/game.ini            the descriptor (below); the rest of the folder is the game as its port ships
//                                        it (executable, the user's game files, ...)
//   game.ini (key = value, ';' comments):
//     title_id     = 584111F7            the title ID (hex)
//     name         = Minecraft: Xbox 360 Edition   the package's display name (the tile's text)
//     title_name   = Minecraft           optional, default: name
//     content_type = D0000               optional (hex), default D0000 (Xbox LIVE Arcade); 4000 installed disc,
//                                        7000 Games on Demand
//     icon         = Base Game Dump/MinecraftIcon.png   the package thumbnail (PNG, at most 0x3D00 bytes)
//     arcade_dir   = Base Game Dump      optional: the folder with ArcadeInfo.xml (copied with every image it names;
//                                        Arcade titles need it)
//     executable   = MinecraftRecomp.exe the PC executable (for launching from My Games, roadmap 7)
//
// The package gets the header tools/scripts/make_game_package.py writes (the same bytes). Its file name is the SHA-1
// of the metadata, so a changed descriptor or icon makes a new package. Each generated package has a marker beside its
// header (Headers/<Type>/<FileName>.recomp: the game's folder and executable); packages with a marker whose game is
// no longer in games/ are removed, as are older versions of a game. Packages without a marker are never touched.

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace kernel::games
{
    struct InstalledGame
    {
        uint32_t titleId = 0, contentType = 0;
        std::string name;
        std::filesystem::path folder, executable, package;
    };

    // <the executable's folder>/games.
    std::filesystem::path GamesFolder();
    // Scans GamesFolder() and brings the hard drive's generated packages in line with it; returns the games listed.
    std::vector<InstalledGame> SyncInstalledGames();
    // Deleting a generated package (System Settings > Storage > Delete; deleting a game deletes its files):
    // removes the game's folder named by the package's marker, so the next start does not install it again. Only a
    // folder directly inside GamesFolder() is removed; returns false (and logs why) otherwise.
    bool RemoveGameFolder(const std::filesystem::path& marker);
}
