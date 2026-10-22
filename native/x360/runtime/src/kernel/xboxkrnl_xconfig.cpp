// The console's configuration store (XConfig): ExGetXConfigSetting / ExSetXConfigSetting.
//
// On a console the kernel keeps the settings in flash, in fixed per-category blocks. Here each (category, setting) has
// its console size (from the dashboard's own buffers, cross-checked with Xenia Canary's layouts) and a default; values
// the guest writes are kept in xconfig.txt in the data directory ("C.SS = hex bytes", one line per setting) and win
// over the defaults on later runs. Defaults: the dashboard's factory reset (dash 0x92281078) writes zeros to the user,
// media-center and console categories, so zero is the console's own fresh value for most settings; the values that
// describe this console (region, language, time zone, video, MAC address, "dashboard set up") come from
// console_settings.ini and the PC. A setting not in the table stops the runtime, so each one is added with its real
// size instead of being guessed. Whole-category reads/writes (setting 0) are not supported yet and stop too.

#include "kernel/kernel.h"
#include "settings.h"
#include "xui/host.h"

#include <windows.h>

#include <format>
#include <fstream>
#include <map>
#include <mutex>
#include <sstream>
#include <vector>

namespace
{
    using Bytes = std::vector<uint8_t>;

    constexpr uint32_t kStatusBufferTooSmall = 0xC0000023;
    constexpr uint32_t kStatusInvalidParameter = 0xC000000D;

    Bytes BigEndian16(uint32_t value) { return { uint8_t(value >> 8), uint8_t(value) }; }
    Bytes BigEndian32(uint32_t value)
    {
        return { uint8_t(value >> 24), uint8_t(value >> 16), uint8_t(value >> 8), uint8_t(value) };
    }

    // Short zone name as the console stores it (4 characters, e.g. "PST"): the initials of the Windows name.
    Bytes ZoneAbbreviation(const wchar_t* name)
    {
        Bytes out(4, 0);
        size_t n = 0;
        bool wordStart = true;
        for (const wchar_t* c = name; *c && n < 4; c++)
        {
            if (*c == L' ')
                wordStart = true;
            else if (wordStart)
            {
                out[n++] = uint8_t(*c < 0x80 ? *c : '?');
                wordStart = false;
            }
        }
        return out;
    }

    // Transition rule as the console stores it: month, week (5 = last), day of week, hour; zero without DST.
    Bytes ZoneDate(const SYSTEMTIME& t)
    {
        return { uint8_t(t.wMonth), uint8_t(t.wDay), uint8_t(t.wDayOfWeek), uint8_t(t.wHour) };
    }

    // XCONFIG_USER_TIME_ZONE_* (settings 1-7): the console's time zone is the PC's (biases in minutes, UTC = local +
    // bias). Zones without daylight saving have an empty DST name, zero dates and zero biases, as the dashboard's own
    // table does.
    Bytes TimeZoneSetting(uint32_t setting)
    {
        TIME_ZONE_INFORMATION tz;
        GetTimeZoneInformation(&tz);
        const bool dst = tz.StandardDate.wMonth != 0;
        switch (setting)
        {
        case 1: return BigEndian32(uint32_t(tz.Bias));
        case 2: return ZoneAbbreviation(tz.StandardName);
        case 3: return dst ? ZoneAbbreviation(tz.DaylightName) : Bytes(4, 0);
        case 4: return dst ? ZoneDate(tz.StandardDate) : Bytes(4, 0);
        case 5: return dst ? ZoneDate(tz.DaylightDate) : Bytes(4, 0);
        case 6: return BigEndian32(dst ? uint32_t(tz.StandardBias) : 0);
        default: return BigEndian32(dst ? uint32_t(tz.DaylightBias) : 0);
        }
    }

