#pragma once

#include <array>
#include <filesystem>
#include <cstdint>
#include <string>

// Console settings the title can query (the dashboard/XConfig values of a real console).
// Stored in console_settings.ini next to TU46.xex; created with these defaults if missing.
struct ConsoleSettings
{
    // XGetAVPack codes: 0 PAL-60 component, 2 PAL-60 SCART, 3 480p component, 4 HDMI+A, 5 composite/S-video,
    // 6 VGA, 7 PAL-60 TV, 8 HDMI.
    uint32_t avPack = 8;
    uint32_t videoWidth = 1280;
    uint32_t videoHeight = 720;
    bool widescreen = true;
    bool interlaced = false;
    // XC_VIDEO_STANDARD: 1 NTSC-M, 2 NTSC-J, 3 PAL-I.
    uint32_t videoStandard = 1;
    float refreshRate = 60.0f;
    // XC_LANGUAGE: 1 English, 2 Japanese, 3 German, 4 French, 5 Spanish, 6 Italian, 7 Korean, 8 Traditional Chinese,
    // 9 Portuguese, 11 Polish, 12 Russian, 13 Swedish, 14 Turkish, 15 Norwegian, 16 Dutch, 17 Simplified Chinese.
    uint32_t language = 1;
    // XC_LOCALE (36 = United States, 35 = Great Britain).
    uint32_t locale = 36;
    // XC_GAME_REGION: high byte region (0 North America, 1 Asia, 2 Europe, 3 rest of world), low byte subregion.
    uint32_t gameRegion = 0x00FF;
    // XamGetSystemVersion: final retail dashboard 2.0.17559.0 (major << 28 | minor << 24 | build << 8 | qfe).
    uint32_t systemVersion = 0x20449700;

    // XTitleGetDeploymentType: 0 optical disc, 1 installed to hard drive, 2 download (Xbox LIVE Arcade), 3 other.
    // The dump includes ArcadeInfo.xml (an XBLA package), so the default is 2.
    uint32_t deploymentType = 2;

    // The console's local (offline) profiles, by gamertag; an empty name is no profile. Profile 1 is signed in to
    // controller 1 (user index 0) at start-up, like a console's auto sign-in profile; the others sign in through the
    // Guide's sign-in UI (kernel/users.h).
    static constexpr size_t kMaxProfiles = 4;
    std::array<std::string, kMaxProfiles> profiles = { "Player", "Player2", "Player3", "Player4" };
    // A profile's offline XUID: 0xE in the top nibble, derived from the gamertag so it is stable between runs.
    static uint64_t OfflineXuid(const std::string& gamertag);

    // The console's Ethernet MAC address (XNADDR.abEnet). A real console always has one, cable or not.
    // Default: Microsoft's Xbox 360 prefix 00:1D:D8 plus three bytes derived from the host computer's name, so it is
    // stable on one PC and differs between PCs.
    std::array<uint8_t, 6> macAddress = {};

    // XContentGetLicenseMask for this XBLA title: bit 0 is the full-game license (0 = trial).
    uint32_t licenseMask = 1;

    // The ports' own Xbox LIVE server (docs/x360/network_protocol.md), "host:port"; empty = no network.
    std::string liveServer = "127.0.0.1:36074";
    // Live debugging (testers' copies): every log line also goes to that server as it is written (live/client.h
    // SendLog), which keeps it in live_logs/ on the server's PC.
    bool liveLog = false;

    // PC only (not a console setting): the window starts windowed (true: borderless full screen); F11 / Alt+Enter
    // switch.
    bool fullscreen = false;

    // The user's own console's decrypted keyvault (0x4000 bytes): CON packages the runtime writes are signed with
    // its key (kernel/stfs_mount.h), so a stock console accepts them. Empty: written unsigned. Read in place, never
    // copied, never logged; not part of any build.
    std::string consoleKeyvault;
    // Or the console's own flash (NAND / eMMC dump: updflash.bin, nanddump.bin) and its CPU key (32 hex digits, or a
    // path to cpukey.txt): the keyvault is decrypted from the flash in memory (kernel/console_keyvault.h). The same
    // rules: read in place, never copied, never logged.
    std::string consoleFlash;
    std::string consoleCpuKey;
};

const ConsoleSettings& Settings();
// Loads (or creates with defaults) the settings file in the given directory.
void LoadSettings(const std::string& directory);
// The directory holding console_settings.ini and the console's writable data (content/).
const std::filesystem::path& DataDirectory();
