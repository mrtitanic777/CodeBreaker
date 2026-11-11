#include "settings.h"

#include "log.h"

#include <windows.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <map>
#include <sstream>
#include <cstdio>

namespace
{
    ConsoleSettings g_settings;
    std::filesystem::path g_dataDirectory;

    std::string Trim(const std::string& s)
    {
        const auto begin = s.find_first_not_of(" \t\r");
        const auto end = s.find_last_not_of(" \t\r");
        return begin == std::string::npos ? std::string() : s.substr(begin, end - begin + 1);
    }

    void WriteDefaults(const std::filesystem::path& path)
    {
        const ConsoleSettings d;
        std::ofstream out(path);
        out << "; Console settings reported to the game (what a real console's dashboard settings would say).\n"
            << "; Delete this file to restore the defaults.\n\n"
            << "; Video cable: 0 PAL-60 component, 2 PAL-60 SCART, 3 480p component, 4 HDMI+A, 5 composite/S-video,\n"
            << "; 6 VGA, 7 PAL-60 TV, 8 HDMI\n"
            << "av_pack = " << d.avPack << "\n\n"
            << "; Output resolution the console reports (the game renders at its own internal resolution)\n"
            << "video_width = " << d.videoWidth << "\n"
            << "video_height = " << d.videoHeight << "\n"
            << "widescreen = " << (d.widescreen ? "true" : "false") << "\n"
            << "interlaced = " << (d.interlaced ? "true" : "false") << "\n"
            << "; Video standard: 1 NTSC-M, 2 NTSC-J, 3 PAL-I\n"
            << "video_standard = " << d.videoStandard << "\n"
            << "refresh_rate = " << d.refreshRate << "\n\n"
            << "; Language: 1 English, 2 Japanese, 3 German, 4 French, 5 Spanish, 6 Italian, 7 Korean,\n"
            << "; 8 Traditional Chinese, 9 Portuguese, 11 Polish, 12 Russian, 13 Swedish, 14 Turkish, 15 Norwegian,\n"
            << "; 16 Dutch, 17 Simplified Chinese\n"
            << "language = " << d.language << "\n"
            << "; Locale: 36 United States, 35 Great Britain\n"
            << "locale = " << d.locale << "\n"
            << "; Game region: 0x00FF North America, 0x0101 Japan, 0x0102 China, 0x01FC rest of Asia,\n"
            << "; 0x0201 Australia/New Zealand, 0x02FE rest of Europe, 0x03FF rest of world\n"
            << "game_region = 0x00FF\n\n"
            << "; Dashboard version reported by XamGetSystemVersion (2.0.17559.0)\n"
            << "system_version = 0x20449700\n\n"
            << "; How the title was installed: 0 disc, 1 installed to hard drive, 2 download (Xbox LIVE Arcade), 3 other\n"
            << "deployment_type = " << d.deploymentType << "\n\n"
            << "; Local gamer profiles on this console (up to 15 characters; an empty name is no profile). The first is\n"
            << "; signed in to controller 1 at start-up, the others sign in from the game (split-screen players).\n"
            << "gamertag = " << d.profiles[0] << "\n"
            << "gamertag2 = " << d.profiles[1] << "\n"
            << "gamertag3 = " << d.profiles[2] << "\n"
            << "gamertag4 = " << d.profiles[3] << "\n\n"
            << "; Console Ethernet MAC address (default: 00:1D:D8 plus bytes derived from this PC's name)\n"
            << "; mac_address = 00:1D:D8:00:00:00\n\n"
            << "; Xbox LIVE Arcade license bits (XContentGetLicenseMask): 1 = full game, 0 = trial\n"
            << "license_mask = " << d.licenseMask << "\n\n"
            << "; Xbox LIVE server (the ports' own, native/x360/server/live_server.py), host:port; several separated by\n"
            << "; commas are tried in order; empty = no network\n"
            << "live_server = " << d.liveServer << "\n\n"
            << "; 1 = the log (<program>.log) also goes to the Xbox LIVE server as it is written, so the developer\n"
            << "; can follow a test run live (testers' copies); 0 = it stays on this PC\n"
            << "live_log = " << (d.liveLog ? 1 : 0) << "\n\n"
            << "; 1 = a title's quick match (a session search that finds nothing) also queues a ticket on the Xbox LIVE\n"
            << "; server, so that two players who search at the same moment are matched. 0 = off (default)\n"
            << "live_matchmaking_queue = " << (d.liveMatchmakingQueue ? 1 : 0) << "\n\n"
            << "; PC window (not a console setting): start in full screen; F11 or Alt+Enter switch at any time\n"
            << "fullscreen = " << (d.fullscreen ? "true" : "false") << "\n\n"
            << "; PC only: your own console's decrypted keyvault (KV.bin, 0x4000 bytes). Packages written to a\n"
            << "; storage device are then signed with your console's key, so your console accepts them. Empty: unsigned\n"
            << "; console_keyvault = C:\\path\\to\\KV.bin\n"
            << "; or your console's flash dump and its CPU key (32 hex digits, or the path of cpukey.txt): the keyvault\n"
            << "; is decrypted from the dump in memory, nothing is written\n"
            << "; console_flash = C:\\path\\to\\updflash.bin\n"
            << "; console_cpu_key = C:\\path\\to\\cpukey.txt\n";
    }
}

