// app/board_projection.hpp -- how often each channel is called in one hour. Without a trace, all of it is a MODEL. With one (board_trace.hpp) the calls it saw are
// MEASURED and stand as they are; the rest stays a model, scaled per kind of function so that the model agrees with the trace where both exist.
//
//     projected calls per hour = static call sites x how often that kind of function runs x how much of the hour the calling file is loaded
//
// The kind of function comes from its name (a message pump runs constantly, a LoadLibrary rarely); the rates below are assumptions, kept in one table so they can be
// changed or replaced by a real trace. A file is resident if it starts by itself or is part of the core of the system, a file a resident file imports is nearly as
// resident, and anything else is taken to run 2% of the hour. Identical copies of a file count once. No operating-system API (board_projection_test.cpp).
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "app/board.hpp"
#include "app/board_flow.hpp"
#include "app/board_index.hpp"
#include "app/board_signals.hpp"
#include "app/board_trace.hpp"

namespace cb::app::board {

enum class ApiKind : std::uint8_t { MessagePump, Sync, Memory, Paint, Query, FileRegistry, Lifecycle, Other };
ApiKind api_kind(std::string_view function);
// The assumed calls per hour of one call site of a function of this kind, in a file loaded the whole hour.
double api_rate(ApiKind kind);

inline constexpr double kIdleResidency = 0.02;
inline constexpr double kResidencyDecay = 0.9;
inline constexpr double kHotPathShare = 0.10;   // a measured hot path goes on only along channels with at least this share of the busiest

struct Projection {
    double max_channel = 0;                  // the busiest module-to-module channel: what every density is relative to
    double total = 0;
    std::vector<std::uint32_t> hot_path;     // the chain through the busiest channel, callers first
    std::vector<double> residency;           // per module
    std::vector<double> activity;            // per module: the calls in and out of it (with a trace, only what it saw)
    std::vector<std::uint32_t> module_of_card;

    bool has_measured = false;
    double measured_seconds = 0;
    std::array<double, 8> kind_scale{1, 1, 1, 1, 1, 1, 1, 1};   // per ApiKind: what the trace says of the model
    bool channel_measured(std::uint32_t from, std::uint32_t to) const;
    bool function_measured(std::uint32_t module, std::string_view key) const;
    bool function_user_measured(std::uint32_t module, std::string_view key, std::uint32_t user) const;
    std::unordered_set<std::uint64_t> measured_channels, measured_function_users;
    std::unordered_set<std::uint32_t> measured_functions;

    double channel(std::uint32_t from, std::uint32_t to) const;
    double function(std::uint32_t module, std::string_view key) const;
    double function_user(std::uint32_t module, std::string_view key, std::uint32_t user) const;

    std::unordered_map<std::uint64_t, double> channels;       // (from << 32 | to)
    std::unordered_map<std::string, std::uint32_t> function_ids;   // "module:key"
    std::vector<double> functions;
    std::unordered_map<std::uint64_t, double> function_users; // (function << 32 | user)
};
Projection build_projection(const Board& board, const Index& index, const Signals* signals, const Measured* measured = nullptr);

// Where the dots of a channel are at time `seconds`, as fractions 0..1 along it. More projected calls, relative to the busiest channel: more dots, moving faster.
// The density is the square root of the share, so a channel at 1% of the busiest still shows. `seed` keeps channels from moving in step.
inline constexpr int kMaxFlowDots = 16;
std::vector<float> flow_dots(double rate, double busiest, double seconds, std::uint32_t seed);
// 0..1: the channel's density relative to the busiest.
double flow_intensity(double rate, double busiest);
// "1.2M", "34K", "870"
std::string rate_words(double rate);

}  // namespace cb::app::board
