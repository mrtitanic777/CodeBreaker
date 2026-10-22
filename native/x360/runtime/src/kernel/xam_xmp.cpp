// XMP: XAM's system music player (app 0xFA). XAM 17559's handler (0x81706478) routes each message to a group of
// handlers that share the player and dashboard state below (offsets of XAM's XMP app object in the comments). Nothing
// is ever played on this console yet: there is no music content and no playback engine; the state the dashboard sets
// up at start-up is kept so later queries answer consistently. Messages not listed stop the runtime.
//
// Buffers are arrays of 32-bit words; the first word is the XMP client (0 dash, 1 HUD, 2 title).

#include "kernel/xmp.h"

#include "kernel/kernel.h"

#include <map>
#include <mutex>

namespace
{
    constexpr uint32_t kOk = 0;
    constexpr uint32_t kFalse = 1;                   // S_FALSE
    constexpr uint32_t kInvalidArg = 0x80070057;     // E_INVALIDARG
    constexpr uint32_t kAccessDenied = 0x80070005;
    constexpr uint32_t kFunctionFailed = 0x8007065B; // HRESULT_FROM_WIN32(ERROR_FUNCTION_FAILED)
    constexpr uint32_t kAlreadyExists = 0x800700B7;  // HRESULT_FROM_WIN32(ERROR_ALREADY_EXISTS)

    constexpr uint32_t kClientDash = 0, kClientHud = 1, kClientTitle = 2;
    constexpr uint32_t kControllerUser = 1;
    constexpr uint32_t kWorkspaceRelease = 3;

    constexpr uint32_t kXnXmpPlaybackControllerChanged = 0x0A000003;
    constexpr uint32_t kXnXmpDashNowPlayingQueueChanged = 0x8A000009; // carries the "populating queue" flag

    // A title playlist lives in its caller's storage: XAM writes a 0x88-byte header there and keeps the songs after
    // it; the handle is the storage address. Only the header fields XAM sets for an empty playlist are written.
    struct Playlist
    {
        uint32_t flags = 0;
        uint32_t maxSongs = 0;
    };

    struct State
    {
        std::mutex mutex;
        std::map<uint32_t, Playlist> playlists; // +0x1DB0
        uint32_t lockedPlaylists = 0;           // +0x1DC4: playlists created with flag 1
        bool dashInitialized = false;           // +0x1D40
        uint32_t dashPlaylist = 0;              // +0x1D48
        uint32_t dashBuffer = 0, dashBufferSize = 0;
        bool populatingQueue = false;           // +0x1D58
        uint32_t controller = 0;                // +0x1D8C: 0 title (game), 1 user, 2 dash, 3 Media Center
        uint32_t controllerLock = 0;            // +0x1D90
        uint32_t controllerOwner = kClientTitle; // +0x1D94
        uint32_t workspace = 0, workspaceSize = 0;
        uint32_t playbackController = 0; // XMPSetPlaybackController
    };
    State g_state;

    uint32_t Word(uint32_t buffer, uint32_t index)
    {
        return guest::g_memory.Read32(buffer + index * 4);
    }

    // 0x7002E (XAM 0x816FFA40): the storage a title playlist of SongCount songs needs.
    // {client, songCount, pSize}
    uint32_t GetTitlePlaylistBufferSize(uint32_t buffer)
    {
        const uint32_t client = Word(buffer, 0), count = Word(buffer, 1), sizePtr = Word(buffer, 2);
        if (buffer == 0 || count == 0 || sizePtr == 0 || client == kClientHud)
            return kInvalidArg;
        guest::g_memory.Write32(sizePtr, client == kClientDash || client == kClientTitle ? count * 1000 + 0x88 : 0);
        return kOk;
    }