    // XCONFIG_USER_RETAIL_FLAGS: a console whose dashboard setup is done (0x40, DashboardInitialized), no Kinect,
    // default startup options, and the clock options of the PC: 0x2 = daylight saving adjustment off,
    // 0x8 = 24-hour clock.
    uint32_t RetailFlags()
    {
        constexpr uint32_t kDstOff = 0x2, kTwentyFourHourClock = 0x8, kDashboardInitialized = 0x40;
        uint32_t flags = kDashboardInitialized;
        DYNAMIC_TIME_ZONE_INFORMATION tz;
        if (GetDynamicTimeZoneInformation(&tz) != TIME_ZONE_ID_INVALID && tz.DynamicDaylightTimeDisabled)
            flags |= kDstOff;
        wchar_t clock[4] = {};
        if (GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_ITIME, clock, 4) && clock[0] == L'1')
            flags |= kTwentyFourHourClock;
        return flags;
    }

    // XCONFIG_SECURED_AV_REGION from the video standard (1 NTSC-M, 2 NTSC-J, 3 PAL).
    uint32_t AvRegion()
    {
        switch (Settings().videoStandard)
        {
        case 2: return 0x00400200;
        case 3: return 0x00400400;
        default: return 0x00400100;
        }
    }

    // XCONFIG_SECURED_DVD_REGION from the game region, as consoles are sold: North America 1, Japan and Europe 2,
    // rest of Asia 3, Australia / New Zealand 4, China 6. "Rest of world" consoles span several DVD regions: 0
    // (none recorded) rather than a guess.
    uint32_t DvdRegion()
    {
        const uint32_t region = Settings().gameRegion;
        switch (region >> 8)
        {
        case 0: return 1;
        case 1: return region == 0x0101 ? 2 : region == 0x0102 ? 6 : 3;
        case 2: return region == 0x0201 ? 4 : 2;
        default: return 0;
        }
    }

    struct Setting
    {
        uint16_t category;
        uint16_t id;
        uint16_t size;
        const char* name;
        Bytes (*fresh)(); // non-zero default; null = zeros
    };

    // The settings the 17559 dashboard and TU46 read or write (dash.xex's ExGet/ExSetXConfigSetting buffers give the
    // sizes; names from Xenia Canary's xconfig.h). Zeros unless `fresh` says otherwise.
    const Setting kSettings[] = {
        { 0, 0x01, 5, "STATIC_FIRST_POWER_ON_DATE", nullptr },
        { 0, 0x02, 0x100, "STATIC_SMC_CONFIG", nullptr },
        { 2, 0x01, 6, "SECURED_MAC_ADDRESS", [] { const auto& m = Settings().macAddress; return Bytes(m.begin(), m.end()); } },
        { 2, 0x02, 4, "SECURED_AV_REGION", [] { return BigEndian32(AvRegion()); } },
        { 2, 0x03, 2, "SECURED_GAME_REGION", [] { return BigEndian16(Settings().gameRegion); } },
        { 2, 0x04, 4, "SECURED_DVD_REGION", [] { return BigEndian32(DvdRegion()); } },
        { 2, 0x05, 4, "SECURED_RESET_KEY", nullptr },
        { 2, 0x06, 4, "SECURED_SYSTEM_FLAGS", nullptr },
        { 3, 0x01, 4, "USER_TIME_ZONE_BIAS", [] { return TimeZoneSetting(1); } },
        { 3, 0x02, 4, "USER_TIME_ZONE_STD_NAME", [] { return TimeZoneSetting(2); } },
        { 3, 0x03, 4, "USER_TIME_ZONE_DLT_NAME", [] { return TimeZoneSetting(3); } },
        { 3, 0x04, 4, "USER_TIME_ZONE_STD_DATE", [] { return TimeZoneSetting(4); } },
        { 3, 0x05, 4, "USER_TIME_ZONE_DLT_DATE", [] { return TimeZoneSetting(5); } },
        { 3, 0x06, 4, "USER_TIME_ZONE_STD_BIAS", [] { return TimeZoneSetting(6); } },
        { 3, 0x07, 4, "USER_TIME_ZONE_DLT_BIAS", [] { return TimeZoneSetting(7); } },
        { 3, 0x08, 8, "USER_DEFAULT_PROFILE", nullptr },
        { 3, 0x09, 4, "USER_LANGUAGE", [] { return BigEndian32(Settings().language); } },
        // Bit 0x10000 = widescreen (the dashboard's display settings toggle it); 0x400000 and 0x100000 are other
        // display options the dashboard sets. Not XGetVideoFlags' 1/2/4 bits.
        { 3, 0x0A, 4, "USER_VIDEO_FLAGS", [] { return BigEndian32(Settings().widescreen ? 0x10000 : 0); } },
        { 3, 0x0B, 4, "USER_AUDIO_FLAGS", nullptr },
        { 3, 0x0C, 4, "USER_RETAIL_FLAGS", [] { return BigEndian32(RetailFlags()); } },
        { 3, 0x0D, 4, "USER_DEVKIT_FLAGS", nullptr },
        { 3, 0x0E, 1, "USER_COUNTRY", [] { return Bytes{ kernel::XConfigCountryFromLocale(Settings().locale) }; } },
        { 3, 0x0F, 1, "USER_PC_FLAGS", nullptr },
        { 3, 0x11, 8, "USER_LIVE_PUID", nullptr },
        { 3, 0x12, 0x10, "USER_LIVE_CREDENTIALS", nullptr },
        { 3, 0x13, 4, "USER_AV_COMPOSITE_SCREENSZ", nullptr },
        { 3, 0x14, 4, "USER_AV_COMPONENT_SCREENSZ", nullptr },
        { 3, 0x15, 4, "USER_AV_VGA_SCREENSZ", nullptr },
        { 3, 0x16, 4, "USER_PC_GAME", nullptr },
        { 3, 0x17, 4, "USER_PC_PASSWORD", nullptr },
        { 3, 0x18, 4, "USER_PC_MOVIE", nullptr },
        { 3, 0x19, 4, "USER_PC_GAME_RATING", nullptr },
        { 3, 0x1A, 4, "USER_PC_MOVIE_RATING", nullptr },
        { 3, 0x1B, 1, "USER_PC_HINT", nullptr },
        { 3, 0x1C, 0x20, "USER_PC_HINT_ANSWER", nullptr },
        { 3, 0x22, 4, "USER_PC_VERSION", nullptr },
        { 3, 0x23, 4, "USER_PC_TV", nullptr },
        { 3, 0x24, 4, "USER_PC_TV_RATING", nullptr },
        { 3, 0x25, 4, "USER_PC_EXPLICIT_VIDEO", nullptr },
        { 3, 0x26, 4, "USER_PC_EXPLICIT_VIDEO_RATING", nullptr },
        { 3, 0x27, 4, "USER_PC_UNRATED_VIDEO", nullptr },
        { 3, 0x20, 4, "USER_20", nullptr }, // xam.xex reads and writes 4 bytes (0x81767F28)
        { 3, 0x28, 4, "USER_PC_UNRATED_VIDEO_RATING", nullptr },
        { 3, 0x29, 4, "USER_VIDEO_OUTPUT_BLACK_LEVELS", nullptr },
        { 3, 0x2B, 4, "USER_ALTERNATE_VIDEO_TIMING_ID", nullptr },
        { 3, 0x2D, 4, "USER_MUSIC_UI_FLAGS", nullptr },
        { 3, 0x2E, 1, "USER_VIDEO_MEDIA_SOURCE_TYPE", nullptr },
        { 3, 0x2F, 1, "USER_MUSIC_MEDIA_SOURCE_TYPE", nullptr },
        { 3, 0x30, 1, "USER_PHOTO_MEDIA_SOURCE_TYPE", nullptr },
        { 4, 0x01, 0x1EC, "XNET_MACHINE_ACCOUNT_DATA", nullptr }, // xam.xex 0x81763B90: 0x1EC bytes
        { 5, 0x01, 0x1EC, "XNET_PARAMETERS_DATA", nullptr }, // xam.xex writes 0x1EC bytes (0x81763A88)
        { 6, 0x01, 0x14, "MEDIA_CENTER_MEDIA_PLAYER", nullptr },
        { 6, 0x02, 10, "MEDIA_CENTER_XESLED_VERSION", nullptr },
        { 6, 0x03, 0x14, "MEDIA_CENTER_XESLED_TRUST_SECRET", nullptr },
        { 6, 0x04, 8, "MEDIA_CENTER_XESLED_TRUST_CODE", nullptr },
        { 6, 0x05, 0x14, "MEDIA_CENTER_XESLED_HOST_ID", nullptr },
        { 6, 0x06, 0x65C, "MEDIA_CENTER_XESLED_KEY", nullptr },
        { 6, 0x07, 6, "MEDIA_CENTER_XESLED_HOST_MAC_ADDRESS", nullptr },
        { 6, 0x08, 0x10, "MEDIA_CENTER_SERVER_UUID", nullptr },
        { 6, 0x09, 0x80, "MEDIA_CENTER_SERVER_NAME", nullptr },
        { 7, 0x01, 2, "CONSOLE_SCREEN_SAVER", nullptr },
        { 7, 0x02, 2, "CONSOLE_AUTO_SHUT_OFF", nullptr },
        { 7, 0x04, 4, "CONSOLE_CAMERA_SETTINGS", nullptr },
        { 7, 0x06, 2, "CONSOLE_MEDIA_DISABLEAUTOLAUNCH", nullptr },
        { 7, 0x07, 2, "CONSOLE_KEYBOARD_LAYOUT", nullptr },
        { 7, 0x08, 100, "CONSOLE_PC_TITLE_EXEMPTIONS", nullptr },
        { 7, 0x09, 0x1C, "CONSOLE_NUI", nullptr },
        // The audio ducker level (party chat lowers other sound): 1-10, xam.xex takes 6 for 0 / unset (0x81902800,
        // XAudioSetDuckerLevel).
        { 7, 0x0A, 1, "CONSOLE_DUCKER_LEVEL", nullptr },
        { 7, 0x0B, 4, "CONSOLE_RETAIL_EX_FLAGS", nullptr },
        { 7, 0x0D, 2, "CONSOLE_TV_DIAGONAL_SIZE_IN_CM", nullptr },
        // Cloud saved games (XAM 0x816A7058): zeros = never set up, "Setting Disabled" = turned off, anything else =
        // enabled. A console that never went online has zeros.
        { 7, 0x0E, 0x14, "CONSOLE_NETWORK_STORAGE", nullptr },
        { 7, 0x0F, 1, "CONSOLE_DISCOVERABLE", nullptr },
        { 7, 0x10, 4, "CONSOLE_LIVE_TV_PROVIDER", nullptr },
        { 7, 0x12, 1, "CONSOLE_12", nullptr },
        { 7, 0x13, 4, "CONSOLE_13", nullptr },
        { 7, 0x14, 0x1A, "CONSOLE_14", nullptr },
        { 7, 0x15, 2, "CONSOLE_15", nullptr },
        { 7, 0x16, 2, "CONSOLE_16", nullptr },
        { 7, 0x17, 4, "CONSOLE_17", nullptr }, // XamVerifyUserSetting0x17 (xam.xex 0x8168EAF0) asks for 4 bytes
        { 8, 0x01, 0x14, "DVD_VOLUME_ID", nullptr },
        { 8, 0x02, 0x280, "DVD_BOOKMARK", nullptr },
        { 9, 0x01, 0x78, "IPTV_SERVICE_PROVIDER_NAME", nullptr }, // WCHAR[60] (XamIptvGetServiceName)
        { 9, 0x03, 0x80, "IPTV_SUPPORT_INFO", nullptr },
        // Category 10: the controllers' authentication delay window (two 64-bit values, min and max, cached from the
        // LIVE hive's RgcAuthenticationDelayMin / Max and passed to XamInputSetMinMaxAuthDelay; xam.xex 0x816E6D50).
        { 10, 0x03, 0x10, "CONTROLLER_AUTH_DELAY", nullptr },
        // The dashboard version whose first-run experience the console has had (major << 28 | minor << 24 | build << 8
        // | qfe): the 9199 XAM's XamFirstRunExperienceShouldRun reads it and stamps its own version (xam_blades.cpp).
        { 10, 0x02, 4, "FIRST_RUN_VERSION", nullptr },
        // Further settings xam.xex reads or writes, with the sizes it uses (its every constant-size Ex*XConfigSetting).
        { 1, 0x02, 4, "STATISTIC_02", nullptr },
        { 1, 0x03, 32, "STATISTIC_03", nullptr },
        { 1, 0x05, 100, "STATISTIC_05", nullptr },
        { 1, 0x06, 96, "STATISTIC_06", nullptr },
        { 1, 0x07, 8, "STATISTIC_07", nullptr },
        { 1, 0x08, 101, "STATISTIC_08", nullptr },
        { 2, 0x08, 4, "SECURED_08", nullptr },
        { 3, 0x1E, 4, "USER_1E", nullptr },
        { 3, 0x1F, 4, "USER_1F", nullptr },
        { 7, 0x05, 0x14, "CONSOLE_PLAY_TIMER_DATA", nullptr },
        { 7, 0x18, 4, "CONSOLE_18", nullptr }, // XamVerifyUserSetting0x18 (xam.xex 0x8168EA60) asks for 4 bytes
        { 11, 0x03, 4, "CATEGORY_B_03", nullptr },
    };

    const Setting* FindSetting(uint32_t category, uint32_t id)
    {
        for (const Setting& s : kSettings)
            if (s.category == category && s.id == id)
                return &s;
        return nullptr;
    }

    const Setting& RequireSetting(const char* function, uint32_t category, uint32_t id)
    {
        if (id == 0)
            Fatal("{}: whole-category access (category {:X}) is not implemented", function, category);
        const Setting* s = FindSetting(category, id);
        if (s == nullptr)
            Fatal("{}: category {:X} setting {:X} is not in the XConfig table (kernel/xboxkrnl_xconfig.cpp)",
                function, category, id);
        return *s;
    }

    std::mutex g_mutex;
    bool g_loaded = false;
    std::map<std::pair<uint16_t, uint16_t>, Bytes> g_stored; // values the guest wrote (xconfig.txt)

    std::filesystem::path StorePath()
    {
        return DataDirectory() / "xconfig.txt";
    }

    void LoadStore()
    {
        if (g_loaded)
            return;
        g_loaded = true;
        std::ifstream in(StorePath());
        std::string line;
        while (std::getline(in, line))
        {
            if (line.empty() || line[0] == ';')
                continue;
            unsigned category = 0, id = 0;
            char hex[2048] = {};
            if (sscanf_s(line.c_str(), "%x.%x = %2047s", &category, &id, hex, unsigned(sizeof(hex))) != 3)
                Fatal("xconfig.txt: cannot read the line \"{}\"", line);
            const Setting* s = FindSetting(category, id);
            Bytes value;
            for (size_t i = 0; hex[i] && hex[i + 1]; i += 2)
                value.push_back(uint8_t(std::stoul(std::string(hex + i, 2), nullptr, 16)));
            if (s == nullptr || value.size() != s->size)
                Fatal("xconfig.txt: {:X}.{:02X} is unknown or has {} bytes", category, id, value.size());
            g_stored[{ uint16_t(category), uint16_t(id) }] = value;
        }
    }

    void SaveStore()
    {
        std::ostringstream out;
        out << "; XConfig settings the dashboard or a title changed (category.setting = bytes). Delete a line to return\n"
               "; to the default from console_settings.ini; delete the file for a console's fresh settings.\n";
        for (const auto& [key, value] : g_stored)
        {
            std::string hex;
            for (uint8_t b : value)
                hex += std::format("{:02X}", b);
            out << std::format("{:X}.{:02X} = {} ; {}\n", key.first, key.second, hex,
                FindSetting(key.first, key.second)->name);
        }
        std::ofstream(StorePath(), std::ios::trunc) << out.str();
    }

    Bytes ReadSetting(const Setting& s)
    {
        std::lock_guard lock(g_mutex);
        LoadStore();
        if (auto it = g_stored.find({ s.category, s.id }); it != g_stored.end())
            return it->second;
        Bytes value = s.fresh ? s.fresh() : Bytes(s.size, 0);
        if (value.size() != s.size)
            Fatal("XConfig {}: default has {} bytes, the setting {}", s.name, value.size(), s.size);
        return value;
    }

    void WriteSetting(const Setting& s, Bytes value)
    {
        std::lock_guard lock(g_mutex);
        LoadStore();
        g_stored[{ s.category, s.id }] = std::move(value);
        SaveStore();
    }

    // XAM's locale table (xam 0x81A81848: online country, XConfig country, XC_LOCALE, ...): the XConfig country of
    // each locale, the first row naming it. Locale 38 is the one row where the two countries differ (111 / 236).
    constexpr uint8_t kXConfigCountryOfLocale[44] = {
        0, 6, 5, 8, 13, 16, 19, 20, 21, 23, 25, 32, 34, 24, 37, 39, 42, 46, 44, 50, 53, 56, 71, 74, 76, 75, 82, 84, 91,
        93, 109, 31, 90, 18, 101, 35, 103, 88, 111, 99, 4, 89, 45, 1 };
}

