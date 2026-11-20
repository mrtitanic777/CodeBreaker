#pragma once

#include "live/client.h"

#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>

// A title's leaderboards on our Xbox LIVE (Shared/docs/network_protocol.md "Stats"): XUserReadStats,
// XUserResetStatsView, XSessionWriteStats / XSessionFlushStats (cached per session and sent at the flush or at
// XSessionEnd) and the stats enumerators (XUserCreateStatsEnumeratorByRank / ByXuid / ByRating, XamUserCreateStatsEnumerator:
// kernel/xam_stats_enum.cpp). kernel/xam_msg.cpp dispatches the XGI messages here.
//
// The XGI messages are 17559's XAM (the XDK's xapilib / xonline code and xam.xex, FUN_8190aac0's switch):
//   0xB0020 XUserResetStatsView  { user index (0xFFFFFFFF: XUserResetStatsViewAllUsers), view id }                 8 bytes
//   0xB0021 XUserReadStats       { title id (0: this title), XUID count, XUID*, spec count, XUSER_STATS_SPEC*,
//                                  results size, XUSER_STATS_READ_RESULTS* }                                      28 bytes
//   0xB0025 XSessionWriteStats   { session object, -, XUID (u64), view count, XSESSION_VIEW_PROPERTIES* }        24 bytes
//   0xB0026 XSessionFlushStats   { session object }
// The enumerator (XAM 0x816702D8, handlers 0xB0022 initialise / 0xB0023 next, 0x816F1070 XamEnumerate): the first
// XEnumerate answers by the creation's kind (xuid 0, rank 1, rating 3) and each later one continues by rank after the last
// row of each view; the results buffer is one XUSER_STATS_READ_RESULTS.
//
// XUSER_STATS_READ_RESULTS as XAM builds it (FUN_81922140): { u32 views, XUSER_STATS_VIEW* } (8 bytes) at the start of
// the buffer, then the views (0x10 each: id, total rows, rows, XUSER_STATS_ROW*), the rows (0x30 each: XUID, rank, rating
// (i64 at 0x10), gamertag (16, at 0x18), column count (0x28), XUSER_STATS_COLUMN* (0x2C)) and the columns (0x18 each:
// u16 id, XUSER_DATA at 8: type byte, value at 0x10). A column the row has no value for is XUSER_DATA_TYPE_NULL (0xFF).
// The size the title allocates is XAM's / the XDK's formula, ResultsSize(): (rows * 0x30 + 0x10) * views + 8 + columns *
// rows * 0x1C, which is more than what is written (0x18 per column).
namespace kernel::stats
{
    constexpr uint32_t kMsgUserResetStatsView = 0x000B0020, kMsgUserReadStats = 0x000B0021,
                       kMsgSessionWriteStats = 0x000B0025, kMsgSessionFlushStats = 0x000B0026;
    constexpr uint32_t kSpecSize = 0x88, kSpecColumnsMax = 64, kViewsMax = 64, kXuidsMax = 101, kRowsMax = 100;

    // The XGI messages above; nullopt for any other. The result is the HRESULT (or Win32 error) the title sees.
    std::optional<uint32_t> Message(uint32_t message, uint32_t buffer, uint32_t length);

    // XSessionEnd: the session's cached stats go to the server (before its end is sent); XSessionDelete: they are dropped.
    void SessionEnded(uint32_t sessionObject);
    void SessionDeleted(uint32_t sessionObject);

    // XUSER_STATS_SPEC: a view and the columns wanted of it.
    struct Spec
    {
        uint32_t view = 0;
        std::vector<uint16_t> columns;
    };
    // `count` specs at guest address `at` (0x88 bytes each: view id, column count, 64 big-endian u16 ids).
    std::vector<Spec> ReadSpecs(uint32_t at, uint32_t count);
    uint32_t ResultsSize(uint32_t rows, const std::vector<Spec>& specs);

    // The results for `views` (the server's, in the order of `specs`) laid out for a buffer at guest address `base` of
    // `capacity` bytes; nullopt when they do not fit. Each row carries every column of its view's spec, in order.
    std::optional<std::vector<uint8_t>> SerializeResults(const std::vector<live::StatsView>& views,
                                                         const std::vector<Spec>& specs, uint32_t base, uint32_t capacity);

    // A stats enumerator's state (the creation's arguments and where each view goes on).
    class Enumerator
    {
    public:
        // kind: 0 by XUID (pivot = the XUID), 1 by rank (pivot = the first rank), 3 by rating (pivot = the rating).
        Enumerator(uint32_t titleId, uint32_t kind, uint64_t pivot, uint32_t rows, std::vector<Spec> specs);
        // XEnumerate's work: writes the next XUSER_STATS_READ_RESULTS to the guest buffer. Returns a Win32 error (0: ok);
        // `rows` is how many rows were written in all.
        uint32_t Next(uint32_t buffer, uint32_t bufferSize, uint32_t& rows);

    private:
        std::mutex mutex_;
        uint32_t titleId_, kind_, rows_;
        uint64_t pivot_;
        std::vector<Spec> specs_;
        bool continued_ = false;
        std::vector<uint32_t> nextRank_; // per spec: the rank to continue from; 0: the view has nothing more
        std::vector<uint32_t> total_;
    };
}