    // 0x7000D (XAM 0x81704C40): creates a title playlist in the caller's storage.
    // {client, storage, size, songs, songCount, name, flags, songHandles, phPlaylist}
    uint32_t CreateTitlePlaylist(uint32_t buffer)
    {
        auto& mem = guest::g_memory;
        const uint32_t client = Word(buffer, 0), storage = Word(buffer, 1), size = Word(buffer, 2);
        const uint32_t songs = Word(buffer, 3), count = Word(buffer, 4), name = Word(buffer, 5);
        const uint32_t flags = Word(buffer, 6), handlePtr = Word(buffer, 8);
        if (handlePtr == 0 || storage == 0 || size == 0 || count == 0 || (songs == 0 && (flags & 1)) ||
            (name == 0 && (flags & 1)) || client == kClientHud)
            return kInvalidArg;
        if (client != kClientDash && client != kClientTitle)
            return kFunctionFailed;
        if (songs != 0)
            Fatal("XMP: creating a title playlist with songs is not implemented");
        memset(mem.Translate(storage), 0, 0x88);
        mem.Write32(storage + 0x00, storage);                        // self
        mem.Write32(storage + 0x0C, client == kClientDash ? 0 : 1);  // playlist type
        for (uint32_t i = 0; name && i < 19; i++)                    // name, WCHAR[20]
        {
            const uint16_t c = mem.Read16(name + i * 2);
            if (c == 0)
                break;
            mem.Write16(storage + 0x10 + i * 2, c);
        }
        mem.Write32(storage + 0x60, count);
        mem.Write32(storage + 0x64, flags);
        mem.Write32(storage + 0x68, storage + 0x88); // song array
        mem.Write32(storage + 0x6C, storage + 0x88 + count * 0x3DC);
        std::lock_guard lock(g_state.mutex);
        g_state.playlists[storage] = { flags, count };
        if (flags & 1)
            g_state.lockedPlaylists++;
        mem.Write32(handlePtr, storage);
        return kOk;
    }

    // 0x70013 (XAM 0x816FFAB0): deletes a title playlist. S_FALSE: it was not the one playing (nothing plays).
    // {client, hPlaylist}
    uint32_t DeleteTitlePlaylist(uint32_t buffer)
    {
        const uint32_t handle = Word(buffer, 1);
        if (handle == 0)
            return kInvalidArg;
        std::lock_guard lock(g_state.mutex);
        auto it = g_state.playlists.find(handle);
        if (it == g_state.playlists.end())
            Fatal("XMP: deleting unknown title playlist {:08X}", handle);
        if (it->second.flags & 1)
            return kAccessDenied;
        g_state.playlists.erase(it);
        if (g_state.dashPlaylist == handle)
            g_state.dashPlaylist = 0;
        return kFalse;
    }

    // 0x7002F, XMPDashInit (XAM 0x817043C0): the dashboard registers its now-playing playlist and buffer; XAM resets
    // the playback controllers, hands playback to the user (background music muted) and announces both.
    // {client, buffer, size, -, -, hPlaylist}
    uint32_t DashInit(uint32_t buffer)
    {
        const uint32_t client = Word(buffer, 0), storage = Word(buffer, 1), size = Word(buffer, 2);
        const uint32_t playlist = Word(buffer, 5);
        if (client != kClientDash || playlist == 0 || storage == 0)
            return kInvalidArg;
        {
            std::lock_guard lock(g_state.mutex);
            g_state.dashInitialized = true;
            g_state.dashPlaylist = playlist;
            g_state.dashBuffer = storage;
            g_state.dashBufferSize = size;
            g_state.populatingQueue = false;
            g_state.controller = kControllerUser;
            g_state.controllerLock = 0;
            g_state.controllerOwner = kClientDash;
        }
        kernel::BroadcastNotification(kXnXmpPlaybackControllerChanged, 0); // controller != title
        kernel::BroadcastNotification(kXnXmpDashNowPlayingQueueChanged, 0);
        return kOk;
    }

    // 0x70044 (XAM 0x81700918): workspace memory for the media sources (type 0 splits it among them; 3 releases it).
    // {client, type, buffer, size}
    uint32_t SetMediaSourceWorkspace(uint32_t buffer)
    {
        const uint32_t client = Word(buffer, 0), type = Word(buffer, 1), storage = Word(buffer, 2);
        const uint32_t size = Word(buffer, 3);
        if (client > kClientTitle || (storage == 0 && type != kWorkspaceRelease) || (storage != 0) != (size != 0))
            return kInvalidArg;
        std::lock_guard lock(g_state.mutex);
        if (type == kWorkspaceRelease)
        {
            g_state.workspace = 0;
            g_state.workspaceSize = 0;
            return kOk;
        }
        if (g_state.workspace != 0)
            return kAlreadyExists;
        if (type != 0)
            Fatal("XMP: media source workspace type {} is not implemented", type);
        g_state.workspace = storage;
        g_state.workspaceSize = size;
        return kOk;
    }

