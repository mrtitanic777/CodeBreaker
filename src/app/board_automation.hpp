// app/board_automation.hpp -- working through a whole board: files nothing links to, saved views ("decks"), the order a batch converts in, and a cluster of the map
// written out for other link-analysis tools. No operating-system API (board_automation_test.cpp).
//
// An orphan is a file no setup script, program, library or driver on the board links to, and that does not start by itself. That is "no link was found": a file can
// still be reached by a path built at run time, or by a cabinet that could not be opened.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "app/board.hpp"
#include "app/board_flow.hpp"
#include "app/board_index.hpp"
#include "app/board_projection.hpp"
#include "app/board_signals.hpp"

namespace cb::app::board {

// ---- orphans

struct Orphans {
    std::vector<char> card;            // per card
    std::uint32_t distinct = 0;        // distinct files
};
Orphans find_orphans(const Board& board, const Index& index, const Signals* signals);

// ---- decks

struct DeckFilter {
    SignalType type = SignalType::RegKey;
    std::string value;
};
struct Deck {
    std::string name;
    std::string query;
    bool orphans = false;
    bool builtin = false;
    std::vector<std::string> names;    // a card's file name holds any of these (lower case); empty: any
    std::vector<DeckFilter> filters;
};
// DirectX and media, dial-up networking, and the Plug and Play boot chain.
std::vector<Deck> builtin_decks();
void save_decks(const std::filesystem::path& root, const std::vector<Deck>& decks);   // the user's; built-in decks are not kept
std::vector<Deck> load_decks(const std::filesystem::path& root);
// Per card: its file name holds one of `names`.
std::vector<char> cards_named(const Board& board, const std::vector<std::string>& names);

// ---- a batch

// The binaries among the cards shown that are worth converting (PE, NE, LE; not done, not known unconvertible, not on their way), most linked-to first.
std::vector<std::uint32_t> batch_order(const Board& board, const Index& index, const Stacks& stacks);

// ---- export

struct ClusterEdge {
    std::uint32_t from = 0, to = 0;    // positions in Cluster::modules
    Link link = Link::Imports;
    std::uint8_t confidence = 0;
    std::uint32_t files = 0;           // distinct source files
    std::uint64_t links = 0, calls = 0;
    double projected = 0;              // calls per hour, a model
};
struct Cluster {
    std::vector<std::uint32_t> modules;
    std::vector<ClusterEdge> edges;
};
// The links among these modules (Graph ids), every kind.
Cluster build_cluster(const Graph& graph, const Index& index, const Projection* projection, std::vector<std::uint32_t> modules);
std::string cluster_json(const Board& board, const Index& index, const Signals* signals, const Graph& graph, const Cluster& cluster);
std::string cluster_graphml(const Board& board, const Index& index, const Graph& graph, const Cluster& cluster);
// One row per link, with each end's entity type: Maltego's table import takes it.
std::string cluster_table(const Board& board, const Graph& graph, const Cluster& cluster);

}  // namespace cb::app::board
