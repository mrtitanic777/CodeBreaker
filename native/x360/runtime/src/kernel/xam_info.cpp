// Console configuration queries (AV pack, video mode, region, language, locale, system version).
// Values come from console_settings.ini (see settings.h).

#include "kernel/kernel.h"
#include "pool.h"
#include "settings.h"
#include "xex.h"
#include "xui/package.h"

#include <windows.h>

#include <cstring>
#include <filesystem>
#include <format>
#include <iterator>
#include <map>
#include <mutex>

namespace
{
    // X_VIDEO_MODE (48 bytes): DisplayWidth, DisplayHeight, IsInterlaced, IsWidescreen, IsHiDef, RefreshRate (float),
    // VideoStandard, PixelRate, WidescreenFlag, Reserved[3].
    void WriteVideoMode(uint32_t mode)
    {
        auto& mem = guest::g_memory;
        const ConsoleSettings& s = Settings();
        float refresh = s.refreshRate;
        uint32_t refreshBits;
        memcpy(&refreshBits, &refresh, 4);
        mem.Write32(mode + 0x00, s.videoWidth);
        mem.Write32(mode + 0x04, s.videoHeight);
        mem.Write32(mode + 0x08, s.interlaced ? 1 : 0);
        mem.Write32(mode + 0x0C, s.widescreen ? 1 : 0);
        mem.Write32(mode + 0x10, s.videoWidth >= 1280 ? 1 : 0);
        mem.Write32(mode + 0x14, refreshBits);
        mem.Write32(mode + 0x18, s.videoStandard);
        mem.Write32(mode + 0x1C, 0x8A);                        // pixel rate code (value observed on hardware per Canary)
        mem.Write32(mode + 0x20, s.widescreen ? 0x01 : 0x03);
        mem.Write32(mode + 0x24, 0);
        mem.Write32(mode + 0x28, 0);
        mem.Write32(mode + 0x2C, 0);
    }
}

// DWORD XGetAVPack()
GUEST_IMPORT(XGetAVPack)
{
    ctx.r3.u64 = Settings().avPack;
}

// ULONG HalGetCurrentAVPack(): the kernel's AV pack, the same codes as XGetAVPack (XAM's XGetAVPack forwards to it).
GUEST_IMPORT(HalGetCurrentAVPack)
{
    ctx.r3.u64 = Settings().avPack;
}

// VOID XGetVideoMode(PXVIDEO_MODE VideoMode)
GUEST_IMPORT(XGetVideoMode)
{
    WriteVideoMode(ctx.r3.u32);
}

// VOID VdQueryVideoMode(PXVIDEO_MODE VideoMode)
GUEST_IMPORT(VdQueryVideoMode)
{
    WriteVideoMode(ctx.r3.u32);
}

// DWORD VdQueryVideoFlags(): XC_VIDEO_FLAGS_WIDESCREEN 1, HDTV_720p 2, HDTV_1080i 4.
GUEST_IMPORT(VdQueryVideoFlags)
{
    const ConsoleSettings& s = Settings();
    ctx.r3.u64 = (s.widescreen ? 1u : 0u) | (s.videoWidth >= 1280 ? 2u : 0u) | (s.videoWidth >= 1920 ? 4u : 0u);
}

// DWORD XGetVideoCapabilities(): stereoscopic 3D capability bits; none.
GUEST_IMPORT(XGetVideoCapabilities)
{
    ctx.r3.u64 = 0;
}

// DWORD XGetGameRegion()
GUEST_IMPORT(XGetGameRegion)
{
    ctx.r3.u64 = Settings().gameRegion;
}

// DWORD XamGetLanguage()
GUEST_IMPORT(XamGetLanguage)
{
    ctx.r3.u64 = Settings().language;
}

namespace
{
    // XamGetLocaleEx's choice: the configured locale if the caller supports it, otherwise a fallback from the game
    // region (XAM 17559: Japan 20 / Korea 21 in Asia, Great Britain 35 in Europe, United States 36 elsewhere).
    uint32_t Locale(uint32_t maxLocale)
    {
        const ConsoleSettings& s = Settings();
        uint32_t locale = s.locale;
        if (locale > (maxLocale & 0xFF))
        {
            const uint32_t region = (s.gameRegion >> 8) & 0xFF;
            if (region == 1)
                locale = s.gameRegion == 0x0101 ? 20 : 21;
            else
                locale = region == 2 ? 35 : 36;
        }
        return locale;
    }

    // XAM's tables (xam 0x81A81848 and 0x81A815D8), first row naming each value: the Xbox LIVE country of an
    // XC_LOCALE, and the Xbox LIVE language of an XC_LANGUAGE (only 13, 14 and 17 differ; 10 has none).
    constexpr uint8_t kOnlineCountryOfLocale[44] = {
        0, 6, 5, 8, 13, 16, 19, 20, 21, 23, 25, 32, 34, 24, 37, 39, 42, 46, 44, 50, 53, 56, 71, 74, 76, 75, 82, 84, 91,
        93, 109, 31, 90, 18, 101, 35, 103, 88, 236, 99, 4, 89, 45, 1 };

    uint8_t OnlineLanguage(uint32_t language)
    {
        switch (language)
        {
        case 13: return 17;
        case 14: return 36;
        case 17: return 10;
        default: return language <= 16 && language != 10 ? uint8_t(language) : 0;
        }
    }
}

// DWORD XamGetLocaleEx(DWORD MaxCountryId, DWORD MaxLocaleId)
GUEST_IMPORT(XamGetLocaleEx)
{
    ctx.r3.u64 = Locale(ctx.r4.u32);
}

