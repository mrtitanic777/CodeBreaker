// app/board_flow.hpp -- the board's links as a map: one module per file name, what flows into and out of a module by kind of file (the Flow view), and its
// callers and dependencies three hops each way (the Trace view).
//
// Everything here is STATIC: a weight counts distinct files (identical copies once) whose tables or bytes name the module. It says what depends on what, never
// how often anything ran, so the "heaviest path" is the most depended-upon chain and not a measured hot path. No operating-system API (board_flow_test.cpp).
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "app/board.hpp"
#include "app/board_index.hpp"

namespace cb::app::board {

// Which kinds of link count: bits of 1 << Link.
inline constexpr unsigned kLinkImports = 1u << static_cast<unsigned>(Link::Imports);
inline constexpr unsigned kLinkInstalls = 1u << static_cast<unsigned>(Link::Installs);
inline constexpr unsigned kLinkReferences = 1u << static_cast<unsigned>(Link::References);

// What a channel's width counts. Files: distinct files that link. Links: functions imported that the target really exports. Calls: places in the code that call
// or load them. Links and Calls exist for imports only, and a call site is in the code: it says nothing of how often it ran.
enum class Width : std::uint8_t { Files, Links, Calls };
std::string_view width_unit(Width width);

struct ModuleEdge {
    std::uint32_t module = 0;
    std::uint32_t weight = 0;          // by the Width chosen; identical copies of a file count once
};

struct Module {
    std::string name;                  // lower case
    std::uint32_t card = 0;            // the copy most links point at
    Pile pile = Pile::Data;
    std::uint32_t fan_in = 0;          // what comes in, by the Width chosen (Files: distinct files)
    std::vector<ModuleEdge> out, in;
};

struct Graph {
    std::vector<Module> modules;       // sorted by name: an id stays the same whichever links are counted
    std::vector<std::uint32_t> module_of_card;
    // The module most files link to; nullopt when nothing links to anything.
    std::optional<std::uint32_t> most_linked() const;
};
Graph build_graph(const Board& board, const Index& index, unsigned links, Width width = Width::Files);

// ---- the Flow view: what comes in and goes out of one module, by kind of file

struct FlowBand {
    Pile pile = Pile::Data;
    std::uint64_t weight = 0;
    std::uint32_t modules = 0;
    std::uint32_t top = 0;             // its most linked module
};
struct Flow {
    std::uint32_t focus = 0;
    std::vector<FlowBand> in, out;     // heaviest first
    std::uint64_t in_total = 0, out_total = 0;
};
Flow compute_flow(const Graph& graph, std::uint32_t focus);

// ---- the Trace view: seven columns, callers three hops out on the left, the module in the middle, dependencies three hops out on the right

inline constexpr std::size_t kTraceColumns = 7;
inline constexpr std::size_t kTraceCenter = 3;

struct TraceLink {
    std::uint8_t column = 0;           // the left end's column; the right end is in column + 1
    std::uint32_t from = 0, to = 0;    // positions in those columns
    std::uint32_t weight = 0;
    bool heaviest = false;             // on the heaviest path
};
struct Trace {
    std::uint32_t focus = 0;
    std::array<std::vector<std::uint32_t>, kTraceColumns> columns;   // modules, most linked first, the heaviest path's first of all
    std::array<std::uint32_t, kTraceColumns> hidden{};               // beyond `per_column`
    std::vector<TraceLink> links;
    std::vector<std::uint32_t> heaviest;                             // the chain through the focus, callers first
};
Trace compute_trace(const Graph& graph, std::uint32_t focus, std::size_t per_column);

// ---- the Library view: what a library imports on the left, each function it exports on the right, and who uses each

struct LibraryUser {
    std::uint32_t module = 0;
    std::uint64_t calls = 0;           // static call sites; identical copies of a file count once
};
struct LibraryChannel {
    std::string label;                 // the function, "#12" when it has no name; or a module's name
    std::string key;                   // lower-case name or "#ordinal": what identifies the function
    std::int64_t module = -1;          // >= 0: this channel is a module, and a click goes to it. -1: a function of the focus
    std::uint64_t weight = 0;          // by the Width chosen
    std::uint64_t calls = 0;
    std::vector<LibraryUser> users;    // a function's, heaviest first
};
struct LibraryView {
    std::uint32_t focus = 0;
    std::string function;              // not empty: centred on this function of the focus module
    std::vector<LibraryChannel> left, right;   // heaviest first
    std::uint64_t left_total = 0, right_total = 0;
    bool function_is_empty() const { return function.empty(); }
};
// A module: its imports left, its exports right. With `function`: the files that use it left, the module that exports it right.
LibraryView compute_library(const Graph& graph, const Index& index, std::uint32_t focus, Width width, std::string_view function = {});

}  // namespace cb::app::board