uint64_t ConsoleSettings::OfflineXuid(const std::string& gamertag)
{
    uint64_t hash = 0xCBF29CE484222325ull; // FNV-1a
    for (unsigned char c : gamertag)
        hash = (hash ^ c) * 0x100000001B3ull;
    return 0xE000000000000000ull | (hash & 0x0FFFFFFFFFFFFFFFull);
}

const ConsoleSettings& Settings()
{
    return g_settings;
}

const std::filesystem::path& DataDirectory()
{
    return g_dataDirectory;
}

void LoadSettings(const std::string& directory)
{
    g_dataDirectory = directory;
    const std::filesystem::path path = std::filesystem::path(directory) / "console_settings.ini";
    if (!std::filesystem::exists(path))
    {
        WriteDefaults(path);
        Log("Created {} with default console settings", path.string());
    }

    std::map<std::string, std::string> values;
    std::ifstream in(path);
    std::string line;
    while (std::getline(in, line))
    {
        line = Trim(line.substr(0, line.find(';')));
        const auto eq = line.find('=');
        if (eq != std::string::npos)
            values[Trim(line.substr(0, eq))] = Trim(line.substr(eq + 1));
    }

    auto number = [&](const char* key, uint32_t& out)
        {
            if (auto it = values.find(key); it != values.end())
                out = uint32_t(std::stoul(it->second, nullptr, 0));
        };
    auto boolean = [&](const char* key, bool& out)
        {
            if (auto it = values.find(key); it != values.end())
                out = it->second == "true" || it->second == "1";
        };

    ConsoleSettings& s = g_settings;
    {
        char name[MAX_COMPUTERNAME_LENGTH + 1] = {};
        DWORD length = sizeof(name);
        GetComputerNameA(name, &length);
        uint32_t hash = 0x811C9DC5; // FNV-1a
        for (DWORD i = 0; i < length; i++)
            hash = (hash ^ uint8_t(name[i])) * 0x01000193;
        s.macAddress = { 0x00, 0x1D, 0xD8, uint8_t(hash >> 16), uint8_t(hash >> 8), uint8_t(hash) };
    }
    if (auto it = values.find("mac_address"); it != values.end())
    {
        unsigned b[6];
        if (std::sscanf(it->second.c_str(), "%x:%x:%x:%x:%x:%x", &b[0], &b[1], &b[2], &b[3], &b[4], &b[5]) != 6)
            Fatal("console_settings.ini: mac_address must look like 00:1D:D8:12:34:56");
        for (int i = 0; i < 6; i++)
            s.macAddress[i] = uint8_t(b[i]);
    }
    number("av_pack", s.avPack);
    number("video_width", s.videoWidth);
    number("video_height", s.videoHeight);
    boolean("widescreen", s.widescreen);
    boolean("interlaced", s.interlaced);
    number("video_standard", s.videoStandard);
    if (auto it = values.find("refresh_rate"); it != values.end())
        s.refreshRate = std::stof(it->second);
    number("language", s.language);
    number("locale", s.locale);
    number("game_region", s.gameRegion);
    number("system_version", s.systemVersion);
    number("deployment_type", s.deploymentType);
    number("license_mask", s.licenseMask);
    boolean("fullscreen", s.fullscreen);
    boolean("live_log", s.liveLog);
    boolean("live_matchmaking_queue", s.liveMatchmakingQueue);
    if (auto it = values.find("live_server"); it != values.end())
        s.liveServer = it->second;
    if (auto it = values.find("console_keyvault"); it != values.end())
        s.consoleKeyvault = it->second;
    if (auto it = values.find("console_flash"); it != values.end())
        s.consoleFlash = it->second;
    if (auto it = values.find("console_cpu_key"); it != values.end())
        s.consoleCpuKey = it->second;
    for (size_t i = 0; i < ConsoleSettings::kMaxProfiles; i++)
    {
        const std::string key = i == 0 ? "gamertag" : std::format("gamertag{}", i + 1);
        if (auto it = values.find(key); it != values.end())
            s.profiles[i] = it->second.substr(0, 15);
    }
    // No gamertag is fine here (unlike a game's port): the dashboard starts at its profile select screen, with the
    // profiles copied into profiles/ and those created on it (build/ and release/ copies ship that way).
    for (size_t i = 0; i < ConsoleSettings::kMaxProfiles; i++)
        for (size_t j = 0; j < i; j++)
            if (!s.profiles[i].empty() && s.profiles[i] == s.profiles[j])
                Fatal("console_settings.ini: two profiles are named {}", s.profiles[i]);
    std::string profiles;
    for (const std::string& name : s.profiles)
        if (!name.empty())
            profiles += std::format("{}{} ({:016X})", profiles.empty() ? "" : ", ", name, ConsoleSettings::OfflineXuid(name));

    Log("Console settings: AV pack {}, {}x{}{} {} Hz, language {}, locale {}, region {:04X}, profiles {}",
        s.avPack, s.videoWidth, s.videoHeight, s.interlaced ? "i" : "p", s.refreshRate, s.language, s.locale, s.gameRegion,
        profiles);
}