// DWORD XamGetLocale(): XAM 17559 calls XamGetLocaleEx(0x6F, 0x2B), every country and locale it knows.
GUEST_IMPORT(XamGetLocale)
{
    ctx.r3.u64 = Locale(0x2B);
}

// VOID XamGetOnlineLanguageAndCountry(XUID Xuid, BYTE* Language, BYTE* Country): a signed-in Xbox LIVE account's
// country and language; for anyone else (no XUID, offline profiles) those of the console's locale and language. This
// console has no Xbox LIVE accounts.
GUEST_IMPORT(XamGetOnlineLanguageAndCountry)
{
    auto& mem = guest::g_memory;
    const uint32_t locale = Locale(0x2B);
    if (ctx.r5.u32)
        mem.Write8(ctx.r5.u32, locale < std::size(kOnlineCountryOfLocale) ? kOnlineCountryOfLocale[locale] : 0);
    if (ctx.r4.u32)
        mem.Write8(ctx.r4.u32, OnlineLanguage(Settings().language));
}

// DWORD XamGetSystemVersion()
GUEST_IMPORT(XamGetSystemVersion)
{
    ctx.r3.u64 = Settings().systemVersion;
}

// ---- Fonts and locale strings ------------------------------------------------------------------------------------

namespace
{
    // A constant UTF-16 (big endian) string in guest memory, allocated once: XAM returns pointers into its own data.
    uint32_t GuestConstantString(const char* text)
    {
        static std::mutex mutex;
        static std::map<std::string, uint32_t> strings;
        std::lock_guard lock(mutex);
        if (auto it = strings.find(text); it != strings.end())
            return it->second;
        auto& mem = guest::g_memory;
        const size_t length = strlen(text);
        const uint32_t address = mem.SystemAlloc(uint32_t(length + 1) * 2);
        for (size_t i = 0; i <= length; i++)
            mem.Write16(address + uint32_t(i) * 2, uint8_t(text[i]));
        strings[text] = address;
        return address;
    }

    // StringCchCopyW into a guest buffer of cch characters: 0x80070057 (E_INVALIDARG) for cch 0 or above
    // 0x7FFFFFFF, 0x8007007A (STRSAFE_E_INSUFFICIENT_BUFFER) when truncated (the copy is still terminated).
    uint32_t CopyWide(uint32_t buffer, uint32_t cch, const char* text)
    {
        if (cch == 0 || cch > 0x7FFFFFFF)
            return 0x80070057;
        auto& mem = guest::g_memory;
        const size_t length = strlen(text);
        const size_t copied = std::min<size_t>(length, cch - 1);
        for (size_t i = 0; i < copied; i++)
            mem.Write16(buffer + uint32_t(i) * 2, uint8_t(text[i]));
        mem.Write16(buffer + uint32_t(copied) * 2, 0);
        return copied < length ? 0x8007007A : 0;
    }

    // XAM's language argument: 0 or anything above 0x11 means the console's language.
    uint32_t TypefaceLanguage(uint32_t language)
    {
        return language == 0 || language > 0x11 ? Settings().language : language;
    }

    constexpr uint32_t kLanguageTChinese = 8;
    constexpr uint32_t kLanguageSChinese = 0x11;
}

// HRESULT XamGetLanguageTypeface(DWORD Language, DWORD cchBuffer, LPWSTR Buffer) (XAM 0x81699810): the URL of the
// language's font in flash: XenonCLatin for traditional Chinese, XenonJKLatin (Latin, Japanese, Korean) for every
// other language. Simplified Chinese's XenonSCLatin.xtt lives in the HDD system update (XAM asks its package manager
// for that path).
GUEST_IMPORT(XamGetLanguageTypeface)
{
    const uint32_t language = TypefaceLanguage(ctx.r3.u32);
    if (language == kLanguageSChinese)
        Fatal("XamGetLanguageTypeface: simplified Chinese (XenonSCLatin.xtt from the HDD system update) is not "
            "implemented");
    ctx.r3.u64 = CopyWide(ctx.r5.u32, ctx.r4.u32, language == kLanguageTChinese ? "file://media:/XenonCLatin.xtt" :
        "file://media:/XenonJKLatin.xtt");
}

// LPCWSTR XamGetLanguageTypefacePatch(DWORD Language) (XAM 0x81699190): the font patch of the flash's active patch
// slot (XboxHardwareInfo +0xE bit 0x1000 = slot 1, 0x2000 = slot 2, neither = no patch), NULL without one.
GUEST_IMPORT(XamGetLanguageTypefacePatch)
{
    const uint32_t language = TypefaceLanguage(ctx.r3.u32);
    const uint16_t bootFlags = kernel::BootLoaderFlags();
    const char* slot = (bootFlags & 0x1000) ? "1" : (bootFlags & 0x2000) ? "2" : nullptr;
    if (slot == nullptr)
    {
        ctx.r3.u64 = 0;
        return;
    }
    const char* font = language == kLanguageSChinese ? "XenonSCLatin" : language == kLanguageTChinese ? "XenonCLatin" :
        "XenonJKLatin";
    ctx.r3.u64 = GuestConstantString(std::format("file://media:/{}.xttp{}", font, slot).c_str());
}

