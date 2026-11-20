// Recompiled games from the `games` folder as installed content packages (kernel/game_import.h).

#include "kernel/stfs_mount.h"
#include "kernel/game_import.h"

#include "kernel/kernel.h"
#include "kernel/users.h"

#include "log.h"
#include "settings.h"

#include "gpu/presenter.h"
#include "live/client.h"
#include "transition/transition.h"

#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <format>
#include <fstream>
#include <map>
#include <optional>
#include <regex>
#include <set>
#include <vector>

namespace fs = std::filesystem;

namespace kernel::games
{
    namespace
    {
        // The header make_game_package.py writes (layout: docs/x360/content_drive_games.md).
        constexpr size_t kHeaderFileSize = 0xA000;
        constexpr size_t kMetadata = 0x344;
        constexpr size_t kFileNameField = 0x971A;
        constexpr size_t kThumbnailMax = 0x3D00;
        constexpr const char* kMarkerExtension = ".recomp";

        fs::path U8(const std::string& s)
        {
            return fs::path(std::u8string(s.begin(), s.end()));
        }

        std::string Trim(const std::string& s)
        {
            const size_t b = s.find_first_not_of(" \t\r\n");
            if (b == std::string::npos)
                return {};
            return s.substr(b, s.find_last_not_of(" \t\r\n") - b + 1);
        }

        std::optional<std::vector<uint8_t>> ReadFile(const fs::path& path)
        {
            std::ifstream in(path, std::ios::binary);
            if (!in)
                return std::nullopt;
            return std::vector<uint8_t>(std::istreambuf_iterator<char>(in), {});
        }

        void Put32(std::vector<uint8_t>& h, size_t at, uint32_t v)
        {
            h[at] = uint8_t(v >> 24), h[at + 1] = uint8_t(v >> 16), h[at + 2] = uint8_t(v >> 8), h[at + 3] = uint8_t(v);
        }

        // UTF-8 text into a big-endian UTF-16 field of `chars` characters (the last one stays 0).
        void PutName(std::vector<uint8_t>& h, size_t at, const std::string& text, int chars)
        {
            wchar_t wide[512] = {};
            const int n = MultiByteToWideChar(CP_UTF8, 0, text.data(), int(text.size()), wide, 511);
            for (int i = 0; i < n && i < chars - 1; i++)
                h[at + i * 2] = uint8_t(wide[i] >> 8), h[at + i * 2 + 1] = uint8_t(wide[i]);
        }

        std::string Sha1Hex(const uint8_t* data, size_t size)
        {
            uint8_t digest[20] = {};
            BCRYPT_HASH_HANDLE hash = nullptr;
            if (!BCRYPT_SUCCESS(BCryptCreateHash(BCRYPT_SHA1_ALG_HANDLE, &hash, nullptr, 0, nullptr, 0, 0)) ||
                !BCRYPT_SUCCESS(BCryptHashData(hash, const_cast<PUCHAR>(data), ULONG(size), 0)) ||
                !BCRYPT_SUCCESS(BCryptFinishHash(hash, digest, sizeof(digest), 0)))
                Fatal("SHA-1 (BCrypt) failed");
            BCryptDestroyHash(hash);
            std::string hex;
            for (uint8_t b : digest)
                hex += std::format("{:02X}", b);
            return hex;
        }

        // The ImagePath attributes of ArcadeInfo.xml (UTF-16 with a byte order mark, as the games ship it, or UTF-8).
        std::vector<std::string> ArcadeImages(const std::vector<uint8_t>& xml)
        {
            std::string text;
            const bool le = xml.size() >= 2 && xml[0] == 0xFF && xml[1] == 0xFE;
            const bool be = xml.size() >= 2 && xml[0] == 0xFE && xml[1] == 0xFF;
            if (le || be)
                for (size_t i = 2; i + 1 < xml.size(); i += 2)
                {
                    const unsigned c = le ? xml[i] | xml[i + 1] << 8 : xml[i] << 8 | xml[i + 1];
                    text += c < 0x80 ? char(c) : '?';
                }
            else
                text.assign(xml.begin(), xml.end());
            std::set<std::string> names;
            static const std::regex imagePath("ImagePath=\"([^\"]+)\"");
            for (std::sregex_iterator it(text.begin(), text.end(), imagePath), end; it != end; ++it)
                names.insert((*it)[1].str());
            return { names.begin(), names.end() };
        }

