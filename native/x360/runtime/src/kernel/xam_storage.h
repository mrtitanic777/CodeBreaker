#pragma once

#include <cstdint>
#include <string>

// XStorage on our Xbox LIVE (Shared/docs/network_protocol.md "Storage"): files at XStorage server paths. The XStorage
// functions are the title's own code (xonline.lib); what reaches XAM, as 17559 handles it:
//
// XStorageBuildServerPath / ...ByXuid: XMsgInProcessCall(0xFC, 0x58035, args) (XAM 0x818EE968), args = { user index (0xFE:
// by XUID) at +0, XUID at +8, facility at +0x10, facility info pointer +0x14 and its size +0x18, item name (wide) +0x1C,
// output buffer +0x20, pointer to its length in characters +0x24 }. The path (XAM's formats, lower-case hex):
//   facility 1 (game clip)    //xestats/u:<xuid>/<title>/<leaderboard id>   (info = the id, 4 bytes)
//   facility 2 (per title)    //title.<title>/t:<title>/<item>
//   facility 3 (per user)     //tuser.<title>/u:<xuid>/<title>/<item>
// The length is set to the characters needed (with the terminator); a buffer too small is ERROR_INSUFFICIENT_BUFFER.
//
// XStorageDownloadToMemory / UploadFromMemory / Enumerate / Delete: Xbox LIVE service tasks (XMsgStartIORequest(0xFC,
// 0x50000 | schema index) with the task of kernel/xam_msg.cpp), the schema's ordinals 0x0E11, 0x0E13, 0x0E14, 0x0E15 (no
// path: XAM does the transfer itself). The marshalled request (big-endian from a title, e.g. Minecraft; little-endian also read) is { u32 user index, u32
// path length with the terminator, that many UTF-16LE characters } and then, for download and upload, { u32 buffer size,
// u32 buffer address (the title's own buffer: the data does not travel in the task) }, for enumerate { u32 starting index,
// u32 most results }. The user buffer of the task (task +0x2C, size +0x30) takes the results: for download the 20-byte
// XSTORAGE_DOWNLOAD_TO_MEMORY_RESULTS { u32 bytes total, u64 owner, FILETIME created }, for enumerate an
// XSTORAGE_ENUMERATE_RESULTS { u32 total, u32 returned, XSTORAGE_FILE_INFO* } (12 bytes) followed by the packed 65-byte
// XSTORAGE_FILE_INFO array and the path names (the descriptors in the XAM schema, ordinal 0x0E14).
namespace kernel::storage
{
    constexpr uint32_t kMsgBuildServerPath = 0x00058035;
    constexpr uint32_t kOrdinalDownload = 0x0E11, kOrdinalUpload = 0x0E13, kOrdinalEnumerate = 0x0E14, kOrdinalDelete = 0x0E15;

    // Whether a service task with this schema ordinal is an XStorage request handled here.
    bool IsTask(uint32_t ordinal);

    // XStorageBuildServerPath's message; the result is the HRESULT (0, E_INVALIDARG or ERROR_INSUFFICIENT_BUFFER).
    uint32_t BuildServerPath(uint32_t args);

    // Performs the task at guest address `task` against our server; returns the HRESULT the title's overlapped gets as
    // its extended error; `used` is the bytes written to the task's user buffer.
    uint32_t RunTask(uint32_t ordinal, uint32_t task, uint32_t& used);

    // The HRESULT of a server's error words ("denied", "not found", "quota exceeded" ...).
    uint32_t ErrorToHresult(const std::string& error);
}