// LPCWSTR XamGetLanguageLocaleFallbackString(DWORD Language) (XAM 0x81698DC8): the language's culture name from XAM's
// table 0x81A817C0, NULL for 0 and English.
GUEST_IMPORT(XamGetLanguageLocaleFallbackString)
{
    static constexpr const char* kCultures[18] = { nullptr, nullptr, "ja-JP", "de-DE", "fr-FR", "es-ES", "it-IT",
        "ko-KR", "zh-TW", "pt-BR", "zh-CHS", "pl-PL", "ru-RU", "sv-SE", "tr-TR", "nb-NO", "nl-NL", "zh-CHS" };
    const uint32_t language = ctx.r3.u32 & 0xFF;
    if (language >= std::size(kCultures))
        Fatal("XamGetLanguageLocaleFallbackString({}): outside XAM's table", language);
    ctx.r3.u64 = kCultures[language] ? GuestConstantString(kCultures[language]) : 0;
}

// DWORD XGetAudioFlags() (XAM 0x816FB0F8): XConfig USER_AUDIO_FLAGS, 0x00010001 when unset; always 2 on AV pack 2.
GUEST_IMPORT(XGetAudioFlags)
{
    uint32_t flags = kernel::XConfigRead32(3, 0x0B);
    if (flags == 0)
        flags = 0x00010001;
    ctx.r3.u64 = Settings().avPack == 2 ? 2 : flags;
}

namespace
{
    // XAM's online language (xam 0x81A815D8, 61 rows) and country (0x81A81848, 237 rows) tables: 8-byte rows indexed
    // by the online value, with a pointer to the code ("en", "US") at +4. Read from the user's xam.xex.
    std::string OnlineCode(uint32_t table, uint32_t rows, uint32_t index)
    {
        const xex::HostImage& xam = kernel::XamImage();
        const uint8_t* row = index < rows ? xam.At(table + index * 8, 8) : nullptr;
        if (row == nullptr)
            Fatal("XAM online code table {:08X}: no row {}", table, index);
        uint32_t address = uint32_t(row[4]) << 24 | row[5] << 16 | row[6] << 8 | row[7];
        std::string code;
        for (const uint8_t* c; (c = xam.At(address, 2)) != nullptr && (c[0] | c[1]) != 0; address += 2)
            code.push_back(char(c[1]));
        return code;
    }

    // StringCchCatW: appends to the terminated string in a buffer of cch characters.
    uint32_t AppendWide(uint32_t buffer, uint32_t cch, const char* text)
    {
        if (cch == 0 || cch > 0x7FFFFFFF)
            return 0x80070057;
        uint32_t length = 0;
        while (length < cch && guest::g_memory.Read16(buffer + length * 2) != 0)
            length++;
        if (length == cch)
            return 0x80070057;
        return CopyWide(buffer + length * 2, cch - length, text);
    }
}

// HRESULT XamProfileGetLiveLegalLocale(XUID Xuid, DWORD cch, LPWSTR Buffer): "<online language>-<online country>"
// (e.g. "en-US") from XamGetOnlineLanguageAndCountry's values, as XAM builds it: the language code, "-" (its result
// ignored), the country code (through a 7-character buffer); the last failing step's HRESULT, 0x8007007A when cut.
GUEST_IMPORT(XamProfileGetLiveLegalLocale)
{
    const uint32_t cch = ctx.r4.u32, buffer = ctx.r5.u32;
    const uint32_t locale = Locale(0x2B);
    const uint32_t country = locale < std::size(kOnlineCountryOfLocale) ? kOnlineCountryOfLocale[locale] : 0;
    const uint32_t language = OnlineLanguage(Settings().language);
    uint32_t result = CopyWide(buffer, cch, OnlineCode(0x81A815D8, 61, language).c_str());
    if (int32_t(result) >= 0)
    {
        AppendWide(buffer, cch, "-");
        std::string countryCode = OnlineCode(0x81A81848, 237, country);
        if (countryCode.size() > 6)
        {
            countryCode.resize(6);
            result = 0x8007007A;
        }
        else
            result = AppendWide(buffer, cch, countryCode.c_str());
    }
    ctx.r3.u64 = result;
}

// BYTE XamGetLanguageFromOnlineLanguage(BYTE OnlineLanguage) / BYTE XamGetLocaleFromOnlineCountry(BYTE OnlineCountry):
// byte 1 of the online language row / byte 2 of the online country row in XAM's tables (no bounds check in XAM).
GUEST_IMPORT(XamGetLanguageFromOnlineLanguage)
{
    const uint8_t* row = ctx.r3.u8 < 61 ? kernel::XamImage().At(0x81A815D8 + ctx.r3.u8 * 8, 8) : nullptr;
    if (row == nullptr)
        Fatal("XamGetLanguageFromOnlineLanguage({}): outside XAM's table", ctx.r3.u8);
    ctx.r3.u64 = row[1];
}

GUEST_IMPORT(XamGetLocaleFromOnlineCountry)
{
    const uint8_t* row = ctx.r3.u8 < 237 ? kernel::XamImage().At(0x81A81848 + ctx.r3.u8 * 8, 8) : nullptr;
    if (row == nullptr)
        Fatal("XamGetLocaleFromOnlineCountry({}): outside XAM's table", ctx.r3.u8);
    ctx.r3.u64 = row[2];
}