uint8_t kernel::XConfigCountryFromLocale(uint32_t locale)
{
    return locale < std::size(kXConfigCountryOfLocale) ? kXConfigCountryOfLocale[locale] : 0;
}

std::vector<uint8_t> kernel::XConfigRead(uint32_t category, uint32_t id)
{
    return ReadSetting(RequireSetting("XConfigRead", category, id));
}

uint32_t kernel::XConfigRead32(uint32_t category, uint32_t id)
{
    const Setting& s = RequireSetting("XConfigRead32", category, id);
    if (s.size != 4)
        Fatal("XConfigRead32: {} has {} bytes", s.name, s.size);
    const Bytes value = ReadSetting(s);
    return uint32_t(value[0]) << 24 | uint32_t(value[1]) << 16 | uint32_t(value[2]) << 8 | value[3];
}

void kernel::XConfigWrite(uint32_t category, uint32_t id, const std::vector<uint8_t>& value)
{
    const Setting& s = RequireSetting("XConfigWrite", category, id);
    if (value.size() != s.size)
        Fatal("XConfigWrite: {} with {} bytes (the setting has {})", s.name, value.size(), s.size);
    WriteSetting(s, Bytes(value.begin(), value.end()));
    Log("XConfig: {} set", s.name);
}

bool kernel::TwentyFourHourClock()
{
    return (RetailFlags() & 0x8) != 0;
}