    // 0x7001A, XMPSetPlaybackController (dash 17559 0x9224A4A8; the 1888 ... 6719 dashboards' 0x9210DA90 ...):
    // {client, controller (XMP_SETPLAYBACKCONTROLLER: who owns playback, the title or the dash), flags}; the dash
    // switches it at start-up ((0, 2, 1) then (0, 1, 0) in 1888). Nothing plays here, so the user changes nothing
    // that can be heard: it is recorded and accepted (XAM's handler for it was not traced).
    uint32_t SetPlaybackController(uint32_t buffer)
    {
        g_state.playbackController = Word(buffer, 1);
        return kOk;
    }

    // 0x7001B, XMPGetPlaybackController: {client, PDWORD Controller, PDWORD Locked}, sent with a length of 0 (Up,
    // 0x8251B660). The controller recorded by the last 0x7001A; never locked, since no user takes the music over.
    uint32_t GetPlaybackController(uint32_t buffer)
    {
        if (const uint32_t controllerPtr = Word(buffer, 1))
            guest::g_memory.Write32(controllerPtr, g_state.playbackController);
        if (const uint32_t lockedPtr = Word(buffer, 2))
            guest::g_memory.Write32(lockedPtr, 0);
        return kOk;
    }

    // 0x70029, XMPGetMediaSources in the 1888 ... 2258 dashboards' XAM (0x9210E200; 17559: 0x7002B with one more
    // argument): {client, PDWORD Count, MSAL_MEDIASOURCEINFO* Sources, DWORD MaxSources}. The dash asks the count
    // first (Sources NULL). This runtime has no media library (the console's music on its hard drive, CDs, PCs), so
    // it has no sources: 0. A call that wants the list stops (none can be given yet).
    uint32_t GetMediaSourcesOld(uint32_t buffer)
    {
        if (Word(buffer, 2) != 0 && Word(buffer, 3) != 0)
            Fatal("XMPGetMediaSources (0x70029): listing {} media sources is not implemented", Word(buffer, 3));
        if (const uint32_t countPtr = Word(buffer, 1))
            guest::g_memory.Write32(countPtr, 0);
        return kOk;
    }

    // 0x70008 in the 1888 ... 2258 dashboards' XAM (dash 1888 0x9210E170, removed in 2832; its name is not known):
    // {client, flag, 0, 0}, sent by the music visualizer's set-up (Visualizer.xur, 0x92131360 -> 0x9212FEC0) after
    // the media source count, the flag 1 or 0 from the visualizer's state: it switches the visualization the music
    // player feeds. Nothing plays here, so there is nothing to visualize: accepted.
    uint32_t VisualizerSwitchOld(uint32_t)
    {
        return kOk;
    }

    // 0x70009, XMPGetStatus: XMP_STATE_IDLE (0); nothing plays. {client, pState}
    uint32_t GetStatus(uint32_t buffer)
    {
        if (const uint32_t statePtr = Word(buffer, 1))
            guest::g_memory.Write32(statePtr, 0);
        return kOk;
    }
}

std::optional<uint32_t> kernel::xmp::Dispatch(uint32_t message, uint32_t buffer, uint32_t length)
{
    switch (message)
    {
    case 0x00070009: return GetStatus(buffer);
    case 0x0007000D: return CreateTitlePlaylist(buffer);
    case 0x00070013: return DeleteTitlePlaylist(buffer);
    case 0x0007001A: return SetPlaybackController(buffer);
    case 0x0007001B: return GetPlaybackController(buffer);
    case 0x00070008: // the old numbering only (an older dashboard, CB_DASH)
        if (kernel::OlderDashVersion().empty())
            return std::nullopt;
        return VisualizerSwitchOld(buffer);
    case 0x00070029: // the old numbering only (an older dashboard, CB_DASH)
        if (kernel::OlderDashVersion().empty())
            return std::nullopt;
        return GetMediaSourcesOld(buffer);
    case 0x0007002E: return GetTitlePlaylistBufferSize(buffer);
    case 0x0007002F: return DashInit(buffer);
    case 0x00070044: return SetMediaSourceWorkspace(buffer);
    default: return std::nullopt;
    }
}