// HRESULT XamLogLocalizationEtx(DWORD EventId, LPCWSTR Text): XAM 17559 (0x81699918 -> 0x81699428 / 0x81699548)
// copies events 0x80300035 / 0x80300034's text (at most 0x10A characters, else 0x8007007A
// ERROR_INSUFFICIENT_BUFFER), splits it at the first space and writes both halves to an ETX producer, and returns
// S_OK whether or not the producer registers; other ids are E_NOTIMPL (0x80004001). ETX is a developer trace with no
// console-visible effect: the text is logged here instead.
GUEST_IMPORT(XamLogLocalizationEtx)
{
    const uint32_t id = ctx.r3.u32;
    if (id != 0x80300035 && id != 0x80300034)
    {
        ctx.r3.u64 = 0x80004001;
        return;
    }
    const std::u16string text = ctx.r4.u32 ? kernel::ReadGuestWideString(ctx.r4.u32, 0x10B) : std::u16string();
    if (text.size() >= 0x10B)
    {
        ctx.r3.u64 = 0x8007007A;
        return;
    }
    std::string narrow;
    for (const char16_t c : text)
        narrow += c < 0x80 ? char(c) : '?';
    Log("XamLogLocalizationEtx({:08X}): {}", id, narrow);
    ctx.r3.u64 = 0;
}

namespace
{
    // XAM's common string `index` (XamLookupCommonStringByIndex).
    uint32_t CommonString(uint32_t index);

    // SYSTEMTIME as XAM reads it: wYear +0, wMonth +2, wDay +6, wHour +8, wMinute +0xA, wSecond +0xC.
    SYSTEMTIME FileTimeFields(uint64_t fileTime)
    {
        const FILETIME ft{ DWORD(fileTime), DWORD(fileTime >> 32) };
        SYSTEMTIME st{};
        if (!FileTimeToSystemTime(&ft, &st))
            Fatal("XamFormat*String: FILETIME {:016X} is out of range", fileTime);
        return st;
    }

    uint8_t Country()
    {
        const auto bytes = kernel::XConfigRead(3, 0x0E); // XCONFIG_USER_COUNTRY
        uint32_t country = bytes.empty() ? 0 : bytes[0];
        // XamValidateCountry (0x816A18C8): 0 or past 0x6F -> one from the game region (and language in Europe).
        if (country == 0 || country > 0x6F)
        {
            const uint32_t region = Settings().gameRegion;
            switch (region >> 8 & 0xFF)
            {
            case 1:
                country = region == 0x101 ? 0x35 : region == 0x102 ? 0x14 : 0x38;
                break;
            case 2:
                if (region == 0x201)
                    country = 6;
                else
                    switch (Settings().language)
                    {
                    case 3: country = 0x18; break;
                    case 4: country = 0x22; break;
                    case 5: country = 0x1F; break;
                    case 6: country = 0x32; break;
                    case 9: country = 0x54; break;
                    case 0xB: country = 0x52; break;
                    case 0xC: country = 0x58; break;
                    default: country = 0x23; break;
                    }
                break;
            default:
                country = 0x67;
                break;
            }
        }
        return uint8_t(country);
    }

    // XamGetLocaleDateFormat(Country) (0x816BEB70): 0 D/M/Y, 1 D.M.Y, 2 DD/MM/YYYY, 3 M/D/Y, 4 Y/M/D, 5 YYYY-MM-DD
    // (each one's string in XamFormatSystemDateString).
    uint32_t LocaleDateFormat(uint32_t country)
    {
        switch (country)
        {
        case 1: case 4: case 5: case 0x10: case 0x12: case 0x15: case 0x18: case 0x19: case 0x1F: case 0x22:
        case 0x23: case 0x2D: case 0x2E: case 0x32: case 0x47: case 0x4B: case 0x54: case 0x58: case 0x59: case 99:
        case 0x6F:
            return 2;
        case 6: case 8: case 0x13: case 0x17: case 0x20: case 0x2C: case 0x4C: case 0x5D:
            return 1;
        case 0xD: case 0x25: case 0x27: case 0x4A: case 0x5B:
            return 0;
        case 0x14: case 0x65:
            return 4;
        case 0x2A: case 0x35: case 0x38: case 0x52: case 0x5A: case 0x6D:
            return 5;
        case 0x67:
            return 3;
        default:
            switch (Settings().gameRegion)
            {
            case 0xFF: return 3;
            case 0x101: return 5;
            case 0x102: return 4;
            default: return 2;
            }
        }
    }

    // XamGetLocaleTimeFormat(Alternate) (0x816BF018) for the console's country, then 0x816BEF30's switch to the
    // console clock (XCONFIG_USER_RETAIL_FLAGS 0x8): formats 0-3 are 24-hour, 4-11 12-hour.
    uint32_t LocaleTimeFormat(bool alternate)
    {
        uint32_t format = 6;
        switch (Country() - 1)
        {
        case 0: case 3: case 4: case 0xC: case 0x11: case 0x12: case 0x16: case 0x17: case 0x18: case 0x21:
        case 0x22: case 0x2B: case 0x2C: case 0x4A: case 0x51: case 0x58: case 0x59: case 0x62: case 0x6E:
            format = alternate ? 1 : 0;
            break;
        case 5: case 0x14: case 0x46:
            format = alternate ? 4 : 5;
            break;
        case 7: case 0x1E: case 0x1F: case 0x26: case 0x29: case 0x2D: case 0x31: case 0x34: case 0x49: case 0x53:
        case 0x57: case 0x5C:
            format = alternate ? 2 : 3;
            break;
        case 0xF: case 0x24: case 0x4B: case 0x66: case 0x6C:
            format = alternate ? 6 : 7;
            break;
        case 0x13: case 0x37: case 0x5A:
            format = alternate ? 8 : 9;
            break;
        case 100:
            format = alternate ? 10 : 11;
            break;
        }
        if (!kernel::TwentyFourHourClock())
        {
            static constexpr uint32_t kTwelveHour[] = { 5, 4, 6, 7 };
            return format < 4 ? kTwelveHour[format] : format;
        }
        static constexpr uint32_t kTwentyFourHour[] = { 1, 0, 2, 3, 2, 3, 1, 0 };
        return format >= 4 && format <= 11 ? kTwentyFourHour[format - 4] : format;
    }