// The shared XUI library's host function (xui/host.h): the Guide's clock.
bool xui::host::TwentyFourHourClock()
{
    return kernel::TwentyFourHourClock();
}

// NTSTATUS ExGetXConfigSetting(WORD Category, WORD Setting, PVOID Buffer, WORD BufferSize, PWORD RequiredSize)
GUEST_IMPORT(ExGetXConfigSetting)
{
    auto& mem = guest::g_memory;
    if (!FindSetting(ctx.r3.u32 & 0xFFFF, ctx.r4.u32 & 0xFFFF))
        Fatal("ExGetXConfigSetting: category {:X} setting {:X} ({} bytes asked, from {:08X}) is not in the XConfig table",
              ctx.r3.u32 & 0xFFFF, ctx.r4.u32 & 0xFFFF, ctx.r6.u32 & 0xFFFF, uint32_t(ctx.lr));
    const Setting& s = RequireSetting("ExGetXConfigSetting", ctx.r3.u32 & 0xFFFF, ctx.r4.u32 & 0xFFFF);
    const uint32_t buffer = ctx.r5.u32;
    const uint32_t bufferSize = ctx.r6.u32 & 0xFFFF;
    if (ctx.r7.u32)
        mem.Write16(ctx.r7.u32, s.size);
    if (buffer == 0)
    {
        ctx.r3.u64 = bufferSize ? kStatusInvalidParameter : kernel::kStatusSuccess;
        return;
    }
    if (bufferSize < s.size)
    {
        ctx.r3.u64 = kStatusBufferTooSmall;
        return;
    }
    const Bytes value = ReadSetting(s);
    memcpy(mem.Translate(buffer), value.data(), value.size());
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS ExSetXConfigSetting(WORD Category, WORD Setting, PVOID Buffer, WORD BufferSize)
GUEST_IMPORT(ExSetXConfigSetting)
{
    const Setting& s = RequireSetting("ExSetXConfigSetting", ctx.r3.u32 & 0xFFFF, ctx.r4.u32 & 0xFFFF);
    const uint32_t buffer = ctx.r5.u32;
    const uint32_t size = ctx.r6.u32 & 0xFFFF;
    if (buffer == 0 || size != s.size)
        Fatal("ExSetXConfigSetting {} with {} bytes at {:08X} (the setting has {})", s.name, size, buffer, s.size);
    const uint8_t* p = guest::g_memory.Translate(buffer);
    WriteSetting(s, Bytes(p, p + size));
    Log("XConfig: {} set", s.name);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS ExReadModifyWriteXConfigSettingUlong(WORD Category, WORD Setting, ULONG KeepMask, ULONG SetBits): a
// 32-bit setting becomes (value & KeepMask) | SetBits (the dash clears and sets retail flag bits, 0x9218A5A0).
GUEST_IMPORT(ExReadModifyWriteXConfigSettingUlong)
{
    const uint32_t category = ctx.r3.u32 & 0xFFFF, id = ctx.r4.u32 & 0xFFFF;
    const uint32_t value = (kernel::XConfigRead32(category, id) & ctx.r5.u32) | ctx.r6.u32;
    kernel::XConfigWrite(category, id, { uint8_t(value >> 24), uint8_t(value >> 16), uint8_t(value >> 8), uint8_t(value) });
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// DWORD XGetVideoStandard() (XAM 0x816FB280): bits 8-15 of the video flags setting (category 2, setting 2); 0 when
// it cannot be read. The older dashboards import it.
GUEST_IMPORT(XGetVideoStandard)
{
    ctx.r3.u64 = kernel::XConfigRead32(2, 2) >> 8 & 0xFF;
}

// DWORD XGetLanguage() (XAM 0x81698E60): the console's language (user setting 3/9) when it is 1-9; otherwise from
// the game region: Japan (0x101) 2, other Asian regions (0x1xx but 0x102) 1 or, for the setting 3/0xE starting '8',
// 7; everything else 1. The older dashboards import it (XamGetLanguage in 17559's).
GUEST_IMPORT(XGetLanguage)
{
    const uint32_t language = kernel::XConfigRead32(3, 9);
    if (language != 0 && language < 10)
    {
        ctx.r3.u64 = language;
        return;
    }
    const uint32_t region = Settings().gameRegion;
    if (region == 0x101)
        ctx.r3.u64 = 2;
    else if ((region & 0xFF00) == 0x100 && region != 0x102)
        Fatal("XGetLanguage: no valid language setting ({}) in game region {:X} (setting 3/0xE is not read here)",
            language, region);
    else
        ctx.r3.u64 = 1;
}