        std::optional<uint32_t> ParseHex(const std::string& s)
        {
            if (s.empty() || s.size() > 8 || s.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos)
                return std::nullopt;
            return uint32_t(std::stoul(s, nullptr, 16));
        }

        void WriteIfChanged(const fs::path& path, const std::vector<uint8_t>& data)
        {
            if (ReadFile(path) == data)
                return;
            std::ofstream out(path, std::ios::binary | std::ios::trunc);
            out.write(reinterpret_cast<const char*>(data.data()), std::streamsize(data.size()));
            if (!out)
                Fatal("Cannot write {}", path.string());
        }

        // One games/<name>/ folder: its package on the hard drive, made or refreshed. Errors in the user's files are
        // logged and the game is left out.
        std::optional<InstalledGame> Install(const fs::path& folder, std::set<fs::path>& markers)
        {
            std::map<std::string, std::string> values;
            {
                std::ifstream in(folder / "game.ini");
                std::string line;
                while (std::getline(in, line))
                {
                    line = Trim(line.substr(0, line.find(';')));
                    const size_t eq = line.find('=');
                    if (eq != std::string::npos)
                        values[Trim(line.substr(0, eq))] = Trim(line.substr(eq + 1));
                }
            }
            auto fail = [&](const std::string& why) -> std::optional<InstalledGame> {
                Log("Games: {} left out: {}", (folder / "game.ini").string(), why);
                return std::nullopt;
            };
            InstalledGame game;
            game.folder = folder;
            const auto titleId = ParseHex(values["title_id"]);
            if (!titleId || !*titleId)
                return fail("title_id must be the title ID in hex (title_id = 584111F7)");
            game.titleId = *titleId;
            const auto type = ParseHex(values.contains("content_type") ? values["content_type"] : "D0000");
            if (!type || !*type)
                return fail("content_type must be hex (D0000 Arcade, 4000 installed disc, 7000 Games on Demand)");
            game.contentType = *type;
            game.name = values["name"];
            if (game.name.empty())
                return fail("no name (name = the title as My Games shows it)");
            const std::string titleName = values["title_name"].empty() ? game.name : values["title_name"];
            if (!values["executable"].empty())
                game.executable = folder / U8(values["executable"]);
            const auto licenseBits = ParseHex(values.contains("license_bits") ? values["license_bits"] : "1");
            if (!licenseBits)
                return fail("license_bits must be hex");

            std::vector<uint8_t> icon;
            if (!values["icon"].empty())
            {
                const auto data = ReadFile(folder / U8(values["icon"]));
                if (!data)
                    return fail(std::format("icon {} not found", values["icon"]));
                if (data->size() > kThumbnailMax)
                    return fail(std::format("icon {}: {} bytes, thumbnails hold at most {}", values["icon"],
                        data->size(), kThumbnailMax));
                icon = *data;
            }

            // ArcadeInfo.xml and the images it names (Arcade titles: the dash reads the title's name, icon and
            // achievement images from the package).
            std::vector<std::pair<std::string, std::vector<uint8_t>>> files;
            if (!values["arcade_dir"].empty())
            {
                const fs::path dir = folder / U8(values["arcade_dir"]);
                const auto info = ReadFile(dir / "ArcadeInfo.xml");
                if (!info)
                    return fail(std::format("{} has no ArcadeInfo.xml", dir.string()));
                files.emplace_back("ArcadeInfo.xml", *info);
                for (const std::string& name : ArcadeImages(*info))
                {
                    const auto image = ReadFile(dir / U8(name));
                    if (!image)
                        return fail(std::format("{} (named in ArcadeInfo.xml) not found", (dir / name).string()));
                    files.emplace_back(name, *image);
                }
            }

            std::vector<uint8_t> h(kHeaderFileSize);
            std::memcpy(h.data(), "LIVE", 4);
            std::fill_n(h.begin() + 0x22C, 8, uint8_t(0xFF));             // license 0: any console, any user
            Put32(h, 0x234, *licenseBits);
            Put32(h, 0x340, 0xAD0E);                                      // header size, as Microsoft packages
            Put32(h, kMetadata + 0x00, game.contentType);
            Put32(h, kMetadata + 0x04, 2);                                // metadata version
            Put32(h, kMetadata + 0x1C, game.titleId);
            PutName(h, kMetadata + 0xCD, game.name, 128);                 // display name, English slot
            PutName(h, 0x1691, titleName, 64);                            // title name
            h[0x1711] = 0xC0;                                             // transfer flags, as Microsoft packages
            Put32(h, 0x1712, uint32_t(icon.size()));                      // thumbnail
            Put32(h, 0x1716, uint32_t(icon.size()));                      // title thumbnail
            std::copy(icon.begin(), icon.end(), h.begin() + 0x171A);
            std::copy(icon.begin(), icon.end(), h.begin() + 0x571A);
            const std::string fileName = Sha1Hex(h.data() + kMetadata, kFileNameField - kMetadata);
            std::memcpy(h.data() + kFileNameField, fileName.data(), fileName.size());

            const fs::path titleDir = DataDirectory() / "content" / "0000000000000000" /
                std::format("{:08X}", game.titleId);
            const std::string typeName = std::format("{:08X}", game.contentType);
            game.package = titleDir / typeName / fileName;
            const fs::path headers = titleDir / "Headers" / typeName;
            std::error_code ec;
            fs::create_directories(game.package, ec);
            fs::create_directories(headers, ec);
            WriteIfChanged(headers / (fileName + ".header"), h);
            for (const auto& [name, data] : files)
            {
                fs::create_directories((game.package / U8(name)).parent_path(), ec);
                WriteIfChanged(game.package / U8(name), data);
            }
            // The marker: which folder the package stands for (removal when the game goes; launching).
            const std::string marker = std::format("folder={}\nexecutable={}\n", reinterpret_cast<const char*>(
                folder.u8string().c_str()), reinterpret_cast<const char*>(game.executable.u8string().c_str()));
            const fs::path markerPath = headers / (fileName + kMarkerExtension);
            WriteIfChanged(markerPath, std::vector<uint8_t>(marker.begin(), marker.end()));
            markers.insert(markerPath);
            return game;
        }