    // XamFormatSystemDateString (0x816BEC08): the arguments each format string takes (widths for its '*'s).
    int32_t FormatDate(const SYSTEMTIME& st, uint32_t buffer, uint32_t count)
    {
        const uint64_t year = st.wYear, month = st.wMonth, day = st.wDay;
        switch (LocaleDateFormat(Country()))
        {
        case 0: return kernel::FormatWide(buffer, count, CommonString(0x26), { day, month, 4, year });
        case 1: return kernel::FormatWide(buffer, count, CommonString(0x25), { day, 2, month, 4, year });
        case 2: return kernel::FormatWide(buffer, count, CommonString(0x27), { 2, day, 2, month, 4, year });
        case 4: return kernel::FormatWide(buffer, count, CommonString(0x29), { 4, year, month, day });
        case 5: return kernel::FormatWide(buffer, count, CommonString(0x28), { 4, year, 2, month, 2, day });
        default: return kernel::FormatWide(buffer, count, CommonString(0x24), { month, day, 4, year });
        }
    }
}

// VOID XamFormatSystemDateString(const SYSTEMTIME* Time, LPWSTR Buffer, DWORD Count): the date in the console
// country's format.
GUEST_IMPORT(XamFormatSystemDateString)
{
    auto& mem = guest::g_memory;
    SYSTEMTIME st{};
    st.wYear = mem.Read16(ctx.r3.u32);
    st.wMonth = mem.Read16(ctx.r3.u32 + 2);
    st.wDay = mem.Read16(ctx.r3.u32 + 6);
    FormatDate(st, ctx.r4.u32, ctx.r5.u32);
}

// VOID XamFormatDateString(FILETIME Time, LPWSTR Buffer, DWORD Count) (0x816BED50): FileTimeToSystemTime, then
// XamFormatSystemDateString. The FILETIME is passed by value in r3.
GUEST_IMPORT(XamFormatDateString)
{
    FormatDate(FileTimeFields(ctx.r3.u64), ctx.r4.u32, ctx.r5.u32);
}

// VOID XamFormatTimeString(FILETIME Time, LPWSTR Buffer, DWORD Count) (0x816BF0E8): the time in
// XamGetLocaleTimeFormat(0)'s format; 12-hour formats use common strings 0xAD (AM) / 0xB2 (PM) and hour 12 for 0.
GUEST_IMPORT(XamFormatTimeString)
{
    const SYSTEMTIME st = FileTimeFields(ctx.r3.u64);
    const uint32_t buffer = ctx.r4.u32, count = ctx.r5.u32;
    const uint64_t hour = st.wHour, minute = st.wMinute, second = st.wSecond;
    const uint64_t h12 = hour % 12 == 0 ? 12 : hour % 12, ampm = CommonString(hour >= 12 ? 0xB2 : 0xAD);
    switch (const uint32_t format = LocaleTimeFormat(false))
    {
    case 1: kernel::FormatWide(buffer, count, CommonString(0xAF), { 2, hour, 2, minute, 2, second }); break;
    case 2: kernel::FormatWide(buffer, count, CommonString(0xB1), { hour, 2, minute, 2, second }); break;
    case 3: kernel::FormatWide(buffer, count, CommonString(0xB0), { hour, 2, minute }); break;
    case 4: kernel::FormatWide(buffer, count, CommonString(0xB3), { 2, h12, 2, minute, 2, second, ampm }); break;
    case 5: kernel::FormatWide(buffer, count, CommonString(0xB4), { 2, h12, 2, minute, ampm }); break;
    case 6: kernel::FormatWide(buffer, count, CommonString(0xB5), { h12, 2, minute, 2, second, ampm }); break;
    case 7: kernel::FormatWide(buffer, count, CommonString(0xB6), { h12, 2, minute, ampm }); break;
    case 8: kernel::FormatWide(buffer, count, CommonString(0xBA), { ampm, h12, 2, minute, 2, second }); break;
    case 9: kernel::FormatWide(buffer, count, CommonString(0xB9), { ampm, h12, 2, minute }); break;
    case 10: kernel::FormatWide(buffer, count, CommonString(0xB8), { ampm, 2, h12, 2, minute, 2, second }); break;
    case 11: kernel::FormatWide(buffer, count, CommonString(0xB7), { ampm, 2, h12, 2, minute }); break;
    default:
        (void)format;
        kernel::FormatWide(buffer, count, CommonString(0xAE), { 2, hour, 2, minute });
        break;
    }
}

// BYTE XamGetCountry() (0x816A1C20): XCONFIG_USER_COUNTRY through XamValidateCountry.
GUEST_IMPORT(XamGetCountry)
{
    ctx.r3.u64 = Country();
}

// DWORD XamGetLocaleDateFormat(BYTE Country)
GUEST_IMPORT(XamGetLocaleDateFormat)
{
    ctx.r3.u64 = LocaleDateFormat(ctx.r3.u8);
}

