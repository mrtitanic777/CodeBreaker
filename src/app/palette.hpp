// app/palette.hpp -- the command palette (Ctrl+K): what it lists for what has been typed. Three sections, in this order: Tools (every tool of tools::all()),
// Recent (the conversions kept in Recent) and Go to (the pages of the window and Settings: Advanced). Typing filters them and ranks what is left; the
// arrow keys move through the result; Enter runs the one chosen. This is only the model (no window, no operating-system API: tested on every system,
// palette_test.cpp); where the rows sit is window_palette.hpp, the events and the drawing are window_win.cpp and window_render_win.cpp.
//
//     auto items = palette::make_items(tools::all(), recent);              // once per opening
//     palette::Result r = palette::search(items, "dis");                   // rows to show: headings and items, best first inside a section
//     int at = palette::move(r, 0, +1);                                    // the arrow keys
//     const palette::Item& chosen = items[r.picks[at]];
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "app/tools.hpp"

namespace cb::app::palette {

enum class Kind { Tool, Recent, GoTo };
// What a Go to item opens.
enum class Place { Convert, Tools, Settings, SettingsAdvanced };

struct Item {
    Kind kind = Kind::Tool;
    std::string title;                 // "Disassemble a program"
    std::string note;                  // beside the title, grey: a tool's group ("Inspect"), a recent's "converted 2 minutes ago"
    std::string hint;                  // at the right edge when the item is not the chosen one: a shortcut ("Ctrl+3")
    std::string search;                // more words that find it (a tool's command name and description); the title is always searched
    int index = -1;                    // Tool: its place in tools::all(); Recent: its place in the Recent list; GoTo: a Place
    bool glyph_file = false;           // Recent: the row shows a page, not the code glyph
};

// One conversion of the Recent list, as the palette needs it.
struct RecentEntry {
    std::string name;                  // "lz32.dll"
    std::string note;                  // "converted 2 minutes ago"
    bool missing = false;              // its folder is gone: it is not listed
};

// Every item there could be: the tools, then the recent conversions whose folder is still there, then the places.
std::vector<Item> make_items(const std::vector<tools::Tool>& tools, const std::vector<RecentEntry>& recent);

// One row of the list shown: a section heading, or an item (`item` indexes the items given to search).
struct Row {
    bool heading = false;
    std::string text;                  // the heading
    int item = -1;
};
struct Result {
    std::vector<Row> rows;
    std::vector<int> picks;            // the item index of each choosable row, top to bottom
};

// The most each section shows.
inline constexpr std::size_t kMaxTools = 5, kMaxRecent = 3, kMaxPlaces = 4;

// How well `text` matches the words of `query`: 0 not at all (some word is missing), else higher is better (whole text starts with it > a word starts with
// it > inside a word). Case is ignored. An empty query matches everything with 1.
int score(std::string_view query, std::string_view text);

// The items that match `query` (every word of it, in the title or the other words), each section best first (ties keep the order of `items`), capped at
// kMax*; empty sections are not shown. An empty query lists the first of each section.
Result search(const std::vector<Item>& items, std::string_view query);

// The choosable position after `from` moved `delta` rows, staying at the ends (not wrapping); 0 when there is nothing to choose.
int move(const Result& result, int from, int delta);

}  // namespace cb::app::palette
