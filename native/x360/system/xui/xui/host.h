#pragma once

// What the XUI library (Shared/runtime/xui, the console's system UI: XUI packages and scenes, the Guide, popups,
// virtual keyboard, achievements) needs from the runtime that builds it:
// - "log.h" on the include path with Log(fmt, ...) and Fatal(fmt, ...) (std::format style), as both runtimes have;
// - the functions below, defined once in the runtime.

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace xui::host
{
    // The console's clock format (XCONFIG retail flag 8): the Guide's clock shows 24-hour time when true.
    bool TwentyFourHourClock();

    // A UI sound to play: an XuiSoundXAudio element's File (XUI plays it when a timeline sets File to a new,
    // non-empty path: _SetFile -> XuiSoundSetDirty -> the sound's timer -> Play, State's default being Play; dash
    // 0x921DBB40 / 0x92204B48). `data` is the whole file from its system UI package (a RIFF wave, format 0x165
    // XMAWAVEFORMAT in every 17559 package); it stays valid while the Assets live, so a player may key a cache on it.
    struct Sound
    {
        const uint8_t* data;
        size_t size;
        std::string_view name;  // the locator as the scene names it ("sharedres://btn_Focus.xma"), for logs
        float volumeDb;         // the element's Volume (dB; 0 = as recorded, hudbkgnd's GuideSound is -18)
        const void* source;     // the element (XUI retires its previous voice, which plays on: dash 0x921DC0C8)
    };
    // Optional, unlike the functions above: the runtime installs a player (any thread may call it, from the UI
    // threads that advance timelines). Without one, sounds are silent.
    using SoundPlayer = void (*)(const Sound& sound);
    void SetSoundPlayer(SoundPlayer player);
}