namespace
{
    // XAM's common strings: XamStrings.xus in the "xam" package of \SystemRoot\huduiskin.xex, loaded at XUI start-up
    // for the console's language (<locale>\XamStrings.xus, else the root (English) table; languages without a folder
    // come from the encrypted L.huduiskin.xex.xam.xzp pack). Each string is copied once to guest memory as UTF-16.
    std::vector<uint32_t> LoadCommonStrings()
    {
        const std::filesystem::path path = DataDirectory() / "SystemUpdate" / "converted" / "huduiskin.xex";
        if (!std::filesystem::exists(path))
            Fatal("{} is missing (XAM's common strings). Convert it from your system update: python -I "
                "codebreaker.formats.xex.basic SystemUpdate/flash/$flash_huduiskin.xex {}", path.string(),
                path.string());
        if (Settings().language != 1)
            Fatal("XamLookupCommonStringByIndex: console language {} (only English's root table is read)",
                Settings().language);
        const xex::HostImage image = xex::ReadImage(path.string());
        const auto resource = image.resources.find("xam");
        const uint8_t* bytes = resource == image.resources.end() ? nullptr
            : image.At(resource->second.first, resource->second.second);
        if (bytes == nullptr)
            Fatal("{}: no \"xam\" resource", path.string());
        xui::Package package;
        std::string error;
        if (!package.Load(std::vector<uint8_t>(bytes, bytes + resource->second.second), error))
            Fatal("{} \"xam\" package: {}", path.string(), error);
        const xui::Package::Entry* entry = package.Find("XamStrings.xus");
        std::vector<std::string> strings;
        if (entry == nullptr || !xui::ReadStringTable(entry->data, entry->size, strings, error))
            Fatal("{}: XamStrings.xus: {}", path.string(), entry ? error : "missing");
        auto& mem = guest::g_memory;
        std::vector<uint32_t> table;
        for (const std::string& s : strings)
        {
            if (s.empty())
            {
                table.push_back(0);
                continue;
            }
            const int length = MultiByteToWideChar(CP_UTF8, 0, s.data(), int(s.size()), nullptr, 0);
            std::wstring wide(size_t(length), L'\0');
            MultiByteToWideChar(CP_UTF8, 0, s.data(), int(s.size()), wide.data(), length);
            const uint32_t address = mem.SystemAlloc(uint32_t(length + 1) * 2);
            for (int i = 0; i < length; i++)
                mem.Write16(address + uint32_t(i) * 2, uint16_t(wide[size_t(i)]));
            table.push_back(address);
        }
        Log("XAM common strings: {} from {}", table.size(), path.string());
        return table;
    }

    uint32_t CommonString(uint32_t index)
    {
        static const std::vector<uint32_t> table = LoadCommonStrings();
        return index < table.size() ? table[index] : 0;
    }
}

// LPCWSTR XamLookupCommonStringByIndex(DWORD Index): XuiLookupStringTableByIndex on XAM's common string table: the
// string, or NULL past the end (and for empty entries).
GUEST_IMPORT(XamLookupCommonStringByIndex)
{
    ctx.r3.u64 = CommonString(ctx.r3.u32);
}

// int XamUniSortCmpString(LPCWSTR String1, LPCWSTR String2, DWORD Count, BOOL IgnoreCase) (XAM 0x816E35A8): compares
// up to Count characters (with IgnoreCase, differing a-z compare as A-Z) and stops at a 0 or the first difference.
// Equal characters, or a 0 on either side: String1's character minus String2's. Otherwise each character's sort
// weight from XAM's tables (0x20-0xF7 at 0x8161A3E0, 0x391-0x451 0x8161A590, 0x2010-0x2312 0x8161A718, 0x2500-0x266F
// 0x8161AD20, 0x3000-0x30FE 0x8161B000, 0x4E00-0x9FA0 0x8161B200, 0xFF01-0xFFE5 0x81625548; a missing or 0 weight is
// the character + 0x10000), and the difference of the weights.
GUEST_IMPORT(XamUniSortCmpString)
{
    auto& mem = guest::g_memory;
    const uint32_t a = ctx.r3.u32, b = ctx.r4.u32, count = ctx.r5.u32;
    const bool ignoreCase = ctx.r6.u32 != 0;
    const xex::HostImage& xam = kernel::XamImage();
    auto weight = [&xam](uint32_t c) -> int64_t {
        static const struct { uint32_t first, last, table; } kRanges[] = { { 0x20, 0xF7, 0x8161A3E0 },
            { 0x391, 0x451, 0x8161A590 }, { 0x2010, 0x2312, 0x8161A718 }, { 0x2500, 0x266F, 0x8161AD20 },
            { 0x3000, 0x30FE, 0x8161B000 }, { 0x4E00, 0x9FA0, 0x8161B200 }, { 0xFF01, 0xFFE5, 0x81625548 } };
        uint32_t w = 0;
        for (const auto& r : kRanges)
            if (c >= r.first && c <= r.last)
            {
                const uint8_t* p = xam.At(r.table + (c - r.first) * 2, 2);
                if (p == nullptr)
                    Fatal("XamUniSortCmpString: XAM sort table {:08X} lies outside xam.xex", r.table);
                w = uint32_t(p[0]) << 8 | p[1];
                break;
            }
        return w ? int64_t(w) : int64_t(c) + 0x10000;
    };
    uint32_t c1 = 0, c2 = 0;
    for (uint32_t i = 0;; i++)
    {
        if (i == count)
        {
            ctx.r3.u64 = 0;
            return;
        }
        c1 = mem.Read16(a + i * 2);
        c2 = mem.Read16(b + i * 2);
        if (ignoreCase && c1 != c2)
        {
            if (c1 > 0x60 && c1 < 0x7B)
                c1 -= 0x20;
            if (c2 > 0x60 && c2 < 0x7B)
                c2 -= 0x20;
        }
        if (c1 == 0 || c1 != c2)
            break;
    }
    const int64_t result = c1 == c2 || c1 == 0 || c2 == 0 ? int64_t(c1) - int64_t(c2) : weight(c1) - weight(c2);
    ctx.r3.u64 = uint64_t(result);
}