        // Generated packages (those with a marker) whose game is gone or changed: removed with their header.
        void RemoveStalePackages(const std::set<fs::path>& keep)
        {
            const fs::path common = DataDirectory() / "content" / "0000000000000000";
            std::error_code ec;
            if (!fs::is_directory(common, ec))
                return;
            std::vector<fs::path> stale;
            for (const auto& title : fs::directory_iterator(common, ec))
                for (const auto& type : fs::directory_iterator(title.path() / "Headers", ec))
                    for (const auto& file : fs::directory_iterator(type.path(), ec))
                        if (file.path().extension() == kMarkerExtension && !keep.contains(file.path()))
                            stale.push_back(file.path());
            for (const fs::path& marker : stale)
            {
                const std::string fileName = marker.stem().string();
                const fs::path type = marker.parent_path();
                const fs::path package = type.parent_path().parent_path() / type.filename() / fileName;
                Log("Games: removing {} (its game is no longer in the games folder)", package.string());
                fs::remove_all(package, ec);
                fs::remove(type / (fileName + ".header"), ec);
                fs::remove(marker, ec);
            }
        }
    }

    fs::path GamesFolder()
    {
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(nullptr, exePath, MAX_PATH);
        return fs::path(exePath).parent_path() / "games";
    }

    std::vector<InstalledGame> SyncInstalledGames()
    {
        std::vector<InstalledGame> games;
        std::set<fs::path> markers;
        const fs::path folder = GamesFolder();
        std::error_code ec;
        if (!fs::is_directory(folder, ec))
        {
            fs::create_directories(folder, ec);
            Log("Games: put recompiled games in {} (one folder each, with a game.ini)", folder.string());
        }
        else
        {
            std::vector<fs::path> folders;
            for (const auto& entry : fs::directory_iterator(folder, ec))
                if (entry.is_directory(ec) && fs::is_regular_file(entry.path() / "game.ini", ec))
                    folders.push_back(entry.path());
            std::sort(folders.begin(), folders.end());
            for (const fs::path& f : folders)
                if (auto game = Install(f, markers))
                {
                    Log("Games: {} ({:08X}, type {:X}) from {}", game->name, game->titleId, game->contentType,
                        f.string());
                    games.push_back(std::move(*game));
                }
        }
        RemoveStalePackages(markers);
        return games;
    }

