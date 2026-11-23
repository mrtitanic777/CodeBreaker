// app/window_board.hpp -- where everything of the Board page is: a header, a row of foundation piles (what has really been converted), and under each pile a
// column of cards. A column is a fanned stack with the rest of its cards in a deck, or, expanded, a scrolling list. Only the cards in view are ever laid
// out: every position is arithmetic, so a column of ten thousand cards costs what a column of ten does. No operating-system API (window_board_test.cpp).
#pragma once

#include <array>
#include <vector>

#include "app/window_layout.hpp"
#include "app/window_pages.hpp"

namespace cb::app::window {

inline constexpr float kBoardColumnMin = 136;
inline constexpr float kBoardColumnGap = 8;
inline constexpr float kBoardColumnWide = 300;   // the most a column grows to when a C filter leaves only a few
inline constexpr float kBoardFoundationHeight = 58;
inline constexpr float kBoardHeadHeight = 26;
inline constexpr float kBoardFanPitch = 24;
inline constexpr float kBoardCardHeight = 64;
inline constexpr float kBoardRowPitch = 46;
inline constexpr float kBoardRowHeight = 42;
inline constexpr float kBoardDeckHeight = 20;
inline constexpr float kBoardDetailHeight = 52;

struct BoardLayout {
    Rect card, title, summary, search, open, cancel, bar, bar_text;
    Rect tableau;                      // the columns: foundations, heads and cards (clipped to it)
    Rect detail, detail_text, convert, open_card;   // the selected card's strip (empty when none is selected)
    Rect empty;                        // the drop area while there is no board
    std::array<Rect, 4> tabs{};        // Board, Flow, Trace, Library (empty while there is no board)
    Rect crumbs;                       // Flow, Trace and Library: the way here
    Rect hot_path;                     // beside it: the projected hot path (empty in a narrow window)
    Rect orphans_button, decks_button, export_button, batch_button;   // automation (the maps have Export only)
    Rect inspect, drawer, drawer_close;   // Inspect on the selected card's strip; the X-Ray drawer and its close button
    std::array<Rect, board::kSignalTypes> signal_pills{};   // the cards: one per kind of signal
    Rect group_copies, filter_row;     // the cards: Group copies, and the active filters' row (empty when there are none)
    Rect c_summary;                    // the cards: the whole OS's measured C result (empty with no score table)
    std::array<Rect, board::kCViews> c_pills{};   // the cards: the filters by C result
    Rect map;                          // Map, on the selected card's strip
    std::array<Rect, 3> chips{};       // Flow and Trace: Imports, Installs, References
    std::array<Rect, 3> widths{};      // Flow and Trace: channels as wide as their Files, Links or Calls
    Rect note, canvas;                 // Flow and Trace: what the picture is, and the picture
    float column_width = kBoardColumnMin;
    float content_width = 0;           // all the columns
};
BoardLayout compute_board_layout(const Layout& shell, const Model& model);
float board_max_hscroll(const BoardLayout& layout);

// One column as it is now.
struct BoardColumn {
    Rect foundation, head, area;       // moved by the page's sideways scroll
    Rect deck;                         // the cards not fanned out; empty when all are, or when expanded
    bool expanded = false;
    bool done_view = false;            // showing the cards on the foundation instead of those to do
    const std::vector<std::uint32_t>* cards = nullptr;   // what the column lists
    int count = 0;
    int first = 0, last = 0;           // the positions in view: [first, last)
    int hidden = 0;                    // in the deck
    float pitch = kBoardFanPitch;      // from one card to the next
    float card_height = kBoardCardHeight;
    float top = 0;                     // where position 0 is
    float content_height = 0;          // expanded: every card
};
BoardColumn compute_board_column(const BoardLayout& layout, const Model& model, std::size_t column);
// A C filter lays out only the columns holding what it found (all of them if it found nothing).
bool board_column_shown(const Model& model, std::size_t column);
std::size_t board_columns_shown(const Model& model);
// The rectangle of the card at a position of a column (the last fanned one is whole; those above it show their top strip).
Rect board_card_rect(const BoardColumn& column, int position);
float board_max_scroll(const BoardColumn& column);

// What is under a point: a card (index: Board::cards), a foundation or a head (index: the column), or a button.
PageHit hit_test_board(const BoardLayout& layout, const Model& model, float x, float y);
// The foundation under a point, -1 for none (where a dragged card is let go).
int board_foundation_at(const BoardLayout& layout, const Model& model, float x, float y);
std::vector<PageHit> focus_order_board(const Model& model);

// ---- automation

void board_toggle_orphans(Model& model);
// The built-in decks and the ones kept with this board.
void board_load_decks(Model& model);
// Apply a deck (again: back to everything). A deck replaces the search, the filters and Orphans.
void board_apply_deck(Model& model, int deck);
void board_save_deck(Model& model);       // the view as it is, named for what it shows
void board_remove_deck(Model& model, int deck);
// The cluster in view (the map's columns, or the cards shown) as JSON, GraphML and a Maltego table; the folder written.
std::filesystem::path board_export(Model& model);

inline constexpr float kDeckRowHeight = 28;
struct DeckPanel {
    Rect outline, save;
    std::vector<Rect> rows, removes;   // one per deck; a built-in deck's remove is empty
};
DeckPanel layout_deck_panel(const BoardLayout& layout, const Model& model);

// ---- the X-Ray inspector and the hover trace

inline constexpr float kInspectorRowHeight = 24;
struct InspectorLayout {
    Rect list;
    int first = 0;
    std::vector<Rect> rows;            // the rows in view
    float max_scroll = 0;
};
InspectorLayout layout_inspector(const BoardLayout& layout, const Model& model);
// Open the drawer on a card, or close it (-1). It follows the selected card while it is open.
void board_inspect_card(Model& model, int card);
void board_sync_inspector(Model& model);
// One near miss of the drawer side by side (`row`: its row), and back to the drawer.
void board_open_compare(Model& model, int row);
void board_close_compare(Model& model);
struct BoardCompareLayout {
    Rect outline, title, close, source_head, assembly_head, source, assembly, footer;
};
BoardCompareLayout layout_board_compare(const BoardLayout& layout);
// The most a side scrolls. `side`: 0 the C, 1 the assembly.
float board_compare_max_scroll(const BoardCompareLayout& panel, const Model& model, int side);
// The card under the pointer lights what it is linked to; -1: nothing is lit.
void board_hover_card(Model& model, int card);

// ---- master cards and signal filters

// The columns again, for the search words, the active filters and Group copies.
void board_rebuild_stacks(Model& model);
// Open a kind of signal's list (again: close it). Its values, the ones most files share first.
void board_open_signals(Model& model, int type);
void board_toggle_filter(Model& model, std::uint32_t signal);
// Filter by C result (the same one again: off). Converted programs live on their foundations, so the columns list those until it is off, then go back as they were.
void board_toggle_c_view(Model& model, board::CView view);
// The score table again if it has changed (a scorer may still be adding to it). A table that cannot be read just now keeps the scores there are; nothing moves
// under a drag or a batch waiting for its second press. True when the page has new numbers.
bool board_refresh_scores(Model& model);

inline constexpr float kSignalRowHeight = 26;
struct SignalPanel {
    Rect outline, list;
    int first = 0;
    std::vector<Rect> rows;            // the values in view
    float max_scroll = 0;
};
SignalPanel layout_signal_panel(const BoardLayout& layout, const Model& model);
// The words of a filter chip: at most `room` characters of its value, after its kind.
std::string board_filter_words(const Model& model, std::uint32_t signal, std::size_t room);
inline constexpr float kFilterChipWidth = 210;

// ---- Flow and Trace

struct FlowRow {
    Rect bar, label, slice;            // the band's end at the side, its words, and its end on the centre bar
};
struct FlowLayout {
    Rect center, title;
    std::vector<FlowRow> in, out;
};
FlowLayout layout_flow(const Rect& area, const board::Flow& flow);

struct TraceLayout {
    std::array<Rect, board::kTraceColumns> headers{}, more{};
    std::array<std::vector<Rect>, board::kTraceColumns> nodes;   // as many of each column as fit
    std::array<std::uint32_t, board::kTraceColumns> more_count{};
};
inline constexpr float kTraceRowPitch = 20;
inline constexpr std::size_t kTraceKept = 64;                    // the most of one column a trace holds
TraceLayout layout_trace(const Rect& area, const board::Trace& trace);

// The graph again for the kinds of link chosen, then the Flow and Trace of the module in focus (the selected card's, else the one most files link to).
void board_refresh_maps(Model& model);
void board_focus_module(Model& model, int module);
void board_show_view(Model& model, int view);
// Centre on a module, or one of its functions, and add it to the breadcrumbs.
void board_go_to(Model& model, int module, const std::string& function = {});
// Back to breadcrumb `index` (what came after it is dropped); -1: the cards. One step back; from the first crumb, the cards.
void board_crumb_back(Model& model, int index);
void board_step_back(Model& model);

// ---- the breadcrumbs

// The words of each crumb: "Board", then a file or "file ! function".
std::vector<std::string> board_crumb_words(const Model& model);
// "user32.dll > kernel32.dll  peak 1.2M calls/h"
std::string board_hot_path_words(const Model& model);
struct CrumbLayout {
    std::vector<Rect> crumbs;          // one per word; empty when it does not fit (the ones nearest the start give way first, the first and the last stay)
    Rect more;                         // the "..." where some gave way
};
CrumbLayout layout_crumbs(const Rect& area, const std::vector<std::string>& words);

// ---- the Library view

inline constexpr float kLibraryRowPitch = 22;
struct LibraryLayout {
    Rect title, left_head, right_head, users_head, center;
    Rect left_area, right_area, users_area;
    int left_first = 0, right_first = 0;      // the first channel of each side in view
    std::vector<Rect> left_rows, right_rows;  // the channels in view
    std::vector<Rect> left_slices, right_slices;   // their ends on the centre bar
    std::vector<Rect> user_rows;              // the hovered export's files that fit
    std::uint64_t users_more = 0;
    float left_max_scroll = 0, right_max_scroll = 0;
};
LibraryLayout layout_library(const Rect& area, const board::LibraryView& view, float left_scroll, float right_scroll, int hover);
// How thick a channel of `weight` is in a row, beside the heaviest of its view: at least 1, never more than the row.
float library_thickness(std::uint64_t weight, std::uint64_t heaviest);

}  // namespace cb::app::window