// ---- Language / locale / country strings --------------------------------------------------------------------------
// XAM's own tables in xam.xex (17559), 8-byte entries with a UTF-16 string pointer at +4:
//   0x81A815D8 (61 entries): [0] Xbox LIVE language, [1] XC_LANGUAGE ("en", "ja", ...);
//   0x81A81848 (237 entries, indexed by Xbox LIVE country): [0] country, [2] XC_LOCALE ("US", "GB", ...).
// The strings are copied as StringCchCopyW / StringCchCatW do (XAM 0x81698C98 / 0x81699668): Count characters
// including the terminator; a string that does not fit is cut and STRSAFE_E_INSUFFICIENT_BUFFER returned.
namespace
{
    constexpr uint32_t kLanguageTable = 0x81A815D8, kLanguageEntries = 0x1E8 / 8;
    constexpr uint32_t kCountryTable = 0x81A81848, kCountryEntries = 0x768 / 8;
    constexpr uint32_t kHresultNotFound = 0x80070490, kHresultBadArgument = 0x80070057,
                       kHresultInsufficientBuffer = 0x8007007A;

    // The table entry whose byte `key` equals value, or 0.
    uint32_t FindEntry(uint32_t table, uint32_t entries, uint32_t key, uint32_t value)
    {
        const xex::HostImage& xam = kernel::XamImage();
        for (uint32_t i = 0; i < entries; i++)
            if (const uint8_t* e = xam.At(table + i * 8, 8); e && e[key] == (value & 0xFF))
                return table + i * 8;
        return 0;
    }

    std::u16string XamString(uint32_t entry)
    {
        const xex::HostImage& xam = kernel::XamImage();
        const uint8_t* p = xam.At(entry + 4, 4);
        uint32_t address = p ? uint32_t(p[0]) << 24 | p[1] << 16 | p[2] << 8 | p[3] : 0;
        std::u16string s;
        while (address)
        {
            const uint8_t* c = xam.At(address, 2);
            if (!c || (c[0] == 0 && c[1] == 0) || s.size() > 64)
                break;
            s += char16_t(c[0] << 8 | c[1]);
            address += 2;
        }
        return s;
    }

    // StringCchCopyW (append = false) / StringCchCatW into the guest buffer of `count` characters.
    uint32_t CopyWide(uint32_t buffer, uint32_t count, const std::u16string& text, bool append)
    {
        auto& mem = guest::g_memory;
        if (count == 0 || count > 0x7FFFFFFF)
            return kHresultBadArgument;
        uint32_t at = 0;
        if (append)
            while (at < count && mem.Read16(buffer + at * 2) != 0)
                at++;
        if (at == count)
            return kHresultBadArgument;
        uint32_t i = 0;
        for (; i < text.size() && at + 1 < count; i++, at++)
            mem.Write16(buffer + at * 2, text[i]);
        mem.Write16(buffer + at * 2, 0);
        return i < text.size() ? kHresultInsufficientBuffer : 0;
    }

    // The string of an entry found by key, copied (or kHresultNotFound).
    uint32_t TableString(uint32_t entry, uint32_t buffer, uint32_t count, bool append = false)
    {
        if (count > 0x7FFFFFFF)
            return kHresultBadArgument;
        return entry ? CopyWide(buffer, count, XamString(entry), append) : kHresultNotFound;
    }
}

// HRESULT XamGetLanguageString(DWORD Language, DWORD Count, LPWSTR Buffer): "en", "ja", ... (XC_LANGUAGE)
GUEST_IMPORT(XamGetLanguageString)
{
    ctx.r3.u64 = TableString(FindEntry(kLanguageTable, kLanguageEntries, 1, ctx.r3.u32), ctx.r5.u32, ctx.r4.u32);
}

// HRESULT XamGetLocaleString(DWORD Locale, DWORD Count, LPWSTR Buffer): "US", "GB", ... (XC_LOCALE)
GUEST_IMPORT(XamGetLocaleString)
{
    ctx.r3.u64 = TableString(FindEntry(kCountryTable, kCountryEntries, 2, ctx.r3.u32), ctx.r5.u32, ctx.r4.u32);
}

// HRESULT XamGetOnlineCountryString(BYTE Country, DWORD Count, LPWSTR Buffer): the Xbox LIVE country's code (the
// table is indexed by it).
GUEST_IMPORT(XamGetOnlineCountryString)
{
    const uint32_t country = ctx.r3.u32 & 0xFF;
    ctx.r3.u64 = TableString(country < kCountryEntries ? kCountryTable + country * 8 : 0, ctx.r5.u32, ctx.r4.u32);
}

// BYTE XamGetOnlineCountryFromLocale(DWORD Locale): the Xbox LIVE country of an XC_LOCALE (0 if none).
GUEST_IMPORT(XamGetOnlineCountryFromLocale)
{
    const uint32_t entry = FindEntry(kCountryTable, kCountryEntries, 2, ctx.r3.u32);
    const uint8_t* e = entry ? kernel::XamImage().At(entry, 1) : nullptr;
    ctx.r3.u64 = e ? e[0] : 0;
}

// HRESULT XamGetLanguageLocaleString(DWORD Language, DWORD Locale, DWORD Count, LPWSTR Buffer): "en-US".
GUEST_IMPORT(XamGetLanguageLocaleString)
{
    const uint32_t language = ctx.r3.u32, locale = ctx.r4.u32, count = ctx.r5.u32, buffer = ctx.r6.u32;
    uint32_t hr = TableString(FindEntry(kLanguageTable, kLanguageEntries, 1, language), buffer, count);
    if (int32_t(hr) >= 0)
        hr = CopyWide(buffer, count, u"-", true);
    if (int32_t(hr) >= 0)
        hr = TableString(FindEntry(kCountryTable, kCountryEntries, 2, locale), buffer, count, true);
    ctx.r3.u64 = hr;
}