    bool RemoveGameFolder(const fs::path& marker)
    {
        const auto text = ReadFile(marker);
        if (!text)
            return false;
        const std::string s(text->begin(), text->end());
        const size_t at = s.find("folder=");
        if (at == std::string::npos)
        {
            Log("Games: {} names no folder; nothing removed", marker.string());
            return false;
        }
        const size_t end = std::min(s.find('\n', at), s.size());
        std::error_code ec;
        const fs::path folder = fs::weakly_canonical(U8(s.substr(at + 7, end - at - 7)), ec);
        if (ec || folder.parent_path() != fs::weakly_canonical(GamesFolder(), ec) || !fs::is_directory(folder, ec))
        {
            Log("Games: {}: {} is not a game folder in {}; not removed", marker.string(), folder.string(),
                GamesFolder().string());
            return false;
        }
        const auto removed = fs::remove_all(folder, ec);
        if (ec)
        {
            Log("Games: removing {} failed after {} files: {}", folder.string(), removed, ec.message());
            return false;
        }
        Log("Games: {} removed ({} files and folders)", folder.string(), removed);
        return true;
    }
}

namespace
{
    // PC-port addition: the game starts signed in as the dashboard's users. Ports keep their own
    // drive beside themselves (Minecraft: content/, console_settings.ini in its folder) and do not read
    // XBOX360RECOMP_CONTENT yet, so each signed-in profile is copied onto the game's drive (content/<XUID>, only when
    // the game has none: its own copy keeps the game's progress) and the game's console_settings.ini signs them in at
    // start-up (gamertag, gamertag2..., the ports' common settings file). No user signed in: the game's settings stay.
    void HandProfilesToGame(const fs::path& gameFolder)
    {
        std::vector<std::string> gamertags;
        std::error_code ec;
        for (uint32_t user = 0; user < kernel::users::kMaxUsers; user++)
        {
            const auto profile = kernel::users::SignedIn(user);
            if (!profile)
                continue;
            gamertags.push_back(profile->gamertag);
            const std::string id = std::format("{:016X}", profile->xuid);
            const fs::path from = DataDirectory() / "content" / id, to = gameFolder / "content" / id;
            if (fs::exists(to, ec) || !fs::is_directory(from, ec))
                continue;
            fs::create_directories(to.parent_path(), ec);
            fs::copy(from, to, fs::copy_options::recursive, ec);
            Log("Games: {} ({}) copied to the game's drive {}{}", profile->gamertag, id, to.string(),
                ec ? " (failed: " + ec.message() + ")" : "");
        }
        if (gamertags.empty())
            return;
        const fs::path settings = gameFolder / "console_settings.ini";
        std::string text;
        if (const auto bytes = kernel::games::ReadFile(settings))
            text.assign(bytes->begin(), bytes->end());
        for (size_t i = 0; i < 4; i++)
        {
            const std::string key = i == 0 ? "gamertag" : std::format("gamertag{}", i + 1);
            const std::string line = key + " = " + (i < gamertags.size() ? gamertags[i] : "");
            size_t at = 0;
            bool replaced = false;
            while (at < text.size())
            {
                const size_t end = std::min(text.find('\n', at), text.size());
                std::string current = text.substr(at, end - at);
                if (!current.empty() && current.back() == '\r')
                    current.pop_back();
                const size_t eq = current.find('=');
                std::string name = eq == std::string::npos ? "" : current.substr(0, eq);
                name.erase(name.find_last_not_of(" \t") + 1);
                if (name == key)
                {
                    text.replace(at, current.size(), line);
                    replaced = true;
                    break;
                }
                at = end + 1;
            }
            if (!replaced)
                text += (text.empty() || text.back() == '\n' ? "" : "\n") + line + "\n";
        }
        if (FILE* f = _wfopen(settings.c_str(), L"wb"))
        {
            std::fwrite(text.data(), 1, text.size(), f);
            std::fclose(f);
        }
        Log("Games: the game starts with {} signed in ({})", gamertags.front(), settings.string());
    }
}