// HRESULT XamGetOnlineLanguageAndCountryString(BYTE Language, BYTE Country, DWORD Count, LPWSTR Buffer): the Xbox
// LIVE language's code (the language table keyed by its byte 0), "-", the Xbox LIVE country's.
GUEST_IMPORT(XamGetOnlineLanguageAndCountryString)
{
    const uint32_t language = ctx.r3.u32, country = ctx.r4.u32 & 0xFF, count = ctx.r5.u32, buffer = ctx.r6.u32;
    uint32_t hr = TableString(FindEntry(kLanguageTable, kLanguageEntries, 0, language), buffer, count);
    if (int32_t(hr) >= 0)
        hr = CopyWide(buffer, count, u"-", true);
    if (int32_t(hr) >= 0)
        hr = TableString(country < kCountryEntries ? kCountryTable + country * 8 : 0, buffer, count, true);
    ctx.r3.u64 = hr;
}

// DWORD XamGetCurrentTitleId(): the title ID in the running executable's execution info (+0xC), 0 without one.
GUEST_IMPORT(XamGetCurrentTitleId)
{
    constexpr uint32_t kXexHeaderExecutionInfo = 0x00040006;
    const uint32_t info = kernel::XexHeaderField(kernel::ExecutableXexHeader(), kXexHeaderExecutionInfo);
    ctx.r3.u64 = info ? guest::g_memory.Read32(info + 0xC) : 0;
}

// BOOL XamGetClosedCaptioningState(): XConfig 7 / 0x12 (the console setting), 0 if it cannot be read.
GUEST_IMPORT(XamGetClosedCaptioningState)
{
    const std::vector<uint8_t> value = kernel::XConfigRead(7, 0x12);
    ctx.r3.u64 = value.empty() ? 0 : value[0];
}

// BOOL XamUserIsParentalControlled(DWORD UserIndex): XAM reads a flag of the user's Xbox LIVE account data (family
// settings). Profiles here carry no Xbox LIVE family settings, so no user is parental controlled.
GUEST_IMPORT(XamUserIsParentalControlled)
{
    ctx.r3.u64 = 0;
}

// HRESULT XamIsIptvEnabled(): S_OK when XCONFIG_USER_RETAIL_FLAGS (3 / 0xC) has bit 0x1000 (IPTV set up), E_FAIL
// otherwise (XAM 17559).
GUEST_IMPORT(XamIsIptvEnabled)
{
    ctx.r3.u64 = (kernel::XConfigRead32(3, 0xC) & 0x1000) ? 0 : 0x80004005;
}

// DWORD XamIptvGetServiceName(LPCWSTR* Name): the IPTV provider's name from XConfig 9 / 1 (WCHAR[60]), or XAM's
// common string 0x125 when there is none; always ERROR_SUCCESS. XAM keeps the name in a buffer of its own (0x81AAC4C8);
// here one guest buffer allocated on first use.
GUEST_IMPORT(XamIptvGetServiceName)
{
    auto& mem = guest::g_memory;
    const std::vector<uint8_t> name = kernel::XConfigRead(9, 1);
    uint32_t result = 0;
    if (name.size() >= 2 && (name[0] | name[1]) != 0)
    {
        static const uint32_t buffer = KernelPool().Allocate(0x7A);
        std::memcpy(mem.Translate(buffer), name.data(), std::min<size_t>(name.size(), 0x78));
        mem.Write16(buffer + 0x78, 0);
        result = buffer;
    }
    else
        result = CommonString(0x125);
    if (ctx.r3.u32)
        mem.Write32(ctx.r3.u32, result);
    ctx.r3.u64 = 0;
}

// VOID VdQueryRealVideoMode(PXVIDEO_MODE VideoMode): the mode the display is driven in, which here is the mode
// VdQueryVideoMode reports (no title-requested scaling sits between them).
GUEST_IMPORT(VdQueryRealVideoMode)
{
    WriteVideoMode(ctx.r3.u32);
}

// BOOL VdGetDisplayDiscoveryData(PXDISPLAY_DISCOVERY_DATA Data): the TV's HDMI discovery data (EDID-derived: the dash
// reads the display type at +0x1D and flags at +4 after setting Data's size, 0x23). FALSE: none is available, as with
// a display that reports nothing; the dash then keeps its default list of resolutions.
GUEST_IMPORT(VdGetDisplayDiscoveryData)
{
    ctx.r3.u64 = 0;
}

// VOID VdEnumerateVideoModes(PFN Callback, PVOID Context): calls Callback once per mode the display reported in its
// discovery data. Without discovery data there are none, so the callback is never called.
GUEST_IMPORT(VdEnumerateVideoModes)
{
}

// BOOL XamNuiIsChatMicEnabled(): bit 3 of XConfig 7 / 9 (CONSOLE_NUI's first dword) clear = the Kinect chat microphone
// is enabled (XAM 17559; also TRUE when the setting cannot be read).
GUEST_IMPORT(XamNuiIsChatMicEnabled)
{
    const std::vector<uint8_t> nui = kernel::XConfigRead(7, 9);
    const uint32_t flags = nui.size() >= 4 ? uint32_t(nui[0]) << 24 | nui[1] << 16 | nui[2] << 8 | nui[3] : 0;
    ctx.r3.u64 = (~flags >> 3) & 1;
}