// DWORD XamContentLaunchImageInternalEx(const XCONTENT_DATA_INTERNAL* Content, LPCSTR Path, DWORD Flags) (XAM
// 0x816F00E8 -> 0x816EFED0 with user 0xFE and the 0x200-byte item): XAM mounts the package as XSYSLAUNCH (content
// create, disposition open-existing; failures return the mount's error, 0x8007xxxx as its low 16 bits), ORs flag
// 0x80 for an optical device, and calls XamLoaderLaunchTitleEx(Path, "XSYSLAUNCH:\", NULL, Flags): the launch is
// queued to XAM's loader thread and the calling title thread sleeps until the loader tears the title down
// (0x816A0798 ends in RtlSleep(-1) outside the system process), so a successful call never returns. The dash calls it
// from 0x92189860 (A on a My Games tile; Flags 0x80000 when its global 0x9296FB14 is set: the loader maps it to bit
// 0x100 of its launch block, meaning not traced) with D3D suspended.
// Here a package generated from games/ (kernel/game_import.h) starts the game's PC executable in its own folder
// (environment XBOX360RECOMP_DASHBOARD = this executable, for the return; XBOX360RECOMP_CONTENT = the hard drive's
// folder, so the game can use the same drive) and the dashboard process ends, as XAM
// ends the dash. Deviations: a game folder without its executable answers ERROR_FILE_NOT_FOUND (the console would
// fail in XexStartExecutable with a fatal error screen); console packages (a default.xex to run) stop.
GUEST_IMPORT(XamContentLaunchImageInternalEx)
{
    auto& mem = guest::g_memory;
    const uint32_t content = ctx.r3.u32, pathPtr = ctx.r4.u32, flags = ctx.r5.u32;
    constexpr uint32_t kErrorInvalidParameter = 0x57, kErrorFileNotFound = 2;
    if (content == 0)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    const std::string path = pathPtr ? reinterpret_cast<const char*>(mem.Translate(pathPtr)) : "";
    const uint32_t type = mem.Read32(content + 4);
    const char* namePtr = reinterpret_cast<const char*>(mem.Translate(content + 0x108));
    const std::string fileName(namePtr, strnlen(namePtr, 42));
    const uint64_t xuid = uint64_t(mem.Read32(content + 0x138)) << 32 | mem.Read32(content + 0x13C);
    const uint32_t title = mem.Read32(content + 0x140);
    const fs::path titleDir = DataDirectory() / "content" / std::format("{:016X}", xuid) / std::format("{:08X}", title);
    const fs::path package = titleDir / std::format("{:08X}", type) / fileName;
    const fs::path marker = titleDir / "Headers" / std::format("{:08X}", type) / (fileName + ".recomp");
    std::error_code ec;
    if (fileName.empty() || !fs::is_directory(package, ec))
    {
        Log("XamContentLaunchImageInternalEx({:08X} type {:X} \"{}\", \"{}\", {:X}): no such package: {}", title, type,
            fileName, path, flags, kErrorFileNotFound);
        ctx.r3.u64 = kErrorFileNotFound;
        return;
    }
    const auto text = kernel::games::ReadFile(marker);
    if (!text)
        Fatal("XamContentLaunchImageInternalEx({:08X} type {:X}, \"{}\", {:X}): {} is a console package (its {} is "
            "not recompiled); only games from the games folder can be launched", title, type, path, flags,
            package.string(), path);
    std::map<std::string, std::string> values;
    {
        const std::string s(text->begin(), text->end());
        size_t at = 0;
        while (at < s.size())
        {
            const size_t end = std::min(s.find('\n', at), s.size());
            const std::string line = s.substr(at, end - at);
            if (const size_t eq = line.find('='); eq != std::string::npos)
                values[line.substr(0, eq)] = line.substr(eq + 1);
            at = end + 1;
        }
    }
    const fs::path folder = kernel::games::U8(values["folder"]), executable = kernel::games::U8(values["executable"]);
    if (values["executable"].empty() || !fs::is_regular_file(executable, ec))
    {
        Log("XamContentLaunchImageInternalEx({:08X} \"{}\"): the game's executable {} is missing (game.ini "
            "\"executable\"): {}", title, path, executable.string(), kErrorFileNotFound);
        ctx.r3.u64 = kErrorFileNotFound;
        return;
    }

    HandProfilesToGame(folder);
    // The game comes back to this dashboard (Xbox Home, its own exit) with these profiles signed in again.
    kernel::users::SaveSignIns();
    wchar_t self[MAX_PATH];
    GetModuleFileNameW(nullptr, self, MAX_PATH);
    SetEnvironmentVariableW(L"XBOX360RECOMP_DASHBOARD", self);
    // Its command line too (a data folder given as its argument), so the game starts it again the same way.
    SetEnvironmentVariableW(L"XBOX360RECOMP_DASHBOARD_COMMAND_LINE", GetCommandLineW());
    SetEnvironmentVariableW(L"XBOX360RECOMP_CONTENT", (DataDirectory() / "content").c_str());
    // Our Xbox LIVE: the game signs the same users in to the same server ("<servers>|<user>:<online XUID>:<gamertag>
    // |..."), so friends see the game and a party carries over (the server keeps the seat while the processes
    // switch). Only users signed in to it now.
    {
        std::string live = Settings().liveServer;
        for (uint32_t user = 0; user < kernel::users::kMaxUsers; user++)
            if (const auto profile = kernel::users::SignedIn(user); profile && live::SignedIn(user))
                live += std::format("|{}:{:016X}:{}", user, profile->onlineXuid, profile->gamertag);
        // Also with no user on it: the game then knows it came from the dashboard and signs no one in by itself (a
        // player kept offline stays offline in the game).
        SetEnvironmentVariableA("XBOX360RECOMP_LIVE", live.empty() ? nullptr : live.c_str());
    }
    // Tests: CB_INPUT_SCRIPT_GAME becomes the game's CB_INPUT_SCRIPT (CB_INPUT_SCRIPT_RETURN goes on to the game,
    // which hands it to the dashboard it returns to).
    if (const char* script = std::getenv("CB_INPUT_SCRIPT_GAME"))
    {
        SetEnvironmentVariableA("CB_INPUT_SCRIPT", script);
        SetEnvironmentVariableA("CB_INPUT_SCRIPT_GAME", nullptr);
    }
    // Seamless, as on a console: the dash's picture fades to black
    // and once its last frame is on the screen, black covers the screen until the game's boot animation is ready (its
    // first frame with content), then this process ends and the black goes with it. CREATE_NO_WINDOW: a port built
    // as a console program gets no console window.
    gpu::FadeToBlack(transition::kFadeToBlack);
    transition::CoverScreen();
    uint32_t pid = 0, error = 0;
    const auto result = transition::Launch(executable.wstring(), folder.wstring(), CREATE_NO_WINDOW,
                                           std::chrono::seconds(60), &pid, &error);
    if (result == transition::Result::kFailed || result == transition::Result::kExited)
    {
        transition::Uncover();
        gpu::ClearFade();
        Log("XamContentLaunchImageInternalEx({:08X} \"{}\"): {} {}", title, path, executable.string(),
            result == transition::Result::kFailed ? std::format("did not start (Windows error {})", error)
                                                  : std::string("ended before showing a frame"));
        ctx.r3.u64 = kErrorFileNotFound; // the dash's "Can't launch" box
        return;
    }
    Log("XamContentLaunchImageInternalEx({:08X} type {:X}, \"{}\", {:X}): started {} (process {}{}); the dashboard "
        "ends as XAM's loader ends the dash", title, type, path, flags, executable.string(), pid,
        result == transition::Result::kTimedOut ? ", no first frame within 60 s" : "");
    kernel::stfs_mount::WaitForAllWrites(); // content packages still being written back
    logging::Flush();
    ExitProcess(0);
}
